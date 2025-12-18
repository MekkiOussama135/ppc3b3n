#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif
#include <string.h>
#include <gtk/gtk.h>

#include <stdlib.h>
#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include <unistd.h> // getcwd
int cours_id_selectionne = -1;



// --------------------------------------------
// Widgets globaux
// --------------------------------------------
GtkWidget *notebook;
enum {
    COL_COURS_ID = 0,
    COL_COURS_NOM,
    COL_COURS_ENTRAINEUR,
    COL_COURS_QUANTITE,
    N_COLS_COURS
};

enum {
    COL_LUNDI = 0,
    COL_MARDI,
    COL_MERCREDI,
    COL_JEUDI,
    COL_VENDREDI,
    COL_SAMEDI,
    COL_DIMANCHE,
    N_COLS_PLANNING
};

/*===========================================
  FONCTION 1: Charger les cours dans le TreeView
  Affiche: ID | Cours | Entraineur | Places
===========================================*/
void load_lessons_into_treeview(GtkWidget *treeview) 
{
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    if (!treeview) {
        printf("load_lessons: treeview=NULL\n");
        return;
    }
    
    // Vérifier si les colonnes existent déjà
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == NULL) {
        // CRÉER LE MODÈLE (4 colonnes STRING)
        store = gtk_list_store_new(N_COLS_COURS, 
                                   G_TYPE_STRING,  // ID
                                   G_TYPE_STRING,  // Cours
                                   G_TYPE_STRING,  // Entraineur
                                   G_TYPE_STRING); // Quantité
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        
        // CRÉER LES COLONNES
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("ID", renderer, "text", COL_COURS_ID, NULL);
        gtk_tree_view_column_set_min_width(column, 40);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Cours", renderer, "text", COL_COURS_NOM, NULL);
        gtk_tree_view_column_set_min_width(column, 150);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Entraineur", renderer, "text", COL_COURS_ENTRAINEUR, NULL);
        gtk_tree_view_column_set_min_width(column, 120);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        
        renderer = gtk_cell_renderer_text_new();
        column = gtk_tree_view_column_new_with_attributes("Places", renderer, "text", COL_COURS_QUANTITE, NULL);
        gtk_tree_view_column_set_min_width(column, 60);
        gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
    }
    
    // RÉCUPÉRER LE STORE ET LE VIDER
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    store = GTK_LIST_STORE(model);
    gtk_list_store_clear(store);
    
    // OUVRIR LE FICHIER
    FILE *fichier = fopen("data7/cours.txt", "r");
    if (!fichier) {
        printf("Erreur : fichier cours.txt introuvable\n");
        return;
    }
    
    // LIRE ET REMPLIR LE TREEVIEW
    char ligne[300];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        ligne[strcspn(ligne, "\r\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        char id[100], cours[100], entraineur[100], qt[100];
        
        // Parser: id;cours;entraineur;quantite
        if (sscanf(ligne, "%99[^;];%99[^;];%99[^;];%99[^\n]", 
                   id, cours, entraineur, qt) == 4) {
            
            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter, 
                              COL_COURS_ID, id,
                              COL_COURS_NOM, cours,
                              COL_COURS_ENTRAINEUR, entraineur,
                              COL_COURS_QUANTITE, qt,
                              -1);
        }
    }
    
    fclose(fichier);
    printf("✓ Cours chargés dans TreeView\n");
}
/*===========================================
  FONCTION 2: Récupérer l'ID du cours sélectionné
===========================================*/
int get_selected_cours_id(GtkWidget *treeview)
{
    GtkTreeSelection *selection;
    GtkTreeModel *model;
    GtkTreeIter iter;
    char *id_str = NULL;
    int id = -1;

    if (!treeview) {
        printf("get_selected_cours_id: treeview=NULL\n");
        return -1;
    }
    selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(treeview));
    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        // Récupérer l'ID (colonne 0) en tant que STRING
        gtk_tree_model_get(model, &iter, COL_COURS_ID, &id_str, -1);
        
        if (id_str != NULL) {
            id = atoi(id_str);  // Convertir STRING → INT
            printf("✓ Cours sélectionné: ID=%d\n", id);
            g_free(id_str);
            return id;
        }
    }

    printf("✗ Aucun cours sélectionné\n");
    return -1;
}
/*FONCTION 3: Afficher le planning (7 colonnes pour 7 jours)
  UNE SEULE LIGNE avec les 7 programmes
===========================================*/
void afficher_planning(GtkWidget *treeview, int id_cours, const char *type_programme)
{
    GtkListStore *store;
    GtkTreeIter iter;
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    
    if (!treeview) {
        printf("afficher_planning: treeview=NULL\n");
        return;
    }
    
    // CRÉER LES COLONNES SI ELLES N'EXISTENT PAS
    if (gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview)) == NULL) {
        // CRÉER LE MODÈLE (7 colonnes STRING pour 7 jours)
        store = gtk_list_store_new(N_COLS_PLANNING,
                                   G_TYPE_STRING,  // Lundi
                                   G_TYPE_STRING,  // Mardi
                                   G_TYPE_STRING,  // Mercredi
                                   G_TYPE_STRING,  // Jeudi
                                   G_TYPE_STRING,  // Vendredi
                                   G_TYPE_STRING,  // Samedi
                                   G_TYPE_STRING); // Dimanche
        gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
        g_object_unref(store);
        
        // CRÉER LES 7 COLONNES (UNE PAR JOUR)
        const char *jours[] = {"Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi", "Dimanche"};
        
        for (int i = 0; i < 7; i++) {
            renderer = gtk_cell_renderer_text_new();
            g_object_set(renderer, "wrap-mode", PANGO_WRAP_WORD, "wrap-width", 150, NULL);
            column = gtk_tree_view_column_new_with_attributes(jours[i], renderer, "text", i, NULL);
            gtk_tree_view_column_set_min_width(column, 120);
            gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);
        }
    }
    
    // RÉCUPÉRER LE STORE ET LE VIDER
    GtkTreeModel *model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    store = GTK_LIST_STORE(model);
    gtk_list_store_clear(store);
    
    // OUVRIR LE FICHIER PROGRAMME
    FILE *fichier = fopen("data7/programmeCours.txt", "r");
    if (!fichier) {
        printf("Erreur : fichier programmeCours.txt introuvable\n");
        return;
    }
    
    // CHERCHER LE PROGRAMME CORRESPONDANT
    char ligne[1024];
    int trouve = 0;
    
    while (fgets(ligne, sizeof(ligne), fichier)) {
        ligne[strcspn(ligne, "\r\n")] = 0;
        
        if (strlen(ligne) == 0) continue;
        
        // Parser: id;categorie;planning
        char id_str[20], categorie[50], planning[900];
        
        if (sscanf(ligne, "%19[^;];%49[^;];%899[^\n]", id_str, categorie, planning) == 3) {
            int id = atoi(id_str);
            
            // VÉRIFIER SI C'EST LE BON COURS ET LE BON TYPE
            if (id == id_cours && strcmp(categorie, type_programme) == 0) {
                trouve = 1;
                printf("✓ Programme trouvé: ID=%d, Type=%s\n", id_cours, type_programme);
                
                // TABLEAU POUR STOCKER LES 7 ACTIVITÉS
                char activites[7][200];
                for (int i = 0; i < 7; i++) {
                    strcpy(activites[i], "N/A");
                }
                
                // PARSER LE PLANNING: Lundi:Activité|Mardi:Activité|...
                char *copie = strdup(planning);
                char *token = strtok(copie, "|");
                int index = 0;
                
                while (token != NULL && index < 7) {
                    // Séparer Jour:Activité
                    char *separateur = strchr(token, ':');
                    if (separateur != NULL) {
                        // Récupérer l'activité (après le ':')
                        char *activite = separateur + 1;
                        
                        // Enlever espaces au début
                        while (*activite == ' ') activite++;
                        
                        // Copier dans le tableau
                        strncpy(activites[index], activite, 199);
                        activites[index][199] = '\0';
                    }
                    
                    token = strtok(NULL, "|");
                    index++;
                }
                
                free(copie);
                
                // AJOUTER UNE SEULE LIGNE AVEC LES 7 ACTIVITÉS
                gtk_list_store_append(store, &iter);
                gtk_list_store_set(store, &iter,
                                 COL_LUNDI, activites[0],
                                 COL_MARDI, activites[1],
                                 COL_MERCREDI, activites[2],
                                 COL_JEUDI, activites[3],
                                 COL_VENDREDI, activites[4],
                                 COL_SAMEDI, activites[5],
                                 COL_DIMANCHE, activites[6],
                                 -1);
                
                printf("✓ Programme affiché dans TreeView\n");
                break;
            }
        }
    }
    
    fclose(fichier);
    
    if (!trouve) {
        printf("✗ Programme introuvable pour cours %d type %s\n", id_cours, type_programme);
    }
}
/*===========================================
  CALLBACK: Changement de sélection dans TreeView cours
===========================================*/
void on_cours_selection_changed(GtkTreeSelection *selection, gpointer user_data)
{
    GtkTreeIter iter;
    GtkTreeModel *model;
    char *id_str = NULL;

    if (gtk_tree_selection_get_selected(selection, &model, &iter)) {
        gtk_tree_model_get(model, &iter, COL_COURS_ID, &id_str, -1);
        
        if (id_str != NULL) {
            cours_id_selectionne = atoi(id_str);
            printf("\n>>> Cours sélectionné: ID=%d <<<\n", cours_id_selectionne);
            g_free(id_str);
        }
    } else {
        cours_id_selectionne = -1;
        printf("\n>>> Aucune sélection <<<\n");
    }
}

