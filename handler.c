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

extern  bool	fBootHomes;
void set_quest( CHAR_DATA * ch, int number, int points, long flags )
{
    CHAR_QUESTS * char_quests;
    bool found = FALSE;

    if ( !ch )
    bug("No character on set_quest",0);

    for ( char_quests = ch->char_quests ; char_quests != NULL ; char_quests = char_quests->next )
    {
	if ( char_quests->quest_number == number )
	{
	found = TRUE;
	break;
	}
    }
    if ( !found )
    {
	char_quests = char_quests_data_alloc();
	char_quests->quest_number = number;
	char_quests->next = ch->char_quests;
	ch->char_quests = char_quests;
    }

    ch->quest_points += points;

    if ( flags != 0 )
	SET_BIT(char_quests->flags, flags );
    return;
}

void check_armor_damage( CHAR_DATA *victim )
{
OBJ_DATA *obj;
int chance_obj;

    for ( chance_obj = 0 ; get_eq_char( victim, chance_obj ) == NULL && chance_obj < MAX_WEAR; chance_obj++ );

    if ( get_eq_char( victim, chance_obj ) == NULL )
	return;

    while( ( obj = get_eq_char( victim, roll_dice( 1, MAX_WEAR ) -1 ) ) == NULL );

    if ( number_range( 1, 19 ) < 2 && !IS_SET( obj->extra_flags, ITEM_MAGIC ) )
    {
	obj->condition = UMAX( 0, obj->condition - 1 );
        act( "`C$p takes a little damage.`w", victim, obj, NULL, TO_CHAR );
    }
    else if ( number_range( 1, 125 ) < 2 && IS_SET( obj->extra_flags, ITEM_MAGIC ) )
    {
        obj->condition = UMAX( 0, obj->condition - 1 );
        act( "`C$p takes a little damage.`w", victim, obj, NULL, TO_CHAR );
    }
}

void equip_mob(CHAR_DATA *mob, long flags)
{
    EQUIP_DATA * equip;
    OBJ_DATA * obj;
    OBJ_INDEX_DATA * pObj;
    if (!mob)
    {
	bug( "No mob equip_mob!", 0 );
	return;
    }
        /*
	 * Remember the equip_data is on the Index of the mob
	 */
    for ( equip = mob->pIndexData->equip ; equip != NULL; equip = equip->next )
    {
	if ( equip->chance < number_percent() )
	    continue;
	if ( !IS_SET( equip->flags, flags ) )
	    continue;
	if ( IS_SET( equip->flags, EQUIP_INVALID ) )
	    continue;
/*
	if ( equip->obj && equip->obj->equip == equip )
	    continue;
*/
	/* 
	 * The mob may already be equiped by something
	 * This is ok, as there may be chances for multiples of the
	 * same type.  
	 */
	if ( ( pObj = get_obj_index( equip->obj_vnum ) ) == NULL || ( obj = create_object( pObj ) ) == NULL )
	{
	    bug( "Bad obj vnum for equip!", equip->obj_vnum );
	    SET_BIT(equip->flags, EQUIP_INVALID);
	    return;
	}
	if ( obj->wear_data == NULL )
	{
	    bug( "No wear_data for obj to equip!", equip->obj_vnum );
	    SET_BIT(equip->flags, EQUIP_INVALID);
	    return;
	}

	obj_to_char(obj,mob);

	if ( !IS_SET( equip->flags, EQUIP_INVENTORY ) )
	{
	    if ( IS_SET( equip->flags, EQUIP_REPLACE ) ) 
		wear_obj(mob, obj, TRUE);
	    else
		wear_obj(mob, obj, FALSE);
	}
	equip->obj = obj;
	equip->obj->equip = equip;
    /*
     * Obj may have something to load inside 
     */
        if ( obj->pIndexData->inside )
        {
	    if ( IS_SET(flags, EQUIP_ON_BOOT ) )
	    {
		load_inside_obj(obj, EQUIP_ON_BOOT );
	    }
	    if ( IS_SET(flags, EQUIP_ON_CREATION ) )
	    {
		load_inside_obj(obj, EQUIP_ON_CREATION );
	    }
        }

    }
}
void load_inside_obj(OBJ_DATA *obj, long flags)
{
    INSIDE_DATA * inside;
    OBJ_DATA * object;
    OBJ_INDEX_DATA * pObj;
    if (!obj)
    {
	bug( "No obj inside!", 0 );
	return;
    }
        /*
	 * Remember the inside_data is on the Index of the obj
	 */
    for ( inside = obj->pIndexData->inside ; inside != NULL; inside = inside->next )
    {
	if ( inside->chance < number_percent() )
	    continue;
	if ( !IS_SET( inside->flags, flags ) )
	    continue;
	if ( IS_SET( inside->flags, EQUIP_INVALID ) )
	    continue;
/*
	if ( inside->obj && inside->obj->inside == inside )
	    continue;
*/
	/* 
	 * The obj may already be inside by something
	 * This is ok, as there may be chances for multiples of the
	 * same type.  
	 */
	if ( ( pObj = get_obj_index( inside->obj_vnum ) ) == NULL || ( object = create_object( pObj ) ) == NULL )
	{
	    bug( "Bad obj vnum for inside!", inside->obj_vnum );
	    SET_BIT(inside->flags, EQUIP_INVALID);
	    return;
	}
	if ( obj->cont_data == NULL )
	{
	    bug( "No container_data for obj to inside!", inside->obj_vnum );
	    SET_BIT(inside->flags, EQUIP_INVALID);
	    return;
	}

	obj_to_obj(object,obj);

	inside->obj = object;
	inside->obj->inside = inside;

    }
}
/* returns material number */
int material_lookup (const char *name)
{
    return 0;
}

/* returns race number */
int race_lookup (const char *name)
{
   int race;

   for ( race = 0; race < top_race ; race++ )
   {
	if (LOWER(name[0]) == LOWER(race_list[race].name[0])
	&&  !str_prefix( name,race_list[race].name))
	    return race;
   }

   return -1;
} 

/* returns clan number */
CLAN_DATA * clan_lookup ( char *name )
{
   CLAN_DATA * clan;

   for ( clan = clan_list; clan != NULL; clan = clan->next)
   {
	if ( is_name( name, clan->name ) )
	    return clan;
   }

   return NULL;
} 
#define MAX_THUG_NAME 3

char * const thug_name[ MAX_THUG_NAME ] =
{
	"Guido",
	"Luigi",
	"Antonio"
};

/* Created by Quin :) */
void next_rtrack( CHAR_DATA * ch, ROOM_INDEX_DATA * pRoom, int spaces , TRACK_TYPE *tracks )
{
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *in_room;
    TRACK_TYPE *tracking;
    TRACK_TYPE *back_track;
    int direction;

    tracks->next=NULL;
    in_room = tracks->room;

    if ( spaces < 1 )
    {
    if ( in_room == pRoom )
        {
        tracks->dir=-1;
        tracks->next=NULL;
        return;
        }

        tracks->dir=6;
        tracks->next=NULL;
        return;
    }

    /* Check each direction for the victim */
    tracks->next =  ( tracking = track_type_alloc() );
    tracking->last = tracks;
    tracking->next = NULL;
    tracks->dir = ( tracking->dir = 6 );

    for ( direction=0 ; direction < 6 ; direction++ )
    {
        if ( in_room->exit[ direction ] != NULL
           && ( to_room = in_room->exit[ direction ]->u1.to_room ) !=NULL
	   && to_room != in_room
	   && can_see_room( ch, to_room ) )
        {
      tracking->dir = 6;
      tracks->dir = direction;
      tracking->room = to_room;
            for( back_track=tracks->last ; ( ( back_track != NULL ) && back_track->room != to_room );
                back_track=back_track->last ) ;  
	    if ( back_track == NULL || back_track->room!=to_room )
                next_rtrack( ch, pRoom, spaces - 1 , tracking );
     	    if ( tracking->dir != 6 )
	    {
		free_track_type( tracking );
                return;
	    }
        }
    }

    /* didn't find prey */
    free_track_type( tracking );
    tracks->dir = 6;
    tracks->next = NULL;
    return;
}

int direction_room( CHAR_DATA * ch, ROOM_INDEX_DATA * pRoom )
{
    TRACK_TYPE *tracks;
    int spaces, dir;

    if ( pRoom == NULL )
	return -1;

    tracks = track_type_alloc();
    tracks->dir = 6;
    for ( spaces = 0 ; spaces < 40 && tracks->dir == 6 ; spaces ++ )
    {
        tracks->next = NULL;
        tracks->last = NULL;
        tracks->room = ch->in_room;
        tracks->dir = 6;
	next_rtrack( ch, pRoom, spaces, tracks );
    }

    dir = tracks->dir;

    if ( dir == -1 )
	dir = -2;
    if ( dir == 6 )
	dir = -1;

    free_track_type( tracks );
    return dir;
}	
/*  Not used???  Galadrail
void do_direction( CHAR_DATA *ch, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * pRoom;
    int vnum, dir;

    one_argument( argument, arg );

    vnum = atoi( arg );

    pRoom = get_room_index( vnum );

    sprintf( arg, "Direction - %d\n\r",( dir = direction_room( ch, pRoom )) );
    send_to_char( arg, ch );
    if ( dir >= 0 && dir < 6 )
     {
	sprintf(arg, "%s\n\r", dir_name[ dir ] );
	send_to_char( arg, ch );
    }

    return;
}
*/

void create_nightmare_mob( CHAR_DATA *ch, int nlevel )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *to_room;
    int level, count = 0;
    int type, i, value, max_number;
    long number;

    if ( ( mob = create_mobile( get_mob_index( MOB_VNUM_NIGHTMARE ) ) ) == NULL )
    {
	bug( "Not valid vnum for creature of nighmare.", 0 );
	return;
    }

    max_number = ch->in_room->area->uvnum - ch->in_room->area->lvnum;

    if ( max_number <= 0 )
    {
	bug( "Create_nightmare, could not get the max_number", 0 );
	return;
    }
    
    for (;;)
    {
        number = number_range(ch->in_room->area->lvnum, ch->in_room->area->uvnum );
        if ( ( to_room = get_room_index(number) ) != NULL )
	{
	    count++;
	    if ( count > max_number )
	    {
		bug( "Create_nightmare, exceeded max_number", 0 );
		return;
	    }

 	    char_to_room(mob,to_room);	   
	    break;
	}
    }

    if ( nlevel < 1 )
        level = UMAX( 1, ch->level - 8 + number_range( 1 , 8 ) );
    else
	level = nlevel;

    value = level * level;
    value = value + ( -1 * value ) % 10;
    sprintf( buf, "`wA creature of `Kdarkness`w is here stalking %s.\n\r", ch->name );
    strncpy( mob->long_descr , buf, 80 );
    mob->armor = 100 - number_range( value * 2 / 25, value * 6 / 25 );
    mob->hitroll = level * 2 / 3;
    mob->damroll = level / 3;
    mob->damage[ 0 ] = UMAX( 1, value / 33 );
    mob->damage[ 1 ] = 3;
    mob->damage[ 2 ] = 0;
    mob->hit = value + (value /2);
    mob->max_hit = value + (value / 2);
    mob->move = value;
    mob->max_move = value;
    mob->guilds = ch->guilds;
    mob->pIndexData->prac_skills = level * 10;
    mob->pIndexData->prac_spells = level * 10;
    value /= 2;
    for (i = 0; i < MAX_STATS; i ++)
        mob->perm_stat[i] = ch->perm_stat[i];

    for ( type = 0 ; type < MAX_ELEMENT_TYPE ; type++ )
    {
        mob->mana[ type ] = value * 4;
        mob->max_mana[ type ] = value * 4;
    }

    strncpy(mob->hunting, ch->name,30);
    mob->level = UMIN( 100, level );
    pick_skills( mob );
    return;
}

