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
#include "i18n.h"
#include "gl2tp.h"
#include "gl2tp_icon.h"

GtkWidget * status;

static void show_status(GtkWidget* widget,const char * str, gpointer user_data)
{
	guint id = GPOINTER_TO_UINT(user_data);
	gtk_statusbar_push((GtkStatusbar*)status,id,str);
}

static void pop_status(GtkWidget* widget,gpointer user_data)
{
	guint id = GPOINTER_TO_UINT(user_data);
	gtk_statusbar_pop((GtkStatusbar*)status,id);
}


int main(int argc, char * argv[], char * env[])
{
	setlocale(LC_ALL,"");
	gtk_set_locale();
	textdomain(GETTEXT_PACKAGE);

	GError * err = NULL;
	gchar * domain_dir=NULL;

	GOptionEntry args[] =
	{
			{"locale",'\0',0,G_OPTION_ARG_STRING,&domain_dir,_("set domain dir root"),N_("dir")},
			{0}
	};

	g_set_prgname(PACKAGE_NAME);

	if(G_UNLIKELY(!gtk_init_with_args(&argc, &argv,PACKAGE_STRING,args,PACKAGE_NAME,&err)))
	{
		g_error("%s",err->message);
	}

	if(domain_dir)
	{
		bindtextdomain(GETTEXT_PACKAGE,domain_dir);
		g_free(domain_dir);
	}

	GdkPixbuf	* pixbuf = gdk_pixbuf_new_from_inline(-1,gl2tp_icon,FALSE,NULL);

	GtkWidget * main = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	gtk_window_set_icon(GTK_WINDOW(main),pixbuf);

	GtkWidget * box = gtk_vbox_new(FALSE,TRUE);

	gtk_container_add(GTK_CONTAINER(main),box);

	g_signal_connect(G_OBJECT (main), "destroy", G_CALLBACK(gtk_main_quit), NULL);

	GtkWidget * gl2tp = gl2tp_new();

	gtk_box_pack_start(GTK_BOX(box),gl2tp,TRUE,TRUE,FALSE);

	status =  gtk_statusbar_new();

	guint id = gtk_statusbar_get_context_id(GTK_STATUSBAR(status),"status");

	gtk_box_pack_end(GTK_BOX(box),status,FALSE,TRUE,TRUE);

	gtk_statusbar_push(GTK_STATUSBAR(status),id,_("Ready"));

	g_signal_connect(gl2tp,"status-changed",G_CALLBACK(show_status),GUINT_TO_POINTER(id));
	g_signal_connect(gl2tp,"status-restore",G_CALLBACK(pop_status),GUINT_TO_POINTER(id));

	gtk_widget_show_all(main);
	gtk_main();
	return 0;
}
