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
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif
 
#include <unistd.h>
#include "merc.h"
 
#if !defined(OLD_RAND)
/* long random(); */
void srandom(unsigned int);
int getpid();
time_t time(time_t *tloc);
#endif
 
#if defined(unix)
/* extern int getrlimit(int resource, struct rlimit *rlp); */
/* extern int setrlimit(int resource, const struct rlimit *rlp); */
#endif
 
#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif

long vnum_test = 0; 
/*
 * Globals.
 */
int mob_list_number	= 0;
QUEST_DATA * 		quest_data_list;
GLOBAL_RANDOM_DATA *	random_data_list;
HELP_DATA *             help_first;
HELP_DATA *             help_last;
 
SHOP_DATA *             shop_first;
SHOP_DATA *             shop_last;
 
CHAR_DATA *             char_free;
EXTRA_DESCR_DATA *      extra_descr_free;
NOTE_DATA *             note_free;
GROUP_DATA *		group_list;
OBJ_DATA *              obj_free;
PC_DATA *               pcdata_free;
char                    bug_buf         [2*MAX_INPUT_LENGTH];
CHAR_DATA *             char_list=NULL;
CHAR_DATA *		mob_list_update[16];
CHAR_DATA * mob_list_current_update = NULL;
ROOM_INDEX_DATA *       room_trig_list=NULL;
CLAN_DATA *             clan_list=NULL;
TOP_TEN_DATA		top_ten_list 	[ 10 ][ 3 ];
/*char *                        help_greeting;*/
char *				help_greeting[MAX_GREETING];
struct 	race_list_type		race_list[MAX_RACE];
struct 			social_type	social_table		[MAX_SOCIALS];
int			social_count		= 0;
sh_int 			language_table[ MAX_LANGUAGE ];
sh_int			reboot_timer = 0;
bool 			disable_timer_abort; /* HOT boot */
bool			reboot_pause = TRUE;
sh_int			top_race=0;
sh_int			top_guild=0;
sh_int			top_greeting=0;
int			LOGON_NUMBER;
char                    log_buf         [2*MAX_INPUT_LENGTH];
char			imp_buf		[2*MAX_INPUT_LENGTH];
KILL_DATA               kill_table      [MAX_LEVEL];
NOTE_DATA *             note_list=NULL;
OBJ_DATA *              object_list=NULL;
TIME_INFO_DATA          time_info[MAX_WORLD];
TIME_INFO_DATA		time_new;
WEATHER_DATA            weather_info[MAX_WORLD];
bool                    free_room[MAX_VNUM]; 
bool                    free_obj_list[MAX_VNUM];
bool                    free_trig_list[MAX_VNUM];
bool                    free_mob_list[MAX_VNUM];
OBJ_INDEX_DATA *	free_obj_index;
MOB_INDEX_DATA *	free_mob_index;

CHANGE_DATA *           change_list = NULL;
CHANGE_DATA *           change_free;

sh_int			gsn_element_power[MAX_ELEMENT_TYPE][MAX_ELEMENT_POWER];
sh_int			gsn_weapon_power[MAX_WEAPON_TYPE][MAX_WEAPON_POWER];
sh_int                  gsn_common;
sh_int                  gsn_belegfea;
sh_int                  gsn_mirros;
sh_int                  gsn_aranna;
sh_int                  gsn_iavartor;
sh_int                  gsn_tarnanthalion;
sh_int                  gsn_milrochdil;
sh_int                  gsn_manlhach;
sh_int                  gsn_nenkhilmen;
sh_int                  gsn_sulkano;
sh_int			gsn_ventriloquate;
sh_int			gsn_poison_blade;
sh_int			gsn_double_strike;
sh_int			gsn_strike;
sh_int                  gsn_shadowstrike;
sh_int                  gsn_retreat;
sh_int                  gsn_dodge;
sh_int                  gsn_quick_strike;
sh_int                  gsn_wind_over_sand;
sh_int                  gsn_hide;
sh_int                  gsn_fade;
sh_int                  gsn_hold_breath;
sh_int                  gsn_peek;
sh_int                  gsn_pick_lock;
sh_int                  gsn_first_aid;
sh_int                  gsn_tame_animal;
sh_int                  gsn_counter_tracking;
sh_int			gsn_cooking;
sh_int                  gsn_track;
sh_int                  gsn_hunt;
sh_int                  gsn_detect_trap;
sh_int                  gsn_disarm_trap;
sh_int                  gsn_sneak;
sh_int                  gsn_steal;
sh_int                  gsn_remove_blindness;
sh_int                  gsn_remove_poison;
sh_int                  gsn_cure_disease;
 
sh_int                  gsn_disarm;
sh_int                  gsn_weapon_enhancement;
sh_int                  gsn_kick;
sh_int                  gsn_parry;
sh_int                  gsn_counter_strike;
sh_int                  gsn_repair;
sh_int                  gsn_rescue;
sh_int                  gsn_second_attack;
sh_int                  gsn_third_attack;
 
sh_int                  gsn_blinding_light;
sh_int                  gsn_sand_storm;
sh_int                  gsn_control_mind;
sh_int                  gsn_curse;
sh_int                  gsn_fire_shield;
sh_int                  gsn_stone_skin;
sh_int                  gsn_sanctuary;
sh_int                  gsn_earth_shield;
sh_int                  gsn_water_shield;
sh_int                  gsn_spirit_shield;

sh_int                  gsn_shadowform;
sh_int                  gsn_mass_shadowform;
sh_int                  gsn_poison;
sh_int                  gsn_plague;
sh_int                  gsn_sleep;
sh_int                  gsn_wind_shield;
 
/* new gsns */
 
sh_int                  gsn_shield_block;
sh_int			gsn_create_minor_portal;
sh_int			gsn_create_water;
sh_int			gsn_sacrifice;
sh_int			gsn_minor_sacrifice;
sh_int			gsn_cure_light;
sh_int			gsn_cure_serious;
sh_int			gsn_cure_critical;
sh_int			gsn_heal;
sh_int			gsn_continual_light;
sh_int			gsn_levitate;
sh_int			gsn_swim;
 
sh_int			gsn_jab;
sh_int			gsn_endurance;
sh_int			gsn_perception;
sh_int			gsn_stun;
sh_int			gsn_thrust;
sh_int			gsn_rejuvenate;
sh_int			gsn_whirl;
sh_int                  gsn_bash;
sh_int			gsn_intimidate;
sh_int			gsn_shriek;
sh_int                  gsn_fast;
sh_int                  gsn_breathfire;
sh_int                  gsn_precision_strike;
sh_int                  gsn_dual_wield;
sh_int                  gsn_berserk;
sh_int                  gsn_dirt;
sh_int                  gsn_hand_to_hand[MAX_WEAPON_POWER];
sh_int                  gsn_trip;
sh_int                  gsn_tail;
sh_int			gsn_slam;
sh_int			gsn_ki_force;
sh_int			gsn_shola_punch;
sh_int			gsn_shola_kick;
sh_int			gsn_high_kick;
sh_int			gsn_knee_punch;
sh_int			gsn_leg_sweep;
sh_int			gsn_shola_toss;
sh_int			gsn_combination;
sh_int                  gsn_acidbrt;
 
 
sh_int                  gsn_bind_wounds;
sh_int                  gsn_courage;
sh_int                  gsn_tarvals_wrath;
sh_int                  gsn_blur;
sh_int                  gsn_haggle;
sh_int                  gsn_riding;
sh_int                  gsn_riding_air;
sh_int                  gsn_lore;
sh_int                  gsn_meditation;
sh_int                  gsn_youna;
sh_int			gsn_deep_meditation;
 
sh_int                  gsn_scrolls;
sh_int                  gsn_staves;
sh_int                  gsn_wands;
sh_int                  gsn_refresh;
sh_int                  gsn_remove_curse;
sh_int                  gsn_awareness;
sh_int                  number_tokens;
sh_int                  number_tokens_2;
sh_int                  number_tokens_3;
sh_int                  number_tokens_4;
sh_int                  number_tokens_5;
sh_int                  number_tokens_6;
sh_int                  number_tokens_7;
sh_int                  number_tokens_8;
sh_int                  number_tokens_9;
sh_int                  number_tokens_10;
sh_int                  r_item;
sh_int			gsn_senses;
sh_int			gsn_detect_shadowform;
sh_int			gsn_detect_hidden;
sh_int			gsn_circle_of_protection;
sh_int			gsn_faerie_fire;
sh_int			gsn_fly;
sh_int			gsn_haste;
sh_int			gsn_slow;
sh_int			gsn_calm_spirit;
sh_int			gsn_infravision;
sh_int			gsn_breathe_underwater;
sh_int			gsn_dimension_walk;
sh_int          gsn_rebuke;

sh_int			gsn_mining;
sh_int			gsn_golem_making;
sh_int			gsn_weapon_smithing;
sh_int			gsn_armor_smithing;
sh_int			gsn_assaying;
sh_int			gsn_metallurgy;
sh_int			gsn_absorb;

/*
 * Locals.
 */
MOB_INDEX_DATA *        mob_index_hash          [MAX_KEY_HASH];
TRIGGER_INDEX_DATA *    trig_index_hash         [MAX_KEY_HASH];
OBJ_INDEX_DATA *        obj_index_hash          [MAX_KEY_HASH];
ROOM_INDEX_DATA *       room_index_hash         [MAX_KEY_HASH];
char *                  string_hash             [MAX_KEY_HASH];
 
AREA_DATA *             area_first;
AREA_DATA *             appartment_area;
AREA_DATA *             area_last;

FINGER_DATA *		finger_list;
 
char *                  string_space;
char *                  top_string;
char                    str_empty       [1];
char *			crnt_area; 
int                     top_area;
int                     top_ed;
int                     top_card_data;
long                    top_exit;
long                    top_str_dup;
int                     top_help;
int                     top_change;
int			top_bfs_queue;
int			top_bfs_room;
int			top_world;
int                     top_reset;
int                     top_event;
int                     top_quest;
int                     top_char_quests;
int                     top_shop;
int                     top_pcdata;
long			top_obj_index;
long			char_index;
long			obj_index;
long			top_mob_index;
long			top_trigger_index;
long			top_affect_obj;
long			top_affect_mob;
long			top_room;
long			top_room_real;
long			top_room_index;
int                     mobile_count = 0;
int                     object_count = 0;
long			top_vnum_room; 
long                    top_cont_data;
long                    top_alloc_mem;
long                    top_read_notes_data;
int                     top_debt_data;
int                     top_descriptor;
int                     top_bet_data;
int                     top_ban_data;
long                    top_skill_list;
long			top_castle_data; /* castle code */
int                     top_buffer;
int                     top_finger_data;
int                     top_wizlist_data;
int                     top_specpro_list;
int                     top_bhost_list;
int                     top_editing_data;
int                     top_note_data;
int                     top_change_data;
int                     top_weather_data;
int                     top_group_data;
int                     top_gainer_data;
int                     top_clan_data;
long			top_pcclan_data;
long                    top_track_data;
long                    top_track_type;
long                    top_inside_area_data;
long                    top_spell_data;
long                    top_gate_data;
long                    top_light_data;
long                    top_approve_data;
long                    top_lock_data;
long                    top_trigger_data;
long                    top_trigger_list_data;
long                    top_script_data;
long                    top_variable_data;
long                    top_wear_data;
long                    top_equip_data;
long                    top_inside_data;
long                    top_weapon_data;
long                    top_edible_data;
long                    top_trap_data;
long                    top_moveable_data;
long                    top_magic_data;
long                    top_logon_data;
long                    top_random_data;

char * world_group [] = { "nenkemen default", "maegmenel default", "lithdor default", "" };
 
/*
 * Memory management.
 * Increase MAX_STRING if you have too.
 * Tune the others only if you understand what you're doing.
 */
/*#define                 MAX_STRING      8990976 */
#define                 MAX_STRING      3500000 
#define                 MAX_PERM_BLOCK  438192
#define                 MAX_MEM_LIST    14 
 
void *                  rgFreeList      [MAX_MEM_LIST];
const long              rgSizeList      [MAX_MEM_LIST] =
{
    16, 32, 64, 128, 256, 1024, 2048, 4096, 8192, 16384, 32768, 65536 ,131072,262144
};
 
int                     nAllocString;
int                     sAllocString;
int                     nAllocPerm;
int                     sAllocPerm;
 
 
 
/*
 * Semi-locals.
 */
bool                    fBootDb;
bool			fBootDB_Area_Update;
bool                    fBootHomes;
FILE *                  fpArea;
FILE *                  fpArea2;
FILE *                  tempArea;
char                    strArea[MAX_INPUT_LENGTH];
sh_int			numWorld;
int                     numArea;
//char                    buf[MAX_STRING_LENGTH];
//char *                  buf2;
 
 
 
DECLARE_SPELL_FUN( spell_null );
#if defined(unix)
/* RT max open files fix */

void maxfilelimit()
{
    struct rlimit r;
 
    getrlimit(RLIMIT_NOFILE, &r);
    r.rlim_cur = r.rlim_max;
    setrlimit(RLIMIT_NOFILE, &r);
}

#endif
 
/*
 * Big mama top level function.
 */
void boot_db( void )
{
    char buf[MAX_STRING_LENGTH];
    
#if defined(unix)
    /* open file fix */
    maxfilelimit();
#endif

    log_string("boot_db");
 
    /*
     * Init some data space stuff.
     */
    {
	int x;
 
        if ( ( string_space = (char *) calloc( 1, MAX_STRING ) ) == NULL )
        {
            bug( "Boot_db: can't alloc %d string space.", MAX_STRING );
            exit( 1 );
        }
        top_string      = string_space;
        fBootDb         = TRUE;
	for ( x = 0 ; x < 31 ; x++)
	per_cpu_usage[x] = 0;
	for ( x = 0; x < 16; x++ )
	mob_list_update[x] = NULL;
    }

 
    object_list = NULL;
    char_list = NULL;
    /*
     * Init random number generator.
     */
    {
         init_mm();
    }

    log_string("Loading Race List");
    load_rlist();
    log_string("Loading Races");
    load_races();
    log_string("Loading Guild List");
    load_glist();
    log_string("Loading Guilds");
    load_guilds();
    log_string("Loading Trigger Index");
    load_trigger_index();

    /*
     * Set time.
     */
    {
        long lhour, lday, lmonth, world;
 
        lhour           = (current_time - 650336715)
                        / (PULSE_TICK / PULSE_PER_SECOND);
	time_new.hour	= lhour %24;
	lday		= lhour / 24;
	time_new.day	= lday % 35;
	lmonth		= lday / 35;
	time_new.month  = lmonth % 17;
	time_new.year	= lmonth / 17;    
	for ( world = 0 ; world < MAX_WORLD ; world++ )
	{
	    lhour += number_percent();
            time_info[ world ].hour  = lhour  % time_table[ world ].hours_day;
            lday            = lhour  / time_table[ world ].hours_day;
            time_info[ world ].day   = lday   % time_table[ world ].days_month;
            lmonth          = lday   / time_table[ world ].days_month;
            time_info[ world ].month = lmonth % time_table[ world ].months_year;
            time_info[ world ].year  = lmonth / time_table[ world ].months_year;
 	}
    }

    /*
     * Assign gsn's for skills which have them.
     */
    {
        int sn,language;
 
	language = 0;
        for ( sn = 0; sn < MAX_SKILL; sn++ )
        {
            if ( skill_table[sn].pgsn != NULL )
                *skill_table[sn].pgsn = sn;
	    if ( is_language( sn ) )
		language_table[ language++ ] = sn;
        }
    }
 
    {
	int race;
	for( race = 0; race < MAX_HORSE_RACE; race++ )
	    setup_race( &horse_race_list[ race ] );

    }


    log_string("Loading Areas");
    /*
     * Read in all the area files.
     */
    {
        FILE *fpList;
	crnt_area = "area";
 
        if ( ( fpList = fopen( AREA_LIST, "r" ) ) == NULL )
        {
            perror( AREA_LIST );
            exit( 1 );
        }
 
        for ( ; ; )
        {
            strcpy( strArea, fread_word( fpList ) );

            if ( strArea[0] == '$' )
                break;
 
	    sprintf(buf,"Loading %s",strArea);
	    log_string(buf);
 
            new_load_area();
 
        }
	if ( fpList )
        fclose( fpList );
    }
 
    /*
     * Fix up exits.
     * Declare db booting over.
     * Reset all areas once.
     * Load up the notes file.
     * Load up the clans file.
     */
    {
        log_string("Fixing Exits");
        fix_exits( );
        fBootDb = FALSE;
        log_string("Loading Wizlist");
	load_wizlist();
	log_string("Loading Finger");
	load_finger();
        log_string("Loading Notes");
        load_notes( );
        log_string("Loading Changes");
        load_changes( );
	clan_list = NULL;
        log_string("Loading Clans");
        load_clans( );
        log_string("Loading Logons");
	if (LOGON_ON)
        load_logon( );
        log_string("Loading Randoms");
        load_random( );
        log_string("Loading Counter");
        load_counter( );
        log_string("Loading TopTen");
	load_topten( );
        log_string("Loading Quests");
	load_quests();
        log_string("Loading Helps");
	load_helps_new();
        log_string("Loading Socials");
	load_socials();
        fBootHomes = TRUE;
        log_string("Loading Perm Homes");
        load_perm_rooms();
        fBootHomes = FALSE;
        log_string("Updating all areas");
        fBootDB_Area_Update = TRUE;
        area_update( );
        fBootDB_Area_Update = FALSE;
    }
 
    return;
}
 
/*
 * Snarf a help section.
 */
void load_helps_new()
{
    HELP_DATA *pHelp;
    FILE *fp;
 
    if ( ( fp = fopen( HELPSAVE_FILE, "r" ) ) == NULL )
        return;

    for ( ; ; )
    {
        pHelp           = new_help();
        pHelp->level    = fread_number( fp );
        pHelp->vnum     = fread_number( fp );
	pHelp->vnum = top_help;
        strncpy( pHelp->keyword, fread_string( fp ), 50 );
        if ( pHelp->keyword[0] == '$' )
            break;
        pHelp->text     = fread_string( fp );
 
        if ( !str_cmp( pHelp->keyword, "greeting" ) )  
            {help_greeting[top_greeting] = pHelp->text;
             top_greeting++;
            }
 
        if ( help_first == NULL )
            help_first = pHelp;
        if ( help_last  != NULL )
            help_last->next = pHelp;
 
        help_last       = pHelp;
        pHelp->next     = NULL;
    }

if ( fp )
fclose(fp);
return;
}
 
#if defined(KEY)
#undef KEY
 #endif
 
#define KEY( literal, field, value )                        \
            if ( !str_cmp( word, literal ) )                \
            {                                               \
                field = value;                              \
                tMatch = TRUE;                              \
                break;                                      \
            }
void load_trigger_index(void)
{
    FILE *fp; 
    TRIGGER_INDEX_DATA *pTrigIndex;
    int iHash;
    char *word, *temp;
    bool tMatch, tDone;
    bool fMatch = TRUE;
    bool fDone = FALSE;

    pTrigIndex = NULL;

    if ( ( fp = fopen( TRIGGER_DATA_FILE, "r" ) ) == NULL )
        return;

    for ( ; fMatch && !fDone; )
    { fMatch = FALSE;
        tMatch = TRUE;
        tDone = FALSE;
        word = feof( fp ) ? str_dup("#END") : fread_word( fp );

	switch ( UPPER( word[0] ) )
	{
	case '*': /* In-file comments */
            fread_to_eol( fp );
            fMatch = TRUE;
	    break;
	case '#':
            if ( !str_cmp( word, "#END" ) )
            {
                fMatch = TRUE;
		fDone = TRUE;
                break;
            }
            if ( !str_cmp( word, "#TRIGGER" ) )
            {
                fMatch          = TRUE;

                pTrigIndex = pTrigIndex_alloc();
		temp = fread_word( fp ) ;

    		for ( ; tMatch && !tDone; )
    		{
    		   word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    		   tMatch = FALSE;

    		   switch ( UPPER(word[0]) )
    		   {
    		      case '*': fread_to_eol( fp ); fMatch = TRUE; break;
    		      case 'B':
			if ( !str_cmp( word, "Builders" ) )
			{
    		            strncpy( pTrigIndex->builders, fread_string( fp ), 30 );
			    tMatch = TRUE;
			    break;
			}
    		       break;
    		      case 'C':
    		        KEY( "Chance",     pTrigIndex->chance, fread_number( fp ) );
    		       break;
    		      case 'D':
			if ( !str_cmp( word, "Desc" ) )
			{
    		            strncpy( pTrigIndex->desc, fread_string( fp ), 80 );
			    tMatch = TRUE;
			    break;
			}
    		       break;
    		      case 'F':
    		        KEY( "Flags",     pTrigIndex->flags, fread_number( fp ) );
    		       break;
    		      case 'K':
			if ( !str_cmp( word, "Keywords" ) || !str_cmp( word, "Key_sim" ) )
			{
    		            strncpy( pTrigIndex->key_sim, fread_string( fp ), 30 );
			    tMatch = TRUE;
			    break;
			} 
			if ( !str_cmp( word, "Key_words" ) )
			{
    		            strncpy( pTrigIndex->key_words, fread_string( fp ), 30 );
			    tMatch = TRUE;
			    break;
			}
			if ( !str_cmp( word, "Key_string" ) )
			{
    		            strncpy( pTrigIndex->key_string, fread_string( fp ), 30 );
			    tMatch = TRUE;
			    break;
			}
    		       break;
    		      case 'E':
    		        if ( !str_cmp( word, "End" ) )
    		        {
    		           tMatch = TRUE;
    		           tDone = TRUE;
    		        }
    		       break;
    		      case 'G':
    		        KEY( "Gets_vnum",     pTrigIndex->gets_vnum, fread_number( fp ) );
    		       break;
    		      case 'M':
    		        KEY( "Mob_vnum",     pTrigIndex->mob_vnum, fread_number( fp ) );
    		       break;
    		      case 'N':
			if ( !str_cmp( word, "Name" ) )
			{
    		            strncpy( pTrigIndex->name, fread_string( fp ), 30 );
			    tMatch = TRUE;
			    break;
			}
    		       break;
    		      case 'Q':
    		        KEY( "Quests",     pTrigIndex->quests, fread_number( fp ) );
    		        KEY( "Quests_pre",     pTrigIndex->quests_pre, fread_number( fp ) );
    		       break;
    		      case 'S':
    		        KEY( "Security",     pTrigIndex->security, fread_number( fp ) );
    		        KEY( "Script_type",  pTrigIndex->script_type, fread_number( fp ) );
    		        KEY( "Step",     pTrigIndex->step, fread_number( fp ) );
    		        KEY( "Step_pre",     pTrigIndex->step_pre, fread_number( fp ) );
    		       if ( !str_cmp( word, "Sc" ) || !str_cmp( word, "Script" ) )
    		       {
    		       SCRIPT_DATA *scr;
	
    		       if ( pTrigIndex->script != NULL )
    		       {
    		          for ( scr = pTrigIndex->script; scr->next != NULL; scr = scr->next )
    		          ;      /* scan to the end */
    		          scr->next                = (SCRIPT_DATA *) alloc_perm( sizeof(*scr) );
    		          scr = scr->next;
			      top_script_data++;
    		       }
    		       else
    		       {
    		           pTrigIndex->script = (SCRIPT_DATA *) alloc_perm( sizeof(*scr) );
    		           scr = pTrigIndex->script;
			      top_script_data++;
    		       }
		
    		       scr->command 		= fread_string( fp );
    		       scr->next                = NULL;
    		       tMatch = TRUE;
    		       }
    		      break;

    		      case 'T':
    		        KEY( "Type",       pTrigIndex->trigger_type, fread_number( fp ) );
    		        KEY( "Timer",     pTrigIndex->timer, fread_number( fp ) );
    		       break;
    		      case 'V':
    		        if ( !str_cmp( word, "Vnum" ) )
 			{
    		        pTrigIndex->vnum = fread_number( fp );
            		iHash                   = pTrigIndex->vnum % MAX_KEY_HASH;
            		pTrigIndex->next         = trig_index_hash[iHash];
            		trig_index_hash[iHash]   = pTrigIndex;
            		free_trig_list[pTrigIndex->vnum] = 1;
    		        tMatch = TRUE;
			}
    		       break;
		    }
		}
    	    }
    	}
    }
    if ( !fMatch )
    {
        fpArea = fp;
        bug( "Load_trigger_index: bad key word.", 0 );
        exit( 1 );
    }
  return;
}
 
/*
 * Snarf a reset section.
 */
void load_resets_new( AREA_DATA *pArea )
{
    RESET_DATA *pReset;
    FILE *fp; 
    char buf[MAX_INPUT_LENGTH];

    sprintf(buf, "%s%s/resets", SAVEAREAS_DIR, pArea->filename );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
        return;

    for ( ; ; )
    {
        ROOM_INDEX_DATA *pRoomIndex;
        EXIT_DATA *pexit;
        char letter;
 
        if ( ( letter = fread_letter( fp ) ) == 'S' )
            break;
 
        if ( letter == '*' )
        {
            fread_to_eol( fp );
            continue;
        }
 
        pReset          = reset_alloc();
        pReset->command = letter;
        pReset->chance  = fread_number( fp );
        pReset->arg1    = fread_number( fp );
        pReset->arg2    = fread_number( fp );
        pReset->vnum    = fread_number( fp );  
        pReset->arg3    = (letter == 'G' || letter == 'R')
                            ? 0 : fread_number( fp );
                          fread_to_eol( fp );
 
	if ( pReset->chance == 0 )   /* to allow for old files */
	    pReset->chance = 100;

        /*
         * Validate parameters.
         * We're calling the index functions for the side effect.
         */

        switch ( letter )
        {
        default:
            bug( "Load_resets: bad command '%c'.", letter );
            exit( 1 );
            break;
 
        case 'M':
            get_mob_index  ( pReset->arg1 );
            get_room_index ( pReset->arg3 );
            break;
 
        case 'O':
            get_room_index ( pReset->arg3 );
	    if (pReset->arg2 < 1 )
	    pReset->arg2 = 1; /* max to allow for older resets */
            break;
 
        case 'P':
            get_obj_index  ( pReset->arg3 );
	    if (pReset->arg2 < 1 )
	    pReset->arg2 = 1; /* max to allow for older resets */
            break;
 
        case 'G':
        case 'E':
            break;
 
        case 'D':
            pRoomIndex = get_room_index( pReset->arg1 );
 
            if ( pReset->arg2 < 0
            ||   pReset->arg2 > 5
            || ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )
            {
                bug( "Load_resets: 'D': exit %d not door.", pReset->arg2 );
                exit( 1 );
            }
            if ( !IS_SET( pexit->exit_info, EX_ISDOOR ) )
            {
                bug( "Load_resets: 'D': exit %d not door. ( Ok to set )", pReset->arg2 );
	        SET_BIT( pexit->exit_info, EX_ISDOOR );
            }
 
            if ( pReset->arg3 < 0 || pReset->arg3 > 2 )
            {
                bug( "Load_resets: 'D': bad 'locks': %d.", pReset->arg3 );
                exit( 1 );
            }
 
            break;
 
        case 'R':
            pRoomIndex          = get_room_index( pReset->arg1 );
 
            if ( pReset->arg2 < 0 || pReset->arg2 > 6 )
            {
                bug( "Load_resets: 'R': bad exit %d.", pReset->arg2 );
                exit( 1 );
            }
 
            break;
        }
 
        if ( area_last->reset_first == NULL )
            area_last->reset_first      = pReset;
        if ( area_last->reset_last  != NULL )
            area_last->reset_last->next = pReset;
            
        area_last->reset_last   = pReset;
        pReset->next            = NULL;
    }
   fclose(fp);
 
    return;
}

void load_event_data(AREA_DATA *pArea)
{
    FILE *fp; 
    char buf[MAX_INPUT_LENGTH];
    EVENT_DATA *event;
    ROOM_INDEX_DATA * room;
    char *word;
    bool tMatch, tDone;
    bool fMatch = TRUE;
    bool fDone = FALSE;

    sprintf(buf, "%s%s/events", SAVEAREAS_DIR, pArea->filename );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open areafile %s ", buf );
        bug( b_buf, 0 );
        return;
    }

    event = NULL;

    for ( ; fMatch && !fDone; )
    { fMatch = FALSE;
        tMatch = TRUE;
        tDone = FALSE;
        word   = feof( fp ) ? str_dup("#END") : fread_word( fp );

	switch ( UPPER( word[0] ) )
	{
	case '*': /* In-file comments */
            fread_to_eol( fp );
            fMatch = TRUE;
	    break;
	case '#':
            if ( !str_cmp( word, "#END" ) )
            {
                fMatch = TRUE;
		fDone = TRUE;
                break;
            }
            if ( !str_cmp( word, "#EVENT" ) )
            {
                fMatch          = TRUE;

                event = event_alloc();

    		for ( ; tMatch && !tDone; )
    		{
    		   word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    		   tMatch = FALSE;

    		   switch ( UPPER(word[0]) )
    		   {
    		      case '*': fread_to_eol( fp ); fMatch = TRUE; break;
    		      case 'C':
    		        KEY( "Chance",     event->chance, fread_number( fp ) );
    		       break;
    		      case 'F':
    		        KEY( "Flags",     event->flags, fread_number( fp ) );
    		       break;
    		      case 'E':
    		        if ( !str_cmp( word, "End" ) )
    		        {
    		           tMatch = TRUE;
    		           tDone = TRUE;
    		        }
    		       break;
    		      case 'M':
    		        KEY( "Mob_vnum",       event->mob_vnum, fread_number( fp ) );
		       break;
    		      case 'O':
    		        KEY( "Obj_vnum",       event->obj_vnum, fread_number( fp ) );
		       break;
    		      case 'R':
    		        KEY( "Room_vnum",       event->room_vnum, fread_number( fp ) );
		       break;
    		      case 'T':
    		        KEY( "Type",       event->type, fread_number( fp ) );
    		       break;
    		      case 'V':
    		        KEY( "Vnum",       event->vnum, fread_number( fp ) );
    		       break;
		    }
		}
		if ( ( room = get_room_index( event->room_vnum ) ) == NULL )
		{
		    bug( "Not valid vnum for event of room.\nEvent INVALID", event->room_vnum );
		    SET_BIT(event->flags, EVENT_INVALID);
		}
		if ( room )
		event->area = room->area;
		event->next = pArea->events;
		pArea->events = event;
    	    }
    	}
    }
    if ( !fMatch )
    {
        fpArea = fp;
        bug( "Load_event_data: bad key word.", 0 );
        exit( 1 );
    }
  return;
}
 
#if defined(KEY)
#undef KEY
 #endif
 
#define KEY( literal, field, value )                        \
            if ( !str_cmp( word, literal ) )                \
            {                                               \
                field = value;                              \
                fMatch = TRUE;                              \
                break;                                      \
            }
 
 
