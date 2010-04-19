/*
 * l2tpclient.c - l2tp client
 *
 *  Created on: 2010-4-18
 *      Author: cai
 */


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif
#include <pty.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "l2tpclient.h"
#include "i18n.h"


static void gl2tpclient_class_init(Gl2tpClientClass*);
static void gl2tpclient_init(Gl2tpClient*);
static void gl2tpclient_finalize(GObject*);
static void gl2tpclient_set_property(GObject *object, guint property_id,const GValue *value, GParamSpec *pspec);
static void gl2tpclient_get_property(GObject *object, guint property_id,GValue *value, GParamSpec *pspec);
static gboolean gl2tp_udpsocket_dispatch(GIOChannel *, GIOCondition , gpointer);

G_DEFINE_TYPE(Gl2tpClient,gl2tpclient,G_TYPE_OBJECT)


void gl2tpclient_class_init(Gl2tpClientClass*klass)
{
	GObjectClass * gclass = (GObjectClass*)klass;
	gclass->finalize = gl2tpclient_finalize;

	int sock = socket(AF_INET,SOCK_DGRAM,0);

	struct sockaddr_in addr={0};

	addr.sin_family = AF_INET;

	addr.sin_port = htons(1701);

	int opt = 1;

	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,opt,sizeof(opt));

	if (bind(sock, (struct sockaddr*) &addr, INET_ADDRSTRLEN) < 0)
	{
		close(sock);
		return;
	}
	klass->udpsocket =  g_io_channel_unix_new(sock);

	g_io_add_watch(klass->udpsocket,G_IO_IN,gl2tp_udpsocket_dispatch,klass);
}

void gl2tpclient_init(Gl2tpClient*obj)
{
//	openpty();



}

void gl2tpclient_finalize(GObject*obj)
{
	//chain the finalize
	G_OBJECT_GET_CLASS(gl2tpclient_parent_class)->finalize(obj);
}

static void gl2tpclient_fork_pty(Gl2tpClient * obj)
{
	int slave;
    struct termios ptyconf;
    char pty_name[256];

	openpty(&obj->fd,&slave,pty_name,&ptyconf,NULL);
    ptyconf.c_cflag &= ~(ICANON | ECHO);
    tcsetattr (obj->fd, TCSANOW, &ptyconf);
}

void gl2tpclient_fork_pppd()
{



}


Gl2tpClient * gl2tpclient_new()
{
	return  (Gl2tpClient*)g_object_new(G_TYPE_GL2TPCLIENT,NULL);
}

gboolean gl2tpclient_bind(Gl2tpClient * obj)
{
	return GL2TPCLIENT_GET_CLASS(obj)->udpsocket !=NULL ;
}

gboolean gl2tp_udpsocket_dispatch(GIOChannel * source, GIOCondition con, gpointer user_data)
{
	return TRUE;
}