/*===========================================
  CALLBACK: Bouton "Afficher Programme"
===========================================*/
void on_inscrirecours1_clicked(GtkWidget *button, gpointer user_data)
{
    // RÉCUPÉRER LES WIDGETS
    GtkWidget *treeview_cours = lookup_widget(button, "treeviewCours");
    GtkWidget *treeview_prog = lookup_widget(button, "treeviewProgramme");
    GtkWidget *radio_normal_widget = lookup_widget(button, "radiobutton_normal");
    GtkWidget *radio_pro_widget = lookup_widget(button, "radiobutton_pro");
    
    if (!treeview_cours || !treeview_prog) {
        printf("✗ Erreur: TreeViews introuvables\n");
        return;
    }
    
    // ÉTAPE 1: RÉCUPÉRER L'ID DU COURS SÉLECTIONNÉ
    int id_cours = get_selected_cours_id(treeview_cours);
    
    if (id_cours == -1) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_MODAL,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "⚠️ Veuillez sélectionner un cours dans la liste!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // ÉTAPE 2: DÉTERMINER LE TYPE DE PROGRAMME (Normal ou Pro)
    const char *type_programme;
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_pro_widget))) {
        type_programme = "Pro";
    } else {
        type_programme = "Normal";
    }
    
    printf("\n=== AFFICHAGE PROGRAMME ===\n");
    printf("Cours ID: %d\n", id_cours);
    printf("Type: %s\n", type_programme);
    printf("===========================\n\n");
    
    // ÉTAPE 3: AFFICHER LE PROGRAMME DANS LE SECOND TREEVIEW
    afficher_planning(treeview_prog, id_cours, type_programme);
    
    // ÉTAPE 4: MESSAGE DE CONFIRMATION
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_MODAL,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "✅ Programme %s chargé!\n\nCours ID: %d", type_programme, id_cours);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}
// ========== ADMIN / COURS ==========

