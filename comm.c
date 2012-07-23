/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  In order to use any part of this Merc Diku Mud, you must comply with   *
 *  both the original Diku license in 'license.doc' as well the Merc       *
 *  license in 'license.txt'.  In particular, you may not remove either of *
 *  these copyright notices.                                               *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#endif
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "merc.h"
int mud_port;
int mob_update_number = 0;
int control_num;
int per_cpu_usage[31];

/*
 * Signal handling.
 * Apollo has a problem with __attribute(atomic) in signal.h,
 *   I dance around it.
 */
#if defined(apollo)
#define __attribute(x)
#endif

#if defined(unix)
#include <signal.h>
#endif

#if defined(apollo)
#undef __attribute
#endif

/*
 * Socket and TCP/IP stuff.
 */
#if	defined(macintosh) || defined(MSDOS)
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif
#if	defined(unix)
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif

/*
 * OS-dependent declarations.
 */
#if	defined(_AIX)
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) ); 
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(apollo)
#include <unistd.h>
void	bzero		args( ( char *b, int length ) );
#endif

#if	defined(__hpux)
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(interactive)
#include <net/errno.h>
#include <sys/fnctl.h>
#endif

#if	defined(linux)
int	close		args( ( int fd ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if	defined(macintosh)
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};

#if	!defined(isascii)
#define	isascii(c)		( (c) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday		args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined(MIPS_OS)
extern	int		errno;
#endif

#if	defined(MSDOS)
int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
int	kbhit		args( ( void ) );
#endif

#if	defined(NeXT)
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined(sequent)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined(htons)
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined(ntohl)
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/* This includes Solaris Sys V as well */
#if defined(sun)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#if defined(SYSV)
int setsockopt		args( ( int s, int level, int optname,
			    const char *optval, int optlen ) );
#else
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined(ultrix)
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			    int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free;	/* Free list for descriptors	*/
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
bool		    god;		/* All new chars are gods!	*/
bool		    merc_down;		/* Shutdown			*/
bool		    wizlock;		/* Game is wizlocked		*/
bool		    newlock;		/* Game is newlocked		*/
char		    str_boot_time[MAX_INPUT_LENGTH];
time_t		    current_time;	/* time of this pulse */	
LOGON_DATA * 	    logon_data_list;
int		    att_str, att_int, att_wis, att_dex, att_con, att_chr;

/*
 * OS-dependent local functions.
 */
#if defined(macintosh) || defined(MSDOS)
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length, bool color ) );
#endif

#if defined(unix)
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( int port ) );
void	new_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d , bool color) );
bool	write_to_descriptor	args( ( int desc, char *txt, long length , bool color) );
#endif

/*
 * Other local functions (OS-independent).
 */
bool	check_parse_name	args( ( char *name ) );
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				    bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d, bool color ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );

int main( int argc, char **argv )
{
    struct timeval now_time;
    int port, control;

    /*
     * Init time.
     */
    gettimeofday( &now_time, NULL );
    current_time 	= (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );

    /*
     * Macintosh console initialization.
     */
#if defined(macintosh)
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Get the port number.
     */
    port = 8100;
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #]\n", argv[0] );
	    exit( 1 );
	}
	else if ( ( port = atoi( argv[1] ) ) <= 10 )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}
    }

    mud_port = port;
    /*
     * Run the game.
     */
#if defined(macintosh) || defined(MSDOS)
    boot_db( );
    log_string( "Electric Dreams is ready to rock" );
    game_loop_mac_msdos( );
#endif

#if defined(unix)
    control = init_socket( port );
    boot_db();
    control_num = control;
    sprintf( log_buf, "Electric Dreams is ready to rock on port %d.", port );
    log_string( log_buf );
    game_loop_unix( control );
    close (control);
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}

#if defined(unix)
int init_socket( int port )
{
    static struct sockaddr_in sa_zero;
    struct sockaddr_in sa;
    int x = 1;
    int fd;

    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof(x) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
	close(fd);
	exit( 1 );
    }

#if defined(SO_DONTLINGER) && !defined(SYSV)
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof(ld) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
	    close(fd);
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
    sa.sin_family   = AF_INET;
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof(sa) ) < 0 )
    {
	perror("Init socket: bind" );
	close(fd);
	exit(1);
    }

    if ( listen( fd, 3 ) < 0 )
    {
	perror("Init socket: listen");
	close(fd);
	exit(1);
    }

    return fd;
}
#endif

#if defined(unix)
void game_loop_unix( int control )
{
    char buf[BUFSIZE];
    bool color;
    static struct timeval null_time;
    struct timeval last_time;
    signal( SIGPIPE, SIG_IGN );

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	fd_set in_set;
	fd_set out_set;
	fd_set exc_set;
	DESCRIPTOR_DATA *d;
	int maxdesc;

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	if ( control != control_num )
	{
	    sprintf( log_buf, "Game_loop_unix Control number lost : %d", control );
	    log_string( log_buf );
	    control = control_num;
	}

	FD_SET( control, &in_set );
	maxdesc	= control;
	for ( d = descriptor_list; d; d = d->next )
	{
	    maxdesc = UMAX( maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;   
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character && d->character->level > 1)
		    save_char_obj( d->character );

		d->outtop	= 0;
		sprintf( buf, "Descriptor exception: %d. (closing socket)", d->descriptor );
		log_string( buf );
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character != NULL ) 
		{
		    d->character->timer = 0;
		    if ( IS_SET( d->character->plr, PLR_ANSI ) )
			color = TRUE;
		    else
			color = FALSE;
		}
		else
		    color = FALSE;

		if ( !read_from_descriptor( d , color) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );

		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character != NULL && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d, FALSE );
	   // if ( d->character != NULL && d->character->hunting[0] != '\0' )
	//	do_hunt( d->character, "" );
// Reverie - Why are we doing a hunt at this point in the descriptor?
	    if ( d->incomm[0] != '\0' )
	    {
		smash_tilde( d->incomm );
		d->fcommand	= TRUE;
		stop_idling( d->character );

        if ( d->pString != NULL ) string_add( d->character,  d->incomm );	/* OLC */
          else
        if ( d->showstr_point )
            show_string( d, d->incomm );
        else
            switch ( d->connected )
            {
            case CON_PLAYING:
                interpret( d->character, d->incomm );
                break;
            case CON_AEDITOR:
                aedit( d->character, d->incomm );
                break;
            case CON_REDITOR:
                redit( d->character, d->incomm );
                break;
            case CON_OEDITOR:
                oedit( d->character, d->incomm );
                break;
            case CON_MEDITOR:
                medit( d->character, d->incomm );
                break;
            case CON_GEDITOR:
                gedit( d->character, d->incomm );
                break;
            case CON_MAEDITOR:
                maedit( d->character, d->incomm );
                break;
            case CON_TEDITOR:
                tedit( d->character, d->incomm );
                break;
            case CON_HEDITOR:
                hedit( d->character, d->incomm );
                break;
            case CON_SEDITOR:
                sedit( d->character, d->incomm );
                break;
            case CON_EDITING:
                interpret( d->character, d->incomm );
                break;
            case CON_RAEDITOR:
                raedit( d->character, d->incomm );
                break;
            default:
                nanny( d, d->incomm );
                break;
       	    }

		d->incomm[0]	= '\0';
	    }
	}

	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next	= d->next;
	}

	/*
	 * Autonomous game motion.
	 */
	
	update_handler( );

	/*
	 * Output.
	 */
	for ( d = descriptor_list; d != NULL; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
	      && FD_ISSET( d->descriptor, &out_set ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character != NULL && d->character->level > 1)
			save_char_obj( d->character );

		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}

	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;
	    long cpu_free;
	    int x, count = 0;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ((int) last_time.tv_usec) - ((int) now_time.tv_usec)
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ((int) last_time.tv_sec ) - ((int) now_time.tv_sec );
	    
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;

	    	cpu_free = 1000000  - ( PULSE_PER_SECOND * usecDelta );
			if ( cpu_free < 0 )
			   cpu_free = cpu_free * -1;
	    	if ( cpu_free == 0 )
			cpu_free = 1;

	    	per_cpu_usage[0] =  cpu_free / 10000;
		for ( x = 30; x > 0; x-- )
		{
		    per_cpu_usage[x] = per_cpu_usage[x -1];
		}
		if ( per_cpu_usage[0] > 10 )
	        {
		    count++;
		    if ( count > 10 )
		    count = 0;
		}
		
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	    else
	    {
		per_cpu_usage[0] = 100;
		for ( x = 30; x > 0; x-- )
		{
		    per_cpu_usage[x] = per_cpu_usage[x -1];
		}
	    }
	}

	mob_list_current_update = mob_list_update[mob_update_number];
	mob_update_number++;
	if (mob_update_number > 15 )
	mob_update_number = 0;
	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif

