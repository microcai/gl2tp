/*
 * main.c
 *
 *  Created on: 2010-4-17
 *      Author: cai
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gtk/gtk.h>
#include <i18n.h>
#include "gl2tp.h"
#include "gl2tp_icon.h"


int main(int argc, char * argv[], char * env[])
{
	gtk_init(&argc,&argv);

	GdkPixbuf	* pixbuf = gdk_pixbuf_new_from_inline(-1,gl2tp_icon,FALSE,NULL);

	GtkWidget * main = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_icon(GTK_WINDOW(main),pixbuf);

	GtkWidget * box = gtk_vbox_new(FALSE,TRUE);

	gtk_container_add(GTK_CONTAINER(main),box);

	g_signal_connect(G_OBJECT (main), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget * gl2tp = gl2tp_new();

	gtk_box_pack_start(GTK_BOX(box),gl2tp,TRUE,TRUE,FALSE);

	GtkWidget * status =  gtk_statusbar_new();

	guint id = gtk_statusbar_get_context_id(GTK_STATUSBAR(status),"status");

	gtk_box_pack_end(GTK_BOX(box),status,FALSE,TRUE,TRUE);

	gtk_statusbar_push(GTK_STATUSBAR(status),id,_("Ready"));

	gtk_widget_show_all(main);
	gtk_main();
	return 0;
}