void on_rechercheadmin1_clicked(GtkWidget *button, gpointer user_data) {
    g_print("Recherche admin cliqué\n");
}

// Ajout admin -> page 5
void on_ajoutadmin1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1");

    if (!notebook) { g_print("Erreur: notebook1 introuvable\n"); return; }

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 5);
}

// Supprimer admin -> page 6
void on_supprimeradmin1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1");

    if (!notebook) { g_print("Erreur: notebook1 introuvable\n"); return; }

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 6);
}

// Modifier cours admin -> page 7
void on_modifcoursadmin1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1");

    if (!notebook) { g_print("Erreur: notebook1 introuvable\n"); return; }

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 7);
}

// ========== RADIOS AJOUT ==========

void on_radio_homme_add1_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    g_print("Radio Homme (ajout) toggled\n");
}

void on_radio_femme_add1_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    g_print("Radio Femme (ajout) toggled\n");
}

void on_radio_mixte_add1_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    g_print("Radio Mixte (ajout) toggled\n");
}

// ========== AJOUT / SUPPRESSION COURS ==========


void on_supprimer_cours1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(button);

    GtkWidget *spin_id_supp1    = lookup_widget(window, "spin_id_supp1");
    GtkWidget *labelMessageSupp = lookup_widget(window, "labelMessageSupp");

    if (!spin_id_supp1 || !labelMessageSupp) {
        g_print("Erreur: widgets suppression introuvables\n");
        return;
    }

    int id_supp = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_id_supp1));

    if (id_supp <= 0) {
        gtk_label_set_text(GTK_LABEL(labelMessageSupp), "⚠️ Veuillez saisir un ID valide.");
        return;
    }

    FILE *f = fopen("data7/cours.txt", "r");
    if (!f) {
        gtk_label_set_text(GTK_LABEL(labelMessageSupp), "❌ Erreur d'ouverture du fichier data7/cours.txt.");
        return;
    }

    FILE *tmp = fopen("data7/cours_tmp.txt", "w");
    if (!tmp) {
        fclose(f);
        gtk_label_set_text(GTK_LABEL(labelMessageSupp), "❌ Erreur création fichier temporaire.");
        return;
    }

    char ligne[1024];
    int found = 0;

    // Lire ligne par ligne (header ou pas, on gère)
    while (fgets(ligne, sizeof(ligne), f))
    {
        // Si header (optionnel) : on le copie tel quel
        if (strstr(ligne, "id;") != NULL) {
            fputs(ligne, tmp);
            continue;
        }

        int id_lu;
        if (sscanf(ligne, "%d;%*[^;\n]", &id_lu) == 1 && id_lu == id_supp) {
            found = 1;      // ligne à supprimer -> on ne la copie pas
            continue;
        }

        // garder la ligne
        fputs(ligne, tmp);
    }

    fclose(f);
    fclose(tmp);

    // Remplacement du fichier original
    if (remove("data7/cours.txt") != 0) {
        gtk_label_set_text(GTK_LABEL(labelMessageSupp), "❌ Erreur: impossible de supprimer l'ancien fichier.");
        return;
    }

    if (rename("data7/cours_tmp.txt", "data7/cours.txt") != 0) {
        gtk_label_set_text(GTK_LABEL(labelMessageSupp), "❌ Erreur: impossible de renommer le fichier temporaire.");
        return;
    }

    if (found)
        gtk_label_set_text(GTK_LABEL(labelMessageSupp), "✅ Cours supprimé avec succès.");
    else
        gtk_label_set_text(GTK_LABEL(labelMessageSupp), "⚠️ Aucun cours trouvé avec cet ID.");
}


