#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "merc.h"

TRIGGER_DATA 		*	trigger_list		= NULL;
BUFFER 			*	buf_free;
// Already defined in db.c, Reverie: CLAN_DATA		*	clan_list;
BHOST_LIST		*	bhost_list_free		= NULL;
AFFECT_DATA		*	affect_free		= NULL;
RESET_DATA		*	reset_free		= NULL;
TRIGGER_DATA		*	trigger_free		= NULL;
TRIGGER_LIST_DATA	*	trigger_list_free	= NULL;
SCRIPT_DATA		*	script_free		= NULL;
VARIABLE_DATA		*	variable_free		= NULL;
LIGHT_DATA		*	light_data_free		= NULL;
APPROVE_DATA		*	approve_data_free	= NULL;
WEAR_DATA		*	wear_data_free		= NULL;
EQUIP_DATA		*	equip_data_free		= NULL;
INSIDE_DATA		*	inside_data_free	= NULL;
MAGIC_DATA		*	magic_data_free		= NULL;
WEAPON_DATA		*	weapon_data_free	= NULL;
EDIBLE_DATA		*	edible_data_free	= NULL;
CONT_DATA		*	cont_data_free		= NULL;
GATE_DATA		*	gate_data_free		= NULL;
TRAP_DATA		*	trap_data_free		= NULL;
MOVEABLE_DATA		*	moveable_data_free	= NULL;
LOGON_DATA		*	logon_data_free		= NULL;
GLOBAL_RANDOM_DATA	*	random_data_free	= NULL;
LOCK_DATA		*	lock_data_free		= NULL;
TRACK_DATA		*	track_data_free		= NULL;
TRACK_TYPE		*	track_type_free		= NULL;
INSIDE_AREA_DATA	*	inside_area_data_free	= NULL;
SPELL_LIST		*	spell_list_free		= NULL;
RESET_DATA		*	reset_data_free		= NULL;
EVENT_DATA		*	event_data_free		= NULL;
QUEST_DATA		*	quest_data_free		= NULL;
CHAR_QUESTS		*	char_quests_free	= NULL;
AREA_DATA		*	area_free		= NULL;
HELP_DATA		*	help_free		= NULL;
// Already defined in db.c, Reverie: HELP_DATA		*	help_last		= NULL;
BFS_QUEUE		*	bfs_queue_free		= NULL;
BFS_ROOM		*	bfs_room_free		= NULL;
WORLD_DATA		*	world_free		= NULL;
// Already defined in db.c, Reverie: EXTRA_DESCR_DATA	*	extra_descr_free	= NULL;
EXIT_DATA		*	exit_free		= NULL;
ROOM_INDEX_DATA		*	room_index_free		= NULL;
OBJ_INDEX_DATA		*	obj_index_free		= NULL;
SHOP_DATA		*	shop_free		= NULL;
MOB_INDEX_DATA		*	mob_index_free		= NULL;
READ_NOTES		*	read_notes_free		= NULL;
DEBT_DATA		*	debt_data_free		= NULL;
CLAN_DATA		*	clan_data_free		= NULL;
PC_CLAN_DATA		*	pcclan_data_free	= NULL;
GAINER_DATA		*	gainer_data_free	= NULL;
WEATHER_DATA		*	weather_data_free	= NULL;
BET_DATA		*	bet_data_free		= NULL;
NOTE_DATA		*	note_data_free		= NULL;
EDITING_DATA		*	editing_data_free	= NULL;
GROUP_DATA		*	group_data_free		= NULL;
BAN_DATA		*	ban_data_free		= NULL;
SKILL_LIST		*	skill_list_free		= NULL;
CASTLE_DATA		*	castle_data_free	= NULL; /* castle code */
WIZLIST_DATA		*	wizlist_data_free	= NULL;
FINGER_DATA		*	finger_data_free	= NULL;
SPECPRO_LIST		*	specpro_list_free	= NULL;
SNOOP_LIST		*	snoop_list_free		= NULL;
ROOM_SNOOP_LIST		*	room_snoop_list_free	= NULL;
extern          int                     top_buffer;
extern          int                     top_specpro_list;
extern          int                     top_ban_data;
extern          int                     top_skill_list;
extern		int			top_castle_data; /* castle code */
extern          int                     top_bhost_list;
extern          int                     top_group_data;
extern          int                     top_editing_data;
extern          int                     top_note_data;
extern          int                     top_bet_data;
extern          int                     top_weather_data;
extern          int                     top_gainer_data;
extern          int                     top_clan_data;
extern		long			top_pcclan_data;
extern          int                     top_debt_data;
extern          long                    top_read_notes_data;
extern          long                    top_cont_data;
extern          long                    top_track_data;
extern          long                    top_track_type;
extern          long                    top_inside_area_data;
extern          long                    top_spell_data;
extern          long                    top_gate_data;
extern          int                     top_numarea;
extern          long                    top_exit;
extern          int                     top_ed;
extern          long                    top_room;
extern          long                    top_light_data;
extern          long                    top_approve_data;
extern          long                    top_lock_data;
extern          long                    top_trigger_data;
extern          long                    top_trigger_index;
extern          long                    top_trigger_list_data;
extern          long                    top_script_data;
extern          long                    top_variable_data;
extern          long                    top_wear_data;
extern          long                    top_equip_data;
extern          long                    top_inside_data;
extern          long                    top_weapon_data;
extern          long                    top_edible_data;
extern          long                    top_trap_data;
extern          long                    top_moveable_data;
extern          long                    top_magic_data;
extern          long                    top_logon_data;
extern          long                    top_random_data;
extern		int			social_count;
extern		int			object_count;
extern		int			top_reset;
extern		int			top_event;
extern		int			top_quest;
extern		int			top_char_quests;
extern		int			top_finger_data;
extern		int			top_wizlist_data;
extern		long			top_room_index;
extern		long			top_room;

DECLARE_SPEC_FUN( spec_thug );
DECLARE_SPEC_FUN( spec_nightmare );

SNOOP_LIST * snoop_list_alloc( void )
{
    SNOOP_LIST * snoop_list;
 
    if ( snoop_list_free == NULL )
    {
        snoop_list = (SNOOP_LIST *) alloc_perm( sizeof( SNOOP_LIST ) );
    }
    else
    {
	snoop_list = snoop_list_free;
	snoop_list_free = snoop_list_free->next;
    }

    snoop_list->next = NULL;
    snoop_list->desc = NULL;

    return snoop_list;
}

ROOM_SNOOP_LIST * room_snoop_list_alloc( void )
{
    ROOM_SNOOP_LIST * room_snoop_list;
 
    if ( room_snoop_list_free == NULL )
    {
        room_snoop_list = (ROOM_SNOOP_LIST *) alloc_perm( sizeof( ROOM_SNOOP_LIST ) );
    }
    else
    {
	room_snoop_list = room_snoop_list_free;
	room_snoop_list_free = room_snoop_list_free->next;
    }

    room_snoop_list->next = NULL;
    room_snoop_list->room = NULL;

    return room_snoop_list;
}

void free_snoop_list( SNOOP_LIST * snoop_list )
{
     snoop_list->next = snoop_list_free;
     snoop_list_free = snoop_list;
}
void free_random_data( GLOBAL_RANDOM_DATA * random_data )
{
     random_data->next = random_data_free;
     random_data_free = random_data;
}

void free_room_snoop_list( ROOM_SNOOP_LIST * room_snoop_list )
{
     room_snoop_list->next = room_snoop_list_free;
     room_snoop_list_free = room_snoop_list;
}

FINGER_DATA * finger_alloc( void )
{
    FINGER_DATA * finger_data;
 
    if ( finger_data_free == NULL )
    {
    finger_data = (FINGER_DATA *) alloc_perm( sizeof( FINGER_DATA ) );
    top_finger_data++;
    }
    else
    {
    finger_data = finger_data_free;
    finger_data_free = finger_data_free->next;
    }

    finger_data->next	= NULL;
    finger_data->name[0]	= '\0';
    finger_data->title[0]	= '\0';
    finger_data->desc	= &str_empty[0];
    finger_data->email[0]	= '\0';
    finger_data->laston	= 0;
    finger_data->level	= 0;
    finger_data->race	= 0;
    finger_data->sex	= 0;

    return finger_data;
}

WIZLIST_DATA * wizlist_data_alloc( void )
{
    WIZLIST_DATA * wizlist_data;
 
    if ( wizlist_data_free == NULL )
    {
    wizlist_data = (WIZLIST_DATA *) alloc_perm( sizeof( WIZLIST_DATA ) );
    top_wizlist_data++;
    }
    else
    {
    wizlist_data = wizlist_data_free;
    wizlist_data_free = wizlist_data_free->next;
    }

    wizlist_data->name[0]	= '\0';
    wizlist_data->title[0]	= '\0';
    wizlist_data->desc[0]	= '\0';
    wizlist_data->order		= 4;
    wizlist_data->flags		= 0;

    return wizlist_data;
}

