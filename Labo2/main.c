/*
 -----------------------------------------------------------------------------------
 Nom du fichier : main.c
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 27.04.2023

 Description    : Ce fichier teste l'implementation de la librairie pour
                  des listes doublement chaînées non circulaires.

 Remarque(s)    : L'utilisation d'assertions est extensive dans ces tests donc ce
                  programme doit être compilé en mode DEBUG.

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
*/
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "listes_dynamiques.h"

/**
 * Fonction qui affiche le test qui a été passé
 * @param test nom du test
 */
void afficherTestOk(const char *test);

/**
 * Fonction qui teste la fonction initialiser de notre liste dynamique
 */
void testInitialiser(void);

/**
 * Fonction qui teste les fonctions d'insertion et suppression en tête de notre
 * liste dynamique
 */
void testInsererEtSupprimerEnTete(void);

/**
 * Fonction qui teste la fonction estVide de notre liste dynamique
 */
void testEstVide(void);

/**
 * Fonction qui teste la fonction longueuer de notre liste dynamique
 */
void testLongueur(void);

/**
 * Fonction qui teste la fonction afficher de notre liste dynamique
 */
void testAfficher(void);

/**
 * Fonction qui teste les fonctions d'insertion et suppression en queue de notre
 * liste dynamique
 */
void testInsererEtSupprimerEnQueue(void);

/**
 * Fonction qui teste la fonction vider de notre liste dynamique
 */
void testVider(void);

/**
 * Fonction qui teste la fonction supprimerSelonCritere de notre liste dynamique
 */
void testSupprimerSelonCritere(void);

/**
 * Fonction qui teste la fonction sontEgales de notre liste dynamique
 */
void testSontEgales(void);

/**
 * Fonction pour tester la suppression selon critere de notre liste dynamique
 * @param pos position dans la liste
 * @param val valeur de l'element dans la position pos de la liste
 * @return true si la valeur n’est pas comprise entre 3 et 7, false sinon
 */
bool estEntre3et7(size_t pos, const Info *val);

/**
 * Fonction pour tester la suppression selon critere de notre liste dynamique
 * @param pos position dans la liste
 * @param val valeur de l'element dans la position pos de la liste
 * @return true si la position est paire, false sinon
 */
bool positionEstPaire(size_t pos, const Info *val);

/**
 * Fonction pour tester la suppression selon critere de notre liste dynamique
 *
 * @param pos position dans la liste
 * @param val valeur de l'element dans la position pos de la liste
 * @return true pour supprimer tous les éléments.
 */
bool suppressionComplete(size_t pos, const Info *val);

int main(void) {
   testInitialiser();
   testInsererEtSupprimerEnTete();
   testEstVide();
   testLongueur();
   testAfficher();
   testInsererEtSupprimerEnQueue();
   testVider();
   testSupprimerSelonCritere();
   testSontEgales();
}

void afficherTestOk(const char *test) {
   printf("%s: %s\n", test, "OK");
}

void testInitialiser(void) {
   Liste *liste = initialiser();
   assert(liste != NULL);
   assert(liste->tete == NULL);
   assert(liste->queue == NULL);
   free(liste);
   afficherTestOk("Test initialiser()");
}

void testInsererEtSupprimerEnTete(void) {
   Liste *liste = initialiser();
   assert(liste != NULL);
   // Test 1 : liste vide, insertion en tête
   Info info1 = 42;
   assert(insererEnTete(liste, &info1) == OK);
   assert(liste->tete != NULL);
   assert(liste->tete->info == info1);
   assert(liste->queue == liste->tete);
   afficherTestOk("Test insererEnTete(), liste vide");

   Info infoObtenue1;
   assert(supprimerEnTete(liste, &infoObtenue1) == OK);
   assert(infoObtenue1 == info1);
   assert(liste->tete == NULL);
   assert(liste->queue == liste->tete);
   afficherTestOk("Test supprimerEnTete(), 1 element");

   // Test 2 : liste non vide, insertion en tête
   Info info2 = 84;
   assert(insererEnTete(liste, &info1) == OK);
   assert(insererEnTete(liste, &info2) == OK);
   // pas besoin de verifier liste->tete, ça a deja ete teste
   assert(liste->tete->suivant != NULL);
   assert(liste->tete->suivant->info == info1);
   assert(liste->queue == liste->tete->suivant);
   assert(liste->queue->precedent != NULL);
   assert(liste->queue->precedent == liste->tete);
   afficherTestOk("Test insererEnTete(), liste non vide");

   Info infoObtenue2;
   assert(supprimerEnTete(liste, &infoObtenue2) == OK);
   assert(infoObtenue2 == info2);
   assert(liste->tete != NULL);
   assert(liste->tete->info == info1);
   assert(liste->tete->suivant == NULL);
   assert(liste->queue == liste->tete);

   assert(supprimerEnTete(liste, &infoObtenue1) == OK);
   assert(infoObtenue1 == info1);
   assert(liste->tete == NULL);
   assert(liste->queue == liste->tete);
   afficherTestOk("Test supprimerEnTete(), 2 elements");

   //test 3. Insertion avec ptr null
   assert(insererEnTete(liste, NULL) == OK);
   assert(liste->tete->info == 0);
   afficherTestOk("Test insererEnTete(), pointeur NULL");
   assert(supprimerEnTete(liste, NULL) == OK);

   //test 4 suppression liste vide
   assert(supprimerEnTete(liste, NULL) == LISTE_VIDE);
   afficherTestOk("Test supprimerEnTete(), liste vide");
   free(liste);
}

