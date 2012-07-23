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
#include <sys/ipc.h>
#include <sys/msg.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/*
 * for autosaves of the system
 */
long autosave_system = A|B|C|D|E|F|G|H|I|J|K;
long changed_system = 0;

int	save_number = 0;

extern const	int	mana_gain_table[];

void reset_doors(AREA_DATA *pArea)
{
    ROOM_INDEX_DATA *pRoomIndex;
    EXIT_DATA *pexit;
    int iHash;
    int door;
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_next;
 
    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for ( pRoomIndex  = room_index_hash[iHash];
              pRoomIndex != NULL;
              pRoomIndex  = pRoomIndex->next )
        {
	    if ( pRoomIndex->area == pArea )
            for ( door = 0; door <= 5; door++ )
            {
                if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
                {
		    pexit->exit_info = pexit->exit_info_default;
		    
    		    for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_next)
    		    {
			lock_next = lock_list->next;
			lock_list->lock_info = lock_list->lock_info_default;  
    		    }
		
		    if ( pexit->trap != NULL )
			if ( IS_SET( pexit->trap->flags, TRAP_SPRUNG ) )
		            REMOVE_BIT( pexit->trap->flags, TRAP_SPRUNG );
		}
	    }
	}
    }
}
void event_reset( long flag, AREA_DATA *pArea )
{
    char buf[MAX_STRING_LENGTH];
    EVENT_DATA * event;
    CHAR_DATA * mob;
    OBJ_DATA * obj;
    ROOM_INDEX_DATA * room;

    for ( event = pArea->events ; event != NULL ; event = event->next )
    {
        if ( event->chance < number_percent() )
	    continue;

	if ( !IS_SET(event->flags, flag ) )
	    continue;

	if ( !IS_SET( flag, EVENT_ON_BOOT) )
	if ( IS_SET(event->flags, EVENT_ON_RESET) && event->area != pArea)
	    continue;
	
	if ( IS_SET(event->flags, EVENT_INVALID) )
	    continue;

	if ( event->type == EVENT_ROOM )
	{
	    if ( event->mob_vnum != 0 ) /* Event must have a mob to reset */
	    {
		/*
		 * Is the mob out there?  Has it ever been created?
		 */
		if ( !event->mob || event != event->mob->event )
		{
		    if ( get_mob_index( event->mob_vnum ) == NULL )
		    {
			SET_BIT(event->flags, EVENT_INVALID);
		    	sprintf(buf,"Invalid Event: Area %s Event %ld\n\r",pArea->name, event->vnum);
		    	bug(buf,event->mob_vnum);
			continue;
		    }
		    
		    if ( ( mob = create_mobile( get_mob_index( event->mob_vnum ) ) ) == NULL )
		    {
		    	sprintf(buf,"Invalid Event: Area %s Event %ld\n\r",pArea->name, event->vnum);
		    	bug(buf,event->mob_vnum);
			return;
		    }
		    if ( ( room = get_room_index( event->room_vnum ) ) == NULL )
		    {
		    	sprintf(buf,"Invalid Event: Area %s Event %ld\n\r",pArea->name, event->vnum);
		    	bug(buf,event->room_vnum);
			return;
		    }
		    char_to_room(mob, room);
		    script_update( mob, TRIG_BORN );
		    event->mob = mob;
		    event->mob->event = event;
		    /*
		     * Mob may have something to equip
		     */
		    if ( mob->pIndexData->equip )
		    {
			if ( IS_SET(flag, EVENT_ON_BOOT ) )
			{
				equip_mob(mob, EQUIP_ON_BOOT );
			}
			if ( IS_SET(flag, EVENT_ON_RESET ) )
			{
				equip_mob(mob, EQUIP_ON_CREATION );
			}
		    }
		}
		/*
		 * The mob may already be there ( somewhere )
		 */
		if ( event->mob && event == event->mob->event )
		{
		    if ( event->mob->pIndexData->equip )
		    {
			if ( IS_SET(flag, EVENT_ON_RESET ) )
			    equip_mob(event->mob, EQUIP_ON_RESET );
		    }
		}
	    }
	    if ( event->obj_vnum != 0 ) /* Event must have a obj to reset */
	    {
		/*
		 * Is the obj out there?  Has it ever been created?
		 */
		if ( !event->obj || event != event->obj->event )
		{
		    
		    if ( get_obj_index( event->obj_vnum ) == NULL )
		    {
			SET_BIT(event->flags, EVENT_INVALID);
		    	sprintf(buf,"Invalid Event: Area %s Event %ld\n\r",pArea->name, event->vnum);
		    	bug(buf,event->obj_vnum);
			continue;
		    }
		    if ( ( obj = create_object( get_obj_index( event->obj_vnum ) ) ) == NULL )
		    {
		    	sprintf(buf,"Invalid Event: Area %s Event %ld\n\r",pArea->name, event->vnum);
		    	bug(buf,event->obj_vnum);
			return;
		    }
		    if ( ( room = get_room_index( event->room_vnum ) ) == NULL )
		    {
		    	sprintf(buf,"Invalid Event: Area %s Event %ld\n\r",pArea->name, event->vnum);
		    	bug(buf,event->room_vnum);
			return;
		    }
		    obj_to_room(obj, room);
		    event->obj = obj;
		    event->obj->event = event;
		    /*
		     * Obj may have something to load inside 
		     */
		    if ( obj->pIndexData->inside )
		    {
			if ( IS_SET(flag, EVENT_ON_BOOT ) )
			{
				load_inside_obj(obj, EQUIP_ON_BOOT );
			}
			if ( IS_SET(flag, EVENT_ON_RESET ) )
			{
				load_inside_obj(obj, EQUIP_ON_CREATION );
			}
		    }
		}
		/*
		 * The obj may already be there ( somewhere )
		 */
		if ( event->obj && event == event->obj->event )
		{
		    if ( event->obj->pIndexData->inside )
		    {
			if ( IS_SET(flag, EVENT_ON_RESET ) )
			    load_inside_obj(event->obj, EQUIP_ON_RESET );
		    }
		}
	    }
	}
    }
} 
void check_rust( OBJ_DATA * obj, int amount )
{
    CHAR_DATA * ch = obj->carried_by;
    OBJ_DATA * obj_in;
    OBJ_DATA * obj_next;
    OBJ_INDEX_DATA * obj_index;
    /* Just returning till I get a chance to look at why containers
       are extracting everything in them too. G */
	return;

    if ( amount < 1 || number_range( 0, 10 ) == 0 )
	return;

    if ( obj->material == MAT_STEEL || obj->material == MAT_IRON )
    {
	obj->condition -= number_range( 0, amount );
	if ( obj->condition < 1 )
	{
	    if ( ch != NULL )
	        act( "$p rusts, and falls apart.\n\r", ch, obj, obj, TO_CHAR );
	    extract_obj( obj );
	    if ( ( obj_index = get_obj_index( OBJ_VNUM_RUST_PILE ) ) != NULL && ch != NULL )
	    {
	        obj = create_object( obj_index );
	        obj_to_room( obj, ch->in_room );
	    }
	}
    }
    else if ( obj->material == MAT_PAPER )
    {
	obj->condition -= number_range( amount, 4 * amount );
	if ( obj->condition < 1 )
	{
	    if ( ch != NULL )
	        act( "$p gets soaked, and falls apart.\n\r", ch, obj, obj, TO_CHAR );
	    extract_obj( obj );
	}
    }
    else if ( obj->material == MAT_SILK )
    {
	obj->condition -= number_range( amount, 3 * amount );
	if ( obj->condition < 1 )
	{
	    if ( ch != NULL )
	        act( "$p gets soaked, and falls apart.\n\r", ch, obj, obj, TO_CHAR );
	    extract_obj( obj );
	}
    }
    else if ( obj->material == MAT_LEATHER )
    {
	obj->condition -= number_range( amount, 2 * amount  );
	if ( obj->condition < 1 )
	{
	    if ( ch != NULL )
	        act( "$p gets soaked, and falls apart.\n\r", ch, obj, obj, TO_CHAR );
	    extract_obj( obj );
	}
    }

    if ( obj->cont_data != NULL && !IS_SET( obj->cont_data->flags, CONT_WATER_PROOF ) )
	for ( obj_in = obj->cont_data->contents ; obj_in != NULL ; obj_in = obj_next )
	{
	    obj_next = obj_in->next_content;
	    check_rust( obj_in, amount - 1);
	}

}

void weather_affect( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA af;
    OBJ_DATA * obj;
    OBJ_DATA *obj_next;
    CHAR_DATA * rch;
    ROOM_INDEX_DATA * in_room = ch->in_room;
    ROOM_INDEX_DATA * to_room;
    EXIT_DATA * pexit;
    AREA_DATA * pArea = in_room->area;
    int sky = pArea->weather->sky;
    int door;
    int hour;
    int value;
    bool found = FALSE;

    if ( sky < 2 || !IS_OUTSIDE( ch )
      || IS_UNDERWATER( ch ) )
	return;

    switch ( sky )
    {
    default:
	break;
    case 2:
	if ( IS_SET( ch->res_flags, RES_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 4;
	else if ( IS_SET( ch->vuln_flags, VULN_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 2;
	else
	    value = get_curr_stat( ch, STAT_CON ) * 3;

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch == ch )
		return;

	    if ( IS_AFFECTED( rch, AFF_COLD ) )
		value -= 5;
	}

	if ( !IS_AFFECTED( ch, AFF_COLD ) && number_range( 0, 600 ) == 0 
	  && !IS_SET( ch->imm_flags, IMM_WATER )
	  && number_percent() / 2 > value )
	{
	    	af.type = 0;
	    	af.level = number_range( 10, 30 );
	    	af.duration = number_range( 10, 25 );
	    	af.location = APPLY_HITROLL;
	    	af.modifier = -3;
	    	affect_to_char( ch, &af );	
	    	af.location = APPLY_DAMROLL;
	    	af.modifier = -3;
	    	affect_to_char( ch, &af );	
	    	af.bitvector = AFF_COLD;
	    	af.bit_type = BIT_AFFECT;
	    	af.location = APPLY_AFFECT;
	    	affect_to_char( ch, &af );	
	    	send_to_char( "Aachoo! you just caught a cold.\n\r", ch );
	    	act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );
	}
	for ( obj = ch->carrying ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    check_rust( obj, 1 );	
	}
	break;
    case 3:
	for ( obj = ch->carrying ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    check_rust( obj, 1 );	
	}
	if ( IS_SET( ch->res_flags, RES_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 4;
	else if ( IS_SET( ch->vuln_flags, VULN_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 2;
	else
	    value = get_curr_stat( ch, STAT_CON ) * 3;

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch == ch )
		return;

	    if ( IS_AFFECTED( rch, AFF_COLD ) )
		value -= 5;
	}

	if ( !IS_AFFECTED( ch, AFF_COLD ) && number_range( 0, 600 ) == 0 
	  && !IS_SET( ch->imm_flags, IMM_WATER )
	  && number_percent() / 2 > value )
	{
	    af.type = 0;
	    af.level = number_range( 10, 30 );
	    af.duration = number_range( 10, 25 );
	    af.location = APPLY_HITROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    af.bitvector = AFF_COLD;
	    af.location = APPLY_AFFECT;
	    af.bit_type = BIT_AFFECT;
	    affect_to_char( ch, &af );	
	    af.bitvector = 0;
	    af.bit_type = 0;
	    af.location = APPLY_DAMROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    send_to_char( "Aachoo! you just caught a cold.\n\r", ch );
	    act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );
	}
	if ( number_range( 0, 5000 ) != 0 )
	    return;
	act( "You are hit by a lightning strike.", ch, NULL, NULL, TO_CHAR );
	act( "$n is hit by a lightning strike.", ch, NULL, NULL, TO_ROOM );
	ch->hit -= 2 * roll_dice( 40, 10 ) + 100;
	update_pos( ch );
	if ( ch->position == POS_DEAD )
	    raw_kill( ch, FALSE, FALSE );
	break;
    case 4:
	if ( IS_SET( ch->res_flags, RES_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 4;
	else if ( IS_SET( ch->vuln_flags, VULN_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 2;
	else
	    value = get_curr_stat( ch, STAT_CON ) * 3;

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch == ch )
		return;

	    if ( IS_AFFECTED( rch, AFF_COLD ) )
		value -= 5;
	}

	if ( !IS_AFFECTED( ch, AFF_COLD ) && number_range( 0, 200 ) == 0 
	  && !IS_SET( ch->imm_flags, IMM_WATER )
	  && number_percent() / 2 > value )
	{
	    af.type = 0;
	    af.level = number_range( 10, 30 );
	    af.duration = number_range( 10, 25 );
	    af.location = APPLY_HITROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    af.bitvector = AFF_COLD;
	    af.location = APPLY_AFFECT;
	    af.bit_type = BIT_AFFECT;
	    affect_to_char( ch, &af );	
	    af.bitvector = 0;
	    af.bit_type = 0;
	    af.location = APPLY_DAMROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    send_to_char( "Aachoo! you just caught a cold.\n\r", ch );
	    act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );
	}
	break;
    case 5:
	if ( IS_SET( ch->res_flags, RES_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 4;
	else if ( IS_SET( ch->vuln_flags, VULN_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 2;
	else
	    value = get_curr_stat( ch, STAT_CON ) * 3;

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch == ch )
		return;

	    if ( IS_AFFECTED( rch, AFF_COLD ) )
		value -= 5;
	}

	if ( !IS_AFFECTED( ch, AFF_COLD ) && number_range( 0, 100 ) == 0 
	  && !IS_SET( ch->imm_flags, IMM_WATER )
	  && 3 * number_percent() / 2 > value )
	{
	    af.type = 0;
	    af.level = number_range( 10, 30 );
	    af.duration = number_range( 10, 25 );
	    af.location = APPLY_HITROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    af.bitvector = AFF_COLD;
	    af.location = APPLY_AFFECT;
	    af.bit_type = BIT_AFFECT;
	    affect_to_char( ch, &af );	
	    af.bitvector = 0;
	    af.bit_type = 0;
	    af.location = APPLY_DAMROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    send_to_char( "Aachoo! you just caught a cold.\n\r", ch );
	    act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );
	}
	if ( !IS_AFFECTED( ch, AFF_BLIND ) && !IS_SET(ch->affected_by_2,AFF_LIGHT_SENSITIVE) && number_range( 0, 200 ) == 0 
	  && ( hour = get_hour( pArea ) ) >= time_table[ pArea->world ].sunrise
	  && hour <= time_table[ pArea->world ].sunset )
	{
	    af.type = gsn_blinding_light;
	    af.level = number_range( 10, 30 );
	    af.duration = number_range( 0, 2 ) / 2;
	    af.location = APPLY_HITROLL;
	    af.modifier = -4;
	    affect_to_char( ch, &af );	
	    af.bitvector = AFF_BLIND;
	    af.location = APPLY_HITROLL;
	    af.bit_type = APPLY_AFFECT;

	    affect_to_char( ch, &af );	
	    send_to_char( "You get blinded by the bright light on the snow.\n\r", ch );
	}
	break;
    case 6:
	if ( IS_SET( ch->res_flags, RES_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 4;
	else if ( IS_SET( ch->vuln_flags, VULN_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 2;
	else
	    value = get_curr_stat( ch, STAT_CON ) * 3;

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch == ch )
		return;

	    if ( IS_AFFECTED( rch, AFF_COLD ) )
		value -= 5;
	}

	if ( !IS_AFFECTED( ch, AFF_COLD ) && number_range( 0, 600 ) == 0 
	  && !IS_SET( ch->imm_flags, IMM_WATER )
	  && number_percent() / 2 > value )
	{
	    af.type = 0;
	    af.level = number_range( 10, 30 );
	    af.duration = number_range( 10, 25 );
	    af.location = APPLY_HITROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    af.location = APPLY_DAMROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    af.bitvector = AFF_COLD;
	    af.bit_type = BIT_AFFECT;
	    af.location = APPLY_AFFECT;
	    affect_to_char( ch, &af );	
	    send_to_char( "Aachoo! you just caught a cold.\n\r", ch );
	    act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );
	}
	for ( obj = ch->carrying ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    check_rust( obj, 1 );	
	}
	break;
    case 7:
	if ( IS_SET( ch->res_flags, RES_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 4;
	else if ( IS_SET( ch->vuln_flags, VULN_WATER ) )
	    value = get_curr_stat( ch, STAT_CON ) * 2;
	else
	    value = get_curr_stat( ch, STAT_CON ) * 3;

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch == ch )
		return;

	    if ( IS_AFFECTED( rch, AFF_COLD ) )
		value -= 5;
	}

	if ( !IS_AFFECTED( ch, AFF_COLD ) && number_range( 0, 600 ) == 0 
	  && !IS_SET( ch->imm_flags, IMM_WATER )
	  && number_percent() / 2 > value )
	{
	    af.type = 0;
	    af.level = number_range( 10, 30 );
	    af.duration = number_range( 10, 25 );
	    af.location = APPLY_HITROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    af.location = APPLY_DAMROLL;
	    af.modifier = -3;
	    affect_to_char( ch, &af );	
	    af.bitvector = AFF_COLD;
	    af.bit_type = BIT_AFFECT;
	    af.location = APPLY_AFFECT;
	    affect_to_char( ch, &af );	
	    send_to_char( "Aachoo! you just caught a cold.\n\r", ch );
	    act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );
	}
	for ( obj = ch->carrying ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    check_rust( obj, 1 );	
	}
	for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_data == NULL || obj->wear_data->weight > number_range( 0, 15 ) || number_range( 0, 2 ) != 0 )
		continue;

	    found = FALSE;
	    for( door = 0 ; door < 6 && !found ; door++ )
		if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
		  && ( to_room = pexit->u1.to_room ) != NULL
		  && to_room != ch->in_room )
		    found = TRUE;

	    if( !found )
		break;

	    while ( ( pexit = ch->in_room->exit[ ( door = number_range( 0, 5 ) ) ] ) == NULL
		  || ( to_room = pexit->u1.to_room ) == NULL
		  || to_room == ch->in_room );

	    sprintf( buf, "Strong winds blow $p %s.", dir_name[ door ] );
	    act( buf, ch, obj, obj, TO_CHAR );
	    act( buf, ch, obj, obj, TO_ROOM );
	    obj_from_room( obj );
	    obj_to_room( obj, to_room );
	    if ( in_room->people != NULL )
	    {
	        sprintf( buf, "Strong windws blow $p into the room." );
	        act( buf, in_room->people, obj, obj, TO_CHAR );
	        act( buf, in_room->people, obj, obj, TO_ROOM );
	    }
	}
	break;
    case 8:
	if ( IS_AFFECTED( ch, AFF_BLIND ) || IS_SET(ch->affected_by_2,AFF_LIGHT_SENSITIVE) || number_range( 0, 200 ) != 0 )
	    return;

	af.type = gsn_dirt;
	af.level = number_range( 10, 30 );
	af.duration = number_range( 0, 2 ) / 2;
	af.location = APPLY_HITROLL;
	af.modifier = -4;
	af.bitvector = AFF_BLIND;
	af.bit_type  = BIT_AFFECT;

	affect_to_char( ch, &af );	
	send_to_char( "Dirt gets blown into your eye.\n\r", ch );
	break;
    case 9:
	if ( number_range( 0, 2000 ) == 0 && !IS_SET( ch->in_room->room_flags, ROOM_NO_RECALL ) )
	{
	    for ( to_room = get_room_index( number_range( pArea->lvnum, pArea->uvnum ) ) ; 
		to_room == NULL || to_room->sector_type == SECT_UNDERWATER || to_room->sector_type == SECT_OCEAN_BOTTOM || IS_SET( to_room->room_flags, ROOM_NO_RECALL|ROOM_INDOORS ) ; 
		    to_room = get_room_index( number_range( pArea->lvnum, pArea->uvnum ) ) );

	    if ( ch->fighting != NULL )
		stop_fighting( ch, FALSE );
	    act( "You are picked up by a tornado and dropped into another room.", ch, NULL, NULL, TO_CHAR );
	    act( "A tornado picks up $n.", ch, NULL, NULL, TO_ROOM );
	    char_from_room( ch );
	    char_to_room( ch, to_room );
	    act( "A tornado drops $n into the room.", ch, NULL, NULL, TO_ROOM );
	    do_look( ch, "auto" );
	}
	for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_data == NULL || obj->wear_data->weight > number_range( 0, 15 ) || number_range( 0, 2 ) != 0 )
		continue;

	    found = FALSE;
	    for( door = 0 ; door < 6 && !found ; door++ )
		if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
		  && ( to_room = pexit->u1.to_room ) != NULL
		  && to_room != ch->in_room )
		    found = TRUE;

	    if( !found )
		break;

	    while ( ( pexit = ch->in_room->exit[ ( door = number_range( 0, 5 ) ) ] ) == NULL
		  || ( to_room = pexit->u1.to_room ) == NULL
		  || to_room == ch->in_room );

	    sprintf( buf, "Strong winds blow $p %s.", dir_name[ door ] );
	    act( buf, ch, obj, obj, TO_CHAR );
	    act( buf, ch, obj, obj, TO_ROOM );
	    obj_from_room( obj );
	    obj_to_room( obj, to_room );
	    if ( in_room->people != NULL )
	    {
	        sprintf( buf, "Strong windws blow $p into the room." );
	        act( buf, in_room->people, obj, obj, TO_CHAR );
	        act( buf, in_room->people, obj, obj, TO_ROOM );
	    }
	}
	break;
    case 10:
	if ( IS_AFFECTED( ch, AFF_BLIND ) || IS_SET(ch->affected_by_2,AFF_LIGHT_SENSITIVE) || number_range( 0, 200 ) != 0 )
	    return;

	af.type = gsn_dirt;
	af.level = number_range( 10, 30 );
	af.duration = number_range( 0, 2 ) / 2;
	af.location = APPLY_HITROLL;
	af.modifier = -4;
	af.bitvector = AFF_BLIND;
	af.bit_type  = BIT_AFFECT;

	affect_to_char( ch, &af );	
	send_to_char( "Dirt gets blown into your eye.\n\r", ch );
	break;
    case 11:
	if ( IS_AFFECTED( ch, AFF_BLIND ) || IS_SET(ch->affected_by_2,AFF_LIGHT_SENSITIVE) || number_range( 0, 75 ) != 0 )
	    return;

	af.type = gsn_dirt;
	af.level = number_range( 10, 30 );
	af.duration = number_range( 1, 3 );
	af.location = APPLY_HITROLL;
	af.modifier = -4;
	af.bitvector = AFF_BLIND;
	af.bit_type  = BIT_AFFECT;

	affect_to_char( ch, &af );	
	send_to_char( "Dirt gets blown into your eye.\n\r", ch );
	break;
    }
}

