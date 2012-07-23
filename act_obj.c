/***************************************************************************
 *  Origina- Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"
#define MAX_IMP 3 

char * const sac_name[MAX_IMP]=
{
	"Galadrail","Reverie","Sands"
};	

int iNest = 0;


/*
 * For those players that always lose thier keys.
 * This simple function will create an apartment key into the inventory
 * of the character.  The character MUST already have a home.
 */
void get_home_key( CHAR_DATA *ch,  char * argument)
{
    ROOM_INDEX_DATA * home;
    EXIT_DATA * pexit;
    OBJ_INDEX_DATA * obji;
    OBJ_DATA	* obj;
    LOCK_DATA * lock_list;
    int door;
 
    if ( ch->pcdata == NULL || ch->home < 1 || ( home = get_room_index(ch->home) ) == NULL )
    {
	send_to_char("You don't seem to have a home\n\r",ch);
	return;	
    }
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = home->exit[door] ) != NULL
	&& IS_SET(pexit->exit_info, EX_ISDOOR)
	&& pexit->keyword[0]  != '\0' )
	{
	    if ( pexit->lock != NULL )
	    { 
		for ( lock_list = pexit->lock; lock_list != NULL; lock_list = lock_list->next )
		{
		    if ( (obji = get_obj_index(lock_list->vnum) ) != NULL )
		    {
			obj = create_object( obji );
			obj_to_char(obj, ch);
			if ( !str_cmp("keylost",argument) )
			send_to_char("You grab your spare key\n\r",ch);
			else
			send_to_char("Your new key is made!\n\r",ch);
			return;
		    }
	 	}
	    }
	}	
    }
}  

