/*
 -----------------------------------------------------------------------------------
 Nom du fichier : bateau.h
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 25.05.2023

 Description    : En-tête publique pour une librairie permettant de modéliser des
                  bateaux de plusieurs types

 Remarque(s)    : -

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
 */
#include <stdint.h>

#ifndef BATEAU_H
#define BATEAU_H

#define BATEAU_CATEGORIE_INVALIDE (-1)
#define BATEAU_NB_CATEGORIES 3

typedef enum {
   TBM_PECHE,
   TBM_PLAISANCE,
   TBM_TAILLE
} TypeBateauMoteur;

typedef enum {
   TB_VOILIER,
   TB_MOTEUR,
   TB_TAILLE
} TypeBateau;

typedef struct {
   uint16_t surface_voilure_m2;
} BateauVoilier;

typedef struct {
   uint8_t tonnage_peche_max;
} BateauPeche;

typedef struct {
   const char *nom_proprietaire;
   uint8_t longueur_m;
} BateauPlaisance;

typedef union {
   BateauPeche bateau_peche;
   BateauPlaisance bateau_plaisance;
} CaracteristiquesBateauMoteur;

typedef struct {
   uint16_t puissance_moteur_cv;
   TypeBateauMoteur type;
   CaracteristiquesBateauMoteur caracteristiques;
} BateauMoteur;

typedef union {
   BateauVoilier bateau_voilier;
   BateauMoteur bateau_moteur;
} CaracteristiquesBateau;

typedef struct {
   const char *nom;
   TypeBateau type;
   CaracteristiquesBateau caracteristiques;
} Bateau;

/**
 * Cree un bateau du type Voilier
 */
Bateau bateau_creer_bateau_voilier(const char *nom_bateau,
                                   uint16_t surface_voilure_m2);

/**
 * Cree un bateau du type Peche
 */
Bateau bateau_creer_bateau_peche(const char *nom_bateau,
                                 uint16_t puissance_moteur_cv,
                                 uint8_t poisson_max_tonnes);

/**
 * Cree un bateau du type Plaisance
 */
Bateau bateau_creer_bateau_plaisance(const char *nom_bateau,
                                     uint16_t puissance_moteur_cv,
                                     const char *nom_proprietaire,
                                     uint8_t longeur_m);


/**
 * Calcule la taxe qui est dûe par un bateau
 */
double bateau_calculer_taxe_euro(const Bateau *bateau);

/**
 * Affiche les caractéristiques d'un bateau
 */
void bateau_afficher(const Bateau *bateau);

/**
 * Identifiant numerique de la categorie du bateau
 * @return un id unique entre 0 et BATEAU_NB_CATEGORIES -1 pour la categorie du
 * bateau ou BATEAU_CATEGORIE_INVALIDE
 */
int32_t bateau_id_categorie(const Bateau *bateau);

/**
 * Retourne le nom de la categorie a partir de son id
 * @param id_categorie un id unique pour une categorie donnée
 * @return le nom de la categorie ou "" si l'id n'est pas valide
 */
const char *bateau_nom_categorie(int32_t id_categorie);

#endif