void create_thug( CHAR_DATA *ch, int tlevel )
{
    char name[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    ROOM_INDEX_DATA *in_room;
    int level;
    int type;
    int value;

    if ( ( mob = create_mobile( get_mob_index( MOB_VNUM_THUG ) ) ) == NULL )
    {
	bug( "No Loan Shark Thug.", 0 );
	return;
    }

    if ( ch->pcdata->debt->timer == 3 )
	level = ch->level - 5;
    else
	level = ch->level + ch->pcdata->debt->timer - 5;

    if ( tlevel < 1 )
        level = URANGE( 1, level, 300);
    else
	level = tlevel;

    value = level * level;
    value = value + ( -1 * value ) % 10;

    strcpy( name, thug_name[ number_range( 0, MAX_THUG_NAME - 1 ) ] );
    sprintf( buf, "%s thug loan", name );
    strncpy( mob->name, buf, 30 );
    sprintf( buf, "%s the thug", name );
    strncpy( mob->short_descr, buf, 80 );
    sprintf( buf, "%s the thug is trying to collect a debt from %s.", name, ch->name );
    strncpy( mob->long_descr, buf, 80 );
    in_room = get_room_index( ch->pcdata->debt->vnum );
    sprintf( buf, "%s the thug is trying to collect a debt from %s.  Could this be yours?\n\rThe large muscles covering his body reminds never to default on a loan.\n\rTo pay your debt go to %s and type pay all\n\r%s\n\r",
	name, 
	ch->name, 
	( in_room == NULL ? "any loan shark" : in_room->name ),
	"(Hopefully you have enough money to pay off your debt)" );
    mob->description = str_dup( buf );

    mob->armor = 100 - number_range( value * 2 / 25, value * 2 / 5 );
    mob->hitroll = level * 4 / 3;
    mob->damroll = level / 3;
    mob->damage[ 0 ] = UMAX( 1, value / 33 );
    mob->damage[ 1 ] = 3;
    mob->damage[ 2 ] = 0;
    mob->hit = value * 2;
    mob->max_hit = value * 2;
    mob->move = value;
    mob->max_move = value;
    mob->pIndexData->prac_skills = level * 8;
    mob->pIndexData->prac_spells = level * 8;
    value /= 2;

    for ( type = 0 ; type < MAX_ELEMENT_TYPE ; type++ )
    {
        mob->mana[ type ] = value * 10;
        mob->max_mana[ type ] = value * 10;
    }
    strncpy(mob->hunting, ch->name,30);
    mob->level = UMIN( 100, level );

    if ( ( in_room = get_room_index( ch->pcdata->debt->vnum ) ) == NULL )
	if ( ( in_room = get_room_index( 2 ) ) == NULL )
	{
	    bug( "No begin room for loan shark thug.", 0 );
	    return;
	}

    char_to_room( mob, in_room );
    act( "$n is created to hunt down $N.", mob, NULL, ch, TO_ROOM );

}

void send_thug( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    NOTE_DATA *pnote;
    DEBT_DATA *debt;
    long new_time;

    if ( ch->pcdata == NULL || ch->pcdata->debt == NULL )
	return;

    debt = ch->pcdata->debt;
    new_time = debt->due - ch->played + ch->logon - current_time;

    if ( new_time > 9000 )
	return;
/* 0 */
    else if ( debt->timer < 1 )
    {
	if ( ch->fighting != NULL )
	    return;
	send_to_char( "Your debt is due in 30 min!\n\r", ch );
	debt->timer++;
	return;
    }
    else if ( new_time > 6000 )
	return;
/* 1 */
    else if ( debt->timer < 2 )
    {
	if ( ch->fighting != NULL )
	    return;
	send_to_char( "Your debt is due in 20 min!\n\r", ch );
	debt->timer++;
	return;
    }
    else if ( new_time > 3000 )
	return;
/* 2 */
    else if ( debt->timer < 3 )
    {
	if ( ch->fighting != NULL )
	    return;
	send_to_char( "Your debt is due in 10 min!\n\r", ch );
	debt->timer++;
	return;
    }
    else if ( new_time > 0 )
	return;
/* 3 */
    else if ( debt->timer < 4 )
    {
	char *strtime;
   	ROOM_INDEX_DATA *in_room;

	if ( ch->fighting != NULL )
	    return;

	in_room = get_room_index( ch->pcdata->debt->vnum );

	send_to_char( "YOUR DEBT IS DUE NOW!\n\r", ch );

	pnote				= note_data_alloc();
	strncpy( pnote->to_list, ch->name, 80 );
	strncpy( pnote->subject, "Re: Your outstanding Debt!", 80 );
	pnote->read_list[0]		= '\0';
	pnote->flags			= 0;
	pnote->next			= NULL;
	strtime				= ctime( &current_time );
	strtime[strlen(strtime)-1]	= '\0';
	strncpy( pnote->date, strtime, 30 );
	pnote->date_stamp		= current_time;
	if ( in_room != NULL )
	    strncpy( pnote->sender, in_room->name, 30 );
	else
	    strncpy( pnote->sender, "An Unknown Loan Shark", 30 );

	sprintf( buf, "Dear %s,\n\r\n\r    It has come to our attention that your loan of %ld %s is past due.\n\rIf you do not pay %ld %s imeadiately, we will be forced to use extreme\n\rmeasures.\n\r%s%s\n\r%s%s\n\r",
	capitalize(ch->name),
	debt->amount/2,
	coin_names[ debt->type ],
	debt->amount,
	coin_names[ debt->type ],
	"                                                    ",
	"Sincerly,",
	"                                                    ",
	( in_room == NULL ? "An Unkown Loan Shark" : in_room->name ) );

	pnote->text 			= str_dup( buf );

	note_add( pnote );
	save_notes();
	debt->timer++;
	return;
    }
    if ( new_time > -9000 )
	return;
/* 4 */
    if ( debt->timer + ( new_time / 9000 ) < 4 && new_time > -24000 )
    {
	create_thug( ch, 0 );
	debt->timer++;
	return;
    }
    if ( new_time > -24000 )
	return;
/* 6 */
    if ( debt->timer + ( ( new_time + 18000 ) / 6000 ) < 6 && new_time > -39000 )
    {
	create_thug( ch, 0 );
	debt->timer++;
	return;
    }
    if ( new_time > -39000 )
	return;
/* 9 */
    if ( debt->timer + ( ( new_time + 36000 ) / 3000 ) < 9 && new_time > -55500 )
    {
	create_thug( ch, 0 );
	debt->timer++;
	return;
    }
    if ( new_time > -55500 )
	return;
/* 15 */
    if ( debt->timer + ( ( new_time + 54000 ) / 1500 ) < 15 && new_time > -72600 )
    {
	create_thug( ch, 0 );
	debt->timer++;
	return;
    }
    if ( new_time > -72600 )
	return;
/* 27 */
    if ( debt->timer + ( ( new_time + 72000 ) / 600 ) < 27 && new_time > -90300 )
    {
	create_thug( ch, 0 );
	debt->timer++;
	return;
    }
    if ( new_time > -90300 )
	return;
/* 57 */
    if ( debt->timer + ( ( new_time + 90000 ) / 300 ) < 57 )
    {
	create_thug( ch, 0 );
	debt->timer++;
	return;
    }
    else
	return;
}

bool check_immortal_order(CHAR_DATA *ch, CHAR_DATA *vch)
{
    if (!ch || !vch)
    {
	bug("Check immortal order, Null chars", 0);
	return FALSE;
    }

    if (IS_OPTIVISION(ch) )
	return TRUE;

    if (IS_NPC(vch) )
	return TRUE;

    if (IS_IMP(ch) && !IS_OPTIVISION(vch) )
	return TRUE;

    if (IS_IMMORTAL(ch) && ( !IS_IMP(vch) || !IS_OPTIVISION(vch) ) )
	return TRUE;

    return FALSE;
}

/* for immunity, vulnerabiltiy, and resistant
   the 'globals' (magic and weapons) may be overriden
   three other cases -- wood, silver, and iron -- are checked in fight.c */

int check_immune(CHAR_DATA *ch, int dam_type)
{
    int immune;
    int bit_res, bit_vuln, bit_imm;

    immune = IS_NORMAL;

    if (dam_type == DAM_NONE)
	return immune;

    if (dam_type <= 3)
    {
	if (IS_SET(ch->res_flags, RES_WEAPON) && IS_SET(ch->vuln_flags, VULN_WEAPON) && !IS_SET(ch->imm_flags, IMM_WEAPON))
	    immune = IS_NORMAL;
	else if (IS_SET(ch->imm_flags,IMM_WEAPON))
	    immune = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_WEAPON))
	    immune = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_WEAPON))
	    immune = IS_VULNERABLE;
    }
    else /* magical attack */
    {	
	if (IS_SET(ch->res_flags, RES_MAGIC) && IS_SET(ch->vuln_flags, VULN_MAGIC) && !IS_SET(ch->imm_flags, IMM_MAGIC))
	    immune = IS_NORMAL;
	else if (IS_SET(ch->imm_flags,IMM_MAGIC))
	    immune = IS_IMMUNE;
	else if (IS_SET(ch->res_flags,RES_MAGIC))
	    immune = IS_RESISTANT;
	else if (IS_SET(ch->vuln_flags,VULN_MAGIC))
	    immune = IS_VULNERABLE;
    }

    /* set bits to check -- VULN etc. must ALL be the same or this will fail */
    switch (dam_type)
    {
	case(DAM_BASH):		
	    bit_res = RES_BASH;
	    bit_vuln = VULN_BASH;
	    bit_imm = IMM_BASH;
	    break;
	case(DAM_PIERCE):
	    bit_res = RES_SLASH;
	    bit_vuln = VULN_SLASH;
	    bit_imm = IMM_SLASH;
	    break;
	case(DAM_SLASH):
	    bit_res = RES_SLASH;
	    bit_vuln = VULN_SLASH;
	    bit_imm = IMM_SLASH;
	    break;
	case(DAM_FIRE):
	    bit_res = RES_FIRE;
	    bit_vuln = VULN_FIRE;
	    bit_imm = IMM_FIRE;
	    break;
	case(DAM_ACID):
	    bit_res = RES_ACID;
	    bit_vuln = VULN_ACID;
	    bit_imm = IMM_ACID;
	    break;
	case(DAM_POISON):
	    bit_res = RES_POISON;
	    bit_vuln = VULN_POISON;
	    bit_imm = IMM_POISON;
	    break;
	case(DAM_ENERGY):
	    bit_res = RES_ENERGY;
	    bit_vuln = VULN_ENERGY;
	    bit_imm = IMM_ENERGY;
	    break;
	case(DAM_MENTAL):
	    bit_res = RES_MENTAL;
	    bit_vuln = VULN_MENTAL;
	    bit_imm = IMM_MENTAL;
	    break;
	case(DAM_DISEASE):
	    bit_res = RES_DISEASE;
	    bit_vuln = VULN_DISEASE;
	    bit_imm = IMM_DISEASE;
	    break;
	case(DAM_DROWNING):
	    bit_res = RES_DROWNING;
	    bit_vuln = VULN_DROWNING;
	    bit_imm = IMM_DROWNING;
	    break;
	case(DAM_LIGHT):
	    bit_res = RES_LIGHT;
	    bit_vuln = VULN_LIGHT;
	    bit_imm = IMM_LIGHT;
	    break;
	case(DAM_WIND):	
	    bit_res = RES_WIND;
	    bit_vuln = VULN_WIND;
	    bit_imm = IMM_WIND;
	    break;
	case(DAM_EARTH):
	    bit_res = RES_EARTH;
	    bit_vuln = VULN_EARTH;
	    bit_imm = IMM_EARTH;
	    break;
	case(DAM_WATER):
	    bit_res = RES_WATER;
	    bit_vuln = VULN_WATER;
	    bit_imm = IMM_WATER;
	    break;
	case(DAM_SPIRIT):
	    bit_res = RES_SPIRIT;
	    bit_vuln = VULN_SPIRIT;
	    bit_imm = IMM_SPIRIT;
	    break;
	default:		return immune;
    }

    if (IS_SET(ch->res_flags, bit_res) && IS_SET(ch->vuln_flags, bit_vuln) && !IS_SET(ch->imm_flags, bit_imm))
	immune = IS_NORMAL;
    else if (IS_SET(ch->imm_flags,bit_imm))
	immune = IS_IMMUNE;
    else if (IS_SET(ch->res_flags,bit_res))
	immune = IS_RESISTANT;
    else if (IS_SET(ch->vuln_flags,bit_vuln))
	immune = IS_VULNERABLE;

    if ( dam_type == DAM_DROWNING && IS_AFFECTED( ch, AFF_BREATHE_UNDERWATER ) )
	immune = IS_IMMUNE;

    return immune;
}

sh_int get_light_adjustment( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    int new_light, added_light;
    int hour, sunrise, sunset, world;
    CHAR_DATA *vch;
    CHAR_DATA *ch_next;
 
    if ( !pRoomIndex && ch)
	pRoomIndex = get_room_index(ch->in_room->vnum);

    new_light = pRoomIndex->light;
    if ( pRoomIndex->sector_type == SECT_CITY || IS_SET( pRoomIndex->room_flags_2, ROOM_LIT ) )
	new_light += 45;

    if ( !IS_SET(pRoomIndex->room_flags,ROOM_INDOORS)
	&& pRoomIndex->sector_type != SECT_UNDERGROUND
	&& pRoomIndex->sector_type != SECT_OCEAN_BOTTOM
	&& pRoomIndex->sector_type != SECT_UNDERWATER )
    {
    	world = pRoomIndex->area->world;
    	sunrise = time_table[ world ].sunrise;
    	sunset = time_table[ world ].sunset;
    	hour = get_hour( pRoomIndex->area );

    	if ( hour - sunrise <= 5 && hour - sunrise >  -3 )
   	    new_light += 25 + ( ( hour - sunrise ) * 10 );
    	else if ( sunset - hour <= 5 && sunset - hour > -3 )
	    new_light += 25 + ( ( sunset - hour ) * 10 );
    	else if ( sunset - hour >= 5 && hour - sunrise >= 5 )
	    new_light += 75;

	if ( pRoomIndex->area->weather->sky == SKY_CLOUDY )
	    new_light -= 20;
    }

    if ( ch != NULL )
    {
    	vch = ch;

    	for ( vch = ch->in_room->people; vch != NULL; vch = ch_next )
	{
            ch_next = vch->next_in_room;

           added_light = get_eq_light( vch );

            if ( added_light > new_light )
                new_light += ( added_light - new_light );
            else if ( added_light < 0 )
                new_light += added_light;

            added_light = get_eq_magic_light( vch );

            if ( added_light > new_light )
                new_light += ( added_light - new_light );
            else if ( added_light < 0 )
                new_light += added_light;

            if ( is_affected(vch, gsn_fire_shield ) && new_light <= 25 )
                new_light += 25;
            if ( is_affected(vch, gsn_faerie_fire ) && new_light <= 25 )
                new_light += 25;
            if ( is_affected(vch, gsn_sanctuary ) && new_light <= 25 )
                new_light += 25;
	}
    }

    return URANGE(1, new_light, 100);
}

sh_int get_hour( AREA_DATA *pArea )
{
     return ( ( time_info[ pArea->world ].hour + pArea->time_zone ) % time_table[ pArea->world ].hours_day ); 
}

sh_int get_season( AREA_DATA *pArea )
{
    int month,season;

    season = UMAX( pArea->weather->fall, UMAX( pArea->weather->winter, UMAX( pArea->weather->spring, pArea->weather->summer )));
    if ( season == pArea->weather->spring )
	season = 0;
    else if ( season == pArea->weather->summer )
	season = 1;
    else if ( season == pArea->weather->winter )
	season = 2;
    else 
	season = 3;

    for ( month = 0 ; month <= time_info[ pArea->world ].month  ; month++ )
    {
	if ( month == pArea->weather->spring )
	    season = 0;
	if ( month == pArea->weather->summer )
	    season = 1;
	if ( month == pArea->weather->fall )
	    season = 2;
	if ( month == pArea->weather->winter )
	    season = 3;
    }

    return season;
}

sh_int get_sky_type( AREA_DATA * pArea )
{

    if ( pArea->weather->mmhg > pArea->weather->chance[ pArea->weather->season ][ 3 ] )
        return ( pArea->weather->storm_type[ pArea->weather->season ] );
    if ( pArea->weather->mmhg > pArea->weather->chance[ pArea->weather->season ][ 2 ] )
	return ( pArea->weather->storm_type[ pArea->weather->season ] - 1 );
    if ( pArea->weather->mmhg > pArea->weather->chance[ pArea->weather->season ][ 1 ] )
	return SKY_CLOUDY;

    return SKY_CLOUDLESS;
}

sh_int	get_height_size( int size )
{
    switch( size )
    {
    default:
	return roll_dice( 3, 6 ) + 58;
    case 0:
	return roll_dice( 3, 6 ) + 6;
    case 1:
	return roll_dice( 4, 6 ) + 34;
    case 2:
	return roll_dice( 3, 6 ) + 58;
    case 3:
	return roll_dice( 12, 6 ) + 70;
    case 4:
	return roll_dice( 30, 12 ) + 120;
    }
}

sh_int	get_weight_size( int size )
{
    switch( size )
    {
    default:
	return roll_dice( 17, 10 ) + 80;
    case 0:
	return roll_dice( 3, 4 ) + 3;
    case 1:
	return roll_dice( 6, 10 ) + 40;
    case 2:
	return roll_dice( 17, 10 ) + 80;
    case 3:
	return roll_dice( 20, 15 ) + 220;
    case 4:
	return roll_dice( 50, 32 ) + 400;
    }
}

int get_skill_level( CHAR_DATA * ch, int sn)
{
    int cost = 0;
    int guild;
    int guilds = 0;

    if ( IS_NPC(ch) && ch->guilds <= 1 )
    {
	cost +=125;
	cost *= race_table[ ch->race ][ ch->sex ].skill_level[ sn ];
	cost /= 100;
	return URANGE( 1, cost , 100 );
    }

    if ( ch->guilds == 1 && !IS_NPC(ch) )
    { 
	cost += guild_table[ 0 ].rating[ skill_table[ sn ].group ];
    	if ( cost > 225 )
    	{
  		cost = 100;
		return cost;
    	}		
	cost *= race_table[ ch->race ][ ch->sex ].skill_level[ sn ];
	cost /= 100;
	return URANGE( 1, cost , 100 );
    } 

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
    {
	if ( !IS_SET( ch->guilds, guild_table[ guild ].flag) || ch->guilds <= 1 )
	     continue;
	cost += guild_table[ guild ].rating[ skill_table[ sn ].group ];
	guilds++;

    }

    if ( guilds > 0 )
    cost /= guilds;

    if ( cost > 225 )
    {
  	cost = 100;
	return cost;
    }		
    else if ( cost > 200 )
	cost = 225;
    else if ( cost > 175 && cost <= 200 )
	cost = 200;
    else if ( cost > 150 && cost <= 175 )
	cost = 175;
    else if ( cost > 125 && cost <= 150 )
	cost = 150;
    else if ( cost > 100 && cost <= 125 )
	cost = 125;
    else if ( cost > 75 && cost <= 100 )
	cost = 100;
    else if ( cost > 50 && cost <= 75 )
	cost = 75;
    else
	cost = 50;

    cost *= race_table[ ch->race ][ ch->sex ].skill_level[ sn ];

    cost /= 100;

    return URANGE( 1, cost , 100 ); 
}

