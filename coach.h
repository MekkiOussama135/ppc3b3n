#ifndef COACH_H_INCLUDED
#define COACH_H_INCLUDED
#include <stdio.h>

typedef struct
{
    int id;
    char nom[50];
    char prenom[50];
    char date_naissance[20];
    char telephone[20];
    char salle_sport[50];
    char motdepasse[50];
    char specialite[100];
    int disponibilite; // 1 = disponible, 0 = non disponible
} Coach;

// Prototypes des fonctions CRUD
int ajouter_coach(char *filename, Coach c);
int modifier_coach(char *filename, int id, Coach nouveau);
int supprimer_coach(char *filename, int id);
Coach chercher_coach(char *filename, int id);

// Fonctions spécifiques aux coaches
int inscrire_centre(char *filename, int id_coach, char *nom_centre);
int rejoindre_cours(char *filename, int id_coach, int id_cours);
int reserver_equipement(char *filename, int id_coach, int id_equipement, char *date);
int calc(char *filename);
#endif // COACH_H_INCLUDED
