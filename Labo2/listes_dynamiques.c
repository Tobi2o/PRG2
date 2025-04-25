/*
 -----------------------------------------------------------------------------------
 Nom du fichier : listes_dynamiques.c
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 27.04.2023

 Description    : Ce fichier implémente une librairie pour des listes doublement
                  chaînées non circulaires.

 Remarque(s)    : Aucune vérification sur le pointeur 'liste' passé en paramètre
                  aux fonctions n'est effectuée

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
*/
#include "listes_dynamiques.h"
#include <stdlib.h>
#include <stdio.h>

const char AFFICHAGE_LISTE_DEBUT = '[';
const char AFFICHAGE_LISTE_FIN = ']';
const char AFFICHAGE_LISTE_ENTRE_ELEMENTS = ',';

/**
 * Macro pour creer des fonctions d'affichage selon le premier element et le
 * prochain element
 * @param SUFFIX suffix du nom de la fonction (afficher#SUFFIX)
 * @param PREMIER_ELEMENT comment atteindre le premier element de la liste (tete
 * ou queue)
 * @param PROCHAIN comment atteindre le prochain element de la liste selon le mode
 * (prochain ou suivant)
 */
#define CREER_FONCTION_AFFICHER(SUFFIX, PREMIER_ELEMENT, PROCHAIN)\
void afficher##SUFFIX (const Liste* liste)                        \
{                                                                 \
   printf("%c", AFFICHAGE_LISTE_DEBUT);                           \
   if(!estVide(liste))                                            \
   {                                                              \
      Element* elementActuel = liste->PREMIER_ELEMENT;            \
      printf("%d", elementActuel->info);                          \
      while((elementActuel = elementActuel->PROCHAIN))            \
      {                                                           \
         printf("%c", AFFICHAGE_LISTE_ENTRE_ELEMENTS);            \
         printf("%d", elementActuel->info);                       \
      }                                                           \
   }                                                              \
   printf("%c", AFFICHAGE_LISTE_FIN);                             \
}

CREER_FONCTION_AFFICHER(EnAvant, tete, suivant)

CREER_FONCTION_AFFICHER(EnArriere, queue, precedent)

/**
 *
 * @param element pointeur vers l'element a supprimer
 * @param info pointeur pour retourner l'information qui se trouve dans l'element
 * a supprimer
 */
static void supprimerElement(Element *element, Info *info);

/**
 *
 * @param liste liste sur laquelle on veut trouver l'element
 * @param position position de la liste de l'element souhaité
 * @param element paramètre d'entrée/sortie pour retourner l'element à position
 * position. Si NULL est passé en paramètre, ce pointeur n'est pas modifié.
 * @return OK si on a trouvé l'element. POSITION_NON_VALIDE si la position
 * position n'est pas valide.
 */
static Status getElement(const Liste *liste, size_t position, Element **element);

Liste *initialiser(void) {
   // on utilise calloc pour mettre directement les éléments à 0 = NULL
   return (Liste *) calloc(1, sizeof(Liste));
}

bool estVide(const Liste *liste) {
   return liste->tete == NULL;
}

size_t longueur(const Liste *liste) {
   Element *elementActuel = liste->tete;
   size_t taille = 0;

   // tant qu'on un element dans la liste, on incrémente la taille
   while (elementActuel) {
      elementActuel = elementActuel->suivant;
      taille++;
   }
   return taille;
}

void afficher(const Liste *liste, Mode mode) {

   switch (mode) {
      case FORWARD:
         afficherEnAvant(liste);
         break;
      case BACKWARD:
         afficherEnArriere(liste);
         break;
      default:
         //on ne devrait jamais arriver ici.
         break;
   }
}

Status insererEnTete(Liste *liste, const Info *info) {
   // On utilise calloc pour avoir tous les elements de Element à 0
   // Pas besoin d'explicitement indiquer la valeur de precedent = NULL
   Element *element = (Element *) calloc(1, sizeof(Element));
   if (element) {

      if (info) {
         element->info = *info;
      }

      // Notre nouveau element doit pointer (avec suivant) sur la tete actuelle de
      // la liste
      element->suivant = liste->tete;

      if (liste->tete) {
         // La tete actuelle a maintenant un precedent (ce nouveau element)
         liste->tete->precedent = element;
      }
      // Ce nouveau element devient la nouvelle tete de la liste
      liste->tete = element;

      //si on a pas encore une queue, il devient aussi la nouvelle queue de la liste
      if (!liste->queue) {
         liste->queue = element;
      }
      return OK;
   }
   return MEMOIRE_INSUFFISANTE;
}

Status insererEnQueue(Liste *liste, const Info *info) {
   // On utilise calloc pour avoir tous les elements de Element à 0
   // Pas besoin d'explicitement indiquer la valeur de suivant = NULL
   Element *element = (Element *) calloc(1, sizeof(Element));

   if (element) {
      if (info) {
         element->info = *info;
      }

      // Notre nouvel element doit pointer (avec precedent) sur la queue actuelle
      // de la liste
      element->precedent = liste->queue;

      if (liste->queue) {
         // La queue actuelle a maintenant un suivant (ce nouveau element)
         liste->queue->suivant = element;
      }
      // Ce nouveau element devient la nouvelle queue de la liste
      liste->queue = element;

      //si on a pas encore une tete, il devient aussi la nouvelle tete de la liste
      if (!liste->tete) {
         liste->tete = element;
      }
      return OK;
   }
   return MEMOIRE_INSUFFISANTE;
}