#define REV_TARGET( target )	( (target) == TAR_CHAR_DEFENSIVE ? TAR_CHAR_OFFENSIVE : ( (target) == TAR_CHAR_OFFENSIVE ? TAR_CHAR_DEFENSIVE : (target) ) )

bool use_area_affect( CHAR_DATA *ch, int target )
{
    CHAR_DATA * vch;

    switch( target )
    {
    case TAR_CHAR_DEFENSIVE:
    case TAR_CHAR_CURATIVE:
        for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room )
        {
	    if ( !IS_NPC( vch ) )
	        continue;

	    if ( vch == ch )
	        continue;

	    if ( is_same_group( vch, ch ) )
		return TRUE;

   	    if ( IS_SET( ch->off_flags, ASSIST_ALL ) )
		return TRUE;

            if ( vch->race == ch->race && IS_SET( ch->off_flags, ASSIST_RACE ) )
		return TRUE;

            if ( !vch->pIndexData)
	        continue;	

            if ( vch->pIndexData->vnum == ch->pIndexData->vnum && IS_SET( ch->off_flags, ASSIST_VNUM ) )
		return TRUE;
        }	
	break;
    case TAR_CHAR_DAMAGING:
    case TAR_CHAR_OFFENSIVE:
        for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room )
        {
	    if ( IS_NPC( vch ) || IS_IMMORTAL( vch ) )
	        continue;

	    if ( vch == ch->fighting )
		continue;

	    if ( vch->fighting == ch )
		return TRUE;

            if ( IS_SET( ch->act, ACT_AGGRESSIVE )  || 
	        ( !IS_SET( vch->guilds, MAJHERE ) && race_table[ ch->race ][ ch->sex ].world != race_table[ vch->race ][ vch->sex ].world && IS_SET( ch->act, ACT_ATTACK_WORLD ) ) 
	      || ( IS_SET( ch->act2, ACT2_ATTACK_GUILD ) && !IS_SET( ch->guilds, vch->guilds ) )
	      || is_name(ch->dislikes, vch->name ) 
	      || is_name(ch->hunting, vch->name ) )
		return TRUE;

    	     if ( vch->fighting == NULL || !IS_NPC( vch->fighting ) )
	        continue;

	    /* check for assiting */

	    if ( is_same_group( vch->fighting, ch ) || is_same_group( vch, ch->fighting ) )
		return TRUE;

	    if ( IS_SET( ch->off_flags, ASSIST_ALL ) )
		return TRUE;

            if ( vch->fighting->race == ch->race && IS_SET( ch->off_flags, ASSIST_RACE ) )
		return TRUE;
	
            if ( vch->fighting->pIndexData->vnum == ch->pIndexData->vnum && IS_SET( ch->off_flags, ASSIST_VNUM ) )
		return TRUE;
	}
    }

    return FALSE;
}
bool mob_has_mana( CHAR_DATA * ch, int sn )
{
    int type;

    for ( type = 0 ; type < MAX_ELEMENT_TYPE ; type++ )
	if ( get_mana_cost( ch, sn, type ) > ch->mana[ type ] )
	    return FALSE;

    return TRUE;
}

int get_skill_mob( CHAR_DATA *ch, CHAR_DATA * victim, int target, bool fArea ) 
{
    SKILL_LIST * skills;
    int sn;
    int spell = -1, level = -1;

    for ( skills = ch->innate_skills ; skills != NULL ; skills = skills->next )
    {
	sn = skills->sn;

        if ( IS_SET(skill_table[sn].target, TAR_AREA) && !fArea )
		continue;

	if ( !can_see( ch, victim ) 
	  && ( !fArea || !IS_SET( skill_table[ sn ].target, TAR_AREA ) )
	  && IS_SET( skill_table[sn].target, TAR_OBJ_INV|TAR_CHAR_OTHER ) )
		continue;

	if ( IS_SET(skill_table[sn].target, TAR_CHAR_SELF) && ch != victim ) 
		continue;	

        if ( IS_SET(skill_table[sn].target, target ) 
	  && !is_affected_bit( victim, sn )
	  && ( !IS_SET(skill_table[sn].spell_flags, SPELL_PROTECTION )
		 || !IS_SET(ch->spell_flags, SPELL_PROTECTION) ) 
	  && ( !IS_SET(skill_table[sn].spell_flags, SPELL_SHIELD )
		 || !IS_SET(ch->spell_flags, SPELL_SHIELD) ) 
          && ( skill_table[sn].spell_fun == spell_null 
	     	 || ( mob_has_mana( ch, sn )
               && ( !IS_SET(skill_table[sn].target, TAR_CHAR_CANCEL)
	         || !check_is_dispelled( victim, sn, REV_TARGET( target ) )   ) ) )
	  && ( level < 1 || number_range(0,1) == 1 ) )
	    {
		level = ch->level;
	        spell = sn;
		if ( number_range(0,2) == 1 )
    			return spell;
		level = -1;
		spell = -1;
	    }
    }

    for ( skills = ch->skills ; skills != NULL ; skills = skills->next )
    {
	sn = skills->sn;

        if ( IS_SET(skill_table[sn].target, TAR_AREA) && !fArea )
		continue;

	if ( !can_see( ch, victim ) 
	  && ( !fArea || !IS_SET( skill_table[ sn ].target, TAR_AREA ) )
	  && IS_SET( skill_table[sn].target, TAR_OBJ_INV|TAR_CHAR_OTHER ) )
		continue;

	if ( IS_SET(skill_table[sn].target, TAR_CHAR_SELF) && ch != victim ) 
		continue;	

        if ( IS_SET(skill_table[sn].target, target ) 
	  && !is_affected_bit( victim, sn )
	  && ( !IS_SET(skill_table[sn].spell_flags, SPELL_PROTECTION )
		 || !IS_SET(ch->spell_flags, SPELL_PROTECTION) ) 
	  && ( !IS_SET(skill_table[sn].spell_flags, SPELL_SHIELD )
		 || !IS_SET(ch->spell_flags, SPELL_SHIELD) ) 
          && ( skill_table[sn].spell_fun == spell_null 
	     	 || ( mob_has_mana( ch, sn )
               && ( !IS_SET(skill_table[sn].target, TAR_CHAR_CANCEL)
	         || !check_is_dispelled( victim, sn, REV_TARGET( target ) )   ) ) )
	  && ( level < 1 || number_range(0,1) == 1 ) )
	    {
		level = get_skill_level( ch, sn);
	        spell = sn;
		if ( number_range(0,2) == 1 && level <= ch->level )
    			return spell;
	    }
    }
    if ( spell == 0 )
	bug( "Get_skill_mob: no spell found", ch->pIndexData->vnum );
    return spell;
}

bool mob_skill( CHAR_DATA *ch )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int race,level,spell;
    bool fArea;

    if ( ch->position != POS_STANDING && ch->position != POS_FIGHTING )
	return FALSE;

    if ( ch->pIndexData == NULL || ch->pIndexData->pShop != NULL ||
	IS_SET( ch->act, ACT_GAIN ) || IS_SET( ch->act, ACT_TRAIN ) )
	return FALSE;

    if ( ch->skills == NULL && ch->innate_skills == NULL )
	return FALSE;

    level = -1;
    spell = -1;

    race = ch->race;
    victim = ch;  

    if ( ch->fighting != NULL && ch->hit < ch->max_hit / 3 && ch->hit < ch->fighting->hit )
    {
	victim = ch;
	spell = get_skill_mob( ch, victim, TAR_CHAR_CURATIVE, use_area_affect( ch, TAR_CHAR_CURATIVE ) );
    }

    if ( spell == -1 && ch->fighting != NULL )
    {
        fArea = use_area_affect( ch, TAR_CHAR_DAMAGING );
	victim = ch->fighting;
	if ( number_range( 0, 10 ) < 6 )
	{
	    spell = get_skill_mob( ch, victim, TAR_CHAR_OFFENSIVE, fArea );
	    if ( spell == -1 )
		spell = get_skill_mob( ch, victim, TAR_CHAR_DAMAGING, fArea );
 	}
	else
 	{
	    spell = get_skill_mob( ch, victim, TAR_CHAR_DAMAGING, fArea );
	    if ( spell == -1 )
		spell = get_skill_mob( ch, victim, TAR_CHAR_OFFENSIVE, fArea );
	}
    }

    fArea = use_area_affect( ch, TAR_CHAR_CURATIVE );

    if ( spell == -1 && ch->hit < ch->max_hit * 9 / 10 )
    {
	victim = ch;
	spell = get_skill_mob( ch, victim, TAR_CHAR_CURATIVE, fArea );
    }
    if ( spell < 1 && fArea )  /* cure a friend */
    {
	victim = NULL;
        for ( vch = ch->in_room->people ; vch != NULL && victim == NULL ; vch = vch_next )
	{
		vch_next = vch->next_in_room;
		if ( !IS_NPC( vch ) || vch == ch || vch->hit >= vch->max_hit * 9 / 10 )
		    continue;
		if ( ch->leader != NULL && IS_NPC( ch->leader ) && !is_same_group( ch, vch ) )
		    continue;
	 	else if ( IS_SET( ch->off_flags, ASSIST_RACE ) && vch->race != ch->race )
		    continue;
	 	else if ( IS_SET( ch->off_flags, ASSIST_VNUM ) && vch->pIndexData->vnum != ch->pIndexData->vnum )
		    continue;

		victim = vch;
		spell = get_skill_mob( ch, victim, TAR_CHAR_CURATIVE, fArea );
	}
    }
    if ( spell == -1 && number_range( 0, 1 ) == 0 )
    {
	victim = ch;
	spell = get_skill_mob( ch, victim, TAR_CHAR_DEFENSIVE, fArea );
        if ( spell == -1 && fArea )
        {
 	    victim = NULL;
            for ( vch = ch->in_room->people ; vch != NULL && victim == NULL ; vch = vch_next )
            {
		vch_next = vch->next_in_room;
		if ( !IS_NPC( vch ) || vch == ch )
		    continue;
		if ( ch->leader != NULL && IS_NPC( ch->leader ) && !is_same_group( ch, vch ) )
		    continue;
	 	else if ( IS_SET( ch->off_flags, ASSIST_RACE ) && vch->race != ch->race )
		    continue;
	 	else if ( IS_SET( ch->off_flags, ASSIST_VNUM ) && vch->pIndexData->vnum != ch->pIndexData->vnum )
		    continue;
		victim = vch;
		spell = get_skill_mob( ch, victim, TAR_CHAR_DEFENSIVE, fArea );
	    }
	}
    }

    if ( spell == -1 || victim == NULL )
	return FALSE;

    if ( skill_table[ spell ].spell_fun != spell_null )
    {
	if ( victim != ch )
	    sprintf( buf, "'%s' '%s'", skill_table[ spell ].name,  victim->name );
	else
	    sprintf( buf, "'%s'", skill_table[ spell ].name );
	do_cast( ch, buf );
	return TRUE;
    }
    else
    {
	sprintf( buf, "%s %s", skill_table[ spell ].name, victim->name );
	interpret( ch, buf );
	return TRUE;
    }
}

