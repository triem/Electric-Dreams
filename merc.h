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
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * Accommodate old non-Ansi compilers.
 */
#if defined(TRADITIONAL)
#define const
#define args( list )			( )
#define DECLARE_DO_FUN( fun )		void fun( )
#define DECLARE_ROOM_FUN( fun )		void fun( )
#define DECLARE_OBJ_FUN( fun )		void fun( )
#define DECLARE_SPEC_FUN( fun )		bool fun( )
#define DECLARE_OBJ_SPEC_FUN( fun )	bool fun( )
#define DECLARE_SPELL_FUN( fun )	void fun( )
#else
#define args( list )			list
#define DECLARE_DO_FUN( fun )		DO_FUN    fun
#define DECLARE_ROOM_FUN( fun )		ROOM_FUN  fun
#define DECLARE_OBJ_FUN( fun )		OBJ_FUN   fun
#define DECLARE_SPEC_FUN( fun )		SPEC_FUN  fun
#define DECLARE_OBJ_SPEC_FUN( fun )	OBJ_SPEC_FUN fun
#define DECLARE_SPELL_FUN( fun )	SPELL_FUN fun
#endif

/* system calls */
int unlink();
int system();

/*
 * Short scalar types.
 * Diavolo reports AIX compiler has bugs with short types.
 */
#if	!defined(FALSE)
#define FALSE	 0
#endif

#if	!defined(TRUE)
#define TRUE	 1
#endif

#if	defined(_AIX)
#if	!defined(const)
#define const
#endif
typedef int				sh_int;
typedef int				bool;
#define unix
#else
typedef short   int			sh_int;
// Didn't need this, already defined in c++: typedef unsigned char			bool;
#endif


/*
 * Structure types.
 */
typedef struct  message_buf		MESSAGE_BUF;  /* Message Ques */
typedef struct  editing_data            EDITING_DATA;	/* OLC */
typedef struct	affect_data		AFFECT_DATA;
typedef struct  area_data       	AREA_DATA;
typedef struct  world_data       	WORLD_DATA;
typedef struct	ban_data		BAN_DATA;
typedef struct	buf_type		BUFFER;
typedef struct	bhost_list		BHOST_LIST;
typedef struct  track_type		TRACK_TYPE;
typedef struct  bet_type		BET_DATA;
typedef struct  horse_race_type		HORSE_RACE_TYPE;
typedef struct  topic_data		TOPIC_DATA;
typedef struct	group_data		GROUP_DATA;
typedef struct	char_data		CHAR_DATA;
typedef struct	clan_type		CLAN_DATA;
typedef struct  pc_clan_data		PC_CLAN_DATA;
typedef struct	top_ten_type		TOP_TEN_DATA;
typedef struct	descriptor_data		DESCRIPTOR_DATA;
typedef struct	snoop_list		SNOOP_LIST;
typedef struct	room_snoop_list		ROOM_SNOOP_LIST;
typedef struct	exit_data		EXIT_DATA;
typedef struct	lock_data		LOCK_DATA;
typedef struct	extra_descr_data	EXTRA_DESCR_DATA;
typedef struct	help_data		HELP_DATA;
typedef struct  room_list_struct	BFS_ROOM;
typedef struct  bfs_queue_struct	BFS_QUEUE;
typedef struct	gainer_type		GAINER_DATA;
typedef struct	moveable		MOVEABLE_DATA;
typedef struct	logon_data		LOGON_DATA;
typedef struct	global_random_data	GLOBAL_RANDOM_DATA;
typedef struct	wizlist_data		WIZLIST_DATA;
typedef struct	finger_data		FINGER_DATA;
typedef struct	kill_data		KILL_DATA;
typedef struct	mob_index_data		MOB_INDEX_DATA;
typedef struct	note_data		NOTE_DATA;
typedef struct	read_notes		READ_NOTES;
typedef struct  spell_list		SPELL_LIST;
typedef struct  magic_data		MAGIC_DATA;
typedef struct	wear_data		WEAR_DATA;
typedef struct	equip_data		EQUIP_DATA;
typedef struct	inside_data		INSIDE_DATA;
typedef struct	edible_data		EDIBLE_DATA;
typedef struct	light_data		LIGHT_DATA;
typedef struct	approve_data		APPROVE_DATA;
typedef struct	gate_data		GATE_DATA;
typedef struct	cont_data		CONT_DATA;
typedef struct	trap_data		TRAP_DATA;
typedef struct	track_data		TRACK_DATA;
typedef struct	weapon_data		WEAPON_DATA;
typedef struct	inside_area		INSIDE_AREA_DATA;
typedef struct	obj_data		OBJ_DATA;
typedef struct	obj_index_data		OBJ_INDEX_DATA;
typedef struct	pc_data			PC_DATA;
typedef struct	card_type		CARD_DATA;
typedef struct	debt_data		DEBT_DATA;
typedef struct	reset_data		RESET_DATA;
typedef struct	event_data		EVENT_DATA;
typedef struct	quest_data		QUEST_DATA;
typedef struct	char_quests		CHAR_QUESTS;
typedef struct	room_index_data		ROOM_INDEX_DATA;
typedef struct	shop_data		SHOP_DATA;
typedef struct	time_info_data		TIME_INFO_DATA;
typedef struct	weather_data		WEATHER_DATA;
typedef struct  specpro_data		SPECPRO_DATA;
typedef struct  specpro_list		SPECPRO_LIST;
typedef struct  script_data             SCRIPT_DATA; /* scripts */
typedef struct  trigger_data            TRIGGER_DATA; /* scripts */
typedef struct  trigger_list_data       TRIGGER_LIST_DATA; /* scripts */
typedef struct  trigger_index_data      TRIGGER_INDEX_DATA; /* scripts */
typedef struct  variable_data           VARIABLE_DATA; /* scripts */
typedef struct  skill_list           	SKILL_LIST; /* mobiles */
typedef struct 	castle_data		CASTLE_DATA; /* castle code */
/*
 * Function types.
 */
typedef	void DO_FUN	  args( ( CHAR_DATA *ch, char *argument ) );
typedef void ROOM_FUN     args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
typedef void OBJ_FUN	  args( ( OBJ_DATA * obj, char * argument ) );
typedef bool SPEC_FUN	  args( ( CHAR_DATA *ch ) );
typedef bool OBJ_SPEC_FUN args( ( OBJ_DATA *obj ) );
typedef void SPELL_FUN	  args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );

/*
 * String and memory management parameters.
 */
#define MAX_VNUM			80000
#define	MAX_KEY_HASH			1024	
#define MAX_STRING_LENGTH		4096 
#define MAX_INPUT_LENGTH		4000
#define PAGELEN				22
#define MAX_BUF_LIST			14
#define MAX_BUF				262144
#define BUF_STRING			256
#define BASE_BUF			1024
#define MAX_COLORS			11
#define MAX_COLORS_LIST			16
#define MAX_MOOD			8
#define MAX_LOGON			5000
#define CONVERSION			0
#define LOGON_ON			1
#define MAGIC_NUM			52571214
#define BUFSIZE				256
extern long	autosave_system;	
extern long	changed_system;	
#define AUTOSAVE_HELPS			A
#define AUTOSAVE_SOCIALS		B
#define AUTOSAVE_CLANS			E
#define AUTOSAVE_TRIGGERS		F
#define AUTOSAVE_RACES			G
#define AUTOSAVE_GUILDS			H
#define AUTOSAVE_LOGONS			I
#define AUTOSAVE_QUESTS			J
#define AUTOSAVE_RANDOM			K
#define CHANGED_HELPS			A
#define CHANGED_SOCIALS			B
#define CHANGED_TRIGGERS		E
#define CHANGED_CLANS			F
#define CHANGED_RACES			G
#define CHANGED_GUILDS			H
#define CHANGED_LOGONS			I
#define CHANGED_QUESTS			J
#define CHANGED_RANDOM			K

extern int	per_cpu_usage[];
/*
 * Game parameters.
 * Increase the max'es if you add more of something.
 * Adjust the pulse numbers to suit yourself.
 */
#define MAX_SPECPRO			37
#define MAX_TOKENS			1500
#define MAX_TOKENS_2			35
#define MAX_GREETING			4 
#define MAX_SOCIALS			512 
#define MAX_SKILL			246
#define MAX_MINERAL			16
#define MAX_ELEMENT_TYPE		5
#define MAX_ELEMENT_POWER		3
#define MAX_WEAPON_TYPE			9
#define MAX_WEAPON_POWER		3
#define MAX_LANGUAGE			11
#define MAX_WORLD			4
#define MAX_RACE			30
#define MAX_HORSE			20
#define MAX_BET				50000
#define MAX_HORSE_RACE			3
#define MAX_LEVEL			60
#define MAX_DAMAGE_MESSAGE		35
#define MAX_DAMAGE_TYPE			34
#define SEND_INVITATION			FALSE
#define MAX_COND			3
#define COST_HOUSE			5000000
#define LEVEL_HERO			(MAX_LEVEL - 10)
#define LEVEL_IMMORTAL			(MAX_LEVEL - 5)
#define MAX_OPTIWIZ			3
#define MAX_GROUP			21
#define MAX_IN_GROUP			30
#define MAX_SCAN_ITEMS      5

#define PULSE_PER_SECOND		4
#define PULSE_VIOLENCE			( 4 * PULSE_PER_SECOND)
#define PULSE_MOBILE			1 
#define PULSE_MESSAGE_QUE		( 2 * PULSE_PER_SECOND)
#define PULSE_OBJ_TRIGGER		( 4 * PULSE_PER_SECOND)
#define PULSE_ROOMS			( 3 * PULSE_PER_SECOND)
#define PULSE_CHAR_DREAM_UPDATE		( 4 * PULSE_PER_SECOND)
#define PULSE_CHAR_FORGE_UPDATE		( 4 * PULSE_PER_SECOND)
#define PULSE_TICK			(30 * PULSE_PER_SECOND)
#define PULSE_REGEN          (5 * PULSE_PER_SECOND)
#define PULSE_TRIGGER			(35 * PULSE_PER_SECOND)
#define PULSE_RACE			( 8 * PULSE_PER_SECOND)
#define PULSE_AREA			(60 * PULSE_PER_SECOND)
#define PULSE_SAVE_AREA			(70 * PULSE_PER_SECOND)
#define PULSE_SAVE_LOGON		(50 * PULSE_PER_SECOND)
#define PULSE_SAVE_RANDOM		(75 * PULSE_PER_SECOND)
#define PULSE_SAVE_QUESTS		(80 * PULSE_PER_SECOND)
#define PULSE_SAVE_WIZLIST		(45 * PULSE_PER_SECOND)
#define PULSE_CHECK_OBJ			( 5 * PULSE_PER_SECOND)
#define PULSE_REBOOT			( 5 * PULSE_PER_SECOND)

#define IMPLEMENTOR			MAX_LEVEL
#define	CREATOR				(MAX_LEVEL - 1)
#define SUPREME				(MAX_LEVEL - 2)
#define DEITY				(MAX_LEVEL - 3)
#define GOD				(MAX_LEVEL - 4)
#define IMMORTAL			(MAX_LEVEL - 5)
#define DEMI				(MAX_LEVEL - 6)
#define ANGEL				(MAX_LEVEL - 7)
#define AVATAR				(MAX_LEVEL - 8)
#define HERO				LEVEL_HERO
#define LEVEL_SCRIPT			(MAX_LEVEL-1) /* for scripts */
#define LEVEL_BUILDER			(MAX_LEVEL - 5) /* scripts */
#define ED_JAIL_HOUSE			18002

/*
 * Trigger types for repetitive loops, command etc. for scripts
 */
#define TRIG_COMMAND          0   /* When a player types x command        */
#define TRIG_EACH_PULSE       1   /* Increments every pulse               */
#define TRIG_COMBAT           2   /* Every combat pulse                   */
#define TRIG_TICK_PULSE       3   /* Every tick (one mud-hour)            */
#define TRIG_BORN             4   /* Happens upon creation                */
#define TRIG_GETS             5   /* Happens when a mob gets/is given     */
#define TRIG_SAY              6   /* Happens when a mob sees a string     */
#define TRIG_TELL             7   /* Happens when a mob sees a string     */
#define TRIG_KILLS_PLAYER     8   /* Happens when a mob kills a player    */
#define TRIG_DIES             9   /* Happens when the mob is killed       */
#define TRIG_ENTER           10   /* Happens on arrival (for each person) */
#define TRIG_MOVES           11   /* Happens whenever a mobile moves      */
#define TRIG_KILLS_MOB       12   /* Happens whenever a mobile kills a mob  */
#define TRIG_LEAVES          13   /* Happens whenever a leaves */
#define TRIG_LOOK            14   /* Happens whenever a looks  */
#define TRIG_WEAR            15   /* Happens someone wears an item */
#define TRIG_GIVE            16   /* Happens someone gives an item */
#define TRIG_DROP            17   /* Happens someone drops an item */
#define TRIG_WIELD           18   /* Happens someone wields an item */
#define TRIG_TROOM           19   /* room techo to mob */
#define TRIG_DOOR_OPEN       20   /* a door is opened */
#define TRIG_GAINS	     21   /* Happens when a player gains a skill */
#define MAX_TRIG_TYPE	     22

#define TRIG_TYPE_MOB		0 /* Script is for Mobiles */
#define TRIG_TYPE_ROOM		1 /* Script is for Rooms */
#define TRIG_TYPE_OBJ		2 /* Script is for Objects */

/*
 * BFS return values.
 */
#define BFS_ERROR           -1 
#define BFS_ALREADY_THERE   -2  
#define BFS_NO_PATH         -3

#define ELEMENT_FIRE		0
#define ELEMENT_WATER		1
#define ELEMENT_EARTH		2
#define ELEMENT_WIND		3
#define ELEMENT_SPIRIT		4

#define MINERAL_STEEL		0
#define MINERAL_COPPER		1
#define MINERAL_IRON		2
#define MINERAL_SILVER		3
#define MINERAL_GOLD		4
#define MINERAL_RUBY		5
#define MINERAL_SAPPHIRE	6
#define MINERAL_EMERALD		7
#define MINERAL_BRONZE		8
#define MINERAL_ADAMANTITE	9
#define MINERAL_TITANIUM	10
#define MINERAL_ALABASTER	11
#define MINERAL_MITHRIL		12
#define MINERAL_OBSIDIAN	13
#define MINERAL_PLATINUM	14
#define MINERAL_DIAMOND		15

/*
 * Internally used bits for triggers. for scripts
 */
#define SCRIPT_ADVANCE		(A) /* Its OK to go to next command set  */
#define SCRIPT_HALT		(B) /* current=NULL & disallow trig      */
#define SCRIPT_PATH		(C) /* Path done  */
/* And some externaly used bits for triggers */
#define TRIG_MOB		(D) /* Trigger will act from a mob */
#define TRIG_PC			(E) /* Trigger will act from a pc */
#define SCRIPT_HALT_RESET	(F) /* current=NULL Until Reset    */
#define TRIG_INACTIVE		(G) /* Trig will not run till set to active  */
#define TRIG_INSTANT		(H) /* Trig runs NOW!  */

#define BUFFER_SAFE		0
#define BUFFER_OVERFLOW		1
#define BUFFER_FREED		2

/*
 * RACE INFO
 */
#define ATTACK_NORMAL		0
#define ATTACK_BITE		1

/*
 * Un colour type.
 */
struct color_data
{
   char         code[10];
   char         act_code[8];
   char         name[15];
   int          number;
};

/*
 * Site ban structure.
 */
struct	ban_data
{
    BAN_DATA *	next;
    char 	name[31];
};

struct buf_type
{
    BUFFER *    next;
    bool        valid;
    sh_int      state;  /* error state of the buffer */
    long        size;   /* size in k */
    char *      string; /* buffer's string rev */
};

struct	bhost_list
{
    BHOST_LIST * next;
    char 	host[16];
    long 	time;
};

struct track_type
{
    TRACK_TYPE *  last;
    TRACK_TYPE *  next;
    int	   dir;
    ROOM_INDEX_DATA * room;
};

struct horse_type
{
    char  name[31];
    sh_int number;
    sh_int odds;
    sh_int curr_odds;
    sh_int position;
    bool racing;
};
struct horse_race_type
{
    struct horse_type horse[MAX_HORSE];
    BET_DATA * bets;
    sh_int timer;
    BET_DATA * pay_offs;
    long in_room;
};

struct bet_type    
{
    CHAR_DATA * ch;
    long amount;
    sh_int coin_type;
    sh_int horse;
    sh_int odds;
    BET_DATA * next;
};

struct skill_list    
{
    SKILL_LIST * next;
    int sn;
};