void testEstVide(void) {

   Liste *liste = initialiser();
   assert(liste != NULL);
   assert(estVide(liste));

   Info info = 42;
   assert(insererEnTete(liste, &info) == OK);
   assert(!estVide(liste));

   supprimerEnTete(liste, NULL);
   assert(estVide(liste));

   afficherTestOk("Test estVide()");
   free(liste);
}

void testLongueur(void) {
   Liste *liste = initialiser();
   assert(liste != NULL);
   assert(longueur(liste) == 0);
   Info info1 = 1, info2 = 2, info3 = 3;
   assert(insererEnTete(liste, &info1) == OK);
   assert(insererEnTete(liste, &info2) == OK);
   assert(insererEnTete(liste, &info3) == OK);
   assert(longueur(liste) == 3);
   supprimerEnTete(liste, &info3);
   assert(longueur(liste) == 2);
   supprimerEnTete(liste, &info3);
   assert(longueur(liste) == 1);
   supprimerEnTete(liste, &info3);
   assert(longueur(liste) == 0);
   free(liste);
   afficherTestOk("Test longueur()");
}

void testAfficher(void) {
   Liste *liste = initialiser();
   assert(liste != NULL);
   printf("Liste vide : ");
   afficher(liste, FORWARD);
   printf("\n");
   printf("Liste vide : ");
   afficher(liste, BACKWARD);
   printf("\n");
   Info info1 = 1, info2 = 2, info3 = 3;
   assert(insererEnTete(liste, &info1) == OK);
   assert(insererEnTete(liste, &info2) == OK);
   assert(insererEnTete(liste, &info3) == OK);
   printf("Liste Forward: ");
   afficher(liste, FORWARD);
   printf("\n");
   printf("Liste Backward: ");
   afficher(liste, BACKWARD);
   printf("\n");
   afficherTestOk("Test afficher()");

   while (!estVide(liste)) {
      supprimerEnTete(liste, NULL);
   }
   free(liste);

}

void testInsererEtSupprimerEnQueue(void) {
   Liste *liste = initialiser();
   assert(liste != NULL);
   // Test 1 : liste vide, insertion en queue
   Info info1 = 42;
   assert(insererEnQueue(liste, &info1) == OK);
   assert(liste->tete != NULL);
   assert(liste->tete->info == info1);
   assert(liste->queue == liste->tete);
   afficherTestOk("Test insererEnQueue(), liste vide");

   Info infoObtenue1;
   assert(supprimerEnQueue(liste, &infoObtenue1) == OK);
   assert(infoObtenue1 == info1);
   assert(liste->tete == NULL);
   assert(liste->queue == liste->tete);
   afficherTestOk("Test supprimerEnQueue(), 1 element");

   // Test 2 : liste non vide, insertion en queue
   Info info2 = 84;
   assert(insererEnQueue(liste, &info1) == OK);
   assert(insererEnQueue(liste, &info2) == OK);
   assert(liste->tete->suivant != NULL);
   assert(liste->tete->info == info1);
   assert(liste->tete->suivant->info == info2);
   assert(liste->queue == liste->tete->suivant);
   assert(liste->queue->precedent != NULL);
   assert(liste->queue->precedent == liste->tete);
   afficherTestOk("Test insererEnQueue(), liste non vide");

   Info infoObtenue2;
   assert(supprimerEnQueue(liste, &infoObtenue2) == OK);
   assert(infoObtenue2 == info2);
   assert(liste->tete != NULL);
   assert(liste->tete->info == info1);
   assert(liste->tete->suivant == NULL);
   assert(liste->queue == liste->tete);

   assert(supprimerEnQueue(liste, &infoObtenue1) == OK);
   assert(infoObtenue1 == info1);
   assert(liste->tete == NULL);
   assert(liste->queue == liste->tete);
   afficherTestOk("Test supprimerEnQueue(), 2 elements");

   //test 3. Insertion avec ptr null
   assert(insererEnQueue(liste, NULL) == OK);
   assert(liste->tete->info == 0);
   afficherTestOk("Test insererEnQueue(), pointeur NULL");
   assert(supprimerEnQueue(liste, NULL) == OK);
   assert(liste->tete == NULL);

   //test 4. suppression liste vide
   assert(supprimerEnQueue(liste, NULL) == LISTE_VIDE);
   afficherTestOk("Test supprimerEnQueue(), liste vide");
   free(liste);
}

