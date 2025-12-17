#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include "coach.h"

// Variables globales
int specialite_selectionnee = 0;
int disponibilite_selectionnee = 1;
int choix_cours[8] = {0, 0, 0, 0, 0, 0, 0, 0}; // Pour les 8 cours

// ============================
// FENÊTRE COACH (c_b_gestion_coach_coach)
// ============================

void on_button2_valider_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_coach");
    GtkWidget *entry_id = lookup_widget(window, "entry1_id");
    GtkWidget *entry_mdp = lookup_widget(window, "entry2_md");
    
    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const char *mdp_text = gtk_entry_get_text(GTK_ENTRY(entry_mdp));
    
    int id = atoi(id_text);
    
    // Rechercher le coach
    Coach c = chercher_coach("coaches.txt", id);
    
    if(c.id != -1 && strcmp(c.motdepasse, mdp_text) == 0) {
        
        // Afficher un message de bienvenue
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Bienvenue %s %s!", c.nom, c.prenom);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "ID ou mot de passe incorrect!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_button1_mdo_clicked(GtkWidget *button, gpointer user_data)
{
    g_print("Fonction mot de passe oublié\n");
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_WARNING,
        GTK_BUTTONS_OK,
        "Contactez l'administrateur pour réinitialiser votre mot de passe.");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_button4_sinsc_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_coach");
    GtkWidget *treeview = lookup_widget(window, "treeview1");
    if(treeview!=NULL){
   refresh_treeview(treeview);
}
    g_print("Inscription à un cours\n");
    GtkWidget *lab = lookup_widget(window, "labelcal");
    int nomb=calc("cour.txt");
    gtk_label_set_text(GTK_LABEL(lab),g_strdup_printf("le nombre de cours est %d ",nomb));
    
    // Récupérer la sélection du treeview
    // TODO: Implémenter la logique de sélection de cours
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Inscription au cours enregistrée!");
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_buttonreserver_coach_equipm_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_coach");
    
    // Récupérer les widgets de la page réservation
    GtkWidget *combo_equip = lookup_widget(window, "combobox__quipmentC");
    GtkWidget *entry_qte = lookup_widget(window, "entryquantit___equipm_reserver");
    GtkWidget *spin_jour = lookup_widget(window, "spinbutton_jour");
    GtkWidget *spin_mois = lookup_widget(window, "spinbuttonmois");
    GtkWidget *spin_annee = lookup_widget(window, "spinbutton_anne");
    GtkWidget *spin_heure = lookup_widget(window, "spinbutton6");
    GtkWidget *spin_min = lookup_widget(window, "spinbutton7");
    
    // Récupérer les valeurs
    const char *equipement = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_equip));
    const char *quantite_text = gtk_entry_get_text(GTK_ENTRY(entry_qte));
    
    int jour = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_jour));
    int mois = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_mois));
    int annee = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_annee));
    int heure = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_heure));
    int minute = gtk_spin_button_get_value_as_int(GTK_SPIN_BUTTON(spin_min));
    int quantite = atoi(quantite_text);
    
    // Formater la date
    char date[50];
    sprintf(date, "%02d/%02d/%04d %02d:%02d", jour, mois, annee, heure, minute);
    
    g_print("Réservation: %s x%d pour le %s\n", equipement, quantite, date);
    
    // TODO: Appeler fonction reserver_equipement avec l'ID du coach connecté
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Équipement '%s' réservé pour le %s", equipement, date);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_s_inscrire_cours_clicked(GtkWidget *button, gpointer user_data)
{
    // Aller à l'onglet "Liste des cours"
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_coach");
    GtkWidget *notebook = lookup_widget(window, "notebook1");
    
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
    
    g_print("Navigation vers la liste des cours\n");
}

void on_coach_reserver_equipm_clicked(GtkWidget *button, gpointer user_data)
{
    // Aller à l'onglet "Réservation équipement"
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_coach");
    GtkWidget *notebook = lookup_widget(window, "notebook1");
    
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 2);
    
    g_print("Navigation vers la réservation d'équipement\n");
}

