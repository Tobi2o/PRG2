/*
 -----------------------------------------------------------------------------------
 Nom du fichier : port.h
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 28.05.2023

 Description    : En-tête publique pour une librairie permettant de modéliser un
                  port avec des bateaux de plusieurs types

 Remarque(s)    : -

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
 */

#include "bateau.h"

#ifndef PORT_H
#define PORT_H

#define PORT_TAILLE 10

typedef Bateau Port[PORT_TAILLE];

/**
 * Fonction qui affiche les caracteristiques des bateaux du port passé en paramètre
 * en ordre decroissant des taxes annuelles dûes par les bâteaux
 */
void port_afficher_taxe_decroissante(const Port port);

/**
 * Fonction qui affiche des statistiques par catégorie de bateau
 */
void port_afficher_statistiques_par_categorie(const Port port);

/**
 * Affiche les caractéristiques de chacun des bâteaux du port
 */
void port_afficher(const Port port);

#endif