void testVider(void) {
   const size_t LONGUEUR = 20;
   Liste *liste = initialiser();
   assert(liste != NULL);
   for (size_t i = 0; i < LONGUEUR; i++) {
      Info aux = (Info) i;
      insererEnQueue(liste, &aux);
   }
   // Test 1 vider moitié
   vider(liste, LONGUEUR / 2lu);
   assert(longueur(liste) == LONGUEUR / 2lu);
   assert(liste->queue->info == 9);
   assert(liste->tete->info == 0);
   afficherTestOk("Test vider(), moitie");

   // Test 2 vider a partir de position non valable
   vider(liste, LONGUEUR / 2);
   assert(longueur(liste) == 10);
   assert(liste->queue->info == 9);
   assert(liste->tete->info == 0);
   afficherTestOk("Test vider(), position non valable");

   // Test 3 vider tout
   vider(liste, 0);
   assert(estVide(liste));
   assert(longueur(liste) == 0);
   assert(liste->tete == NULL);
   assert(liste->queue == NULL);
   afficherTestOk("Test vider(), tout");

   free(liste);
}

void testSupprimerSelonCritere(void) {
   Liste *liste = initialiser();
   assert(liste != NULL);
   for (int i = 0; i < 10; i++) {
      insererEnQueue(liste, &i);
   }
   supprimerSelonCritere(liste, estEntre3et7);
   assert(liste->tete->info == 3);
   assert(liste->queue->info == 7);
   assert(longueur(liste) == 5);
   afficherTestOk("Test supprimer selon critere (info)");
   vider(liste, 0);

   for (int i = 0; i < 10; i++) {
      insererEnQueue(liste, &i);
   }
   supprimerSelonCritere(liste, positionEstPaire);
   assert(liste->tete->info == 1);
   assert(liste->queue->info == 9);
   assert(longueur(liste) == 5);
   afficherTestOk("Test supprimer selon critere (position)");

   vider(liste, 0);
   for (int i = 0; i < 10; i++) {
      insererEnQueue(liste, &i);
   }
   supprimerSelonCritere(liste, suppressionComplete);
   assert(liste->tete == NULL);
   assert(liste->queue == NULL);
   assert(estVide(liste));
   assert(longueur(liste) == 0);
   afficherTestOk("Test supprimer selon critere (tout)");

   free(liste);
}

void testSontEgales(void) {
   Liste *l1 = initialiser();
   Liste *l2 = initialiser();
   assert(l1 != NULL);
   assert(l2 != NULL);
   assert(sontEgales(l1, l2));
   afficherTestOk("Test listes vides egales");

   Info info1 = 1, info2 = 2, info3 = 3;
   assert(insererEnQueue(l1, &info1) == OK);
   assert(insererEnQueue(l1, &info2) == OK);
   assert(insererEnQueue(l1, &info3) == OK);
   assert(insererEnQueue(l2, &info1) == OK);
   assert(insererEnQueue(l2, &info2) == OK);
   assert(insererEnQueue(l2, &info3) == OK);

   assert(sontEgales(l1, l2));
   afficherTestOk("Test listes egales, 3 elements chaque");

   assert(insererEnQueue(l1, &info1) == OK);
   assert(insererEnQueue(l1, &info2) == OK);
   assert(insererEnQueue(l1, &info3) == OK);
   assert(!sontEgales(l1, l2));
   afficherTestOk("Test listes differentes, 3 premiers elements "
                  "correspondent");

   vider(l1, 0);
   vider(l2, 0);
   assert(insererEnQueue(l1, &info1) == OK);
   assert(insererEnQueue(l1, &info2) == OK);
   assert(insererEnQueue(l1, &info3) == OK);
   assert(insererEnQueue(l2, &info3) == OK);
   assert(insererEnQueue(l2, &info2) == OK);
   assert(insererEnQueue(l2, &info1) == OK);
   assert(!sontEgales(l1, l2));
   afficherTestOk("Test listes differentes, meme nombre d'elements");

   vider(l1, 0);
   vider(l2, 0);
   free(l1);
   free(l2);
}

bool estEntre3et7(size_t pos, const Info *val) {
   (void) pos; //eviter le warning de parametre non utilise
   return *val < 3 || *val > 7;
}

bool positionEstPaire(size_t pos, const Info *val) {
   (void) val; //eviter le warning de parametre non utilise
   return pos % 2 == 0;
}

bool suppressionComplete(size_t pos, const Info *val) {
   (void) pos; //eviter le warning de parametre non utilise
   (void) val;
   return true;
}