CLAN_DATA * clan_data_alloc( void )
{
    CLAN_DATA * clan_data;
 
    if ( clan_data_free == NULL )
    {
    clan_data = (CLAN_DATA *) alloc_perm( sizeof( CLAN_DATA ) );
    top_clan_data++;
    }
    else
    {
    clan_data = clan_data_free;
    clan_data_free = clan_data_free->next;
    }

    clan_data->name[0]		= '\0';
    clan_data->title[0]		= '\0';
    clan_data->leader		= NULL;
    clan_data->leader_name[0]	= '\0';
    clan_data->sponser		= NULL;
    clan_data->sponser_name[0]	= '\0';
    clan_data->treasury		= 0;
    clan_data->clan_info	= 0;
    clan_data->recall_room	= 2;
    clan_data->donate_room	= 2;
    clan_data->rank1[0]		= '\0';
    clan_data->rank2[0]         = '\0';
    clan_data->rank3[0]         = '\0';
    clan_data->rank4[0]         = '\0';
    clan_data->rank5[0]         = '\0';
    clan_data->rank6[0]         = '\0';
    clan_data->rank7[0]         = '\0';
    clan_data->rank8[0]         = '\0';
    clan_data->rank9[0]         = '\0';
    clan_data->rank10[0]        = '\0';

    return clan_data;
}

PC_CLAN_DATA * pcclan_data_alloc( void )
{
    PC_CLAN_DATA * pcclan_data;

    if ( pcclan_data_free == NULL )
    {
    pcclan_data = (PC_CLAN_DATA *) alloc_perm( sizeof( PC_CLAN_DATA ) );
    top_pcclan_data++;
    }
    else
    {
    pcclan_data = pcclan_data_free;
    pcclan_data_free = pcclan_data_free->next;
    }

    pcclan_data->clan           = NULL;
    pcclan_data->clanrank       = 0;

   return pcclan_data;
}

BHOST_LIST * bhost_list_alloc( void )
{
    BHOST_LIST * bhost_list;
 
    if ( bhost_list_free == NULL )
    {
    bhost_list = (BHOST_LIST *) alloc_perm( sizeof( BHOST_LIST ) );
    top_bhost_list++;
    }
    else
    {
    bhost_list = bhost_list_free;
    bhost_list_free = bhost_list_free->next;
    }

    bhost_list->time		= current_time + 1500;
    bhost_list->host[0]		= '\0';

    return bhost_list;
}
DEBT_DATA * debt_data_alloc( void )
{
    DEBT_DATA * debt_data;
 
    if ( debt_data_free == NULL )
    {
    debt_data = (DEBT_DATA *) alloc_perm( sizeof( DEBT_DATA ) );
    top_debt_data++;
    }
    else
    {
    debt_data = debt_data_free;
    debt_data_free = debt_data_free->next;
    }

    debt_data->due	= 0;
    debt_data->amount	= 0;
    debt_data->vnum	= 2;
    debt_data->type	= 0;
    debt_data->timer	= 0;

    return debt_data;
}
SKILL_LIST * skill_list_alloc( void )
{
    SKILL_LIST * skill;
 
    if ( skill_list_free == NULL )
    {
    skill = (SKILL_LIST *) alloc_perm( sizeof( SKILL_LIST ) );
    top_skill_list++;
    }
    else
    {
    skill = skill_list_free;
    skill_list_free = skill_list_free->next;
    }

    skill->sn	= 0;

    return skill;
}

/* castle code */
CASTLE_DATA * castle_data_alloc( void )
{
    CASTLE_DATA * castle_data;

    if ( castle_data_free == NULL )
    {
        castle_data = (CASTLE_DATA * ) alloc_perm( sizeof( CASTLE_DATA ) );
        top_castle_data++;
    }
    else
    {
        castle_data = castle_data_free;
        castle_data_free = castle_data_free->next;
    }

    castle_data->owner[30] = '\0';
    castle_data->castle = 0;
    castle_data->barracks = 0;
    castle_data->barracks2 = 0;
    castle_data->library = 0;
    castle_data->library2 = 0;
    castle_data->smithy = 0;
    castle_data->smithy2 = 0;
    castle_data->inn = 0;
    castle_data->inn2 = 0;
    castle_data->armory = 0;
    castle_data->magic_shop = 0;
    castle_data->grainery = 0;
    castle_data->bank = 0;
    castle_data->track = 0;
    return castle_data;
}

BAN_DATA * ban_data_alloc( void )
{
    BAN_DATA * ban_data;
 
    if ( ban_data_free == NULL )
    {
    ban_data = (BAN_DATA *) alloc_perm( sizeof( BAN_DATA ) );
    top_ban_data++;
    }
    else
    {
    ban_data = ban_data_free;
    ban_data_free = ban_data_free->next;
    }

    ban_data->name[0]	= '\0';

    return ban_data;
}

SPECPRO_LIST * specpro_list_alloc( void )
{
    SPECPRO_LIST * specpro_list;
 
    if ( specpro_list_free == NULL )
    {
    specpro_list = (SPECPRO_LIST *) alloc_perm( sizeof( SPECPRO_LIST ) );
    top_specpro_list++;
    }
    else
    {
    specpro_list = specpro_list_free;
    specpro_list_free = specpro_list_free->next;
    }

    return specpro_list;
}
GROUP_DATA * group_data_alloc( void )
{
    GROUP_DATA * group_data;
	sh_int x;
 
    if ( group_data_free == NULL )
    {
    group_data = (GROUP_DATA *) alloc_perm( sizeof( GROUP_DATA ) );
    top_group_data++;
    }
    else
    {
    group_data = group_data_free;
    group_data_free = group_data_free->next;
    }

    group_data->name[0]		= '\0';
    group_data->leader		= NULL;
    group_data->treasurer	= NULL;
    for ( x = 0; x < MAX_WORLD; x++ )
	group_data->gold[x] 	= 0;
    group_data->pkills		= 0;
    group_data->mob_kills	= 0;
    group_data->flags		= 0;

    return group_data;
}
EDITING_DATA * editing_data_alloc( void )
{
    EDITING_DATA * editing_data;
 
    if ( editing_data_free == NULL )
    {
    editing_data = (EDITING_DATA *) alloc_perm( sizeof( EDITING_DATA ) );
    top_editing_data++;
    }
    else
    {
    editing_data = editing_data_free;
    editing_data_free = editing_data_free->next;
    }

    editing_data->vnum		= 0;

    return editing_data;
}
NOTE_DATA * note_data_alloc( void )
{
    NOTE_DATA * note_data;
 
    if ( note_data_free == NULL )
    {
    note_data = (NOTE_DATA *) alloc_perm( sizeof( NOTE_DATA ) );
    top_note_data++;
    }
    else
    {
    note_data = note_data_free;
    note_data_free = note_data_free->next;
    }

    note_data->sender[0]	= '\0';
    note_data->date[0]		= '\0';
    note_data->to_list[0]	= '\0';
    note_data->subject[0]	= '\0';
    note_data->text		= &str_empty[0];
    note_data->date_stamp	= current_time;
    note_data->read_list[0]	= '\0';
    note_data->flags		= 0;

    return note_data;
}
BET_DATA * bet_data_alloc( void )
{
    BET_DATA * bet_data;
 
    if ( bet_data_free == NULL )
    {
    bet_data = (BET_DATA *) alloc_perm( sizeof( BET_DATA ) );
    top_bet_data++;
    }
    else
    {
    bet_data = bet_data_free;
    bet_data_free = bet_data_free->next;
    }

    bet_data->ch	= NULL;
    bet_data->amount	= 0;
    bet_data->coin_type	= 0;
    bet_data->horse	= 0;
    bet_data->odds	= 0;

    return bet_data;
}
WEATHER_DATA * weather_data_alloc( void )
{
    WEATHER_DATA * weather;
 
    if ( weather_data_free == NULL )
    {
    weather = (WEATHER_DATA *) alloc_perm( sizeof( WEATHER_DATA ) );
    top_weather_data++;
    }
    else
    {
    weather = weather_data_free;
    weather_data_free = weather_data_free->next;
    }

    weather->spring = 2;
    weather->summer = 5;
    weather->fall   = 8;
    weather->winter = 11;
    weather->season = 0;
    weather->chance[ 0 ][ 0 ] = 0;
    weather->chance[ 0 ][ 1 ] = 25;
    weather->chance[ 0 ][ 2 ] = 50;
    weather->chance[ 0 ][ 3 ] = 75;
    weather->chance[ 1 ][ 0 ] = 0;
    weather->chance[ 1 ][ 1 ] = 25;
    weather->chance[ 1 ][ 2 ] = 50;
    weather->chance[ 1 ][ 3 ] = 75;
    weather->chance[ 2 ][ 0 ] = 0;
    weather->chance[ 2 ][ 1 ] = 25;
    weather->chance[ 2 ][ 2 ] = 50;
    weather->chance[ 2 ][ 3 ] = 75;
    weather->chance[ 3 ][ 0 ] = 0;
    weather->chance[ 3 ][ 1 ] = 25;
    weather->chance[ 3 ][ 2 ] = 50;
    weather->chance[ 3 ][ 3 ] = 75;
    weather->storm_type[ 0 ] = SKY_LIGHTNING;
    weather->storm_type[ 1 ] = SKY_LIGHTNING;
    weather->storm_type[ 2 ] = SKY_LIGHTNING;
    weather->storm_type[ 3 ] = SKY_LIGHTNING;
    weather->speed = 6;
    weather->mmhg = 0;
    weather->change = 0;
    weather->sky = SKY_CLOUDLESS;
    weather->sunlight = SUN_RISE;

    return weather;
}