/*
 * castle code
 */
struct castle_data
{
    CASTLE_DATA * next;

    char owner[30];
    long castle;
    long barracks;
    long barracks2;
    long library;
    long library2;
    long smithy;
    long smithy2;
    long magic_shop;
    long armory;
    long grainery;
    long inn;
    long inn2;
    long bank;
    long track;
};

/*
 * Quests for player data
 */
struct char_quests
{
   CHAR_QUESTS * next;
   sh_int	quest_number;
   long		flags;
};


/* 
 * for scripts
 */
struct trigger_index_data
{
   TRIGGER_INDEX_DATA * next;
   SCRIPT_DATA *        script;
   sh_int               trigger_type;
   long                 bits;
   char                 keywords[31];
   char                 key_sim[31];
   char                 key_words[31];
   char                 key_string[31];
   char                 name[31];
   char                 desc[81];
   sh_int		security;
   sh_int		chance;
   char 		builders[31];	
   sh_int		script_type;
   sh_int		vnum;
   long			gets_vnum;
   long			mob_vnum;
   long			flags;
   long			step;
   long			step_pre;
   sh_int		quests;
   sh_int		quests_pre;
   sh_int		timer;
};

/* 
 * for scripts
 */
struct trigger_list_data
{
   TRIGGER_LIST_DATA *  next;
   sh_int	vnum;
};

/* 
 * for scripts
 */
struct trigger_data
{
   TRIGGER_DATA *       next;
   TRIGGER_DATA *       next_in_list;
   SCRIPT_DATA *        script;
   SCRIPT_DATA *        current;
   AREA_DATA *		area;
   sh_int               tracer;
   sh_int               timer;
   long                 activated;
   long                 attempts;
   sh_int               waiting;
   sh_int               trigger_type;
   long                 bits;
   char                 keywords[31];
   char                 key_sim[31];
   char                 key_words[31];
   char                 key_string[31];
   char                 name[31];
   sh_int		vnum;
   long			gets_vnum;
   long			mob_vnum;
   long			flags;
   long			step;
   long			step_pre;
   sh_int		quests;
   sh_int		quests_pre;
};

/* 
 * for scripts
 */
struct script_data
{
   SCRIPT_DATA *        next;
   char *               command;
};

/* 
 * for scripts
 */
struct variable_data
{
   VARIABLE_DATA *      next;
   char                 name[81];
   char                 value[81];
};

/*
 * Time and weather stuff.
 */
#define SUN_DARK		    0
#define SUN_RISE		    1
#define SUN_LIGHT		    2
#define SUN_SET			    3

#define SKY_CLOUDLESS		    	0
#define SKY_CLOUDY		    	1
#define SKY_RAINING		    	2
#define SKY_LIGHTNING		    	3
#define SKY_SNOWING			4
#define SKY_BLIZZARD			5
#define SKY_RAINING_WINDY	    	6
#define SKY_HURICANE			7
#define SKY_WINDY			8
#define SKY_TORNADO			9
#define SKY_WINDY_2			10
#define SKY_SANDSTORM			11
#define MAX_SKY				12

struct time_table_type
{
    int		sunrise;
    char *  	sunrise_dir;
    int		sunset;
    char * 	sunset_dir;
    int		hours_day;
    int 	days_week;
    int 	days_month;
    int		months_year;
};

struct	time_info_data
{
    int		hour;
    int		day;
    int		month;
    int		year;
};

#define MAX_SEASON 4
struct	weather_data
{
    WEATHER_DATA	* next;
    /* the month that each season occurs on */
    sh_int	spring;
    sh_int	summer;
    sh_int	fall;
    sh_int	winter;
    sh_int	season;
    /* the probability of each weather type happening during each season */
    /* ie 25, 50, 75, 100 */
    sh_int	chance[ MAX_SEASON ][ 4 ];
    sh_int	storm_type[ MAX_SEASON ];
    sh_int	speed;
    int		mmhg;
    int		change;
    int		sky;
    int		sunlight;
};

/*
 * Connected state for a channel.
*/
#define CON_AEDITOR                     -1
#define CON_REDITOR                     -2
#define CON_MEDITOR                     -3
#define CON_OEDITOR                     -4
#define CON_HEDITOR                     -5
#define CON_SEDITOR                     -6
#define CON_RAEDITOR			-7
#define CON_MAEDITOR                    -8
#define CON_EDITING                     -9
#define CON_TEDITOR                     -10
#define CON_PEDITOR			-11
#define CON_GEDITOR                     -12
#define CON_PLAYING			 0
#define CON_COPYOVER_RECOVER		-15
#define CON_GET_NAME			 1
#define CON_GET_OLD_PASSWORD		 2
#define CON_CONFIRM_NEW_NAME		 3
#define CON_GET_NEW_PASSWORD		 4
#define CON_CONFIRM_NEW_PASSWORD	 5
#define CON_GET_NEW_RACE		 6
#define CON_GET_NEW_SEX			 7
#define CON_DEFAULT_CHOICE		10 
#define CON_GET_NEW_GUILDS		11 
#define CON_CHOOSE_CREATION		12
#define CON_READ_IMOTD			13
#define CON_READ_MOTD			14
#define CON_BREAK_CONNECT		15
#define CON_DEFAULT_COLOR		16
#define CON_ROLL_STATS			17
#define CON_AFT_LOGIN			100
#define CON_DEAD			101
#define CON_DO_GREETING			103

#define MAX_HISTORY			10

struct snoop_list
{
    DESCRIPTOR_DATA *	desc;
    SNOOP_LIST *	next;
};

/*
 * Descriptor (channel) structure.
 */
struct	descriptor_data
{
    DESCRIPTOR_DATA *	next;
    SNOOP_LIST *	snoop_by;
    CHAR_DATA *		character;
    CHAR_DATA *		original;
    char 		host[16];
    char *		host_save;
    sh_int		descriptor;
    sh_int		connected;
    bool		fcommand;
    char		inbuf		[8 * MAX_INPUT_LENGTH];
    char		incomm		[MAX_INPUT_LENGTH];
    char *		inlast[MAX_HISTORY];
    int			repeat;
    char *		outbuf;
    long		outsize;
    long		outtop;
    char *		showstr_head;
    char *		showstr_point;
    EDITING_DATA *      editing;	/* OLC */
    void *              pEdit;		/* OLC */
    char **             pString;	/* OLC */
    bool		color;		/* for beginning screen */
    char                alias_point     [MAX_INPUT_LENGTH];
    char                alias_arg       [MAX_INPUT_LENGTH];

};


/*
 * Attribute bonus structures.
 */
struct	str_app_type
{
    sh_int	tohit;
    sh_int	todam;
    sh_int	carry;
    sh_int	wield;
};

struct	int_app_type
{
    sh_int	learn;
};

struct	wis_app_type
{
    sh_int	practice;
};

struct	chr_app_type
{
    sh_int	haggle;
};

struct	dex_app_type
{
    sh_int	defensive;
};

struct	con_app_type
{
    sh_int	hitp;
    sh_int	shock;
};

/*
 * TO types for act.
 */
#define TO_ROOM		    0
#define TO_NOTVICT	    1
#define TO_VICT		    2
#define TO_CHAR		    3
#define TO_WORLD	    4

struct bfs_queue_struct 
{
   ROOM_INDEX_DATA *room;
   sh_int   dir;
   sh_int   depth;
   BFS_QUEUE *next;
};

struct room_list_struct 
{
   ROOM_INDEX_DATA *room;
   BFS_ROOM *next;
};
/*
 * Help table types.
 */
struct	help_data
{
    HELP_DATA *	next;
    sh_int	level;
    char 	keyword[51];
    char *	text;
    int		vnum;
    int		type;
};

/*
 * Shop types.
 */
#define MAX_TRADE	 5

struct	shop_data
{
    SHOP_DATA *	next;			/* Next shop in list		*/
    AREA_DATA * load_with_area;
    long	keeper;			/* Vnum of shop keeper mob	*/
    sh_int	type;			/* Shop Type */
    sh_int	profit_buy;		/* Cost multiplier for buying	*/
    sh_int	profit_sell;		/* Cost multiplier for selling	*/
    sh_int	open_hour;		/* First opening hour		*/
    sh_int	close_hour;		/* First closing hour		*/
/*	Added by Trice for POS (Player Ownable Shops)			*/
/*   char      * owner;			 Shop owners name if ownable  
    bool	ownable;		 Is the shop buyable		*/

};

/*
 * Per-class stuff.
 */
#define MAX_GUILD 	12
#define MAX_STATS 	6
#define MAX_AGE		195
#define STAT_STR 	0
#define STAT_INT	1
#define STAT_WIS	2
#define STAT_DEX	3
#define STAT_CON	4
#define STAT_CHR	5

struct attack_type
{
    char *	name;			/* name and message */
    int   	damage;			/* damage class */
};

struct mood_type
{
    char *	mood;			/* name and message mood */
    int   	type;			/* type of mood */
};

struct exp_type
{
    int		level;			/* Level for exp */
    long   	exp_at_level;			/* Amount of exp total to level */
    long   	exp_for_mob;			/* Amount base of exp for mob */
};

struct exp_type_old  /* once all char are past version 9 remove this */
{
    int		level;			/* Level for exp */
    long   	exp_at_level;			/* Amount of exp total to level */
    long   	exp_for_mob;			/* Amount base of exp for mob */
};

struct top_ten_type
{
    char 	saying[81];
    char  	name[31];
    int		value;
};

struct clan_type
{
    CLAN_DATA * next;
    char 	name[31];
    char 	title[81];
    CHAR_DATA * leader;
    char 	leader_name[31];
    CHAR_DATA * sponser;
    char 	sponser_name[31];
    long	treasury;
    long	clan_info;
    long	recall_room;
    long	donate_room;
    char	rank1[31];
    char	rank2[31];
    char	rank3[31];
    char	rank4[31];
    char	rank5[31];
    char	rank6[31];
    char	rank7[31];
    char	rank8[31];
    char	rank9[31];
    char	rank10[31];
};

struct pc_clan_data 
{
    PC_CLAN_DATA * next;
    CLAN_DATA    * clan;
    sh_int	   clanrank;
};

#define RACE_CHANGED 		(A)

struct race_list_type
{
    char 	filename[21];
    char 	name[16];
    char 	builders[31];
    sh_int 	security;
    long	race_flags;
};

struct race_type
{
    char 	name[16];			/* call name of the race */
    bool	pc_race;		/* can be chosen by pcs */
    sh_int      world;     		/* World number */
    sh_int      size;
    sh_int      dam_type;
    long	act;			/* act bits for the race */
    long	act2;			/* act2 bits for the race */
    long	aff;			/* aff bits for the race */
    long	aff2;			/* aff2 bits for the race */
    long	off;			/* off bits for the race */
    long	imm;			/* imm bits for the race */
    long        res;			/* res bits for the race */
    long	vuln;			/* vuln bits for the race */
    long	form;			/* default form flag for the race */
    sh_int	language;
    char 	who_name[6];
    sh_int	scan_dist;		/* Scanning distance */
    sh_int 	stats[MAX_STATS];	/* starting stats */
    sh_int	max_stats[MAX_STATS];	/* maximum stats */
    long	weapon;			/* First weapon			*/
    long	map;
    sh_int	gold;			/* Amount of gold per level ( average ) */
    sh_int 	start_age;		/* Starting Age */
    sh_int	year_per_hour;		/* Year of age per hour played */
    sh_int	thac0_00;		/* Thac0 for level  0		*/
    sh_int	thac0_32;		/* Thac0 for level 32		*/
    sh_int	hp_mult;
    sh_int	dam_dice;
    sh_int	dam_die;
    sh_int	dam_bonus;
    sh_int	ac_dice[4];
    sh_int	ac_die[4];
    sh_int	ac_bonus[4];
    sh_int 	mana_dice[MAX_ELEMENT_TYPE];
    sh_int 	mana_die[MAX_ELEMENT_TYPE];
    sh_int	mana_bonus[MAX_ELEMENT_TYPE];
    sh_int	age_stats[40][MAX_STATS];
    sh_int      max_condition[MAX_COND];
    sh_int      condition_rate[MAX_COND];
    sh_int	skill_level[MAX_SKILL];
    sh_int	rating[MAX_SKILL];
    sh_int	adept[MAX_SKILL];
    sh_int	starting[MAX_SKILL];
    bool	gained[MAX_SKILL];
    long	begin;
    long	die;
    long	donate;
 };

#define GUILD_CHANGED 		(A)

struct guild_type
{
    char 	name[31];			
    char 	filename[31];
    char 	title[31];
    char	builders[31];
    long	flag;
    sh_int 	security;
    long	guild_flags;
    sh_int	rating[MAX_GROUP];
};

#define NOTE_NO_DELETE		1
#define NOTE_RECIEPT		2

/*
 * Data structure for notes.
 */
struct	note_data
{
    NOTE_DATA *	next;
    char 	sender[31];
    char 	date[31];
    char 	to_list[81];
    char 	subject[31];
    char *	text;
    time_t  	date_stamp;
    char 	read_list[81];
    long	flags;
};

/*
 * For bit type in affect_data 
 */
/*
Why not just do a 
typedef enum
{BIT_AFFECT = 1,BIT_VULN,BIT_RES,BIT,AFFECT2,BIT_IMM} b_affect;

This would make it easier to add bits, less typeing. let the
code do the numbering for us.

Sounds good to me.. Just not sure how to do it :)
Wana show me? G
*/
#define BIT_AFFECT	1
#define BIT_VULN	2
#define BIT_RES		3
#define BIT_AFFECT2	4
#define BIT_IMM		5

/*
 * An affect.
 */
struct	affect_data
{
    AFFECT_DATA *	next;
    sh_int		type;
    sh_int		level;
    sh_int		duration;
    sh_int		location;
    sh_int		location2;
    sh_int		modifier;
    long		bitvector;
    sh_int		bit_type;
};

/*
 * A kill structure (indexed by level).
 */
struct	kill_data
{
    sh_int		number;
    sh_int		killed;
};

/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (Start of section ... start here)                     *
 *                                                                         *
 ***************************************************************************/

/*
 * Well known mob virtual numbers.
 * Defined in #MOBILES.
 */
#define MOB_VNUM_FIDO		   18000 /* A rat */
#define MOB_VNUM_RANDOM		   18000 /* used in db.c for random mobs like turkey's */
#define MOB_VNUM_CITYGUARD	   18000 /* black moray */

/* RT ASCII conversions -- used so we can have letters in this file */

#define A		  	1
#define B			2
#define C			4
#define D			8
#define E			16
#define F			32
#define G			64
#define H			128

#define I			256
#define J			512
#define K		        1024
#define L		 	2048
#define M			4096
#define N		 	8192
#define O			16384
#define P			32768

#define Q			65536
#define R			131072
#define S			262144
#define T			524288
#define U			1048576
#define V			2097152
#define W			4194304
#define X			8388608

#define Y			16777216
#define Z			33554432
#define aa			67108864 
#define bb			134217728
#define cc			268435456    
#define dd			536870912
#define ee			1073741824
#define ALL_FLAGS	        2147483647 

/* A few defines for olc */
#define         SEX_NONE        5
#define         ROOM_NONE       0
#define         EX_NONE         0
#define         ITEM_NONE       0
#define         EXTRA_NONE      0
#define         ITEM_WEAR_NONE  0
#define         ACT_NONE        0
#define         AFFECT_NONE     0
/*
 * ACT bits for mobs.
 * Used in #MOBILES.
 */
#define ACT_IS_NPC		(A)		/* Auto set for mobs	*/
#define ACT_SENTINEL	    	(B)		/* Stays in one room	*/
#define ACT_SCAVENGER	      	(C)		/* Picks up objects	*/
#define ACT_ATTACK_WORLD	(D)		/*aggresive to other worlds*/
#define ACT_STAY_AREA		(E)		/* Won't leave area	*/
#define ACT_AGGRESSIVE		(F)    		/* Attacks PC's		*/
#define ACT_REPAIR		(G)
#define ACT_WIMPY		(H)
#define ACT_PET			(I)		/* Auto set for pets	*/
#define ACT_TRAIN		(J)		/* Can train PC's	*/
#define ACT_PRACTICE		(K)		/* Can practice PC's	*/
#define ACT_HUNTER		(U)		/*Will use hunting skill*/
#define ACT_NOPURGE		(V)             /*Safe from the Purge */
#define ACT_OUT_DOORS		(W)	        /*Will not wonder outside */
#define ACT_IN_DOORS		(Y)	        /*Will not wonder inside */
#define ACT_IS_HEALER		(aa)
#define ACT_GAIN		(bb)
#define ACT_UPDATE_ALWAYS	(cc)
#define ACT_GAMBLE		(dd)
#define ACT_MOUNT               (ee)