#if defined(unix)
void new_descriptor( int control )
{
    static DESCRIPTOR_DATA d_zero;
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *dnew;
    BAN_DATA *pban;
    bool color;
    sh_int greeting;
    extern sh_int top_greeting;
    struct sockaddr_in sock;
    int desc, x;
    int size;

    if ( control != control_num )
    {
	sprintf( log_buf, "New_descriptor Control number lost : %d", control );
	log_string( log_buf );
	control = control_num;
    }

    size = sizeof( sock );
    getsockname( control, (struct sockaddr *) &sock, (socklen_t *) &size );
    if ( ( desc = accept( control, ( struct sockaddr * ) &sock, (socklen_t * ) &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined(FNDELAY)
#define FNDELAY O_NDELAY
#endif

    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }

    /*
     * Cons a new descriptor.
     */
    if ( descriptor_free == NULL )
    {
	dnew		= (DESCRIPTOR_DATA *) alloc_perm( sizeof(*dnew) );
	top_descriptor++;
    }
    else
    {
	dnew		= descriptor_free;
	descriptor_free	= descriptor_free->next;
    }

    *dnew		= d_zero;
    dnew->descriptor	= desc;
    dnew->connected	= CON_GET_NAME;
    dnew->showstr_head	= &str_empty[0];
    dnew->showstr_point = NULL;
    dnew->outsize	= 2000;
    dnew->pEdit		= NULL; /* OLC */
    dnew->pString	= NULL; /* OLC */
    dnew->outbuf	= (char *) alloc_mem( dnew->outsize );
    for ( x = 0; x < MAX_HISTORY ; x++ )
    dnew->inlast[x]     = &str_empty[0];
    dnew->repeat	= 0;

    size = sizeof(sock);
    if ( getpeername( desc, (struct sockaddr *) &sock, (socklen_t *) &size ) < 0 )
    {
	perror( "New_descriptor: getpeername" );
	strncpy( dnew->host, "(unknown)", 15 );
    }
    else
    {
	/*
	 * Would be nice to use inet_ntoa here but it takes a struct arg,
	 * which ain't very compatible between gcc and system libraries.
	 */
	int addr;

	addr = ntohl( sock.sin_addr.s_addr );
	sprintf( buf, "%d.%d.%d.%d",
	    ( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	    ( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	    );

	sprintf( log_buf, "Sock.sinaddr:  %s", buf );
	log_string( log_buf );
	//if ( !str_cmp("66.188.185.133", buf) )
	//    do_ban(NULL, buf);

	strncpy( dnew->host, buf, 15 );
  }
	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of Hiddenorlds.
     */
    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_suffix( pban->name, dnew->host ) ) 
	{
	    color = FALSE;
	    write_to_descriptor( desc,
		"Your site has been banned from this Mud.\n\r", 0, color );
	    close( desc );
	    free_mem( dnew->outbuf, dnew->outsize );
	    dnew->next		= descriptor_free;
	    descriptor_free	= dnew;
	    return;
	}
    }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {  
	
	extern char * help_greeting[MAX_GREETING]; 
	
	if ( top_greeting > 0 )
	{
            greeting = number_range( 0, top_greeting - 1 );
	    if ( help_greeting[greeting][0] == '.' )
	        write_to_buffer( dnew, (help_greeting[greeting])+1, 0 );
	    else
	        write_to_buffer( dnew, (help_greeting[greeting])  , 0 );
	}
	else
	{
	    bug( "No greetings ", 0 );
	    write_to_buffer( dnew, "We are experiencing technical dificulties.\n\r\n\rName: ", 0 );
	}
    }

    return;
}
#endif

void close_socket( DESCRIPTOR_DATA *dclose )
{
    SNOOP_LIST * snoop_list;
    SNOOP_LIST * snoop_list_next;
    CHAR_DATA *ch;

    if ( !dclose )
	return;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    for ( snoop_list = dclose->snoop_by ; snoop_list != NULL ; snoop_list = snoop_list_next )
    {
	snoop_list_next = snoop_list->next;
	write_to_buffer( snoop_list->desc,
	    "Your victim has left the game.\n\r", 0 );
	free_snoop_list( snoop_list );
    }

    dclose->snoop_by = NULL;

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    while ( d->snoop_by && d->snoop_by->desc == dclose )
	    {
		snoop_list = d->snoop_by;
		d->snoop_by = d->snoop_by->next;
		free_snoop_list( snoop_list );
	    }

	    for ( snoop_list = d->snoop_by ; snoop_list != NULL ; snoop_list = snoop_list_next )
	    {
		snoop_list_next = snoop_list->next;

		while ( snoop_list_next != NULL && snoop_list_next->desc == dclose )
		{
		    snoop_list->next = snoop_list_next->next;
		    free_snoop_list( snoop_list_next );
		    snoop_list_next = snoop_list->next;
		}
	    }
	}
    }

    if ( ( ch = dclose->character ) != NULL )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
	info_channel ( ch, log_buf , INFOACT_LINK);
	if (LOGON_ON)
	{
	   if (ch->logon_data != NULL) /* for those that type the wrong passwd */
	   {
	   if (!str_cmp(ch->logon_data->exit,"Menu-Quit") )
	   	strncpy( ch->logon_data->exit, "Menu-Quit", 30 );
	   else
	   	strncpy( ch->logon_data->exit, "Lost-link", 30 );
	   }
	}
	if ( dclose->connected < CON_PLAYING )
	     dclose->connected = CON_PLAYING; 
	if ( dclose->connected == CON_PLAYING )
	{
	    act( "$n has lost $s link.", ch, NULL, NULL, TO_ROOM );
	/*
	 *  Cleanup from the editors
	 */
	    ch->desc = NULL;
	    ch->edit_flags = 0;
	    ch->edit_buffer[0] = '\0';
	}
	else
	{   /* for dumbasses that fail the passwd on a reconnect */
	    if ( !check_reconnect( dclose, ch->name, FALSE ) )
	    	free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d != NULL )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

    close( dclose->descriptor );
    free_string(dclose->showstr_head);  /* added back Galadrail */

    /* RT socket leak fix -- I hope */

    free_mem(dclose->outbuf,dclose->outsize);
    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined(MSDOS) || defined(macintosh)
    exit(1);
#endif
    return;
}

bool read_from_descriptor( DESCRIPTOR_DATA *d, bool color )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen(d->inbuf);
    if ( (unsigned) iStart >= sizeof(d->inbuf) - 10 )
    {
	sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );
	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0, color );
	return FALSE;
    }

    /* Snarf input. */
#if defined(macintosh)
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof(d->inbuf) - 10 )
	    break;
    }
#endif

#if defined(MSDOS) || defined(unix)
    for ( ; ; )
    {
	int nRead;

	nRead = read( d->descriptor, d->inbuf + iStart,
	    sizeof(d->inbuf) - 10 - iStart );
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
	else if ( errno == EWOULDBLOCK || errno == EAGAIN ) /* added check for EAGAIN -- G */
	    break;
	else
	{
	    perror( "Read_from_descriptor" );
	    
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d, bool color )
{
    int i, j, k, x;
    bool found = FALSE;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0, color );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	else if ( isascii(d->inbuf[i]) && isprint(d->inbuf[i]) )
	    d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && str_cmp( d->incomm, d->inlast[0] ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    int value = 25;

	    if ( d->character != NULL && !IS_IMMORTAL( d->character ) )
		value = UMAX( 25, d->character->level );
	    else if ( d->character != NULL && IS_IMMORTAL( d->character ) )
		value = UMAX( 100, d->character->level );

	    if ( ++d->repeat >= value )
	    {
		sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
		write_to_descriptor( d->descriptor,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0, color );
		strcpy( d->incomm, "quit" );
	    }
	}
    }

    /*
     * Do '!' substitution and the command history.
     * The history part was added by Galadrail 07/10/97
     * Similar to the unix history command.
     */
    if ( d->incomm[0] == '!' )
    {
	/*
	 * Show the history of commands but does not update
	 * the command history.
	 */
	if (d->incomm[1] == 'l' )  
	{
	    strcpy( d->incomm, "history" );
	}
	else
	{
	    /* 
	     * Loop through all the command history checking
	     * for a possible match. Example:  "!4"  will put
	     * the inlast[4] into the current command without
	     * updating the command history.
	     */
	    for ( x = 1; x < MAX_HISTORY; x++ )
	    {
		if ( d->incomm[1] == x + 48 ) /* Remember DEX */
		{
		    strcpy( d->incomm, d->inlast[x] );
		    found = TRUE;	
		}
	    }
	    /* 
	     * If we did not find a match, we just use the
	     * last command, IE normal "!".  Again we are not
	     * updating the command history.
	     */
	    if (!found)
	    strcpy( d->incomm, d->inlast[0] );
	}
    }
    else
    {
	/*
	 * Work in reverse to keep them in order.
	 * Move each command up 1 in the history.
	 * Don't bother updating if its just a return.
	 */
      if ( d->incomm[0] != '\n' ) 
      if ( d->incomm[0] != '\r' )
      {
	for ( x = MAX_HISTORY -1; x > 0; x-- )
	{
	   free_string( d->inlast[x] );
	   d->inlast[x] = str_dup(d->inlast[x-1]);
	}
	/* 
	 * Our first history is now our current command.
	 */
	free_string(d->inlast[0]);
	d->inlast[0] = str_dup( d->incomm );
      }
    }

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}

char *figurestate(int current, int max)
{
    static char status[40];
    
    bzero(status, sizeof(status));
    if (current >= (max/3*2)) sprintf(status, "`W%d`w", current);
    else if (current >= max/3) sprintf(status, "`Y%d`w", current);
    else sprintf(status, "`R%d`w", current);
    return (status);
}

char *damstatus2 (CHAR_DATA *ch)
{
    int percent;
    static char wound[40];
 
    bzero(wound, sizeof(wound));
    if (ch->max_hit > 0)
        percent = ch->hit * 100 / ch->max_hit;
    else
        percent = -1;
    if ( IS_NPC( ch ) && IS_AFFECTED( ch, AFF_BERSERK ) )
	percent = 100;
    if (percent >= 100)
                sprintf(wound,"`R***`Y***`G***`w");
            else if (percent >= 95)
                sprintf(wound,"`R***`Y***`G**`g*`w");
	    else if (percent >= 85)
		sprintf(wound,"`R***`Y***`G*`g**`w");
            else if (percent >= 75)
                sprintf(wound,"`R***`Y***`g***`w");
            else if (percent >= 65)
                sprintf(wound,"`R***`Y**`y*`g***`w");
            else if (percent >= 55)
                sprintf(wound,"`R***`Y*`y**`g***`w");
            else if (percent >= 45)
                sprintf(wound,"`R***`y***`g***`w");
	    else if (percent >= 35)
		sprintf(wound,"`R**`r*`y***`g***`w");
	    else if (percent >= 25)
		sprintf(wound,"`R*`r**`y***`g***`w");
	    else if (percent >= 10)
		sprintf(wound,"`r***`y***`g***`w");
            else if (percent >= 0)
                sprintf(wound,"`Y*WARNING*`w");
            else
                sprintf(wound,"`R*WARNING*`w");
 
    return (wound);
}

char *damstatus (CHAR_DATA *ch)
{
    int percent;
    static char wound[40];
 
    bzero(wound, sizeof(wound));
    if (ch->max_hit > 0)
        percent = ch->hit * 100 / ch->max_hit;
    else
        percent = -1;
    if ( IS_AFFECTED( ch, AFF_BERSERK ) )
	percent = 100;
    if (percent >= 100)
                sprintf(wound,"`Glooks very healthy.");
            else if (percent >= 90)
                sprintf(wound,"`gis in near perfect health.");
	    else if (percent >= 80)
		sprintf(wound,"`Mhas small cuts and bruises.");
            else if (percent >= 75)
                sprintf(wound,"`mhas some small bleeding wounds.");
            else if (percent >= 50)
                sprintf(wound,"`Bhas large wounds and gashes.");
            else if (percent >= 30)
                sprintf(wound,"`bwounds are bleeding profusely.");
            else if (percent >= 15)
                sprintf(wound,"`Yhas wounds that bleed freely.");
	    else if (percent >= 10)
		sprintf(wound,"`ywill die soon from repulsive wounds.");
            else if (percent >= 0)
                sprintf(wound,"`Ris fainting from the lack of blood.");
            else
                sprintf(wound,"`ris having a near death experience.");
 
    return (wound);
}


