#include <stdio.h>
#include <gtk/gtk.h>
#include <gnome.h>
#include <glade/glade.h>

GladeXML *glade;

int on_loginok_clicked( GtkButton *gbutton, gpointer userdata) {
    GtkWidget *window = glade_xml_get_widget( glade, "wlogin");
    GtkWidget *textentry = glade_xml_get_widget( glade, "wtextentry");
    GtkEntry *name = GTK_ENTRY(glade_xml_get_widget( glade, "name"));
    GtkEntry *password = GTK_ENTRY(glade_xml_get_widget( glade, "password"));
    GtkEntry *server = GTK_ENTRY(glade_xml_get_widget( glade, "server"));
    gchar* _name = gtk_entry_get_text( name);
    gchar* _password = gtk_entry_get_text( password);
    gchar* _server = gtk_entry_get_text( server);

    if (strlen( _name) > 0 && strlen( _password) > 0 && strlen( _server) > 0)
    {
        gtk_widget_hide( window);
    } else {
        gtk_widget_show( textentry);
    }

    printf("User %s, Password %s, Server %s\n", 
    	_name, _password, _server);
}

int main(int argc, char **argv) {
    gnome_init ("dr1", "1.0", argc, argv);
    glade_gnome_init();

    /* Read RC file */
    gtk_rc_parse( "dr1.rc");

    /* load the interface */
    glade = glade_xml_new("dr1.glade", NULL);
    /* connect the signals in the interface */
    glade_xml_signal_autoconnect(glade);
    /* start the event loop */
    gtk_main();
    return 0;
}