/*
 *  There can never be 
 *  Too many acts!
 */ 

#define ACT2_MOB_NO_SEE		(A) /* cannot see mob with look */
#define ACT2_IGNORE		(B) /* used to not show up on score command */
#define ACT2_HALT               (C)         /* scripts  */
#define ACT2_TRACKSCR           (D)         /* scripts  */
#define ACT2_MOVEABLE_AREA      (E)  /* Use so area's are moveable   */
#define ACT2_MOB_NO_RANDOM      (F)  /* Mobs will not get random objects on reset */
#define ACT2_SHIP	        (G)  /* Mobs will not enter non water area's */
#define ACT2_OPEN_LAND	        (H)  /* Mobs will not enter hills, mountains */
#define ACT2_SEE_HIDDEN	        (I)  /* Mobs can enter hidden doors */
#define ACT2_STAY_GUILD		(K)
#define ACT2_ATTACK_GUILD	(L)
#define ACT2_ENTER_PORTAL	(M)  /* Mobs can enter portals they find */
#define ACT2_SEE_SECRET	        (N)  /* Mobs will enter secret doors */
#define ACT2_WALKTO	        (O)  /* Scripts in-state */
#define ACT2_STEERING	        (P)  /* Scripts in-state */
#define ACT2_WALKTO_MOB	        (Q)  /* Scripts in-state */ 
#define ACT2_DREAMSTATE		(R)
#define ACT2_WEAPON_LOST	(S)  /* Used for droped weapons on mobs */
#define ACT2_SMART_AGGRESSIVE	(T)  /* Attacts on good chance to win */
#define ACT2_GUIDE		(U)  /* Guides that can enter NO_MOB rooms */

/* damage classes */

#define DAM_NONE                0
#define DAM_BASH                1
#define DAM_PIERCE              2
#define DAM_SLASH               3
#define DAM_FIRE                4

#define DAM_ACID                7
#define DAM_POISON              8

#define DAM_ENERGY              11
#define DAM_MENTAL              12
#define DAM_DISEASE             13
#define DAM_DROWNING            14
#define DAM_LIGHT		15
#define DAM_OTHER               16
#define DAM_HARM		17

#define DAM_WATER		19
#define DAM_EARTH		20
#define DAM_SPIRIT		21
#define DAM_WIND		22
#define DAM_VORPAL		23
#define DAM_CRITICAL		24  /* critical hits */

/* OFF bits for mobiles */
#define OFF_AREA_ATTACK         (A)
#define OFF_BASH                (C)
#define OFF_BERSERK             (D)
#define OFF_DISARM              (E)
#define OFF_FAST                (H)
#define OFF_RESCUE              (L)
#define OFF_TRIP                (N)
#define ASSIST_ALL       	(P)
#define ASSIST_ALIGN	        (Q)
#define ASSIST_RACE    	     	(R)
#define ASSIST_PLAYERS      	(S)
#define ASSIST_VNUM		(U)
#define OFF_SECOND		(V)
#define OFF_THIRD		(W)
#define OFF_BREATHFIRE		(X)

/* return values for check_imm */
#define IS_NORMAL		0
#define IS_IMMUNE		1
#define IS_RESISTANT		2
#define IS_VULNERABLE		3

/* IMM bits for mobs */
#define IMM_SUMMON              (A)
#define IMM_CONTROL_MIND        (B)
#define IMM_MAGIC               (C)
#define IMM_WEAPON              (D)
#define IMM_BASH                (E)
#define IMM_PIERCE              (F)
#define IMM_SLASH               (G)
#define IMM_FIRE                (H)
#define IMM_LIGHTNING           (J)
#define IMM_ACID                (K)
#define IMM_POISON              (L)
#define IMM_ENERGY              (O)
#define IMM_MENTAL              (P)
#define IMM_DISEASE             (Q)
#define IMM_DROWNING            (R)
#define IMM_LIGHT		(S)
#define IMM_WIND		(U)
#define IMM_WATER		(V)
#define IMM_EARTH		(W)
#define IMM_SPIRIT		(X)

/* RES bits for mobs */
#define RES_CONTROL_MIND	(B)
#define RES_MAGIC               (C)
#define RES_WEAPON              (D)
#define RES_BASH                (E)
#define RES_PIERCE              (F)
#define RES_SLASH               (G)
#define RES_FIRE                (H)
#define RES_LIGHTNING           (J)
#define RES_ACID                (K)
#define RES_POISON              (L)
#define RES_ENERGY              (O)
#define RES_MENTAL              (P)
#define RES_DISEASE             (Q)
#define RES_DROWNING            (R)
#define RES_LIGHT		(S)
#define RES_WIND		(U)
#define RES_WATER		(V)
#define RES_EARTH		(W)
#define RES_SPIRIT		(X)

/* VULN bits for mobs */
#define VULN_MAGIC              (C)
#define VULN_WEAPON             (D)
#define VULN_BASH               (E)
#define VULN_PIERCE             (F)
#define VULN_SLASH              (G)
#define VULN_FIRE               (H)
#define VULN_LIGHTNING          (J)
#define VULN_ACID               (K)
#define VULN_POISON             (L)
#define VULN_ENERGY             (O)
#define VULN_MENTAL             (P)
#define VULN_DISEASE            (Q)
#define VULN_DROWNING           (R)
#define VULN_LIGHT		(S)
#define VULN_WIND		(bb)
#define VULN_WATER		(cc)
#define VULN_EARTH		(dd)
#define VULN_SPIRIT		(ee)
 
/* body form */
#define FORM_INSTANT_DECAY      (D)
#define FORM_RAND_25		(E)
 
/* actual form */
#define FORM_ANIMAL             (G)
#define FORM_SENTIENT           (H)

/* materiel typs */
#define MAT_NONE		0
#define MAT_STEEL		1
#define MAT_MITHRIL		2
#define MAT_STAINLESS_STEEL	3
#define MAT_SILVER		4
#define MAT_GOLD		5
#define MAT_DIAMOND		6
#define MAT_PAPER		7
#define MAT_GLASS		8
#define MAT_PLATINUM		9
#define MAT_GEM_STONE		10
#define MAT_WOOD		11
#define MAT_LEATHER		12
#define MAT_SILK		13
#define MAT_IRON		14
#define MAT_CLAY		15
#define MAT_BONE		16
#define MAT_ROCK		17
#define MAT_STONE		18
#define MAT_CANVAS		19
#define MAT_CLOTH		20
#define MAT_FISH_SKIN		21
#define MAT_CORAL		22
#define MAT_OBSIDIAN		23
#define MAT_PEARL		24
#define MAT_RUBY		25
#define MAT_EMERALD		26
#define MAT_SAPPHIRE		27
#define MAT_BRONZE		28
#define MAT_COPPER		29
#define MAT_ADAMANTITE		30
#define MAT_TITANIUM		31
#define MAT_ALABASTER		32
#define MAX_MATERIAL		33

/*
 * Bits for 'affected_by'.
 * Used in #MOBILES.
 */
#define AFF_BLIND		(A)
#define AFF_SHADOWFORM		(B)
#define AFF_DETECT_WORLD	(C)
#define AFF_DETECT_SHADOWFORM	(D)
#define AFF_DETECT_MAGIC	(E)
#define AFF_DETECT_HIDDEN	(F)
#define AFF_TONGUES		(G)		
#define AFF_SANCTUARY		(H)
#define AFF_FAERIE_FIRE		(I)
#define AFF_INFRARED		(J)
#define AFF_CURSE		(K)
#define AFF_HOLD_BREATH		(L)		
#define AFF_POISON		(M)
#define AFF_BREATHE_UNDERWATER	(N)
#define AFF_MOUNTED		(O)		
#define AFF_SNEAK		(P)
#define AFF_HIDE		(Q)
#define AFF_SLEEP		(R)
#define AFF_CONTROL_MIND	(S)
#define AFF_FLYING		(T)
#define AFF_FLY			(T)
#define AFF_DIMENSION_WALK	(U)
#define AFF_HASTE		(V)
#define AFF_CALM_SPIRIT		(W)
#define AFF_PLAGUE		(X)
#define AFF_LEVITATING		(Y)
#define AFF_LEVITATE		(Y)
#define AFF_DARK_VISION		(Z)
#define AFF_BERSERK		(aa)
#define AFF_SWIM		(bb)
#define AFF_REGENERATION        (cc)
#define AFF_COLD                (dd)
#define AFF_NEW_MOB		(ee)


/*
 * Here are affects bitvector2's
 */
#define AFF_SLOW		(A)
#define AFF_FADE		(B)
#define AFF_SENSES		(C)
#define AFF_STUN		(D)
#define AFF_LIGHT_SENSITIVE	(E)
#define AFF_FALLING		(F)
#define AFF_MANA_GEN		(G)

/*
 * Sex.
 * Used in #MOBILES.
 */
#define SEX_NEUTER		      1
#define SEX_MALE		      2
#define SEX_FEMALE		      3

/* dice */
#define DICE_NUMBER			0
#define DICE_TYPE			1
#define DICE_BONUS			2

/* size */
#define SIZE_TINY			0
#define SIZE_SMALL			1
#define SIZE_MEDIUM			2
#define SIZE_LARGE			3
#define SIZE_HUGE			4
#define SIZE_GIANT			5

/*
 * Well known object virtual numbers.
 * Defined in #OBJECTS.
 */

#define ROOM_CAVERN		  34600
#define ROOM_TUNNEL		  34601
#define WHEEL_START_ITEM	  10050
#define WHEEL_END_ITEM		  10055
#define MOB_VNUM_THUG		  18009
#define MOB_VNUM_NIGHTMARE	  18008 
#define OBSIDIAN_GOLEM		  18010
#define GOLD_GOLEM		  18011
#define SILVER_GOLEM		  18012
#define MITHRIL_GOLEM		  18013
#define PLATINUM_GOLEM		  18014
#define COPPER_GOLEM		  18015
#define IRON_GOLEM		  18016
#define BRONZE_GOLEM		  18017
#define ADAMANTITE_GOLEM	  18018
#define STEEL_GOLEM		  18019
#define RUBY_GOLEM		  18020
#define SAPPHIRE_GOLEM		  18021
#define ALABASTER_GOLEM		  18022
#define EMERALD_GOLEM		  18023
#define DIAMOND_GOLEM		  18024
#define TITANIUM_GOLEM		  18025
#define OBJ_VNUM_MONEY_ONE	      2
#define OBJ_VNUM_MONEY_SOME	      3
#define OBJ_VNUM_TOMBSTONE	  18102
#define OBJ_VNUM_CORPSE_NPC	     10
#define OBJ_VNUM_CORPSE_PC	     11
#define OBJ_VNUM_SEVERED_HEAD	     12
#define OBJ_VNUM_TORN_HEART	     13
#define OBJ_VNUM_SLICED_ARM	     14
#define OBJ_VNUM_SLICED_LEG	     15
#define OBJ_VNUM_GUTS		     16
#define OBJ_VNUM_BRAINS		     17

#define VNUM_NEW_YEAR		     20
#define OBJ_VNUM_MUSHROOM	     20
#define OBJ_VNUM_DUNG		     20
#define OBJ_VNUM_RUST_PILE	     20
#define OBJ_VNUM_REPAIR_KIT	     18201
#define OBJ_VNUM_LIGHT_BALL	     21
#define OBJ_VNUM_SPRING		     22

