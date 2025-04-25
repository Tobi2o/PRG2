/*
 -----------------------------------------------------------------------------------
 Nom du fichier : liste_taxes.h
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 25.05.2023

 Description    : En-tête privé d'une librairie permettant de modéliser des
                  listes de taxes (doubles)

 Remarque(s)    : Ce fichier ne doit pas être inclus ailleurs que les fichiers
                  source de la librairie

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
*/

#include <stdint.h>

#ifndef LISTE_TAXES_INTERNAL_H
#define LISTE_TAXES_INTERNAL_H

typedef struct{
   double somme,
   somme_carre, //sert à calculer la variance et l'ecart-type
   moyenne;
}Statistiques;

typedef struct{
   double* buffer;
   size_t taille;
   size_t capacite;
   Statistiques statistiques;
}ListeTaxes;

#endif