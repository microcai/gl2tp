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
#include "md5.h"

#define STRLEN 80
#define MAXSTRLEN 120
#define MD_SIG_SIZE 16

struct challenge
{
    struct MD5Context md5;
    unsigned char ss;           /* State we're sending in */
    unsigned char secret[MAXSTRLEN];    /* The shared secret */
    unsigned char *challenge;       /* The original challenge */
    unsigned char response[MD_SIG_SIZE];        /* What we expect as a respsonse */
    unsigned char reply[MD_SIG_SIZE];   /* What the peer sent */
    unsigned char *vector;
    unsigned int vector_len;
    int state;                  /* What state is challenge in? */
};


struct call
{
/*	int rbit;		Set the "R" bit on the next packet? */
    int lbit;                   /* Should we send length field? */
/*	int throttle;	Throttle the connection? */
    int seq_reqd;               /* Sequencing required? */
    int tx_pkts;                /* Transmitted packets */
    int rx_pkts;                /* Received packets */
    int tx_bytes;               /* transmitted bytes */
    int rx_bytes;               /* received bytes */
    struct schedule_entry *zlb_xmit;
    /* Scheduled ZLB transmission */
/*	struct schedule_entry *dethrottle; */
    /* Scheduled dethrottling (overrun) */
/*	int timeout;	Has our timeout expired? If so, we'll go ahead
					 and transmit, full window or not, and set the
					 R-bit on this packet.  */
    int prx;                    /* What was the last packet we sent
                                   as an Nr? Used to manage payload ZLB's */
    int state;                  /* Current state */
    int frame;                  /* Framing being used */
    struct call *next;          /* Next call, for linking */
    int debug;
    int msgtype;                /* What kind of message are we
                                   working with right now? */

    int ourcid;                 /* Our call number */
    int cid;                    /* Their call number */
    int qcid;                   /* Quitting CID */
    int bearer;                 /* Bearer type of call */
    unsigned int serno;         /* Call serial number */
    unsigned int addr;          /* Address reserved for this call */
    int txspeed;                /* Transmit speed */
    int rxspeed;                /* Receive speed */
    int ppd;                    /* Packet processing delay (of peer) */
    int physchan;               /* Physical channel ID */
    char dialed[MAXSTRLEN];     /* Number dialed for call */
    char dialing[MAXSTRLEN];    /* Original caller ID */
    char subaddy[MAXSTRLEN];    /* Sub address */

    int needclose;              /* Do we need to close this call? */
    int closing;                /* Are we actually in the process of closing? */
    /*
       needclose            closing         state
       =========            =======         =====
       0                       0            Running
       1                       0            Send Closing notice
       1                       1            Waiting for closing notice
       0                       1            Closing ZLB received, actulaly close
     */
    struct tunnel *container;   /* Tunnel we belong to */
    int fd;                     /* File descriptor for pty */
    struct termios *oldptyconf;
    int die;
    int nego;                   /* Show negotiation? */
    int pppd;                   /* PID of pppd */
    int result;                 /* Result code */
    int error;                  /* Error code */
    int fbit;                   /* Use sequence numbers? */
    int ourfbit;                /* Do we want sequence numbers? */
/*	int ourrws;		Our RWS for the call */
    int cnu;                    /* Do we need to send updated Ns, Nr values? */
    int pnu;                    /* ditto for payload packet */
    char errormsg[MAXSTRLEN];   /* Error message */
/*	int rws;		Receive window size, or -1 for none */
    struct timeval lastsent;    /* When did we last send something? */
    guint16 data_seq_num;          /* Sequence for next payload packet */
    guint16 data_rec_seq_num;      /* Sequence for next received payload packet */
    guint16 closeSs;               /* What number was in Ns when we started to
                                   close? */
    int pLr;                    /* Last packet received by peer */
    struct lns *lns;            /* LNS that owns us */
    struct lac *lac;            /* LAC that owns us */
    char dial_no[128];          /* jz: dialing number for outgoing call */
};


struct tunnel
{
    int fc;                     /* Framing capabilities of peer */
    struct schedule_entry *hello;
    int ourfc;                  /* Our framing capabilities */
    int bc;                     /* Peer's bearer channels */
    int hbit;                   /* Allow hidden AVP's? */
    int ourbc;                  /* Our bearer channels */
    guint64 tb;                    /* Their tie breaker */
    guint64 ourtb;                 /* Our tie breaker */
    int tid;                    /* Peer's tunnel identifier */
    int ourtid;                 /* Our tunnel identifier */
    int qtid;                   /* TID for disconnection */
    int firmware;               /* Peer's firmware revision */
    struct sockaddr_in peer;    /* Peer's Address */
    int debug;                  /* Are we debugging or not? */
    int nego;                   /* Show Negotiation? */
    int count;                  /* How many membmer calls? */
    int state;                  /* State of tunnel */
    guint16 control_seq_num;       /* Sequence for next packet */
    guint16 control_rec_seq_num;   /* Next expected to receive */
    int cLr;                    /* Last packet received by peer */
    char hostname[MAXSTRLEN];   /* Remote hostname */
    char vendor[MAXSTRLEN];     /* Vendor of remote product */
    struct challenge chal_us;   /* Their Challenge to us */
    struct challenge chal_them; /* Our challenge to them */
    char secret[MAXSTRLEN];     /* Secret to use */
    int rws;                    /* Peer's Receive Window Size */
    int ourrws;                 /* Receive Window Size */
    struct call *self;
    struct lns *lns;            /* LNS that owns us */
    struct lac *lac;            /* LAC that owns us */
};


struct lac
{
    struct sockaddr_in lns;           /* LNS's we can connect to */
    struct schedule_entry *rsched;
    int tun_rws;                /* Receive window size (tunnel) */
    int call_rws;               /* Call rws */
    int active;                 /* Is this connection in active use? */
    int hbit;                   /* Permit hidden AVP's? */
    int lbit;                   /* Use the length field? */
    int challenge;              /* Challenge authenticate the peer? */
    unsigned int localaddr;     /* Local IP address */
    unsigned int remoteaddr;    /* Force remote address to this */
    char authname[STRLEN];      /* Who we authenticate as */
    char peername[STRLEN];      /* Force peer name to this */
    char hostname[STRLEN];      /* Hostname to report */
    char entname[STRLEN];       /* Name of this entry */
    int authpeer;               /* Authenticate our peer? */
    int authself;               /* Authenticate ourselves? */
    int pap_require;            /* Require PAP auth for PPP */
    int chap_require;           /* Require CHAP auth for PPP */
    int pap_refuse;             /* Refuse PAP authentication for us */
    int chap_refuse;            /* Refuse CHAP authentication for us */
    int idle;                   /* Idle timeout in seconds */
    int autodial;               /* Try to dial immediately? */
    int defaultroute;           /* Use as default route? */
    int redial;                 /* Redial if disconnected */
    int rmax;                   /* Maximum # of consecutive redials */
    int rtries;                 /* # of tries so far */
    int rtimeout;               /* Redial every this many # of seconds */
    char pppoptfile[STRLEN];    /* File containing PPP options */
    int debug;
    struct tunnel *t;           /* Our tunnel */
    struct call *c;             /* Our call */
};


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

	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

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
//	为 l2tp 隧道连接初始化



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