#define OBJ_VNUM_GATE		   18304
#define OBJ_VNUM_HANDBOOK	   18107
#define OBJ_VNUM_BEGIN_LIGHT       21   /* in outfit */
#define OBJ_VNUM_BEGIN_LIGHTER     18306   /* in outfit */
#define OBJ_VNUM_RODOFREV	   18028
#define OBJ_VNUM_MEDICAL_HERB	   18006 /* healing herb */
#define OBJ_VNUM_TENT		   18005 
#define OBJ_VNUM_SHOVEL		   18298 /* shovel for digging */
#define OBJ_VNUM_BIN		   18093 /* storage container for minerals */
#define OBJ_VNUM_EXTRACTING_KIT	   18001 /* used to extract minerals from ore */
#define OBJ_VNUM_ORE_LARGE	   18002 /* ore from mining */
#define OBJ_VNUM_ORE_MEDIUM	   18003 /* ore from mining */
#define OBJ_VNUM_ORE_SMALL	   18004 /* ore from mining */
#define OBJ_VNUM_MINERAL_SPECIAL   18008 /* special rare mineral */
#define OBJ_VNUM_FORGE_DAGGER	   18009 /* dagger used in forging */
#define OBJ_VNUM_FORGE_SWORD	   18012
#define OBJ_VNUM_FORGE_SPEAR	   18013
#define OBJ_VNUM_FORGE_AXE	   18014
#define OBJ_VNUM_FORGE_POLEARM	   18015
#define OBJ_VNUM_FORGE_MACE	   18016
#define VNUM_HAMMER_1		   11916 /* one of the magical hammers for forging */
#define OBJ_VNUM_FORGE_HEAD	   18025
#define OBJ_VNUM_FORGE_BODY	   18024
#define OBJ_VNUM_FORGE_ARMS	   18023
#define OBJ_VNUM_FORGE_LEGS	   18022
#define OBJ_VNUM_FORGE_FEET	   18021
#define OBJ_VNUM_FORGE_HANDS	   18020
#define OBJ_VNUM_FORGE_SHIELD	   18019
#define OBJ_VNUM_BREATHFIRE	   18305
#define OBJ_VNUM_COOKING_KIT	   18297
#define OBJ_VNUM_BUILDER	   18005
#define OBJ_VNUM_RENAME            18005 
#define OBJ_VNUM_TOKEN		   18108 
#define OBJ_VNUM_TOKEN_2	   18109 
#define OBJ_VNUM_TOKEN_3	   18110 
#define OBJ_VNUM_TOKEN_4	   18111 
#define OBJ_VNUM_TOKEN_5	   18112 
#define OBJ_VNUM_TOKEN_6	   18113
#define OBJ_VNUM_TOKEN_7           18114
#define OBJ_VNUM_TOKEN_8           18115
#define OBJ_VNUM_TOKEN_9           18116
#define OBJ_VNUM_TOKEN_10          18117
#define OBJ_VNUM_TOKEN_11          18118
#define OBJ_VNUM_TOKEN_12          18119
#define OBJ_VNUM_TOKEN_13          18120
#define OBJ_VNUM_TOKEN_14          18121
#define OBJ_VNUM_TOKEN_15          18122
#define OBJ_VNUM_TOKEN_16          18123
#define OBJ_VNUM_TOKEN_17          18124
#define OBJ_VNUM_TOKEN_18          18125 
#define OBJ_VNUM_TOKEN_19          18126 
#define OBJ_VNUM_TOKEN_20          18127 
#define OBJ_VNUM_TOKEN_21          18128 
#define OBJ_VNUM_TOKEN_22          18129 
#define OBJ_VNUM_TOKEN_23          18132
#define OBJ_VNUM_TOKEN_24          18133
#define OBJ_VNUM_TOKEN_25          18134
#define OBJ_VNUM_TOKEN_26          18135
#define OBJ_VNUM_TOKEN_27          18136
#define OBJ_VNUM_TOKEN_28          18137
#define OBJ_VNUM_TOKEN_29          18138
#define OBJ_VNUM_TOKEN_30          18139
#define OBJ_VNUM_TOKEN_31          18140
#define OBJ_VNUM_TOKEN_32          18141
#define OBJ_VNUM_TOKEN_33          18142
#define OBJ_VNUM_TOKEN_34          18143
#define OBJ_VNUM_TOKEN_35          18144
#define OBJ_VNUM_TOKEN_36          18145
#define OBJ_VNUM_TOKEN_37          18146
#define OBJ_VNUM_TOKEN_38          18147
#define OBJ_VNUM_TOKEN_39          18148
#define OBJ_VNUM_TOKEN_40          18149
#define OBJ_VNUM_TOKEN_41          18150
#define OBJ_VNUM_TOKEN_42          18151
#define OBJ_VNUM_TOKEN_43          18152
#define OBJ_VNUM_TOKEN_44          18153
#define OBJ_VNUM_TOKEN_45          18154
#define OBJ_VNUM_TOKEN_46          18155
#define OBJ_VNUM_TOKEN_47          18156
#define OBJ_VNUM_TOKEN_48          18157
#define OBJ_VNUM_TOKEN_49          18158
#define OBJ_VNUM_TOKEN_50          18159
#define OBJ_VNUM_TOKEN_51          18160
#define OBJ_VNUM_TOKEN_52          18161
#define OBJ_VNUM_TOKEN_53          18162
#define OBJ_VNUM_TOKEN_54          18163
#define OBJ_VNUM_TOKEN_55          18164
#define OBJ_VNUM_TOKEN_56          18165
#define OBJ_VNUM_TOKEN_57          18166
#define OBJ_VNUM_TOKEN_58          18167
#define OBJ_VNUM_TOKEN_59          18168
#define OBJ_VNUM_TOKEN_60          18169
#define OBJ_VNUM_TOKEN_61          18170
#define OBJ_VNUM_TOKEN_62          18171
#define OBJ_VNUM_TOKEN_63          18172
#define OBJ_VNUM_TOKEN_64          18173
#define OBJ_VNUM_TOKEN_65          18174
#define OBJ_VNUM_TOKEN_66          18175
#define OBJ_VNUM_TOKEN_67          18176
#define OBJ_VNUM_TOKEN_68          18177
#define OBJ_VNUM_TOKEN_69          18178
#define OBJ_VNUM_TOKEN_70          18179
#define OBJ_VNUM_TOKEN_71          18180
#define OBJ_VNUM_TOKEN_72          18181
#define OBJ_VNUM_TOKEN_73          18182
#define OBJ_VNUM_TOKEN_74          18183
#define OBJ_VNUM_TOKEN_75          18184
#define OBJ_VNUM_TOKEN_76          18185
#define OBJ_VNUM_TOKEN_77          18186
#define OBJ_VNUM_TOKEN_78          18187
#define OBJ_VNUM_TOKEN_79          18188
#define OBJ_VNUM_TOKEN_80          18189
#define OBJ_VNUM_TOKEN_81          18192
#define OBJ_VNUM_TOKEN_82          18193
#define OBJ_VNUM_TOKEN_83          18202
#define OBJ_VNUM_TOKEN_84          18203
#define OBJ_VNUM_TOKEN_85          18204
#define OBJ_VNUM_TOKEN_86          18205
#define OBJ_VNUM_TOKEN_87          18206
#define OBJ_VNUM_TOKEN_88          18207
#define OBJ_VNUM_TOKEN_89          18208
#define OBJ_VNUM_TOKEN_90          18209
#define OBJ_VNUM_TOKEN_91          18210
#define OBJ_VNUM_TOKEN_92          18211
#define OBJ_VNUM_TOKEN_93          18212
#define OBJ_VNUM_TOKEN_94          18213
#define OBJ_VNUM_TOKEN_95          18214
#define OBJ_VNUM_TOKEN_96          18215
#define OBJ_VNUM_TOKEN_97          18216
#define OBJ_VNUM_TOKEN_98          18217
#define OBJ_VNUM_TOKEN_99          18218
#define OBJ_VNUM_TOKEN_100          18219
#define OBJ_VNUM_TOKEN_101          18220
#define OBJ_VNUM_TOKEN_102          18221
#define OBJ_VNUM_TOKEN_103          18222
#define OBJ_VNUM_TOKEN_104          18223
#define OBJ_VNUM_TOKEN_105          18224
#define OBJ_VNUM_TOKEN_106          18225
#define OBJ_VNUM_TOKEN_107          18226
#define OBJ_VNUM_TOKEN_108          18227
#define OBJ_VNUM_TOKEN_109          18228
#define OBJ_VNUM_TOKEN_110          18229
#define OBJ_VNUM_TOKEN_111          18230
#define OBJ_VNUM_TOKEN_112          18231
#define OBJ_VNUM_TOKEN_113          18232
#define OBJ_VNUM_TOKEN_114          18233
#define OBJ_VNUM_TOKEN_115          18234
#define OBJ_VNUM_TOKEN_116          18235
#define OBJ_VNUM_TOKEN_117          18236
#define OBJ_VNUM_TOKEN_118          18237
#define OBJ_VNUM_TOKEN_119          18238
#define OBJ_VNUM_TOKEN_120          18239
#define OBJ_VNUM_TOKEN_121          18240
#define OBJ_VNUM_TOKEN_122          18241
#define OBJ_VNUM_TOKEN_123          18242
#define OBJ_VNUM_TOKEN_124          18243
#define OBJ_VNUM_TOKEN_125          18244
#define OBJ_VNUM_TOKEN_126          18245
#define OBJ_VNUM_TOKEN_127          18246
#define OBJ_VNUM_TOKEN_128          18247
#define OBJ_VNUM_TOKEN_129          18248
#define OBJ_VNUM_TOKEN_130          18249
#define OBJ_VNUM_TOKEN_131          18250
#define OBJ_VNUM_TOKEN_132          18251
#define OBJ_VNUM_TOKEN_133          18252
#define OBJ_VNUM_TOKEN_134          18253
#define OBJ_VNUM_TOKEN_135          18254
#define OBJ_VNUM_TOKEN_136          18255
#define OBJ_VNUM_TOKEN_137          18256
#define OBJ_VNUM_TOKEN_138          18257
#define OBJ_VNUM_TOKEN_139          18258
#define OBJ_VNUM_TOKEN_140          18259
#define OBJ_VNUM_TOKEN_141          18260
#define OBJ_VNUM_TOKEN_142          18261
#define OBJ_VNUM_TOKEN_143          18262
#define OBJ_VNUM_TOKEN_144          18263
#define OBJ_VNUM_TOKEN_145          18264
#define OBJ_VNUM_TOKEN_146          18265
#define OBJ_VNUM_TOKEN_147          18266
#define OBJ_VNUM_TOKEN_148          18267
#define OBJ_VNUM_TOKEN_149          18268
#define OBJ_VNUM_TOKEN_150          18269
#define OBJ_VNUM_TOKEN_151          18270
#define OBJ_VNUM_TOKEN_152          18271
#define OBJ_VNUM_TOKEN_153          18272
#define OBJ_VNUM_TOKEN_154          18273
#define OBJ_VNUM_TOKEN_155          18274
#define OBJ_VNUM_TOKEN_156          18275
#define OBJ_VNUM_TOKEN_157          18277
#define OBJ_VNUM_TOKEN_158          18278
#define OBJ_VNUM_TOKEN_159          18279
#define OBJ_VNUM_TOKEN_160          18280
#define OBJ_VNUM_TOKEN_161          18281
#define OBJ_VNUM_TOKEN_162          18282
#define OBJ_VNUM_TOKEN_163          18283
#define OBJ_VNUM_TOKEN_164          18284
#define OBJ_VNUM_TOKEN_165          18285
#define OBJ_VNUM_TOKEN_166          18286
#define OBJ_VNUM_TOKEN_167          18287
#define OBJ_VNUM_TOKEN_168          18288
#define OBJ_VNUM_BLACK_GATE         18304
#define OBJ_VNUM_INV 		    18000 
#define OBJ_VNUM_WED_RING	    18042 
#define OBJ_VNUM_PRAC_ITEM	    21 

/* shope types */
#define SHOP_BAKER		(A)
#define SHOP_MAGIC		(B)
#define SHOP_BAR		(C)
#define SHOP_ARMOUR		(D)
#define SHOP_WEAPON		(E)
#define SHOP_JEWELLER		(F)
#define SHOP_GROCER		(G)
#define SHOP_FURNITURE		(H)
#define SHOP_BOAT		(I)
#define SHOP_BARDING		(J)
#define SHOP_OWNABLE		(K)

/*
 * Item types.
 * Used in #OBJECTS.
 */
#define ITEM_LIGHT		      1
#define ITEM_SCROLL		      2
#define ITEM_WAND		      3
#define ITEM_STAFF		      4
#define ITEM_WEAPON		      5
#define ITEM_TREASURE		      8
#define ITEM_ARMOR		      9
#define ITEM_POTION		     10
#define ITEM_CLOTHING		     11
#define ITEM_FURNITURE		     12
#define ITEM_TRASH		     13
#define ITEM_CONTAINER		     15
#define ITEM_DRINK_CON		     17
#define ITEM_KEY		     18
#define ITEM_FOOD		     19
#define ITEM_MONEY		     20
#define ITEM_BOAT		     22
#define ITEM_CORPSE_NPC		     23
#define ITEM_CORPSE_PC		     24
#define ITEM_FOUNTAIN		     25
#define ITEM_PILL		     26
#define ITEM_PROTECT		     27
#define ITEM_MAP		     28
#define ITEM_GATE		     29
#define ITEM_PC		 	     30
#define ITEM_NO_FOOD_REQ	     31
#define ITEM_NO_WATER_REQ	     32
#define ITEM_NO_FOOD_OR_WATER	     33
#define ITEM_POISONED		     34
#define ITEM_TYPE_BARDING	     35
#define ITEM_FUEL		     36
#define ITEM_LIGHTER		     37
#define ITEM_SNOWSHOE		     38

/*
 * Special flags.
 * Used in #OBJECTS ( index )
 */
#define ITEM_SPEC_STRING	(A)

/*
 * Extra flags.
 * Used in #OBJECTS.
 */
#define ITEM_GLOW		(A)
#define ITEM_HUM		(B)
#define ITEM_NO_SHOW		(C)
#define ITEM_BUCKLER		(D)
#define ITEM_HAMMER		(E)
#define ITEM_SHADOWFORM		(F)
#define ITEM_MAGIC		(G)
#define ITEM_NODROP		(H)
#define ITEM_ANTI_NENKEMEN	(K)
#define ITEM_ANTI_MAEGMENEL	(J)
#define ITEM_ANTI_LITHDOR	(L)
#define ITEM_NOREMOVE		(M)
#define ITEM_INVENTORY		(N)
#define ITEM_NOPURGE		(O)
#define ITEM_ROT_DEATH		(P)
#define ITEM_VIS_DEATH		(Q)
#define ITEM_LIT		(R)
#define ITEM_MAGIC_LIT          (S)
#define ITEM_NO_SAVE            (T)
#define ITEM_NO_LOCATE          (U)
#define ITEM_BURIED		(V)
#define ITEM_ASSAYED		(W)
#define ITEM_NO_IDENTIFY        (X)
#define ITEM_FLOAT		(Y)
#define ITEM_BARDING		(Z)
#define ITEM_STAY_ON_DEATH	(aa)
#define ITEM_PICK		(bb)
#define ITEM_MOB_INSIDE		(cc)
#define ITEM_HOME		(dd)
#define ITEM_NEWBIE		(ee)
/*
 * Wear flags.
 * Used in #OBJECTS.
 */
#define ITEM_TAKE		(A)
#define ITEM_WEAR_FINGER	(B)
#define ITEM_WEAR_NECK		(C)
#define ITEM_WEAR_BODY		(D)
#define ITEM_WEAR_HEAD		(E)
#define ITEM_WEAR_LEGS		(F)
#define ITEM_WEAR_FEET		(G)
#define ITEM_WEAR_HANDS		(H)
#define ITEM_WEAR_ARMS		(I)
#define ITEM_WEAR_SHIELD	(J)
#define ITEM_WEAR_ABOUT		(K)
#define ITEM_WEAR_WAIST		(L)
#define ITEM_WEAR_WRIST		(M)
#define ITEM_WIELD		(N)
#define ITEM_HOLD		(O)
#define ITEM_TWO_HANDS		(P)
#define ITEM_MONEY_0		(Q)
#define ITEM_MONEY_1		(R)
#define ITEM_MONEY_2		(S)
#define ITEM_MONEY_3		(T)
#define ITEM_WEAR_EAR		(U)
#define ITEM_WEAR_LIGHT		(V)

/* weapon class */
#define WEAPON_EXOTIC		0
#define WEAPON_SWORD		1
#define WEAPON_DAGGER		2
#define WEAPON_SPEAR		3
#define WEAPON_MACE		4
#define WEAPON_AXE		5
#define WEAPON_FLAIL		6
#define WEAPON_WHIP		7	
#define WEAPON_POLEARM		8
/* weapon types */
#define WEAPON_FLAMING		(A)
#define WEAPON_FROST		(B)
#define WEAPON_VAMPIRIC		(C)
#define WEAPON_SHARP		(D)
#define WEAPON_VORPAL		(E)
#define WEAPON_TWO_HANDS	(F)
#define WEAPON_BASTARD		(G)
#define WEAPON_KNIFE		(H)
#define WEAPON_POISON		(I)

/* Apply types (for affects).
 * Used by rooms
 */
#define APPLY_ROOM_FLAG			1
#define APPLY_ROOM_FLAGS		1
#define APPLY_SECTOR_TYPE		2

/*
 * Apply types (for affects).
 * Used in #OBJECTS.
 */
#define APPLY_NONE		      0
#define APPLY_STR		      1
#define APPLY_DEX		      2
#define APPLY_INT		      3
#define APPLY_WIS		      4
#define APPLY_CON		      5
#define APPLY_SEX		      6
#define APPLY_CHR		      7
#define APPLY_LEVEL		      8
#define APPLY_AGE		      9
#define APPLY_HEIGHT		     10
#define APPLY_WEIGHT		     11
#define APPLY_MANA		     12
#define APPLY_HIT		     13
#define APPLY_MOVE		     14
#define APPLY_GOLD		     15
#define APPLY_EXP		     16
#define APPLY_AC		     17
#define APPLY_HITROLL		     18
#define APPLY_DAMROLL		     19
#define APPLY_SAVING_PARA	     20
#define APPLY_SAVING_ROD	     21
#define APPLY_SAVING_PETRI	     22
#define APPLY_SAVING_BREATH	     23
#define APPLY_SAVING_SPELL	     24
#define APPLY_NO_WATER		     25
#define APPLY_NO_FOOD		     26
#define APPLY_AFFECT		     27
#define APPLY_MANA_FIRE 	     30
#define APPLY_MANA_WATER	     31
#define APPLY_MANA_EARTH	     32
#define APPLY_MANA_WIND 	     33
#define APPLY_MANA_SPIRIT	     34
#define APPLY_DREAMS		     35
#define APPLY_NIGHTMARES	     36
#define APPLY_MAX		     37 /* Please increase as more applys are added */

/*
 * Values for containers (value[1]).
 * Used in #OBJECTS.
 */
#define CONT_CLOSEABLE		      1
#define CONT_PICKPROOF		      2
#define CONT_CLOSED		      4
#define CONT_LOCKED		      8
#define CONT_HOLDING		     16
#define CONT_EXTRA_HOLDING	     32
#define CONT_SUPER_HOLDING	     64
#define CONT_DEVOURING		    128
#define CONT_PERM_SAVE		    256
#define CONT_WATER_PROOF	    512

/*
 * Values for containers (value[5]).
 * Used in #OBJECTS.
 */
#define PERM_SAVE		      1

/*
 * Well known room virtual numbers.
 * Defined in #ROOMS.
 */
#define ROOM_VNUM_TEMPLATE	      2   /* scripts */
#define ROOM_VNUM_LIMBO		      2
#define ROOM_VNUM_CHAT		   3001
#define ROOM_VNUM_TEMPLE	   3001
#define ROOM_VNUM_ALTAR		   3054
#define ROOM_VNUM_SCHOOL	   2228
#define DEFAULT_HELP_VNUM	   1
#define BEGIN_ROOM_COACH  	   171
#define BEGIN_ROOM_SHIP		   122


/*
 * Room flags.
 * Used in #ROOMS.
 */
#define ROOM_DARK		(A)
#define ROOM_SILENCE		(B)
#define ROOM_NO_MOB		(C)
#define ROOM_INDOORS		(D)
#define ROOM_MANAGEN		(E)
#define ROOM_HPGEN		(F)
#define ROOM_MVGEN		(G)
#define ROOM_MOUNT_ONLY		(H)
#define ROOM_NO_MOUNT		(I)
#define ROOM_PRIVATE		(J)
#define ROOM_SAFE		(K)
#define ROOM_SOLITARY		(L)
#define ROOM_PET_SHOP		(M)
#define ROOM_NO_RECALL		(N)
#define ROOM_IMP_ONLY		(O)
#define ROOM_GODS_ONLY		(P)
#define ROOM_HEROES_ONLY	(Q)
#define ROOM_NEWBIES_ONLY	(R)
#define ROOM_BOTTOMLESS		(T)
#define ROOM_MOVE_UP		(U)
#define ROOM_MOVE_DOWN		(V)
#define ROOM_MOVE_NORTH		(W)
#define ROOM_MOVE_SOUTH		(X)
#define ROOM_MOVE_EAST		(Y)
#define ROOM_MOVE_WEST		(Z)
#define ROOM_FORGE		(aa)
#define ROOM_THIRST		(cc)
#define ROOM_MOUNT_SHOP		(dd)

/*
 * Room flags 2.
 * Used in #ROOMS.
 */
