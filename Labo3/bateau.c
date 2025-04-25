/*
 -----------------------------------------------------------------------------------
 Nom du fichier : bateau.c
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 25.05.2023

 Description    : Implementation d'une librairie permettant de modéliser des
                  bateaux de plusieurs types

 Remarque(s)    : -

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
*/

#include "bateau.h"
#include <inttypes.h>
#include <stdio.h>


//Formatage
#define AFFICHAGE_FORMAT_UINT8 "%3" PRIu8
#define AFFICHAGE_FORMAT_NOM_BATEAU "[%-18s]"
#define AFFICHAGE_FORMAT_TAXE  "Taxe: %7.2f EUR"
#define AFFICHAGE_FORMAT_TYPE_BATEAU "Categorie: %9s"
#define AFFICHAGE_FORMAT_PUISSANCE_BATEAU "Puissance: %5" PRIu16 "CV"
#define AFFICHAGE_FORMAT_NOM_PROPRIETAIRE "Proprietaire: '%s'"
#define AFFICHAGE_FORMAT_LONGUEUR "Longueur: " AFFICHAGE_FORMAT_UINT8 "m"
#define AFFICHAGE_FORMAT_TONNAGE "Tonnage: " AFFICHAGE_FORMAT_UINT8 "t"
#define AFFICHAGE_FORMAT_VOILURE "Voilure: " AFFICHAGE_FORMAT_UINT8 "m2"
#define AFFICHAGE_FORMAT_SEPARATOR " "

//Valeurs des taxes en euros
static const double BATEAU_VOILIER_TAXE_BASE = 50.;
static const double BATEAU_MOTEUR_TAXE_BASE = 100.;
static const double BATEAU_VOILIER_TAXE_DEPASSEMENT_SEUIL = 25.;
static const double BATEAU_VOILIER_TAXE_SOUS_SEUIL = 0.;
static const double BATEAU_PECHE_TAXE_DEPASSEMENT_SEUIL = 100.;
static const double BATEAU_PECHE_TAXE_SOUS_SEUIL = 0.;

static const double BATEAU_PLAISANCE_TAXE_SOUS_SEUIL = 50.;
static const double BATEAU_PLAISANCE_TAXE_PAR_M_DEPASSEMENT_SEUIL = 15.;

//Seuils pour les calculs des taxe
static const uint8_t BATEAU_PECHE_TONNAGE_SEUIL = 20;
static const uint16_t BATEAU_VOILIER_SURFACE_VOILURE_SEUIL = 200;
static const uint8_t BATEAU_PLAISANCE_PUISSANCE_SEUIL = 100;

static const char *BATEAU_TYPE_BATEAU_STRING[] = {"Voilier", "Moteur"};
static const char *BATEAU_TYPE_BATEAU_MOTEUR_STRING[] = {"Peche", "Plaisance"};

//Empecher la compilation si la taille des tableaux et la taille des enums n'est
// pas la même
//source: https://stackoverflow.com/questions/9907160/how-to-convert-enum-names-to-string-in-c
#define ASSERT_ENUM_TO_STR(sarray, max) \
  typedef char assert_sizeof[(sizeof(sarray)/sizeof(sarray[0]) == (max)) ? 1 : -1]

ASSERT_ENUM_TO_STR(BATEAU_TYPE_BATEAU_STRING, TB_TAILLE);
ASSERT_ENUM_TO_STR(BATEAU_TYPE_BATEAU_MOTEUR_STRING, TBM_TAILLE);

//Calculateurs de taxe
static double bateau_calculer_taxe_bateau_moteur(const BateauMoteur *bateau);

static double bateau_calculer_taxe_bateau_voilier(const BateauVoilier *bateau);

//Affichage
static void bateau_moteur_afficher(const BateauMoteur *bateau);

static void bateau_voilier_afficher(const BateauVoilier *bateau);

static void bateau_peche_afficher(const BateauPeche *bateau);

static void bateau_plaisance_afficher(const BateauPlaisance *bateau);

Bateau bateau_creer_bateau_voilier(const char *nom_bateau,
                                   uint16_t surface_voilure_m2) {

   return (Bateau) {
      .nom=nom_bateau,
      .type=TB_VOILIER,
      .caracteristiques= {
         .bateau_voilier = {
            .surface_voilure_m2 = surface_voilure_m2}}};
}

Bateau bateau_creer_bateau_peche(const char *nom_bateau,
                                 uint16_t puissance_moteur_cv,
                                 uint8_t poisson_max_tonnes) {

   return (Bateau) {
      .nom=nom_bateau,
      .type=TB_MOTEUR,
      .caracteristiques= {
         .bateau_moteur = {
            .puissance_moteur_cv = puissance_moteur_cv,
            .type = TBM_PECHE,
            .caracteristiques = {
               .bateau_peche = {
                  .tonnage_peche_max = poisson_max_tonnes}}}}};
}

Bateau bateau_creer_bateau_plaisance(const char *nom_bateau,
                                     uint16_t puissance_moteur_cv,
                                     const char *nom_proprietaire,
                                     uint8_t longeur_m) {
   return (Bateau) {
      .nom=nom_bateau,
      .type=TB_MOTEUR,
      .caracteristiques= {
         .bateau_moteur = {
            .puissance_moteur_cv = puissance_moteur_cv,
            .type = TBM_PLAISANCE,
            .caracteristiques = {
               .bateau_plaisance = {
                  .nom_proprietaire =
                  nom_proprietaire,
                  .longueur_m = longeur_m}}}}};
}

