#include "coach.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int ajouter_coach(char *filename, Coach c)
{
    FILE *f = fopen(filename, "a");
    if(f != NULL)
    {
        fprintf(f, "%d %s %s %s %s %s %s %d\n", c.id, c.nom, c.prenom, c.date_naissance, c.telephone, c.salle_sport, c.specialite, c.disponibilite);
        fclose(f);
        return 1;
    }
    return 0;
}

int modifier_coach(char *filename, int id, Coach nouv)
{
    int tr = 0;
    Coach c;
    FILE *f = fopen(filename, "r");
    if(f == NULL) 
    return 0;
    
    FILE *f2 = fopen("temp.txt", "w");
    if(f2 == NULL) {
        fclose(f);
        return 0;
    }
    
    while(fscanf(f, "%d %s %s %s %s %s %s %d", &c.id, c.nom, c.prenom, c.date_naissance, c.telephone, c.salle_sport, c.specialite, &c.disponibilite) != EOF)
    {
        if(c.id == id)
        {
            fprintf(f2, "%d %s %s %s %s %s %s %d\n",nouv.id, nouv.nom, nouv.prenom, nouv.date_naissance, nouv.telephone, nouv.salle_sport, nouv.specialite, nouv.disponibilite);
            tr = 1;
        }
        else
        {
            fprintf(f2, "%d %s %s %s %s %s %s %d\n", c.id, c.nom, c.prenom, c.date_naissance, c.telephone, c.salle_sport, c.specialite, c.disponibilite);
        }
    }
    
    fclose(f);
    fclose(f2);
    
    if(remove(filename) != 0) 
    return 0;
    if(rename("temp.txt", filename) != 0) 
    return 0;
    
    return tr;
}

Coach chercher_coach(char *filename, int id)
{
    Coach c;
    int tr = 0;
    FILE *f = fopen(filename, "r");
    
    if(f != NULL)
    {
        while(!tr && fscanf(f, "%d %s %s %s %s %s %s %d", &c.id, c.nom, c.prenom, c.date_naissance, c.telephone, c.salle_sport, c.specialite, &c.disponibilite) != EOF)
        {
            if(c.id == id)
                tr = 1;
        }
        fclose(f);
    }
    
    if(!tr)
        c.id = -1;
    return c;
}

int inscrire_centre(char *filename, int id_coach, char *nom_centre)
{
    FILE *f = fopen(filename, "a");
    if(f != NULL)
    {
        fprintf(f, "%d %s\n", id_coach, nom_centre);
        fclose(f);
        return 1;
    }
    else 
        return 0;
}

int rejoindre_cours(char *filename, int id_coach, int id_cours)
{
    FILE *f = fopen(filename, "a");
    if(f != NULL)
    {
        fprintf(f, "%d %d\n", id_coach, id_cours);
        fclose(f);
        
        Coach c = chercher_coach("coaches.txt", id_coach);
        if(c.id != -1)
        {
            c.disponibilite = 0; 
            modifier_coach("coaches.txt", id_coach, c);
            printf("Coach %d a rejoint le cours %d\n", id_coach, id_cours);
        }
        return 1;
    }
    else
        return 0;
}
int reserver_equipement(char *filename, int id_coach, int id_equipement, char *date)
{
    FILE *f = fopen(filename, "a");
    if(f != NULL)
    {
        fprintf(f, "%d %d %s\n", id_coach, id_equipement, date);
        fclose(f);
        return 1;
    }
    else 
        return 0;
}
int supprimer_coach(char *filename, int id) {
    int tr = 0;
    Coach c;
    FILE *f = fopen(filename, "r");
    if(f == NULL) 
        return 0;
    
    FILE *f2 = fopen("temp.txt", "w");
    if(f2 == NULL) {
        fclose(f);
        return 0;
    }
    
    while(fscanf(f, "%d %s %s %s %s %s %s %d", 
                 &c.id, c.nom, c.prenom, c.date_naissance, 
                 c.telephone, c.salle_sport, c.specialite, &c.disponibilite) != EOF) {
        if(c.id == id) {
            tr = 1; // Coach trouvé et sera supprimé
        } else {
            fprintf(f2, "%d %s %s %s %s %s %s %d\n",
                    c.id, c.nom, c.prenom, c.date_naissance, 
                    c.telephone, c.salle_sport, c.specialite, c.disponibilite);
        }
    }
    
    fclose(f);
    fclose(f2);
    
    if(tr) {
        if(remove(filename) != 0) {
            remove("temp.txt");
            return 0;
        }
        if(rename("temp.txt", filename) != 0) {
            return 0;
        }
    } else {
        remove("temp.txt");
    }
    
    return tr;
}


int calc(char *filename){
char nom[100],date[100],heure[100],salle[100];
int no=0;
FILE *f = fopen(filename, "r");
while(fscanf(f, "%s %s %s %s", 
                 nom,date,heure,salle) != EOF) {
no+=1;}
fclose(f);
return no;
}