// ============================
// FENÊTRE ADMIN (c_b_gestion_coach_add)
// ============================

void on_button2_valider_add_clicked(GtkWidget *button, gpointer user_data)
{
    // Connexion administrateur
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    GtkWidget *entry_id = lookup_widget(window, "entry1_id");
    GtkWidget *entry_mdp = lookup_widget(window, "entry2_md");
    
    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    const char *mdp_text = gtk_entry_get_text(GTK_ENTRY(entry_mdp));
    
    // Vérification simple (à remplacer par une vraie vérification)
    if(strcmp(id_text, "admin") == 0 && strcmp(mdp_text, "admin123") == 0) {
        g_print("✅ Admin connecté\n");
        
        // Aller à l'onglet gestion des coaches
        GtkWidget *notebook = lookup_widget(window, "notebook2");
        gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 1);
    } else {
        g_print("❌ Identifiants admin incorrects\n");
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Identifiants administrateur incorrects!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

void on_button4_supprimer_coach_clicked(GtkWidget *button, gpointer user_data)
{
    // Aller à l'onglet suppression
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    GtkWidget *notebook = lookup_widget(window, "notebook2");
    
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 3);
    
    g_print("Navigation vers suppression coach\n");
}

void on_button3_ajouter_coach_clicked(GtkWidget *button, gpointer user_data)
{
    // Aller à l'onglet ajout
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    GtkWidget *notebook = lookup_widget(window, "notebook2");
    
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 2);
    
    g_print("Navigation vers ajout coach\n");
}

void on_button5_modofier_coach_clicked(GtkWidget *button, gpointer user_data)
{
    // Aller à l'onglet modification
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    GtkWidget *notebook = lookup_widget(window, "notebook2");
    
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 4);
    
    g_print("Navigation vers modification coach\n");
}

void on_button6_chercher_coach_clicked(GtkWidget *button, gpointer user_data)
{
    // Aller à l'onglet recherche
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    GtkWidget *notebook = lookup_widget(window, "notebook2");
    
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 5);
    
    g_print("Navigation vers recherche coach\n");
}

// ============================
// AJOUT DE COACH
// ============================

void on_radiobutton1_1a_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        specialite_selectionnee = 1;
        
    }
}

void on_radiobutton2_2a_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        specialite_selectionnee = 2;
       
    }
}

void on_radiobutton3_3a_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        specialite_selectionnee = 3;
        
    }
}

// Callbacks pour les cases à cocher (cours)
void on_checkbutton1_BC_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[0] = gtk_toggle_button_get_active(togglebutton);
    
}

void on_checkbutton2_BA_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[1] = gtk_toggle_button_get_active(togglebutton);
    
}

void on_checkbutton3_BP_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
   
}

void on_checkbutton4_BB_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[3] = gtk_toggle_button_get_active(togglebutton);
    
}

void on_checkbutton5_Car_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[4] = gtk_toggle_button_get_active(togglebutton);
    
}

void on_checkbutton6_yoga_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[5] = gtk_toggle_button_get_active(togglebutton);
    
}

void on_checkbutton7_zoum_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[6] = gtk_toggle_button_get_active(togglebutton);
    
}

void on_checkbutton8_LD_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[7] = gtk_toggle_button_get_active(togglebutton);
    
}

// ============================
// FONCTIONS AUXILIAIRES
// ============================



// Fonction pour construire la chaîne de spécialités
void construire_specialite(char *specialite_str)
{
    strcpy(specialite_str, "");
    
    const char *cours_noms[8] = {
        "Body combat", "Body Attack", "Body Pump", "Body Balance",
        "Cardio", "Yoga", "Zoumba", "Lesmiless Dance"
    };
    
    int premier = 1;
    for(int i = 0; i < 8; i++) {
        if(choix_cours[i]) {
            if(!premier) {
                strcat(specialite_str, ", ");
            }
            strcat(specialite_str, cours_noms[i]);
            premier = 0;
        }
    }
    
    if(strlen(specialite_str) == 0) {
        strcpy(specialite_str, "Aucune spécialité");
    }
}

