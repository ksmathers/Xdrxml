#include <gtk/gtk.h>
#include <glade/glade.h>

#include "dr1.h"
#include "player.h"
#include "common.h"
#include "text.h"
#include "race.h"


void
cgenerate_setErrorStatus( char *msg) {
    GtkLabel *lerror = GTK_LABEL( glade_xml_get_widget( common.glade, "errorstatus"));
    gtk_label_set_text( lerror, msg);
}

void
cgenerate_setPlayer( dr1Player *p) {
    char buf[30];
    GtkWidget *wgenerate = glade_xml_get_widget( common.glade, "wgenerate");

    GtkLabel *lname = GTK_LABEL( glade_xml_get_widget( common.glade, "cgname"));
    GtkLabel *lrace = GTK_LABEL( glade_xml_get_widget( common.glade, "race"));
    GtkLabel *lclass = GTK_LABEL( glade_xml_get_widget( common.glade, "class"));
    GtkLabel *lsex = GTK_LABEL( glade_xml_get_widget( common.glade, "sex"));
    GtkLabel *lhits = GTK_LABEL( glade_xml_get_widget( common.glade, "hits"));
    GtkLabel *lgold = GTK_LABEL( glade_xml_get_widget( common.glade, "gold"));
    GtkLabel *lstr = GTK_LABEL( glade_xml_get_widget( common.glade, "_str"));
    GtkLabel *lint = GTK_LABEL( glade_xml_get_widget( common.glade, "_int"));
    GtkLabel *lwis = GTK_LABEL( glade_xml_get_widget( common.glade, "_wis"));
    GtkLabel *ldex = GTK_LABEL( glade_xml_get_widget( common.glade, "_dex"));
    GtkLabel *lcon = GTK_LABEL( glade_xml_get_widget( common.glade, "_con"));
    GtkLabel *lcha = GTK_LABEL( glade_xml_get_widget( common.glade, "_cha"));

    gtk_label_set_text( lname, p->name);
    gtk_label_set_text( lrace, race_name[ p->race]);
    gtk_label_set_text( lclass, class_name[ p->class]);
    gtk_label_set_text( lsex, p->sex == DR1R_MALE ? "Male":"Female");
    if ( p->hp > 0 ) {
	sprintf(buf, "%d", p->hp);
	gtk_label_set_text( lhits, buf);
    } else {
	gtk_label_set_text( lhits, "invalid");
    }
    sprintf(buf, "%ld", dr1Money_value( &p->purse, 0) / 200);
    gtk_label_set_text( lgold, buf);
    sprintf(buf, "%d", p->base_attr._str);
    gtk_label_set_text( lstr, buf);
    sprintf(buf, "%d", p->base_attr._int);
    gtk_label_set_text( lint, buf);
    sprintf(buf, "%d", p->base_attr._wis);
    gtk_label_set_text( lwis, buf);
    sprintf(buf, "%d", p->base_attr._dex);
    gtk_label_set_text( ldex, buf);
    sprintf(buf, "%d", p->base_attr._con);
    gtk_label_set_text( lcon, buf);
    sprintf(buf, "%d", p->base_attr._cha);
    gtk_label_set_text( lcha, buf);
    gtk_widget_show( wgenerate);
}

/* CLOSE DIALOG */
void on_cgcancel_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_CANCEL);
}

void on_cgdone_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_DONE);
}

/* STATS */
void on_reroll_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_ROLL);
}

void on_swap_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_ROLL);
}

void on_train_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_ROLL);
}

/* NAME */
gboolean on_changename_focus_out_event (
	GtkWidget *widget,
        GdkEventFocus *event, 
	gpointer user_data) 
{
    char *name;
    GtkEntry *newname = GTK_ENTRY( widget);

    cgenerate_setErrorStatus("");
    name = gtk_entry_get_text( newname);
    assert( strlen(name) < 20);
    psendMessage( &common.ios, DR1CMD_NAME, name);
    return 0;
}

/* SEXES */
void on_sexmale_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_SEX, "male");
}

void on_sexfemale_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_SEX, "female");
}

/* CLASSES */
void on_classfighter_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_CLASS, "fighter");
}

void on_classmage_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_CLASS, "mu");
}

void on_classcleric_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_CLASS, "cleric");
}

void on_classthief_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_CLASS, "thief");
}

/* RACES */
void on_raceelf_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_RACE, "elf");
}

void on_racehalfelf_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_RACE, "half-elf");
}

void on_racehalforc_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_RACE, "half-orc");
}

void on_racehuman_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_RACE, "human");
}

void on_racehobbit_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_RACE, "hobbit");
}

void on_racedwarf_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_RACE, "dwarf");
}

void on_racegnome_clicked( GtkButton *gbutton, gpointer userdata) {
    cgenerate_setErrorStatus("");
    psendMessage( &common.ios, DR1CMD_RACE, "gnome");
}