const int mana_stat[ MAX_ELEMENT_TYPE ] =
{
	STAT_STR, /*  Fire   */
	STAT_INT, /*  Water  */
	STAT_CON, /*  Earth  */
	STAT_DEX, /*  Wind   */
	STAT_WIS  /*  Spirit */
};

void gain_exp( CHAR_DATA *ch, int gain )
{
    int levels=0;
    int gain_prac=0;

    if ( IS_NPC(ch)) 
	return;

    if ( IS_IMMORTAL(ch))
	return; 

    if ( ch->level >= 60 ) /* level 60 */
    	return;

    ch->exp += gain;
    while ( ch->exp >= exp_table[ ch->level + 1 ].exp_at_level )
    {
	ch->level += 1;
	levels++;
    }

    if ( levels < 1 )
	return;

    if ( levels < 2 )
    {
    	gain_prac += wis_app[get_curr_stat(ch, STAT_WIS)].practice + number_range(0, 3);
    	ch->practice += gain_prac;
        sprintf( log_buf, "`RYou gain %d practices`w\n\r", gain_prac ); 
	send_to_char( log_buf, ch );
	sprintf( log_buf, "`RYou raise a level!!`w\n\r");
	send_to_char( log_buf, ch );
	sprintf( log_buf, "%s has gained a level.", ch->name);
        info_channel( ch, log_buf, INFOACT_LEVEL );
	log_string(log_buf);
	return;
    }

    sprintf( log_buf, "`RYou raise %d levels!!`w\n\r", levels );
    send_to_char( log_buf, ch );
    sprintf( log_buf, "%s has gained $d levels.", ch->name );
    info_channel( ch, log_buf, INFOACT_LEVEL );
    log_string(log_buf);
    return;
}

/*
 * Regeneration stuff.
 */

void set_position( CHAR_DATA * ch, int position )
{
    ch->position = position;
    ch->regen_timer = current_time;
}

void check_regen( CHAR_DATA * ch )
{
    if ( ch->position >= POS_DEAD )
    {
	hit_gain( ch );
	mana_gain( ch );
	move_gain( ch );
    }

    ch->regen_timer = current_time;
} 

void hit_gain( CHAR_DATA *ch )
{
    int gain;
    int number;

    if ( IS_NPC(ch) )
    {
	gain =  5 + ch->level;
 	if (IS_AFFECTED(ch,AFF_REGENERATION))
	    gain += gain/2;
        if (IS_SET(ch->in_room->room_flags,ROOM_HPGEN))
         { gain = gain * 2; } 
	switch(ch->position)
	{
	    default : 		gain /= 2;			break;
	    case POS_SLEEPING: 	gain = 3 * gain/2;		break;
	    case POS_RESTING:  					break;
	    case POS_FIGHTING:	gain /= 3;		 	break;
 	}
    }
    else
    {
	gain = UMAX(3,get_curr_stat(ch,STAT_CON) - 3 + ch->level/2); 
 	number = number_percent();

	if (number < get_skill( ch, gsn_bind_wounds ) )
	{
	    gain += number * gain / 100;
	    if (ch->hit < ch->max_hit)
		check_improve(ch,gsn_bind_wounds,TRUE,6);
	}

	if (number < get_skill( ch, gsn_rejuvenate ) )
	{
	    gain += number * gain / 100;
	    if (ch->hit < ch->max_hit)
		check_improve(ch,gsn_rejuvenate,TRUE,6);
	}

 	if (IS_AFFECTED(ch,AFF_REGENERATION))
	    gain += gain/2;

        if (IS_SET(ch->in_room->room_flags,ROOM_HPGEN))
            gain *= 3; 

	switch ( ch->position )
	{
	    default:	   	gain /= 4;			break;
	    case POS_SLEEPING: 					break;
	    case POS_RESTING:  	gain /= 2;			break;
	    case POS_FIGHTING:  gain /= 8;			break;
	}

	if (IS_AFFECTED(ch, AFF_REGENERATION) && ch->position == POS_FIGHTING)
		gain *= 2;		

	if ( ch->pcdata->condition[COND_DRUNK] != 0 )
	    gain *= 2;
    }

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
	gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE))
	gain /=2 ;

    gain = gain * ( ( int ) ( current_time - ch->regen_timer ) ) / 30;

    ch->hit = UMIN( ch->hit + gain , ch->max_hit );
}


void mana_gain( CHAR_DATA *ch )
{
    int gain;
    int number;
    int type;

    if (IS_NPC(ch) && (ch->position != POS_FIGHTING ) )
    {
        for ( type = 0 ; type < MAX_ELEMENT_TYPE ; type++ )
        {
            ch->mana[ type ] = UMIN( ch->max_mana[ type ], ch->mana[ type] + ( 3 * (ch->level) ) );
	}      
        return;
    }

    for ( type = 0 ; type < MAX_ELEMENT_TYPE ; type++ )
    {
	gain = 4 * mana_gain_table[ get_curr_stat( ch, mana_stat[ type ] ) ];
	gain *= ( get_skill( ch, gsn_element_power[ type ][ 0 ] ) +
		  get_skill( ch, gsn_element_power[ type ][ 1 ] ) +
		  get_skill( ch, gsn_element_power[ type ][ 2 ] ) );
	gain /= 100;
	number = number_percent();

    	if(!IS_NPC(ch) && (number < get_skill( ch, gsn_meditation ) ) )
    	{
            gain += number * gain / 100;
            if (ch->mana[ type ] < ch->max_mana[ type ])
        	check_improve(ch,gsn_meditation,TRUE,10);
    	}

	if ( !IS_NPC( ch ) && ( number < get_skill( ch, gsn_deep_meditation ) ) )
	{
	    gain += number * gain / 100;
	    if ( ch->mana[ type ] < ch->max_mana[ type ] )
		check_improve( ch, gsn_deep_meditation, TRUE, 10 );
	}

        if ( IS_SET( ch->in_room->room_flags, ROOM_MANAGEN ) )
            gain *= 3;
	else if ( IS_SET( ch->affected_by_2, AFF_MANA_GEN ) )
	    gain *= 2;

	switch ( ch->position )
	{
	    default:		gain /= 4;			break;
	    case POS_SLEEPING: 	gain *= 2;			break;
	    case POS_RESTING:	gain /= 2;			break;
	    case POS_FIGHTING:	gain /= 6;			break;
	}

/* if NPC calculate and stop here */
    	if ( IS_NPC(ch) )
	{
        	gain = gain * ( ( int ) ( current_time - ch->regen_timer ) ) / 30;
		ch->mana[ type ] = UMIN( ch->max_mana[ type ] , ch->mana[ type ] + gain );
		return;
	}
	if ( ch->pcdata->condition[COND_FULL]  <= 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] <= 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_DRUNK] > 0 )
	    gain /= 2;

        if ( IS_AFFECTED( ch, AFF_POISON ) )
	    gain /= 4;

        if (IS_AFFECTED(ch, AFF_PLAGUE))
            gain /= 8;

        if (IS_AFFECTED(ch,AFF_HASTE))
            gain /=2 ;

        gain = gain * ( ( int ) ( current_time - ch->regen_timer ) ) / 30;

	gain = UMAX( gain, 5); 

	ch->mana[ type ] = UMIN( ch->max_mana[ type ] , ch->mana[ type ] + gain );
    }
}

void move_gain( CHAR_DATA *ch )
{
    int number,gain;

    if ( IS_NPC(ch) )
    {
	gain = ch->level * 5;
    }
    else
    {
        gain = UMAX(3,get_curr_stat(ch,STAT_CON) - 3 + ch->level/2);

	switch ( ch->position )
	{
	case POS_STANDING: gain /= 3;					break;
	case POS_FIGHTING: gain /= 6;					break;
	case POS_SLEEPING: gain += get_curr_stat(ch,STAT_CON);		break;
	case POS_RESTING:  gain += get_curr_stat(ch,STAT_CON) / 2;	break;
	}
        if (IS_SET (ch->in_room->room_flags,ROOM_MVGEN))
               gain *= 4;
	if ( ch->pcdata->condition[COND_FULL]   == 0 )
	    gain /= 2;

	if ( ch->pcdata->condition[COND_THIRST] == 0 )
	    gain /= 2;
    }

    number = number_percent();
    if(!IS_NPC(ch) && (number < get_skill( ch, gsn_youna ) ) )
    {
        gain += number * gain / 100;
        if (ch->move < ch->max_move)
        check_improve(ch,gsn_youna,TRUE,6);
    }

    if ( IS_AFFECTED(ch, AFF_POISON) )
	gain /= 4;

    if (IS_AFFECTED(ch, AFF_PLAGUE))
        gain /= 8;

    if (IS_AFFECTED(ch,AFF_HASTE))
        gain /=2 ;

    gain = gain * ( ( int ) ( current_time - ch->regen_timer ) ) / 30;

    ch->move = UMIN( ch->move + gain, ch->max_move );
}

void gain_condition( CHAR_DATA *ch, int iCond, int value )
{
    CHAR_DATA *victim;
    int condition;

    if ( value == 0 || IS_NPC(ch) || IS_IMMORTAL(ch) || ch->level <= 5 )
	return;

    if ( ch->pcdata->condition[iCond] < 0 )
    {
	ch->pcdata->condition[iCond] = 0;
	return;
    }

    condition				= ch->pcdata->condition[iCond];
    if ( value > 0 )
	condition = UMAX( 0, condition );

    ch->pcdata->condition[iCond]	= URANGE( 0, condition + value, race_table[ ch->race ][ ch->sex ].max_condition[iCond] );

    if ( iCond == COND_DRUNK )
	ch->pcdata->condition[iCond]	= UMAX( 0, ch->pcdata->condition[iCond]);

    if ( ch->pcdata->condition[iCond] <= 0 )
    {
	switch ( iCond )
	{
	case COND_FULL:
        if ( condition >= 1 && ch->position > POS_SLEEPING )
            send_to_char( "You are hungry.\n\r",  ch );
	    break;

	case COND_THIRST:
        if ( condition >= 1 && ch->position > POS_SLEEPING )
            send_to_char( "You are thirsty.\n\r", ch );
	    break;

	case COND_DRUNK:
	    if ( condition != 0 )
            send_to_char( "You are sober.\n\r", ch );
	    break;
	}
    }

    update_pos( ch );
    victim = ch;
    switch( victim->position )
    {
    case POS_MORTAL:
	cact( "$n is mortally wounded, and will die soon, if not aided.",
	    victim, NULL, NULL, TO_ROOM, "`r" );
	send_to_char( 
	    "You are mortally wounded, and will die soon, if not aided.\n\r",
	    victim );
	break;

    case POS_INCAP:
	cact( "$n is incapacitated and may die, if not aided.",
	    victim, NULL, NULL, TO_ROOM, "`R" );
	send_to_char(
	    "You are incapacitated and may die, if not aided.\n\r",
	    victim );
	break;

    case POS_STUNNED:
	cact( "$n is stunned, but will probably recover.",
	    victim, NULL, NULL, TO_ROOM, "`Y" );
	send_to_char("You are stunned, but will probably recover.\n\r",
	    victim );
	break;

    case POS_DEAD:
	cact( "$n is DEAD!!", victim, 0, 0, TO_ROOM, "`R" );
	send_to_char( "You have been KILLED!!\n\r\n\r", victim );
	raw_kill( victim, FALSE, FALSE );
	break;

    }
    return;
}

void remove_track( EXIT_DATA * pexit )
{
    TRACK_DATA * tracks;
    TRACK_DATA * tracks_next;

    if ( pexit == NULL || pexit->tracks == NULL )
	return;

    for ( ; pexit->tracks != NULL && pexit->tracks->time < current_time ; pexit->tracks = tracks_next )
    {
	tracks_next = pexit->tracks->next;
        pexit->tracks->next = track_data_free;
        track_data_free = pexit->tracks;
        
    }

    if ( pexit->tracks == NULL )
	return;

    for ( tracks = pexit->tracks ; tracks != NULL ; )
    {
	tracks_next = tracks->next;
	if( tracks_next != NULL && tracks_next->time < current_time )
	{
	    tracks->next = tracks_next->next;
        tracks_next->next = track_data_free;
        track_data_free = tracks_next;
	}
	else 
	    tracks = tracks_next;
    }

}

void 	wear_best( CHAR_DATA *ch )
{
    OBJ_DATA * obj;
    OBJ_DATA * obj_best;
    OBJ_DATA * obj_wear;
    int iWear;

    if ( !IS_NPC( ch ) )
	return;

    for ( iWear = 0 ; iWear < MAX_WEAR ; iWear++ )
    {
	obj_best = ( obj_wear = get_eq_char( ch, iWear ) );

	for ( obj = ch->carrying ; obj != NULL ; obj = obj->next_content )
	{
	    if ( obj->wear_data == NULL )
		continue;
	    
	    if ( !IS_SET( obj->wear_data->wear_flags, wear_location_flag( iWear ) ) )
		continue;

	    if ( obj->wear_data->wear_loc != WEAR_NONE )
		continue;

            if (   ( IS_OBJ_STAT( obj, ITEM_ANTI_NENKEMEN  ) && race_table[ ch->race ][ ch->sex ].world == 0 )
		|| ( IS_OBJ_STAT( obj, ITEM_ANTI_MAEGMENEL    ) && race_table[ ch->race ][ ch->sex ].world == 1 )
		|| ( IS_OBJ_STAT( obj, ITEM_ANTI_LITHDOR      ) && race_table[ ch->race ][ ch->sex ].world == 2 ) )
		continue;

	    if ( ( obj_best == NULL ? 0 : obj_best->mpoints ) < obj->mpoints )
		obj_best = obj;

	}

	if ( obj_best != obj_wear )
	{
		/* Do mobs have duel wield????
		 * Otherwise they switch from left to right with the
		 * same object */
		if ( iWear != WEAR_WIELD_L )
	       wear_obj( ch, obj_best, TRUE );
	}


    }    

}

