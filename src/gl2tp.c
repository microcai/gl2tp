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
static void gl2tp_finalize(GObject*);
static void gl2tp_set_property(GObject *object, guint property_id,const GValue *value, GParamSpec *pspec);
static void gl2tp_get_property(GObject *object, guint property_id,GValue *value, GParamSpec *pspec);
static gboolean gtk_entry_focus(GtkWidget *widget, GdkEventFocus *event,gpointer user_data);

G_DEFINE_TYPE(Gl2tp,gl2tp,GTK_TYPE_TABLE)

enum{
	GL2TP_IP = 3,
	GL2TP_NAME ,
	GL2TP_PASSWD ,
	GL2TP_STATUS
};

void gl2tp_class_init(Gl2tpClass*klass)
{
	GObjectClass	* gobjclass = G_OBJECT_CLASS(klass);
	gobjclass->finalize = gl2tp_finalize;
	gobjclass->set_property = gl2tp_set_property;
	gobjclass->get_property = gl2tp_get_property;

	g_object_class_install_property(gobjclass,GL2TP_IP,g_param_spec_string("ip","ip","ip","",G_PARAM_READWRITE));
	g_object_class_install_property(gobjclass,GL2TP_NAME,g_param_spec_string("name","name","name","",G_PARAM_READWRITE));
	g_object_class_install_property(gobjclass,GL2TP_PASSWD,g_param_spec_string("passwd","passwd","passwd","",G_PARAM_READWRITE));

	g_signal_new("status-changed",G_OBJECT_CLASS_TYPE(klass),G_SIGNAL_RUN_LAST,0,0,0,g_cclosure_marshal_VOID__STRING,G_TYPE_NONE,1,G_TYPE_STRING);
	g_signal_new("status-restore",G_OBJECT_CLASS_TYPE(klass),G_SIGNAL_RUN_LAST,0,0,0,g_cclosure_marshal_VOID__VOID,G_TYPE_NONE,0);

}

void gl2tp_init(Gl2tp* widget)
{
	gtk_table_resize(GTK_TABLE(widget),4,4);
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

	gtk_widget_set_tooltip_text(widget->ip,_("dial server ip address"));//GTK_ENTRY_ICON_PRIMARY
	gtk_widget_set_tooltip_text(widget->name,_("username"));//GTK_ENTRY_ICON_PRIMARY
	gtk_widget_set_tooltip_text(widget->passwd,_("passwd"));//GTK_ENTRY_ICON_PRIMARY

	gtk_table_attach(GTK_TABLE(widget),widget->ip,1,3,0,1,GTK_FILL,GTK_EXPAND,20,5);
	gtk_table_attach(GTK_TABLE(widget),widget->name,1,3,1,2,GTK_FILL,GTK_EXPAND,20,5);
	gtk_table_attach(GTK_TABLE(widget),widget->passwd,1,3,2,3,GTK_FILL,GTK_EXPAND,20,5);


	"focus-out-event";

	GtkWidget * bt = gtk_button_new_from_stock(GTK_STOCK_EXECUTE);

//	gtk_button_set_label(GTK_BUTTON(bt),_("_connect"));

	gtk_table_attach_defaults(GTK_TABLE(widget),bt,2,3,3,4);

	g_signal_connect(widget->ip,"focus-in-event",G_CALLBACK(gtk_entry_focus),widget);
	g_signal_connect(widget->ip,"focus-out-event",G_CALLBACK(gtk_entry_focus),widget);
	g_signal_connect(widget->name,"focus-in-event",G_CALLBACK(gtk_entry_focus),widget);
	g_signal_connect(widget->name,"focus-out-event",G_CALLBACK(gtk_entry_focus),widget);
	g_signal_connect(widget->passwd,"focus-in-event",G_CALLBACK(gtk_entry_focus),widget);
	g_signal_connect(widget->passwd,"focus-out-event",G_CALLBACK(gtk_entry_focus),widget);

//	gtk_statusbar_new();
}

gboolean gtk_entry_focus(GtkWidget *widget, GdkEventFocus *event,gpointer user_data)
{
	if(event->in)
	{
		gchar * tip = gtk_widget_get_tooltip_text(widget);

		gchar * status = g_strdup_printf(_("input : %s"),tip);
		g_free(tip);
		g_signal_emit_by_name(user_data,"status-changed",status);
		g_free(status);
	}else
	{
		g_signal_emit_by_name(user_data,"status-restore");
	}
	return FALSE;
}

void gl2tp_finalize(GObject*obj)
{

}

GtkWidget * gl2tp_new()
{
	g_object_new(G_TYPE_GL2TP,NULL);
}

void gl2tp_set_property(GObject *object, guint property_id,const GValue *value, GParamSpec *pspec)
{
	Gl2tp * obj = (Gl2tp*) object;

	g_return_if_fail(IS_GL2TP(obj));

	switch (property_id)
	{
	case GL2TP_IP:
		gtk_entry_set_text(GTK_ENTRY(obj->ip),g_value_get_string(value));
		break;
	case GL2TP_NAME:
		gtk_entry_set_text(GTK_ENTRY(obj->name),g_value_get_string(value));
		break;
	case GL2TP_PASSWD:
		gtk_entry_set_text(GTK_ENTRY(obj->passwd),g_value_get_string(value));
		break;
	default:
		g_warn_if_reached();
		break;
	}

}

void gl2tp_get_property(GObject *object, guint property_id,GValue *value, GParamSpec *pspec)
{
	Gl2tp * obj = (Gl2tp*) object;

	g_return_if_fail(IS_GL2TP(obj));

	switch (property_id)
	{
	case GL2TP_IP:
		g_value_set_string(value,gtk_entry_get_text(GTK_ENTRY(obj->ip)));
		break;
	case GL2TP_NAME:
		g_value_set_string(value,gtk_entry_get_text(GTK_ENTRY(obj->name)));
		break;
	case GL2TP_PASSWD:
		g_value_set_string(value,gtk_entry_get_text(GTK_ENTRY(obj->passwd)));
		break;
	default:
		g_warn_if_reached();
		break;
	}
}