READ_NOTES * read_notes_alloc( void )
{
    READ_NOTES * read_notes;
 
    if ( read_notes_free == NULL )
    {
    read_notes = (READ_NOTES *) alloc_perm( sizeof( READ_NOTES ) );
    top_read_notes_data++;
    }
    else
    {
    read_notes = read_notes_free;
    read_notes_free = read_notes_free->next;
    }

    read_notes->date_stamp = current_time;

    return read_notes;
}
GAINER_DATA * gainer_data_alloc( void )
{
    GAINER_DATA * gainer;
    int x;
 
    if ( gainer_data_free == NULL )
    {
    gainer = (GAINER_DATA *) alloc_perm( sizeof( GAINER_DATA ) );
    top_gainer_data++;
    }
    else
    {
    gainer = gainer_data_free;
    gainer_data_free = gainer_data_free->next;
    }

    for ( x = 0; x < MAX_SKILL; x++ )
    gainer->skill[x] = FALSE;

    return gainer;
}

RESET_DATA * reset_alloc( void )
{
    RESET_DATA * reset;

    if ( reset_data_free == NULL )
    {
    reset = (RESET_DATA *) alloc_perm( sizeof( RESET_DATA ) );
    top_reset++;
    }
    else
    {

    reset = reset_data_free;
    reset_data_free = reset_data_free->next;

    }
    reset->next = NULL;
    reset->command = ' ';
    reset->arg1 = 0;
    reset->arg2 = 0;
    reset->arg3 = 0;
    reset->vnum = 0;
    reset->chance = 100;

    return reset;
}
EVENT_DATA * event_alloc( void )
{
    EVENT_DATA * event;

    if ( event_data_free == NULL )
    {
    event = (EVENT_DATA *) alloc_perm( sizeof( EVENT_DATA ) );
    top_event++;
    event->vnum		= top_event;
    }
    else
    {
    event = event_data_free;
    event_data_free = event_data_free->next;
    }

/*
 * Don't reset the vnum!
 */
 
    event->next = NULL;
    event->flags	= 0;
    event->type		= 0;
    event->chance	= 100;
    event->mob		= NULL;
    event->obj		= NULL;
    event->room		= NULL;
    event->room_vnum	= 0;
    event->mob_vnum	= 0;
    event->obj_vnum	= 0;
    event->obj_vnum_to	= 0;
    event->area		= NULL;

    return event;
}

TRIGGER_INDEX_DATA * pTrigIndex_alloc( void )
{
    TRIGGER_INDEX_DATA * pTrigIndex;

    pTrigIndex = (TRIGGER_INDEX_DATA *) alloc_perm( sizeof( TRIGGER_INDEX_DATA ) );


    pTrigIndex->next 		= NULL;
    pTrigIndex->script		= NULL;
    pTrigIndex->trigger_type	= 0;
    pTrigIndex->bits		= 0;
    pTrigIndex->quests		= 0;
    pTrigIndex->quests_pre	= 0;
    pTrigIndex->flags		= TRIG_PC;
    pTrigIndex->step		= 0;
    pTrigIndex->step_pre	= 0;
    pTrigIndex->vnum		= 0;
    pTrigIndex->gets_vnum	= 0;
    pTrigIndex->timer		= 0;
    pTrigIndex->mob_vnum	= 0;
    pTrigIndex->script_type	= 0;
    pTrigIndex->security	= 9;
    pTrigIndex->chance		= 100;
    pTrigIndex->keywords[0]	= '\0';
    pTrigIndex->key_sim[0]	= '\0';
    pTrigIndex->key_words[0]	= '\0';
    pTrigIndex->key_string[0]	= '\0';
    pTrigIndex->builders[0]	= '\0';
    pTrigIndex->name[0]		= '\0';
    pTrigIndex->desc[0]		= '\0';
    top_trigger_index++;
    return pTrigIndex;
}

TRIGGER_DATA * trigger_alloc( void )
{
    TRIGGER_DATA * trigger;

    if ( trigger_free == NULL )
    {
    trigger = (TRIGGER_DATA *) alloc_perm( sizeof( TRIGGER_DATA ) );
    top_trigger_data++;
    }
    else
    {

    trigger = trigger_free;
    trigger_free = trigger_free->next;

    }

    trigger->next 		= NULL;
    trigger->area 		= NULL;
    trigger->waiting		= 0;
    trigger->current		= NULL;
    trigger->script		= NULL;
    trigger->trigger_type	= 0;
    trigger->bits		= 0;
    trigger->timer		= 0;
    trigger->flags		= 0;
    trigger->step		= 0;
    trigger->step_pre		= 0;
    trigger->quests		= 0;
    trigger->quests_pre		= 0;
    trigger->tracer		= 0;
    trigger->attempts		= 0;
    trigger->activated		= 0;
    trigger->waiting		= 0;
    trigger->vnum		= 0;
    trigger->gets_vnum		= 0;
    trigger->mob_vnum		= 0;
    trigger->keywords[0]	= '\0';
    trigger->key_sim[0]		= '\0';
    trigger->key_words[0]	= '\0';
    trigger->key_string[0]	= '\0';
    trigger->name[0]		= '\0';
	
    /* Link it to the full list */
    trigger->next_in_list = trigger_list;
    trigger_list	 = trigger;

    return trigger;
}

TRIGGER_LIST_DATA * trigger_list_alloc( void )
{
    TRIGGER_LIST_DATA * trigger_list_data;

    if ( trigger_list_free == NULL )
    {
    trigger_list_data = (TRIGGER_LIST_DATA *) alloc_perm( sizeof( TRIGGER_LIST_DATA ) );
    top_trigger_list_data++;
    }
    else
    {

    trigger_list_data = trigger_list_free;
    trigger_list_free = trigger_list_free->next;

    }

    trigger_list_data->next 		= NULL;
    trigger_list_data->vnum		= 0;
	
    return trigger_list_data;
}

QUEST_DATA * quest_data_alloc( void )
{
    QUEST_DATA * quest_data;

    if ( quest_data_free == NULL )
    {
    quest_data = (QUEST_DATA *) alloc_perm( sizeof( QUEST_DATA ) );
    top_quest++;
    }
    else
    {

    quest_data = quest_data_free;
    quest_data_free = quest_data_free->next;

    }

    quest_data->next 		= NULL;
    quest_data->flag		= 0;
    quest_data->quest_number	= 0;
    quest_data->name[0]		= '\0';
    quest_data->description[0]	= '\0';
	
    return quest_data;
}
CHAR_QUESTS * char_quests_data_alloc( void )
{
    CHAR_QUESTS * char_quests;

    if ( char_quests_free == NULL )
    {
    char_quests = (CHAR_QUESTS *) alloc_perm( sizeof( CHAR_QUESTS ) );
    top_char_quests++;
    }
    else
    {

    char_quests = char_quests_free;
    char_quests_free = char_quests_free->next;

    }

    char_quests->next 		= NULL;
    char_quests->flags		= 0;
    char_quests->quest_number	= 0;
	
    return char_quests;
}

LIGHT_DATA * light_alloc( void )
{
    LIGHT_DATA * light;

    if ( light_data_free == NULL )
    {

    light = (LIGHT_DATA *) alloc_perm( sizeof( LIGHT_DATA ) );
    top_light_data++;
    }
    else
    {

    light = light_data_free;
    light_data_free = light_data_free->next;

    }

    light->flags = 0;
    light->fuel = 1;
    light->max_fuel = 1;
    light->brightness = 0;

    return light;
    
}

APPROVE_DATA * approve_data_alloc( void )
{
    APPROVE_DATA * approve_data;

    if ( approve_data_free == NULL )
    {

    approve_data = (APPROVE_DATA *) alloc_perm( sizeof( APPROVE_DATA ) );
    top_approve_data++;
    }
    else
    {

    approve_data = approve_data_free;
    approve_data_free = approve_data_free->next;

    }

    approve_data->flags		= 0;
    approve_data->creater[0]	= '\0';
    approve_data->approver[0]	= '\0';
    approve_data->modifier[0]	= '\0';
    approve_data->modified_date	= current_time;
    approve_data->approved_date	= current_time;
    approve_data->creation_date	= current_time;

    return approve_data;
    
}

LOCK_DATA * lock_alloc( void )
{
    LOCK_DATA * lock;

    if ( lock_data_free == NULL )
    {
 
    lock = (LOCK_DATA *) alloc_perm( sizeof( LOCK_DATA ) );
    top_lock_data++;
    }
    else
    {

    lock = lock_data_free;
    lock_data_free = lock_data_free->next;

    }

    lock->trap = NULL;
    lock->pick_penalty = 0;
    lock->lock_info = 0;
    lock->vnum = 0;
    lock->next = NULL;

    return lock;
}