/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    CHAR_DATA * npc;
    SNOOP_LIST * snoop_list;
    extern bool merc_down;

    bool color = TRUE;
    if ( ( d->character !=  NULL) && IS_SET(d->character->plr, PLR_ANSI) )
	color = TRUE;
    else
	if ( d->color )
	      color = TRUE;
	else
	      color = FALSE;

    npc = d->character;

    /*
     * Bust a prompt.
     */     
    if ( d->pString != NULL ) /* OLC */
	write_to_buffer( d, "> ", 2 );
    else
    if ( fPrompt && !merc_down &&
       ( d->connected == CON_PLAYING
      || d->connected == CON_AFT_LOGIN
      || d->connected == CON_EDITING
      || d->connected == CON_PEDITOR
      || d->connected == CON_AEDITOR
      || d->connected == CON_HEDITOR
      || d->connected == CON_SEDITOR
      || d->connected == CON_RAEDITOR
      || d->connected == CON_REDITOR
      || d->connected == CON_OEDITOR
      || d->connected == CON_MAEDITOR
      || d->connected == CON_TEDITOR
      || d->connected == CON_GEDITOR
      || d->connected == CON_MEDITOR ) )
    {
        if ( d->showstr_point )
	    write_to_buffer(d,"`w[(C)ontinue (B)ack, (R)efresh, (H)elp, (S)kip To Last Page, (#)Page, (Q)uit]\n\r",0);
    	else if ( IS_NPC( npc ) )
	    write_to_buffer( d, "> ", 2 );
    	else 
    	{
   	    CHAR_DATA *ch;

	    ch = d->character;
	    if (IS_SET(ch->plr, PLR_ANSI) )
	        color = TRUE;
	    else
	        color = FALSE;

	    ch = d->original ? d->original : d->character;
	    if (!IS_SET(ch->comm, COMM_COMPACT) && ch->prompt_num == 0 )
	        write_to_buffer( d, "\n\r", 2 );

	    if (ch->pcdata->prompt[0] != '\0' && ch->prompt_num==0 )
	    {
                char buf[MAX_STRING_LENGTH];
                char buf2[MAX_STRING_LENGTH];
                CHAR_DATA *tank,*victim;
	        int promptlen;       
	        int c1=0,c2=0;
 
	        promptlen=strlen(ch->pcdata->prompt);

                ch = d->character;
                buf[0]=0;
                promptlen=strlen(ch->pcdata->prompt);

	        buf2[0] = '\0';

                for (;c1<promptlen;c1++)
                {
                if ( IS_IMMORTAL( ch ) )
                {
/* OLC */   	switch (ch->desc->connected)
        	{
        		case CON_AEDITOR:
        	            sprintf( buf, "AEdit-> " );
                            strcat(buf2,buf);
        	            break;
        	        case CON_PEDITOR:
        	            sprintf( buf, "PEdit-> " );
        	            strcat( buf2,buf);
        	            break;
        	        case CON_AFT_LOGIN:
        	            sprintf( buf, "Electric Dreams->" );
                            strcat(buf2,buf);
        	            break;
        	        case CON_RAEDITOR:
        	            sprintf( buf, "RaEdit-> " );
                            strcat(buf2,buf);
        	            break;
        	        case CON_REDITOR:
			    if ( ch->edit_buffer )
			    {
				sprintf( buf, "REdit->%s",ch->edit_buffer );
				strcat(buf2,buf);
			    }
			    else
			    {
				sprintf( buf, "REdit->" );
				strcat(buf2,buf);
			    }
        	            break;
        	        case CON_OEDITOR:
			    if ( ch->edit_buffer )
			    {
				sprintf( buf, "OEdit->%s",ch->edit_buffer );
				strcat(buf2,buf);
			    }
			    else
			    {
				sprintf( buf, "OEdit->" );
				strcat(buf2,buf);
			    }
        	            break;
        	        case CON_MEDITOR:
			    if ( ch->edit_buffer )
			    {
				sprintf( buf, "MEdit->%s",ch->edit_buffer );
				strcat(buf2,buf);
			    }
			    else
			    {
				sprintf( buf, "MEdit->" );
				strcat(buf2,buf);
			    }
        	            break;
        	        case CON_GEDITOR:
			    if ( ch->edit_buffer )
			    {
				sprintf( buf, "GEdit->%s",ch->edit_buffer );
				strcat(buf2,buf);
			    }
			    else
			    {
				sprintf( buf, "GEdit->" );
				strcat(buf2,buf);
			    }
        	            break;
        	        case CON_MAEDITOR:
        	            sprintf( buf, "MaEdit-> " );
                            strcat(buf2,buf);
        	            break;
        	        case CON_TEDITOR:
        	            sprintf( buf, "TEdit-> " );
                            strcat(buf2,buf);
        	            break;
        	        case CON_HEDITOR:
        	            sprintf( buf, "HEdit-> " );
                            strcat(buf2,buf);
        	            break;
        	        case CON_SEDITOR:
        	            sprintf( buf, "SEdit-> " );
                            strcat(buf2,buf);
        	            break;
        	        default:
        	            buf2[0] = '\0';
        	            break;
              	}
	        }
                if (ch->pcdata->prompt[c1]!='%' )
                {
                c2=strlen(buf);
                buf[c2++]=ch->pcdata->prompt[c1];
                buf[c2]=0;
                }
                else
                if (c1<promptlen-1)
                {
              	c1++;
              	switch (ch->pcdata->prompt[c1])
              	{
                    case 'R':if (IS_IMMORTAL(ch))
                    {
                           sprintf(buf2,"%ld",ch->in_room->vnum);
                           strcat(buf,buf2);
                    }; break;
                    case 'a':if (IS_IMMORTAL(ch))
                    {
                           sprintf(buf2,"%d",ch->in_room->area->areanum);
                           strcat(buf,buf2);
                    }; break;
		    case 'r':if (IS_IMMORTAL(ch)) strcat(buf,ch->in_room->name); break;
		    case 'A':if (IS_IMMORTAL(ch)) strcat(buf,ch->in_room->area->filename); break;
		    case 'h':sprintf(buf2,"%d",ch->hit); strcat(buf,buf2); break;
                    case 'H':sprintf(buf2,"%d",ch->max_hit); strcat(buf,buf2); break;
                    case 'f':sprintf(buf2,"%d",ch->mana[0]); strcat(buf,buf2); break;
                    case 'F':sprintf(buf2,"%d",ch->max_mana[0]); strcat(buf,buf2); break;
                    case 'i':sprintf(buf2,"%d",ch->mana[1]); strcat(buf,buf2); break;
                    case 'I':sprintf(buf2,"%d",ch->max_mana[1]); strcat(buf,buf2); break;
                    case 'e':sprintf(buf2,"%d",ch->mana[2]); strcat(buf,buf2); break;
                    case 'E':sprintf(buf2,"%d",ch->max_mana[2]); strcat(buf,buf2); break;
                    case 'w':sprintf(buf2,"%d",ch->mana[3]); strcat(buf,buf2); break;
                    case 'W':sprintf(buf2,"%d",ch->max_mana[3]); strcat(buf,buf2); break;
                    case 's':sprintf(buf2,"%d",ch->mana[4]); strcat(buf,buf2); break;
                    case 'S':sprintf(buf2,"%d",ch->max_mana[4]); strcat(buf,buf2); break;
                    case 'v':sprintf(buf2,"%d",ch->move); strcat(buf,buf2); break;
                    case 'V':sprintf(buf2,"%d",ch->max_move); strcat(buf,buf2); break;
               	    case 'c':	sprintf(buf2, "%s", figurestate( ( IS_AFFECTED( ch, AFF_BERSERK ) ? ch->max_hit : ch->hit ), ch->max_hit));
		    				strcat(buf, buf2);
						break;
				case 'd':	sprintf(buf2, "%s", figurestate(ch->move, ch->max_move));
	    					strcat(buf, buf2);
						break;
				case 'l':  	if ((tank=ch->fighting) != NULL) 
		 		                    if ((tank=tank->fighting) != NULL) 
						    {
		 		 		        sprintf(buf2, "%s", damstatus(tank));
		    				        strcat(buf, buf2);
					            }
						break;
				case 'b':	if ((tank=ch->fighting) != NULL)
						    if ((tank=tank->fighting) != NULL)
						    {
							sprintf(buf2, "%s", damstatus2(tank));
							strcat(buf, buf2);
						    }
						break;
				case 'B':	if ((victim=ch->fighting) != NULL)
						{
						    sprintf(buf2, "%s", damstatus2(victim));
						    strcat(buf, buf2);
						}
						break;
				case 'q':	if ((victim=ch->fighting) != NULL) 
						{
						    sprintf(buf2, "%s", damstatus(victim));
			  			    strcat(buf, buf2);
					        }
						break;
				case 'Q':	sprintf(buf2, "%s", damstatus(ch));
	    					strcat(buf, buf2);
						break;
				case 't':	sprintf(buf2, "%d %s", (time_new.hour % 12 == 0)
						? 12 : time_new.hour % 12, time_new.hour >=
						12 ? "pm" : "am");
		    				strcat(buf, buf2);
						break;
				case 'T':	sprintf(buf2, "%d", time_new.hour);
	    					strcat(buf, buf2);
						break;
				case '#':	strcat(buf, "\n\r");
						break;
				case ';':	strcat(buf, "\n\r");
						break;
			        case 'L':       if (IS_SET(ch->tmpact,TMPACT_AWAY))
						{
						 sprintf(buf2, "`WAWAY`w ");
	                                         strcat(buf, buf2);
						}
						if (IS_SET(ch->plr,PLR_THIEF))
						{
						 sprintf(buf2, " `KTHIEF`w ");
                                	         strcat(buf, buf2);
						}
						if (IS_SET(ch->plr2,PLR_PLAYER_HELPER))
						{
						 sprintf(buf2, " `MPH`w ");
                                	         strcat(buf, buf2);
						}
					        if (IS_IMMORTAL(ch)&&IS_SET(ch->plr,PLR_WIZINVIS)) 
						{
						    if (ch->invis_level >= 61) 
							strcat(buf," OPTIWIZ ");
						    else
						    {
						 	sprintf(buf2, " `BWizi:%d`w",ch->invis_level);
						 	strcat(buf , buf2 );
						    }
						}
	                                        break;
                case '%':c2=strlen(buf); buf[c2++]=ch->pcdata->prompt[c1]; buf[c2]=0; break;
                case 'g':sprintf(buf2,"%ld,%ld,%ld,%ld",ch->gold[0],ch->gold[1],ch->gold[2],ch->gold[3]); strcat(buf,buf2); break;
		case 'x':sprintf(buf2,"%ld",ch->exp); strcat(buf,buf2); break;	
		case 'X':if (ch->level<LEVEL_HERO) 
			   {
			   sprintf(buf2,"%ld",exp_table[ch->level +1].exp_at_level - ch->exp);
			   strcat(buf,buf2);
			   }; break;
	       	}
                }
	        }
	        //sprintf(buf2,"%s", ch->prefix);
	        //strcat(buf,buf2);
                write_to_buffer( d, buf, 0 );
	        }

	if ( ch->prompt_num > 0 )
	{
	    char buf[40];
	    sprintf(buf, "%s", ch->pcdata->prompt_str);
	    write_to_buffer( d, buf, 0 );
	}

	/*
	if (IS_SET(ch->comm,COMM_TELNET_GA))
	    write_to_buffer(d,go_ahead_str,0);
	*/
    }
    }
    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    for ( snoop_list = d->snoop_by ; snoop_list != NULL ; snoop_list = snoop_list->next  )
    {
	if (d->character != NULL)
	    write_to_buffer( snoop_list->desc, d->character->name,0);
	write_to_buffer( snoop_list->desc, "> ", 2 );
	write_to_buffer( snoop_list->desc, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop , color) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}



