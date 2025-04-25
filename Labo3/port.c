/*
 -----------------------------------------------------------------------------------
 Nom du fichier : port.c
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 28.05.2023

 Description    : Implémentation d'une librairie permettant de modéliser un
                  port avec des bateaux de plusieurs types

 Remarque(s)    : -

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
 */

#include "port.h"
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#include "liste_taxes.h"

typedef struct {
   double somme, moyenne, mediane, ecart_type;
   size_t nb_elements;
} StatistiquesCategorie;


typedef struct {
   const char *nom_categorie;
   ListeTaxes *taxes_liste;
   StatistiquesCategorie statistiques;
   int32_t id_categorie;
   bool is_liste_incomplete;
} Categorie;

#define AFFICHAGE_FORMAT_FLOAT "%7.2f"
#define AFFICHAGE_FORMAT_NOM_CATEGORIE "Categorie: %-9s"
#define AFFICHAGE_FORMAT_SOMME_CATEGORIE "Somme: " AFFICHAGE_FORMAT_FLOAT
#define AFFICHAGE_FORMAT_MOYENNE_CATEGORIE "Moyenne: " AFFICHAGE_FORMAT_FLOAT
#define AFFICHAGE_FORMAT_NB_BATEAUX_CATEGORIE "#: %" PRIuMAX
#define AFFICHAGE_FORMAT_MEDIANE_CATEGORIE "Mediane: " AFFICHAGE_FORMAT_FLOAT
#define AFFICHAGE_FORMAT_ECART_TYPE_CATEGORIE "Ecart-type: " AFFICHAGE_FORMAT_FLOAT
#define AFFICHAGE_FORMAT_SEPARATEUR_LIGNE " "
#define AFFICHAGE_FORMAT_SEPARATEUR_ENTRE_LIGNES "\n"
#define AFFICHAGE_FORMAT_MESSAGE_ERREUR_STATISTIQUES \
                     "Impossible d'afficher les statistiques pour la categorie %9s\n"


/**
 * Calcule la taxe due par les deux bateaux b1 et b2 et les compare
 * Utilisé avec la fonction qsort de la stdlib
 */
static int port_compare_bateau_taxe_decroissante(const void *b1, const void *b2);

/**
 * Affiche les statistiques d'une catégorie de bateaux
 * @param categorie
 */
static void port_afficher_statistiques_categorie(const Categorie *categorie);

/**
 * Calcule les statistiques à partir d'une liste de taxes
 */
static StatistiquesCategorie port_calculer_statistiques(ListeTaxes *liste);

int port_compare_bateau_taxe_decroissante(const void *b1, const void *b2) {

   return (int) (bateau_calculer_taxe_euro(b2) -
                 bateau_calculer_taxe_euro(b1));
}

void port_afficher_taxe_decroissante(const Port port) {
   Port port_trie;
   memcpy(port_trie, port, PORT_TAILLE * sizeof(port[0]));
   qsort(port_trie, PORT_TAILLE, sizeof(port[0]),
         port_compare_bateau_taxe_decroissante);
   port_afficher(port_trie);
}

void port_afficher(const Port port) {
   for (size_t i = 0; i < PORT_TAILLE; i++) {
      bateau_afficher(&port[i]);
      printf(AFFICHAGE_FORMAT_SEPARATEUR_ENTRE_LIGNES);
   }
}

static StatistiquesCategorie port_calculer_statistiques(ListeTaxes *liste) {
   size_t taille = liste_taxes_taille(liste);
   if (taille == 0) {
      return (StatistiquesCategorie) {0};
   }

   return (StatistiquesCategorie) {.somme=liste_taxes_somme(liste),
      .moyenne=liste_taxes_moyenne(liste),
      .ecart_type=liste_taxes_ecart_type(liste),
      .mediane= liste_taxes_mediane(liste),
      .nb_elements= taille};
}

void port_afficher_statistiques_categorie(const Categorie *categorie) {

   printf(AFFICHAGE_FORMAT_NOM_CATEGORIE
          AFFICHAGE_FORMAT_SEPARATEUR_LIGNE
          AFFICHAGE_FORMAT_NB_BATEAUX_CATEGORIE
          AFFICHAGE_FORMAT_SEPARATEUR_LIGNE
          AFFICHAGE_FORMAT_SOMME_CATEGORIE
          AFFICHAGE_FORMAT_SEPARATEUR_LIGNE
          AFFICHAGE_FORMAT_MOYENNE_CATEGORIE
          AFFICHAGE_FORMAT_SEPARATEUR_LIGNE
          AFFICHAGE_FORMAT_MEDIANE_CATEGORIE
          AFFICHAGE_FORMAT_SEPARATEUR_LIGNE
          AFFICHAGE_FORMAT_ECART_TYPE_CATEGORIE,
          categorie->nom_categorie,
          categorie->statistiques.nb_elements,
          categorie->statistiques.somme,
          categorie->statistiques.moyenne,
          categorie->statistiques.mediane,
          categorie->statistiques.ecart_type);
}

void port_afficher_statistiques_par_categorie(const Port port) {

   Categorie categories[BATEAU_NB_CATEGORIES];

   //Remplir les informations pour chaque categorie et creer les listes
   for (size_t i = 0; i < BATEAU_NB_CATEGORIES; i++) {
      ListeTaxes *liste = liste_taxes_creer();

      categories[i] = (Categorie) {.nom_categorie = bateau_nom_categorie(
         (int32_t) i),
         .taxes_liste=liste,
         .statistiques=(StatistiquesCategorie) {0},
         .is_liste_incomplete= liste == NULL,
         .id_categorie=(int32_t) i};

   }
   //iterer sur tous les bateaux et ajouter la taxe à la liste de la categorie
   // correspondante
   for (size_t i = 0; i < PORT_TAILLE; i++) {

      int32_t id_categorie = bateau_id_categorie(&port[i]);

      if (id_categorie == BATEAU_CATEGORIE_INVALIDE)
         continue;

      if (!categories[id_categorie].is_liste_incomplete) {
         double taxe = bateau_calculer_taxe_euro(&port[i]);
         bool is_ajout_ok = liste_taxes_ajouter(categories[id_categorie].taxes_liste,
                                                taxe);
         categories[id_categorie].is_liste_incomplete = !is_ajout_ok;
      }

   }
   //Calculer et afficher les statistiques
   for (size_t i = 0; i < BATEAU_NB_CATEGORIES; i++) {

      if (categories[i].is_liste_incomplete) {
         printf(AFFICHAGE_FORMAT_MESSAGE_ERREUR_STATISTIQUES,
                categories[i].nom_categorie);
         continue;
      }

      categories[i].statistiques = port_calculer_statistiques(categories[i]
                                                                 .taxes_liste);
      port_afficher_statistiques_categorie(&categories[i]);
      printf(AFFICHAGE_FORMAT_SEPARATEUR_ENTRE_LIGNES);
      liste_taxes_detruire(categories[i].taxes_liste);
   }
}

