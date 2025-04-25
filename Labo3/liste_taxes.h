/*
 -----------------------------------------------------------------------------------
 Nom du fichier : liste_taxes.h
 Auteur(s)      : Andre Costa, Amir Mouti, Ouweis Harun
 Date creation  : 25.05.2023

 Description    : En-tête publique d'une librairie permettant de modéliser des
                  listes de doubles avec le calcul de statistiques

 Remarque(s)    : Cette librairie implémente seulement quelques fonctions de base
                  et quelques fonctions spécifiques pour être utilisés au labo 3
                  de PRG2 2023. Vu les spécifités, certaines fonctions comme la
                  suppression d'éléments n'existent pas.

 Compilateur    : Mingw-w64 gcc 12.2.0
 -----------------------------------------------------------------------------------
*/

#include <stdint.h>
#include <stdbool.h>

#ifndef LISTE_TAXES_H
#define LISTE_TAXES_H

#ifndef LISTE_TAXES_INTERNAL_H

typedef void ListeTaxes;

#endif

/**
 * Crée une liste
 * /!\ Pour restituer la mémoire, il est conseillé de passer par liste_taxes_detruire /!\
 * @return pointeur qui peut être utilisé avec les fonctions de cette librairie ou
 * null
 */
ListeTaxes *liste_taxes_creer();

/**
 * Ajoute l'élement element à la fin de la liste liste
 * @param liste
 * @param element
 * @return true si l'element a pu être ajouté false sinon
 */
bool liste_taxes_ajouter(ListeTaxes *liste, double element);

/**
 *
 * @param liste
 * @return le nombre d'elements de la liste
 */
size_t liste_taxes_taille(const ListeTaxes *liste);


/**
 * Libere la memoire allouée pour la liste liste
 * @param liste
 */
void liste_taxes_detruire(ListeTaxes *liste);

/**
 *
 * @param liste
 * @return  la moyenne des elements de la liste liste
 */
double liste_taxes_moyenne(const ListeTaxes *liste);

/**
 *
 * @param liste
 * @return  la somme des elements de la liste liste
 */
double liste_taxes_somme(const ListeTaxes *liste);

/**
 *
 * @param liste
 * @return  la variance des elements de la liste liste ou 0 si la liste est vide
 */
double liste_taxes_variance(const ListeTaxes *liste);

/**
 *
 * @param liste
 * @return l'ecart-type des elements de la liste liste
 */
double liste_taxes_ecart_type(const ListeTaxes *liste);

/**
 *
 * @param liste
 * @return la mediane des elements de la liste. La liste n'a pas besoin d'être triée
 * @remark O(N)
 */
double liste_taxes_mediane(ListeTaxes *liste);

#endif