void load_obj_new( AREA_DATA *pArea )
{
    OBJ_INDEX_DATA *pObjIndex;
    FILE *fp;
    EXTRA_DESCR_DATA * pEdesc;
    LOCK_DATA * lock_list;
    AFFECT_DATA * pAff;
    AFFECT_DATA * af;
    AFFECT_DATA * pAf;
    AFFECT_DATA * af_next;
    int mod;
    bool changed;
    char *word;
    char buf[MAX_STRING_LENGTH];
    int iHash;
    int level = 1;
    int wear;
    int tempnum;
    bool fWear=FALSE;
    bool fMatch;
    bool fDone;
    bool fDoneAll = FALSE;

    sprintf(buf, "%s%s/objects", SAVEAREAS_DIR, pArea->filename );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open areafile %s ", buf );
        bug( b_buf, 0 );
        return;
    }
    pObjIndex = NULL;
    
    for ( ; ; )
    {
        /*
         * Well, once again, we should make the defaults as reasonable as
         * is humanly possibly.  In this case, it should be a bit easier,
         * though.
         */
        fDone = FALSE;       
        for ( ; ; )
        {
            word = fread_word( fp );
            fMatch = FALSE;
            switch (word[0])
            {
                case '$' :
                    if (!str_cmp(word, "$$"))
                    {
                        fDone = TRUE;
                        fMatch = TRUE;
			if ( pObjIndex->wear_data != NULL && pObjIndex->wear_data->wear_flags == 0 )
			{
			    free_wear_data(pObjIndex->wear_data );	
			    pObjIndex->wear_data = NULL;
			}
                        break;
                    }
                    if (!str_cmp(word, "$E"))
                    {   
                        fDoneAll = TRUE;
                        fDone = TRUE;
                        fMatch = TRUE;
                        break;
                    }
                    break;            
                case 'A' :
		    if ( !str_cmp( "AC", word ) )
		    {
			if ( pObjIndex->wear_data == NULL )
			     pObjIndex->wear_data = wear_alloc();
			pObjIndex->wear_data->ac = fread_number( fp );
			fMatch = TRUE;
			if (pObjIndex->wear_data->ac > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }
                    if (!str_cmp(word,"AFS"))
                    {   fMatch = TRUE;
			if ( pObjIndex->wear_data == NULL )
			{
			    fread_to_eol( fp );
			    break;
			}

                        pAff = new_affect(FALSE);
                        pAff->type = fread_number( fp );
                        pAff->level = fread_number( fp );
                        pAff->duration = fread_number( fp );
                        pAff->location = fread_number( fp );
			pAff->location2 = 0;
                        pAff->modifier = fread_number( fp );
                        pAff->bitvector = fread_number( fp );
                        pAff->next = pObjIndex->wear_data->affected;
	 		pObjIndex->wear_data->affected = pAff;
			level = pAff->level;
			if (pAff->type > 32000 ||
			pAff->level > 32000 ||
			pAff->duration > 32000 ||
			pAff->location > 32000 ||
			pAff->modifier > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
                        break;
                    }
                    if (!str_cmp(word,"AFF"))
                    {   fMatch = TRUE;
			if ( pObjIndex->wear_data == NULL )
			{
			    fread_to_eol( fp );
			    break;
			}

                        pAff = new_affect(FALSE);
                        pAff->type = fread_number( fp );
                        pAff->level = fread_number( fp );
                        pAff->duration = fread_number( fp );
                        pAff->location = fread_number( fp );
                        pAff->location2 = fread_number( fp );
                        pAff->modifier = fread_number( fp );
                        pAff->bitvector = fread_number( fp );
                        pAff->next = pObjIndex->wear_data->affected;
	 		pObjIndex->wear_data->affected = pAff;
		 	level = pAff->level;
			if (pAff->type > 32000 ||
			pAff->level > 32000 ||
			pAff->duration > 32000 ||
			pAff->location > 32000 ||
			pAff->modifier > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
                        break;
                    }
                    if (!str_cmp(word,"AFN"))
                    {   fMatch = TRUE;
			if ( pObjIndex->wear_data == NULL )
			{
			    fread_to_eol( fp );
			    break;
			}

                        pAff = new_affect(FALSE);
                        pAff->type = fread_number( fp );
                        pAff->level = fread_number( fp );
                        pAff->duration = fread_number( fp );
                        pAff->location = fread_number( fp );
                        pAff->location2 = fread_number( fp );
                        pAff->modifier = fread_number( fp );
                        pAff->bitvector = fread_number( fp );
                        pAff->bit_type = fread_number( fp );
                        pAff->next = pObjIndex->wear_data->affected;
	 		pObjIndex->wear_data->affected = pAff;
		 	level = pAff->level;
			if (pAff->type > 32000 ||
			pAff->level > 32000 ||
			pAff->duration > 32000 ||
			pAff->location > 32000 ||
			pAff->modifier > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
                        break;
                    }
		    if ( !str_cmp( "APPROVER", word ) )
		    {
                    	strncpy( pObjIndex->approve_data->approver, fread_string( fp ), 30 );
		    	fMatch = TRUE;
		    	break;
		    }
		    if ( !str_cmp( "APPROVE_DATA", word ) )
		    {
			pObjIndex->approve_data->flags = fread_number( fp );
			pObjIndex->approve_data->creation_date = fread_number( fp );
			pObjIndex->approve_data->approved_date = fread_number( fp );
			pObjIndex->approve_data->modified_date = fread_number( fp );
			fMatch = TRUE;
			break;
		    }
                    break;
		case 'B':
		    break;
                case 'C' :
		    if ( !str_cmp( word, "COND" ) )
		    {
			pObjIndex->condition = fread_number(fp);
			if ( pObjIndex->condition < 1 )
			pObjIndex->condition = 100;
			fMatch = TRUE;
			break;
		    }
                    KEY( "COUN", tempnum, fread_number( fp ) ); 
		    if ( !str_cmp( word, "CONT" ) )
		    {
			if ( pObjIndex->cont_data == NULL )
			    pObjIndex->cont_data = cont_alloc();

			pObjIndex->cont_data->capacity = fread_number(fp);
			pObjIndex->cont_data->flags = fread_number(fp);
			fMatch = TRUE;
			if (pObjIndex->cont_data->flags > 32000 ||
			pObjIndex->cont_data->capacity > 32000 ) 
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }

		    if ( !str_cmp( "COST", word ) )
		    {
			if ( pObjIndex->wear_data == NULL )
			    fread_number( fp );
     			else
			    pObjIndex->wear_data->cost = fread_number( fp );
			fMatch = TRUE;
			break;
		    }
		    if ( !str_cmp( "CREATER", word ) )
		    {
                    	strncpy( pObjIndex->approve_data->creater, fread_string( fp ), 30 );
			fMatch = TRUE;
			break;
		    }
                    break;
                case 'D' :
		    if ( !str_cmp( word, "DESC" ) )
		    {
                    	strncpy( pObjIndex->description, fread_string( fp ), 80 );
			fMatch = TRUE;
			break;
		    }
                    break;
                case 'E' :
                    KEY( "EXFL", pObjIndex->extra_flags, fread_number( fp ) );
                    if (!str_cmp(word, "EDESC"))
                    {
                        char *tempbuf;
			fMatch = TRUE;
                        pEdesc = new_extra_descr();
                        strncpy( pEdesc->keyword, fread_string( fp ), 30 );
                        tempbuf = fread_string(fp );
                        pEdesc->description = str_dup(tempbuf+1);
                        free_string(tempbuf);
                        pEdesc->next = pObjIndex->extra_descr;
                        pObjIndex->extra_descr = pEdesc;
                        break;
                    }
		    if ( !str_cmp( word, "EDIBLE" ) )
		    {
			if ( pObjIndex->edible_data == NULL )
			    pObjIndex->edible_data = edible_alloc();

			pObjIndex->edible_data->type  = fread_number(fp);
			pObjIndex->edible_data->flags = fread_number(fp);
			pObjIndex->edible_data->amount = fread_number(fp);
			pObjIndex->edible_data->max_amount = fread_number(fp);
			if ( pObjIndex->edible_data->max_amount == 0 )
			    pObjIndex->edible_data->max_amount = 1;
			fMatch = TRUE;
			if (pObjIndex->edible_data->type > 32000 ||
			pObjIndex->edible_data->amount > 32000 ||
			pObjIndex->edible_data->max_amount > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }
		    if ( !str_cmp( "ESPELL", word ) )
		    {
			SPELL_LIST* spell;
                        char *tempbuf;

			if ( pObjIndex->edible_data == NULL )
			    pObjIndex->edible_data = edible_alloc();
			tempnum = fread_number(fp);
			tempbuf = fread_string(fp);
			if (skill_lookup(tempbuf) != -1 )
		        {
			spell = spell_alloc();
			spell->level = tempnum;
			spell->spell = skill_lookup(tempbuf);
			spell->next = pObjIndex->edible_data->spells;
			pObjIndex->edible_data->spells = spell;
			}
			fMatch = TRUE;
			break;
		    }
                    break;
		case 'G' :
		    if ( !str_cmp( word, "GATE" ) )
		    {
			if ( pObjIndex->gate_data == NULL )
			    pObjIndex->gate_data = gate_alloc();

			pObjIndex->gate_data->vnum = fread_number( fp );
			pObjIndex->gate_data->flags = fread_number( fp );
			pObjIndex->gate_data->ave_timer = fread_number( fp );

			fMatch = TRUE;
			if (pObjIndex->gate_data->vnum > 32000 || 
			pObjIndex->gate_data->ave_timer > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }
		    break;
                case 'I' :
		    if ( !str_cmp( word, "INSIDE" ) )
		    {
			RESET_DATA * reset;
			RESET_DATA * lReset;
			char cmd;

			if ( pObjIndex->cont_data == NULL )
			    pObjIndex->cont_data = cont_alloc();

			reset = reset_alloc();

			for ( lReset = pObjIndex->cont_data->inside ; lReset != NULL && lReset->next != NULL ; lReset = lReset->next );

			do
			{
			   cmd = getc( fp );
			}
			while( isspace( cmd ) );

			reset->command = cmd;
			reset->arg1 = fread_number( fp );
			reset->arg2 = fread_number( fp );
			reset->arg3 = fread_number( fp );
			reset->vnum = fread_number( fp );
			reset->chance = fread_number( fp );
			reset->next = NULL;

			if ( lReset == NULL )
			    pObjIndex->cont_data->inside = reset;
			else
			    lReset->next = reset;

			fMatch = TRUE;
			break;
		    }
                    if (!str_cmp(word, "INSIDE_OBJ"))
                    {
			INSIDE_DATA * inside;
		
			inside = inside_alloc();
			inside->obj_vnum = fread_number(fp);
			inside->mob_vnum = fread_number(fp);
			inside->chance = fread_number(fp);
			inside->flags = fread_number(fp);
			inside->next = pObjIndex->inside;
			pObjIndex->inside = inside;
                        fMatch = TRUE;        
                        break;
		    }
		    if ( !str_cmp( word, "ITTY" ) ) 
		    {
			int value;
			value = fread_number( fp );
			SET_BIT( pObjIndex->shop_type, get_shop_type( value ) );
			fMatch = TRUE;
			break;
		    }
                    break;
                case 'L' :
		    if ( !str_cmp( word, "LIGHT" ) )
		    {
			if ( pObjIndex->light_data == NULL )
			    pObjIndex->light_data = light_alloc();

			pObjIndex->light_data->flags = fread_number( fp );
			pObjIndex->light_data->fuel = fread_number( fp );
			pObjIndex->light_data->max_fuel = fread_number( fp );
			pObjIndex->light_data->brightness = fread_number( fp );
			if (pObjIndex->light_data->brightness < 0 )
        		    bug( "Check obj: Vnum %d Light < 0.", pObjIndex->vnum );
			fMatch = TRUE;
			break;
		    }
		    if ( !str_cmp( word, "LOCKS" ) )
		    {
			lock_list = lock_alloc();
			lock_list->vnum = fread_number( fp );
			lock_list->lock_info = fread_number( fp );
			lock_list->pick_penalty = fread_number( fp );
			if ( pObjIndex->cont_data == NULL )
			    pObjIndex->cont_data = cont_alloc();

		        lock_list->next = pObjIndex->cont_data->lock;
		        pObjIndex->cont_data->lock = lock_list;

			fMatch = TRUE;
			break;
		    }
                    break;
                case 'M' :
                    KEY( "MATE", pObjIndex->material, fread_number( fp ) );
		    if ( !str_cmp( "MODIFIER", word ) )
		    {
                    	strncpy( pObjIndex->approve_data->modifier, fread_string( fp ), 30 );
			fMatch = TRUE;
			break;
		    }
		    if ( !str_cmp( "MAGICAL", word ) )
		    {
			if ( pObjIndex->wear_data == NULL )
			     pObjIndex->wear_data = wear_alloc();
			if ( pObjIndex->wear_data->magic_data == NULL )
			     pObjIndex->wear_data->magic_data = magic_alloc();
			pObjIndex->wear_data->magic_data->type = fread_number( fp );
			pObjIndex->wear_data->magic_data->flags = fread_number( fp );
			pObjIndex->wear_data->magic_data->charges = fread_number( fp );
			pObjIndex->wear_data->magic_data->max_charges = fread_number( fp );
			fMatch = TRUE;
			if (pObjIndex->wear_data->magic_data->type > 32000 ||
			pObjIndex->wear_data->magic_data->charges > 32000 ||
			pObjIndex->wear_data->magic_data->max_charges > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }
                    break;
                case 'N' :
		    if ( !str_cmp( "NAME", word ) )
		    {
			/* Name must be first */
        		pObjIndex = new_obj_index();
			pObjIndex->in_area = area_last;
			strncpy( pObjIndex->name, fread_string(fp), 30 );
			fMatch = TRUE;
                        break;
		    }
                    break;
		case 'P' :
		    break;
                case 'R' :
                    KEY( "RNUM", tempnum, fread_number( fp ) );
                    break;
                case 'S' :
                    KEY( "SPFL", pObjIndex->special_flags, fread_number( fp ) );
		    if ( !str_cmp( "SLASH", word ) )
		    {
			if ( pObjIndex->wear_data == NULL )
			     pObjIndex->wear_data = wear_alloc();
			pObjIndex->wear_data->ac = fread_number( fp );
			fMatch = TRUE;
			if (pObjIndex->wear_data->ac > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }
		    if ( !str_cmp( "SPELL", word ) )
		    {
			SPELL_LIST* spell;
                        char *tempbuf;

			if ( pObjIndex->wear_data == NULL )
			     pObjIndex->wear_data = wear_alloc();
			if ( pObjIndex->wear_data->magic_data == NULL )
			     pObjIndex->wear_data->magic_data = magic_alloc();

			tempnum = fread_number(fp);
			tempbuf = fread_string(fp);
			if (skill_lookup(tempbuf) != -1 )
		        {
			spell = spell_alloc();
			spell->level = tempnum;
			spell->spell = skill_lookup(tempbuf);
			spell->next = pObjIndex->wear_data->magic_data->spells;
			pObjIndex->wear_data->magic_data->spells = spell;
			}
			fMatch = TRUE;
			break;
		    }
		    if ( !str_cmp( "SIZE", word ) )
		    {
			if ( pObjIndex->wear_data == NULL )
			     pObjIndex->wear_data = wear_alloc();
			     pObjIndex->wear_data->size = fread_number( fp );
			fMatch = TRUE;
			if (pObjIndex->wear_data->size > SIZE_GIANT )
        		    bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }
		    if ( !str_cmp( "SHDE", word ) )
		    {
                    	strncpy( pObjIndex->short_descr, fread_string( fp ), 80 );
			fMatch = TRUE;
			break;
		    }
		    KEY( "SHOP", pObjIndex->shop_type, fread_number( fp ) );
                    break;
		case 'T' :
                    KEY( "TIMER", pObjIndex->timer, fread_number( fp ) );
 		    if (!str_cmp( word, "TRAP" ) )
		    {
			if ( pObjIndex->cont_data == NULL )
			    pObjIndex->cont_data = cont_alloc();
			if ( pObjIndex->cont_data->trap == NULL )
			    pObjIndex->cont_data->trap = trap_alloc();

			pObjIndex->cont_data->trap->flags = fread_number( fp );
			fMatch = TRUE;
			break;
		    }
                    if (!str_cmp(word, "TRIG_LIST"))
                    {
			TRIGGER_LIST_DATA * trigger_list_data;
		
			trigger_list_data = trigger_list_alloc();
			trigger_list_data->vnum = fread_number(fp);
			trigger_list_data->next = pObjIndex->trigger_list;
			pObjIndex->trigger_list = trigger_list_data;
                        fMatch = TRUE;        
                        break;
		    }
		    if (!str_cmp(word, "TSPELL" ) )
		    {
			SPELL_LIST* spell;
			SPELL_LIST* spell2;
                        char *tempbuf;

			if ( pObjIndex->cont_data == NULL )
			    pObjIndex->cont_data = cont_alloc();
			if ( pObjIndex->cont_data->trap == NULL )
			    pObjIndex->cont_data->trap = trap_alloc();

			for ( spell2 = pObjIndex->cont_data->trap->spells ; spell2 != NULL && spell2->next != NULL ; spell2 = spell2->next );

			tempnum = fread_number(fp);
			tempbuf = fread_string(fp);
			if (skill_lookup(tempbuf) != -1 )
		        {
			spell = spell_alloc();
			spell->level = tempnum;
			spell->spell = skill_lookup(tempbuf);
			spell->next = NULL;
			if ( spell2 == NULL )
			    pObjIndex->cont_data->trap->spells = spell;
			else
			    spell2->next = spell;
			fMatch = TRUE;
			}
			break;
		    }
		    break;
                case 'V' :
                    if (!str_cmp(word, "VALs") )
                    {
			fread_number( fp );
			fread_number( fp );
			fread_number( fp );
			fread_number( fp );
			fread_number( fp );
                        fMatch = TRUE;
                        break;
                    }
                    KEY( "VNUM", pObjIndex->vnum, fread_number( fp ) );
                    break;
                case 'W' :
		    if ( !str_cmp( "WEIG", word ) )
		    {
			if ( pObjIndex->wear_data == NULL )
			     pObjIndex->wear_data = wear_alloc();
			     pObjIndex->wear_data->weight = fread_number( fp );
			fMatch = TRUE;
			if (pObjIndex->wear_data->weight > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
			break;
		    }
		    if ( !str_cmp( word, "WRFL" ) )
		    {
			fMatch = TRUE;
			wear = fread_number( fp );
			fWear = TRUE;
			if ( wear == 0 )
			    break;
			else if ( pObjIndex->wear_data == NULL )
			     pObjIndex->wear_data = wear_alloc();
            		pObjIndex->wear_data->wear_flags       = wear;
			break;
		   }
		if ( !str_cmp( "WTYPE", word ) )
		{
		    if ( pObjIndex->wear_data == NULL )
			pObjIndex->wear_data = wear_alloc();
		    if ( pObjIndex->wear_data->weapon_data == NULL )
			pObjIndex->wear_data->weapon_data = weapon_alloc();

		    pObjIndex->wear_data->weapon_data->type = fread_number( fp );
		    fMatch = TRUE;
			if (pObjIndex->wear_data->weapon_data->type > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
		    break;
		}
		if ( !str_cmp( "WFLAGS", word ) )
		{
		    if ( pObjIndex->wear_data == NULL )
			pObjIndex->wear_data = wear_alloc();
		    if ( pObjIndex->wear_data->weapon_data == NULL )
			pObjIndex->wear_data->weapon_data = weapon_alloc();

		    pObjIndex->wear_data->weapon_data->flags = fread_number( fp );
		    fMatch = TRUE;
		    break;
		}
		if ( !str_cmp( "WDT", word ) )
		{
		    if ( pObjIndex->wear_data == NULL )
			pObjIndex->wear_data = wear_alloc();
		    if ( pObjIndex->wear_data->weapon_data == NULL )
			pObjIndex->wear_data->weapon_data = weapon_alloc();

		    pObjIndex->wear_data->weapon_data->dam_type = fread_number( fp );
			if (pObjIndex->wear_data->weapon_data->dam_type > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
		    fMatch = TRUE;
		    break;
		}
		if ( !str_cmp( "WDAMAGE", word ) )
		{
		    if ( pObjIndex->wear_data == NULL )
			pObjIndex->wear_data = wear_alloc();
		    if ( pObjIndex->wear_data->weapon_data == NULL )
			pObjIndex->wear_data->weapon_data = weapon_alloc();

		    pObjIndex->wear_data->weapon_data->dice = fread_number( fp );
		    pObjIndex->wear_data->weapon_data->die  = fread_number( fp );
			if (pObjIndex->wear_data->weapon_data->dice > 32000 ||
			pObjIndex->wear_data->weapon_data->die > 32000 )
        		bug( "Check obj: Vnum %d.", pObjIndex->vnum );
		    fMatch = TRUE;
		    break;
		}

            }
            if ( !fMatch )
            {            
                sprintf(buf, "load obj No match for : %s", word);
                log_string(buf);
                fread_to_eol( fp );
            }
            if ( fDoneAll )
            {
		if( fp )
    		fclose(fp);
                return;
            }
            if ( fDone )
            {
                break;
            }
        }

        if (pObjIndex->vnum != 0)
        {
            iHash                   = pObjIndex->vnum % MAX_KEY_HASH;
            pObjIndex->next              = obj_index_hash[iHash];
            obj_index_hash[iHash]   = pObjIndex;
            free_obj_list[pObjIndex->vnum] = 1;
    	    set_obj_index_points(pObjIndex);
  	/*  sprintf(buf, "Loading Object : %s  ->%d", pObjIndex->name, pObjIndex->vnum);
    	  log_string(buf);
  	*/      }
        else 
	    bug ("ERROR!!! ERROR!!! ITEM WTH NO VNUM<!!!!!!\n\r",0);

	if ( pObjIndex->wear_data != NULL )
	{
	    af = pObjIndex->wear_data->affected;
            if ( af != NULL && ( af->location == APPLY_SAVING_PARA || af->location == APPLY_SAVING_ROD || af->location == APPLY_SAVING_PETRI || af->location == APPLY_SAVING_BREATH ) )
            {
	    	mod = af->modifier;
            	pObjIndex->wear_data->affected = af->next;
            	free_affect(af);
		pAf	= new_affect(FALSE);
		pAf->location = APPLY_SAVING_SPELL;
		pAf->modifier = mod;
		pAf->bitvector = 0;
		pAf->duration = -1;
		pAf->type = -1;
		pAf->next = pObjIndex->wear_data->affected;
		pObjIndex->wear_data->affected = pAf;
        	if ( !IS_SET(pObjIndex->extra_flags, ITEM_MAGIC) )
            	    SET_BIT(pObjIndex->extra_flags, ITEM_MAGIC);
                bug( "changed apply: %ld.", pObjIndex->vnum );
            }
	    else
	    {
		for ( af = pObjIndex->wear_data->affected; af != NULL; af = af_next )
		{
	    	    changed = FALSE;
	    	    af_next = af->next;
	    	    mod = af->modifier;
		    if ( af_next )
	    	    switch ( af->location )
	    	    {
			case APPLY_SAVING_PARA:
		    	    af->next = af_next->next;
		    	    free_affect( af_next );
		    	    changed = TRUE;
		    	    break;
			case APPLY_SAVING_ROD:
		    	    af->next = af_next->next;
		    	    free_affect( af_next );
		    	    changed = TRUE;
		    	    break;
			case APPLY_SAVING_PETRI:
		    	    af->next = af_next->next;
		    	    free_affect( af_next );
		    	    changed = TRUE;
		    	    break;
			case APPLY_SAVING_BREATH:
		    	    af->next = af_next->next;
		    	    free_affect( af_next );
		    	    changed = TRUE;
		    	    break;
			default:
		    	    break;
	    	    }

	    	    if ( changed == TRUE )
	    	    {
			pAf	= new_affect(FALSE);
			pAf->location = APPLY_SAVING_SPELL;
			pAf->modifier = mod;
			pAf->bitvector = 0;
			pAf->duration = -1;
			pAf->type = -1;
			pAf->next = pObjIndex->wear_data->affected;
			pObjIndex->wear_data->affected = pAf;
        		if ( !IS_SET(pObjIndex->extra_flags, ITEM_MAGIC) )
            	    	    SET_BIT(pObjIndex->extra_flags, ITEM_MAGIC);
                	bug( "changed apply: %ld.", pObjIndex->vnum );
	    	    }
		}
	    }
	}	
    }
}
 
/*
 * Load mobiles in new style
 */                
void load_mob_new( AREA_DATA *pArea )
{
    MOB_INDEX_DATA *pMobIndex;
    SKILL_LIST * skills;
    FILE *fp;
    char *word;
    char buf[MAX_STRING_LENGTH];
    int iHash,i,x,count;
    bool fMatch;
    bool fDone;
    bool fDoneAll = FALSE;
 
    sprintf(buf, "%s%s/mobiles", SAVEAREAS_DIR, pArea->filename );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open areafile %s ", buf );
        bug( b_buf, 0 );
        return;
    }

    pMobIndex = NULL;
    for ( ; ; )
    {
        /*
         * Well, once again, we should make the defaults as reasonable as
         * is humanly possibly.  In this case, it should be a bit easier,
         * though.
         */

        fDone = FALSE;
        
        for ( ; ; )
        {
            word = fread_word( fp );
            fMatch = FALSE;
            switch (word[0])
            {
                case '$' :
                    if (!str_cmp(word, "$$"))
                    {
                        fDone = TRUE;
                        fMatch = TRUE;
                        break;
                    }
                    if (!str_cmp(word, "$E"))
                    {
                        fDoneAll = TRUE;
                        fDone = TRUE;
                        fMatch = TRUE;
                         
			break;
                    }
                    break;            
                case 'A' :                 
		    KEY( "ARMOR", pMobIndex->armor, fread_number(fp));
                    if (!str_cmp(word, "AC"))
                    {
                        fread_number(fp);
                        pMobIndex->armor = fread_number(fp); 
                        fread_number(fp); 
                        fread_number(fp); 
                        fMatch = TRUE;        
                        break;
                    }               
		    if ( !str_cmp( "ACT", word ) )
		    {
			pMobIndex->act = fread_number( fp );

			fMatch = TRUE;
			break;
		    }
		    if ( !str_cmp( "ACT2", word ) )
		    {
			pMobIndex->act2 = fread_number( fp );

			fMatch = TRUE;
			break;
		    }
                    KEY("AFF",pMobIndex->affected_by, fread_number(fp) );
                    KEY("AFF2",pMobIndex->affected_by_2, fread_number(fp) );
                    if (!str_cmp(word, "AREA_SR"))
                    {
			if ( pMobIndex->moveable == NULL )
			{
			    pMobIndex->moveable = moveable_alloc();
			}
			pMobIndex->moveable->area_start_room = fread_number(fp);
                        fMatch = TRUE;        
                        break;
                    }
                    break;
		case 'B' :
                    if (!str_cmp(word, "B_MSG"))
                    {
			if ( pMobIndex->moveable == NULL )
			{
			    pMobIndex->moveable = moveable_alloc();
			}
			strncpy( pMobIndex->moveable->board_msg, fread_string(fp), 15 );
                        fMatch = TRUE;        
                        break;
                    }
                    break;
                case 'C' :
		    /* castle code */
		    if ( !str_cmp( word, "CASTLE" ) )
		    {
			pMobIndex->castle_area = fread_number( fp );
			fMatch = TRUE;
			break;
		    }

                    if (!str_cmp(word, "COINS"))
                    {
                        pMobIndex->gold[0] = fread_number(fp); 
                        pMobIndex->gold[1] = fread_number(fp); 
                        pMobIndex->gold[2] = fread_number(fp); 
                        pMobIndex->gold[3] = fread_number(fp); 
                        fMatch = TRUE;        
                        break;
                    }
                case 'D' :
                    if (!str_cmp(word, "DAM"))
                    {
                        pMobIndex->damage[0] = fread_number(fp); 
                        pMobIndex->damage[1] = fread_number(fp); 
                        pMobIndex->damage[2] = fread_number(fp); 
                        fMatch = TRUE;        
                        break;
                    }
                    KEY("DAMT",pMobIndex->dam_type, fread_number(fp) );
                    KEY("DESC",pMobIndex->description, fread_string(fp) );
                    KEY("DPOS",pMobIndex->default_pos, fread_number(fp) );
                    break;
                case 'E' :
                    if (!str_cmp(word, "EQUIP"))
                    {
			EQUIP_DATA * equip;
		
			equip = equip_alloc();
			equip->obj_vnum = fread_number(fp);
			equip->chance = fread_number(fp);
			equip->flags = fread_number(fp);
			equip->next = pMobIndex->equip;
			pMobIndex->equip = equip;
                        fMatch = TRUE;        
                        break;
		    }
                    break;
                case 'F' :
                    KEY("FORM",pMobIndex->form, fread_number(fp) );
                    KEY("FOLL",pMobIndex->follower, fread_number(fp) );
                    break;
		case 'G' :
		    KEY( "GUILDS",pMobIndex->guilds, fread_number( fp ) );
		    break;
                case 'H' :
                    if (!str_cmp(word, "HIT"))
                    {                
                        pMobIndex->hit[0] = fread_number(fp); 
                        pMobIndex->hit[1] = fread_number(fp); 
                        pMobIndex->hit[2] = fread_number(fp); 
                        fMatch = TRUE;        
                        break;
                    }
                    KEY("HEIGHT",pMobIndex->height, fread_number(fp) );
                    KEY("HITR",pMobIndex->hitroll, fread_number(fp) );
                    break;
                case 'I' :
                    KEY("IMMFL",pMobIndex->imm_flags, fread_number(fp) );
                    break;
                case 'L' :
		    if (!str_cmp(word, "L_PRAC"))
		    {
			int value;

			value = fread_number( fp );
                        pMobIndex->prac_skills = UMAX( pMobIndex->level, value * 4 / 3 ); 
                        pMobIndex->prac_spells = fread_number(fp) * 4 / 3; 
                        fMatch = TRUE;        
                        break;
		    }
		    if (!str_cmp(word, "L_PRACS"))
		    {
                        pMobIndex->prac_skills = fread_number(fp); 
                        pMobIndex->prac_spells = fread_number(fp); 
                        fMatch = TRUE;        
                        break;
		    }
		    if (!str_cmp(word, "L_SKILLS"))
		    {
			int sn;

			if ( ( sn = skill_lookup( fread_string( fp ) ) ) > 1 )
			{
			    skills = skill_list_alloc();
			    skills->sn = sn;
			    skills->next = pMobIndex->innate_skills;
			    pMobIndex->innate_skills = skills;
			}
                        fMatch = TRUE;        
                        break;
		    }
                    if (!str_cmp(word, "L_MSG"))
                    {
			if ( pMobIndex->moveable == NULL )
			{
			    pMobIndex->moveable = moveable_alloc();
			}
			strncpy( pMobIndex->moveable->leave_msg, fread_string(fp), 15 );
                        fMatch = TRUE;        
                        break;
                    }
		    if ( !str_cmp( word, "LDESC" ) )
		    {
                    	strncpy( pMobIndex->long_descr, fread_string(fp), 80 );
			fMatch = TRUE;
			break;
		    }
                    KEY("LEVEL",pMobIndex->level, fread_number(fp) );
                    break;
                case 'M' :
                    if (!str_cmp(word, "MAN"))
                    {
			int type;
			type = fread_number(fp);
                        pMobIndex->mana[0][type] = fread_number(fp); 
                        pMobIndex->mana[1][type] = fread_number(fp); 
                        pMobIndex->mana[2][type] = fread_number(fp); 
                        fMatch = TRUE;        
                        break;
                    }
                    if (!str_cmp(word, "M_MSG"))
                    {
			if ( pMobIndex->moveable == NULL )
			{
			    pMobIndex->moveable = moveable_alloc();
			}
			strncpy( pMobIndex->moveable->move_msg, fread_string(fp), 15 );
                        fMatch = TRUE;        
                        break;
                    }
                    if (!str_cmp(word, "MOVE_MSG"))
                    {
			strncpy( pMobIndex->movement_msg, fread_string(fp), 15 );
                        fMatch = TRUE;        
                        break;
                    }
                    KEY("MTRL",pMobIndex->material, fread_number(fp) );
                    KEY("MOOD",pMobIndex->mood, fread_number(fp) );
                    break;
                case 'O' :
                    KEY("OFFFL",pMobIndex->off_flags, fread_number(fp) );
                    break;
                case 'P' :
                    if ( !str_cmp( word, "PNAM" ) )
                    {
                        strncpy( pMobIndex->player_name, fread_string(fp), 30 );
                        fMatch = TRUE;
                        break;
                    }
                    break;
                case 'R' :
                    KEY("RACE",pMobIndex->race, fread_number(fp) );
                    KEY("RESFL",pMobIndex->res_flags, fread_number(fp) );
                    break;
                case 'S' :
		    if (!str_cmp(word,"SPEC_FUN"))
		    {
			strncpy( pMobIndex->spec_name, fread_string(fp), 30 );
			pMobIndex->spec_fun = spec_lookup(pMobIndex->spec_name);
                        fMatch = TRUE;        
                        break;
		    }
                    if (!str_cmp(word, "SKILL"))
                    {
			if ( pMobIndex->gainer_data == NULL )
			{
			    pMobIndex->gainer_data = gainer_data_alloc();
			    for ( i = 0 ; i < MAX_SKILL ; i++ ) pMobIndex->gainer_data->skill[i] = FALSE;
			}
			i = skill_lookup(fread_string(fp));
			if ( i > 0 )
			    pMobIndex->gainer_data->skill[ i ] = TRUE;
                        fMatch = TRUE;        
                        break;
                    }
                    if (!str_cmp(word, "Scriptfile"))
		    {
			fread_to_eol(fp);
                        fMatch = TRUE;        
                        break;
		    }
		    if ( !str_cmp( word, "SDESC" ) )
		    {
                    	strncpy( pMobIndex->short_descr, fread_string(fp), 80 );
			fMatch = TRUE;
			break;
		    }
                    KEY("SEX",pMobIndex->sex, fread_number(fp) );
                    KEY("SIZE",pMobIndex->size, fread_number(fp) );
                    KEY("SPOS",pMobIndex->start_pos, fread_number(fp) ); 
                    break;
                case 'T' :
                    if (!str_cmp(word, "TRIG_LIST"))
                    {
			TRIGGER_LIST_DATA * trigger_list_data;
		
			trigger_list_data = trigger_list_alloc();
			trigger_list_data->vnum = fread_number(fp);
			trigger_list_data->next = pMobIndex->trigger_list;
			pMobIndex->trigger_list = trigger_list_data;
                        fMatch = TRUE;        
                        break;
		    }
                    break;
                case 'V' :
                    if (!str_cmp(word, "VNUM"))
		    {
			/* Vnum must be first */
        		pMobIndex = alloc_pMobIndex();
        		pMobIndex->load_with_area = area_last;
        		pMobIndex->vnum = fread_number(fp);
                        fMatch = TRUE;        
                        break;
		    }
                    KEY("VULFL",pMobIndex->vuln_flags, fread_number(fp) );
                    if (!str_cmp(word, "VALID_SEC"))
                    {
			for (x = 0; x < MAX_SECTORS -1; x++ )
			pMobIndex->valid_sector[x] = fread_number(fp);
			fread_to_eol(fp); 
                        fMatch = TRUE;        
                        break;
                    }
                    if (!str_cmp(word, "VAL_SEC"))
                    {
			fread_to_eol(fp);
                        fMatch = TRUE;        
                        break;
                    }
                    break;
                case 'W' :
                    KEY("WEIGHT",pMobIndex->weight, fread_number(fp) );
                case '>' :
		    fread_to_eol(fp);
	            break;
            }
            if ( !fMatch )
            {            
                sprintf(buf, "load mob No match for : %s", word);
                log_string(buf);
                fread_to_eol( fp );
            }

            if ( fDoneAll )
            {
		if( fp )
    		fclose(fp);
                return;
            }
            if ( fDone )
            {
                break;
            }
        }
	for ( count = 0; count < MAX_ELEMENT_TYPE; count++)
	{
	    pMobIndex->mana[2][count] = pMobIndex->level * 50;
	}	
    skills = skill_list_alloc();
    skills->sn = gsn_common;
    skills->next = pMobIndex->innate_skills;
    pMobIndex->innate_skills = skills;

    if (pMobIndex->sex == 4 ) /* no more either to much problems */
        pMobIndex->sex = number_range(SEX_MALE,SEX_FEMALE);

        if (pMobIndex->vnum != 0)
        {
	    if (pMobIndex->sex == 0 )
		pMobIndex->sex = 1; 
            iHash                   = pMobIndex->vnum % MAX_KEY_HASH;
            pMobIndex->next         = mob_index_hash[iHash];
            mob_index_hash[iHash]   = pMobIndex;
	    free_mob_list[pMobIndex->vnum] = 1;
	    REMOVE_BIT(pMobIndex->affected_by, AFF_NEW_MOB);
            kill_table[URANGE(0, pMobIndex->level, MAX_LEVEL-1)].number++;

 /*   sprintf(buf, "Loading Mobile : %s  ->%d", pMobIndex->short_descr, pMobIndex->vnum);
    log_string(buf);
 */       }
        else
        {
        }
    }
}
 
 
#if defined(KEY)
#undef KEY
#endif
 
#define KEY( literal, field, value )                        \
            if ( !str_cmp( word, literal ) )                \
            {                                               \
                field = value;                              \
                fMatch = TRUE;                              \
                break;                                      \
            }
 
void load_rooms_new( AREA_DATA *pArea )
{
    EXTRA_DESCR_DATA *pEdesc;
    FILE *fp;
    char buf[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    EXIT_DATA *pExitData;
    LOCK_DATA *lock_list;
    int exit_num;
    char *word;
    bool fMatch, fDone, fDoneRec, fDoneAll;
    int iHash;
    int count;
    fDoneAll = FALSE; 

    sprintf(buf, "%s%s/rooms", SAVEAREAS_DIR, pArea->filename );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open areafile %s ", buf );
        bug( b_buf, 0 );
        return;
    }
    pRoomIndex = NULL; 
    pExitData = NULL;
    for ( ;!fDoneAll ; )
    {
        fDone = FALSE;
 
        for ( ;!fDone; )
        {
            fMatch = FALSE;
            word = fread_word(fp);
            switch(word[0])
            {
                case '$' :
                    if (!str_cmp(word, "$!"))
                    {
                        fDone = TRUE;
                        fMatch = TRUE;
                        break;
                    }
                    if (!str_cmp(word, "$E"))
                    {
                        fDone = TRUE;
                        fMatch = TRUE;
                        fDoneAll = TRUE;
                        break;
                    }
                    
                    break;  
                case 'D' : 
                    KEY( "DESC", pRoomIndex->description, fread_string(fp));
                    break; 
                case 'E' :
                    if (!str_cmp(word, "EDESC"))
                    {
                        char * tempbuff;
                        pEdesc = new_extra_descr();
                        strncpy( pEdesc->keyword, fread_string(fp), 30 );
                        tempbuff = fread_string(fp);
                        pEdesc->description = str_dup(tempbuff + 1);
                        free_string(tempbuff);
                        pEdesc->next = pRoomIndex->extra_descr;
                        pRoomIndex->extra_descr = pEdesc;
                        fMatch = TRUE;
                        break;
                    }
                    break;
                case 'F' :
                    KEY( "FLAG", pRoomIndex->room_flags, fread_number(fp));
                    KEY( "FLAG_2", pRoomIndex->room_flags_2, fread_number(fp));
                    break;
		case 'G' :
		    KEY( "Guilds", pRoomIndex->guilds, fread_number( fp ) );
                case 'L' :
                    KEY( "LIGH", pRoomIndex->light, fread_number(fp));
                    break;
                case 'M' :
		    if (!str_cmp(word,"Movemess"))
		    	{
			  fread_string(fp);
			  fMatch = TRUE;
			}
		    if (!str_cmp(word,"Move_dir"))
		    	{
			  fread_number(fp);
			  fMatch = TRUE;
			}
                    break;
                case 'N' :
		        if (!str_cmp(word,"NAME"))
			{
			/* Name must be first */
        		pRoomIndex =  pRoomIndex_alloc();
        		pRoomIndex->area = area_last;
			strncpy( pRoomIndex->name, fread_string(fp), 80 );
			fMatch = TRUE;
                        break;
			}
                case 'S' :
                    KEY( "SEC", pRoomIndex->sector_type, fread_number(fp));
                    KEY( "SIZE", pRoomIndex->size, fread_number(fp));
		    if (!str_cmp(word,"SPN"))
			{ SPECPRO_LIST *s;
			  int	i;
			  i = fread_number(fp);
			  s = specpro_list_alloc();
			  s->next = pRoomIndex->spro;
			  pRoomIndex->spro = s;
			  s->sp = spec_pro_list+i;
                          s->num = s->sp->spn; 
			  fMatch = TRUE;
			 }
                    break; 
                case 'T' :
                    if (!str_cmp(word, "TRIG_LIST"))
                    {
			TRIGGER_LIST_DATA * trigger_list_data;
		
			trigger_list_data = trigger_list_alloc();
			trigger_list_data->vnum = fread_number(fp);
			trigger_list_data->next = pRoomIndex->trigger_list;
			pRoomIndex->trigger_list = trigger_list_data;
                        fMatch = TRUE;        
                        break;
		    }
                    break;
                case 'V' :
                    KEY( "VNUM", pRoomIndex->vnum, fread_number(fp));
                    break;
            }
            
            if ( !fMatch )
            {            
                bug( "load_rooms_new : no match.", 0 );
                bug(word,0);
		fread_to_eol( fp );
            }
           /* 
            if ( fDone )
            {
                break;
            }*/
        }
        if (fDoneAll)
        {
	    if( fp )
	    fclose(fp);
	    return;
        }
        if (pRoomIndex->vnum == 0)
        {
            bug ( "load_rooms_new: No vnum given for room", 0 );
            /* Read until the next $$ section divider */
            for ( ; ; )
            {        
                if ( str_cmp(fread_word(fp), "$$") )
                    fread_to_eol(fp);
                else
                    break;
            }
        }
        else
        {
            fDoneRec=FALSE;
            for (count = 0; count < 6; count++)
            {
                pRoomIndex->exit[count] = NULL;
            }
 
            for ( ;!fDoneRec; )
            {         
                /* I choose six because zero is a valid possibility */
                exit_num = 6;
                fDone = FALSE;
                for ( ;!fDone; )
                { 
                    fMatch = FALSE;
                    word = fread_word(fp);
		    if (word[0] == '$' && word[1] == '$')
                        {
                          fMatch = TRUE;
                          fDone = TRUE;
                          fDoneRec = TRUE;
                          goto tempbugfix;
                         }
  
                    switch (word[0])
                    {
                        case '$':
                            if (!str_cmp(word, "$@") )
                            {
                                fMatch = TRUE;
                                fDone = TRUE;
                            }
                            break;
                            
                        case 'D':
			    if ( !str_cmp( word, "DESC" ) )
			    {
				strncpy( pExitData->description, fread_string( fp ), 80 );
			  	fMatch = TRUE;
				break;
			    }
                            break;
                        case 'E':
			    if ( !str_cmp( word, "EXNUM" ) )
			    {
				/* Exit Num must be first */
                		pExitData = new_exit();
				exit_num = fread_number(fp);
				fMatch = TRUE;
				break;
			    }
			    if ( !str_cmp( word, "EXIN" ) )
			    {
				pExitData->exit_info_default = fread_number(fp);
				pExitData->exit_info = pExitData->exit_info_default;
				fMatch = TRUE;
				break;
			    }
                            break;
                        case 'K':
			    if ( !str_cmp( word, "KEYW" ) )
			    {
                            	strncpy( pExitData->keyword, fread_string(fp), 30 );
				fMatch = TRUE;
				break;
			    }
                            break;
                        case 'L':
			    if ( !str_cmp( word, "LOCKS" ) )
			    {
				lock_list = lock_alloc();
				lock_list->next = pExitData->lock;
				lock_list->vnum = fread_number(fp);
				lock_list->lock_info = fread_number(fp);
				lock_list->lock_info_default = lock_list->lock_info;
				lock_list->pick_penalty = fread_number(fp);
				pExitData->lock = lock_list;
				fMatch = TRUE;
				break;
			    }
			    break;
                        case 'M':
			    if ( !str_cmp( word, "MTC" ) )
			    {
                            	strncpy( pExitData->mes_to_char, fread_string(fp), 80 );
				fMatch = TRUE;
				break;
			    }
			    if ( !str_cmp( word, "MTR" ) )
			    {
				strncpy( pExitData->mes_to_room, fread_string(fp), 80 );
				fMatch = TRUE;
				break;
			    }
			    if ( !str_cmp( word, "MAR" ) )
			    {
				strncpy( pExitData->mes_at_room, fread_string(fp), 80 );
				fMatch = TRUE;
				break;
			    }
			    break;
                        case 'T':
                            KEY( "TO", pExitData->u1.vnum, fread_number(fp));
			    if ( !str_cmp( word, "TRAP" ) )
			    {
				pExitData->trap = trap_alloc();
				pExitData->trap->flags = fread_number( fp );
				fMatch = TRUE;
				break;
			    }
			    if ( !str_cmp( word, "TSPELL" ) )
			    {
				SPELL_LIST * spell;
				SPELL_LIST * spell2;
				if ( pExitData->trap == NULL )
				    pExitData->trap = trap_alloc();

				for ( spell2 = pExitData->trap->spells ; spell2 != NULL && spell2->next != NULL ; spell2 = spell2->next );

				spell = spell_alloc();
				spell->level = fread_number( fp );
				spell->spell = skill_lookup( fread_string( fp ) );
				spell->next = NULL;
				if ( spell2 == NULL )
				    pExitData->trap->spells = spell;
				else
				    spell2->next = spell;
				fMatch = TRUE;
				break;
			    }
                            break;
                   	default:
				log_string(word);
				bug("Error, I don't know Word.\n", 0);
			/*	exit(1); */
 
			}
 
                    if ( !fMatch )
                    {            
                        bug( "load_rooms_new : no match.", 0 );
                        fread_to_eol( fp );
                    }
                    
                    /*if ( fDone )
                    {
                        break;
                    }*/
                } /* of 2nd Fdone */
                /*  Oh God, is this a hack or what!  Ah, well, maybe when
                    I am thinking clearer I will find a better way, but for 
                    now...
                */
                if (fDoneRec)
                {
		}
                if (pRoomIndex->exit[exit_num] != NULL)
                {
                    log_string ("load_rooms_new: Same exit was assigned twice");
                    /* Yes, I know that continue is bad structured programming
                       blah, blah, blah
                    */
                    continue;
                }     
                
                if (pExitData->u1.vnum == 0)
                {
                    log_string ("load_rooms_new: No vnum given for exit");
                    /* Ditto */
                    continue;
                } 
                
                if (exit_num == 6)
                {
                    log_string ("load_rooms_new: no direction given for exit");
                    /* And, once more */
                    continue;
                }
                
                /* and now, with all conceivable errors taken care of (I hope)
                */
                
                pRoomIndex->exit[exit_num] = pExitData;
            }
            tempbugfix: 

	    if ( pRoomIndex->trigger_list != NULL ) /* load rooms just loads the vnums of the triggers */
		load_room_triggers(NULL,pRoomIndex);
	    if ( pRoomIndex->size == 0 )
	    {
	    	if ( ( pRoomIndex->sector_type == SECT_UNDERGROUND )
		    || IS_SET( pRoomIndex->room_flags, ROOM_INDOORS ) )
    		    pRoomIndex->size = 3;
	        else
    		    pRoomIndex->size = 5;
	    }

            /* Now, add the room to the hash table */
            iHash                   = pRoomIndex->vnum % MAX_KEY_HASH;
            pRoomIndex->next        = room_index_hash[iHash];
            room_index_hash[iHash]  = pRoomIndex;
             free_room[pRoomIndex->vnum] = 1; 
	    top_vnum_room = top_vnum_room < pRoomIndex->vnum ? pRoomIndex->vnum : top_vnum_room; /* OLC */
	    if ( pRoomIndex->description != NULL && pRoomIndex->description != ""  && !strstr("congeal", pRoomIndex->description) ) 
		top_room_real++;
	    /*assign_area_vnum( pRoomIndex->vnum );  OLC not used */
/*    sprintf(buf, "Loading Room : %s  ->%d", pRoomIndex->name, pRoomIndex->vnum);
    log_string(buf);
*/	    if (pRoomIndex->vnum > top_room_index) top_room_index = pRoomIndex->vnum; 
            /* And start the whole damn thing again */
        } 
    }    
}            
        
 
 
 
 
 
/*
 * Snarf a shop section.
 */
void load_shops_new( AREA_DATA *pArea )
{
    SHOP_DATA *pShop;
    char buf[MAX_INPUT_LENGTH];
    FILE *fp;

    sprintf(buf, "%s%s/shops", SAVEAREAS_DIR, pArea->filename );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open areafile %s ", buf );
        bug( b_buf, 0 );
        return;
    }
 
    for ( ; ; )
    {
        MOB_INDEX_DATA *pMobIndex;
	int type;
        int iTrade;
	int buy,sell;

	if ( feof( fp ) )
	    break;

        pShop                   = new_shop();
        pShop->load_with_area   = area_last;
        pShop->keeper           = fread_number( fp );
        if ( pShop->keeper == 0 )
            break;
	pShop->type = 0;
        for ( iTrade = 0; iTrade < MAX_TRADE; iTrade++ )
	{
	    type = fread_number( fp );
	    if ( type == -1 )
	    {
		pShop->type = fread_number( fp );
		break;
	    }
	    SET_BIT( pShop->type, get_shop_type( type ) );
	}
	buy = fread_number( fp );
	sell = fread_number( fp );
        pShop->profit_buy       = buy ;
        pShop->profit_sell      = sell ;
        /*pShop->profit_sell      = UMIN( 90, buy );*/
        pShop->open_hour        = fread_number( fp );
        pShop->close_hour       = fread_number( fp );
                                  fread_to_eol( fp );
        pMobIndex               = get_mob_index( pShop->keeper );
        pMobIndex->pShop        = pShop;
 
 	/* Added by Trice for POS (Player Ownable Shops) */
        if ( shop_first == NULL )
            shop_first = pShop;
        if ( shop_last  != NULL )
            shop_last->next = pShop;
 
        shop_last       = pShop;
        pShop->next     = NULL;
    }
    if( fp )
    fclose(fp);
 
    return;
}
 
/*
 * Snarf notes file.
 */
void load_notes( void )
{
    FILE *fp;
 
    if ( ( fp = fopen( NOTE_FILE, "r" ) ) == NULL )
        return;
 
    for ( ; ; )
    {
        NOTE_DATA *pnote;
        char letter;
 
        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
    	 	if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        pnote           = note_data_alloc();
 
        if ( str_cmp( fread_word( fp ), "sender" ) )
            break;
        strncpy( pnote->sender, fread_string( fp ), 30 );
 
        if ( str_cmp( fread_word( fp ), "date" ) )
            break;
        strncpy( pnote->date, fread_string( fp ), 30 );
 
	pnote->flags	= 0;
        if ( str_cmp( fread_word( fp ), "stamp" ) )
	{
	    pnote->flags	= fread_number( fp );
	    if ( str_cmp( fread_word( fp ), "stamp" ) )
                 break;
	}
        pnote->date_stamp = fread_number(fp);
 
 	pnote->read_list[0] = '\0';
        if ( str_cmp( fread_word( fp ), "to" ) )
	{
	    strncpy( pnote->read_list, fread_string( fp ), 80 );
	    if ( str_cmp( fread_word( fp ), "to" ) )	
                 break;
	}
        strncpy( pnote->to_list, fread_string( fp ), 80 );
 
        if ( str_cmp( fread_word( fp ), "subject" ) )
            break;
        strncpy( pnote->subject, fread_string( fp ), 30 );
 
        if ( str_cmp( fread_word( fp ), "text" ) )
            break;
        pnote->text     = fread_string( fp );
 
        if ( pnote->date_stamp < current_time - (28*24*60*60) /* 4 wks */
	  && !IS_SET( pnote->flags, NOTE_NO_DELETE ) )
        {
            pnote->next     = note_free;
            note_free       = pnote;
            pnote           = NULL;
            continue;
        }

	note_add( pnote ); 
    }
 
    strcpy( strArea, NOTE_FILE );
    fpArea = fp;
    bug( "Load_notes: bad key word.", 0 );
    exit( 1 );
    return;
}


void load_changes( void )
{
    FILE *fp;

    if ( ( fp = fopen( CHANGE_FILE, "r" ) ) == NULL )
        return;

    for ( ; ; )
    {
        CHANGE_DATA *change;
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
                if( fp )
                    fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );

        change = change_data_alloc();

        if ( str_cmp( fread_word( fp ), "VNum" ) )
            break;
        change->vnum = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Author" ) )
            break;
        strncpy( change->author, fread_string( fp ), 30 );

        if ( str_cmp( fread_word( fp ), "Stamp" ) )
            break;
        change->date_stamp = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Sec" ) )
            break;
        change->security = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Text" ) )
            break;
        change->text = fread_string( fp );

        change_add( change );
    }

    strcpy( strArea, CHANGE_FILE );
    fpArea = fp;
    bug( "Load_changes: bad key word.", 0 );
    exit( 1 );
    return;
}

void save_changes( void )
{
    CHANGE_DATA *change;
    FILE *fp;

    if ( ( fp = fopen( CHANGE_FILE, "w" ) ) == NULL )
    {
        perror( CHANGE_FILE );
    }
    else
    {
        for ( change = change_list; change != NULL; change = change->next )
        {
            fprintf( fp, "VNum    %d\n",  change->vnum);
            fprintf( fp, "Author  %s~\n", change->author);
            fprintf( fp, "Stamp   %d\n",  (int) change->date_stamp);
            fprintf( fp, "Sec     %d\n",  change->security);
            fprintf( fp, "Text\n%s~\n",   change->text);
        }
        if ( fp )
            fclose( fp );
    }
    return;
}


void load_clans( void )
{
    FILE *fp;
 
    if ( ( fp = fopen( CLAN_FILE, "r" ) ) == NULL )
        return;
 
    for ( ; ; )
    {
        CLAN_DATA *clan;
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
    	 	if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        clan           = clan_data_alloc();
	clan->leader = NULL;
	clan->sponser = NULL;
 
        if ( str_cmp( fread_word( fp ), "title" ) )
            break;

        strncpy( clan->title, fread_string( fp ), 80 );
 
        if ( str_cmp( fread_word( fp ), "name" ) )
            break;

        strncpy( clan->name, fread_string( fp ), 30 );
 
        if ( str_cmp( fread_word( fp ), "leader" ) )
            break;

        strncpy( clan->leader_name, fread_string( fp ), 30 );
 
        if ( str_cmp( fread_word( fp ), "sponser" ) )
            break;

        strncpy( clan->sponser_name, fread_string(fp), 30 );
 
        if ( str_cmp( fread_word( fp ), "treasury" ) )
            break;

        clan->treasury  = fread_number( fp );
 
        if ( str_cmp( fread_word( fp ), "clan_info" ) )
            break;

        clan->clan_info  = fread_number( fp );
 
        if ( str_cmp( fread_word( fp ), "recall" ) )
            break;

        clan->recall_room  = fread_number( fp );
 
        if ( str_cmp( fread_word( fp ), "donate" ) )
            break;

        clan->donate_room = fread_number( fp );

	if (str_cmp( fread_word( fp ), "rank1" ) )
 	   break;

  	strncpy(clan->rank1, fread_string(fp), 30);	

        if (str_cmp( fread_word( fp ), "rank2" ) )
           break;

        strncpy(clan->rank2, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank3" ) )
           break;

        strncpy(clan->rank3, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank4" ) )
           break;

        strncpy(clan->rank4, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank5" ) )
           break;

        strncpy(clan->rank5, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank6" ) )
           break;

        strncpy(clan->rank6, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank7" ) )
           break;

        strncpy(clan->rank7, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank8" ) )
           break;

        strncpy(clan->rank8, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank9" ) )
           break;

        strncpy(clan->rank9, fread_string(fp), 30);

        if (str_cmp( fread_word( fp ), "rank10" ) )
           break;

        strncpy(clan->rank10, fread_string(fp), 30);

	clan->next = clan_list;
	clan_list = clan;
    }
 
    strcpy( strArea, CLAN_FILE );
    fpArea = fp;
    bug( "Load_clans: bad key word.", 0 );
    exit( 1 );
    return;
}

void imp_log_string( const char *str )			/* logs imm commands to../log/implog.txt */
{
    FILE *fp;
 
    if ( ( fp = fopen( IMP_LOG, "a" ) ) != NULL)
    {
    char *strtime;
    strtime                    = ctime( &current_time );/*sets up time stamp*/
    strtime[strlen(strtime)-1] = '\0';    	 
    
    fprintf( fp, "%s :: %s\n", strtime, str );		/* puts time stamp and str into IMP_LOG */       
    if( fp )
    fclose( fp );
    }
}
 
 
void load_counter( void )
{
FILE *fp;

    if ( ( fp = fopen( LOGON_COUNTER, "r" ) ) == NULL )
        return;

        LOGON_NUMBER = fread_number( fp );
     if( fp )
     fclose( fp );
return;
}

void load_quests( void )
{
    FILE *fp;
    QUEST_DATA *quest;
    QUEST_DATA *quest_last;
    quest_last = NULL; 
    if ( ( fp = fopen( QUEST_DATA_FILE, "r" ) ) == NULL )
        return;
 
    for ( ; ; )
    {
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
     		if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        quest           = quest_data_alloc();
 
        if ( str_cmp( fread_word( fp ), "Name" ) )
            break;
        strncpy( quest->name, fread_string( fp ), 30 );
 
        if ( str_cmp( fread_word( fp ), "Desc" ) )
            break;
        strncpy( quest->description, fread_string( fp ), 512 );

        if ( str_cmp( fread_word( fp ), "Number" ) )
            break;
        quest->quest_number  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Flag" ) )
            break;
        quest->flag  = fread_number( fp );

        if ( quest_data_list == NULL )
            quest_data_list = quest;

        if ( quest_last  != NULL )
            quest_last->next = quest;
 
        quest_last       = quest;
        quest->next     = NULL;

    }
 
    bug( "Load_quest: bad key word.", 0 );
    exit( 1 );
    return;
}

void load_logon( void )
{
    FILE *fp;
    LOGON_DATA *logon;
    LOGON_DATA *logon_last;
    logon_last = NULL; 
    if ( ( fp = fopen( LOGON_DATA_FILE, "r" ) ) == NULL )
        return;
 
    for ( ; ; )
    {
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
     		if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        logon           = logon_data_alloc();
 
        if ( str_cmp( fread_word( fp ), "name" ) )
            break;
        strncpy( logon->name, fread_string( fp ), 30 );
 
        if ( str_cmp( fread_word( fp ), "time_start" ) )
            break;
        logon->time_start  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "time_end" ) )
            break;
        logon->time_end  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "kills" ) )
            break;
        logon->kills  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "pkills" ) )
            break;
        logon->pkills  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "exp" ) )
            break;
        logon->exp  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "rooms" ) )
            break;
        logon->rooms  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "mobiles" ) )
            break;
        logon->mobiles  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "objects" ) )
            break;
        logon->objects  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "exit" ) )
            break;
        strncpy( logon->exit, fread_string( fp ), 30 );

	if (!str_cmp(logon->exit, "Logged-IN"))
	    strncpy( logon->exit, "Crash", 30 );

        if ( str_cmp( fread_word( fp ), "level" ) )
            break;
        logon->level  = fread_number( fp );

	if ( str_cmp( fread_word( fp ), "last_ip" ) )
	    break;
	strncpy( logon->last_ip, fread_string( fp ), 15 );

        if ( logon_data_list == NULL )
            logon_data_list = logon;

        if ( logon_last  != NULL )
            logon_last->next = logon;
 
        logon_last       = logon;
        logon->next     = NULL;

    }
 
    strcpy( strArea, LOGON_DATA_FILE );
    fpArea = fp;
    bug( "Load_logon: bad key word.", 0 );
    exit( 1 );
    return;
}
 
