/*
 -----------------------------------------------------------------------------------
 Nom du fichier : liste_taxes.c
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 25.05.2023

 Description    : Implementation d'une librairie permettant de modéliser des
                  listes de double à taille variable avec des calculs de statistiques

 Remarque(s)    : -

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
*/

#include "liste_taxes_internal.h"
#include "liste_taxes.h"
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/**
 * Double la capacite de stockage de notre liste
 * @param liste
 * @return true si la capacite a pu être incrementé, false sinon
 */
static bool liste_taxes_incrementer_capacite(ListeTaxes *liste);

/**
 * Implémentation de l'algorithme de selection rapide
 * @param buffer
 * @param i_bas
 * @param i_haut
 * @param indice_cible
 * @remark O(N)
 */
static void liste_taxes_selection_rapide(double *buffer, size_t i_bas, size_t i_haut,
                                         size_t indice_cible);

/**
 * Implémentation de l'algorithme de partition utilisé par l'algorithme de
 * selection rapide
 * @param buffer
 * @param i_bas
 * @param i_haut
 * @return
 * @remark O(N)
 */
static size_t liste_taxes_partition(double *buffer, size_t i_bas, size_t i_haut);

/**
 * Met à jour les statistiques de la liste lorsqu'un nouvel élément est ajouté
 * @param liste
 * @remark Aucune itération est effectué, complexité en O(1)
 */
static void liste_taxes_mettre_a_jour_statistiques(ListeTaxes *liste);

/**
 * Fonction utilitaire qui swap les elements pointés par a et b
 * @param a
 * @param b
 */
static void liste_taxes_swap_elements(double *a, double *b);

bool liste_taxes_incrementer_capacite(ListeTaxes *liste) {
   size_t nouvelle_capacite = liste->capacite > 0 ? liste->capacite * 2 : 1;
   double *tmp = (double *) realloc(liste->buffer, nouvelle_capacite *
                                                   sizeof(double));

   if (!tmp) {
      return false;
   }
   liste->capacite = nouvelle_capacite;
   liste->buffer = tmp;
   return true;
}

ListeTaxes *liste_taxes_creer() {
   return calloc(1, sizeof(ListeTaxes));
}

void liste_taxes_mettre_a_jour_statistiques(ListeTaxes *liste) {

   //on vient d'ajouter le premier element, initialisation des valeurs
   if (liste->taille == 1) {

      liste->statistiques.moyenne = liste->buffer[0];
      liste->statistiques.somme = liste->buffer[0];
      liste->statistiques.somme_carre = 0.;
      return;
   }

   //Calcul de la moyenne, somme et somme_carre en cours de route
   double x = liste->buffer[liste->taille - 1];

   //on doit garder l'ancienne moyenne pour la methode de Welford
   double ancienne_moyenne = liste->statistiques.moyenne;

   liste->statistiques.somme += x;

   liste->statistiques.moyenne = ancienne_moyenne +
                                 (x - ancienne_moyenne) /
                                 (double) liste->taille;

   //Sert à utiliser la méthode de Welford pour calculer la variance et écart-type
   liste->statistiques.somme_carre   = liste->statistiques.somme_carre +
                                     (x - ancienne_moyenne) *
                                     (x - liste->statistiques.moyenne);
}

bool liste_taxes_ajouter(ListeTaxes *liste, double element) {

   if (!liste)
      return false;
   if (liste->taille == liste->capacite) {
      if (!liste_taxes_incrementer_capacite(liste))
         return false;
   }

   liste->buffer[liste->taille] = element;
   liste->taille++;
   //Calcul des statistiques en cours de route (lorsqu'un élément est ajouté)
   liste_taxes_mettre_a_jour_statistiques(liste);
   return true;
}

size_t liste_taxes_taille(const ListeTaxes *liste) {
   if (!liste)
      return 0;
   return liste->taille;
}

void liste_taxes_detruire(ListeTaxes *liste) {
   free(liste);
}

void liste_taxes_swap_elements(double *a, double *b) {
   double tmp = *a;
   *a = *b;
   *b = tmp;
}

double liste_taxes_moyenne(const ListeTaxes *liste) {
   if (!liste || !liste->taille)
      return 0.;
   return liste->statistiques.moyenne;
}

double liste_taxes_somme(const ListeTaxes *liste) {
   if (!liste || !liste->taille)
      return 0.;
   return liste->statistiques.somme;
}

double liste_taxes_variance(const ListeTaxes *liste) {
   if (!liste || !liste->taille)
      return 0.;


   return liste->statistiques.somme_carre / (double) (liste->taille);
}

double liste_taxes_ecart_type(const ListeTaxes *liste) {
   return sqrt(liste_taxes_variance(liste));
}

double liste_taxes_mediane(ListeTaxes *liste) {
   if (!liste || !liste->taille)
      return 0.;

   //pour calculer la mediane, on effectue une selection rapide pour trouver
   // l'element qui devrait se trouver à la position N/2 si le tablau était trié

   liste_taxes_selection_rapide(liste->buffer, 0, liste->taille, liste->taille / 2);
   double mediane = liste->buffer[liste->taille / 2];

   //refaire la même chose si le nb d'elements est pair
   if (liste->taille % 2 == 0) {
      liste_taxes_selection_rapide(liste->buffer, 0, liste->taille,
                                   liste->taille / 2 - 1);

      mediane = (mediane + liste->buffer[liste->taille / 2 - 1]) / 2;
   }
   return mediane;
}

size_t liste_taxes_partition(double *buffer,
                             size_t i_bas,
                             size_t i_haut) {

   size_t pivot = i_haut - 1;
   size_t i = i_bas - 1;
   size_t j = pivot;

   while (1) {
      do
         i++;
      while (buffer[i] < buffer[pivot]);
      do
         j > 0 ? j-- : 0;
      while (j > i_bas && buffer[pivot] < buffer[j]);

      if (i >= j)
         break;

      liste_taxes_swap_elements(buffer + i, buffer + j);

   }
   liste_taxes_swap_elements(buffer + i, buffer + pivot);

   return i;
}

void liste_taxes_selection_rapide(double *buffer, size_t i_bas, size_t i_haut,
                                  size_t indice_cible) {
   if (i_bas < i_haut) {
      //choix aleatoire du pivot
      size_t indice_pivot = i_bas + (rand() % (i_haut - i_bas));
      liste_taxes_swap_elements(buffer + indice_pivot, buffer + i_haut - 1);
      indice_pivot = liste_taxes_partition(buffer, i_bas, i_haut);

      if (indice_cible == indice_pivot) {
         return;
      }
      if (indice_cible < indice_pivot) {
         liste_taxes_selection_rapide(buffer, i_bas, indice_pivot, indice_cible);
      } else {
         liste_taxes_selection_rapide(buffer, indice_pivot + 1, i_haut, indice_cible);
      }
   }
}

