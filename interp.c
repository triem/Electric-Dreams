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

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "interp.h"


extern		int	social_count;

/*
 * Command logging types. Moved to interp.h G
#define LOG_NORMAL	0
#define LOG_ALWAYS	1
#define LOG_NEVER	2
#define NO_HAPPY_HOUR   4
#define HELP_COMM	5
#define HELP_COMBAT	6
#define HELP_INFO	7
*/
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

/*
 * Log-all switch.
 */
bool				fLogAll		= FALSE;

/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[] =
{
    /*
     * Common movement commands.
     */
    { "north",		do_north,	POS_STANDING,    PL,  LOG_NEVER|STARTING|HELP_COMM, 0, 0, 0 },
    { "east",		do_east,	POS_STANDING,	 PL,  LOG_NEVER|STARTING|HELP_MOVEMENT, 0, 0, 0 },
    { "south",		do_south,	POS_STANDING,	 PL,  LOG_NEVER|STARTING|HELP_MOVEMENT, 0, 0, 0 },
    { "west",		do_west,	POS_STANDING,	 PL,  LOG_NEVER|STARTING|HELP_MOVEMENT, 0, 0, 0 },
    { "up",		do_up,		POS_STANDING,	 PL,  LOG_NEVER|STARTING|HELP_MOVEMENT, 0, 0, 0 },
    { "down",		do_down,	POS_STANDING,	 PL,  LOG_NEVER|STARTING|HELP_MOVEMENT, 0, 0, 0 },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "at",             do_at,          POS_DEAD,	 AP,  LOG_NORMAL, 1, 0, 0 },
    { "auction",        do_auction,     POS_RESTING,     PL,  LOG_NORMAL, 1, 0, 0 },
    { "affects",	do_affects,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "buy",		do_buy,		POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "cast",		do_cast,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_SKILLS, 1, 0, 0 },
    { "channels",       do_channels,    POS_DEAD,        PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "exits",		do_exits,	POS_RESTING,	 PL,  LOG_NORMAL|STARTING|HELP_MOVEMENT, 1, 0, 0 },
    { "enter",		do_enter,	POS_STANDING,    PL,  LOG_NORMAL|STARTING|HELP_MOVEMENT, 1, 0, 0 },
    { "empty",		do_empty,	POS_STANDING,    PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "board",		do_board,	POS_STANDING,    GM|PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "get",		do_get,		POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "goto",           do_goto,        POS_DEAD,       GD,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "hit",		do_kill,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 0, 0, 0 },
    { "hit",		do_hkill,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 0, 0, 0 },
    { "inventory",	do_inventory,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "kill",		do_kill,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "look",		do_look,	POS_RESTING,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "level",		do_level,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "lvl",		do_level,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "order",		do_order,	POS_RESTING,	 PL,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "pray",           do_pray,   	POS_SLEEPING,    PL,  LOG_NORMAL|STARTING|HELP_COMM, 1, 0, 0 }, 
    { "rest",		do_rest,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "sit",		do_sit,		POS_SLEEPING,    PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "search",         do_search,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "dig",         	do_dig,		POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "fset",       	do_fset,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "mine",         	do_mine,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "gforge",         do_forge_golem,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "minerals",       list_minerals,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "wforge",         do_forge_weapon,POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "aforge",         do_forge_armor,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "cover",         	do_cover,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "extract",     	do_extract,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "absorb",     	do_absorb,	POS_STANDING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "sockets",        do_sockets,	POS_DEAD,        GI, LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "stand",		do_stand,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "tell",		do_tell,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "mate",		do_mate,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "[",		do_mate,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "wield",		do_wear,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "wizhelp",	do_wizhelp,	POS_DEAD,	 GD, LOG_NORMAL|HELP_INFO, 1, 0, 0 },

    /*
     * Informational commands.
     */
    { "areas",		do_areas,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "bug",		do_bug,		POS_DEAD,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "changes",	do_changes,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "color",		do_set_color,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "ansi",		do_set_color,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "commands",	do_commands,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "compare",	do_compare,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "consider",	do_consider,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "cook",		do_cook,	POS_STANDING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "poison",		do_poison_blade,	POS_STANDING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "count",		do_count,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "credits",	do_credits,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "equipment",	do_equipment,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "examine",	do_examine,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "help",		do_help,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING, 1, 0, 0 },
    { "idea",		do_idea,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "infochan",	do_infochan,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "motd",		do_motd,	POS_DEAD,        PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "perception",	do_perception,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_SKILLS, 1, 0, 0 }, 
    { "lore",		do_lore,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_SKILLS, 1, 0, 0 }, 
    { "news",		do_news,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "read",		do_read,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "report",		do_report,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "rules",		do_rules,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "score",		do_score,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "scan",		do_scan,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_INFO, 1, 0, 0 },
    { "map",		do_map,		POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_CONFIG, 1, 0, 0 },
    { "run",		do_run, 	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_MOVEMENT, 1, 0, 0 },
    { "socials",	do_socials,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "skills",		do_skills,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_SKILLS, 1, 0, 0 },
    { "slist",		do_slist,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_SKILLS, 1, 0, 0 },
    { "spells",		do_spells,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_SKILLS, 1, 0, 0 },
    { "story",		do_story,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "time",		do_time,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "typo",		do_typo,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "quests",		do_quest,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "vitals",     do_vitals,  POS_DEAD,    PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "weather",	do_weather,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "who",		do_who,		POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "whogroup",	do_whogroup,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 }, 
    { "wizlist",	do_wizlist,	POS_DEAD,        PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "topten",		do_topten,	POS_DEAD,        PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "worth",		do_worth,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },

    /*
     * Configuration commands.
     */
   /* { "alias",		do_alias,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 }, */
    { "config",		do_autolist,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autolist",	do_autolist,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autoassist",	do_autoassist,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autodirs",	do_autodirs,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autoexit",	do_autoexit,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "automap",	do_automap,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autogold",	do_autogold,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autoloot",	do_autoloot,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autosac",	do_autosac,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "autosplit",	do_autosplit,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "brief",		do_brief,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
/*  { "channels",	do_channels,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 }, */
    { "combine",	do_combine,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "compact",	do_compact,	POS_DEAD,        PL,  LOG_NORMAL|STARTING|HELP_CONFIG, 1, 0, 0 },
    { "description",	do_description,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "delet",		do_delet,	POS_DEAD,	 PL,  LOG_ALWAYS, 0, 0, 0 },
    { "delete",		do_delete,	POS_DEAD,	 PL,  LOG_ALWAYS|STARTING|HELP_OTHER, 1, 0, 0 },
    { "finger",		do_finger,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "nofollow",	do_nofollow,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "noloot",		do_noloot,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "nomiss",		do_nomiss,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "nosummon",	do_nosummon,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "outfit",		do_outfit,	POS_RESTING,	 PL,  LOG_ALWAYS|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "password",	do_password,	POS_DEAD,	 PL,  LOG_NEVER|HELP_CONFIG,  1, 0, 0 },
    { "prompt",		do_prompt,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "prefi",		do_prefi,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "prefix",		do_prefix,	POS_DEAD,        PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "scroll",		do_scroll,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_SKILLS, 1, 0, 0 },
    { "title",		do_title,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
   /* { "unalias",	do_unalias,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 }, */
    { "wimpy",		do_wimpy,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },

    /*
     * Communication commands.
     */
    { "away",		do_awaymode,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0},
    { "deaf",		do_deaf,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "emote",		do_emote,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "ooc",		do_ooc,		POS_SLEEPING,	 PL,  LOG_NORMAL, 0, 0, 0 },
    { ".",		do_think,	POS_SLEEPING,	 PL,  LOG_NORMAL, 0, 0, 0 },
    { "think",		do_think,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "gossip",		do_think,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { ",",		do_emote,	POS_RESTING,	 PL,  LOG_NORMAL, 0, 0, 0 },
    { "gtell",		do_gtell,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_GROUP, 1, 0, 0 },
    { ";",		do_gtell,	POS_RESTING,	 PL,  LOG_NORMAL, 0, 0, 0 },
    { "note",		do_note,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "quiet",		do_quiet,	POS_SLEEPING, 	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "reply",		do_reply,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "say",		do_say,		POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "'",		do_say,		POS_RESTING,	 PL,  LOG_NORMAL, 0, 0, 0 },
    { "clan",		do_clan,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "yell",		do_yell,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "infoset",	do_setinfo,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },

    /*
     * Object manipulation commands.
     */
    { "brandish",	do_brandish,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "close",		do_close,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "drink",		do_drink,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "drop",		do_drop,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "eat",		do_eat,		POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0},
    { "extinguish",     do_extinguish,  POS_RESTING,     PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "fill",		do_fill,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
/*    { "refill",		do_refill,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 }, */
    { "give",		do_give,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "heal",		do_heal,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OTHER, 1, 0, 0 }, 
    { "hold",		do_wear,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "list",		do_list,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "light",          do_light,       POS_RESTING,     PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0},  
    { "lock",		do_lock,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "open",		do_open,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "pick",		do_pick,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "put",		do_put,		POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "quaff",		do_drink,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "recite",		do_recite,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "refuel",         do_refuel,      POS_RESTING,     PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0},
    { "remove",		do_remove,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "throw",		do_throw,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "name",		do_rename,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, OBJ_VNUM_RENAME, 0 },
    { "sell",		do_sell,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR, 1, 0, 0 },
    { "take",		do_get,		POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "sacrifice",	do_sacrifice,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "junk",           do_sacrifice,   POS_RESTING,     PL,  LOG_NORMAL|HELP_OBJECTS, 0, 0, 0 },
    { "tap",      	do_sacrifice,   POS_RESTING,     PL,  LOG_NORMAL|HELP_OBJECTS, 0, 0, 0 },   
    { "unlock",		do_unlock,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "value",		do_value,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "wear",		do_wear,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "zap",		do_zap,		POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "collect",	do_collect,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    /*
     * Combat commands.
     */
    { "shadowstrike",	do_shadowstrike,POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "strike",		do_strike,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "double strike",	do_double_strike,POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "ds",		do_double_strike,POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "bash",		do_bash,	POS_FIGHTING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "precision-strike",do_precision_strike,POS_FIGHTING, PL, LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "ps",		do_precision_strike,	POS_FIGHTING,    PL, LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "quick-strike",	do_quick_strike,POS_FIGHTING,    PL, LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "qs",		do_quick_strike,POS_FIGHTING,    PL, LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "breathfire",	do_breathfire,	POS_FIGHTING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "st",		do_shadowstrike,POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 0, 0, 0 },
    { "berserk",	do_berserk,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "tar-vals wrath",	do_tarvals_wrath,POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "tw",		do_tarvals_wrath,POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "blur",		do_blur,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "dirt",		do_dirt,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "flee",		do_flee,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "disarm",		do_disarm,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "dt",		do_disarm_trap,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "murde",		do_murde,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR, 0, 0, 0 },
    { "thrust",		do_thrust,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "jab",		do_jab,		POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "whirl",		do_whirl,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "kick",		do_kick,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "murder",		do_murder,	POS_FIGHTING,	 PL,  LOG_ALWAYS|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "intimidate",	do_intimidate,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "shriek",		do_shriek,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "retreat",	do_retreat,	POS_FIGHTING,	 PL, LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "rescue",		do_rescue,	POS_FIGHTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "trip",		do_trip,	POS_FIGHTING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 },
    { "slam",		do_slam,	POS_FIGHTING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 }, 
    { "ki-force",	do_ki_force,	POS_FIGHTING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 }, 
    { "kf",		do_ki_force,	POS_FIGHTING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 }, 
    { "stun",		do_stun,	POS_FIGHTING,    PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_COMBAT, 1, 0, 0 }, 

    /*
     * Miscellaneous commands.
     */
    { "cshow",		do_cshow,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "clist",		do_clist,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "recruit",        do_recruit,     POS_DEAD,       GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "blackball",	do_blackball,	POS_DEAD,	AD,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "clanrank", 	do_clanrank, 	POS_DEAD, 	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "rlist",		do_rlist,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "glist",		do_glist,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "cook",		do_cook,	POS_STANDING,	 PL,  LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "balance",	do_balance,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 370}, 
    { "deposit",	do_deposit,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 370},
    { "exchange",	do_exchange,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 370},
    { "follow",		do_follow,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_GROUP, 1, 0, 0 },
    { "found",		do_found,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_GROUP, 1, 0, 0 },
    { "appoint",	do_appoint,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_GROUP, 1, 0, 0 },
    { "disband",	do_disband,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_GROUP, 1, 0, 0 },
    { "defect",		do_defect,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_GROUP, 1, 0, 0 },
    { "lines",		do_scroll,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "gain",		do_gain,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_SKILLS, 1, 0, 0 },
    { "gohome",		do_gohome,	POS_STANDING, 	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 }, 
    { "group",		do_group,	POS_SLEEPING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_GROUP, 1, 0, 0 },
    { "hide",		do_hide,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_MOVEMENT, 1, 0, 0 },
    { "fade",		do_fade,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_MOVEMENT, 1, 0, 0 },
    { "qui",		do_qui,		POS_DEAD,	 PL,  LOG_NORMAL, 0, 0, 0 },
    { "quit",		do_quit,	POS_DEAD,	 PL,  LOG_NORMAL|STARTING|HELP_OTHER, 1, 0, 0 },
    { "rent",		do_rent,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_OTHER, 1, OBJ_VNUM_TENT, 0 },
    { "recall",		do_recall,	POS_FIGHTING,	 PL,  LOG_NORMAL|STARTING|HELP_MOVEMENT, 1, 0, 0 },
    { "/",		do_recall,	POS_FIGHTING,	 PL,  LOG_NORMAL, 0, 0, 0 },
    { "save",		do_save,	POS_DEAD,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "saveout",	do_saveout,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0},
    { "sleep",		do_sleep,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "tame",		do_tame_animal,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OTHER, 1, 0, 0 },
    { "first",		do_first_aid,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OTHER, 1, OBJ_VNUM_MEDICAL_HERB, 0 },
    { "aid",		do_first_aid,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OTHER, 1, OBJ_VNUM_MEDICAL_HERB, 0 },
    { "mount",		do_mount,	POS_STANDING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "dismount",	do_dismount,	POS_STANDING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "sneak",		do_sneak,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_MOVEMENT, 1, 0, 0 },
    { "track",		do_track,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_MOVEMENT, 1, 0, 0 },
    { "hunt",		do_hunt,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_MOVEMENT, 1, 0, 0 },
    { "repair",		do_repair,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OTHER, 1, 0, 0 },
    { "split",		do_split,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OTHER, 1, 0, 0 },
    { "steal",		do_steal,	POS_STANDING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_OBJECTS, 1, 0, 0 },
    { "train",		do_train,	POS_RESTING,	 PL,  LOG_NORMAL|NO_HAPPY_HOUR|HELP_SKILLS, 1, 0, 0 },
    { "transfer",	do_cointrans,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 370},
    { "visible",	do_visible,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "wake",		do_wake,	POS_SLEEPING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "where",		do_where,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "history",	do_history,	POS_RESTING,	 PL,  LOG_NORMAL|STARTING|HELP_INFO, 1, 0, 0 },
    { "withdraw",	do_withdraw,	POS_RESTING,	 PL,  LOG_NORMAL|HELP_OTHER, 1, 0, 370},   


    /*
     * Hero Commands
     */

    /*
     * Immortal commands.
     */
    { "advance",	do_advance,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "dump",		do_dump,	POS_DEAD,	GC,  LOG_ALWAYS|HELP_OBJECTS, 0, 0, 0 },
    { "rot",		do_rot_string,	POS_DEAD,	AD,  LOG_ALWAYS, 1, 0, 0 },

    { "allow",		do_allow,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "bonus",		do_bonus,	POS_DEAD,	AD,  LOG_ALWAYS, 1, 0, 0 },
    { "backup",		do_backup,	POS_DEAD,	GC,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "ban",		do_ban,		POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "bankset",	do_bankset,	POS_DEAD,	AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0}, 
    { "beeper",		do_beeper,	POS_DEAD,	GD,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 }, 
    { "color_check",	do_color_check,	POS_DEAD,        PL,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "cpr",		do_cpr,		POS_DEAD,	BP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "deny",		do_deny,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "disconnect",	do_disconnect,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "freeze",		do_freeze,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "object",		do_object,	POS_DEAD,       AB,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "room",		do_room,	POS_DEAD,       AB,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "roomstats",	do_roomstats,	POS_DEAD,       GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "sectorstats",	do_sectortype,	POS_DEAD,       GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "simulate",	do_simulate,	POS_DEAD,       GD,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "reboo",		do_reboo,	POS_DEAD,	GC,  LOG_NORMAL, 0, 0, 0 },
    { "reboot",		do_reboot,	POS_DEAD,	GC,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "script",		do_script,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "set",		do_set,		POS_DEAD,	AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "setgroup",	do_setgroup,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "setguild",	do_setguild,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "settitle",	do_settitle,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "shutdow",	do_shutdow,	POS_DEAD,	GC,  LOG_NORMAL, 0, 0, 0 },
    { "skillz",		do_skillz,	POS_DEAD,	AD,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "shutdown",	do_shutdown,	POS_DEAD,	GC,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
/*  { "sockets",	do_sockets,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 }, */
    { "wizlock",	do_wizlock,	POS_DEAD,	GC,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "wed",		do_wed,		POS_DEAD,	AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },

    { "force",		do_force,	POS_DEAD,	AB|AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "load",		do_load,	POS_DEAD,	BB|BP,  LOG_ALWAYS|HELP_OBJECTS, 1, 0, 0 },
    { "load_room_triggers",do_load_room_triggers,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "newlock",	do_newlock,	POS_DEAD,	GC,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "nochannels",	do_nochannels,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "noemote",	do_noemote,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "noshout",	do_noshout,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "notell",		do_notell,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "pecho",		do_pecho,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 }, 
    { "pardon",		do_pardon,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "purge",		do_purge,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "purgearea",	do_purge_area,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "restore",	do_restore,	POS_DEAD,	AB|AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "sla",		do_sla,		POS_DEAD,	AB,  LOG_NORMAL, 0, 0, 0 },
    { "slay",		do_slay,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_COMBAT, 1, 0, 0 },
    { "teleport",	do_transfer,    POS_DEAD,	BP,  LOG_ALWAYS|HELP_MOVEMENT, 1, 0, 0 },	
    { "transfer",	do_transfer,	POS_DEAD,	AP|AB,  LOG_ALWAYS|HELP_MOVEMENT, 1, 0, 0 },
    { "jail",		do_jail,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "saveclans",	do_saveclans,	POS_DEAD,	AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "saveraces",	do_saveraces,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "saveguild",	do_saveguilds,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "savetriggers",	save_triggers,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "saveresets",	do_save_resets,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "areastat",	do_areastat,	POS_DEAD,	BB,  LOG_ALWAYS|HELP_INFO, 1, 0, 0 },
    { "fixit",		do_fixexit,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_INFO, 1, 0, 0 },
    { "poofin",		do_bamfin,	POS_DEAD,	GD,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "poofout",	do_bamfout,	POS_DEAD,	GD,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "gecho",		do_echo,	POS_DEAD,	AP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "wecho",		do_wecho,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "aecho",		do_aecho,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "holylight",	do_holylight,	POS_DEAD,	GD,  LOG_NORMAL|STARTING|HELP_OTHER, 1, 0, 0 },
    { "invis",		do_invis,	POS_DEAD,	GD,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "laston",		do_logondata,	POS_DEAD,	GI,  LOG_ALWAYS|HELP_INFO, 1, 0, 0 },    
    { "log",		do_log,		POS_DEAD,	AD,  LOG_ALWAYS|HELP_INFO, 1, 0, 0 },
    { "logon",		do_logondata,	POS_DEAD,	GI,  LOG_ALWAYS|HELP_INFO, 1, 0, 0 },
    { "memory",		do_memory,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "mwhere",		do_mwhere,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "peace",		do_peace,	POS_DEAD,	BB|BP,  LOG_NORMAL|HELP_COMBAT, 1, 0, 0 },
    { "echo",		do_recho,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "return",         do_return,      POS_DEAD,       GM|AB|BP,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "sellhome",	do_sellhome,	POS_RESTING,	AB|AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 }, 
    { "snoop",		do_snoop,	POS_DEAD,	AP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "show_skills",	do_show_skills,	POS_DEAD,	AB,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "stat",		do_stat,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "string",		do_string,	POS_DEAD,	BP,  LOG_ALWAYS|HELP_OBJECTS, 1, 0, 0 },
    { "switch",		do_switch,	POS_DEAD,	AB|BP,  LOG_ALWAYS|HELP_COMM, 1, 0, 0 },
    { "wizinvis",	do_invis,	POS_DEAD,	GD,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "wizpass",	do_wizpass,	POS_DEAD,	GD,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "vnum",		do_vnum,	POS_DEAD,	GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },

    { "makeclan",	do_makeclan,	POS_DEAD,	AP,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "clone",		do_clone,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OBJECTS, 1, 0, 0 },
    { "security",	do_security,	POS_DEAD,	AD,  LOG_ALWAYS|HELP_OTHER, 1, 0, 0 },
    { "immtalk",	do_immtalk,	POS_DEAD,	GD,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "helpme",		do_helpme,	POS_DEAD,	PL,  LOG_NORMAL|HELP_COMM, 1, 0, 0 },
    { "imotd",          do_imotd,       POS_DEAD,       GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { ":",		do_immtalk,	POS_DEAD,	GD,  LOG_NORMAL, 0, 0, 0 },
    { "donate",		do_donate,	POS_RESTING,	PL,   LOG_NORMAL|HELP_OBJECTS, 1, 0, 0 },
    { "]",		do_impchat,	POS_DEAD,	AB|AP,  LOG_NORMAL, 0, 0, 0 }, 
    { "leave",		do_exit,	POS_STANDING,	 PL,  LOG_NORMAL|HELP_MOVEMENT, 1, 0, 0 },
    { "nuke",		do_nuke,	POS_STANDING,	AD, LOG_ALWAYS|HELP_OTHER,  1, 0, 0 },
    { "listroom",	do_specroom,	POS_DEAD,	GI, LOG_NORMAL|HELP_INFO,  1, 0, 0 },
    { "find",		do_find,	POS_DEAD,	GI, LOG_NORMAL|HELP_INFO,  1, 0, 0 },
    /*
     * Online creation commands.
     */
    { "aedit",		do_aedit,	POS_DEAD,    AB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "hedit",		do_hedit,	POS_DEAD,    BP,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "sedit",		do_sedit,	POS_DEAD,    BP,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "raedit",		do_raedit,	POS_DEAD,    AB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "redit",		do_redit,	POS_DEAD,    BB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "oedit",          do_oedit,	POS_DEAD,    BB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "medit",          do_medit,	POS_DEAD,    BB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "maedit",         do_maedit,	POS_DEAD,    AB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "tedit",          do_tedit,	POS_DEAD,    AB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "gedit",          do_gedit,	POS_DEAD,    AB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "sshow",          do_sshow,	POS_DEAD,    GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "tshow",          do_tshow,	POS_DEAD,    GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "gshow",          do_gshow,	POS_DEAD,    AB,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "ashow",          do_ashow,	POS_DEAD,    GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "rshow",          do_rshow,	POS_DEAD,    GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "oshow",          do_oshow,	POS_DEAD,    GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "resets",         do_resets,      POS_DEAD,    BB,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "area_reset",     do_area_reset,  POS_DEAD,    BB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "reset_events",   reset_events,   POS_DEAD,    BB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "asave",          do_asave,	POS_DEAD,    BB,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "autosave",       do_autosave,	POS_DEAD,    AB,  LOG_NORMAL|HELP_CONFIG, 1, 0, 0 },
    { "areasave",	do_asave,	POS_DEAD,    BB,  LOG_ALWAYS|HELP_OTHER, 0, 0, 0 },
    { "alist",		do_arealist,	POS_DEAD,    GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "makegrid",	do_makegrid,	POS_DEAD,    AB,  LOG_NORMAL, 1, 0, 0 },
    { "tset",		do_tset_char,	POS_DEAD,    GM|AB,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "tlist",		do_trigger_list,	POS_DEAD,    GI,  LOG_NORMAL|HELP_INFO, 1, 0, 0 },
    { "savehelps",	save_help,	POS_DEAD,    BP,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "savesocials",	save_social,	POS_DEAD,    BP,  LOG_NORMAL|HELP_OTHER, 1, 0, 0 },
    { "virtual",	do_virtual,	POS_DEAD,    GC,  LOG_NORMAL, 0, 0, 0 },
    { "xyzzy",		do_xyzzy,	POS_DEAD,    GC,  LOG_NORMAL, 0, 0, 0 },
    { "system",		do_system,	POS_DEAD,    AB,  LOG_NORMAL, 1, 0, 0 },
    { "set_room_template",		do_set_room_template,	POS_DEAD,    AB,  LOG_NORMAL, 1, 0, 0 },
    { "cpu",		do_cpu,		POS_DEAD,    AB,  LOG_NORMAL, 1, 0, 0 },
/*
 *Mob Commands
 *
 */
    { "tail",		do_tail,	POS_FIGHTING,   GM,  LOG_NORMAL, 0, 0, 0 },
    { "mpasound",	do_mpasound,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0}, 
    { "mpjunk",		do_mpjunk,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpecho",		do_mpecho,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpset_exit_flags",	do_mpset_exit_flags,	POS_DEAD,	GM,  LOG_NORMAL,	0, 0, 0},
    { "mpdig",		do_mpdig,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpechoat",	do_mpechoat,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpechoaround",	do_mpechoaround,POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpkill",		do_mpkill,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpmload",	do_mpmload,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpoload",	do_mpoload,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mppurge",	do_mppurge,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpgoto",		do_mpgoto,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpat",		do_mpat,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
 /*
  *   { "mphunt",	do_mphunt,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
  *   { "mphuntold",	do_mphunt_old,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
  */  
    { "mptransfer",	do_mptransfer,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},
    { "mpforce",	do_mpforce,	POS_DEAD,	GM,  LOG_NORMAL, 0, 0, 0},


 /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 GM|PL,  LOG_NORMAL, 0, 0, 0 }
};

bool can_do_command(CHAR_DATA *ch, int cmd)
{

    if (IS_SET(cmd_table[cmd].groups, PL ) && IS_SET(ch->cmd_groups, PL ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, AD ) && IS_SET(ch->cmd_groups, AD ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, BB ) && IS_SET(ch->cmd_groups, BB ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, AB ) && IS_SET(ch->cmd_groups, AB ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, GI ) && IS_SET(ch->cmd_groups, GI ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, GD ) && IS_SET(ch->cmd_groups, GD ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, GC ) && IS_SET(ch->cmd_groups, GC ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, AP ) && IS_SET(ch->cmd_groups, AP ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, BP ) && IS_SET(ch->cmd_groups, BP ) )
	return TRUE; 
    if (IS_SET(cmd_table[cmd].groups, GM ) && IS_SET(ch->cmd_groups, GM ) )
	return TRUE; 
return FALSE;
}
/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char command[MAX_INPUT_LENGTH];
    char logline[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char buf2[MAX_INPUT_LENGTH];
    SNOOP_LIST * snoop_list;
    int language;
    int cmd;
    int count;
    bool found, spec_found = FALSE;
    SPECPRO_LIST *s;
    buf[0] = '\0';
    /*
     * Strip leading spaces.
     */
    while ( isspace(*argument) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_FREEZE) )
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	return;
    }
    if ( !IS_NPC(ch)  &&  IS_SET(ch->forge_flags, IS_FORGING ) )
    {
	send_to_char( "Your consentraion is broken while forging!\n\r", ch );
	ch->forge_count = 0;
	REMOVE_BIT(ch->forge_flags, IS_FORGING );
	return;
    }

    if ( !IS_NPC(ch) && (ch->prompt_num > 0) &&
	(ch->prompt_fun != NULL) )
    {
	(*ch->prompt_fun)(ch, argument);
	return;
    }
    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha(argument[0]) && !isdigit(argument[0]) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace(*argument) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }


/*	only 2 or more leeter commands for spec pro's */
/*	to allow common single letter commands to be done */
    if ( command[1] != '\0' )
        for (s = ch->in_room->spro; s != NULL; s = s->next)
        {
           if ( !str_prefix(command,s->sp->trigger) &&
	      (s->sp->type == 'C' 
	      || s->sp->type == 'X'  /* Normal char should be able to leave moveable areas */
	      || s->sp->type == 'Y'  /* Look also for steering room */
	      || ( IS_IMMORTAL(ch) && s->sp->type == 'G') 
	      || ( IS_OPTIVISION(ch) && s->sp->type == 'I')))
		{  (*spec_pro_list[s->sp->spn].command)(ch,argument);
		    ch->happy_hour = 0;
		   return;
		}
         }		
    for ( count = 0; count < MAX_SPECPRO; count++ )
    {
	if (!str_cmp(spec_pro_list[ count ].trigger,command ) ) 
	{
	if ( strlen( spec_pro_list[ count ]. message ) > 0 )
	send_to_char( spec_pro_list[ count ].message, ch );
	spec_found = TRUE;
	}
    }
    found = FALSE;
    /*
     * Look for a command on a mobile.
     */
    if ( ch->in_room != NULL && !IS_NPC(ch) && !found )
    {
        CHAR_DATA *mob;
	bool trig = FALSE;

        for ( mob = ch->in_room->people;  mob != NULL;
              mob = mob->next_in_room )
        {
            if ( IS_NPC(mob) )
            {
	        buf2[0] = '\0';
		strcat(buf2, command);
		strcat(buf2, " ");
		strcat(buf2, argument);
		trig = do_trigger( mob, ch, NULL, buf2, TRIG_COMMAND, TRIG_TYPE_MOB);
		if (trig)
		found = TRUE;	
            }
        }
        if ( found )
            return;
    }
    /*
     * Look for a command on a room.
     */
    if ( ch->in_room != NULL && !found )
    {
	buf2[0] = '\0';
	strcat(buf2, command);
	strcat(buf2, " ");
	strcat(buf2, argument);
        found = do_trigger( ch->in_room, ch, NULL, command, TRIG_COMMAND, TRIG_TYPE_ROOM);
    }
    if ( found )
	return;
    /*
     * Look for a command on a obj.
     */
    if ( ch->in_room != NULL && !found )
    {
        OBJ_DATA *obj;
	bool trig = FALSE;
	for ( obj = ch->in_room->contents ; obj != NULL; obj = obj->next_content )
	{
	    buf2[0] = '\0';
	    strcat(buf2, command);
	    strcat(buf2, " ");
	    strcat(buf2, argument);
            trig = do_trigger( obj, ch, NULL, command, TRIG_COMMAND, TRIG_TYPE_OBJ);
	    if (trig)
	    found = TRUE;	
	}
        /*for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room)*/
        for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	{
	    buf2[0] = '\0';
	    strcat(buf2, command);
	    strcat(buf2, " ");
	    strcat(buf2, argument);
            trig = do_trigger( obj, ch, NULL, command, TRIG_COMMAND, TRIG_TYPE_OBJ);
	    if (trig)
	    found = TRUE;	
	}
	
	if ( found )
	return;
    }

    /*
     * Look for command in command table.
     */
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {   
      
	if ( (command[0] == cmd_table[cmd].name[0])
	&&  !str_prefix( command, cmd_table[cmd].name )
	&&  can_do_command(ch, cmd) 
        &&  (cmd_table[cmd].vnum ? get_obj_carry_vnum(ch,cmd_table[cmd].vnum)!=NULL : 1) /* check if command is item specific */     
        &&  (cmd_table[cmd].room_vnum ? (ch->in_room->vnum == cmd_table[cmd].room_vnum) : 1)) /* check if command is Room specific */	
        {
	    found = TRUE;
	    break;
	}
    }
    if ( found )
    if (!IS_SET(ch->plr, PLR_HOLYLIGHT ) )
    if (!IS_NPC(ch))
    if (!IS_SET( cmd_table[cmd].log , STARTING ) && IS_SET( ch->in_room->room_flags_2, ROOM_STARTING ) )
    {
	send_to_char( "Character creation in progress ( `RType: help creation`w )\n\r", ch );
	return;
    }
    if ( !found )
      for ( language = 0 ; language < MAX_LANGUAGE ; language++ ) 
      {
        if ( !str_prefix(command,skill_table[ language_table[ language ] ].name) )
        {  
    	if (!IS_SET(ch->plr, PLR_HOLYLIGHT ) )
    	if (!IS_NPC(ch))
    	if (IS_SET( ch->in_room->room_flags_2, ROOM_STARTING ) )
    	{
		send_to_char( "Character creation in progress ( `RType: help creation`w )\n\r", ch );
		return;
    	}
	    do_language( ch, argument, language_table[ language ] );
	    return;
	}
    }		

    /*
     * Log and snoop.
     */
    if ( IS_SET( cmd_table[cmd].log , LOG_NEVER ) )
	strcpy( logline, "" );

    if ( ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_LOG) )
    ||   fLogAll)

    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	log_string( log_buf );
    }
    
    if ( IS_SET( cmd_table[cmd].log , LOG_ALWAYS ) )
    {

    sprintf( log_buf, "Log %s: %s", ch->name, logline );
    imp_log_string( log_buf );
    }

    if ( ch->desc != NULL )
	for ( snoop_list = ch->desc->snoop_by ; snoop_list != NULL ; snoop_list = snoop_list->next )
	{
	    write_to_buffer( snoop_list->desc, "% ",    2 );
	    write_to_buffer( snoop_list->desc, logline, 0 );
	    write_to_buffer( snoop_list->desc, "\n\r",  2 );
	}

    if ( IS_SET( cmd_table[cmd].log , NO_HAPPY_HOUR ) )
	ch->happy_hour = 0;
    
    if (!found)
    if (!IS_SET(ch->plr, PLR_HOLYLIGHT ) )
    if (!IS_NPC(ch))
    if (IS_SET( ch->in_room->room_flags_2, ROOM_STARTING ) )
    {
	send_to_char( "Character creation in progress ( `RType: help creation`w )\n\r", ch );
	return;
    }
    if (!found)
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	{
	if (!spec_found)
	    send_to_char( "Huh?\n\r", ch );
	}
	return;
    }


    /*
     * Character not in position for command?
     */
    if ( !IS_IMMORTAL( ch ) && ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r", ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "You are too busy battling with your nightmares!\n\r", ch );
	    break;


	case POS_RESTING:
	    send_to_char( "You can't do that while this relaxed.\n\r", ch);
	    break;

	case POS_SITTING:
	    send_to_char( "Better stand up first.\n\r",ch);
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r", ch);
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) ( ch, argument );

    tail_chain( );
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    char buf[MAX_INPUT_LENGTH];
    int cmd;
    bool found;
    int RSocial;
    int NSocial; 
    int SSocial; 
    found  = FALSE;

    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	&&   !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC(ch) && IS_SET(ch->comm, COMM_NOEMOTE) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r", ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too bad for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r", ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "You are too busy battling with your nightmares!\n\r", ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	cact( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM,"`g");
	cact( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR,"`g");
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL &&
	( !IS_IMMORTAL( ch ) || ( victim = get_char_world( ch, arg ) ) == NULL ) )
    {
	cact( social_table[cmd].char_not_found,   ch, NULL, victim, TO_CHAR,"`g");
    }
    else if ( IS_SET( victim->act2, ACT2_MOVEABLE_AREA ) )
    {
	sprintf(buf,"%s doesn't respond\n\r",victim->short_descr);
	send_to_char( buf, ch );
    }
	
    else if ( victim == ch )
    {
	if (!IS_NPC(victim) )
	{
	cact( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM,"`g");
	cact( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR,"`g");
	}
    }
    else
    {
	cact( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT,"`g");
	cact( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR,"`g");
	cact( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT,"`g");
      if ( social_table[cmd].item > 0 ) /* Give the char some useless item */ 
	{
	if ( ( pObjIndex = get_obj_index(  social_table[cmd].item  ) ) == NULL )
	  {
	   sprintf(buf,"Social %s: Bad vnum %ld \n\r",social_table[cmd].name, social_table[cmd].item );
 	   bug(buf,0);
	   return TRUE;
	  }
	 obj = create_object( get_obj_index(social_table[cmd].item));
	 obj->wear_data->cost = 0;
	 obj_to_char(obj,victim);
	}

	SSocial = 0;
	if ( !IS_NPC(ch) && IS_NPC(victim)
	&&   !IS_AFFECTED(victim, AFF_CONTROL_MIND)
	&&   IS_AWAKE(victim) 
	&&   victim->desc == NULL)
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:
		cact( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT,"`g" );
		cact( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR,"`g"    );
		cact( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT,"`g");
		break;

	    case 1: 
		cact( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT,"`g");
		cact( "You slap $N.",  victim, NULL, ch, TO_CHAR,"`g");
		cact( "$n slaps you.", victim, NULL, ch, TO_VICT,"`g");
		break;
	    case 2:  case 3:  case 4: case 5:
	    case 6:  case 7:  case 8: case 9:
	    case 10: case 11: case 12:
	    if ( social_table[cmd].type > 0 )  /* Check for a Type */
	      {
		 RSocial = ( number_range (0, ( social_count -1 )) ); 
		 for (NSocial = RSocial; NSocial < social_count; NSocial++ )
		   {
		     if ( NSocial == ( social_count -1 ) )
			{
			NSocial = 0;
			break;
			}
		     if ( social_table[NSocial].type == social_table[cmd].type ) /*  Like types */
			{
			cact( social_table[NSocial].others_found,
			    victim, NULL, ch, TO_NOTVICT,"`g" );
			cact( social_table[NSocial].char_found,
			    victim, NULL, ch, TO_CHAR,"`g"    );
			cact( social_table[NSocial].vict_found,
			    victim, NULL, ch, TO_VICT,"`g");
			SSocial = NSocial;
			NSocial = social_count;
			break;
			}
		   }
		}
	/* next section check for damage item and action */
	      if ( social_table[SSocial].damage > 0 )  /* Ok, lets do some damage to the Charactor! */
		{
		 send_to_char("`ROUCH!`w\n\r",ch);
		 if (ch->hit > 5)
		 ch->hit--; 
		}	
	      if ( social_table[SSocial].item > 0 ) /* Give the char some useless item */ 
		{
		if ( ( pObjIndex = get_obj_index(  social_table[SSocial].item  ) ) == NULL )
		  {
		   sprintf(buf,"Social %s: Bad vnum %ld \n\r",social_table[SSocial].name, social_table[SSocial].item );
 		   bug(buf,0);
		   return TRUE;
		  }
		 obj = create_object( get_obj_index(social_table[SSocial].item));
		 obj->wear_data->cost = 0;
		 obj_to_char(obj,ch);
		}
	      if ( social_table[cmd].mood != 0 )  /* Increase or Decreace Mobs mood. */
		{
		 if ( social_table[cmd].mood > 0 )
		   {
		   if ( victim->mood < MAX_MOOD )
		   victim->mood++; 
		   }
		 else
		   {
		 if ( social_table[cmd].mood < 0 )
		 if ( victim->mood  > 0 ) 
		 victim->mood--;
	    	    if ( victim->mood == 0 && victim->pIndexData->pShop == NULL
			 && !IS_SET(victim->act, ACT_TRAIN)
			 && !IS_SET(victim->act, ACT_IS_HEALER))
	    	     {
			send_to_char("`Y",ch);
			act("$n screams, and attacks!", victim, NULL, ch, TO_VICT );
			if ( get_skill( victim, gsn_shadowstrike ) )
			{
			    do_shadowstrike( victim, ch->name );
	    	        multi_hit( victim, ch, TYPE_UNDEFINED );
			}
			else
			{
			    victim->fighting = ch;
	    	        multi_hit( victim, ch, TYPE_UNDEFINED );
			}
	    	     }
		   }
		}	
	      break;

	    }
	}
    }

    return TRUE;
}