void load_random( void )
{
    FILE *fp;
    GLOBAL_RANDOM_DATA *grandom;
    GLOBAL_RANDOM_DATA *random_last;
    random_last = NULL; 
    if ( ( fp = fopen( RANDOM_DATA_FILE, "r" ) ) == NULL )
        return;
 
    for ( ; ; )
    {
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
     		if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        grandom           = random_data_alloc();
 
        if ( str_cmp( fread_word( fp ), "World" ) )
            break;
        grandom->world  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Area" ) )
            break;
        grandom->area  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Global" ) )
            break;
        grandom->global  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Chance" ) )
            break;
        grandom->chance  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Max" ) )
            break;
        grandom->max  = fread_number( fp );

        if ( str_cmp( fread_word( fp ), "Vnum" ) )
            break;
        grandom->vnum  = fread_number( fp );

        if ( random_data_list == NULL )
            random_data_list = grandom;

        if ( random_last  != NULL )
            random_last->next = grandom;
 
        random_last       = grandom;
        grandom->next     = NULL;

    }
 
    strcpy( strArea, RANDOM_DATA_FILE );
    fpArea = fp;
    bug( "Load_random: bad key word.", 0 );
    exit( 1 );
    return;
}

void load_topten( void )
{
    FILE *fp;
    int i,c;
    char letter;
 
    if ( ( fp = fopen( TOP_TEN_FILE, "r" ) ) == NULL )
        return;
 
    for ( c = 0 ; c < 3  ; c++ )
    for ( i = 0 ; i < 10 ; i++ )
    {
        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
     		if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        strncpy( top_ten_list[ i ][ c ].name, fread_string( fp ), 30 );
        strncpy( top_ten_list[ i ][ c ].saying, fread_string( fp ), 80 );
        top_ten_list[ i ][ c ].value  = fread_number( fp );
    }
 
    if( fp )
    fclose( fp );
    return;
}
 