// ========== RADIOS MODIF ==========

void on_radio_homme_modif1_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    g_print("Radio Homme (modif) toggled\n");
}

void on_radio_femme_modif1_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    g_print("Radio Femme (modif) toggled\n");
}

void on_radio_mixte_modif1_toggled(GtkToggleButton *togglebutton, gpointer user_data) {
    g_print("Radio Mixte (modif) toggled\n");
}

// ========== MODIFIER COURS ==========

// Vérifier si l'ID existe dans data7/cours.txt
static int id_existe_dans_cours_txt(int id)
{
    FILE *f = fopen("data7/cours.txt", "r");
    if (!f) return 0;

    char ligne[512];
    int id_lu;

    while (fgets(ligne, sizeof(ligne), f))
    {
        // On suppose que l'ID est en début de ligne: id;...
        if (sscanf(ligne, "%d;", &id_lu) == 1) {
            if (id_lu == id) {
                fclose(f);
                return 1;
            }
        }
    }

    fclose(f);
    return 0;
}

void on_modifiercours1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(button);

    GtkWidget *spin_id_modif1     = lookup_widget(window, "spin_id_modif1");
    GtkWidget *entry_coach_modif1 = lookup_widget(window, "entry_coach_modif1");
    GtkWidget *entry_date_modif1  = lookup_widget(window, "entry_date_modif1");
    GtkWidget *entry_heure_modif1 = lookup_widget(window, "entry_heure_modif1");
    GtkWidget *spin_centre_modif1 = lookup_widget(window, "spin_centre_modif1");
    GtkWidget *spin_places_modif1 = lookup_widget(window, "spin_places_modif1");
    GtkWidget *radio_homme_modif1 = lookup_widget(window, "radio_homme_modif1");
    GtkWidget *radio_femme_modif1 = lookup_widget(window, "radio_femme_modif1");
    GtkWidget *radio_mixte_modif1 = lookup_widget(window, "radio_mixte_modif1");

    GtkWidget *labelMessageModif  = lookup_widget(window, "labelMessageModif");

    if (!spin_id_modif1 || !entry_coach_modif1 || !entry_date_modif1 || !entry_heure_modif1 ||
        !spin_centre_modif1 || !spin_places_modif1 ||
        !radio_homme_modif1 || !radio_femme_modif1 || !radio_mixte_modif1 ||
        !labelMessageModif)
    {
        g_print("Erreur: widgets modification introuvables\n");
        return;
    }

    int id = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_id_modif1));
    const gchar *coach = gtk_entry_get_text(GTK_ENTRY(entry_coach_modif1));
    const gchar *date  = gtk_entry_get_text(GTK_ENTRY(entry_date_modif1));
    const gchar *heure = gtk_entry_get_text(GTK_ENTRY(entry_heure_modif1));
    int centre = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_centre_modif1));
    int places = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_places_modif1));

    if (id <= 0 || !coach || !date || !heure || strlen(coach)==0 || strlen(date)==0 || strlen(heure)==0) {
        gtk_label_set_text(GTK_LABEL(labelMessageModif), "⚠️ Veuillez remplir tous les champs.");
        return;
    }

    char type[20];
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_homme_modif1)))
        strcpy(type, "Homme");
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_femme_modif1)))
        strcpy(type, "Femme");
    else
        strcpy(type, "Mixte");

    // ✅ 1) Vérifier ID dans data7/cours.txt
    if (!id_existe_dans_cours_txt(id)) {
        gtk_label_set_text(GTK_LABEL(labelMessageModif),
                           "⚠️ ID introuvable dans data7/cours.txt (modification refusée).");
        return;
    }

    // ✅ 2) Ouvrir cours2.txt (à la racine) et réécrire dans un tmp
    FILE *f = fopen("cours2.txt", "r");
    FILE *tmp = fopen("cours2_tmp.txt", "w");
    if (!tmp) {
        if (f) fclose(f);
        gtk_label_set_text(GTK_LABEL(labelMessageModif),
                           "❌ Erreur: impossible de créer le fichier temporaire.");
        return;
    }

    char ligne[1024];
    int found = 0;

    if (f) {
        while (fgets(ligne, sizeof(ligne), f))
        {
            int id_lu;
            if (sscanf(ligne, "%d;", &id_lu) == 1 && id_lu == id) {
                fprintf(tmp, "%d;%s;%s;%s;%d;%d;%s\n",
                        id, coach, date, heure, centre, places, type);
                found = 1;
            } else {
                fputs(ligne, tmp);
            }
        }
        fclose(f);
    }

    // ✅ 3) Si l’ID n’existait pas dans cours2.txt => on l’ajoute
    if (!found) {
        fprintf(tmp, "%d;%s;%s;%s;%d;%d;%s\n",
                id, coach, date, heure, centre, places, type);

        gtk_label_set_text(GTK_LABEL(labelMessageModif),
                           "ℹ️ ID non trouvé dans cours2.txt : ligne ajoutée.");
    } else {
        gtk_label_set_text(GTK_LABEL(labelMessageModif),
                           "✅ Modification effectuée avec succès.");
    }

    fclose(tmp);

    // ✅ 4) Remplacer cours2.txt
    remove("cours2.txt");
    rename("cours2_tmp.txt", "cours2.txt");
}