OBJ_DATA * get_magic_item( CHAR_DATA *ch, int TARGET )
{
    SPELL_LIST * spell;
    OBJ_DATA * obj;
    OBJ_DATA * obj_best = NULL;
    int points;
    int best=0;
 
    for ( obj = ch->carrying ; obj != NULL ; obj = obj->next_content )
    {
	points = 0;
	if ( obj->wear_data->magic_data != NULL && obj->wear_data->wear_loc != WEAR_NONE && obj->wear_data->magic_data->charges > 0 )
	    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
	    {
		if ( IS_SET( skill_table[ spell->spell ].target, TARGET )
	          && ( ( !IS_SET(TARGET, TAR_CHAR_DEFENSIVE) 
		      || ( !is_affected_bit( ch, spell->spell ) 
		        || ( IS_SET(TARGET, TAR_CHAR_CANCEL) 
			  && !check_is_dispelled( ch, spell->spell, TAR_CHAR_DEFENSIVE ) ) ) )
		    && ( !IS_SET(TARGET, TAR_CHAR_OFFENSIVE) 
		      || ( !is_affected_bit( ch->fighting, spell->spell ) 
                        || ( IS_SET(TARGET, TAR_CHAR_CANCEL) 
			  && !check_is_dispelled( ch->fighting, spell->spell, TAR_CHAR_OFFENSIVE ) ) ) ) ) )
		points += spell->level * ( ( skill_table[ spell->spell ].element[ 0 ].mana + skill_table[ spell->spell ].element[ 1 ].mana + 
			skill_table[ spell->spell ].element[ 2 ].mana + skill_table[ spell->spell ].element[ 3 ].mana + skill_table[ spell->spell ].element[ 4 ].mana )  / 15 + 1 );
	    }
	if ( obj->edible_data != NULL && IS_SET( TARGET, TAR_CHAR_DEFENSIVE|TAR_CHAR_CURATIVE|TAR_CHAR_CANCEL )
	  && obj->edible_data->amount > 0 )
	    for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell->next )
	    {
		if ( IS_SET( skill_table[ spell->spell ].target, TARGET )
	          && ( ( !IS_SET(TARGET, TAR_CHAR_DEFENSIVE) 
		      || ( !is_affected_bit( ch, spell->spell ) 
		        || ( IS_SET(TARGET, TAR_CHAR_CANCEL) 
			  && !check_is_dispelled( ch, spell->spell, TAR_CHAR_DEFENSIVE ) ) ) ) ) )
		points += spell->level * ( ( skill_table[ spell->spell ].element[ 0 ].mana + skill_table[ spell->spell ].element[ 1 ].mana + 
			skill_table[ spell->spell ].element[ 2 ].mana + skill_table[ spell->spell ].element[ 3 ].mana + skill_table[ spell->spell ].element[ 4 ].mana )  / 15 + 1 );
	    }
	if ( points > best )
	{
	    best = points;
	    obj_best = obj;
	}
    }
    return obj_best;
}

void web_who_update ( void )
{
    char new_time[MAX_INPUT_LENGTH];
    FILE *fp;
    CHAR_DATA * ch;
    int players = 0;
    int immortals = 0;

    if ( mud_port != 6100 )
	return;

    if ( ( fp = fopen( "/home/httpd/html/who.html" , "w" )  ) == NULL )
    {
	bug( "Unable to open web who file.", 0 );
	if ( fp )
	fclose( fp );
	return;
    }

    strcpy( new_time, (char *) ctime( &current_time ) );
    new_time[ strlen( new_time ) -1 ] = '\0';
    fprintf( fp, "<HTML>\n" );
    fprintf( fp, "<TITLE>who is on Electric Dreams</TITLE>\n\n" );
    fprintf( fp, "<BASEFONT SIZE=+1>\n" );
    fprintf( fp, "<BODY BGCOLOR=%c#000000%c BACKGROUND=%cimages/stars.gif%c text=%c#0080FF%c VLINK=%c#800080%c>\n", 34, 34, 34, 34, 34, 34, 34, 34 );
    fprintf( fp, "<CENTER><HEAD><FONT SIZE=+3>Who is on our Mudsite</HEAD><BR>\n" );
    fprintf( fp, "<FONT SIZE=-1>Refresh or reload viewer to update<BR>\n" );
    fprintf( fp, "<FONT SIZE=-1>Last updated %s<BR>\n", new_time );
    fprintf( fp, "<BODY>\n" );
    fprintf( fp, "<PRE><FONT SIZE=+2>\n\n" );

    for( ch = char_list ; ch != NULL ; ch = ch->next )
    {
	if ( !IS_NPC( ch )
	  && !IS_SET( ch->plr, PLR_WIZINVIS )
	  && !IS_SET( ch->affected_by, AFF_SNEAK )
	  && !IS_SET( ch->affected_by, AFF_HIDE )
	  && !IS_SET( ch->affected_by_2, AFF_FADE )
	  && !IS_SET( ch->affected_by, AFF_SHADOWFORM ) )
	{
	    if ( IS_IMMORTAL( ch ) )
		immortals++;
	    else
	        players++;
	    fprintf( fp, "%s%s\n", ch->name, ch->pcdata->title );
	}
    }

    fprintf( fp, "</center>\n\n");
    fprintf( fp, "%d visible players.\n%d visible immortals.\n", players, immortals );
    fprintf( fp, "\n\n");
    fprintf( fp, "<A HREF=%cindex.html%c>Take me back to the Home page.</A>\n\n\n", 34, 34 );
    fprintf( fp, "</BODY>\n</html>\n" );
    if ( fp )
    fclose( fp );

}
void room_update_trigger( void )
{
    ROOM_INDEX_DATA * room;

    for ( room = room_trig_list ; room != NULL; room = room->next_with_trigger )
	if (HAS_SCRIPT_ROOM(room) )
		script_update_room(room , TRIG_EACH_PULSE );
	
}

void room_update( void )
{
    EXIT_DATA * pexit;
    ROOM_INDEX_DATA * room;
    AFFECT_DATA * paf;
    AFFECT_DATA * paf_next;
    int iHash;
    int door;

    for ( iHash = 0 ; iHash < MAX_KEY_HASH ; iHash++ )
    {
	for ( room = room_index_hash[iHash ] ; room != NULL ; room = room->next )
	{
	    for ( door = 0 ; door < 6 ; door++ )
	    {
		pexit = room->exit[ door ];

	 	if ( pexit != NULL )
		    if ( pexit->tracks != NULL )
		    remove_track( pexit );
	    }

	    for ( paf = room->affected_by ; paf != NULL; paf = paf_next )
	    {
		paf_next = paf->next;

		if ( paf->duration > 0 )
		{
		    paf->duration--;
		    if ( number_range( 0, 4 ) == 0 && paf->level > 0 )
			paf->level--;
		}
		else
		{
		    if ( paf_next == NULL
		    ||   paf_next->type != paf->type
		    ||   paf_next->duration > 0 )
		    {
		        if ( paf->type > 0 && skill_table[paf->type].msg_off )
		        {
			    send_to_room( room->vnum, skill_table[paf->type].msg_off );
			    send_to_room( room->vnum, "\n\r" );
		        }
		    }
		    affect_remove_room( room, paf );

		}
	    }
	}
    }
}

/*
 * Mob autonomous action.
 * This function takes 25% to 35% of ALL Merc cpu time.
 * -- Furey
 */
void mobile_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *pRoom;
    ROOM_INDEX_DATA *location;
    EXIT_DATA *pexit;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *obj2;
    OBJ_DATA *obj2_next;
    MOB_INDEX_DATA *pMob;
    bool found = FALSE;
    int door;
    char buf[MAX_INPUT_LENGTH];
    /* Examine all mobs. */
    /*
     *  Note, there are 16 lists of mobs.
     */
    for ( ch = mob_list_current_update; ch != NULL; ch = ch_next )
    {
	ch_next = ch->mob_next_update;

    	if ( !IS_NPC(ch) || ch->in_room == NULL )
    	    continue;
	if ( ch->extract == TRUE )
	{
	extract_char(ch, TRUE);
	continue;
	}

	if (ch->in_room->area->nplayer == 0 && !IS_SET(ch->act,ACT_UPDATE_ALWAYS))
	    continue;
	

	if ( ch->fighting == NULL )
 	    ch->wait = UMAX( 0, ch->wait - PULSE_MOBILE  );
	if ( ch->wait > 0 )
	    continue;

	if (HAS_SCRIPT(ch) )
	{
		script_update(ch , TRIG_EACH_PULSE );
	}
	if ( IS_AFFECTED(ch,AFF_CONTROL_MIND))
	    continue;

        if ( ch->task[0] != '\0' && ch->fighting == NULL && strlen(ch->task) > 0 )
        {
                do_task( ch );
        }

	if ( IS_SET( ch->act, ACT_HUNTER ) && ch->fighting == NULL 
	  && ( ch->dislikes[0] != '\0' || IS_SET( ch->act, ACT_AGGRESSIVE )
	  || ch->hunting[0] != '\0'
	  || IS_SET( ch->act, ACT_ATTACK_WORLD) 
	  || IS_SET( ch->act2, ACT2_ATTACK_GUILD )
	  || IS_SET( ch->off_flags, ASSIST_ALL|ASSIST_RACE|ASSIST_VNUM ) )
	  && !IS_SET( ch->act, ACT_SENTINEL ) ) 
	{
	    if ( ch->hunting[0] == '\0' && ch->dislikes[0] != '\0' ) 
	        strncpy(ch->hunting, ch->dislikes,30);
	    for ( d = descriptor_list ; d != NULL && ch->hunting[0] == '\0' ; d = d->next )
	    {
		if ( d->connected <= CON_PLAYING
		  && d->character->in_room->area == ch->in_room->area 
		  && !IS_IMMORTAL( d->character )
		  && number_range( 0 , 30 ) > get_curr_stat( d->character , STAT_CHR )
		  && (  ( !IS_SET( d->character->guilds, MAJHERE ) 
			&& race_table[ d->character->race ][ d->character->sex ].world != race_table[ ch->race ][ ch->sex ].world 
			&& IS_SET( ch->act, ACT_ATTACK_WORLD ) )
		  ||  ( IS_SET( ch->act2, ACT2_ATTACK_GUILD ) 
			&& !IS_SET( d->character->guilds, ch->guilds ) )
		  ||  ( d->character->fighting != NULL 
			&& IS_NPC( d->character->fighting ) 
			&& ( IS_SET( ch->off_flags, ASSIST_ALL ) 
  	  	  ||  ( d->character->fighting->race == ch->race 
			&& IS_SET( ch->off_flags, ASSIST_RACE ) ) 
		  ||  ( d->character->fighting->pIndexData->vnum == ch->pIndexData->vnum 
			&& IS_SET( ch->off_flags, ASSIST_VNUM ) ) ) )
		  || IS_SET( ch->act, ACT_AGGRESSIVE ) ) )
		{
		    /* send something to the player? */
		    strncpy(ch->hunting, d->character->name,30);
		    ch->task[0] = '\0';
		    continue;
		}
		if ( d->connected <= CON_PLAYING
		  && d->character->in_room->vnum == ch->in_room->vnum 
		  && d->character->fighting != NULL
		  && IS_SET( ch->off_flags, ASSIST_PLAYERS )
		  && ( !IS_SET( ch->off_flags, ASSIST_RACE ) || ch->race == d->character->race )
		  && !IS_IMMORTAL( d->character )
		  && number_range( 0 , 26 ) < get_curr_stat( d->character , STAT_CHR ) )
		{
		    act("$N tells you 'I will save you!'", d->character, NULL, ch, TO_CHAR );
		    strncpy(ch->hunting, d->character->fighting->name,30);
		}
	    }
	}
	/* Examine call for special procedure */
	if ( ch->spec_fun != 0 )
	{
	    if ( (*ch->spec_fun) ( ch ) )
		continue;
	}
	if ( ch->hunting[0] != '\0' )
	{
 	        if ( do_mphunt( ch, ch->hunting ) )
		{
	            continue;
	  	}
	}
	if (IS_SET(ch->act2, ACT2_WEAPON_LOST ) 
	    && !IS_AFFECTED(ch, AFF_BLIND)
	    && !IS_SET(ch->affected_by_2, AFF_LIGHT_SENSITIVE) 
	    && number_bits( 1 ) == 0 )
	{

	    REMOVE_BIT(ch->act2, ACT2_WEAPON_LOST );
	    for ( obj = ch->in_room->contents; obj; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( obj->pIndexData->vnum == 11 )
			continue;
		if ( obj->wear_data != NULL )
	 	if ( obj->wear_data->weapon_data != NULL )
		if ( CAN_WEAR( obj, ITEM_TAKE )
		    && can_see_obj( ch, obj ) )
		{
  		    obj_from_room( obj );
		    obj_to_char( obj, ch );
		    act( "$n gets $p.", ch, obj, NULL, TO_ROOM );
		}
	    }
	    wear_best( ch );
	}
	if ( mob_skill( ch ) )
		continue;

	if ( IS_NPC(ch) 
	&&  ch->pIndexData->follower != 0  
	&&  ch->group == NULL 
 	&&  ch->master == NULL )
            for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
            {
		vch_next = vch->next_in_room;
		if ( vch == ch )
		continue;
		if ( IS_NPC(vch))
		{
		if (ch->pIndexData->follower == vch->pIndexData->vnum  )
		{
		    if (vch->group == NULL )
		    {
			sprintf(buf,"xyZzy %s",vch->name );
			do_found(vch, buf );
		    }
		    add_follower(ch, vch);
		    add_to_group(ch, vch); 
		    ch->task[0] = '\0';
		}
		
		}
	    }	

	if ( ch->position < POS_RESTING )
	  continue;

	/* Scavenge */
	if ( IS_SET( ch->act, ACT_SCAVENGER )
	    && !IS_AFFECTED(ch, AFF_BLIND)
	    && !IS_SET(ch->affected_by_2, AFF_LIGHT_SENSITIVE) 
	    && number_bits( 1 ) == 0 )
	{
	    OBJ_DATA *obj_best;
	    int       max;

	   found = FALSE;
	    max         = 0;
	    obj_best    = 0;
	    for ( obj = ch->in_room->contents; obj; obj = obj_next )
	    {
		obj_next = obj->next_content;
		if ( IS_SET( obj->extra_flags, ITEM_BURIED ) )
		continue;
		if ( obj->pIndexData->vnum == 11 )
			continue;
		if ( obj->cont_data != NULL 
		  && can_loot( ch, obj ) 
		  && ( !IS_SET( obj->cont_data->flags, CONT_CLOSED ) 
		  || ( !is_locked( obj->cont_data->lock ) || has_key( ch, obj->cont_data->lock ) ) ) )
		for ( obj2 = obj->cont_data->contents; obj2 != NULL ; obj2 = obj2_next ) 
		{
		    obj2_next = obj2->next_content;
 		    if ( CAN_WEAR( obj2, ITEM_TAKE )
		        && obj2->mpoints > max
		        && can_see_obj( ch, obj2 ) )
		    {
			found       = TRUE;
		        obj_best    = obj2;
		        max         = obj2->mpoints;
		    }
		}

		if ( CAN_WEAR( obj, ITEM_TAKE )
		    && obj->mpoints > max
		    && can_see_obj( ch, obj ) )
		{
		    found       = FALSE;
		    obj_best    = obj;
		    max         = obj->mpoints;
		}
	    }

	    if ( obj_best != NULL )
	    {
		if ( !found )
		{
  		    obj_from_room( obj_best );
		    obj_to_char( obj_best, ch );
		    act( "$n gets $p.", ch, obj_best, NULL, TO_ROOM );
		}
		else
		{
		    if ( is_locked( obj_best->in_obj->cont_data->lock ) )
			do_unlock( ch, obj_best->in_obj->name );
		    if ( IS_SET( obj_best->in_obj->cont_data->flags, CONT_CLOSED ) )
			do_open( ch, obj_best->in_obj->name );
		    if ( IS_SET( obj_best->in_obj->cont_data->flags, CONT_CLOSED ) )
			continue;
		    act( "$n gets $p from $P.", ch, obj_best, obj_best->in_obj, TO_ROOM );
		    obj = obj_best->in_obj;
  		    obj_from_obj( obj_best );
		    obj_to_char( obj_best, ch );
		}
	    }
	    wear_best( ch );
	}

        /* Mobs use curative items if they get low */
	found = FALSE;
        if ( ch->hit < ( ch->max_hit / 2 ) && IS_SET( ch->pIndexData->form, FORM_SENTIENT )
	     &&  ( ch->pIndexData != NULL && ch->pIndexData->pShop == NULL )
             &&  ( obj = get_magic_item( ch , TAR_CHAR_CURATIVE ) ) != NULL )
        {
	    if ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL )
	    {
		if ( obj->wear_data->magic_data->type == MAGIC_STAFF )
		{
		   found = TRUE;
                   do_brandish( ch, obj->name );
		}
		else if ( obj->wear_data->magic_data->type == MAGIC_WAND )
		{
		   found = TRUE;
		    sprintf( buf,"%s %s",obj->name,ch->name );
                    do_zap( ch, buf );
		}
		else if ( obj->wear_data->magic_data->type == MAGIC_SCROLL )
		{
		   found = TRUE;
		   sprintf( buf, "%s %s", obj->name, ch->name );
                   do_recite( ch, obj->name );      
		}
	    }
	    else if ( obj->edible_data != NULL )
	    {
		if ( !IS_SET( obj->edible_data->flags, EDIBLE_FOOD ) )
		{
		   found = TRUE;
                   do_drink( ch, obj->name );      
		}
		else
		{
		   found = TRUE;
                   do_eat( ch, obj->name );      
		}
	    }
        }

	if ( found ) 
	    continue;

       /* mobs will now use defensive items if needed */
	found = FALSE;
	if ( IS_SET( ch->form, FORM_SENTIENT )
	     &&  ( ch->pIndexData != NULL && ch->pIndexData->pShop == NULL )
             &&  ( obj = get_magic_item( ch , TAR_CHAR_DEFENSIVE ) ) != NULL )
        {
	    if ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL )
	    {
		if ( obj->wear_data->magic_data->type == MAGIC_STAFF )
		{
		   found = TRUE;
                   do_brandish( ch, obj->name );
		}
		else if ( obj->wear_data->magic_data->type == MAGIC_WAND )
		{
		   found = TRUE;
		    sprintf( buf,"%s %s",obj->name,ch->name );
                    do_zap( ch, buf );
		}
		else if ( obj->wear_data->magic_data->type == MAGIC_SCROLL )
		{
		   found = TRUE;
		   sprintf( buf, "%s %s", obj->name, ch->name );
                   do_recite( ch, obj->name );      
		}
	    }
	    else if ( obj->edible_data != NULL )
	    {
		if ( !IS_SET( obj->edible_data->flags, EDIBLE_FOOD ) )
		{
		   found = TRUE;
                   do_drink( ch, obj->name );      
		}
		else
		{
		   found = TRUE;
                   do_eat( ch, obj->name );      
		}
	    }
        }

	if ( found ) 
	    continue;

       /* If fighting mobs will now use offensive or damaging items */
	if ( ch->fighting != NULL && IS_SET( ch->form, FORM_SENTIENT )
	     &&  ( ch->pIndexData != NULL && ch->pIndexData->pShop == NULL )
             && (  ( obj = get_magic_item( ch , TAR_CHAR_OFFENSIVE ) ) != NULL 
               ||  ( obj = get_magic_item( ch , TAR_CHAR_DAMAGING ) ) != NULL ) )
        {
	    if ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL )
	    {
		if ( obj->wear_data->magic_data->type == MAGIC_STAFF )
		{
		   found = TRUE;
                   do_brandish( ch, obj->name );
		}
		else if ( obj->wear_data->magic_data->type == MAGIC_WAND )
		{
		   found = TRUE;
		    sprintf( buf,"%s %s",obj->name,ch->fighting->name );
                    do_zap( ch, buf );
		}
		else if ( obj->wear_data->magic_data->type == MAGIC_SCROLL )
		{
		   found = TRUE;
		   sprintf( buf, "%s %s", obj->name, ch->fighting->name );
                   do_recite( ch, obj->name );      
		}
	    }
        }

	if ( found ) 
	    continue;