#define ROOM_SAVE		(A)
#define ROOM_STARTING		(B)
#define ROOM_BFS_MARK		(C)
#define ROOM_NOSAVE		(D)
#define ROOM_DELETE		(E)
#define ROOM_GEN_DUNGEON        (F)
#define ROOM_LIT		(G)

/*
 * Directions.
 * Used in #ROOMS.
 */
#define DIR_NORTH		      0
#define DIR_EAST		      1
#define DIR_SOUTH		      2
#define DIR_WEST		      3
#define DIR_UP			      4
#define DIR_DOWN		      5

#define PORTAL_MOVES		      1
#define PORTAL_STAY_IN_AREA	      2
#define PORTAL_STAY_IN_WORLD	      4

#define GATE_STAY_AREA		      (A)
#define GATE_STAY_WORLD		      (B)
#define GATE_TO_STAY_AREA	      (C)
#define GATE_TO_STAY_WORLD	      (D)
#define GATE_UNSTABLE		      (F)
#define GATE_TO_MOB		      (G)
#define GATE_TO_OBJ		      (H)
#define GATE_STAY_ROOM		      (I)
#define GATE_TO_STAY_ROOM	      (J)
#define GATE_OPAQUE		      (K)

/*
 * Exit flags.
 * Used in #ROOMS.
 */
#define EX_ISDOOR		      (A)
#define EX_CLOSED		      (B)
#define EX_HIDDEN		      (G)

#define EX_BASHED                     (H)
#define EX_BASHPROOF                  (I)
#define EX_PASSPROOF                  (J)
#define EX_SECRET		      (K)
#define EX_WALL			      (L)
#define EX_ILLUSION		      (M) /* Transparent wall */

#define LOCK_CLOSED		      (B)  /*ignored*/
#define LOCK_LOCKED		      (C)
#define LOCK_DEAD_BOLT		      (D)
#define LOCK_PICKPROOF		      (F)

#define TRAP_AREA		      (A)
#define TRAP_RANDOM_AREA	      (B)
#define TRAP_RANDOM_ONE		      (C)
#define TRAP_SPRUNG		      (D)
#define TRAP_UNLOCK		      (E)
#define TRAP_PICK		      (F)
#define TRAP_OPEN		      (G)
#define TRAP_ENTER		      (H)
#define TRAP_AUTO_RESET		      (I)

#define LIGHT_0				0
#define LIGHT_1				1
#define LIGHT_2				2
#define LIGHT_3				3
#define LIGHT_4				4

#define CLAN_UNLISTED		    	(A)

/*
 * Sector types.
 * Used in #ROOMS.
 */
#define SECT_NONE		 	0
#define SECT_CITY		 	1
#define SECT_FIELD		 	2
#define SECT_FOREST		 	3
#define SECT_HILLS		 	4
#define SECT_MOUNTAIN		 	5
#define SECT_WATER_SWIM		 	6
#define SECT_WATER_NOSWIM	 	7
#define SECT_UNDERWATER		 	8
#define SECT_AIR		 	9
#define SECT_DESERT		 	10
#define SECT_LAVA		 	11
#define	SECT_DEATH		 	12
#define SECT_PROTOTYPE		 	13
#define	SECT_SWAMP		 	14
#define	SECT_DOCK		 	15
#define	SECT_UNDERGROUND	 	16
#define	SECT_SNOW		 	17
#define	SECT_ICE		 	18
#define	SECT_DIRT		 	19
#define	SECT_GRASS		 	20
#define	SECT_SAND		 	21
#define	SECT_STREAM		 	22
#define SECT_OCEAN_BOTTOM		23
#define SECT_PC			 	31
#define SECT_MAX		 	32  /* don't confuse with MAX_SECTOR */
#define MAX_SECTORS			24  /* change as you add more valid sectors */
/*
 * Equpiment wear locations.
 * Used in #RESETS.
 */
#define WEAR_NONE		     -1
#define WEAR_LIGHT		      0
#define WEAR_FINGER_L		      1
#define WEAR_FINGER_R		      2
#define WEAR_NECK_1		      3
#define WEAR_NECK_2		      4
#define WEAR_BODY		      5
#define WEAR_HEAD		      6
#define WEAR_LEGS		      7
#define WEAR_FEET		      8
#define WEAR_HANDS		      9
#define WEAR_ARMS		     10
#define WEAR_SHIELD		     11
#define WEAR_ABOUT		     12
#define WEAR_WAIST		     13
#define WEAR_WRIST_L		     14
#define WEAR_WRIST_R		     15
#define WEAR_WIELD		     16
#define WEAR_WIELD_R		     16
#define WEAR_HOLD		     17
#define WEAR_WIELD_TWO_HANDED	     18
#define WEAR_WIELD_L		     19
#define WEAR_EAR_L		     20
#define WEAR_EAR_R		     21
#define MAX_WEAR		     22


/***************************************************************************
 *                                                                         *
 *                   VALUES OF INTEREST TO AREA BUILDERS                   *
 *                   (End of this section ... stop here)                   *
 *                                                                         *
 ***************************************************************************/

/*
 * Conditions.
 */
#define COND_DRUNK		      0
#define COND_FULL		      1
#define COND_THIRST		      2

/*
 * Positions.
 */
#define POS_DEAD		      0
#define POS_MORTAL		      1
#define POS_INCAP		      2
#define POS_STUNNED		      3
#define POS_SLEEPING		      4
#define POS_RESTING		      5
#define POS_SITTING		      6
#define POS_FIGHTING		      7
#define POS_STANDING		      8



/*
 * TMPACT bits for players.
 */
#define TMPACT_AWAY		(A)

/*
 * INFOACT bits for players.
 */
#define INFOACT_DEATH		(A)
#define INFOACT_PERMISSION      (B)
#define INFOACT_RESET           (C)
#define INFOACT_BUG             (D)
#define INFOACT_RENT            (E)
#define INFOACT_QUIT            (F)
#define INFOACT_LINK            (G)
#define INFOACT_LOGIN           (H)
#define INFOACT_RECONNECT	(I)
#define INFOACT_SNOOPS		(J)
#define INFOACT_LEVEL		(K)
#define INFOACT_CORPSE		(L)
#define INFOACT_SAVE_ZONE	(M)
#define INFOACT_OBJ_CHECK	(N)
#define INFOACT_HELP		(O)
#define INFOACT_DEBUGRESET      (Z)
#define INFOACT_IMP_LOG		(aa)
#define INFOACT_CODER		(bb)


/*
 * plr bits for players.
 */
#define PLR_IS_NPC		(A)		/* Don't EVER set.	*/
#define PLR_BOUGHT_PET		(B)

/* RT auto flags */
#define PLR_AUTOASSIST		(C)
#define PLR_AUTOEXIT		(D)
#define PLR_AUTOLOOT		(E)
#define PLR_AUTOSAC             (F)
#define PLR_AUTOGOLD		(G)
#define PLR_AUTOSPLIT		(H)
#define PLR_AUTODIRS		(I)
#define PLR_ANSI		(J)
#define PLR_AUTOPROMPT		(K)
#define PLR_FLEE		(L)
#define PLR_AUTOMAP		(M)

/* RT personal flags */
#define PLR_HOLYLIGHT		(N)
#define PLR_WIZINVIS		(O)
#define PLR_CANLOOT		(P)
#define PLR_NOSUMMON		(Q)
#define PLR_NOFOLLOW		(R)
#define PLR_ROOMSTATS		(S)
#define PLR_SECTORSTATS		(T)
#define PLR_WIZPASS		(U)
#define PLR_WEATHER_OFF		(V)

/* penalty flags */
#define PLR_LOG			(W)
#define PLR_DENY		(X)
#define PLR_FREEZE		(Y)
#define PLR_THIEF		(Z)
#define PLR_PK			(aa)
#define PLR_RUNNING		(dd)
#define PLR_DIRECT		(ee)  /* direct login (Quin) */

/* PLR2 flags - never can be enough player flags */

#define PLR_NOMISS		(A)
#define PLR_FREE_SPACE		(B)  /* old auto-teach, now free space */
#define PLR_SPECIAL_PASS	(C)  /* special temporary passing for room_move */
#define PLR_PLAYER_HELPER	(D)  /* For those mortals/immortals that are helpers*/
#define PLR_WHO_GODS		(E)
#define PLR_WHO_NORMAL		(F)
#define PLR_FASTEXP		(H) /* Used for leveling fast.  This is not for player use */
#define PLR_DREAMSTATE		(I) 
#define PLR_VNUMS_ON		(J) 

/*
 * For oedit 
 */
#define OEDIT_ANY			1
#define MEDIT_ANY			1
#define REDIT_ANY			1
#define GEDIT_ANY			1
#define OEDIT_MENU			2
#define MEDIT_MENU			2
#define REDIT_MENU			2
#define GEDIT_MENU			2
#define REDIT_DOOR_MENU			3
#define MEDIT_FORM			3
#define GEDIT_WORLD			3
#define OEDIT_LIGHT_MENU		3
#define OEDIT_WEAR_MENU			4
#define REDIT_SPEC_MENU			4
#define GEDIT_AREA			4
#define MEDIT_COMMANDS			4
#define MEDIT_FORM_MOVE		 	5
#define OEDIT_WEAR_WEAPON_MENU		5
#define GEDIT_GLOBAL			5
#define OEDIT_WEAR_MAGIC_MENU		6
#define MEDIT_SPECIALS			6
#define OEDIT_WEAR_AFFECT_MENU		7
#define REDIT_AFFECTS_MENU		7
#define MEDIT_SKILLS			7
#define REDIT_EVENT_MENU		8
#define OEDIT_INSIDE_MENU		8
#define MEDIT_EVENTS			8
#define OEDIT_GATE_MENU			9
#define MEDIT_FORM_MAGIC		9
#define OEDIT_EDIBLE_MENU		10	
#define MEDIT_FORM_GROUP		10	
#define OEDIT_CONTAINER_MENU		11	
#define MEDIT_FORM_AFFECTS		11	
#define OEDIT_CONTAINER_LOCK_MENU	12	
#define MEDIT_FORM_COMBAT		12	
#define OEDIT_CONTAINER_TRAP_MENU	13	
#define OEDIT_EXTRA_MENU		14	
#define MEDIT_ADVANCED		 	14
#define REDIT_EXTRA_MENU		14	
#define SECURITY_1			1
#define SECURITY_2			2
#define SECURITY_3			3
#define SECURITY_4			4
#define SECURITY_5			5
#define SECURITY_6			6
#define SECURITY_7			7
#define SECURITY_8			8
#define SECURITY_9			9
#define SECURITY_CHECK		(A)
/* Group flags */
#define MAX_CMD_GROUPS		10
#define CMD_GROUP_PLAYER	(A)
#define CMD_GROUP_BASIC_BUILD	(B)
#define CMD_GROUP_ADVANCE_BUILD (C)
#define CMD_GROUP_INFO		(D)
#define CMD_GROUP_DEFAULT	(E)
#define CMD_GROUP_GAME_CONTROL	(F)
#define CMD_GROUP_ADVANCE_PHELP	(G)
#define CMD_GROUP_BASIC_PHELP	(H)
#define CMD_GROUP_ADMIN		(I)
#define CMD_GROUP_MOBILE	(J)

/* RT comm flags -- may be used on both mobs and chars */

#define COMM_QUIET              (A)
#define COMM_DEAF               (B)
#define COMM_NOWIZ              (C)
#define COMM_NOAUCTION          (D)
#define COMM_NOTHINK            (E)
#define COMM_NOOOC   	        (F)
#define COMM_NOPRAY             (G)
#define COMM_INFO		(H)
#define COMM_NOHELP		(J)

/* display flags */
#define COMM_COMPACT		(L)
#define COMM_BRIEF		(M)
#define COMM_PROMPT		(N)
#define COMM_COMBINE		(O)
#define COMM_TELNET_GA		(P)
/* 3 flags reserved, Q-S */

/* penalties */
#define COMM_NOEMOTE		(T)
#define COMM_NOSHOUT		(U)
#define COMM_NOTELL		(V)
#define COMM_NOCHANNELS		(W) 
#define COMM_NO_REC_TELL	(X) 

/* forging flags */
#define IS_FORGING		(A)
#define FORGE_COMPLETE		(B)

/* Dream types for players */
#define IS_DREAMING		(A)
#define DREAM_IS_NIGHTMARE	(B)
#define DREAM_IS_NORMAL		(C)
#define DREAM_IS_NORMAL_2	(D)
#define DREAM_IS_NORMAL_3	(E)
#define DREAM_IS_NORMAL_4	(F)
#define DREAM_IS_GOOD		(G)
#define DREAM_IS_GOOD_2		(H)
#define DREAM_IS_GOOD_3		(I)
#define DREAM_IS_GOOD_4		(J)
/*
 * Prototype for a mob.
 * This is the in-memory version of #MOBILES.
 */
struct gainer_type
{
    GAINER_DATA *	next;
    bool		skill[MAX_SKILL];
};

/*
 * Moveable data 
 */
struct moveable
{
    MOVEABLE_DATA	* next;
    long	area_start_room;		/* Starting room number for area */
    char 	leave_msg[16];			/*message char-room sees while leaving */
    char 	board_msg[16];			/*message char-room sees while boarding */
    char 	move_msg[16];			/*message char-room sees while moving */
};
/*
 * Random data 
 */
struct global_random_data
{
    GLOBAL_RANDOM_DATA *next;
    sh_int		chance;
    sh_int		max;
    sh_int		count;
    sh_int		world;
    sh_int		area;
    sh_int		global;
    long		vnum;
};

/*
 * Logon data 
 */
struct logon_data
{
    LOGON_DATA *	next;
    char 		name[31];
    time_t		time_start;
    time_t		time_end;
    sh_int		kills;
    sh_int		pkills;
    long		exp;
    sh_int		rooms;
    sh_int		objects;
    sh_int		mobiles;
    char 		exit[31];
    sh_int		level;
    char		last_ip[16];
};

/*
 * Finger Data
 */
struct finger_data
{
    FINGER_DATA *	next;
    char 		name[31];
    char 		title[81];
    char *		desc;
    char 		email[81];
    long		laston;
    int			level;
    int			race;
    int			sex;
};

/*
 * Wizlist data 
 */
#define WIZLIST_DELETED A
struct wizlist_data
{
    WIZLIST_DATA *	next;
    char 		name[31];
    char 		title[31];
    char 		desc[81];
    int			order;
    long		flags;
};

/*
 * Equip Flags
 */
#define EQUIP_INVENTORY		(A)
#define EQUIP_REPLACE		(B)
#define EQUIP_ON_BOOT		(C)
#define EQUIP_ON_CREATION	(D)
#define EQUIP_ON_RESET		(E)
#define EQUIP_INVALID		(F)
/*
 * Equip Data
 */
struct equip_data
{
    EQUIP_DATA * 	next;
    OBJ_DATA *		obj;
    long		obj_vnum;
    int			chance;
    long		flags;
};
/*
 * Inside Data
 */
struct inside_data
{
    INSIDE_DATA * 	next;
    OBJ_DATA *		obj;
    CHAR_DATA *		mob;
    long		obj_vnum;
    long		mob_vnum;
    int			chance;
    long		flags;
};


struct	mob_index_data
{
    char                spec_name[31];	/* OLC */
    AREA_DATA *		area;		/* OLC */
    MOB_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    SPEC_FUN *		spec_fun;
    SHOP_DATA *		pShop;
    AREA_DATA *		load_with_area;
    TRIGGER_DATA *	triggers;	/* scripts */ 
    TRIGGER_LIST_DATA *	trigger_list;	/* scripts */ 
    VARIABLE_DATA *	variables;	/* scripts */
    GAINER_DATA *	gainer_data;
    MOVEABLE_DATA *	moveable;
    SKILL_LIST *	skills;
    SKILL_LIST *	innate_skills;
    EQUIP_DATA	*	equip;
    char *		script_fn;	/* scripts */
    sh_int		valid_sector[SECT_MAX];	/*Valid sectors for moveable to goto or steer */
    int			prac_skills;
    int			prac_spells;
    long		vnum;
    sh_int		count;
    sh_int		killed;
    char 		player_name[31];
    char 		short_descr[81];
    char 		long_descr[81];
    char *		description;
    char		movement_msg[16];
    long		act;
    long		act2;
    long		affected_by;
    long		affected_by_2;
    sh_int		level;
    long		follower;
    sh_int		hitroll;
    sh_int		hit[3];
    sh_int		mana[3][MAX_ELEMENT_TYPE];
    sh_int		damage[3];
    sh_int		armor;
    sh_int 		dam_type;
    /* castle code */
    sh_int		castle_area;
    long		off_flags;
    long		imm_flags;
    long		res_flags;
    long		vuln_flags;
    sh_int		start_pos;
    sh_int		default_pos;
    sh_int		sex;
    sh_int		mood;
    sh_int		race;
    long		gold[ MAX_WORLD ];
    long		form;
    long 		guilds;
    sh_int		size;
    sh_int		material;
    sh_int		height;
    sh_int		weight;
};

