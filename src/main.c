/*
 * main.c
 *
 *  Created on: 2010-4-17
 *      Author: cai
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <unistd.h>
#include <pty.h>
#include <gtk/gtk.h>
#include <unique/unique.h>

#include "i18n.h"
#include "gl2tp.h"
#include "gl2tp_icon.h"
#include "l2tpclient.h"


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

static UniqueResponse active_instance(UniqueApp *app, gint command,
		UniqueMessageData *message_data, guint time_, gpointer user_data)
{

	gtk_window_present(GTK_WINDOW(user_data));
	return UNIQUE_RESPONSE_OK;
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
	g_set_application_name(_("gl2tp - GUI frontend for l2tp connection"));

	if(G_UNLIKELY(!gtk_init_with_args(&argc, &argv,PACKAGE_STRING,args,PACKAGE_NAME,&err)))
	{
		g_error("%s",err->message);
	}

	if(domain_dir)
	{
		bindtextdomain(GETTEXT_PACKAGE,domain_dir);
		g_free(domain_dir);
	}

	UniqueApp * unique = unique_app_new("net.vpn." PACKAGE_NAME,NULL);

	if (unique_app_is_running(unique))
	{
		g_message("already running!");

		if(UNIQUE_RESPONSE_OK != unique_app_send_message(unique,UNIQUE_ACTIVATE,NULL))
		{
			GtkWidget  * dlg = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_INFO,GTK_BUTTONS_CLOSE,
					_("Another one is running"));
			gtk_dialog_run(GTK_DIALOG(dlg));
		}else
		{
			g_message("active previous one");
		}
		return 1;
	}

	Gl2tpClient * l2client = gl2tpclient_new();

	if(!gl2tpclient_bind(l2client))
	{

		GtkWidget  * dlg = gtk_message_dialog_new(NULL,GTK_DIALOG_MODAL,GTK_MESSAGE_ERROR,GTK_BUTTONS_CLOSE,
				_("unable to bind to local port 1701? Another one running?"));
		gtk_dialog_run(GTK_DIALOG(dlg));
		g_error(_("unable to bind to local port 1701? Another one running?"));
	}


	GdkPixbuf	* pixbuf = gdk_pixbuf_new_from_inline(-1,gl2tp_icon,FALSE,NULL);

	GtkWidget * main = gtk_window_new(GTK_WINDOW_TOPLEVEL);

	g_signal_connect(unique,"message-received",G_CALLBACK(active_instance),main);

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