bool is_bastard( CHAR_DATA *ch, OBJ_DATA *obj )
{
    int size_diff;

    if ( obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
	return FALSE;

    size_diff = ch->size - obj->wear_data->size;

    if ( ch->race == 6 )
	size_diff = size_diff + 1;

    switch (size_diff)
    {
	default:
	    log_string( "Bad size (Is_bastard)" );
	    return FALSE;
	    break;
	case -5:
	case -4:
	case -3:
	    return FALSE;
	case -2:
	    return IS_WEAPON_STAT( obj, WEAPON_KNIFE );
	case -1:
	    return !IS_WEAPON_STAT( obj, WEAPON_KNIFE|WEAPON_BASTARD|WEAPON_TWO_HANDS );
	case  0:
	    return IS_WEAPON_STAT( obj, WEAPON_BASTARD );
	case  1:
	    return IS_WEAPON_STAT( obj, WEAPON_TWO_HANDS );
	case  2:
	case  3:
	case  4:
	case  5:
	     return FALSE;
	}

    return FALSE;
}


bool is_knife_sized( CHAR_DATA *ch, OBJ_DATA *obj )
{
    int size_diff;

    if ( obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
	return FALSE;

    size_diff = ch->size - obj->wear_data->size;

    if ( ch->race == 6 )
	size_diff = size_diff + 1;

    switch (size_diff)
    {
	default:
	    log_string( "Bad size (Is_knife_sized)" );
	    return FALSE;
	    break;
	case -5:
	case -4:
	case -3:
	case -2:
	case -1:
	    return FALSE;
	case  0:
	    return IS_WEAPON_STAT( obj, WEAPON_KNIFE );
	case  1:
	    return !IS_WEAPON_STAT( obj, WEAPON_KNIFE|WEAPON_BASTARD|WEAPON_TWO_HANDS );
	case  2:
	    return IS_WEAPON_STAT( obj, WEAPON_BASTARD );
	case  3:
	    return IS_WEAPON_STAT( obj, WEAPON_TWO_HANDS );
	case  4:
	case  5:
	     return FALSE;
	}

    return FALSE;
}


bool is_one_handed( CHAR_DATA *ch, OBJ_DATA *obj )
{
    int size_diff;

    if ( obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
	return FALSE;

    size_diff = ch->size - obj->wear_data->size;

    if ( ch->race == 6 )
	size_diff = size_diff + 1;

    switch (size_diff)
    {
	default:
	    log_string( "Bad size (Is_one_handed)" );
	    return FALSE;
	    break;
	case -5:
	case -4:
	case -3:
	case -2:
	    return FALSE;
	case -1:
	    return IS_WEAPON_STAT( obj, WEAPON_KNIFE );
	case  0:
	    return !IS_WEAPON_STAT( obj, WEAPON_KNIFE|WEAPON_BASTARD|WEAPON_TWO_HANDS );
	case  1:
	    return IS_WEAPON_STAT( obj, WEAPON_BASTARD );
	case  2:
	    return IS_WEAPON_STAT( obj, WEAPON_TWO_HANDS );
	case  3:
	case  4:
	case  5:
	     return FALSE;
	}

    return FALSE;
}


bool is_two_handed( CHAR_DATA *ch, OBJ_DATA *obj )
{
    int size_diff;

    if ( obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
	return FALSE;

    size_diff = ch->size - obj->wear_data->size;

    if ( ch->race == 6 )
	size_diff = size_diff + 1;

    switch (size_diff)
    {
	default:
	    log_string( "Bad size (Is_two_handed)" );
	    return FALSE;
	    break;
	case -5:
	case -4:
	    return FALSE;
	case -3:
	    return IS_WEAPON_STAT( obj, WEAPON_KNIFE );
	case -2:
	    return !IS_WEAPON_STAT( obj, WEAPON_KNIFE|WEAPON_BASTARD|WEAPON_TWO_HANDS );
	case -1:
	    return IS_WEAPON_STAT( obj, WEAPON_BASTARD );
	case  0:
	    return IS_WEAPON_STAT( obj, WEAPON_TWO_HANDS );
	case  1:
	case  2:
	case  3:
	case  4:
	case  5:
	     return FALSE;
	}

    return FALSE;
}

long haggle_cost( CHAR_DATA *ch, long cost )
{
    char buf[MAX_STRING_LENGTH];
    int skill;
    
    if ( ( skill = ( get_skill( ch, gsn_haggle ) ) ) > 0 )
    {
	if ( number_range(1, 105 ) < skill )
	{
	    cost = ( 100 - ( skill / 3 ) ) * cost / 100;
	    check_improve( ch, gsn_haggle, TRUE, 1 );
	    sprintf( buf, "You haggle the price down to %ld coins.\n\r", cost );
	    send_to_char( buf, ch );
	    return( cost );
	}
	else
	{
	    cost = ( 100 + ( skill / 3 ) ) * cost / 100;
	    check_improve( ch, gsn_haggle, FALSE, 1 );
	    sprintf( buf, "You haggle but lose track and end up spending %ld coins.\n\r", cost );
	    send_to_char( buf, ch );
	    return( cost );
	}
    }
    else
	return( cost );
}

long get_money_total( CHAR_DATA *ch, int coin_type )
{
    int num;
    long total;

    total = 0;
    for( num = 0 ; num < MAX_WORLD ; num++ )
        total += exchange_rate[ num ] [ coin_type ] *  ch->gold[ num ] / 100;
	
    return( total );
}


void get_char_money( CHAR_DATA *ch, long cost, int coin_type )
{
    int num;
    long total;

    total = cost;
    for ( num = coin_type ; total > 0 ; num ++ )
    {  
	cost = UMIN( ch->gold[ num % MAX_WORLD ] , ( 100 * total % exchange_rate[ num % MAX_WORLD ] [ coin_type ] ? 100 * total / exchange_rate[ num % MAX_WORLD ] [ coin_type ] + 1 : 100 * total / exchange_rate[ num % MAX_WORLD ] [ coin_type ] ) );  
	ch->gold[ num % MAX_WORLD ] -= cost;
	total -= exchange_rate[ num % MAX_WORLD ] [ coin_type ] * cost / 100;	
	if ( num > 6 )
	{
	    bug("get_char_money : not enough money to cover cost",cost);
	    return;
	}
    }

    return;
}

void transfer_money( CHAR_DATA *ch, CHAR_DATA *victim, long cost, int coin_type ) 
{
    int num;
    long total;

    total = cost;
    for ( num = coin_type ; total > 0 ; num ++ )
    {  
	cost = UMIN( ch->gold[ num % MAX_WORLD ] , ( 100 * total % exchange_rate[ num % MAX_WORLD ] [ coin_type ] ? 100 * total / exchange_rate[ num % MAX_WORLD ] [ coin_type ] + 1 : 100 * total / exchange_rate[ num % MAX_WORLD ] [ coin_type ] ) );  
	ch->gold[ num % MAX_WORLD ] -= cost;
	victim->gold[ num % MAX_WORLD ] += cost;
	total -= exchange_rate[ num % MAX_WORLD ] [ coin_type ] * cost / 100;	
	if ( num > 20 )
	{
	    bug("transfer_money : not enough money to cover cost",cost);
	    return;
	}
    }

    return;
}

int get_coin_type( char * coin )
{
   int num;

    for ( num = 0 ; num < MAX_WORLD ; num ++ )
	if ( !str_cmp( coin, coin_name[ num ] ) || !str_cmp( coin, coin_names[ num ] ) ) 
	    return( num );

    return( -1 );
}

/* RT part of the corpse looting code */

bool can_loot( CHAR_DATA *ch, OBJ_DATA *obj )
{
    CHAR_DATA *owner, *wch;

    if ( IS_IMMORTAL( ch ) )
	return TRUE;

    if ( obj->pIndexData->vnum != 11 )
	return TRUE;

    if ( !obj->owner || obj->owner == NULL )
	return TRUE;

    owner = NULL;
    for ( wch = char_list; wch != NULL ; wch = wch->next )
        if ( !str_cmp( wch->name, obj->owner ) )
            owner = wch;

    if ( owner == NULL )
	return TRUE;

    if ( !str_cmp( ch->name, owner->name ) )
	return TRUE;

    if ( !IS_NPC( owner ) && IS_SET( owner->plr,PLR_CANLOOT ) ) 
	return TRUE;

    if ( is_same_group( ch, owner ) )
	return TRUE;

    return FALSE;
}

void get_obj( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container )
{
    /* variables for AUTOSPLIT */
    char buffer[100];
    CHAR_DATA *gch;
    bool trig;
    int members = 0;
    bool found = TRUE;

    if ( !CAN_WEAR( obj, ITEM_TAKE|ITEM_MONEY_0|ITEM_MONEY_1|ITEM_MONEY_2|ITEM_MONEY_3 ) )
    {
	send_to_char( "You can't take that.\n\r", ch );
	return;
    }

    if ( ( obj->pIndexData->vnum == OBJ_VNUM_ORE_LARGE ||
	obj->pIndexData->vnum == OBJ_VNUM_ORE_MEDIUM ||
	obj->pIndexData->vnum == OBJ_VNUM_ORE_SMALL ||
	obj->pIndexData->vnum == 18008 ) && get_skill( ch, gsn_mining ) > 0 )
    {
	if ( obj->material == MAT_STEEL )
	    ch->pcdata->minerals[MINERAL_STEEL] = URANGE( 0, ch->pcdata->minerals[MINERAL_STEEL] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_GOLD )
	    ch->pcdata->minerals[MINERAL_GOLD] = URANGE( 0, ch->pcdata->minerals[MINERAL_GOLD] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_SILVER )
	    ch->pcdata->minerals[MINERAL_SILVER] = URANGE( 0, ch->pcdata->minerals[MINERAL_SILVER] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_RUBY )
	    ch->pcdata->minerals[MINERAL_RUBY] = URANGE( 0, ch->pcdata->minerals[MINERAL_RUBY] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_SAPPHIRE )
	    ch->pcdata->minerals[MINERAL_SAPPHIRE] = URANGE( 0, ch->pcdata->minerals[MINERAL_SAPPHIRE] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_IRON )
	    ch->pcdata->minerals[MINERAL_IRON] = URANGE( 0, ch->pcdata->minerals[MINERAL_IRON] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_BRONZE )
	    ch->pcdata->minerals[MINERAL_BRONZE] = URANGE( 0, ch->pcdata->minerals[MINERAL_BRONZE] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_COPPER )
	    ch->pcdata->minerals[MINERAL_COPPER] = URANGE( 0, ch->pcdata->minerals[MINERAL_COPPER] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_EMERALD )
	    ch->pcdata->minerals[MINERAL_EMERALD] = URANGE( 0, ch->pcdata->minerals[MINERAL_EMERALD] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_PLATINUM )
	    ch->pcdata->minerals[MINERAL_PLATINUM] = URANGE( 0, ch->pcdata->minerals[MINERAL_PLATINUM] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_MITHRIL )
	    ch->pcdata->minerals[MINERAL_MITHRIL] = URANGE( 0, ch->pcdata->minerals[MINERAL_MITHRIL] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_DIAMOND )
	    ch->pcdata->minerals[MINERAL_DIAMOND] = URANGE( 0, ch->pcdata->minerals[MINERAL_DIAMOND] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_OBSIDIAN )
	    ch->pcdata->minerals[MINERAL_OBSIDIAN] = URANGE( 0, ch->pcdata->minerals[MINERAL_OBSIDIAN] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_ALABASTER )
	    ch->pcdata->minerals[MINERAL_ALABASTER] = URANGE( 0, ch->pcdata->minerals[MINERAL_ALABASTER] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_TITANIUM )
	    ch->pcdata->minerals[MINERAL_TITANIUM] = URANGE( 0, ch->pcdata->minerals[MINERAL_TITANIUM] + obj->wear_data->weight, 1000 );
	else if ( obj->material == MAT_ADAMANTITE )
	    ch->pcdata->minerals[MINERAL_ADAMANTITE] = URANGE( 0, ch->pcdata->minerals[MINERAL_ADAMANTITE] + obj->wear_data->weight, 1000 );
	else
	   found = FALSE;

	if ( found )
	{
	    send_to_char( "The mineral was added to your storage.\n\r", ch );
	    extract_obj( obj );
	    return;
	}
    }

    if ( ch->carry_number >= can_carry_n( ch ) )
    {
	send_to_char( "You can't carry that many items.\n\r",ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	act( "$d: you can't carry that much weight.",
	    ch, NULL, obj->name, TO_CHAR );
	return;
    }

    if ( container != NULL && !can_loot(ch,container) )
    {
	act("Corpse looting is not permitted.",ch,NULL,NULL,TO_CHAR );
	return;
    }

    if ( container != NULL )
    {
	act( "`WYou get $p.`W", ch, obj, container, TO_CHAR );
	act( "`W$n gets $p.`W", ch, obj, container, TO_ROOM );
	obj_from_obj( obj );
    }
    else
    {
	act( "`WYou get $p.`W", ch, obj, container, TO_CHAR );
	act( "`W$n gets $p.`W", ch, obj, container, TO_ROOM );
	obj_from_room( obj );
    }

    if ( CAN_WEAR( obj, ITEM_MONEY_0|ITEM_MONEY_1|ITEM_MONEY_2|ITEM_MONEY_3 ) )
    {
	int coin_type=0;
	int value=obj->wear_data->cost;

	if ( IS_SET( obj->wear_data->wear_flags, ITEM_MONEY_0 ) )
	    coin_type = 0;
	if ( IS_SET( obj->wear_data->wear_flags, ITEM_MONEY_1 ) )
	    coin_type = 1;
	if ( IS_SET( obj->wear_data->wear_flags, ITEM_MONEY_2 ) )
	    coin_type = 2;
	if ( IS_SET( obj->wear_data->wear_flags, ITEM_MONEY_3 ) )
	    coin_type = 3;

	ch->gold[ coin_type ] += value;

        if (IS_SET(ch->plr,PLR_AUTOSPLIT))
        { /* AUTOSPLIT code */
	 
    	  for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	  {
            if ( is_same_group( gch, ch ) )
              members++;
    	  }

	  if ( members > 1 && value > 1)
	  {
	    sprintf(buffer,"%d %s.",value, coin_name[coin_type]);
	    do_split(ch,buffer);	
	  }
        }
 
	extract_obj( obj );
    }
    else
    {
	trig = do_trigger( ch->in_room, ch, obj, NULL, TRIG_GETS, TRIG_TYPE_ROOM);
	trig = do_trigger( obj, ch, obj, NULL, TRIG_GETS, TRIG_TYPE_OBJ);
	trig = do_trigger( ch, ch, obj, NULL, TRIG_GETS, TRIG_TYPE_MOB);
	obj_to_char( obj, ch );
	if ( obj->owner[0] == '\0' )
	    strncpy(obj->owner, ch->name,30 );
    }

    if ( obj->donate_timer > 0 )
	obj->donate_timer = 0;

    return;
}

void do_poison_blade( CHAR_DATA *ch, char *argument )
{
    int sn, skill;
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *blade;
    OBJ_DATA *component;
    one_argument( argument, arg );

    if ( ( skill = get_skill( ch, gsn_poison_blade ) ) <= 0 )
    {
        send_to_char("You don't know how to poison blades.\n\r", ch );
        return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "What weapon do you want to poison?\n\r", ch );
        return;
    }

    if ( ( blade = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that weapon.\n\r", ch );
	return;
    }

    if ( blade->wear_data == NULL || blade->wear_data->weapon_data == NULL )
    {
	send_to_char( "That is not a weapon.\n\r", ch );
	return;
    }

    if ( IS_SET( blade->wear_data->weapon_data->flags, WEAPON_POISON ) )
    {
	send_to_char( "This blade is already poisoned.\n\r", ch );
	return;
    }

    for ( component = ch->carrying; component != NULL && component->pIndexData->vnum != 18050 ; component = component->next_content );

    if ( component == NULL )
    {
	send_to_char( "You need your poison component.\n\r", ch );
	return;
    }

    if ( number_range(1, 101) > skill )
    {
	send_to_char( "You failed at your attempt to poison this blade.\n\r", ch );
        check_improve( ch, gsn_poison_blade, FALSE, 1 );
	extract_obj( component );
	return;
    }
    else
    {
	SPELL_LIST * spell;

	send_to_char( "You poisoned the blade!\n\r", ch );
	check_improve( ch, gsn_poison_blade, TRUE, 1 );
	extract_obj( component );
	sn = skill_lookup( "poison" );
	if ( blade->wear_data->magic_data == NULL );
	    blade->wear_data->magic_data = magic_alloc();

	blade->wear_data->magic_data->charges = -1;
	blade->wear_data->magic_data->max_charges = -1;
	spell = spell_alloc();
	spell->level = URANGE( 1, ch->level + number_range( -5, 5 ), 60 );
	spell->spell = sn;
	spell->next = blade->wear_data->magic_data->spells;
	blade->wear_data->magic_data->spells = spell;
    }
    
    return;
}

void do_absorb( CHAR_DATA *ch, char * argument )
{
    OBJ_DATA *stone;
    long exp;
    int skill, mana = 1;
    bool failed = TRUE;
    char buf[MAX_STRING_LENGTH];

    if ( ( skill = get_skill( ch, gsn_absorb ) ) <= 0 )
    {
        send_to_char("You don't know how to absorb minerals.\n\r", ch );
        return;
    }

    stone = get_eq_char( ch, WEAR_HOLD );
    
    if ( stone == NULL )
    {
	send_to_char("You need to hold a stone first.\n\r", ch );
	return;
    }

    if ( skill <= number_range( 1, 105 ) )
	failed = FALSE;

    if ( stone->pIndexData->vnum == 18026 )
    {
	if ( failed == TRUE )
	{
	    exp = ( ch->level * number_range( 5000, 25000 ) ) / 2;
            check_improve( ch, gsn_absorb, FALSE, 3 );
	}
	else
	{
    	    exp = ch->level * number_range(5000, 25000);
	    check_improve( ch, gsn_absorb, TRUE, 5 );
	}

    	gain_exp( ch, exp );
    	sprintf( buf, "`YYou gain `y%ld `Yexperience.\n\r", exp );
    	send_to_char(buf, ch);
    }
    else if ( ( stone->pIndexData->vnum == 18002 || stone->pIndexData->vnum == 18003 || stone->pIndexData->vnum == 18004 ) && IS_SET( stone->extra_flags, ITEM_MAGIC ) )
    {
	if ( stone->pIndexData->vnum == 18002 )
	    mana = 150;
	else if ( stone->pIndexData->vnum == 18003 )
	    mana = 100;
	else if ( stone->pIndexData->vnum == 18004 )
	    mana = 50;

	if ( failed == TRUE )
	    mana /= 2;

	if ( stone->material == MAT_SAPPHIRE )
	    ch->mana[ ELEMENT_WATER ] = UMIN( ch->max_mana[ ELEMENT_WATER ] , ch->mana[ ELEMENT_WATER ] + mana );
	else if ( stone->material == MAT_RUBY )
	    ch->mana[ ELEMENT_FIRE ] = UMIN( ch->max_mana[ ELEMENT_FIRE ] , ch->mana[ ELEMENT_FIRE ] + mana );
	else if ( stone->material == MAT_GOLD )
	    ch->mana[ ELEMENT_EARTH ] = UMIN( ch->max_mana[ ELEMENT_EARTH ] , ch->mana[ ELEMENT_EARTH ] + mana );
	else if ( stone->material == MAT_COPPER )
	    ch->mana[ ELEMENT_WIND ] = UMIN( ch->max_mana[ ELEMENT_WIND ] , ch->mana[ ELEMENT_WIND ] + mana );
	else if ( stone->material == MAT_SILVER )
	    ch->mana[ ELEMENT_SPIRIT ] = UMIN( ch->max_mana[ ELEMENT_SPIRIT ] , ch->mana[ ELEMENT_SPIRIT ] + mana );
	else if ( stone->material == MAT_BRONZE )
	    ch->move = UMIN( ch->max_move, ch->move + mana );
	else if ( stone->material == MAT_IRON )
	    ch->hit = UMIN( ch->max_hit, ch->hit + mana );
	else if ( stone->material == MAT_MITHRIL || stone->material == MAT_OBSIDIAN || stone->material == MAT_PLATINUM )
	{
	    ch->mana[ ELEMENT_SPIRIT ] = UMIN( ch->max_mana[ ELEMENT_SPIRIT ] , ch->mana[ ELEMENT_SPIRIT ] + mana );
	    ch->mana[ ELEMENT_WATER ] = UMIN( ch->max_mana[ ELEMENT_WATER ] , ch->mana[ ELEMENT_WATER ] + mana );
	    ch->mana[ ELEMENT_EARTH ] = UMIN( ch->max_mana[ ELEMENT_EARTH ] , ch->mana[ ELEMENT_EARTH ] + mana );
	    ch->mana[ ELEMENT_WIND ] = UMIN( ch->max_mana[ ELEMENT_WIND ] , ch->mana[ ELEMENT_WIND ] + mana );
	    ch->mana[ ELEMENT_FIRE ] = UMIN( ch->max_mana[ ELEMENT_FIRE ] , ch->mana[ ELEMENT_FIRE ] + mana );
	    ch->move = UMIN( ch->max_move, ch->move + mana );
	    ch->hit = UMIN( ch->max_hit, ch->hit + mana );
	}
	else if ( stone->material == MAT_TITANIUM )
	{
	    ch->mana[ ELEMENT_WATER ] = ch->max_mana[ ELEMENT_WATER ];
	    ch->mana[ ELEMENT_FIRE ] = ch->max_mana[ ELEMENT_FIRE ];
	    ch->mana[ ELEMENT_WIND ] = ch->max_mana[ ELEMENT_WIND ];
	    ch->mana[ ELEMENT_EARTH ] = ch->max_mana[ ELEMENT_EARTH ];
	    ch->mana[ ELEMENT_SPIRIT ] = ch->max_mana[ ELEMENT_SPIRIT ];
	    ch->move = ch->max_move;
	    ch->hit = ch->max_hit;
	}    	
	else
	{
	    send_to_char( "You arn't holding an absorbable item.\n\r", ch );
	    return;
	}
	
	if ( failed == TRUE )
	    check_improve( ch, gsn_absorb, FALSE, 3 );
	else
	    check_improve( ch, gsn_absorb, TRUE, 5 );
    }
    else
    {
	send_to_char("You arn't holding an absorbable item.\n\r", ch );
	return;
    }

    act( "$p glows brightly in $n's hand then disappears.", ch, stone, ch, TO_ROOM );
    send_to_char("`YThe air around you becomes thick and suffacating.\n\r", ch);
    extract_obj( stone );
    return;
}

void do_mrepair( CHAR_DATA *ch, CHAR_DATA *smithy, char * arg )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int cost, chance, coin_type;

    if ( IS_NPC( ch ) )
	return;

    if ( smithy == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return;
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "Repair what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
        send_to_char( "You don't have that.\n\r", ch );
        return;
    }

    if ( obj->wear_data == NULL )
    {
	sprintf( buf, "%s cannot repair %s.\n\r", smithy->short_descr, obj->short_descr );
        send_to_char( buf, ch );
        return;
    }
   
    if ( obj->wear_data->cost <= 0 )
    {
	sprintf( buf, "%s cannot repair %s.\n\r", smithy->short_descr, obj->short_descr );
        send_to_char( buf, ch );
	return;
    }

    cost = obj->wear_data->cost / 10; 
    coin_type = ch->in_room->area->world;

    if ( get_money_total( ch, coin_type ) < cost )
    {
	sprintf( buf, "You need %d coins to repair this.\n\r", cost );
        send_to_char( buf, ch );
        return;
    }

    chance = number_range( 75, 150 );

    if ( ( chance * smithy->level / 100 ) + ( obj->condition ) < number_percent() )
    {
 	send_to_char( "They failed.\n\r", ch );
        obj->condition -= number_range( 1, ( 100 - chance ) / 2 );
        if ( obj->condition <= 0 )
        {
            act( "$p breaks while they attempt to repair it.", ch, obj, obj , TO_CHAR );
            extract_obj( obj );
        }
        return;
    }

    obj->condition = UMIN( 100 - obj->repaired, obj->condition + URANGE( 75, chance, 100 ) / 2 );

    if ( obj->condition <= 0 )
    {
        act( "$p breaks while they attempt to repair it.", ch, obj, obj , TO_CHAR );
        extract_obj( obj );
    }
    transfer_money( ch, smithy, cost, coin_type );
    sprintf( buf, "%s takes %d coins from you for the repair.\n\r", smithy->short_descr, cost );
    send_to_char( buf, ch );
    act( "They repair $p`w.", ch, obj, obj, TO_CHAR );
    obj->repaired += 5;
}

void do_bury( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg[ MAX_INPUT_LENGTH ];
    DESCRIPTOR_DATA * d;
    EXTRA_DESCR_DATA * ed;
    OBJ_INDEX_DATA * pObj;
    OBJ_DATA * obj;
    OBJ_DATA * corpse;
    AFFECT_DATA * af;
    AFFECT_DATA * af_next;
    CHAR_DATA * vch = NULL;
    int i, length;
    int cost = 0;
    int level;
 
    argument = one_argument( argument, arg );

    if ( strlen( argument ) > 60 )
    {
	send_to_char( "Epitath is too long, must be 60 characters or less.\n\r", ch );
	return;
    }

    sprintf( arg2, "%c%s%c", 34, argument, 34 );
    pObj = get_obj_index( OBJ_VNUM_TOMBSTONE );

    if ( pObj == NULL || ( obj = create_object( pObj ) ) == NULL )
    {
	send_to_char( "Unable to create tombstone.\n\r", ch );
	return;
    }

    if ( ( corpse = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You don't have that.\n\r", ch );
	return;
    }

    if ( corpse->pIndexData->vnum != 11 )
    {
	send_to_char( "You cannot bury that here.\n\r", ch );
	return;
    }

    for ( d = descriptor_list ; d != NULL && vch == NULL ; d = d->next )
    {
	if ( d->character != NULL
	   && d->connected <= CON_PLAYING
	   && d->character->pcdata != NULL
	   && d->character->pcdata->corpse == corpse )
	    vch = d->character;
    }

    if ( vch == NULL )
    {
	send_to_char( "The owner of the corpse is not on.\n\r", ch );
	return;
    }

    level = vch->level;

    if ( level < 10 )
	cost = 0;
    else if ( level < 20 )
	cost = ch->level * ( UMIN( 1, ch->level / 2 ) ) * ( UMIN( 1, ch->level / 3 ) ) * 2;

    if ( get_money_total( ch, ch->in_room->area->world ) < cost )
    {
	send_to_char( "You cannot afford the funeral costs.\n\r", ch );
	return;
    }

    *buf = '\0';
    strcpy( buf, "The writing on the tombstone says:\n\r\n\r" );
    for ( i = 0 ; i < 36 ; i++ ) 
	strcat( buf, " " );
    strcat( buf, "HERE LIES\n\r" );
    length = strlen( vch->name );
    for ( i = 0 ; i < ( 80 - length ) / 2 + 1 ; i++ ) 
	strcat( buf, " " );
    strcat( buf, capitalize( vch->name ) );
    if ( *argument != '\0' )
    {
        strcat( buf, "\n\r\n\r" );
        length = strlen( arg2 );
        for ( i = 0 ; i < ( 80 - length ) / 2 + 1 ; i++ ) strcat( buf, " " );
        strcat( buf, arg2 );
    }

    ed = new_extra_descr();
    strncpy( ed->keyword, "tomb tombstone headstone stone", 30 );
    ed->description = str_dup( buf );
    ed->next = obj->extra_descr;
    obj->extra_descr = ed;

    obj_to_room( obj, ch->in_room );
    get_char_money( ch, cost, ch->in_room->area->world );
    extract_obj( corpse );

    for ( af = vch->affected ; af != NULL ; af = af_next )
    {
	af_next = af->next;
	if ( af->type == -2 )
	    affect_remove( vch, af );
    }

    send_to_area( ch->in_room->area, "You hear a soft trumpet solo, signalling the end of a funeral.\n\r" );
    send_to_char( "With your old body finally buried,  your spirit makes the final\n\rtransition into your new body.\n\r", vch ); 
    if ( vch != ch )
	send_to_char( "You bury the body.\n\r", ch );
}

void do_throw( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int num=0;

    strcpy( buf1, argument );
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Throw what?\n\r", ch );
	return;
    }

    if  ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	sprintf( buf, "You do not have any %s.\n\r", arg );
	send_to_char( buf, ch );
	return;
    }

    if ( obj->pIndexData->vnum == 18048 )
    {
	if ( ch->sex != SEX_FEMALE )
	{
	    send_to_char( "Only a bride can throw the bouque.\n\r", ch );
	    return;
	}

	if ( ch->mate[0] == '\0' )
	{
	    send_to_char( "You are not married yet.\n\r", ch );
	    return;
	}

	victim = NULL;
	for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room ) 
	{
	    if ( vch != ch
	  	&& !IS_NPC( vch )
		&& IS_AWAKE( vch )
		&&  ( vch->mate[0] == '\0' )
		&& vch->sex == SEX_FEMALE )
	    {
		if ( number_range( 0, num ) == 0 )
		    victim = vch;
		num++;
	    }
	}

	if ( victim == NULL )
	{
	    send_to_char( "There is no one to catch it.", ch );
	    return;
	}

	obj_from_char( obj, "D" );
	obj_to_char( obj, victim);

	act( "$n throws $p.", ch, obj, victim, TO_ROOM );
	act( "You throw $p.", ch, obj, victim, TO_CHAR );

	act( "You catch $p!", victim, obj, ch, TO_CHAR );
	act( "$n catches $p.", victim, obj, ch, TO_ROOM );

	return;
    }
    else if ( obj->pIndexData->vnum == 18046 )
    {
	if ( ch->sex != SEX_MALE )
	{
	    send_to_char( "Only a groom can throw the garter.\n\r", ch );
	    return;
	}

	if ( ch->mate[0] == '\0' )
	{
	    send_to_char( "You are not married yet.\n\r", ch );
	    return;
	}

	victim = NULL;
	for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room ) 
	{
	    if ( vch != ch
	  	&& !IS_NPC( vch )
		&& IS_AWAKE( vch )
		&&  ( vch->mate[0] == '\0' )
		&& vch->sex == SEX_MALE )
	    {
		if ( number_range( 0, num ) == 0 )
		    victim = vch;
		num++;
	    }
	}

	if ( victim == NULL )
	{
	    send_to_char( "There is no one to catch it.", ch );
	    return;
	}

	obj_from_char( obj, "E" );
	obj_to_char( obj, victim );

	act( "$n throws $p.", ch, obj, victim, TO_ROOM );
	act( "You throw $p.", ch, obj, victim, TO_CHAR );

	act( "You catch $p!", victim, obj, ch, TO_CHAR );
	act( "$n catches $p.", victim, obj, ch, TO_ROOM );

	return;
    }
    else if ( obj->pIndexData->vnum == 18073 )
    {

	if ( *argument == '\0' )
	{
	    act( "You throw a handfull of rice into the air.", ch, NULL, NULL, TO_CHAR );
	    act( "$n throws a handfull of rice into the air.", ch, NULL, NULL, TO_ROOM );
	    return;
	}
	else if ( ( victim = get_char_room( ch, argument ) ) == NULL )
	{
	    send_to_char( "They are not here.\n\r", ch );
	    return;
	}
	else
	{
	    act( "You throw a handfull of rice at $N.", ch, NULL, victim, TO_CHAR );
	    act( "$n throws a handfull of rice at $N.", ch, NULL, victim, TO_NOTVICT );
	    act( "$n throws a handfull of rice at you.", ch, NULL, victim, TO_VICT );
	    return;
	}

	return;
    }
    else
	do_drop( ch, buf1 );

}

void do_cook( CHAR_DATA * ch, char * argument )
{
    OBJ_DATA *obj;

    if ( *argument == '\0' )
    {
	send_to_char( "Cook what?\n\r", ch );
	return;
    }

    if ( !has_obj_vnum( ch, OBJ_VNUM_COOKING_KIT ) )
    {
	send_to_char( "You don't have anything to cook it with.\n\r", ch );
	return;
    }

    for ( obj = ch->carrying; obj != NULL && ( obj->light_data == NULL || !IS_SET( obj->light_data->flags, LIGHT_BURNING ) || !IS_SET( obj->light_data->flags, LIGHT_LIT|LIGHT_MAGIC_LIT ) ) ; obj = obj->next_content ); 

    if ( obj == NULL )
        for ( obj = ch->in_room->contents; obj != NULL && ( obj->light_data == NULL || !IS_SET( obj->light_data->flags, LIGHT_BURNING ) || !IS_SET( obj->light_data->flags, LIGHT_LIT|LIGHT_MAGIC_LIT ) ) ; obj = obj->next_content ); 

    if ( obj == NULL )
    {
	send_to_char( "You need a fire to cook it on.\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, argument ) ) == NULL
	&& ( obj = get_obj_here( ch, argument ) ) == NULL )
    {
	send_to_char( "You don't have that.\n\r", ch );
	return;
    }

	if ( obj->edible_data == NULL || !IS_SET( obj->edible_data->flags, EDIBLE_FOOD ) )
	{
	    send_to_char( "You cannot cook that.\n\r", ch );
	    return;
	}
	else if ( !IS_SET( obj->edible_data->flags, FOOD_UNCOOKED ) )
	{
	    act( "$n heats up $p.", ch, obj, obj, TO_ROOM );
	    act( "You heat up $p.", ch, obj, obj, TO_CHAR );
	    return;
 	}

    if ( get_skill( ch, gsn_cooking ) < number_percent() )
    {
    	check_improve( ch, gsn_cooking, FALSE, 1 );
	act( "$n burns $p.", ch, obj, obj, TO_ROOM );
	act( "You burn $p.", ch, obj, obj, TO_CHAR );
	extract_obj( obj );
	return;
    }
    check_improve( ch, gsn_cooking, TRUE, 1 );

    act( "$n cooks $p.", ch, obj, obj, TO_ROOM );
    act( "You cook $p.", ch, obj, obj, TO_CHAR);
    REMOVE_BIT( obj->edible_data->flags, FOOD_UNCOOKED );
    obj->wear_data->weight = 1;
}

void do_repair( CHAR_DATA * ch, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *smithy;
    OBJ_DATA *obj;
    int skill = 0;

    one_argument( argument, arg );

    for ( smithy = ch->in_room->people; smithy != NULL ; smithy = smithy->next_in_room )
    {
 	if ( smithy == ch )
 	continue;

 	if ( !can_see( ch, smithy ) )
 	continue;

 	if ( IS_NPC( smithy ) && IS_SET( smithy->act, ACT_REPAIR ) )
	    break;
    }

    if ( smithy != NULL )
    {
	do_mrepair( ch, smithy, arg );
        return;
    }

    if ( *argument == '\0' )
    {
	send_to_char( "Repair what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, argument ) ) == NULL )
    {
	send_to_char( "You don't have that.\n\r", ch );
	return;
    }

    if ( obj->wear_data == NULL )
    {
	send_to_char( "You cannot repair that.\n\r", ch );
	return;
    }

    if ( !has_obj_vnum( ch, OBJ_VNUM_REPAIR_KIT ) )
    {
	send_to_char( "You don't have a repair kit.\n\r", ch );
	return;
    }
    
    if ( obj->wear_data->weapon_data != NULL && get_skill( ch, gsn_weapon_smithing ) < 1 )
    {
	send_to_char( "You don't know how to reapir weapons.  You must learn weapon smithing.\n\r", ch );
	return;
    } 

    if ( obj->wear_data->ac != 0 && get_skill( ch, gsn_armor_smithing ) < 1 )
    {
	send_to_char( "You don't know how to reapir armor.  You must learn armor smithing.\n\r", ch );
	return;
    } 

    if ( get_skill( ch, gsn_repair ) < 1 || ( skill = get_skill( ch, gsn_repair ) * ch->level / 100 ) + ( obj->condition ) < number_percent() )
    {
	send_to_char( "You failed.\n\r", ch );
	obj->condition -= number_range( 1, ( 100 - skill ) / 2 );
	if ( obj->condition <= 0 )
	{
	    act( "$p breaks while you attempt to repair it.", ch, obj, obj , TO_CHAR );
	    check_improve( ch, gsn_repair, FALSE, 2 );
	    extract_obj( obj );
	}
	return;
    }

    obj->condition = UMIN( 100 - obj->repaired, obj->condition + skill / 2 );

    if ( obj->condition <= 0 )
    {
        act( "$p breaks while you attempt to repair it.", ch, obj, obj , TO_CHAR );
    	check_improve( ch, gsn_repair, FALSE, 2 );
        extract_obj( obj );
    }

    act( "You repair $p`w.", ch, obj, obj, TO_CHAR );
    check_improve( ch, gsn_repair, TRUE, 2 );
    obj->repaired += 5;
}

void do_borrow( CHAR_DATA *ch, char*argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    long value;
    int type;

    if ( ch->pcdata != NULL && ch->pcdata->debt != NULL )
    {
	send_to_char( "You already owe money.\n\r", ch );
	return;
    }

    if ( ch->level < 5 || ch->pcdata == NULL )
    {
	send_to_char( "Come back in a few levels, kid.\n\r", ch );
        return;
    }

     argument = one_argument( argument, arg1 );
     strcpy( arg2, argument );

    if ( arg1[0] == '\0' || !is_number( arg1 ) || ( value = atoi(arg1) ) < 0 )
    {
	send_to_char( "How much do you wish to borrow?\n\r", ch );
	return;
    }

    if ( ( type = get_coin_type( arg2 ) ) == -1 )
    {
	send_to_char( "Invalid coin type.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL( ch ) )
    {
	send_to_char( "You are given the money (no debt).\n\r", ch );
	ch->gold[ type ] += value;
	return;
    }

    if ( value > ch->level * ch->level * 20 )
    {
	sprintf( buf, "Sorry you cannot borrow more then %d.\n\r", ch->level * ch->level * 20 );
	send_to_char( buf, ch );
	return;
    }

    ch->pcdata->debt = debt_data_alloc();
    ch->pcdata->debt->type = type;
    ch->pcdata->debt->vnum = ch->in_room->vnum;
    ch->pcdata->debt->amount = value * 2;
    ch->gold[ type ] += value;
    ch->pcdata->debt->due = ch->played + 18000 * 3 + current_time - ch->logon;
    ch->pcdata->debt->timer = 0;

    sprintf( buf, "You now owe %ld %s to be payed within 3 hours!\n\r",
	ch->pcdata->debt->amount,
	coin_name[ ch->pcdata->debt->type ] );
    send_to_char( buf, ch );
    return;
}

void do_pay( CHAR_DATA *ch, char*argument )
{
    char buf[MAX_INPUT_LENGTH];
    long value;

    if ( ch->pcdata == NULL || ch->pcdata->debt == NULL )
    {
	send_to_char( "You don't owe money.\n\r", ch );
	return;
    }

    if ( ch->in_room->vnum != ch->pcdata->debt->vnum 
      && get_room_index( ch->pcdata->debt->vnum ) != NULL )
    {
	send_to_char( "You don't owe money here.\n\r", ch );
	return;
    }

    value = 0;

    if ( !str_cmp( argument, "all" ) )
	value = ch->pcdata->debt->amount;
    else if ( *argument == '\0' || !is_number( argument ) )
    {
	send_to_char( "How much do you want to pay?\n\r", ch );
	return;
    }

    if ( is_number( argument ) && ( value = atoi( argument ) ) < 0 )
    {
	send_to_char( "Only positive amounts can be used.\n\r", ch );
	return;
    }

    if ( value > ch->pcdata->debt->amount )
	value = ch->pcdata->debt->amount;

    if ( value > ch->gold[ ch->pcdata->debt->type ] )
    {
	sprintf( buf, "You do not have %ld %s.\n\r", value, coin_name[ ch->pcdata->debt->type ]  );
	send_to_char( buf, ch );
	return;
    }

    ch->gold[ ch->pcdata->debt->type ] -= value;
    ch->pcdata->debt->amount -= value;
    if ( ch->pcdata->debt->amount <= 0 )
    {
	free_debt_data( ch->pcdata->debt);
	ch->pcdata->debt = NULL;
	send_to_char( "You have paid off your debt!\n\r", ch );
	return;
    }

    sprintf( buf, "You have paid %ld %s, and now owe %ld %s.\n\r",
	value,
	coin_name[ ch->pcdata->debt->type ],
	ch->pcdata->debt->amount,
	coin_name[ ch->pcdata->debt->type ]);
    send_to_char( buf, ch );
    return;

}

void do_tame_animal( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA  *victim;
    AFFECT_DATA af;
    OBJ_DATA *tame_food;  
    OBJ_DATA *food_used=NULL;  
    int amount=0;

    one_argument(argument,arg);
    if ( arg[0] == '\0' )
    {
        send_to_char("What do you want to tame?\n\r",ch);
        return;
    }

    if ( !( victim = get_char_room(ch,arg) ) )
    {
        send_to_char("It is not here.\n\r",ch);
        return;
    }

    if ( !IS_SET( victim->form, FORM_ANIMAL ) || IS_AFFECTED( victim, AFF_CONTROL_MIND ) )
    {
	send_to_char( "I think they are pretty tame already!\n\r", ch );
	return;
    }

    /* Check if character has any food to tame the beast with. */
    for ( tame_food = ch->carrying; tame_food; tame_food = tame_food->next_content )
    {
	if ( tame_food->edible_data == NULL || !IS_SET( tame_food->edible_data->flags, EDIBLE_FOOD ) || tame_food->edible_data->amount <= amount )
	    continue;

	amount = tame_food->edible_data->amount;
 	food_used = tame_food;

    }

    if ( amount < 1 )
    {
	send_to_char( "You do not have any food to tame the beast with.\n\r", ch );
	return;
    }

    if ( IS_SET( victim->act, ACT_AGGRESSIVE ) )
    {
	if ( number_percent() > get_skill( ch, gsn_tame_animal ) + get_curr_stat( ch, STAT_CHR ) 
	    || ( ch->level + amount / 10 ) < victim->level
	    || get_skill( ch, gsn_tame_animal ) <= 0 )
	{
	    act("$N rejects the temptation.", ch, NULL, victim, TO_CHAR );
	    return;
	}
	else
	{
	    act("$N has been calmed!", ch, NULL, victim, TO_CHAR );
	    REMOVE_BIT( victim->act, ACT_AGGRESSIVE );
	}
    }

    if ( IS_AFFECTED( victim,     AFF_CONTROL_MIND )
	|| ( ch->level + amount / 10 ) < victim->level
	|| saves_spell( ch->level , victim ) 
	|| get_skill( ch, gsn_tame_animal ) < 2 
	|| number_percent() > get_skill( ch, gsn_tame_animal ) + get_curr_stat( ch, STAT_CHR ) )
    { 
	act("$N rejects the food.", ch, NULL, victim, TO_CHAR );
	if ( number_percent() > get_skill( ch, gsn_tame_animal ) + get_curr_stat( ch, STAT_CHR ) 
	  && !IS_IMMORTAL ( ch ) )
	    multi_hit( victim, ch, TYPE_UNDEFINED ); 
	return;
    }

    if ( victim->master != NULL )
	stop_follower( victim );
    add_follower( victim, ch );
    victim->leader = ch;
    af.type      = gsn_tame_animal;
    af.duration  = number_fuzzy( ch->level / 4 + amount / 3 );
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_CONTROL_MIND;
    af.bit_type  = BIT_AFFECT;
    affect_to_char( victim, &af );

    send_to_char( "Ok.\n\r", ch );
    act( "Isn't $n just so nice?", ch, NULL, victim, TO_VICT );
    if( IS_SET(victim->act, ACT_MOUNT ) )
    	ch->mount = victim;

    /* Food is used up */
    extract_obj( food_used );

    return;
}

void do_empty( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj_next;
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;

    if ( *argument == '\0' )
    {
	send_to_char( "Empty what?\n\r", ch );
	return;
    }

    if ( ( in_obj = get_obj_carry( ch, argument ) ) == NULL )
    {
	send_to_char( "You are not carrying that.\n\r", ch );
	return;
    }

    if ( in_obj->cont_data == NULL && ( in_obj->edible_data == NULL || IS_SET( in_obj->edible_data->flags, FOOD_EDIBLE ) ) )
    {
	send_to_char( "You cannot empty that.\n\r", ch );
	return;
    }

    if ( in_obj->cont_data != NULL )
    {
	for ( obj = in_obj->cont_data->contents ; obj != NULL ; obj = obj_next )
 	{
	    obj_next = obj->next_content;

	    if ( !can_see_obj( ch, obj )
	      || IS_SET( obj->extra_flags, ITEM_NODROP ) )
		continue;

	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}
    }

    if ( in_obj->edible_data != NULL && !IS_SET( in_obj->edible_data->flags, FOOD_EDIBLE ) )
    {
	in_obj->edible_data->amount = 0;
    }

    act( "You empty $p.", ch, in_obj, in_obj, TO_CHAR );
    act( "$n empties $p.", ch, in_obj, in_obj, TO_ROOM );
}

void do_get( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    bool found;
    int count = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"from"))
	argument = one_argument(argument,arg2);

    /* Get type. */
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Get what?\n\r", ch );
	return;
    }

    if ( arg2[0] == '\0' )
    {
	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{
	    /* 'get obj' */
	    obj = get_obj_list( ch, arg1, ch->in_room->contents );
	    if ( obj == NULL || ( IS_SET( obj->extra_flags, ITEM_BURIED) && !IS_IMMORTAL(ch) ) )
	    {
		act( "I see no $T here.", ch, NULL, arg1, TO_CHAR );
		return;
	    }
	
   	    REMOVE_BIT( obj->extra_flags, ITEM_NO_SHOW );
	    get_obj( ch, obj, NULL );
	}
	else
	{
	    /* 'get all' or 'get all.obj' */
	    found = FALSE;
	    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
		&&   can_see_obj( ch, obj ) && !IS_SET( obj->extra_flags, ITEM_NO_SHOW ) && !IS_SET( obj->extra_flags, ITEM_BURIED ) )
		{
		    count++;
		    found = TRUE;
		    get_obj( ch, obj, NULL );
		    if ( count > 20 )
			break;
		}
	    }

	    if ( !found ) 
	    {
		if ( arg1[3] == '\0' )
		    send_to_char( "I see nothing here.\n\r", ch );
		else
		    act( "I see no $T here.", ch, NULL, &arg1[4], TO_CHAR );
	    }
	}
    }
    else
    {
	/* 'get ... container' */
	if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}

	if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	    return;
	}

	if ( container->cont_data == NULL )
	{
	    send_to_char( "That is not a container.\n\r", ch );
	    return;
	}

	if ( !can_loot( ch, container ) )
	{
	    send_to_char( "You can't do that.\n\r", ch );
	    return;
	}

	if ( IS_SET(container->cont_data->flags, CONT_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	    return;
	}


	if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
	{

		obj = get_obj_list( ch, arg1, container->cont_data->contents );
		if ( obj == NULL )
		{
		    act( "I see nothing like that in the $T.",
			    ch, NULL, arg2, TO_CHAR );
		    return;
		}
		get_obj( ch, obj, container );

		REMOVE_BIT( obj->extra_flags, ITEM_BURIED );
		REMOVE_BIT( obj->extra_flags, ITEM_NO_SHOW );
	    /* 'get obj container perm_save' */
	}
	else
	{

	/* 'get all container' or 'get all.obj container' */
	found = FALSE;
	for ( obj = container->cont_data->contents; obj != NULL; obj = obj_next )
		{
			obj_next = obj->next_content;
			if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
			&&   can_see_obj( ch, obj ) && !IS_SET( obj->extra_flags, ITEM_NO_SHOW ) )
			{
				found = TRUE;
				get_obj( ch, obj, container );
			}
		}
	
		if ( !found )
		{
			if ( arg1[3] == '\0' )
			act( "I see nothing in the $T.",
			ch, NULL, arg2, TO_CHAR );
			else
			act( "I see nothing like that in the $T.",
			ch, NULL, arg2, TO_CHAR );
		}
	}	
   }	
	
	return;	
}	
	