/*
 * Struct for Group code
 * Added by Trice Sep 13, 1996
 */
#define GROUP_IS_SECRET		A 	/* group only seen by group members */
#define GROUP_IS_PUBLIC		B 	/* group seen by anyone */
#define GROUP_IS_MOBILE		C 	/* Mobile Groups */

struct group_data 
{
	char		name[81];
	CHAR_DATA	*leader;
	CHAR_DATA	*treasurer;
	long		gold[MAX_WORLD];
	int		pkills;
	int		mob_kills;
	long		flags;
	GROUP_DATA	*next; 
};
	
struct card_type
{
    CARD_DATA *		next;
    int			value;
    int			suit;
};

/*
 * One character (PC or NPC).
 */
struct char_data
{
    CHAR_DATA *		next;
    CHAR_DATA *		mob_next_update;
    CHAR_DATA *		next_in_room;
    CHAR_DATA *		master;
    CHAR_DATA *		leader;
    CHAR_DATA *		fighting;
    CHAR_DATA *		reply;
    CHAR_DATA *		follower;
    CHAR_DATA *		next_follower;
    CHAR_DATA *		next_in_group;
    GROUP_DATA *	group;
    CHAR_DATA *		pet;
    CHAR_DATA *		mount;
    CHAR_DATA *		victim;
    SPEC_FUN *		spec_fun;
    MOB_INDEX_DATA *	pIndexData;
    DESCRIPTOR_DATA *	desc;
    DESCRIPTOR_DATA *	chdesc;
    AFFECT_DATA *	affected;
    NOTE_DATA *		pnote;
    OBJ_DATA *		carrying;
    ROOM_INDEX_DATA *	in_room;
    ROOM_INDEX_DATA *	was_in_room;
    PC_DATA *		pcdata;
    INSIDE_AREA_DATA *	inside_area;
    MOVEABLE_DATA *	moveable;
    TRIGGER_DATA *	triggers;	/* scripts */
    VARIABLE_DATA * 	variables;	/* scripts */
    LOGON_DATA *	logon_data;
    SKILL_LIST *	skills;
    SKILL_LIST *	innate_skills;
    EVENT_DATA *	event;
    CHAR_QUESTS *	char_quests;
    int			quest_points; /* in process */
    long		cmd_groups;
    bool		extract;
    long		guilds;
    long		spell_flags;
    long		edit_flags;
    long		vnum_test;
    long		room_template;
    int			edit_menu;
    int			spec_tracer;	/* scripts */
    int			mob_update_number; /* game loops */
    long		dream_flags;    /* Dreaming */
    long		forge_flags;    /* Forging */
    sh_int		dream_count;    /* Dreaming */
    sh_int		forge_from;    /* Forging */
    sh_int		forge_count;    /* Forging */
    char 		forge_item[11];    /* Forging */
    char 		dislikes[31];
    char 		hunting[31];
    char  		name[31];
    char 		mate[13];
    char  		task[81];
    sh_int		version;
    char 		short_descr[81];
    char 		long_descr[81];
    char *		description;
    char		movement_msg[16];
    sh_int		sex;
    sh_int		mood;
    sh_int		race;
    sh_int		colors[MAX_COLORS];
    sh_int		level;
    sh_int		color;
    sh_int		hands_free;
    sh_int		specattack;
    long		played;
    sh_int		birth_day;
    sh_int		birth_month;
    long		walkto;
    long		walkfrom;
    int			lines;  /* for the pager */
    time_t		logon;
    sh_int		timer;
    sh_int		gos_timer;
    sh_int		happy_hour;
    sh_int		outfit_timer;
    sh_int		wait;
    sh_int		hit;
    sh_int		max_hit;
    sh_int		mana[MAX_ELEMENT_TYPE];
    sh_int		max_mana[MAX_ELEMENT_TYPE];
    sh_int		move;
    sh_int		max_move;
    long		gold[ MAX_WORLD ];
    long		exp;
    int			kills;
    int			pkills;
    int			died;
    long		act;
    long		plr;
    long		plr2;
    long		act2;
    long		tmpact;
    long		infoact;
    long		comm;   /* RT added to pad the vector */
    long		res_flags;
    long		imm_flags;
    long		vuln_flags;
    long		perm_imm;
    long		perm_res;
    long		perm_vuln;
    long		home; 
    sh_int		invis_level;
    long		affected_by;
    long		affected_by_2;
    long		regen_timer;
    sh_int		position;
    sh_int		practice;
    long		carry_weight;
    sh_int		carry_number;
    sh_int		saving_throw;
    sh_int		hitroll;
    sh_int		damroll;
    sh_int		armor;
    sh_int		wimpy;
    /* castle code */
    sh_int		castle_area;
    /* stats */
    sh_int		perm_stat[MAX_STATS];
    sh_int		mod_stat[MAX_STATS];
    /* form stuff */
    long		form;
    sh_int		size;
    sh_int		material;
    sh_int		height;		/* in inches */
    sh_int		weight;		/* in pounds */
    /* mobile stuff */
    long		off_flags;
    sh_int		damage[3];
    sh_int		dam_type;
    sh_int		start_pos;
    sh_int		default_pos;
    long		from_room;
/*
 * Valid sectors for moveable to goto or steer 
 */
    sh_int		valid_sector[MAX_SECTORS];	
    long		bank;
    char		awaymessage[41];
    char 		prefix[31];
    sh_int		prompt_num;
    DO_FUN *		prompt_fun;
    char 		edit_buffer[81];
};

struct debt_data
{
    DEBT_DATA *	next;
    long 	due;
    long 	amount;
    long 	vnum;
    int  	type;
    int		timer;
};

struct read_notes
{
    READ_NOTES *		next;
    time_t		date_stamp;
};

/*
 * Data which only PC's have.
 */
struct	pc_data
{
    PC_DATA *		next;
    OBJ_DATA *		corpse; /* for saving corpses */
    DEBT_DATA *		debt;
    char 		prompt_str[161];
    char 		Oprompt[161];
    char 		prompt[161];
    char 		pwd[31];
    char 		bamfin[160];
    char 		bamfout[160];
    char 		title[81];
    READ_NOTES	*	read_notes;
    CARD_DATA *		discarded;
    CARD_DATA * 	deck;
    CARD_DATA * 	played;
    CARD_DATA *		hand;
    sh_int		perm_hit;
    sh_int		perm_mana[MAX_ELEMENT_TYPE];
    sh_int		perm_move;
    sh_int		true_sex;
    int			minerals[16];
    int			forge[16];
    long		perm_aff;
    long		perm_aff_2;
    long		last_level;
    sh_int		condition	[3];
    sh_int		learned		[MAX_SKILL];
    sh_int		skill_mod	[MAX_SKILL];
    bool		teaching	[MAX_SKILL];
    sh_int		dreams;
    sh_int 		nightmares;
    bool              	confirm_delete;
    int			security; 
    char 		imm_title[31];    /* Title shown on players and most immortal who */
    int			home_timer;
    PC_CLAN_DATA *      pcclan;
};


/*
 * Liquids.
 */
#define LIQ_WATER        0
#define LIQ_MAX		20

struct	liq_type
{
    char *	liq_name;
    char *	liq_color;
    sh_int	liq_affect[3];
};

/*
 * Extra description data for a room or object.
 */
struct	extra_descr_data
{
    EXTRA_DESCR_DATA *next;	/* Next in list                     */
    char keyword[31];              /* Keyword in look/examine          */
    char *description;          /* What to see                      */
};



/*
 * Prototype for an object.
 */
struct	obj_index_data
{
    AREA_DATA *		area;	/* OLC */
    OBJ_INDEX_DATA *	next;
    EXTRA_DESCR_DATA *	extra_descr;
    AREA_DATA *		in_area;
    OBJ_SPEC_FUN *	spec_fun;
    WEAR_DATA *		wear_data;
    LIGHT_DATA *	light_data;
    APPROVE_DATA *	approve_data;
    CONT_DATA *		cont_data;
    GATE_DATA *		gate_data;
    EDIBLE_DATA *	edible_data;
    TRIGGER_DATA *	triggers;	/* scripts */ 
    TRIGGER_LIST_DATA *	trigger_list;	/* scripts */ 
    VARIABLE_DATA *	variables;	/* scripts */
    INSIDE_DATA * 	inside;
    char 		name[31];
    char		short_descr[81];
    char 		description[81];
    sh_int		shop_type;
    long		extra_flags;
    long		special_flags;
    long		vnum;
    sh_int		material;
    sh_int		timer;
    sh_int 		condition;
    sh_int		count;
    sh_int		points;
    sh_int		mpoints;
};

struct inside_area
{
    INSIDE_AREA_DATA *	next;
    char 		mob_name[31];
    char		short_descr[81];
    long		start_room;
};

struct weapon_data
{
    WEAPON_DATA	*	next;
    int			type;
    int			dice;
    int			die;
    int			dam_type;
    long		flags;
};

DECLARE_SPELL_FUN( spell_null	);
struct spell_list
{
    SPELL_LIST *	next;
    int			level;
    int			spell;
};

struct magic_data
{
    SPELL_LIST * 	spells;
    MAGIC_DATA *	next;   /*  Used for multiple types on one item and 
				  command words
    char *		extra_name; */
    int 		type;
    int			charges;
    int			max_charges;
    long		flags;
};

#define MAGIC_STAFF  	1
#define MAGIC_WAND 	2
#define MAGIC_SCROLL	3
#define MAGIC_TYPE_MAX  4  /*( please increase as Magic types are added )*/

#define ROT_USED	1

#define LIGHT_LIT		1
#define LIGHT_MAGIC_LIT		2
#define LIGHT_BURNING		4
#define LIGHT_LIGHTABLE		8	/* unlightable = fuel */
#define LIGHT_REFUELABLE	16
#define LIGHT_FUEL		32

struct light_data
{
    LIGHT_DATA *	next;
    long		flags;	
    int			fuel;
    int			max_fuel; 	/* -1 = infinite */ 
    sh_int		brightness;     /* torches brighter then candles
					0-10? 0-100? */
};

/*
 * Approval data for objects and mobs(?) 
 */
#define APPROVED		A
#define APPROVE_HOLD		B
struct approve_data
{
    APPROVE_DATA *	next;
    long		flags;
    char 		creater[31];
    char 		approver[31];
    char 		modifier[31];
    time_t		creation_date;
    time_t		modified_date;
    time_t		approved_date;
};

struct track_data
{
    TRACK_DATA *	next;
    CHAR_DATA *		ch;
    long		time;
    long		ctime;
    int 		track_penalty;
};

struct trap_data
{
    TRAP_DATA	*	next;
    SPELL_LIST *	spells;
    long		flags;
};

struct cont_data
{
    CONT_DATA	* next;
    RESET_DATA	* inside;
    TRAP_DATA	* trap;
    LOCK_DATA	* lock;
    OBJ_DATA	* contents;
    int		capacity;
    long	flags;
};

struct gate_data
{
    GATE_DATA		*next;
    void		*leads_to;
    long		flags;
    long		vnum;        /* room, mob, item */
    int			ave_timer;
};

struct edible_data
{
    EDIBLE_DATA	*	next;
    SPELL_LIST *	spells;
    long			flags;
    sh_int		type;
    sh_int		amount;
    sh_int		max_amount;
};

#define FOOD_POISONED		1
#define FOOD_EDIBLE		2
#define EDIBLE_FOOD		2	/* must be eaten, otherwise drunk 
					   or quaffed */
#define FOOD_UNCOOKED		4
#define FOOD_DRINK		8       /* Not used ( only for display ) */

struct wear_data
{
    WEAR_DATA   *	next;
    WEAPON_DATA *	weapon_data;
    AFFECT_DATA *	affected;
    MAGIC_DATA *	magic_data;
    long		cost;
    int			ac;
    sh_int		size;
    long		wear_flags;
    sh_int		wear_loc;
    sh_int		weight;
};

/*
 * One object.
 */
struct	obj_data
{
    OBJ_DATA *		next;
    OBJ_DATA *		next_content;
    OBJ_DATA *		in_obj;
    EXTRA_DESCR_DATA *	extra_descr;
    OBJ_INDEX_DATA *	pIndexData;
    ROOM_INDEX_DATA *	in_room;
    OBJ_SPEC_FUN *	spec_fun;
    WEAR_DATA *		wear_data;
    LIGHT_DATA *	light_data;
    CONT_DATA *		cont_data;
    GATE_DATA *		gate_data;
    EDIBLE_DATA *	edible_data;
    TRIGGER_DATA *	triggers;	/* scripts */
    VARIABLE_DATA * 	variables;	/* scripts */
    EVENT_DATA	*	event;
    EQUIP_DATA	*	equip;
    INSIDE_DATA	*	inside;
    char 		name[31];
    char 		short_descr[81];
    char 		description[81];
    long		extra_flags;
    long		special_flags;

/* to be moved */
    CHAR_DATA *		carried_by;
    char 	        owner[31];      /*  Static Allocation - GATE */ 
    sh_int		shop_type;

/* flag */
    bool		enchanted;   /* Dynamic Allocation - Player Container */
    sh_int 		condition;
    sh_int		material;
    sh_int		timer;
    sh_int		donate_timer;
    sh_int		points;
    sh_int		mpoints;
    sh_int		repaired;
};

/*
 * Exit data.
 */
struct	exit_data
{
    union
    {
	ROOM_INDEX_DATA *	to_room;
	long			vnum;
    } u1;

    ROOM_INDEX_DATA *	to_room; /* OLC */
    EXIT_DATA *		next;	/* OLC */
    LOCK_DATA *		lock;
    TRAP_DATA *		trap;
    TRACK_DATA *	tracks;
    char 		keyword[31];
    char 		description[81];
    char 		mes_to_char[81];
    char 		mes_to_room[81];
    char 		mes_at_room[81];
    long		exit_info;
    long		exit_info_default;
};

struct lock_data
{
    LOCK_DATA *		next;
    TRAP_DATA *		trap;
    long		vnum;
    long		lock_info;
    long		lock_info_default;
    sh_int		pick_penalty;
};

/*
 * Reset commands:
 *   '*': comment
 *   'M': read a mobile 
 *   'O': read an object
 *   'P': put object in object
 *   'G': give object to mobile
 *   'E': equip object to mobile
 *   'D': set state of door
 *   'R': randomize room exits
 *   'S': stop (end of list)
 */

/*
 * Area-reset definition.
 */
struct	reset_data
{
    RESET_DATA *	next;
    char		command;
    long		arg1;
    sh_int		arg2;
    long		arg3;
    long		vnum;             /* used for grouped and mounted 
					     mobs */
    sh_int		chance;
};
/*
 * Event Types
 */
#define EVENT_ROOM	1 /* Event type room */
#define EVENT_OBJ	2 /* Event type objects */
#define EVENT_MOB	3 /* Event type mobiles */
/*
 * Event Flags
 */
#define EVENT_ON_BOOT		(A)
#define EVENT_ON_RESET		(B)
#define EVENT_INVALID		(C)
/*
 * Event definition.
 */
struct	event_data
{
    EVENT_DATA *	next;
    CHAR_DATA *		mob;
    OBJ_DATA *		obj;
    ROOM_INDEX_DATA *	room;
    AREA_DATA	*	area;
    long		flags;
    long		vnum;    
    long		room_vnum;    
    long		obj_vnum;    
    long		obj_vnum_to;    
    long		mob_vnum;    
    sh_int		type;
    sh_int		chance;
};

#define	QUEST_COMPLETED		A
#define	STEP1			B
#define STEP2			C
#define	STEP3			D
#define STEP4			E
#define	STEP5			F
#define STEP6			G
#define	STEP7			H
#define STEP8			I
#define STEP9			J
#define STEP10			K
#define STEP11			L
#define STEP12			M
#define STEP13			N
#define STEP14			O
#define STEP15			P
#define STEP16			Q
#define STEP17			R
#define STEP18			S
#define STEP19			T
#define STEP20			U

/*
 * Quest data.
 */
struct	quest_data
{
    QUEST_DATA * 	next;
    char 	 	name[31];
    char  		description[513];
    int 		quest_number;
    long		flag;
};

struct world_data
{
    WORLD_DATA	*	next;
    char 		name[11];
};

/*
 * Area definition.
 */
