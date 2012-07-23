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

/***************************************************************************
 *  The MOBprograms have been contributed by N'Atas-ha.  Any support for   *
 *  these routines should not be expected from Merc Industries.  However,  *
 *  under no circumstances should the blame for bugs, etc be placed on     *
 *  Merc Industries.  They are not guaranteed to work on all systems due   *
 *  to their frequent use of strxxx functions.  They are also not the most *
 *  efficient way to perform their tasks, but hopefully should be in the   *
 *  easiest possible way to install and begin using. Documentation for     *
 *  such installation can be found in INSTALL.  Enjoy........    N'Atas-Ha *
 ***************************************************************************/

#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"

/*
 * Local functions.
 */

char *			mprog_type_to_name	args( ( int type ) );

/* prints the argument to all the rooms aroud the mobile */

void do_mpasound( CHAR_DATA *ch, char *argument )
{

  ROOM_INDEX_DATA *was_in_room;
  int              door;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        bug( "Mpasound - No argument: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    was_in_room = ch->in_room;
    for ( door = 0; door <= 5; door++ )
    {
      EXIT_DATA       *pexit;
      
      if ( ( pexit = was_in_room->exit[door] ) != NULL
	  &&   pexit->u1.to_room != NULL
	  &&   pexit->u1.to_room != was_in_room )
      {
	ch->in_room = pexit->u1.to_room;
	act( argument, ch, NULL, NULL, TO_ROOM );
      }
    }

  ch->in_room = was_in_room;
  return;

}

/* Created by Quin :) */
void next_mtrack(char *arg, int spaces , TRACK_TYPE *tracks ) 
{
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *in_room;
    CHAR_DATA *victim;
    TRACK_TYPE *tracking;
    TRACK_TYPE *back_track;
    int direction;

    tracks->next=NULL;
    in_room = tracks->room;

    if ( spaces < 1 )
    {

    for( victim = in_room->people;
        ( victim !=  NULL ) 
        ; victim = victim->next_in_room)
    if ( is_name( arg, victim->name ) && !IS_IMMORTAL( victim ) )
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
	   && ( to_room->area == in_room->area )
	   && !IS_SET( to_room->room_flags, ROOM_NO_MOB ) )
        {
      tracking->dir = 6;
      tracks->dir = direction;
      tracking->room = to_room;
            for( back_track=tracks->last ; ( ( back_track != NULL ) && back_track->room != to_room );
                back_track=back_track->last ) ;  
	    if ( back_track == NULL || back_track->room!=to_room )
                next_mtrack( arg,spaces - UMAX( 1, track_loss[ in_room->sector_type ] ) , tracking );
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
bool do_mphunt( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *vch;
    ROOM_INDEX_DATA *in_room=ch->in_room;
    EXIT_DATA *pexit;
    TRACK_DATA * tracks;
    long time=0;
    int count;
    int door;
    int current = -1;
    bool fPass;

    if (ch == NULL )
	return FALSE;

    if ( ch->position != POS_STANDING || ch->fighting != NULL )
	return FALSE;

    if ( ch->master != NULL )
	for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room )
	    if ( ch->master == vch )
		return FALSE;

    if ( get_char_room( ch, argument ) != NULL )     
	return FALSE;

    for ( door = 0 ; door < 6 ; door++ )
    {
	if ( ( pexit = in_room->exit[ door ] ) == NULL )
	    continue;

	count = 0;

	for ( tracks = pexit->tracks ; tracks != NULL ; tracks = tracks->next )
	{
	    count++;

	    for ( vch = char_list ; vch != NULL && vch != tracks->ch ; vch = vch->next );

	    fPass = get_skill( ch, gsn_hunt ) - tracks->track_penalty - ( 5 * count ) < number_percent();
	    fPass = TRUE;

	    if ( vch == NULL
	      || ( !is_name( argument, vch->name ) && fPass )
	      || ( is_name( argument, vch->name ) && !fPass ) )
		continue;

	    fPass = get_skill( ch, gsn_hunt ) - tracks->track_penalty - ( 5 * count ) > number_percent();
	    fPass = TRUE;

	    if ( time < tracks->ctime && fPass )
	    {
		time = tracks->ctime;
		current = door;
	    }
	}
    }

    if ( current == -1 )
	return FALSE;

    if ( IS_SET( ch->act, ACT_WIMPY ) && ch->hit < ch->max_hit * 2 / 3 )
    {
	if ( ch->in_room->exit[ rev_dir[ current ] ] != NULL )
	    current = rev_dir[ current ];
	else
	{
	    for ( door = 0 ; door < 6 ; door++ )
	    {
	    	if ( door == current )
		    continue;
		if ( ch->in_room->exit[ door ] != NULL )
		{
		    current = door;
		    break;
		}
	    }
	    if ( door == 6 )
	        return FALSE;
	}
    }

    if ( ( pexit = ch->in_room->exit[ current ] ) == NULL )
	return FALSE;

	if ( !IS_SET( ch->form, FORM_SENTIENT ) || ( IS_AFFECTED( ch, AFF_DIMENSION_WALK ) && !IS_SET( pexit->exit_info, EX_PASSPROOF ) ) 
	    || strlen( pexit->keyword ) == 0 )
  	    move_char( ch, current, TRUE );
	else
	{
	    sprintf(buf,"%s %s",pexit->keyword,dir_name[ current ] );
	    if ( is_locked_door( ch->in_room, current ) )
	    {
		do_unlock( ch, buf );
		if( is_locked_door( ch->in_room, current ) )
		    return TRUE;
	    }
	    if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
	    	do_open( ch, buf );
	    move_char( ch, current, TRUE );
	    if ( ch->in_room != in_room && ( pexit = ch->in_room->exit[ rev_dir[ current ] ] ) != NULL
	      && pexit->u1.to_room == in_room )
	    {
	        sprintf(buf,"%s %s",pexit->keyword,dir_name[ rev_dir[ current ] ] );
		do_close( ch, buf );
		do_lock( ch, buf );
	    }
	}

	return TRUE;
}	


/* lets the mobile kill any player or mobile without murder*/

void do_mpkill( CHAR_DATA *ch, char *argument )
{
    char      arg[ MAX_INPUT_LENGTH ];
    CHAR_DATA *victim;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	bug( "MpKill - no argument: vnum %d.",
		ch->pIndexData->vnum );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	bug( "MpKill - Victim not in room: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    if ( victim == ch )
    {
	bug( "MpKill - Bad victim to attack: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CONTROL_MIND ) && ch->master == victim )
    {
	bug( "MpKill - Charmed mob attacking master: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {	
	bug( "MpKill - Already fighting: vnum %d",
	    ch->pIndexData->vnum );
	return;
    }

    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}


/* lets the mobile destroy an object in its inventory
   it can also destroy a worn object and it can destroy 
   items using all.xxxxx or just plain all of them */

void do_mpjunk( CHAR_DATA *ch, char *argument )
{
    char      arg[ MAX_INPUT_LENGTH ];
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( arg[0] == '\0')
    {
        bug( "Mpjunk - No argument: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( str_cmp( arg, "all" ) && str_prefix( "all.", arg ) )
    {
      if ( ( obj = get_obj_wear( ch, arg ) ) != NULL )
      {
	unequip_char( ch, obj );
	extract_obj( obj );
	return;
      }
      if ( ( obj = get_obj_carry( ch, arg ) ) == NULL )
	return; 
      extract_obj( obj );
    }
    else
      for ( obj = ch->carrying; obj != NULL; obj = obj_next )
      {
        obj_next = obj->next_content;
        if ( arg[3] == '\0' || is_name( &arg[4], obj->name ) )
        {
          if ( obj->wear_data != NULL && obj->wear_data->wear_loc != WEAR_NONE)
	    unequip_char( ch, obj );
          extract_obj( obj );
        } 
      }

    return;

}

/* prints the message to everyone in the room other than the mob and victim */

void do_mpechoaround( CHAR_DATA *ch, char *argument )
{
  char       arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *victim;

    if ( !IS_NPC( ch ) )
    {
       send_to_char( "Huh?\n\r", ch );
       return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
       bug( "Mpechoaround - No argument:  vnum %d.", ch->pIndexData->vnum );
       return;
    }

    if ( ( victim=get_char_room( ch, arg ) ) == NULL )
    {
        bug( "Mpechoaround - victim does not exist: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    act( argument, ch, NULL, victim, TO_NOTVICT );
    return;
}

void do_mpset_exit_flags( CHAR_DATA *ch, char *argument )
{
  char       arg1[ MAX_INPUT_LENGTH ];
  char       arg2[ MAX_INPUT_LENGTH ];
  AREA_DATA * pArea;
  ROOM_INDEX_DATA * pRoomIndex;
  int door;
  long x;

    /*if ( !IS_NPC( ch ) )
    {
       send_to_char( "Huh?\n\r", ch );
       return;
    }
    add later
    */
    pArea = ch->in_room->area;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2);

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
       bug( "Mpset_exit_flags - No argument",0);
       return;
    }
    if ( !str_cmp( arg1, "xyzzy" ) )
    {
	 for ( x = pArea->lvnum ; x < pArea->uvnum ; x++ )
	    {
		if ( ( pRoomIndex = get_room_index(x) ) != NULL )
		{
		    for ( door = 0; door < 6; door++ )
            		if ( pRoomIndex->exit[door] != NULL )
			{
            		SET_BIT(pRoomIndex->exit[door]->exit_info, get_exit_flag_number( arg2 ));
			}
		}
	   }
	return;
    }

    return;
}

void do_mpdig( CHAR_DATA *ch, char * argument )
{
    OBJ_DATA * obj;


        for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj->next_content )
        {

            if ( IS_SET( obj->extra_flags , ITEM_BURIED ) )
            {
                act( "$n digs something out of the earth.", ch, NULL, NULL, TO_ROOM );
                REMOVE_BIT(obj->extra_flags,ITEM_BURIED);
                return;
            }
        }
        act( "$n digs a hole in the earth.", ch, NULL, NULL, TO_ROOM );
        return;
}

/* prints the message to only the victim */

void do_mpechoat( CHAR_DATA *ch, char *argument )
{
  char       arg[ MAX_INPUT_LENGTH ];
  CHAR_DATA *victim;

    if ( !IS_NPC( ch ) )
    {
       send_to_char( "Huh?\n\r", ch );
       return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
       bug( "Mpechoat - No argument:  vnum %d.",
	   ch->pIndexData->vnum );
       return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
        bug( "Mpechoat - victim does not exist: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    act( argument, ch, NULL, victim, TO_VICT );
    return;
}

/* prints the message to the room at large */

void do_mpecho( CHAR_DATA *ch, char *argument )
{
    if ( !IS_NPC(ch) )
    {
        send_to_char( "Huh?\n\r", ch );
        return;
    }

    if ( argument[0] == '\0' )
    {
        bug( "Mpecho - called w/o argument: vnum %d.",
	    ch->pIndexData->vnum );
        return;
    }

    act( argument, ch, NULL, NULL, TO_ROOM );
    return;

}

/* lets the mobile load an item or mobile.  All items
are loaded into inventory.  you can specify a level with
the load object portion as well. */

void do_mpmload( CHAR_DATA *ch, char *argument )
{
    char            arg[ MAX_INPUT_LENGTH ];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA      *victim;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	bug( "Mpmload - Bad vnum as arg: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	bug( "Mpmload - Bad mob vnum: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    return;
}

void do_mpoload( CHAR_DATA *ch, char *argument )
{
    char arg1[ MAX_INPUT_LENGTH ];
    char arg2[ MAX_INPUT_LENGTH ];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA       *obj;
    int             level;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
 
    if ( arg1[0] == '\0' || !is_number( arg1 ) )
    {
        bug( "Mpoload - Bad syntax: vnum %d.",
	    ch->pIndexData->vnum );
        return;
    }
 
    if ( arg2[0] == '\0' )
    {
	level = ch->level;
    }
    else
    {
	/*
	 * New feature from Alander.
	 */
        if ( !is_number( arg2 ) )
        {
	    bug( "Mpoload - Bad syntax: vnum %d.", ch->pIndexData->vnum );
	    return;
        }
	level = atoi( arg2 );
	 if ( level < 0 || level >  MAX_LEVEL)
	{
	    bug( "Mpoload - Bad level: vnum %d.", ch->pIndexData->vnum );
	    return;
	}
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	bug( "Mpoload - Bad vnum arg: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    obj = create_object( pObjIndex );
    if ( CAN_WEAR(obj, ITEM_TAKE) )
    {
	obj_to_char( obj, ch );
    }
    else
    {
	obj_to_room( obj, ch->in_room );
    }

    return;
}

/* lets the mobile purge all objects and other npcs in the room,
   or purge a specified object or mob in the room.  It can purge
   itself, but this had best be the last command in the MOBprogram
   otherwise ugly stuff will happen */

void do_mppurge( CHAR_DATA *ch, char *argument )
{
    char       arg[ MAX_INPUT_LENGTH ];
    CHAR_DATA *victim;
    OBJ_DATA  *obj;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
        /* 'purge' */
        CHAR_DATA *vnext;
        OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	  	vnext = victim->next_in_room;
	  	if ( IS_NPC( victim ) && victim != ch )
	    		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	  	obj_next = obj->next_content;
	  	extract_obj( obj );
	}

	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) != NULL )
    {
    	if ( !IS_NPC( victim ) )
    	{
		bug( "Mppurge - Purging a PC: vnum %d.", ch->pIndexData->vnum );
		return;
    	}
	if (victim == ch )
	ch->extract = TRUE;
	else
    	extract_char( victim, TRUE );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) )
    {
	extract_obj( obj );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "No mob by that name\n\r", ch );
	return;
    }

    if ( !IS_NPC( victim ) )
    {
	bug( "Mppurge - Purging a PC: vnum %d.", ch->pIndexData->vnum );
	return;
    }

}
ROOM_INDEX_DATA  *  find_location ( CHAR_DATA *ch, char *argument);

/* lets the mobile goto any location it wishes that is not private */

void do_mpgoto( CHAR_DATA *ch, char *argument )
{
    char             arg[ MAX_INPUT_LENGTH ];
    ROOM_INDEX_DATA *location;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
	return;
    }

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	bug( "Mpgoto - No argument: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	bug( "Mpgoto - No such location: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( ch->fighting != NULL )
	stop_fighting( ch, TRUE );

    char_from_room( ch );
    char_to_room( ch, location );

    return;
}

/* lets the mobile do a command at another location. Very useful */

void do_mpat( CHAR_DATA *ch, char *argument )
{
    char             arg[ MAX_INPUT_LENGTH ];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    CHAR_DATA       *wch;

    if ( !IS_NPC( ch ) )
    {
        send_to_char( "Huh?\n\r", ch );
	return;
    }
 
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	bug( "Mpat - Bad argument: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	bug( "Mpat - No such location: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    original = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    break;
	}
    }

    return;
}
 
/* lets the mobile transfer people.  the all argument transfers
   everyone in the current room to the specified location */

void do_mptransfer( CHAR_DATA *ch, char *argument )
{
    char             arg1[ MAX_INPUT_LENGTH ];
    char             arg2[ MAX_INPUT_LENGTH ];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA       *victim;

    if ( !IS_NPC( ch ) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	bug( "Mptransfer - Bad syntax: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( !str_cmp( arg1, "all" ) )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected <= CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    bug( "Mptransfer - No such location: vnum %d.",
	        ch->pIndexData->vnum );
	    return;
	}

	if ( room_is_private( location ) )
	{
	    bug( "Mptransfer - Private room: vnum %d.",
		ch->pIndexData->vnum );
	    return;
	}
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	bug( "Mptransfer - No such person: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    if ( victim->in_room == NULL )
    {
	bug( "Mptransfer - Victim in Limbo: vnum %d.",
	    ch->pIndexData->vnum );
	return;
    }

    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );

    char_from_room( victim );
    char_to_room( victim, location );

    return;
}

/* lets the mobile force someone to do something.  must be mortal level
   and the all argument only affects those in the room with the mobile */

void do_mpforce( CHAR_DATA *ch, char *argument )
{
    char arg[ MAX_INPUT_LENGTH ];

    if ( !IS_NPC( ch ) )
    {
	send_to_char( "Huh?\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	bug( "Mpforce - Bad syntax: vnum %d.", ch->pIndexData->vnum );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	    if ( vch->in_room == ch->in_room
		&& !IS_NPC(vch)
		&& (!IS_IMMORTAL(vch) || IS_IMP(ch)) 
		&& (!IS_IMP(vch) || IS_OPTIVISION(ch) )
		&& can_see( ch, vch ) )
	    {
		interpret( vch, argument );
	    }
	}
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    bug( "Mpforce - No such victim: vnum %d.",
	  	ch->pIndexData->vnum );
	    return;
	}

	if ( victim == ch )
    	{
	    bug( "Mpforce - Forcing oneself: vnum %d.",
	    	ch->pIndexData->vnum );
	    return;
	}

	interpret( victim, argument );
    }

    return;
}
bool do_mphunt_old( CHAR_DATA *ch, char *argument )
{
    TRACK_TYPE *tracks;
    ROOM_INDEX_DATA *in_room;
    EXIT_DATA *pexit;
    char buf[MAX_INPUT_LENGTH];
    int spaces;

    return FALSE;
    if ( get_char_room( ch, argument ) != NULL )     
	return FALSE;

    tracks = track_type_alloc();
    tracks->dir = 6;
    for ( spaces = 0 ; spaces < ( 6 + ch->level ) && tracks->dir == 6 ; spaces ++ )
    {
        tracks->next = NULL;
        tracks->last = NULL;
        tracks->room = ch->in_room;
        tracks->dir = 6;
	next_mtrack( argument, spaces, tracks );
    }

    if ( tracks->dir == -1 )
    {
	free_track_type( tracks );
	return TRUE;
    }

    if ( tracks->dir == 6 )
    {
	free_track_type( tracks );
	return FALSE;
    }

    if ( IS_SET( ch->act, ACT_WIMPY ) && ch->hit < ch->max_hit * 2 / 3 )
    {
	if ( ( pexit = ch->in_room->exit[ rev_dir[ tracks->dir ] ] ) != NULL )
	    tracks->dir = rev_dir[ tracks->dir ];
	else
	{
	    for ( spaces = 0 ; spaces < 6 ; spaces++ )
	    {
	    	if ( spaces == tracks->dir )
		    continue;
		if ( ( pexit = ch->in_room->exit[ spaces ] ) != NULL )
		{
		    tracks->dir = spaces;	
		    break;
		}
	    }
	    if ( spaces == 6 )
	    {
		free_track_type( tracks );
	        return FALSE;
	    }
	}
    }

    if ( ( pexit = ch->in_room->exit[ tracks->dir ] ) == NULL )
    {
	free_track_type( tracks );
	return FALSE;
    }

    ch->wait++;

    if ( tracks->dir > -1 && tracks->dir < 6 )
    {
	if ( !IS_SET( ch->form, FORM_SENTIENT ) || ( IS_AFFECTED( ch, AFF_DIMENSION_WALK ) && !IS_SET( pexit->exit_info, EX_PASSPROOF ) ) 
	    || strlen( pexit->keyword ) == 0 )
  	    move_char( ch, tracks->dir, TRUE );
	else
	{
	    sprintf(buf,"%s %s",pexit->keyword,dir_name[ tracks->dir ] );
	    if ( is_locked_door( ch->in_room, tracks->dir ) )
	    {
		do_unlock( ch, buf );
		if( is_locked_door( ch->in_room, tracks->dir ) )
		{
		    free_track_type( tracks );
		    return TRUE;
		}
	    }
	    if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
	    	do_open( ch, buf );
	    in_room = ch->in_room;
	    move_char( ch, tracks->dir, TRUE );
	    if ( ch->in_room != in_room && ( pexit = ch->in_room->exit[ rev_dir[ tracks->dir ] ] ) != NULL
	      && pexit->u1.to_room == in_room )
	    {
	        sprintf(buf,"%s %s",pexit->keyword,dir_name[ rev_dir[ tracks->dir ] ] );
		do_close( ch, buf );
		do_lock( ch, buf );
	    }
	}

	free_track_type( tracks );
	return TRUE;
    }

    free_track_type( tracks );
    return FALSE;
}	
