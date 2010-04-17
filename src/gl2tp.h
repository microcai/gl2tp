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


GType gl2tp_get_type() G_GNUC_CONST;
GtkWidget * gl2tp_new();

G_END_DECLS

#endif /* GL2TP_H_ */