int get_mana_cost( CHAR_DATA * ch, int sn, int i)
{
    int cost = 0;
    int guild;
    int guilds = 0;
    SKILL_LIST *skills;

    if ( IS_NPC( ch ) )
    for ( skills = ch->innate_skills ; skills != NULL ; skills = skills->next )
    {
	if( sn == skills->sn )
	    return 10;
    }

    if ( ch->guilds <= 1 )
    {
	cost += guild_table[ 0 ].rating[ skill_table[ sn ].group ];

    	if ( cost > 225 )
	    cost = 250;
    	else if ( cost > 200 && cost <= 225 )
	    cost = 155;
    	else if ( cost > 175 && cost <= 200 )
	    cost = 140;
   	else if ( cost > 150 && cost <= 175 )
	    cost = 125;
    	else if ( cost > 125 && cost <= 150 )
	    cost = 110;
   	else if ( cost > 100 && cost <= 125 )
	    cost = 95;
    	else if ( cost > 75 && cost <= 100 )
	    cost = 80;
    	else if ( cost > 50 && cost <= 75 )
	    cost = 65;
    	else
	    cost = 50;

	cost *= (skill_table[ sn ].element[ i ].mana + 25);
	cost /= 100;

        if ( get_skill( ch, gsn_element_power[ i ][ 0 ] ) == 100 )
	    cost = cost - ( cost * 10 / 100 );

	if ( get_skill( ch, gsn_element_power[ i ][ 1 ] ) == 100 )
	    cost = cost - ( cost * 10 / 100 );

	if ( get_skill( ch, gsn_element_power[ i ][ 2 ] ) == 100 )
	    cost = cost - ( cost * 10 / 100 );

        if ( skill_table[sn].element[i].mana == 0 )
	    return 0;

	return URANGE( 25, cost , 5000 );
    }

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
    {
	if ( !IS_SET( ch->guilds, guild_table[ guild ].flag) || ch->guilds <= 1 )
	     continue;
	
	cost += guild_table[ guild ].rating[ skill_table[ sn ].group ];
	guilds++;
    }

    if ( guilds <= 0 )
    {
	cost += guild_table[ 0 ].rating[ skill_table[ sn ].group ];
	cost *= (skill_table[ sn ].element[ i ].mana + 25);
	cost /= 100;

	if ( skill_table[ sn ].element[ i ].mana == 0 )
	    return 0;

	return URANGE( 25, cost , 5000 );
    }

    cost /= guilds;

    if ( cost > 225 )
	cost = 250;
    else if ( cost > 200 && cost <= 225 )
	cost = 155;
    else if ( cost > 175 && cost <= 200 )
	cost = 140;
    else if ( cost > 150 && cost <= 175 )
	cost = 125;
    else if ( cost > 125 && cost <= 150 )
	cost = 110;
    else if ( cost > 100 && cost <= 125 )
	cost = 95;
    else if ( cost > 75 && cost <= 100 )
	cost = 80;
    else if ( cost > 50 && cost <= 75 )
	cost = 65;
    else
	cost = 50;

    cost *= (skill_table[ sn ].element[ i ].mana + 25);

    cost /= 100;

    if ( get_skill( ch, gsn_element_power[ i ][ 0 ] ) == 100 )
    	cost = cost - ( cost * 10 / 100 );

    if ( get_skill( ch, gsn_element_power[ i ][ 1 ] ) == 100 )
    	cost = cost - ( cost * 10 / 100 );

    if ( get_skill( ch, gsn_element_power[ i ][ 2 ] ) == 100 )
    	cost = cost - ( cost * 10 / 100 );

    if ( skill_table[ sn ].element[ i ].mana == 0 )
	return 0;

    return URANGE( 25, cost , 5000 );
}

int get_skill_cost( CHAR_DATA * ch, int sn)
{
    int cost = 1;
    int guild;
    int guilds = 1;

    if ( ch->guilds <= 1 )
    {
	cost *= guild_table[ 0 ].rating[ skill_table[ sn ].group ];
	cost *= race_table[ ch->race ][ ch->sex ].rating[ sn ];
	cost /= 100;
	return URANGE(1, cost, 600);
    }
    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
    {
	if ( !IS_SET( ch->guilds, guild_table[ guild ].flag ) || ch->guilds <= 1 )
	     continue;

	cost *= guild_table[ guild ].rating[ skill_table[ sn ].group ];
	guilds++;
    }

    cost *= race_table[ ch->race ][ ch->sex ].rating[ sn ];

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
    {
	if ( !IS_SET( ch->guilds, guild_table[ guild ].flag ) )
	     continue;
	cost /= 100;
    }

    return URANGE( 1, cost , 600 );
}

long get_gain_cost( CHAR_DATA * ch, int sn)
{
    long cost = 1;
    int guild;
    int count = 0;

    if ( ch->level < 10)
    {
	cost = 10 * ch->level;
	return URANGE(1, cost, 100);
    }

    if ( ch->guilds < 1 )
	cost = 175;

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
    {
	if ( !IS_SET( ch->guilds, guild_table[ guild ].flag ) || ch->guilds < 1 )
	     continue;

	cost *= guild_table[ guild ].rating[ skill_table[ sn ].group ] / 10;
    }

    if ( ch->level > 40 )
	cost *= get_skill_cost( ch, sn) * 1000 / 10 * ch->level; 
    else if ( ch->level > 20 )
	cost *= get_skill_cost( ch, sn) * 350  / 10 * ch->level; 
    else if ( ch->level >= 10 )
    	cost *= get_skill_cost( ch, sn) * 75 / 10 * ch->level; 

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
    {
	if ( !IS_SET( ch->guilds, guild_table[ guild ].flag ) || ch->guilds <= 1 )
	     continue;
	if ( count != 0 )
	    cost /= 10;
	count = 1;
    }

    return URANGE( 1, cost, 5000000 );
}

/* for returning skill information */
int get_skill(CHAR_DATA *ch, int sn)
{
    int skill=0;
    int i;
    int level;

    if (sn == -1) /* shorthand for level based skills */
    {
	skill = ch->level * 5 / 2;
    }
    else if (sn < -1 || sn > MAX_SKILL)
    {
	bug("Bad sn %d in get_skill.",sn);
	skill = 0;
    }
    else
    {
	if ( !IS_NPC(ch) )
	{
		if (  ch->pcdata->learned[ sn ] < 1 )
	    		skill = 0;
		else
	    		skill = ch->pcdata->learned[sn] + ch->pcdata->skill_mod[sn];
	

	/* must have previous level of element power to have skill */
        for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	    for ( level = 1 ; level < 3 ; level++ )
	        if ( sn == gsn_element_power[ i ][ level ] 
	          && get_skill( ch, gsn_element_power[ i ][ level - 1 ] ) < 85 )
		    return 0;

	/* must have skill in element powers to have skill in a spell */
        if ( skill_table[ sn ].spell_fun != spell_null )
	    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	        if ( ( level = skill_table[ sn ].element[ i ].level ) >= 0 )
		    if ( get_skill( ch, gsn_element_power[ i ][ level ] ) <= 0 )
		        return 0;
    	}
    }

    if ( IS_NPC(ch) ) /* mobiles */
    {
	SKILL_LIST * skills;
	bool found = FALSE;
	bool found_innate = FALSE;

	for ( skills = ch->innate_skills ; skills != NULL ; skills = skills->next )
	    if ( skills->sn == sn )
	    {
		found_innate = TRUE;
		break;
	    }
   
	if ( found_innate == FALSE )
	{
	    for ( skills = ch->skills ; skills != NULL ; skills = skills->next )
	    	if ( skills->sn == sn )
	    	{
		    found = TRUE;
		    break;
	    	}
	}

	if ( found_innate )
	{
           skill = 85;
	   return skill;
	}

	if ( !found )
	    return 0;

 	if (sn == gsn_trip && IS_SET(ch->off_flags,OFF_TRIP))
	    skill = 10 + 3 * ch->level;
 	else if (sn == gsn_bash && IS_SET(ch->off_flags,OFF_BASH))
	    skill = 10 + 3 * ch->level;
 	else if (sn == gsn_breathfire && IS_SET(ch->off_flags,OFF_BREATHFIRE) )
	    skill = 10 + 3 * ch->level;
	else if (sn == gsn_disarm && IS_SET(ch->off_flags,OFF_DISARM) )
	    skill = 20 + 3 * ch->level;
	else if (sn == gsn_berserk && IS_SET(ch->off_flags,OFF_BERSERK))
	    skill = 3 * ch->level;
	else if (is_language(sn) )
	    skill = race_table[ ch->race ][ ch->sex ].adept[ sn ];
        else if ( skill_table[ sn ].spell_fun != spell_null )
           skill = URANGE( 40, 40 + ( ch->level * 3 / 2 ), 100 );
	else if ( get_skill_level(ch, sn) <= ch->level )
   	   skill = URANGE(40, 40 + ( ch->level * 3 / 2 ), 100 );
	else 
	   skill = 0;

        skill = URANGE(0,skill,100);
    }

    if (IS_AFFECTED(ch,AFF_BERSERK))
	skill -= 10;

    return skill;
}

