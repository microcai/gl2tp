/*
 * gl2tp.h
 *
 *  Created on: 2010-4-17
 *      Author: cai
 */

#ifndef GL2TP_H_
#define GL2TP_H_

#include <gtk/gtk.h>

G_BEGIN_DECLS

typedef struct _Gl2tp{
	GtkTable	parent;
	GtkWidget * ip, *name, *passwd;
}Gl2tp;

typedef struct _Gl2tpClass{
	GtkTableClass	parent;


}Gl2tpClass;

#define G_TYPE_GL2TP	gl2tp_get_type()
#define GL2TP(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_GL2TP, Gl2tp))
#define GL2TP_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), G_TYPE_GL2TP, Gl2tpClass))
#define IS_GL2TP(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_GL2TP))
#define IS_GL2TP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), G_TYPE_GL2TP))
#define GL2TP_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), G_TYPE_GL2TP, Gl2tpClass))


GType gl2tp_get_type() G_GNUC_CONST;
GtkWidget * gl2tp_new();

G_END_DECLS

#endif /* GL2TP_H_ */