/*
 * Return true if an argument is completely numeric.
 */
bool is_number ( char *arg )
{
 
    if ( *arg == '\0' )
        return FALSE;
 
    if ( *arg == '+' || *arg == '-' )
        arg++;
 
    for ( ; *arg != '\0'; arg++ )
    {
        if ( !isdigit( *arg ) )
            return FALSE;
    }
 
    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy (arg,  pdot+1 );
	    return number;
	}
    }

    strcpy (arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}
char *one_argument_any( char *argument, char *arg_first )
{
    char cEnd;

   
    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	if ( *argument == '\n' )
	    break;
	*arg_first = LOWER(*argument);
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

   /* while ( isspace(*argument) )
	argument++; */

    return argument;
}
char *one_argument_case( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace(*argument) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace(*argument) )
	argument++;

    return argument;
}

/*
 * Contributed by Alander.
 */
void do_commands( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];

    int cmd;
    int col;
    buf1[0] = '\0'; 
    col = 0;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if   (can_do_command(ch, cmd) 
	&&   IS_SET(cmd_table[cmd].groups, PL )
	&&   cmd_table[cmd].show
	&&   !cmd_table[cmd].vnum
   	&&   !cmd_table[cmd].room_vnum)
	{
	    sprintf( buf, "%-12s", cmd_table[cmd].name );
	    strcat( buf1, buf );
	    if ( ++col % 6 == 0 )
		strcat( buf1,"\n\r");
	}
    }
    page_to_char(buf1,ch); 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}