int get_weapon_skill( CHAR_DATA * ch, OBJ_DATA * obj )
{
    int type;
    int skill;

    if ( IS_NPC( ch ) )
    {
        if ( obj == NULL || obj->wear_data == NULL || obj->wear_data->weapon_data == NULL
	   || ( type = obj->wear_data->weapon_data->type ) < 0 || type >= MAX_WEAPON_TYPE )
	{
	    skill = UMIN( 100, 40 + ch->level * 8 );
	    skill += URANGE( 0, ( ch->level - 12 ) * 8, 100 );
	    skill += get_skill( ch, gsn_hand_to_hand[2] );
	    return skill;
	}
	else
	{
	    skill = UMIN( 100, 40 + ch->level * 8 );
	    skill += URANGE( 0, ( ch->level - 12 ) * 8, 100 );
	    skill += get_skill( ch, gsn_weapon_power[type][2] );
	    return skill;
	}
    }

    if ( obj == NULL || obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
    {
	skill = get_skill( ch, gsn_hand_to_hand[0] ) + get_skill( ch, gsn_hand_to_hand[1] ) + get_skill( ch, gsn_hand_to_hand[2] );
	return ( ch->hands_free == 2 ? skill : ( ch->hands_free == 1 ? UMIN( 150, skill ) : UMIN( 50, skill ) ) );
    }

    type = obj->wear_data->weapon_data->type;

    if ( type < 0 || type >= MAX_WEAPON_TYPE )
    {
	skill = get_skill( ch, gsn_hand_to_hand[0] ) + get_skill( ch, gsn_hand_to_hand[1] ) + get_skill( ch, gsn_hand_to_hand[2] );
	return ( ch->hands_free == 2 ? skill : ( ch->hands_free == 1 ? UMIN( 150, skill ) : UMIN( 50, skill ) ) );
    }

    skill = get_skill( ch, gsn_weapon_power[ type ][ 0 ] ) + get_skill( ch, gsn_weapon_power[ type ][ 1 ] ) + get_skill( ch, gsn_weapon_power[ type ][ 2 ] );

    return skill;
}

/* used to de-screw characters */
void reset_char(CHAR_DATA *ch)
{
     int loc,mod,stat;
     OBJ_DATA *obj;
     AFFECT_DATA *af;
     int i;

     if (IS_NPC(ch))
	return;

    ch->desc->pString = NULL;

    if (ch->pcdata->perm_hit == 0 
    ||	ch->pcdata->perm_mana[0] == 0
    ||	ch->pcdata->perm_mana[1] == 0
    ||	ch->pcdata->perm_mana[2] == 0
    ||	ch->pcdata->perm_mana[3] == 0
    ||	ch->pcdata->perm_mana[4] == 0
    ||  ch->pcdata->perm_move == 0
    ||	ch->pcdata->last_level == 0)
    {
    /* do a FULL reset */
	for (loc = 0; loc < MAX_WEAR; loc++)
	{
	    obj = get_eq_char(ch,loc);
	    if (obj == NULL)
		continue;

	    if ( obj->wear_data != NULL )
            for ( af = obj->wear_data->affected; af != NULL; af = af->next )
            {
                mod = af->modifier;
                switch(af->location)
                {
                    case APPLY_SEX:     ch->sex         -= mod;         break;
                    case APPLY_MANA:
		    for( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		        ch->max_mana[i]    -= mod;
	            break;
    case APPLY_MANA_FIRE:	ch->max_mana[ ELEMENT_FIRE ]	-= mod; break;
    case APPLY_MANA_WATER:	ch->max_mana[ ELEMENT_WATER ]	-= mod; break;
    case APPLY_MANA_EARTH:	ch->max_mana[ ELEMENT_EARTH ]	-= mod; break;
    case APPLY_MANA_WIND:	ch->max_mana[ ELEMENT_WIND ]	-= mod; break;
    case APPLY_MANA_SPIRIT:	ch->max_mana[ ELEMENT_SPIRIT ]	-= mod; break;
                    case APPLY_HIT:     ch->max_hit     -= mod;         break;
                    case APPLY_MOVE:    ch->max_move    -= mod;         break;
                }
            }
	}
	/* now reset the permanent stats */
	ch->pcdata->perm_hit 	= ch->max_hit;
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	    ch->pcdata->perm_mana[i] 	= ch->max_mana[i];
	ch->pcdata->perm_move	= ch->max_move;
	ch->pcdata->last_level	= ch->played/3600;
	if (ch->pcdata->true_sex < 1 || ch->pcdata->true_sex > 3)
	{
	    if (ch->sex > 1 && ch->sex < 4)
	        ch->pcdata->true_sex	= ch->sex;
	    else
	    	ch->pcdata->true_sex 	= 1;
	}
    }

    if ( ch->birth_day == 0 || ch->birth_month == 0 )
    {
	ch->birth_day = number_range( 1, time_table[ race_table[ ch->race ][ ch->sex ].world ].days_month );
	ch->birth_month = number_range( 1, time_table[ race_table[ ch->race ][ ch->sex ].world ].months_year );
    }

    /* now restore the character to his/her true condition */
    for (stat = 0; stat < MAX_STATS; stat++)
	ch->mod_stat[stat] = 0;

    ch->hands_free = 2;
    if (ch->pcdata->true_sex < 1 || ch->pcdata->true_sex > 3)
	ch->pcdata->true_sex = 1; 
    ch->sex		= ch->pcdata->true_sex;
    ch->max_hit 	= ch->pcdata->perm_hit;
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
        ch->max_mana[i] = ch->pcdata->perm_mana[i];

    ch->max_move	= ch->pcdata->perm_move;
    ch->armor	= 100;
    ch->hitroll		= 0;
    ch->damroll		= 0;
    ch->saving_throw	= 0;
    ch->pcdata->dreams	= 0;
    ch->pcdata->nightmares	= 0;

    /* now start adding back the effects */
    for (loc = 0; loc < MAX_WEAR; loc++)
    {
        obj = get_eq_char(ch,loc);
        if (obj == NULL || obj->wear_data == NULL)
            continue;

   	if ( obj->wear_data->wear_loc == WEAR_WIELD_R )
	    ch->hands_free--;

   	if ( obj->wear_data->wear_loc == WEAR_WIELD_L )
	    ch->hands_free--;

   	if ( obj->wear_data->wear_loc == WEAR_WIELD_TWO_HANDED )
	    ch->hands_free=0;

   	if ( obj->wear_data->wear_loc == WEAR_SHIELD && !IS_SET( obj->extra_flags, ITEM_BUCKLER ) )
	    ch->hands_free--;

   	if ( obj->wear_data->wear_loc == WEAR_HOLD && !IS_SET( obj->extra_flags, ITEM_BUCKLER ) )
	    ch->hands_free--;

	ch->armor -= obj->wear_data->ac;
        for ( af = obj->wear_data->affected; af != NULL; af = af->next )
        {
            mod = af->modifier;
            switch(af->location)
            {
                case APPLY_STR:         ch->mod_stat[STAT_STR]  += mod; break;
                case APPLY_DEX:         ch->mod_stat[STAT_DEX]  += mod; break;
                case APPLY_INT:         ch->mod_stat[STAT_INT]  += mod; break;
                case APPLY_WIS:         ch->mod_stat[STAT_WIS]  += mod; break;
                case APPLY_CON:         ch->mod_stat[STAT_CON]  += mod; break;
                case APPLY_CHR:         ch->mod_stat[STAT_CHR]  += mod; break;
 
                case APPLY_SEX:         ch->sex                 += mod; break;
                case APPLY_MANA:
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	            ch->max_mana[i]         += mod; 
		break;
    		case APPLY_MANA_FIRE:	ch->max_mana[ ELEMENT_FIRE ]	+= mod; break;
    		case APPLY_MANA_WATER:	ch->max_mana[ ELEMENT_WATER ]	+= mod; break;
    		case APPLY_MANA_EARTH:	ch->max_mana[ ELEMENT_EARTH ]	+= mod; break;
    		case APPLY_MANA_WIND:	ch->max_mana[ ELEMENT_WIND ]	+= mod; break;
    		case APPLY_MANA_SPIRIT:	ch->max_mana[ ELEMENT_SPIRIT ]	+= mod; break;
    		case APPLY_HIT:         	ch->max_hit             += mod; break;
    		case APPLY_MOVE:        	ch->max_move            += mod; break;
    		case APPLY_DREAMS:		ch->pcdata->dreams	+= mod; break; 
    		case APPLY_NIGHTMARES:	ch->pcdata->nightmares  += mod; break;
    		case APPLY_AC:		ch->armor 		+= mod; break;
    		case APPLY_HITROLL:     	ch->hitroll             += mod; break;
    		case APPLY_DAMROLL:     	ch->damroll             += mod; break;
    		case APPLY_SAVING_SPELL:    ch->saving_throw 	+= mod; break;
            }
	}
    }
  
    /* now add back spell effects */
    for (af = ch->affected; af != NULL; af = af->next)
    {
        mod = af->modifier;
        switch(af->location)
        {
                case APPLY_STR:         ch->mod_stat[STAT_STR]  += mod; break;
                case APPLY_DEX:         ch->mod_stat[STAT_DEX]  += mod; break;
                case APPLY_INT:         ch->mod_stat[STAT_INT]  += mod; break;
                case APPLY_WIS:         ch->mod_stat[STAT_WIS]  += mod; break;
                case APPLY_CON:         ch->mod_stat[STAT_CON]  += mod; break;
                case APPLY_CHR:         ch->mod_stat[STAT_CHR]  += mod; break;
 
                case APPLY_SEX:         ch->sex                 += mod; break;
                case APPLY_MANA:
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	            ch->max_mana[i]         += mod; 
		break;
    		case APPLY_MANA_FIRE:	ch->max_mana[ ELEMENT_FIRE ]	+= mod; break;
    		case APPLY_MANA_WATER:	ch->max_mana[ ELEMENT_WATER ]	+= mod; break;
    		case APPLY_MANA_EARTH:	ch->max_mana[ ELEMENT_EARTH ]	+= mod; break;
    		case APPLY_MANA_WIND:	ch->max_mana[ ELEMENT_WIND ]	+= mod; break;
    		case APPLY_MANA_SPIRIT:	ch->max_mana[ ELEMENT_SPIRIT ]	+= mod; break;
                case APPLY_HIT:         ch->max_hit             += mod; break;
                case APPLY_MOVE:        ch->max_move            += mod; break;
 		case APPLY_DREAMS:	ch->pcdata->dreams	+= mod; break; 
		case APPLY_NIGHTMARES:	ch->pcdata->nightmares  += mod; break;
                case APPLY_AC:		ch->armor		+= mod; break;
                case APPLY_HITROLL:     ch->hitroll             += mod; break;
                case APPLY_DAMROLL:     ch->damroll             += mod; break;
                case APPLY_SAVING_SPELL:        ch->saving_throw += mod; break;
        } 
    }

    /* make sure sex is RIGHT!!!! */
    if (ch->sex < 1 || ch->sex > 3)
	ch->sex = ch->pcdata->true_sex;
}

/*
 * Retrieve a character's age.
 */
int get_age( CHAR_DATA *ch )
{
    int starting,rate;
    long current;

    starting = race_table[ ch->race ][ ch->sex ].start_age;
    rate = UMAX( 1 , race_table[ ch->race ][ ch->sex ].year_per_hour * 18000 );
    current = current_time - ch->logon + ch->played;

    return UMIN( MAX_AGE, starting + ( current / rate ) );
}

/* command for retrieving stats */
int get_curr_stat( CHAR_DATA *ch, int pstat )
{
    return URANGE(3,ch->perm_stat[pstat] + ch->mod_stat[pstat], 25 );
}

/* command for returning max training score */
int get_max_train( CHAR_DATA *ch, int pstat )
{
    int max;

    if (IS_NPC(ch) || IS_IMMORTAL(ch))
	return 25;

    max = race_table[ch->race][ch->sex].max_stats[pstat];

    return UMIN(max,25);
}
   
/*
 * Retrieve a character's carry capacity.
 */
int can_carry_n( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && IS_IMMORTAL(ch) )
	return 1000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;

    return MAX_WEAR + ( ( get_curr_stat( ch, STAT_DEX ) + get_curr_stat( ch, STAT_STR ) ) / 2 );
}

/*
 * Retrieve a character's carry capacity.
 */
long can_carry_w( CHAR_DATA *ch )
{
    if ( !IS_NPC(ch) && IS_IMMORTAL(ch))
	return 1000000;

    if ( IS_NPC(ch) && IS_SET(ch->act, ACT_PET) )
	return 0;

    return str_app[get_curr_stat(ch,STAT_STR)].carry;
}

/*
 * See if a string is one of the names of an object.
 */
bool is_part_name( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
	if ( !strncmp( str, name, 3 ) )
	    return TRUE;
    }
}


/*
 * See if a string is one of the names of an object.
 */
bool is_name( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char *argument;

    if ( str == NULL || namelist == NULL ) return FALSE;

    strcpy(name,str);
    argument = one_argument( name, arg );

    if ( *argument != '\0' && !is_name( argument, namelist ) )
	return FALSE;

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
    	if ( !str_prefix( arg, name) )            /* str_cmp for full matching */
	    return TRUE;
    }
}
bool is_name_3( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char *argument;

    if ( str == NULL || namelist == NULL ) return FALSE;

    strcpy(name,str);
    argument = one_argument( name, arg );

    if ( *argument != '\0' && !is_name( argument, namelist ) )
	return FALSE;

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
    if ( !str_cmp( arg, name) )            /* str_cmp for full matching */
	    return TRUE;
    }
}

bool check_for_quests(TRIGGER_DATA *trig, CHAR_DATA *ch)
{
    CHAR_QUESTS * char_quests;
    CHAR_QUESTS * char_quests_pre;
    bool found = FALSE;
    bool quest_found = FALSE;

    if (IS_NPC(ch ) )
	return TRUE;  /* mobs do not get quests */

    for ( char_quests = ch->char_quests ; char_quests != NULL; char_quests = char_quests->next )
    {
	if ( char_quests->quest_number == trig->quests )
	{
	    quest_found = TRUE;
	    break; /* find the quest*/
	}
    }

    if ( trig->quests_pre != 0 )
    {
        for ( char_quests_pre = ch->char_quests ; char_quests_pre != NULL; char_quests_pre = char_quests_pre->next )
	    if ( char_quests_pre && char_quests_pre->quest_number == trig->quests_pre )
		if ( IS_SET(char_quests_pre->flags, QUEST_COMPLETED ) )
	{
	found = TRUE; /* Must have pre */  
	break;
	}
    
        if ( !found )
	    return FALSE;
    }
    if ( !quest_found && trig->step_pre != 0 )
	return FALSE;

    if ( quest_found && IS_SET( char_quests->flags, QUEST_COMPLETED ) )
	return FALSE; /* Char has completed quest */

    if ( quest_found && IS_SET( char_quests->flags, trig->step ) )
	return FALSE; /* They have completed this step */
    
    if ( quest_found && trig->step_pre != 0 )
    if ( !IS_SET( char_quests->flags, trig->step_pre ) )
	return FALSE;

    return TRUE;
   
}
bool can_do_trigger(TRIGGER_DATA *trig, CHAR_DATA *ch)
{

   if ( !check_for_quests( trig, ch ) )
	return FALSE;

   if (IS_NPC(ch) && IS_SET(trig->flags, TRIG_MOB) 
	&& ( ch->pIndexData->vnum == trig->mob_vnum || trig->mob_vnum == 0 ) )
	return TRUE;
   if (!IS_NPC(ch) && IS_SET(trig->flags, TRIG_PC) )
	return TRUE;
   return FALSE;

}

bool is_trig_keyword(TRIGGER_DATA *trig, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *key_words;
    char *arg1;

    if ( argument == NULL )
	return TRUE;

    if ( strstr( argument, trig->key_sim ) != NULL && (strlen(trig->key_sim) > 0) )
	return TRUE;

    key_words = trig->key_words;

    for ( ; ; )
    {
	key_words = one_argument(key_words, arg );
	if (arg[0] == '\0' )
	break;
	arg1 = argument;
	for ( ; ; )
	{
	    arg1 = one_argument(arg1, arg2);	

	    if ( arg2[0] == '\0' )
	    break;

	    if (!str_cmp( arg, arg2 ))
	    return TRUE;

	}
    }
/*
    if (strstr( trig->key_words, argument ) != NULL && strlen(trig->key_words) > 1) 
	return TRUE;
*/

    if (!str_cmp( argument, trig->key_string ) && strlen(trig->key_string) > 1) 
	return TRUE;

return FALSE;
}

/* used for allowing shortforms Ie look Princess = l pr */

bool is_name_prefix( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char *argument;

    if ( str == NULL || namelist == NULL ) return FALSE;

    strcpy(name,str);
    argument = one_argument( name, arg );

    if ( *argument != '\0' && !is_name_prefix( argument, namelist ) )
	return FALSE;

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
    if ( !str_prefix( arg, name ) )            
	    return TRUE;
    }
}

bool is_name_2( const char *str, char *namelist )
{
    char name[MAX_INPUT_LENGTH];

    if ( str == NULL || namelist == NULL ) return FALSE;

    for ( ; ; )
    {
	namelist = one_argument( namelist, name );
	if ( name[0] == '\0' )
	    return FALSE;
    if ( !str_cmp( str, name ) )
	    return TRUE;
    }
}

char *  cut_name ( char *str, char *namelist )
{   
    char name[MAX_INPUT_LENGTH], part[MAX_INPUT_LENGTH];
    char *list;
    char string[MAX_INPUT_LENGTH];

    string[0] = '\0';
    list = one_argument(namelist,part);
    one_argument(str,name);
    for ( ; part[0] != '\0' ; )
    {
	if ( str_cmp(part,name) )
	{
	    strcat(string,part);
	    strcat(string," ");
	}
	list = one_argument(list,part);	    
    }
    return str_dup(string);
}

/*
 * Apply or remove an affect to a character.
 */
void affect_modify( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd )
{
    OBJ_DATA *wield;
    int mod,i;

    mod = paf->modifier;
    if ( paf->type > 0 )
    {
    	if ( IS_SET(skill_table[paf->type].spell_flags, SPELL_PROTECTION ) )
    	{
    	    if (fAdd)
		SET_BIT( ch->spell_flags, SPELL_PROTECTION );
    	    else
		REMOVE_BIT( ch->spell_flags, SPELL_PROTECTION );
	}

    	if ( IS_SET(skill_table[paf->type].spell_flags, SPELL_SHIELD ) )
    	{
    	    if (fAdd)
		SET_BIT( ch->spell_flags, SPELL_SHIELD );
    	    else
		REMOVE_BIT( ch->spell_flags, SPELL_SHIELD );
        }
    }

    switch (paf->bit_type)
    {
	default:
	break;
	case BIT_AFFECT:
	    if (!IS_NPC(ch) )
	    {
    	    	if (IS_SET(ch->pcdata->perm_aff, paf->bitvector ) )
		    return;
	    }

	    if (fAdd)
		SET_BIT( ch->affected_by, paf->bitvector );
	    else
		REMOVE_BIT( ch->affected_by, paf->bitvector );
	break;
	case BIT_AFFECT2:
	    if (!IS_NPC(ch) )
	    {
    	    	if (IS_SET(ch->pcdata->perm_aff_2, paf->bitvector ) )
		    return;
	    }

	    if (fAdd)
		SET_BIT( ch->affected_by_2, paf->bitvector );
	    else
		REMOVE_BIT( ch->affected_by_2, paf->bitvector );
	break;
	case BIT_IMM:
	    if (fAdd)
		SET_BIT( ch->imm_flags, paf->bitvector );
	    else
	    {
		REMOVE_BIT( ch->imm_flags, paf->bitvector );
		ch->imm_flags |= ch->perm_imm;
	    }
	break;
	case BIT_RES:
	    if (fAdd)
		SET_BIT( ch->res_flags, paf->bitvector );
	    else
	    {
		REMOVE_BIT( ch->res_flags, paf->bitvector );
		ch->res_flags |= ch->perm_res;
	    }
	break;
	case BIT_VULN:
	    if (fAdd)
		SET_BIT( ch->vuln_flags, paf->bitvector );
	    else
	    {
		REMOVE_BIT( ch->vuln_flags, paf->bitvector );
		ch->vuln_flags |= ch->perm_vuln;
	    }
	break;
    }

    if (!fAdd)
	mod = 0 - mod;			    

    switch ( paf->location )
    {
    default:
	bug( "Affect_modify: unknown location %d.", paf->location );
	return;

    case APPLY_NONE:						break;
    case APPLY_STR:           ch->mod_stat[STAT_STR]	+= mod;	break;
    case APPLY_DEX:           ch->mod_stat[STAT_DEX]	+= mod;	break;
    case APPLY_INT:           ch->mod_stat[STAT_INT]	+= mod;	break;
    case APPLY_WIS:           ch->mod_stat[STAT_WIS]	+= mod;	break;
    case APPLY_CON:           ch->mod_stat[STAT_CON]	+= mod;	break;
    case APPLY_CHR:           ch->mod_stat[STAT_CHR]	+= mod;	break;
    case APPLY_SEX:           ch->sex			+= mod;	break;
    case APPLY_LEVEL:						break;
    case APPLY_AGE:						break;
    case APPLY_HEIGHT:						break;
    case APPLY_WEIGHT:						break;
    case APPLY_MANA:
        for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
          ch->max_mana[i]		+= mod;	
	break;
    case APPLY_MANA_FIRE:	ch->max_mana[ ELEMENT_FIRE ]	+= mod; break;
    case APPLY_MANA_WATER:	ch->max_mana[ ELEMENT_WATER ]	+= mod; break;
    case APPLY_MANA_EARTH:	ch->max_mana[ ELEMENT_EARTH ]	+= mod; break;
    case APPLY_MANA_WIND:	ch->max_mana[ ELEMENT_WIND ]	+= mod; break;
    case APPLY_MANA_SPIRIT:	ch->max_mana[ ELEMENT_SPIRIT ]	+= mod; break;
    case APPLY_HIT:           ch->max_hit		+= mod; break;
    case APPLY_MOVE:          ch->max_move		+= mod;	break;
    case APPLY_GOLD:						break;
    case APPLY_EXP:						break;
    case APPLY_AC:	      ch->armor			+= mod; break;
    case APPLY_HITROLL:       ch->hitroll		+= mod;	break;
    case APPLY_DAMROLL:       ch->damroll		+= mod;	break;
    case APPLY_SAVING_SPELL:  ch->saving_throw		+= mod;	break;
    case APPLY_DREAMS:	      
	if (IS_NPC(ch ) )
	break;
	ch->pcdata->dreams	+= mod; break;
    case APPLY_NIGHTMARES:   
	if (IS_NPC( ch) )
	break;
	ch->pcdata->nightmares    += mod; break;
    case APPLY_AFFECT:					        break;
    }

    if (ch->in_room != NULL) /* Char could be Quiting at this point */
    if ( !fAdd && IS_SET( paf->bitvector , AFF_BREATHE_UNDERWATER ) && IS_UNDERWATER( ch ) && !IS_IMMORTAL( ch ) )
    {
	send_to_char("You plug your nose and hold your breath.\n\r", ch );
	do_hold_breath( ch, "" );
    }

    if ( fAdd && IS_SET( paf->bitvector, AFF_BREATHE_UNDERWATER ) && IS_SET( ch->affected_by, AFF_HOLD_BREATH ) )
	affect_strip( ch, gsn_hold_breath );

    if ( ch->in_room )
    if ( !IS_UNDERWATER( ch ) && IS_SET( ch->affected_by, AFF_HOLD_BREATH ) )
	affect_strip( ch, gsn_hold_breath );

    /*
     * Check for weapon wielding.
     * Guard against recursion (for weapons with affects).
     */
    if ( !IS_NPC(ch) && ( ( wield = get_eq_char( ch, WEAR_WIELD ) ) != NULL || ( wield = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) != NULL )
    &&   get_obj_weight(wield) > str_app[get_curr_stat(ch,STAT_STR)].wield )
    {
	static int depth;

	if ( depth == 0 )
	{
	    depth++;
	    act( "You drop $p.", ch, wield, NULL, TO_CHAR );
	    act( "$n drops $p.", ch, wield, NULL, TO_ROOM );
	    obj_from_char( wield, "Q" );
	    obj_to_room( wield, ch->in_room );
	    depth--;
	}
    }
    return;
}

