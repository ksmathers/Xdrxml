/*
**  default GNOME application
**
*/


/********************** INCLUDE **********************/

#include <gnome.h>

#include "gtksdl.h"

gint LoadLog(char *filename);


/********************** DEFINE **********************/

#define APPTITLE	"You Know"
#define APPVERSION	"0.1"
#define APPAUTHOR	"Derethor"

/********************** GLOBALS **********************/

GtkWidget *appGlobal;		/* Main Application Handle */

/********************** CALLBACKS **********************/

static void
cbFileOpenOK (GtkWidget *w, GtkFileSelection *dlgFile)
{
  char fn[255];
  
  	strncpy( fn, gtk_file_selection_get_filename( GTK_FILE_SELECTION(dlgFile)), 254);
	fn[255] = '\0';
	gtk_widget_destroy( GTK_WIDGET(dlgFile) );
	if (LoadLog(fn) == FALSE) g_print("Error showing file");
}

static void
cbOpenLog (GtkWidget *w, gpointer data)
{

  GtkWidget *dlgFile = NULL;

	dlgFile = gtk_file_selection_new ("Open a log");
	gtk_window_set_modal ( GTK_WINDOW(dlgFile), TRUE );
	
	gtk_signal_connect ( GTK_OBJECT( GTK_FILE_SELECTION(dlgFile)->ok_button ),
			     "clicked",
			     (GtkSignalFunc) cbFileOpenOK,
			     dlgFile );
	
	gtk_widget_show (dlgFile);

}

static void
cbAbout (GtkWidget *w, gpointer data)
{
 GtkWidget *dlg;
 char *authors[] = {APPAUTHOR, NULL};
 
 	dlg = gnome_about_new ( APPTITLE,
				APPVERSION,
				"(!c) 2000",
				(const gchar **)authors,
				NULL,
				NULL);

	gnome_dialog_set_parent ( GNOME_DIALOG(dlg), GTK_WINDOW(appGlobal) );

	gtk_widget_show(dlg);
}

/* Free the global allocated data and finish the main loop */
static void cbAppDestroy(void)
{
	/* Close SDL (prevents the ticks to continue running) */
	SDL_Quit ();

	/* End with the GTK*/
	gtk_main_quit ();
}

/********************** MENU DEFINITIONS **********************/