// Fonction pour initialiser les checkboxes (optionnel)
void initialiser_checkboxes()
{
    for(int i = 0; i < 8; i++) {
        choix_cours[i] = 0;
    }
}

void on_button1_enregistrer_A_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    
    // Récupérer tous les widgets
    GtkWidget *entry_nom = lookup_widget(window, "entry1_nom");
    GtkWidget *entry_prenom = lookup_widget(window, "entry2_pren");
    GtkWidget *calendar = lookup_widget(window, "calendar1_dn");
    GtkWidget *entry_tel = lookup_widget(window, "entry3_numtel");
    GtkWidget *combo_salle = lookup_widget(window, "comboboxentry3");
    GtkWidget *spin_capacite = lookup_widget(window, "spinbutton1_cap");
    GtkWidget *entry_mdp = lookup_widget(window, "entry4_md");
    GtkWidget *entry_id = lookup_widget(window, "entry5_id");
    
    // Validation des champs obligatoires
    const char *nom = gtk_entry_get_text(GTK_ENTRY(entry_nom));
    const char *prenom = gtk_entry_get_text(GTK_ENTRY(entry_prenom));
    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    
    if(strlen(nom) == 0 || strlen(prenom) == 0 || strlen(id_text) == 0) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Veuillez remplir tous les champs obligatoires!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Créer la structure Coach
    Coach c;
    c.id = atoi(id_text);
    strcpy(c.nom, nom);
    strcpy(c.prenom, prenom);
    strcpy(c.telephone, gtk_entry_get_text(GTK_ENTRY(entry_tel)));
    strcpy(c.motdepasse, gtk_entry_get_text(GTK_ENTRY(entry_mdp)));
    
    // Date de naissance depuis le calendrier
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    sprintf(c.date_naissance, "%02d/%02d/%04d", day, month + 1, year);
    
    // Salle de sport depuis combo
    const char *salle = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_salle));
    if(salle != NULL) {
        strcpy(c.salle_sport, salle);
    } else {
        strcpy(c.salle_sport, "Non spécifié");
    }
    
    // Construire la spécialité
    char specialite_str[200];
    construire_specialite(specialite_str);
    strcpy(c.specialite, specialite_str);
    
    // Disponibilité
    c.disponibilite = disponibilite_selectionnee;
    
    // Ajouter le coach
    if(ajouter_coach("coaches.txt", c)) {
        g_print("✅ Coach ajouté: ID=%d, Nom=%s, Prénom=%s\n", c.id, c.nom, c.prenom);
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Coach %s %s ajouté avec succès!\nID: %d\nSpécialités: %s",
            c.nom, c.prenom, c.id, c.specialite);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        
        // Réinitialiser le formulaire
        gtk_entry_set_text(GTK_ENTRY(entry_nom), "");
        gtk_entry_set_text(GTK_ENTRY(entry_prenom), "");
        gtk_entry_set_text(GTK_ENTRY(entry_tel), "");
        gtk_entry_set_text(GTK_ENTRY(entry_mdp), "");
        gtk_entry_set_text(GTK_ENTRY(entry_id), "");
        
        
    } else {
        g_print("❌ Erreur lors de l'ajout du coach\n");
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Erreur lors de l'ajout du coach!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ============================
// SUPPRIMER DE COACH
// ============================


void on_button3_supp_clicked(GtkWidget *button, gpointer user_data) {
   
  GtkWidget *w=lookup_widget(button,"c_b_gestion_coach_add");
  int id=atoi(gtk_entry_get_text(GTK_ENTRY(lookup_widget(w,"entry11_id"))));


  if(supprimer_coach("coaches.txt",id)){
    GtkWidget*dlg=gtk_message_dialog_new(NULL,GTK_DIALOG_DESTROY_WITH_PARENT,GTK_MESSAGE_INFO,GTK_BUTTONS_OK,"Coach supprimé avec succès");
    gtk_dialog_run(GTK_DIALOG(dlg)); gtk_widget_destroy(dlg);
}
}
// ============================
// MODIFICATION DE COACH
// ============================

void on_button_chercher_id_M_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    GtkWidget *entry_id_recherche = lookup_widget(window, "entryid_modifer");
    
    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id_recherche));
    
    if(strlen(id_text) == 0) {
        g_print("❌ ID vide\n");
        return;
    }
    
    int id = atoi(id_text);
    
    // Rechercher le coach
    Coach c = chercher_coach("coaches.txt", id);
    
    if(c.id == -1) {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Coach avec ID %d non trouvé!", id);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    g_print("✅ Coach trouvé: %s %s\n", c.nom, c.prenom);
    
    // Remplir le formulaire avec les données du coach
    GtkWidget *entry_nom = lookup_widget(window, "entry6_nom");
    GtkWidget *entry_prenom = lookup_widget(window, "entry7_prenom");
    GtkWidget *calendar = lookup_widget(window, "calendar2");
    GtkWidget *entry_tel = lookup_widget(window, "entry8_numtel");
    GtkWidget *combo_salle = lookup_widget(window, "comboboxentry2");
    GtkWidget *entry_mdp = lookup_widget(window, "entry9_md_M");
    GtkWidget *entry_id_form = lookup_widget(window, "entry10_id_M");
    
    // Remplir les champs
    gtk_entry_set_text(GTK_ENTRY(entry_nom), c.nom);
    gtk_entry_set_text(GTK_ENTRY(entry_prenom), c.prenom);
    gtk_entry_set_text(GTK_ENTRY(entry_tel), c.telephone);
    gtk_entry_set_text(GTK_ENTRY(entry_mdp), c.motdepasse);
    gtk_entry_set_text(GTK_ENTRY(entry_id_form), id_text);
    
    // Date de naissance
    int jour, mois, annee;
    sscanf(c.date_naissance, "%d/%d/%d", &jour, &mois, &annee);
    gtk_calendar_select_month(GTK_CALENDAR(calendar), mois - 1, annee);
    gtk_calendar_select_day(GTK_CALENDAR(calendar), jour);
    
    // TODO: Remplir les checkboxes de spécialités
    // TODO: Remplir le combobox de salle
    
    // Message de confirmation
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "Coach %s %s chargé. Modifiez les champs puis cliquez sur Enregistrer.",
        c.nom, c.prenom);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