void do_wizhelp( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    int cmd;
    int col, s, x;
    bool first = TRUE;
 
    col = 0;
    s = 1;
    for ( x = 1;  x < MAX_CMD_GROUPS; x++ )
    {
	if ( x == 1 )
	s = PL;
	if ( x == 2 )
	s = BB;
	if ( x == 3 )
	s = AB;
	if ( x == 4 )
	s = GI;
	if ( x == 5 )
	s = GD;
	if ( x == 6 )
	s = GC;
	if ( x == 7 )
	s = AP;
	if ( x == 8 )
	s = BP;
	if ( x == 9 )
	s = AD;
	if ( x == 10 )
	s = GM;
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
        if ( can_do_command(ch, cmd)
	&&   !IS_SET(cmd_table[cmd].groups, PL)
	&&   IS_SET(cmd_table[cmd].groups, s)
        &&   cmd_table[cmd].show
        &&   !cmd_table[cmd].vnum
        &&   !cmd_table[cmd].room_vnum)
	{
	    if ( first )
	    {
    		if ( col % 6 != 0 )
		send_to_char( "\n\r", ch );
		sprintf(buf, "%s\n\r", cmd_group_type_name( s ) );
		send_to_char(buf,ch);
		col = 0;
		first = FALSE;
	    }
	    sprintf( buf, "%-12s",cmd_table[cmd].name );
	    send_to_char( buf, ch );
	    if ( ++col % 6 == 0 )
		send_to_char( "\n\r", ch );
	}
    }
    first = TRUE;
    }
 
    if ( col % 6 != 0 )
	send_to_char( "\n\r", ch );
    return;
}

