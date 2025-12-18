#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "evenement.h"

void afficher_menu() {
    printf("\nGESTION DES EVENEMENTS\n");
    printf("1. Ajouter un evenement\n");
    printf("2. Modifier un evenement\n");
    printf("3. Supprimer un evenement\n");
    printf("4. Rechercher un evenement\n");
    printf("5. Afficher tous les evenements\n");
    printf("6. Afficher le nombre total d'evenements\n"); 
    printf("0. Quitter\n");
    printf("Votre choix: ");
}

void ajouter_evenement_interactif() {
    Evenement e;
    
    printf("\nAJOUT D'UN NOUVEL EVENEMENT\n");
    
    printf("ID: ");
    scanf("%d", &e.id);
    getchar();
    
    printf("Nom: ");
    fgets(e.nom, 50, stdin);
    e.nom[strcspn(e.nom, "\n")] = 0;
    
    printf("Type (competition/cardio/meet_n_greet/autres): ");
    fgets(e.type, 20, stdin);
    e.type[strcspn(e.type, "\n")] = 0;
    
    printf("Date (jour mois annee): ");
    scanf("%d %d %d", &e.jour, &e.mois, &e.annee);
    getchar();
    
    printf("Lieu: ");
    fgets(e.lieu, 100, stdin);
    e.lieu[strcspn(e.lieu, "\n")] = 0;
    
    printf("Responsable: ");
    fgets(e.responsable, 50, stdin);
    e.responsable[strcspn(e.responsable, "\n")] = 0;
    
    printf("Description: ");
    fgets(e.description, 200, stdin);
    e.description[strcspn(e.description, "\n")] = 0;
    
    if(ajouter_evenement("evenements.txt", e)) {
        printf("Evenement ajoute avec succes!\n");
    } else {
        printf("Erreur lors de l'ajout.\n");
    }
}

void modifier_evenement_interactif() {
    int id;
    Evenement ancien, nouv;
    char buffer[100];
    
    printf("\nMODIFICATION D'UN EVENEMENT\n");
    
    printf("ID de l'evenement a modifier: ");
    scanf("%d", &id);
    getchar();
    
    ancien = chercher_evenement("evenements.txt", id);
    if(ancien.id == -1) {
        printf("Evenement non trouve.\n");
        return;
    }
    
    printf("\nEvenement actuel:\n");
    afficher_evenement(ancien);
    
    printf("\nLaissez vide pour conserver la valeur actuelle.\n");
    
    nouv.id = id;
    nouv.nom[0] = nouv.type[0] = nouv.lieu[0] = nouv.responsable[0] = nouv.description[0] = '\0';
    nouv.jour = nouv.mois = nouv.annee = -1;
    
    printf("Nouveau nom [%s]: ", ancien.nom);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strlen(buffer) > 0) strcpy(nouv.nom, buffer);
    
    printf("Nouveau type [%s]: ", ancien.type);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strlen(buffer) > 0) strcpy(nouv.type, buffer);
    
    printf("Nouvelle date (jour mois annee) [%d %d %d]: ", ancien.jour, ancien.mois, ancien.annee);
    fgets(buffer, sizeof(buffer), stdin);
    if(strlen(buffer) > 1) {
        sscanf(buffer, "%d %d %d", &nouv.jour, &nouv.mois, &nouv.annee);
    }
    
    printf("Nouveau lieu [%s]: ", ancien.lieu);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strlen(buffer) > 0) strcpy(nouv.lieu, buffer);
    
    printf("Nouveau responsable [%s]: ", ancien.responsable);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strlen(buffer) > 0) strcpy(nouv.responsable, buffer);
    
    printf("Nouvelle description [%s]: ", ancien.description);
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = 0;
    if(strlen(buffer) > 0) strcpy(nouv.description, buffer);
    
    if(modifier_evenement("evenements.txt", id, nouv)) {
        printf("Evenement modifie avec succes!\n");
    } else {
        printf("Erreur lors de la modification.\n");
    }
}

void afficher_total_evenements() {
    int total = compter_total_evenements("evenements.txt");
    printf("\nNOMBRE TOTAL D'EVENEMENTS\n");
    printf("Nombre total d'evenements enregistres: %d\n", total);
}

int main() {
    int choix;
    
    printf("SYSTEME DE GESTION DES EVENEMENTS\n");
    
    do {
        afficher_menu();
        scanf("%d", &choix);
        getchar();
        
        switch(choix) {
            case 1:
                ajouter_evenement_interactif();
                break;
                
            case 2:
                modifier_evenement_interactif();
                break;
                
            case 3:
                {
                    int id;
                    printf("\nSUPPRESSION D'UN EVENEMENT\n");
                    printf("ID de l'evenement a supprimer: ");
                    scanf("%d", &id);
                    
                    if(supprimer_evenement("evenements.txt", id)) {
                        printf("Evenement supprime avec succes!\n");
                    } else {
                        printf("Evenement non trouve.\n");
                    }
                }
                break;
                
            case 4:
                {
                    int id;
                    printf("\nRECHERCHE D'UN EVENEMENT\n");
                    printf("ID de l'evenement a rechercher: ");
                    scanf("%d", &id);
                    
                    Evenement resultat = chercher_evenement("evenements.txt", id);
                    afficher_evenement(resultat);
                }
                break;
                
            case 5:
                afficher_tous_evenements("evenements.txt");
                break;
                
            case 6:
                afficher_total_evenements();
                break;
                
            case 0:
                printf("Au revoir!\n");
                break;
                
            default:
                printf("Choix invalide!\n");
                break;
        }
        
        if(choix != 0) {
            printf("\nAppuyez sur Entree pour continuer...");
            getchar();
        }
        
    } while(choix != 0);
    
    return 0;
}