TRAP_DATA * trap_alloc( void )
{
    TRAP_DATA * trap;

    if ( trap_data_free == NULL )
    {
    trap = (TRAP_DATA *) alloc_perm( sizeof( TRAP_DATA ) );
    top_trap_data++;
    }
    else
    {

    trap = trap_data_free;
    trap_data_free = trap_data_free->next;

    }

    trap->spells = NULL;
    trap->flags = 0;

    return trap;
}

TRACK_DATA * track_alloc( void )
{
    TRACK_DATA * track;

    if ( track_data_free == NULL )
    {
    track = (TRACK_DATA *) alloc_perm( sizeof( TRACK_DATA ) );
    top_track_data++;
    }
    else
    {

    track = track_data_free;
    track_data_free = track_data_free->next;
    }

    track->next = NULL;
    track->ch = NULL;
    track->time = 0;
    track->ctime = current_time;
    track->track_penalty = 0;

    return track;
}

TRACK_TYPE * track_type_alloc( void )
{
    TRACK_TYPE * track;

    if ( track_type_free == NULL )
    {
    track = (TRACK_TYPE *) alloc_perm( sizeof( TRACK_TYPE ) );
    top_track_type++;
    }
    else
    {

    track = track_type_free;
    track_type_free = track_type_free->next;
    }

    track->next = NULL;
    track->last = NULL;
    track->room = NULL;
    track->dir  = 0;

    return track;
}

CONT_DATA * cont_alloc( void )
{
    CONT_DATA * cont;
 
    if ( cont_data_free == NULL )
    {
    cont = (CONT_DATA *) alloc_perm( sizeof( CONT_DATA ) );
    top_cont_data++;
    }
    else
    {
    cont = cont_data_free;
    cont_data_free = cont_data_free->next;
    }

    cont->lock = NULL;
    cont->trap = NULL;
    cont->inside = NULL; 
    cont->contents = NULL;
    cont->capacity = 0;
    cont->flags = 0;

    return cont;
}

GATE_DATA * gate_alloc( void )
{
    GATE_DATA * gate;
 
    if ( gate_data_free == NULL )
    {
    gate = (GATE_DATA *) alloc_perm( sizeof( GATE_DATA ) );
    top_gate_data++;
    }
    else
    {
    gate = gate_data_free;
    gate_data_free = gate_data_free->next;
    }

    gate->leads_to = NULL;
    gate->flags = 0;
    gate->vnum = 0;
    gate->ave_timer = -1;

    return gate;
}

WEAR_DATA * wear_alloc( void )
{
    WEAR_DATA * wear;

    if ( wear_data_free == NULL )
    {
    wear = (WEAR_DATA *) alloc_perm( sizeof( WEAR_DATA ) );
    top_wear_data++;
    }
    else
    {

    wear = wear_data_free;
    wear_data_free = wear_data_free->next;

    }
    wear->magic_data = NULL;
    wear->wear_flags = 0;
    wear->size = SIZE_MEDIUM;
    wear->wear_loc = WEAR_NONE;
    wear->weight = 0;
    wear->ac = 0;
    wear->cost = 0;
    wear->weapon_data = NULL;
    wear->affected = NULL;

    return wear;
}
EQUIP_DATA * equip_alloc( void )
{
    EQUIP_DATA * equip;

    if ( equip_data_free == NULL )
    {
    equip = (EQUIP_DATA *) alloc_perm( sizeof( EQUIP_DATA ) );
    top_equip_data++;
    }
    else
    {

    equip = equip_data_free;
    equip_data_free = equip_data_free->next;

    }
    equip->obj_vnum = 0;
    equip->flags = 0;
    equip->chance = 100;
    equip->obj = NULL;

    return equip;
}
INSIDE_DATA * inside_alloc( void )
{
    INSIDE_DATA * inside;

    if ( inside_data_free == NULL )
    {
    inside = (INSIDE_DATA *) alloc_perm( sizeof( INSIDE_DATA ) );
    top_inside_data++;
    }
    else
    {

    inside = inside_data_free;
    inside_data_free = inside_data_free->next;

    }
    inside->obj_vnum = 0;
    inside->mob_vnum = 0;
    inside->flags = 0;
    inside->chance = 100;
    inside->obj = NULL;
    inside->mob = NULL;

    return inside;
}

MAGIC_DATA * magic_alloc( void )
{
    MAGIC_DATA * magic;

    if ( magic_data_free == NULL )
    {
    magic = (MAGIC_DATA *) alloc_perm( sizeof( MAGIC_DATA ) );
    top_magic_data++;
    }
    else
    {

    magic = magic_data_free;
    magic_data_free = magic_data_free->next;

    }
    magic->type = 0;
    magic->spells = NULL;
    magic->charges = 0;
    magic->max_charges = 0;
    magic->flags = 0;

    return magic;
}

EDIBLE_DATA * edible_alloc( void )
{
    EDIBLE_DATA * food;

    if ( edible_data_free == NULL )
    {
    food = (EDIBLE_DATA *) alloc_perm( sizeof( EDIBLE_DATA ) );
    top_edible_data++;
    }
    else
    {

    food = edible_data_free;
    edible_data_free = edible_data_free->next;

    }
    food->flags = 0;
    food->type = 0;
    food->spells = NULL;
    food->amount = 1;
    food->max_amount = 1;

    return food;
}

INSIDE_AREA_DATA * inside_area_alloc( void )
{
    INSIDE_AREA_DATA * inside_area;

    if ( inside_area_data_free == NULL )
    {
    top_inside_area_data++;
    inside_area = (INSIDE_AREA_DATA *) alloc_perm( sizeof( INSIDE_AREA_DATA ) );
    }
    else
    {

    inside_area = inside_area_data_free;
    inside_area_data_free = inside_area_data_free->next;

    }
    inside_area->mob_name[0] = '\0';
    inside_area->short_descr[0] = '\0';
    inside_area->start_room = 0;

    return inside_area;
}

MOVEABLE_DATA * moveable_alloc( void )
{
    MOVEABLE_DATA * moveable;

    if ( moveable_data_free == NULL )
    {
    top_moveable_data++;
    moveable = (MOVEABLE_DATA *) alloc_perm( sizeof( MOVEABLE_DATA ) );
    }
    else
    {

    moveable = moveable_data_free;
    moveable_data_free = moveable_data_free->next;

    }

    moveable->leave_msg[0] = '\0';
    moveable->board_msg[0] = '\0';
    moveable->move_msg[0] = '\0';
    moveable->area_start_room = 0;
    
    return moveable;
}

GLOBAL_RANDOM_DATA * random_data_alloc( void )
{
    GLOBAL_RANDOM_DATA * random_data;

    if ( random_data_free == NULL )
    {
    top_random_data++;
    random_data = (GLOBAL_RANDOM_DATA *) alloc_perm( sizeof( GLOBAL_RANDOM_DATA ) );
    }
    else
    {

    random_data = random_data_free;
    random_data_free = random_data_free->next;
    random_data->count		 = 0;

    }
    random_data->world		 = -1;
    random_data->area		 = 0;
    random_data->global		 = 0;
    random_data->chance		 = 1000;
    random_data->max		 = 100;
    random_data->count		 = 0;
    random_data->vnum		 = 0;

    return random_data;
}


LOGON_DATA * logon_data_alloc( void )
{
    LOGON_DATA * logon_data;

    if ( logon_data_free == NULL )
    {
    top_logon_data++;
    logon_data = (LOGON_DATA *) alloc_perm( sizeof( LOGON_DATA ) );
    }
    else
    {

    logon_data = logon_data_free;
    logon_data_free = logon_data_free->next;

    }
    logon_data->name[0]		 = '\0';
    logon_data->time_start	 = current_time;
    logon_data->time_end	 = current_time;
    logon_data->kills		 = 0;
    logon_data->pkills		 = 0;
    logon_data->exp		 = 0;
    logon_data->rooms		 = 0;
    logon_data->objects		 = 0;
    logon_data->mobiles		 = 0;
    logon_data->level		 = 0;
    logon_data->exit[0]		 = '\0';
    logon_data->last_ip[0]	 = '\0';

    return logon_data;
}


SPELL_LIST * spell_alloc( void )
{
    SPELL_LIST * spell;
    
    if ( spell_list_free == NULL )
    {
    spell = (SPELL_LIST *) alloc_perm( sizeof( SPELL_LIST ) );
    top_spell_data++;
    }
    else
    {

    spell = spell_list_free;
    spell_list_free = spell_list_free->next;

    }
    spell->level = 0;
    spell->spell = 0;

    return spell;
}

WEAPON_DATA * weapon_alloc( void )
{
    WEAPON_DATA * weapon;

    if ( weapon_data_free == NULL )
    {
    weapon = (WEAPON_DATA *) alloc_perm( sizeof( WEAPON_DATA ) );
    top_weapon_data++;
    }
    else
    {

    weapon = weapon_data_free;
    weapon_data_free = weapon_data_free->next;

    }
    weapon->type = 0;
    weapon->dice = 0;
    weapon->die = 0;
    weapon->dam_type = 0;
    weapon->flags = 0;

    return weapon;
}