void do_put( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *container;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *obj_in;
    OBJ_DATA *obj1;
    OBJ_DATA *obj1_next;
    OBJ_DATA *obj_cont;
    int weight = 0;
    int count = 0;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if (!str_cmp(arg2,"in"))
	argument = one_argument(argument,arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Put what in what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "all" ) || !str_prefix( "all.", arg2 ) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return;
    }

    if ( ( container = get_obj_here( ch, arg2 ) ) == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg2, TO_CHAR );
	return;
    }

    if ( container->cont_data == NULL )
    {
	send_to_char( "That's not a container.\n\r", ch );
	return;
    }

    if ( IS_SET(container->cont_data->flags, CONT_CLOSED) )
    {
	act( "The $d is closed.", ch, NULL, container->name, TO_CHAR );
	return;
    }

    for ( obj_in = container->cont_data->contents ; obj_in != NULL ; obj_in = obj_in->next_content ) 
    {
	if ( IS_SET(container->cont_data->flags, CONT_HOLDING) )
	    weight += get_obj_weight( obj_in ) / 2;
	else if ( IS_SET(container->cont_data->flags, CONT_EXTRA_HOLDING) )
	    weight += get_obj_weight( obj_in ) / 4;
	else if ( IS_SET(container->cont_data->flags, CONT_SUPER_HOLDING) )
	    weight += get_obj_weight( obj_in ) / 8;
	else
	    weight += get_obj_weight( obj_in );
    }

    if ( str_cmp( arg1, "all" ) && str_prefix( "all.", arg1 ) )
    {
	/* 'put obj container' */
	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( obj == container )
	{
	    send_to_char( "You can't fold it into itself.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj, FALSE ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}

	if ( get_obj_weight( obj ) + weight
	     > container->cont_data->capacity )
	{
	    send_to_char( "It won't fit.\n\r", ch );
	    return;
	}

	if ( container->in_room != NULL && IS_IMMORTAL( ch ) && !IS_OPTIVISION( ch ) )
	{
	    send_to_char( "You can't put items in a container that is not in your inventory.\n\r", ch );
	    return;
	}

    	if ( container->in_room != NULL && IS_SET( container->in_room->room_flags_2, ROOM_SAVE ) )
    	{
            for ( obj1 = container->in_room->contents ; obj1 != NULL ; obj1 = obj1_next )
            {
            	obj1_next = obj1->next_content;
            	count++;

                if ( obj1->cont_data != NULL )
                    for ( obj_cont = obj1->cont_data->contents; obj_cont != NULL; obj_cont = obj_cont->next_content )
                    	count++;

            	if ( count >= 50 )
            	{
                    send_to_room( obj1->in_room->vnum, "`WThis room is full and you can no longer fit more items here.\n\r" );
                	return;
            	}
            }
    	}

	obj_from_char( obj, "F" );
	
	obj_to_obj( obj, container );
	act( "$n puts $p `win $P`w.", ch, obj, container, TO_ROOM );
	act( "You put $p `win $P`w.", ch, obj, container, TO_CHAR );
    }
    else
    {
	/* 'put all container' or 'put all.obj container' */
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg1[3] == '\0' || is_name( &arg1[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_data != NULL
	    &&   obj->wear_data->wear_loc == WEAR_NONE
	    &&   obj != container
	    &&   can_drop_obj( ch, obj, TRUE )
	    &&   get_obj_weight( obj ) + weight
		 <= container->cont_data->capacity )
	    {
	    	if ( get_obj_weight( obj ) + weight > container->cont_data->capacity )
	    	{
	    	    send_to_char( "It won't fit.\n\r", ch );
	    	    break;
	    	}

		count++;
		obj_from_char( obj, "G" );
		obj_to_obj( obj, container );
		act( "$n puts $p `win $P`w.", ch, obj, container, TO_ROOM );
		act( "You put $p `win $P`w.", ch, obj, container, TO_CHAR );

     		if ( count > 20 )
	    	    break;
	    }
	}
    }

    return;
}


void do_drop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    bool found, too_many = FALSE, trig;
    int	count = 0;
    int value = 0;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Drop what?\n\r", ch );
	return;
    }

    for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj->next_content ) value++;

    if ( value >= 400 )
    {
	send_to_char( "There is no room to drop anything here.\n\r", ch );
	return;
    }

    if ( IS_IMMORTAL( ch ) && !IS_OPTIVISION( ch ) )
    {
	send_to_char( "You cannot drop items, if you wish to purge an item, eat it.\n\r", ch );
	return;
    }

    if ( is_number( arg ) )
    {
	/* 'drop NNNN coins' */
	int amount;
	int coin_type;

	amount   = atoi(arg);
	argument = one_argument( argument, arg );
	if ( !str_cmp( arg, "coins" ) || !str_cmp( arg, "coin" ) ||
	    !str_cmp( arg, "gold"  ) )
	    coin_type = race_table[ ch->race ][ ch->sex ].world; 
	else
	    coin_type = get_coin_type( arg );
	if ( amount <= 0 || coin_type < 0 )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	if ( ch->gold[ coin_type ]< amount )
	{
	    sprintf( buf, "You don't have that many %s.\n\r", coin_names[ coin_type ] );
	    send_to_char( buf , ch );
	    return;
	}

	ch->gold[ coin_type ] -= amount;

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_data == NULL 
	      || ( coin_type == 0 && !CAN_WEAR( obj, ITEM_MONEY_0 ) )
	      || ( coin_type == 1 && !CAN_WEAR( obj, ITEM_MONEY_1 ) )
	      || ( coin_type == 2 && !CAN_WEAR( obj, ITEM_MONEY_2 ) )
	      || ( coin_type == 3 && !CAN_WEAR( obj, ITEM_MONEY_3 ) ) )
		continue;

	    switch ( obj->pIndexData->vnum )
	    {
	    case OBJ_VNUM_MONEY_ONE:
		amount += 1;
		extract_obj( obj );
		break;

	    case OBJ_VNUM_MONEY_SOME:
		amount += obj->wear_data->cost;
		extract_obj( obj );
		break;
	    }
	}

	sprintf( buf, "$n drops some %s.\n\r", coin_names[ coin_type ] );
	act( buf, ch, NULL, NULL, TO_ROOM );
	send_to_char( "OK.\n\r", ch );
	obj_to_room( create_money( amount, coin_type ), ch->in_room );
	return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
	/* 'drop obj' */
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	if ( !can_drop_obj( ch, obj, TRUE ) )
	{
	    send_to_char( "You can't let go of it.\n\r", ch );
	    return;
	}
        
	act( "$n drops $p.`w", ch, obj, NULL, TO_ROOM );
	act( "You drop $p.`w", ch, obj, NULL, TO_CHAR );
	obj_from_char( obj, "H" );
	obj_to_room( obj, ch->in_room );
	trig = do_trigger( ch, ch, obj, NULL, TRIG_DROP, TRIG_TYPE_MOB);
	trig = do_trigger( obj, ch, obj, NULL, TRIG_DROP, TRIG_TYPE_OBJ);
	trig = do_trigger( ch->in_room, ch, obj, NULL, TRIG_DROP, TRIG_TYPE_ROOM);
    }
    else
    {
	/* 'drop all' or 'drop all.obj' */
	found = FALSE;
	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;

	    if ( ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
	    &&   can_see_obj( ch, obj )
	    &&   obj->wear_data != NULL
	    &&   obj->wear_data->wear_loc == WEAR_NONE
	    &&   can_drop_obj( ch, obj, TRUE ) )
	    {
		count++;
		value++;
		found = TRUE;
		act( "$n drops $p.`w", ch, obj, NULL, TO_ROOM );
		act( "You drop $p.`w", ch, obj, NULL, TO_CHAR );
		obj_from_char( obj,"I" );
		obj_to_room( obj, ch->in_room );
		trig = do_trigger( ch, ch, obj, NULL, TRIG_DROP, TRIG_TYPE_MOB);
		trig = do_trigger( obj, ch, obj, NULL, TRIG_DROP, TRIG_TYPE_OBJ);
		trig = do_trigger( ch->in_room, ch, obj, NULL, TRIG_DROP, TRIG_TYPE_ROOM);
		if ( count > 20 || value >= 100 )
		{
		    too_many = TRUE;
		    break;
		}
	    }
	}

	if ( !found )
	{
	    if ( arg[3] == '\0' )
		act( "You are not carrying anything.",
		    ch, NULL, arg, TO_CHAR );
	    else
		act( "You are not carrying any $T.",
		    ch, NULL, &arg[4], TO_CHAR );
	}
	if ( too_many )
	{
	if (value >= 100 ) 
	act( "There is no more room to fit any more $T.",
	    ch, NULL, arg, TO_CHAR );
	else
	act( "You take a break from droping $T.",
	    ch, NULL, arg, TO_CHAR );
	}
	
    }

    return;
}