void on_button2_enregistrer_M_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    
    // Récupérer l'ID original (celui utilisé pour la recherche)
    GtkWidget *entry_id_recherche = lookup_widget(window, "entryid_modifer");
    const char *id_original_text = gtk_entry_get_text(GTK_ENTRY(entry_id_recherche));
    
    if(strlen(id_original_text) == 0) {
        g_print("❌ Aucun coach chargé pour modification\n");
        return;
    }
    
    int id_original = atoi(id_original_text);
    
    // Récupérer les données du formulaire
    GtkWidget *entry_nom = lookup_widget(window, "entry6_nom");
    GtkWidget *entry_prenom = lookup_widget(window, "entry7_prenom");
    GtkWidget *calendar = lookup_widget(window, "calendar2");
    GtkWidget *entry_tel = lookup_widget(window, "entry8_numtel");
    GtkWidget *combo_salle = lookup_widget(window, "comboboxentry2");
    GtkWidget *entry_mdp = lookup_widget(window, "entry9_md_M");
    GtkWidget *entry_id_form = lookup_widget(window, "entry10_id_M");
    
    // Créer la nouvelle structure
    Coach nouv;
    nouv.id = atoi(gtk_entry_get_text(GTK_ENTRY(entry_id_form)));
    strcpy(nouv.nom, gtk_entry_get_text(GTK_ENTRY(entry_nom)));
    strcpy(nouv.prenom, gtk_entry_get_text(GTK_ENTRY(entry_prenom)));
    strcpy(nouv.telephone, gtk_entry_get_text(GTK_ENTRY(entry_tel)));
    strcpy(nouv.motdepasse, gtk_entry_get_text(GTK_ENTRY(entry_mdp)));
    
    // Date
    guint year, month, day;
    gtk_calendar_get_date(GTK_CALENDAR(calendar), &year, &month, &day);
    sprintf(nouv.date_naissance, "%02d/%02d/%04d", day, month + 1, year);
    
    // Salle
    const char *salle = gtk_combo_box_get_active_text(GTK_COMBO_BOX(combo_salle));
    if(salle != NULL) {
        strcpy(nouv.salle_sport, salle);
    }
    
    // Spécialités
    char specialite_str[200];
    construire_specialite(specialite_str);
    strcpy(nouv.specialite, specialite_str);
    
    // Disponibilité
    nouv.disponibilite = 1;
    
    // Modifier le coach
    if(modifier_coach("coaches.txt", id_original, nouv)) {
       
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_INFO,
            GTK_BUTTONS_OK,
            "Coach modifié avec succès!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    } else {
        g_print("❌ Erreur modification\n");
        
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Erreur lors de la modification!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
    }
}