ROOM_INDEX_DATA * pRoomIndex_alloc( void )
{
ROOM_INDEX_DATA * pRoomIndex;

    if ( room_index_free == NULL )
    {
    pRoomIndex = (ROOM_INDEX_DATA *) alloc_perm( sizeof( ROOM_INDEX_DATA ) );
    top_room++;
    }
    else
    {

    pRoomIndex = room_index_free;
    room_index_free = room_index_free->next;

    }
    pRoomIndex->next = NULL;
    pRoomIndex->next_with_trigger = NULL;
    pRoomIndex->area = NULL;
    pRoomIndex->exit[0] = NULL;
    pRoomIndex->exit[1] = NULL;
    pRoomIndex->exit[2] = NULL;
    pRoomIndex->exit[3] = NULL;
    pRoomIndex->exit[4] = NULL;
    pRoomIndex->exit[5] = NULL;
    pRoomIndex->vnum = 0;
    pRoomIndex->room_flags = 0;
    pRoomIndex->room_flags_2 = 0;
    pRoomIndex->light = 0;
    pRoomIndex->snoop_by = NULL; 
    pRoomIndex->sector_type = 0;
    pRoomIndex->guilds = 0;
    pRoomIndex->name[0] = '\0';
    pRoomIndex->spro = NULL;
    pRoomIndex->affected_by = NULL;
    pRoomIndex->description = NULL;
    pRoomIndex->people = NULL;
    pRoomIndex->contents = NULL;
    pRoomIndex->extra_descr = NULL;
    pRoomIndex->triggers = NULL;
    pRoomIndex->variables = NULL;
    return pRoomIndex;
}


void free_reset_data( RESET_DATA *pReset )
{
    pReset->next            = reset_free;
    reset_free              = pReset;
    return;
}
void free_skill_list( SKILL_LIST *skill )
{
    skill->next            = skill_list_free;
    skill_list_free        = skill;
    return;
}

/* castle code */
void free_castle_data( CASTLE_DATA *castle_data )
{
    castle_data->next = castle_data_free;
    castle_data_free = castle_data;
    return;
}

void free_finger( FINGER_DATA * finger )
{
    finger->next	= finger_data_free;
    finger_data_free	= finger;
    return;
}

void free_wizlist( WIZLIST_DATA *wizlist )
{
    wizlist->next            = wizlist_data_free;
    wizlist_data_free        = wizlist;
    return;
}
void free_bet_data( BET_DATA *bet_data )
{
    bet_data->next            = bet_data_free;
    bet_data_free             = bet_data;
    return;
}
void free_bhost_list( BHOST_LIST *bhost_list )
{
    bhost_list->next		= bhost_list_free;
    bhost_list_free		= bhost_list;
    return;
}
void free_debt_data( DEBT_DATA *debt_data )
{
    debt_data->next          = debt_data_free;
    debt_data_free           = debt_data;
    return;
}

void free_inside_area_data( INSIDE_AREA_DATA *inside_area )
{
    inside_area->next       = inside_area_data_free;
    inside_area_data_free   = inside_area;
    return;
}


HELP_DATA *new_help( void )
{
    HELP_DATA *pHelp;

    if ( help_free == NULL )
    {  
        pHelp   =   (HELP_DATA *) alloc_perm( sizeof(HELP_DATA) );
        top_help++;
    }
    else
    {
        pHelp       =   help_free;
        help_free   =   help_free->next;
    } 

    pHelp->vnum             =   top_help;
    pHelp->keyword[0]       =   '\0';
    pHelp->text             =   &str_empty[0]; 
    pHelp->level	    =   0;
    pHelp->type		    =   0;

    return pHelp;
}

BFS_QUEUE *alloc_bfs_queue( void )
{
    BFS_QUEUE *bfs_queue;

    if ( bfs_queue_free == NULL )
    {  
        bfs_queue   =   (BFS_QUEUE *) alloc_perm( sizeof(BFS_QUEUE) );
        top_bfs_queue++;
    }
    else
    {
        bfs_queue       =   bfs_queue_free;
        bfs_queue_free   =   bfs_queue_free->next;
    } 

    bfs_queue->room     =   NULL;
    bfs_queue->dir      =   0;
    bfs_queue->depth    =   0;

    return bfs_queue;
}
BFS_ROOM *alloc_bfs_room( void )
{
    BFS_ROOM *bfs_room;

    if ( bfs_room_free == NULL )
    {  
        bfs_room   =   (BFS_ROOM *) alloc_perm( sizeof(BFS_ROOM) );
        top_bfs_room++;
    }
    else
    {
        bfs_room       =   bfs_room_free;
        bfs_room_free   =   bfs_room_free->next;
    } 

    bfs_room->room     =   NULL;

    return bfs_room;
}



AREA_DATA *new_area( void )
{
    AREA_DATA *pArea;

    if ( area_free == NULL )
    {
        pArea   =   (AREA_DATA *) alloc_perm( sizeof(AREA_DATA) );
	top_area++;
    }
    else
    {
        pArea       =   area_free;
        area_free   =   area_free->next;
    }

    pArea->castle_data = castle_data_alloc();
    pArea->reset_first  = NULL;
    pArea->reset_last   = NULL;
    pArea->max_age      = 31;
    pArea->max_age_empty= 15;
    pArea->age          = pArea->max_age + 1;
    pArea->nplayer      = 0;
    pArea->wander_time  = 2;
    pArea->weather	= weather_data_alloc();
    pArea->weather->spring = 2;
    pArea->weather->summer = 5;
    pArea->weather->fall   = 8;
    pArea->weather->winter = 11;
    pArea->weather->season = 0;
    pArea->weather->chance[ 0 ][ 0 ] = 0;
    pArea->weather->chance[ 0 ][ 1 ] = 25;
    pArea->weather->chance[ 0 ][ 2 ] = 50;
    pArea->weather->chance[ 0 ][ 3 ] = 75;
    pArea->weather->chance[ 1 ][ 0 ] = 0;
    pArea->weather->chance[ 1 ][ 1 ] = 25;
    pArea->weather->chance[ 1 ][ 2 ] = 50;
    pArea->weather->chance[ 1 ][ 3 ] = 75;
    pArea->weather->chance[ 2 ][ 0 ] = 0;
    pArea->weather->chance[ 2 ][ 1 ] = 25;
    pArea->weather->chance[ 2 ][ 2 ] = 50;
    pArea->weather->chance[ 2 ][ 3 ] = 75;
    pArea->weather->chance[ 3 ][ 0 ] = 0;
    pArea->weather->chance[ 3 ][ 1 ] = 25;
    pArea->weather->chance[ 3 ][ 2 ] = 50;
    pArea->weather->chance[ 3 ][ 3 ] = 75;
    pArea->weather->storm_type[ 0 ] = SKY_LIGHTNING;
    pArea->weather->storm_type[ 1 ] = SKY_LIGHTNING;
    pArea->weather->storm_type[ 2 ] = SKY_LIGHTNING;
    pArea->weather->storm_type[ 3 ] = SKY_LIGHTNING;
    pArea->weather->speed = 6;
    pArea->weather->mmhg = 0;
    pArea->weather->change = 0;
    pArea->weather->sky = SKY_CLOUDLESS;
    pArea->weather->sunlight = SUN_RISE;
    pArea->time_zone	= 0;
    pArea->visible      = 60;
    pArea->security     = 1;
    pArea->lvnum        = 0;
    pArea->uvnum        = 0;
    pArea->vnum		= top_area;
    pArea->areanum          =   top_area;
    pArea->next             =   NULL;
    pArea->name[0]          =   '\0';
    pArea->credits[0]       =   '\0';
    pArea->world            =   0;
    pArea->area_flags       =   AREA_ADDED|AREA_NO_TELEPORT;
    pArea->builders[0]      =   '\0';
    pArea->filename[0]      =   '\0';

    return pArea;
}



void free_area( AREA_DATA *pArea )
{
    RESET_DATA *pReset;

    for ( pReset = pArea->reset_first; pReset != NULL; pReset = pReset->next )
    {
        free_reset_data( pReset );
    }

    pArea->next         =   area_free->next;
    area_free           =   pArea;
    return;
}



EXIT_DATA *new_exit( void )
{
    EXIT_DATA *pExit;

    if ( exit_free == NULL )
    {
        pExit           =   (EXIT_DATA *) alloc_perm( sizeof(*pExit) );
        top_exit++;
    }
    else
    {
        pExit           =   exit_free;
        exit_free       =   exit_free->next;
    }

    pExit->to_room      	=   NULL;
    pExit->u1.vnum      	=   0;
    pExit->next         	=   NULL;
    pExit->exit_info    	=   0;
    pExit->exit_info_default    =   0;
    pExit->tracks      	 	=   NULL;
    pExit->trap         	=   NULL;
    pExit->mes_to_char[0]  	=   '\0';
    pExit->mes_to_room[0]  	=   '\0';
    pExit->mes_at_room[0]  	=   '\0';
    pExit->lock         	=   NULL;
    pExit->keyword[0]      	=   '\0';
    pExit->description[0]  	=   '\0';

    return pExit;
}
void free_exit( EXIT_DATA *pExit )
{
    pExit->next         =   exit_free;
    exit_free           =   pExit;
    return;
}
void free_read_notes( READ_NOTES *pNote )
{
    pNote->next         =   read_notes_free;
    read_notes_free           =   pNote;
    return;
}
void free_bfs_queue( BFS_QUEUE *bfs_queue )
{
    bfs_queue->next         =   bfs_queue_free;
    bfs_queue_free           =   bfs_queue;
    return;
}
void free_bfs_room( BFS_ROOM *bfs_room )
{
    bfs_room->next         =   bfs_room_free;
    bfs_room_free           =   bfs_room;
    return;
}
void free_spec_pro( SPECPRO_LIST *specpro_list )
{
    specpro_list->next = specpro_list_free;
    specpro_list_free = specpro_list;
    return;
}