struct	area_data
{
    AREA_DATA *		next;
    RESET_DATA *	reset_first;
    RESET_DATA *	reset_last; 
    EVENT_DATA *	events;
    WEATHER_DATA *	weather;
    CASTLE_DATA *	castle_data; /* castle code */
    char 		name[81];
    sh_int		age;
    sh_int		max_age;
    sh_int		max_age_empty;
    sh_int		nplayer;
    sh_int		wander_time;
    sh_int		areanum;
    sh_int		world;
    sh_int		time_zone;
    long		area_flags;
    char 		filename[31];
    char 		builders[31];	/* OLC */ /* Listing of */
    char 		credits[31];	/* OLC */ /* Text for 'area' */
    sh_int		security;	/* OLC */ /* Value 1-9  */
    sh_int		visible;	/* OLC */ /* Visible Level  */
    long		lvnum;		/* OLC */ /* Lower vnum */
    long		uvnum;		/* OLC */ /* Upper vnum */
    int			vnum;		/* OLC */ /* Area vnum  */
    long 		recall;         /* OLC */
};

struct	specpro_data 
{
	char		type;
	char * const	descr;	
	char * const 	trigger;
	DO_FUN *	command;
	int		spn;
	char * const    message;
};
	
struct specpro_list
{	SPECPRO_LIST	*next;
	SPECPRO_DATA	*sp;
	int		 num;
};

struct room_snoop_list
{
    ROOM_INDEX_DATA *	room;
    ROOM_SNOOP_LIST *	next;
};

/*
 * Room type.
 */
struct	room_index_data
{
    ROOM_INDEX_DATA *	next;
    CHAR_DATA *		people;
    OBJ_DATA *		contents;
    EXTRA_DESCR_DATA *	extra_descr;
    AREA_DATA *		area;
    EXIT_DATA *		exit	[6];
    SPECPRO_LIST *	spro;
    TRIGGER_DATA *	triggers;	/* scripts */
    TRIGGER_LIST_DATA *	trigger_list;	/* scripts */ 
    VARIABLE_DATA * 	variables;	/* scripts */
    ROOM_INDEX_DATA *	next_with_trigger;
    ROOM_SNOOP_LIST *   snoop_by;
    AFFECT_DATA * 	affected_by;
    char 		name[81];
    char *		description;
    long		vnum;
    long		room_flags;
    long		room_flags_2;
    long		guilds;
    sh_int		light;
    sh_int		light_adjustment;
    sh_int		sector_type;
    sh_int		size;
};

/*
 * Types of attacks.
 * Must be non-overlapping with spell/skill types,
 * but may be arbitrary beyond that.
 */
#define TYPE_UNDEFINED               -1
#define TYPE_HIT                     1000

/*
 *  Target types.
 */
#define TAR_IGNORE		    (A) /* no target or area spell */
#define TAR_CHAR_SELF		    (B)
#define TAR_CHAR_OTHER		    (C) /* defaults to person they are fighting */
#define TAR_OBJ_INV		    (D)
  /* Bellow used for Mob Inteligence */
#define TAR_CHAR_OFFENSIVE	    (E)
#define TAR_CHAR_DEFENSIVE	    (F)
#define TAR_CHAR_CURATIVE 	    (G)
#define TAR_CHAR_DAMAGING	    (H)
#define TAR_CHAR_CANCEL		    (I)
#define TAR_AREA		    (J)
#define START_FIGHTING		    (K)
#define SPELL_SILENT		    (L)
#define SKILL_STATIC		    (M)
#define TAR_CHAR_NONACTIVE	    (N)


#define MAX_PREREQUISITE	3

struct prerequisite_type
{
    sh_int * 	pgsn;
    int		percent;
};

#define MAX_COMPONENT		3
#define COMP_COMMON		1
#define COMP_RARE		2
#define COMP_REQUIRED		3

struct component_type
{
    long	vnum;
    long	flags;
    long	vnum_2;
};

struct spell_elements
{
    sh_int	mana;
    sh_int	level;
};

/*
 * Skills include spells as a particular case.
 */
struct	skill_type
{
    char *	name;			/* Name of skill		*/
    char * 	char_pass;
    char * 	other_pass;
    char * 	vict_pass;
    char * 	char_fail;
    char * 	other_fail;
    char * 	vict_fail;
    struct	prerequisite_type	prerequisite[ MAX_PREREQUISITE ];
    struct	component_type		component[ MAX_COMPONENT ];
    struct	spell_elements		element[ MAX_ELEMENT_TYPE ];
    SPELL_FUN *	spell_fun;		/* Spell pointer (for spells)	*/
    long	target;			/* Legal targets		*/
    sh_int	minimum_position;	/* Position for caster / user	*/
    sh_int *	pgsn;			/* Pointer to associated gsn	*/
    sh_int	beats;			/* Waiting time after use	*/
    char * 	noun_damage;		/* Damage message		*/
    char *	msg_off;		/* Wear off message		*/
    long 	group; 			/* Possible Guilds 		*/
    long 	spell_flags;		/* Special flags for the spell */
};
/* 
 * spell_flags 
 */
#define SPELL_PROTECTION		(A)
#define SPELL_SHIELD			(B)

/* guilds */
#define DEFAULT_GLD		0
#define DHOZAN_GLD		1
#define CLANNGUARD_GLD		2
#define MIZUDAN_GLD		3
#define MAR_TUUR_GLD		4
#define TOH_KO_MERE_GLD		5
#define TOR_VAL_GLD		6
#define SUUR_GLD		7
#define SCHRIES_GLD		8
#define TATH_GLD		9
#define MAJHERE_GLD		10

/* races */
#define BELEGFEA		0
#define MIRROS			1
#define ARANNA			2
#define IAVARTOR		3
#define MILROCHDIL		4
#define TARNANTHALION		5
#define	MANLHACH		6
#define NENKHILMEN		7
#define SULKANO			8

/* Guilds for skills and spells */
/* Feel free to change the guild names */
/* Just remember to change them in const.c and bit.c too */
#define ALL_GUILDS	        2147483647 
#define DEFAULT			(A)
#define DHOZAN			(B)
#define CLANNGUARD		(C)
#define MIZUDAN			(D)
#define MAR_TUUR		(E)
#define TOH_KO_MERE		(F)
#define TOR_VAL			(G)
#define SUUR			(H)
#define SCHRIES			(I)
#define TATH			(J)
#define MAJHERE			(K)
#define DARKLING		(L)
/*
 * These are skill_lookup return values for common skills and spells.
 *
*/
extern	bool	free_room[MAX_VNUM];
extern  ROOM_INDEX_DATA *       room_index_hash         [MAX_KEY_HASH];
extern  ROOM_INDEX_DATA *       room_trig_list;
extern  OBJ_INDEX_DATA *	obj_index_hash		[MAX_KEY_HASH];
extern  MOB_INDEX_DATA *	mob_index_hash		[MAX_KEY_HASH];
extern  TRIGGER_INDEX_DATA *	trig_index_hash		[MAX_KEY_HASH];
extern  ROOM_INDEX_DATA *	room_free;
extern  SPECPRO_DATA 	spec_pro_list		[MAX_SPECPRO];

extern  char	* const	group_name[ MAX_GROUP ];

extern  bool	free_obj_list[MAX_VNUM];
extern  bool	free_trig_list[MAX_VNUM];
extern  bool	free_mob_list[MAX_VNUM];
extern  long	top_room;
extern  long	top_room_index;
extern  int     top_ed;
extern  sh_int  gsn_weapon_power[MAX_WEAPON_TYPE][MAX_WEAPON_POWER];
extern  sh_int	gsn_element_power[MAX_ELEMENT_TYPE][MAX_ELEMENT_POWER];
extern  sh_int  gsn_common;
extern  sh_int  gsn_belegfea;
extern  sh_int  gsn_mirros;
extern  sh_int  gsn_aranna;
extern  sh_int  gsn_wind_shield;
extern  sh_int  gsn_iavartor;
extern  sh_int  gsn_tarnanthalion;
extern  sh_int  gsn_milrochdil;
extern  sh_int  gsn_manlhach;
extern  sh_int  gsn_nenkhilmen;
extern  sh_int  gsn_sulkano;
extern  sh_int  gsn_ventriloquate;
extern  sh_int  gsn_poison_blade;
extern  sh_int  gsn_double_strike;
extern  sh_int  gsn_strike;
extern	sh_int	gsn_shadowstrike;
extern	sh_int	gsn_retreat;
extern	sh_int	gsn_dodge;
extern	sh_int	gsn_quick_strike;
extern	sh_int	gsn_wind_over_sand;
extern	sh_int	gsn_hide;
extern	sh_int	gsn_fade;
extern	sh_int	gsn_hold_breath;
extern	sh_int	gsn_peek;
extern	sh_int	gsn_pick_lock;
extern	sh_int	gsn_counter_tracking;
extern  sh_int  gsn_cooking;
extern	sh_int	gsn_track;
extern	sh_int	gsn_first_aid;
extern	sh_int	gsn_tame_animal;
extern	sh_int	gsn_hunt;
extern	sh_int	gsn_detect_trap;
extern	sh_int	gsn_disarm_trap;
extern	sh_int	gsn_sneak;
extern	sh_int	gsn_steal;
extern	sh_int	gsn_disarm;
extern  sh_int  gsn_shield_block;
extern	sh_int	gsn_weapon_enhancement;
extern	sh_int	gsn_kick;
extern	sh_int	gsn_parry;
extern	sh_int	gsn_counter_strike;
extern	sh_int	gsn_rescue;
extern	sh_int	gsn_repair;
extern	sh_int	gsn_second_attack;
extern	sh_int	gsn_third_attack;
extern	sh_int	gsn_refresh;
extern	sh_int	gsn_remove_curse;

extern	sh_int	gsn_blinding_light;
extern	sh_int	gsn_sand_storm;
extern	sh_int	gsn_control_mind;
extern	sh_int	gsn_curse;
extern	sh_int	gsn_fire_shield;
extern	sh_int	gsn_stone_skin;
extern	sh_int	gsn_earth_shield;
extern	sh_int	gsn_water_shield;
extern	sh_int	gsn_shadowform;
extern	sh_int	gsn_mass_shadowform;
extern  sh_int  gsn_plague;
extern	sh_int	gsn_poison;
extern	sh_int	gsn_sleep;

extern sh_int  gsn_jab;
extern sh_int  gsn_endurance;
extern sh_int  gsn_perception;
extern sh_int  gsn_stun;
extern sh_int  gsn_thrust;
extern sh_int  gsn_rejuvenate;
extern sh_int  gsn_whirl;
extern sh_int  gsn_bash;
extern sh_int  gsn_intimidate;
extern sh_int  gsn_shriek;
extern sh_int  gsn_fast;
extern sh_int  gsn_breathfire;
extern sh_int  gsn_precision_strike;
extern sh_int  gsn_dual_wield;
extern sh_int  gsn_berserk;
extern sh_int  gsn_tarvals_wrath;
extern sh_int  gsn_sanctuary;
extern sh_int  gsn_blur;
extern sh_int  gsn_courage;
extern sh_int  gsn_dirt;
extern sh_int  gsn_hand_to_hand[MAX_WEAPON_POWER];
extern sh_int  gsn_tail;
extern sh_int  gsn_trip;
extern sh_int  gsn_remove_blindness;
extern sh_int  gsn_remove_poison;
extern sh_int  gsn_cure_disease;
extern sh_int  gsn_slam;
extern sh_int  gsn_ki_force;
extern sh_int  gsn_shola_punch;
extern sh_int  gsn_shola_kick;
extern sh_int  gsn_knee_punch;
extern sh_int  gsn_high_kick;
extern sh_int  gsn_combination;
extern sh_int  gsn_shola_toss;
extern sh_int  gsn_leg_sweep;
 
extern sh_int  gsn_bind_wounds;
extern sh_int  gsn_haggle;
extern sh_int  gsn_riding;
extern sh_int  gsn_riding_air;
extern sh_int  gsn_lore;
extern sh_int  gsn_deep_meditation;
extern sh_int  gsn_meditation;
extern sh_int  gsn_youna;
 
extern sh_int  gsn_scrolls;
extern sh_int  gsn_staves;
extern sh_int  gsn_wands;
extern sh_int  gsn_awareness;

extern sh_int  gsn_create_minor_portal;
extern sh_int  gsn_create_water;
extern sh_int  gsn_minor_sacrifice;
extern sh_int  gsn_sacrifice;
extern sh_int  gsn_cure_light;
extern sh_int  gsn_cure_serious;
extern sh_int  gsn_cure_critical;
extern sh_int  gsn_heal;
extern sh_int  gsn_continual_light;
extern sh_int  gsn_levitate;
extern sh_int  gsn_swim;
extern sh_int  gsn_senses;
extern sh_int  gsn_detect_shadowform;
extern sh_int  gsn_detect_hidden;
extern sh_int  gsn_circle_of_protection;
extern sh_int  gsn_faerie_fire;
extern sh_int  gsn_fly;
extern sh_int  gsn_haste;
extern sh_int  gsn_slow;
extern sh_int  gsn_calm_spirit;
extern sh_int  gsn_infravision;
extern sh_int  gsn_breathe_underwater;
extern sh_int  gsn_dimension_walk;

extern sh_int  gsn_mining;
extern sh_int  gsn_armor_smithing;
extern sh_int  gsn_weapon_smithing;
extern sh_int  gsn_golem_making;
extern sh_int  gsn_assaying;
extern sh_int  gsn_metallurgy;
extern sh_int  gsn_absorb;

extern sh_int  number_tokens; /*token counter*/
extern sh_int  number_tokens_2; /*token counter*/

/*
 * Utility macros.
 */
#define IS_VALID(data)		((data) != NULL && (data)->valid)
#define VALIDATE(data)		((data)->valid = TRUE)
#define INVALIDATE(data)	((data)->valid = FALSE)
#define UMIN(a, b)		((a) < (b) ? (a) : (b))
#define UMAX(a, b)		((a) > (b) ? (a) : (b))
#define URANGE(a, b, c)		((b) < (a) ? (a) : ((b) > (c) ? (c) : (b)))
#define LOWER(c)		((c) >= 'A' && (c) <= 'Z' ? (c)+'a'-'A' : (c))
#define UPPER(c)		((c) >= 'a' && (c) <= 'z' ? (c)+'A'-'a' : (c))
#define IS_SET(flag, bit)	((flag) & (bit))
#define SET_BIT(var, bit)	((var) |= (bit))
#define REMOVE_BIT(var, bit)	((var) &= ~(bit))
#define ADV_PROMPT(ch)		(ch)->prompt_num++
#define RST_PROMPT(ch)		(ch)->prompt_num = 0

/*
 * Character macros.
 */
#define IS_NPC(ch)		(IS_SET((ch)->act, ACT_IS_NPC) || (ch)->pcdata == NULL) 
#define IS_IMMORTAL(ch)		(IS_SET((ch)->plr, PLR_HOLYLIGHT) )
#define IS_OPTIVISION(ch)	(IS_IMMORTAL(ch) && is_optivision(ch))
#define IS_A_BUILDER(ch)	((IS_SET((ch)->cmd_groups, CMD_GROUP_ADVANCE_BUILD ) || IS_SET((ch)->cmd_groups, CMD_GROUP_BASIC_BUILD)) || IS_OPTIVISION(ch))
#define IS_IMP(ch)		(IS_IMMORTAL(ch) && IS_SET( (ch)->cmd_groups, CMD_GROUP_ADMIN ) && IS_OPTIVISION(ch))
#define IS_AFFECTED(ch, sn)	(IS_SET((ch)->affected_by, (sn)))
#define IS_AFFECTED_2(ch, sn)	(IS_SET((ch)->affected_by_2, (sn)))

#define GET_AGE(ch)		((int) (race_table[ (ch)->race ][ (ch)->sex ].start_age  + ((ch)->played \
				    + current_time - (ch)->logon ) / ( 18000 * race_table[ (ch)->race ][ (ch)->sex ].year_per_hour) ) )

#define IS_MALE(ch)		(ch->sex == 2)
#define IS_FEMALE(ch)		(ch->sex == 3)
 
#define IS_AWAKE(ch)		(ch->position > POS_SLEEPING)
#define GET_AC(ch)		( IS_NPC(ch) ? (ch)->armor : ((ch)->armor			    \
		        + ( IS_AWAKE(ch)			    \
			? dex_app[get_curr_stat(ch,STAT_DEX)].defensive : 0 )))  

#define GET_HITROLL(ch)	\
		((ch)->hitroll+str_app[get_curr_stat(ch,STAT_DEX)].tohit)
#define GET_DAMROLL(ch) \
		((ch)->damroll+str_app[get_curr_stat(ch,STAT_STR)].todam\
		* ch->size * ch->size / 4 )