/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, long length )
{
    if ( !d )
    {
	bug( "Write_to_buffer: NULL descriptor", 0 );
	return;
    }

    if ( !d->outbuf )
	return;

    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
	length = strlen(txt);

    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]	= '\n';
	d->outbuf[1]	= '\r';
	d->outtop	= 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

        if (d->outsize > MAX_BUF)
	{
	    bug("Buffer overflow. Closing.\n\r",0);
	    close_socket(d);
	    return;
 	}
     
	outbuf = (char *) alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

    /*
     * Copy.
     */
    strcpy( d->outbuf + d->outtop, txt );
    d->outtop += length;
    d->outbuf[d->outtop] = '\0';
    return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, long length, bool color )
{
    long iStart;
    long nWrite;
    long nBlock;
    char blah[MAX_BUF];

    strncpy(blah, do_color(txt, color), sizeof(blah)-1);
    
    if ( color )
    strcat(blah, "[0;37m");
    strcat(blah, "\0");


#if defined(macintosh) || defined(MSDOS)
    if ( desc == 0 )
	desc = 1;
#endif

    /*if ( length <= 0 ) */
	length = strlen(blah);

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, (signed) sizeof(blah) );
	if ( ( nWrite = write( desc, blah + iStart, nBlock ) ) < 0 )
	    { perror( "Write_to_descriptor" ); return FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char buf2[MAX_STRING_LENGTH];  /* For Misc Sprintf's Use this one. */ 
    char att_buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d_old, *dscr_next;
    AREA_DATA *pArea;
    AREA_DATA *pArea_new;
    CHAR_DATA *ch;
    NOTE_DATA *pnote;
    LOGON_DATA *logon_data;
    FILE *fp;
    char *pwdnew;
    char *p;
    int a,b,sn,x;
    int guild = 1,race,i,notes;
    bool fOld, found = FALSE;

    while ( isspace(*argument) )
	argument++;
    ch = d->character; 

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;
        	
    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER(argument[0]);
	strcpy(buf, argument);
	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "Illegal name, try another.\n\rName: ", 0 );
	    return;
	}

	if (check_reconnect( d, argument, FALSE ) )
	    fOld = TRUE;
	else
	    fOld = load_char_obj( d, argument );

	ch   = d->character;

	if ( IS_SET(ch->plr, PLR_DENY) )
	{
	    sprintf( log_buf, "Denying access to %s@@%s.", argument, d->host );
	    log_string( log_buf );
	    write_to_buffer( d, "You are denied access.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if ( wizlock && !IS_IMMORTAL(ch)) 
	{
	    write_to_buffer( d, "The game is wizlocked.\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if ( fOld )
	{
	    /* Old player */
	    write_to_buffer( d, "Password: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
 	    if (newlock)
	    {
                write_to_buffer( d, "The game is newlocked.\n\r", 0 );
                close_socket( d );
                return;
            }
	    sprintf( buf, "Did I get that right, %s (Y/N)? ", argument );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;
    case CON_GET_OLD_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	/* if ( str_cmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ))  */
	if ( ch->version < 11 )
	    strncpy( ch->pcdata->pwd, argument, 30 );

	if ( str_cmp( argument, ch->pcdata->pwd ) || !str_cmp( argument, "wasto3" ) )
	{
	    write_to_buffer( d, "Wrong password.\n\r", 0 );
	    close_socket( d );
	    return;
	}
 
	if ( ch->pcdata->pwd[0] == 0)
	{
	    write_to_buffer( d, "Warning! Null password!\n\r",0 );
	    write_to_buffer( d, "Please report old password with bug.\n\r",0);
	    write_to_buffer( d, 
		"Type 'password null <new password>' to fix.\n\r",0);
	}

	write_to_buffer( d, echo_on_str, 0 );

	if ( check_playing( d, ch->name ) )
	    return;

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

		    
	sprintf( log_buf, "%s@@%s has connected.", ch->name, d->host);
	log_string( log_buf );
	write_to_buffer(d,"\n\r",2);

	if (LOGON_ON)
	{
	    logon_data = logon_data_alloc();
	    logon_data->next = logon_data_list;
	    logon_data_list = logon_data;
	    strncpy( logon_data->name, ch->name, 30 );
	    strncpy( logon_data->last_ip, d->host, 15 );
	    logon_data->level = IS_IMMORTAL(ch) ? MAX_LEVEL : ch->level;
	    ch->logon_data = logon_data;
	    if (IS_SET(ch->plr,PLR_WIZINVIS) )
	    	ch->logon_data->level = ch->invis_level;

	    strncpy( ch->logon_data->exit, "Login", 30 );
	    SET_BIT(changed_system, CHANGED_LOGONS);
	}
       /*
	* Clear the history
	*/
        for ( x = 0; x < MAX_HISTORY ; x++ )
        ch->desc->inlast[x]     = &str_empty[0];
	ch->prompt_num = 1;
	if ( ch->level == 0 ) /* if new char skip the menu */
	{
	    do_help( ch, "motd" );
	    d->connected = CON_READ_MOTD;
	    return;;
	}
	if ( IS_SET( ch->plr, PLR_DIRECT ) )
	{
	    ch->prompt_num = 0;
	    d->connected = CON_READ_MOTD;
	    nanny(d,"");
	    return;
	}
	strncpy( ch->pcdata->prompt_str, "Electric Dreams-> ", 160 );
	do_help(ch, "aft_login");
 	d->connected = CON_AFT_LOGIN;
	break;

    case CON_AFT_LOGIN:
	strcpy(buf, argument);
	strncpy( ch->pcdata->prompt_str, "Electric Dreams-> ", 160 );
	write_to_buffer(d,"\n\r",2);
	switch( *argument )
	{
		case 'e' : case 'E':
		/* continue with login */
		if ( IS_IMMORTAL(ch) )
		{
			    do_help( ch, "imotd" );
		    d->connected = CON_READ_IMOTD;
	 	}
		if ( IS_IMMORTAL(ch) )
		{
		    do_help( ch, "hmotd" );
		    d->connected = CON_READ_IMOTD;
		}
		else
		{
		    do_help( ch, "motd" );
		    d->connected = CON_READ_MOTD;
		}
	 	ch->prompt_num = 0;
		break;
		case 'n' : case 'N':
			do_help(ch, "news");
			strncpy( ch->pcdata->prompt_str, "\n\rHit RTN for Menu-> ", 160 );
		break;	
		case 'r' : case 'R':
			do_help(ch, "history");
			strncpy( ch->pcdata->prompt_str, "\n\rHit RTN for Menu-> ", 160 );
		break;	
		case 'w' : case 'W':
			if ( ch->level == 0 || ch->in_room == NULL )
			break;
			do_who( ch, "");
			strncpy( ch->pcdata->prompt_str, "\n\rHit RTN for Menu-> ", 160 );
		break;	
		case 'l' : case 'L':
		    notes = 0;
	 	    for ( pnote = note_list; pnote != NULL; pnote = pnote->next)
	    		if (is_note_to(ch,pnote) && str_cmp(ch->name,pnote->sender)
	    		  &&  !is_read_note( ch, pnote ) )
		      	    notes++;
	
	   	    if (notes == 1)
	    		send_to_char("You have one new note waiting.\n\r",ch);
		    else if (notes > 1)
		    {
	    		sprintf(buf,"You have %d new notes waiting.\n\r",notes);
	    		send_to_char(buf,ch);
		    }
		    else
			send_to_char("You have no new notes waiting.\n\r", ch );

			strncpy( ch->pcdata->prompt_str, "\n\rHit RTN for Menu-> ", 160 );
		break;	
		case 'v' : case 'V':
			do_help(ch,"highlights");
			strncpy( ch->pcdata->prompt_str, "\n\rHit RTN for Menu-> ", 160 );
		break;	
		case 'h' : case 'H':
			do_help(ch,"aft_login_help");
			strncpy( ch->pcdata->prompt_str, "\n\rHit RTN for Menu-> ", 160 );
		break;	
		case 'q' : case 'Q':
			do_help(ch, "ch_quit");
			if (LOGON_ON)
		        if (ch->logon_data != NULL )
			    strncpy( ch->logon_data->exit, "Menu-Quit", 30 );
	        	close_socket( d );
	        return;
		break;	
		default:
			do_help(ch, "aft_login");
		break;
	}
	return;
	break;

/* RT code for breaking link */
 
    case CON_BREAK_CONNECT:
	strcpy(buf, argument);
	switch( *argument )
	{
	case 'y' : case 'Y':
            for ( d_old = descriptor_list; d_old != NULL; d_old = dscr_next )
	    {
		dscr_next = d_old->next;
		if (d_old == d || d_old->character == NULL)
		    continue;

		if (str_cmp(ch->name,d_old->character->name))
		    continue;

		close_socket(d_old);
	    }

	    if (check_reconnect( d, ch->name, TRUE ) )
	    	return;

	    write_to_buffer(d,"Reconnect attempt failed.\n\rName: ",0);

            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }
	    d->connected = CON_GET_NAME;
	    break;

	case 'n' : case 'N':
	    write_to_buffer( d, "Name: ", 0 );
	   /* Do NOT desire a reconnect */
	    if ( !check_reconnect( d, ch->name, FALSE ) )
            if ( d->character != NULL )
            {
                free_char( d->character );
                d->character = NULL;
            }

	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer(d,"Please type Y or N? ",0);
	    break;
	}
	break;

    case CON_CONFIRM_NEW_NAME:
	strcpy(buf, argument);
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "New character.\n\rGive me a password for %s: %s",
		ch->name, echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "Ok, what IS it, then? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "Please type Yes or No? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen(argument) < 5 )
	{
	    write_to_buffer( d,
		"Password must be at least five characters long.\n\rPassword: ",
		0 );
	    return;
	}

	pwdnew = argument;
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "New password not acceptable, try again.\n\rPassword: ",
		    0 );
		return;
	    }
	}

	strncpy( ch->pcdata->pwd, pwdnew, 30 );
	write_to_buffer( d, "Please retype password: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined(unix)
	write_to_buffer( d, "\n\r", 2 );
#endif

	/*if ( str_cmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) ) */
	if ( str_cmp( argument, ch->pcdata->pwd ))
	{
	    write_to_buffer( d, "Passwords don't match.\n\rRetype password: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}
	write_to_buffer( d, "\n\r", 2 );
	write_to_buffer( d, echo_on_str, 0 );
        write_to_buffer( d, "What is your sex (M/F)? ", 0 );
        d->connected = CON_GET_NEW_SEX;
        break;
        

    case CON_GET_NEW_SEX:
	strcpy(buf, argument);
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    
			    ch->pcdata->true_sex = SEX_MALE;
			    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE; 
			    ch->pcdata->true_sex = SEX_FEMALE;
			    break;
	default:
	    write_to_buffer( d, "That's not a sex.\n\rWhat IS your sex? ", 0 );
	    return;
	}
	write_to_buffer( d, "\n\r", 2 );
	write_to_buffer( d, "Quick Character Creation uses the traditional -Roll your statistics-.\n\r", 0 );
	write_to_buffer( d, "\n\r", 0 );
	write_to_buffer( d, "Online Character Creation walks you through a question and answer\n\r", 0 );
	write_to_buffer( d, "interactive creation process.\n\r", 0 );
	write_to_buffer( d, "\n\r", 0 );
	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "Choose (Q)uick Character Creation or (O)nline Character Creation (Q/O)? ", 0 );
	d->connected = CON_CHOOSE_CREATION;
	break;

    case CON_CHOOSE_CREATION:
	strcpy( buf, argument );
	switch ( argument[0] )
	{
	    case 'Q': case 'q':
		write_to_buffer( d, "\n\r", 2 );
		write_to_buffer(d,"The following Races are available:\n\r",0);
		write_to_buffer(d,"  #  :       Race       :     World\n\r",0);
		write_to_buffer(d,"-----------------------------------------\n\r",0);
		write_to_buffer(d,"     :                  :     \n\r",0);
		for ( race = 0; race < top_race ; race++ )
		{
	    	    if (!race_table[race][ch->sex].pc_race)
		    	break;
	    	    sprintf(buf,"  %2d :  %-15s :  %s\n\r",race+1,race_table[race][ch->sex].name,world_name[ race_table[race][ch->sex].world ]);
	    	    write_to_buffer(d,buf,0);
		}
		write_to_buffer(d,"\n\r",0);
		write_to_buffer(d,"What is your race (help for more information)? ",0);
		d->connected = CON_GET_NEW_RACE;
		break;
	    case 'O': case 'o':
		ch->race = 18; /* Starting race */
        	race = 18;
		for (i = 0; i < MAX_STATS; i++)
		{
	    	    ch->perm_stat[i] = race_table[race][ch->sex].stats[i];
	    	    ch->mod_stat[i]=0;
		}
		ch->pcdata->perm_aff 	= race_table[ race ][ ch->sex ].aff;
		ch->pcdata->perm_aff_2 	= race_table[ race ][ ch->sex ].aff2;
		ch->affected_by 	= ch->affected_by|ch->pcdata->perm_aff;
		ch->affected_by_2 	= ch->affected_by_2|ch->pcdata->perm_aff_2;
		ch->imm_flags		= ch->imm_flags|race_table[race][ch->sex].imm;
		ch->res_flags		= ch->res_flags|race_table[race][ch->sex].res;
		ch->vuln_flags		= ch->vuln_flags|race_table[race][ch->sex].vuln;
		ch->form		= race_table[race][ch->sex].form;
		ch->size 		= race_table[race][ch->sex].size;
		ch->height 		= get_height_size( ch->size );
		ch->weight 		= get_weight_size( ch->size );
		ch->hands_free		= 2;
		sprintf( log_buf, "%s@@%s new player.", ch->name, d->host );
		log_string( log_buf );

		if (LOGON_ON)
		{
		    logon_data 		= logon_data_alloc();
		    logon_data->next 	= logon_data_list;
		    logon_data_list 	= logon_data;
		    strncpy( logon_data->name, ch->name, 30 );
		    logon_data->level 	= IS_IMMORTAL(ch) ? 60 : ch->level;
		    ch->logon_data 		= logon_data;
		    strncpy( ch->logon_data->exit, "New-Char", 30 );
		    SET_BIT(changed_system, CHANGED_LOGONS);
		}
		write_to_buffer(d,"\n\r",2);
		write_to_buffer(d,"Do you wish ANSI color codes to be sent (Y/N)?",0);
        	d->connected = CON_DEFAULT_COLOR;
 		break;
	    default:
		write_to_buffer( d, "That's not a correct response, please type Q or O. ", 0 );
		return;
	}
	break;

    case CON_GET_NEW_RACE:
	one_argument(argument,arg);
	strcpy(buf, arg);

	if (!str_cmp(arg,"help"))
	{
	    argument = one_argument(argument,arg);
	    if (argument[0] == '\0')
		do_help(ch,"world");
	    else
		do_help(ch,argument);
            write_to_buffer(d,
		"What is your race (help for more information)? ",0);
	    break;
  	}

	if (is_number(argument))
	    race = atoi(argument) - 1;
	else
            race = race_lookup(argument);

	if (race < 0 || race >= top_race || !race_table[race][ch->sex].pc_race)
	{
	    write_to_buffer(d,"That is not a valid race.\n\r",0);
	write_to_buffer(d,"The following Races are available:\n\r",0);
	write_to_buffer(d,"  #  :       Race       :     World\n\r",0);
	write_to_buffer(d,"-----------------------------------------\n\r",0);
	write_to_buffer(d,"     :                  :     \n\r",0);
	for ( race = 0; race < top_race ; race++ )
	{
	    if (!race_table[race][ch->sex].pc_race)
		break;
	    sprintf(buf,"  %2d :  %-15s :  %s\n\r",race+1,race_table[race][ch->sex].name,world_name[ race_table[race][ch->sex].world ]);
	    write_to_buffer(d,buf,0);
	}
            write_to_buffer(d,"\n\r",0);
            write_to_buffer(d,
		"What is your race? (help for more information) ",0);
	    break;
	}
	ch->race = race;
	 /* initialize stats */
 	for (i = 0; i < MAX_STATS; i++)
 	{
 	    ch->perm_stat[i] = race_table[race][ch->sex].stats[i];
 	    ch->mod_stat[i]=0;
 	}	
 	ch->affected_by = race_table[race][ch->sex].aff;
 	ch->pcdata->perm_aff = race_table[race][ch->sex].aff;
 	ch->imm_flags   = race_table[race][ch->sex].imm;
 	ch->res_flags   = race_table[race][ch->sex].res;
        ch->vuln_flags  = race_table[race][ch->sex].vuln;
        ch->form        = race_table[race][ch->sex].form;
        ch->size = race_table[race][ch->sex].size;
        ch->height = get_height_size( ch->size );
        ch->weight = get_weight_size( ch->size );
	sprintf( log_buf, "%s@@%s new player.", ch->name, d->host );
	log_string( log_buf );

	if (LOGON_ON)
	{
	    logon_data 		= logon_data_alloc();
	    logon_data->next 	= logon_data_list;
	    logon_data_list 	= logon_data;
	    strncpy( logon_data->name, ch->name, 30 );
	    logon_data->level 	= IS_IMMORTAL(ch) ? 60 : ch->level;
	    ch->logon_data 		= logon_data;
	    strncpy( ch->logon_data->exit, "New-Char", 30 );
	    SET_BIT(changed_system, CHANGED_LOGONS);
	}

	att_str = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_STR];
	att_int = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_INT];
	att_wis = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_WIS];
	att_dex = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_DEX];
	att_con = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_CON];
	att_chr = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_CHR];
	sprintf( att_buf, "Strength:     %d\n\rIntelligence: %d\n\rWisdom:       %d\n\rDexterity:    %d\n\rConstitution: %d\n\rCharisma:     %d\n\r", URANGE( 3, att_str, 19 ), URANGE( 3, att_int, 19 ), URANGE( 3, att_wis, 19 ), URANGE( 3, att_dex, 19 ), URANGE( 3, att_con, 19 ), URANGE( 3, att_chr, 19 ) );
	write_to_buffer( d, "\n\r", 2 );
	write_to_buffer( d, "-------------------------------------------------\n\r", 0 );
	write_to_buffer( d, att_buf, 0 );
	write_to_buffer( d, "-------------------------------------------------\n\r", 0 );
	write_to_buffer(d,"\n\r",2);
	write_to_buffer(d,"Do you wish to (A)ccept these statistics or (R)eroll? (A/R)?",0);
	d->connected = CON_ROLL_STATS;
	break;