void free_spell( SPELL_LIST *pSpell )
{
    pSpell->next         =   spell_list_free;
    spell_list_free 	=   pSpell;
    return;
}
void free_magic_data( MAGIC_DATA *pMagic )
{
	pMagic->next	=	magic_data_free;
	magic_data_free	=	pMagic;	
	return;
}
void free_group_data( GROUP_DATA *pGroup )
{
	pGroup->next	=	group_data_free;
	group_data_free	=	pGroup;	
	return;
}
void free_lock_data( LOCK_DATA *pLock )
{
	pLock->next	=	lock_data_free;
	lock_data_free	=	pLock;	
	return;
}
void free_cont_data( CONT_DATA *pCont )
{
	pCont->next	=	cont_data_free;
	cont_data_free	=	pCont;	
	return;
}

void free_wear_data( WEAR_DATA *pWear )
{
	pWear->next		=	wear_data_free;
	wear_data_free		=	pWear;	
	return;
}
void free_equip_data( EQUIP_DATA *equip )
{
	equip->next		=	equip_data_free;
	equip_data_free		=	equip;	
	return;
}
void free_inside_data( INSIDE_DATA *inside )
{
	inside->next		=	inside_data_free;
	inside_data_free	=	inside;	
	return;
}
void free_edible_data( EDIBLE_DATA *pEdible )
{
	pEdible->next		=	edible_data_free;
	edible_data_free	=	pEdible;	
	return;
}
void free_trap_data( TRAP_DATA *pTrap )
{
	pTrap->next		=	trap_data_free;
	trap_data_free		=	pTrap;	
	return;
}
void free_gate_data( GATE_DATA *pGate )
{
	pGate->next		=	gate_data_free;
	gate_data_free		=	pGate;	
	return;
}
void free_event_data( EVENT_DATA *event )
{
	event->next		=	event_data_free;
	event_data_free		=	event;	
	return;
}
void free_quest_data( QUEST_DATA *quest )
{
	quest->next		=	quest_data_free;
	quest_data_free		=	quest;	
	return;
}
void free_char_quests( CHAR_QUESTS *quest )
{
	quest->next		=	char_quests_free;
	char_quests_free	=	quest;	
	return;
}
void free_weapon_data( WEAPON_DATA *pWeapon )
{
	pWeapon->next		=	weapon_data_free;
	weapon_data_free	=	pWeapon;	
	return;
}
void free_light_data( LIGHT_DATA *pLight )
{
	pLight->next		=	light_data_free;
	light_data_free		=	pLight;	
	return;
}
void free_approve_data( APPROVE_DATA *approve_data )
{
	approve_data->next	=	approve_data_free;
	approve_data_free	=	approve_data;	
	return;
}
void free_track_type( TRACK_TYPE *track )
{
    track->next         =   track_type_free;
    track_type_free 	=   track;
    return;
}
void free_pcclan_data( PC_CLAN_DATA *pcclan )
{
    pcclan->next	=   pcclan_data_free;
    pcclan_data_free    =   pcclan;
    return;
}

EXTRA_DESCR_DATA *new_extra_descr( void )
{
    EXTRA_DESCR_DATA *pExtra;

    if ( extra_descr_free == NULL )
    {
        pExtra              =   (EXTRA_DESCR_DATA *) alloc_perm( sizeof(*pExtra) );
        top_ed++;
    }
    else
    {
        pExtra              =   extra_descr_free;
        extra_descr_free    =   extra_descr_free->next;
    }

    pExtra->keyword[0]      =   '\0';
    pExtra->description     =   &str_empty[0];
    pExtra->next            =   NULL;

    return pExtra;
}



void free_extra_descr( EXTRA_DESCR_DATA *pExtra )
{

    pExtra->next        =   extra_descr_free;
    extra_descr_free    =   pExtra;
    return;
}



/*
void free_room_index( ROOM_INDEX_DATA *pRoom )
{
    int door;
    EXTRA_DESCR_DATA *pExtra;


    for ( door = 0; door < MAX_DIR; door++ )
    {
        if ( pRoom->exit[door] != NULL )
            free_exit( pRoom->exit[door] );
    }

    for ( pExtra = pRoom->extra_descr; pExtra != NULL; pExtra = pExtra->next )
    {
        free_extra_descr( pExtra );
    }

    pRoom->next     =   room_index_free;
    room_index_free =   pRoom;
    return;
}

*/

AFFECT_DATA *new_affect( bool MoBile )
{
    AFFECT_DATA *pAf;

    if ( affect_free == NULL )
    {
        pAf             =   (AFFECT_DATA *) alloc_perm( sizeof(*pAf) );
	if (MoBile)
        top_affect_mob++;
	else
        top_affect_obj++;
    }
    else
    {
        pAf             =   affect_free;
        affect_free     =   affect_free->next;
    }

    pAf->next       =   NULL;
    pAf->location   =   0;
    pAf->location2  =   0;
    pAf->modifier   =   0;
    pAf->type       =   0;
    pAf->duration   =   0;
    pAf->bitvector  =   0;
    pAf->bit_type   =   0;
    pAf->level	    =   0;

    return pAf;
}



void free_affect( AFFECT_DATA* pAf )
{
    pAf->next           = affect_free;
    affect_free         = pAf;
    return;
}



SHOP_DATA *new_shop( void )
{
    SHOP_DATA *pShop;

    if ( shop_free == NULL )
    {
        pShop           =   (SHOP_DATA *) alloc_perm( sizeof(*pShop) );
        top_shop++;
    }
    else
    {
        pShop           =   shop_free;
        shop_free       =   shop_free->next;
    }

    pShop->next         =   NULL;
    pShop->keeper       =   0;
    pShop->type		=   0;
    pShop->profit_buy   =   100;
    pShop->profit_sell  =   100;
    pShop->open_hour    =   0;
    pShop->close_hour   =   23;

    return pShop;
}



void free_shop( SHOP_DATA *pShop )
{
    pShop->next = shop_free;
    shop_free   = pShop;
    return;
}



OBJ_INDEX_DATA *new_obj_index( void )
{
    OBJ_INDEX_DATA *pObj2;

    if ( obj_index_free == NULL )
    {
        pObj2           =   (OBJ_INDEX_DATA *) alloc_perm( sizeof(*pObj2) );
        top_obj_index++;
    }
    else
    {
        pObj2            =   obj_index_free;
        obj_index_free  =   obj_index_free->next;
    }

    pObj2->next          =   NULL;
    pObj2->inside        =   NULL;
    pObj2->extra_descr   =   NULL;
    pObj2->in_area       =   NULL;
    pObj2->trigger_list  =   NULL;
    pObj2->triggers 	 =   NULL;
    pObj2->variables 	 =   NULL;
    pObj2->name[0]          =   '\0';
    pObj2->short_descr[0]   =   '\0';
    pObj2->description[0]   =   '\0';
    pObj2->vnum          =   0;
    pObj2->extra_flags   =   0;
    pObj2->special_flags =   0;
    pObj2->wear_data     =   NULL;
    pObj2->count         =   0;
    pObj2->condition     =   100;
    pObj2->approve_data = approve_data_alloc();
    return pObj2;
}



void free_obj_index_data( OBJ_INDEX_DATA *pObj2 )
{
    EXTRA_DESCR_DATA *pExtra;
    AFFECT_DATA *pAf;

    if ( pObj2->wear_data != NULL )
    {
        for ( pAf = pObj2->wear_data->affected; pAf != NULL; pAf = pAf->next )
        {
            free_affect( pAf );
        } 
	
	  free_wear_data( pObj2->wear_data );
    }

    for ( pExtra = pObj2->extra_descr; pExtra != NULL; pExtra = pExtra->next )
    {
        free_extra_descr( pExtra );
    }
    
    pObj2->next              = obj_index_free;
    obj_index_free          = pObj2;
    return;
}



