#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <gtk/gtk.h>

// ==============================
// PROTOTYPES des callbacks
// ==============================
void
on_ajout_cours1_clicked                (GtkWidget       *button,
                                        gpointer         user_data);
// Cours & planning
void on_cours_selection_changed(GtkTreeSelection *selection, gpointer user_data);
int get_selected_cours_id(GtkWidget *treeview);
void load_lessons_into_treeview(GtkWidget *treeview);
void afficher_planning(GtkWidget *treeview, int id_cours, const char *type_programme);
void on_inscrirecours1_clicked(GtkWidget *button, gpointer user_data);

// Admin / Cours
void on_rechercheadmin1_clicked(GtkWidget *button, gpointer user_data);
void on_modifcoursadmin1_clicked(GtkWidget *button, gpointer user_data);
void on_supprimeradmin1_clicked(GtkWidget *button, gpointer user_data);
void on_ajoutadmin1_clicked(GtkWidget *button, gpointer user_data);

// Radios Ajout
void on_radio_homme_add1_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radio_femme_add1_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radio_mixte_add1_toggled(GtkToggleButton *togglebutton, gpointer user_data);

// Supprimer Cours
void on_supprimer_cours1_clicked(GtkWidget *button, gpointer user_data);

// Radios Modif
void on_radio_homme_modif1_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radio_femme_modif1_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radio_mixte_modif1_toggled(GtkToggleButton *togglebutton, gpointer user_data);

// Modifier Cours
void on_modifiercours1_clicked(GtkWidget *button, gpointer user_data);

// Identification Admin
void on_identidier1_clicked(GtkWidget *button, gpointer user_data);
void on_idenifier_membre1_clicked(GtkWidget *button, gpointer user_data);

// Entrée Buttons
void on_button_entree_mem_clicked(GtkWidget *button, gpointer user_data);
void on_button_entree_admin_clicked(GtkWidget *button, gpointer user_data);

// Recherche Buttons
void on_button_rechercher1_clicked(GtkWidget *button, gpointer user_data);
void on_treeview1_row_activated(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *column, gpointer user_data);
void on_button_retourner1_clicked(GtkWidget *button, gpointer user_data);

// Recherche par filtres
void on_button_rech_date1_clicked(GtkWidget *button, gpointer user_data);
void on_button_rech_type_clicked(GtkWidget *button, gpointer user_data);
void on_button_rech_coach1_clicked(GtkWidget *button, gpointer user_data);
void on_button_rech_id1_clicked(GtkWidget *button, gpointer user_data);
void on_button_cherchcoach_clicked(GtkWidget *button, gpointer user_data);
void on_button_rechdate_clicked(GtkWidget *button, gpointer user_data);
void on_button_rechtype_clicked(GtkWidget *button, gpointer user_data);

#endif // CALLBACKS_H




void
on_button_sinscrire_clicked            (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_annuler1_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_annuler3_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_annuler6_clicked             (GtkButton       *button,
                                        gpointer         user_data);

void
on_button_annuler8_clicked             (GtkButton       *button,
                                        gpointer         user_data);