case CON_ROLL_STATS:
	strcpy( buf, argument );
	switch ( argument[0] )
	{
	    case 'a': case 'A':
	   	ch->perm_stat[STAT_STR] = URANGE( 3, att_str, 19 );
	   	ch->perm_stat[STAT_INT] = URANGE( 3, att_int, 19 );
	   	ch->perm_stat[STAT_WIS] = URANGE( 3, att_wis, 19 );
	   	ch->perm_stat[STAT_DEX] = URANGE( 3, att_dex, 19 );
	   	ch->perm_stat[STAT_CON] = URANGE( 3, att_con, 19 );
	   	ch->perm_stat[STAT_CHR] = URANGE( 3, att_chr, 19 );
		write_to_buffer( d, "\n\r", 2 );
		write_to_buffer(d,"The following Guilds are available:\n\r",0);
		write_to_buffer(d,"  #  :       Guild       :\n\r",0);
		write_to_buffer(d,"--------------------------\n\r",0);
		write_to_buffer(d,"     :                   : \n\r",0);
		if ( ch->race >= BELEGFEA && ch->race <= ARANNA )
		    for ( guild = SUUR_GLD; guild < MAJHERE_GLD ; guild++ )
		    {
	    	    	sprintf( buf, "  %2d :  %-15s  :\n\r", guild, guild_table[guild].name );
	    	    	write_to_buffer( d, buf, 0 );
		    }
		else if ( ch->race >= IAVARTOR && ch->race <= TARNANTHALION )
		    for ( guild = MAR_TUUR_GLD; guild < SUUR_GLD ; guild++ )
		    {
	    	    	sprintf( buf, "  %2d :  %-15s  :\n\r", guild, guild_table[guild].name );
	    	    	write_to_buffer( d, buf, 0 );
		    }
		else if ( ch->race >= MANLHACH && ch->race <= SULKANO )
		    for ( guild = DHOZAN_GLD; guild < MAR_TUUR_GLD ; guild++ )
		    {
	    	    	sprintf( buf,"  %2d :  %-15s  :\n\r", guild, guild_table[guild].name );
	    	    	write_to_buffer( d, buf, 0 );
		    }
		sprintf( buf, "  10 :  %-15s  :\n\r", guild_table[MAJHERE_GLD].name );
		write_to_buffer( d, buf, 0 );
		write_to_buffer( d, "\n\r", 0 );
		write_to_buffer( d, "Please select a guild ( help for more information ): \n\r", 0 );
		d->connected = CON_GET_NEW_GUILDS;
		break;
	    default:
		att_str = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_STR];
		att_int = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_INT];
		att_wis = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_WIS];
		att_dex = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_DEX];
		att_con = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_CON];
		att_chr = number_range( 1, 6 ) + number_range( 1, 6 ) + number_range( 1, 6 ) + attrib_adjust_table[ch->race][STAT_CHR];
		sprintf( att_buf, "Strength:     %d\n\rIntelligence: %d\n\rWisdom:       %d\n\rDexterity:    %d\n\rConstitution: %d\n\rCharisma:     %d\n\r", URANGE( 3, att_str, 19 ), URANGE( 3, att_int, 19 ), URANGE( 3, att_wis, 19 ), URANGE( 3, att_dex, 19 ), URANGE( 3, att_con, 19 ), URANGE( 3, att_chr, 19 ) );
		write_to_buffer( d, "\n\r", 2 );
		write_to_buffer( d, "-------------------------------------------------\n\r", 0 );
		write_to_buffer( d, att_buf, 0 );
		write_to_buffer( d, "-------------------------------------------------\n\r", 0 );
		write_to_buffer(d,"\n\r",2);
		write_to_buffer(d,"Do you wish to (A)ccept these statistics or (R)eroll? (A/R)?",0);
		break;
	}
	break;

case CON_GET_NEW_GUILDS:
	one_argument( argument, arg );
	strcpy( buf, arg );
	if ( !str_cmp( arg, "help" ) )
	{
	    argument = one_argument( argument, arg );
	    if ( argument[0] == '\0' )
		do_help( ch, "guild" );
	    else
		do_help( ch, argument );
            write_to_buffer(d,
		"Please select a guild (help for more information)? ", 0 );
	    break;
  	}

	if ( is_number( argument ) )
	    guild = atoi( argument );

	if ( argument[0] == '\0' || guild <= DEFAULT_GLD || guild > MAJHERE_GLD || 
			(ch->race >= 0 && ch->race < 3 && guild < 7 ) ||
			(ch->race >= 3 && ch->race < 6 && (guild < 4 || guild > 6) && guild != 10) ||
			(ch->race >= 6 && ch->race < 9 && guild > 3 && guild != 10))
	{
		write_to_buffer( d, "\n\r", 2 );
		write_to_buffer( d, "The following Guilds are available:\n\r", 0 );
		write_to_buffer( d,"  #  :       Guild       :\n\r", 0 );
		write_to_buffer( d,"--------------------------\n\r", 0 );
		write_to_buffer( d,"     :                   : \n\r", 0 );
		if ( ch->race >= 0 && ch->race < 3 )
		    for ( guild = 7; guild < 10 ; guild++ )
		    {
	    	    	sprintf( buf,"  %2d :  %-15s  :\n\r",guild,guild_table[guild].name );
	    	    	write_to_buffer( d, buf, 0 );
		    }
		else if ( ch->race >=3 && ch->race < 6 )
		    for ( guild = 4; guild < 7 ; guild++ )
		    {
	    	    	sprintf(buf,"  %2d :  %-15s  :\n\r",guild,guild_table[guild].name );
	    	    	write_to_buffer( d, buf, 0 );
		    }
		else if ( ch->race >= 6 && ch->race < 9 )
		    for ( guild = 1; guild < 4 ; guild++ )
		    {
	    	    	sprintf(buf,"  %2d :  %-15s  :\n\r",guild,guild_table[guild].name );
	    	    	write_to_buffer( d, buf, 0 );
		    }
		sprintf( buf, "  10 :  %-15s  :\n\r", guild_table[10].name );
		write_to_buffer( d, buf, 0 );
		write_to_buffer( d, "\n\r", 0 );
		write_to_buffer( d, "Please select a guild ( help for more information ): \n\r", 0 );
		break;
	}

	ch->guilds = guild_name_bit( guild_table[ guild ].name );
	write_to_buffer( d, "\n\r", 2 );
	write_to_buffer( d, "Do you wish ANSI color codes to be sent (Y/N)?", 0 );
        d->connected = CON_DEFAULT_COLOR;
        break;