/*good*/
	/* That's all for sleeping / busy monster, and empty zones */
	if ( ch->position != POS_STANDING )
	    continue;

        if ( ch->task[0] != '\0' && strlen(ch->task) > 0 ) /* Got a task? don't wander */
	    continue;

	if (ch->walkto != 0 )
	{
	   if ( (pRoom = get_room_index( ch->walkto )) == NULL )
	   {
		sprintf(buf,"Bad walkto room: %s Mob: %ld  Room %ld.\n\r",ch->short_descr,ch->pIndexData->vnum, ch->walkto);
		bug(buf,ch->pIndexData->vnum);
		REMOVE_BIT(ch->act2, ACT2_WALKTO);
		REMOVE_BIT(ch->act2, ACT2_STEERING);
		ch->walkto = 0;
		ch->walkfrom = 0;
		continue;
	   }

	   if(IS_SET(ch->act2, ACT2_STEERING ) )
	   {
		if ( (pMob = get_mob_index( ch->walkfrom )) == NULL )
		{
		    sprintf(buf,"Bad walkfrom room: %s Mob: %ld  Room %ld.\n\r",ch->short_descr,ch->pIndexData->vnum, ch->walkfrom);
		    bug(buf,ch->pIndexData->vnum);
		    REMOVE_BIT(ch->act2, ACT2_WALKTO);
		    REMOVE_BIT(ch->act2, ACT2_STEERING);
		    ch->walkto = 0;
		    ch->walkfrom = 0;
		    continue;
		}

	   	location = NULL;
	     	for ( victim = char_list; victim != NULL ; victim = victim->next )
	        {
		    if ( is_name( pMob->player_name, victim->name ) )
		        break;
	        }
		
		if ( victim )
		    location = victim->in_room;
		
		if ( location == NULL )
		{
		    sprintf(buf,"Bad walkto room: %s Mob: %ld  Room %ld.\n\r",ch->short_descr,ch->pIndexData->vnum, ch->walkfrom);
		    bug(buf,ch->pIndexData->vnum);
		    REMOVE_BIT(ch->act2, ACT2_WALKTO);
		    REMOVE_BIT(ch->act2, ACT2_STEERING);
		    ch->walkto = 0;
		    ch->walkfrom = 0;
		    continue;
		}
	   }
	   else
	       location = ch->in_room;
	     
	   door = find_first_step( location, pRoom);
	   if( door == -2 ) 
	   {
		sprintf(buf, "arrive %ld",  ch->walkto  );
		send_to_char_trig( buf, ch);
		REMOVE_BIT(ch->act2, ACT2_WALKTO);
		REMOVE_BIT(ch->act2, ACT2_STEERING);
		ch->walkto = 0;
		ch->walkfrom = 0;
	   }
	   else if( door < 0 ) 
	   {
		REMOVE_BIT(ch->act2, ACT2_WALKTO);
		REMOVE_BIT(ch->act2, ACT2_STEERING);
		ch->walkto = 0;
		ch->walkfrom = 0;
	   }
	   else 
	   {
		if (IS_SET(ch->act2, ACT2_WALKTO) )
		    move_char( ch, door, TRUE );
		if(IS_SET(ch->act2, ACT2_STEERING ) )
		    do_steer(ch, dir_name[door] );
		if( ch->in_room == pRoom )
		{
		    REMOVE_BIT(ch->act2, ACT2_WALKTO);
		    REMOVE_BIT(ch->act2, ACT2_STEERING);
		}
	     continue;
	   }
	    
	}   /* End of walkto */
	pexit = NULL;

	/* Wander */
	
        door = number_range( 0, 5 );
	pexit = ch->in_room->exit[door];    
	if ( check_wander(ch, pexit, door) )
	{
	    if ( !IS_SET( ch->pIndexData->form, FORM_SENTIENT ) || ( IS_AFFECTED( ch, AFF_DIMENSION_WALK ) && !IS_SET( pexit->exit_info, EX_PASSPROOF ) ) )
	    {
	        move_char( ch, door, FALSE );
	    }
	    else
	    {
		to_room=pexit->u1.to_room;
		in_room=ch->in_room;
		sprintf( buf, "%s %s",pexit->keyword, dir_name[ door ] );
		if ( is_locked_door( ch->in_room, door ) )
		{
		    do_unlock( ch, buf );
		    if ( is_locked_door( ch->in_room, door ) )
		        continue;
		}
		if ( is_door(pexit ))
	        {
	    	    if (IS_SET( pexit->exit_info, EX_CLOSED ))
	            do_open( ch, buf );
	 	}
	        move_char( ch, door, FALSE );
		/* ch->in_room cannot be NULL 
		 * The should be looked at sometime
		 * for now doing a quick check for a NULL in_room 
		 * Galadrail
		 */
		if ( !ch->in_room )
		    continue; 
		if ( ch->in_room != in_room && ch->in_room->exit[ rev_dir[ door ] ] != NULL 
		   && ch->in_room->exit[ rev_dir[ door ] ]->u1.to_room == in_room )
		{
		    sprintf( buf, "%s %s",ch->in_room->exit[ rev_dir[ door ] ]->keyword, dir_name[ rev_dir[ door ] ] );
	    	    if (IS_SET( pexit->exit_info, EX_CLOSED ))
		    do_close( ch, buf );
		    do_lock( ch, buf );
		}
	    }
	}
	/* Wander into portals */
	else if ( !IS_SET(ch->act, ACT_SENTINEL) 
	&& ( ch->leader == NULL || ch->leader == ch )
	&& number_range(0,50) == 0 ) /* Chance of a Wandering mob Wandering */
	{
	    GATE_DATA * gate;
	    to_room = NULL;

	    for ( obj = ch->in_room->contents ; obj != NULL 
		&& !( ( gate = obj->gate_data ) != NULL
		&& ( to_room = gate_leads_to( gate ) ) != NULL
		&&   IS_SET( ch->act2, ACT2_ENTER_PORTAL )
		&& check_wander(ch, pexit, door) ) ; obj = obj->next_content ); 

	    if ( to_room != NULL && obj != NULL )
	    {
		act( "$n enters $p.", ch, obj, NULL, TO_ROOM );
		char_from_room( ch );
		char_to_room( ch, to_room );
		act( "$n steps into the room.", ch, NULL, NULL, TO_ROOM );
	    }
	}

    }

/*    return; */
}

char * const weather_change [ MAX_SKY ][ 2 ] =
{
    { "Bug - bad sky type\n\r",
      "Bug - bad sky type\n\r"},
    { "Bug - bad sky type\n\r",
      "Bug - bad sky type\n\r"},
    { "A light rain starts to fall from the sky.\n\r",
      "The rain stops.\n\r" },
    { "The sky is lit up by a flash of lightning.\n\r",
      "The lightning has appeared to stop.\n\r" },
    { "A small snowflake lands on your nose.\n\r",
      "The snow fall slowly thins and stops.\n\r" },
    { "The snow fall has turned into a blizzard.\n\r",
      "The blizzard has passed.\n\r" },
    { "Rain starts to fall from the sky.\n\r",
      "The rain stops.\n\r" },
    { "Strong winds start to blow against you, a huricane is starting.\n\r",
      "The huricane is starting to pass.\n\r" },
    { "Very strong winds start to blow about you.\n\r",
      "The wind starts to slow down.\n\r" },
    { "A large tornado appears in the sky.\n\r",
      "The tornado slowly disapates.\n\r" },
    { "Very strong winds start to blow about you.\n\r",
      "The wind starts to slow down.\n\r" },
    { "Sand hits your face as you enter a sandstorm.\n\r",
      "The sandstorm has passed you by.\n\r" },
};

/*
 * Update the weather.
 */
void weather_update( void )
{
    char buf[MAX_STRING_LENGTH];
    AREA_DATA *pArea;
    DESCRIPTOR_DATA *d;
    DESCRIPTOR_DATA *d_next;
    int sky, world;

    for ( world = 0; world < MAX_WORLD ; world++ )
    {

        if ( ++time_info[ world ].hour == time_table[ world ].sunrise - 1 )
 	    weather_info[ world ].sunlight = SUN_LIGHT;
        else if ( time_info[ world ].hour == time_table[ world ].sunrise )
	    weather_info[ world ].sunlight = SUN_RISE;
        else if ( time_info[ world ].hour == time_table[ world ].sunset )
	    weather_info[ world ].sunlight = SUN_SET;
        else if ( time_info[ world ].hour == time_table[ world ].sunset + 1 )
	    weather_info[ world ].sunlight = SUN_DARK;

        if ( time_info[ world ].hour == time_table[ world ].hours_day )
        {
	    time_info[ world ].hour = 0;
	    time_info[ world ].day++;
        }

        if ( time_info[ world ].day   >= time_table[ world ].days_month )
        {
	    time_info[ world ].day = 0;
	    time_info[ world ].month++;
        }

        if ( time_info[ world ].month >= time_table[ world ].months_year )
        {
	    CHAR_DATA *victim;
	    OBJ_INDEX_DATA * pobj;
	    OBJ_DATA *obj;

	    time_info[ world ].month = 0;
	    time_info[ world ].year++;
	    send_to_world( world, "The town crier shouts 'Happy New Year!'\n\r" );

	    if ( ( pobj = get_obj_index( VNUM_NEW_YEAR ) ) != NULL )
            for ( d = descriptor_list ; d != NULL ; d = d_next )
            {
		d_next = d->next;
                 if ( d->connected > CON_PLAYING 
		   || ( victim = d->character ) == NULL
		   || d->character->in_room->area->world != world )
		    continue;

		obj = create_object( pobj );
		obj_to_char( obj, victim );
		act( "The town crier gives you $p", victim, obj, NULL, TO_CHAR );
	    }
        }
    }

    for ( d = descriptor_list ; d != NULL ; d = d->next )
    {
	CHAR_DATA * victim;
	ROOM_INDEX_DATA * pRoom;

	if ( d->connected > CON_PLAYING
	    || ( victim = d->character ) == NULL )
	    continue;

	world = race_table[ victim->race ][ victim->sex ].world;

	if ( time_info[ world ].hour == time_table[ world ].sunrise 
	  && time_info[ world ].day == victim->birth_day
	  && time_info[ world ].month == victim->birth_month
	  && ( pRoom = get_room_index( race_table[ victim->race ][ victim->sex ].begin ) ) != NULL )
	{
	    sprintf( buf, "The town crier shouts 'Happy Birthday %s!'\n\r", capitalize( victim->name ) );
	    send_to_area( pRoom->area, buf );
	}

  	if ( !IS_OUTSIDE(d->character)
	    || IS_UNDERWATER( d->character )
	    || IS_SET(d->character->plr, PLR_WEATHER_OFF)
	    || !IS_AWAKE(d->character) )
	    continue;

	     if ( get_hour( d->character->in_room->area ) == time_table[ d->character->in_room->area->world ].sunrise - 1 )
	        send_to_char( "The day is about to begin.\n\r", d->character ); 
	else if ( get_hour( d->character->in_room->area ) == time_table[ d->character->in_room->area->world ].sunrise )
	{
	        sprintf( buf, "The sun starts to rise in the %s.\n\r", time_table[ d->character->in_room->area->world ].sunrise_dir );
	        send_to_char( buf, d->character );
	}
	else if ( get_hour( d->character->in_room->area ) == time_table[ d->character->in_room->area->world ].sunset )
	{
	        sprintf( buf, "The sun starts to set in the %s.\n\r", time_table[ d->character->in_room->area->world ].sunset_dir );
	        send_to_char( buf, d->character );
	}
	else if ( get_hour( d->character->in_room->area ) == time_table[ d->character->in_room->area->world ].sunset + 1 )
	{
		send_to_char( "The night has begun.\n\r", d->character );
	}

    }

    /*
     * Weather change.
     */
    for ( pArea = area_first ; pArea != NULL ; pArea = pArea->next )
    {
	pArea->weather->change += number_range( -1 * pArea->weather->speed, pArea->weather->speed );
	pArea->weather->change = URANGE( -1 * pArea->weather->speed , pArea->weather->change, pArea->weather->speed );
	pArea->weather->mmhg   += pArea->weather->change;
	pArea->weather->mmhg   = URANGE( 0, pArea->weather->mmhg, 100 );

	pArea->weather->season = get_season( pArea );
	if ( pArea->weather->sky == ( sky = get_sky_type( pArea ) ) )
	    continue;

    	if ( pArea->weather->sky > 2 && pArea->weather->sky % 2 == sky % 2 
	   && sky > 2 )
	    continue;

	if ( sky == SKY_CLOUDLESS )
	{
	    if ( pArea->weather->sky == SKY_CLOUDY )
	    {
		if ( number_bits(2) == 0 )
		{
		    send_weather( pArea, "The clouds disappear.\n\r" );
		    pArea->weather->sky = SKY_CLOUDLESS;
		}
 	    }
	    else if ( pArea->weather->sky % 2 == 0 )
	    {
		send_weather( pArea, weather_change[ pArea->weather->sky ][ 1 ] );
	 	pArea->weather->sky = SKY_CLOUDY;
	    }
	    else
	    {
		send_weather( pArea, weather_change[ pArea->weather->sky ][ 1 ] );
		pArea->weather->sky--;
	    }
	}
	else if ( sky == SKY_CLOUDY )
	{
	    if ( pArea->weather->sky == SKY_CLOUDLESS )
	    {
		if ( number_bits(2) == 0 )
		{
		    send_weather( pArea, "Large clouds start to form in the sky.\n\r" );
		    pArea->weather->sky = SKY_CLOUDY;
		}
	    }
	    else if ( pArea->weather->sky % 2 == 0 )
	    {
		if ( number_bits(2) == 0 )
		{
		    send_weather( pArea, weather_change[ pArea->weather->sky ][ 1 ] );
	 	    pArea->weather->sky = SKY_CLOUDY;
	    	}
	    }
	    else
	    {
		send_weather( pArea, weather_change[ pArea->weather->sky ][ 1 ] );
		pArea->weather->sky--;
	    }
	}
	else if ( sky % 2 == 0 )
	{
	    if ( pArea->weather->sky == SKY_CLOUDLESS )
	    {
		send_weather( pArea, "Large clouds start to form in the sky.\n\r" );
		pArea->weather->sky = SKY_CLOUDY;
	    }
	    else if ( pArea->weather->sky == SKY_CLOUDY )
	    {
		if ( number_bits(2) == 0 )
		{
		    send_weather( pArea, weather_change[ sky ][ 0 ] );
	 	    pArea->weather->sky = sky;
		}	    }
	    else
	    {
		if ( number_bits(2) == 0 )
		{
		    send_weather( pArea, weather_change[ pArea->weather->sky ][ 1 ] );
		    pArea->weather->sky--;
		}
	    }
	}
	else
	{
	    if ( pArea->weather->sky == SKY_CLOUDLESS )
	    {
		send_weather( pArea, "Large clouds start to form in the sky.\n\r" );
		pArea->weather->sky = SKY_CLOUDY;
	    }
	    else if ( pArea->weather->sky == SKY_CLOUDY )
	    {
		send_weather( pArea, weather_change[ sky - 1 ][ 0 ] );
	 	pArea->weather->sky = sky - 1;
	    }
	    else 
	    {
		if ( number_bits(2) == 0 )
		{
		    pArea->weather->sky++;
 		    send_weather( pArea, weather_change[ pArea->weather->sky ][ 0 ] );
		}
	    }
	}
    }
    return;
}