GnomeUIInfo file_menu[] = {
	GNOMEUIINFO_MENU_OPEN_ITEM(cbOpenLog,NULL),
	GNOMEUIINFO_MENU_CLOSE_ITEM(gtk_main_quit,NULL),
	GNOMEUIINFO_MENU_EXIT_ITEM(gtk_main_quit,NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo edit_menu[] = {
	GNOMEUIINFO_MENU_FIND_ITEM(gtk_main_quit,NULL),
	GNOMEUIINFO_MENU_FIND_AGAIN_ITEM(gtk_main_quit,NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo settings_menu[] = {
	GNOMEUIINFO_MENU_PREFERENCES_ITEM(gtk_main_quit,NULL),
	GNOMEUIINFO_END
};

GnomeUIInfo help_menu[] = {
         GNOMEUIINFO_MENU_ABOUT_ITEM(cbAbout, NULL),
         GNOMEUIINFO_END
};


GnomeUIInfo menubar[] = {
	GNOMEUIINFO_MENU_FILE_TREE(file_menu),
	GNOMEUIINFO_MENU_EDIT_TREE (edit_menu),
	GNOMEUIINFO_MENU_SETTINGS_TREE (settings_menu),
	GNOMEUIINFO_MENU_HELP_TREE (help_menu),
	GNOMEUIINFO_END
};

/********************** TOOLBAR DEFINITIONS **********************/

GnomeUIInfo toolbar[] = {
         GNOMEUIINFO_ITEM_STOCK("Open","Open a new log",
                                cbOpenLog,
                                GNOME_STOCK_PIXMAP_OPEN),
				
         GNOMEUIINFO_ITEM_STOCK("Refresh","reload the log",
                                gtk_main_quit,
                                GNOME_STOCK_PIXMAP_REFRESH),
				
         GNOMEUIINFO_ITEM_STOCK("Search","Search for text",
                                gtk_main_quit,
                                GNOME_STOCK_PIXMAP_SEARCH),
				
         GNOMEUIINFO_ITEM_STOCK("Exit","Exit the application",
                                gtk_main_quit,
                                GNOME_STOCK_PIXMAP_EXIT),
         GNOMEUIINFO_END
};

/********************** PROCEDURES **********************/

void banner(void)
{
	g_print (APPTITLE " " APPVERSION " (" __DATE__ ")\n\n");
	g_print ("Written by Javier Loureiro Varela <derethor@poves.com> \n");
}


gint LoadLog(char *filename)
{
  GtkText *text = NULL;
  FILE *log;
  char buffer[1024];
  int nchars;

	/* Load the GtkText */
	text = GTK_TEXT(GNOME_APP(appGlobal)->contents);
	if (text==NULL) return(FALSE);
	gtk_text_set_point( text, 0);

	gtk_text_freeze ( text);
	log = fopen(filename, "r");
	if (log)
	{
		while (1)
		{
			nchars = fread(buffer, 1, 1024, log);
			gtk_text_insert ( text, NULL, NULL, NULL, buffer, nchars );
			if (nchars < 1024) break;
		}
		fclose (log);
	}
	else return(FALSE);

	gtk_text_thaw ( text);

	return(TRUE);

}

/********************** APP PROCEDURES **********************/

gint AppInit(int argc, char *argv[])
{

	/* show some banner */
	banner();

	/* Init all gnome enviroment */
	if (gnome_init (APPTITLE, APPVERSION, argc, argv) != 0) return(FALSE);

	/* Create a Gnome app widget, which sets up a basic window for your application */
	appGlobal = gnome_app_new (APPTITLE, APPTITLE);
	if (appGlobal == NULL) return(FALSE);

	/* allows the user to close the app */
	gtk_signal_connect (GTK_OBJECT (appGlobal), "delete_event",
        		GTK_SIGNAL_FUNC (cbAppDestroy), NULL);
	gtk_signal_connect (GTK_OBJECT (appGlobal), "destroy",
        		GTK_SIGNAL_FUNC (cbAppDestroy), NULL);

	/* Initialise the SDL library */
	if ( SDL_InitSubSystem ( SDL_INIT_VIDEO) < 0)
	{
		fprintf (stderr, "Unable to init SDL: %s", SDL_GetError() );
		return (FALSE);
	}

	return(TRUE);
}

gint AppContents(void )
{
 GtkWidget *w;

	/* Text display */
	w = gtk_sdl_new (320, 200, 0, SDL_HWSURFACE | SDL_OPENGL);

	/* Set the table as the main content of the application */
	gnome_app_set_contents (GNOME_APP (appGlobal), w);

	return(TRUE);
}

gint AppMenus(void )
{
	/*create the menus for the application*/
	gnome_app_create_menus (GNOME_APP (appGlobal), menubar);
	
	return(TRUE);
}

gint AppToolBar(void )
{
	/*create the toolbar for the application*/
	gnome_app_create_toolbar (GNOME_APP (appGlobal), toolbar);
	return(TRUE);
}

gint AppStatusBar(void )
{
  GtkWidget *w;

	/*create the statusbar for the application*/
	w = gnome_appbar_new (TRUE, TRUE, GNOME_PREFERENCES_USER);
	gnome_app_set_statusbar ( GNOME_APP(appGlobal), w);
	gnome_app_install_menu_hints (GNOME_APP(appGlobal), menubar);
	gnome_appbar_set_default ( GNOME_APPBAR(w), "Done");
	
	return(TRUE);
}


gint AppShow(void )
{
        /* show everything inside this app widget and the app widget itself */
	gtk_widget_show_all (appGlobal);
	return(TRUE);
}

gint AppCreate(void )
{

	if ( !AppContents() )
	{
		g_print("\n[!] Contents NOT created\n");
		return(FALSE);
	}

	if ( !AppMenus() )
	{
		g_print("\n[!] Menus NOT created\n");
		return(FALSE);
	}

	if ( !AppToolBar() )
	{
		g_print("\n[!] ToolBar NOT created\n");
		return(FALSE);
	}

	if ( !AppStatusBar() )
	{
		g_print("\n[!] StatusBar NOT created\n");
		return(FALSE);
	}


	if ( !AppShow() )
	{
		g_print("\n[!] Error showing app contents\n");
		return(-1);
	}


	return(TRUE);

}

gint AppMainLoop(void )
{
	/* enter the main loop */
	gtk_main ();
	return(TRUE);
}

/********************** MAIN **********************/

int main(int argc, char *argv[])
{

	/* Init the Aplication enviroment*/
	if ( !AppInit(argc, argv) )
	{
		g_print("\n[!] NOT initialised\n");
		return(-1);
	}

	/* Create the main windows, and add the widgets */
	if ( !AppCreate() )
	{
		g_print("\n[!] contents NOT created\n");
		return(-1);
	}

	/* Enter in the event handling main loop */
	if ( !AppMainLoop() )
	{
		g_print("\n[!] Error entering app mail loop\n");
		return(-1);
	}

	/* See you later! */
	return(0);
}