MOB_INDEX_DATA *alloc_pMobIndex( void )
{
    MOB_INDEX_DATA *pMob2;
    int i;

    if ( mob_index_free == NULL )
    {
        pMob2           =   (MOB_INDEX_DATA *) alloc_perm( sizeof( MOB_INDEX_DATA ) );
        top_mob_index++;
    }
    else
    {
        pMob2            =   mob_index_free;
        mob_index_free  =   mob_index_free->next;
    }

    pMob2->next        		  =   NULL;
    pMob2->equip		= NULL;
    pMob2->spec_fun    		  =   NULL;
    pMob2->spec_name[0]  	  =   '\0';
    pMob2->pShop       		  =   NULL;
    pMob2->gainer_data  	  =   NULL;
    pMob2->load_with_area 	 =   NULL;
    pMob2->trigger_list		  =   NULL;
    pMob2->extra_descr		  =   NULL;
    pMob2->triggers 		 =   NULL;
    pMob2->variables 		 =   NULL;
    pMob2->moveable 		 =   NULL;
    pMob2->script_fn 		 =   NULL;
    /* castle code */
    pMob2->castle_area		 =   0;
    pMob2->movement_msg[0]	 =   '\0';
    pMob2->player_name[0]  	 =   '\0';
    pMob2->short_descr[0]  	 =   '\0';
    pMob2->long_descr[0]   	 =   '\0';
    pMob2->description  	 =   &str_empty[0];
    pMob2->skills		 =   NULL;
    pMob2->innate_skills	 =   NULL;
    pMob2->prac_skills		 =   0;
    pMob2->prac_spells		 =   0;
    pMob2->vnum         	 =   0;
    pMob2->follower         	 =   0;
    pMob2->race         	 =   4;
    pMob2->count        	 =   0;
    pMob2->killed       	 =   0;
    pMob2->sex          	 =   1;
    pMob2->level        	 =   0;
    pMob2->off_flags        	 =   0;
    pMob2->imm_flags        	 =   0;
    pMob2->res_flags        	 =   0;
    pMob2->vuln_flags        	 =   0;
    pMob2->default_pos        	 =   8;
    pMob2->start_pos        	 =   8;
    pMob2->mood         	 =   4;
    pMob2->form         	 =   0;
    pMob2->material         	 =   0;
    pMob2->size         	 =   2;
    pMob2->weight		 =   0;
    pMob2->height		 =   0;
    pMob2->act          	 =   1;	/* NPC */
    pMob2->dam_type          	 =   0;	/* NPC */
    pMob2->affected_by  	 =   0;
    pMob2->affected_by_2  	 =   0;
    pMob2->hitroll		=   0;
    pMob2->guilds		=   0;
    pMob2->hit[0]		=   0;
    pMob2->hit[1]		=   0;
    pMob2->hit[2]		=   0;
    pMob2->hit[3]		=   0;
    for ( i = 0 ; i < SECT_MAX ; i++ )	
	pMob2->valid_sector[ i ] = TRUE;
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        pMob2->mana[0][i]	=   0;
        pMob2->mana[1][i]	=   0;
        pMob2->mana[2][i]	=   0;
        pMob2->mana[3][i]	=   0;
    }
    pMob2->damage[0]		=   0;
    pMob2->damage[1]		=   0;
    pMob2->damage[2]		=   0;
    pMob2->damage[3]		=   0;
    pMob2->armor		=   0;
    pMob2->act2	        	=   0;
    pMob2->gold[0]      	    =   0;
    pMob2->gold[1]     		    =   0;
    pMob2->gold[2]      	    =   0;
    pMob2->gold[3]      	    =   0;

    return pMob2;
}



void free_mob_index_data( MOB_INDEX_DATA *pMob2 )
{

    free_shop( pMob2->pShop );

    pMob2->next              = mob_index_free;
    mob_index_free          = pMob2;
    return;
}

/*
 * Frees a variable for scripts.
 * This is used most often.
 */
void free_variable( VARIABLE_DATA *var )
{
    if ( var == NULL ) return;

    var->next        = variable_free;
    variable_free    = var;
    return;
}

/*
 * Frees a trigger, used when a mob dies.
 */
void free_trigger( TRIGGER_DATA *trig )
{
    if ( trig == NULL ) return;

    trig->current = NULL;

    trig->next       = trigger_free;
    trigger_free     = trig;
    /* unlink it from the main list */
    if ( trig == trigger_list )
    {
       trigger_list = trig->next_in_list;
    }
    else
    {
	TRIGGER_DATA *prev;

	for ( prev = trigger_list; prev != NULL; prev = prev->next_in_list )
	{
	    if ( prev->next_in_list == trig )
	    {
		prev->next_in_list = trig->next_in_list;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Free trigger : trigger not found.", 0 );
	    return;
	}
    }
    return;
}
/*
 * Frees a trigger list, used for removing triggers in medit 
 */
void free_trigger_list( TRIGGER_LIST_DATA *trig )
{
    if ( trig == NULL ) return;

    trig->next       = trigger_list_free;
    trigger_list_free     = trig;
    return;
}

/*
 * Hardly ever used ( will be when you can use OLC to delete indexes )
 */
void free_script( SCRIPT_DATA *scr )
{
    if ( scr == NULL ) return;

    scr->next       = script_free;
    script_free    = scr;
    return;
}

/*
 * Extract an obj from the world.
 */
void extract_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_content;
    OBJ_DATA *obj_next;
    DESCRIPTOR_DATA *d;
    GLOBAL_RANDOM_DATA *grandom;
    
   object_count--; 
   if ( obj == NULL )
	{
	    bug( "Extract_obj: obj not found.", 0);
	    return;
	} 
   for ( grandom = random_data_list; grandom != NULL; grandom = grandom->next )
   {
   	if ( grandom->vnum == obj->pIndexData->vnum )
   	    grandom->count--;

   	if ( grandom->count < 0 )
   	    grandom->count = 0;
   }

   if ( obj->in_room != NULL )
	obj_from_room( obj );
    else if ( obj->carried_by != NULL )
	obj_from_char( obj, "S" );
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );
	
   obj->event = NULL;
   if ( obj->cont_data != NULL )
   {   
	for ( d = descriptor_list ; d != NULL ; d = d->next )
	{
	    if ( d->character != NULL && d->character->pcdata != NULL && d->character->pcdata->corpse == obj )  
	    {
		d->character->pcdata->corpse = NULL;
		break;
	    }
	}

        for ( obj_content = obj->cont_data->contents; obj_content; obj_content = obj_next )
        {
	    obj_next = obj_content->next_content;
            extract_obj( obj_content );
        }
    }

    if ( object_list == obj )
    {
	object_list = obj->next;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = object_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == obj )
	    {
		prev->next = obj->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_obj: obj %d not found.", obj->pIndexData->vnum );
	    return;
	}
    }

    {
	AFFECT_DATA *paf;
	AFFECT_DATA *paf_next;

	if ( obj->wear_data != NULL )
	    for ( paf = obj->wear_data->affected; paf != NULL; paf = paf_next )
	    {
	        paf_next    = paf->next;
	        paf->next   = affect_free;
	        affect_free = paf;
	    }
    }

    {
	EXTRA_DESCR_DATA *ed;
	EXTRA_DESCR_DATA *ed_next;

	for ( ed = obj->extra_descr; ed != NULL; ed = ed_next )
	{
	    ed_next		= ed->next;
	    extra_descr_free	= ed;
	}
   }

   if ( obj->light_data != NULL )
   {
	LIGHT_DATA * light_data;
        light_data = obj->light_data;
	light_data->next	=	light_data_free;
	light_data_free		=	light_data;	
   }
   if ( obj->edible_data != NULL )
   {
	EDIBLE_DATA * edible_data;
   	if ( obj->edible_data->spells != NULL )
   	{
		SPELL_LIST * spell_data;
   	     	spell_data = obj->edible_data->spells;
		spell_data->next	=	spell_list_free;
		spell_list_free		=	spell_data;	
   	}
        edible_data = obj->edible_data;
	edible_data->next	=	edible_data_free;
	edible_data_free	=	edible_data;	
   }
   if ( obj->gate_data != NULL )
   {
	GATE_DATA * gate_data;
        gate_data = obj->gate_data;
	gate_data->next		=	gate_data_free;
	gate_data_free		=	gate_data;	
   }
   if ( obj->cont_data != NULL )
   {
	CONT_DATA * cont_data;
   	if ( obj->cont_data->inside != NULL )
   	{
		RESET_DATA * reset_data;
   	     	reset_data = obj->cont_data->inside;
		reset_data->next	=	reset_data_free;
		reset_data_free		=	reset_data;	
   	}
   	if ( obj->cont_data->trap != NULL )
   	{
		TRAP_DATA * trap_data;
   		if ( obj->cont_data->trap->spells != NULL )
   		{
			SPELL_LIST * spell_data;
   		     	spell_data = obj->cont_data->trap->spells;
			spell_data->next	=	spell_list_free;
			spell_list_free		=	spell_data;	
   		}
   	     	trap_data = obj->cont_data->trap;
		trap_data->next		=	trap_data_free;
		trap_data_free		=	trap_data;	
   	}
   	if ( obj->cont_data->lock != NULL )
   	{
		LOCK_DATA * lock_data;
   		if ( obj->cont_data->lock->trap != NULL )
   		{
			TRAP_DATA * trap_data;
   			if ( obj->cont_data->lock->trap->spells != NULL )
   			{
				SPELL_LIST * spell_data;
   			     	spell_data = obj->cont_data->lock->trap->spells;
				spell_data->next	=	spell_list_free;
				spell_list_free		=	spell_data;	
   			}
   		     	trap_data = obj->cont_data->lock->trap;
			trap_data->next		=	trap_data_free;
			trap_data_free		=	trap_data;	
   		}
   	     	lock_data = obj->cont_data->lock;
		lock_data->next		=	lock_data_free;
		lock_data_free		=	lock_data;	
   	}
        cont_data = obj->cont_data;
	cont_data->next		=	cont_data_free;
	cont_data_free		=	cont_data;	
   }
   if ( obj->wear_data != NULL )
   {
	WEAR_DATA * wear_data;

   	if ( obj->wear_data->weapon_data != NULL )
   	{
		WEAPON_DATA * weapon_data;
   	     	weapon_data = obj->wear_data->weapon_data;
		weapon_data->next	=	weapon_data_free;
		weapon_data_free	=	weapon_data;	
   	}
   	if ( obj->wear_data->magic_data != NULL )
   	{
		MAGIC_DATA * magic_data;
   		if ( obj->wear_data->magic_data->spells != NULL )
   		{
			SPELL_LIST * spell_data;
   		     	spell_data = obj->wear_data->magic_data->spells;
			spell_data->next	=	spell_list_free;
			spell_list_free		=	spell_data;	
   		}
   	     	magic_data = obj->wear_data->magic_data;
		magic_data->next	=	magic_data_free;
		magic_data_free		=	magic_data;	
   	}

        wear_data = obj->wear_data;
	wear_data->next		=	wear_data_free;
	wear_data_free		=	wear_data;	
   }

    obj->owner[0] = '\0'; 

   /* the following will not be freed unless the object
      is a special ITEM_SPEC_STRING. */
    --obj->pIndexData->count;
    obj->next	= obj_free;
    obj_free	= obj;
    return;
}