void save_clans( void )
{
    CLAN_DATA *clan;
    char buf[MAX_STRING_LENGTH];
    FILE *fp;
    if ( ( fp = fopen( CLAN_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save clan file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    for ( clan = clan_list ; clan != NULL ; clan = clan->next )
    {
	fprintf( fp, "title %s~\n", clan->title );
	fprintf( fp, "name %s~\n", clan->name );
	fprintf( fp, "leader %s~\n", clan->leader_name );
	fprintf( fp, "sponser %s~\n", clan->sponser_name );
	fprintf( fp, "treasury %ld\n", clan->treasury );
	fprintf( fp, "clan_info %ld\n", clan->clan_info );
	fprintf( fp, "recall %ld\n", clan->recall_room );
	fprintf( fp, "donate %ld\n", clan->donate_room ); 
	fprintf( fp, "rank1 %s~\n", clan->rank1 );
	fprintf( fp, "rank2 %s~\n", clan->rank2 );
	fprintf( fp, "rank3 %s~\n", clan->rank3 );
	fprintf( fp, "rank4 %s~\n", clan->rank4 );
	fprintf( fp, "rank5 %s~\n", clan->rank5 );
	fprintf( fp, "rank6 %s~\n", clan->rank6 );
	fprintf( fp, "rank7 %s~\n", clan->rank7 );
	fprintf( fp, "rank8 %s~\n", clan->rank8 );
	fprintf( fp, "rank9 %s~\n", clan->rank9 );
	fprintf( fp, "rank10 %s~\n", clan->rank10 );
    }
 
    if( fp )
    fclose( fp );
    {
	sprintf( buf, "Autosave:  Clans" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	save_rlist();
	REMOVE_BIT( changed_system, CHANGED_CLANS );
    }
    return;
}

void save_quests( void )
{
    QUEST_DATA *quest;
    FILE *fp;
    int x;
    char buf[MAX_STRING_LENGTH];

    if ( ( fp = fopen( QUEST_DATA_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save quests file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    for ( x = 1 ; x <= top_quest ; x++ )
    for ( quest = quest_data_list ; quest != NULL ; quest = quest->next )
    {
	if ( x == quest->quest_number )
	{
	fprintf( fp, "Name %s~\n", quest->name );
	fprintf( fp, "Desc %s~\n", quest->description );
	fprintf( fp, "Number %d\n", quest->quest_number );
	fprintf( fp, "Flag %ld\n", quest->flag ); 
	}
    }
 
    if( fp )
    fclose( fp );
    {
	sprintf( buf, "Autosave:  Quests" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	REMOVE_BIT( changed_system, CHANGED_QUESTS );
    }
    return;
}

void save_logon( void )
{
    LOGON_DATA *logon;
    FILE *fp;
    int x = 0;
    char buf[MAX_STRING_LENGTH];

    if ( ( fp = fopen( LOGON_DATA_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save logon file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    for ( logon = logon_data_list ; logon != NULL ; logon = logon->next )
    {
	fprintf( fp, "name %s~\n", logon->name );
	fprintf( fp, "time_start %ld\n", logon->time_start );
	fprintf( fp, "time_end %ld\n", logon->time_end );
	fprintf( fp, "kills %d\n", logon->kills );
	fprintf( fp, "pkills %d\n", logon->pkills );
	fprintf( fp, "exp %ld\n", logon->exp );
	fprintf( fp, "rooms %d\n", logon->rooms ); 
	fprintf( fp, "mobiles %d\n", logon->mobiles ); 
	fprintf( fp, "objects %d\n", logon->objects ); 
	fprintf( fp, "exit %s~\n", logon->exit ); 
	fprintf( fp, "level %d\n", logon->level ); 
	fprintf( fp, "last_ip %s~\n", logon->last_ip );
	x++;
	if ( x > MAX_LOGON )
	break;
    }
 
    if( fp )
    fclose( fp );
    {
	sprintf( buf, "Autosave:  Logons" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	REMOVE_BIT( changed_system, CHANGED_LOGONS );
    }
    return;
}

void save_random( void )
{
    GLOBAL_RANDOM_DATA *grandom;
    FILE *fp;
    int x = 0;
    char buf[MAX_STRING_LENGTH];

    if ( ( fp = fopen( RANDOM_DATA_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save random file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    for ( grandom = random_data_list ; grandom != NULL ; grandom = grandom->next )
    {
	fprintf( fp, "World %d\n", grandom->world );
	fprintf( fp, "Area %d\n", grandom->area );
	fprintf( fp, "Global %d\n", grandom->global );
	fprintf( fp, "Chance %d\n", grandom->chance );
	fprintf( fp, "Max %d\n", grandom->max );
	fprintf( fp, "Vnum %ld\n", grandom->vnum ); 
	x++;
    }
 
    if( fp )
    fclose( fp );
    {
	sprintf( buf, "Autosave:  Randoms" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	REMOVE_BIT( changed_system, CHANGED_RANDOM );
    }
    return;
}
void load_rlist( )
{
    FILE *fp;
    char letter;
    char *word;

    if ( ( fp = fopen( RACE_FILE, "r" ) ) == NULL )
    {
	bug("Cannot race file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    top_race = 0;
    while ( !feof(fp) && top_race < MAX_RACE )
    {
        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
        	if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );

	word = fread_word( fp );
	if ( !str_cmp( word, "End" ) )
	{
            if( fp )
	    fclose( fp );
	    return;
	}

	race_list[ top_race ].race_flags = 0;
	strncpy( race_list[ top_race ].filename, word, 20 );
	word = fread_word( fp );
	strncpy( race_list[ top_race ].name, word, 15 );
	word = fread_string( fp );
	strncpy( race_list[ top_race ].builders, word, 30 );
	race_list[ top_race ].security = fread_number(fp);
	top_race++;

    }
    if( fp )
    fclose( fp );
    return;
}

void save_top_ten( void )
{
    FILE *fp;
    char *strtime;
    int i,c;

    if ( ( fp = fopen( TOP_TEN_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save top ten file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    for ( c = 0 ; c < 3  ; c++ )
    for ( i = 0 ; i < 10 ; i++ )
    {
	fprintf( fp, "%s~%s~ %d\n", top_ten_list[ i ][ c ].name, top_ten_list[ i ][ c ].saying, top_ten_list[ i ][ c ].value );
    }
 
    if( fp )
    fclose( fp );

    if ( mud_port != 6100 )
    {
	return;
    }

    if ( ( fp = fopen( "/home/httpd/html/topten.html", "w" ) ) == NULL )
    {
	bug("Cannot save top ten web page file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }

    strtime                    = ctime( &current_time );/*sets up time stamp*/
    strtime[strlen(strtime)-1] = '\0';    	 
    fprintf( fp, "<HTML>\n" );
    fprintf( fp, "<TITLE>Topten.html</TITLE>\n" );
    fprintf( fp, "<BASEFONT SIZE=+1>\n\n" );
    fprintf( fp, "<HEAD><CENTER><FONT SIZE=+4><B>The Top Ten</B></CENTER><FONT SIZE=-1>\n" );
    fprintf( fp, "<CENTER><I>Last updated %s</I></CENTER><HEAD>\n", strtime );
    fprintf( fp, "<HR SIZE=10>\n" );
    fprintf( fp, "<BODY BGCOLOR=%c000000%c TEXT=%c0080FF%c VLINK=%c800080%c BACKGROUND=%cimages/stars.gif%c>\n", 34, 34, 34, 34, 34, 34, 34, 34 );
    fprintf( fp, "<H2>\n" );
    fprintf( fp, "<CENTER>Congratulations to</CENTER><BR>\n" );
    fprintf( fp, "<TABLE BORDER=10 CELLPADDING=5 CELLSPACING=5 WIDTH=%c100%c%c>\n", 34, 37, 34 );
    fprintf( fp, "<TR><TD  ALIGN=CENTER VALIGN=TOP>\n" );
    fprintf( fp, "<B>Best PKiller</B><BR>\n" );
    fprintf( fp, "%s  (%d players killed)<br>\n\n\n", capitalize( top_ten_list[ 0 ][ 0 ].name ) , top_ten_list[ 0 ][ 0 ].value );
    fprintf( fp, "</td><td ALIGN=CENTER VALIGN=TOP>\n" );
    fprintf( fp, "<b>Best Killer</b><br>\n" );
    fprintf( fp, "%s  (%d mobs killed)<BR>\n\n<br>\n\n", capitalize( top_ten_list[ 0 ][ 1 ].name ) , top_ten_list[ 0 ][ 1 ].value );
    fprintf( fp, "</td></tr></table>\n" );
    fprintf( fp, "<BR>\n" );
    fprintf( fp, "<BR>\n" );
    fprintf( fp, "<HR SIZE=2>\n" );
    fprintf( fp, "<CENTER>\n" );
    fprintf( fp, "<TABLE BORDER=5 CELLPADDING=5 CELLSPACING=5>\n" );
    fprintf( fp, "<TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD><B>PKILLER</B></TD><H2></H2>\n" );
    fprintf( fp, "<TD><B>KILLER</B></TD><H4></H4>\n\n" );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 1 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 1 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 2 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 2 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 3 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 3 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 4 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 4 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 5 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 5 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 6 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 6 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 7 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 7 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 8 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 8 ][ 1 ].name ) );

    fprintf( fp, "</TR><TR ALIGN=CENTER>\n" );
    fprintf( fp, "<TD>%s</TD><H4></H4>\n", capitalize( top_ten_list[ 9 ][ 0 ].name ) );
    fprintf( fp, "<TD>%s</TD>\n\n", capitalize( top_ten_list[ 9 ][ 1 ].name ) );

    fprintf( fp, "</TR></TABLE></CENTER>\n" );
    fprintf( fp, "<P><HR><BR>\n\n\n\n\n" );

    fprintf( fp, "<A HREF=%cindex.html%c>\n", 34, 34 );
    fprintf( fp, "<IMG SRC=%cbakred.gif%c>\n", 34, 34 );
    fprintf( fp, "</BODY>\n" );
    fprintf( fp, "</HTML>\n" );

    if( fp )
    fclose( fp );
    return;
}

void save_rlist( )
{
    FILE *fp;
    int race;
    if ( ( fp = fopen( RACE_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save race list",0);
        if( fp )
	fclose( fp );
	return;
    }

    for ( race = 0; race < top_race ; race++ )
    {
	fprintf( fp, "%s ", race_list[ race ].filename );
	fprintf( fp, "%s ", race_list[ race ].name );
	fprintf( fp, "%s~", race_list[ race ].builders );
	fprintf( fp, "%d\n", race_list[ race ].security );
    }
    fprintf( fp, "End\n" );
    if( fp )
    fclose( fp );
    return;
}

 
void save_races( void )
{
    char buf[MAX_INPUT_LENGTH];
    int race;
 
    for ( race = 0 ; race < top_race ; race++ )
    {
	save_race( race );
     }
    {
	sprintf( buf, "Autosave:  Races" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	save_rlist();
	REMOVE_BIT( changed_system, CHANGED_RACES );
    }
}

void save_race( int race )
{
    char buf[MAX_INPUT_LENGTH];
    FILE *fp;
    int sex,skill,sn,i;

    sprintf( buf, "../races/%s", race_list[ race ].filename );
    if ( ( fp = fopen( buf, "w" ) ) == NULL )
    {
	bug("Cannot save race file.", 0 );
        if( fp )
	fclose( fp );
	return;
    }

    REMOVE_BIT( race_list[ race ].race_flags, RACE_CHANGED );
    fprintf( fp, "#RACE\n" ); 
    for ( sex = 1; sex < 4 ; sex++ )
    {
        fprintf( fp, "#%s\n", ( sex == SEX_MALE ? "Male" : ( sex == SEX_FEMALE ? "Female" : "Nueter" ) ) );
        fprintf( fp, "Name %s~\n", race_table[ race ][ sex ].name ); 
	if ( race_table[ race ][ sex ].pc_race )
	    fprintf( fp, "Pc_race\n" );
	else
	    fprintf( fp, "Npc_race\n" );
	fprintf( fp, "World %d\n", race_table[ race ][ sex ].world ); 
	fprintf( fp, "Size %d\n", race_table[ race ][ sex ].size ); 
	fprintf( fp, "Gold %d\n", race_table[ race ][ sex ].gold );  
	fprintf( fp, "Dt %d\n", race_table[ race ][ sex ].dam_type ); 
	fprintf( fp, "Act %ld\n", race_table[ race ][ sex ].act ); 
	fprintf( fp, "Aff %ld\n", race_table[ race ][ sex ].aff ); 
	fprintf( fp, "Aff2 %ld\n", race_table[ race ][ sex ].aff2 ); 
	fprintf( fp, "Off %ld\n", race_table[ race ][ sex ].off ); 
	fprintf( fp, "Imm %ld\n", race_table[ race ][ sex ].imm ); 
	fprintf( fp, "Res %ld\n", race_table[ race ][ sex ].res ); 
	fprintf( fp, "Vuln %ld\n", race_table[ race ][ sex ].vuln ); 
	fprintf( fp, "Form %ld\n", race_table[ race ][ sex ].form ); 
	fprintf( fp, "Language %s~\n", skill_table[ race_table[ race ][ sex ].language ].name ); 
	fprintf( fp, "Who %s~\n", race_table[ race ][ sex ].who_name ); 
	fprintf( fp, "AC ");
	for ( i = 0 ; i < 4 ; i++ )
	{
	    fprintf( fp, " %d %d %d ", 
		race_table[ race ][ sex ].ac_dice[ i ],
		race_table[ race ][ sex ].ac_die[ i ],
		race_table[ race ][ sex ].ac_bonus[ i ]);
	}
	fprintf( fp, "\n" );
        fprintf( fp, "Dam %d %d %d\n",
	    race_table[ race ][ sex ].dam_dice,
	    race_table[ race ][ sex ].dam_die,
	    race_table[ race ][ sex ].dam_bonus);
	fprintf( fp, "Scan %d\n", race_table[ race ][ sex ].scan_dist ); 
	fprintf( fp, "Stats");
	for ( skill = 0 ; skill < MAX_STATS ; skill++ )
	    fprintf( fp, " %d", race_table[ race ][ sex ].stats[ skill ] );
	fprintf( fp, "\n");
	fprintf( fp, "Max_stats");
	for ( skill = 0 ; skill < MAX_STATS ; skill++ )
	    fprintf( fp, " %d", race_table[ race ][ sex ].max_stats[ skill ] );
	fprintf( fp, "\n");
	fprintf( fp, "Weapon %ld\n", race_table[ race ][ sex ].weapon );
	fprintf( fp, "Map %ld\n", race_table[ race ][ sex ].map );
	fprintf( fp, "Sage %d\n", race_table[ race ][ sex ].start_age );
	fprintf( fp, "Rage %d\n", race_table[ race ][ sex ].year_per_hour);
	fprintf( fp, "Thac0 %d\n", race_table[ race ][ sex ].thac0_00 );
	fprintf( fp, "Thac32 %d\n", race_table[ race ][ sex ].thac0_32 );
	fprintf( fp, "Hp_mult %d\n", race_table[ race ][ sex ].hp_mult );
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
 	    fprintf( fp, "Man %d %d %d %d\n", i, race_table[ race ][ sex ].mana_dice[ i ],
	    race_table[ race ][ sex ].mana_die[ i ], race_table[ race ][ sex].mana_bonus[ i ] );
	fprintf( fp, "Begin %ld\n", race_table[ race ][ sex ].begin );
	fprintf( fp, "Die %ld\n", race_table[ race ][ sex ].die );
	fprintf( fp, "Donate %ld\n", race_table[ race ][ sex ].donate );
	fprintf( fp, "Max_cons");
	for ( skill = 0 ; skill < MAX_COND ; skill++ )
	    fprintf( fp, " %d", race_table[ race ][ sex ].max_condition[ skill ] );
	fprintf( fp, "\n");
	for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
	{
	if ( sn == 1 )
	    fprintf( fp, "Ski %s~ 1 1 100 100 1\n", skill_table[ sn ].name);
	else 
	    fprintf( fp, "Ski %s~ %d %d %d %d %d\n", skill_table[ sn ].name,
			race_table[ race ][ sex ].skill_level[ sn ], 
			race_table[ race ][ sex ].rating[ sn ],
			race_table[ race ][ sex ].adept[ sn ],
			race_table[ race ][ sex ].starting[ sn ],
			race_table[ race ][ sex ].gained[ sn ]);
	}
	fprintf( fp, "End\n");
    }
    fprintf( fp, "#END\n" ); 

    if( fp )
    fclose( fp );
    return;
}
 
#if defined(KEY)
#undef KEY
#endif
 
#define KEY( literal, field, value )                        \
            if ( !str_cmp( word, literal ) )                \
            {                                               \
                field = value;                              \
                fMatch = TRUE;                              \
                break;                                      \
            }

void load_race( int race )
{
    char *word;
    FILE *fp;
    char buf[MAX_STRING_LENGTH];
    int sex,i,sn,age;
    bool fMatch;
    bool fEnd=FALSE;
    bool fDone=FALSE;
 
    default_race( race ); 

    sprintf( buf, "../races/%s", race_list[ race ].filename );
    if ( ( fp = fopen( buf , "r" ) ) == NULL )
    {
	bug("Cannot read race file",0);
        if( fp )
	fclose( fp );    
	return;
    }

    log_string( buf );

    word = fread_word( fp );

    if ( str_cmp( word, "#RACE" ) )
    {
	bug("Bad race file",0);
        if( fp )
	fclose( fp );
	return;
    }

    sex = 1;
    for ( ; !fDone ; )
    {
        word = fread_word( fp );
	if ( !str_cmp( word, "#End" ) )
	{
	    fDone = TRUE;
            if( fp )
	    fclose( fp );
	    return;
	}
	else if ( word[0] != '\0' )
	{
	    switch ( word[1] )
	    {
	    default:
		bug("Bad sex in race file",0);
                if( fp )
	    	fclose( fp );
		return;
	    	break;
	    case 'F':
		sex = SEX_FEMALE;
		break;
	    case 'M':
		sex = SEX_MALE;
		break;
	    case 'N':
		sex = SEX_NEUTER;
		break;
	    }
	}
        
	fEnd = FALSE;
        for ( ; !fEnd ; )
        {
            word = fread_word( fp );
            fMatch = FALSE;
            switch (word[0])
            {
                case 'A' :                 
		    if ( !str_cmp( word, "Age_stats" ) )
		    {
			age = fread_number( fp );
			for ( i = 0 ; i < MAX_STATS ; i++ )
			    race_table[ race ][ sex ].age_stats[ age ][ i ] = fread_number(fp);
			fMatch=TRUE;
			break;
		    }
		    if ( !str_cmp( word, "AC" ) )
		    {
			for ( i = 0 ; i < 4 ; i++ )
			{
			    race_table[ race ][ sex ].ac_dice[ i ] = fread_number(fp);
			    race_table[ race ][ sex ].ac_die[ i ] = fread_number(fp);
			    race_table[ race ][ sex ].ac_bonus[ i ] = fread_number(fp);
			}
			fMatch=TRUE;
			break;
		    }
		    KEY("Act",race_table[ race ][ sex ].act, fread_number(fp)|ACT_IS_NPC);
		    KEY("Aff",race_table[ race ][ sex ].aff, fread_number(fp));
		    KEY("Aff2",race_table[ race ][ sex ].aff2, fread_number(fp));
                    break;
		case 'B' :
		    KEY("Begin",race_table[ race ][ sex ].begin, fread_number(fp));
		    break;
                case 'C' :
                    break;
                case 'D' :
		    KEY("Donate",race_table[ race ][ sex ].donate, fread_number(fp));
		    KEY("Die",race_table[ race ][ sex ].die, fread_number(fp));
		    KEY("Dt",race_table[ race ][ sex ].dam_type, fread_number(fp));
		    if ( !str_cmp( word, "Dam" ) )
		    {
			race_table[ race ][ sex ].dam_dice = fread_number(fp);
			race_table[ race ][ sex ].dam_die = fread_number(fp);
			race_table[ race ][ sex ].dam_bonus = fread_number(fp);
			fMatch=TRUE;
			break;
		    }
                    break;
                case 'E' :
		    if ( !str_cmp( word, "End" ) )
		    {
			fMatch=TRUE;
			fEnd = TRUE;
			break;
		    }
		    break;
                case 'F' :
		    KEY("Form",race_table[ race ][ sex ].form, fread_number(fp));
                    break;
                case 'G' :
		    KEY("Gold",race_table[ race ][ sex ].gold, fread_number(fp));
                    break;
                case 'H' :
		    KEY("Hp_min", i, fread_number(fp));
		    KEY("Hp_max", i, fread_number(fp));
		    if ( !str_cmp( word, "Hp" ) )
		    {
			fMatch=TRUE;
			fread_to_eol(fp);
			break;
		    }
		    KEY("Hp", i, fread_number(fp));
		    KEY("Hp_mult", race_table[ race ][ sex ].hp_mult, fread_number(fp));
                    break;
                case 'I' :
		    KEY("Imm",race_table[ race ][ sex ].imm, fread_number(fp));
                    break;
                case 'L' :
                    if (!str_cmp(word, "Language"))
                    {
			word = fread_string(fp);
			race_table[ race ][ sex ].language = skill_lookup( word );
                        fMatch = TRUE;        
                        break;
                    }               
                    break;
                case 'M' :
		    if ( !str_cmp( word, "Max_con" ) )
		    {
			for ( i = 1 ; i < MAX_COND ; i++ )
			    race_table[ race ][ sex ].max_condition[ i ] = fread_number(fp);
			fMatch=TRUE;
			break;
		    }
		    if ( !str_cmp( word, "Max_cons" ) )
		    {
			for ( i = 0 ; i < MAX_COND ; i++ )
			    race_table[ race ][ sex ].max_condition[ i ] = fread_number(fp);
			fMatch=TRUE;
			break;
		    }
                    if (!str_cmp(word, "Mana"))
                    {
			int type, die, dice, bonus;

			dice = fread_number( fp );
			die = fread_number( fp );
			bonus = fread_number( fp );
			for ( type = 0 ; type < MAX_ELEMENT_TYPE ; type++ )
			{
			    race_table[ race ][ sex ].mana_dice[ type ] = dice;
			    race_table[ race ][ sex ].mana_die[ type ] = die;
			    race_table[ race ][ sex ].mana_bonus[ type ] = bonus;
			}
                        fMatch = TRUE;        
                        break;
                    }
                    if (!str_cmp(word, "Man"))
                    {
			int type;
			type = fread_number(fp);
                        race_table[ race ][ sex ].mana_dice[type] = fread_number(fp); 
                        race_table[ race ][ sex ].mana_die[type] = fread_number(fp); 
                        race_table[ race ][ sex ].mana_bonus[type] = fread_number(fp); 
                        fMatch = TRUE;        
                        break;
		    }
		    KEY("Map",race_table[ race ][ sex ].map, fread_number(fp));
		    KEY("Mult", i, fread_number(fp));
                    if (!str_cmp(word, "Max_stats"))
                    {
			for ( i = 0 ; i < MAX_STATS ; i++ )
			    race_table[ race ][ sex ].max_stats[ i ] = fread_number(fp);
                        fMatch = TRUE;        
                        break;
                    }               
                    break;
                case 'N' :
                    if (!str_cmp(word, "Npc_race"))
                    {
			race_table[ race ][ sex ].pc_race = FALSE;
                        fMatch = TRUE;        
                        break;
                    }
		    if ( !str_cmp(word, "Name" ) )
		    {               
                    	strncpy( race_table[ race ][ sex ].name, fread_string(fp), 15 );
			fMatch = TRUE;
			break;
		    }
                    break;
                case 'O' :
		    KEY("Off",race_table[ race ][ sex ].off, fread_number(fp));
                    break;
                case 'P' :
		    KEY("Points", i, fread_number(fp));
                    if (!str_cmp(word, "Pc_race"))
                    {
                        race_table[ race ][ sex ].pc_race = TRUE;
                        fMatch = TRUE;
                        break;
                    }
                    break;
                case 'R' :
		    KEY("Rage",race_table[ race ][ sex ].year_per_hour, fread_number(fp));
		    KEY("Res",race_table[ race ][ sex ].res, fread_number(fp));
                    break;
                case 'S' :
                    KEY("Sage",race_table[ race ][ sex ].start_age, fread_number(fp) );
                    KEY("Scan",race_table[ race ][ sex ].scan_dist, fread_number(fp) );
                    KEY("Size",race_table[ race ][ sex ].size, fread_number(fp) );
                    if (!str_cmp(word, "Skill"))
                    {
			sn = skill_lookup( fread_string(fp) );
			race_table[ race ][ sex ].skill_level[ sn ] = fread_number(fp);
			race_table[ race ][ sex ].rating[ sn ] = fread_number(fp);
			race_table[ race ][ sex ].adept[ sn ] = fread_number(fp);
                        fMatch = TRUE;        
                        break;
                    }               
                    if (!str_cmp(word, "Ski"))
                    {
			sn = skill_lookup( fread_string(fp) );
			race_table[ race ][ sex ].skill_level[ sn ] = fread_number(fp);
			race_table[ race ][ sex ].rating[ sn ] = fread_number(fp);
			race_table[ race ][ sex ].adept[ sn ] = fread_number(fp);
			race_table[ race ][ sex ].starting[ sn ] = fread_number(fp);
			race_table[ race ][ sex ].gained[ sn ] = fread_number(fp); 
                        fMatch = TRUE;        
                        break;
                    }               
                    if (!str_cmp(word, "Stats"))
                    {
			for ( i = 0 ; i < MAX_STATS ; i++ )
			    race_table[ race ][ sex ].stats[ i ] = fread_number(fp);
                        fMatch = TRUE;        
                        break;
                    }               
                    break;
                case 'T' :
		    KEY("Thac0",race_table[ race ][ sex ].thac0_00, fread_number(fp));
		    KEY("Thac32",race_table[ race ][ sex ].thac0_32, fread_number(fp));
                    break;
                case 'V' :
		    KEY("Vuln",race_table[ race ][ sex ].vuln, fread_number(fp));
                    break;
                case 'W' :
                    KEY("Weapon",race_table[ race ][ sex ].weapon, fread_number(fp) );
                    KEY("World",race_table[ race ][ sex ].world, fread_number(fp) );
		    if ( !str_cmp( word, "Who" ) )
		    {
			word = fread_string( fp );
			for( i = 0 ; i < 6 && word[i] != '\0' ; i++ )
			    race_table[ race ][ sex ].who_name[i] = word[i];
			race_table[ race ][ sex ].who_name[ UMIN( 5, i ) ] = '\0';
			fMatch = TRUE;
			break;
		    }
                    break;
            }
            if ( !fMatch )
            {            
                sprintf(buf, "Load_race No match for : %s", word);
                log_string(buf);
                fread_to_eol( fp );
            }
        }
    }
}
 
void default_race( int race )
{
    int sex,skill,age,sn,i;
 
    for ( sex = 1; sex < 4 ; sex++ )
    {
	strncpy( race_table[ race ][ sex ].name, "Default", 15 );
	race_table[ race ][ sex ].pc_race = FALSE;
	race_table[ race ][ sex ].world = 0;
	race_table[ race ][ sex ].size = SIZE_MEDIUM;
	race_table[ race ][ sex ].dam_type = 0;
	race_table[ race ][ sex ].act = 0;
	race_table[ race ][ sex ].aff = 0;
	race_table[ race ][ sex ].aff2 = 0;
	race_table[ race ][ sex ].off = 0;
	race_table[ race ][ sex ].imm = 0;
	race_table[ race ][ sex ].res = 0;
	race_table[ race ][ sex ].vuln = 0;
	race_table[ race ][ sex ].form = 0;
	race_table[ race ][ sex ].language = gsn_common;
	race_table[ race ][ sex ].who_name[0] = '\0';
	for ( i = 0 ; i < 3 ; i++ )
	{
	    race_table[ race ][ sex ].ac_dice[ i ] = 2;
	    race_table[ race ][ sex ].ac_die[ i ] = 3;
	    race_table[ race ][ sex ].ac_bonus[ i ] = 2;
	}
        race_table[ race ][ sex ].ac_dice[ 3 ] = 1;
        race_table[ race ][ sex ].ac_die[ 3 ] = 5;
        race_table[ race ][ sex ].ac_bonus[ 3 ] = 1;
        race_table[ race ][ sex ].dam_dice = 1;
        race_table[ race ][ sex ].dam_die = 8;
        race_table[ race ][ sex ].dam_bonus = 2;
	race_table[ race ][ sex ].scan_dist = 3;
	for ( skill = 0 ; skill < MAX_STATS ; skill++ )
	    race_table[ race ][ sex ].stats[ skill ] = 13;
	for ( skill = 0 ; skill < MAX_STATS ; skill++ )
	    race_table[ race ][ sex ].max_stats[ skill ] = 18;
	race_table[ race ][ sex ].weapon = 3702;
	race_table[ race ][ sex ].map = 238;
	race_table[ race ][ sex ].start_age = 13;
	race_table[ race ][ sex ].year_per_hour = 4;
	race_table[ race ][ sex ].thac0_00 = 0;
	race_table[ race ][ sex ].thac0_32 = 13;
	race_table[ race ][ sex ].hp_mult = 100;
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	{ 
	    race_table[ race ][ sex ].mana_dice[ i ] = 1; 
	    race_table[ race ][ sex ].mana_die[ i ]  = 4; 
	    race_table[ race ][ sex ].mana_bonus[ i ] = 1; 
	}
	race_table[ race ][ sex ].begin = 2;
	race_table[ race ][ sex ].die = 2;
	race_table[ race ][ sex ].donate = 2;
	for ( skill = 0 ; skill < MAX_COND ; skill++ )
	    race_table[ race ][ sex ].max_condition[ skill ] = 48;
	for ( age = 0 ; age < 40 ; age++ )
	    for ( skill = 0; skill < MAX_STATS ; skill++ )
		race_table[ race ][ sex ].age_stats[ age ][ skill ] = 25;
	for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
	{
	    race_table[ race ][ sex ].skill_level[ sn ] = 55;
	    race_table[ race ][ sex ].rating[ sn ] = -1;
	    race_table[ race ][ sex ].adept[ sn ] = ( skill_table[ sn ].spell_fun == spell_null ? 85 : 95 );
	    race_table[ race ][ sex ].starting[ sn ] = 0;
	}
    }

    return;
}

void load_races( void )
{
    int race;

    for ( race = 0 ; race < top_race ; race++ )
    {
	load_race( race );
     }
}
 
void save_glist( )
{
    FILE *fp;
    int guild;

    if ( ( fp = fopen( GUILD_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save guild list",0);
        if( fp )
	fclose( fp );
	return;
    }

    for ( guild = 0; guild < top_guild ; guild++ )
    {
	fprintf( fp, "%s ", guild_table[ guild ].filename );
	fprintf( fp, "%s ", guild_table[ guild ].name );
	fprintf( fp, "%s~", guild_table[ guild ].builders );
	fprintf( fp, "%d\n", guild_table[ guild ].security );
	fprintf( fp, " %ld ", guild_table[ guild ].flag );
    }
    fprintf( fp, "End\n" );
    if( fp )
    fclose( fp );
    return;
}
void load_glist( )
{
    FILE *fp;
    char letter;
    char *word;
    long flag = 1;

    if ( ( fp = fopen( GUILD_FILE, "r" ) ) == NULL )
    {
	bug("Cannot guild file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    top_guild = 0;
    while ( !feof(fp) && top_guild < MAX_GUILD )
    {
        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
        	if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );

	word = fread_word( fp );
	if ( !str_cmp( word, "End" ) )
	{
	    if( fp )
	    fclose( fp );
	    return;
	}

	guild_table[ top_guild ].guild_flags = 0;
	strncpy( guild_table[ top_guild ].filename, word, 30 );
	word = fread_word( fp );
	strncpy( guild_table[ top_guild ].name, word, 30 );
	word = fread_string( fp );
	strncpy( guild_table[ top_guild ].builders, word, 30 );
	guild_table[ top_guild ].security = fread_number(fp);
	if ( ( letter = getc( fp ) ) == ' ' )
	{
	    guild_table[ top_guild ].flag = fread_number( fp );
	}	
	else
	{
	    guild_table[ top_guild ].flag = flag;
	    ungetc( letter, fp );
	}
	top_guild++;

	if ( top_guild == 1)
	    flag = 2;
	else
	    flag *= 2;
    }
    if( fp )
    fclose( fp );
    return;
}

void load_guild( int guild )
{
    char *word;
    FILE *fp;
    char buf[MAX_STRING_LENGTH];
    bool fMatch;
    bool fEnd=FALSE;
    bool fDone=FALSE;
 
    /*default_guild( guild ); */

    sprintf( buf, "../guilds/%s", guild_table[ guild ].filename );
    if ( ( fp = fopen( buf , "r" ) ) == NULL )
    {
	bug("Cannot read guild file",0);
        if( fp )
	fclose( fp );    
	return;
    }


    word = fread_word( fp );

    if ( str_cmp( word, "#GUILD" ) )
    {
	bug("Bad guild file",0);
        if( fp )
	fclose( fp );
	return;
    }

	fEnd = FALSE;
        for ( ; !fEnd ; )
        {
            word = fread_word( fp );
            fMatch = FALSE;
            switch (word[0])
            {
                case '#' :
		if ( !str_cmp( word, "#END" ) )
		{
		    fDone = TRUE;
        	    if( fp )
		    fclose( fp );
		    return;
		}
		break;
		case 'G' :
                if (!str_cmp(word, "G_NAME"))
                {
                    char tempbuf[31];
		    int tempnum;
                    strncpy( tempbuf, fread_string(fp), 30 );
		    tempnum = fread_number(fp);
		    guild_table[guild].rating[tempnum] = fread_number(fp);
                    fMatch = TRUE;
                    break;
                }
                break;
		case 'T':
		if ( !str_cmp( word, "Title" ) )
		{
		    strncpy( guild_table[guild].title, fread_string(fp), 30 );
		    fMatch = TRUE;
		    break;
		}
		break;
            }
            if ( !fMatch )
            {            
                sprintf(buf, "Load_guild No match for : %s", word);
                log_string(buf);
                fread_to_eol( fp );
            }
    }
if( fp )
fclose(fp);
}
void load_guilds( void )
{
    int guild;

    for ( guild = 0 ; guild < top_guild ; guild++ )
    {
	load_guild( guild );
     }
}
void default_guild( int guild)
{
	strncpy( guild_table[ guild ].title, "none", 30 );
    return;
}

void save_guilds( void )
{
    int guild;
    char buf[MAX_STRING_LENGTH];

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
    {
	save_guild( guild );
    }
    save_glist();
    {
	sprintf( buf, "Autosave:  Guilds" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	REMOVE_BIT( changed_system, CHANGED_GUILDS );
    }
}
void save_guild( int guild )
{
    char buf[MAX_INPUT_LENGTH];
    FILE *fp;
    int sn;
    sprintf( buf, "../guilds/%s", guild_table[ guild ].filename );
    if ( ( fp = fopen( buf, "w" ) ) == NULL )
    {
	bug("Cannot save guild file.", 0 );
	if( fp )
	fclose( fp );
	return;
    }

    REMOVE_BIT( guild_table[ guild ].guild_flags, GUILD_CHANGED );
    fprintf( fp, "Title %s~\n",guild_table[guild].title);
    fprintf( fp, "#GUILD\n" ); 
    {
	for ( sn = 0; sn < MAX_GROUP; sn++ )
	    fprintf(fp,"G_NAME %20s~ %d %d \n",group_name[ sn ], sn,guild_table[guild].rating[sn] );
    }
    fprintf( fp, "#END\n" ); 

    if( fp )
    fclose( fp );
    return;

}

/*
 * Translate all room exits from virtual to real.
 * Has to be done after all rooms are read in.
 * Check for bad reverse exits.
 */
void fix_exits( void )
{
    char buf[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    EXIT_DATA *pexit_rev;
    int iHash;
    int door;
 
    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for ( pRoomIndex  = room_index_hash[iHash];
              pRoomIndex != NULL;
              pRoomIndex  = pRoomIndex->next )
        {
            bool fexit;
 
            fexit = FALSE;
            for ( door = 0; door <= 5; door++ )
            {
                if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
                {
                    if ( pexit->u1.vnum <= 0 
                    || get_room_index(pexit->u1.vnum) == NULL)
                        pexit->u1.to_room = NULL;
                    else
                    {
                        fexit = TRUE; 
                        pexit->u1.to_room = get_room_index( pexit->u1.vnum );
                    }
                }
            }
            if (!fexit)
                SET_BIT(pRoomIndex->room_flags,ROOM_NO_MOB);
        }
    }
 
    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for ( pRoomIndex  = room_index_hash[iHash];
              pRoomIndex != NULL;
              pRoomIndex  = pRoomIndex->next )
        {
            for ( door = 0; door <= 5; door++ )
            {
                if ( ( pexit     = pRoomIndex->exit[door]       ) != NULL
                &&   ( to_room   = pexit->u1.to_room            ) != NULL
                &&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
                &&   pexit_rev->u1.to_room != pRoomIndex 
                &&   (pRoomIndex->vnum < 1200 || pRoomIndex->vnum > 1299))
                {
                    sprintf( buf, "Fix_exits: %ld:%d -> %ld:%d -> %ld.",
                        pRoomIndex->vnum, door,
                        to_room->vnum,    rev_dir[door],
                        (pexit_rev->u1.to_room == NULL)
                            ? 0 : pexit_rev->u1.to_room->vnum );
                    /* bug( buf, 0 ); */ 
                }
            }
        }
    }
 
    return;
}
 
 
 
/*
 * Repopulate areas periodically.
 */
void area_update( void )
{
    AREA_DATA *pArea;

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
 
        if ( pArea->age < 0 || ++pArea->age < 3 )
            continue;

        /*
         * Check age and reset if greater then max_age.
         */

        if ( ( (pArea->nplayer == 0 && pArea->age >= pArea->max_age_empty))
        ||    pArea->age >= pArea->max_age)
        {
	    reset_area(pArea);

            pArea->age = number_range( 0, 3 );
            if (pArea->nplayer == 0) 
		pArea->age = -1;
        }
    }
 
    return;
}

void reset_area(AREA_DATA *pArea )
{
    TRIGGER_DATA *trigger;

    	/* reset all triggers that have the SCRIPT_HALT_RESET */

    	for ( trigger = trigger_list; trigger != NULL; trigger = trigger->next_in_list )
    	{
	    if ( trigger->area == pArea )
	    REMOVE_BIT(trigger->flags, SCRIPT_HALT_RESET );
    	}

	    event_reset( fBootDB_Area_Update ? EVENT_ON_BOOT : EVENT_ON_RESET, pArea);
	/* Reset all the doors for the area */
	reset_doors(pArea);

	{
            sprintf(log_buf, "Resetting Area: %s", pArea->name);
            info_channel( NULL, log_buf , INFOACT_RESET );
            /*log_string( log_buf ); No longer reporting to the logs */
	}
}
/*
 * Hatching from containers
 */ 
void hatch( OBJ_DATA * cont )
{
    RESET_DATA *pReset;
    RESET_DATA *pReset_next;
    CHAR_DATA *mob;
    CHAR_DATA *leader;
    ROOM_INDEX_DATA *pRoomIndex;
    bool last;
    int level;
    char buf[MAX_STRING_LENGTH];

    if ( cont->cont_data == NULL 
      || ( pRoomIndex = get_in_room_obj( cont ) ) == NULL
      || cont->cont_data->inside == NULL )
	return;

    mob         = NULL;
    last        = TRUE;
    level       = 0;
    for ( pReset = cont->cont_data->inside; pReset != NULL; pReset = pReset->next )
    {
        MOB_INDEX_DATA *pMobIndex;
        OBJ_INDEX_DATA *pObjIndex;
        EXIT_DATA *pexit;
	LOCK_DATA *lock_list;
        OBJ_DATA *obj_to;
        OBJ_DATA *obj;
	CHAR_DATA *rch;
 

  	if ( pReset->chance < number_percent() )
	    continue;

        switch ( pReset->command )
        {
        default:
            bug( "Hatch: bad command %c.", pReset->command );
            break;
 
        case 'M':
            if ( ( pMobIndex = get_mob_index( pReset->arg1 ) ) == NULL )
            {
                bug( "Hatch: 'M': bad vnum %d.", pReset->arg1 );
                continue;
            }
 
            if ( pMobIndex->count >= pReset->arg2 )
            {
                last = FALSE;
		mob = NULL;
                break;
            }

	    if ( pReset->vnum == -1 )
	    {
	        if ( !last || mob == NULL )
	        {
		    last = FALSE;
		    mob = NULL;
		    break;
	        }
		leader = mob;
                mob = create_mobile( pMobIndex );
		add_follower(mob, leader);
 		mob->leader = leader;
	        leader->mount = mob;
		SET_BIT( mob->affected_by, AFF_MOUNTED );
	    }
	    else if ( pReset->vnum > 0 )
	    {
			/* looks for last in room (ie first reset) */
		leader = NULL;
		for ( rch = pRoomIndex->people ; rch != NULL ; rch = rch->next_in_room )
		    if ( IS_NPC( rch ) && rch->pIndexData->vnum == pReset->vnum )
			leader = rch;
		if ( leader == NULL )
		{
		    last = FALSE;
		    mob = NULL;
		    break;
		}
                mob = create_mobile( pMobIndex );
		add_follower(mob, leader);
		mob->leader = leader;
		leader->leader = leader;
	    }
	    else
                mob = create_mobile( pMobIndex );

            if ( room_is_dark( pRoomIndex, NULL ) )
                SET_BIT(mob->affected_by, AFF_INFRARED);
 
            char_to_room( mob, pRoomIndex );
	    act( "$n pops out of $p.", mob, cont, NULL, TO_ROOM );

	    if ( IS_AFFECTED( mob, AFF_CONTROL_MIND ) )
	    {
		CHAR_DATA * vch = NULL;
		CHAR_DATA * victim;
		int number = 1;

		for ( victim = pRoomIndex->people ; victim != NULL ; victim = victim->next_in_room )
		    if ( !IS_NPC( victim )
		      && number_range( 1, number++ ) == 1 )
			vch = victim;

		if ( vch != NULL )
		    {
			act( "$n starts to follow $N.", mob, NULL, vch, TO_NOTVICT );
			act( "$n starts to follow you.", mob, NULL, vch, TO_VICT );

			add_follower(mob, vch);
			mob->leader = vch;

			if ( IS_SET( mob->act, ACT_PET ) )
			    vch->pet = mob;
			if ( IS_SET( mob->act, ACT_MOUNT ) )
			    vch->mount = mob;
		    }
	    }
	    script_update( mob, TRIG_BORN );
            level = URANGE( 0, mob->level - 2, LEVEL_HERO - 1 );
            last  = TRUE;
            break;
 
        case 'O':
            if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
            {
                bug( "Hatch: 'O': bad vnum %d.", pReset->arg1 );
                continue;
            }
 
            if ( count_obj_list( pObjIndex, pRoomIndex->contents ) > pReset->arg2 -1 
		|| ( pObjIndex->gate_data != NULL && count_obj( pObjIndex ) >= pReset->arg2  ) )
            {
                last = FALSE;
                break;
            }
 
            obj       = create_object( pObjIndex );

	    if ( obj->wear_data != NULL )
                obj->wear_data->cost = 0;
            obj_to_room( obj, pRoomIndex );
	    sprintf( buf, "%s pops out of %s!\n\r", obj->short_descr, cont->short_descr );
	    send_to_room( pRoomIndex->vnum, buf );
            last = TRUE;
            break;
 
        case 'P':
            if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
            {
                bug( "Hatch: 'P': bad vnum %d.", pReset->arg1 );
                continue;
            }
 
	    for ( obj_to = pRoomIndex->contents ; obj_to != NULL && obj_to->pIndexData->vnum != pReset->arg3 ; obj_to = obj_to->next_content );

            if ( obj_to == NULL )
            {
                bug( "Hatch: 'P': bad vnum %d.", pReset->arg3 );
                continue;
            }
 
            if ( ( obj_to->in_room == NULL && !last )
	    ||   obj_to->cont_data == NULL
            ||   count_obj_list( pObjIndex, obj_to->cont_data->contents ) > pReset->arg2  -1) /* max for inside objects */
            {
                last = FALSE;
                break;
            }
 
            obj = create_object( pObjIndex );
            obj_to_obj( obj, obj_to );
            last = TRUE;
            break;
 
        case 'G':
        case 'E':
            if ( ( pObjIndex = get_obj_index( pReset->arg1 ) ) == NULL )
            {
                bug( "Hatch: 'E' or 'G': bad vnum %d.", pReset->arg1 );
                continue;
            }
 
            if ( !last )
                break;
 
            if ( mob == NULL )
            {
                bug( "Hatch: 'E' or 'G': null mob for vnum %d.",
                    pReset->arg1 );
                last = FALSE;
                break;
            }
 
            if ( mob->pIndexData->pShop != NULL )
            {
		if ( pObjIndex->wear_data != NULL )
                    obj = create_object( pObjIndex );
		else
		    obj = create_object( pObjIndex );

                SET_BIT( obj->extra_flags, ITEM_INVENTORY );
            }
 
            else
            {
                int limit;
                if (pReset->arg2 > 50) /* old format */
                    limit = 6;
                else if (pReset->arg2 < 1) /* no limit */
                    limit = 999;
                else
                    limit = pReset->arg2;
 
                if (pObjIndex->count < limit)
                    obj=create_object(pObjIndex);
                else
                    break;
            }
            obj_to_char( obj, mob );
            if ( pReset->command == 'E' )
                equip_char( mob, obj, pReset->arg3 );
            last = TRUE;
            break;
 
        case 'D':
            if ( ( pexit = pRoomIndex->exit[pReset->arg2] ) == NULL )
                break;
 
            switch ( pReset->arg3 )
            {
            case 0:
                REMOVE_BIT( pexit->exit_info, EX_CLOSED );
		for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_list->next )
		    REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED );
                break;
 
            case 1:
                SET_BIT(    pexit->exit_info, EX_CLOSED );
		for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_list->next )
		    REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED );
                break;
 
            case 2:
                SET_BIT(    pexit->exit_info, EX_CLOSED );
		for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_list->next )
		    SET_BIT( lock_list->lock_info, LOCK_LOCKED );
                break; 
            }
 
            last = TRUE;
            break;
 
        case 'R':
            {
                int d0;
                int d1;
 
                for ( d0 = 0; d0 < pReset->arg2 - 1; d0++ )
                {
                    d1                   = number_range( d0, pReset->arg2-1 );
                    pexit                = pRoomIndex->exit[d0];
                    pRoomIndex->exit[d0] = pRoomIndex->exit[d1];
                    pRoomIndex->exit[d1] = pexit;
                }
            }
            break;
        }
    }
 
    for ( pReset = cont->cont_data->inside ; pReset != NULL ; pReset = pReset_next )
    {
	pReset_next = pReset->next;
	free_reset_data(pReset );
    }

    cont->cont_data->inside = NULL;

    return;
}
void do_load_room_triggers( CHAR_DATA *ch, char *argument )
{
load_room_triggers(ch, ch->in_room );
}
void load_room_triggers( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    ROOM_INDEX_DATA * pRoom;
    ROOM_INDEX_DATA * pRoom_next;
    TRIGGER_LIST_DATA *trigger_list_data;
    TRIGGER_INDEX_DATA *pTrig;
    TRIGGER_DATA *trigger;
    TRIGGER_DATA *trig;
    TRIGGER_DATA *trig_next;
    VARIABLE_DATA *var;

    if ( ch != NULL ) /* command called from interp.c */
    pRoomIndex = ch->in_room;

    if ( pRoomIndex->trigger_list != NULL )
    {
        if ( pRoomIndex->triggers != NULL )
	{
	    if ( room_trig_list == pRoomIndex )
		room_trig_list = room_trig_list->next_with_trigger;
	    else	    
	    {
		for ( pRoom = room_trig_list ; pRoom != NULL ; pRoom = pRoom_next )
		{
		    pRoom_next = pRoom->next_with_trigger;
		    if ( pRoom_next == pRoomIndex )
			pRoom->next_with_trigger = pRoom_next->next_with_trigger;
		}
	    }
	    pRoomIndex->next_with_trigger = NULL;
	}

    for ( trig = pRoomIndex->triggers; trig != NULL; trig = trig_next )
    {
	trig_next = trig->next;
	free_trigger( trig );
    }

    pRoomIndex->triggers = NULL;

    for ( trigger_list_data = pRoomIndex->trigger_list;  trigger_list_data != NULL;  trigger_list_data = trigger_list_data->next )
    {
        if ( ( pTrig = get_trig_index( trigger_list_data->vnum ) ) == NULL )
        {
            bug( "Load_room_triggers: vnum %d bad trigger index.", trigger_list_data->vnum );
	    bug( "Room %ld",pRoomIndex->vnum);
	    continue; /* There might be more good ones in the list */
        }

	if ( pRoomIndex->triggers == NULL )
	{
	    pRoomIndex->next_with_trigger  = room_trig_list;
	    room_trig_list = pRoomIndex;
	}

        trigger      = trigger_alloc(); 

        strncpy( trigger->name , pTrig->name, 30 );
        strncpy( trigger->key_words, pTrig->key_words, 30 );
        strncpy( trigger->key_string, pTrig->key_string, 30 );
        strncpy( trigger->key_sim, pTrig->key_sim, 30 );
        trigger->trigger_type = pTrig->trigger_type;
        trigger->script       = pTrig->script;          /* Uses The Index of Triggers */
        trigger->current      =
             trigger->trigger_type == TRIG_BORN ? pTrig->script : NULL;
        trigger->tracer       = 0;
        trigger->timer        = 0;
        trigger->waiting      = 0;
        trigger->vnum         = pTrig->vnum;
        trigger->bits         = SCRIPT_ADVANCE;
        trigger->flags        = pTrig->flags;
        trigger->step         = pTrig->step;
        trigger->step_pre     = pTrig->step_pre;
	REMOVE_BIT(trigger->flags, SCRIPT_HALT_RESET);
        trigger->gets_vnum    = pTrig->gets_vnum;
        trigger->mob_vnum     = pTrig->mob_vnum;
        trigger->area         = pRoomIndex->area;

        trigger->next    = pRoomIndex->triggers;
        pRoomIndex->triggers    = trigger;
    }
    for ( var = pRoomIndex->variables; var != NULL; var = var->next )
    {
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];

        sprintf( buf1, "%s", var->name );
        sprintf( buf2, "%s", var->value );
        assign_variable_room( buf1, buf2, pRoomIndex );
    } 
    }
return;
}

void	pick_skills( CHAR_DATA * ch )
{
    SKILL_LIST * skills;
    int sn;
    int pracs;
    int num = 0;
    int picked_skills = 0;
    bool picked[ MAX_SKILL ];

    picked_skills = 0;
    for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
	picked[ sn ] = FALSE;

    pracs = ch->pIndexData->prac_skills;

    while ( pracs > 0 )
    {
	if ( picked_skills >= MAX_SKILL - 3 )
	{
	    pracs = 0;
	    continue;
	}

	sn = number_range( 1, MAX_SKILL - 1 );

	while( picked[ sn ] )
	sn = number_range( 1, MAX_SKILL - 1 );

	picked_skills++;
	picked[ sn ] = TRUE;

	if ( skill_table[ sn ].spell_fun != spell_null 
	  || !IS_SET( skill_table[ sn ].target,TAR_CHAR_DEFENSIVE|TAR_CHAR_OFFENSIVE|TAR_CHAR_CURATIVE|TAR_CHAR_DAMAGING|TAR_CHAR_NONACTIVE ) )
	    continue;

	if ( pracs < get_skill_cost( ch, sn )
	  || get_skill_cost(ch, sn) < 1  
	  || ch->level < get_skill_level(ch, sn) ) 
	    continue;

	/*
	 * Mobs that gain will not pick shadowform.
	 * They can be given the spell of course.
	 */
	if ( ( IS_SET( ch->act, ACT_GAIN ) || IS_SET( ch->act, ACT_REPAIR ) ) && sn == gsn_shadowform )
	continue;

        for ( skills = ch->skills ; skills != NULL && skills->sn != sn ; skills = skills->next );

	if ( skills != NULL )
	    continue;

	skills = skill_list_alloc();
	skills->sn = sn;
	skills->next = ch->skills;
	ch->skills = skills;
	pracs -= get_skill_cost( ch, sn );
	num = 0;
    }

    picked_skills = 0;
    for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
	picked[ sn ] = FALSE;

    pracs = ch->pIndexData->prac_spells;

    while ( pracs > 0 )
    {
	if ( picked_skills >= MAX_SKILL - 3 )
	{
	    pracs = 0;
	    continue;
	}

	sn = number_range( 1, MAX_SKILL - 1 );

	while( picked[ sn ] )
	sn = number_range( 1, MAX_SKILL - 1 );

	picked_skills++;
	picked[ sn ] = TRUE;

	if ( skill_table[ sn ].spell_fun == spell_null 
	  || !IS_SET( skill_table[ sn ].target,TAR_CHAR_DEFENSIVE|TAR_CHAR_OFFENSIVE|TAR_CHAR_CURATIVE|TAR_CHAR_DAMAGING|TAR_CHAR_NONACTIVE ) )
	    continue;

	if ( pracs < get_skill_cost( ch, sn )
	  || ch->level < get_skill_level( ch, sn ) ) 
	    continue;

        for ( skills = ch->skills ; skills != NULL && skills->sn != sn ; skills = skills->next );

	if ( skills != NULL )
	    continue;

	skills = skill_list_alloc();
	skills->sn = sn;
	skills->next = ch->skills;
	ch->skills = skills;
	pracs -= get_skill_cost( ch, sn );
	num = 0;
    }
}

/*
 * Create an instance of a mobile.
 */
CHAR_DATA *create_mobile( MOB_INDEX_DATA *pMobIndex )
{
    SKILL_LIST * skills;
    CHAR_DATA *mob;
    VARIABLE_DATA *var;
    OBJ_INDEX_DATA *pobj;
    OBJ_DATA *obj;
    GLOBAL_RANDOM_DATA *grandom;
    int i, x;
 
    mobile_count++;
 
    if ( pMobIndex == NULL )
    {
        bug( "Create_mobile: NULL pMobIndex.", 0 );
        exit( 1 );
    }
	if ( char_free == NULL )
	{ 
 	        mob             = (CHAR_DATA *) alloc_perm( sizeof(*mob) );
	        char_index++;
	}
	else
	{
		mob = char_free;
		char_free = char_free->next;
	} 
    clear_char( mob );
    mob->pIndexData     = pMobIndex;
 
    strncpy( mob->name, pMobIndex->player_name, 30 );
    strncpy( mob->short_descr, pMobIndex->short_descr, 80 );
    strncpy( mob->long_descr, pMobIndex->long_descr, 80 );
    mob->description    = pMobIndex->description;
    mob->spec_fun       = pMobIndex->spec_fun;
    /* castle code */
    mob->castle_area	= pMobIndex->castle_area;
    mob->dislikes[0]	= '\0'; 
    mob->hunting[0]	= '\0'; 
    mob->mate[0]	= '\0'; 
    mob->skills		= NULL;
    mob->act2           = 0;
    mob->walkto         = 0;
    mob->vnum_test	= vnum_test++;
    mob->walkfrom       = 0;
    mob->extract	= FALSE;
    mob->guilds 	= 0;
    mob->spell_flags 	= 0;
    mob->spec_tracer    = 0;
    mob->cmd_groups	= CMD_GROUP_PLAYER|CMD_GROUP_MOBILE;
    mob->triggers = NULL;
    mob->variables = NULL;

    for ( skills = pMobIndex->skills ; skills != NULL ; skills = skills->next )
    {
	SKILL_LIST * skill_list;
	
	skill_list = skill_list_alloc();
	skill_list->sn = skills->sn;
	skill_list->next = mob->skills;
	mob->skills = skill_list;
    }

    for ( skills = pMobIndex->innate_skills ; skills != NULL ; skills = skills->next )
    {
	SKILL_LIST * skill_list;
	
	skill_list = skill_list_alloc();
	skill_list->sn = skills->sn;
	skill_list->next = mob->innate_skills;
	mob->innate_skills = skill_list;
    }

    for (x = 0; x < MAX_SECTORS; x++ )
    mob->valid_sector[x] = pMobIndex->valid_sector[x];

    if ( pMobIndex->trigger_list != NULL )
    {
    TRIGGER_LIST_DATA *trigger_list_data;
    TRIGGER_INDEX_DATA *pTrig;
    TRIGGER_DATA *trigger;

    	for ( trigger_list_data = pMobIndex->trigger_list;  trigger_list_data != NULL;  trigger_list_data = trigger_list_data->next )
    	{
            if ( ( pTrig = get_trig_index( trigger_list_data->vnum ) ) == NULL )
            {
            	bug( "Load_mobiles: vnum %d bad trigger index.", trigger_list_data->vnum );
	    	bug( "MOb %ld",pMobIndex->vnum);
	    	continue; /* There might be more good ones in the list */
            }

	    if ( pTrig->chance < number_percent() )
	    	continue;

            trigger      = trigger_alloc(); 
            strncpy( trigger->name, pTrig->name, 30 );
            strncpy( trigger->key_words, pTrig->key_words, 30 );
            strncpy( trigger->key_string, pTrig->key_string, 30 );
            strncpy( trigger->key_sim, pTrig->key_sim, 30 );
            trigger->trigger_type = pTrig->trigger_type;
            trigger->script       = pTrig->script;          /* Uses The Index of Triggers */
            trigger->current      =
            trigger->trigger_type == TRIG_BORN ? pTrig->script : NULL;
            trigger->tracer       = 0;
            trigger->timer        = 0;
            trigger->waiting      = 0;
            trigger->flags        = pTrig->flags;
            trigger->step         = pTrig->step;
            trigger->step_pre     = pTrig->step_pre;
	    REMOVE_BIT(trigger->flags, SCRIPT_HALT_RESET);
            trigger->quests       = pTrig->quests;
            trigger->quests_pre   = pTrig->quests_pre;
            trigger->gets_vnum    = pTrig->gets_vnum;
            trigger->mob_vnum     = pTrig->mob_vnum;
            trigger->vnum		= pTrig->vnum;
            trigger->bits         = SCRIPT_ADVANCE;
	    trigger->area	      = pMobIndex->load_with_area;
            trigger->next    = mob->triggers;
            mob->triggers    = trigger;
    	}
    }
    for ( var = pMobIndex->variables; var != NULL; var = var->next )
    {
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];

        sprintf( buf1, "%s", var->name );
        sprintf( buf2, "%s", var->value );
        assign_variable( buf1, buf2, mob );
    } 
        /* read from prototype */
        mob->act                = pMobIndex->act;
        mob->act2               = pMobIndex->act2;
	mob->guilds		= pMobIndex->guilds;
        mob->comm               = 0;
        mob->affected_by        = pMobIndex->affected_by;
        mob->affected_by_2      = pMobIndex->affected_by_2;
        mob->level              = pMobIndex->level;
        mob->hitroll            = pMobIndex->hitroll;
        mob->damroll            = pMobIndex->damage[DICE_BONUS];
        mob->max_hit            = roll_dice(pMobIndex->hit[DICE_NUMBER],
                                       pMobIndex->hit[DICE_TYPE])
                                  + pMobIndex->hit[DICE_BONUS];
        mob->hit                = mob->max_hit;
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	{
            mob->max_mana[i]        = roll_dice(pMobIndex->mana[DICE_NUMBER][i],
                                        pMobIndex->mana[DICE_TYPE][i])
                                  + pMobIndex->mana[DICE_BONUS][i];
            mob->mana[i]            = mob->max_mana[i];
	}
        mob->move		= 100 + roll_dice(mob->level,10);
        mob->max_move		= 100 + roll_dice(mob->level,10);
        mob->damage[DICE_NUMBER]= pMobIndex->damage[DICE_NUMBER];
        mob->damage[DICE_TYPE]  = pMobIndex->damage[DICE_TYPE];
        mob->dam_type           = pMobIndex->dam_type;
        mob->armor       	= pMobIndex->armor; 
        mob->off_flags          = pMobIndex->off_flags;
        mob->perm_imm           = pMobIndex->imm_flags;
        mob->perm_res           = pMobIndex->res_flags;
        mob->perm_vuln          = pMobIndex->vuln_flags;
        mob->imm_flags          = pMobIndex->imm_flags;
        mob->res_flags          = pMobIndex->res_flags;
        mob->vuln_flags         = pMobIndex->vuln_flags;
        mob->start_pos          = pMobIndex->start_pos;
        mob->default_pos        = pMobIndex->default_pos;
	if ( mob->start_pos == POS_DEAD )
	    mob->start_pos = POS_STANDING;
	if ( mob->default_pos == POS_DEAD )
	    mob->default_pos = POS_STANDING;
        mob->sex                = pMobIndex->sex;
        mob->mood               = pMobIndex->mood;
	mob->moveable 		= NULL;
	if (pMobIndex->moveable != NULL )
	{
	    mob->moveable = moveable_alloc();
            mob->moveable->area_start_room    = pMobIndex->moveable->area_start_room;
	    strncpy( mob->moveable->move_msg, pMobIndex->moveable->move_msg, 15 );
	    strncpy( mob->moveable->board_msg, pMobIndex->moveable->board_msg, 15 );
	    strncpy( mob->moveable->leave_msg, pMobIndex->moveable->leave_msg, 15 );
	}
	if (pMobIndex->movement_msg[0] != '\0' )
	    strncpy( mob->movement_msg, pMobIndex->movement_msg, 15 );
	mob->race = pMobIndex->race;
	if (mob->race >= top_race )
	    mob->race = 4;
        if (pMobIndex->gold[0] == 0)
            mob->gold[0] = 0;
        else
            mob->gold[0]        = number_range(pMobIndex->gold[0]/2,
                                               pMobIndex->gold[0] * 3/2);
        if (pMobIndex->gold[1] == 0)
            mob->gold[1] = 0;
        else
            mob->gold[1]        = number_range(pMobIndex->gold[1]/2,
                                               pMobIndex->gold[1] * 3/2);
        if (pMobIndex->gold[2] == 0)
            mob->gold[2] = 0;
        else
            mob->gold[2]        = number_range(pMobIndex->gold[2]/2,
                                               pMobIndex->gold[2] * 3/2);
        if (pMobIndex->gold[3] == 0)
            mob->gold[3] = 0;
        else
            mob->gold[3]        = number_range(pMobIndex->gold[3]/2,
                                               pMobIndex->gold[3] * 3/2);
        mob->form               = pMobIndex->form;
        mob->size               = pMobIndex->size;
	mob->height		= pMobIndex->height;
	if ( mob->height == 0 )
	    mob->height = get_height_size( mob->size );
	mob->weight		= pMobIndex->weight;
	if ( mob->weight == 0 )
	    mob->weight = get_weight_size( mob->size );
        mob->material           = pMobIndex->material;
	mob->hands_free		= 2;
 
        /* computed on the spot */
 
        for (i = 0; i < MAX_STATS; i ++)
            mob->perm_stat[i] = UMIN(25,11 + mob->level/4);
            
        if (IS_SET(mob->off_flags,OFF_FAST))
            mob->perm_stat[STAT_DEX] += 2;
            
        mob->perm_stat[STAT_STR] += mob->size - SIZE_MEDIUM;
        mob->perm_stat[STAT_CON] += (mob->size - SIZE_MEDIUM) / 2;

    mob->regen_timer = current_time;
    mob->position = mob->start_pos;

    pick_skills( mob );
/*
    if ( pMobIndex->equip ) 
    equip_mob( mob );
*/
    if ( !IS_SET(mob->act2, ACT2_MOB_NO_RANDOM ) )
    {
	for (grandom = random_data_list; grandom != NULL; grandom = grandom->next )
	{
    	    if ( number_range(1, grandom->chance) != 2)
	        continue;

	    if ( grandom->count >= grandom->max )
		continue;

	    if ( grandom->global > 0 )
	    {
	    	if ( ( pobj = get_obj_index( grandom->vnum ) ) == NULL )
	    	{
        	    bug( "Create_mobile: NULL pObjIndex in RANDOM.", grandom->vnum );
		    break;
	    	}

	    	obj = create_object(pobj);
	    	obj_to_char(obj, mob);
	    	grandom->count++;
	    }

	    if ( grandom->world == pMobIndex->load_with_area->world )
	    {
	    	if ( ( pobj = get_obj_index( grandom->vnum ) ) == NULL )
	    	{
        	    bug( "Create_mobile: NULL pObjIndex in RANDOM.", grandom->vnum );
		    break;
	    	}

	    	obj = create_object(pobj);
	    	obj_to_char(obj, mob);
	    	grandom->count++;
	    }

	    if ( grandom->area == pMobIndex->load_with_area->vnum )
	    {
	    	if ( ( pobj = get_obj_index( grandom->vnum ) ) == NULL )
	    	{
        	    bug( "Create_mobile: NULL pObjIndex in RANDOM.", grandom->vnum );
		    break;
	    	}

	    	obj = create_object(pobj);
	    	obj_to_char(obj, mob);
	    	grandom->count++;
	    }
	}
    }

    /* link the mob to the world list */
    mob->next           = char_list;
    char_list           = mob;

    /* link the mob to the mob list for mobile update */
    mob->mob_next_update	= mob_list_update[mob_list_number];
    mob_list_update[mob_list_number]		= mob;

    mob->mob_update_number = mob_list_number;
    mob_list_number++;
    if ( mob_list_number > 15 )
	mob_list_number = 0;

    pMobIndex->count++;
    return mob;
}
 
/* duplicate a mobile exactly -- except inventory */
void clone_mobile(CHAR_DATA *parent, CHAR_DATA *clone)
{
    int i;
    AFFECT_DATA *paf;
    return; /* this command should go away! */
 
    if ( parent == NULL || clone == NULL || !IS_NPC(parent))
        return;
    
    /* start fixing values */ 
    strncpy( clone->name, parent->name, 30 );
    clone->version      = parent->version;
    strncpy( clone->short_descr, parent->short_descr, 80 );
    strncpy( clone->long_descr, parent->long_descr, 80 );
    clone->description  = str_dup(parent->description);
    clone->sex          = parent->sex;
    clone->race         = parent->race;
    clone->level        = parent->level;
    clone->timer        = parent->timer;
    clone->wait         = parent->wait;
    clone->hit          = parent->hit;
    clone->max_hit      = parent->max_hit;
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        clone->mana[i]      = parent->mana[i];
        clone->max_mana[i]  = parent->max_mana[i];
    } 
    clone->move         = parent->move;
    clone->max_move     = parent->max_move;
    clone->gold[0]      = parent->gold[0];
    clone->gold[1]      = parent->gold[1];
    clone->gold[2]      = parent->gold[2];
    clone->gold[3]      = parent->gold[3];
    clone->exp          = parent->exp;
    clone->kills        = parent->kills;
    clone->act          = parent->act;
    clone->act2         = parent->act2;
    clone->guilds       = parent->guilds;
    clone->comm         = parent->comm;
    clone->imm_flags    = parent->imm_flags;
    clone->res_flags    = parent->res_flags;
    clone->vuln_flags   = parent->vuln_flags;
    clone->invis_level  = parent->invis_level;
    clone->affected_by  = parent->affected_by;
    clone->affected_by_2  = parent->affected_by_2;
    clone->regen_timer  = parent->regen_timer;
    clone->position     = parent->position;
    clone->moveable	= NULL;
    clone->practice     = parent->practice;
    clone->saving_throw = parent->saving_throw;
    clone->hitroll      = parent->hitroll;
    clone->damroll      = parent->damroll;
    clone->wimpy        = parent->wimpy;
    clone->form         = parent->form;
    clone->size        = parent->size;
    clone->height	= parent->height;
    clone->weight	= parent->weight;
    clone->material     = parent->material;
    clone->off_flags    = parent->off_flags;
    clone->dam_type     = parent->dam_type;
    clone->start_pos    = parent->start_pos;
    clone->default_pos  = parent->default_pos;
    clone->spec_fun     = parent->spec_fun;
    
    clone->armor	= parent->armor;
 
    for (i = 0; i < MAX_STATS; i++)
    {
        clone->perm_stat[i]     = parent->perm_stat[i];
        clone->mod_stat[i]      = parent->mod_stat[i];
    }
 
    for (i = 0; i < 3; i++)
        clone->damage[i]        = parent->damage[i];
 
    /* now add the affects */
    for (paf = parent->affected; paf != NULL; paf = paf->next)
        affect_to_char(clone,paf);
 
}
 
/*
 * Create an instance of an object.
 */
OBJ_DATA *create_object( OBJ_INDEX_DATA *pObjIndex )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    AFFECT_DATA *paf;
    VARIABLE_DATA *var;
    char buf[MAX_STRING_LENGTH];

    object_count++;
 
    if ( pObjIndex == NULL )
    {
        bug( "Create_object: NULL pObjIndex.", 0 );
	pObjIndex = get_obj_index(15300); /* Added this for debugging ( bread ) */
    }

    if ( pObjIndex->approve_data != NULL && !IS_SET( pObjIndex->approve_data->flags, APPROVED ) )
    {
	sprintf( buf, "Create_object: Unapproved Object: %ld", pObjIndex->vnum );
	bug( buf, 0 );
    } 

    if ( obj_free == NULL )
    {
        obj             = (OBJ_DATA *) alloc_perm( sizeof(*obj) );
	obj_index++;
    }
    else
    {
        obj             = obj_free;
        obj_free        = obj_free->next;
    }
 
    *obj                = obj_zero;
    clear_obj( obj );
    obj->pIndexData     = pObjIndex;
    obj->in_room        = NULL;
    obj->in_obj         = NULL;
    obj->equip		= NULL;
 
    /* Objects that get renamed need to be free able.
     * Examples are extracting ore.
     */
    if (IS_SET(pObjIndex->special_flags, ITEM_SPEC_STRING ) )
    {
    	strncpy( obj->name, pObjIndex->name, 30 );
    	strncpy( obj->short_descr, pObjIndex->short_descr, 80 );
    	strncpy( obj->description, pObjIndex->description, 80 );
    }
    else
    {
    	strncpy( obj->name, pObjIndex->name, 30 );
    	strncpy( obj->short_descr, pObjIndex->short_descr, 80 );
    	strncpy( obj->description, pObjIndex->description, 80 );
    }
    obj->material       = pObjIndex->material;
    obj->extra_flags    = pObjIndex->extra_flags;
    obj->special_flags  = pObjIndex->special_flags;
    obj->spec_fun	= pObjIndex->spec_fun;
    obj->shop_type	= pObjIndex->shop_type;
    obj->timer		= pObjIndex->timer;
    obj->light_data	= NULL;
    if ( pObjIndex->light_data != NULL )
    {
	obj->light_data = light_alloc();
	obj->light_data->flags = pObjIndex->light_data->flags;
 	obj->light_data->fuel = pObjIndex->light_data->fuel;
	obj->light_data->max_fuel = pObjIndex->light_data->max_fuel;
	obj->light_data->brightness = pObjIndex->light_data->brightness;
    }
    obj->cont_data	= NULL;
    if ( pObjIndex->cont_data != NULL )
    {
	SPELL_LIST * spells;
	SPELL_LIST * spell;
	LOCK_DATA * lock_list;
	LOCK_DATA * lock;
	RESET_DATA * reset;
	RESET_DATA * pReset;
	RESET_DATA * last_reset=NULL;

	obj->cont_data = cont_alloc();
	obj->cont_data->capacity  = pObjIndex->cont_data->capacity;
	obj->cont_data->flags = pObjIndex->cont_data->flags;

	if ( pObjIndex->cont_data->trap != NULL )
	{
	    obj->cont_data->trap = trap_alloc();
	    obj->cont_data->trap->flags = pObjIndex->cont_data->trap->flags;

	    for ( spells = pObjIndex->cont_data->trap->spells ; spells != NULL ; spells = spells->next )
	    {
		spell = spell_alloc();
		spell->level = spells->level;
		spell->spell = spells->spell;
		spell->next = obj->cont_data->trap->spells;
		obj->cont_data->trap->spells = spell;
	    }
	}

	for ( lock_list = pObjIndex->cont_data->lock ; lock_list != NULL ; lock_list = lock_list->next )
	{
	    lock = lock_alloc();
	    lock->vnum = lock_list->vnum;
	    lock->lock_info = lock_list->lock_info;
	    lock->pick_penalty = lock_list->pick_penalty;
	    lock->next = obj->cont_data->lock;
	    obj->cont_data->lock = lock;
	}

	for ( reset = pObjIndex->cont_data->inside ; reset != NULL ; reset = reset->next )
	{
	    pReset = reset_alloc();
	    pReset->command	= reset->command;
	    pReset->arg1	= reset->arg1;
	    pReset->arg2	= reset->arg2;
	    pReset->arg3	= reset->arg3;
	    pReset->vnum	= reset->vnum;
	    pReset->chance	= reset->chance;
	    pReset->next	= NULL;
	    if ( last_reset == NULL )
	        obj->cont_data->inside = pReset;
	    else
		last_reset->next = pReset;

	    last_reset = pReset;
	}
    }
    obj->gate_data 	= NULL;
    if ( pObjIndex->gate_data != NULL )
    {
	CHAR_DATA *mob;
	OBJ_DATA *obj_to;

	obj->gate_data = gate_alloc();
	obj->gate_data->flags = pObjIndex->gate_data->flags;
	obj->gate_data->vnum  = pObjIndex->gate_data->vnum;
	obj->gate_data->ave_timer = pObjIndex->gate_data->ave_timer;
	obj->gate_data->leads_to = NULL;
	if ( !IS_SET( obj->gate_data->flags, GATE_TO_MOB | GATE_TO_OBJ ) )
	    obj->gate_data->leads_to = (void *) ( get_room_index( obj->gate_data->vnum ) );
	else if ( IS_SET( obj->gate_data->flags, GATE_TO_MOB ) )
	{
	    for ( mob = char_list ; mob != NULL && ( !IS_NPC( mob ) || mob->pIndexData->vnum != obj->gate_data->vnum ) ; mob = mob->next );

	    obj->gate_data->leads_to = (void *) mob;
	}
	else if ( IS_SET( obj->gate_data->flags, GATE_TO_OBJ ) )
	{
	    for ( obj_to = object_list ; obj_to != NULL && obj_to->pIndexData->vnum != obj->gate_data->vnum ; obj_to = obj_to->next );

	    obj->gate_data->leads_to = (void *) obj_to;
	}
    }
    obj->edible_data    = NULL;
    if ( pObjIndex->edible_data != NULL )
    {
	SPELL_LIST * spell;
	SPELL_LIST * spells;
        obj->edible_data = edible_alloc();
	obj->edible_data->type = pObjIndex->edible_data->type;
	obj->edible_data->flags = pObjIndex->edible_data->flags;
	obj->edible_data->amount = pObjIndex->edible_data->amount;
	obj->edible_data->max_amount = pObjIndex->edible_data->max_amount;

	for ( spells = pObjIndex->edible_data->spells ; spells != NULL ; spells = spells->next )
	{
	    spell = spell_alloc();
	    spell->spell = spells->spell;
	        spell->level = number_fuzzy( number_fuzzy( spells->level ) );
	    spell->next  = obj->edible_data->spells;
	    obj->edible_data->spells = spell;
	}

    }
    obj->wear_data	= NULL;
    if ( pObjIndex->wear_data != NULL && pObjIndex->wear_data->wear_flags )
    {
	obj->wear_data 	 = wear_alloc();
	obj->wear_data->wear_flags = pObjIndex->wear_data->wear_flags;
	obj->wear_data->wear_loc = WEAR_NONE;
	obj->wear_data->weight = pObjIndex->wear_data->weight;
	if (obj->wear_data->weight < 1 )
	obj->wear_data->weight = 1;
	obj->wear_data->size   = pObjIndex->wear_data->size;
	obj->wear_data->ac     = pObjIndex->wear_data->ac;
        obj->wear_data->cost       = pObjIndex->wear_data->cost;
	obj->wear_data->magic_data = NULL;
	if ( pObjIndex->wear_data->magic_data != NULL )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spells;

	    obj->wear_data->magic_data = magic_alloc();
	    obj->wear_data->magic_data->flags = pObjIndex->wear_data->magic_data->flags;
	    obj->wear_data->magic_data->charges = pObjIndex->wear_data->magic_data->charges;
	    obj->wear_data->magic_data->charges = number_fuzzy( obj->wear_data->magic_data->charges );
	    obj->wear_data->magic_data->max_charges = pObjIndex->wear_data->magic_data->max_charges;
	    obj->wear_data->magic_data->type = pObjIndex->wear_data->magic_data->type;
	    obj->wear_data->magic_data->spells = NULL;

	    for ( spells = pObjIndex->wear_data->magic_data->spells ; spells != NULL ; spells = spells->next )
	    {
		spell = spell_alloc();
		spell->next = obj->wear_data->magic_data->spells;
		spell->level = spells->level;
		spell->spell = spells->spell;
		obj->wear_data->magic_data->spells = spell;
	    }

	}

	obj->wear_data->weapon_data = NULL;
 	if ( pObjIndex->wear_data->weapon_data != NULL )
	{
	    obj->wear_data->weapon_data = weapon_alloc();
	    obj->wear_data->weapon_data->type = pObjIndex->wear_data->weapon_data->type;
	    obj->wear_data->weapon_data->dice = pObjIndex->wear_data->weapon_data->dice;
	    obj->wear_data->weapon_data->die = pObjIndex->wear_data->weapon_data->die;
	    obj->wear_data->weapon_data->dam_type = pObjIndex->wear_data->weapon_data->dam_type;
	    obj->wear_data->weapon_data->flags = pObjIndex->wear_data->weapon_data->flags;
	}
	obj->wear_data->affected = NULL;
 	if ( pObjIndex->wear_data->affected != NULL )
          for (paf = pObjIndex->wear_data->affected; paf != NULL; paf = paf->next) 
            affect_to_obj(obj,paf);

    }
    if ( pObjIndex->trigger_list != NULL )
    {
    TRIGGER_LIST_DATA *trigger_list_data;
    TRIGGER_INDEX_DATA *pTrig;
    TRIGGER_DATA *trigger;

    for ( trigger_list_data = pObjIndex->trigger_list;  trigger_list_data != NULL;  trigger_list_data = trigger_list_data->next )
    {
        if ( ( pTrig = get_trig_index( trigger_list_data->vnum ) ) == NULL )
        {
            bug( "Load_objects: vnum %d bad trigger index.", trigger_list_data->vnum );
	    bug( "Obj %ld", pObjIndex->vnum);
	    continue; /* There might be more good ones in the list */
        }
	if ( pTrig->chance < number_percent() )
	    continue;

        trigger      = trigger_alloc(); 

        strncpy( trigger->name, pTrig->name, 30 );
        strncpy( trigger->key_words, pTrig->key_words, 30 );
        strncpy( trigger->key_string, pTrig->key_string, 30 );
        strncpy( trigger->key_sim, pTrig->key_sim, 30 );
        trigger->trigger_type = pTrig->trigger_type;
        trigger->script       = pTrig->script;          /* Uses The Index of Triggers */
        trigger->current      =
             trigger->trigger_type == TRIG_BORN ? pTrig->script : NULL;
        trigger->tracer       = 0;
        trigger->timer        = 0;
        trigger->waiting      = 0;
        trigger->flags        = pTrig->flags;
        trigger->step         = pTrig->step;
        trigger->step_pre     = pTrig->step_pre;
	REMOVE_BIT(trigger->flags, SCRIPT_HALT_RESET);
        trigger->gets_vnum    = pTrig->gets_vnum;
        trigger->mob_vnum     = pTrig->mob_vnum;
        trigger->vnum		= pTrig->vnum;
        trigger->bits         = SCRIPT_ADVANCE;
	trigger->area	      = pObjIndex->in_area;

        trigger->next    = obj->triggers;
        obj->triggers    = trigger;
    }
    }
    for ( var = pObjIndex->variables; var != NULL; var = var->next )
    {
        char buf1[MAX_STRING_LENGTH];
        char buf2[MAX_STRING_LENGTH];

        sprintf( buf1, "%s", var->name );
        sprintf( buf2, "%s", var->value );
        assign_variable_obj( buf1, buf2, obj );
    } 
    obj->condition      = pObjIndex->condition;
    obj->repaired	= 0;

    obj->next           = object_list;
    object_list         = obj;
    pObjIndex->count++;

    set_obj_points( obj ); 

    return obj;
}
 
/* duplicate an object exactly -- except contents */
void clone_object(OBJ_DATA *parent, OBJ_DATA *clone)
{
    AFFECT_DATA *paf;
    EXTRA_DESCR_DATA *ed,*ed_new; 
 
    if (parent == NULL || clone == NULL)
        return;
 
    /* start fixing the object */
    strncpy( clone->name, parent->name, 30 );
    strncpy( clone->short_descr, parent->short_descr, 80 );
    strncpy( clone->description, parent->description, 80 );
    clone->extra_flags  = parent->extra_flags;
    clone->special_flags = parent->special_flags;
    clone->spec_fun	= parent->spec_fun;
    clone->gate_data 	= NULL;
    clone->light_data	= NULL;
    if ( parent->light_data != NULL )
    {
	clone->light_data = light_alloc();
	clone->light_data->flags = parent->light_data->flags;
 	clone->light_data->fuel = parent->light_data->fuel;
	clone->light_data->max_fuel = parent->light_data->max_fuel;
	clone->light_data->brightness = parent->light_data->brightness;
    }
    clone->cont_data	= NULL;
    if ( parent->cont_data != NULL )
    {
	SPELL_LIST * spells;
	SPELL_LIST * spell;
	LOCK_DATA * lock_list;
	LOCK_DATA * lock;
	RESET_DATA * reset;
	RESET_DATA * pReset;
	RESET_DATA * last_reset=NULL;

	clone->cont_data = cont_alloc();
	clone->cont_data->capacity  = parent->cont_data->capacity;
	clone->cont_data->flags = parent->cont_data->flags;

	if ( parent->cont_data->trap != NULL )
	{
	    clone->cont_data->trap = trap_alloc();
	    clone->cont_data->trap->flags = parent->cont_data->trap->flags;

	    for ( spells = parent->cont_data->trap->spells ; spells != NULL ; spells = spells->next )
	    {
		spell = spell_alloc();
		spell->level = spells->level;
		spell->spell = spells->spell;
		spell->next = clone->cont_data->trap->spells;
		clone->cont_data->trap->spells = spell;
	    }
	}

	for ( lock_list = parent->cont_data->lock ; lock_list != NULL ; lock_list = lock_list->next )
	{
	    lock = lock_alloc();
	    lock->vnum = lock_list->vnum;
	    lock->lock_info = lock_list->lock_info;
	    lock->pick_penalty = lock_list->pick_penalty;
	    lock->next = clone->cont_data->lock;
	    clone->cont_data->lock = lock;
	}

	for ( reset = parent->cont_data->inside ; reset != NULL ; reset = reset->next )
	{
	    pReset = reset_alloc();
	    pReset->command	= reset->command;
	    pReset->arg1	= reset->arg1;
	    pReset->arg2	= reset->arg2;
	    pReset->arg3	= reset->arg3;
	    pReset->vnum	= reset->vnum;
	    pReset->chance	= reset->chance;
	    pReset->next	= NULL;
	    if ( last_reset == NULL )
	        clone->cont_data->inside = pReset;
	    else
		last_reset->next = pReset;

	    last_reset = pReset;
	}
    }
    if ( parent->gate_data != NULL )
    {
	GATE_DATA * gate;

	gate = gate_alloc();
	gate->flags = parent->gate_data->flags;
	gate->vnum  = parent->gate_data->vnum;
	gate->ave_timer = parent->gate_data->ave_timer;
	if ( !IS_SET( gate->flags, GATE_TO_MOB | GATE_TO_OBJ ) )
	    gate->leads_to = (void *) ( get_room_index( gate->vnum ) );
	if ( !IS_SET( gate->flags, GATE_TO_MOB ) )
	    gate->leads_to = (void *) ( get_mob_index( gate->vnum ) );
	if ( !IS_SET( gate->flags, GATE_TO_OBJ ) )
	    gate->leads_to = (void *) ( get_obj_index( gate->vnum ) );
	clone->gate_data = gate;
    }
    clone->edible_data = NULL;
    if ( parent->edible_data != NULL )
    {
	SPELL_LIST * spell;
	SPELL_LIST * spells;

	clone->edible_data = edible_alloc();
	clone->edible_data->type = parent->edible_data->type;
	clone->edible_data->flags = parent->edible_data->flags;
	clone->edible_data->amount = parent->edible_data->amount;
	clone->edible_data->max_amount = parent->edible_data->max_amount;

	for ( spells = parent->edible_data->spells ; spells != NULL ; spells = spells->next )
	{
	    spell = spell_alloc();
	    spell->spell = spells->spell;
	    spell->level = spells->level;
	    spell->next  = clone->edible_data->spells;
	    clone->edible_data->spells = spell;
	}
    }
    clone->wear_data	= NULL;
    if ( parent->wear_data != NULL )
    {
	clone->wear_data 	 = wear_alloc();
	clone->wear_data->wear_flags = parent->wear_data->wear_flags;
	clone->wear_data->wear_loc = WEAR_NONE;	
	clone->wear_data->weight = parent->wear_data->weight;
	clone->wear_data->size   = parent->wear_data->size;
        clone->wear_data->cost   = parent->wear_data->cost;
	clone->wear_data->ac     = parent->wear_data->ac;

	clone->wear_data->magic_data = NULL;
	if ( parent->wear_data->magic_data != NULL )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spells;

	    clone->wear_data->magic_data = magic_alloc();
	    clone->wear_data->magic_data->flags = parent->wear_data->magic_data->flags;
	    clone->wear_data->magic_data->charges = parent->wear_data->magic_data->charges;
	    clone->wear_data->magic_data->max_charges = parent->wear_data->magic_data->max_charges;
	    clone->wear_data->magic_data->type = parent->wear_data->magic_data->type;
	    clone->wear_data->magic_data->spells = NULL;

	    for ( spells = parent->wear_data->magic_data->spells ; spells != NULL ; spells = spells->next )
	    {
		spell = spell_alloc();
		spell->next = clone->wear_data->magic_data->spells;
		spell->level = spells->level;
		spell->spell = spells->spell;
		clone->wear_data->magic_data->spells = spell;
	    }

	}

	clone->wear_data->weapon_data = NULL;
	if ( parent->wear_data->weapon_data != NULL )
	{
	    clone->wear_data->weapon_data = weapon_alloc();
	    clone->wear_data->weapon_data->dice = parent->wear_data->weapon_data->dice;
	    clone->wear_data->weapon_data->die = parent->wear_data->weapon_data->die;
	    clone->wear_data->weapon_data->type = parent->wear_data->weapon_data->type;
	    clone->wear_data->weapon_data->flags = parent->wear_data->weapon_data->flags;
	    clone->wear_data->weapon_data->dam_type = parent->wear_data->weapon_data->dam_type;
	}
    }
    clone->condition    = parent->condition;
    clone->material     = parent->material;
    clone->timer        = parent->timer;
    clone->shop_type    = parent->shop_type;
    clone->repaired	= parent->repaired;

    /* affects */
    clone->enchanted    = parent->enchanted;
  
    if ( parent->wear_data != NULL )
    for (paf = parent->wear_data->affected; paf != NULL; paf = paf->next) 
        affect_to_obj(clone,paf);
 
    /* extended desc */

    clone->extra_descr = NULL;
    for (ed = parent->extra_descr; ed != NULL; ed = ed->next)
    {
        ed_new              = new_extra_descr();
 
        strncpy( ed_new->keyword, ed->keyword, 30 );
        ed_new->description     = str_dup( ed->description );
        ed_new->next            = clone->extra_descr;
        clone->extra_descr      = ed_new;
    }
 
    set_obj_points( clone );
}
 
 
/*
 * Clear a new character.
 */
void clear_char( CHAR_DATA *ch )
{
    static CHAR_DATA ch_zero;
    int i;
 
    *ch                         = ch_zero;
    ch->name[0]                 = '\0';
    ch->short_descr[0]		= '\0';
    ch->long_descr[0]           = '\0';
    ch->description             = &str_empty[0];
    ch->prefix[0]               = '\0';
    ch->logon                   = current_time;
    ch->lines                   = PAGELEN;
    ch->guilds                  = DEFAULT;
    ch->edit_flags              = 0;
    ch->armor 			= 100;
    ch->comm                    = 0;
    ch->regen_timer		= current_time;
    ch->position                = POS_STANDING;
    ch->practice                = 0;
    ch->edit_menu               = 0;
    ch->hit                     = 20;
    ch->max_hit                 = 20;
    /*for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        ch->mana[i]                    = race_table[ ch->race ][ ch->sex ].mana_die[ i ] * race_table[ ch->race ][ ch->sex ].mana_dice[ i ] + race_table[ ch->race ][ ch->sex ].mana_bonus[ i ];
	ch->mana[i]		      *= 5; 
        ch->max_mana[i]                = ch->mana[i];
    }*/
    ch->move                    = 100;
    ch->max_move                = 100;
    ch->tmpact			= 0;
    ch->infoact			= 0xFFFFFFFF;
    ch->follower		= NULL;
    ch->next_follower		= NULL;
    ch->group			= NULL;
    ch->next_in_group		= NULL;
    ch->race			= 1;
    for (i = 0; i < MAX_STATS; i ++)
    {
        ch->perm_stat[i] = 13; 
        ch->mod_stat[i] = 0;
    }
    return;
}
 
/*
 * Clear a new Object.
 */
void clear_obj( OBJ_DATA *obj )
{
    static OBJ_DATA obj_zero;
 
    *obj               =   obj_zero;
    obj->extra_descr   =   NULL; 
    obj->short_descr[0]	= '\0';
    obj->name[0]       =   '\0';
    obj->description[0] =   '\0';
    obj->extra_flags   =   0;
    obj->special_flags   =   0;
    obj->wear_data     =   NULL;
    obj->gate_data     =   NULL;
    obj->edible_data   =   NULL;
    obj->light_data    =   NULL;
    obj->cont_data     =   NULL;
    obj->spec_fun      =   NULL;
    obj->condition     =   0;
    obj->material      =   0;
    obj->timer	       =   0;
    obj->donate_timer  =   0;
    obj->shop_type       =   0;
    obj->carried_by    =   NULL;
    obj->owner[0]  	=   '\0';
    obj->enchanted  	=   0;
    obj->points		=   0;
    obj->mpoints	=   0;
    return;
}
 
 
 
/*
 * Free a character.
 */
void free_char( CHAR_DATA *ch )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    TRIGGER_DATA *trigger;
    TRIGGER_DATA *trigger_next;
    VARIABLE_DATA *var;
    VARIABLE_DATA *var_next;
    READ_NOTES *read_notes;
    READ_NOTES *read_notes_next;
    SKILL_LIST *skills;
    SKILL_LIST *skills_next;
    CHAR_QUESTS *quests;
    CHAR_QUESTS *quest_next;

    for ( trigger = ch->triggers; trigger != NULL; trigger = trigger_next )
    {
    trigger_next     = trigger->next;
    trigger->current = NULL;
    trigger->tracer  = 0;
    free_trigger( trigger );
    }

    for ( var = ch->variables; var != NULL; var = var_next )
    {
    var_next = var->next;
    free_variable( var );
    }

    if ( IS_NPC( ch ) )
    {
    	for ( skills = ch->innate_skills; skills != NULL; skills = skills_next )
    	{
    	    skills_next = skills->next;
    	    free_skill_list( skills );
    	}
    }

    for ( skills = ch->skills; skills != NULL; skills = skills_next )
    {
    skills_next = skills->next;
    free_skill_list( skills );
    }

    for ( quests = ch->char_quests; quests != NULL; quests = quest_next )
    {
    quest_next = quests->next;
    free_char_quests(quests);
    }

    
    if (IS_NPC(ch))
        mobile_count--;
 
    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
        obj_next = obj->next_content;
        extract_obj( obj );
    }

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
        paf_next = paf->next;
        affect_remove( ch, paf );
    }
    
    if (!IS_NPC(ch) )
   	free_string( ch->description );

    if ( ch->pcdata != NULL )
    {
        ch->pcdata->next = pcdata_free;
        pcdata_free      = ch->pcdata;
    }
    if ( !IS_NPC(ch) )
    {
    	for ( read_notes = ch->pcdata->read_notes ; read_notes != NULL ; read_notes = read_notes_next )
    	{
            read_notes_next = read_notes->next;
	    free_read_notes(read_notes);
    	}
    }
 
    ch->next         = char_free;
    char_free        = ch;
    return;
}
 
 
 
/*
 * Get an extra description from a list.
 */
char *get_extra_descr( const char *name, EXTRA_DESCR_DATA *ed )
{
    for ( ; ed != NULL; ed = ed->next )
    {
        if ( is_name( (char *) name, ed->keyword ) )
            return ed->description;
    }
    return NULL;
}
 
 
/*
 * Translates triggers virtual number to its trigger index struct.
 * Hash table lookup.
 */
TRIGGER_INDEX_DATA *get_trig_index( int vnum )
{
    TRIGGER_INDEX_DATA *pTrigIndex;
 
    for ( pTrigIndex  = trig_index_hash[vnum % MAX_KEY_HASH];
          pTrigIndex != NULL;
          pTrigIndex  = pTrigIndex->next )
    {
        if ( pTrigIndex->vnum == vnum )
            return pTrigIndex;
    }
 
    if ( fBootDb )
    {
        bug( "Get_trig_index: bad vnum %d.", vnum );
    }
 
    return NULL;
}
 
 
/*
 * Translates mob virtual number to its mob index struct.
 * Hash table lookup.
 */
MOB_INDEX_DATA *get_mob_index( long vnum )
{
    MOB_INDEX_DATA *pMobIndex;
 
    for ( pMobIndex  = mob_index_hash[vnum % MAX_KEY_HASH];
          pMobIndex != NULL;
          pMobIndex  = pMobIndex->next )
    {
        if ( pMobIndex->vnum == vnum )
            return pMobIndex;
    }
 
    if ( fBootDb )
    {
        bug( "Get_mob_index: bad vnum %d.", vnum );
        exit( 1 );
    }
 
    return NULL;
}
 
 
 
/*
 * Translates mob virtual number to its obj index struct.
 * Hash table lookup.
 */
OBJ_INDEX_DATA *get_obj_index( long vnum )
{
    OBJ_INDEX_DATA *pObjIndex;
 
    if ( vnum < 1 )
    return NULL;
    for ( pObjIndex  = obj_index_hash[vnum % MAX_KEY_HASH];
          pObjIndex != NULL;
          pObjIndex  = pObjIndex->next )
    {
        if ( pObjIndex->vnum == vnum )
            return pObjIndex;
    }
 
    if ( fBootDb )
    {
        bug( "Get_obj_index: bad vnum %d.", vnum );
        exit( 1 );
    }
 
    return NULL;
}
 
 
 
/*
 * Translates mob virtual number to its room index struct.
 * Hash table lookup.
 */
ROOM_INDEX_DATA *get_room_index( long vnum )
{
    ROOM_INDEX_DATA *pRoomIndex;
    if ( vnum < 0 || vnum > MAX_VNUM )
	return NULL;
 
    for ( pRoomIndex  = room_index_hash[vnum % MAX_KEY_HASH];
          pRoomIndex != NULL;
          pRoomIndex  = pRoomIndex->next )
    {
        if ( pRoomIndex->vnum == vnum )
            return pRoomIndex;
    }
 
    if ( fBootDb )
    {
        bug( "Get_room_index: bad vnum %ld.", vnum );
        exit( 1 );
    }

    return NULL;
}
 
 
 
/*
 * Read a letter from a file.
 */
char fread_letter( FILE *fp )
{
    char c;
 
    do
    {
        c = getc( fp );
    }
    while ( isspace(c) );
 
    return c;
}
 
 
 
/*
 * Read a number from a file.
 */
int fread_number( FILE *fp )
{
    int number;
    bool sign;
    char c;
 
    do
    {
        c = getc( fp );
    }
    while ( isspace(c) );
 
    number = 0;
 
    sign   = FALSE;
    if ( c == '+' )
    {
        c = getc( fp );
    }
    else if ( c == '-' )
    {
        sign = TRUE;
        c = getc( fp );
    }
 
    if ( !isdigit(c) )
    {
	char buf[MAX_INPUT_LENGTH];

	sprintf( buf, "Fread_number: %c %d\n", c, c );
	log_string( buf );
        bug( "Fread_number: bad format.", 0 );
        exit( 1 );
    }
 
    while ( isdigit(c) )
    {
        number = number * 10 + c - '0';
        c      = getc( fp );
    }
 
    if ( sign )
        number = 0 - number;
 
    if ( c == '|' )
        number += fread_number( fp );
    else if ( c != ' ' )
        ungetc( c, fp );
 
    return number;
}
 
long fread_flag( FILE *fp)
{
    int number;
    char c;
 
    do
    {
        c = getc(fp);
    }
    while ( isspace(c));
 
    number = 0;
 
    if (!isdigit(c))
    {
        while (('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z'))
        {
            number += flag_convert(c);
            c = getc(fp);
        }
    }
 
    while (isdigit(c))
    {
        number = number * 10 + c - '0';
        c = getc(fp);
    }
 
    if (c == '|')
        number += fread_flag(fp);
 
    else if  ( c != ' ')
        ungetc(c,fp);
 
    return number;
}
 
long flag_convert(char letter )
{
    long bitsum = 0;
    char i;
 
    if ('A' <= letter && letter <= 'Z') 
    {
        bitsum = 1;
        for (i = letter; i > 'A'; i--)
            bitsum *= 2;
    }
    else if ('a' <= letter && letter <= 'z')
    {
        bitsum = 67108864; /* 2^26 */
        for (i = letter; i > 'a'; i --)
            bitsum *= 2;
    }
 
    return bitsum;
}
 
 
 
 
/*
 *   Non share version
 */
char *fread_string_ns( FILE *fp )
{
    char *plast;
    char c;
 
    plast = top_string + sizeof(char *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
        bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
        exit( 1 );
    }
 
    do
    {
        c = getc( fp );
    }
    while ( isspace(c) );
 
    if ( ( *plast++ = c ) == '~' )
        return &str_empty[0];
 
    for ( ;; )
    {
 
        switch ( *plast = getc(fp) )
        {
        default:
            plast++;
            break;
 
        case EOF:
            bug( "Fread_string: EOF", 0 );
            return NULL;
            break;
 
        case '\n':
            plast++;
            *plast++ = '\r';
            break;
 
        case '\r':
            break;
 
        case '~':
            plast++;
            {
                char *pString;
 
                plast[-1] = '\0';

                if ( fBootDb )
                {
                    pString             = top_string;
                    top_string          = plast;
                    nAllocString += 1;
                    sAllocString += top_string - pString;
                    return pString + sizeof(char *);
                }
                else
                {
                    return str_dup( top_string + sizeof(char *) );
                }
            }
        }
    }
}
 
char *fread_string_eol( FILE *fp )
{
    static bool char_special[256-EOF];
    char *plast;
    char c;
 
    if ( char_special[EOF-EOF] != TRUE )
    {
        char_special[EOF -  EOF] = TRUE;
        char_special['\n' - EOF] = TRUE;
        char_special['\r' - EOF] = TRUE;
    }
 
    plast = top_string + sizeof(char *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
        bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
        exit( 1 );
    }
 
    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
        c = getc( fp );
    }
    while ( isspace(c) );
 
    if ( ( *plast++ = c ) == '\n')
        return &str_empty[0];
 
    for ( ;; )
    {
        if ( !char_special[ ( *plast++ = getc( fp ) ) - EOF ] )
            continue;
 
        switch ( plast[-1] )
        {
        default:
            break;
 
        case EOF:
            bug( "Fread_string_eol  EOF", 0 );
/*
            exit( 1 );
            break;
*/ 
        case '\n':  case '\r':
            {
                union
                {
                    char *      pc;
                    char        rgc[sizeof(char *)];
                } u1;
                int ic;
                int iHash;
                char *pHash;
                char *pHashPrev;
                char *pString;
 
                plast[-1] = '\0';
                iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
                for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
                {
                    for ( ic = 0; ic < (signed) sizeof(char *); ic++ )
                        u1.rgc[ic] = pHash[ic];
                    pHashPrev = u1.pc;
                    pHash    += sizeof(char *);
 
                    if ( top_string[sizeof(char *)] == pHash[0]
                    &&   !str_cmp( top_string+sizeof(char *)+1, pHash+1 ) )
                        return pHash;
                }
 
                if ( fBootDb )
                {
                    pString             = top_string;
                    top_string          = plast;
                    u1.pc               = string_hash[iHash];
                    for ( ic = 0; ic < (signed) sizeof(char *); ic++ )
                        pString[ic] = u1.rgc[ic];
                    string_hash[iHash]  = pString;
 
                    nAllocString += 1;
                    sAllocString += top_string - pString;
                    return pString + sizeof(char *);
                }
                else
                {
                    return str_dup( top_string + sizeof(char *) );
                }
            }
        }
    }
}
 
 
 
/*
 * Read to end of line (for comments).
 */
void fread_to_eol( FILE *fp )
{
    char c;
 
    do
    {
        c = getc( fp );
    }
    while ( c != '\n' && c != '\r' );
 
    do
    {
        c = getc( fp );
    }
    while ( c == '\n' || c == '\r' );
 
    ungetc( c, fp );
    return;
}
 
 
 
/*
 * Read one word (into static buffer).
 */
char *fread_word( FILE *fp )
{
    static char word[MAX_INPUT_LENGTH];
    char *word2;
    char *pword;
    char cEnd;
 
    do
    {
        cEnd = getc( fp );
    }
    while ( isspace( cEnd ) );
 
    if ( cEnd == '\'' || cEnd == '"' )
    {
        pword   = word;
	word2   = word;
    }
    else
    {
	word2   = word;
        word[0] = cEnd;
        pword   = word+1;
        cEnd    = ' ';
    }
 
    for ( ; pword < word + MAX_INPUT_LENGTH; pword++ )
    {
        *pword = getc( fp );
        if ( cEnd == ' ' ? isspace(*pword) : *pword == cEnd )
        {
            if ( cEnd == ' ' )
                ungetc( *pword, fp );
            *pword = '\0';
            return word;
        }
    }
 
    bug( "Fread_word: word too long.", 0 );
    exit( 1 );
    return NULL;
}
 
/*
 * Allocate some ordinary memory,
 *   with the expectation of freeing it someday.
 */
void *alloc_mem( long sMem )
{
    void *pMem;
    long *magic;
    int iList;

    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
        if ( sMem <= rgSizeList[iList] )
            break;
    }

    if ( iList == MAX_MEM_LIST )
    {
        bug( "Alloc_mem: size %d too large.", sMem );
        exit( 1 );
    }

    if ( rgFreeList[iList] == NULL )
    {
        pMem              = alloc_perm( rgSizeList[iList] );
	top_alloc_mem++;
    }
    else
    {
        pMem              = rgFreeList[iList];
        rgFreeList[iList] = * ((void **) rgFreeList[iList]);
    }

    magic = (long *) pMem;
    *magic = MAGIC_NUM;
    pMem = (void *) ((unsigned long) pMem + sizeof(*magic));

    return pMem;
}

/*
 * Free some memory.
 * Recycle it back onto the free list for blocks of that size.
 */
void free_mem( void *pMem, long sMem )
{
    int iList;
    long *magic;

    if ( !pMem )
	return;


    pMem = (void *) ((unsigned long) pMem - sizeof(*magic));
    magic = (long *) pMem;

    if (*magic != MAGIC_NUM)
    {
        coder_log("Attempt to recyle invalid memory of size %d.",sMem);
        coder_log((char*) pMem + sizeof(*magic),0);
        return;
    }

    *magic = 0;
    sMem += sizeof(*magic);

    for ( iList = 0; iList < MAX_MEM_LIST; iList++ )
    {
        if ( sMem <= rgSizeList[iList] )
            break;
    }

    if ( iList == MAX_MEM_LIST )
    {
        bug( "Free_mem: size %d too large.", sMem );
        exit( 1 );
    }
    * ((void **) pMem) = rgFreeList[iList];
    rgFreeList[iList]  = pMem;
    return;
}

 
 
/*
 * Allocate some permanent memory.
 * Permanent memory is never freed,
 *   pointers into it may be copied safely.
 */
void *alloc_perm( long sMem )
{
    static char *pMemPerm;
    static long iMemPerm;
    void *pMem;
 
    while ( sMem % sizeof(long) != 0 )
        sMem++;
    if ( sMem > MAX_PERM_BLOCK )
    {
        bug( "Alloc_perm: %d too large.", sMem );
        exit( 1 );
    }
 
    if ( pMemPerm == NULL || iMemPerm + sMem > MAX_PERM_BLOCK )
    {
        iMemPerm = 0;
        if ( ( pMemPerm = (char *) calloc( 1, MAX_PERM_BLOCK ) ) == NULL )
        {
            perror( "Alloc_perm" );
            exit( 1 );
        }
    }
 
    pMem        = pMemPerm + iMemPerm;
    iMemPerm   += sMem;
    nAllocPerm += 1;
    sAllocPerm += sMem;
    return pMem;
}
 
/*
 * Duplicate a string into dynamic memory.
 * Fread_strings are read-only and shared.
*/
char *str_dup( const char *str)
{
    char *str_new;
 
    if ( str[0] == '\0' )
     return &str_empty[0];

    if ( str >= string_space && str < top_string )
        return (char *) str;

    str_new = (char *) alloc_mem( strlen(str) + 1 );
    top_str_dup++;
    /*str_new = malloc( strlen(str) +1 );*/
    strcpy( str_new, str );
    return str_new;
}
/*
 *  Don't share the string.
*/
char *str_dup_ns( const char *str )
{
    char *str_new;
 
    if ( str[0] == '\0' )
     return &str_empty[0];

    top_str_dup++;
    str_new = (char *) malloc( strlen(str) +1 );
    strcpy( str_new, str );
    return str_new;
}
 
 
/*
 * Free a string.
 * Null is legal here to simplify callers.
 * Read-only shared strings are not touched.
 */
void free_string( char *pstr )
{
    if ( pstr == NULL
    ||   pstr == &str_empty[0]
    || ( pstr >= string_space && pstr < top_string ) )
        return;

    free_mem( pstr, strlen(pstr) + 1 );
    top_str_dup--;
    /*free( pstr );*/
    return;
}
 
 
 
void do_areas( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_BUF];
    AREA_DATA *pArea;
    buf[0] = '\0';
    buf1[0] = '\0';

    if (argument[0] != '\0')
    {
        send_to_char("No argument is used with this command.\n\r",ch);
        return;
    }
    sprintf(buf,"******************** The World of Maegmenel ********************  \n\r"); 
    strcat(buf1,buf);
    for ( pArea = area_first; pArea != NULL ; pArea = pArea->next )
    {
	if (ch->level >= pArea->visible && pArea->visible >= 0)
	{
	  if ( pArea->world == 1 )
	  {
        	sprintf( buf, "%-35s %s"
		,pArea->name, pArea->credits);
		strcat(buf,"\n\r");
	        strcat(buf1,buf);
	  }
	}		
    }
    sprintf(buf,"\n\r********************* The World of Lithdor ********************* \n\r"); 
    strcat(buf1,buf);
    for ( pArea = area_first; pArea != NULL ; pArea = pArea->next )
    {
	if (ch->level >= pArea->visible && pArea->visible >= 0)
	{
	  if ( pArea->world == 2 )
	  {
        	sprintf( buf, "%-35s %s"
		,pArea->name, pArea->credits);
		strcat(buf,"\n\r");
		strcat(buf1,buf);
	  }
	}		
    }
    sprintf(buf,"\n\r********************* The World of Nenkemen ******************** \n\r"); 
    strcat(buf1,buf);
    for ( pArea = area_first; pArea != NULL ; pArea = pArea->next )
    {
	if (ch->level >= pArea->visible && pArea->visible >= 0)
	{
	  if ( pArea->world == 0 )
	  {
        	sprintf( buf, "%-35s %s"
		,pArea->name, pArea->credits);
		strcat(buf,"\n\r");
		strcat(buf1,buf);
	  }
	}		
    }
    sprintf(buf,"\n\r************************ The Darkworld ************************ \n\r"); 
    strcat(buf1,buf);
    for ( pArea = area_first; pArea != NULL ; pArea = pArea->next )
    {
	if (ch->level >= pArea->visible && pArea->visible >= 0)
	{
	  if ( pArea->world == 3 )
	  {
        	sprintf( buf, "%-35s %s"
		,pArea->name, pArea->credits);
		strcat(buf,"\n\r");
		strcat(buf1,buf);
	  }
	}		
    }
 page_to_char( buf1, ch );
 return;
}
void do_memory( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
 
    sprintf( buf, "Affects  %5ld  Mobs       %5ld  Objs      %5ld  Cards    %5d\n\r", top_affect_mob + top_affect_obj, top_affect_mob,top_affect_obj, top_card_data); send_to_char( buf, ch );
    sprintf( buf, "Areas    %5d  Weapon     %5ld  ReadNote  %5ld\n\r", top_area,top_weapon_data, top_read_notes_data ); send_to_char( buf, ch );
    sprintf( buf, "ExDes    %5d  Magic      %5ld  Wear      %5ld  Finger   %5d\n\r", top_ed,top_magic_data,top_wear_data,top_finger_data ); send_to_char( buf, ch );
    sprintf( buf, "Exits    %5ld  Containers %5ld  Debt      %5d  Events   %5d\n\r", top_exit,top_cont_data,top_debt_data, top_event); send_to_char( buf, ch );
    sprintf( buf, "Helps    %5d  Tracks     %5ld  Clan      %5d  Equips   %5ld\n\r", top_help,top_track_data,top_clan_data, top_equip_data); send_to_char( buf, ch );
    sprintf( buf, "Inside   %5ld  Gainers   %5d  Insides  %5ld\n\r", top_inside_area_data, top_gainer_data, top_inside_data); send_to_char( buf, ch );
    sprintf( buf, "Socials  %5d  Weather    %5d  Bet Data  %5d  Worlds   %5d\n\r", social_count,top_weather_data,top_bet_data, top_world ); send_to_char( buf, ch );
    sprintf( buf, "Resets   %5d  Spells     %5ld  Notes     %5d  bfsqueue %5d\n\r", top_reset,top_spell_data, top_note_data, top_bfs_queue); send_to_char( buf, ch );
    sprintf( buf, "Rooms    %5ld  Real       %5ld  Gates     %5ld  bfsroom  %5d\n\r", top_room,top_room_real,top_gate_data,top_bfs_room     ); send_to_char( buf, ch );
    sprintf( buf, "Shops    %5d  Lights     %5ld  Editing   %5d\n\r", top_shop,top_light_data, top_editing_data ); send_to_char( buf, ch );
    sprintf( buf, "Locks    %5ld  Triggers   %5ld  Groups    %5d  Quests   %d\n\r", top_lock_data,top_trigger_data,top_group_data, top_quest ); send_to_char( buf, ch );
    sprintf( buf, "Scripts  %5ld  Variables  %5ld  Ban Data  %5d  C-Quests %d\n\r", top_script_data,top_variable_data, top_ban_data,top_char_quests); send_to_char( buf, ch );
    sprintf( buf, "Traps    %5ld  Moveables  %5ld  Descrips  %5d  Changes  %5d\n\r", top_trap_data,top_moveable_data,top_descriptor,top_change_data); send_to_char( buf, ch );
    sprintf( buf, "Edibles  %5ld  Logons     %5ld  Alloc MEM %5ld\n\r", top_edible_data,top_logon_data, top_alloc_mem); send_to_char( buf, ch );
    sprintf( buf, "T-Lists  %5ld  Top Trig   %5ld  PcData    %5d\n\r", top_trigger_list_data, top_trigger_index, top_pcdata); send_to_char( buf, ch );
    sprintf( buf, "SKill-L  %5ld  Str_dup    %5ld  Approve   %5ld\n\r", top_skill_list,top_str_dup, top_approve_data); send_to_char( buf, ch );
    sprintf( buf, "Top Room %5ld  Tracking   %5ld  Bhost     %5d\n\r", top_room_index, top_track_type,top_bhost_list ); send_to_char( buf, ch );
    sprintf( buf, "Ch Index %5ld  Obj Index  %5ld  Specpro   %5d\n\r", char_index, obj_index, top_specpro_list); send_to_char( buf, ch );
    sprintf( buf, "Mobs     %5ld  In use     %5d  Wizlist   %5d\n\r", top_mob_index,mobile_count, top_wizlist_data ); send_to_char( buf, ch );
    sprintf( buf, "PC Clan    %5ld\n\r", top_pcclan_data ); send_to_char( buf, ch );
    sprintf( buf, "Castles  %5ld \n\r", top_castle_data ); send_to_char( buf, ch );
    sprintf( buf, "Objs     %5ld  In use     %5d  Buffers   %5d\n\r", top_obj_index,object_count, top_buffer); send_to_char( buf, ch );
    sprintf( buf, "Strings %5d strings of %7d bytes (max %d).\n\r",
        nAllocString, sAllocString, MAX_STRING );
    send_to_char( buf, ch );
    sprintf( buf, "Perms   %5d blocks  of %7d bytes.\n\r",
        nAllocPerm, sAllocPerm );
    send_to_char( buf, ch );
 
    return;
}
 
void do_dump( CHAR_DATA *ch, char *argument )
{
    long num_pcs,aff_count;
    OBJ_INDEX_DATA *pObjIndex;
    FILE *fp;
    long vnum,nMatch = 0;
    char buf[MAX_STRING_LENGTH];
    /* open file */
/*
    fp = fopen("../misc/mem.dmp","w");
*/
    /* report use of data structures */
    
    num_pcs = 0;
    aff_count = 0;
 
    /* mobile prototypes */
/*
    fprintf(fp,"MobProt %4ld (%8ld bytes)\n",
    top_mob_index, top_mob_index * (sizeof(*pMobIndex))); 
*/
    /* mobs */
/*
    count = 0;  count2 = 0;
    for (fch = char_list; fch != NULL; fch = fch->next)
    {
	count++;
	if (fch->pcdata != NULL)
	    num_pcs++;
	for (af = fch->affected; af != NULL; af = af->next)
	    aff_count++;
    }
    for (fch = char_free; fch != NULL; fch = fch->next)
	count2++;
    fprintf(fp,"Mobs    %4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*fch)), count2, count2 * (sizeof(*fch)));
*/
    /* pcdata */
/*
    count = 0;
    for (pc = pcdata_free; pc != NULL; pc = pc->next)
	count++; 
    fprintf(fp,"Pcdata  %4d (%8d bytes), %2d free (%d bytes)\n",
	num_pcs, num_pcs * (sizeof(*pc)), count, count * (sizeof(*pc)));
*/
    /* descriptors */
/*
    count = 0; count2 = 0;
    for (d = descriptor_list; d != NULL; d = d->next)
	count++;
    for (d= descriptor_free; d != NULL; d = d->next)
	count2++; 
    fprintf(fp, "Descs  %4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*d)), count2, count2 * (sizeof(*d)));
*/
    /* object prototypes */

/*    for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
	for( pObjIndex = obj_index_hash[vnum]; pObjIndex != NULL; pObjIndex = pObjIndex->next )
	{
	    if ( pObjIndex->wear_data != NULL )
		for (af = pObjIndex->wear_data->affected; af != NULL; af = af->next)
		    aff_count++;
	    nMatch++;
	} 
    fprintf(fp,"ObjProt %4ld (%8ld bytes)\n",
	top_obj_index, top_obj_index * (sizeof(*pObjIndex)));
 */
    /* objects */

/*    count = 0;  count2 = 0;
    for (obj = object_list; obj != NULL; obj = obj->next)
    {
	count++;
	if ( obj->wear_data != NULL )
	    for (af = obj->wear_data->affected; af != NULL; af = af->next)
		aff_count++;
    }
    for (obj = obj_free; obj != NULL; obj = obj->next)
	count2++;
    fprintf(fp,"Objs    %4d (%8d bytes), %2d free (%d bytes)\n",
	count, count * (sizeof(*obj)), count2, count2 * (sizeof(*obj)));
 */
    /* affects */
/*
    count = 0;
    for (af = affect_free; af != NULL; af = af->next)
	count++; 
    fprintf(fp,"Affects %4d (%8d bytes), %2d free (%d bytes)\n",
	aff_count, aff_count * (sizeof(*af)), count, count * (sizeof(*af))); 
*/
    /* rooms */
/*
    fprintf(fp,"Rooms   %4ld (%8ld bytes)\n",
	top_room, top_room * (sizeof(*room)));
*/
    /* exits */
/*
    fprintf(fp,"Exits   %4ld (%8ld bytes)\n",
	top_exit, top_exit * (sizeof(*exit)));
    if( fp )
    fclose(fp);
 */
    /* start printing out mobile data */
/*
    fp = fopen("../misc/mob.dmp","w");
    fprintf(fp,"\nMobile Analysis\n");
    fprintf(fp,  "---------------\n");
    nMatch = 0;
    for (vnum = 0; vnum < MAX_KEY_HASH; vnum++)
	for( pMobIndex = mob_index_hash[vnum]; pMobIndex != NULL; pMobIndex = pMobIndex->next )
	{
	    nMatch++;
	    fprintf(fp,"#%-4ld %3d active %3d killed     %s\n%s\n%s\n%s Dam_type%d\n=================================================================\n",
		pMobIndex->vnum,pMobIndex->count, pMobIndex->killed,pMobIndex->short_descr,
		pMobIndex->player_name,pMobIndex->long_descr, pMobIndex->description, pMobIndex->dam_type);
	}
    if( fp )
    fclose(fp);
 */
    /* start printing out object data */

    fp = fopen("../misc/obj.dmp","w");
    fprintf(fp,"\nObject Analysis\n");
    fprintf(fp,  "---------------\n");
    nMatch = 0;
    for (vnum = 0; vnum < MAX_KEY_HASH; vnum++)
    {
	for( pObjIndex = obj_index_hash[vnum]; pObjIndex != NULL; pObjIndex = pObjIndex->next )
        {
	    if ( pObjIndex->wear_data == NULL )
		continue;
            if ( pObjIndex->wear_data->ac == 0)
		continue;
/*	    if ( pObjIndex->wear_data->weapon_data == NULL )
		continue;	
*/	    if ( pObjIndex->wear_data != NULL )  
    	    {
        	nMatch++;
		fprintf(fp,"\n======================================================================\n");
            	fprintf(fp,"#%-5ld\n",
                    pObjIndex->vnum);
		sprintf(buf, "In area: '%s' (filename: '%s')",
	    	    pObjIndex->in_area->name, pObjIndex->in_area->filename);
    		fprintf(fp,buf);
/*		sprintf( buf, "\nVnum: %ld",
		    pObjIndex->vnum );
    		fprintf(fp,buf); 
		sprintf( buf, "\nShort description: %s\nLong description: %s\n",
		    pObjIndex->short_descr, pObjIndex->description );
    		fprintf(fp,buf);					    			
 		sprintf( buf, "Extra bits: %s\n", 
		    extra_bit_name( pObjIndex->extra_flags ) );
    		fprintf(fp,buf);
 		sprintf( buf, "Special bits: %s\n", 
		    special_bit_name( pObjIndex->special_flags ) );
    		fprintf(fp,buf);
    		sprintf( buf, "Condition: %d  Timer: %d\n",
		    pObjIndex->condition, pObjIndex->timer );
    		fprintf(fp,buf);

		if ( pObjIndex->wear_data != NULL )
    		{
        	    sprintf( buf, "Armor class is %d\n", 
			pObjIndex->wear_data->ac);
	    	    fprintf(fp,buf);
	            if ( pObjIndex->wear_data->weapon_data != NULL )
        	    {
	    		WEAPON_DATA * weapon = pObjIndex->wear_data->weapon_data;
 	    		fprintf(fp,"Weapon type is ");
	    		switch (weapon->type)
			{
	    		    case(WEAPON_EXOTIC) 	: fprintf(fp,"exotic\n");	break;
	    		    case(WEAPON_SWORD)  	: fprintf(fp,"sword\n");	break;	
	    		    case(WEAPON_DAGGER) 	: fprintf(fp,"dagger\n");	break;
	    		    case(WEAPON_SPEAR)	: fprintf(fp,"spear/staff\n");	break;
	    		    case(WEAPON_MACE) 	: fprintf(fp,"mace/club\n");	break;
	   		    case(WEAPON_AXE)	: fprintf(fp,"axe\n");		break;
	    		    case(WEAPON_FLAIL)	: fprintf(fp,"flail\n");	break;
	    		    case(WEAPON_WHIP)	: fprintf(fp,"whip\n");		break;
	    		    case(WEAPON_POLEARM)	: fprintf(fp,"polearm\n");	break;
	    		    default			: fprintf(fp,"unknown\n");	break;
			}
	       	    	sprintf(buf,"Damage is %dd%d (average %d)\n",
			    weapon->dice, weapon->die, (1 + weapon->die) * weapon->dice / 2 );
		    	fprintf(fp,buf);
		    	if (weapon->flags)
		    	{
		            sprintf(buf,"Weapons flags: %s\n",weapon_bit_name(weapon->flags));
		            fprintf(fp,buf);
	            	}
		    }
		    if ( pObjIndex->wear_data->magic_data != NULL )
		    {
		   	sprintf(buf,"%s with %d(%d) charges of : \n", 
		    	    ( pObjIndex->wear_data->magic_data->type == MAGIC_STAFF ? "Staff" :
		    	    ( pObjIndex->wear_data->magic_data->type == MAGIC_WAND  ? "Wand" : "Scroll" ) ),
			    pObjIndex->wear_data->magic_data->charges,
			    pObjIndex->wear_data->magic_data->max_charges);
		    	fprintf(fp,buf);
		    	if ( pObjIndex->wear_data->magic_data->spells == NULL )	
			    fprintf(fp, "    None.\n\r");
		    	for ( spell = pObjIndex->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
		    	{
			    sprintf( buf, "    Level %d spell of %s.\n", spell->level, skill_table[ spell->spell ].name );
			    fprintf(fp,buf);
		    	}
		    }
		}
    		if (pObjIndex->wear_data != NULL )
		{
    		    for ( paf = pObjIndex->wear_data->affected; paf != NULL; paf = paf->next )
    		    {
			if ( paf->location == APPLY_AFFECT )
			{
			    sprintf( buf, "Applies affect of %s.\n", bit_type_name( paf->bit_type, paf->bitvector ) );
			}
			else
  	    		    sprintf( buf, "Affects %s by %d, level %d.\n",
	        		affect_loc_name( paf ), paf->modifier,paf->level );
			    fprintf(fp,buf);
    		    }
    		}
		if ( pObjIndex->extra_descr != NULL || pObjIndex->extra_descr != NULL )
    		{
		    EXTRA_DESCR_DATA *ed;
		    fprintf(fp,"Extra description keywords: ");
		    for ( ed = pObjIndex->extra_descr; ed != NULL; ed = ed->next )
		    {
	    		sprintf(buf,"%s ",ed->keyword);
	   		fprintf(fp,buf);
		    }
		    for ( ed = pObjIndex->extra_descr; ed != NULL; ed = ed->next )
		    {
	    		sprintf(buf,"%s ", ed->keyword);
	   		fprintf(fp,buf);
		    }
		    fprintf(fp,"\n");
    		}
   	 	if ( pObjIndex->spec_fun != NULL )
		    fprintf(fp,"This object has a special function.\n");
*/
	    }
        }
    }
    /* close file */

    if( fp )
    fclose(fp);
}
 
 
 
/*
 * Stick a little fuzz on a number.
 */
int number_fuzzy( int number )
{
    switch ( number_bits( 2 ) )
    {
    case 0:  number -= 1; break;
    case 3:  number += 1; break;
    }
 
    return UMAX( 1, number );
}
 
 
 
/*
 * Generate a random number.
 */
int number_range( int from, int to )
{
    int power;
    int number;
 
    if (from == 0 && to == 0)
        return 0;
 
    if ( ( to = to - from + 1 ) <= 1 )
        return from;
 
    for ( power = 2; power < to; power <<= 1 )
        ;
 
    while ( ( number = number_mm() & (power -1 ) ) >= to )
        ;
 
    return from + number;
}
 
 
 
/*
 * Generate a percentile roll.
 */
int number_percent( void )
{
    int percent;
 
    while ( ( percent = number_mm() & (128-1) ) > 99 )
        ;
 
    return 1 + percent;
}
 
 
 
/*
 * Generate a random door.
 */
int number_door( void )
{
    int door;
 
    while ( ( door = number_mm() & (8-1) ) > 5)
        ;
 
    return door;
}
 
int number_bits( int width )
{
    return number_mm( ) & ( ( 1 << width ) - 1 );
}
 
 
 
 
/*
 * I've gotten too many bad reports on OS-supplied random number generators.
 * This is the Mitchell-Moore algorithm from Knuth Volume II.
 * Best to leave the constants alone unless you've read Knuth.
 * -- Furey
 */

/* I noticed streaking with this random number generator, so I switched
   back to the system srandom call.  If this doesn't work for you, 
   define OLD_RAND to use the old system -- Alander */

/*
 * I have notice the same thing.  Lets try this for a bit and see what
 * happens.  Galadrail
 */ 

#if defined (OLD_RAND)
static  int     rgiState[2+55];
#endif
 
void init_mm( )
{
#if defined (OLD_RAND)
    int *piState;
    int iState;
 
    piState     = &rgiState[2];
 
    piState[-2] = 55 - 55;
    piState[-1] = 55 - 24;
 
    piState[0]  = ((int) current_time) & ((1 << 30) - 1);
    piState[1]  = 1;
    for ( iState = 2; iState < 55; iState++ )
    {
        piState[iState] = (piState[iState-1] + piState[iState-2])
                        & ((1 << 30) - 1);
    }
#else
    srandom(time(NULL)^getpid());
#endif
    return;
}
 
 
 
long number_mm( void )
{
#if defined (OLD_RAND)
    int *piState;
    int iState1;
    int iState2;
    int iRand;
 
    piState             = &rgiState[2];
    iState1             = piState[-2];
    iState2             = piState[-1];
    iRand               = (piState[iState1] + piState[iState2])
                        & ((1 << 30) - 1);
    piState[iState1]    = iRand;
    if ( ++iState1 == 55 )
        iState1 = 0;
    if ( ++iState2 == 55 )
        iState2 = 0;
    piState[-2]         = iState1;
    piState[-1]         = iState2;
    return iRand >> 6;
#else
    return random() >> 6;
#endif
}
 
/*
 * Roll some dice.
 */
int roll_dice( int number, int size )
{
    int idice;
    int sum;
 
    switch ( size )
    {
    case 0: return 0;
    case 1: return number;
    }
 
    for ( idice = 0, sum = 0; idice < number; idice++ )
        sum += number_range( 1, size );
 
    return sum;
}
 
 
 
/*
 * Simple linear interpolation.
 */
int interpolate( int level, int value_00, int value_32 )
{
    return value_00 + level * (value_32 - value_00) / 32;
}
 
 
 
/*
 * Removes the tildes from a string.
 * Used for player-entered strings that go into disk files.
 */
void smash_tilde( char *str )
{
    for ( ; *str != '\0'; str++ )
    {
        if ( *str == '~' )
            *str = '-';
    }
 
    return;
}
 
 
 
/*
 * Compare strings, case insensitive.
 * Return TRUE if different
 *   (compatibility with historical functions).
 */
int str_cmp( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
        bug( "Str_cmp: null astr.", 0 );
        return -1;
    }
 
    if ( bstr == NULL )
    {
        bug( "Str_cmp: null bstr.", 0 );
        return 1;
    }
 
    for ( ; *astr || *bstr; astr++, bstr++ )
    {
	if ( LOWER(*astr) == LOWER(*bstr) )
	    continue;
        if ( LOWER(*astr) < LOWER(*bstr) )
            return -1;
        if ( LOWER(*astr) > LOWER(*bstr) )
            return 1;
    }
 
    return 0;
}
 
 
 
/*
 * Compare strings, case insensitive, for prefix matching.
 * Return TRUE if astr not a prefix of bstr
 *   (compatibility with historical functions).
 */
bool str_prefix( const char *astr, const char *bstr )
{
    if ( astr == NULL )
    {
        bug( "Strn_cmp: null astr.", 0 );
        return TRUE;
    }
 
    if ( bstr == NULL )
    {
        bug( "Strn_cmp: null bstr.", 0 );
        return TRUE;
    }
 
    for ( ; *astr; astr++, bstr++ )
    {
        if ( LOWER(*astr) != LOWER(*bstr) )
            return TRUE;
    }
 
    return FALSE;
}
 
 
 
/*
 * Compare strings, case insensitive, for match anywhere.
 * Returns TRUE is astr not part of bstr.
 *   (compatibility with historical functions).
 */
bool str_infix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
    int ichar;
    char c0;
 
    if ( ( c0 = LOWER(astr[0]) ) == '\0' )
        return FALSE;
 
    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);
 
    for ( ichar = 0; ichar <= sstr2 - sstr1; ichar++ )
    {
        if ( c0 == LOWER(bstr[ichar]) && !str_prefix( astr, bstr + ichar ) )
            return FALSE;
    }
 
    return TRUE;
}
 
 
 
/*
 * Compare strings, case insensitive, for suffix matching.
 * Return TRUE if astr not a suffix of bstr
 *   (compatibility with historical functions).
 */
bool str_suffix( const char *astr, const char *bstr )
{
    int sstr1;
    int sstr2;
 
    sstr1 = strlen(astr);
    sstr2 = strlen(bstr);
    if ( sstr1 <= sstr2 && !str_cmp( astr, bstr + sstr2 - sstr1 ) )
        return FALSE;
    else
        return TRUE;
}
 
 
 
/*
 * Returns an initial-capped string.
 */
char *capitalize( const char *str )
{
    static char strcap[MAX_STRING_LENGTH];
    int i;
 
    for ( i = 0; str[i] != '\0'; i++ )
        strcap[i] = LOWER(str[i]);
    strcap[i] = '\0';
    strcap[0] = UPPER(strcap[0]);
    return strcap;
}
 
/*
 * Append a string to a file.
 */
void append_file( CHAR_DATA *ch, char *file, char *str )
{
    FILE *fp;
 
    if ( IS_NPC(ch) || str[0] == '\0' )
        return;
 
    if ( ( fp = fopen( file, "a" ) ) == NULL )
    {
        perror( file );
        send_to_char( "Could not open the file!\n\r", ch );
    }
    else
    {
        fprintf( fp, "[%5ld] %s: %s\n",
            ch->in_room ? ch->in_room->vnum : 0, ch->name, str );
        if( fp )
        fclose( fp );
    }
 
    return;
}
 
/*
 * Reports to coders 
 */
void coder_log( const char *str, int param )
{
    char buf[MAX_STRING_LENGTH];
    FILE *fp;
 
    if ( fpArea != NULL )
    {
        int iLine;
        int iChar;
 
        if ( fpArea == stdin )
        {
            iLine = 0;
        }
        else
        {
            iChar = ftell( fpArea );
            fseek( fpArea, 0, 0 );
            for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
            {
                while ( getc( fpArea ) != '\n' )
                    ;
            }
            fseek( fpArea, iChar, 0 );
        }
 
        sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );
        log_string( buf );
        
        if ( ( fp = fopen( "shutdown.bug", "a" ) ) != NULL )
        {
            fprintf( fp, "[*****] %s\n", buf );
            if( fp )
            fclose( fp );
        }
    }
 
    strcpy( buf, "[*****] CODERS: " );
    sprintf( buf + strlen(buf), str, param );
    log_string( buf );
    info_channel ( NULL, buf, INFOACT_CODER );
    return;
}
 
void bug( const char *str, int param )
{
    char buf[MAX_STRING_LENGTH];
    FILE *fp;
 
    if ( fpArea != NULL )
    {
        int iLine;
        int iChar;
 
        if ( fpArea == stdin )
        {
            iLine = 0;
        }
        else
        {
            iChar = ftell( fpArea );
            fseek( fpArea, 0, 0 );
            for ( iLine = 0; ftell( fpArea ) < iChar; iLine++ )
            {
                while ( getc( fpArea ) != '\n' )
                    ;
            }
            fseek( fpArea, iChar, 0 );
        }
 
        sprintf( buf, "[*****] FILE: %s LINE: %d", strArea, iLine );
        log_string( buf );
        
        if ( ( fp = fopen( "shutdown.bug", "a" ) ) != NULL )
        {
            fprintf( fp, "[*****] %s\n", buf );
            if( fp )
            fclose( fp );
        }
    }
 
    strcpy( buf, "[*****] BUG: " );
    sprintf( buf + strlen(buf), str, param );
    log_string( buf );
    info_channel ( NULL, buf, INFOACT_BUG );
/* RT removed due to bug-file spamming 
    if ( ( fp = fopen( BUG_FILE, "a" ) ) != NULL )
    {
        fprintf( fp, "%s\n", buf );
        if( fp )
        fclose( fp );
    }
*/
 
	if (fBootDb)
        bug2( "Check the above file first.",0 ); 
    return;
}
 
 
 
 
 
/*
 * Reports a bug 2.
 */
void bug2( const char *str, int param )
{
    char buf[MAX_STRING_LENGTH];
    FILE *fp;
 
    if ( fpArea2 != NULL )
    {
        int iLine;
        int iChar;
 
        if ( fpArea2 == stdin )
        {
            iLine = 0;
        }
        else
        {
            iChar = ftell( fpArea2 );
            fseek( fpArea2, 0, 0 );
            for ( iLine = 0; ftell( fpArea2 ) < iChar; iLine++ )
            {
                while ( getc( fpArea2 ) != '\n' )
                    ;
            }
            fseek( fpArea2, iChar, 0 );
        }
 
        sprintf( buf, "[*****] FILE: %s LINE: %d", crnt_area, iLine );
        log_string( buf );
        
        if ( ( fp = fopen( "shutdown.bug", "a" ) ) != NULL )
        {
            fprintf( fp, "[*****] %s\n", buf );
            if( fp )
            fclose( fp );
        }
    }
 
    strcpy( buf, "[*****] BUG: " );
    sprintf( buf + strlen(buf), str, param );
    log_string( buf );
    info_channel ( NULL, buf, INFOACT_BUG );
/* RT removed due to bug-file spamming 
    if ( ( fp = fopen( BUG_FILE, "a" ) ) != NULL )
    {
        fprintf( fp, "%s\n", buf );
        if( fp )
        fclose( fp );
    }
*/
 
    return;
}
 
 
 
/*
 * Writes a string to the log.
 */
void log_string( const char *str )
{
    char *strtime;
 
    strtime                    = ctime( &current_time );
    strtime[strlen(strtime)-1] = '\0';
    fprintf( stderr, "%s :: %s\n", strtime, str );
    return;
}
 
 
 
/*
 * This function is here to aid in debugging.
 * If the last expression in a function is another function call,
 *   gcc likes to generate a JMP instead of a CALL.
 * This is called "tail chaining."
 * It hoses the debugger call stack for that call.
 * So I make this the last call in certain critical functions,
 *   where I really need the call stack to be right for debugging!
 *
 * If you don't understand this, then LEAVE IT ALONE.
 * Don't remove any calls to tail_chain anywhere.
 *
 * -- Furey
 */
void tail_chain( void )
{
    return;
}
 
 
void save_rooms(AREA_DATA *pArea, char *arg)
{	SPECPRO_LIST	*s;
        ROOM_INDEX_DATA *pRoom;
        EXTRA_DESCR_DATA *pEdesc;
	LOCK_DATA *lock_list;
        char buf[MAX_STRING_LENGTH];
        int Hash_loc;
        sh_int counter;
	AFFECT_DATA * paf;
        FILE *f;
        sprintf(buf, "%s%s/rooms", SAVEAREAS_DIR, arg);
        if ( ( f = fopen(buf, "w") ) == NULL )
        {
                log_string("Error opening area file");
     		bug( "areasave rooms:  open area file", 0 );
		return;
        }

        for (Hash_loc = 0; Hash_loc < MAX_KEY_HASH; Hash_loc++)
        {
           for (pRoom = room_index_hash[Hash_loc]; pRoom != NULL; pRoom= pRoom->next)
            {  if (pRoom->sector_type == SECT_PC ) continue; /* Don't Save the player containers */
	       if (IS_SET( pRoom->room_flags_2, ROOM_NOSAVE ) || IS_SET( pRoom->room_flags_2, ROOM_DELETE ) ) continue; /* Don't save a room that has been edited for ingame use only */
               if (pRoom->vnum < 0) continue; 
               if (pRoom->area == pArea)
                {
		    for ( paf = pRoom->affected_by ; paf != NULL ; paf = paf->next )
			affect_modify_room( pRoom, paf, FALSE );

			/*  Name must be first */
                    fprintf(f,"NAME %s~\n", pRoom->name);
                    fprintf(f,"VNUM %ld\n", pRoom->vnum);
                    fprintf(f,"FLAG %ld\n", pRoom->room_flags);
                    fprintf(f,"FLAG_2 %ld\n", pRoom->room_flags_2);
                    fprintf(f,"LIGH %d\n", pRoom->light);
                    fprintf(f,"SEC  %d\n", pRoom->sector_type);
		    fprintf(f,"SIZE %d\n", pRoom->size);
		    fprintf(f,"Guilds %ld\n", pRoom->guilds);
		    fprintf(f,"DESC %s~\n", pRoom->description);
		    for (s = pRoom->spro; s!= NULL; s=s->next)
		    fprintf(f,"SPN  %d\n",s->sp->spn);
		    if ( pRoom->trigger_list != NULL )
		    {
			TRIGGER_LIST_DATA *trigger_list_data;
			for ( trigger_list_data = pRoom->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			fprintf(f, "TRIG_LIST %d\n",trigger_list_data->vnum);	
		    }
                    for ( pEdesc = pRoom->extra_descr ; pEdesc != NULL;
                                        pEdesc = pEdesc->next)
                        fprintf(f,"EDESC %s~ .%s~\n", pEdesc->keyword,
                                                     pEdesc->description); 
                    fprintf(f,"$!\n");
                    for (counter = 0; counter <=5; counter++)
                    {
                        if (pRoom->exit[counter] != NULL)
                        {
			    if ( IS_SET( pRoom->exit[counter]->u1.to_room->room_flags_2, ROOM_NOSAVE )  || IS_SET(pRoom->exit[counter]->u1.to_room->room_flags_2, ROOM_DELETE) ) continue;
                            fprintf(f,"EXNUM %d\n",counter);
                            fprintf(f,"EXIN %ld\n",pRoom->exit[counter]->exit_info_default);
		 	    for ( lock_list = pRoom->exit[counter]->lock ; lock_list != NULL ; lock_list = lock_list->next )
			    {
				 if ( lock_list->vnum > 0 )
					fprintf(f,"LOCKS %ld %ld %d\n",lock_list->vnum,lock_list->lock_info_default,lock_list->pick_penalty);
			    }
			    if ( pRoom->exit[counter]->mes_to_char[0] != '\0' )
                            fprintf(f,"MTC %s~\n",pRoom->exit[counter]->mes_to_char);
			    if ( pRoom->exit[counter]->mes_to_room[0] != '\0' )
                            fprintf(f,"MTR %s~\n",pRoom->exit[counter]->mes_to_room);
			    if ( pRoom->exit[counter]->mes_at_room[0] != '\0' )
                            fprintf(f,"MAR %s~\n",pRoom->exit[counter]->mes_at_room);
			    if ( pRoom->exit[counter]->trap != NULL )
			    {
				SPELL_LIST * spell;
				fprintf(f,"TRAP %ld\n",pRoom->exit[counter]->trap->flags );

				for ( spell = pRoom->exit[counter]->trap->spells ; spell != NULL ; spell = spell->next )
				    fprintf(f,"TSPELL %d %s~\n", spell->level, skill_table[ spell->spell ].name );
			    }
                            fprintf(f,"KEYW %s~\n",pRoom->exit[counter]->keyword);
                            fprintf(f,"DESC %s~\n",pRoom->exit[counter]->description);
                            fprintf(f,"TO %ld\n",pRoom->exit[counter]->u1.to_room->vnum);
                            fprintf(f,"$@\n");
                        }
                    }

		    for ( paf = pRoom->affected_by ; paf != NULL ; paf = paf->next )
			affect_modify_room( pRoom, paf, TRUE );

                    fprintf(f,"$$\n");
                }
            }
        }
        fprintf(f,"$E\n");
        if( f )
        fclose(f);

        return;
}
 
void save_mobs(AREA_DATA *pArea, char *arg)
{
        MOB_INDEX_DATA *pMobIndex;
        char buf[MAX_STRING_LENGTH];
        int Hash_loc,i, x;
        FILE *f;
        sprintf(buf, "%s%s/mobiles", SAVEAREAS_DIR, arg);
        if ( ( f = fopen(buf, "w") ) == NULL )
        {
                log_string("Error opening area file");
     		bug( "areasave rooms:  open area file", 0 );
		return;
        }

        for (Hash_loc = 0; Hash_loc < MAX_KEY_HASH; Hash_loc++)
        {
            for (pMobIndex = mob_index_hash[Hash_loc]; pMobIndex != NULL; pMobIndex= pMobIndex->next)
            {
                if (pMobIndex->load_with_area == pArea)
                {
			/* Vnum must be first */
                    fprintf(f,"VNUM %ld\n",pMobIndex->vnum);
                    fprintf(f,"PNAM %s~\n",pMobIndex->player_name);
                    fprintf(f,"SDESC %s~\n",pMobIndex->short_descr);
                    fprintf(f,"LDESC %s~\n",pMobIndex->long_descr);
                    fprintf(f,"DESC %s~\n",pMobIndex->description);
                    fprintf(f,"ACT %ld\n",pMobIndex->act);
                    fprintf(f,"ACT2 %ld\n",pMobIndex->act2);
		    fprintf(f,"GUILDS %ld\n",pMobIndex->guilds);
                    fprintf(f,"AFF %ld\n",pMobIndex->affected_by);
                    fprintf(f,"AFF2 %ld\n",pMobIndex->affected_by_2);
                    fprintf(f,"LEVEL %d\n",pMobIndex->level);
                    fprintf(f,"HITR %d\n",pMobIndex->hitroll);
                    fprintf(f,"HIT %d %d %d\n",pMobIndex->hit[0], pMobIndex->hit[1], pMobIndex->hit[2]);
		    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
                        fprintf(f,"MAN %d %d %d %d\n", i, pMobIndex->mana[0][i], pMobIndex->mana[1][i], pMobIndex->mana[2][i]);
                    fprintf(f,"DAM %d %d %d\n",pMobIndex->damage[0], pMobIndex->damage[1], pMobIndex->damage[2]);
                    fprintf(f,"ARMOR %d\n",pMobIndex->armor);
                    fprintf(f,"DAMT %d\n",pMobIndex->dam_type);
                    fprintf(f,"OFFFL %ld\n",pMobIndex->off_flags);
                    fprintf(f,"IMMFL %ld\n",pMobIndex->imm_flags);
                    fprintf(f,"RESFL %ld\n",pMobIndex->res_flags);
                    fprintf(f,"VULFL %ld\n",pMobIndex->vuln_flags);
                    fprintf(f,"SPOS %d\n",pMobIndex->start_pos);
                    fprintf(f,"DPOS %d\n",pMobIndex->default_pos);
                    fprintf(f,"SEX %d\n",pMobIndex->sex);
                    fprintf(f,"RACE %d\n",pMobIndex->race);
		    /*
		     * Reverie - The below sets gold to sentient and removes from non-sentient 
     		    if ( pMobIndex->gold[0] != 0 && IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[0] = number_range(25,75) * race_table[ pMobIndex->race ][ pMobIndex->sex ].gold * pMobIndex->level / 100;
    		    else if ( pMobIndex->gold[0] != 0 && !IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[0] = 0;
    		    if ( pMobIndex->gold[1] != 0 && IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[1] = number_range(25,75) * race_table[ pMobIndex->race ][ pMobIndex->sex ].gold * pMobIndex->level / 100;
    		    else if ( pMobIndex->gold[1] != 0 && !IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[1] = 0;
    		    if ( pMobIndex->gold[2] != 0 && IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[2] = number_range(25,75) * race_table[ pMobIndex->race ][ pMobIndex->sex ].gold * pMobIndex->level / 100;
    		    else if ( pMobIndex->gold[2] != 0 && !IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[2] = 0;
    		    if ( pMobIndex->gold[3] != 0 && IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[3] = number_range(25,75) * race_table[ pMobIndex->race ][ pMobIndex->sex ].gold * pMobIndex->level / 100;
    		    else if ( pMobIndex->gold[3] != 0 && !IS_SET(pMobIndex->form, FORM_SENTIENT) )
        		pMobIndex->gold[3] = 0;
		    */
		    fprintf(f,"COINS %ld %ld %ld %ld\n",pMobIndex->gold[0],pMobIndex->gold[1],pMobIndex->gold[2],pMobIndex->gold[3]);
                    fprintf(f,"FORM %ld\n",pMobIndex->form);
                    fprintf(f,"SIZE %d\n",pMobIndex->size);
                    fprintf(f,"HEIGHT %d\n",pMobIndex->height);
                    fprintf(f,"WEIGHT %d\n",pMobIndex->weight);
                    fprintf(f,"MTRL %d\n",pMobIndex->material);
                    fprintf(f,"FOLL %ld\n",pMobIndex->follower);
                    fprintf(f,"MOOD %d\n",pMobIndex->mood);
		    if ( pMobIndex->spec_name[0] != '\0' )
                    fprintf(f,"SPEC_FUN %s~\n",pMobIndex->spec_name);
                    fprintf(f,"VALID_SEC ");
			for (x = 0; x < SECT_MAX; x++ )
		        if ( x < MAX_SECTORS )
			fprintf(f,"%d ",pMobIndex->valid_sector[x]);
			else
			fprintf(f,"1 ");
			fprintf(f,"\n");
		    if (IS_SET(pMobIndex->act2, ACT2_MOVEABLE_AREA ) )
		    if (pMobIndex->moveable != NULL )
	            {
                    fprintf(f,"AREA_SR %ld\n",pMobIndex->moveable->area_start_room);
                    fprintf(f,"B_MSG %s~\n",pMobIndex->moveable->board_msg);
                    fprintf(f,"L_MSG %s~\n",pMobIndex->moveable->leave_msg);
                    fprintf(f,"M_MSG %s~\n",pMobIndex->moveable->move_msg);
		    }
		    if ( pMobIndex->trigger_list != NULL )
		    {
			TRIGGER_LIST_DATA *trigger_list_data;
			for ( trigger_list_data = pMobIndex->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			fprintf(f, "TRIG_LIST %d\n",trigger_list_data->vnum);	
		    }
		    if ( pMobIndex->equip != NULL )
		    {
			EQUIP_DATA *equip;
			for ( equip = pMobIndex->equip; equip != NULL; equip = equip->next )
			fprintf(f, "EQUIP %ld %d %ld\n",equip->obj_vnum, equip->chance, equip->flags);	
		    }
		    fprintf( f, "CASTLE %d\n", pMobIndex->castle_area );
		    if (pMobIndex->movement_msg[0] != '\0' )
                        fprintf(f,"MOVE_MSG %s~\n",pMobIndex->movement_msg);
		    if (pMobIndex->gainer_data != NULL)
		    {
			for ( i = 0 ; i < MAX_SKILL ; i++ )
			    if (pMobIndex->gainer_data->skill[i])
				fprintf(f,"SKILL %s~\n",skill_table[i].name);
		    }
		    fprintf(f,"L_PRACS %d %d\n",pMobIndex->prac_skills, pMobIndex->prac_spells);
		    if (pMobIndex->innate_skills != NULL)
		    {
			SKILL_LIST * skills;

			for ( skills = pMobIndex->innate_skills ; skills !=NULL ; skills = skills->next )
				fprintf(f,"L_SKILLS %s~\n",skill_table[ skills->sn ].name);
		    }
                    fprintf(f,"$$\n");
                }
            }
        }
        fprintf(f, "$E\n");
        if( f )
        fclose(f);
        return;
}
void save_objs(AREA_DATA *pArea, char *arg)
{
        char buf[MAX_STRING_LENGTH];
        OBJ_INDEX_DATA *pObjIndex;
        AFFECT_DATA * pAff;
        EXTRA_DESCR_DATA * pEdesc;
	SPELL_LIST * spell;
        int Hash_loc;
        FILE *f;
        sprintf(buf, "%s%s/objects", SAVEAREAS_DIR, arg);
      /* chmod(buf,00440); */
	    if ( ( f = fopen(buf, "w") ) == NULL )
        {
                log_string("Error opening area file");
     		bug( "areasave rooms:  open area file", 0 );
		return;
        }
     
	for (Hash_loc = 0; Hash_loc < MAX_KEY_HASH; Hash_loc++)
        {
            for (pObjIndex = obj_index_hash[Hash_loc]; pObjIndex != NULL; pObjIndex= pObjIndex->next)
            {
                if (pObjIndex->in_area == pArea)
                {
                    fprintf(f,"NAME %s~\n", pObjIndex->name);
                    fprintf(f,"SHDE %s~\n", pObjIndex->short_descr);
                    fprintf(f,"DESC %s~\n", pObjIndex->description);
                    fprintf(f,"VNUM %ld\n", pObjIndex->vnum);
                    fprintf(f,"MATE %d\n", pObjIndex->material);
                    fprintf(f,"EXFL %ld\n", pObjIndex->extra_flags);
                    fprintf(f,"SPFL %ld\n", pObjIndex->special_flags);
                    fprintf(f,"COND %d\n", pObjIndex->condition);
		    if ( pObjIndex->trigger_list != NULL )
		    {
			TRIGGER_LIST_DATA *trigger_list_data;
			for ( trigger_list_data = pObjIndex->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			fprintf(f, "TRIG_LIST %d\n",trigger_list_data->vnum);	
		    }
		    if ( pObjIndex->light_data != NULL )
			fprintf(f,"LIGHT %ld %d %d %d\n",
			    pObjIndex->light_data->flags,
			    pObjIndex->light_data->fuel,
			    pObjIndex->light_data->max_fuel,
			    pObjIndex->light_data->brightness);
		    if ( pObjIndex->cont_data != NULL )
		    {
			LOCK_DATA * lock;
			RESET_DATA * reset;

			fprintf(f,"CONT %d %ld\n", pObjIndex->cont_data->capacity, pObjIndex->cont_data->flags );

			for ( lock = pObjIndex->cont_data->lock ; lock != NULL ; lock = lock->next )
			   if ( lock->vnum > 0 )
			    fprintf(f,"LOCKS %ld %ld %d\n",
				lock->vnum,
				lock->lock_info,
				lock->pick_penalty);
			for ( reset = pObjIndex->cont_data->inside ; reset != NULL ; reset = reset->next )
			    fprintf(f,"INSIDE %c %ld %d %ld %ld %d\n",
				reset->command,
				reset->arg1,
				reset->arg2,
				reset->arg3,
				reset->vnum,
				reset->chance);

			if ( pObjIndex->cont_data->trap != NULL )
			{
			    fprintf(f,"TRAP %ld\n", pObjIndex->cont_data->trap->flags );

			    for ( spell = pObjIndex->cont_data->trap->spells; spell != NULL ; spell = spell->next )
				fprintf(f,"TSPELL %d %s~\n", spell->level, skill_table[ spell->spell ].name );
			}
		    }
		    if ( pObjIndex->gate_data != NULL )
		    {
			fprintf(f,"GATE %ld %ld %d\n",
			    pObjIndex->gate_data->vnum,
			    pObjIndex->gate_data->flags,
			    pObjIndex->gate_data->ave_timer);
		    }
		    if ( pObjIndex->inside != NULL )
		    {
			INSIDE_DATA *inside;
			for ( inside = pObjIndex->inside; inside != NULL; inside = inside->next )
			fprintf(f, "INSIDE_OBJ %ld %ld %d %ld\n",inside->obj_vnum, inside->mob_vnum, inside->chance, inside->flags);	
		    }
		    if ( pObjIndex->approve_data != NULL )
		    {
			fprintf(f,"APPROVE_DATA %ld %ld %ld %ld\nAPPROVER %s~\nCREATER %s~\nMODIFIER %s~\n",
			    pObjIndex->approve_data->flags,
			    pObjIndex->approve_data->creation_date,
			    pObjIndex->approve_data->approved_date,
			    pObjIndex->approve_data->modified_date,
			    pObjIndex->approve_data->approver,
			    pObjIndex->approve_data->creater,
			    pObjIndex->approve_data->modifier);
		    }
		    if ( pObjIndex->edible_data != NULL )
		    {
			fprintf(f,"EDIBLE %d %ld %d %d\n",
			    pObjIndex->edible_data->type,
			    pObjIndex->edible_data->flags,
			    pObjIndex->edible_data->amount,
			    pObjIndex->edible_data->max_amount);

			for ( spell = pObjIndex->edible_data->spells ; spell != NULL ; spell = spell->next  )
			     fprintf(f,"ESPELL %d %s~\n",
				spell->level,
				skill_table[ spell->spell ].name );
		    }
		    if ( pObjIndex->wear_data != NULL && 
			pObjIndex->wear_data->wear_flags != 0)
		    {
			fprintf(f,"WRFL %ld\n", pObjIndex->wear_data->wear_flags );
			fprintf(f,"SIZE %d\n", pObjIndex->wear_data->size );
			fprintf(f,"AC %d\n", pObjIndex->wear_data->ac );
                        fprintf(f,"WEIG %d\n", pObjIndex->wear_data->weight);
                        fprintf(f,"COST %ld\n", pObjIndex->wear_data->cost);
			if ( pObjIndex->wear_data->weapon_data != NULL )
			{
			    fprintf(f,"WTYPE %d\n", pObjIndex->wear_data->weapon_data->type );
			    fprintf(f,"WFLAGS %ld\n", pObjIndex->wear_data->weapon_data->flags );
			    fprintf(f,"WDT %d\n", pObjIndex->wear_data->weapon_data->dam_type );
			    fprintf(f,"WDAMAGE %d %d\n", pObjIndex->wear_data->weapon_data->dice, pObjIndex->wear_data->weapon_data->die );
			}
			if ( pObjIndex->wear_data->magic_data != NULL )
			{
			    fprintf(f,"MAGICAL %d %ld %d %d\n", pObjIndex->wear_data->magic_data->type, pObjIndex->wear_data->magic_data->flags, pObjIndex->wear_data->magic_data->charges, pObjIndex->wear_data->magic_data->max_charges);
			    for ( spell = pObjIndex->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
			    {
				fprintf(f,"SPELL %d %s~\n", spell->level, skill_table[ spell->spell ].name );
			    }
			}
		    }
                   /* fprintf(f,"COUN %d\n", pObjIndex->count); */
		    if ( pObjIndex->timer > 0 )
                        fprintf(f,"TIMER %d\n", pObjIndex->timer);
		    if ( pObjIndex->shop_type > 0 )
                        fprintf(f,"SHOP %d\n", pObjIndex->shop_type);

		    if ( pObjIndex->wear_data != NULL )
                    for ( pAff = pObjIndex->wear_data->affected ; pAff != NULL;
                                        pAff = pAff->next)
                        fprintf(f,"AFN %d %d %d %d %d %d %ld %d\n",
                            pAff->type, pAff->level, pAff->duration,
                            pAff->location, pAff->location2, pAff->modifier, pAff->bitvector, pAff->bit_type);
                    for ( pEdesc = pObjIndex->extra_descr ; pEdesc != NULL;
                                        pEdesc = pEdesc->next)
                        fprintf(f,"EDESC %s~ .%s~\n", pEdesc->keyword,
                                                     pEdesc->description); 
                   fprintf(f,"$$\n");
                }
            }
        }
        fprintf(f, "$E\n");
        if( f )
        fclose(f);
        return;
}

/* OLC */
#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )                \
				if ( !str_cmp( word, literal ) )	\
                {                                   \
                    field  = value;                 \
                    fMatch = TRUE;                  \
                    break;                          \
				}

void default_weather( AREA_DATA *pArea )
{
    int season;
    static int storm[ 4 ] = { SKY_HURICANE, SKY_BLIZZARD, SKY_RAINING, SKY_RAINING };

    if ( pArea->world == 2 )
    {
	for ( season = 0 ; season < 4 ; season++ )
	{
	    pArea->weather->chance[ season ][ 0 ] =  0;
	    pArea->weather->chance[ season ][ 1 ] = 50;
	    pArea->weather->chance[ season ][ 2 ] =101;
	    pArea->weather->chance[ season ][ 3 ] =101;
	}
    }

    for ( season = 0 ; season < 4 ; season++ )
	pArea->weather->storm_type[ season ] = storm[ pArea->world ];

}

void new_load_area()
{
    FILE *fp;
    AREA_DATA *pArea;
    char      buf[MAX_INPUT_LENGTH];
    char      *word = NULL;
    int       season,i;
    bool      fMatch;
    bool      fWeather=FALSE;
    
    sprintf(buf, "%s%s/area", SAVEAREAS_DIR, strArea );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open areafile %s ", buf );
        bug( b_buf, 0 );
        return;
    }

    pArea               = new_area();
    pArea->max_age_empty= 15;
    pArea->max_age      = 31;
    pArea->age          = pArea->max_age + 1;
    pArea->nplayer      = 0;
    strncpy( pArea->filename, strArea, 30 );
    pArea->vnum         = top_area;
    strncpy( pArea->name, "New Area", 80 );
    strncpy( pArea->builders, "None", 30 );
    pArea->time_zone	= 0;
    pArea->security     = 1;
    pArea->lvnum        = 0;
    pArea->visible      = 0;
    pArea->uvnum        = 0;
    pArea->area_flags   = 8;
    pArea->recall       = ROOM_VNUM_TEMPLE;
    pArea->areanum      = top_area;
    pArea->world        = numWorld;

    for ( ; ; )
    {
       word   = feof( fp ) ? str_dup("End") : fread_word( fp );
       fMatch = FALSE;

       switch ( UPPER(word[0]) )
       {
	   case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
            	strncpy( pArea->name, fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
           case 'S':
             KEY( "Summer", pArea->weather->summer, fread_number( fp ) );
             KEY( "Spring", pArea->weather->spring, fread_number( fp ) );
             KEY( "Speed", pArea->weather->speed, fread_number( fp ) );
	     if ( !str_cmp( word, "Storm" ) )
	     {
		pArea->weather->storm_type[ 0 ] = fread_number( fp );
		pArea->weather->storm_type[ 1 ] = fread_number( fp );
		pArea->weather->storm_type[ 2 ] = fread_number( fp );
		pArea->weather->storm_type[ 3 ] = fread_number( fp );
		fMatch = TRUE;
	     }
             KEY( "Security", pArea->security, fread_number( fp ) );
            break;
           case 'V':
	    KEY( "Vnum", pArea->vnum, fread_number(fp) );
             KEY( "Visible", pArea->visible, fread_number( fp ) );
            if ( !str_cmp( word, "VNUMs" ) )
            {
                pArea->lvnum = fread_number( fp );
                pArea->uvnum = fread_number( fp );
		fMatch = TRUE;
            }
            break;
	   case 'W':
	    KEY( "Winter", pArea->weather->winter, fread_number(fp) );
	    KEY( "Wander_time", pArea->wander_time, fread_number(fp) );
	    KEY( "World", pArea->world, fread_number(fp) );
	    break;
           case 'A':
            if ( !str_cmp( word, "AGEs" ) )
            {
		pArea->max_age = fread_number( fp );
		pArea->max_age_empty = fread_number( fp );
		fMatch = TRUE;
            }
            break;
           case 'E':
             if ( !str_cmp( word, "End" ) )
             {
		 if ( !fWeather )
		     default_weather( pArea );	
	         if ( !str_cmp( pArea->name, "Appartments" ) )
	    	      appartment_area = pArea;
                 fMatch = TRUE;
                 if ( area_first == NULL )
                    area_first = pArea;
                 if ( area_last  != NULL )
                    area_last->next = pArea;
                 area_last   = pArea;
                 pArea->next = NULL;
		/* set sunlight type */

		if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
		    time_table[ pArea->world ].sunrise    ) pArea->weather->sunlight = SUN_DARK;
		else if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
		    time_table[ pArea->world ].sunrise +1 ) pArea->weather->sunlight = SUN_RISE;
		else if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
		    time_table[ pArea->world ].sunset     ) pArea->weather->sunlight = SUN_LIGHT;
		else if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
		    time_table[ pArea->world ].sunset + 1 ) pArea->weather->sunlight = SUN_SET;
		else			pArea->weather->sunlight = SUN_DARK;
	
		pArea->weather->change = 0;
		pArea->weather->mmhg   = 0;

		if ( time_info[ pArea->world ].month >= time_table[ pArea->world ].months_year / 2 )
		    pArea->weather->mmhg += number_range( 1, 50 );
		else
		    pArea->weather->mmhg += number_range( 1, 50 );
	 
		pArea->weather->sky =get_sky_type( pArea );

        	if( fp )
		fclose(fp);

		load_mob_new(pArea);
		load_obj_new(pArea); 
		load_rooms_new(pArea);
		load_shops_new(pArea); 
		load_event_data(pArea);
		load_resets_new(pArea);
		return;
            }
            break;
	   case 'F':
	     KEY( "Flags", pArea->area_flags, fread_number( fp ) | AREA_AUTOSAVE );
	     KEY( "Fall", pArea->weather->fall, fread_number( fp ) );
	     break;
           case 'B':
	    if ( !str_cmp( word, "Builders" ) )
	    {
            	strncpy( pArea->builders, fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }
            break;
	   case 'C':
	     /* castle code */
	     if ( !str_cmp( word, "Castle" ) )
	     {/*
		strncpy( pArea->castle_data->owner, fread_string( fp ), 30 );*/
		pArea->castle_data->castle = fread_number( fp );
		pArea->castle_data->barracks = fread_number( fp );
		pArea->castle_data->barracks2 = fread_number( fp );
		pArea->castle_data->library = fread_number( fp );
		pArea->castle_data->library2 = fread_number( fp );
		pArea->castle_data->smithy = fread_number( fp );
		pArea->castle_data->smithy2 = fread_number( fp );
		pArea->castle_data->inn = fread_number( fp );
		pArea->castle_data->inn2 = fread_number( fp );
		pArea->castle_data->armory = fread_number( fp );
		pArea->castle_data->magic_shop = fread_number( fp );
		pArea->castle_data->grainery = fread_number( fp );
		pArea->castle_data->bank = fread_number( fp );
		pArea->castle_data->track = fread_number( fp );
		fMatch = TRUE;
		break;
	     }

	     if ( !str_cmp( word, "Chance" ) )
	     {
		for ( season = 0 ; season < 4 ; season++ )
		    for( i = 0 ; i < 4 ; i++ )
			pArea->weather->chance[ season ][ i ] = fread_number(fp);
		fWeather = TRUE;
		fMatch = TRUE;
	     }
	    if ( !str_cmp( word, "Credits" ) )
	    {
            	strncpy( pArea->credits, fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }
	    break;
           case 'R':
             KEY( "Recall", pArea->recall, fread_number( fp ) );
            if ( !str_cmp( word, "Reset" ) )
            {
                fread_to_eol( fp );
                fMatch = TRUE;
            }
            break;
       }
    }
	/* set sunlight type */

             if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
	        time_table[ pArea->world ].sunrise    ) pArea->weather->sunlight = SUN_DARK;
        else if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
	        time_table[ pArea->world ].sunrise +1 ) pArea->weather->sunlight = SUN_RISE;
        else if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
	        time_table[ pArea->world ].sunset     ) pArea->weather->sunlight = SUN_LIGHT;
        else if ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day <  
	        time_table[ pArea->world ].sunset + 1 ) pArea->weather->sunlight = SUN_SET;
	else			pArea->weather->sunlight = SUN_DARK;

	pArea->weather->change = 0;
	pArea->weather->mmhg   = 0;

        if ( time_info[ pArea->world ].month >= time_table[ pArea->world ].months_year / 2 )
	    pArea->weather->mmhg += number_range( 1, 50 );
        else
	    pArea->weather->mmhg += number_range( 1, 50 );
 
	    pArea->weather->sky =get_sky_type( pArea );

    if( fp )
    fclose(fp);

    load_mob_new(pArea);
    load_obj_new(pArea); 
    load_rooms_new(pArea);
    load_shops_new(pArea); 

}


/*
 * Sets vnum range for area using OLC protection features.
 */
void assign_area_vnum( long vnum )
{
    if ( area_last->lvnum == 0 || area_last->uvnum == 0 )
	area_last->lvnum = area_last->uvnum = vnum;

    if ( vnum != URANGE( area_last->lvnum, vnum, area_last->uvnum ) )
    {
	if ( vnum < area_last->lvnum )
	    area_last->lvnum = vnum;
	else
	    area_last->uvnum = vnum;
    }

    return;
}
    
void save_triggers(CHAR_DATA *ch, char *argument)
{
    FILE *fp;
    TRIGGER_INDEX_DATA *pTrigIndex;
    int Hash_loc;
    char buf[MAX_STRING_LENGTH];
    SCRIPT_DATA *scr;

    sprintf(buf, "%s", TRIGGER_DATA_FILE );
    if ( ( fp = fopen( buf,  "w" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open %s trigger file!", buf );
        bug( b_buf, 0 );
        return;
    }
    for (Hash_loc = 0; Hash_loc < MAX_KEY_HASH; Hash_loc++)
    {
        for (pTrigIndex = trig_index_hash[Hash_loc]; pTrigIndex != NULL; pTrigIndex= pTrigIndex->next)
        {
	 if (strlen(pTrigIndex->builders) < 2)
		continue;
		
		fprintf( fp,"#TRIGGER %s~\n",pTrigIndex->name);
		fprintf( fp,"Key_sim %s~\n", pTrigIndex->key_sim );
		fprintf( fp,"Key_words %s~\n", pTrigIndex->key_words );
		fprintf( fp,"Key_string %s~\n", pTrigIndex->key_string );
		fprintf( fp,"Vnum %d\n", pTrigIndex->vnum );
		fprintf( fp,"Type %d\n", pTrigIndex->trigger_type );
		fprintf( fp,"Name %s~\n", pTrigIndex->name );
		fprintf( fp,"Desc %s~\n", pTrigIndex->desc );
		fprintf( fp,"Builders %s~\n", pTrigIndex->builders );
		fprintf( fp,"Flags %ld\n", pTrigIndex->flags ); 
		fprintf( fp,"Step %ld\n", pTrigIndex->step ); 
		fprintf( fp,"Step_pre %ld\n", pTrigIndex->step_pre ); 
		fprintf( fp,"Quests %d\n", pTrigIndex->quests ); 
		fprintf( fp,"Quests_pre %d\n", pTrigIndex->quests_pre ); 
		fprintf( fp,"Gets_vnum %ld\n", pTrigIndex->gets_vnum );
		fprintf( fp,"Mob_vnum %ld\n", pTrigIndex->mob_vnum );
		fprintf( fp,"Timer %d\n", pTrigIndex->timer );
		fprintf( fp,"Chance %d\n", pTrigIndex->chance );
		fprintf( fp,"Security %d\n", pTrigIndex->security );
		fprintf( fp,"Script_type %d\n", pTrigIndex->script_type );
    		for ( scr = pTrigIndex->script; scr != NULL ; scr = scr->next )
		{
		    fprintf( fp,"Sc\n%s~\n", scr->command );
		}
		fprintf( fp,"End\n\n" );
        }
    }
    fprintf( fp,"#END\n" );
    if( fp )
    fclose(fp);
    if (ch != NULL )
	send_to_char("Triggers saved\n\r",ch);
    else
    {
	sprintf( buf, "Autosave:  Triggers" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	REMOVE_BIT( changed_system, CHANGED_TRIGGERS );
    }
    return;
}
void save_events(AREA_DATA *pArea, char *argument)
{
    FILE *fp;
    EVENT_DATA *event;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf, "%s%s/events", SAVEAREAS_DIR, pArea->filename );
	
    if ( ( fp = fopen( buf, "w" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open %s event file!", buf );
        bug( b_buf, 0 );
        return;
    }
    for (event = pArea->events; event != NULL; event = event->next)
    {
		
	if (IS_SET(event->flags, EVENT_INVALID ) )
	    continue;
	if (pArea != event->area )
	    continue;
	fprintf( fp,"#EVENT\n");
	fprintf( fp,"Vnum %ld\n", event->vnum );
	fprintf( fp,"Mob_vnum %ld\n", event->mob_vnum );
	fprintf( fp,"Obj_vnum %ld\n", event->obj_vnum );
	fprintf( fp,"Room_vnum %ld\n", event->room_vnum );
	fprintf( fp,"Type %d\n", event->type );
	fprintf( fp,"Flags %ld\n", event->flags ); 
	fprintf( fp,"Chance %d\n", event->chance );
	fprintf( fp,"End\n\n" );
    }
    fprintf( fp,"#END\n" );
    if( fp )
    fclose(fp);
    return;
}

/* snarf a socials file */
void load_socials( )
{
    FILE *fp;
    char buf[MAX_STRING_LENGTH];

    sprintf(buf, "%s", SOCIALSAVE_FILE );
    if ( ( fp = fopen( buf,  "r" ) ) == NULL )
    {
        char b_buf[MAX_STRING_LENGTH];
        sprintf( b_buf, "Cannot open %s social file!", buf );
        bug( b_buf, 0 );
        return;
    }
    for ( ; ; ) 
    {
    	struct social_type social;
    	char *temp;
        /* clear social */
	social.char_no_arg[0] = '\0';
	social.others_no_arg[0] = '\0';
	social.char_found[0] = '\0';
	social.others_found[0] = '\0';
	social.vict_found[0] = '\0'; 
	social.char_not_found[0] = '\0';
	social.char_auto[0] = '\0';
	social.others_auto[0] = '\0';

    	temp = fread_word(fp);
    	if (!str_cmp(temp,"#0"))
	    return;  /* done */

    	strcpy(social.name,temp);
	social.type   = fread_number(fp); 
	social.damage = fread_number(fp);
	social.item   = fread_number(fp);
	social.action = fread_number(fp);
	social.mood   = fread_number(fp);
    	fread_to_eol(fp);

	temp = fread_string_eol(fp);
	if (!str_cmp(temp,"$") || !str_cmp(temp,"(null)" ) )
	     social.char_no_arg[0] = '\0';
	else if (!str_cmp(temp,"#"))
	{
	     social_table[social_count] = social;
	     social_count++;
	     continue; 
	}
        else
	    strncpy( social.char_no_arg, temp, 80 );

        temp = fread_string_eol(fp);
        if (!str_cmp(temp,"$") || !str_cmp(temp,"(null)" ) )
             social.others_no_arg[0] = '\0';
        else if (!str_cmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    strncpy( social.others_no_arg, temp, 80 );

        temp = fread_string_eol(fp);
        if (!str_cmp(temp,"$") || !str_cmp(temp, "(null)" ) )
             social.char_found[0] = '\0';
        else if (!str_cmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
       	else
	    strncpy( social.char_found, temp, 80 );

        temp = fread_string_eol(fp);
        if (!str_cmp(temp,"$") || !str_cmp(temp, "(null)" ) )
             social.others_found[0] = '\0';
        else if (!str_cmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    strncpy( social.others_found, temp, 80 ); 

        temp = fread_string_eol(fp);
        if (!str_cmp(temp,"$") || !str_cmp(temp, "(null)" ) )
             social.vict_found[0] = '\0';
        else if (!str_cmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    strncpy( social.vict_found, temp, 80 );

        temp = fread_string_eol(fp);
        if (!str_cmp(temp,"$") || !str_cmp(temp, "(null)" ) )
             social.char_not_found[0] = '\0';
        else if (!str_cmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    strncpy( social.char_not_found, temp, 80 );

        temp = fread_string_eol(fp);
        if (!str_cmp(temp,"$") || !str_cmp(temp, "(null)" ) )
             social.char_auto[0] = '\0';
        else if (!str_cmp(temp,"#"))
        {
	     social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    strncpy( social.char_auto, temp, 80 );
         
        temp = fread_string_eol(fp);
        if (!str_cmp(temp,"$") || !str_cmp(temp, "(null)" ) )
             social.others_auto[0] = '\0';
        else if (!str_cmp(temp,"#"))
        {
             social_table[social_count] = social;
             social_count++;
             continue;
        }
        else
	    strncpy( social.others_auto, temp, 80 ); 
	
	social_table[social_count] = social;
    	social_count++;
   }
   if( fp )
   fclose(fp);
   return;
}
    
char *fread_string( FILE *fp )
{
    char *plast;
    char c;

    plast = top_string + sizeof(char *);
    if ( plast > &string_space[MAX_STRING - MAX_STRING_LENGTH] )
    {
	bug( "Fread_string: MAX_STRING %d exceeded.", MAX_STRING );
	exit( 1 );
    }

    /*
     * Skip blanks.
     * Read first char.
     */
    do
    {
	c = getc( fp );
    }
    while ( isspace(c) );

    if ( ( *plast++ = c ) == '~' )
	return &str_empty[0];

    for ( ;; )
    {
        /*
         * Back off the char type lookup,
         *   it was too dirty for portability.
         *   -- Furey
         */

	switch ( *plast = getc(fp) )
	{
        default:
            plast++;
            break;
 
        case EOF:
	/* temp fix */
            bug( "Fread_string: EOF", 0 );
	    return NULL;
            /* exit( 1 ); */
            break;
 
        case '\n':
            plast++;
            *plast++ = '\r';
            break;
 
        case '\r':
            break;
 
        case '~':
            plast++;
	    {
		union
		{
		    char *	pc;
		    char	rgc[sizeof(char *)];
		} u1;
		int ic;
		int iHash;
		char *pHash;
		char *pHashPrev;
		char *pString;

		plast[-1] = '\0';
		iHash     = UMIN( MAX_KEY_HASH - 1, plast - 1 - top_string );
		for ( pHash = string_hash[iHash]; pHash; pHash = pHashPrev )
		{
		    for ( ic = 0; ic < (signed) sizeof(char *); ic++ )
			u1.rgc[ic] = pHash[ic];
		    pHashPrev = u1.pc;
		    pHash    += sizeof(char *);

		    if ( top_string[(signed) sizeof(char *)] == pHash[0]
		    &&   !str_cmp( top_string+sizeof(char *)+1, pHash+1 ) )
			return pHash;
		}

		if ( fBootDb )
		{
		    pString		= top_string;
		    top_string		= plast;
		    u1.pc		= string_hash[iHash];
		    for ( ic = 0; ic < (signed) sizeof(char *); ic++ )
			pString[ic] = u1.rgc[ic];
		    string_hash[iHash]	= pString;

		    nAllocString += 1;
		    sAllocString += top_string - pString;
		    return pString + sizeof(char *);
		}
		else
		{
		    return str_dup( top_string + sizeof(char *) );
		}
	    }
	}
    }
}

void save_finger( void )
{
    FINGER_DATA * finger;
    FILE *fp;

    if ( ( fp = fopen( FINGER_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save finger file.", 0 );
        if( fp )
	fclose( fp );
        return;
    }
 
    for ( finger = finger_list ; finger != NULL ; finger = finger->next )
    {
	fprintf( fp, "Name %s\n", finger->name );
	fprintf( fp, "Title %s~\n", finger->title );
	fprintf( fp, "Desc %s~\n", finger->desc );
	fprintf( fp, "Email %s~\n", finger->email );
	fprintf( fp, "Laston %ld\n", finger->laston );
	fprintf( fp, "Level %d\n", finger->level );
	fprintf( fp, "Race %d\n", finger->race );
	fprintf( fp, "Sex %d\n", finger->sex );
    }

    fprintf( fp, "End\n" );
 
    if( fp )
    fclose( fp );
    return;
}

void load_finger( void )
{
    char buf[MAX_INPUT_LENGTH];
    FINGER_DATA * finger;
    FILE *fp;
    char * word = NULL;
    bool fMatch = FALSE;

    finger_list = NULL;

    if ( ( fp = fopen( FINGER_FILE, "r" ) ) == NULL )
        return;

    finger = NULL;

    for ( ; ; )
    {
        word   = feof( fp ) ? str_dup("End") : fread_word( fp );
        fMatch = FALSE;

	if ( finger == NULL && str_cmp( word, "Name" ) )
	    finger = finger_alloc();

        switch( word[0] )
	{
	default: break;
	case 'D':
	     KEY( "Desc", finger->desc, fread_string( fp ) );
	     break;
	case 'E':
             if ( !str_cmp( word, "End" ) )
             {
                 fMatch = TRUE;
                 return;
             }
	     if ( !str_cmp( word, "Email" ) )
	     {
	     	strncpy( finger->email, fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	     }
	     break;
	case 'L':
	     KEY( "Level", finger->level, fread_number( fp ) );
	     KEY( "Laston", finger->laston, fread_number( fp ) );
	     break;
	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
		finger = finger_alloc();

		strncpy( finger->name, fread_word( fp ), 30 );
		finger->next = finger_list;
		finger_list = finger;
		fMatch = TRUE;
		break;
	    }
	    break;
	case 'R':
	    KEY( "Race", finger->race, fread_number( fp ) );
	    break;
	case 'S':
	    KEY( "Sex", finger->sex, fread_number( fp ) );
	    break;
	case 'T':
	    if ( !str_cmp( word, "Title" ) )
	    {
	    	strncpy( finger->title, fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    break;
	}

	if ( !fMatch )
	{
	    sprintf( buf, "Load finger - %s not found\n", word );
	    log_string( buf );
	    fread_to_eol( fp );
	}

    }

}  

void load_wizlist( void )
{
    FILE *fp;
    WIZLIST_DATA *wizlist;
    WIZLIST_DATA *wizlist_last;
    wizlist_last = NULL; 
    if ( ( fp = fopen( WIZLIST_DATA_FILE, "r" ) ) == NULL )
        return;
 
    for ( ; ; )
    {
        char letter;

        do
        {
            letter = getc( fp );
            if ( feof(fp) )
            {
    		if( fp )
                fclose( fp );
                return;
            }
        }
        while ( isspace(letter) );
        ungetc( letter, fp );
 
        wizlist           = wizlist_data_alloc();
 
        if ( str_cmp( fread_word( fp ), "name" ) )
            break;
        strncpy( wizlist->name, fread_string( fp ), 30 );

        if ( str_cmp( fread_word( fp ), "title" ) )
            break;
        strncpy( wizlist->title, fread_string( fp ), 30 );

        if ( str_cmp( fread_word( fp ), "desc" ) )
            break;
        strncpy( wizlist->desc, fread_string( fp ), 80 );

        if ( str_cmp( fread_word( fp ), "order" ) )
            break;
        wizlist->order   = fread_number( fp );

        if ( wizlist_data_list == NULL )
            wizlist_data_list = wizlist;

        if ( wizlist_last  != NULL )
            wizlist_last->next = wizlist;
 
        wizlist_last       = wizlist;
        wizlist->next     = NULL;

    }
 
    strcpy( strArea, WIZLIST_DATA_FILE );
    fpArea = fp;
    bug( "Load_wizlist: bad key word.", 0 );
    exit( 1 );
    return;
}


void save_wizlist( void )
{
    WIZLIST_DATA *wizlist;
    FILE *fp;
    if ( ( fp = fopen( WIZLIST_DATA_FILE, "w" ) ) == NULL )
    {
	bug("Cannot save wizlist file.", 0 );
    	if( fp )
	fclose( fp );
        return;
    }
 
    for ( wizlist = wizlist_data_list ; wizlist != NULL ; wizlist = wizlist->next )
    {
	if ( !IS_SET(wizlist->flags, WIZLIST_DELETED ) )
	{
	fprintf( fp, "name %s~\n", wizlist->name );
	fprintf( fp, "title %s~\n", wizlist->title );
	fprintf( fp, "desc %s~\n", wizlist->desc );
	fprintf( fp, "order %d\n", wizlist->order );
	}
    }
 
    if( fp )
    fclose( fp );
    return;
}