double bateau_calculer_taxe_euro(const Bateau *bateau) {
   if (!bateau)
      return 0.;

   switch (bateau->type) {
      case TB_MOTEUR:

         return bateau_calculer_taxe_bateau_moteur(&bateau->caracteristiques
            .bateau_moteur);

      case TB_VOILIER:
         return bateau_calculer_taxe_bateau_voilier(&bateau->caracteristiques
            .bateau_voilier);

      default:
         return 0.;
   }
}


static double bateau_calculer_taxe_bateau_moteur(const BateauMoteur *bateau) {
   double taxe = BATEAU_MOTEUR_TAXE_BASE;

   switch (bateau->type) {
      case TBM_PLAISANCE:
         if (bateau->puissance_moteur_cv < BATEAU_PLAISANCE_PUISSANCE_SEUIL)
            taxe += BATEAU_PLAISANCE_TAXE_SOUS_SEUIL;
         else
            taxe += bateau->caracteristiques.bateau_plaisance.longueur_m *
                    BATEAU_PLAISANCE_TAXE_PAR_M_DEPASSEMENT_SEUIL;
         break;
      case TBM_PECHE:
         if (bateau->caracteristiques.bateau_peche.tonnage_peche_max <
             BATEAU_PECHE_TONNAGE_SEUIL)
            taxe += BATEAU_PECHE_TAXE_SOUS_SEUIL;
         else
            taxe += BATEAU_PECHE_TAXE_DEPASSEMENT_SEUIL;

         break;
      default:
         return BATEAU_MOTEUR_TAXE_BASE;
   }
   return taxe;

}

static double bateau_calculer_taxe_bateau_voilier(const BateauVoilier *bateau) {
   double taxe = BATEAU_VOILIER_TAXE_BASE;

   if (bateau->surface_voilure_m2 < BATEAU_VOILIER_SURFACE_VOILURE_SEUIL)
      taxe += BATEAU_VOILIER_TAXE_SOUS_SEUIL;
   else
      taxe += BATEAU_VOILIER_TAXE_DEPASSEMENT_SEUIL;

   return taxe;
}

void bateau_moteur_afficher(const BateauMoteur *bateau) {
   printf(AFFICHAGE_FORMAT_PUISSANCE_BATEAU AFFICHAGE_FORMAT_SEPARATOR,
          bateau->puissance_moteur_cv);

   switch (bateau->type) {
      case TBM_PECHE:
         bateau_peche_afficher(&bateau->caracteristiques.bateau_peche);
         break;
      case TBM_PLAISANCE:
         bateau_plaisance_afficher(&bateau->caracteristiques.bateau_plaisance);
      default:
         break;
   }
}

void bateau_voilier_afficher(const BateauVoilier *bateau) {
   printf(AFFICHAGE_FORMAT_VOILURE, bateau->surface_voilure_m2);
}

void bateau_peche_afficher(const BateauPeche *bateau) {
   printf(AFFICHAGE_FORMAT_TONNAGE, bateau->tonnage_peche_max);
}

void bateau_plaisance_afficher(const BateauPlaisance *bateau) {
   printf(
      AFFICHAGE_FORMAT_LONGUEUR AFFICHAGE_FORMAT_SEPARATOR AFFICHAGE_FORMAT_NOM_PROPRIETAIRE,
      bateau->longueur_m,
      bateau->nom_proprietaire ? bateau->nom_proprietaire
                                                   : "");

}

void bateau_afficher(const Bateau *bateau) {
   if (!bateau)
      return;

   printf(AFFICHAGE_FORMAT_NOM_BATEAU AFFICHAGE_FORMAT_SEPARATOR
          AFFICHAGE_FORMAT_TAXE AFFICHAGE_FORMAT_SEPARATOR,
          bateau->nom ? bateau->nom : "",
          bateau_calculer_taxe_euro(bateau));

   switch (bateau->type) {
      case TB_MOTEUR:
         printf(AFFICHAGE_FORMAT_TYPE_BATEAU AFFICHAGE_FORMAT_SEPARATOR,

                BATEAU_TYPE_BATEAU_MOTEUR_STRING[bateau->caracteristiques.bateau_moteur
                   .type]
         );
         bateau_moteur_afficher(&bateau->caracteristiques.bateau_moteur);
         break;
      case TB_VOILIER:
         printf(AFFICHAGE_FORMAT_TYPE_BATEAU AFFICHAGE_FORMAT_SEPARATOR,
                BATEAU_TYPE_BATEAU_STRING[TB_VOILIER]);

         bateau_voilier_afficher(&bateau->caracteristiques.bateau_voilier);
         break;
      default:
         break;
   }
}


int32_t bateau_id_categorie(const Bateau *bateau) {

   if (!bateau)
      return BATEAU_CATEGORIE_INVALIDE;

   switch (bateau->type) {
      case TB_MOTEUR:
         return (int32_t) (TB_MOTEUR +
                           bateau->caracteristiques.bateau_moteur.type);
      case TB_VOILIER:
         return TB_VOILIER;
      default:
         return BATEAU_CATEGORIE_INVALIDE;
   }
}

const char *bateau_nom_categorie(int32_t indice_categorie) {
   const char *noms_categories[] = {BATEAU_TYPE_BATEAU_STRING[TB_VOILIER],
                                    BATEAU_TYPE_BATEAU_MOTEUR_STRING[TBM_PECHE],
                                    BATEAU_TYPE_BATEAU_MOTEUR_STRING[TBM_PLAISANCE]};

   if (indice_categorie < 0 ||
       indice_categorie >= sizeof(noms_categories) / sizeof(noms_categories[0]))
      return "";

   return noms_categories[indice_categorie];
}