/*
 * Give an affect to a char.
 */
void affect_to_char( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    paf_new		= new_affect(TRUE);

    *paf_new		= *paf;
    paf_new->next	= ch->affected;
    ch->affected	= paf_new;

    affect_modify( ch, paf_new, TRUE );
    return;
}

/* give an affect to an object */
void affect_to_obj(OBJ_DATA *obj, AFFECT_DATA *paf)
{
    AFFECT_DATA *paf_new;

    if ( obj->wear_data == NULL )
	return;

	paf_new	= new_affect(FALSE);

    *paf_new		= *paf;
    paf_new->next	= obj->wear_data->affected;
    obj->wear_data->affected	= paf_new;

    return;
}



/*
 * Remove an affect from a char.
 */
void affect_remove( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    if ( ch->affected == NULL )
    {
	bug( "Affect_remove: no affect.", 0 );
	return;
    }

    affect_modify( ch, paf, FALSE );

    if ( paf == ch->affected )
    {
	ch->affected	= paf->next;
    }
    else
    {
	AFFECT_DATA *prev;

	for ( prev = ch->affected; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == paf )
	    {
		prev->next = paf->next;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Affect_remove: cannot find paf.", 0 );
	    return;
	}
    }

    free_affect(paf);
    return;
}

bool affect_modify_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf, bool fAdd )
{
     int mod;

     if ( paf == NULL )
	return FALSE;

     mod = paf->modifier;

     if ( !fAdd )
     {
	if ( mod != 0 )
	    mod *= -1;
	else
	    mod = -1;
     }

     switch ( paf->location )
     {
     default:
     case APPLY_NONE:
	break;
     case APPLY_ROOM_FLAGS:
	if ( fAdd
	  && ( ( mod < 0 && !IS_SET( room->room_flags, paf->bitvector ) )
	    || ( mod >= 0 && IS_SET( room->room_flags, paf->bitvector ) ) ) )
	    return FALSE;

	if ( mod < 0 )
	    REMOVE_BIT( room->room_flags, paf->bitvector );
	else
	    SET_BIT( room->room_flags, paf->bitvector );

	break;
     case APPLY_SECTOR_TYPE:
	break;
     }

    return TRUE;
}

void affect_apply_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_new;

    if ( !affect_modify_room( room, paf, TRUE ) )
	return;

    paf_new = new_affect(TRUE); 

    *paf_new		= *paf;
    paf_new->next	= room->affected_by;
    room->affected_by	= paf_new;
    return;
}

void affect_remove_room( ROOM_INDEX_DATA *room, AFFECT_DATA *paf )
{
    if ( room->affected_by == NULL )
    {
	bug( "Affect_remove_room: no affect.", 0 );
	return;
    }

    if ( room->affected_by == paf )
    {
	room->affected_by = paf->next;
    }
    else
    {
        AFFECT_DATA *prev;

        for ( prev = room->affected_by; prev != NULL; prev = prev->next )
        {
            if ( prev->next == paf )
            {
                prev->next = paf->next;
                break;
            }
        }

        if ( prev == NULL )
        {
            bug( "Affect_remove_room: cannot find paf.", 0 );
            return;
        }
    }

    affect_modify_room( room, paf, FALSE );

    paf->next   = affect_free;
    affect_free = paf;
    return;
}

void affect_remove_obj( OBJ_DATA *obj, AFFECT_DATA *paf)
{
    if ( obj->wear_data == NULL || obj->wear_data->affected == NULL )
    {
        bug( "Affect_remove_object: no affect.", 0 );
        return;
    }

    if (obj->carried_by != NULL && obj->wear_data != NULL && obj->wear_data->wear_loc != -1)
	affect_modify( obj->carried_by, paf, FALSE );

    if ( paf == obj->wear_data->affected )
    {
        obj->wear_data->affected    = paf->next;
    }
    else
    {
        AFFECT_DATA *prev;

        for ( prev = obj->wear_data->affected; prev != NULL; prev = prev->next )
        {
            if ( prev->next == paf )
            {
                prev->next = paf->next;
                break;
            }
        }

        if ( prev == NULL )
        {
            bug( "Affect_remove_object: cannot find paf.", 0 );
            return;
        }
    }

    paf->next   = affect_free;
    affect_free = paf;
    return;
}



/*
 * Strip all affects of a given sn.
 */
void affect_strip( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    for ( paf = ch->affected; paf != NULL; paf = paf_next )
    {
	paf_next = paf->next;
	if ( paf->type == sn )
	    affect_remove( ch, paf );
    }

    return;
}



/*
 * Return true if a char is affected by a spell.
 */
bool is_affected( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}

/* checks to see if they have the spell bit already also */
bool is_affected_bit( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    if ( IS_SET( ch->affected_by, spell_affect_bit( sn ) ) )
	return TRUE;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}

/* checks to see if they have the spell bit already also */
bool is_affected_bit_2( CHAR_DATA *ch, int sn )
{
    AFFECT_DATA *paf;

    if ( IS_SET( ch->affected_by_2, spell_affect_bit_2( sn ) ) )
	return TRUE;

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->type == sn )
	    return TRUE;
    }

    return FALSE;
}



/*
 * Add or enhance an affect.
 */
void affect_join( CHAR_DATA *ch, AFFECT_DATA *paf )
{
    AFFECT_DATA *paf_old;
    bool found;

    found = FALSE;
    for ( paf_old = ch->affected; paf_old != NULL; paf_old = paf_old->next )
    {
	if ( paf_old->type == paf->type )
	{
	    paf->level = ( paf->level += paf_old->level) / 2;
	    paf->duration += paf_old->duration;
	    paf->modifier += paf_old->modifier;
	    affect_remove( ch, paf_old );
	    break;
	}
    }

    affect_to_char( ch, paf );
    return;
}



/*
* fn to get total light value of  chars equip
*/

int get_eq_light(CHAR_DATA *ch)
{
  int total=0 ;  
  OBJ_DATA *obj, *next_obj ;
  
  for (obj=ch->carrying; obj!=NULL;obj=next_obj)
  {
    next_obj=obj->next_content ;
          
    if (obj->wear_data == NULL
      || obj->light_data == NULL
      || obj->wear_data->wear_loc == WEAR_NONE)
	continue;

    if ( IS_SET( obj->light_data->flags, LIGHT_LIT ) )
      total += obj->light_data->brightness;
  }
  return total ;
}                          


int get_eq_magic_light(CHAR_DATA *ch)
{
  int total=0 ;  
  OBJ_DATA *obj, *next_obj ;
  
  for (obj=ch->carrying; obj!=NULL;obj=next_obj)
  {
    next_obj = obj->next_content;
    if (obj->wear_data == NULL
      || obj->light_data == NULL
      || obj->wear_data->wear_loc == WEAR_NONE)
	continue;

    if ( IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) )
      total += obj->light_data->brightness;
  }
  return total ;
}                          

/*
 * Move a char out of a room.
 */
void char_from_room( CHAR_DATA *ch )
{

    if ( ch->in_room == NULL )
    {
	bug( "Char_from_room: NULL.", 0 );
	return;
    }

    if (ch->in_room != NULL) ch->from_room = ch->in_room->vnum;

    if ( !IS_NPC(ch) && !IS_SET( ch->plr, PLR_WIZINVIS ) )
	--ch->in_room->area->nplayer;

    ch->in_room->light_adjustment = get_light_adjustment ( ch, ch->in_room );

    if ( ch == ch->in_room->people )
    {
	ch->in_room->people = ch->next_in_room;
    }
    else
    {
	CHAR_DATA *prev;

	for ( prev = ch->in_room->people; prev; prev = prev->next_in_room )
	{
	    if ( prev->next_in_room == ch )
	    {
		prev->next_in_room = ch->next_in_room;
		break;
	    }
	}
	if ( prev == NULL )
	    bug( "Char_from_room: ch not found.", 0 );
    }

    ch->in_room      = NULL;
    ch->next_in_room = NULL;
    return;
}

ROOM_INDEX_DATA * gate_leads_to( GATE_DATA * gate )
{
    CHAR_DATA * ch;
    OBJ_DATA * obj;
    ROOM_INDEX_DATA * room;

    if ( gate == NULL )
	return NULL;

    if ( IS_SET( gate->flags, GATE_UNSTABLE ) )
    {
	for ( room = get_room_index( number_range( 0, top_room_index ) );
	    room != NULL ;
	    room = get_room_index( number_range( 0, top_room_index ) ) );

	return room;
    }

    if ( IS_SET( gate->flags, GATE_TO_OBJ ) )
    {
        for ( obj = object_list ; obj != NULL && obj != (OBJ_DATA *) gate->leads_to ; obj = obj->next );

	if ( obj == NULL )
	{
	    for ( obj = object_list ; obj != NULL && obj->pIndexData->vnum != gate->vnum ; obj = obj->next );

	    if ( obj == NULL )
	   	return NULL;
	    else
	    {
	        gate->leads_to = (void *) obj;
	        return get_in_room_obj ( obj );
	    }
	}
	else
	    return get_in_room_obj( obj );
    }
    else if ( IS_SET( gate->flags, GATE_TO_MOB ) )
    {
        for ( ch = char_list ; ch != NULL && ch != (CHAR_DATA *) gate->leads_to ; ch = ch->next );

	if ( ch == NULL )
	{
	    for ( ch = char_list ; ch != NULL && (!IS_NPC( ch ) || ch->pIndexData->vnum != gate->vnum ) ; ch = ch->next );

	    if ( ch == NULL )
	   	return NULL;
	    else
	    {
	        gate->leads_to = (void *) ch;
	        return ch->in_room;
	    }
	}
	else
	    return ch->in_room;
    }
    else
    {
	if ( gate->leads_to == NULL )
	{
	    gate->leads_to = (void *) get_room_index( gate->vnum );
	    return (ROOM_INDEX_DATA *) gate->leads_to;
	}
	else
	    return (ROOM_INDEX_DATA *) gate->leads_to;
    }
}


void char_to_obj (CHAR_DATA *ch, OBJ_DATA *obj)
{
  ROOM_INDEX_DATA * to_room;

  if (IS_PCG(obj))
    { 
        to_room = gate_leads_to( obj->gate_data );

   	if ( to_room == NULL )
	{
	    bug( "Bad to_room: char_to_obj",0);
	    return;
	}

      char_from_room(ch);
      char_to_room (ch,to_room);
      return;
    }
    else
    { bug("CHAR_TO_OBJ:: obj is not a player container or gate!!!",0);
      return;
    }
}

/*
 * Move a char into a room.
 */
void char_to_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
     
    if ( pRoomIndex == NULL )
    {
	bug( "Char_to_room: NULL.", 0 );
	return;
    }

    ch->in_room		= pRoomIndex;
    ch->next_in_room	= pRoomIndex->people;
    pRoomIndex->people	= ch;

    if ( !IS_NPC(ch) && !IS_SET( ch->plr, PLR_WIZINVIS ) )
    {
	if ( ch->in_room->area->age < 0 )
            ch->in_room->area->age = 0;

	++ch->in_room->area->nplayer;
    }

    ch->in_room->light_adjustment = get_light_adjustment ( ch, pRoomIndex );
  
    if ( !IS_IMMORTAL(ch) && !IS_AFFECTED( ch, AFF_HOLD_BREATH) 
        && !IS_AFFECTED( ch, AFF_BREATHE_UNDERWATER ) 
        && IS_UNDERWATER( ch ) )
    {	
	send_to_char("You gasp for air as you go underwater!\n\r",ch);
	do_hold_breath( ch, "" );
    }

    if ( IS_AFFECTED( ch, AFF_PLAGUE ) )
    {
	static AFFECT_DATA aff_zero;
        AFFECT_DATA *af, plague;
        CHAR_DATA *vch;
        int save;
        
        for ( af = ch->affected; af != NULL; af = af->next )
        {
            if ( af->type == gsn_plague )
                break;
        }
        
        if ( af == NULL )
        {
            REMOVE_BIT( ch->affected_by, AFF_PLAGUE );
            return;
        }
        
        if ( af->level == 1 )
            return;

        plague			= aff_zero;
        plague.type 		= gsn_plague;
        plague.level 		= af->level - 1; 
        plague.duration 	= number_range( 1, 2 * plague.level );
        plague.location		= APPLY_STR;
        plague.modifier 	= -5;
        plague.bitvector 	= AFF_PLAGUE;
        
        for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room )
        {
            switch( check_immune( vch, DAM_DISEASE ) )
            {
            	case( IS_NORMAL ) 	: save = af->level - 4;	break;
            	case( IS_IMMUNE ) 	: save = 0;		break;
            	case( IS_RESISTANT )  	: save = af->level - 8;	break;
            	case( IS_VULNERABLE )	: save = af->level; 	break;
            	default			: save = af->level - 4;	break;
            }
            
            if ( save != 0 && !saves_spell( save, vch ) && !IS_IMMORTAL( vch ) &&
            	!IS_AFFECTED( vch, AFF_PLAGUE ) && number_bits( 6 ) == 0 )
            {
            	send_to_char( "You feel hot and feverish.\n\r", vch );
            	act( "$n shivers and looks very ill.", vch, NULL, NULL, TO_ROOM );
            	affect_join( vch, &plague );
            }
        }
    }

    return;
}

