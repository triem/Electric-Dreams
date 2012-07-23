/* this is a listing of all the commands and command related data */

/* for command types */
#define ML 	MAX_LEVEL	/* implementor */
#define L1	MAX_LEVEL - 1  	/* creator */
#define L2	MAX_LEVEL - 2	/* supreme being */
#define L3	MAX_LEVEL - 3	/* deity */
#define L4 	MAX_LEVEL - 4	/* god */
#define L5	MAX_LEVEL - 5	/* immortal */
#define L6	MAX_LEVEL - 6	/* king */
#define L7	MAX_LEVEL - 7	/* duke */
#define L8	MAX_LEVEL - 8	/* count */
#define L9	MAX_LEVEL - 9	/* baron */
#define L10	MAX_LEVEL - 10	/* knight */
#define IM	LEVEL_IMMORTAL 	/* immortal */
#define HE	LEVEL_HERO	/* hero */


/*
 * Structure for a command in the command lookup table.
 */
struct	cmd_type
{
    char * const	name;
    DO_FUN *		do_fun;
    sh_int		position;
    long		groups;
    sh_int		log;
    bool             	show;
    long               vnum;
    long 		room_vnum;
};

/* the command table itself */
extern	const	struct	cmd_type	cmd_table	[];

extern  const	struct	cmd_type	mob_cmd_table	[];


/*
 * Command logging types.
 */
#define LOG_NORMAL	A
#define LOG_ALWAYS	B
#define LOG_NEVER	C
#define NO_HAPPY_HOUR   D
#define HELP_COMM	E
#define HELP_COMBAT	F
#define HELP_OTHER	G
#define HELP_MOVEMENT	H
#define HELP_INFO	I
#define HELP_OBJECTS	J
#define HELP_GROUP	K
#define HELP_SKILLS	L
#define HELP_CONFIG	M
#define STARTING	N

/* If you add a group, you must add it to merc.h also.
   search off of CMD_GROUP */
#define PL		(1)
#define BB		(2)
#define AB		(4)
#define GI		(8)
#define GD		(16)
#define GC		(32)
#define AP		(64)
#define BP		(128)
#define AD		(256)
#define GM		(512)