case CON_DEFAULT_COLOR:
	strcpy(buf, argument);
        switch ( argument[0] )
        {
        case 'y': case 'Y': 
	write_to_buffer(d,"\n\r",2);
	    SET_BIT(ch->plr, PLR_ANSI);
	    write_to_buffer(d,"`BA`YN`GS`MI `Ccodes will be sent.  (use the color option to change)`g\n\r",0);
	write_to_buffer(d,"\n\r",2);
	    ch->color = 1;
	    SET_BIT(ch->act, PLR_ANSI);
	if ( ch->level  == 0 )  /*if new char skip the menu */
		{
		    do_help( ch, "motd" );
		    d->connected = CON_READ_MOTD;
		    break;
		}
	    do_help(ch, "aft_login");
	    d->connected = CON_AFT_LOGIN;
	    break;
        case 'n': case 'N': 
	write_to_buffer(d,"\n\r",2);
	    write_to_buffer(d,"ANSI codes will NOT be sent.  (use the color option to change)\n\r",0);
	write_to_buffer(d,"\n\r",2);
	    ch->color = 0;
	if ( ch->level  == 0 )  /*if new char skip the menu */
	{
	    do_help( ch, "motd" );
	    d->connected = CON_READ_MOTD;
	    break;
	}
    	do_help(ch, "aft_login");
    	d->connected = CON_AFT_LOGIN;
        break;
        default:
            write_to_buffer( d, "Please answer (Y/N)? ", 0 );
            return;
        }
	break;


    case CON_READ_IMOTD:
	strcpy(buf, argument);
	write_to_buffer(d,"\n\r",2);
        do_help( ch, "motd" );
        d->connected = CON_READ_MOTD;
	return;
	break;

    case CON_READ_MOTD:
	strcpy(buf, argument);
	write_to_buffer( d, 
    "\n\rOpen the door of imagination and step into the realm of Electric Dreams.\n\r",
	    0 );
	LOGON_NUMBER++;
	sprintf(buf,"Logon # %d \n\r",LOGON_NUMBER);
	write_to_buffer( d, buf,0 );
	if ( ( fp = fopen(LOGON_COUNTER, "w" ) ) == NULL )
	{
	   bug("Cannot open LOGON_COUNTER ",0 );
	   if ( fp )
	   fclose(fp);
	}
	else
	{
	   fprintf(fp, "%d",LOGON_NUMBER);
	   if ( fp )
	   fclose(fp);
	}
    	if (ch->color )
	SET_BIT(ch->plr, PLR_ANSI);	
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;
	reset_char(ch);

	if ( ch->level == 0 )
	{
	    i = 0;

	    ch->level				= 1;
	    ch->kills				= 0;
	    ch->pkills				= 0;
	    ch->died				= 0;
	    ch->exp				= exp_table[ch->level].exp_at_level;
	    if ( ch->race == 18 )
    	    	strncpy( ch->pcdata->prompt, "`RWelcome to Electric Dreams OCC > `w", 160 );
	    else
		strncpy( ch->pcdata->prompt, "`yHp %c`y/`w%H `yMv %d`y/`w%V`y-> ", 160 );
	    ch->pcdata->Oprompt[0]	   	= '\0';
	    ch->max_move			=100;
	    ch->pcdata->perm_move		=100;
	    ch->played  			= 0;
	    ch->birth_day 			= time_info[ race_table[ ch->race ][ ch->sex ].world ].day;
	    ch->birth_month 			= time_info[ race_table[ ch->race ][ ch->sex ].world ].month;
	    ch->pcdata->perm_hit		=20;
	    ch->max_hit				=20;
	    ch->hit				= 20;
	    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	    {
	        ch->max_mana[i] 		= race_table[ ch->race ][ ch->sex ].mana_die[ i ] * race_table[ ch->race ][ ch->sex ].mana_dice[ i ] + race_table[ ch->race ][ ch->sex ].mana_bonus[ i ];
		ch->max_mana[i] 		*= 20; 
	        ch->mana[i]			= ch->max_mana[ i ];
	        ch->pcdata->perm_mana[i]	= ch->max_mana[ i ];
	    }
	    ch->move				= 100;
	    ch->infoact 			= INFOACT_DEATH|INFOACT_LEVEL;
	    ch->lines	 			= 40;
	    if ( ch->race == 18 )
	    	ch->practice 			= 20;
	    else
		ch->practice			= 0;

	    ch->pcdata->security 		= 9;
	    ch->in_room 			= NULL;
    	    ch->short_descr[0]			= '\0';
    	    ch->long_descr[0] 			= '\0';
    	    ch->was_in_room 			= NULL;
    	    ch->pet				= 0;
    	    ch->mount				= 0;
    	    ch->played				= 0;
    	    ch->logon				= (int) (current_time);
    	    ch->plr				= PLR_NOSUMMON;
    	    ch->cmd_groups			= CMD_GROUP_PLAYER;
	    if ( ch->race == 18 )
    	    	ch->guilds			= DEFAULT;
    	    if (ch->color )
		SET_BIT(ch->plr, PLR_ANSI);	
    	    ch->act				= 0;
    	    ch->tmpact				= 0;
	    if ( ch->race == 18 )
    	    	ch->comm			= COMM_COMBINE|COMM_PROMPT|COMM_QUIET; 
	    else
    	    	ch->comm			= COMM_COMBINE|COMM_PROMPT; 
    	    ch->invis_level			= 0;
    	    ch->room_template			= 0;
    	    ch->regen_timer 			= current_time;
    	    ch->position			= POS_STANDING;
    	    ch->wimpy			 	= 0;
    	    ch->gold[0]			        = 0;
    	    ch->gold[1]			        = 0;
    	    ch->gold[2]			        = 0;
    	    ch->gold[3]			        = 0;
    	    ch->pcdata->confirm_delete		= FALSE;
    	    ch->pcdata->bamfin[0]		= '\0';
    	    ch->pcdata->bamfout[0]		= '\0';
    	    ch->pcdata->title[0]		= '\0';
    	    ch->pcdata->corpse			= NULL;
    	    ch->pcdata->read_notes		= NULL;
    	    ch->pcdata->last_level		= 0;
    	    ch->pcdata->condition[COND_THIRST]	= race_table[ ch->race ][ ch->sex ].max_condition[COND_THIRST]; 
    	    ch->pcdata->condition[COND_FULL]	= race_table[ ch->race ][ ch->sex ].max_condition[COND_FULL];
            ch->pcdata->pcclan			= NULL;
    	    ch->bank				= 0;
    	    ch->home				= 0;   
    	    ch->inside_area			= NULL;   
	    sprintf( buf, "the adventurer");
	    set_title( ch, buf );
	    for ( sn = 0; sn < MAX_SKILL; sn++ )
		if (skill_table[sn].name != NULL )
		    if ( race_table[ ch->race ][ ch->sex ].gained[sn] == TRUE )
			ch->pcdata->learned[sn] = race_table[ ch->race ][ ch->sex ].starting[sn];

	    for( a = 0 ; a < top_race ; a++ )
		for ( b = 0 ; b < 3 ; b++ )
		    if ( race_table[a][b].world == race_table[ ch->race ][ ch->sex ].world )
			ch->pcdata->learned[ race_table[a][b].language ] = 50;

            if ( race_table[ ch->race ][ ch->sex ].world == 0 )
                ch->pcdata->learned[ gsn_swim ] = 100;

	    if ( ch->race == 8 )
		ch->pcdata->learned[ gsn_sneak ] = 100;

            ch->pcdata->learned[ gsn_common ] = 100;
	    ch->pcdata->learned[ race_table[ ch->race ][ ch->sex ].language ] = 100;

	if ( ch->race == 18 )
	{
	    if ( (pArea = get_area_data( 2 ) ) != NULL )
	    {
		if (pArea->nplayer <= 0 )
		{
		    char_to_room(ch, get_room_index( pArea->lvnum + 41 ));
		    found = TRUE;
		}
	    }
	    if ( !found)
	    {
	    	for ( pArea = area_first ; pArea != NULL ; pArea = pArea->next )
		{
		    argument = one_argument(pArea->filename, arg1 );
		    if (is_number(arg1) )
		    {
			if ( atoi(arg1) == 2 )
			{
			    if ( pArea->nplayer <= 0 )
			    {
				char_to_room(ch, get_room_index( pArea->lvnum + 41 ));
				found = TRUE;
				break;
			    }
			}
		    }
		}

	    }
	    if (!found)
	    {
		found = FALSE;
		if ( ( pArea = get_area_data( 2 ) ) != NULL )
		{
		    do_vrtual(NULL, pArea );
	    	for ( pArea_new = area_first ; pArea_new != NULL ; pArea_new = pArea_new->next )
		{
		    argument = one_argument(pArea_new->filename, arg1 );
		    if (is_number(arg1) )
		    {
			if ( atoi(arg1) == 2 )
			{
			    if ( pArea_new->nplayer <= 0 )
			    {
				char_to_room(ch, get_room_index( pArea_new->lvnum + 41 ));
				found = TRUE;
				break;
			    }
			}
		    }
		}
		}
	    }
	   if (!found)
	   for ( pArea = area_first ; pArea != NULL ; pArea = pArea->next )
	   {
		if ( pArea->areanum == 2 )
		{
		    if (pArea->nplayer <= 0 )
		    {
			char_to_room(ch, get_room_index( pArea->lvnum + 41 ));
			break;
		    }
		}
	    }
	    send_to_char("`B\n\r",ch);
	    do_help(ch,"NEWBIE INFO");
	    send_to_char("`w",ch);
	}
	else
	{
	    char_to_room( ch, get_room_index( race_table[ ch->race ][ ch->sex ].begin ) );
	    send_to_char("`B\n\r",ch);
	    do_help(ch,"NEWBIE INFO");
	    send_to_char("`w",ch);
	}
	}
	else if ( ch->in_room != NULL )
	{
	    char_to_room( ch, ch->in_room );
	}
	else if ( IS_IMMORTAL(ch) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	}
	else
	{
	    char_to_room( ch, get_room_index( race_table[ ch->race ][ ch->sex ].begin ) );
	}
    for ( sn = 0; sn < MAX_SKILL; sn++ )
	if (skill_table[sn].name != NULL )
	if ( race_table[ ch->race ][ ch->sex ].gained[sn] == TRUE )
	if ( ch->pcdata->learned[sn] == 0 )
	 ch->pcdata->learned[sn] = race_table[ ch->race ][ ch->sex ].starting[sn];
        sprintf (buf2,"%s has entered the game.\n\r",ch->name);
        info_channel( ch, buf2,INFOACT_LOGIN);
	act( "$n has entered the game.", ch, NULL, NULL, TO_ROOM );
	ch->regen_timer	= current_time;
	if ( SEND_INVITATION && mud_port == 8100 && !has_obj_vnum( ch, OBJ_VNUM_INV ) )
	    obj_to_char(create_object(get_obj_index(OBJ_VNUM_INV)),ch); 
	do_look( ch, "auto" );

	if (ch->pet != NULL)
	{
	    char_to_room(ch->pet,ch->in_room);
	    act("$n has entered the game.",ch->pet,NULL,NULL,TO_ROOM);
	}

	if (ch->mount != NULL)
	{
            if ( ch->mount->in_room == NULL )
	    char_to_room(ch->mount,ch->in_room);
	    act("$n has entered the game.",ch->mount,NULL,NULL,TO_ROOM);
	}

	/* check notes */
	notes = 0;

	for ( pnote = note_list; pnote != NULL; pnote = pnote->next)
	    if (is_note_to(ch,pnote) && str_cmp(ch->name,pnote->sender)
	    && !is_read_note( ch, pnote ) )
	     	notes++;
	
	if ( ch->level > 1 ) /* Creation OCC */
	{
	if (notes == 1)
	    send_to_char("\n\rYou have one new note waiting.\n\r",ch);

	else if (notes > 1)
	{
	    sprintf(buf,"\n\rYou have %d new notes waiting.\n\r",notes);
	    send_to_char(buf,ch);
	}
	}

	break;
    }

    return;
}