// ========== IDENTIFICATION ==========

static int verifier_login(const char *filepath, const char *id, const char *mdp)
{
    FILE *f = fopen(filepath, "r");
    if (!f) return 0;

    char line[256];
    char id_f[100], mdp_f[100];

    while (fgets(line, sizeof(line), f))
    {
        // lire: id;mdp
        if (sscanf(line, "%99[^;];%99[^\n]", id_f, mdp_f) == 2) {
            if (strcmp(id_f, id) == 0 && strcmp(mdp_f, mdp) == 0) {
                fclose(f);
                return 1; // trouvé
            }
        }
    }

    fclose(f);
    return 0; // pas trouvé
}
void on_identidier1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(button);

    GtkWidget *entry_id_auth1  = lookup_widget(window, "entry_id_auth1");
    GtkWidget *entry_mpd_auth1 = lookup_widget(window, "entry_mpd_auth1");
    GtkWidget *labelMessage2   = lookup_widget(window, "labelMessage2");
    GtkWidget *notebook        = lookup_widget(window, "notebook1"); // ✅ notebook

    if (!entry_id_auth1 || !entry_mpd_auth1 || !labelMessage2 || !notebook) {
        g_print("Erreur: widgets admin introuvables\n");
        return;
    }

    const gchar *id  = gtk_entry_get_text(GTK_ENTRY(entry_id_auth1));
    const gchar *mdp = gtk_entry_get_text(GTK_ENTRY(entry_mpd_auth1));

    if (!id || !mdp || strlen(id) == 0 || strlen(mdp) == 0) {
        gtk_label_set_text(GTK_LABEL(labelMessage2),
                           "⚠️ Veuillez remplir ID et mot de passe.");
        return;
    }

    if (verifier_login("admin.txt", id, mdp)) {
        gtk_label_set_text(GTK_LABEL(labelMessage2),
                           "✅ Admin authentifié !");

        // ✅ Aller à la page 3 du notebook
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);

    } else {
        gtk_label_set_text(GTK_LABEL(labelMessage2),
                           "❌ Identifiant ou mot de passe incorrect.");
        gtk_entry_set_text(GTK_ENTRY(entry_mpd_auth1), "");
    }
}