void supprimerElement(Element *element, Info *info) {

   if (!element) {
      return;
   }

   //stocker l'information de l'élément actuel dans notre paramètre d'entrée/sortie
   if (info) {
      *info = element->info;
   }

   //Mettre à jour les pointeurs des elements voisins. En vérifiant d'abord s'ils
   // existent
   if (element->precedent) {
      element->precedent->suivant = element->suivant;
   }
   if (element->suivant) {
      element->suivant->precedent = element->precedent;
   }

   // restituer mémoire
   free(element);
}

Status supprimerEnTete(Liste *liste, Info *info) {
   // verifier si la liste est vide avant de faire quoi que ce soit
   if (estVide(liste)) {
      return LISTE_VIDE;
   }

   // garder un pointeur sur la tete actuelle pour pouvoir restituer la mémoire
   Element *tete = liste->tete;

   // nouvelle tete est le deuxieme element (s'il existe sinon NULL)
   liste->tete = tete->suivant;

   //si la liste devient vide, il faut aussi mettre à jour la queue
   if (!liste->tete) {
      liste->queue = NULL;
   }

   // supprimer l'ancienne tete
   supprimerElement(tete, info);

   return OK;
}

Status supprimerEnQueue(Liste *liste, Info *info) {
   // verifier si la liste est vide avant de faire quoi que ce soit
   if (estVide(liste)) {
      return LISTE_VIDE;
   }
   // garder un pointeur sur la queue actuelle pour pouvoir restituer la mémoire
   Element *queue = liste->queue;

   // nouvelle queue est l'avant dernier element (s'il existe sinon NULL)
   liste->queue = queue->precedent;

   //si la liste devient vide, il faut aussi mettre à jour la tete
   if (!liste->queue) {
      liste->tete = NULL;
   }
   // supprimer l'ancienne queue
   supprimerElement(queue, info);

   return OK;
}

void supprimerSelonCritere(Liste *liste,
                           bool (*critere)(size_t position, const Info *info)) {
   // verifier les parametres d'entree
   if (!critere || estVide(liste)) {
      return;
   }
   size_t position = 0;
   Element *elementActuel = liste->tete;

   //iterer sur la liste et verifier le critere pour chacun des elements
   while (elementActuel) {
      if (critere(position, &elementActuel->info)) {
         //critere ok, on supprime l'element
         // verifier si c'est la tete ou la queue
         if (elementActuel == liste->tete) {
            supprimerEnTete(liste, NULL);
            elementActuel = liste->tete;
         } else if (elementActuel == liste->queue) {
            supprimerEnQueue(liste, NULL);
            elementActuel = NULL;
         } else {
            // pas besoin de verifier si suivant existe car on serait en train de
            // supprimer la queue si c'etait le cas
            elementActuel = elementActuel->suivant;
            supprimerElement(elementActuel->precedent, NULL);
         }
      } else {
         // on passe au prochain element
         elementActuel = elementActuel->suivant;
      }
      position++;
   }
}

void vider(Liste *liste, size_t position) {
   Element *elementActuel;
   Status status = getElement(liste, position, &elementActuel);
   if (status == POSITION_NON_VALIDE) {
      return;
   }

   //Pour vider la liste, on fait la queue pointer sur l'element precedent
   // a l'element a la position position et on libere ensuite la memoire elements.
   liste->queue = elementActuel->precedent;


   if (elementActuel == liste->tete) {
      liste->tete = NULL;
   }

   // verifier si on a toujours une queue
   if (liste->queue) {
      // notre queue n'a pas d'element suivant
      liste->queue->suivant = NULL;
   }

   // Liste a jour, il suffit de restituer la memoire
   while (elementActuel) {
      Element *tmp = elementActuel;
      elementActuel = elementActuel->suivant;
      free(tmp);
   }
}

static Status getElement(const Liste *liste, size_t position, Element **element) {
   if (estVide(liste)) {
      return POSITION_NON_VALIDE;
   }

   Element *elementActuel = liste->tete;

   // on itere sur notre liste tant que on a un element valable
   // en decrementant la position
   while (elementActuel && position--) {
      elementActuel = elementActuel->suivant;
   }

   if (!elementActuel) {
      return POSITION_NON_VALIDE;
   }

   if (element) {
      *element = elementActuel;
   }
   return OK;
}

bool sontEgales(const Liste *liste1, const Liste *liste2) {
   Element *elementActuelL1 = liste1->tete;
   Element *elementActuelL2 = liste2->tete;

   // itérer tant qu'on a des pointeurs valables
   while (elementActuelL1 && elementActuelL2) {
      if (elementActuelL1->info != elementActuelL2->info) {
         return false;
      }
      elementActuelL1 = elementActuelL1->suivant;
      elementActuelL2 = elementActuelL2->suivant;
   }
   // si on est arrivé à la fin des deux listes, p1 et p2 sont nulles et les listes
   // sont égales
   return elementActuelL1 == NULL && elementActuelL2 == NULL;
}