// ============================
// RECHERCHE DE COACH
// ============================

void on_button4_chercher_clicked(GtkWidget *button, gpointer user_data)
{
    GtkWidget *window = lookup_widget(button, "c_b_gestion_coach_add");
    GtkWidget *entry_id = lookup_widget(window, "entry12_id");
    
    const char *id_text = gtk_entry_get_text(GTK_ENTRY(entry_id));
    
    if(strlen(id_text) == 0)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_WARNING,
            GTK_BUTTONS_OK,
            "Veuillez entrer un ID!");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    int id = atoi(id_text);
    
    // Rechercher le coach
    Coach c = chercher_coach("coaches.txt", id);
    
    if(c.id == -1)
    {
        GtkWidget *dialog = gtk_message_dialog_new(NULL,
            GTK_DIALOG_DESTROY_WITH_PARENT,
            GTK_MESSAGE_ERROR,
            GTK_BUTTONS_OK,
            "Coach avec ID %d non trouvé!", id);
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Afficher les informations - CORRECTION ICI: déclarer message
    char message[500];  
    sprintf(message,
        "=== INFORMATIONS COACH ===\n"
        "ID: %d\n"
        "Nom: %s\n"
        "Prénom: %s\n"
        "Date naissance: %s\n"
        "Téléphone: %s\n"
        "Salle: %s\n"
        "Spécialités: %s\n"
        "Disponibilité: %s\n"
        "Mot de passe: %s",
        c.id, c.nom, c.prenom, c.date_naissance,
        c.telephone, c.salle_sport, c.specialite,
        c.disponibilite ? "Disponible" : "Non disponible",
        c.motdepasse);  
    
    GtkWidget *dialog = gtk_message_dialog_new(NULL,
        GTK_DIALOG_DESTROY_WITH_PARENT,
        GTK_MESSAGE_INFO,
        GTK_BUTTONS_OK,
        "%s", message);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
    
    g_print("Coach trouvé \n");
}
// ============================
// CALBACKS POUR MODIFICATION 
// ============================

void on_radiobutton4_1ans_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        specialite_selectionnee = 1;
        g_print("Contrat 1 an sélectionné (modif)\n");
    }
}

void on_radiobutton5_2ans_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        specialite_selectionnee = 2;
        g_print("Contrat 2 ans sélectionné (modif)\n");
    }
}

void on_radiobutton6_3ans_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    if(gtk_toggle_button_get_active(togglebutton)) {
        specialite_selectionnee = 3;
        g_print("Contrat 3 ans sélectionné (modif)\n");
    }
}

void on_checkbutton9_BC_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[0] = gtk_toggle_button_get_active(togglebutton);
    g_print("Body combat (modif): %s\n", choix_cours[0] ? "Oui" : "Non");
}

void on_checkbutton10_BA_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[1] = gtk_toggle_button_get_active(togglebutton);
    g_print("Body Attack (modif): %s\n", choix_cours[1] ? "Oui" : "Non");
}

