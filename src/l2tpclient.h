/*
 * l2tpclient.h
 *
 *  Created on: 2010-4-18
 *      Author: cai
 */

#ifndef L2TPCLIENT_H_
#define L2TPCLIENT_H_

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _Gl2tpClient{
	GObject	parent;
	int		fd ; //fd for conmunicate with pppd's pre-sudo tty
	pid_t	pppd; //pid for pppd
}Gl2tpClient;

typedef struct _Gl2tpClientClass{
	GObjectClass	parent;
	GIOChannel	*	udpsocket;
}Gl2tpClientClass;

#define G_TYPE_GL2TPCLIENT	gl2tpclient_get_type()
#define GL2TPCLIENT(obj) (G_TYPE_CHECK_INSTANCE_CAST ((obj), G_TYPE_GL2TPCLIENT, Gl2tpClient))
#define GL2TPCLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), G_TYPE_GL2TPCLIENT, Gl2tpClientClass))
#define IS_GL2TPCLIENT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), G_TYPE_GL2TPCLIENT))
#define IS_GL2TPCLIENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), G_TYPE_GL2TPCLIENT))
#define GL2TPCLIENT_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), G_TYPE_GL2TPCLIENT, Gl2tpClientClass))

GType gl2tpclient_get_type() G_GNUC_CONST;

Gl2tpClient * gl2tpclient_new();
gboolean gl2tpclient_bind(Gl2tpClient * obj);

G_END_DECLS

#endif /* L2TPCLIENT_H_ */