void do_specroom (CHAR_DATA *ch, char *argument)
{ char buf[MAX_STRING_LENGTH];
  SPECPRO_LIST *s;

  sprintf (buf,"%-10s %-15s%-45s%-7s\n\r","Number", "Trigger","Description","Type");
  send_to_char (buf,ch);
  if (ch->in_room->spro == NULL)
    { sprintf(buf,"`W%-42s`w\n\r","`WNONE`w");
      send_to_char (buf,ch);
      return;
    }
  for (s = ch->in_room->spro;s!=NULL;s = s->next)
   {  sprintf (buf,"`W%-10d %-15s%-45s%-4ic`w\n\r",s->sp->spn, s->sp->trigger,s->sp->descr,s->sp->type);
      send_to_char (buf,ch);
   }
  return;
}
 
void do_show_roomstats (CHAR_DATA *ch, char *argument)
{ char buf[MAX_STRING_LENGTH];

  sprintf (buf,"`G%s\n\r","Current Room Flags`w");
  send_to_char (buf,ch);
  if (ch->in_room->room_flags == 0 && ch->in_room->room_flags_2 == 0)
  { 
	sprintf(buf,"`G%-42s\n\r`w","`GNONE`w");
      	send_to_char (buf,ch);
      	return;
  }

  if (ch->in_room->room_flags_2 != 0)
  {
  sprintf (buf,"`G%s `w", room_bit_name_2( ch->in_room->room_flags_2 ) );
    send_to_char (buf,ch);
  }

  if (ch->in_room->room_flags != 0 )
  {
  sprintf (buf,"`G%s`w", room_bit_name( ch->in_room->room_flags ) );
    send_to_char (buf,ch);
  }

  send_to_char( "\n\r", ch );
  return;
}