void on_idenifier_membre1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(button);

    GtkWidget *entry_id_membre_auth1  = lookup_widget(window, "entry_id_auth_mem");
    GtkWidget *entry_mdp_membre_auth1 = lookup_widget(window, "entry_mdp_auth_mem");
    GtkWidget *labelMessage           = lookup_widget(window, "labelMessage");
    GtkWidget *notebook               = lookup_widget(window, "notebook1"); // 🔑 notebook

    if (!entry_id_membre_auth1 || !entry_mdp_membre_auth1 || !labelMessage || !notebook) {
        g_print("Erreur: widgets membre introuvables\n");
        return;
    }

    const gchar *id  = gtk_entry_get_text(GTK_ENTRY(entry_id_membre_auth1));
    const gchar *mdp = gtk_entry_get_text(GTK_ENTRY(entry_mdp_membre_auth1));

    if (!id || !mdp || strlen(id) == 0 || strlen(mdp) == 0) {
        gtk_label_set_text(GTK_LABEL(labelMessage),
                           "⚠️ Veuillez remplir ID et mot de passe.");
        return;
    }

    if (verifier_login("membre.txt", id, mdp)) {
        gtk_label_set_text(GTK_LABEL(labelMessage),
                           "✅ Membre authentifié !");

        // ✅ Aller à la page 4 du notebook
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 4);

    } else {
        gtk_label_set_text(GTK_LABEL(labelMessage),
                           "❌ Identifiant ou mot de passe incorrect.");
        gtk_entry_set_text(GTK_ENTRY(entry_mdp_membre_auth1), "");
    }
}
// ========== ENTRÉES ==========

void on_button_entree_mem_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1"); // ID du notebook

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
}

// ========== ENTRÉE ADMIN ==========
void on_button_entree_admin_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1"); // ID du notebook

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 2);
}

