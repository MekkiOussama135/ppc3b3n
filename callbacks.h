#ifndef CALLBACKS_H_INCLUDED
#define CALLBACKS_H_INCLUDED

#include <gtk/gtk.h>

/* Fenêtre coach */
void on_button2_valider_clicked(GtkWidget *button, gpointer user_data);
void on_alignment1_add(GtkContainer *container, GtkWidget *widget, gpointer user_data);
void on_button1_mdo_clicked(GtkWidget *button, gpointer user_data);
void on_button4_sinsc_clicked(GtkWidget *button, gpointer user_data);
void on_alignment2_add(GtkContainer *container, GtkWidget *widget, gpointer user_data);
void on_buttonreserver_coach_equipm_clicked(GtkWidget *button, gpointer user_data);
void on_s_inscrire_cours_clicked(GtkWidget *button, gpointer user_data);
void on_coach_reserver_equipm_clicked(GtkWidget *button, gpointer user_data);

/* Fenêtre admin */
void on_button4_supprimer_coach_clicked(GtkWidget *button, gpointer user_data);
void on_button3_ajouter_coach_clicked(GtkWidget *button, gpointer user_data);
void on_button5_modofier_coach_clicked(GtkWidget *button, gpointer user_data);
void on_button6_chercher_coach_clicked(GtkWidget *button, gpointer user_data);

/* Radio buttons */
void on_radiobutton1_1a_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton2_2a_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton3_3a_toggled(GtkToggleButton *togglebutton, gpointer user_data);

/* Check buttons (ajout) */
void on_checkbutton3_BP_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton2_BA_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton1_BC_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton4_BB_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton8_LD_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton7_zoum_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton6_yoga_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton5_Car_toggled(GtkToggleButton *togglebutton, gpointer user_data);

/* Boutons admin - CHANGEMENT ICI: GtkWidget au lieu de GtkButton */
void on_button1_enregistrer_A_clicked(GtkWidget *button, gpointer user_data);
void on_button3_supp_clicked(GtkWidget *button, gpointer user_data);

/* Radio buttons (modification) */
void on_radiobutton4_1ans_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton5_2ans_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_radiobutton6_3ans_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);

/* Check buttons (modification) */
void on_checkbutton13_C_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton16_LD_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton15_Z_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton14_Y_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton9_BC_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton10_BA_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton11_BP_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);
void on_checkbutton12_BB_M_toggled(GtkToggleButton *togglebutton, gpointer user_data);

/* Boutons modification */
void on_button_chercher_id_M_clicked(GtkWidget *button, gpointer user_data);
void on_button2_enregistrer_M_clicked(GtkWidget *button, gpointer user_data);
void on_button4_chercher_clicked(GtkWidget *button, gpointer user_data);

void refresh_treeview(GtkWidget *treeview);
void load_cours_into_treeview(GtkWidget *treeview);
void on_window_show(GtkWidget *widget, gpointer user_data);

#endif /* CALLBACKS_H_INCLUDED */
