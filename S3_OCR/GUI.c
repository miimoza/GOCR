#include <stdlib.h>

#include <gtk/gtk.h>
char * filename;
void getfilename(GtkFileChooser * chooser){
	filename = gtk_file_chooser_get_filename(chooser);
	g_print("%s",filename);
}

void setimage(GtkImage * visu){
	gtk_image_set_from_file(visu, filename);
}

int main(int argc, char **argv)

{
    /* Declaration of widgets */

    GtkWidget * MainWindow;
    //GtkWidget * label1;
    GtkWidget * lol;
    GtkButton * launch;
    //GtkWidget * VBox;
    //GtkWidget * res;
	GtkImage * visu;
	GtkFileChooser * chooser;
	GtkBuilder * builder;
	gchar * gladefile;
	GError * error = NULL;
    /* Initialization of GTK+ */

    gtk_init(&argc, &argv);
	builder = gtk_builder_new();
	gladefile = g_build_filename("ocr.glade", NULL);
	gtk_builder_add_from_file(builder, gladefile, &error);
	g_free(gladefile);
	if(error)
	{
		gint code = error->code;
		g_printerr("%s\n", error->message);
        g_error_free (error);
        return code;
	}
    /* Creation of window */
	MainWindow = GTK_WIDGET(gtk_builder_get_object(builder, "MainWindow"));
	//g_signal_connect(G_OBJECT(MainWindow), "destroy", (GCallback)gtk_main_quit, NULL);
	lol = GTK_WIDGET(gtk_builder_get_object(builder, "lol"));
	g_signal_connect(G_OBJECT(lol), "clicked", (GCallback)gtk_main_quit, NULL);
    chooser = GTK_FILE_CHOOSER(gtk_builder_get_object(builder, "chooser"));
	//filename = gtk_file_chooser_get_filename(chooser);
	visu = GTK_IMAGE(gtk_builder_get_object(builder, "visu"));
	//gtk_image_set_from_file(visu, gtk_file_chooser_get_filename(chooser));
	launch = GTK_BUTTON(gtk_builder_get_object(builder, "launch"));
	
	/* Displaying and event loop */
	gtk_widget_show_all (MainWindow);
	//filename = gtk_file_chooser_get_filename(chooser);
	g_signal_connect_swapped(G_OBJECT(launch), "clicked", (GCallback)getfilename, chooser);
	g_signal_connect_swapped(G_OBJECT(launch), "clicked", (GCallback)setimage, visu);
	//gtk_builder_connect_signals(builder, chooser);
    //gtk_widget_show_all(MainWindow);
    gtk_main();
    return EXIT_SUCCESS;

}