void char_forge_update()
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    char buf[MAX_STRING_LENGTH];

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

        if (IS_NPC(ch) )
	    continue;

	if ( ch->in_room == NULL )
	    continue;
	if (IS_SET(ch->forge_flags, IS_FORGING ) )
	{
	    if ( ch->forge_count > 0 )
	    {
		switch (ch->forge_count )
		{
		    case 5:
		    send_to_char("You put your metals in the oven until they are `Rred hot`w.\n\r",ch);
		    break;
		    case 4:
		    send_to_char("You remove the metals from the oven and begin pounding and shaping.\n\r",ch);
		    break;
		    case 3:
		    send_to_char("While the metal is still hot you customize them with special tools.\n\r",ch);
		    break;
		    case 2:
		    send_to_char("You place the results in cold water, steam billowing upwards.\n\r",ch);
		    break;
		    case 1:
		    send_to_char("You polish the item to perfection.\n\r",ch);
		    SET_BIT(ch->forge_flags, FORGE_COMPLETE);
		    REMOVE_BIT(ch->forge_flags, IS_FORGING );
		    sprintf(buf, "%s",ch->forge_item );
		    if ( ch->forge_from == 1 )
		    	do_forge_weapon(ch, buf);
		    if ( ch->forge_from == 2 )
			do_forge_armor(ch, buf);
		    if ( ch->forge_from == 3 )
			do_forge_golem( ch, buf );
		    break;
		}
		ch->forge_count--;
	    }
	    else 
	    {
		REMOVE_BIT(ch->forge_flags, IS_FORGING );
		continue;
	    } 
	}
    }
}

void char_dream_update()
{
CHAR_DATA *ch;
CHAR_DATA *ch_next;
AFFECT_DATA af;
int sn;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

        if (IS_NPC(ch) )
	    continue;

	if ( ch->in_room == NULL )
	    continue;

	if (check_for_dream(ch) )
	{
	    ch->dream_count++;
	    if (IS_SET(ch->dream_flags, DREAM_IS_NIGHTMARE ) )
	    {
		switch(ch->dream_count)
		{
		    case 1:
		    send_to_char("You begin to dream.\n\r",ch);
		    break;
		    case 2:
		    send_to_char("Strange creatures, evil and vile, enter your thoughts...\n\r",ch);
		    break;
		    case 3:
		    send_to_char("You feel the presence of someone watching you through your dream.\n\r",ch);
		    break;
		    case 4:
		    send_to_char("Waves of heat radiate to you from one paticular evil being.\n\r",ch);
		    break;
		    case 5:
		    send_to_char("It reaches for you, pulling at your soul.\n\r",ch);
		    break;
		    case 6:
		    send_to_char("As you struggle for your soul it lunges at you.\n\r",ch);
		    break;
		    case 7:
		    send_to_char("You feel part of your soul enter it.\n\r",ch);
		    break;
		    case 8:
		    send_to_char("You break its hold just before it reaches you.\n\r",ch);
		    create_nightmare_mob(ch,0);
		    ch->dream_flags = 0;
		    ch->dream_count = 0;
		    do_wake(ch, "");
		    break;
		}
	    } 
	    if (IS_SET(ch->dream_flags, DREAM_IS_NORMAL ) )
	    {
		switch(ch->dream_count)
		{
		    	case 1:
		   	send_to_char("You begin to dream.\n\r",ch);
		   	break;
		    	case 2:
		    	send_to_char("Memories of present and past fade in and out.\n\r",ch);
		    	break;
		    	case 3:
		    	send_to_char("Memories you had thought you had forgotten come forth in your dream.\n\r",ch);
		    	break;
		    	case 4:
		    	send_to_char("Some of happiness, embarrasment, hatred and fear.\n\r",ch);
		    	break;
		    	case 5:
		    	send_to_char("As soon as they are remembered they fade and are forgotten once again.\n\r",ch);
		    	ch->dream_flags = 0;
		    	ch->dream_count = 0;
		    	break;
		}
	    } 
	    if (IS_SET(ch->dream_flags, DREAM_IS_GOOD_2 ) )
	    {
		switch(ch->dream_count)
		{
		    	case 1:
			send_to_char("You begin to dream.\n\r",ch);
			break;
			case 2:
			send_to_char("The sound of drums intrudes upon your rest.\n\r",ch);
			break;
			case 3:
			send_to_char("Slowly, the sound grows louder and you begin to hear other things, as well:\n\rwar cries, swords crashing shields, the shrieking of blood-crazed horses...\n\r",ch);
			break;
			case 4:
			send_to_char("All at once, you realize that the battle is below you. You seem to float high in the air.\n\rThe combatants appear as black specks, so close together that they nearly block\n\rthe ground from your view.\n\r",ch);
			break;
			case 5:
			send_to_char("Without moving, you feel your perception change.  You're closer now.\n\rThe blood-smells and battle noises nearly overwhelm you.\n\r",ch);
			break;
			case 6:
			send_to_char("Again your perceptions shift.  The noises and odors are lodged somewhere in t he back of your mind.\n\rNow you see clearly everyone in the battle.\n\rThe rage-twisted faces of demons and the crushed skulls of children parade in equal time across your vision.\n\r",ch);
			break;
			case 7:
			send_to_char("You move higher again.  Now you hear only the sounds of the dead.\n\rStrange shapes, barely visible, fly up from the bodies strewn across the dirt.\n\rMany of them touch you as they pass.  Some seem familiar, even kind.\n\rOthers freeze your blood and cause your skin to crawl.\n\r",ch);
			break;
			case 8:
			send_to_char("You don't know how long you look down at the earth through those shapes.\n\rBut finally the last one passes and as it does you hear a whisper...\n\r",ch);
			break;
			case 9:
			send_to_char("Be ready.\n\r",ch);
 			ch->dream_flags = 0;
		   	ch->dream_count = 0;
			sn = skill_lookup( "courage" );
		    	af.type      = sn;
    			af.level     = ch->level;
    			af.duration  = 6 + ( ch->level / 3 );
    			af.location  = APPLY_HITROLL;
    			af.modifier  = ch->level / 6;
    			af.bitvector = 0;
    			affect_to_char( ch, &af );
    			af.location  = APPLY_SAVING_SPELL;
    			af.modifier  = 0 - ch->level / 6;
    			affect_to_char( ch, &af );
    			send_to_char( "You feel courageous.\n\r", ch );
			break;
		}
	    }
	    if (IS_SET(ch->dream_flags, DREAM_IS_GOOD ) )
	    {
		switch(ch->dream_count)
		{
		    	case 1:
		    	send_to_char("You begin to dream.\n\r",ch);
		    	break;
		    	case 2:
		    	send_to_char("You see yourself standing within a field of golden wheat which rythmicly bends with the song of the wind.\n\r",ch);
		    	break;
		    	case 3:
		    	send_to_char("You turn south as sounds of a birds song draws your attention.\n\r",ch);
		    	break;
		    	case 4:
		    	send_to_char("As the bird closes the distance between you the sun reflects from its golden wings, momentarily blinding your view.\n\r",ch);
		    	break;
		    	case 5:
		    	send_to_char("You stand among a lush garden of brilliantly colored exotic flowers.\n\r",ch);
		    	break;
		    	case 6:
		    	send_to_char("A flash of light appears then fades, and in its place you see an angel with long feathered wings.\n\r",ch);
		    	break;
		    	case 7:
		    	send_to_char("The angel floats quietly before you and lightly taps you on your shoulder.\n\r",ch);
		    	break;
		    	case 8:
		    	send_to_char("As the comfort of the darkness surrounds you once again and the angel fades from your vision and memory, you feel peacefull and content with life.\n\r",ch);
		    	ch->dream_flags = 0;
		    	ch->dream_count = 0;
    			ch->hit = UMIN( ch->hit + ( roll_dice(3, 8) + ch->level / 8 ), ch->max_hit );
			send_to_char( "You feel a boost of health.\n\r", ch );
		    	break;	
		}
	    } 
	}
    }
}