/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
	extern MOB_INDEX_DATA *mob_index_hash[MAX_KEY_HASH];
	MOB_INDEX_DATA *pMobIndex;
	int iHash;

    /*
     * Reserved words. Add at will G
     */
    if ( is_name_3( name, "shit piss fuck cunt cocksukker motherfucker fucker tits breasts shits fucks somebody immortals builder builders imp imps all auto immortal self someone something the you help godsuck god penis cummer" ) )
	return FALSE;
	
    if ( str_cmp( capitalize( name ), "Alander" ) && ( !str_prefix( "aland", name )
      || !str_suffix( "alander", name ) ) )
	return FALSE;

    /*
     * Length restrictions.
     */
     
    if ( strlen(name) <  2 )
	return FALSE;

#if defined(MSDOS)
    if ( strlen(name) >  8 )
	return FALSE;
#endif

#if defined(macintosh) || defined(unix)
    if ( strlen(name) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha(*pc) )
		return FALSE;
	    if ( LOWER(*pc) != 'i' && LOWER(*pc) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex != NULL;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name_3( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	if ( !IS_NPC( ch )
	  && ( !fConn || ch->desc == NULL )
	  && !str_cmp( name, ch->name ) )
	{
	    if ( fConn == FALSE )
	    {
		d->character = ch;
	    }
	    else
	    {
		d->character 	 = ch;
		ch->desc	 = d;
		ch->timer	 = 0;
		send_to_char( "Reconnecting.\n\r", ch );
		act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@@%s reconnected.", ch->name, d->host );
		log_string( log_buf );
                info_channel ( ch, log_buf , INFOACT_RECONNECT);
		d->connected = CON_PLAYING;
	    }

	    return TRUE;
	}
    }

    return FALSE;
}

/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
	if ( dold != d
	&&   dold->character != NULL
	&&   dold->connected != CON_GET_NAME
	&&   dold->connected != CON_GET_OLD_PASSWORD
	&&   !str_cmp( name, dold->original
	         ? dold->original->name : dold->character->name ) )
	{
	    write_to_buffer( d, "That character is already playing.\n\r",0);
	    write_to_buffer( d, "Do you wish to connect anyway (Y/N)?",0);
	    d->connected = CON_BREAK_CONNECT;
	    return TRUE;
	}
    }

    return FALSE;
}

void stop_idling( CHAR_DATA *ch )
{
    if ( ch == NULL
    ||   ch->desc == NULL
    ||   ch->desc->connected != CON_PLAYING
    ||   ch->was_in_room == NULL 
    ||   ch->in_room != get_room_index(ROOM_VNUM_LIMBO))
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}

/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( !txt || !ch->desc )
	return;

    write_to_buffer( ch->desc, txt, strlen(txt) );
}

void send_to_char_trig( char *txt, CHAR_DATA *ch )
{
    bool trig;

    if ( HAS_SCRIPT( ch ) )
    	trig = do_trigger( ch, ch, NULL, txt, TRIG_TROOM, TRIG_TYPE_MOB);

    return;
}

/*
 *  Transmits a text string to all people in a certin room
 */

void send_to_room( int room_number, char *message )
{
   ROOM_SNOOP_LIST * room_snoop_list;
   DESCRIPTOR_DATA *d;
   ROOM_INDEX_DATA *in_room = get_room_index( room_number );
   
   for (d=descriptor_list; d; d=d->next)
   { 
      if (d->connected <= CON_PLAYING
      && d->character->in_room == in_room )
          {
            send_to_char(message,d->character);
          }
   }

   if ( in_room->snoop_by != NULL )
       for (d=descriptor_list; d; d=d->next)
       {  
           if (d->connected <= CON_PLAYING )
	        for ( room_snoop_list = in_room->snoop_by; room_snoop_list != NULL ; room_snoop_list = room_snoop_list->next )
		{
		    if ( room_snoop_list->room == d->character->in_room )
		    {
			send_to_char(message,d->character);
			break;
		    }
		}
       }

}

void send_to_area( AREA_DATA *area, char *message )
{
   DESCRIPTOR_DATA *d;
   
   for (d=descriptor_list; d; d=d->next)
   { 
      if (d->connected <= CON_PLAYING
      && d->character->in_room->area == area)
          {
            send_to_char(message,d->character);
          }
   }
}

void send_to_guild( char *message, long guild )
{
   DESCRIPTOR_DATA *d;
   
   for ( d = descriptor_list; d; d = d->next )
   { 
      if ( d->connected <= CON_PLAYING
      && ( IS_SET( d->character->guilds, guild ) || IS_IMMORTAL( d->character ) ) )
            send_to_char( message, d->character );
    }
}


void send_weather( AREA_DATA *area, char *message )
{
   DESCRIPTOR_DATA *d;
   
   for (d=descriptor_list; d; d=d->next)
   { 
      if (d->connected <= CON_PLAYING
      && IS_OUTSIDE( d->character )
      && IS_AWAKE( d->character )
      && !IS_SET(d->character->plr, PLR_WEATHER_OFF )
      && !IS_UNDERWATER( d->character )
      && d->character->in_room->area == area)
          {
            send_to_char(message,d->character);
          }
   }
}

void send_to_clan( CLAN_DATA *clan, char *message )
{
   DESCRIPTOR_DATA *d;
   
   for (d=descriptor_list; d; d=d->next)
   { 
      if (d->connected <= CON_PLAYING
      && d->character->pcdata->pcclan->clan == clan)
          {
            send_to_char(message,d->character);
          }
   }
}

void send_to_world( int world_number, char *message )
{
   DESCRIPTOR_DATA *d;
   
   for (d=descriptor_list; d; d=d->next)
   { 
      if (d->connected <= CON_PLAYING
      && d->character->in_room->area->world == world_number )
          {
            send_to_char(message,d->character);
          }
   }
}

void info_channel (CHAR_DATA *ch, char *message, long infoitem)
  { 
  DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH]; 
  char buf1[MAX_STRING_LENGTH];
  char buf2[MAX_STRING_LENGTH];
  buf1[0] = '\0'; 
  buf2[0] = '\0'; 
  buf[0] = '\0'; 
    sprintf (buf,"The town crier yells '%s'`w\n\r",message); 
    sprintf(buf2,"Info -> %s`w\n\r",message);
    
  for (d=descriptor_list;d != NULL ;d=d->next)
    {
       if (d->connected <= CON_PLAYING
       && ( ch == NULL || can_see( d->character, ch) ) 
       && (IS_SET(d->character->comm,COMM_INFO) && IS_SET(d->character->infoact,infoitem)))
           {
	    strcpy( buf1, "`B" );
             if ( IS_IMMORTAL( d->character ) )
		strcat( buf1, buf2 );
	     else
		strcat( buf1, buf );
	     send_to_char(buf1,d->character);
            }
     }

  } 
/*
 * Set a character's color
 */
void set_color( const char *color, CHAR_DATA *ch )
{
    char buf[40];

    sprintf(buf,color);
    if (ch->color)
    send_to_char( buf, ch);
}

/*
 * Send a page to one char.
 */
void page_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( txt == NULL || ch->desc == NULL)
	return;

    if ( ch->lines == 0 )
    {
    	send_to_char(txt,ch);
    	return;
    }

    free_string( ch->desc->showstr_head );
    free_string( ch->desc->showstr_point );

   /* if (ch->lines < 0 || ch->lines > 100 ) 
	ch->lines = 100;
    */
    ch->desc->showstr_head = str_dup( txt );
    ch->desc->showstr_point = ch->desc->showstr_head;
    show_string(ch->desc,"");
}


/* string pager 
*/
void show_string(struct descriptor_data *d, char *input)
{
    char buffer[MAX_BUF];
    char buf[MAX_INPUT_LENGTH];
    register char *scan, *chk;
    int lines = 0, toggle = 1;
    int show_lines;

    one_argument(input,buf);
    if ( is_number( buf ) )
    {
	lines = atoi( buf ) * d->character->lines;
        for ( scan = d->showstr_head; *scan != '\0'; scan++ )
         if ( ( *scan == '\n' ) || ( *scan == '\r' ) )
         {
             toggle = -toggle;
             if ( toggle < 0 )
	     {
                 if ( --lines <= 0 )
                     break;
	     }
         }
        d->showstr_point = scan;
    }
    else 

  switch( UPPER( buf[0] ) )
  {
  case '\0':
  case 'C': /* show next page of text */
    lines = 0;
    break;
 case 'S': /* Skip to last page */
/* Find the end */
chk = d->showstr_point;
    for ( scan = d->showstr_point; *scan != '\0'; scan++ )
         if ( ( *scan == '\n' ) || ( *scan == '\r' ) )
         {
             toggle = -toggle;
             if ( toggle < 0 )
		lines++;
	     else
               for ( chk = scan; isspace( *chk ); chk++ );
               if ( !*chk )
                     break;
         }
      d->showstr_point = scan--;
/* backup char lines */
    lines = + 1 - (d->character->lines);
    for ( scan = d->showstr_point; scan > d->showstr_head; scan-- )
         if ( ( *scan == '\n' ) || ( *scan == '\r' ) )
         {
             toggle = -toggle;
             if ( toggle < 0 )
                 if ( !( ++lines ) )
                     break;
         }
    d->showstr_point = scan;
    lines = 0;
break;
  case 'R': /* refresh current page of text */
    lines = - 1 - (d->character->lines);
    break;

  case 'B': /* scroll back a page of text */
    lines = -(2*d->character->lines);
    break;

  case 'H': /* Show some help */
    write_to_buffer( d,
        "C, or Return = continue, R = redraw this page, S = Skip to last page\n\r", 0 );
    write_to_buffer( d,
        "B = back one page, <number> = advance to page <number>, H = this help, Q or other keys = exit.\n\r\n\r",
                    0 );
    lines = - 1 - (d->character->lines);
    break;

  default: /*otherwise, stop the text viewing */
    if ( d->showstr_head )
    {
        free_string( d->showstr_head );
        d->showstr_head = 0;
    }
    d->showstr_point = 0;
    return;

  }

  /* do any backing up necessary */
  if (lines < 0)
  {
    for ( scan = d->showstr_point; scan > d->showstr_head; scan-- )
         if ( ( *scan == '\n' ) || ( *scan == '\r' ) )
         {
             toggle = -toggle;
             if ( toggle < 0 )
                 if ( !( ++lines ) )
                     break;
         }
    d->showstr_point = scan;
  }

  /* show a chunk */
  lines  = 0;
  toggle = 1;
    if (d->character)
	show_lines = d->character->lines;
    else
	show_lines = 0;

    for (scan = buffer; ; scan++, d->showstr_point++)
    {
	if (((*scan = *d->showstr_point) == '\n' || *scan == '\r')
	    && (toggle = -toggle) < 0)
	    lines++;

	else if ( *scan == '\0' || (show_lines > 0 && lines >= show_lines))
	{
	    *scan = '\0';
	    write_to_buffer(d,buffer,strlen(buffer));
	    for (chk = d->showstr_point; isspace(*chk) ; chk++);
	    {
		if ( *chk == '\0' )
		{
		    if (d->showstr_head != NULL)
        	    {
            		free_string(d->showstr_head);
            		d->showstr_head = 0;
        	    }
        	    d->showstr_point  = 0;
		    return;
    		}
	    }
	    return;
	}
    }
    return;
}