void do_show_sector_type (CHAR_DATA *ch, char *argument)
{ char buf[MAX_STRING_LENGTH];

  sprintf (buf,"`G%s\n\r","Current Sector Type`w ");
  send_to_char (buf,ch);
  if (ch->in_room->sector_type == 0)
    { sprintf(buf,"`G%-42s\n\r","NONE`w");
      send_to_char (buf,ch);
      return;
    }
   {
  sprintf (buf,"`G%s`w\n\r", sector_bit_name( ch->in_room->sector_type ) );
    send_to_char (buf,ch);
  return;
   }
}



void do_color_check (CHAR_DATA *ch, char *argument)
{ 
  send_to_char("\n\r`kDark Black",ch);
  send_to_char("\n\r`rDark Red",ch);
  send_to_char("\n\r`gDark Green",ch);
  send_to_char("\n\r`yDark Yellow",ch);
  send_to_char("\n\r`bDark Blue",ch);
  send_to_char("\n\r`mDark Magenta",ch);
  send_to_char("\n\r`cDark Cyan",ch);
  send_to_char("\n\r`wDark White",ch);
  send_to_char("\n\r`KLight Black",ch);
  send_to_char("\n\r`RLight Red",ch);
  send_to_char("\n\r`GLight Green",ch);
  send_to_char("\n\r`YLight Yellow",ch);
  send_to_char("\n\r`BLight Blue",ch);
  send_to_char("\n\r`MLight Magenta",ch);
  send_to_char("\n\r`CLight Cyan",ch);
  send_to_char("\n\r`WLight White`w",ch);
return;
}