/*
 * Update all chars, including mobs.
*/
void char_update( void )
{   
    AFFECT_DATA af;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;
    OBJ_DATA * obj;
    OBJ_DATA * obj_next;
    CHAR_DATA *rch;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *ch_quit;
    char buf[MAX_INPUT_LENGTH];
    int value;
    /*int sn;*/

    ch_quit	= NULL;

    /* update save counter */
    save_number++;

    if (save_number > 29)
	save_number = 0;

    for ( ch = char_list; ch != NULL; ch = ch_next )
    {
	ch_next = ch->next;

	if ( ch->in_room == NULL )
	    continue;

	if ( ch->fighting == NULL && IS_SET( ch->plr, PLR_FLEE ) )
	    REMOVE_BIT( ch->plr, PLR_FLEE );

        if ( ch->timer > 30 && !IS_IMMORTAL(ch))
            ch_quit = ch;

	/* Lossing link rather then quiting for immortals bugs me.
	 * If there not one of the few ( us ) then they can be disconnected like the rest.
	 * However, will give them a bit more time.
	 * Galadrail.
	 */
        if ( ch->timer > 500 && !IS_OPTIVISION(ch))
            ch_quit = ch;

	if ( !IS_NPC( ch ) && !IS_IMMORTAL( ch ) )
	    weather_affect( ch );

	if ( IS_UNDERWATER( ch )
	  || ( ch->in_room->sector_type == SECT_WATER_SWIM && !IS_AFFECTED( ch, AFF_FLYING|AFF_LEVITATE ) ) )
	{
	    for ( obj = ch->carrying ; obj != NULL ; obj = obj_next )
	    {
	        obj_next = obj->next_content;
	        check_rust( obj, 3 );	
	    }
	}

	for ( obj = ch->carrying ; obj != NULL ; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->condition <= 0 )
	    {
	    	sprintf( buf, "%s falls apart.\n\r", obj->short_descr );
	    	send_to_char ( buf, ch );
		extract_obj( obj );
	    }
	}

	if (ch->timer < 12 ) /*Not in void */

	if ( ch->pcdata != NULL && ch->pcdata->debt != NULL )
	    send_thug( ch );

	if ( ch->timer < 12 && IS_AFFECTED( ch, AFF_COLD ) && !IS_IMMORTAL( ch ) )
	{
	    if ( ch->position == POS_SLEEPING )
		set_position( ch, POS_STANDING );
	    send_to_char( "Aachooo!\n\r", ch );
	    act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );

	    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	    {
	        if ( rch == ch || IS_AFFECTED( rch, AFF_COLD ) || IS_IMMORTAL( rch ) )
		    continue;

	    	if ( IS_SET( rch->res_flags, RES_WATER ) )
	    	    value = get_curr_stat( rch, STAT_CON ) * 4;
	    	else if ( IS_SET( rch->vuln_flags, VULN_WATER ) )
	    	    value = get_curr_stat( rch, STAT_CON ) * 2;
	    	else
	    	    value = get_curr_stat( rch, STAT_CON ) * 3;

		if ( number_range( 0, 100 ) == 0 
		  && !IS_SET( rch->imm_flags, IMM_WATER )
		  && number_percent() / 2 > value )
		{
	    	af.type = 0;
	    	af.level = number_range( 10, 30 );
	    	af.duration = number_range( 10, 25 );
	    	af.location = APPLY_HITROLL;
	    	af.modifier = -3;
	    	affect_to_char( ch, &af );	
	    	af.location = APPLY_DAMROLL;
	    	af.modifier = -3;
	    	affect_to_char( ch, &af );	
	    	af.bitvector = AFF_COLD;
	    	af.bit_type = BIT_AFFECT;
	    	af.location = APPLY_AFFECT;
	    	affect_to_char( ch, &af );	
	    	send_to_char( "Aachoo! you just caught a cold.\n\r", ch );
	    	act( "$n sneezes.", ch, NULL, NULL, TO_ROOM );
		}
	    }
	}

	if (ch->pcdata != NULL && ch->pcdata->home_timer < 100 )
	    ch->pcdata->home_timer++;

	if (ch->gos_timer > 0 )
	    ch->gos_timer--;

	if (ch->position != POS_FIGHTING && ch->level > 5 && !IS_IMMORTAL( ch ) )
	{
	  ch->happy_hour++; /* Used for No food or Water while socializing */
	  if (ch->happy_hour == 10 )
	  {
		send_to_char("`YHappy Hour!`w\n\rIt seems as if you have been just socializing.  Happy Hour in affect.\n\rFood and drink are on the house!\n\r",ch);
	  }
	if (ch->happy_hour > 500 )
	   ch->happy_hour = 11;
	}
	else
	ch->happy_hour = 0;

	if (ch->outfit_timer > 0 )
	    ch->outfit_timer--;

	if (LOGON_ON)
	if ( !IS_NPC(ch) )
	{
	    if (ch->logon_data != NULL)
	    {
		if ( str_cmp(ch->logon_data->exit, "Lost-link") )
		    ch->logon_data->time_end = current_time;

		if ( str_cmp(ch->logon_data->exit,"Lost-link"))
		    if ( str_cmp(ch->logon_data->exit,"Logged-IN"))
		    	strncpy( ch->logon_data->exit, "Logged-IN", 30 );
	    }
	}
	if (ch->timer < 12 ) /*Not in void */
	    check_regen( ch );

	if (ch->timer < 12 ) /*Not in void */
	if ( ch->position == POS_STUNNED || ch->position == POS_INCAP)
	    update_pos( ch );

	if ( !IS_NPC(ch) )
	ch->timer++;

	if ( !IS_NPC(ch) && !IS_IMMORTAL(ch) )
	{


	    if ( ch->timer >= 12 )
	    {
		if ( ch->was_in_room == NULL && ch->in_room != NULL )
		{
		    ch->was_in_room = ch->in_room;
		    if ( ch->fighting != NULL )
			stop_fighting( ch, TRUE );
		    act( "$n disappears into the void.",
			ch, NULL, NULL, TO_ROOM );
		    send_to_char( "You disappear into the void.\n\r", ch );
		    if (ch->level > 1)
		        save_char_obj( ch );
		    char_from_room( ch );
		    char_to_room( ch, get_room_index( ROOM_VNUM_LIMBO ) );
		}
	    }

	    if ( (ch->timer < 12 && ch->happy_hour < 6) 
	    || (race_table[ch->race][ch->sex].world == 0 ) ) /* Not in void */
	    	gain_condition( ch, COND_THIRST, -1  );
	    if (ch->timer < 12 && ch->happy_hour < 6)
		gain_condition( ch, COND_FULL, -1 );

	}


    /* I moved this out so immortals will get sober.. */
   if ( ch->timer < 12 && ch->happy_hour < 6) /* Not in void */
	   gain_condition( ch, COND_DRUNK, -1 );

   if ( ch->timer < 12 ) /* Not in void */
   {
	for ( paf = ch->affected; paf != NULL; paf = paf_next )
	{
	    paf_next	= paf->next;
	    if ( paf->duration > 0 )
	    {
		paf->duration--;
		if (number_range(0,4) == 0 && paf->level > 0)
		  paf->level--;  /* spell strength fades with time */
		if ( paf->bit_type == BIT_AFFECT && paf->bitvector == AFF_HOLD_BREATH
		  && IS_UNDERWATER( ch ) && !IS_AFFECTED( ch, AFF_BREATHE_UNDERWATER ) )
		{
		    if ( !IS_NPC( ch ) && ch->desc == NULL )
			paf->duration ++;
		    switch(paf->duration)
		    {
			case 0:
			    send_to_char("You are drowning!.\n\r",ch);
			    act("$n's face is bright blue.\n\r", ch, NULL, NULL, TO_ROOM);
			    break;
			case 1:
			    send_to_char("You are about to drown.\n\r",ch);
			    act("$n's face starts turning blue.\n\r", ch, NULL, NULL, TO_ROOM);
			    break;
			case 2:
			    send_to_char("You start to feel sleepy due to lack of air.\n\r",ch);
			    break;
		    }
		}
            }
	    else if ( paf->duration < 0 )
		;
	    else
	    {
		if ( paf_next == NULL
		||   paf_next->type != paf->type
		||   paf_next->duration > 0 )
		{
		    if ( paf->type > 0 && skill_table[paf->type].msg_off )
		    {
			send_to_char( skill_table[paf->type].msg_off, ch );
			send_to_char( "\n\r", ch );
		    }
		}
		affect_remove( ch, paf );

		if ( paf->bit_type == BIT_AFFECT && paf->bitvector == AFF_CONTROL_MIND )
		    stop_follower( ch);	  
		if ( paf->bit_type == BIT_AFFECT && paf->bitvector == AFF_BREATHE_UNDERWATER && !IS_IMMORTAL(ch)
		&& IS_UNDERWATER( ch ) )
		{
		     send_to_char("You plug your nose and try to hold your breath.\n\r",ch);
		     do_hold_breath( ch, "" );
		}
	/* Quin 'Remove next lines to stop DROWNINGS' */
		if ( paf->bit_type == BIT_AFFECT && paf->bitvector == AFF_HOLD_BREATH && IS_UNDERWATER( ch ) && !IS_IMMORTAL(ch) )
		{
		    act("$n has drowned.\n\r",ch ,NULL, NULL, TO_ROOM);
		    send_to_char("As you try to gasp for air, you lungs fill up with water, and you drown.\n\r", ch );
  		    if ( !IS_NPC(ch) )
		    {
	    		sprintf( buf, "%s drowned at %ld",
			    ch->name,ch->in_room->vnum );
			info_channel( ch, buf, INFOACT_DEATH);
		        log_string( buf );
		     }

		    raw_kill(ch, FALSE, FALSE);

		     if ( IS_NPC(ch) )
    		     {
			extract_char( ch, TRUE );
		    }
		    else
		    {
		        extract_char( ch, FALSE );
			for ( paf = ch->affected; paf != NULL; paf = paf_next)
			{
			    paf_next = paf->next;
			    affect_remove( ch, paf );
			}
		        ch->affected_by	= ch->pcdata->perm_aff;
		        ch->armor = 100;
			set_position( ch, POS_RESTING );
		        REMOVE_BIT(ch->plr, PLR_THIEF);
    			REMOVE_BIT(ch->plr, PLR_BOUGHT_PET);
		    } 

		}

	    }
	}

	/*
	 * Careful with the damages here,
	 *   MUST NOT refer to ch after damage taken,
	 *   as it may be lethal damage (on NPC).
	 */

        if (is_affected(ch, gsn_plague) && ch != NULL)
        {
            AFFECT_DATA plague;
            CHAR_DATA *vch;
            int save, dam;

	    if (ch->in_room == NULL)
		return;
            
	    act("$n writhes in agony as plague sores erupt from $s skin.",
		ch,NULL,NULL,TO_ROOM);
	    send_to_char("You writhe in agony from the plague.\n\r",ch);
            for ( paf = ch->affected; paf != NULL; paf = paf_next )
            {
		paf_next = paf->next;
            	if (paf->type == gsn_plague)
                    break;
            }
        
            if (paf == NULL)
            {
            	REMOVE_BIT(ch->affected_by,AFF_PLAGUE);
            	return;
            }
        
            if (paf->level == 1)
            	return;
        
            plague.type 		= gsn_plague;
            plague.level 		= paf->level - 1; 
            plague.duration 	= number_range(1,2 * plague.level);
            plague.location		= APPLY_STR;
            plague.modifier 	= -5;
 	    plague.bit_type	= BIT_AFFECT;
            plague.bitvector 	= AFF_PLAGUE;
        
            for ( vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
            {
            	switch(check_immune(vch,DAM_DISEASE))
            	{
            	    case(IS_NORMAL) 	: save = paf->level - 4;	break;
            	    case(IS_IMMUNE) 	: save = 0;		break;
            	    case(IS_RESISTANT) 	: save = paf->level - 8;	break;
            	    case(IS_VULNERABLE)	: save = paf->level; 	break;
            	    default			: save = paf->level - 4;	break;
            	}
            
                if (save != 0 && !saves_spell(save,vch) && !IS_IMMORTAL(vch)
            	&&  !IS_AFFECTED(vch,AFF_PLAGUE) && number_bits(4) == 0)
            	{
            	    send_to_char("You feel hot and feverish.\n\r",vch);
            	    act("$n shivers and looks very ill.",vch,NULL,NULL,TO_ROOM);
            	    affect_join(vch,&plague);
            	}
            }

	    dam = UMIN(ch->level,5);
	    ch->mana[ 0 ] -= dam;
	    ch->mana[ 1 ] -= dam;
	    ch->mana[ 2 ] -= dam;
	    ch->mana[ 3 ] -= dam;
	    ch->mana[ 4 ] -= dam;
	    ch->move -= dam;
	    damage( ch, ch, dam, gsn_plague,DAM_DISEASE);
        }
	else if ( IS_AFFECTED(ch, AFF_POISON) && ch != NULL)
	{
	    act( "$n shivers and suffers.", ch, NULL, NULL, TO_ROOM );
	    send_to_char( "You shiver and suffer.\n\r", ch );
	    damage( ch, ch, 2 + number_range(1, 26 - get_curr_stat(ch, STAT_CON) ), gsn_poison, DAM_POISON );
	}
/*
	else if ( ch->position == POS_INCAP && number_range(0,4) == 0)
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE );
	}
	else if ( ch->position == POS_MORTAL )
	{
	    damage( ch, ch, 1, TYPE_UNDEFINED, DAM_NONE);
	}
*/
    }
   } /* not in void end */
    /*
     * Autosave and autoquit.
     * Check that these chars still exist.
     */
   for ( ch = char_list; ch != NULL; ch = ch_next )
   {
       ch_next = ch->next;
 
    if ( ch )
    {
	if (ch->desc != NULL && ch->desc->descriptor % 30 == save_number)
	    save_char_obj(ch);

        if ( ch == ch_quit )
             auto_rent (ch);
    }
 
  }
 
    return;
}


void obj_update_trigger( void )
{   
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next;

	if (HAS_SCRIPT_OBJ(obj) )
	{
		script_update_obj(obj , TRIG_EACH_PULSE );
	}
    }
}

void trigger_update( void )
{   
    TRIGGER_DATA *trigger;
    for ( trigger = trigger_list; trigger != NULL; trigger = trigger->next_in_list )
    {
    if ( trigger->timer > 0 )
    trigger->timer--;  
    }
}

/*
 * Update all objs.
 * This function is performance sensitive.
 */
void obj_update( void )
{   
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    ROOM_INDEX_DATA *to_room;

    for ( obj = object_list; obj != NULL; obj = obj_next )
    {
	CHAR_DATA *rch;
	char *message;

	obj_next = obj->next;
	if (HAS_SCRIPT_OBJ(obj) )
	    script_update_obj(obj , TRIG_TICK_PULSE );

	if ( obj->spec_fun != 0 )
	    (*obj->spec_fun) ( obj );

	if ( obj->light_data != NULL && obj->light_data->fuel != 0 
	  && obj->light_data->fuel != -1 && obj->light_data->max_fuel != -1
	  && IS_SET( obj->light_data->flags, LIGHT_LIT|LIGHT_BURNING|LIGHT_MAGIC_LIT ) )
 	{
	    obj->light_data->fuel--;
  	    if ( obj->light_data->fuel == 0 )
	    {
		REMOVE_BIT( obj->light_data->flags, LIGHT_LIT|LIGHT_BURNING|LIGHT_MAGIC_LIT );
		if ( obj->carried_by != NULL )
		     act( "$p burns out.", obj->carried_by, obj, obj, TO_CHAR );
		else if ( obj->in_room != NULL )
		{
		    sprintf( buf, "%s burns out.\n\r", obj->short_descr );
		    send_to_room( obj->in_room->vnum, buf );
		}		   
	    }
	}
	/*
	if ( obj->wear_data != NULL )
            for ( paf = obj->wear_data->affected; paf != NULL; paf = paf_next )
            {
            	paf_next    = paf->next;
            	if ( paf->duration > 0 )
            	{
                    paf->duration--;
                    if (number_range(0,4) == 0 && paf->level > 0)
                  	paf->level--;
            	}
            	else if ( paf->duration < 0 )
                    ;
            	else
            	{
                    if ( paf_next == NULL
                    ||   paf_next->type != paf->type
                    ||   paf_next->duration > 0 )
                    {
                    	if ( paf->type > 0 && skill_table[paf->type].msg_off )
                    	{
                            act_new( skill_table[paf->type].msg_off, obj->carried_by, obj,
				 NULL, POS_SLEEPING,TO_CHAR );
                    	}
                    }

                    affect_remove_obj( obj, paf );
            	}
            }
	*/
	if ( obj->donate_timer != 0 )
	{
	    if ( --obj->donate_timer == 1 )
	    {
		extract_obj( obj );
		return;
	    }
	}

	if ( obj->timer <= 0 || --obj->timer > 0 )
	    continue;

	if ( obj->cont_data != NULL && obj->cont_data->inside != NULL )
	    hatch( obj );

	if ( obj->edible_data != NULL )
	{
	    if ( IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) )
		message = "$p becomes overgrown with mold and decomposes.";
	    else if ( obj->edible_data->max_amount == -1 )
		message = "$p dries up.";
	    else
		message = "$p has evaporated from disuse.";
	}
	else
	    message = "$p crumbles into dust.";

	if ( obj->gate_data != NULL &&  obj->gate_data->ave_timer > 0 )
	{
	CHAR_DATA * ch=NULL;
	OBJ_DATA * obj_to=NULL;
	long vnum;
	bool found;

	    if ( !IS_SET( obj->gate_data->flags, GATE_STAY_ROOM ) )
	    {
	    	for( to_room = get_room_index( number_range(0,MAX_VNUM) ) ; to_room == NULL 
		|| ( to_room->area != obj->in_room->area && IS_SET( obj->gate_data->flags , GATE_STAY_AREA ) )
	        || ( to_room->area->world != obj->in_room->area->world && IS_SET( obj->gate_data->flags , GATE_STAY_WORLD ) );
		to_room = get_room_index ( number_range( 0, MAX_VNUM ) ) );

	    	for ( rch = obj->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	      	    act("$p slowly fades away.", rch, obj, NULL, TO_CHAR);

	    	for ( rch = to_room->people ; rch != NULL ; rch = rch->next_in_room )
	      	    act("$p opens before you.", rch, obj, NULL, TO_CHAR);

	    	obj_from_room( obj );
	    	obj_to_room( obj, to_room );
	    }

	    if ( !IS_SET( obj->gate_data->flags, GATE_TO_STAY_ROOM ) )
 	    {
 	    ROOM_INDEX_DATA * in_room = gate_leads_to( obj->gate_data );
	    	if (!in_room)
	    	{
	   	    extract_obj(obj);
		    continue;
	    	}
		
	    	found = FALSE;
	    	for( vnum = number_range(0,MAX_VNUM) ; !found ; vnum = number_range(0,MAX_VNUM ) )
	    	{
		    if ( IS_SET( obj->gate_data->flags, GATE_TO_MOB ) )
		    {
		    	for ( ch = char_list ; ch != NULL && ( !IS_NPC( ch ) || ch->pIndexData->vnum != vnum ) ; ch = ch->next );
		    
		    	if ( ch == NULL )
			    continue;

		    	to_room = ch->in_room;
		    	if ( ( to_room->area != in_room->area  
			&& IS_SET( obj->gate_data->flags , GATE_TO_STAY_AREA ) )
		      	|| ( to_room->area->world != in_room->area->world
			&& IS_SET( obj->gate_data->flags , GATE_TO_STAY_WORLD ) ) )
			    continue;

		    	found = TRUE;
		    	obj->gate_data->vnum = vnum;
		    	obj->gate_data->leads_to = ( void * ) ch;
		    }
		    else if ( IS_SET( obj->gate_data->flags, GATE_TO_OBJ ) )
		    {
		    	for ( obj_to = object_list ; obj_to != NULL && obj->pIndexData->vnum != vnum ; obj_to = obj_to->next );

		    	if ( obj_to == NULL )
			    continue;

		    	to_room = get_in_room_obj( obj_to );
		    	if ( ( to_room->area != in_room->area  
			&& IS_SET( obj->gate_data->flags , GATE_TO_STAY_AREA ) )
		      	|| ( to_room->area->world != in_room->area->world
			&& IS_SET( obj->gate_data->flags , GATE_TO_STAY_WORLD ) ) )
			    continue;

		    	found = TRUE;
		    	obj->gate_data->vnum = vnum;
		    	obj->gate_data->leads_to = ( void * ) ch;
		    }
		    else
		    {
		    	if ( ( to_room = get_room_index( vnum ) ) == NULL )
			    continue;

		    	if ( ( to_room->area != in_room->area  
			&& IS_SET( obj->gate_data->flags , GATE_TO_STAY_AREA ) )
		      	|| ( to_room->area->world != in_room->area->world
			&& IS_SET( obj->gate_data->flags , GATE_TO_STAY_WORLD ) ) )
			    continue;

		    	found = TRUE;
		    	obj->gate_data->vnum = vnum;
		    	obj->gate_data->leads_to = ( void * ) ch;
		    }
	     	}
	    }

	    obj->timer = UMAX( 1, ( obj->gate_data->ave_timer * number_range( 50, 150 ) ) / 100 );
	    continue;
	}

	if ( obj->wear_data != NULL && obj->carried_by != NULL )
	{
	    if (IS_NPC(obj->carried_by) 
	    &&  obj->carried_by->pIndexData->pShop != NULL)
		obj->carried_by->gold[ race_table[ obj->carried_by->race ][ obj->carried_by->sex ].world ] += obj->wear_data->cost/5;
	    else
	    	act( message, obj->carried_by, obj, NULL, TO_CHAR );
	}
	else if ( obj->in_room != NULL
	&& ( rch = obj->in_room->people ) != NULL )
	{
	    if ( ! ( obj->in_obj && !CAN_WEAR(obj->in_obj,ITEM_TAKE) ) )
	    {
	    	act( message, rch, obj, NULL, TO_ROOM );
	    	act( message, rch, obj, NULL, TO_CHAR );
	    }
	}

        if (obj->cont_data != NULL && obj->cont_data->contents)
	{   /* save the contents */
     	    OBJ_DATA *t_obj, *next_obj;

	    for (t_obj = obj->cont_data->contents; t_obj != NULL; t_obj = next_obj)
	    {
		next_obj = t_obj->next_content;
		obj_from_obj(t_obj);

		if (obj->in_obj) /* in another object */
		    obj_to_obj(t_obj,obj->in_obj);

		if (obj->carried_by)  /* carried */
		    obj_to_char(t_obj,obj->carried_by);

		if (obj->in_room == NULL)  /* destroy it */
		    extract_obj(t_obj);

		else /* to a room */
		    obj_to_room(t_obj,obj->in_room);
	    }
	}

	extract_obj( obj );
    }

    return;
}