void add_to_shop( OBJ_DATA * obj, CHAR_DATA * ch )
{
    OBJ_DATA * obj2;

    for ( obj2 = ch->carrying ; obj2 != NULL ; obj2 = obj2->next_content )
    {
	if ( obj2->wear_data->wear_loc == WEAR_NONE && !str_cmp(obj2->short_descr, obj->short_descr ) )
	{
	    obj->next_content	= obj2->next_content;
	    obj2->next_content	= obj;
	    obj->carried_by	= ch;
	    obj->in_room	= NULL;
	    obj->in_obj		= NULL;
	    ch->carry_number++;
	    ch->carry_weight	+= get_obj_weight( obj );
	    return;
	}
    }
    obj_to_char( obj, ch );
}

/*
 * Give an obj to a char.
 */
void obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch )
{
    obj->next_content	 = ch->carrying;
    ch->carrying	 = obj;
    obj->carried_by	 = ch;
    obj->in_room	 = NULL;
    obj->in_obj		 = NULL;
    ch->carry_number++;
    ch->carry_weight	+= get_obj_weight( obj );
}

/*
 * Take an obj from its character.
 */
void obj_from_char( OBJ_DATA *obj, char *argument )
{
    CHAR_DATA *ch;
    char buf[MAX_INPUT_LENGTH];

    if ( ( ch = obj->carried_by ) == NULL )
    {
	sprintf(buf,"Obj_from_char: null ch at -> %s",argument);
	bug( buf, 0 );
	return;
    }

    if ( obj->wear_data != NULL && obj->wear_data->wear_loc != WEAR_NONE )
	unequip_char( ch, obj );

    if ( ch->carrying == obj )
    {
	ch->carrying = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = ch->carrying; prev != NULL; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	    bug( "Obj_from_char: obj not in list.", 0 );
    }
    obj->carried_by	 = NULL;
    obj->next_content	 = NULL;
    ch->carry_number--;
    ch->carry_weight	-= get_obj_weight( obj );
    /*
     * The equip_mob may now load a new one
     */
    obj->equip = NULL;
    return;
}


/*
 * Find a piece of eq on a character.
 */
OBJ_DATA *get_eq_char( CHAR_DATA *ch, int iWear )
{
    OBJ_DATA *obj;

    if (ch == NULL)
	return NULL;

    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_data != NULL && obj->wear_data->wear_loc == iWear )
	    return obj;
    }

    return NULL;
}



/*
 * Equip a char with an obj.
 */
void equip_char( CHAR_DATA *ch, OBJ_DATA *obj, int iWear )
{
    AFFECT_DATA *paf;
    char buf[MAX_INPUT_LENGTH];
    bool trig;
  

    if ( get_eq_char( ch, iWear ) != NULL )
    {
	/* Its ok for a npc as they may have more then 1 chance at
	   loading the same type of equipment G*/

	if (!IS_NPC( ch)  )
	{
	    sprintf( buf, "Equip_char: (%s) already equipped (%d).", ch->name, iWear );
	    bug( buf, iWear );
	}
	return;
    }

    if ( ( IS_SET( obj->extra_flags, ITEM_BARDING ) && !IS_SET( ch->act, ACT_MOUNT ) )
      || ( !IS_SET( obj->extra_flags, ITEM_BARDING ) &&  IS_SET( ch->act, ACT_MOUNT ) ) )
    {
	cact( "You can't wear $p`w!", ch, obj, NULL, TO_CHAR, "`W" );
	cact( "$n tries to put on $p`w.",   ch, obj, NULL, TO_ROOM, "`W" );
	return;
    }

   if ( obj->wear_data == NULL 
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_NENKEMEN)  && race_table[ ch->race ][ ch->sex ].world == 0 )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_MAEGMENEL)  && race_table[ ch->race ][ ch->sex ].world == 1 )
    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_LITHDOR)  && race_table[ ch->race ][ ch->sex ].world == 2 ) )
    {
	cact( "You are zapped by $p`w.", ch, obj, NULL, TO_CHAR, "`W" );
	cact( "$n is zapped by $p`w.",   ch, obj, NULL, TO_ROOM, "`W" );
	obj_from_char( obj, "R" );
	obj_to_room( obj, ch->in_room );
	return;
    }

    if ( ( iWear == WEAR_HOLD || iWear == WEAR_WIELD_R || iWear == WEAR_SHIELD || iWear == WEAR_WIELD_L )
      && !IS_SET( obj->extra_flags, ITEM_BUCKLER ) )
	ch->hands_free--;
    else if ( iWear == WEAR_WIELD_TWO_HANDED )
	ch->hands_free -= 2;

    ch->armor      	-= obj->wear_data->ac;
    obj->wear_data->wear_loc	 = iWear;

    	for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
	    affect_modify( ch, paf, TRUE );

	if ( obj->light_data != NULL && ch->in_room != NULL )
	    if ( ( IS_SET( obj->light_data->flags, LIGHT_LIT ) && !IS_SET( ch->in_room->room_flags, ROOM_DARK ) )
		|| IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) )
		ch->in_room->light_adjustment += obj->light_data->brightness;
	trig = do_trigger( ch->in_room, ch, obj, NULL, TRIG_WEAR, TRIG_TYPE_ROOM);
	trig = do_trigger( obj, ch, obj, NULL, TRIG_WEAR, TRIG_TYPE_OBJ);
	trig = do_trigger( ch, ch, obj, NULL, TRIG_WEAR, TRIG_TYPE_MOB);
    return;
}

/*
 * Unequip a char with an obj.
 */
void unequip_char( CHAR_DATA *ch, OBJ_DATA *obj )
{
    AFFECT_DATA *paf;
    OBJ_DATA *weapon;

    if ( obj->wear_data == NULL || obj->wear_data->wear_loc == WEAR_NONE )
    {
	bug( "Unequip_char: already unequipped.", 0 );
	return;
    }

    if ( ( obj->wear_data->wear_loc == WEAR_HOLD
      || obj->wear_data->wear_loc == WEAR_SHIELD || obj->wear_data->wear_loc == WEAR_WIELD_L 
      || obj->wear_data->wear_loc == WEAR_WIELD_R ) && !IS_SET( obj->extra_flags, ITEM_BUCKLER ) )
    {
	if ( ( weapon = get_eq_char( ch, WEAR_WIELD ) ) != NULL 
	  && is_bastard( ch, weapon ) && weapon != obj )
	{
	    weapon->wear_data->wear_loc = WEAR_WIELD_TWO_HANDED;
	    act( "$n wields $p in two hands.", ch, weapon, NULL, TO_ROOM );
 	    act( "You wield $p in two hands.", ch, weapon, NULL, TO_CHAR );
	}
	else
	    ch->hands_free = UMIN( 2, ch->hands_free + 1 );
    }
    else if ( obj->wear_data->wear_loc == WEAR_WIELD_TWO_HANDED )
	ch->hands_free = UMIN( 2, ch->hands_free + 2 );

    ch->armor	+= obj->wear_data->ac;
    obj->wear_data->wear_loc	 = -1;

    	for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
	    affect_modify( ch, paf, FALSE );

	if ( obj->light_data != NULL && ch->in_room != NULL )
	    if ( ( IS_SET( obj->light_data->flags, LIGHT_LIT ) && !IS_SET( ch->in_room->room_flags, ROOM_DARK ) )
		|| IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) )
		ch->in_room->light_adjustment -= obj->light_data->brightness;

    return;
}



/*
 * Count occurrences of an obj in a list.
 */
int count_obj_list( OBJ_INDEX_DATA *pObjIndex, OBJ_DATA *list )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}

/* counts the number of an object in the mud */

int count_obj( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;
    int nMatch;

    nMatch = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    nMatch++;
    }

    return nMatch;
}



/*
 * Move an obj out of a room.
 */
void obj_from_room( OBJ_DATA *obj )
{
    ROOM_INDEX_DATA *in_room;

    if ( ( in_room = obj->in_room ) == NULL )
    {
	bug( "obj_from_room: NULL.", 0 );
	return;
    }

    if ( obj == in_room->contents )
    {
	in_room->contents = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = in_room->contents; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_room: obj not found.", 0 );
	    return;
	}
    }

	if ( obj->light_data != NULL && obj->in_room != NULL )
	    if ( ( IS_SET( obj->light_data->flags, LIGHT_LIT ) && !IS_SET( obj->in_room->room_flags, ROOM_DARK ) )
		|| IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) )
		obj->in_room->light -= obj->light_data->brightness;

    obj->in_room      = NULL;
    obj->next_content = NULL;
    /*
     * The event may now happen again
     */
    obj->event = NULL;
    if ( IS_SET( in_room->room_flags_2, ROOM_SAVE )  )
	save_perm_rooms( in_room );
    return;
}



/*
 * Move an obj into a room.
 */
void obj_to_room( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj1_next;
    CHAR_DATA *victim;
    int count = 0;

    for ( obj1 = pRoomIndex->contents ; obj1 != NULL ; obj1 = obj1_next )
    {
    	obj1_next = obj1->next_content;
    	count++;

   	if ( count >= 200 )
    	{
	    sprintf( buf, "`W%s is lost in the clutter of the room.\n\r", obj->short_descr );
	    send_to_room( pRoomIndex->vnum, buf );
	    extract_obj( obj );
	    return;
        }
    }

    if ( IS_SET( pRoomIndex->room_flags, ROOM_BOTTOMLESS ) && 
	 !IS_SET( obj->extra_flags, ITEM_FLOAT ) )
    {
	if ( pRoomIndex->exit[ DIR_DOWN ] == NULL )
	{
	    sprintf( buf, "`W%s falls down, and disappears.\n\r", obj->short_descr );
	    for ( victim = pRoomIndex->people ; victim != NULL ; victim = victim->next_in_room )
	    {
		if ( victim->position < POS_RESTING )
		    continue;
	
		if ( !can_see_obj( victim, obj ) )
		    send_to_char("Something falls down, and disappears.\n\r", victim );
		else
		    send_to_char( buf, victim );

	    }
	    extract_obj( obj );
	    return;
	}
	else if ( pRoomIndex->exit[ DIR_DOWN ]->u1.to_room != NULL
	          && !IS_SET( pRoomIndex->exit[ DIR_DOWN ]->exit_info, EX_CLOSED ) )
	{
	    sprintf( buf, "`W%s falls downward.\n\r", obj->short_descr );
	    for ( victim = pRoomIndex->people ; victim != NULL ; victim = victim->next_in_room )
	    {
		if ( victim->position < POS_RESTING )
		    continue;
	
		if ( !can_see_obj( victim, obj ) )
		    send_to_char("Something falls downward.\n\r", victim );
		else
		    send_to_char( buf, victim );

	    }
	    sprintf( buf, "`W%s falls into the room from above.\n\r", obj->short_descr );
	    for ( victim = pRoomIndex->exit[ DIR_DOWN ]->u1.to_room->people ; victim != NULL ; victim = victim->next_in_room )
	    {
		if ( victim->position < POS_RESTING )
		    continue;
	
		if ( !can_see_obj( victim, obj ) )
		    send_to_char("Something falls into the room from above.\n\r", victim );
		else
		    send_to_char( buf, victim );

	    }
	    obj_to_room( obj, pRoomIndex->exit[ DIR_DOWN ]->u1.to_room );
	    return;
	}
    }

    obj->next_content		= pRoomIndex->contents;
    pRoomIndex->contents	= obj;
    obj->in_room		= pRoomIndex;
    obj->carried_by		= NULL;
    obj->in_obj			= NULL;

	if ( obj->light_data != NULL && pRoomIndex != NULL )
	    if ( ( IS_SET( obj->light_data->flags, LIGHT_LIT ) && !IS_SET( pRoomIndex->room_flags, ROOM_DARK ) )
		|| IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) )
		pRoomIndex->light += obj->light_data->brightness;

    if (!fBootHomes) /* If booting don't save the objects in the rooms */
    if ( IS_SET( pRoomIndex->room_flags_2, ROOM_SAVE )  )
	save_perm_rooms( pRoomIndex ); 
    return;
}

/*
 * Move an object into an object.
 */
void obj_to_obj( OBJ_DATA *obj, OBJ_DATA *obj_to )
{
    OBJ_DATA *in_obj;

    if (obj_to == NULL)
    {
	bug("Obj to NULL\n\r",0);
	return;
    }

    if (obj_to->cont_data == NULL)
    {
	bug("Obj to not container\n\r",0);
	return;
    }

    for ( in_obj = obj_to ; in_obj->in_obj != NULL ; in_obj = in_obj->in_obj );

    if ( in_obj->carried_by != NULL )
	    in_obj->carried_by->carry_weight -= get_obj_weight( in_obj );

    obj->next_content		= obj_to->cont_data->contents;
    obj_to->cont_data->contents	= obj;
    obj->in_obj			= obj_to;
    obj->in_room		= NULL;
    obj->carried_by		= NULL;

    if ( IS_SET( obj_to->cont_data->flags, CONT_DEVOURING ) )
	obj->timer = ( obj->timer < 1 ? number_fuzzy( 5 ) : UMIN( obj->timer, number_fuzzy( 5 ) ) );

    if ( in_obj->carried_by != NULL )
	    in_obj->carried_by->carry_weight += get_obj_weight( in_obj );
   
    if ( obj_to->in_room != NULL )
    {
    	if (!fBootHomes)
    	if ( IS_SET( in_obj->in_room->room_flags_2, ROOM_SAVE )  )
	    save_perm_rooms( in_obj->in_room ); 
    } 
    return;
}



/*
 * Move an object out of an object.
 */
void obj_from_obj( OBJ_DATA *obj )
{
    OBJ_DATA *obj_from;
    OBJ_DATA *in_obj;

    if ( ( obj_from = obj->in_obj ) == NULL )
    {
	bug( "Obj_from_obj: null obj_from.", 0 );
	return;
    }
    if ( obj_from->cont_data == NULL )    
    {
	bug( "Obj_from_obj: obj_from not a container.", 0 );
	return;
    }

    for ( in_obj = obj_from ; in_obj->in_obj != NULL ; in_obj = in_obj->in_obj );

    if ( in_obj->carried_by != NULL )
        in_obj->carried_by->carry_weight -= get_obj_weight( in_obj );

    if ( obj == obj_from->cont_data->contents )
    {
	obj_from->cont_data->contents = obj->next_content;
    }
    else
    {
	OBJ_DATA *prev;

	for ( prev = obj_from->cont_data->contents; prev; prev = prev->next_content )
	{
	    if ( prev->next_content == obj )
	    {
		prev->next_content = obj->next_content;
		break;
	    }
	}

	if ( prev == NULL )
	{
	    bug( "Obj_from_obj: obj not found.", 0 );
	    return;
	}
    }

    obj->next_content = NULL;
    obj->in_obj       = NULL;

    if ( in_obj->carried_by != NULL )
        in_obj->carried_by->carry_weight += get_obj_weight( in_obj );

    return;
}

bool obj_vnum_in_room( ROOM_INDEX_DATA *in_room, long vnum )
{
    OBJ_DATA *obj;

    if ( in_room == NULL )
	return FALSE;

    for ( obj = in_room->contents ; obj != NULL ; obj = obj->next_content )
	if ( obj->pIndexData->vnum == vnum )
	    return  TRUE;

    return FALSE;
}

