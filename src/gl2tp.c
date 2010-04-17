/*
 * gl2tp.c
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

static void gl2tp_class_init(Gl2tpClass*);
static void gl2tp_init(Gl2tp*);
static void gl2tp_realize(Gl2tp*);
static void gl2tp_map(Gl2tp*);
static void gl2tp_draw(Gl2tp*);
static void gl2tp_init(Gl2tp*);

G_DEFINE_TYPE(Gl2tp,gl2tp,GTK_TYPE_TABLE)


void gl2tp_class_init(Gl2tpClass*klass)
{

}

void gl2tp_init(Gl2tp* widget)
{
	gtk_table_resize(GTK_TABLE(widget),3,3);
	GtkWidget * label_dial_ip = gtk_label_new_with_mnemonic(_("_ip to dial: "));
	GtkWidget * label_name = gtk_label_new_with_mnemonic(_("_name: "));
	GtkWidget * label_passwd = gtk_label_new_with_mnemonic(_("_passwd: "));
	gtk_label_set_justify(GTK_LABEL(label_dial_ip),GTK_JUSTIFY_RIGHT);
	gtk_label_set_justify(GTK_LABEL(label_name),GTK_JUSTIFY_RIGHT);
	gtk_label_set_justify(GTK_LABEL(label_passwd),GTK_JUSTIFY_RIGHT);

	gtk_table_attach(GTK_TABLE(widget),label_dial_ip,0,1,0,1,GTK_EXPAND,GTK_EXPAND,20,5);
	gtk_table_attach(GTK_TABLE(widget),label_name,0,1,1,2,GTK_EXPAND,GTK_EXPAND,20,5);
	gtk_table_attach(GTK_TABLE(widget),label_passwd,0,1,2,3,GTK_EXPAND,GTK_EXPAND,20,5);

	gtk_widget_show(label_dial_ip);
	gtk_widget_show(label_name);
	gtk_widget_show(label_passwd);

	widget->ip = gtk_entry_new();
	widget->name = gtk_entry_new();
	widget->passwd = gtk_entry_new();

	gtk_table_attach(GTK_TABLE(widget),widget->ip,1,3,0,1,GTK_EXPAND,GTK_EXPAND,20,5);
	gtk_table_attach(GTK_TABLE(widget),widget->name,1,3,1,2,GTK_EXPAND,GTK_EXPAND,20,5);
	gtk_table_attach(GTK_TABLE(widget),widget->passwd,1,3,2,3,GTK_EXPAND,GTK_EXPAND,20,5);


}

GtkWidget * gl2tp_new()
{
	g_object_new(G_TYPE_GL2TP,NULL);
}