/*
 * Aggress.
 *
 * for each mortal PC
 *     for each mob in room
 *         aggress on some random PC
 *
 * This function takes 25% to 35% of ALL Merc cpu time.
 * Unfortunately, checking on each PC move is too tricky,
 *   because we don't the mob to just attack the first PC
 *   who leads the party into the room.
 *
 * -- Furey
 */
void aggr_update( void )
{
    CHAR_DATA *wch;
    CHAR_DATA *cch;
    CHAR_DATA *cch_next;
    CHAR_DATA *wch_next;
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *victim;
    int chance;
    for ( wch = char_list; wch != NULL; wch = wch_next )
    {
 	wch_next = wch->next;

	if ( IS_NPC(wch)
	||  IS_IMMORTAL(wch) 
	||   wch->in_room == NULL )
	    continue;

	for ( ch = wch->in_room->people; ch != NULL; ch = ch_next )
	{
	    int count;

	    ch_next	= ch->next_in_room;

	    if ( !IS_NPC(ch)
	    ||   (  !IS_SET(ch->act, ACT_ATTACK_WORLD)
	          &&  !IS_SET(ch->act, ACT_AGGRESSIVE ) 
		  && !IS_SET( ch->act2, ACT2_ATTACK_GUILD )
		  && !IS_SET( ch->act2, ACT2_SMART_AGGRESSIVE )
		  && ch->dislikes[0] == '\0' 
		  && ch->hunting[0] == '\0' )
	    ||   IS_SET(ch->in_room->room_flags,ROOM_SAFE)
	    ||   IS_AFFECTED(ch,AFF_CALM_SPIRIT)
	    ||   ch->fighting != NULL
	    ||   !IS_AWAKE(ch)
	    ||   ( IS_SET(ch->act, ACT_WIMPY) && IS_AWAKE(wch)
		  && ch->hit <= ch->max_hit * 2 / 3 )
	    ||   !can_see( ch, wch ) 
	    ||   number_bits(1) == 0)
		continue;

	    if ( ch->dislikes[0] != '\0' )
	    if ( is_name( ch->dislikes, wch->name ) )
	    {
		act("$n remembers you and attacks!", ch, NULL, wch, TO_VICT );
		if ( get_skill( ch, gsn_shadowstrike ) )
		{
		    do_shadowstrike( ch, wch->name );
	            multi_hit( ch, wch, TYPE_UNDEFINED );
		}
		else
		{
		    ch->fighting = wch;
	            multi_hit( ch, wch, TYPE_UNDEFINED );
		}
		continue;
	    }

	    /*
	     * Ok we have a 'wch' player character and a 'ch' npc aggressor.
	     * Now make the aggressor fight a RANDOM pc victim in the room,
	     *   giving each 'vch' an equal chance of selection.
	     */
	    count	= 0;
	    victim	= NULL;
	    for ( vch = wch->in_room->people; vch != NULL; vch = vch_next )
	    {
		vch_next = vch->next_in_room;
	 
		if ( !IS_NPC(vch)
		&&  !IS_IMMORTAL(vch) 
		&&  can_see( ch, vch )
		&&  ( !IS_SET(ch->act, ACT_WIMPY) 
		    || !IS_AWAKE(vch) )
		&&  ( (IS_SET(ch->act, ACT_AGGRESSIVE) 
		    || IS_SET(ch->act2, ACT2_SMART_AGGRESSIVE) )
		||  ( IS_SET( ch->act2, ACT2_ATTACK_GUILD ) 
		    && !IS_SET( ch->guilds, vch->guilds ) )
		||  ( !IS_SET( vch->guilds, MAJHERE ) 
		    && IS_SET( ch->act, ACT_ATTACK_WORLD )
		    && race_table[ ch->race ][ ch->sex ].world != race_table[ vch->race ][ vch->sex ].world ) ) )
		{
		  if ( IS_SET(ch->act2, ACT2_SMART_AGGRESSIVE ) )
		  {
			chance = 60;
			chance=chance + ((ch->level - vch->level)*20);
			if ( !IS_AWAKE(vch) )
			    chance+=10;

			if ( get_eq_char(vch, WEAR_WIELD ) == NULL )
			    chance+=10;

			chance+=vch->max_hit/vch->hit;
	    		for ( cch = wch->in_room->people; cch != NULL; cch = cch_next )
	    		{		
			    cch_next = cch->next_in_room;
			    if ( cch == ch )
				continue;
			    if ( IS_NPC(cch ) && can_see(cch, vch ))
			    if ( IS_SET( cch->off_flags, ASSIST_ALL ) 
            		     || ( ch->race == cch->race && IS_SET( cch->off_flags, ASSIST_RACE ) )
            		     || ( ch->pIndexData->vnum == cch->pIndexData->vnum && IS_SET( cch->off_flags, ASSIST_VNUM ) ))
			     { 
			     	chance+=80;
			     	chance=chance + ((cch->level - vch->level)*10);
			     }

			}
		        if ( chance >= 100 )
			victim=vch;
		  }
		  else
		  {
		    if ( number_range( 0, count ) <= 1 )
       		      if ( roll_dice(1,30) > get_curr_stat( vch, STAT_CHR) )
			victim = vch;
		    count++;
		  }
		}
	    }

	    if ( victim == NULL )
		continue;

	    if ( victim->mount != NULL && victim->mount->in_room == ch->in_room && roll_dice(1,5) < 4 )
		victim = victim->mount;

	    if ( get_skill( ch, gsn_shadowstrike ) )
	    {
	        do_shadowstrike( ch, victim->name );
	        multi_hit( ch, wch, TYPE_UNDEFINED );
	    }
	    else
	    {
	        ch->fighting = victim;
	        multi_hit( ch, wch, TYPE_UNDEFINED );	
	    }
	}
    }

    return;
}



/*
 * Handle all kinds of updates.
 * Called once per pulse from game loop.
 * Random times to defeat tick-timing clients and players.
 */

void update_handler( void )
{
    char buf[MAX_INPUT_LENGTH];
    static  int     pulse_area;
    static  int     pulse_mobile;
    static  int     pulse_violence;
    static  int     pulse_point;
    static  int     pulse_race;
    static  int     pulse_save_area;
    static  int     pulse_save_logon;
    static  int     pulse_save_random;
    static  int     pulse_save_quests;
    static  int     pulse_save_wizlist;
    static  int     pulse_reboot;
    static  int     pulse_room_trigger;
    static  int     pulse_char_dream_update;
    static  int     pulse_char_forge_update;
    static  int     pulse_obj_trigger;
    static  int     pulse_trigger;

    if ( --pulse_area     <= 0 )
    {
	pulse_area	= number_range( PULSE_AREA / 2, 3 * PULSE_AREA / 2 );
	area_update	( );
    }

    if ( --pulse_mobile   <= 0 )
    {
	pulse_mobile	= PULSE_MOBILE;
	mobile_update	( );
    }

    if ( --pulse_trigger   <= 0 )
    {
	pulse_trigger	= PULSE_TRIGGER;
	trigger_update	( );
    }

    if ( --pulse_violence <= 0 )
    {
	pulse_violence	= PULSE_VIOLENCE;
	violence_update	( );
    }
    if ( --pulse_char_dream_update <= 0 )
    {
	pulse_char_dream_update	= PULSE_CHAR_DREAM_UPDATE;
	char_dream_update( );
    }
    if ( --pulse_char_forge_update <= 0 )
    {
	pulse_char_forge_update	= PULSE_CHAR_FORGE_UPDATE;
	char_forge_update( );
    }

    if ( --pulse_room_trigger <= 0 )
    {
	pulse_room_trigger	= PULSE_ROOMS;
	room_update_trigger	( );
    }
    if ( --pulse_obj_trigger <= 0 )
    {
	pulse_obj_trigger	= PULSE_OBJ_TRIGGER;
	obj_update_trigger	( );
    }

    if ( --pulse_race <= 0 )
    {
	pulse_race	= PULSE_RACE;
	race_update	( );
    }

    if ( !reboot_pause && reboot_timer > 0 && --pulse_reboot <= 0 )
    {
	DESCRIPTOR_DATA *d;

	reboot_timer -= 5;
	pulse_reboot  = PULSE_REBOOT;

	buf[ 0 ] = '\0';

	if ( reboot_timer <= 0 )
	    reboot();
	else if ( reboot_timer % 60 == 0 && reboot_timer > 60 )
	    sprintf( buf, "Reboot in %d minutes.\n\r", reboot_timer / 60 );
	else if ( reboot_timer == 60 )
	    strcpy( buf, "Reboot in 1 minute.\n\r" );
	else if ( reboot_timer == 45 )
	    strcpy( buf, "Reboot in 45 seconds.\n\r" );
	else if ( reboot_timer == 30 )
	    strcpy( buf, "Reboot in 30 seconds.\n\r" );
	else if ( reboot_timer <= 20 )
	    sprintf( buf, "Reboot in %d seconds!\n\r", reboot_timer );

	if ( buf[ 0 ] != '\0' )
	    for( d = descriptor_list ; d != NULL ; d= d->next )
		if ( d->connected <= CON_PLAYING )
		    send_to_char( buf, d->character );
    }

    if ( --pulse_save_area <= 0 )
    {
	pulse_save_area	= PULSE_SAVE_AREA;
	strcpy(buf,"changed");
	do_asave(NULL,buf);
	do_count(NULL,"auto"); /* This is here for lack of a better place G */
    }

    if ( --pulse_save_logon <= 0 )
    {
	pulse_save_logon	= PULSE_SAVE_LOGON;
	if (IS_SET(autosave_system, AUTOSAVE_LOGONS) && IS_SET(changed_system, CHANGED_LOGONS) )
	save_logon();
    }
    if ( --pulse_save_random <= 0 )
    {
	pulse_save_random	= PULSE_SAVE_RANDOM;
	if (IS_SET(autosave_system, AUTOSAVE_RANDOM) && IS_SET(changed_system, CHANGED_RANDOM) )
	save_random();
    }
    if ( --pulse_save_quests <= 0 )
    {
	pulse_save_quests	= PULSE_SAVE_QUESTS;
	if (IS_SET(autosave_system, AUTOSAVE_QUESTS) && IS_SET(changed_system, CHANGED_QUESTS) )
	save_quests();
    }

    if ( --pulse_save_wizlist <= 0 )
    {
	pulse_save_wizlist	= PULSE_SAVE_WIZLIST;
	save_wizlist();
	save_finger();
	if (IS_SET(autosave_system, AUTOSAVE_HELPS) && IS_SET(changed_system, CHANGED_HELPS) )
	save_help(NULL, NULL);
	if (IS_SET(autosave_system, AUTOSAVE_SOCIALS) && IS_SET(changed_system, CHANGED_SOCIALS) )
	save_social(NULL, NULL);
	if (IS_SET(autosave_system, AUTOSAVE_TRIGGERS) && IS_SET(changed_system, CHANGED_TRIGGERS) )
	save_triggers(NULL, NULL);
	if (IS_SET(autosave_system, AUTOSAVE_CLANS) && IS_SET(changed_system, CHANGED_CLANS) )
	save_clans	( );
	if (IS_SET(autosave_system, AUTOSAVE_RACES) && IS_SET(changed_system, CHANGED_RACES) )
	do_saveraces(NULL, NULL);
	if (IS_SET(autosave_system, AUTOSAVE_GUILDS) && IS_SET(changed_system, CHANGED_GUILDS) )
	do_saveguilds(NULL, NULL);
    }

    if ( --pulse_point    <= 0 )
    {
	CHAR_DATA *mob;
	CHAR_DATA *mob_next;

	pulse_point     = PULSE_TICK;

	for ( mob = char_list; mob != NULL; mob = mob_next )
	{	
	    mob_next = mob->next;
	    script_update(mob, TRIG_TICK_PULSE );
	}

	room_update	( );
	weather_update	( );
	char_update	( );
	obj_update	( );
/*	web_who_update ( );   not until httpd is open, reverie */
    }

    aggr_update( );
    tail_chain( );
    return;
}

void do_task( CHAR_DATA *ch )
{
    char arg;
    char * task;
    char buf[MAX_STRING_LENGTH];
    task = ch->task;
    arg=*task;
    task++;
    switch( arg )
    {
	default:
	    sprintf(buf,"Bad char Task: %s Mob: %ld.\n\r",ch->task,ch->pIndexData->vnum);
	    bug(buf,ch->pIndexData->vnum);
	    strncpy( ch->task, task, 80 );
	    return;
	case '\0':
	    ch->task[0] = '\0';
	    return;
	case ' ':
	    break;	
	case 'N':
	case 'n':
	    if ( ch->position == POS_STANDING )
    	        move_char( ch, DIR_NORTH, TRUE);
	    break;
	case 'S':
	case 's':
	    if ( ch->position == POS_STANDING )
    	        move_char( ch, DIR_SOUTH, TRUE);
	    break;
	case 'E':
	case 'e':
	    if ( ch->position == POS_STANDING )
    	        move_char( ch, DIR_EAST, TRUE);
	    break;
	case 'W':
	case 'w':
	    if ( ch->position == POS_STANDING )
    	        move_char( ch, DIR_WEST, TRUE);
	    break;
	case 'U':
	case 'u':
	    if ( ch->position == POS_STANDING )
    	        move_char( ch, DIR_UP, TRUE);
	    break;
	case 'D':
	case 'd':
	    if ( ch->position == POS_STANDING )
    	        move_char( ch, DIR_DOWN, TRUE);
	    break;
	case '1':
	    ch->wait += PULSE_MOBILE;
	    break;
	case '2':
	    ch->wait += PULSE_MOBILE*2;
	    break;
	case '3':
	    ch->wait += PULSE_MOBILE*3;
	    break;
	case '4':
	    ch->wait += PULSE_MOBILE*4;
	    break;
	case '5':
	    ch->wait += PULSE_MOBILE*5;
	    break;
	case '6':
	    ch->wait += PULSE_MOBILE*6;
	    break;
	case '7':
	    ch->wait += PULSE_MOBILE*7;
	    break;
	case '8':
	    ch->wait += PULSE_MOBILE*8;
	    break;
	case '9':
	    ch->wait += PULSE_MOBILE*9;
	    break;

    }
	
    sprintf( buf,"%s",task);
    strncpy( ch->task, buf, 80 );
    return;
}