bool has_obj_vnum( CHAR_DATA *ch, long vnum )
{
    OBJ_DATA *obj;

    for ( obj = ch->carrying ; obj != NULL ; obj = obj->next_content )
	if ( obj->pIndexData->vnum == vnum )
	    return  TRUE;

    return FALSE;
}

/*
 * Find a char in the room.
 */
CHAR_DATA *get_char_room( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *rch;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    if ( !str_cmp( arg, "self" ) )
	return ch;
    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( !can_see( ch, rch ) || !is_name( arg, rch->name ) )
	    continue;
	if ( ++count == number )
	    return rch;
    }

    return NULL;
}




/*
 * Find a char in the world.
 */
CHAR_DATA *get_char_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *wch;
    int number;
    int count;

    if ( argument == NULL )
	return NULL;

    if ( ( wch = get_char_room( ch, argument ) ) != NULL )
	return wch;

    number = number_argument( argument, arg );
    count  = 0;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( wch->in_room == NULL || !can_see( ch, wch ) 
	||   !is_name( arg, wch->name ) )
	    continue;
	if ( ++count == number )
	    return wch;
    }
    for ( wch = char_list; wch != NULL ; wch = wch->next )
    {
	if ( wch->in_room == NULL || !can_see( ch, wch ) 
	||   !is_name( arg, wch->short_descr ) )
	    continue;
	if ( ++count == number )
	    return wch;
    }

    return NULL;
}



/*
 * Find some object with a given index data.
 * Used by area-reset 'P' command.
 */
OBJ_DATA *get_obj_type( OBJ_INDEX_DATA *pObjIndex )
{
    OBJ_DATA *obj;

    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( obj->pIndexData == pObjIndex )
	    return obj;
    }

    return NULL;
}


/*
 * Find an obj in a list.
 */
OBJ_DATA *get_obj_list( CHAR_DATA *ch, char *argument, OBJ_DATA *list )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's inventory.
 */
OBJ_DATA *get_obj_carry_vnum ( CHAR_DATA *ch, long arg )
{
  OBJ_DATA *obj;

  for (obj = ch->carrying; obj != NULL; obj = obj->next_content )
  {
   if ((can_see_obj (ch, obj))  
   &&  (arg == obj->pIndexData->vnum))
       { return obj;}
  }
 return NULL;
}
 
OBJ_DATA *get_obj_carry( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE
	&&   (can_see_obj( ch, obj ) ) 
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in player's equipment.
 */
OBJ_DATA *get_obj_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_data != NULL && obj->wear_data->wear_loc != WEAR_NONE
	&&   can_see_obj( ch, obj )
	&&   is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}



/*
 * Find an obj in the room or in inventory.
 */
OBJ_DATA *get_obj_here( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;

    obj = get_obj_list( ch, argument, ch->in_room->contents );
    if ( obj != NULL )
	return obj;

    if ( ( obj = get_obj_carry( ch, argument ) ) != NULL )
	return obj;

    if ( ( obj = get_obj_wear( ch, argument ) ) != NULL )
	return obj;

    return NULL;
}



/*
 * Find an obj in the world.
 */
OBJ_DATA *get_obj_world( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int number;
    int count;

    if ( ( obj = get_obj_here( ch, argument ) ) != NULL )
	return obj;

    number = number_argument( argument, arg );
    count  = 0;
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( can_see_obj( ch, obj ) && is_name( arg, obj->name ) )
	{
	    if ( ++count == number )
		return obj;
	}
    }

    return NULL;
}

/*
 * Create a 'money' obj.
 */
OBJ_DATA *create_money( int amount, int coin_type )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;

    if ( amount <= 0 )
    {
	bug( "Create_money: zero or negative money %d.", amount );
	amount = 1;
    }

    if ( amount == 1 )
    {
	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_ONE ) );
	sprintf( buf, "`Ya %s", coin_name[ coin_type ] );
	strncpy( obj->short_descr, buf, 80 );
	sprintf( buf, "`Ya single %s", coin_name[ coin_type ] );
	strncpy( obj->description, buf, 80 );
	sprintf( buf, "`Ycoin %s", coin_name[ coin_type ] );
	strncpy( obj->name, buf, 30 );
	obj->wear_data->cost = 1;
	REMOVE_BIT( obj->wear_data->wear_flags,ITEM_MONEY_0|ITEM_MONEY_1|ITEM_MONEY_2|ITEM_MONEY_3 );
	switch (coin_type)
	{
	case 0:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_0 );
	    break;
	case 1:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_1 );
	    break;
	case 2:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_2 );
	    break;
	case 3:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_3 );
	    break;
	}
    }
    else
    {
	obj = create_object( get_obj_index( OBJ_VNUM_MONEY_SOME ) );
	sprintf( buf, "`Y%d %s", amount, coin_names[ coin_type ] );
	strncpy( obj->short_descr, buf, 80 );
	sprintf( buf, "`YSome %s.\n\r", coin_names[ coin_type ] );
	strncpy( obj->description, buf, 80 );
	sprintf( buf, "coins %s\n\r", coin_names[ coin_type ] );
	strncpy( obj->name, buf, 30 );
	obj->wear_data->cost = amount;
	REMOVE_BIT( obj->wear_data->wear_flags,ITEM_MONEY_0|ITEM_MONEY_1|ITEM_MONEY_2|ITEM_MONEY_3 );
	switch (coin_type)
	{
	case 0:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_0 );
	    break;
	case 1:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_1 );
	    break;
	case 2:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_2 );
	    break;
	case 3:
	    SET_BIT( obj->wear_data->wear_flags, ITEM_MONEY_3 );
	    break;
	}
    }

    return obj;
}

/*
 * Return weight of an object, including weight of contents.
 */
int get_obj_weight( OBJ_DATA *obj )
{
    int weight;
    int cweight=0;

    if ( obj->wear_data == NULL )
	return 0;

    weight = obj->wear_data->weight;
    if ( obj->cont_data != NULL )
    {
	OBJ_DATA * content;

        for ( content = obj->cont_data->contents; content != NULL; content = content->next_content )
	    cweight += get_obj_weight( content );

	if ( IS_SET( obj->cont_data->flags, CONT_HOLDING ) )
	    cweight /= 5;

	if ( IS_SET( obj->cont_data->flags, CONT_EXTRA_HOLDING ) )
	    cweight /= 10;

	if ( IS_SET( obj->cont_data->flags, CONT_SUPER_HOLDING ) )
	    cweight /= 15;

	weight += cweight;
    }

    return weight;
}

ROOM_INDEX_DATA * get_in_room_obj( OBJ_DATA *obj )
{
    if ( obj == NULL )
	return NULL;

    if ( obj->carried_by != NULL )
	return obj->carried_by->in_room;

    if ( obj->in_room != NULL )
	return obj->in_room;

    if ( obj->in_obj != NULL )
	return get_in_room_obj( obj->in_obj );

    return NULL;
}

CHAR_DATA * get_obj_carried_by( OBJ_DATA *obj )
{
    if ( obj->carried_by != NULL )
	return obj->carried_by;

    if ( obj->in_obj != NULL )
	return get_obj_carried_by( obj->in_obj );

    return NULL;
}

/*
* returns room light (forces a maximum of9)
*/

bool room_is_dark( ROOM_INDEX_DATA *pRoomIndex, CHAR_DATA *ch )
{
    int hour;
    int world = pRoomIndex->area->world;
    bool found = FALSE;
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( pRoomIndex == NULL )
	{
	bug( "Room_is_dark: No room! Very Bad!\n\r", 0 );
	return TRUE;
	}	
    
    if (IS_SET( pRoomIndex->room_flags, ROOM_DARK ) )
    {
	/* Must have a magic light that is lit to see */
	for ( victim = pRoomIndex->people; victim != NULL; victim = victim->next_in_room )
	{
	    for ( obj = victim->carrying; obj != NULL; obj = obj->next_content )
	    {
		if (obj->light_data != NULL && IS_SET(obj->light_data->flags, LIGHT_MAGIC_LIT ) )
		found = TRUE;
	    }
	}
	return !found;
	/*
	 * This is assuming that a 'magic light' proceeds all 
	 * If you have a magic light, you can see.  If you don't have
	 * a magic light, and the room is dark, you don't see.
	 * If this is true or false, nothing else matters, thats why
	 * the return of found.
	 */
    }

    if ( ch )
    pRoomIndex->light_adjustment = get_light_adjustment(ch, ch->in_room);

    if ( pRoomIndex->light_adjustment >= 25 )
	return FALSE;

    if ( pRoomIndex->light_adjustment < 25 )
	return TRUE;

    if ( pRoomIndex->sector_type == SECT_UNDERGROUND
	|| pRoomIndex->room_flags == ROOM_INDOORS 
	|| pRoomIndex->sector_type == SECT_UNDERWATER
	|| pRoomIndex->sector_type == SECT_OCEAN_BOTTOM )
	return TRUE;

    hour = get_hour( pRoomIndex->area );

    if ( hour < time_table[ world ].sunrise 
      || hour > time_table[ world ].sunset )
	return TRUE;

    return FALSE;
}

bool check_room_size( ROOM_INDEX_DATA *pRoomIndex, CHAR_DATA *ch )
{

    if ( ch->size > pRoomIndex->size )
	return FALSE;

    return TRUE;
}

/*
 * True if room is private.
 */
bool room_is_private( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA *rch;
    int count;

    count = 0;
    for ( rch = pRoomIndex->people; rch != NULL; rch = rch->next_in_room )
	count++;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_PRIVATE)  && count >= 2 )
	return TRUE;

    if ( IS_SET(pRoomIndex->room_flags, ROOM_SOLITARY) && count >= 1 )
	return TRUE;
    
    if ( IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) )
	return TRUE;

    return FALSE;
}

/* visibility on a room -- for entering and exits */
bool can_see_room( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex )
{
    if (IS_SET(pRoomIndex->room_flags, ROOM_IMP_ONLY) 
    && !IS_IMP(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags, ROOM_GODS_ONLY)
    &&  !IS_IMMORTAL(ch))
	return FALSE;

    if (IS_SET(pRoomIndex->room_flags,ROOM_NEWBIES_ONLY)
    &&  ch->level > 5 && !IS_IMMORTAL(ch))
	return FALSE;

    if ( !IS_NPC(ch) && !IS_IMMORTAL(ch) )
    	if (pRoomIndex->area->visible > ch->level )
	    return FALSE;

    if ( IS_NPC(ch) && IS_SET( pRoomIndex->room_flags, ROOM_NO_MOB ) 
	&& !IS_SET( ch->act, ACT_MOUNT ) && !IS_SET( ch->affected_by, AFF_CONTROL_MIND ) 
	&& !IS_SET(ch->act2, ACT2_WALKTO) && !IS_SET( ch->act, ACT_PET ) && !IS_SET(ch->act2, ACT2_GUIDE))
	return FALSE;

    return TRUE;
}

bool in_same_plane ( CHAR_DATA *ch, CHAR_DATA *victim )
{

    if ( IS_NPC(victim) && !IS_NPC(ch) )
    {
    	if ( ( ( IS_SET(victim->act2, ACT2_DREAMSTATE) && !IS_SET(ch->plr2, PLR_DREAMSTATE) ) 
	|| ( !IS_SET(victim->act2, ACT2_DREAMSTATE) && IS_SET(ch->plr2, PLR_DREAMSTATE) ) )
	&& !IS_IMMORTAL(ch) )
	    return FALSE;
    }
    else if ( IS_NPC(victim) && IS_NPC(ch) )
    {
       	if ( ( IS_SET(victim->act2, ACT2_DREAMSTATE) && !IS_SET(ch->act2, ACT2_DREAMSTATE) )
  	|| ( !IS_SET(victim->act2, ACT2_DREAMSTATE) && IS_SET(ch->act2, ACT2_DREAMSTATE) ) )
	    return FALSE;
    }
    else if ( !IS_NPC(victim) && IS_NPC(ch) )
    {
       	if ( ( IS_SET(victim->plr2, PLR_DREAMSTATE) && !IS_SET(ch->act2, ACT2_DREAMSTATE) )
	|| ( !IS_SET(victim->plr2, PLR_DREAMSTATE) && IS_SET(ch->act2, ACT2_DREAMSTATE) ) )
	    return FALSE;
    }
    else if ( !IS_NPC(victim) && !IS_NPC(ch) )
    {
       	if ( ( ( IS_SET(victim->plr2, PLR_DREAMSTATE) && !IS_SET(ch->plr2, PLR_DREAMSTATE) ) 
	|| ( !IS_SET(victim->plr2, PLR_DREAMSTATE) && IS_SET(ch->plr2, PLR_DREAMSTATE) ) )
	&& !IS_IMMORTAL(ch) )
	    return FALSE;
    }

    return TRUE;
}

/*
 * True if char can see victim.
 */
bool can_see( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *pRoomIndex;
    int chance;
    bool found = FALSE;
    
    pRoomIndex = ch->in_room;

    if ( ch == victim )
	return TRUE;

    if (IS_OPTIVISION(ch) )
	return TRUE;

    if ( !in_same_plane(ch, victim) )
	return FALSE;

    if ( IS_SET(victim->act2, ACT2_MOB_NO_SEE) && !IS_IMMORTAL(ch) )
	return FALSE;
    
    if ( !IS_NPC(victim)
    &&   IS_SET(victim->plr, PLR_WIZINVIS)
    &&   ch->level < victim->invis_level )
	{
	if ( check_immortal_order(ch, victim) )
	    return TRUE;
	if ( IS_IMMORTAL(ch) && victim->invis_level <= 62 )
	    return TRUE;
	if ( IS_IMP(ch) && victim->invis_level < 63 )
	    return TRUE;

	return FALSE;
	}

    if ( (!IS_NPC(ch) && IS_SET(ch->plr, PLR_HOLYLIGHT)) 
    ||   (IS_NPC(ch) && IS_IMMORTAL(ch)))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) || IS_SET(ch->affected_by_2,AFF_LIGHT_SENSITIVE ) )
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_SHADOWFORM)
    &&   !IS_AFFECTED(ch, AFF_DETECT_SHADOWFORM) )
	return FALSE;

    if ( IS_AFFECTED(victim, AFF_HIDE)
    &&   !IS_AFFECTED(ch, AFF_DETECT_HIDDEN)
    &&   victim->fighting == NULL )
	return FALSE;

   if ( IS_AFFECTED_2( victim, AFF_FADE ) && ch->fighting == NULL )
   {
	chance = get_skill( ch, gsn_awareness );

        if ( number_percent() < chance || IS_SET( ch->affected_by_2, AFF_SENSES ) )
	{
		send_to_char("You see an individual faded into the surroundings!\n\r", ch);
		if ( chance != 0 )
        	    check_improve(ch,gsn_awareness,TRUE,4);
		REMOVE_BIT(victim->affected_by_2, AFF_FADE);
		send_to_char("You have been revealed!\n\r", victim);
	}
        else
	{
		if ( chance != 0 )
                    check_improve(ch,gsn_awareness,FALSE,4);
		return FALSE; 
	}
   }

    if (IS_SET( pRoomIndex->room_flags, ROOM_DARK ) )
    {
	/* Must have a magic light that is lit to see */
	for ( victim = pRoomIndex->people; victim != NULL; victim = victim->next_in_room )
	{
	    for ( obj = victim->carrying; obj != NULL; obj = obj->next_content )
	    {
		if (obj->light_data != NULL && IS_SET(obj->light_data->flags, LIGHT_MAGIC_LIT ) )
		found = TRUE;
	    }
	}
	return found;
	/*
	 * This is assuming that a 'magic light' proceeds all 
	 * If you have a magic light, you can see.  If you don't have
	 * a magic light, and the room is dark, you don't see.
	 * If this is true or false, nothing else matters, thats why
	 * the return of found.
	 */
    }

    if ( pRoomIndex->light_adjustment < 25 
	&& !IS_SET(ch->affected_by, AFF_INFRARED) 
	&& !IS_SET(ch->affected_by, AFF_DARK_VISION) )
	return FALSE;

   return TRUE;
}