void do_give( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;
    bool trig;
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Give what to whom?\n\r", ch );
	return;
    }

    if ( is_number( arg1 ) )
    {
	/* 'give NNNN coins victim' */
	int amount;
	int coin_type;

	amount   = atoi(arg1);
	if ( !str_cmp( arg2, "coins" ) || !str_cmp( arg2, "coin" ) ||
	    !str_cmp( arg2, "gold"  ) )
	    coin_type = race_table[ ch->race ][ ch->sex ].world; 
	else
	    coin_type = get_coin_type( arg2 );
	if ( amount <= 0 || coin_type < 0 )
	{
	    send_to_char( "Sorry, you can't do that.\n\r", ch );
	    return;
	}

	argument = one_argument( argument, arg2 );
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Give what to whom?\n\r", ch );
	    return;
	}

	if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

    	if ( IS_IMMORTAL( ch ) && !IS_OPTIVISION( ch ) && !IS_IMMORTAL( victim ) )
    	{
	    send_to_char( "You cannot give items to mortal player.\n\r", ch );
	    return;
    	}

	if ( ch->gold[ coin_type ] < amount )
	{
	    sprintf( buf, "You don't have that many %s.\n\r", coin_names[ coin_type ] );
	    send_to_char( buf , ch );
	    return;
	}

	ch->gold[ coin_type ]     -= amount;
	victim->gold[ coin_type ] += amount;
        if ( amount == 1 )
	{
	    sprintf(buf,"$n gives you a %s.", coin_name[ coin_type ] );
	    act( buf, ch, NULL, victim, TO_VICT    );
	    sprintf(buf,"$n gives $N a %s.", coin_name[ coin_type ] );
	    act( buf,  ch, NULL, victim, TO_NOTVICT );
	    sprintf(buf,"You give $N a %s.", coin_name[ coin_type ] );
	    act( buf, ch, NULL, victim, TO_CHAR    );
	}
	else
	{
	    sprintf(buf,"$n gives you %d %s.",amount, coin_names[ coin_type ] );
	    act( buf, ch, NULL, victim, TO_VICT    );
	    sprintf(buf,"$n gives $N some %s.", coin_names[ coin_type ] );
	    act( buf,  ch, NULL, victim, TO_NOTVICT );
	    sprintf(buf,"You give $N %d %s.",amount, coin_names[ coin_type ] );
	    act( buf, ch, NULL, victim, TO_CHAR    );
	}
    	trig = do_trigger( ch, victim, NULL, NULL, TRIG_GIVE, TRIG_TYPE_MOB);
    	trig = do_trigger( ch->in_room, ch, NULL, NULL, TRIG_GIVE, TRIG_TYPE_ROOM);
    	trig = do_trigger( victim, ch, NULL ,NULL, TRIG_GETS, TRIG_TYPE_MOB);
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if ( obj->wear_data != NULL && obj->wear_data->wear_loc != WEAR_NONE )
    {
	send_to_char( "You must remove it first.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    if ( IS_IMMORTAL( ch ) && !IS_OPTIVISION( ch ) && !IS_IMMORTAL( victim ) )
    {
    	send_to_char( "You cannot give items to mortal player.\n\r", ch );
    	return;
    }

    if ( !can_drop_obj( ch, obj, FALSE ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( victim->carry_number >= can_carry_n( victim ) )
    {
	act( "$N has $S hands full.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( victim->carry_weight + get_obj_weight( obj ) > can_carry_w( victim ) )
    {
	act( "$N can't carry that much weight.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( !can_see_obj( victim, obj ) )
    {
	act( "$N can't see it.", ch, NULL, victim, TO_CHAR );
	return;
    }
    act( "$n gives $p to $N.", ch, obj, victim, TO_NOTVICT );
    act( "$n gives you $p.",   ch, obj, victim, TO_VICT    );
    act( "You give $p to $N.", ch, obj, victim, TO_CHAR    );
    obj_from_char( obj, "J" );
    obj_to_char( obj, victim);
    trig = do_trigger( victim, ch, obj, NULL, TRIG_GETS, TRIG_TYPE_MOB);
    trig = do_trigger( ch, victim, obj, NULL, TRIG_GIVE, TRIG_TYPE_MOB);
    trig = do_trigger( obj, ch, obj, NULL, TRIG_GIVE, TRIG_TYPE_OBJ);
    trig = do_trigger( ch->in_room, ch, obj, NULL, TRIG_GIVE, TRIG_TYPE_ROOM);
    if ( obj->owner[0] == '\0' || is_name( ch->name, obj->owner ) )
    {
        if ( !IS_NPC( victim ) )
            strncpy(obj->owner ,victim->name,30 );
        else
	    obj->owner[0] = '\0';
    }
    return;
}




void do_fill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fountain;
    SPELL_LIST * spell;
    SPELL_LIST * spell_next;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Fill what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    found = FALSE;
    for ( fountain = ch->in_room->contents; fountain != NULL;
	fountain = fountain->next_content )
    {
	if ( fountain->edible_data != NULL && !IS_SET( fountain->edible_data->flags, FOOD_EDIBLE )
	    && fountain->edible_data->max_amount < 0 )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
    {
	send_to_char( "There is no fountain here!\n\r", ch );
	return;
    }

    if ( obj->edible_data == NULL || IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) )
    {
	send_to_char( "You can't fill that.\n\r", ch );
	return;
    }

    if ( obj->edible_data->amount != 0 
      && obj->edible_data->type != fountain->edible_data->type )
    {
	send_to_char( "There is already another liquid in it.\n\r", ch );
	return;
    }

    if ( obj->edible_data->amount >= obj->edible_data->max_amount )
    {
	send_to_char( "Your container is full.\n\r", ch );
	return;
    }

    act( "You fill $p.", ch, obj, NULL, TO_CHAR );

    if ( obj->edible_data->amount > 0 )
	SET_BIT( obj->edible_data->flags, ( fountain->edible_data->flags & FOOD_POISONED ) );
    else
    {
	obj->edible_data->type = fountain->edible_data->type;
	obj->edible_data->flags = fountain->edible_data->flags;
    }

    for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell_next )
    {
	spell_next = spell->next;
	free_spell( spell );
    }
    obj->edible_data->spells = NULL;

    for ( spell = fountain->edible_data->spells ; spell != NULL ; spell = spell->next )
    {
	spell_next = spell_alloc();
	spell_next->spell = spell->spell;
	spell_next->level = spell->level;
	spell_next->next = obj->edible_data->spells;
	obj->edible_data->spells = spell_next;
    }

    obj->edible_data->amount = obj->edible_data->max_amount;
    return;
}



void do_drink( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int amount, x;
    int liquid;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 );
    if ( arg[0] == '\0' )
    {
	for ( obj = ch->in_room->contents; obj; obj = obj->next_content )
	{
	    if ( obj->edible_data != NULL && !IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) 
		&& obj->edible_data->max_amount < 0 )
		break;
	}

	if ( obj == NULL )
	{
	    send_to_char( "Drink what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( obj = get_obj_here( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    if ( obj->edible_data == NULL || IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) )
    {
	send_to_char( "You cannot drink that!\n\r", ch );
	return;
    }

    if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK] > 10 )
    {
	send_to_char( "You fail to reach your mouth.  *Hic*\n\r", ch );
	return;
    }
	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_THIRST] >= race_table[ ch->race ][ ch->sex ].max_condition[COND_THIRST] )
	{   
	    send_to_char( "You are too full to drink more.\n\r", ch );
	    return;
	}

	if ( obj->edible_data->amount == 0 )
	{
	    send_to_char( "It is already empty.\n\r", ch );
	    return;
	}

	if ( ( liquid = obj->edible_data->type ) >= LIQ_MAX )
	{
	    bug( "Do_drink: bad liquid number %d.", liquid );
	    send_to_char( "Bad liquid.\n\r", ch );
	    return;
	}

	if ( arg1[0] == '\0' )
	    amount = 1;
    	else if ( !IS_NPC( ch ) && !strncmp( arg1, "full",1) )
	    amount = race_table[ ch->race ][ ch->sex ].max_condition[COND_THIRST] - ch->pcdata->condition[COND_THIRST];
	else if ( !is_number( arg1 ) )
	{
	    send_to_char("Drink how many portions?\n\r", ch);
	    return;
	}
	else
	    amount = atoi(arg1);

	if ( obj->edible_data->max_amount != -1 )
	    if ( obj->edible_data->amount < amount )
	    	amount = obj->edible_data->amount;

	
	if ( obj->edible_data->max_amount != -1 )
	    obj->edible_data->amount -= amount;
	
	act( "$n `wdrinks $T `wfrom $p`w.",
	    ch, obj, liq_table[liquid].liq_name, TO_ROOM );

	if ( arg1[0] == '\0' )
	    act( "You drink $T `wfrom $p`w.",
	    	ch, obj, liq_table[liquid].liq_name, TO_CHAR );
	else
	{
	    if ( amount > 1 )
	    	sprintf( buf, "You drink %d portions of %s`w.\n\r", amount, liq_table[liquid].liq_name );
	    else
	    	sprintf( buf, "You drink %d portion of %s`w.\n\r", amount, liq_table[liquid].liq_name );
	    send_to_char(buf, ch);
	}

	for ( x = 0; x < amount; x++ )
	{
	    gain_condition( ch, COND_DRUNK,
	    	1 * liq_table[liquid].liq_affect[COND_DRUNK  ] );
	    gain_condition( ch, COND_FULL,
	    	1 * liq_table[liquid].liq_affect[COND_FULL   ] );
	    gain_condition( ch, COND_THIRST,
	    	1 * liq_table[liquid].liq_affect[COND_THIRST ] );

	    if ( obj->edible_data->spells != NULL )
	    {
		obj_cast_spell( ch, ch, NULL, obj->edible_data->spells, 0 );
		
		if ( obj->edible_data->max_amount != -1 && obj->edible_data->amount == 0 )
		{
	    	    act( "$p `wfades away.", ch, obj, NULL, TO_CHAR );
		    extract_obj( obj );
		}
	    }

	    if ( IS_SET( obj->edible_data->flags, FOOD_POISONED ) )
	    {
	    	/* The shit was poisoned ! */
	    	AFFECT_DATA af;

	    	act( "$n chokes and gags.", ch, NULL, NULL, TO_ROOM );
	    	send_to_char( "You choke and gag.\n\r", ch );
	    	af.type      = gsn_poison;
	    	af.level	 = number_fuzzy(amount); 
	    	af.duration  = 3 * amount;
	    	af.location  = APPLY_AFFECT;
	    	af.modifier  = 0;
	    	af.bitvector = AFF_POISON;
	    	af.bit_type  = BIT_AFFECT;
	    	affect_join( ch, &af );
	    }
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_DRUNK]  == race_table[ ch->race ][ ch->sex ].max_condition[COND_DRUNK] )
	    	send_to_char( "You feel drunk.\n\r", ch );

    return;
}

void do_eat( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    int amount;
    OBJ_DATA *obj;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Eat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    amount = 1000;
    if ( !IS_IMMORTAL(ch) )
    {
	if ( obj->edible_data == NULL || !IS_SET( obj->edible_data->flags, EDIBLE_FOOD ) )
	{
	    send_to_char( "That's not edible.\n\r", ch );
	    return;
	}
	else if ( IS_SET( obj->edible_data->flags, FOOD_UNCOOKED ) )
	{
	    send_to_char( "It needs to be cooked first.\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) && ch->pcdata->condition[COND_FULL] >= race_table[ ch->race ][ ch->sex ].max_condition[COND_FULL] )
	{   
	    send_to_char( "You are too full to eat more.\n\r", ch );
	    return;
	}
    }
    else if ( obj->edible_data == NULL )
    {
        act( "$n eats $p.",  ch, obj, NULL, TO_ROOM );
        act( "You eat $p.", ch, obj, NULL, TO_CHAR );
	extract_obj( obj );
	return;
    }

    act( "$n eats $p.",  ch, obj, NULL, TO_ROOM );
    act( "You eat $p.", ch, obj, NULL, TO_CHAR );

	if ( !IS_NPC(ch) && !IS_IMMORTAL(ch) ) /* for simulating */
	{
	    int condition;

	    condition = ch->pcdata->condition[COND_FULL];
	    amount = UMIN( obj->edible_data->amount, race_table[ ch->race ][ ch->sex ].max_condition[COND_FULL] - condition );
	    gain_condition( ch, COND_FULL, amount );
	    if ( condition == 0 && ch->pcdata->condition[COND_FULL] > 0 )
		send_to_char( "You are no longer hungry.\n\r", ch );
	    else if ( ch->pcdata->condition[COND_FULL] == race_table[ ch->race ][ ch->sex ].max_condition[COND_FULL] )
		send_to_char( "You are full.\n\r", ch );
	}

	if ( IS_SET( obj->edible_data->flags, FOOD_POISONED ) )
	{
	    /* The shit was poisoned! */
	    AFFECT_DATA af;

	    act( "$n chokes and gags.", ch, 0, 0, TO_ROOM );
	    send_to_char( "You choke and gag.\n\r", ch );

	    af.type      = gsn_poison;
	    af.level 	 = number_fuzzy(obj->edible_data->amount);
	    af.duration  = 2 * obj->edible_data->amount;
	    af.location  = APPLY_AFFECT;
	    af.modifier  = 0;
	    af.bitvector = AFF_POISON;
	    af.bit_type  = BIT_AFFECT;
	    affect_join( ch, &af );
	}

	if ( obj->edible_data->spells != NULL && amount > 0 )
	    obj_cast_spell( ch, ch, NULL, obj->edible_data->spells, 0 );

    if ( obj->edible_data->max_amount > 0 
      && ( obj->edible_data->amount -= amount ) < 1 )
        extract_obj( obj );
    return;
}



/*
 * Remove an object.
 */
bool remove_obj( CHAR_DATA *ch, int iWear, bool fReplace )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	return TRUE;

    if ( !fReplace )
	return FALSE;

    if ( IS_SET(obj->extra_flags, ITEM_NOREMOVE) )
    {
	act( "You can't remove $p`w.", ch, obj, NULL, TO_CHAR );
	return FALSE;
    }

    unequip_char( ch, obj );
    act( "$n stops using $p`w.", ch, obj, NULL, TO_ROOM );
    act( "You stop using $p`w.", ch, obj, NULL, TO_CHAR );
    return TRUE;
}

bool can_wear_size( CHAR_DATA *ch, OBJ_DATA *obj )
{

    if ( obj->wear_data->size < ch->size && !IS_IMMORTAL(ch) )
    {
    	act( "You cannot wear $p`w, it is too small.", ch, obj, NULL, TO_CHAR );
    	return FALSE;
    }

    if ( obj->wear_data->size > ch->size && !IS_IMMORTAL(ch) ) 
    {
    	act( "You cannot wear $p`w, it is too large.", ch, obj, NULL, TO_CHAR );
    	return FALSE;
    }

    return TRUE;
}

/*
 * Wear one object.
 * Optional replacement of existing objects.
 * Big repetitive code, ick.
 */
void wear_obj( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace )
{
    OBJ_DATA *weapon;

    if ( obj->wear_data == NULL || obj->wear_data->wear_flags == WEAR_NONE )
    {
	act( "You cannot wear $p`w.", ch, obj, NULL, TO_CHAR );
	return;
    }

    if ( ( IS_SET( obj->extra_flags, ITEM_BARDING ) && !IS_SET( ch->act, ACT_MOUNT ) )
      || ( !IS_SET( obj->extra_flags, ITEM_BARDING ) && IS_SET( ch->act, ACT_MOUNT ) ) )
    {
	send_to_char( "You cannot wear this object.\n\r", ch );
	act( "$n tries to use $p`w.",
	    ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LIGHT ) )
    {
	if ( !remove_obj( ch, WEAR_LIGHT, fReplace ) )
	    return;

	act( "$n holds $p`w.", ch, obj, NULL, TO_ROOM );
	act( "You hold $p`w.",  ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LIGHT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FINGER ) )
    {
	if ( get_eq_char( ch, WEAR_FINGER_L ) != NULL
	&&   get_eq_char( ch, WEAR_FINGER_R ) != NULL
	&&   !remove_obj( ch, WEAR_FINGER_L, fReplace )
	&&   !remove_obj( ch, WEAR_FINGER_R, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	if ( get_eq_char( ch, WEAR_FINGER_L ) == NULL )
	{
	    act( "$n wears $p `won $s left finger.",    ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `won your left finger.",  ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_L );
	    return;
	}

	if ( get_eq_char( ch, WEAR_FINGER_R ) == NULL )
	{
	    act( "$n wears $p `won $s right finger.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `won your right finger.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_FINGER_R );
	    return;
	}

	bug( "Wear_obj: no free finger.", 0 );
	send_to_char( "You already wear two rings.\n\r", ch );
	return;
    }
        if ( CAN_WEAR( obj, ITEM_WEAR_EAR ) )
    {
	if ( get_eq_char( ch, WEAR_EAR_L ) != NULL
	&&   get_eq_char( ch, WEAR_EAR_R ) != NULL
	&&   !remove_obj( ch, WEAR_EAR_L, fReplace )
	&&   !remove_obj( ch, WEAR_EAR_R, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	if ( get_eq_char( ch, WEAR_EAR_L ) == NULL )
	{
	    act( "$n wears $p `wthrough $s left ear.",    ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `wthrough your left ear.",  ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_EAR_L );
	    return;
	}

	if ( get_eq_char( ch, WEAR_EAR_R ) == NULL )
	{
	    act( "$n wears $p `wthrough $s right ear.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `wthrough your right ear.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_EAR_R );
	    return;
	}

	bug( "Wear_obj: no free ear lobe.", 0 );
	send_to_char( "You already wear two earrings.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_NECK ) )
    {
	if ( get_eq_char( ch, WEAR_NECK_1 ) != NULL
	&&   get_eq_char( ch, WEAR_NECK_2 ) != NULL
	&&   !remove_obj( ch, WEAR_NECK_1, fReplace )
	&&   !remove_obj( ch, WEAR_NECK_2, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	if ( get_eq_char( ch, WEAR_NECK_1 ) == NULL )
	{
	    act( "$n wears $p `waround $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `waround your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_1 );
	    return;
	}

	if ( get_eq_char( ch, WEAR_NECK_2 ) == NULL )
	{
	    act( "$n wears $p `waround $s neck.",   ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `waround your neck.", ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_NECK_2 );
	    return;
	}

	bug( "Wear_obj: no free neck.", 0 );
	send_to_char( "You already wear two neck items.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_BODY ) )
    {
	if ( !remove_obj( ch, WEAR_BODY, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `won $s body.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `won your body.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_BODY );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HEAD ) )
    {
	if ( !remove_obj( ch, WEAR_HEAD, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `won $s head.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `won your head.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HEAD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_LEGS ) )
    {
	if ( !remove_obj( ch, WEAR_LEGS, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `won $s legs.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `won your legs.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_LEGS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_FEET ) )
    {

	if ( IS_SET( ch->guilds, TATH ) || IS_SET( ch->guilds, MAR_TUUR )
	|| IS_SET( ch->guilds, CLANNGUARD ) )
	{
	    send_to_char( "Your guild doesn't allow you to wear this.\n\r", ch );
	    return;
	}

	if ( !remove_obj( ch, WEAR_FEET, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `won $s feet.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `won your feet.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_FEET );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_HANDS ) )
    {
	if ( IS_SET( ch->guilds, TATH ) || IS_SET( ch->guilds, MAR_TUUR )
	|| IS_SET( ch->guilds, CLANNGUARD ) || IS_SET( ch->guilds, MAJHERE ) )
	{
	    send_to_char( "Your guild doesn't allow you to wear this.\n\r", ch );
	    return;
	}

	if ( !remove_obj( ch, WEAR_HANDS, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `won $s hands.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `won your hands.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HANDS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ARMS ) )
    {
	if ( !remove_obj( ch, WEAR_ARMS, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `won $s arms.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `won your arms.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ARMS );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_ABOUT ) )
    {
	if ( !remove_obj( ch, WEAR_ABOUT, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `wabout $s body.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `wabout your body.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_ABOUT );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WAIST ) )
    {
	if ( !remove_obj( ch, WEAR_WAIST, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	act( "$n wears $p `wabout $s waist.",   ch, obj, NULL, TO_ROOM );
	act( "You wear $p `wabout your waist.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_WAIST );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_WRIST ) )
    {
	if ( get_eq_char( ch, WEAR_WRIST_L ) != NULL
	&&   get_eq_char( ch, WEAR_WRIST_R ) != NULL
	&&   !remove_obj( ch, WEAR_WRIST_L, fReplace )
	&&   !remove_obj( ch, WEAR_WRIST_R, fReplace ) )
	    return;

	if ( !can_wear_size( ch, obj ) )
	    return;

	if ( get_eq_char( ch, WEAR_WRIST_L ) == NULL )
	{
	    act( "$n wears $p `waround $s left wrist.",
		ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `waround your left wrist.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_L );
	    return;
	}

	if ( get_eq_char( ch, WEAR_WRIST_R ) == NULL )
	{
	    act( "$n wears $p `waround $s right wrist.",
		ch, obj, NULL, TO_ROOM );
	    act( "You wear $p `waround your right wrist.",
		ch, obj, NULL, TO_CHAR );
	    equip_char( ch, obj, WEAR_WRIST_R );
	    return;
	}

	bug( "Wear_obj: no free wrist.", 0 );
	send_to_char( "You already wear two wrist items.\n\r", ch );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WEAR_SHIELD ) )
    {
	if ( !IS_SET( obj->extra_flags, ITEM_BUCKLER ) && ( IS_SET( ch->guilds, TATH ) || IS_SET( ch->guilds, MAR_TUUR )
	|| IS_SET( ch->guilds, CLANNGUARD ) ) )
	{
	    send_to_char( "Your guild doesn't allow you to wear this.\n\r", ch );
	    return;
	}
	else if ( IS_SET( ch->guilds, MAJHERE ) )
	{
	    send_to_char( "Your guild doesn't allow you to wear this.\n\r", ch );
	    return;
	}

	if ( !remove_obj( ch, WEAR_SHIELD, fReplace ) )
	    return;

	if ( ch->hands_free == 0 && !IS_SET( obj->extra_flags, ITEM_BUCKLER ) )
	{
  	    if ( ( weapon = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) != NULL 
   	      && is_bastard( ch, weapon ) )
	    {
	        weapon->wear_data->wear_loc = WEAR_WIELD;
	        ch->hands_free++;
		act( "$n wields $p `win one hand.", ch, weapon, NULL, TO_ROOM );
		act( "You wield $p `win one hand.", ch, weapon, NULL, TO_CHAR );

	    }
	    else
	    {
		act( "You do not have any hands free to wear $p`w.", ch, obj, NULL, TO_CHAR );
		return;
	    }
	}

	act( "$n wears $p `was a shield.", ch, obj, NULL, TO_ROOM );
	act( "You wear $p `was a shield.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_SHIELD );
	return;
    }

    if ( CAN_WEAR( obj, ITEM_WIELD ) )
    {
	OBJ_DATA * weapon1;
	int skill;

	if ( ( IS_SET( ch->guilds, CLANNGUARD ) || IS_SET( ch->guilds, MAR_TUUR )
	|| IS_SET( ch->guilds, TATH ) ) || ( IS_SET( ch->guilds, MAJHERE ) 
	&& obj->wear_data->weapon_data != NULL && obj->wear_data->weapon_data->type != 2 ) )
	{
	    send_to_char( "Your guild doesn't allow you to wield this.\n\r", ch );
	    return;
	}

	if ( !IS_NPC(ch) 
	&& get_obj_weight( obj ) > str_app[get_curr_stat(ch,STAT_STR)].wield )
	{
	    send_to_char( "It is too heavy for you to wield.\n\r", ch );
	    return;
	}

	/* required to wield two handed */

	if ( is_two_handed( ch, obj ) )
 	{
	    if ( ch->hands_free < 2 )
	    {
	        act("You need two hands free to wield $p`w.", ch, obj, NULL, TO_CHAR );
	        return;
	    }
	    else
	    {

	        if ( !remove_obj( ch, WEAR_WIELD, fReplace )
		 || !remove_obj( ch, WEAR_WIELD_TWO_HANDED, fReplace) 
	          || !remove_obj( ch, WEAR_WIELD_L, fReplace) )
	    	    return;

	        act( "$n wields $p `win two hands.", ch, obj, NULL, TO_ROOM );
	        act( "You wield $p `win two hands.", ch, obj, NULL, TO_CHAR );
	        equip_char( ch, obj, WEAR_WIELD_TWO_HANDED );

	    }
	}
	/* can be wielded with 2 or 1 hand */
	else if ( is_bastard( ch, obj ) )
	{
	    if ( !remove_obj( ch, WEAR_WIELD, fReplace ) 
	      || !remove_obj( ch, WEAR_WIELD_TWO_HANDED, fReplace) 
	      || !remove_obj( ch, WEAR_WIELD_L, fReplace) )
	        return;


	    if ( ch->hands_free == 0 )
	    {
	        act("You need a free hand to wield $p`w.", ch, obj, NULL, TO_CHAR );
	        return;
	    }
	    else if ( ch->hands_free == 1 )
	    {

	        act( "$n wields $p`w.", ch, obj, NULL, TO_ROOM );
	        act( "You wield $p`w.", ch, obj, NULL, TO_CHAR );
	        equip_char( ch, obj, WEAR_WIELD );

	    }
	    else
	    {
	        act( "$n wields $p `win two hands.", ch, obj, NULL, TO_ROOM );
	        act( "You wield $p `win two hands.", ch, obj, NULL, TO_CHAR );
	        equip_char( ch, obj, WEAR_WIELD_TWO_HANDED );
	    }
	}
	else if ( is_one_handed( ch, obj ) || is_knife_sized( ch, obj ) )
	{
	    if ( get_skill( ch, gsn_dual_wield ) < 1 )
	    {

	        if ( !remove_obj( ch, WEAR_WIELD, fReplace ) || !remove_obj( ch, WEAR_WIELD_TWO_HANDED, fReplace) 
	          || !remove_obj( ch, WEAR_WIELD_L, fReplace) )
	    	    return;

	        if ( ch->hands_free == 0 )
	        {
	            act("You need a free hand to wield $p`w.", ch, obj, NULL, TO_CHAR );
	            return;
	        }

	        act( "$n wields $p`w.", ch, obj, NULL, TO_ROOM );
	        act( "You wield $p`w.", ch, obj, NULL, TO_CHAR );
	        equip_char( ch, obj, WEAR_WIELD );
	    }
  	    else 
	    {
	        if ( !remove_obj( ch, WEAR_WIELD_TWO_HANDED, fReplace) )
	    	    return;

		weapon1 = get_eq_char( ch, WEAR_WIELD_R );

  	        if ( get_obj_weight( obj ) > str_app[get_curr_stat(ch,STAT_STR)].wield/2 
		  || weapon1 == NULL )
 	        {
	            if ( !remove_obj( ch, WEAR_WIELD_R, fReplace ) )
		    {
          	        act( "$p `wis too heavy for you to wield in your other hand.", ch, obj, NULL, TO_CHAR );
	     	        return;
		    }
		    else
		    {
	   	        if ( ch->hands_free == 0 )
	   	        {
	   	             act("You need a free hand to wield $p`w.", ch, obj, NULL, TO_CHAR );
	   	             return;
	   	     	}

		            act( "$n wields $p`w.", ch, obj, NULL, TO_ROOM );
	        	    act( "You wield $p`w.", ch, obj, NULL, TO_CHAR );
	 	            equip_char( ch, obj, WEAR_WIELD_R );
		    }
		}
		else
		{
		    if( !remove_obj( ch, WEAR_WIELD_L, fReplace ) )
		    {
		        if ( !remove_obj( ch, WEAR_WIELD_R, fReplace ) )
		  	    return;
			else
			{
	     	            if ( ch->hands_free == 0 )
	   	            {
	   	             	act("You need a free hand to wield $p`w.", ch, obj, NULL, TO_CHAR );
	   	             	return;
	   	     	    }

		            act( "$n wields $p`w.", ch, obj, NULL, TO_ROOM );
	        	    act( "You wield $p`w.", ch, obj, NULL, TO_CHAR );
	 	            equip_char( ch, obj, WEAR_WIELD_R );
			}
		    }
		    else
		    {
	   	        if ( ch->hands_free == 0 )
	   	        {
	   	             act("You need a free hand to wield $p`w.", ch, obj, NULL, TO_CHAR );
	   	             return;
	   	     	}

		        act( "$n wields $p`w.", ch, obj, NULL, TO_ROOM );
	        	act( "You wield $p`w.", ch, obj, NULL, TO_CHAR );
	 	        equip_char( ch, obj, WEAR_WIELD_L );
		    }
		}
	    }
	}

        skill = get_weapon_skill(ch,obj);
	if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_WIELD_L )
	    skill = ( skill * get_skill( ch, gsn_dual_wield ) );

        if (skill >= 250)
            act("$p `wfeels like a part of you!",ch,obj,NULL,TO_CHAR);
        else if (skill > 200)
            act("You feel quite confident with $p`w.",ch,obj,NULL,TO_CHAR);
        else if (skill > 150)
            act("You are skilled with $p`w.",ch,obj,NULL,TO_CHAR);
        else if (skill > 100)
            act("Your skill with $p `wis adequate.",ch,obj,NULL,TO_CHAR);
        else if (skill > 50)
            act("$p `wfeels a little clumsy in your hands.",ch,obj,NULL,TO_CHAR);
        else if (skill > 15)
            act("You fumble and almost drop $p`w.",ch,obj,NULL,TO_CHAR);
        else
            act("You don't even know which is end is up on $p.",
                ch,obj,NULL,TO_CHAR);

	return;
    }

    if ( CAN_WEAR( obj, ITEM_HOLD ) )
    {
	if ( !remove_obj( ch, WEAR_HOLD, fReplace ) )
	    return;

	if ( ch->hands_free == 0 && !IS_SET( obj->extra_flags, ITEM_BUCKLER ) )
	{
  	    if ( ( weapon = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) != NULL 
   	      && is_bastard( ch, weapon ) )
	    {
	        weapon->wear_data->wear_loc = WEAR_WIELD;
	        ch->hands_free++;
		act( "$n wields $p `win one hand.", ch, weapon, NULL, TO_ROOM );
		act( "You wield $p `win one hand.", ch, weapon, NULL, TO_CHAR );

	    }
	    else
	    {
		act( "You do not have any hands free to hold $p`w.", ch, obj, NULL, TO_CHAR );
		return;
	    }
	}

	act( "$n `wholds $p `win $s `whands.",   ch, obj, NULL, TO_ROOM );
	act( "You hold $p `win your hands.", ch, obj, NULL, TO_CHAR );
	equip_char( ch, obj, WEAR_HOLD );
	return;
    }

    if ( fReplace )
	send_to_char( "You can't wear, wield, or hold that.\n\r", ch );

    return;
}



void do_wear( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Wear, wield, or hold what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE && can_see_obj( ch, obj ) )
	    	wear_obj( ch, obj, FALSE );
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	wear_obj( ch, obj, TRUE );
    }

    return;
}



void do_remove( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int c1;


    one_argument( argument, arg );
       
    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove what?\n\r", ch );
	return;
    }

   
    if ( !strcasecmp(argument,"all") ) 
    {
	int oFound=FALSE;
        for (c1=0;c1<MAX_WEAR;c1++)
   	    if (get_eq_char( ch, c1 )) 
		{
		oFound=TRUE;
		remove_obj( ch, c1, TRUE );
		}
	if (!oFound) send_to_char( "You are not wearing anything.\n\r", ch );
        return;
    }
 
    if ( ( obj = get_obj_wear( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    remove_obj( ch, obj->wear_data->wear_loc, TRUE );
    return;
}



void do_sacrifice( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    int gold;
    int coin_type;
    char * sname;
   /* variables for AUTOSPLIT */
    CHAR_DATA *gch;
    int members;
    char buffer[100];
    sname = sac_name[ number_range( 0, MAX_IMP - 1 ) ];
    one_argument( argument, arg );

    if ( arg[0] == '\0' || !str_cmp( arg, ch->name ) )
    {
	act( "$n offers $mself to the gods, who graciously declines.",
	    ch, NULL, NULL, TO_ROOM );
	send_to_char(
	    "The gods appreciates your offer and may accept it later.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
            do_sacrifice( ch, obj->name );
	}
	return;
    }

    if ( !str_prefix( "all.", arg ) )
    {
	for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( is_name( &arg[4], obj->name ) )
	        do_sacrifice( ch, obj->name );
	}
	return;
    }

    for ( obj = ch->in_room->contents ; obj != NULL && !is_name( arg, obj->name ) ; obj = obj->next_content );

    if ( obj == NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }
    if ( !can_loot( ch, obj ) )
    {
	send_to_char( "Some force prevents you!\n\r", ch );
	return;
    }

    if ( obj->cont_data != NULL )
	if ( obj->cont_data->contents != NULL 
	 || obj->pIndexData->vnum == 11 ) 
    {
	send_to_char( "That may not be a good idea...\n\r", ch );
	return;
    }


    if ( !CAN_WEAR(obj, ITEM_TAKE))
    {
	act( "$p is not an acceptable sacrifice.", ch, obj, 0, TO_CHAR );
	return;
    }

    if ( IS_SET( obj->extra_flags, ITEM_NOPURGE ) )
    {
	act( "$p is not an acceptable sacrifice.", ch, obj, 0, TO_CHAR );
	return;
    }

    gold = 0;
    if ( obj->wear_data != NULL )
        gold = UMAX(1,obj->points * 3);

    if ( obj->pIndexData->vnum != 11 && obj->pIndexData->vnum != 10 )
    	gold = UMIN(gold,obj->wear_data->cost);

    coin_type = race_table[ ch->race ][ ch->sex ].world;

    if (gold == 1)
    {
	sprintf(  buf, "%s gives you one %s for your sacrifice of $p`w.", sname,  coin_name[ coin_type ] );
	        act( buf, ch, obj, NULL, TO_CHAR );
    }
    else
    {
	sprintf(buf,"%s gives you %d %s for your sacrifice of $p.",sname,   gold, coin_names[ coin_type ] );
        act( buf, ch, obj, NULL, TO_CHAR );
    }

    ch->gold[ coin_type ] += gold;
    
    if (IS_SET(ch->plr,PLR_AUTOSPLIT) )
    { /* AUTOSPLIT code */
    	members = 0;
	for (gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    	{
    	    if ( is_same_group( gch, ch ) )
            members++;
    	}

	if ( members > 1 && gold > 1)
	{
	    sprintf(buffer,"%d %s",gold, coin_names[ coin_type ] );
	    do_split(ch,buffer);	
	}
    }

    extract_obj( obj );
    return;
}

void do_recite( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *scroll;
    OBJ_DATA *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( ( scroll = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that scroll.\n\r", ch );
	return;
    }

    if ( scroll->wear_data->magic_data == NULL || 
	scroll->wear_data->magic_data->type != MAGIC_SCROLL )
    {
	send_to_char( "You can recite only scrolls.\n\r", ch );
	return;
    }

    obj = NULL;
    if ( arg2[0] == '\0' )
    {
	victim = ch;
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg2 ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    act( "$n recites $p.", ch, scroll, NULL, TO_ROOM );
    act( "You recite $p.", ch, scroll, NULL, TO_CHAR );

    if (number_percent() >= 20 + get_skill(ch,gsn_scrolls) * 4/5)
    {
	send_to_char("You mispronounce a syllable.\n\r",ch);
	check_improve(ch,gsn_scrolls,FALSE,2);
    }

    else
    {
    	obj_cast_spell( ch, victim, obj, scroll->wear_data->magic_data->spells, MAGIC_SCROLL );
	check_improve(ch,gsn_scrolls,TRUE,2);
    }
   
    scroll->wear_data->magic_data->charges -=1; 
    if ( scroll->wear_data->magic_data->max_charges >= 0 && scroll->wear_data->magic_data->charges <= 0 )
    {
	    act( "$p disappear's `was you recite it.\n\r", ch, scroll, NULL, TO_CHAR );
	    extract_obj( scroll );

/*	if ( IS_SET( scroll->wear_data->magic_data->flags, ROT_USED ) )
	    extract_obj( scroll );
	else
	{
	    SPELL_LIST * spell;

	    act( "The words disappear from the $p `was you read it.\n\r", ch, scroll, NULL, TO_CHAR );
	    for ( spell = scroll->wear_data->magic_data->spells ; spell != NULL ; spell = scroll->wear_data->magic_data->spells )
	    {
		scroll->wear_data->magic_data->spells = spell->next;
		free_spell( spell );
	    }

	    free_magic_data(scroll->wear_data->magic_data );
	    scroll->wear_data->magic_data = NULL;
	}
*/
    }
    return;
}



void do_brandish( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA *staff;
    char arg[MAX_INPUT_LENGTH];
    int num;

    one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
        staff = get_eq_char( ch, WEAR_HOLD );
        for ( num = MAX_WEAR-1 ; num > 0 && ( staff == NULL || staff->wear_data->magic_data == NULL || staff->wear_data->magic_data->type != MAGIC_STAFF ) ; num-- )
        {
	    staff = get_eq_char( ch, num );
	}
    }
    else
	staff = get_obj_wear( ch, arg );

    if ( staff  == NULL || staff->wear_data->magic_data == NULL || 
	staff->wear_data->magic_data->type != MAGIC_STAFF )
    {
        if ( arg[0] == '\0' )
	    send_to_char( "You don't have a staff.\n\r", ch );
	else
	    send_to_char( "That is not a staff.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, PULSE_VIOLENCE );

    if ( staff->wear_data->magic_data->charges > 0 )
    {
	act( "$n brandishes $p.", ch, staff, NULL, TO_ROOM );
	act( "You brandish $p.",  ch, staff, NULL, TO_CHAR );
	if ( number_percent() >= 20 + get_skill(ch,gsn_staves) * 4/5)
 	{
	    act ("You fail to invoke $p.",ch,staff,NULL,TO_CHAR);
	    act ("...and nothing happens.",ch,NULL,NULL,TO_ROOM);
	    check_improve(ch,gsn_staves,FALSE,2);
	}
	
	else for ( vch = ch->in_room->people; vch; vch = vch_next )
	{
	    vch_next	= vch->next_in_room;
		
    	    obj_cast_spell( ch, vch, NULL, staff->wear_data->magic_data->spells, MAGIC_STAFF );
	    check_improve(ch,gsn_staves,TRUE,2);
	}
    }
    else
    {
	act( "$p fizzles.", ch, staff, NULL, TO_CHAR );
	return;
    }

    if ( staff->wear_data->magic_data->max_charges >= 0 &&
	--staff->wear_data->magic_data->charges <= 0 &&
	IS_SET( staff->wear_data->magic_data->flags, ROT_USED ) )
    {
	act( "$n's $p blazes bright and is gone.", ch, staff, NULL, TO_ROOM );
	act( "Your $p blazes bright and is gone.", ch, staff, NULL, TO_CHAR );
	extract_obj( staff );
    }

    return;
}



void do_zap( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *wand;
    OBJ_DATA *obj;
    int num;

    argument = one_argument( argument, arg );
    one_argument( argument, arg2 );
    if ( arg[0] == '\0' && ch->fighting == NULL )
    {
	send_to_char( "Zap whom or what?\n\r", ch );
	return;
    }


    if ( arg2[0] == '\0' )
    {
        wand = get_eq_char( ch, WEAR_HOLD );
        for ( num = MAX_WEAR-1 ; num > 0 && ( wand == NULL || wand->wear_data->magic_data == NULL || wand->wear_data->magic_data->type != MAGIC_WAND ) ; num-- )
        {
	    wand = get_eq_char( ch, num );
	}
    }
    else
	wand = get_obj_wear( ch, arg2 );

    if ( wand == NULL || wand->wear_data->magic_data == NULL || wand->wear_data->magic_data->type != MAGIC_WAND )
    {
	if ( arg2[0] == '\0' )
	    send_to_char( "You don't have a wand.\n\r", ch );
	else
	    send_to_char( "You don't have that.\n\r", ch );
	return;
    }

    obj = NULL;
    if ( arg[0] == '\0' )
    {
	if ( ch->fighting != NULL )
	{
	    victim = ch->fighting;
	}
	else
	{
	    send_to_char( "Zap whom or what?\n\r", ch );
	    return;
	}
    }
    else
    {
	if ( ( victim = get_char_room ( ch, arg ) ) == NULL
	&&   ( obj    = get_obj_here  ( ch, arg ) ) == NULL )
	{
	    send_to_char( "You can't find it.\n\r", ch );
	    return;
	}
    }

    WAIT_STATE( ch, PULSE_VIOLENCE );

    if ( wand->wear_data->magic_data->charges > 0 )
    {
	if ( victim != NULL )
	{
	    act( "$n `wzaps $N `wwith $p`w.", ch, wand, victim, TO_ROOM );
	    act( "You zap $N `wwith $p`w.", ch, wand, victim, TO_CHAR );
	}
	else
	{
	    act( "$n `wzaps $P `wwith $p`w.", ch, wand, obj, TO_ROOM );
	    act( "You zap $P `wwith $p`w.", ch, wand, obj, TO_CHAR );
	}

 	if (number_percent() >= 20 + get_skill(ch,gsn_wands) * 4/5) 
	{
	    act( "Your efforts with $p `wproduce only smoke and sparks.",
		 ch,wand,NULL,TO_CHAR);
	    act( "$n's `wefforts with $p `wproduce only smoke and sparks.",
		 ch,wand,NULL,TO_ROOM);
	    check_improve(ch,gsn_wands,FALSE,2);
	}
	else
	{
    	    obj_cast_spell( ch, victim, obj, wand->wear_data->magic_data->spells, MAGIC_WAND );
	    check_improve(ch,gsn_wands,TRUE,2);
	}
    }
    else
    {
	act( "$p `wfizzles.", ch, wand, NULL, TO_CHAR );
	return;
    }

    if ( wand->wear_data->magic_data->max_charges >= 0 &&
	--wand->wear_data->magic_data->charges <= 0 &&
	IS_SET( wand->wear_data->magic_data->flags, ROT_USED ) )
    {
	act( "$n's `w$p `wexplodes into fragments.", ch, wand, NULL, TO_ROOM );
	act( "Your $p `wexplodes into fragments.", ch, wand, NULL, TO_CHAR );
	extract_obj( wand );
    }

    return;
}

void do_steal( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_INPUT_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int coin_type;
    int percent;
    int amount;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Steal what from whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg2 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That's pointless.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
	return;

    if (victim->position == POS_FIGHTING)
    {
	send_to_char("You'd better not -- you might get hit.\n\r",ch);
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_steal].beats );
    percent  = number_percent( ) + ( IS_AWAKE(victim) ? 10 : -50 );

    if ( ch->level + 5 < victim->level
    ||   victim->position == POS_FIGHTING
    ||   !IS_NPC(victim)
    || ( !IS_NPC(ch) && percent > get_skill( ch, gsn_steal ) ) )
    {
	/*
	 * Failure.
	 */
	send_to_char( "Oops.\n\r", ch );
	act( "$n tried to steal from you.\n\r", ch, NULL, victim, TO_VICT    );
	act( "$n tried to steal from $N.\n\r",  ch, NULL, victim, TO_NOTVICT );
	switch(number_range(0,3))
	{
	case 0 :
	   sprintf( buf, "%s is a lousy thief!", ch->name );
	   break;
        case 1 :
	   sprintf( buf, "%s couldn't rob %s way out of a paper bag!",
		    ch->name,(ch->sex == SEX_FEMALE) ? "her" : "his");
	   break;
	case 2 :
	    sprintf( buf,"%s tried to rob me!",ch->name );
	    break;
	case 3 :
	    sprintf(buf,"Keep your hands out of there, %s!",ch->name);
	    break;
        }
	do_yell( victim, buf );
	if ( !IS_NPC(ch) )
	{
	    if ( IS_NPC(victim) )
	    {
	        check_improve(ch,gsn_steal,FALSE,2);
		multi_hit( victim, ch, TYPE_UNDEFINED );
	    }
	    else
	    {
		log_string( buf );
		if ( !IS_SET(ch->plr, PLR_THIEF) )
		{
		    SET_BIT(ch->plr, PLR_THIEF);
		    send_to_char( "*** You are now a THIEF!! ***\n\r", ch );
		    save_char_obj( ch );
		}
	    }
	}

	return;
    }

	if ( !str_cmp( arg1, "coins" ) || !str_cmp( arg1, "coin" ) ||
	    !str_cmp( arg1, "gold"  ) )

    for ( coin_type = 0 ; coin_type < MAX_WORLD ; coin_type++ )
    {

	amount = victim->gold[ coin_type ] * number_range(1, 10) / 100;
	if ( amount <= 0 )
	{
	    sprintf( buf, "You couldn't get any  %s\n\r", coin_names[ coin_type ] );
	    return;
	}

	ch->gold[ coin_type ]     += amount;
	victim->gold[ coin_type ] -= amount;
	sprintf( buf, "Bingo!  You got %d %s.\n\r", amount, coin_names[ coin_type ] );
	send_to_char( buf, ch );
	check_improve(ch,gsn_steal,TRUE,2);
	return;
    }

/* Cannot use get_obj_carry as the can_see is used from the victim
 * insted of the charactor. G
 *    if ( ( obj = get_obj_carry( victim, arg1 ) ) == NULL )
 *   {
 *	send_to_char( "You can't find it.\n\r", ch );
 *	return;
 *   }
 */
    for ( obj = victim->carrying; obj != NULL; obj = obj->next_content )
    {
	if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE
	&&   (can_see_obj( ch, obj ) ) /* can see from ch not victim */ 
	&&   is_name( arg1, obj->name ) )
	break;
    }
    if ( obj ==  NULL )
    {
	send_to_char( "You can't find it.\n\r", ch );
	return;
    }
	
    if ( !can_drop_obj( ch, obj, FALSE ))
    {
	send_to_char( "You can't pry it away.\n\r", ch );
	return;
    }

    if ( ch->carry_number >= can_carry_n( ch ) )
    {
	send_to_char( "You have your hands full.\n\r", ch );
	return;
    }

    if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) )
    {
	send_to_char( "You can't carry that much weight.\n\r", ch );
	return;
    }

    obj_from_char( obj, "A" );
    obj_to_char( obj, ch);
    act( "Bingo! you got $p from $N!", ch, obj, victim, TO_CHAR );
    check_improve(ch,gsn_steal,TRUE,2);
    return;
}



/*
 * Shopping commands.
 */
CHAR_DATA *find_keeper( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;

    pShop = NULL;
    for ( keeper = ch->in_room->people; keeper; keeper = keeper->next_in_room )
    {
	if ( IS_NPC(keeper) && (pShop = keeper->pIndexData->pShop) != NULL )
	    break;
    }

    if ( pShop == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return NULL;
    }

    /*
     * Undesirables.
     */
    if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_THIEF) )
    {
	do_say( keeper, "Thieves are not welcome!" );
	sprintf( buf, "%s the THIEF is over here!\n\r", ch->name );
	do_yell( keeper, buf );
	return NULL;
    }

    /*
     * Shop hours.
     */
    if ( ( time_info[ keeper->in_room->area->world ].hour < pShop->open_hour ) && !IS_IMMORTAL(ch) )
    {
	do_say( keeper, "Sorry, I am closed. Come back later." );
	return NULL;
    }
    
    if ( time_info[ keeper->in_room->area->world ].hour > pShop->close_hour )
    {
	do_say( keeper, "Sorry, I am closed. Come back tomorrow." );
	return NULL;
    }

    /*
     * Invisible or hidden people.
     */
    if ( !can_see( keeper, ch ) && !IS_IMMORTAL(ch) )
    {
	do_say( keeper, "I don't trade with folks I can't see." );
	return NULL;
    }

    return keeper;
}



int get_cost( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy, CHAR_DATA *ch )
{
    SHOP_DATA *pShop;
    int cost;

    if ( obj == NULL || ( pShop = keeper->pIndexData->pShop ) == NULL 
	|| obj->wear_data == NULL )
	return 0;


    if ( fBuy )
    {
	cost = obj->wear_data->cost * pShop->profit_sell  / 100;
    }
    else
    {
/*	OBJ_DATA *obj2; */
	int itype;

	cost = 0;
	for ( itype = 0; itype < MAX_TRADE; itype++ )
	{
	    if ( IS_SET( pShop->type, obj->shop_type  ) )
	    {
		cost = obj->wear_data->cost * pShop->profit_buy / 100;
		break;
	    }
	}

	/*for ( obj2 = keeper->carrying; obj2; obj2 = obj2->next_content )
	{
	    if ( obj->pIndexData == obj2->pIndexData )
            {
                cost = 0;
                break;
            }
	}*/
    }
    if ( cost < 1 )
	cost = UMAX(1,5 * obj->points);  
    if ( obj->wear_data->magic_data != NULL 
	&& obj->wear_data->magic_data->charges > 0 
	&& obj->wear_data->magic_data->max_charges > 0 )
	cost = cost * obj->wear_data->magic_data->charges / obj->wear_data->magic_data->max_charges;
    if ( race_table[ ch->race ][ ch->sex ].world != race_table[ keeper->race ][ keeper->sex ].world )
	cost *= 3;

    return cost;
}

void do_copy( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj,*key;
    char arg[MAX_INPUT_LENGTH];
    int cost;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char("What do you wish to copy.\n\r",ch);
	return;
    }

    if (  ( obj= get_obj_carry( ch, arg ) ) == NULL || !can_see_obj( ch, obj ) )
    {
	send_to_char("You do not have that.\n\r",ch);
	return;
    }

/* NEW FLAG NEEDED - Quin
    if ( IS_SET( obj->extra_flags, ITEM_COPYABLE )
	|| obj->wear_data == NULL || obj->wear_data->cost < 1 )
    {
*/
	send_to_char("You cannot copy that.\n\r", ch);
	return;
/*
    }
*/

    cost = obj->wear_data->cost;

    if ( cost > get_money_total( ch, ch->in_room->area->world ) )
    {
	send_to_char("You cannot afford to get a copy of that key made.\n\r",ch);
	return;
    }
    get_char_money( ch, cost, ch->in_room->area->world );

    key = create_object( obj->pIndexData );
    clone_object( obj, key );
    obj_to_char( key, ch );
    send_to_char("Your key has been copied.\n\r",ch);
}

void do_enchant( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[ MAX_INPUT_LENGTH ];
    int cost;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "What do you want enchanted?\n\r", ch );
	return;
    }

    if (  ( obj = get_obj_carry( ch, arg ) ) == NULL || !can_see_obj( ch, obj ) )
    {
	send_to_char("You do not have that.\n\r",ch);
	return;
    }

    if ( ( obj->pIndexData->vnum == 18002 || obj->pIndexData->vnum == 18003 || obj->pIndexData->vnum == 18004 ) && !IS_SET( obj->extra_flags, ITEM_MAGIC ) )
    {
    	cost = obj->wear_data->cost;

    	if ( cost > get_money_total( ch, ch->in_room->area->world ) )
    	{
	    send_to_char("You cannot afford to resize that\n\r",ch);
	    return;
    	}

	send_to_char( "Your mineral is now enchanted.\n\r", ch );
    	get_char_money( ch, cost, ch->in_room->area->world );
	SET_BIT( obj->extra_flags, ITEM_MAGIC );
    }
    else
    {
	send_to_char( "You can't enchant that.\n\r", ch );
	return;
    }

    return;
}

void do_resize( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    int cost;

    argument = one_argument( argument, arg );
    argument = one_argument( argument, arg1 );
    if ( arg[0] == '\0' )
    {
	send_to_char("What do you wish to resize?\n\r",ch);
	return;
    }

    if (  ( obj= get_obj_carry( ch, arg ) ) == NULL || !can_see_obj( ch, obj ) )
    {
	send_to_char("You do not have that.\n\r",ch);
	return;
    }

    if (obj->wear_data == NULL || obj->wear_data->cost < 100 )
    {
	send_to_char("That item is not worth resizing.\n\r", ch);
	return;
    }

    cost = obj->wear_data->cost/10;
    if ( cost > get_money_total( ch, ch->in_room->area->world ) )
    {
	send_to_char("You cannot afford to resize that\n\r",ch);
	return;
    }

    if ( arg1[0] == '\0' )
    {
	send_to_char("Would you like to make it larger or smaller?\n\r",ch);
	return;
    }	
   
    if (!strncmp( arg1, "larger",1) )
    {	
	if ( obj->wear_data->size > SIZE_HUGE )
	{
	    send_to_char("Can't make it any Larger!\n\r",ch);
	    return;
	}
	
    	get_char_money( ch, cost, ch->in_room->area->world );
  	obj->wear_data->size++;
	obj->condition -= number_range( 1, 20 );
	if ( obj->condition <= 0 )
	{
	    act( "$p breaks while they attempt to resize it.", ch, obj, obj , TO_CHAR );
	    extract_obj( obj );
	}
    }
    else if (!strncmp( arg1, "smaller",1) )
    {	
	if ( obj->wear_data->size < SIZE_SMALL )
	{
	    send_to_char("Can't make it any Smaller!\n\r",ch);
	    return;
	}
	
    	get_char_money( ch, cost, ch->in_room->area->world );
  	obj->wear_data->size--;
	obj->condition -= number_range( 1, 10 );
	if ( obj->condition <= 0 )
	{
	    act( "$p breaks while they attempt to resize it.", ch, obj, obj , TO_CHAR );
	    extract_obj( obj );
	}
    }
    else
    {
	send_to_char("Would you like to increase(larger) or decrease(smaller) the size?\n\r",ch);
	return;
    }	
    send_to_char("Your item has been resized!\n\r",ch);
}

void buy_obj( CHAR_DATA * ch, CHAR_DATA * keeper, OBJ_DATA * obj, int number ) 
{
    char buf[MAX_STRING_LENGTH];
    int coin_type=0;
    int cost;
    int num;

    if ( number < 1 )
    {
	send_to_char( "Invalid number of objects to buy.\n\r", ch );
	return;
    }

	    if ( obj == NULL || keeper != obj->carried_by
	      || ( cost = get_cost( keeper, obj, TRUE, ch ) ) <= 0 || !can_see_obj( ch, obj ) )
	    {
	        act( "$n tells you 'I don't sell that -- try 'list''.",
		     keeper, NULL, ch, TO_VICT );
	        ch->reply = keeper;
	        return;
	     }

	    if ( number > 1 && !IS_SET( obj->extra_flags, ITEM_INVENTORY ) )
	    {
		act( "$n tells you 'I don't have that many'.", keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
	  	return;
	    }

		/* Check to see if player owns shop. If so
		   the cost is 0 -Trice
		if( keeper->pIndexData->pShop->ownable
		&&  keeper->pIndexData->pShop->owner == ch->name )
			cost = 0;
		*/
	     cost = haggle_cost( ch, cost );
	     coin_type = race_table[ keeper->race ][ keeper->sex ].world;

	     if ( get_money_total( ch, coin_type ) < cost * number )
	     {
		if ( number > 1 )
		    sprintf( buf, "$n tells you 'You can't afford to buy %d $p'.", number );
		else
		    sprintf( buf, "$n tells you 'You can't afford to buy $p'." );

	         act( buf, keeper, obj, ch, TO_VICT );
	         ch->reply = keeper;
	         return;
	      }
	
  	     if ( ch->carry_number + number > can_carry_n( ch ) )
	     {
	         send_to_char( "You can't carry that many items.\n\r", ch );
	         return;
	     }

	     if ( ch->carry_weight + get_obj_weight( obj ) * number > can_carry_w( ch ) && (ch->home && get_room_index(ch->home) == NULL ) )
	     {
	         send_to_char( "You can't carry that much weight.\n\r", ch );
	         send_to_char( "Too bad you don't have a home to store this in\n\r", ch );
 	         return;
	      }

	     if ( number > 1 )
	     {
	         sprintf( buf, "$n buys %d $p.", number );
		 act( buf, ch, obj, NULL, TO_ROOM );
	         sprintf( buf, "You buy %d $p.", number );
		 act( buf, ch, obj, NULL, TO_CHAR );
	     }
	     else
	     {
	         act( "$n buys $p.", ch, obj, NULL, TO_ROOM );
	         act( "You buy $p.", ch, obj, NULL, TO_CHAR );
	     }
	     transfer_money( ch, keeper, cost * number, coin_type );

	if ( number > 1 )
	{
	    for ( num = 0 ; num < number ; num++ )
	    {
	         obj = create_object( obj->pIndexData );

	     if (obj->timer > 0)
	         obj->timer = 0;

	     if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) && (ch->home && get_room_index(ch->home) != NULL ) )
	     {
	         send_to_char( "You can't carry that much weight.\n\r", ch );
	         send_to_char( "I'll have this sent to your home\n\r", ch );
	         obj_to_room(obj, get_room_index(ch->home) );
	     }
	     else
	         obj_to_char( obj, ch);
	     if ( !IS_NPC(ch) )
	     {
	         strncpy(obj->owner , ch->name,30 );
	     }
	     if (cost < obj->wear_data->cost)
	         obj->wear_data->cost = cost;

	    }
	}
	else
	{
 	     if ( IS_SET( obj->extra_flags, ITEM_INVENTORY ) || IS_SET( obj->extra_flags, ITEM_HOME) )
	         obj = create_object( obj->pIndexData );
	     else
	         obj_from_char( obj,"B" );


	     if (obj->timer > 0)
	         obj->timer = 0;

	     if ( ch->carry_weight + get_obj_weight( obj ) > can_carry_w( ch ) && (ch->home && get_room_index(ch->home) != NULL ) )
	     {
	         send_to_char( "You can't carry that much weight.\n\r", ch );
	         send_to_char( "I'll have this sent to your home\n\r", ch );
	         obj_to_room(obj, get_room_index(ch->home) );
	     }
	     else
	         obj_to_char( obj, ch);
	     if ( !IS_NPC(ch) )
	     {
	         strncpy(obj->owner , ch->name,30 );
	     }
	     if (cost < obj->wear_data->cost)
	         obj->wear_data->cost = cost;

	     if ( IS_SET(obj->extra_flags, ITEM_INVENTORY) )
	         REMOVE_BIT(obj->extra_flags, ITEM_INVENTORY);

	}
}

void do_buy( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA * obj_next;
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int number=1;
    int cost;
    long checknum;
    int coin_type;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Buy what?\n\r", ch );
	return;
    }


    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP)
      ||  IS_SET(ch->in_room->room_flags, ROOM_MOUNT_SHOP) )
    {
	CHAR_DATA *pet;
	ROOM_INDEX_DATA *pRoomIndexNext;
	ROOM_INDEX_DATA *in_room;
	long value,anum;
	bool fGuard = FALSE;

	if ( IS_NPC(ch) )
	    return;

	argument = one_argument(argument,arg);
  	if ( is_number( arg ) )
	{
	    checknum = atoi( arg );
	    if ( checknum > 100 || checknum < 1 )
	    {
		send_to_char( "Sorry, you can't buy that here.\n\r", ch );
		return;
	    }
	   	
	}

	pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_buy: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	in_room     = ch->in_room;
	ch->in_room = pRoomIndexNext;
  	if ( is_number( arg ) )
	{
	    value = atoi( arg );
	    anum = 0;
	    for ( pet = ch->in_room->people; pet && anum != value ; pet = pet->next_in_room )
	        if ( IS_SET(pet->act, ACT_PET) || IS_SET(pet->act, ACT_MOUNT) || pet->spec_fun == spec_guard_dog )
		    anum++;

	    if ( anum != value )
		pet = NULL;
	}
	else
	    pet = get_char_room( ch, arg );

	ch->in_room = in_room;

	if ( pet == NULL || ( !IS_SET(pet->act, ACT_PET) && !IS_SET(pet->act, ACT_MOUNT) && pet->spec_fun != spec_guard_dog ) )
	{
	    send_to_char( "Sorry, you can't buy that here.\n\r", ch );
	    return;
	}

	if ( pet->spec_fun == spec_guard_dog )
	    fGuard = TRUE;

	if ( ch->pet != NULL && IS_SET(ch->in_room->room_flags,ROOM_PET_SHOP) && !fGuard )
	{
	    send_to_char("You already own a pet.\n\r",ch);
	    return;
	}

	if ( ch->mount != NULL && IS_SET(ch->in_room->room_flags,ROOM_MOUNT_SHOP))
	{
	    send_to_char("You already own a faithful mount.\n\r",ch);
	    return;
	}

 	if ( IS_SET( ch->in_room->room_flags,ROOM_PET_SHOP) )
	    cost = 10 * pet->level * pet->level * ( fGuard ? pet->level : 1 ); 
	else
	    cost = 40 * pet->level * pet->level; 

	cost = haggle_cost( ch, cost );
	coin_type = ch->in_room->area->world;

	if ( get_money_total( ch, coin_type ) < cost )
	{
	    send_to_char( "You can't afford it.\n\r", ch );
	    return;
	}

	if ( fGuard && ch->home < 2 )
	{
	    send_to_char( "You don't have a home to guard.\n\r", ch );
	    return;
	}

	if ( ch->level < pet->level && !fGuard )
	{
	    send_to_char( "You're not powerful enough to master this pet.\n\r", ch );
	    return;
	}

	argument = one_argument( argument, arg );
  	if ( is_number( arg ) )
	{
	    checknum = atoi( arg );
	    if ( checknum > 100 || checknum < 1 )
	    {
		send_to_char( "Sorry, you can't buy that here.\n\r", ch );
		return;
	    }
	   	
	}

	get_char_money( ch, cost, coin_type );
	pet = create_mobile( pet->pIndexData );
	if ( arg[0] != '\0' )
	{
	    sprintf( buf, "%s %s", pet->name, arg );
	    strncpy( pet->name , buf, 30 );
	}

	sprintf( buf, "%sA neck tag says 'I belong to %s'.\n\r",
	    pet->description, ch->name );
	free_string(pet->description);
	pet->description = str_dup( buf );
	if ( fGuard )
	{
	    ROOM_INDEX_DATA * pRoom;

	    pRoom = get_room_index( ch->home );

	    if ( pRoom == NULL )
	    {
		send_to_char( "Cannot find your home.\n\r", ch );
		char_to_room( pet, ch->in_room );
		return;
	    }

	    char_to_room( pet, pRoom );
	    act( "$N has been sent to your home.", ch, pet, pet, TO_CHAR );
	    return;
	}

        if ( IS_SET(ch->in_room->room_flags,ROOM_PET_SHOP) )
	{
 	    SET_BIT(ch->plr, PLR_BOUGHT_PET);
	    SET_BIT(pet->act, ACT_PET);
        }
        if ( IS_SET(ch->in_room->room_flags,ROOM_MOUNT_SHOP) )
	   pet->act = pet->act | ACT_MOUNT;
	SET_BIT(pet->affected_by, AFF_CONTROL_MIND);
	pet->comm = COMM_NOTELL|COMM_NOSHOUT|COMM_NOCHANNELS;

	char_to_room( pet, ch->in_room );
	add_follower( pet, ch );
	pet->leader = ch;
	if ( IS_SET(ch->in_room->room_flags,ROOM_PET_SHOP) )
	{
	    send_to_char( "Enjoy your pet.\n\r", ch );
	    act( "$n bought $N as a pet.", ch, NULL, pet, TO_ROOM );
 	    ch->pet = pet;
	}
	else
	{
	    send_to_char( "Enjoy your new mount.\n\r", ch );
	    act( "$n bought $N as a mount.", ch, NULL, pet, TO_ROOM );
	    ch->mount = pet;
	}
	return;
    }
    else
    {
        char *args;
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int anum,value;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;
	    
        args = one_argument( argument, arg );
        if ( is_number( arg ) && *args != '\0' )
	{
	    number = atoi( arg );
	    if ( number > 100 || number < 1 )
	    {
		send_to_char( "Sorry, you can't buy that here.\n\r", ch );
		return;
	    }
	   	
	    argument = args;
 	}
	else if ( *args != '\0' )
	{
	    buf[ 0 ] = '\0';
	    for (  ; *args != '\0' ; )
	    {
		strcat( buf, " " );
		strcat( buf, arg );		
		args = one_argument( args, arg );
        if ( is_number( arg ) && *args != '\0' )
	{
	    checknum = atoi( arg );
	    if ( checknum > 100 || checknum < 1 )
	    {
		send_to_char( "Sorry, you can't buy that here.\n\r", ch );
		return;
	    }
	   	
 	}
	    }

	    if ( is_number( arg ) )
	    {
		argument = buf;
		number = atoi( arg );
	    if ( number > 100 || number < 1 )
	    {
		send_to_char( "Sorry, you can't buy that here.\n\r", ch );
		return;
	    }
	    }
	}

  	if ( is_number( argument ) )
	{
	    value = atoi( argument );
	    if ( value > 100 || value < 1 )
	    {
		send_to_char( "Sorry, you can't buy that here.\n\r", ch );
		return;
	    }

	    anum = 1;
	    for ( obj = keeper->carrying; obj && anum != value ; obj = obj->next_content )
	        if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE
	        &&   can_see_obj( ch, obj )
	        &&   ( cost = get_cost( keeper, obj, TRUE, ch ) ) > 0 )
		    anum++;

	    if ( anum != value )
		obj = NULL;
	    else
		buy_obj( ch, keeper, obj, number );

	}
	else if ( !str_cmp( argument, "all" ) )
	{
	    for ( obj = keeper->carrying ; obj != NULL ; obj = obj_next )
	    {
		obj_next = obj->next_content;

		if ( obj->wear_data == NULL || obj->wear_data->cost == 0
		  || !can_see_obj( ch, obj ) || ( cost = get_cost( keeper, obj, TRUE, ch ) ) < 1 )
		continue;

		buy_obj( ch, keeper, obj, number );

	    }
	}
	else if ( !str_prefix( "all.", argument ) )
	{
	    for ( obj = keeper->carrying ; obj != NULL ; obj = obj_next )
	    {
		obj_next = obj->next_content;

		if ( obj->wear_data == NULL || obj->wear_data->cost == 0
		  || !can_see_obj( ch, obj ) || ( cost = get_cost( keeper, obj, TRUE, ch ) ) < 1 
		  || !is_name( &argument[4], obj->name ) )
		continue;

		buy_obj( ch, keeper, obj, number );

	    }
        }
	else
	{
	    obj  = get_obj_carry( keeper, argument );

	    buy_obj( ch, keeper, obj, number );
	}
	return;
    }
}
/*
void do_refill( CHAR_DATA *ch, char *argument )
{
    int cost;
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int coin_type;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Refill what?\n\r", ch );
	return;
    }

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;

	if ( (obj  = get_obj_carry( ch, argument ) ) == NULL)
	{
		send_to_char( "You can't refill something you don't have!\n\r", ch );
		return;
	}	

	if ( (obj2 = get_obj_carry( keeper, argument ) ) == NULL 
	  || !IS_SET( obj2->extra_flags, ITEM_INVENTORY ) )
	{
		do_say( keeper, "I can't refill that!" );
		return;
	}	

	cost = get_cost( keeper, obj2, TRUE, ch )*3/4;

	if ( ch->race != keeper->race )
	    if ( race_table[ ch->race ][ ch->sex ].world != race_table[ keeper->race ][ keeper->sex ].world )
		cost *= 3;

	if ( obj->edible_data == NULL || IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) )
	{
	    send_to_char( "That is not a drink container.\n\r", ch );
	    return;
	}

        if ( obj->edible_data->amount >= obj->edible_data->max_amount )
        {
	    send_to_char( "Your container is full.\n\r", ch );
	    return;
        }

	if ( cost <= 0 || !can_see_obj( ch, obj ) || obj->wear_data == NULL )
	{
	    act( "$n tells you 'I don't refill that -- try 'list''.",
		keeper, NULL, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}

	coin_type = race_table[ keeper->race ][ keeper->sex ].world;
	
	if ( get_money_total( ch, coin_type ) < cost )
	{
	    act( "$n tells you 'You can't afford to refill $p'.",
		keeper, obj, ch, TO_VICT );
	    ch->reply = keeper;
	    return;
	}
	
	act( "$n refills $p.", ch, obj, NULL, TO_ROOM );
	act( "You refill $p.", ch, obj, NULL, TO_CHAR );
	transfer_money( ch, keeper, cost, coin_type );

	obj->edible_data->amount = obj->edible_data->max_amount;
	return;
}
*/
void do_extinguish( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Extinguish what?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for (obj=ch->carrying;  obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( can_see_obj( ch, obj ) 
      && (obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE))
		extinguish_obj( ch, obj);
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL && ( obj = get_obj_list( ch, arg, ch->in_room->contents ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	extinguish_obj( ch, obj);
    }
    return ;
}

void extinguish_obj (CHAR_DATA *ch, OBJ_DATA *obj)
{
  if (obj->light_data != NULL)
    {
        if (!IS_SET(obj->light_data->flags, LIGHT_LIT))
            {
        act("But $p is not lit!",ch,obj,NULL,TO_CHAR) ;
    }                  
    else
    {        
    act("You extinguish $p.",ch,obj,NULL,TO_CHAR) ;
    REMOVE_BIT(obj->light_data->flags, LIGHT_LIT|LIGHT_BURNING) ;
          act("$n extinguishes $p.",ch,obj,NULL,TO_ROOM) ;
    }
    }    
    else
    {
    act("You cannot extinguish $p!",ch,obj,NULL,TO_CHAR) ;
    }              
return ;
}



void do_light( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj, *nextobj, *objcarried ;
  bool lighter=FALSE ;
      

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Light what?\n\r", ch );
	return;
    }

    for (objcarried=ch->carrying; objcarried!=NULL;objcarried=nextobj)
    {
   
    nextobj=objcarried->next_content ;

    if (objcarried->light_data != NULL && IS_SET( objcarried->light_data->flags, LIGHT_BURNING ) )
    {
      lighter=TRUE ;
      break ;
    }
    }    

    if (!lighter)
	for ( objcarried = ch->in_room->contents ; objcarried != NULL; objcarried = nextobj )
	{
	    nextobj=objcarried->next_content ;

	    if (objcarried->light_data != NULL && IS_SET( objcarried->light_data->flags, LIGHT_BURNING ) )
	    {
	      lighter=TRUE ;
	      break ;
	    }
	}

    if (!lighter)
    {
	if ( !IS_IMMORTAL( ch ) )
	{
	      send_to_char("But you do not have anything to use as a light!\n\r",ch) ;
	      return ;
	}
	else
	{
	     send_to_char("You use your immortal powers to light it.\n\r", ch );
	}
    }

    if ( !str_cmp( arg, "all" ) )
    {
	OBJ_DATA *obj_next;

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( can_see_obj( ch, obj ) 
	            && (obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE))
		light_obj( ch, obj);
	}
	return;
    }
    else
    {
	if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	{
	    send_to_char( "You do not have that item.\n\r", ch );
	    return;
	}

	light_obj( ch, obj);
    }
    return ;
}

void light_obj (CHAR_DATA *ch, OBJ_DATA *obj)
{
  if (obj->light_data != NULL && IS_SET( obj->light_data->flags, LIGHT_LIGHTABLE ) )
    {
        if (IS_SET(obj->light_data->flags, LIGHT_LIT))
        {
            act("$p is already lit!",ch,obj,NULL,TO_CHAR) ;
        }                  
        else if (obj->light_data->fuel == 0)
        {
            act("$p has already burned out!",ch,obj,NULL,TO_CHAR);            
        }      
    else    
    {        
        act("You light $p",ch,obj,NULL,TO_CHAR) ;
        SET_BIT(obj->light_data->flags, LIGHT_LIT|LIGHT_BURNING);
        act("$n lights $p.",ch,obj,NULL,TO_ROOM) ;
	ch->in_room->light_adjustment = get_light_adjustment ( ch, ch->in_room );
    }
    }    
    else
    {
    act("You cannot light $p!",ch,obj,NULL,TO_CHAR) ;
    }              
return ;
}

void do_refuel( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *fuel ;
    int amount;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Refuel what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    found = FALSE;
    for ( fuel = ch->carrying; fuel != NULL;
	fuel = fuel->next_content )
    {
	if ( fuel->light_data != NULL && fuel->light_data->fuel > 0 )
	if ( IS_SET(fuel->light_data->flags, LIGHT_FUEL ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
    {
	send_to_char("You have nothing to refill it with!\n\r", ch );
	return;
    }

    if ( obj->light_data == NULL || !IS_SET( obj->light_data->flags, LIGHT_REFUELABLE ) )
    {
	send_to_char( "You can't refuel that.\n\r", ch );
	return;
    }

    if ( obj->light_data->fuel >= obj->light_data->max_fuel )
    {
	send_to_char( "It is already full.\n\r", ch );
	return;
    }

    amount = UMIN( fuel->light_data->fuel, obj->light_data->max_fuel - obj->light_data->fuel );
    obj->light_data->fuel += amount;
    fuel->light_data->fuel -= amount;
    act( "You refuel $p.", ch, obj, NULL, TO_CHAR );
    if ( fuel->light_data->fuel == 0 )
        extract_obj(fuel) ;
    return;
}

void do_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET(ch->in_room->room_flags, ROOM_PET_SHOP) 
      || IS_SET(ch->in_room->room_flags, ROOM_MOUNT_SHOP) )
    {
	ROOM_INDEX_DATA *pRoomIndexNext;
	CHAR_DATA *pet;
	bool found;
	int anum;

	pRoomIndexNext = get_room_index( ch->in_room->vnum + 1 );
	if ( pRoomIndexNext == NULL )
	{
	    bug( "Do_list: bad pet shop at vnum %d.", ch->in_room->vnum );
	    send_to_char( "You can't do that here.\n\r", ch );
	    return;
	}

	found = FALSE;
	anum = 1;
	for ( pet = pRoomIndexNext->people; pet; pet = pet->next_in_room )
	{
	    if ( IS_SET(pet->act, ACT_PET) || pet->spec_fun == spec_guard_dog )
	    {
		if ( !found )
		{
		    found = TRUE;
		    sprintf(buf,"Pets for sale:  (values in %s)\n\r", coin_names[ ch->in_room->area->world ]);
		    send_to_char( buf, ch );
		}
		sprintf( buf, "[%3d] [%2d] %8d - %s\n\r", anum++,
		    pet->level,
		    10 * pet->level * pet->level /* * world_mult */ * ( pet->spec_fun == spec_guard_dog ? pet->level : 1 ),
		    pet->short_descr );
		send_to_char( buf, ch );
	    }
	    if ( IS_SET(pet->act, ACT_MOUNT) )
	    {
		if ( !found )
		{
		    found = TRUE;
		    sprintf(buf,"Mounts for sale:  (values in %s)\n\r", coin_names[ ch->in_room->area->world ]);
		    send_to_char( buf, ch );
		}
		sprintf( buf, "[%3d] [%2d] %8d - %s\n\r", anum++,
		    pet->level,
		    40 * pet->level * pet->level /* * world_mult */ ,
		    pet->short_descr );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "Sorry, we're out of stock right now.\n\r", ch );
	return;
    }
    else
    {
	char arg[MAX_INPUT_LENGTH];
	char arg2[MAX_INPUT_LENGTH];
	char arg3[MAX_INPUT_LENGTH];
	CHAR_DATA *keeper;
	OBJ_DATA *obj;
	int cost,anum;
 	int min_level = 0;
	int max_level = 1000;
	bool found;

	if ( ( keeper = find_keeper( ch ) ) == NULL )
	    return;
        argument = one_argument(argument,arg);
	argument = one_argument( argument, arg2 ); 
	argument = one_argument( argument, arg3 ); 

	if ( is_number( arg ) )
	{
	    if ( is_number( arg2 ) )
	    {
		min_level = atoi( arg );
		max_level = atoi( arg2 );
		strcpy( arg, arg3 );
	    }
	    else
	    {
		max_level = atoi( arg );
		strcpy( arg, arg2 );
	    }
	}
	else if ( is_number( arg2 ) )
	{
	    if ( is_number( arg3 ) )
	    {
		min_level = atoi( arg2 );
		max_level = atoi( arg3 );
	    }
	    else
		max_level = atoi( arg2 );
	}

	found = FALSE;
	anum = 0;
	for ( obj = keeper->carrying; obj; obj = obj->next_content )
	{
	    if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE
	    &&   can_see_obj( ch, obj )
	    &&  !IS_SET(obj->extra_flags, ITEM_NO_SAVE )
	    &&   ( cost = get_cost( keeper, obj, TRUE, ch ) ) > 0 
	    &&   ( arg[0] == '\0'  
 	       ||  is_name(arg,obj->name) ))
	    {
		anum++;

		if ( arg[0] != '\0' && !is_name( arg, obj->name ) )
		    continue;

		if ( !found )
		{
		    found = TRUE;
		    sprintf(buf,"      `W[Price] Item    (values in %s)\n\r",coin_names[ race_table[ keeper->race ][ keeper->sex ].world ] );
		    send_to_char( buf, ch );
		}

		sprintf( buf, "`w[%3d] [%5d] %s.\n\r", anum,
		    cost, obj->short_descr);
		send_to_char( buf, ch );

/* refill removed due to lack of reasoning to have it.....

		if ( obj->edible_data != NULL && !IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) 
		  && IS_SET( obj->extra_flags, ITEM_INVENTORY
		   ) )
		{
		    sprintf( buf, "      `w[%5d] %s (refill).\n\r",
		        cost*3/4, obj->short_descr);
		    send_to_char( buf, ch );
		}
*/
	    }
	}

	if ( !found )
	    send_to_char( "You can't buy anything here.\n\r", ch );
	return;
    }
}



void do_sell( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    SHOP_DATA *pShop;
    OBJ_DATA *pObj;
    OBJ_DATA *obj_next;
    OBJ_DATA *obj;
    int cost;
    int coin_type;
    int items, itype;
    bool found;

    one_argument( argument, arg );

    if ( IS_IMMORTAL( ch ) && !IS_OPTIVISION( ch ) )
    {
	send_to_char( "Immortals have no need to sell items to mortal shops.\n\r", ch );
	return;
    }
  
    if ( arg[0] == '\0' )
    {
	send_to_char( "Sell what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( !str_cmp("all", arg ) )
    {
	for ( obj = ch->carrying ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE )
	  	do_sell( ch, obj->name );
	}
	return;
    }

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    found = FALSE;
    pShop = keeper->pIndexData->pShop;

    for ( itype = 0; itype < MAX_TRADE; itype++ )
    {
	if ( IS_SET( pShop->type, obj->shop_type  ) )
	    found = TRUE;
    }

    if ( found == FALSE )
    {
	send_to_char( "They don't buy that.\n\r", ch );
	return;
    }

    if ( !can_drop_obj( ch, obj, FALSE ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
	act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
	return;
    }

    /* won't buy rotting goods */
    if ( obj->timer || ( cost = get_cost( keeper, obj, FALSE, ch ) ) <= 0 
	|| obj->wear_data == NULL )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }

    items = 1;
    for ( pObj = keeper->carrying ; pObj != NULL ; pObj =pObj->next_content )
    {
	if ( pObj->wear_data->wear_loc == WEAR_NONE && !str_cmp( obj->short_descr, pObj->short_descr ) )
	    if ( items++ == 2 )   /* limit 2 of each item in a shop */
	    {
		act( "$n tells you 'I already have 2 of them, and I don't need any more.'",
		    keeper, NULL, ch, TO_VICT );
		ch->reply = keeper;
		return;
	    }
    }
    
    /* Cost is 0 for owners -Trice
    if( keeper->pIndexData->pShop->ownable 
    &&  keeper->pIndexData->pShop->owner == ch->name )
    	cost = 0;
    */

    coin_type = keeper->in_room->area->world;

    if ( get_money_total( keeper, coin_type ) < cost )
    {
	act("$n tells you 'I'm sorry, but I can't afford that.",
	    keeper,obj,ch,TO_VICT);
	return;
    }

    act( "$n sells $p.", ch, obj, NULL, TO_ROOM );

    sprintf( buf, "You sell $p for %d %s.",
	cost, cost == 1 ? coin_name[ coin_type ] : coin_names[ coin_type ] );
    act( buf, ch, obj, NULL, TO_CHAR );
    transfer_money( keeper, ch, cost, coin_type );

    if ( keeper->gold[0] < 0 )
	keeper->gold[0] = 0;
    if ( keeper->gold[1] < 0 )
	keeper->gold[1] = 0;
    if ( keeper->gold[2] < 0 )
	keeper->gold[2] = 0;
    if ( keeper->gold[3] < 0 )
	keeper->gold[3] = 0;

	obj_from_char( obj, "C" );
	obj->timer = number_range(50,100);
	add_to_shop( obj, keeper );

    return;
}

void do_value( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *keeper;
    OBJ_DATA *obj;
    SHOP_DATA *pShop;
    int cost, itype;
    bool found;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Value what?\n\r", ch );
	return;
    }

    if ( ( keeper = find_keeper( ch ) ) == NULL )
	return;

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
    {
	act( "$n tells you 'You don't have that item'.",
	    keeper, NULL, ch, TO_VICT );
	ch->reply = keeper;
	return;
    }

    if (!can_see_obj(keeper,obj))
    {
        act("$n doesn't see what you are offering.",keeper,NULL,ch,TO_VICT);
        return;
    }

    found = FALSE;
    pShop = keeper->pIndexData->pShop;

    for ( itype = 0; itype < MAX_TRADE; itype++ )
    {
	if ( IS_SET( pShop->type, obj->shop_type  ) )
	    found = TRUE;
    }

    if ( found == FALSE )
    {
	send_to_char( "They don't buy that.\n\r", ch );
	return;
    }


    if ( !can_drop_obj( ch, obj, FALSE ) )
    {
	send_to_char( "You can't let go of it.\n\r", ch );
	return;
    }

    if ( obj->timer || ( cost = get_cost( keeper, obj, FALSE, ch ) ) <= 0 )
    {
	act( "$n looks uninterested in $p.", keeper, obj, ch, TO_VICT );
	return;
    }

    if ( ch->race != keeper->race )
    {
    	if ( race_table[ ch->race ][ ch->sex ].world != race_table[ keeper->race ][ keeper->sex ].world )
	    cost /= 3;
	else
	    cost = cost * 2 / 3;
    }

    if ( cost > get_money_total( keeper, race_table[ keeper->race ][ keeper->sex ].world ) )
    {
	act( "$n tells you 'I can't afford $p'.", keeper, obj, ch, TO_VICT );
        ch->reply = keeper;
	return;
    }

    sprintf( buf, "$n tells you 'I'll give you %d coins for $p'.", cost );
    act( buf, keeper, obj, ch, TO_VICT );
    ch->reply = keeper;

    return;
}

#define MEDICAL_HERB_VNUM 300

void do_first_aid ( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA  *victim;
    OBJ_DATA *herb;

    one_argument(argument,arg);
    if ( arg[0] == '\0' )
    {
        send_to_char("Who do you want to do first aid on?\n\r",ch);
        return;
    }

    if ( !( victim = get_char_room(ch,arg) ) )
    {
        send_to_char("They are not here.\n\r",ch);
        return;
    }
 
    /* Check to see if victim is fighting */
    if ( victim->position == POS_FIGHTING )
    {
        send_to_char( "They are moving around to much to perform first aid on.\n\r", ch );
	return;
    }

    /* Check if character has the medical herb. */
    for ( herb = ch->carrying; herb != NULL && herb->pIndexData->vnum != OBJ_VNUM_MEDICAL_HERB ; herb = herb->next_content );   

    if ( herb == NULL )
    {
	send_to_char( "You do not have the right herb to do first aid.\n\r", ch );
	return;
    }

    /* check to see if victim is at or more than 2/3 hps */

    if ( victim->hit >= ( victim->max_hit * 2 ) / 3 )
    {
	send_to_char( "Your skills at healing can do no more for them.\n\r", ch );
	return;
    }

    /* Herb is used up */
    extract_obj( herb );
    victim->hit += ( roll_dice(ch->level, 2 ) * 2 / 7 ) + ( get_skill( ch, gsn_first_aid ) * 2 / 10);
    update_pos( victim );

    WAIT_STATE( ch, skill_table[gsn_first_aid].beats );

    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    send_to_char( "Your wounds have been mended!\n\r", victim );
    return;
}


void do_rename( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  name <obj-name> <field> <string>\n\r",ch);
	send_to_char("    fields: name desc\n\r",ch);
	return;
    }
    
    	
   	if ( ( obj = get_obj_carry( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "You don't have anything like that.\n\r", ch );
	    return;
    	}
    	
        if ( !str_prefix( arg2, "name" ) )
    	{
	    act( "$n names $p as $T.", ch, obj, arg3 , TO_ROOM );
	    act( "You name $p as $T.", ch, obj, arg3 , TO_CHAR );
	    strncpy( obj->name, arg3, 30 );
	    if ( ( obj = get_obj_carry_vnum(ch,OBJ_VNUM_RENAME) ) )
	     {
	      act( "$p fades into mist.", ch, obj, NULL, TO_ROOM );
	      act( "$p fades into mist.", ch, obj, NULL, TO_CHAR );
	      extract_obj(obj);
	     }
	    return;
    	}

    	if ( !str_prefix( arg2, "desc" ) )
    	{
	    act( "$n describes $p as $T.", ch, obj, arg3, TO_ROOM );
	    act( "You describe $p as $T.", ch, obj, arg3, TO_CHAR );
	    strncpy( obj->short_descr, arg3, 80 );
	    if ( ( obj = get_obj_carry_vnum(ch,OBJ_VNUM_RENAME) ) )
	     {
	      act( "$p fades into mist.", ch, obj, NULL, TO_ROOM );
	      act( "$p fades into mist.", ch, obj, NULL, TO_CHAR );
	      extract_obj(obj);
	     }
	    return;
    	}

/* at this time I am leaving in the long description, although I think it will
  confuse players more then help.  Changing the name and descriptions (short)
  should be the most we would want a player to be able to do. */

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    strncpy( obj->description, arg3, 80 );
	    return;
    	}
    	
    /* echo bad use message */
    do_rename(ch,"");
}

void do_pull(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' ) 
    {
	send_to_char( "Pull what? ", ch );
	return;
    }

    if ( str_cmp(arg, "rope" ) )
    {
	send_to_char("That is not here.\n\r", ch );
	return;
    }

    for ( d = descriptor_list ; d != NULL ; d = d->next )
    {
	if ( d->character == NULL
	   || d->connected > CON_PLAYING )
	    continue;

	if ( d->character->in_room->area == ch->in_room->area )
	    send_to_char("The chapel bell rings.\n\r", d->character );
	else
	    send_to_char("You hear a bell ring in the distance.\n\r", d->character );

    }

}

void do_sign(CHAR_DATA *ch, char *argument)
{
char arg[MAX_INPUT_LENGTH];
char buf[MAX_STRING_LENGTH];
char buf3[MAX_STRING_LENGTH];
char buf2[MAX_STRING_LENGTH];
OBJ_DATA *obj;
EXTRA_DESCR_DATA *ed;
AREA_DATA *pArea;
pArea = ch->in_room->area;
argument = one_argument(argument,arg);
if (arg[0] == '\0')
	{
	send_to_char("What do you wish to sign?\n\r",ch);
	return;
	}
if (argument[0] == '\0')
	{
	send_to_char("Syntax: Sign <item> <message>\n\r",ch);
	send_to_char("\n\r",ch);
	send_to_char("The message should fit on one line.\n\r",ch);
	send_to_char("Example: Sign book Congradulations!  Best wishes and all my love!\n\r",ch);
	send_to_char("\n\r",ch);
	send_to_char("This would then show in the book as:\n\r",ch);
	sprintf(buf,"%s: Congradulations!  Best wishes and all my love!\n\r",ch->name);
 	send_to_char(buf,ch);	
	send_to_char("( look book ) for more examples :)",ch);
	return;
	}
obj = get_obj_list( ch, arg, ch->in_room->contents );
    if ( obj == NULL )
    {
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return;
    }

for ( ed = obj->pIndexData->extra_descr; ed != NULL; ed = ed->next )
	{
	if (is_name(arg, ed->keyword ) )
	break;
	}
if ( ed == NULL )
	{
	send_to_char("Keyword not found\n\r",ch);
	return;
	}
sprintf(buf3,"%s%-12s-%s\n\r",ed->description,ch->name,argument);
free_string(ed->description);
ed->description = str_dup(buf3);
act( "$n Signs $p.", ch, obj, NULL, TO_ROOM );
act( "You Sign $p.", ch, obj, NULL, TO_CHAR );
strcpy(buf2,"changed_no_message");
SET_BIT(pArea->area_flags, AREA_CHANGED);
do_asave(NULL,buf2);
}