void on_checkbutton11_BP_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[2] = gtk_toggle_button_get_active(togglebutton);
    g_print("Body Pump (modif): %s\n", choix_cours[2] ? "Oui" : "Non");
}

void on_checkbutton12_BB_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[3] = gtk_toggle_button_get_active(togglebutton);
    g_print("Body Balance (modif): %s\n", choix_cours[3] ? "Oui" : "Non");
}

void on_checkbutton13_C_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[4] = gtk_toggle_button_get_active(togglebutton);
    g_print("Cardio (modif): %s\n", choix_cours[4] ? "Oui" : "Non");
}

void on_checkbutton14_Y_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[5] = gtk_toggle_button_get_active(togglebutton);
    g_print("Yoga (modif): %s\n", choix_cours[5] ? "Oui" : "Non");
}

void on_checkbutton15_Z_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[6] = gtk_toggle_button_get_active(togglebutton);
    g_print("Zoumba (modif): %s\n", choix_cours[6] ? "Oui" : "Non");
}

void on_checkbutton16_LD_M_toggled(GtkToggleButton *togglebutton, gpointer user_data)
{
    choix_cours[7] = gtk_toggle_button_get_active(togglebutton);
    g_print("Lesmiless Dance (modif): %s\n", choix_cours[7] ? "Oui" : "Non");
}


void load_cours_into_treeview(GtkWidget *treeview) {
    GtkListStore *store;
    GtkTreeIter iter;

    char line[256];
    char  nom[100], date[100], heure[100], salle[100];
   

   
    if (treeview == NULL) {
        g_print("Erreur : TreeView non trouvé\n");
        return;
    }


    store = gtk_list_store_new(4,
                               G_TYPE_STRING,  
                               G_TYPE_STRING,  
                               G_TYPE_STRING,  
                               G_TYPE_STRING  
                               );    

   
    gtk_tree_view_set_model(GTK_TREE_VIEW(treeview), GTK_TREE_MODEL(store));
   
   
    GList *columns = gtk_tree_view_get_columns(GTK_TREE_VIEW(treeview));
    GList *col;
    for (col = columns; col != NULL; col = col->next) {
        gtk_tree_view_remove_column(GTK_TREE_VIEW(treeview), GTK_TREE_VIEW_COLUMN(col->data));
    }
    g_list_free(columns);

   
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;

   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("nom", renderer, "text", 0, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("date", renderer, "text", 1, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("heure", renderer, "text", 2, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

   
    renderer = gtk_cell_renderer_text_new();
    column = gtk_tree_view_column_new_with_attributes("salle", renderer, "text", 3, NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(treeview), column);

   
   
    FILE *f = fopen("cour.txt", "r");
    if (!f) {
        g_print("Aucun cours trouvé. Le fichier sera créé lors du premier ajout.\n");
        g_object_unref(store);
        return;
    }

    while (fgets(line, sizeof(line), f)) {
       
        line[strcspn(line, "\n")] = 0;
       
       
        if (sscanf(line, "%s %s %s %s ",
                   nom, date,heure,salle) == 4) {

            gtk_list_store_append(store, &iter);
            gtk_list_store_set(store, &iter,
                               0, nom,
                               1, date,
                               2, heure,
                               3, salle,
                               
                               -1);
        }
    }

    fclose(f);
    g_object_unref(store);
}



void on_window_show(GtkWidget *widget, gpointer user_data)
{
    GtkWidget *treeview_member;
   
    treeview_member = lookup_widget(widget, "treeview1");
    if (treeview_member != NULL) {
        load_cours_into_treeview(treeview_member);
    }
}


void refresh_treeview(GtkWidget *treeview)
{
    GtkTreeModel *model;
   
    model = gtk_tree_view_get_model(GTK_TREE_VIEW(treeview));
    if (model != NULL) {
        gtk_list_store_clear(GTK_LIST_STORE(model));
    }
   
    load_cours_into_treeview(treeview);
}
