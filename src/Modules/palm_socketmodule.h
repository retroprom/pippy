#ifndef _PALM_SOCKETMODULE_H_
#define _PALM_SOCKETMODULE_H_


/*  The following were gleaned from the windows version of the
    palm SDK:  sdk35-core.zip.  Also, see NetSocket.c. */

/* This makes O_NDELAY the same value for PalmOS as Linux port */
#ifndef O_NDELAY
#define O_NDELAY _FNONBLOCK
#endif

#define FORCE_ANSI_FUNC_DEFS

#ifdef FORCE_ANSI_FUNC_DEFS
#define BUILD_FUNC_DEF_1( fnname, arg1type, arg1name )	\
fnname( arg1type arg1name )

#define BUILD_FUNC_DEF_2( fnname, arg1type, arg1name, arg2type, arg2name ) \
fnname( arg1type arg1name, arg2type arg2name )

#define BUILD_FUNC_DEF_3( fnname, arg1type, arg1name, arg2type, arg2name , arg3type, arg3name )	\
fnname( arg1type arg1name, arg2type arg2name, arg3type arg3name )

#define BUILD_FUNC_DEF_4( fnname, arg1type, arg1name, arg2type, arg2name , arg3type, arg3name, arg4type, arg4name )	\
fnname( arg1type arg1name, arg2type arg2name, arg3type arg3name, arg4type arg4name )

#else /* !FORCE_ANSI_FN_DEFS */
#define BUILD_FUNC_DEF_1( fnname, arg1type, arg1name )	\
fnname( arg1name )	\
	arg1type arg1name;

#define BUILD_FUNC_DEF_2( fnname, arg1type, arg1name, arg2type, arg2name ) \
fnname( arg1name, arg2name )	\
	arg1type arg1name;	\
	arg2type arg2name;

#define BUILD_FUNC_DEF_3( fnname, arg1type, arg1name, arg2type, arg2name, arg3type, arg3name ) \
fnname( arg1name, arg2name, arg3name )	\
	arg1type arg1name;	\
	arg2type arg2name;	\
	arg3type arg3name;

#define BUILD_FUNC_DEF_4( fnname, arg1type, arg1name, arg2type, arg2name, arg3type, arg3name, arg4type, arg4name ) \
fnname( arg1name, arg2name, arg3name, arg4name )	\
	arg1type arg1name;	\
	arg2type arg2name;	\
	arg3type arg3name;	\
	arg4type arg4name;

#endif /* !FORCE_ANSI_FN_DEFS */


/* The object holding a socket.  It holds some extra information,
   like the address family, which is used to decode socket address
   arguments properly. */

typedef struct {
	PyObject_HEAD
	int sock_fd;		/* Socket file descriptor */
	int sock_family;	/* Address family, e.g., AF_INET */
	int sock_type;		/* Socket type, e.g., SOCK_STREAM */
	int sock_proto;		/* Protocol type, usually 0 */
	union sock_addr {
		struct sockaddr_in in;

	} sock_addr;
} PySocketSockObject;


extern PyTypeObject PySocketSock_Type;
#define PySocketSock_Check(op) ((op)->ob_type == &PySocketSock_Type)


#endif /* _PALM_SOCKETMODULE_H_ */