/*
 * True if char can see obj.
 */
bool can_see_obj( CHAR_DATA *ch, OBJ_DATA *obj )
{
    if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_HOLYLIGHT) )
	return TRUE;

    if ( IS_SET(obj->extra_flags,ITEM_VIS_DEATH))
	return FALSE;

    if ( ( IS_AFFECTED( ch, AFF_BLIND ) || IS_SET(ch->affected_by_2,AFF_LIGHT_SENSITIVE) )  &&
	( ( obj->edible_data != NULL && obj->edible_data->spells != NULL )
	  || ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL && obj->wear_data->magic_data->spells != NULL ) ) )
	return TRUE;
    else if ( IS_AFFECTED( ch, AFF_BLIND ) || IS_SET(ch->affected_by_2, AFF_LIGHT_SENSITIVE) )
	return FALSE;

    if ( obj->light_data != NULL && obj->light_data->fuel != 0 )
	return TRUE;

    if ( IS_SET(obj->extra_flags, ITEM_SHADOWFORM)
    &&   !IS_AFFECTED(ch, AFF_DETECT_SHADOWFORM) )
        return FALSE;

    if ( IS_OBJ_STAT(obj,ITEM_GLOW))
	return TRUE;

    if ( ch->level > 5 )
    if ( IS_SET(obj->extra_flags,ITEM_NEWBIE))
	return FALSE;

    if ( room_is_dark( ch->in_room, ch )
	 && !IS_AFFECTED(ch, AFF_INFRARED)
	 && !IS_AFFECTED(ch, AFF_DARK_VISION) )
	return FALSE;

    return TRUE;
}

/*
 * True if char can drop obj.
 */
bool can_drop_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool inapartment )
{
    ROOM_INDEX_DATA *pRoomIndex;
    OBJ_DATA *obj1;
    OBJ_DATA *obj1_next;
    OBJ_DATA *obj_cont;
    int count = 0;
    int count_cont = 0;
    
    if ( !IS_NPC(ch) && IS_IMMORTAL(ch))
	return TRUE;

    if ( inapartment )
    {
    	pRoomIndex = get_room_index( ch->in_room->vnum );
    	for ( obj1 = pRoomIndex->contents ; obj1 != NULL ; obj1 = obj1_next )
    	{
 	    obj1_next = obj1->next_content;
 	    count++;

            if ( IS_SET( pRoomIndex->room_flags_2, ROOM_SAVE ) )
            {
            	if ( obj1->cont_data != NULL )
            	{
                    for ( obj_cont = obj1->cont_data->contents; obj_cont != NULL; obj_cont = obj_cont->next_content )
                    	count_cont++;
                }

                if ( ( count + count_cont ) >= 50 )
                {
                    send_to_room( pRoomIndex->vnum, "`WThis room is full!\n\r" );
                	return FALSE;
            	}
            }
    	}
    }

    if ( !IS_SET(obj->extra_flags, ITEM_NODROP) )
	return TRUE;

    return FALSE;
}

bool check_for_dream( CHAR_DATA *ch )
{
    int chance;

    if (IS_NPC(ch) )	
	return FALSE;

    if (ch->position != POS_SLEEPING)
	return FALSE;

    if (IS_SET(ch->dream_flags, IS_DREAMING) )
	return TRUE;

   chance = ch->level / 2 + 2 + ch->pcdata->dreams;

    if ( number_range(0,400 - chance ) == 0 )
    {
        chance = URANGE( 0, number_range(0,10) + 
		( ch->level < 10 ? number_range(0,10) : 0 ) -
		( ch->pcdata->nightmares / 10 ) /*change as more
					          dreams,nightmares added */
		, 10 );	
	switch(chance)
	{
	case 0:
	SET_BIT(ch->dream_flags, DREAM_IS_NIGHTMARE );
	break;
	case 1:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 2:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 3:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 4:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 5:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 6:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 7:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 8:
	SET_BIT(ch->dream_flags, DREAM_IS_NORMAL );
	break;
	case 9:
	SET_BIT(ch->dream_flags, DREAM_IS_GOOD );
	break;
	case 10:
	SET_BIT(ch->dream_flags, DREAM_IS_GOOD_2 );
	break;
	default:
    	{
	    bug( "Bad dream type", 0 );
	    ch->dream_flags = 0;
	    return FALSE;
    	}
	break;
	}
	SET_BIT(ch->dream_flags, IS_DREAMING );
	return TRUE;
    }

    return FALSE;
}

bool script_in_process(CHAR_DATA *victim)
{
    TRIGGER_DATA *pTrig;
        for ( pTrig = victim->triggers; pTrig != NULL; pTrig = pTrig->next )
	if ( pTrig->current != NULL )
	return TRUE;
    return FALSE;
}

bool do_trigger( void  *vo, CHAR_DATA *ch, OBJ_DATA *object, char *argument,  int TRIGGER, int TRIG_TYPE )
{
    TRIGGER_DATA *pTrig;
    TRIGGER_INDEX_DATA *pTrigIndex;
    CHAR_DATA * victim;
    OBJ_DATA * obj;
    ROOM_INDEX_DATA *room;
    bool found = FALSE;

    if ( TRIG_TYPE == TRIG_TYPE_MOB )
    {
	victim = (CHAR_DATA *) vo;
        for ( pTrig = victim->triggers; pTrig != NULL; pTrig = pTrig->next )
        {
            if ( pTrig->trigger_type == TRIGGER  )
		pTrig->attempts++;
            if ( pTrig->trigger_type == TRIGGER  
	      && can_do_trigger(pTrig, ch)
	      && (object == NULL
	      || (object->pIndexData->vnum == pTrig->gets_vnum || pTrig->gets_vnum == 0 ) )
	      && is_trig_keyword(pTrig, argument)
	      && !IS_SET(pTrig->bits, SCRIPT_HALT)
	      && !IS_SET(pTrig->flags, SCRIPT_HALT_RESET)
	      && !IS_SET(pTrig->flags, TRIG_INACTIVE)
	      && !IS_SET(victim->act2, ACT2_HALT)
	      && pTrig->timer < 1
              && pTrig->current == NULL )
            {
            	pTrig->current = pTrig->script;
            	pTrig->bits = SCRIPT_ADVANCE;
            	act_trigger( victim, pTrig->name, argument, IS_NPC(ch) ? ch->short_descr : ch->name, NULL );
	    	pTrig->activated++;
	    	pTrigIndex = get_trig_index(pTrig->vnum);
	    	pTrig->timer = pTrigIndex->timer;
	    	found = TRUE;
	    	script_update( victim, TRIG_EACH_PULSE ); 
            }
        }
    }
    if ( TRIG_TYPE == TRIG_TYPE_OBJ )
    {
	obj = (OBJ_DATA *) vo;
    	for ( pTrig = obj->triggers; pTrig != NULL; pTrig = pTrig->next )
    	{
            if ( pTrig->trigger_type == TRIGGER  )
		pTrig->attempts++;
    	    if ( pTrig->trigger_type == TRIGGER  
		  && can_do_trigger(pTrig, ch)
		  && is_trig_keyword(pTrig, argument)
		  && !IS_SET(pTrig->bits, SCRIPT_HALT)
	          && pTrig->timer < 1
	          && !IS_SET(pTrig->flags, TRIG_INACTIVE)
		  && (obj->owner == '\0' || !str_cmp(obj->owner,ch->name) )
	          && !IS_SET(pTrig->flags, SCRIPT_HALT_RESET)
    	      && pTrig->current == NULL )
    	     {
    	        pTrig->current = pTrig->script;
    	        pTrig->bits    = SCRIPT_ADVANCE;
    	        act_trigger_obj(obj, pTrig->name, argument, IS_NPC(ch) ? ch->short_descr : ch->name, NULL );
	        pTrig->activated++;
	        pTrigIndex = get_trig_index(pTrig->vnum);
	        pTrig->timer = pTrigIndex->timer;
	        found = TRUE;
		script_update_obj( obj, TRIG_EACH_PULSE );
    	     }
    	}
    }
    if ( TRIG_TYPE == TRIG_TYPE_ROOM )
    {
	room = (ROOM_INDEX_DATA *) vo;
	if (!room)
	return FALSE;
	for ( pTrig = room->triggers; pTrig != NULL; pTrig = pTrig->next )
	{
            if ( pTrig->trigger_type == TRIGGER  )
		pTrig->attempts++;
	    if ( pTrig->trigger_type == TRIGGER  
		&& can_do_trigger(pTrig, ch)
		&& is_trig_keyword(pTrig, argument)
	        && (object == NULL
	        || (object->pIndexData->vnum == pTrig->gets_vnum || pTrig->gets_vnum == 0 ) )
		&& !IS_SET(pTrig->bits, SCRIPT_HALT)
	        && !IS_SET(pTrig->flags, TRIG_INACTIVE)
	        && !IS_SET(pTrig->flags, SCRIPT_HALT_RESET)
	        && pTrig->timer < 1
		&& pTrig->current == NULL )
		{
		pTrig->current = pTrig->script;
		pTrig->bits    = SCRIPT_ADVANCE;
		act_trigger_room(room, pTrig->name, argument, IS_NPC(ch) ? ch->short_descr : ch->name, NULL );
	        pTrigIndex = get_trig_index(pTrig->vnum);
	        pTrig->timer = pTrigIndex->timer;
	        pTrig->activated++;
	        found = TRUE;
		script_update_room(room, TRIG_EACH_PULSE );
		}
	}
    }
    return found;
}
bool check_string (char *name)
{
 char *pc;
   for ( pc = name; *pc != '\0'; pc++ )
       {
	 if (!isalnum(*pc) )
	 if (!isspace(*pc) )
	 if (!ispunct(*pc) )
         return FALSE;
       }
  return TRUE;
}
void approve_object( OBJ_DATA *obj )
{
    SET_BIT(obj->pIndexData->approve_data->flags, APPROVED);
    strncpy( obj->pIndexData->approve_data->approver, "(System)", 30 );
    obj->pIndexData->approve_data->approved_date = current_time;
    return;
}
bool check_wander(CHAR_DATA *ch, EXIT_DATA *pexit, int door)
{
bool WANDER_DEBUG = TRUE;
    if ( IS_SET(ch->act, ACT_SENTINEL) )
    {
	if ( WANDER_DEBUG )
	    send_to_char("Sentinel, No wander\n\r",ch);
	return FALSE;
    }
    if ( script_in_process(ch) )
    {
	if ( WANDER_DEBUG )
	    send_to_char("Script in process, No wander\n\r",ch);
	return FALSE;
    }
    if ( ch->master != NULL && ch->master != ch )
    {
	if ( WANDER_DEBUG )
	    send_to_char("Following somebody other then myself, No wander\n\r",ch);
	return FALSE;
    }
    if ( number_bits(ch->in_room->area->wander_time) != 0 )
    {
	if ( WANDER_DEBUG )
	    send_to_char("Area wander time NOT 0, No wander\n\r",ch);
	return FALSE;
    }
    if ( pexit == NULL )  
    {
	if ( WANDER_DEBUG )
	    send_to_char("Picked an invalid direction, No wander\n\r",ch);
	return FALSE;
    }
    if ( pexit->u1.to_room == NULL )
    {
	if ( WANDER_DEBUG )
	    send_to_char("u1.to_room is NULL, No wander\n\r",ch);
	return FALSE;
    }
    if ( (pexit->u1.to_room->sector_type == SECT_AIR ) && !IS_AFFECTED( ch, AFF_FLYING ) )
    {
	if ( WANDER_DEBUG )
	    send_to_char("I'm not flying and to room is air, No wander\n\r",ch);
	return FALSE;
    }
    if ( ch->valid_sector[pexit->u1.to_room->sector_type] == FALSE )
    {
	if ( WANDER_DEBUG )
	    send_to_char("My valid_sector is FALSE, No wander\n\r",ch);
	return FALSE;
    }
    if ( !can_see_room(ch, pexit->u1.to_room) )
    {
	if ( WANDER_DEBUG )
	    send_to_char("Can't see to room!, No wander\n\r",ch);
	return FALSE;
    }
    if ( IS_SET(pexit->exit_info, EX_CLOSED) )
    {
	if ( !IS_AFFECTED( ch, AFF_DIMENSION_WALK ) && IS_SET(pexit->exit_info,EX_PASSPROOF ) )
	{
	    if ( WANDER_DEBUG )
		send_to_char("Door is passproof- No dimension walk, No wander\n\r",ch);
	    return FALSE;
	}
	if ( !IS_SET( ch->pIndexData->form , FORM_SENTIENT ) )
	{
	    if ( WANDER_DEBUG )
		send_to_char("Door and I'm not Sentient, No wander\n\r",ch);
	    return FALSE;
	}
	if ( IS_SET(pexit->exit_info, EX_HIDDEN) && !IS_SET(ch->pIndexData->act2, ACT2_SEE_HIDDEN ) )
	{
	    if ( WANDER_DEBUG )
		send_to_char("Door with hidden exit-  I can't see hidden, No wander\n\r",ch);
	    return FALSE;
	}
	if ( IS_SET(pexit->exit_info, EX_SECRET) && !IS_SET(ch->pIndexData->act2, ACT2_SEE_SECRET ) )
	{
	    if ( WANDER_DEBUG )
		send_to_char("Door with secret exit- I can't see secret, No wander\n\r",ch);
	    return FALSE;
	}
    }
    if ( IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) )
    {
	if ( WANDER_DEBUG )
	    send_to_char("To room is NO_MOB, No wander\n\r",ch);
	return FALSE;
    }
	   
    if ( IS_SET(ch->act, ACT_STAY_AREA) && pexit->u1.to_room->area != ch->in_room->area ) 
    {
	if ( WANDER_DEBUG )
	    send_to_char("To room is out of my area.  I'm ACT_STAY_AREA No wander\n\r",ch);
	return FALSE;
    }
    if ( IS_SET( ch->act2, ACT2_STAY_GUILD ) && !IS_SET( ch->guilds, pexit->u1.to_room->guilds ) )
    {
	if ( WANDER_DEBUG )
	    send_to_char("To room is not my guild - I'm ACT2_STAY_GUILD, No wander\n\r",ch);
	return FALSE;
    }
    if ( IS_SET(ch->act, ACT_IN_DOORS ) && !IS_SET( pexit->u1.to_room->room_flags, ROOM_INDOORS ) )
    {
	if ( WANDER_DEBUG )
	    send_to_char("To room is out side - I'm ACT_IN_DOORS, No wander\n\r",ch);
	return FALSE;
    }
    if ( IS_SET(ch->act, ACT_OUT_DOORS ) && IS_SET( pexit->u1.to_room->room_flags, ROOM_INDOORS ) ) 
    {
	if ( WANDER_DEBUG )
	    send_to_char("To room is ROOM_INDOORS - I'm ACT_OUT_DOORS, No wander\n\r",ch);
	return FALSE;
    }
    return TRUE;
}


