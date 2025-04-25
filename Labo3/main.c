/*
 -----------------------------------------------------------------------------------
 Nom du fichier : main.c
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 25.05.2023

 Description    : Programme de test pour la modélisation d'un port et de bateaux

 Remarque(s)    : Le nombre de bateaux doit correspondre à la taille du port

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
*/
#include "bateau.h"
#include "port.h"

int main() {
   Port port = {bateau_creer_bateau_peche("Titanic Pecheur", 49000, 19),
                bateau_creer_bateau_peche("Balticdeck", 52000, 20),
                bateau_creer_bateau_peche("Le thon est top", 60000, 50),
                bateau_creer_bateau_voilier(NULL, 199),
                bateau_creer_bateau_voilier("CaliforniaWear", 200),
                bateau_creer_bateau_voilier("Makoscope", 50),
                bateau_creer_bateau_plaisance("Surfmain", 99, "Ouweis", 255),
                bateau_creer_bateau_plaisance("Airwish", 105, NULL, 100),
                bateau_creer_bateau_plaisance("ArabianFive", 65535, "Amir", 200),
                bateau_creer_bateau_plaisance("RRH", 60000, "Rentsch", 55)};

   port_afficher_taxe_decroissante(port);
   port_afficher_statistiques_par_categorie(port);
   return 0;
}