// ========== RECHERCHE / TREEVIEW ==========

void on_button_rechercher1_clicked(GtkWidget *button, gpointer user_data) {
    g_print("Bouton rechercher cliqué\n");
}




void on_ajout_cours1_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = gtk_widget_get_toplevel(button);

    GtkWidget *entry_coach_add1 = lookup_widget(window, "entry_coach_add1");
    GtkWidget *entry_date_add1  = lookup_widget(window, "entry_date_add1");
    GtkWidget *entry_heure_add1 = lookup_widget(window, "entry_heure_add1");
    GtkWidget *spin_centre_add1 = lookup_widget(window, "spin_centre_add1");
    GtkWidget *spin_places_add1 = lookup_widget(window, "spin_places_add1");
    GtkWidget *radio_homme_add1 = lookup_widget(window, "radio_homme_add1");
    GtkWidget *radio_femme_add1 = lookup_widget(window, "radio_femme_add1");
    GtkWidget *radio_mixte_add1 = lookup_widget(window, "radio_mixte_add1");
    GtkWidget *labelMessageAjout = lookup_widget(window, "labelMessageAjout");

    if (!entry_coach_add1 || !entry_date_add1 || !entry_heure_add1 ||
        !spin_centre_add1 || !spin_places_add1 ||
        !radio_homme_add1 || !radio_femme_add1 || !radio_mixte_add1 ||
        !labelMessageAjout)
    {
        g_print("Erreur: widgets ajout introuvables\n");
        return;
    }

    const gchar *coach = gtk_entry_get_text(GTK_ENTRY(entry_coach_add1));
    const gchar *date  = gtk_entry_get_text(GTK_ENTRY(entry_date_add1));
    const gchar *heure = gtk_entry_get_text(GTK_ENTRY(entry_heure_add1));
    int centre = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_centre_add1));
    int places = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_places_add1));

    // 🔎 Contrôle de saisie
    if (!coach || !date || !heure ||
        strlen(coach) == 0 || strlen(date) == 0 || strlen(heure) == 0)
    {
        gtk_label_set_text(GTK_LABEL(labelMessageAjout),
                           "⚠️ Veuillez remplir tous les champs.");
        return;
    }

    char type[20];
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_homme_add1)))
        strcpy(type, "Homme");
    else if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(radio_femme_add1)))
        strcpy(type, "Femme");
    else
        strcpy(type, "Mixte");

    FILE *f = fopen("cours2.txt", "a");
    if (!f) {
        gtk_label_set_text(GTK_LABEL(labelMessageAjout),
                           "❌ Erreur d'ouverture du fichier cours2.txt.");
        return;
    }

    int n = fprintf(f, "%s;%s;%s;%d;%d;%s\n",
                    coach, date, heure, centre, places, type);

    fflush(f);
    fclose(f);

    if (n > 0) {
        gtk_label_set_text(GTK_LABEL(labelMessageAjout),
                           "✅ Cours ajouté avec succès.");

        // Optionnel : vider les champs après ajout
        gtk_entry_set_text(GTK_ENTRY(entry_coach_add1), "");
        gtk_entry_set_text(GTK_ENTRY(entry_date_add1), "");
        gtk_entry_set_text(GTK_ENTRY(entry_heure_add1), "");
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_places_add1), 0);
    } else {
        gtk_label_set_text(GTK_LABEL(labelMessageAjout),
                           "❌ Erreur lors de l'écriture dans le fichier.");
    }
}

void
on_button_sinscrire_clicked            (GtkButton       *button,
                                        gpointer         user_data)
{GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1"); // ID du notebook

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 4);

}


void
on_button_annuler1_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1"); // ID du notebook

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);
}


void
on_button_annuler3_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{

    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1"); // ID du notebook

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);

}


void
on_button_annuler6_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{ 

    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1"); // ID du notebook

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);

}


void
on_button_annuler8_clicked             (GtkButton       *button,
                                        gpointer         user_data)
{ 

    GtkWidget *window   = gtk_widget_get_toplevel(button);
    GtkWidget *notebook = lookup_widget(window, "notebook1"); // ID du notebook

    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);

}