#define IS_OUTSIDE(ch)		(!IS_SET((ch)->in_room->room_flags, ROOM_INDOORS) && (ch)->in_room->sector_type != SECT_UNDERGROUND )
#define IS_UNDERWATER(ch)	((ch)->in_room->sector_type == SECT_OCEAN_BOTTOM || (ch)->in_room->sector_type == SECT_UNDERWATER )
#define WAIT_STATE(ch, npulse)	((ch)->wait = UMAX((ch)->wait, (npulse)))

#define GET_NAME(ch)	((can_see(ch)) ? ch->name : "someone")
#define NAME( ch )              ( STR(ch, short_descr) ) /* scripts */
#define HAS_SCRIPT(ch)          ( IS_NPC(ch) && ch->pIndexData->trigger_list != NULL ) /* scripts */
#define HAS_SCRIPT_OBJ(obj)     ( obj->pIndexData->trigger_list != NULL ) /* scripts */
#define HAS_SCRIPT_ROOM(room)   ( room->trigger_list != NULL ) /* scripts */

/*
 * Object macros.
 */
#define CAN_WEAR(obj, part)	((obj)->wear_data != NULL && IS_SET((obj)->wear_data->wear_flags,  (part)))
#define IS_OBJ_STAT(obj, stat)	(IS_SET((obj)->extra_flags, (stat)))
#define IS_WEAPON_STAT(obj,stat)( (obj)->wear_data != NULL && (obj)->wear_data->weapon_data != NULL \
			&& IS_SET((obj)->wear_data->weapon_data->flags,(stat)))
#define IS_PROTOTYPE(room)((room)->sector_type == 32)
#define IS_PCG(obj)		((obj)->gate_data != NULL )

/*
 * Description macros.
 */
#define PERS(ch, looker)	( can_see( looker, (ch) ) ?		\
				( IS_NPC(ch) ? (ch)->short_descr	\
				: (ch)->name ) : "someone" )

/*
 * Structure for a social in the socials table.
 */
struct	social_type
{
    char     name[20];
    char     char_no_arg[81];
    char     others_no_arg[81];
    char     char_found[81];
    char     others_found[81];
    char     vict_found[81];
    char     char_not_found[81];
    char     char_auto[81];
    char     others_auto[81];
    int	     type;
    int	     damage;
    long     item;
    int	     action;
    int      mood;
};

/*
 * Global constants.
 */
extern	const	struct	str_app_type	str_app		[26];
extern	const	struct	int_app_type	int_app		[26];
extern	const	struct	wis_app_type	wis_app		[26];
extern	const	struct	dex_app_type	dex_app		[26];
extern	const	struct	con_app_type	con_app		[26];
extern	const	struct	chr_app_type	chr_app		[26];
extern  sh_int				top_race;
extern  sh_int				top_guild;
extern	const	struct	attack_type	attack_table	[];
extern	const	struct	mood_type	mood_table	[];
extern	const	struct	exp_type	exp_table	[];
extern	const	struct	exp_type_old	exp_table_old	[];
extern	const	int	train_cost_table		[ 26 ][ 6 ];
extern  const 	sh_int track_loss[SECT_MAX];
extern	char	* const	attrib_sub_table		[ 26 ][ 6 ];
extern 		char *			opti_wiz_list 	[ MAX_OPTIWIZ ];
extern  	struct  race_list_type	race_list	[MAX_RACE];
extern  	struct  race_type	race_table	[MAX_RACE][3];
extern  	struct  guild_type	guild_table	[MAX_GUILD];
extern	const	struct	liq_type	liq_table	[LIQ_MAX];
extern	const	struct	skill_type	skill_table	[MAX_SKILL];
extern          struct social_type      social_table[MAX_SOCIALS];
extern  const	int			age_table	[MAX_STATS]
							[MAX_AGE/5]
							[MAX_RACE];
extern  const   struct	time_table_type	time_table      [MAX_WORLD];
extern  char *  const			world_name	[MAX_WORLD];
extern  char *  const			world_name_olc	[MAX_WORLD];
extern  char *  const			coin_name	[MAX_WORLD];
extern  char *  const			coin_names	[MAX_WORLD];
extern  const  		sh_int		bank_exchange_rate [MAX_WORLD]
							[MAX_WORLD];
extern  const		sh_int		attrib_adjust_table [ 9 ] [ MAX_STATS ];
extern  const  		sh_int		exchange_rate	[MAX_WORLD]
							[MAX_WORLD];
extern  const   struct  color_data      color_table     []; /* colors */

extern          int                     FORCE_LEVEL; /* scripts */

/*
 * Global variables.
 */
extern		HELP_DATA	  *	help_first;
extern 		TOPIC_DATA 	  *	topics;
extern		SHOP_DATA	  *	shop_first;

extern		BAN_DATA	  *	ban_list;
extern		CHAR_DATA	  *	char_list;
extern		CHAR_DATA 	  *	mob_list_update[16];
extern 		CHAR_DATA	  *	mob_list_current_update;
extern		TRIGGER_DATA	  *	trigger_list;
extern		CLAN_DATA	  *	clan_list;
extern		TOP_TEN_DATA	  	top_ten_list[ 10 ][ 3 ];
extern		HORSE_RACE_TYPE 	horse_race_list[ MAX_HORSE_RACE ];
extern		sh_int			language_table[ MAX_LANGUAGE ];
extern		DESCRIPTOR_DATA   *	descriptor_list;
extern		NOTE_DATA	  *	note_list;
extern		LOGON_DATA	  *	logon_data_list;
extern		GLOBAL_RANDOM_DATA  *	random_data_list;
extern		QUEST_DATA	  *	quest_data_list;
extern		WIZLIST_DATA	  *	wizlist_data_list;
extern		FINGER_DATA	  *	finger_list;
extern		OBJ_DATA	  *	object_list;

extern		AFFECT_DATA	  *	affect_free;
extern		BAN_DATA	  *	ban_free;
extern		SKILL_LIST	  *	skill_list_free;
extern		CASTLE_DATA	  *	castle_data_free; /* castle code */
extern		SPECPRO_LIST	  *	specpro_list_free;
extern		CHAR_DATA	  *	char_free;
extern		DESCRIPTOR_DATA	  *	descriptor_free;
extern		EXTRA_DESCR_DATA  *	extra_descr_free;
extern		NOTE_DATA	  *	note_free;
extern		OBJ_DATA	  *	obj_free;
extern		LIGHT_DATA	  *	light_data_free;
extern		WEAR_DATA	  *	wear_data_free;
extern		EQUIP_DATA	  *	equip_data_free;
extern		INSIDE_DATA	  *	inside_data_free;
extern		MAGIC_DATA	  *	magic_data_free;
extern		WEAPON_DATA	  *	weapon_data_free;
extern		EDIBLE_DATA	  *	edible_data_free;
extern		CONT_DATA	  *	cont_data_free;
extern		GATE_DATA	  *	gate_data_free;
extern		TRAP_DATA	  *	trap_data_free;
extern		MOVEABLE_DATA	  *	moveable_data_free;
extern		TRACK_DATA	  *	track_data_free;
extern		LOCK_DATA	  *	lock_data_free;
extern		INSIDE_AREA_DATA  *	inside_area_data_free;
extern		WIZLIST_DATA  	  *	wizlist_data_free;
extern		FINGER_DATA  	  *	finger_data_free;
extern		SPELL_LIST  	  *	spell_list_free;
extern		RESET_DATA  	  *	reset_data_free;
extern		EVENT_DATA  	  *	event_data_free;
extern		QUEST_DATA  	  *	quest_data_free;
extern		CHAR_QUESTS  	  *	char_quests_free;
extern		PC_DATA		  *	pcdata_free;
extern		GROUP_DATA	  *	group_free;
extern		GROUP_DATA	  *	group_list;

extern		char			bug_buf		[];
extern		time_t			current_time;
extern		bool			fLogAll;
extern		KILL_DATA		kill_table	[];
extern		char			log_buf		[];
extern		char			imp_buf		[];	
extern		TIME_INFO_DATA		time_info[ MAX_WORLD ];
extern		TIME_INFO_DATA		time_new;
extern		WEATHER_DATA		weather_info[ MAX_WORLD ];
extern          SCRIPT_DATA       *     script_free; /* scripts */
extern          TRIGGER_DATA      *     trigger_free; /* scripts */
extern          SCRIPT_DATA       *     script_free; /* scripts */
extern          VARIABLE_DATA     *     variable_free; /* scripts */
extern		EVENT_DATA	  * 	event_list;


/*
 * OS-dependent declarations.
 * These are all very standard library functions,
 *   but some systems have incomplete or non-ansi header files.
 */
#if	defined(apollo)
int	atoi		args( ( const char *string ) );
void *	calloc		args( ( unsigned nelem, size_t size ) );
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(hpux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(linux)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(macintosh)
#define NOCRYPT 
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(MIPS_OS)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(MSDOS)
#define NOCRYPT
#if	defined(unix)
#undef	unix
#endif
#endif

#if	defined(NeXT)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif

#if	defined(sequent)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(sun)
char *	crypt		args( ( const char *key, const char *salt ) );
int	fclose		args( ( FILE *stream ) );
int	fprintf		args( ( FILE *stream, const char *format, ... ) );
#if	defined(SYSV)
siz_t	fread		args( ( void *ptr, size_t size, size_t n, 
			    FILE *stream) );
#else
int	fread		args( ( void *ptr, int size, int n, FILE *stream ) );
#endif
int	fseek		args( ( FILE *stream, long offset, int ptrname ) );
void	perror		args( ( const char *s ) );
int	ungetc		args( ( int c, FILE *stream ) );
#endif

#if	defined(ultrix)
char *	crypt		args( ( const char *key, const char *salt ) );
#endif



/*
 * The crypt(3) function is not available on some operating systems.
 * In particular, the U.S. Government prohibits its export from the
 *   United States to foreign countries.
 * Turn on NOCRYPT to keep passwords in plain text.
 */
#if	defined(NOCRYPT)
#define crypt(s1, s2)	(s1)
#endif



/*
 * Data files used by the server.
 *
 * AREA_LIST contains a list of areas to boot.
 * All files are read in completely at bootup.
 * Most output files (bug, idea, typo, shutdown) are append-only.
 *
 * The NULL_FILE is held open so that we have a stream handle in reserve,
 *   so players can go ahead and telnet to all the other descriptors.
 * Then we close it whenever we need to open a file (e.g. a save file).
 */
#if defined(macintosh)
#define PLAYER_DIR	""		/* Player files			*/
#define PLAYER_TEMP	"temp"
#define NULL_FILE	"proto.are"		/* To reserve one stream	*/
#endif

#if defined(MSDOS)
#define NULL_FILE	"nul"		/* To reserve one stream	*/
#endif

#if defined(unix)
#define SAVEAREAS_DIR	"../savearea/"	/*Saved are files (online editing */
#define SAVEWORLDS_DIR  "../worlds/"
#define SAVEHOMES_DIR	"../homes/"	/*Saved are files player homes */
#define PLAYER_DIR	"../player/"	/* Player files			*/
#define PLAYER_DIR_OLD	"../playerold/"	/* Player files			*/
#define SCRIPT_DIR	"../scripts/"	/* Script files			*/
#define PLAYER_TEMP	"../player/temp"
#define NULL_FILE	"/dev/null"	/* To reserve one stream	*/
#endif

#define AREA_LIST	"area.lst"	/* List of areas		*/

#define BUG_FILE	"../misc/bugs.txt"      /* For 'bug' and bug( )		*/
#define IDEA_FILE	"../misc/ideas.txt"	/* For 'idea' */
#define TYPO_FILE	"../misc/typos.txt"     /* For 'typo'			*/
#define NOTE_FILE	"../notes/notes.txt"	/* For 'notes'			*/
#define RACE_FILE	"../races/race.lst"		/* For the race list		*/
#define GUILD_FILE	"../guilds/guild.lst"		/* For the guild list		*/
#define CLAN_FILE	"../clan/clan.lst"	/* For all clan info		*/
#define LOGON_DATA_FILE	"../misc/logon.lst"	/* For all logon info		*/
#define RANDOM_DATA_FILE "../misc/random.lst"	/* For all random info		*/
#define QUEST_DATA_FILE	"../misc/quests"	/* For all quests info		*/
#define WIZLIST_DATA_FILE "../misc/wizlist"	/* For all wizlist info */
#define FINGER_FILE 	"../misc/finger"	/* For all finger info */
#define HELPSAVE_FILE	"../helps/help.are"	/* For all helps	*/
#define SOCIALSAVE_FILE	"../social/social.are"	/* For all socials	*/
#define TRIGGER_DATA_FILE "../scripts/trigger.file" /* For all triggers	*/
#define EVENT_DATA_FILE "../events/event.file" /* For all events	*/
#define LOGON_COUNTER	"../misc/logon_counter"	/* For number of logons */
#define IMP_LOG		"../log/implog.txt"	/* imm commands used */
#define TOP_TEN_FILE	"../misc/top_ten.lst"	/* For Quin's Top Ten List	*/
#define SHUTDOWN_FILE	"shutdown.txt"	/* For 'shutdown'		*/

/*
 * Area flags. OLC
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */
#define         AREA_AUTOSAVE   8	/* Used for autosaving */
#define         AREA_NO_TELEPORT   16	/* Used for new area's */
#define         AREA_VIRTUAL    32	/* Used for new area's */
#define		AREA_PKSAFE	64      /* Areas which are safe from pking */

/*
 * online creation externals  OLC
 */
extern          AREA_DATA *             area_first;
extern          AREA_DATA *             appartment_area;
extern          AREA_DATA *             area_last;
extern          HELP_DATA *             help_last;
extern  	SHOP_DATA *             shop_last;

extern		sh_int			reboot_timer;
extern		bool			reboot_pause;
extern          long                     top_affect_obj;
extern          long                     top_affect_mob;
extern          int                     LOGON_NUMBER;
extern          int                     top_area;
extern          int                     top_ed;
extern          long                     top_exit;
extern          int                     top_help;
extern		int			top_bfs_queue;
extern		int			top_bfs_room;
extern		int			top_world;
extern          long                     top_mob_index;
extern          long                     top_obj_index;
extern          int                     newobjs;
extern          int                     newmobs;
extern          int                     top_reset;
extern          long                     top_room;
extern          int                     top_shop;

extern          long                     top_vnum_mob;
extern          long                     top_vnum_obj;
extern          long                     top_vnum_room;

extern          char                    str_empty       [1];

extern  MOB_INDEX_DATA *        mob_index_hash  [MAX_KEY_HASH];
extern  TRIGGER_INDEX_DATA *    trig_index_hash  [MAX_KEY_HASH];
extern  OBJ_INDEX_DATA *        obj_index_hash  [MAX_KEY_HASH];
extern  ROOM_INDEX_DATA *       room_index_hash [MAX_KEY_HASH];



/*
 * Online editing structure.
 */
struct  editing_data	/* OLC */
{
    EDITING_DATA *	next;
    int                 vnum;	
};

/*
 * Area flags. OLC
 */
#define         AREA_NONE       0
#define         AREA_CHANGED    1	/* Area has been modified. */
#define         AREA_ADDED      2	/* Area has been added to. */
#define         AREA_LOADING    4	/* Used for counting in db.c */
#define MAX_DIR			      6	/* OLC */
extern	char *	const	dir_name        [];	/* OLC */
#define TOGGLE_BIT(var, bit)    ((var) ^= (bit))	/* OLC */
#define STR(dat, field)         (( (dat)->field != NULL                    \
                                             ? (dat)->field                \
                                             : (dat)->pIndexData->field ))	/* OLC */

#define PC(ch,field)            ((ch)->pcdata->field)	/* OLC */
#define GET_PC(ch,field,nopc)   ((ch->pcdata != NULL ? ch->pcdata->field    \
                                                     : (nopc)) )	/* OLC */

#define IS_BUILDER(ch, Area)   ((GET_PC(ch,security,8) <= Area->security && GET_PC(ch,security,8) != 0)             \
                             || strstr( Area->builders, ch->name ) != NULL  \
                             || strstr( Area->builders, "All" ) != NULL     \
                             || (IS_NPC(ch) && ch->pIndexData->area == Area) )
#define IS_BUILDER_TRIG(ch, pTrigIndex)   ((GET_PC(ch,security,8) <= pTrigIndex->security && GET_PC(ch,security,8) != 0)             \
                             || strstr( pTrigIndex->builders, ch->name ) != NULL  \
                             || strstr( pTrigIndex->builders, "All" ) != NULL ) 

#include "funct.h"