/*
 * Extract a char from the world.
 */
void extract_char( CHAR_DATA *ch, bool fPull )
{
    CHAR_DATA *wch;
    CHAR_DATA *wch_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    
    if ( ch->in_room == NULL )
    {
	bug( "Extract_char: NULL.", 0 );
	return;
    }
    if (LOGON_ON)
    if (ch->logon_data != NULL )
    if (!str_cmp(ch->logon_data->exit, "Logged-IN") )
	strncpy( ch->logon_data->exit, "Unknown", 30 );

    nuke_pets(ch);
    if ( IS_NPC( ch ) && ch->leader != NULL && IS_SET( ch->act, ACT_PET ) )
	ch->leader->pet = NULL;

    ch->mount = NULL;
    ch->pet = NULL; /* just in case */
    set_position( ch, POS_DEAD );
    if (IS_NPC(ch) )
    {
    if ( ch == mob_list_update[ch->mob_update_number] )
    {
       mob_list_update[ch->mob_update_number] = ch->mob_next_update;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = mob_list_update[ch->mob_update_number]; prev != NULL; prev = prev->mob_next_update )
	{
	    if ( prev->mob_next_update == ch )
	    {
		prev->mob_next_update = ch->mob_next_update;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_char: char not found.", 0 );
	    return;
	}
    }
    }
    if (!IS_NPC(ch))
    {
        if ( ch->pcdata->pcclan != NULL )
            free_pcclan_data(ch->pcdata->pcclan);
    }
    /*
     * Clear the Event
     */
    ch->event = NULL;

     
    stop_fighting( ch, TRUE );
    
    if ( ch->group )
    {
    	if ( ch->group->leader == ch )
    	{
    		do_disband( ch, "" );
    	}
    	else
    	{
    		remove_from_group( ch );
    	}
   }
   
   if ( ch->master )
   	stop_follower( ch );
   	
   if ( ch->follower )
   	die_to_followers( ch );

    if (ch->inside_area != NULL )
	free_inside_area_data(ch->inside_area);
	ch->inside_area = NULL;
    char_from_room( ch );
    if ( !fPull )
    {
	if ( ch->pcdata->pcclan == NULL || get_room_index( ch->pcdata->pcclan->clan->recall_room ) == NULL )
	    char_to_room( ch, get_room_index( race_table[ ch->race ][ ch->sex ].die ) );
	else
	    char_to_room( ch, get_room_index( ch->pcdata->pcclan->clan->recall_room ) );
	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
        if (obj == obj->next_content) { log_string("Holy Sheep Shit batman!!!"); exit(1); }	
	extract_obj( obj );
    }

    if ( IS_NPC(ch) )
	--ch->pIndexData->count;
    else if ( ch->pcdata->corpse != NULL )
	extract_obj( ch->pcdata->corpse );

    if ( ch->desc != NULL && ch->desc->original != NULL )
	do_return( ch, "" );

    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
	wch_next = wch->next;

	if ( wch->reply == ch )
	    wch->reply = NULL;
	if ( wch->mount == ch )
	    wch->mount = NULL;
	if ( is_name(wch->dislikes, ch->name) )
	{
	    wch->dislikes[0] = '\0'; 
	}
	if ( is_name(wch->hunting, ch->name) )
	{
	    wch->hunting[0] = '\0'; 
	    if ( wch->pIndexData != NULL 
	      && ( wch->spec_fun == spec_thug || wch->spec_fun ==spec_nightmare ) )
		extract_char( wch, TRUE );
	}
    }

    if ( ch == char_list )
    {
       char_list = ch->next;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = char_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == ch )
	    {
		prev->next = ch->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Extract_char: char not found.", 0 );
	    return;
	}
    }

    if ( ch->desc != NULL)
    {
	FINGER_DATA * finger;

	for ( finger = finger_list ; finger != NULL ; finger = finger->next )
	    if ( !str_cmp( finger->name, ch->name ) && !IS_SET( ch->plr, PLR_WIZINVIS ) )
		finger->laston = current_time;


	ch->desc->character = NULL;
    }

    free_char( ch );
    return;
}

void	do_disband	(CHAR_DATA *ch, char *argument );
DECLARE_DO_FUN(do_return	);

/* buffer sizes */
const long buf_size[MAX_BUF_LIST] =
{
    16,32,64,128,256,1024,2048,4096,8192,16384,32768,65536,131072,262144-64
};

/* local procedure for finding the next acceptable size */
/* -1 indicates out-of-boundary error */
long get_size (long val)
{
    long i;

    for (i = 0; i < MAX_BUF_LIST; i++)
	if (buf_size[i] >= val)
	{
	    return buf_size[i];
	}
    
    return -1;
}

BUFFER *new_buf()
{
    BUFFER *buffer;

    if (buf_free == NULL) 
    {
	buffer = (BUFFER *) alloc_perm(sizeof(BUFFER));
	top_buffer++;
    }
    else
    {
	buffer = buf_free;
	buf_free = buf_free->next;
    }

    buffer->next	= NULL;
    buffer->state	= BUFFER_SAFE;
    buffer->size	= get_size(BASE_BUF);

    buffer->string	= (char *) alloc_mem(buffer->size);
    buffer->string[0]	= '\0';
    VALIDATE(buffer);

    return buffer;
}

BUFFER *new_buf_size(long size)
{
    BUFFER *buffer;
 
    if (buf_free == NULL)
    {
        buffer = (BUFFER *) alloc_perm(sizeof(BUFFER));
	top_buffer++;
    }
    else
    {
        buffer = buf_free;
        buf_free = buf_free->next;
    }
 
    buffer->next        = NULL;
    buffer->state       = BUFFER_SAFE;
    buffer->size        = get_size(size);
    if (buffer->size == -1)
    {
        bug("new_buf: buffer size %d too large.",size);
        exit(1);
    }
    buffer->string      = (char *) alloc_mem(buffer->size);
    buffer->string[0]   = '\0';
    VALIDATE(buffer);
 
    return buffer;
}


void free_buf(BUFFER *buffer)
{
    if (!IS_VALID(buffer))
	return;

    free_mem(buffer->string,buffer->size);
    buffer->string = NULL;
    buffer->size   = 0;
    buffer->state  = BUFFER_FREED;
    INVALIDATE(buffer);

    buffer->next  = buf_free;
    buf_free      = buffer;
}


bool add_buf(BUFFER *buffer, char *string)
{
    long len;
    char *oldstr;
    long oldsize;

    oldstr = buffer->string;
    oldsize = buffer->size;

    if (buffer->state == BUFFER_OVERFLOW) /* don't waste time on bad strings! */
	return FALSE;

    len = strlen(buffer->string) + strlen(string) + 1;

    while (len >= buffer->size) /* increase the buffer size */
    {
	buffer->size 	= get_size(buffer->size + 1);
	{
	    if (buffer->size == -1) /* overflow */
	    {
		buffer->size = oldsize;
		buffer->state = BUFFER_OVERFLOW;
		bug("buffer overflow past size %d",buffer->size);
		return FALSE;
	    }
  	}
    }

    if (buffer->size != oldsize)
    {
	buffer->string	= (char *) alloc_mem(buffer->size);

	strcpy(buffer->string,oldstr);
	free_mem(oldstr,oldsize);
    }

    strcat(buffer->string,string);
    return TRUE;
}


void clear_buf(BUFFER *buffer)
{
    buffer->string[0] = '\0';
    buffer->state     = BUFFER_SAFE;
}


char *buf_string(BUFFER *buffer)
{
    return buffer->string;
}