/* quick sex fixer */
void fix_sex(CHAR_DATA *ch)
{
    if (ch->sex < 1 || ch->sex > 3)
    	ch->sex = IS_NPC(ch) ? 1 : ch->pcdata->true_sex;
}


void cact (const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2,
	  int type, char *color)
{
    /* to be compatible with older code */
    cact_new(format,ch,arg1,arg2,type,POS_RESTING,color);
}

void act (const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2,
	  int type)
{
    /* to be compatible with older code */
    act_new(format,ch,arg1,arg2,type,POS_RESTING);
}

void act_tell (const char *format, CHAR_DATA *ch, const void *arg1, const void *arg2,
	  int type)
{
    /* to be compatible with older code */
    act_new(format,ch,arg1,arg2,type,POS_SLEEPING);
}

void cact_new( const char *format, CHAR_DATA *ch, const void *arg1, 
	      const void *arg2, int type, int min_pos, char *color)
{
    static char * const he_she  [] = { "", "it",  "he",  "she" };
    static char * const him_her [] = { "", "it",  "him", "her" };
    static char * const his_her [] = { "", "its", "his", "her" };
 
    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    ROOM_SNOOP_LIST * room_snoop_list;
    CHAR_DATA *to;
    CHAR_DATA *to_next;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
    buf[0] = '\0'; 
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
        return;

    /* discard null rooms and chars */
    if (ch == NULL || ch->in_room == NULL)
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( vch == NULL )
        {
            bug( "Act: null vch with TO_VICT.", 0 );
            return;
        }

	if (vch->in_room == NULL)
	    return;

        to = vch->in_room->people;
    }

    room_snoop_list = ch->in_room->snoop_by;
    for ( ; to != NULL ; to = to_next )
    {
	
	to_next = to->next_in_room;

        while ( to_next == NULL && ( type == TO_ROOM || type == TO_NOTVICT )
	  && room_snoop_list != NULL )
	{
	   to_next = room_snoop_list->room->people;
	   room_snoop_list = room_snoop_list->next;
	}

        if ( to->desc == NULL || to->position < min_pos )
            continue;
        if ( type == TO_CHAR && to != ch )
            continue;
        if ( type == TO_VICT && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM && to == ch )
            continue;
        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;
 
        point   = buf;
        str     = format;
        while ( *str != '\0' )
        {
            if ( *str != '$' )
            {
                *point++ = *str++;
                continue;
            }
            ++str;
 
            if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
            {
                bug( "Act: missing arg2 for code %d.", *str );
                i = " <@@@@@@> ";
            }
            else
            {
                switch ( *str )
                {
                default:  bug( "Act: bad code %d.", *str );
                          i = " <@@@@@@> ";                                break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;                            break;
                case 'T': i = (char *) arg2;                            break;
                case 'n': i = PERS( ch,  to  );                         break;
                case 'N': i = PERS( vch, to  );                         break;
                case 'e': i = he_she  [URANGE(1, ch  ->sex, 3)];        break;
                case 'E': i = he_she  [URANGE(1, vch ->sex, 3)];        break;
                case 'm': i = him_her [URANGE(1, ch  ->sex, 3)];        break;
                case 'M': i = him_her [URANGE(1, vch ->sex, 3)];        break;
                case 's': i = his_her [URANGE(1, ch  ->sex, 3)];        break;
                case 'S': i = his_her [URANGE(1, vch ->sex, 3)];        break;
 
                case 'p':
                    i = can_see_obj( to, obj1 )
                            ? obj1->short_descr
                            : "something";
                    break;
 
                case 'P':
                    i = can_see_obj( to, obj2 )
                            ? obj2->short_descr
                            : "something";
                    break;
 
                case 'd':
                    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
                    {
                        i = "door";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                }
            }
 
            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }
 
        *point++ = '\n';
        *point++ = '\r';
        buf[0]   = UPPER(buf[0]);
	if (to->desc != NULL )
	 {  set_color( color, to );
            write_to_buffer( to->desc, buf, point - buf );
	    set_color( "`w", to);
         }
    }
    return;
}

void act_new( const char *format, CHAR_DATA *ch, const void *arg1, 
	      const void *arg2, int type, int min_pos)
{
    ROOM_SNOOP_LIST * room_snoop_list;
    static char * const he_she  [] = {"", "it",  "he",  "she" };
    static char * const him_her [] = {"", "it",  "him", "her" };
    static char * const his_her [] = {"", "its", "his", "her" };
 
    char buf[MAX_STRING_LENGTH];
    char fname[MAX_INPUT_LENGTH];
    CHAR_DATA *to;
    CHAR_DATA *to_next;
    CHAR_DATA *vch = (CHAR_DATA *) arg2;
    OBJ_DATA *obj1 = (OBJ_DATA  *) arg1;
    OBJ_DATA *obj2 = (OBJ_DATA  *) arg2;
    const char *str;
    const char *i;
    char *point;
    buf[0] = '\0'; 
    /*
     * Discard null and zero-length messages.
     */
    if ( format == NULL || format[0] == '\0' )
        return;

    /* discard null rooms and chars */
    if (ch == NULL || ch->in_room == NULL)
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
        if ( vch == NULL )
        {
            bug( "Act: null vch with TO_VICT.", 0 );
            return;
        }

	if (vch->in_room == NULL)
	    return;

        to = vch->in_room->people;
    }
 
    room_snoop_list = ch->in_room->snoop_by;
    for ( ; to != NULL; to = to_next )
    {
	to_next = to->next_in_room;

        while ( to_next == NULL && ( type == TO_ROOM || type == TO_NOTVICT )
	  && room_snoop_list != NULL )
	{
	   to_next = room_snoop_list->room->people;
	   room_snoop_list = room_snoop_list->next;
	}

        if ( to->desc == NULL || to->position < min_pos )
            continue;
        if ( type == TO_CHAR && to != ch )
            continue;
        if ( type == TO_VICT && ( to != vch || to == ch ) )
            continue;
        if ( type == TO_ROOM && to == ch )
            continue;
        if ( type == TO_NOTVICT && (to == ch || to == vch) )
            continue;
 
        point   = buf;
        str     = format;
        while ( *str != '\0' )
        {
            if ( *str != '$' )
            {
                *point++ = *str++;
                continue;
            }
            ++str;
 
            if ( arg2 == NULL && *str >= 'A' && *str <= 'Z' )
            {
                bug( "Act: missing arg2 for code %d.", *str );
                i = " <@@@@@@> ";
            }
            else
            {
                switch ( *str )
                {
                default:  bug( "Act: bad code %d.", *str );
                          i = " <@@@@@@> ";                                break;
                /* Thx alex for 't' idea */
                case 't': i = (char *) arg1;                            break;
                case 'T': i = (char *) arg2;                            break;
                case 'n': i = PERS( ch,  to  );                         break;
                case 'N': i = PERS( vch, to  );                         break;
                case 'e': i = he_she  [URANGE(1, ch  ->sex, 3)];        break;
                case 'E': i = he_she  [URANGE(1, vch ->sex, 3)];        break;
                case 'm': i = him_her [URANGE(1, ch  ->sex, 3)];        break;
                case 'M': i = him_her [URANGE(1, vch ->sex, 3)];        break;
                case 's': i = his_her [URANGE(1, ch  ->sex, 3)];        break;
                case 'S': i = his_her [URANGE(1, vch ->sex, 3)];        break;
 
                case 'p':
                    i = can_see_obj( to, obj1 )
                            ? obj1->short_descr
                            : "something";
                    break;
 
                case 'P':
                    i = can_see_obj( to, obj2 )
                            ? obj2->short_descr
                            : "something";
                    break;
 
                case 'd':
                    if ( arg2 == NULL || ((char *) arg2)[0] == '\0' )
                    {
                        i = "door";
                    }
                    else
                    {
                        one_argument( (char *) arg2, fname );
                        i = fname;
                    }
                    break;
                }
            }
 
            ++str;
            while ( ( *point = *i ) != '\0' )
                ++point, ++i;
        }
 
        *point++ = '\n';
        *point++ = '\r';
        buf[0]   = UPPER(buf[0]);
        if (to->desc != NULL) 
	    write_to_buffer( to->desc, buf, point - buf );
    }
    return;
}


/*
 * Macintosh support functions.
 */
#if defined(macintosh)
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif

char *do_color(char *plaintext, bool color)
{
	static char color_text[MAX_BUF];
	char *ct_point;
	
	bzero(color_text, sizeof(color_text));
	ct_point=color_text;
	while ( *plaintext != '\0' ) {
		if ( *plaintext != '`' ) {
			*ct_point = *plaintext;
			ct_point++;
			plaintext++;
			continue;
		}
		plaintext++;
		if (!color)
		  switch(*plaintext) {
		  	case 'k':
	  		case 'K':
		  	case 'r':
		  	case 'R':
		  	case 'b':
	  		case 'B':
		  	case 'c':
	  		case 'C':
		  	case 'Y':
		  	case 'y':
	  		case 'm':
		  	case 'M':
		  	case 'w':
	  		case 'W':
		  	case 'g':
		  	case 'G':
		  		plaintext++;
	  			break;
		  	default:
				strcat(color_text, "`");
				ct_point++;
				break;
		  }
		else
		  switch(*plaintext) {
			case 'k':
				strcat(color_text, "[0;30m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'K':
				strcat(color_text, "[1;30m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'r':
				strcat(color_text, "[0;31m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'R':
				strcat(color_text, "[1;31m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'g':
				strcat(color_text, "[0;32m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'G':
				strcat(color_text, "[1;32m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'y':
				strcat(color_text, "[0;33m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'Y':
				strcat(color_text, "[1;33m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'b':
				strcat(color_text, "[0;34m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'B':
				strcat(color_text, "[1;34m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'm':
				strcat(color_text, "[0;35m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'M':
				strcat(color_text, "[1;35m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'c':
				strcat(color_text, "[0;36m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'C':
				strcat(color_text, "[1;36m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'w':
				strcat(color_text, "[0;37m");
				ct_point+=7;
				plaintext++; 
				break;
			case 'W':
				strcat(color_text, "[1;37m");
				ct_point+=7;
				plaintext++; 
				break;
			default:
				strcat(color_text, "`");
				ct_point++;
				break;
		}
	}
	return(color_text);
}

