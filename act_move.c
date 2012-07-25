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
#include <time.h>
#else
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "merc.h"

char *	const	dir_name	[]		=
{
    "north", "east", "south", "west", "up", "down"
};
char *	const	wall_name	[]	=
{
    "north wall", "east wall", "south wall", "west wall", "ceiling", "floor"
};

char *	const	dir_name_2	[]		=
{
    "the north", "the east", "the south", "the west", "above", "below"
};
const	sh_int	movement_loss	[SECT_MAX]	=
{ /*1  2  3  4  5  6  7  8  9  10 */
    1, 1, 3, 3, 4, 5, 4, 4, 3, 2, 
    3, 5, 9, 4, 5, 1, 1, 4, 3, 1,
    1, 4, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1 
};

const	sh_int	track_loss	[SECT_MAX]	=
{
    3, 2, 1, 1, 1, 2, 2, 3, 4, 4, 1,
    4, 4, 1, 1, 1, 2, 0, 4, 1, 1, 3,
    1, 4, 1, 1, 1, 1, 1, 1, 1, 1 
};

const int rev_dir []  =
{
    2, 3, 0, 1, 5, 4, 6
};

bool is_door( EXIT_DATA *pexit )
{
    return ( pexit != NULL 
	&& pexit->u1.to_room != NULL
	&& pexit->keyword[0] != '\0'
	&& strlen(pexit->keyword) > 0
	&& IS_SET( pexit->exit_info, EX_ISDOOR ) );

}

bool is_locked( LOCK_DATA * lock )
{
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_next;

    for ( lock_list = lock ; lock_list != NULL ; lock_list = lock_next )
    {
	lock_next = lock_list->next;
	if ( IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
	    return TRUE;
    }

    return FALSE;
}

bool is_bolted( LOCK_DATA * lock )
{
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_next;

    for ( lock_list = lock ; lock_list != NULL ; lock_list = lock_next )
    {
	lock_next = lock_list->next;
	if ( IS_SET( lock_list->lock_info, LOCK_LOCKED ) 
	  && IS_SET( lock_list->lock_info, LOCK_DEAD_BOLT ) )
	    return TRUE;
    }

    return FALSE;
}


bool is_locked_door( ROOM_INDEX_DATA *in_room, sh_int door )
{
    EXIT_DATA *pexit1;
    EXIT_DATA *pexit2;

    pexit1 = in_room->exit[ door ];
    pexit2 = pexit1->u1.to_room->exit[ rev_dir[ door ] ];

    return ( is_door( pexit1 )
	&& ( is_locked( pexit1->lock ) 
   	|| ( is_door( pexit2 )  && pexit2->u1.to_room == in_room
	&& is_bolted( pexit2->lock ) ) ) );
}

bool has_key( CHAR_DATA *ch, LOCK_DATA * lock )
{
    OBJ_DATA *obj;
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_next;
    bool found=FALSE;

    for ( lock_list = lock ; lock_list != NULL ; lock_list = lock_next )
    {
	lock_next = lock_list->next;
        for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
        {
	    if ( obj->pIndexData->vnum == lock_list->vnum )
	        found = TRUE;
        }
	if ( !found )
	{
	    if ( ch->home == ch->in_room->vnum )
	    {
	    	get_home_key(ch,"keylost");
	    	return TRUE;
	    }
  	    else
	    	return FALSE;
	}
    }
    return found;
}


bool  can_find_door	(CHAR_DATA *ch, sh_int doorflags)
{
 int chance;
 if (IS_SET(doorflags,EX_WALL) && !IS_IMMORTAL(ch) && !IS_SET( ch->plr2, PLR_SPECIAL_PASS ) )
	return FALSE;

 if (IS_SET(doorflags,EX_ILLUSION ) && IS_NPC(ch) )
	return FALSE;

 if ((!IS_SET(doorflags,EX_CLOSED) && IS_SET(doorflags, EX_ILLUSION  )) && !IS_SET(ch->plr, PLR_HOLYLIGHT ) )
	return FALSE; 

 if (!IS_SET(doorflags,EX_CLOSED) )
	return TRUE; /*open doors can always be seen */

 if (IS_SET(doorflags,EX_HIDDEN ) && !IS_IMMORTAL(ch) && !IS_NPC(ch) )
     { 
       chance = get_skill( ch, gsn_awareness ) + (IS_SET(ch->affected_by_2, AFF_SENSES ) ? ch->level : 0 );
       if (number_percent() < chance)
           {
             check_improve(ch,gsn_awareness,TRUE,1);
             return TRUE;
           }
       else
           {
             check_improve(ch,gsn_awareness,FALSE,1);
             return FALSE;
           }
     }

  return TRUE;
}

void do_disarm_trap( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA * obj;
    EXIT_DATA * pexit;
    char arg[MAX_INPUT_LENGTH];
    int skill;
    int door;
    bool found = FALSE;

    if ( ( skill = get_skill( ch, gsn_disarm_trap ) ) <= 0 )
    {
	send_to_char( "You don't know how to disarm traps.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char("Disarm what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_list( ch, arg, ch->in_room->contents ) ) != NULL )
    {
	act( "You attempt to disarm the trap in $p.", ch, obj, NULL, TO_CHAR );

	if ( obj->cont_data != NULL
	  && obj->cont_data->trap != NULL
	  && get_skill( ch, gsn_detect_trap ) + skill > number_range( 1, 200 ) )
	{

	    if ( !IS_SET(obj->cont_data->trap->flags, TRAP_AUTO_RESET ) )
	    	SET_BIT( obj->cont_data->trap->flags, TRAP_SPRUNG );

            check_improve(ch,gsn_disarm_trap,TRUE,1);
	}
    }
    else if ( ( obj = get_obj_list( ch, arg, ch->carrying ) ) != NULL )
    {
	act( "You attempt to disarm the trap in $p.", ch, obj, NULL, TO_CHAR );

	if ( obj->cont_data != NULL
	  && obj->cont_data->trap != NULL
	  && get_skill( ch, gsn_detect_trap ) + skill > number_range( 1, 200 ) )
	{

	    if ( !IS_SET(obj->cont_data->trap->flags, TRAP_AUTO_RESET ) )
	    	SET_BIT( obj->cont_data->trap->flags, TRAP_SPRUNG );

            check_improve(ch,gsn_disarm_trap,TRUE,1);
	}
    }
    else
    {
        for ( door = 0; door < 6 && !found ; door++ )
	if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
	    && pexit->keyword[0] != '\0'
	    && strlen(pexit->keyword) > 1 
	    && is_name( arg, pexit->keyword ) )
	{
	    found = TRUE;
	    act( "You attempt to disarm the trap on $d.", ch, NULL, pexit->keyword, TO_CHAR );

	    if ( pexit->trap != NULL
	      && get_skill( ch, gsn_detect_trap ) + skill > number_range( 1, 200 ) )
	    {
                check_improve(ch,gsn_disarm_trap,TRUE,1);

	        if ( !IS_SET( pexit->trap->flags, TRAP_AUTO_RESET ) )
	            SET_BIT( pexit->trap->flags, TRAP_SPRUNG );
	    }
	}

    }

    if ( !found )
    {
	send_to_char( "That is not here.\n\r", ch );
	return;
    }

    return;
}

void do_mine( CHAR_DATA *ch, char * argument )
{
    char arg1[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *tunnel;
    ROOM_INDEX_DATA *cavern;
    EXIT_DATA *pexit;
    OBJ_DATA *obj;
    OBJ_DATA *pick;
    AREA_DATA *pArea;
    int loop,door,skill,iHash,chance,move;
    long vnum;

    pick = NULL;
    in_room = ch->in_room;
    pArea = in_room->area;
    if ( ( skill = get_skill( ch,gsn_mining ) ) <= 0 )
    {
        send_to_char("You don't know how to mine.\n\r",ch);
        return;
    }

    for ( pick = ch->carrying; pick != NULL && !IS_SET( pick->extra_flags, ITEM_PICK ) ; pick = pick->next_content );

    if ( pick == NULL )
    {
	send_to_char( "You do not have the right equipment to mine.\n\r", ch );
        return;
    }

    if ( ( ch->in_room->sector_type != SECT_UNDERGROUND 
	&& ch->in_room->sector_type != SECT_MOUNTAIN )
	|| IS_SET( ch->in_room->area->area_flags, AREA_PKSAFE ) )
    {
	send_to_char( "These are not the right mining conditions.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg1 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Mine which direction?\n\r", ch );
	return;
    }

         if ( !str_cmp( arg1, "n" ) || !str_cmp( arg1, "north" ) ) door = 0;
    else if ( !str_cmp( arg1, "e" ) || !str_cmp( arg1, "east"  ) ) door = 1;
    else if ( !str_cmp( arg1, "s" ) || !str_cmp( arg1, "south" ) ) door = 2;
    else if ( !str_cmp( arg1, "w" ) || !str_cmp( arg1, "west"  ) ) door = 3;
    else if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up"    ) ) door = 4;
    else if ( !str_cmp( arg1, "d" ) || !str_cmp( arg1, "down"  ) ) door = 5;
    else
    {
        send_to_char( "Mine which direction?\n\r", ch );
        return;
    }

    if ( ( pexit = ch->in_room->exit[door] ) != NULL )
    {
        send_to_char( "There isn't anything to mine there.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "u" ) || !str_cmp( arg1, "up" ) )
    {
	send_to_char( "You can't mine upward.\n\r", ch );
	return;
    }

    if ( pArea->lvnum == 0 || pArea->uvnum == 0 )
    {
	send_to_char( "You are unable to mine here due to bad mining conditions.\n\r", ch );
	return;
    }

    for ( vnum = pArea->lvnum; free_room[vnum]; vnum++ );

    if ( vnum > pArea->uvnum || vnum < pArea->lvnum )
    {
	send_to_char( "You are unable to mine here due to bad mining conditions.\n\r", ch );
	return;
    }

    if ( free_room[vnum] )
    {
	send_to_char( "You are unable to mine here due to bad mining conditions.\n\r", ch );
	return;
    }

    chance = get_skill( ch, gsn_mining );
    move = 10;
    if ( ch->move < move )
    {
	send_to_char( "You don't have enough energy to mine.\n\r", ch );
	return;
    }

    send_to_char( "You begin mining for ore.\n\r", ch );
    act( "$n begins mining for ore.", ch, NULL, NULL, TO_ROOM );
    WAIT_STATE( ch, 5 ); 
    if ( chance > 1 && number_percent( ) < chance )
    {
	check_mv_improve( ch, move );
	tunnel = get_room_index( ROOM_TUNNEL );
	cavern = get_room_index( ROOM_CAVERN );
    	free_room[vnum] 		= 1;
    	to_room                  	= pRoomIndex_alloc();
    	to_room->vnum            	= vnum;
    	to_room->area            	= pArea;
	SET_BIT(to_room->room_flags_2, ROOM_NOSAVE);
	SET_BIT(to_room->room_flags, ROOM_INDOORS);
    	to_room->sector_type		= SECT_UNDERGROUND;

	switch ( number_range( 1, 5 ) )
	{
	    case 1:
	    case 2:
	    case 3:
    	    	to_room->size			= 3;
      	    	strncpy( to_room->name, tunnel->name, 80 );
    	    	to_room->description     	= str_dup ( tunnel->description );
        	for ( loop = 0; loop < ( skill / 7 ) ; loop++ )
		{
		switch ( number_range(1, 100) )
		{
		    case 1:
		    case 2:
			obj = create_object( get_obj_index( OBJ_VNUM_ORE_LARGE ) );
			obj_to_room( obj, to_room );
			break;
		    case 3:
		    case 4:
		    case 5:
		    case 6:
		    case 7:
			obj = create_object( get_obj_index( OBJ_VNUM_ORE_MEDIUM ) );
			obj_to_room( obj, to_room );
			break;
		    case 8:
		    case 9:
		    case 10:
		    case 11:
		    case 12:
		    case 13:
		    case 14:
		    case 15:
		    case 16:
		    case 17:
			obj = create_object( get_obj_index( OBJ_VNUM_ORE_SMALL ) );
			obj_to_room( obj, to_room );
			break;
		    case 18:
		    case 19:
		    case 20:
		    case 21:
		    case 22:
			if ( number_range( 0, 5 ) == 2 )
			{
			    obj = create_object( get_obj_index( 18008 ) );
			    switch ( number_range( 1, 8 ) )
			    {
				case 1:
				    if ( ch->in_room->area->world == 0 )
				    {
				    	strncpy( obj->name, "giant platinum", 30 );
				    	strncpy( obj->short_descr,"a giant piece of platinum", 80 );
				    	strncpy( obj->description, "A giant piece of platinum lies here.\n\r", 80);
				    	obj->material = MAT_PLATINUM;
				    }
				    else if ( ch->in_room->area->world == 1 )
				    {
					strncpy( obj->name, "giant mithril", 30 );
					strncpy( obj->short_descr,"a giant piece of mithril", 80 );
					strncpy( obj->description, "A giant piece of mithril lies here.\n\r", 80 );
				    	obj->material = MAT_MITHRIL;
				    }
				    else if ( ch->in_room->area->world == 2 )
				    {
					strncpy( obj->name, "giant obsidian", 30 );
					strncpy( obj->short_descr,"a giant obsidian rock", 80 );
					strncpy( obj->description, "A giant obsidian rock lies here.\n\r", 80 );
				    	obj->material = MAT_OBSIDIAN;
				    }
				    break;
				case 2:
				    strncpy( obj->name, "giant diamond", 30 );
				    strncpy( obj->short_descr,"a giant piece of diamond", 80 );
				    strncpy( obj->description, "A giant piece of diamond lies here.\n\r", 80 );
				    obj->material = MAT_DIAMOND;
				    break;
				case 3:
				    strncpy( obj->name, "giant silver", 30 );
				    strncpy( obj->short_descr, "a giant piece of silver", 80 );
				    strncpy( obj->description, "A giant piece of silver lies here.\n\r", 80 );
				    obj->material = MAT_SILVER;
				    break;
				case 4:
				    strncpy( obj->name, "giant gold", 30 );
				    strncpy( obj->short_descr , "a giant piece of gold", 80);
				    strncpy( obj->description, "A giant piece of gold lies here.\n\r", 80 );
				    obj->material = MAT_GOLD;
				    break;
				case 5:
				    strncpy( obj->name, "giant ruby", 30 );
				    strncpy( obj->short_descr, "a giant ruby stone", 80 );
				    strncpy( obj->description, "A giant ruby stone lies here.\n\r", 80 );
				    obj->material = MAT_RUBY;
				    break;
				case 6:
				    strncpy( obj->name, "giant sapphire", 30 );
				    strncpy( obj->short_descr, "a giant sapphire stone", 80 );
				    strncpy( obj->description, "A giant sapphire stone lies here.\n\r", 80 );
				    obj->material = MAT_SAPPHIRE;
				    break;
				case 7:
				    strncpy( obj->name, "giant emerald", 30 );
				    strncpy( obj->short_descr, "a giant emerald stone", 80 );
				    strncpy( obj->description, "A giant emerald stone lies here.\n\r", 80 );
				    obj->material = MAT_EMERALD;
				    break;
				case 8:
				    strncpy( obj->name, "giant alabaster", 30 );
				    strncpy( obj->short_descr, "a giant alabaster", 80 );
				    strncpy( obj->description, "A giant alabaster lies here.\n\r", 80 );
				    obj->material = MAT_ALABASTER;
				    break;
			    }
			    obj_to_room( obj, to_room );
			}
			else if ( number_range( 0, 5 ) == 1 )
			{
			    obj = create_object( get_obj_index( 18008 ) );
			    switch ( number_range( 1, 8 ) )
			    {
				case 1:
				    if ( ch->in_room->area->world == 0 )
				    {
                        strncpy( obj->name, "giant platinum", 30 );
                        strncpy( obj->short_descr, "a giant piece of platinum", 80 );
                        strncpy( obj->description, "A giant piece of platinum lies here.\n\r", 80 );
                        obj->material = MAT_PLATINUM;
				    }
				    else if ( ch->in_room->area->world == 1 )
				    {
				    	obj->material = MAT_MITHRIL;
					strncpy( obj->name, "giant mithril", 30 );
					strncpy( obj->short_descr, "a giant piece of mithril", 80 );
					strncpy( obj->description, "A giant piece of mithril lies here.\n\r", 80 );
				    }
				    else if ( ch->in_room->area->world == 2 )
				    {
					strncpy( obj->name, "giant obsidian", 30 );
					strncpy( obj->short_descr, "a giant obsidian rock", 80 );
					strncpy( obj->description, "A giant obsidian rock lies here.\n\r", 80 );
				    	obj->material = MAT_OBSIDIAN;
				    }
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
				case 2:
				    strncpy( obj->name, "giant diamond", 30 );
				    strncpy( obj->short_descr, "a giant diamond rock", 80 );
				    strncpy( obj->description, "A giant diamond rock lies here.\n\r", 80 );
				    obj->material = MAT_DIAMOND;
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
				case 3:
				    strncpy( obj->name, "giant silver", 30 );
				    strncpy( obj->short_descr, "a giant piece of silver", 80 );
				    strncpy( obj->description, "A giant piece of silver lies here.\n\r", 80 );
				    obj->material = MAT_SILVER;
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
				case 4:
				    strncpy( obj->name, "giant gold", 30 );
				    strncpy( obj->short_descr, "a giant piece of gold", 80 );
				    strncpy( obj->description, "A giant piece of gold lies here.\n\r", 80 );
				    obj->material = MAT_GOLD;
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
				case 5:
				    strncpy( obj->name, "giant ruby", 30 );
				    strncpy( obj->short_descr,"a giant ruby stone", 80);
				    strncpy( obj->description, "A giant ruby stone lies here.\n\r", 80 );
				    obj->material = MAT_RUBY;
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
				case 6:
				    strncpy( obj->name, "giant sapphire", 30 );
				    strncpy( obj->short_descr, "a giant sapphire stone", 80 );
				    strncpy( obj->description, "A giant sapphire stone lies here.\n\r", 80 );
				    obj->material = MAT_SAPPHIRE;
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
				case 7:
				    strncpy( obj->name, "giant emerald", 30 );
				    strncpy( obj->short_descr, "a giant emerald stone", 80 );
				    strncpy( obj->description, "A giant emerald stone lies here.\n\r", 80 );
				    obj->material = MAT_EMERALD;
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
				case 8:
                    strncpy( obj->name, "giant alabaster", 30 );
                    strncpy( obj->short_descr, "a giant alabaster", 80 );
                    strncpy( obj->description, "A giant alabaster lies here.\n\r", 80 );
                    obj->material = MAT_ALABASTER;
				    REMOVE_BIT( obj->extra_flags, ITEM_MAGIC );
				    break;
			    }

			    obj_to_room( obj, to_room );
			}
			break;
		    case 23:
			if ( ch->level >= 43 && number_range( 1, 10 ) == 10 )
			    char_to_room( create_mobile( get_mob_index( 21025 ) ), to_room );
			if ( ch->level >= 38 && number_range( 1, 8 ) == 8 )
			    char_to_room( create_mobile( get_mob_index( 18002 ) ), to_room );
			if ( ch->level >= 34 && number_range( 1, 6 ) == 6 )
			    char_to_room( create_mobile( get_mob_index( 28605 ) ), to_room );
			if ( ch->level >= 29 && number_range( 1, 4 ) == 4 )
			    char_to_room( create_mobile( get_mob_index( 7803 ) ), to_room );
			if ( ch->level >= 1 && number_range( 1, 2 ) == 2 )
			{
			    if ( number_range( 0, 1 ) == 0 )
			    	char_to_room( create_mobile( get_mob_index( 18001 ) ), to_room );
			    else
			    	char_to_room( create_mobile( get_mob_index( 19901 ) ), to_room );
			}
	
			break;
		    case 24:
			if ( number_range(1, 100) <= 2 )
			{	
			    obj = create_object( get_obj_index( 18026 ) );
			    obj_to_room( obj, to_room );
			}
			break;
		    case 25:
		    case 26:
		    case 27:
		    case 28:
		    case 29:
		    case 30:
		    case 31:
		    case 32:
		    case 33:
		    case 34:
		    case 35:
		    case 36:
		    case 37:
		    case 38:
		    case 39:
		    case 40:
		    case 41:
		    case 42:
		    case 43:
		    case 44:
		    case 45:
		    case 46:
		    case 47:
		    case 48:
		    case 49:
		    case 50:
		    case 51:
		    case 52:
		    case 53:
		    case 54:
		    case 55:
		    case 56:
		    case 57:
		    case 58:
		    case 59:
		    case 60:
		    case 61:
		    case 62:
		    case 63:
		    case 64:
		    case 65:
		    default:
			break;
		}
		}
		break;
	    case 4:
	    case 5:
    	    	to_room->size			= 5;
      	    	strncpy( to_room->name, cavern->name, 80 );
    	    	to_room->description     	= str_dup ( cavern->description );
        	for ( loop = 0; loop < ( skill / 5 ) ; loop++ )
		{
		switch ( number_range(1, 100) )
		{
		    case 1:
			obj = create_object( get_obj_index( OBJ_VNUM_ORE_LARGE ) );
			obj_to_room( obj, to_room );
			break;
		    case 2:
		    case 3:
		    case 4:
		    case 5:
			obj = create_object( get_obj_index( OBJ_VNUM_ORE_MEDIUM ) );
			obj_to_room( obj, to_room );
			break;
		    case 6:
		    case 7:
		    case 8:
		    case 9:
		    case 10:
			obj = create_object( get_obj_index( OBJ_VNUM_ORE_SMALL ) );
			obj_to_room( obj, to_room );
			break;
		    case 11:
		    case 12:
			if ( ch->level >= 43 && number_range( 1, 10 ) == 10 )
			    char_to_room( create_mobile( get_mob_index( 21025 ) ), to_room );
			if ( ch->level >= 38 && number_range( 1, 8 ) == 8 )
			    char_to_room( create_mobile( get_mob_index( 18002 ) ), to_room );
			if ( ch->level >= 34 && number_range( 1, 6 ) == 6 )
			    char_to_room( create_mobile( get_mob_index( 28605 ) ), to_room );
			if ( ch->level >= 29 && number_range( 1, 4 ) == 4 )
			    char_to_room( create_mobile( get_mob_index( 7803 ) ), to_room );
			if ( ch->level >= 1 && number_range( 1, 2 ) == 2 )
			{
			    if ( number_range( 0, 1 ) == 0 )
			    	char_to_room( create_mobile( get_mob_index( 18001 ) ), to_room );
			    else
			    	char_to_room( create_mobile( get_mob_index( 19901 ) ), to_room );
			}
	
			break;
		    case 13:
		    case 14:
		    case 15:
		    case 16:
		    case 17:
		    case 18:
		    case 19:
		    case 20:
		    case 21:
		    case 22:
		    case 23:
		    case 24:
		    case 25:
		    case 26:
		    case 27:
		    case 28:
		    case 29:
		    case 30:
		    case 31:
		    case 32:
		    case 33:
		    case 34:
		    case 35:
		    case 36:
		    case 37:
		    case 38:
		    case 39:
		    case 40:
		    case 41:
		    case 42:
		    case 43:
		    case 44:
		    case 45:
		    case 46:
		    case 47:
		    case 48:
		    case 49:
		    case 50:
		    case 51:
		    case 52:
		    case 53:
		    case 54:
		    case 55:
		    case 56:
		    case 57:
		    case 58:
		    case 59:
		    case 60:
		    case 61:
		    case 62:
		    case 63:
		    case 64:
		    case 65:
		    default:
			break;
		}
		}
		break;
	}

    	iHash                       	= vnum % MAX_KEY_HASH;
    	to_room->next            	= room_index_hash[iHash];
    	room_index_hash[iHash]      	= to_room;
    	send_to_char( "You mine a tunnel successfully.\n\r", ch );
	act( "$n mines a tunnel successfully.", ch, NULL, NULL, TO_ROOM );
    	make_exit(in_room, to_room, door);
    	make_exit(to_room, in_room, rev_dir[door]);
	check_improve( ch, gsn_mining, TRUE, 2 );
    }
    else if ( chance < number_range( 1, 100 ) )
    {
	send_to_char( "You chip at the rocks surface for ore.\n\r", ch );
	act( "$n chips at the rocks surface for ore.", ch, NULL, NULL, TO_ROOM );
	// only half move for failure
	move = 5;
	switch ( number_range(1, 100) )
	{
	    case 1:
		obj = create_object( get_obj_index( OBJ_VNUM_ORE_LARGE ) );
		obj_to_room( obj, in_room );
		check_improve( ch, gsn_mining, TRUE, 1 );
		break;
	    case 2:
	    case 3:
		obj = create_object( get_obj_index( OBJ_VNUM_ORE_MEDIUM ) );
		obj_to_room( obj, in_room );
		check_improve( ch, gsn_mining, TRUE, 1 );
		break;
	    case 4:
	    case 5:
	    case 6:
	    case 7:
		obj = create_object( get_obj_index( OBJ_VNUM_ORE_SMALL ) );
		obj_to_room( obj, in_room );
		check_improve( ch, gsn_mining, TRUE, 1 );
		break;
	    case 8:
	    case 9:
	    case 10:
	    case 11:
	    case 12:
	    case 13:
	    case 14:
	    case 15:
	    case 16:
	    case 17:
	    case 18:
	    case 19:
	    case 20:
	    default:
		send_to_char( "You fail to mine anything from the rock surface.\n\r", ch );
		check_improve( ch, gsn_mining, FALSE, 1 );
		break;
	}
    }    
    else
    {
	send_to_char( "You fail to mine anything from the rocks surface.\n\r", ch );
	act( "$n fails to mine anything from the rocks surface.", ch, NULL, NULL, TO_ROOM );
	check_improve( ch, gsn_mining, FALSE, 1 );
    }

    ch->move = UMAX( 0, ch->move - move );
    return;
}

void do_dig( CHAR_DATA *ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA * obj;
    OBJ_DATA * shovel;

	for ( shovel =  ch->carrying; shovel != NULL && shovel->pIndexData->vnum != OBJ_VNUM_SHOVEL ; shovel = shovel->next_content );

        if ( shovel == NULL )
        {
                send_to_char( "You do not have the right equipment to dig.\n\r", ch );
                return;
        }

	for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj->next_content )
        {

            if ( IS_SET( obj->extra_flags , ITEM_BURIED ) )
            {
		act( "$n digs something out of the earth.", ch, NULL, NULL, TO_ROOM );
                sprintf( buf, "You find %s!\n\r", obj->short_descr);
                send_to_char( buf, ch );
		REMOVE_BIT(obj->extra_flags,ITEM_BURIED);
		return;
            }
        }
	act( "$n digs a hole in the earth.", ch, NULL, NULL, TO_ROOM );
	send_to_char("You didn't find anything buried here.\n\r", ch );
        return;
}

void do_cover( CHAR_DATA *ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA * obj;
    OBJ_DATA * shovel;

        if ( ch->in_room->sector_type == SECT_CITY 
		|| ch->in_room->sector_type == SECT_NONE 
	    	|| ch->in_room->sector_type == SECT_UNDERWATER
	    	|| ch->in_room->sector_type == SECT_WATER_SWIM
	    	|| ch->in_room->sector_type == SECT_WATER_NOSWIM
	    	|| ch->in_room->sector_type == SECT_AIR
	    	|| ch->in_room->sector_type == SECT_LAVA)
	{
                send_to_char( "You can't cover anything here effectivly.\n\r", ch );
		return;
	}

	for ( shovel =  ch->carrying; shovel != NULL && shovel->pIndexData->vnum != OBJ_VNUM_SHOVEL ; shovel = shovel->next_content );
        if ( shovel == NULL )
        {
                send_to_char( "You do not have the right equipment to cover it.\n\r", ch );
                return;
        }

  	argument = one_argument( argument, arg );

    	if ( arg[ 0 ] == '\0' )
        {
                send_to_char( "What do you want to cover?\n\r", ch );
                return;
        }

	if ( ( obj = get_obj_list( ch, arg, ch->in_room->contents ) ) != NULL )
        {
		if ( obj->wear_data == NULL || !IS_SET(obj->wear_data->wear_flags, ITEM_TAKE) )
		{
			send_to_char("You can't cover that.\n\r",ch);
			return;
		}

		if (obj->wear_data == NULL || obj->wear_data->weight >= 100 || obj->wear_data->size >= 3)
		{
			send_to_char("That is too large to cover effectivly.\n\r", ch);
			return;
		}

		if ( IS_SET( obj->extra_flags , ITEM_BURIED ) )
		{
			send_to_char("It is already buried.\n\r", ch);
			return;
		}

		act( "$n covers something beneath the surface of the earth.", ch, NULL, NULL, TO_ROOM );
                sprintf( buf, "You cover %s under the ground!\n\r", obj->short_descr);
                send_to_char( buf, ch );
		SET_BIT(obj->extra_flags,ITEM_BURIED);
	}
	else
	    	send_to_char("That is not on the ground to cover.\n\r", ch );
        return;
}

void do_search( CHAR_DATA *ch, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    OBJ_DATA * obj;
    EXIT_DATA * pexit;
    int door, skill;
    int bonus = ( IS_SET( ch->affected_by_2, AFF_SENSES ) ? ch->level : 0);
    bool found=FALSE;


    argument = one_argument( argument, arg );

    if ( arg[ 0 ] == '\0' )
    {
        for ( door = 0; door < 6 ; door++ )
	if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
	    && pexit->keyword[0] != '\0'
	    && strlen(pexit->keyword) > 1 )
  	{
	    if ( ( !IS_SET( pexit->exit_info, EX_WALL ) || IS_IMMORTAL(ch) )
	      && IS_SET( pexit->exit_info, EX_ISDOOR )
	      && IS_SET( pexit->exit_info, EX_CLOSED )
	      && IS_SET( pexit->exit_info, EX_SECRET )
	      && number_percent() < 50 )
	    {
	        found = TRUE;
	        sprintf( buf, "You find a secret $d in the %s!", wall_name[ door ]);
	        act( buf, ch, NULL, pexit->keyword, TO_CHAR );
		if ( pexit->trap != NULL && !IS_SET(pexit->trap->flags, TRAP_SPRUNG )
		  && number_percent() < get_skill( ch, gsn_detect_trap ) -  25 + bonus )
		{
		    sprintf( buf, "You find a trap on the $d in the %s!", dir_name[ door ] );
	            act( buf, ch, NULL, pexit->keyword, TO_CHAR );
                    check_improve(ch,gsn_detect_trap,TRUE,1);
		}
	    } 
	    else if ( pexit->trap != NULL && !IS_SET(pexit->trap->flags, TRAP_SPRUNG )
		  && number_percent() < get_skill( ch, gsn_detect_trap ) - 25 + bonus )
		{
		    found = TRUE;
		    sprintf( buf, "You find a trap on the $d in the %s!", dir_name[ door ] );
	            act( buf, ch, NULL, pexit->keyword, TO_CHAR );
                    check_improve(ch,gsn_detect_trap,TRUE,1);
		}
	}

	skill = get_skill( ch, gsn_detect_trap ) - 25 + bonus;

	for ( obj = ch->in_room->contents ; obj != NULL ; obj = obj->next_content )
	{

	    if ( obj->cont_data != NULL && obj->cont_data->trap != NULL
	      && !IS_SET( obj->cont_data->trap->flags , TRAP_SPRUNG )
	      && can_see_obj( ch, obj )
	      && !IS_SET( obj->extra_flags, ITEM_NO_SHOW )
	      && !IS_SET( obj->extra_flags, ITEM_BURIED )
	      && number_percent() < skill )
	    {
	        sprintf( buf, "You find a trap on %s!\n\r", obj->short_descr);
		send_to_char( buf, ch );
                check_improve(ch,gsn_detect_trap,TRUE,1);
		found = TRUE;
	    }
	}

        if ( !found )
        {
       	    send_to_char("You didn't find any secret doors or traps.\n\r", ch );
        }
	return;
    }
    else if ( ( obj = get_obj_list( ch, arg, ch->in_room->contents ) ) != NULL 
          || ( obj = get_obj_list( ch, arg, ch->carrying ) ) != NULL )
    {
	    if ( obj->cont_data != NULL && obj->cont_data->trap != NULL
	      && !IS_SET( obj->cont_data->trap->flags , TRAP_SPRUNG )
	      && number_percent() < get_skill( ch, gsn_detect_trap ) + bonus )
	    {
	        sprintf( buf, "You find a trap on %s!\n\r", obj->short_descr);
		send_to_char( buf, ch );
                check_improve(ch,gsn_detect_trap,TRUE,1);
	    }
	    else
		send_to_char( "You didn't find a trap.\n\r", ch );
	return;
    }
    else
    {
	found = FALSE;
        for ( door = 0; door < 6 ; door++ )
	if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
	    && pexit->keyword[0] != '\0'
	    && strlen(pexit->keyword) > 1 
	    && is_name( arg, pexit->keyword ) )
  	{
	    if ( ( !IS_SET( pexit->exit_info, EX_WALL ) || IS_IMMORTAL(ch) )
	      && IS_SET( pexit->exit_info, EX_ISDOOR )
	      && IS_SET( pexit->exit_info, EX_CLOSED )
	      && IS_SET( pexit->exit_info, EX_SECRET ) )
	    {
	        found = TRUE;
	        sprintf( buf, "You find a secret $d in the %s!", wall_name[ door ]);
	        act( buf, ch, NULL, pexit->keyword, TO_CHAR );
		if ( pexit->trap != NULL
		  && number_percent() < get_skill( ch, gsn_detect_trap ) + bonus )
		{
		    sprintf( buf, "You find a trap on the $d in the %s!", dir_name[ door ] );
	            act( buf, ch, NULL, pexit->keyword, TO_CHAR );
                    check_improve(ch,gsn_detect_trap,TRUE,1);
		}
		return;
	    } 
	    else if ( pexit->trap != NULL
		  && number_percent() < get_skill( ch, gsn_detect_trap ) + bonus )
		{
		    found = TRUE;
		    sprintf( buf, "You find a trap on the $d in the %s!", dir_name[ door ] );
	            act( buf, ch, NULL, pexit->keyword, TO_CHAR );
                    check_improve(ch,gsn_detect_trap,TRUE,1);
		    return;
		}
	}
	if ( !found )
	{
	    send_to_char( "You didn't find any secret doors or traps.", ch );
	    return;
	}

    }

}

void move_char( CHAR_DATA *ch, int door, bool follow )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    char buf[MAX_INPUT_LENGTH];
    bool found_boat = FALSE, trig;
    bool boat_needed = TRUE;
    int i;
    int minerals = 0;

    *buf = '\0';
    if ( ch->pcdata != NULL )
    	for ( i = 0 ; i < MAX_MINERAL ; i++ )
	    minerals += ch->pcdata->forge[i];

    if ( minerals > 0 )
    {
	send_to_char( "You can't leave the forge while you have minerals in the forge.\n\r", ch );
	return;
    }

    if ( ch->pcdata != NULL && ( ch->pcdata->hand != NULL
      || ch->pcdata->deck != NULL 
      || ch->pcdata->played != NULL
      || ch->pcdata->discarded != NULL ) )
    {
	send_to_char( "You cannot leave a card game in progress(ask dealer to type deck stop)\n\r", ch );
	return;
    }

    if ( ch->mount != NULL && IS_SET( ch->mount->affected_by , AFF_MOUNTED ) 
	&& ch->mount->position < POS_STANDING )
    {
	if ( IS_SET( ch->plr2, PLR_SPECIAL_PASS ) )
	    ch->mount->position = POS_STANDING;
	else
	{
	    send_to_char("Your mount can't move right now.\n\r", ch );
	    return;
	}
    }

    if ( door < 0 || door > 5 )
    {
	bug( "Do_move: bad door %d.", door );
	return;
    }

    in_room = ch->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||	 !can_see_room( ch, pexit->u1.to_room ) )
    {
	send_to_char( "You bump into something solid.\n\r", ch );
	if ( !IS_NPC( ch ) )
	    act( "$n bumps into something solid.\n\r", ch, NULL, NULL, TO_ROOM );
	return;
    }

    if ( ( ( IS_SET(pexit->exit_info, EX_CLOSED )
    &&   !IS_SET(pexit->exit_info, EX_ILLUSION ) /* No matter what */
    &&   !IS_AFFECTED(ch, AFF_DIMENSION_WALK) )
    ||	 IS_SET( pexit->exit_info, EX_WALL ) )
    &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
    &&   !IS_SET(ch->plr, PLR_WIZPASS ) )
    {
	if ( !can_find_door( ch, pexit->exit_info ) )
        { 
	    send_to_char( "You bump into something solid.\n\r", ch );
	    if ( !IS_NPC( ch ) )
    	        act( "$n bumps into something solid.\n\r", ch, NULL, NULL, TO_ROOM );
        } 
        else if ( IS_SET( pexit->exit_info, EX_SECRET ) )
        { 
	    send_to_char( "You bump into something solid.\n\r", ch );
	    if ( !IS_NPC(ch) )
	    	act( "$n bumps into something solid.\n\r", ch, NULL, NULL, TO_ROOM );
        } 
	else
	{
	 if ( IS_NPC(ch) && IS_SET(ch->act2, ACT2_WALKTO) )
	 {
	  do_open(ch, pexit->keyword);
	  return;
	 }
         {act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );}

	}
	return;
    }

    if ( IS_SET(pexit->exit_info, EX_CLOSED )
    &&   IS_SET(pexit->exit_info, EX_PASSPROOF) 
    &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
    &&   !IS_SET(ch->plr, PLR_WIZPASS ) )
    {
	if (!can_find_door(ch,pexit->exit_info))
        {
	send_to_char( "You bump into something solid.\n\r", ch );
	if ( !IS_NPC(ch) )
	    act( "$n bumps into something solid.\n\r", ch, NULL, NULL, TO_ROOM );
        } 
        else
         {act( "Some force prevents you.", ch, NULL, pexit->keyword, TO_CHAR );}
	return;
    }
    if ( IS_AFFECTED(ch, AFF_CONTROL_MIND)
    &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
    &&   ch->master != NULL
    &&   in_room == ch->master->in_room )
    {
	send_to_char( "What?  And leave your beloved master?\n\r", ch );
	return;
    }

    if ( !check_room_size( to_room, ch ) && !IS_IMMORTAL(ch) )
    {
	    send_to_char("You are too big to fit through.\n\r", ch);
	    return;
    }

    if ( room_is_private( to_room ) 
    &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )  &&
       (IS_OPTIVISION(ch) || !IS_SET(ch->plr,PLR_WIZINVIS)))
    {
	send_to_char( "The room is not large enough to accept another body.\n\r", ch );
	return;
    }

    if ( to_room->guilds != 0
      && !IS_SET( ch->guilds, to_room->guilds )
      && !IS_IMMORTAL( ch ) )
    {
	send_to_char( "You are not a member of that guild.\n\r", ch );
	return;
    }

    {
	int move;

	if ( pexit->trap != NULL && IS_SET( pexit->trap->flags, TRAP_ENTER ) )
	{
	   trap_cast_spell( ch, pexit->trap );
	   if ( ch->in_room != in_room )
		return;
	}

	if ( in_room->sector_type == SECT_DEATH 
	||  to_room->sector_type == SECT_DEATH )
	{ 
	  if (!IS_IMMORTAL(ch) && !IS_NPC(ch) ) {
	  char_from_room(ch);
	  char_to_room (ch,to_room);
	  do_look(ch,"");
	  send_to_char("`rYou are DEAD!!!`w\n\r",ch);
	  raw_kill (ch,FALSE,FALSE);
	  if ( !IS_NPC(ch) )
	  {
	      extract_obj(ch->pcdata->corpse);
	      ch->pcdata->corpse = NULL;
	      pardon_death( ch );
	  }
	  return;}
		
        }

	if ( in_room->sector_type == SECT_LAVA
	||  to_room->sector_type == SECT_LAVA)
	{
	  if (!is_affected(ch,gsn_stone_skin) && !IS_IMMORTAL(ch) 
		&& !IS_SET(ch->imm_flags, IMM_FIRE) && !IS_NPC(ch))
	      { 
		if ( IS_SET(ch->res_flags, RES_FIRE) )
		     ch->hit = UMAX(1,ch->hit - number_range(25,75) );
		else
		    ch->hit = UMAX(1,ch->hit - number_range(50,100) );
		send_to_char("You are burnt severely by lava!\n\r", ch);
	      }	
	 }


	if ( ( IS_UNDERWATER( ch )
	  || in_room->sector_type == SECT_WATER_SWIM )
	  &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
	  && !IS_IMMORTAL( ch ) )
        {
	    OBJ_DATA * obj;
	    bool floating = FALSE;

	    if ( to_room->sector_type == SECT_WATER_SWIM )
	    {
		for ( obj = ch->carrying ; obj != NULL ; obj = obj->next_content )
		    if ( IS_SET( obj->extra_flags, ITEM_FLOAT ) )
			floating = TRUE;

		if ( IS_AFFECTED( ch, AFF_FLYING|AFF_LEVITATING ) )
		    floating = TRUE;
	    }

	    if (!IS_NPC(ch) || ch->pIndexData->moveable == NULL ) 
	    if ( ( to_room->sector_type == SECT_UNDERWATER || to_room->sector_type == SECT_OCEAN_BOTTOM  
	     || ( to_room->sector_type == SECT_WATER_SWIM && !floating ) ) )
	    {
	       if ( get_skill( ch, gsn_swim ) < number_percent() && !IS_AFFECTED( ch, AFF_SWIM ) ) 	
	    {
		send_to_char( "The swift current holds you back.\n\r",ch );
		check_improve( ch, gsn_swim, FALSE, 1 );
		return;
	    }
	    else
		check_improve( ch, gsn_swim, TRUE, 1 );
	    }
	}

	if ( to_room->sector_type != SECT_AIR
	&& IS_SET(ch->affected_by_2, AFF_FALLING)
	&& !IS_IMMORTAL(ch) )
	{
	    if ( to_room->sector_type == SECT_UNDERWATER
		|| to_room->sector_type == SECT_OCEAN_BOTTOM
		|| to_room->sector_type == SECT_WATER_NOSWIM
		|| to_room->sector_type == SECT_WATER_SWIM )
		send_to_char("You land with a splash!\n\r", ch);
	    else
	    {
		send_to_char("You land HARD! Ouch!\n\r", ch);
		ch->hit = UMAX(1,ch->hit - number_range(5,39) );
	    }
		REMOVE_BIT(ch->affected_by_2, AFF_FALLING);
	}

	if ( to_room->sector_type == SECT_UNDERWATER || to_room->sector_type == SECT_OCEAN_BOTTOM )
	{
	    if ( IS_NPC(ch) && !IS_SET( ch->affected_by, AFF_BREATHE_UNDERWATER ) )
		return;
	    if ( !IS_IMMORTAL(ch)
	      &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
                && ( ch->mount != NULL 
                   && !IS_SET( ch->mount->affected_by , AFF_BREATHE_UNDERWATER ) ))
	    {
		if ( IS_SET( ch->mount->affected_by, AFF_MOUNTED) )
		{		
	    	    send_to_char( "Your mount will not move, because it doesn't want to drown.\n\r", ch );
	 	    return;
	        }
		else
		{
		    send_to_char( "You mount will not go into the water\n\r",ch);
		    set_position( ch, POS_RESTING );
		}
	    }
	    if ( !IS_IMMORTAL(ch) && !IS_AFFECTED( ch, AFF_BREATHE_UNDERWATER ) && !IS_AFFECTED(ch, AFF_HOLD_BREATH) )
	    {
		send_to_char( "You take a large breath of air before you go underwater.\n\r", ch );
		do_hold_breath( ch, "");
	    }
	}

	if (( in_room->sector_type == SECT_WATER_NOSWIM
	||    to_room->sector_type == SECT_WATER_NOSWIM )
        &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
  	&&    !IS_AFFECTED(ch,AFF_FLYING|AFF_LEVITATING) 
                && ( ch->mount == NULL 
                   || IS_SET( ch->mount->affected_by , AFF_MOUNTED )
                   || !IS_AFFECTED( ch->mount , AFF_FLYING|AFF_LEVITATING ) ) )
	{
	    OBJ_DATA *obj;
 	    boat_needed = TRUE;

	    /*
	     * Look for a boat.
	     */

	    if (IS_IMMORTAL(ch) || race_table[ch->race][ch->sex].world == 0 )
		boat_needed = FALSE;

	    for ( obj = ch->carrying; obj != NULL; obj = obj->next_content )
	    {
		if ( IS_SET( obj->extra_flags, ITEM_FLOAT ) )
		{
		    found_boat = TRUE;
		    break;
		}
	    }
	    if ( !found_boat && boat_needed)
	    {
		send_to_char( "You need a boat to go there.\n\r", ch );
		return;
	    }
	}

	if ( IS_SET( to_room->room_flags, ROOM_NO_MOB ) && IS_NPC( ch ) 
 	&& !IS_SET( ch->act, ACT_MOUNT ) && !IS_SET( ch->affected_by, AFF_CONTROL_MIND ) 
	&& !IS_SET(ch->act2, ACT2_WALKTO) && !IS_SET( ch->act, ACT_PET )
	&& !IS_SET(ch->act2, ACT2_GUIDE))
	    return;

	if ( IS_SET( to_room->room_flags, ROOM_MOUNT_ONLY ) 
	  &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
	  && (ch->mount == NULL || !IS_SET( ch->mount->affected_by, AFF_MOUNTED ) ) )
	{
	    send_to_char( "You must be mounted to go there.\n\r", ch );
	    return;
	}

	if ( IS_SET( to_room->room_flags, ROOM_NO_MOUNT ) 
	  &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
	  && (ch->mount != NULL && ch->mount->position >= POS_STANDING && ch->mount->in_room == ch->in_room ) )
	{
	    send_to_char( "You cannot bring a mount in there.\n\r", ch );
	    return;
	}

	/* Sector type will determine the moves- see movement chart */

	move = movement_loss[UMIN(SECT_MAX-1, in_room->sector_type)]
	     + movement_loss[UMIN(SECT_MAX-1, to_room->sector_type)]
	     ;

        move /= 2;  /* i.e. the average */

        if ( ch->mount != NULL && IS_SET( ch->mount->affected_by , AFF_MOUNTED ) )
	{
       	    if ( ch->mount->move < move*3/2 )
	    {
	        send_to_char( "Youre mount is too exhausted.\n\r", ch );
	        return;
	    }

	    ch->mount->move -= move*3/2;
	    move = 1;
	}

	if ( IS_NPC(ch) && ch->moveable != NULL )
	    move = 0;

    if ( IS_SET( ch->guilds, CLANNGUARD )
      || IS_SET( ch->guilds, TATH )
      || IS_SET( ch->guilds, MAR_TUUR ) )
        check_mv_improve( ch, move );

	if (IS_AFFECTED_2( ch, AFF_SLOW ) )
        	move *= 2; 

	if (IS_AFFECTED( ch, AFF_FLYING ) )
		move /= 2;
	else if (IS_AFFECTED( ch, AFF_LEVITATING ) )
		move = move * 2 / 3;
       
	// we are experimenting with zero movement loss while in town
	if ( to_room->sector_type == SECT_CITY )
	    move = 0;

	if ( ch->move < move )
	{
	    send_to_char( "You are too exhausted.\n\r", ch );
	    return;
	}

	WAIT_STATE( ch, 1 );
	ch->move -= move;

	if ( !IS_IMMORTAL( ch ) )
	    make_track( ch, pexit );   

	if ( ( to_room->sector_type != SECT_UNDERWATER && to_room->sector_type != SECT_OCEAN_BOTTOM )
	    && IS_AFFECTED( ch, AFF_HOLD_BREATH ) )
	{
            affect_strip( ch, gsn_hold_breath );
	    send_to_char( "You fill your lungs with fresh, clean air!\n\r", ch );
	}
    }

	send_to_char("`Y",ch);

        if (IS_SET(ch->plr, PLR_FLEE ) )
	        sprintf(buf, "Now which way did you go?");
	else if (pexit->mes_to_char[0] != '\0' )
            sprintf(buf,"%s",pexit->mes_to_char);
    	else if ( IS_AFFECTED_2( ch, AFF_FALLING ) )
		sprintf(buf, "You fall %s.", dir_name[door]);
        else if ( ch->mount != NULL && IS_SET( ch->mount->affected_by , AFF_MOUNTED ) )
            sprintf(buf,"You ride %s, mounted on %s.",dir_name[door],ch->mount->short_descr);
    	else if (IS_SET( ch->plr, PLR_RUNNING ) )
		sprintf(buf, "You sprint %s.", dir_name[door]);
	else if ( IS_AFFECTED( ch, AFF_LEVITATING ) )
            sprintf(buf,"You levitate %s.",dir_name[door]);
	else if ( IS_AFFECTED( ch, AFF_FLYING ) )
            sprintf(buf,"You fly %s.",dir_name[door]);
	else if ( ch->in_room->sector_type == SECT_UNDERWATER )
            sprintf(buf,"You swim %s.",dir_name[door]);
	else if ( ch->in_room->sector_type == SECT_OCEAN_BOTTOM )
	    sprintf(buf,"You swim %s along the ocean floor.",dir_name[door]);
	else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
            sprintf(buf,"You swim %s.",dir_name[door]);
	else if ( !found_boat && ch->in_room->sector_type == SECT_WATER_NOSWIM )
            sprintf(buf,"You swim %s.",dir_name[door]);
	else if ( IS_AFFECTED( ch, AFF_HASTE ) && !IS_AFFECTED_2( ch, AFF_SLOW) )
            sprintf(buf,"You cruise %s.",dir_name[door]);
	else if ( IS_AFFECTED_2( ch, AFF_SLOW ) && !IS_AFFECTED(ch, AFF_HASTE) )
            sprintf(buf,"You struggle %s.",dir_name[door]);
	else if (found_boat)
            sprintf(buf,"You paddle %s.",dir_name[door]);
        else if (IS_SET(ch->plr, PLR_FLEE ) )
	        sprintf(buf, "Now which way did you go?");
        else sprintf(buf,"You walk %s.",dir_name[door]); /* Default */
	buf[0] = UPPER(buf[0]);
	send_to_char(buf,ch);
	send_to_char("\n\r",ch);
	
  for ( fch = in_room->people; fch != NULL; fch = fch_next )
  {
    fch_next = fch->next_in_room;
    
    if (ch == fch || ch->mount == fch )
	continue;
    
    if (!can_see(fch, ch) || !IS_AWAKE(fch) )
	continue;

    if ( ch->mount != NULL && !IS_SET( ch->mount->affected_by, AFF_MOUNTED ) )
	continue;

    else if (IS_SET( ch->plr, PLR_FLEE ) )
	sprintf(buf, "%s flee's %s.", PERS(ch,fch) , dir_name[door] );
    else if (pexit->mes_to_room[0] != '\0' )
	sprintf(buf,"%s %s",PERS(ch, fch),pexit->mes_to_room);
    else if ( ch->mount != NULL && IS_SET( ch->mount->affected_by , AFF_MOUNTED )
	 && (can_see(fch,ch) || can_see(fch, ch->mount) ) )
	sprintf(buf,"%s rides %s, mounted on %s.",PERS(ch, fch),dir_name[door],ch->mount->short_descr);
    else if ( IS_AFFECTED(ch, AFF_SNEAK) )
	{
    	    if ( !IS_NPC(fch) && IS_SET(fch->plr, PLR_HOLYLIGHT) )
		sprintf(buf, "%s sneaks %s.", PERS(ch,fch) , dir_name[door]);
	    else
		buf[0] = '\0';
	}
    else if (IS_SET( ch->act2, ACT2_MOVEABLE_AREA ) && ch->moveable->move_msg[0] != '\0' )
	sprintf(buf,"%s `w%s %s.",ch->short_descr,ch->moveable->move_msg,dir_name[door]);
    else if ( IS_AFFECTED_2( ch, AFF_FALLING ) )
	sprintf(buf,"%s `wfalls %s.", PERS(ch, fch), dir_name[door] );
    else if ( IS_AFFECTED( ch, AFF_LEVITATING ) )
	sprintf(buf,"%s `wlevitates %s.", PERS(ch, fch), dir_name[door] );
    else if ( IS_AFFECTED( ch, AFF_FLYING ) )
	sprintf(buf,"%s `wflies %s.", PERS(ch, fch), dir_name[door] );
    else if ( ch->in_room->sector_type == SECT_UNDERWATER )
	sprintf(buf, "%s `wswims %s.", PERS(ch, fch) , dir_name[door] );
    else if ( ch->in_room->sector_type == SECT_OCEAN_BOTTOM )
	sprintf(buf, "%s `wswims %s along the ocean floor.", PERS(ch, fch), dir_name[door] );
    else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
	sprintf(buf, "%s `wswims %s.", PERS(ch, fch) , dir_name[door] );
    else if ( !found_boat && ch->in_room->sector_type == SECT_WATER_NOSWIM )
	sprintf(buf, "%s `wswims %s.", PERS(ch, fch) , dir_name[door] );
    else if ( IS_AFFECTED( ch, AFF_HASTE ) && !IS_AFFECTED_2( ch, AFF_SLOW) )
	sprintf(buf, "%s `wcruises %s.", PERS(ch,fch) , dir_name[door] );
    else if ( IS_AFFECTED_2( ch, AFF_SLOW ) && !IS_AFFECTED( ch, AFF_HASTE) )
	sprintf(buf, "%s `wstruggles %s.", PERS(ch,fch) , dir_name[door] );
    else if (IS_SET( ch->plr, PLR_RUNNING ) )
	sprintf(buf, "%s `wsprints %s.", PERS(ch,fch) , dir_name[door] );
    else if (IS_SET( ch->act2, ACT2_SHIP ) )
	sprintf(buf, "%s `wsails %s.", PERS(ch,fch), dir_name[door] );
    else if (found_boat)
	sprintf(buf, "%s `wpaddles %s.", PERS(ch,fch), dir_name[door] );
    else if (IS_NPC(ch) && ch->movement_msg[0] != '\0'  )
	sprintf(buf, "%s `w%s %s.", ch->short_descr,ch->movement_msg, dir_name[door]);
/* default movement is walks */
    else 
	sprintf(buf, "%s `wwalks %s.", PERS(ch,fch) , dir_name[door] );
    buf[0] = UPPER(buf[0]);
    if ( strlen(buf) > 2 )
    send_to_char(buf,fch);
    if ( strlen(buf) > 2 )
    send_to_char("\n\r",fch);
    trig = do_trigger( fch, ch, NULL, NULL, TRIG_LEAVES, TRIG_TYPE_MOB);
    }
    trig = do_trigger( ch->in_room, ch, NULL, NULL, TRIG_LEAVES, TRIG_TYPE_ROOM);

    send_to_char("`w",ch);
    char_from_room( ch );
    char_to_room( ch, to_room );
    send_to_char("`Y",ch);
 	
    if ( IS_SET(ch->affected_by, AFF_HIDE ) )
	REMOVE_BIT( ch->affected_by, AFF_HIDE );

	do_look(ch, "auto");

    for ( fch = to_room->people; fch != NULL; fch = fch_next )
    {

	fch_next = fch->next_in_room;

    	if (ch == fch || ch->mount == fch )
	continue;
    
    	if (!can_see(fch, ch) || !IS_AWAKE(fch) )
	continue;

    	if ( ch->mount != NULL && !IS_SET( ch->mount->affected_by, AFF_MOUNTED ) )
	break;
	
	if ( pexit->mes_at_room[0] != '\0' )
		sprintf(buf, "%s %s", PERS(ch,fch),pexit->mes_at_room );
        else if ( IS_AFFECTED(ch, AFF_SNEAK) )
	{
    	    if ( !IS_NPC(fch) && IS_SET(fch->plr, PLR_HOLYLIGHT) )
		sprintf(buf, "%s sneaks in from %s.", PERS(ch,fch) , dir_name_2[rev_dir[door]]);
	    else
		buf[0] = '\0';
	}
    	else if (IS_SET( ch->plr, PLR_FLEE ) )
		sprintf(buf, "%s `wflee's in from %s.", PERS(ch,fch) , dir_name_2[rev_dir[door]]);
    	else if ( IS_AFFECTED_2( ch, AFF_FALLING ) )
		sprintf(buf,"%s `wfalls in from %s.", PERS(ch, fch), dir_name_2[rev_dir[door]]);
    	else if ( ch->mount != NULL && (can_see(fch,ch) || can_see(fch, ch->mount) ) )
		sprintf(buf,"%s `whas arrived riding %s, mounted on %s`w.",PERS(ch, fch),dir_name[door],ch->mount->short_descr);
    	else if (IS_SET( ch->act2, ACT2_MOVEABLE_AREA ) && ch->moveable->move_msg[0] != '\0' )
            	sprintf(buf,"%s `w%s in from %s.",ch->short_descr,ch->moveable->move_msg,dir_name_2[rev_dir[door]]);
    	else if ( IS_AFFECTED( ch, AFF_LEVITATING ) )
		sprintf(buf,"%s `wlevitates in from %s.", PERS(ch, fch), dir_name_2[rev_dir[door]]);
    	else if ( IS_AFFECTED( ch, AFF_FLYING ) )
		sprintf(buf,"%s `wflies in from %s.", PERS(ch, fch), dir_name_2[rev_dir[door]]);
    	else if ( ch->in_room->sector_type == SECT_UNDERWATER )
		sprintf(buf, "%s `wswims in from %s.", PERS(ch, fch) , dir_name_2[rev_dir[door]]);
	else if ( ch->in_room->sector_type == SECT_OCEAN_BOTTOM )
		sprintf(buf, "%s `wswims in from %s.", PERS(ch, fch), dir_name_2[rev_dir[door]]);
    	else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
		sprintf(buf, "%s `wswims  in from %s.", PERS(ch, fch) , dir_name_2[rev_dir[door]]);
    	else if ( IS_AFFECTED( ch, AFF_HASTE ) && !IS_AFFECTED_2( ch, AFF_SLOW) )
		sprintf(buf, "%s `wcruises in from %s.", PERS(ch,fch) , dir_name_2[rev_dir[door]]);
    	else if ( IS_AFFECTED_2( ch, AFF_SLOW ) && !IS_AFFECTED( ch, AFF_HASTE) )
		sprintf(buf, "%s `wstruggles in from %s.", PERS(ch,fch) , dir_name_2[rev_dir[door]]);
    	else if (IS_SET( ch->plr, PLR_RUNNING ) )
		sprintf(buf, "%s `wsprints in from %s.", PERS(ch,fch) , dir_name_2[rev_dir[door]]);
    	else if (IS_SET( ch->act2, ACT2_SHIP ) )
		sprintf(buf, "%s `wsails in from %s.", PERS(ch,fch), dir_name_2[rev_dir[door]]);
    	else if (IS_NPC(ch) && ch->movement_msg[0] != '\0' ) 
		sprintf(buf, "%s `w%s in from %s.",ch->short_descr, ch->movement_msg, dir_name_2[rev_dir[door]]);
    	else if (found_boat)
		sprintf(buf, "%s `wpaddles in from %s.", PERS(ch,fch), dir_name_2[rev_dir[door]]);
	
/* default movement is walks */
  	  else 
		sprintf(buf, "%s `wwalks in from %s.", PERS(ch,fch) , dir_name_2[rev_dir[door]]);
  	  buf[0] = UPPER(buf[0]);
          if ( strlen(buf) > 2 )
  	  send_to_char(buf,fch);
          if ( strlen(buf) > 2 )
  	  send_to_char("`w\n\r",fch);
	trig = do_trigger( fch, ch, NULL, NULL, TRIG_ENTER, TRIG_TYPE_MOB);

    }
trig = do_trigger( ch->in_room, ch, NULL, NULL, TRIG_ENTER, TRIG_TYPE_ROOM);
	trig = do_trigger( ch, ch, NULL, NULL, TRIG_MOVES, TRIG_TYPE_MOB);

    if (in_room == to_room) /* no circular follows */
	return;

    if ( ch->mount != NULL && IS_SET( ch->mount->affected_by , AFF_MOUNTED ) )
    {
	if ( ch->mount->in_room != in_room )
	{
	    sprintf( buf, "do_move: Mounted rider (%s) not in room (%ld) with mount (%ld)\n\r",ch->name,in_room->vnum,ch->mount->in_room->vnum);
	    bug( buf, 0 );
	    REMOVE_BIT( ch->mount->affected_by, AFF_MOUNTED);
	}
	else
	{
            char_from_room( ch->mount );
            char_to_room( ch->mount, to_room );
	}
    }
	

    for ( fch = in_room->people; fch != NULL; fch = fch_next )
    {

	fch_next = fch->next_in_room;

	if ( fch->master == ch && IS_AFFECTED(fch,AFF_CONTROL_MIND) && !IS_SET(fch->act,ACT_MOUNT)
	&&   fch->position < POS_STANDING)
	    do_stand(fch,"");

	if ( fch->master == ch && fch->position == POS_STANDING )
	{
	    act( "You follow $N.", fch, NULL, ch, TO_CHAR );
	    move_char( fch, door, TRUE );
	}
    }

    if ( IS_SET(to_room->room_flags, ROOM_MOVE_UP)
      || IS_SET(to_room->room_flags, ROOM_MOVE_DOWN)
      || IS_SET(to_room->room_flags, ROOM_MOVE_NORTH)
      || IS_SET(to_room->room_flags, ROOM_MOVE_SOUTH)
      || IS_SET(to_room->room_flags, ROOM_MOVE_EAST)
      || IS_SET(to_room->room_flags, ROOM_MOVE_WEST))
		    if (!IS_IMMORTAL(ch)&&!IS_AFFECTED(ch,AFF_FLYING|AFF_LEVITATING)
			&& ( ch->mount == NULL || !IS_SET(ch->mount->affected_by,AFF_FLYING|AFF_LEVITATING) ) )
		    {
                        WAIT_STATE( ch, 3  * PULSE_PER_SECOND );
                        if ( IS_SET(to_room->room_flags, ROOM_MOVE_UP))
				move_char(ch,DIR_UP,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_DOWN))
				move_char(ch,DIR_DOWN,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_NORTH))
				move_char(ch,DIR_NORTH,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_SOUTH))
				move_char(ch,DIR_SOUTH,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_EAST))
				move_char(ch,DIR_EAST,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_WEST))
				move_char(ch,DIR_WEST,FALSE); 
			return;
		    }
		
		    if ( to_room->sector_type == SECT_AIR
			&& !IS_SET(ch->affected_by, AFF_FLYING)
			&& !IS_IMMORTAL(ch) )
		    {
			if ( !IS_SET(ch->affected_by, AFF_LEVITATE) )
			    SET_BIT(ch->affected_by_2, AFF_FALLING);

			send_to_char("You fall through the air!\n\r", ch);
			move_char(ch,DIR_DOWN,FALSE); 
			return;
		    }
    return;
}

void do_north( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_NORTH, FALSE );
    return;
}

void do_east( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_EAST, FALSE );
    return;
}

void do_south( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_SOUTH, FALSE );
    return;
}

void do_west( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_WEST, FALSE );
    return;
}

void do_up( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_UP, FALSE );
    return;
}

void do_down( CHAR_DATA *ch, char *argument )
{
    move_char( ch, DIR_DOWN, FALSE );
    return;
}

void do_hold_breath( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;

    if ( IS_AFFECTED( ch,AFF_BREATHE_UNDERWATER) || IS_AFFECTED( ch, AFF_HOLD_BREATH) || IS_NPC( ch ) )
	return;
    else
    {
        af.duration  = 4 + ( ch->level * URANGE( 1, get_skill( ch, gsn_hold_breath ), 100 ) / 200 );
        af.type      = gsn_hold_breath;
        af.level     = ch->level;
        af.location  = APPLY_AFFECT;
        af.modifier  = 0;
        af.bitvector = AFF_HOLD_BREATH;
	af.bit_type  = BIT_AFFECT;
        affect_to_char( ch, &af );
	check_improve( ch, gsn_hold_breath, TRUE, 1 );
        return;
    }
}

void do_mount( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char   arg[MAX_INPUT_LENGTH];
    int check;

    one_argument(argument,arg);

    if( ch->mount != NULL && IS_SET( ch->mount->affected_by , AFF_MOUNTED ) )
    {
        send_to_char("You are already mounted.\n\r",ch);
	return;
    }
 
    if ( arg[0] == '\0' )
    {
         send_to_char("Whom do you wish to mount?\n\r",ch);
	return;
    }
    
    if ( ( victim = get_char_room( ch , arg ) ) == NULL )
    {
         send_to_char("They are not here.\n\r",ch);
	return;
    }


   if ( !IS_SET(victim->act,ACT_MOUNT) )
   {
       act("You mount $N and ride off into the sunset.", ch, NULL, victim, TO_CHAR);
       act("$n mounts you and rides off into the sunset.", ch, NULL, victim, TO_VICT);
       act("$n mounts $N and rides off into the sunset.", ch, NULL, victim, TO_NOTVICT);
	return;
    }

   if ( ch->mount != victim )
    {
         send_to_char("That is not your mount.\n\r",ch);
	return;
    }

    check = ( IS_SET( victim->affected_by,AFF_FLYING|AFF_LEVITATING ) ? get_skill( ch, gsn_riding_air ) *3/4 : get_skill( ch, gsn_riding )*2);

    if ( number_percent( ) > UMIN( 95 , check ) )
    {
        act("While trying to mount $N, you fall off.", ch, NULL, victim, TO_CHAR );
        if ( number_percent( ) > UMIN( 97, check ) )
        {
            act("Due to poor handling, $N leaves you stranded.", ch, NULL, victim, TO_CHAR );
	    if ( ch->mount->master == ch )
            stop_follower( victim );
  	    check_improve(ch,(IS_AFFECTED(victim,AFF_FLYING|AFF_LEVITATING)?gsn_riding_air:gsn_riding),FALSE,2);
	}
  	check_improve(ch,(IS_AFFECTED(victim,AFF_FLYING|AFF_LEVITATING)?gsn_riding_air:gsn_riding),FALSE,2);
	return;
    }
       act("You mount $N.", ch, NULL, victim, TO_CHAR);
       act("$n mounts $N.", ch, NULL, victim, TO_NOTVICT);
    victim->affected_by=victim->affected_by|AFF_MOUNTED;
    check_improve(ch,(IS_AFFECTED(victim,AFF_FLYING|AFF_LEVITATING)?gsn_riding_air:gsn_riding),TRUE,2);
    if ( victim->master == NULL )
    add_follower( victim, ch);
    return;
}

void do_dismount( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if( ch->mount == NULL || !IS_SET( ch->mount->affected_by , AFF_MOUNTED ) )
    {
        send_to_char("You are not mounted.",ch);
	return;
    }
 
	victim = ch->mount;
    
       act("You dismount $N.", ch, NULL, victim, TO_CHAR);
       act("$n dismounts $N.", ch, NULL, victim, TO_NOTVICT);

    REMOVE_BIT( victim->affected_by , AFF_MOUNTED );
    return;
}



int find_door( CHAR_DATA *ch, char *arg )
{   char arg2[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    int door;
    arg = one_argument (arg,arg2); /* Command Arg2 Arg */
                                    /* open   door
                                       open   door north */

	 if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) || !str_cmp( arg2, "north" ) ) door = 0;
    else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) || !str_cmp( arg2, "east" ) ) door = 1;
    else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) || !str_cmp( arg2, "south" ) ) door = 2;
    else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) || !str_cmp( arg2, "west" ) ) door = 3;
    else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) || !str_cmp( arg2, "up" ) ) door = 4;
    else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) || !str_cmp( arg2, "down" ) ) door = 5;
    else
    {   if (arg[0] == '\0') arg = arg2;

	for ( door = 0; door <= 5; door++ )
	{
	    if ( ( pexit = ch->in_room->exit[door] ) != NULL
	    &&   IS_SET(pexit->exit_info, EX_ISDOOR)
	    &&   pexit->keyword[0] != '\0'
	    &&   is_name( arg2, pexit->keyword ) )
		return door;
	}
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( ( pexit = ch->in_room->exit[door] ) == NULL 
    || !can_find_door(ch,pexit->exit_info))
    {
	act( "I see no $T here.", ch, NULL, arg, TO_CHAR );
	return -1;
    }

    if ( !IS_SET(pexit->exit_info, EX_ISDOOR) )
    {
	send_to_char( "You can't do that.\n\r", ch );
	return -1;
    }

    return door;
}



void do_open( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;
    bool trig;
    CHAR_DATA *rch;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Open what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'open object' */
	if ( obj->cont_data == NULL )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->cont_data->flags, CONT_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( !IS_SET(obj->cont_data->flags, CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }
	if ( is_locked( obj->cont_data->lock ) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(obj->cont_data->flags, CONT_CLOSED);
	send_to_char( "Ok.\n\r", ch );
	act( "$n opens $p.", ch, obj, NULL, TO_ROOM );

	if ( obj->cont_data->trap != NULL && IS_SET( obj->cont_data->trap->flags, TRAP_OPEN ) )
	   trap_cast_spell( ch, obj->cont_data->trap );
	if ( obj->cont_data->inside != NULL )
	    hatch( obj );
	return;
    }

    if ( ( door = find_door( ch, argument ) ) >= 0 )
    {
	/* 'open door' */
	ROOM_INDEX_DATA *to_room;
	ROOM_INDEX_DATA *in_room=ch->in_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's already open.\n\r",      ch ); return; }
	if ( is_locked_door( ch->in_room, door ) )
	    { send_to_char( "It's locked.\n\r",            ch ); return; }

	REMOVE_BIT(pexit->exit_info, EX_CLOSED);
	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    trig = do_trigger( rch, ch, NULL, NULL, TRIG_DOOR_OPEN, TRIG_TYPE_MOB);
	}
	act( "$n opens the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

    if ( IS_SET(in_room->room_flags, ROOM_MOVE_UP)
      || IS_SET(in_room->room_flags, ROOM_MOVE_DOWN)
      || IS_SET(in_room->room_flags, ROOM_MOVE_NORTH)
      || IS_SET(in_room->room_flags, ROOM_MOVE_SOUTH)
      || IS_SET(in_room->room_flags, ROOM_MOVE_EAST)
      || IS_SET(in_room->room_flags, ROOM_MOVE_WEST))
	for ( rch = ch->in_room->people ; rch != NULL ; rch=rch->next_in_room )
	    if (!IS_IMMORTAL(rch)&&!IS_AFFECTED(rch,AFF_FLYING|AFF_LEVITATING)
		&& ( rch->mount == NULL || !IS_SET(rch->mount->affected_by,AFF_FLYING|AFF_LEVITATING) ) )
	    {
            	WAIT_STATE( ch, 3  * PULSE_PER_SECOND );
                if ( IS_SET(in_room->room_flags, ROOM_MOVE_UP))
		    move_char(rch,DIR_UP,FALSE); 
                else if ( IS_SET(in_room->room_flags, ROOM_MOVE_DOWN))
		    move_char(rch,DIR_DOWN,FALSE); 
                else if ( IS_SET(in_room->room_flags, ROOM_MOVE_NORTH))
		    move_char(rch,DIR_NORTH,FALSE); 
                else if ( IS_SET(in_room->room_flags, ROOM_MOVE_SOUTH))
		    move_char(rch,DIR_SOUTH,FALSE); 
                else if ( IS_SET(in_room->room_flags, ROOM_MOVE_EAST))
		    move_char(rch,DIR_EAST,FALSE); 
                else if ( IS_SET(in_room->room_flags, ROOM_MOVE_WEST))
		    move_char(rch,DIR_WEST,FALSE); 
		return;
	    }

	for ( rch = ch->in_room->people ; rch != NULL ; rch=rch->next_in_room )
	    if ( in_room->sector_type == SECT_AIR
		&& !IS_SET(rch->affected_by, AFF_FLYING)
		&& !IS_IMMORTAL(rch) )
	    {
		if ( !IS_SET(rch->affected_by, AFF_LEVITATE) )
		    SET_BIT(rch->affected_by_2, AFF_FALLING);

		send_to_char("You fall through the air!\n\r", rch);
		move_char(rch,DIR_DOWN,FALSE); 
		return;
	    }

	/* open the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != NULL
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    REMOVE_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
	    {
		act( "The $d opens.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	        trig = do_trigger( rch, ch, NULL, NULL, TRIG_DOOR_OPEN, TRIG_TYPE_MOB);
	    }

	if ( pexit->trap != NULL && IS_SET( pexit->trap->flags, TRAP_OPEN ) )
	   trap_cast_spell( ch, pexit->trap );
	}
    }

    return;
}



void do_close( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Close what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'close object' */
	if ( obj->cont_data == NULL )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( IS_SET(obj->cont_data->flags, CONT_CLOSED) )
	    { send_to_char( "It's already closed.\n\r",    ch ); return; }
	if ( !IS_SET(obj->cont_data->flags, CONT_CLOSEABLE) )
	    { send_to_char( "You can't do that.\n\r",      ch ); return; }

	if ( obj->cont_data->trap != NULL 
	  && IS_SET( obj->cont_data->trap->flags, TRAP_OPEN ) 
	  && IS_SET( obj->cont_data->trap->flags, TRAP_AUTO_RESET ) )
	    REMOVE_BIT( obj->cont_data->trap->flags, TRAP_SPRUNG );

	SET_BIT(obj->cont_data->flags, CONT_CLOSED);
	send_to_char( "Ok.\n\r", ch );
	act( "$n closes $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, argument ) ) >= 0 )
    {
	/* 'close door' */
	ROOM_INDEX_DATA *to_room;
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	    	{
		send_to_char( "It's already closed.\n\r",    ch );
		return; 
		}
	if ( IS_SET(pexit->exit_info, EX_BASHED) )
	    	{
		send_to_char( "It has been bashed open.\n\r",    ch );
		return; 
		}

	if ( pexit->trap != NULL 
	  && IS_SET( pexit->trap->flags, TRAP_OPEN ) 
	  && IS_SET( pexit->trap->flags, TRAP_AUTO_RESET ) )
	    REMOVE_BIT( pexit->trap->flags, TRAP_SPRUNG );

	SET_BIT(pexit->exit_info, EX_CLOSED);
	act( "$n closes the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	send_to_char( "Ok.\n\r", ch );

	/* close the other side */
	if ( ( to_room   = pexit->u1.to_room            ) != NULL
	&&   ( pexit_rev = to_room->exit[rev_dir[door]] ) != 0
	&&   pexit_rev->u1.to_room == ch->in_room )
	{
	    CHAR_DATA *rch;

	if ( pexit_rev->trap != NULL 
	  && IS_SET( pexit_rev->trap->flags, TRAP_OPEN ) 
	  && IS_SET( pexit_rev->trap->flags, TRAP_AUTO_RESET ) )
	    REMOVE_BIT( pexit_rev->trap->flags, TRAP_SPRUNG );

	    SET_BIT( pexit_rev->exit_info, EX_CLOSED );
	    for ( rch = to_room->people; rch != NULL; rch = rch->next_in_room )
		act( "The $d closes.", rch, NULL, pexit_rev->keyword, TO_CHAR );
	}
    }

    return;
}


void do_run( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *in_room;
    EXIT_DATA *pexit;
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_next;
    char arg[MAX_INPUT_LENGTH];
    int rooms;
    int direction;
    bool brief,d_hide,autoexit,automap;
    int num,dex;

    argument = one_argument(argument,arg);

    if ( arg[0] == '\0' )
    {
	send_to_char("Which direction do you want to run?\n\r",ch);
	return;
    }

    dex = get_curr_stat( ch, STAT_DEX );

    if ( *argument == '\0' )
	num = 100;
    else if ( is_number( argument ) )
	num = atoi( argument );
    else
    {
	send_to_char( "Invalid number of rooms.\n\r", ch );
	return;
    }

    if      ( !strncmp(arg,"north",strlen(arg) ) ) direction = 0; 
    else if ( !strncmp(arg,"east" ,strlen(arg) ) ) direction = 1;
    else if ( !strncmp(arg,"south",strlen(arg) ) ) direction = 2;
    else if ( !strncmp(arg,"west" ,strlen(arg) ) ) direction = 3;
    else if ( !strncmp(arg,"up"   ,strlen(arg) ) ) direction = 4;
    else if ( !strncmp(arg,"down" ,strlen(arg) ) ) direction = 5;
    else 
    {
	send_to_char("That is not a valid direction.\n\r",ch);
	return;
    }

    autoexit = ( IS_SET( ch->plr, PLR_AUTOEXIT ) ? TRUE : FALSE );
    automap = ( IS_SET( ch->plr, PLR_AUTOMAP ) ? TRUE : FALSE );
    brief = ( IS_SET( ch->comm, COMM_BRIEF ) ? TRUE : FALSE );
    d_hide = ( IS_SET( ch->affected_by , AFF_DETECT_HIDDEN ) ? TRUE : FALSE );
    SET_BIT( ch->comm, COMM_BRIEF );
    SET_BIT( ch->plr, PLR_RUNNING );
    REMOVE_BIT( ch->affected_by, AFF_DETECT_HIDDEN );
    REMOVE_BIT( ch->affected_by, AFF_SNEAK );
    REMOVE_BIT( ch->affected_by_2, AFF_FADE );
    REMOVE_BIT( ch->affected_by, AFF_HIDE );
    REMOVE_BIT( ch->plr, PLR_AUTOEXIT );
    REMOVE_BIT( ch->plr, PLR_AUTOMAP );
    sprintf(arg,"%s sprints off heading %s.\n\r",(IS_NPC(ch)?ch->short_descr:ch->name),dir_name[direction] );
    act(arg, ch, NULL, NULL, TO_ROOM); 
    in_room = NULL;
    for ( rooms = 0 ; rooms < get_curr_stat( ch, STAT_CON ) && ch->in_room != in_room ; rooms++ )
    {
	in_room = ch->in_room;

        if ( num <= rooms )
        {
	    dex += 2;
	    if ( number_range( 3, 30 ) <= dex - ( num / 2 ) )
	    {
		send_to_char( "You stop running.\n\r", ch );
	        if ( !brief ) REMOVE_BIT( ch->comm, COMM_BRIEF );
	        if ( d_hide ) SET_BIT( ch->affected_by, AFF_DETECT_HIDDEN );
	        if ( autoexit ) SET_BIT( ch->plr, PLR_AUTOEXIT );
	        if ( automap ) SET_BIT( ch->plr, PLR_AUTOMAP );
    	        REMOVE_BIT( ch->plr, PLR_RUNNING );
		return;
	    }
	    if ( num == rooms )
		send_to_char( "You over shoot your stop.\n\r", ch );
	}
	     
        if ( ( pexit   = in_room->exit[direction] ) == NULL
        ||   (  pexit->u1.to_room   ) == NULL )
        {
	    send_to_char( "You run into a wall, OOoch!\n\r", ch );
	    ch->hit = UMAX( -1, ch->hit - roll_dice(rooms + 1 ,2) );
	    update_pos( ch );
	    if ( !brief ) REMOVE_BIT( ch->comm, COMM_BRIEF );
	    if ( d_hide ) SET_BIT( ch->affected_by, AFF_DETECT_HIDDEN );
	    if ( autoexit ) SET_BIT( ch->plr, PLR_AUTOEXIT );
    	    REMOVE_BIT( ch->plr, PLR_RUNNING );
	    return;
        }

        if ( IS_SET(pexit->exit_info, EX_CLOSED)
          &&   !IS_AFFECTED(ch, AFF_DIMENSION_WALK) 
	  &&   !IS_SET(ch->plr, PLR_WIZPASS )  ) 
        {
	    if ( number_percent() < rooms * race_table[ ch->race ][ ch->sex ].size * (is_locked_door( ch->in_room, direction )?1:3) && !IS_SET(pexit->exit_info, EX_BASHPROOF ) )
      	    {
  		act( "You run into the $d, OOoch!", ch, NULL, pexit->keyword, TO_CHAR );
		send_to_char("And it breaks open.\n\r", ch );
	        ch->hit = UMAX( -1, ch->hit - roll_dice(rooms + 1,2) / 2 );
		if ( pexit->u1.to_room->exit[rev_dir[direction]]->u1.to_room == ch->in_room )
		{
		    for ( lock_list = pexit->u1.to_room->exit[rev_dir[direction]]->lock ; lock_list != NULL ; lock_list = lock_next )
		    {
			lock_next = lock_list->next;
	     	        REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED );
		    }
	     	    REMOVE_BIT( pexit->u1.to_room->exit[rev_dir[direction]]->exit_info, EX_CLOSED );
	     	    SET_BIT( pexit->u1.to_room->exit[rev_dir[direction]]->exit_info, EX_BASHED );
		}
		for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_next )
		{
		    lock_next = lock_list->next;
		    REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED );
		}
		REMOVE_BIT( pexit->exit_info, EX_CLOSED );
		SET_BIT( pexit->exit_info, EX_BASHED );
	    }
	    else if (!can_find_door(ch,pexit->exit_info))
            {
	        send_to_char( "You run into a wall, OOoch!\n\r", ch );
	        ch->hit = UMAX( -1, ch->hit - roll_dice(rooms + 1,2) );
            } 
            else
            {
  		act( "You run into the $d, OOoch!", ch, NULL, pexit->keyword, TO_CHAR );
	        ch->hit = UMAX( -1, ch->hit - roll_dice(rooms + 1,2) );
	    }
 	    if ( IS_IMMORTAL( ch ) )
	    {
		ch->hit = UMAX( 1, ch->hit );
	    }
            update_pos( ch );
	    if ( !brief ) REMOVE_BIT( ch->comm, COMM_BRIEF );
	    if ( d_hide ) SET_BIT( ch->affected_by, AFF_DETECT_HIDDEN );
	    if ( autoexit ) SET_BIT( ch->plr, PLR_AUTOEXIT );
	    if ( automap ) SET_BIT( ch->plr, PLR_AUTOMAP );
    	    REMOVE_BIT( ch->plr, PLR_RUNNING );
	    return;
        }

	if ( ch->in_room->exit[ direction ]->u1.to_room == ch->in_room )
	{
	    send_to_char("The gods stop you from running.\n\r",ch);
	    if ( !brief ) REMOVE_BIT( ch->comm, COMM_BRIEF );
	    if ( d_hide ) SET_BIT( ch->affected_by, AFF_DETECT_HIDDEN );
	    if ( autoexit ) SET_BIT( ch->plr, PLR_AUTOEXIT );
	    if ( automap ) SET_BIT( ch->plr, PLR_AUTOMAP );
    	    REMOVE_BIT( ch->plr, PLR_RUNNING );
	    return;
	}

        move_char( ch, direction, FALSE );
	ch->move = UMAX( 0, ch->move - 10 );
    }

    if ( rooms == get_curr_stat( ch, STAT_CON ) )
	send_to_char("You have run out of breath.\n\r",ch);

    if ( !brief ) REMOVE_BIT( ch->comm, COMM_BRIEF );
    if ( d_hide ) SET_BIT( ch->affected_by, AFF_DETECT_HIDDEN );
    if ( autoexit ) SET_BIT( ch->plr, PLR_AUTOEXIT );
    if ( automap ) SET_BIT( ch->plr, PLR_AUTOMAP );
    REMOVE_BIT( ch->plr, PLR_RUNNING );
    return;
}

void do_lock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_list2;
    LOCK_DATA *lock_next;
    LOCK_DATA *lock_next2;
    OBJ_DATA *key;
    int door;
    bool found=FALSE;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Lock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'lock object' */
	if ( obj->cont_data == NULL )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->cont_data->flags, CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->cont_data->lock == NULL  )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->cont_data->lock ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }

        for ( lock_list = obj->cont_data->lock ; lock_list != NULL ; lock_list = lock_next )
	{
	    lock_next = lock_list->next;
	    if ( IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		continue;
	    for ( key = ch->carrying ; key != NULL && key->pIndexData->vnum != lock_list->vnum ; key = key->next_content );
	    if ( key == NULL )
		continue;
	    found = TRUE;
	    SET_BIT( lock_list->lock_info, LOCK_LOCKED);
	    send_to_char( "*Click*\n\r", ch );
	}

	if ( !found )
	{
	    send_to_char("Sorry but you don't have any keys to lock it.\n\r",ch );
	    return;
	}

	if ( obj->cont_data->trap != NULL 
	  && IS_SET( obj->cont_data->trap->flags, TRAP_UNLOCK|TRAP_PICK ) 
	  && IS_SET( obj->cont_data->trap->flags, TRAP_AUTO_RESET ) )
	    REMOVE_BIT( obj->cont_data->trap->flags, TRAP_SPRUNG );

	act( "$n locks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, argument ) ) >= 0 )
    {
	/* 'lock door' */
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	pexit_rev = pexit->u1.to_room->exit[ rev_dir[ door ] ];

	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->lock == NULL )
	    { send_to_char( "It can't be locked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->lock) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }

        for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_next )
	{
	    lock_next = lock_list->next;
	    if ( IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		continue;
	    for ( key = ch->carrying ; key != NULL && key->pIndexData->vnum != lock_list->vnum ; key = key->next_content );
	    if ( key == NULL )
		continue;
	    found = TRUE;
	    if ( is_door( pexit_rev ) && pexit_rev->u1.to_room == ch->in_room )
	    for ( lock_list2 = pexit_rev->lock; lock_list2 != NULL; lock_list2 = lock_next2 )
	    {
		lock_next2 = lock_list2->next;
		if ( lock_list->vnum == lock_list2->vnum )
		    SET_BIT( lock_list2->lock_info, LOCK_LOCKED );
	    }
	    SET_BIT( lock_list->lock_info, LOCK_LOCKED);
	    send_to_char( "*Click*\n\r", ch );
	}

	if ( !found )
	{
	    send_to_char("Sorry but you don't have any keys to lock it.\n\r",ch );
	    return;
	}

	if ( pexit->trap != NULL 
	  && IS_SET( pexit->trap->flags, TRAP_UNLOCK|TRAP_PICK ) 
	  && IS_SET( pexit->trap->flags, TRAP_AUTO_RESET ) )
	    REMOVE_BIT( pexit->trap->flags, TRAP_SPRUNG );

	act( "$n locks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	return;
    }

    return;
}


void do_unlock( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_list2;
    LOCK_DATA *lock_next;
    LOCK_DATA *lock_next2;
    OBJ_DATA *key;
    int door;
    bool found=FALSE;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Unlock what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'lock object' */
	if ( obj->cont_data == NULL )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->cont_data->flags, CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->cont_data->lock == NULL  )
	    { send_to_char( "It can't be unlocked.\n\r",     ch ); return; }
	if ( !has_key( ch, obj->cont_data->lock ) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }

        for ( lock_list = obj->cont_data->lock ; lock_list != NULL ; lock_list = lock_next )
	{
	    lock_next = lock_list->next;
	    if ( !IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		continue;
	    for ( key = ch->carrying ; key != NULL && key->pIndexData->vnum != lock_list->vnum ; key = key->next_content );
	    if ( key == NULL )
		continue;
	    found = TRUE;
	    REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED);
	    send_to_char( "*Click*\n\r", ch );
	}

	if ( !found )
	{
	    send_to_char("Sorry but you don't have any keys to unlock it.\n\r",ch );
	    return;
	}
	if ( obj->cont_data->trap != NULL && IS_SET( obj->cont_data->trap->flags, TRAP_OPEN ) )
	   trap_cast_spell( ch, obj->cont_data->trap );
	act( "$n unlocks $p.", ch, obj, NULL, TO_ROOM );
	return;
    }

    if ( ( door = find_door( ch, argument ) ) >= 0 )
    {
	/* 'unlock door' */
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit	= ch->in_room->exit[door];
	pexit_rev = pexit->u1.to_room->exit[ rev_dir[ door ] ];

	if ( !IS_SET(pexit->exit_info, EX_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->lock == NULL )
	    { send_to_char( "It can't be unlocked.\n\r",     ch ); return; }
	if ( !has_key( ch, pexit->lock) )
	    { send_to_char( "You lack the key.\n\r",       ch ); return; }
	if ( !is_locked_door( ch->in_room, door ) )
	    { send_to_char( "It is not locked.\n\r",       ch ); return; }

        for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_next )
	{
	    lock_next = lock_list->next;
	    if ( !IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		continue;
	    for ( key = ch->carrying ; key != NULL && key->pIndexData->vnum != lock_list->vnum ; key = key->next_content );
	    if ( key == NULL )
		continue;
	    found = TRUE;
	    if ( is_door( pexit_rev ) && pexit_rev->u1.to_room == ch->in_room )
	    for ( lock_list2 = pexit_rev->lock; lock_list2 != NULL; lock_list2 = lock_next2 )
	    {
		lock_next2 = lock_list2->next;
		if ( lock_list->vnum == lock_list2->vnum )
		    REMOVE_BIT( lock_list2->lock_info, LOCK_LOCKED );
	    }
	    REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED);
	    send_to_char( "*Click*\n\r", ch );
	}

	if ( !found )
	{
	    send_to_char("Sorry but you don't have any keys to unlock it.\n\r",ch );
	    return;
	}
	act( "$n unlocks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	if ( pexit->trap != NULL && IS_SET( pexit->trap->flags, TRAP_OPEN ) )
	   trap_cast_spell( ch, pexit->trap );
	return;
    }

    return;
}



void do_pick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;
    OBJ_DATA *obj;
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_list2;
    LOCK_DATA *lock_next;
    LOCK_DATA *lock_next2;
    char arg[MAX_INPUT_LENGTH];
    int door;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Pick what?\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_pick_lock].beats );

    /* look for guards */
    for ( gch = ch->in_room->people; gch; gch = gch->next_in_room )
    {
	if ( IS_NPC(gch) && IS_AWAKE(gch) && ch->level + 5 < gch->level )
	{
	    act( "$N is standing too close to the lock.",
		ch, NULL, gch, TO_CHAR );
	    return;
	}
    }


    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	/* 'pick object' */
	if ( obj->cont_data == NULL )
	    { send_to_char( "That's not a container.\n\r", ch ); return; }
	if ( !IS_SET(obj->cont_data->flags, CONT_CLOSED) )
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( obj->cont_data->lock == NULL )
	    { send_to_char( "It can't be unlocked.\n\r",   ch ); return; }
	if ( !is_locked( obj->cont_data->lock ) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	for ( lock_list = obj->cont_data->lock ; lock_list != NULL ; lock_list = lock_next )
	{
	    lock_next = lock_list->next;
	    if ( !IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		continue;
	    if ( !IS_NPC(ch) && number_percent( ) > get_skill( ch, gsn_pick_lock ) - lock_list->pick_penalty )
	    {
		send_to_char( "You failed.\n\r", ch);
		check_improve(ch,gsn_pick_lock,FALSE,2);
		continue;
	    }
	    if ( IS_SET( lock_list->lock_info, LOCK_PICKPROOF) )
	        { send_to_char( "You failed.\n\r",             ch ); continue; }
	    REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED);
	    send_to_char( "*Click*\n\r", ch );
    	    check_improve(ch,gsn_pick_lock,TRUE,2);
	}

	act( "$n picks $p.", ch, obj, NULL, TO_ROOM );
	if ( obj->cont_data->trap != NULL && IS_SET( obj->cont_data->trap->flags, TRAP_OPEN ) )
	   trap_cast_spell( ch, obj->cont_data->trap );
	return;
    }

    if ( ( door = find_door( ch, argument ) ) >= 0 )
    {
	/* 'pick door' */
	EXIT_DATA *pexit;
	EXIT_DATA *pexit_rev;

	pexit = ch->in_room->exit[door];
	pexit_rev = pexit->u1.to_room->exit[ rev_dir[ door ] ];
	if ( !IS_SET(pexit->exit_info, EX_CLOSED) && !IS_IMMORTAL(ch))
	    { send_to_char( "It's not closed.\n\r",        ch ); return; }
	if ( pexit->lock == NULL && !IS_IMMORTAL(ch))
	    { send_to_char( "It can't be picked.\n\r",     ch ); return; }
	if ( !is_locked_door( ch->in_room, door ) )
	    { send_to_char( "It's already unlocked.\n\r",  ch ); return; }

	for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_next )
	{
	    lock_next = lock_list->next;
	    if ( !IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		continue;
	    if ( IS_SET( lock_list->lock_info, LOCK_PICKPROOF) && !IS_IMMORTAL(ch))
	        { send_to_char( "You failed.\n\r",             ch ); continue; }
	    if ( !IS_NPC(ch) && number_percent( ) > get_skill( ch, gsn_pick_lock ) )
	    {
		send_to_char( "You failed.\n\r", ch);
		check_improve(ch,gsn_pick_lock,FALSE,2);
		continue;
	    }
	    if ( is_door( pexit_rev ) && pexit_rev->u1.to_room == ch->in_room )
	    for ( lock_list2 = pexit_rev->lock ; lock_list2 != NULL ; lock_list2 = lock_next2 )
	    {
		lock_next2 = lock_list2->next;
	        if ( lock_list2->vnum == lock_list->vnum )
		    REMOVE_BIT( lock_list2->lock_info, LOCK_LOCKED );
	    }
	    REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED);
	    send_to_char( "*Click*\n\r", ch );
	    check_improve(ch,gsn_pick_lock,TRUE,2);
        }
	act( "$n picks the $d.", ch, NULL, pexit->keyword, TO_ROOM );
	if ( pexit->trap != NULL && IS_SET( pexit->trap->flags, TRAP_OPEN ) )
	   trap_cast_spell( ch, pexit->trap );
    }

    return;
}



void do_stand( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	if ( IS_AFFECTED(ch, AFF_SLEEP) )
	{ 
	    send_to_char( "You wake slowly, struggle to rise to your feet, then fall back into a deep slumber.!\n\r", ch ); 
	    return; 
	}

    	if (IS_SET(ch->dream_flags, DREAM_IS_NIGHTMARE ) )
    	{ 
	    send_to_char( "You can't tear yourself away from this Nightmare!\n\r", ch ); 
	    return; 
    	}

	send_to_char( "You wake and adjust to your surroundings.\n\r", ch );
	act( "$n wakes from a dreamless sleep.", ch, NULL, NULL, TO_ROOM );
	set_position( ch, POS_STANDING );
	break;

    case POS_RESTING: case POS_SITTING:
	send_to_char( "You climb to your feet.\n\r", ch );
	act( "$n climbs to $s feet.", ch, NULL, NULL, TO_ROOM );
	set_position( ch, POS_STANDING );
	break;

    case POS_STANDING:
	send_to_char( "You are on your feet at the moment.\n\r", ch );
	break;

    case POS_FIGHTING:
	send_to_char( "You stop fighting and stand back.\n\r", ch );
	act( "$n stops fighting and stands back.", ch, NULL, NULL, TO_ROOM );
	stop_fighting( ch, FALSE );
	set_position( ch, POS_STANDING );
	break;
    }

    return;
}



void do_rest( CHAR_DATA *ch, char *argument )
{
    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You wake up, wipe the sleep from your eyes, and sit there tiredly.\n\r", ch );
	act ("$n wakes up, wipes the sleep from $s eyes, and sits their tiredly.",ch,NULL,NULL,TO_ROOM);
	set_position( ch, POS_RESTING );
	break;


    case POS_RESTING:
	send_to_char( "You are currently comfortable.\n\r", ch );
	break;

    case POS_STANDING:
	send_to_char( "You rest your weary bones.\n\r", ch );
	act( "$n rests $s weary bones.", ch, NULL, NULL, TO_ROOM );
	set_position( ch, POS_RESTING );
	break;

    case POS_SITTING:
	send_to_char("You sit more relaxed, resting your knotted muscles.\n\r",ch);
	act("$n sits more relaxed, resting $s knotted muscles.",ch,NULL,NULL,TO_ROOM);
	set_position( ch, POS_RESTING );
	break;

    case POS_FIGHTING:
	send_to_char( "You stop fighting and rest your weary bones.\n\r", ch );
	act( "$n stops fighting and rest $s weary bones.", ch, NULL, NULL, TO_ROOM );
	stop_fighting( ch, FALSE );
	set_position( ch, POS_RESTING );
	break;
    }


    return;
}


void do_sit (CHAR_DATA *ch, char *argument )
{
    switch (ch->position)
    {
	case POS_SLEEPING:
	    send_to_char("You wake and find a comfortable sitting position.\n\r",ch);
	    act("$n wakes and sits comfortably.\n\r",ch,NULL,NULL,TO_ROOM);
	    set_position( ch, POS_SITTING );
	    break;
 	case POS_RESTING:
	    send_to_char("You straighten up and sit with better posture.\n\r",ch);
	    set_position( ch, POS_SITTING );
	    break;
	case POS_SITTING:
	    send_to_char("You are already sitting comfortably.\n\r",ch);
	    break;
	case POS_FIGHTING:
  	    send_to_char( "You stop fighting and find a suitable place to sit.\n\r", ch );
	    act( "$n stops fighting and finds a suitable place to sit.", ch, NULL, NULL, TO_ROOM );
	    stop_fighting( ch, FALSE );
	    set_position( ch, POS_SITTING );
	    break;
	case POS_STANDING:
	    send_to_char("You find a suitable place to sit.\n\r",ch);
	    act("$n finds a suitable place to sit.\n\r",ch,NULL,NULL,TO_ROOM);
	    set_position( ch, POS_SITTING );
	    break;
    }
    return;
}


void do_sleep( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA * rch;

    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	if ( rch->fighting != NULL )
	{
	    send_to_char( "You cannot sleep with a battle going on around you!", ch );
	    return;
	}

    switch ( ch->position )
    {
    case POS_SLEEPING:
	send_to_char( "You are currently dreaming of falling into a deep slumber.\n\r", ch );
	break;

    case POS_RESTING:
    case POS_SITTING:
    case POS_STANDING: 
	send_to_char( "You lie down and begin counting sheep.\n\r", ch );
	act( "$n finds a comfortable place to lie down, and soon falls asleep.", ch, NULL, NULL, TO_ROOM );
	set_position( ch, POS_SLEEPING );
	break;

    case POS_FIGHTING:
	send_to_char( "You are in battle!\n\r", ch );
	break;
    }

    return;
}


void do_wake( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if (IS_SET(ch->dream_flags, DREAM_IS_NIGHTMARE ) )
    { 
	send_to_char( "You can't tear yourself away from this Nightmare!\n\r", ch ); 
	return; 
    }

    if ( arg[0] == '\0' )
    {
	do_stand( ch, argument );
    	ch->dream_flags = 0;
    	ch->dream_count = 0;
        return;
    }

    if ( !IS_AWAKE(ch) )
	{ send_to_char( "They aren't there?  Wait, is this a nightmare?\n\r",       ch ); return; }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "You vainly search for someone to wake, but fail miserably\n\r",              ch ); return; }

    if ( IS_AWAKE(victim) )
	{ act( "You begin to shake $M, but soon realize that they are already awake.", ch, NULL, victim, TO_CHAR ); return; }

    if (IS_SET(victim->dream_flags, DREAM_IS_NIGHTMARE ) )
        { send_to_char( "You struggle to wake them but their eyes will not open!\n\r", ch ); return; }

    if ( IS_AFFECTED(victim, AFF_SLEEP) )
	{ act( "$N refuses to awaken!",   ch, NULL, victim, TO_CHAR );  return; }

    set_position( victim, POS_STANDING );
    ch->dream_flags = 0;
    ch->dream_count = 0;
    act( "You shake $N, and $E wakes slowly.", ch, NULL, victim, TO_CHAR );
    act( "You awaken to someone shaking you gently.  You open your eyes and see $n staring at you.", ch, NULL, victim, TO_VICT );
    return;
}

void do_sneak( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    int brightness , fail_chance=0 ;
    send_to_char( "You attempt to move silently.\n\r", ch );
    affect_strip( ch, gsn_sneak );

   brightness = get_light_adjustment( ch, ch->in_room ) ;

   fail_chance = brightness/2;

   if (IS_AFFECTED( ch, AFF_SANCTUARY))
     fail_chance +=80 ;

   if (IS_AFFECTED( ch, AFF_FAERIE_FIRE))
     fail_chance +=60 ;

    if ( IS_NPC(ch) || number_percent( ) < ( get_skill( ch,gsn_sneak ) - fail_chance ) )
    {
	check_improve(ch,gsn_sneak,TRUE,3);
	af.type      = gsn_sneak;
	af.level     = ch->level; 
	af.duration  = ch->level;
	af.location  = APPLY_AFFECT;
	af.modifier  = 0;
	af.bitvector = AFF_SNEAK;
	af.bit_type  = BIT_AFFECT;
	affect_to_char( ch, &af );
    }
    else
	check_improve(ch,gsn_sneak,FALSE,3);

    return;
}



void do_hide( CHAR_DATA *ch, char *argument )
{
int brightness, fail_chance=0 ;

   brightness = get_light_adjustment( ch, ch->in_room ) ;
    fail_chance= brightness/10 ;

    if (IS_AFFECTED (ch, AFF_SANCTUARY))
     fail_chance +=80 ;

    if (IS_AFFECTED( ch, AFF_FAERIE_FIRE))
      fail_chance+=60 ;

    if ( IS_NPC(ch) && ( IS_AFFECTED(ch, AFF_HIDE)
	|| IS_AFFECTED_2(ch, AFF_FADE) ) )
	return;

    if ( IS_AFFECTED_2(ch, AFF_FADE) )
	REMOVE_BIT(ch->affected_by_2, AFF_FADE);

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

    if ( number_percent( ) < ( get_skill( ch,gsn_hide)-fail_chance) )
    {
	SET_BIT(ch->affected_by, AFF_HIDE);
	check_improve(ch,gsn_hide,TRUE,3);
    	send_to_char( "You attempt to find a place to hide.\n\r", ch );
    }
    else
	{
	check_improve(ch,gsn_hide,FALSE,3);
    	send_to_char( "Your attempt to find a place to hide.\n\r", ch );
	}

    return;
}

void do_fade( CHAR_DATA *ch, char *argument )
{
int brightness, fail_chance=0 ;

   brightness = get_light_adjustment( ch, ch->in_room ) ;
    fail_chance= brightness/10 ;

   if (IS_AFFECTED (ch, AFF_SANCTUARY))
      fail_chance +=80 ;

   if (IS_AFFECTED( ch, AFF_FAERIE_FIRE))
      fail_chance+=60 ;

   if ( IS_NPC(ch) && ( IS_AFFECTED_2(ch, AFF_FADE)
	|| IS_AFFECTED(ch, AFF_HIDE) ) )
	return;

    if ( IS_AFFECTED(ch, AFF_HIDE) )
	REMOVE_BIT(ch->affected_by, AFF_HIDE);

   if ( IS_AFFECTED_2(ch, AFF_FADE) )
	REMOVE_BIT(ch->affected_by_2, AFF_FADE);

   if ( number_percent( ) <( get_skill( ch,gsn_fade)-fail_chance) )
   {
	SET_BIT(ch->affected_by_2, AFF_FADE);
	check_improve(ch,gsn_fade,TRUE,3);
    	send_to_char( "You attempt to fade into your surroundings succeeds.\n\r", ch );
   }
   else
   {
	check_improve(ch,gsn_fade,FALSE,3);
    	send_to_char( "Your attempt to fade into your surroundings.\n\r", ch );
   }

   return;
}



/*
 * Contributed by Alander.
 */
void do_visible( CHAR_DATA *ch, char *argument )
{
    affect_strip ( ch, gsn_shadowform			);
    affect_strip ( ch, gsn_mass_shadowform		);
    affect_strip ( ch, gsn_sneak			);
    REMOVE_BIT   ( ch->affected_by, AFF_HIDE		);
    REMOVE_BIT   ( ch->affected_by_2, AFF_FADE		);
    REMOVE_BIT   ( ch->affected_by, AFF_SHADOWFORM	);
    REMOVE_BIT   ( ch->affected_by, AFF_SNEAK		);
    if (!IS_NPC(ch) )
    {
    if ( ch->pcdata != NULL )
    ch->affected_by=ch->affected_by|ch->pcdata->perm_aff;
    ch->affected_by_2=ch->affected_by_2|ch->pcdata->perm_aff_2;
    }
    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_recall( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    ROOM_INDEX_DATA *location;
    MOB_INDEX_DATA *guide = NULL;
    CHAR_DATA *mob;
    bool found = FALSE;

    if (IS_NPC(ch) && !IS_SET(ch->act2, ACT2_GUIDE) && !IS_SET(ch->act,ACT_PET) && !IS_SET(ch->act,ACT_MOUNT))
    {
	send_to_char("Only players can recall.\n\r",ch);
	return;
    }

    if ( ch->level >= 10 && !IS_IMMORTAL(ch) && !IS_NPC( ch ) ) 
    {
	send_to_char("Did you remember your Return to Safety scrolls?\n\r", ch );
	return;
    }

    act( "$n prays for transportation!", ch, 0, 0, TO_ROOM );
    location = NULL;
    if ( !IS_NPC( ch ) && ch->pcdata->pcclan == NULL )
    {
        if ( ( location = get_room_index( race_table[ ch->race ][ ch->sex ].begin ) ) == NULL )
	    location = get_room_index( 2 );
    } 
    else if ( !IS_NPC( ch ) )
    {
    	location = get_room_index( ch->pcdata->pcclan->clan->recall_room );
        if ( location == NULL )
	    if ( ( location = get_room_index( race_table[ ch->race ][ ch->sex ].begin ) ) == NULL )
	        location = get_room_index( 2 );
    }
    else if ( IS_NPC( ch ) )
	if ( ( location = get_room_index( race_table[ ch->race ][ ch->sex ].begin ) ) == NULL )
		location = get_room_index( 2 );

    if ( ch->in_room == location || location == NULL )
	return;

/* for some reason in_room was NULL.. how can this be??? */

    if ( ch->in_room == NULL )
    {
	char_to_room( ch, location );
	act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
	do_look( ch, "auto" );
	bug("No room on recall!",0);
   	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "Some force prevents you!\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) != NULL )
    {
	int lose,skill;

	 skill = 40 + ch->level;

	if ( number_percent() < 80 * skill / 100 )
	{
	    WAIT_STATE( ch, 4 );
	    sprintf( buf, "You failed!.\n\r");
	    send_to_char( buf, ch );
	    return;
	}

	lose = (ch->desc != NULL) ? 25 : 50;
	gain_exp( ch, 0 - lose );
	sprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
	send_to_char( buf, ch );
	stop_fighting( ch, TRUE );
	
    }

    ch->move /= 2;
    act( "$n disappears.", ch, NULL, NULL, TO_ROOM );
    if ( ch->inside_area != NULL )
	free_inside_area_data(ch->inside_area);

    ch->inside_area = NULL;	
    char_from_room( ch );
    char_to_room( ch, location );
    act( "$n appears in the room.", ch, NULL, NULL, TO_ROOM );
    do_look( ch, "auto" );
    if (ch->mount != NULL )
    {
        if ( IS_SET(ch->mount->affected_by,AFF_MOUNTED) )
    	{
            char_from_room( ch->mount );
            char_to_room( ch->mount, location );
	}
	else
	    do_recall( ch->mount, "" );
    }

    if (ch->pet != NULL)
	do_recall(ch->pet,"");
    else if ( !IS_NPC( ch ) && ch->pet == NULL && ch->level < 10 ) 
    {
	if ( location->area->world == 0 )
	{
	    found = TRUE;
            guide = get_mob_index( 16559 );
	}
        else if ( location->area->world == 1 )
	{
	    found = TRUE;
            guide = get_mob_index( 1519 );
	}
        else if ( location->area->world == 2 )
	{
	    found = TRUE;
            guide = get_mob_index( 11125 );
	}
	else
	    found = FALSE;

        if ( found && guide )
        {
            send_to_char( "A familiar appears to help you get started(help familiar)!\n\r", ch );
            mob = create_mobile( guide );
            char_to_room( mob, ch->in_room );
            add_follower( mob, ch);
            add_follower( ch, mob);
            mob->leader = ch;
            ch->pet = mob;
        }
    }

    return;
}

void do_train( CHAR_DATA *ch, char *argument )
{
    char buf1[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *mob;
    sh_int stat = - 1;
    int type = -1;
    char *pOutput = NULL;
    int cost=12;

    if ( IS_NPC(ch) )
	return;

    /*
     * Check for trainer.
     */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
	if ( IS_NPC(mob) && IS_SET(mob->act, ACT_TRAIN) )
	    break;
    }

    if ( mob == NULL )
    {
	send_to_char( "You can't do that here.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	sprintf( buf, "You have %d practices.\n\r", ch->practice );
	send_to_char( buf, ch );
	argument = "foo";
    }

    if ( !str_cmp( argument, "str" ) )
    {
	stat        = STAT_STR;
	pOutput     = "strength";
    }

    else if ( !str_cmp( argument, "int" ) )
    {
	stat	    = STAT_INT;
	pOutput     = "intelligence";
    }

    else if ( !str_cmp( argument, "wis" ) )
    {
	stat	    = STAT_WIS;
	pOutput     = "wisdom";
    }

    else if ( !str_cmp( argument, "dex" ) )
    {
	stat  	    = STAT_DEX;
	pOutput     = "dexterity";
    }

    else if ( !str_cmp( argument, "con" ) )
    {
	stat	    = STAT_CON;
	pOutput     = "constitution";
    }

    else if ( !str_cmp( argument, "chr" ) )
    {
	stat	    = STAT_CHR;
	pOutput     = "charisma";
    }

    else if ( !str_cmp(argument, "hp" ) )
	cost = 3;

    else if ( ( type = get_element_type( argument ) ) != -1 )
	cost = 3;

    else
    {
	strcpy( buf, "You can train:" );
	if ( ch->perm_stat[STAT_STR] < get_max_train(ch,STAT_STR)) 
	{
	    sprintf( buf1, " str( %d )", train_cost_table[ ch->perm_stat[ STAT_STR ] + 1 ][ STAT_STR ] );
	    strcat( buf, buf1 );
	}
	if ( ch->perm_stat[STAT_INT] < get_max_train(ch,STAT_INT))  
	{
	    sprintf( buf1, " int( %d )", train_cost_table[ ch->perm_stat[ STAT_INT ] + 1 ][ STAT_INT ] );
	    strcat( buf, buf1 );
	}
	if ( ch->perm_stat[STAT_WIS] < get_max_train(ch,STAT_WIS)) 
	{
	    sprintf( buf1, " wis( %d )", train_cost_table[ ch->perm_stat[ STAT_WIS ] + 1 ][ STAT_WIS ] );
	    strcat( buf, buf1 );
	}
	if ( ch->perm_stat[STAT_DEX] < get_max_train(ch,STAT_DEX))  
	{
	    sprintf( buf1, " dex( %d )", train_cost_table[ ch->perm_stat[ STAT_DEX ] + 1 ][ STAT_DEX ] );
	    strcat( buf, buf1 );
	}
	if ( ch->perm_stat[STAT_CON] < get_max_train(ch,STAT_CON))  
	{
	    sprintf( buf1, " con( %d )", train_cost_table[ ch->perm_stat[ STAT_CON ] + 1 ][ STAT_CON ] );
	    strcat( buf, buf1 );
	}
	if ( ch->perm_stat[STAT_CHR] < get_max_train(ch,STAT_CHR))  
	{
	    sprintf( buf1, " chr( %d )", train_cost_table[ ch->perm_stat[ STAT_CHR ] + 1 ][ STAT_CHR ] );
	    strcat( buf, buf1 );
	}

	strcat( buf, "\n\rhp( 3 )  `Rfire( 3 )`Cwater( 3 )  `Gearth( 3 )  `Bwind( 3 ) `Wspirit( 3 )\n\r");
	send_to_char( buf, ch );
	return;
    }

    if ( stat != -1 )
	cost = train_cost_table[ ch->perm_stat[ stat ] + 1 ][ stat ];

    if ( !str_cmp( "hp", argument ) )
    {
    	if ( cost > ch->practice )
    	{
       	    send_to_char( "You don't have enough practices.\n\r", ch );
            return;
        }

	ch->practice -= cost;
        ch->pcdata->perm_hit += 10;
        ch->max_hit += 10;
        ch->hit +=10;
        act( "Your durability increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's durability increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }
 
    if ( type != -1 )
    {
        if (cost > ch->practice )
        {
            send_to_char("You don't have enough practices.\n\r", ch);
            return;
        }

	ch->practice -= cost;
        ch->pcdata->perm_mana[ type ] += 30;
        ch->max_mana[ type ] += 30;
        ch->mana[ type ] += 30;
        act( "Your power increases!",ch,NULL,NULL,TO_CHAR);
        act( "$n's power increases!",ch,NULL,NULL,TO_ROOM);
        return;
    }

    if ( ch->perm_stat[stat]  >= get_max_train(ch,stat) )
    {
	act( "Your $T is already at maximum.", ch, NULL, pOutput, TO_CHAR );
	return;
    }

    if ( cost > ch->practice  )
    {
	send_to_char( "You don't have enough practices.\n\r", ch );
	return;
    }

    ch->practice		-= cost;
  
    ch->perm_stat[stat]		+= 1;
    act( "Your $T increases!", ch, NULL, pOutput, TO_CHAR );
    act( "$n's $T increases!", ch, NULL, pOutput, TO_ROOM );
    return;
}

void do_gohome (CHAR_DATA *ch, char *argument)
{
    ROOM_INDEX_DATA * home;

    if ( !IS_IMMORTAL( ch ) && ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) ) )
    {
	send_to_char( "God has forsaken you.\n\r", ch );
	return;
    }

    if ( ch->pcdata == NULL || ch->home < 1 || ( home = get_room_index(ch->home) ) == NULL ) 
    {
       send_to_char ("You don't have a home, your just a homeless mudder.\n\r",ch);
       return;
    }

    if ( ch->in_room == home )
    {
	send_to_char( "You are already there.\n\r", ch );
	return;
    }
 
    if ( !IS_IMMORTAL( ch ) )
    {
	if ( ch->in_room->area->world != home->area->world )
	{
	    send_to_char( "You must be in the same world as your home.\n\r", ch );
	    return;
	}

	if ( ch->pcdata->home_timer < 5 )
	{
	    send_to_char( "You failed.\n\r", ch );
	    return;
	}


	 ch->move /= 2;
    }

    ch->pcdata->home_timer = 0;

    if (strlen(ch->pcdata->bamfout) != 0)
        cact (ch->pcdata->bamfout,ch,NULL,ch,TO_ROOM,"`b");
     else
    {
        cact ("$n vanishes.\n\r",ch,NULL,ch,TO_ROOM,"`b");
        cact ("You think about home.\n\rThe world shimmers, and you are there.\n\r",ch,NULL,NULL,TO_CHAR,"`b");
    }

    if ( ch->inside_area != NULL )
	free_inside_area_data(ch->inside_area);
	ch->inside_area = NULL;	

    char_from_room(ch);
    char_to_room(ch,get_room_index(ch->home));
    do_look( ch, "auto" );
    return; 
} 
           
sh_int get_direction_number (const char *dir)
{               
  if ( (!str_cmp (dir,"n")) || (!str_cmp(dir,"north"))) return 0;
  if ( (!str_cmp (dir,"e")) || (!str_cmp(dir,"east")))  return 1;
  if ( (!str_cmp (dir,"s")) || (!str_cmp(dir,"south"))) return 2;
  if ( (!str_cmp (dir,"w")) || (!str_cmp(dir,"west")))  return 3;
  if ( (!str_cmp (dir,"u")) || (!str_cmp(dir,"up")))    return 4;
  if ( (!str_cmp (dir,"d")) || (!str_cmp(dir,"down")))  return 5;
  return -1;
}

void do_enter (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  char buf[MAX_STRING_LENGTH];
  ROOM_INDEX_DATA * in_room;
  ROOM_INDEX_DATA * to_room;
  CHAR_DATA *fch;
  CHAR_DATA *fch_next;
  OBJ_DATA *obj;
  bool trig;
  
  one_argument (argument,arg);
  if (arg[0] == '\0')
     { send_to_char ("What do you wish to enter?\n\r",ch);
       return;
     }
  if ((obj = get_obj_list (ch, arg, ch->in_room->contents)) == NULL)
    {   act ("There is no $T here to enter.",ch,NULL,arg,TO_CHAR);
        return; 
    }
  if (IS_PCG(obj)) 
      { 
	trig = do_trigger( ch->in_room, ch, NULL, NULL, TRIG_LEAVES, TRIG_TYPE_ROOM);
	if ( obj->gate_data == NULL )
	{
       		send_to_char ("Some strange force prevents you!\n\r",ch);
		sprintf(buf,"BAD Gate for obj %ld",obj->pIndexData->vnum);
		bug(buf,0);
       		return;
	}
	if (!can_enter_gate(ch, obj) )
	{
       		send_to_char ("Some strange force prevents you!\n\r",ch);
       		return;
	}
        to_room = gate_leads_to( obj->gate_data );
	if ( to_room == NULL )
	{
        act ("$p fades away as $n touches it.",ch,obj,NULL,TO_ROOM);	
        act ("$p fades away as you touch it.",ch,obj,NULL,TO_ROOM);	
	extract_obj(obj);
	return;
	}
	
        act ("$n enters $T.",ch,NULL,obj->short_descr,TO_ROOM);	
        act ("You enter $T.",ch,NULL,obj->short_descr,TO_CHAR);	
	in_room = ch->in_room;
        char_to_obj (ch,obj); 
        do_look (ch,"auto");
        act ("$n steps into the room.",ch,NULL,obj->short_descr,TO_ROOM);	

        for ( fch = in_room->people; fch != NULL; fch = fch_next )
        {   

	    fch_next = fch->next_in_room;

	    if ( fch->master == ch && IS_AFFECTED(fch,AFF_CONTROL_MIND) && !IS_SET(fch->act,ACT_MOUNT)
	    &&   fch->position < POS_STANDING)
	        do_stand(fch,"");

  	    if ( fch->master == ch && fch->position == POS_STANDING )
	    {

	        act( "You follow $N.", fch, NULL, ch, TO_CHAR );
	        do_enter( fch, obj->name );
	    }
        }
	if ( to_room == NULL )
	{
	bug("Bad to_room entering obj",0);
	return;
	}
	if ( to_room->sector_type == SECT_UNDERWATER || to_room->sector_type == SECT_OCEAN_BOTTOM )
	{
	    if ( IS_NPC(ch) && !IS_SET( ch->affected_by, AFF_BREATHE_UNDERWATER ) )
		return;
	    if ( !IS_IMMORTAL(ch)
	      &&   !IS_SET( ch->plr2, PLR_SPECIAL_PASS )
                && ( ch->mount != NULL 
                   && !IS_SET( ch->mount->affected_by , AFF_BREATHE_UNDERWATER ) ))
	    {
		if ( IS_SET( ch->mount->affected_by, AFF_MOUNTED) )
		{		
	    	    send_to_char( "Your mount will not move, because it doesn't want to drown.\n\r", ch );
	 	    return;
	        }
		else
		{
		    send_to_char( "You mount will not go into the water\n\r",ch);
		    set_position( ch, POS_RESTING );
		}
	    }
	    if ( !IS_IMMORTAL(ch) && !IS_AFFECTED( ch, AFF_BREATHE_UNDERWATER ) && !IS_AFFECTED(ch, AFF_HOLD_BREATH) )
	    {
		send_to_char( "You take a large breath of air before you go underwater.\n\r", ch );
		do_hold_breath( ch, "");
	    }
	}

    if ( IS_SET(to_room->room_flags, ROOM_MOVE_UP)
      || IS_SET(to_room->room_flags, ROOM_MOVE_DOWN)
      || IS_SET(to_room->room_flags, ROOM_MOVE_NORTH)
      || IS_SET(to_room->room_flags, ROOM_MOVE_SOUTH)
      || IS_SET(to_room->room_flags, ROOM_MOVE_EAST)
      || IS_SET(to_room->room_flags, ROOM_MOVE_WEST))
		    if (!IS_IMMORTAL(ch)&&!IS_AFFECTED(ch,AFF_FLYING|AFF_LEVITATING)
			&& ( ch->mount == NULL || !IS_SET(ch->mount->affected_by,AFF_FLYING|AFF_LEVITATING) ) )
		    {
                        WAIT_STATE( ch, 3  * PULSE_PER_SECOND );
                        if ( IS_SET(to_room->room_flags, ROOM_MOVE_UP))
				move_char(ch,DIR_UP,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_DOWN))
				move_char(ch,DIR_DOWN,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_NORTH))
				move_char(ch,DIR_NORTH,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_SOUTH))
				move_char(ch,DIR_SOUTH,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_EAST))
				move_char(ch,DIR_EAST,FALSE); 
                        else if ( IS_SET(to_room->room_flags, ROOM_MOVE_WEST))
				move_char(ch,DIR_WEST,FALSE); 
			return;
		    }

		    if ( to_room->sector_type == SECT_AIR
			&& !IS_SET(ch->affected_by, AFF_FLYING)
			&& !IS_IMMORTAL(ch) )
		    {
			if ( !IS_SET(ch->affected_by, AFF_LEVITATE) )
			    SET_BIT(ch->affected_by_2, AFF_FALLING);

			send_to_char("You fall through the air!\n\r", ch);
			move_char(ch,DIR_DOWN,FALSE); 
			return;
		    }
    }
     else
     { 
       send_to_char ("You wont' fit.\n\r",ch);
       return;
     }
}

void do_board (CHAR_DATA *ch, char *argument)
{
  char arg[MAX_STRING_LENGTH];
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *location;
    char buf[MAX_INPUT_LENGTH];
  
  one_argument (argument,arg);
  if (arg[0] == '\0')
     { send_to_char ("What do you wish to board?\n\r",ch);
       return;
     }
  if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{ send_to_char( "They aren't here.\n\r",              ch ); return; }

  if (IS_SET(victim->act2, ACT2_MOVEABLE_AREA))
	{
    	if ( ( location = get_room_index( victim->moveable->area_start_room ) ) == NULL )
    	{
		send_to_char( "You can't board that now\n\r", ch );
		return;
    	}
        act ("$n boards $T",ch,NULL,victim->short_descr,TO_ROOM);	
	sprintf(buf,"You board %s\n\r", victim->short_descr);
        send_to_char (buf,ch);
	char_from_room(ch);
	char_to_room (ch,location);
        act ("$n boards $T",ch,NULL,victim->short_descr,TO_ROOM);	
	if ( ch->inside_area == NULL )
	ch->inside_area = inside_area_alloc();
	strncpy( ch->inside_area->mob_name, victim->name, 30 );
	strncpy( ch->inside_area->short_descr, victim->short_descr, 80 );
 	ch->inside_area->start_room = victim->moveable->area_start_room; 
        do_look (ch,"");
	return;
	}
  send_to_char( "You can't board that!\n\r", ch );
  return;
}
void do_exit (CHAR_DATA *ch, char *argument)
{ 
  CHAR_DATA *victim;
  ROOM_INDEX_DATA *pRoom;
  char buf[80];
  ROOM_INDEX_DATA *location;
  SPECPRO_LIST *s; 

  for ( s = ch->in_room->spro;s != NULL; s = s->next ) /* possible moveable area */
   {
   if ( s->sp->type == 'X' ) /* moveable-- char should be able to leave the area */
   { 
    if ( ch->inside_area == NULL ) /* oops! Summon, Or just starting the game.. This may be NULL But we need to set */
	{
	  for ( victim = char_list; victim != NULL; victim = victim->next )
	  {
	   if ( victim->moveable != NULL && victim->moveable->area_start_room != 0 ) /* This is a possible moveable area we need to check */
	     {
	       pRoom = get_room_index( victim->moveable->area_start_room );
	       if ( ch->in_room->area == pRoom->area ) /* If so, we must have the right moveable area */
		{
		ch->inside_area = inside_area_alloc();
		strncpy( ch->inside_area->mob_name, victim->name, 30 );
		strncpy( ch->inside_area->short_descr, victim->short_descr, 80 );
 		ch->inside_area->start_room = victim->moveable->area_start_room; 
		break;
		}
	     } 
	  }
	}
    if ( ( location = find_location(ch, ch->inside_area->mob_name ) ) == NULL )
     {
        send_to_char ("Yikes!  Your lost now!  Call for help!",ch); /* This had better never happen */
	return;
     }

	act ("$n leaves $T",ch,NULL,ch->inside_area->short_descr,TO_ROOM);	
	sprintf(buf,"You climb down from %s\n\r",ch->inside_area->short_descr);
	send_to_char (buf,ch);
	char_from_room(ch);
	char_to_room (ch,location);
        act ("$n leaves $T",ch,NULL,ch->inside_area->short_descr,TO_ROOM);	
	free_inside_area_data(ch->inside_area); /* Wont need till they board again */
	ch->inside_area = NULL;
        do_look (ch,"");
	return;
    }    
   }
  if (ch->inside_area != NULL )
   {
     sprintf(buf,"You can't leave %s from here",ch->inside_area->short_descr);
     send_to_char(buf,ch);
     return;
   }
  return;
}

void make_track( CHAR_DATA * ch, EXIT_DATA * pexit )
{
    static long track_time [ SECT_MAX ] =
	{
	    /* 300 = 1 minute */

	    300, 600, 1200, 700, 600, 300, 300, 300, 200, 900,
	    500, 500, 400, 1500, 300, 

	    300, 300, 300, 300, 300, 300, 300,
	    300, 300, 300, 300, 300, 300, 300, 300, 300, 300
	};
    static int track_penalty [ SECT_MAX ] =
	{
	    0, 0, -15, 0, 0, 0, 0, 0, 0, -40,
	    0, 0, 0, -25, 0,

	    0, 0, 0, 0, 0, 0, 0,
	    0, 0, 0, 0, 0, 0, 0, 0, 0, 0

	};
    TRACK_DATA * track;

    track = track_alloc();
    track->ch = ch;
    track->track_penalty = track_penalty[ ch->in_room->sector_type ];
    track->time = track_time[ ch->in_room->sector_type ] * ( ch->size + 1 );
    track->time *= number_range( 50, 150 );
    track->track_penalty -= get_skill( ch, gsn_counter_tracking ) / 3;
    check_improve(ch,gsn_counter_tracking,TRUE,1);
    if ( ( ch->mount != NULL && ch->in_room == ch->mount->in_room && IS_AFFECTED( ch->mount, AFF_MOUNTED ) && !IS_AFFECTED( ch->mount, AFF_FLYING|AFF_LEVITATING ) )
      || ( IS_AFFECTED( ch, AFF_MOUNTED ) && !IS_AFFECTED( ch, AFF_FLYING|AFF_LEVITATING ) ) )
    {
	track->time *= 2;
	track->track_penalty -= 25;
    }
    else if ( IS_AFFECTED( ch, AFF_FLYING|AFF_LEVITATING )
	  || ( ch->mount != NULL && ch->mount->in_room == ch->in_room && IS_AFFECTED( ch->mount, AFF_MOUNTED ) && IS_AFFECTED( ch->mount, AFF_FLYING|AFF_LEVITATING ) ) )
    {
	track->time /= 3;
	track->track_penalty += 20;
    }
    if ( IS_SET( ch->in_room->room_flags, ROOM_INDOORS ) )
    {
	track->time = track->time * 2 / 3;
    }
    if ( IS_NPC( ch ) )
	track->time /= 3;
    track->time /= 100;
    track->time += current_time;
    track->ctime = current_time;
    track->next = pexit->tracks;
    pexit->tracks = track;
}

/* Created by Quin :) */
void next_track(char *arg, int spaces , TRACK_TYPE *tracks ) 
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

    /* Check if victim is in room, and if it is sentinel */
    /* Sentinel mobs don't make tracks, thus cannot be tracked or hunted */
    for( victim = in_room->people;
        ( victim !=  NULL ) 
        ; victim = victim->next_in_room)
    if ( !IS_SET( victim->act, ACT_SENTINEL) && is_name( arg, victim->name )
	&& ( !IS_IMMORTAL( victim ) || !IS_SET( victim->plr, PLR_WIZINVIS ) ) )
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
    tracks->next =   ( tracking = track_type_alloc() );
    tracking->last = tracks;
    tracking->next = NULL;
    tracks->dir = ( tracking->dir = 6 );

    for ( direction=0 ; direction < 6 ; direction++ )
    {
        if ( in_room->exit[ direction ] != NULL
           && ( to_room = in_room->exit[ direction ]->u1.to_room ) !=NULL
	   && to_room != in_room
	   && ( to_room->area == in_room->area ))
        {
      tracking->dir = 6;
      tracks->dir = direction;
      tracking->room = to_room;
            for( back_track=tracks->last ; ( ( back_track != NULL ) && back_track->room != to_room );
                back_track=back_track->last ) ;  
	    if ( back_track == NULL || back_track->room!=to_room )
                next_track( arg,spaces - UMAX( 1, track_loss[ in_room->sector_type ] ) , tracking );
     	    if ( tracking->dir != 6 )
	    {
    		free_track_type(tracking);
                return;
	    }
        }
    }

    /* didn't find prey */
    free_track_type(tracking);
    tracks->dir = 6;
    tracks->next = NULL;
    return;
}

/* Track Skill by Quin :) */
void do_track( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int track_list[ 6 ] = { 0, 0, 0, 0, 0, 0 };
    ROOM_INDEX_DATA * room = ch->in_room;
    EXIT_DATA * pexit;
    CHAR_DATA * vch;
    TRACK_DATA * tracks;
    int skill;
    int door;
    int current=-1;
    int count;
    long time=0;
    bool found=FALSE;
    bool fPass;

    /* get argument and see if it contains anything */
    one_argument( argument, arg );
    if ( arg[ 0 ] == '\0' )
    {
	send_to_char( "What do you want to track?\n\r", ch );
	return;
    }

    if ( ( skill = get_skill( ch, gsn_track ) ) <= 0 )
    {
	send_to_char( "You found no tracks.\n\r", ch );
	return;
    }

    skill += ( IS_SET( ch->affected_by_2, AFF_SENSES ) ? ch->level : 0 );

    /* Take the time to look for tracks */
    WAIT_STATE( ch, ( 3 - ( ch->level / 15 ) ) * PULSE_PER_SECOND );

    for ( door = 0 ; door < 6 ; door++ )
    {
	pexit = room->exit[ door ];

	if ( pexit == NULL || pexit->tracks == NULL )
	    continue;

	count = 0;

	for ( tracks = pexit->tracks ; tracks != NULL ; tracks = tracks->next )
	{
	    count++;
	    for ( vch = char_list ; vch != NULL && vch != tracks->ch ; vch = vch->next );

	    if ( IS_IMMORTAL(ch) )
		fPass = TRUE;
	    else
		fPass = skill - tracks->track_penalty - ( count * 5 ) > number_percent();

	    if ( vch == NULL 
	      || ( !is_name( arg, vch->name ) && fPass )
	      || ( is_name( arg, vch->name ) && !fPass ) )
		continue;

	    found = TRUE;
	    track_list[ door ]++;

	    if ( IS_IMMORTAL(ch) )
		fPass = TRUE;
	    else
		fPass = skill / 2 - tracks->track_penalty - ( count * 5 ) > number_percent();

	    if ( tracks->ctime > time && fPass )
	    {
		time = tracks->ctime;
		current = door;
	    }
	}
    }

    if ( !found )
    {
	send_to_char( "You found no tracks.\n\r", ch );
        check_improve(ch,gsn_track,FALSE,1);
	return;
    }

    buf[0] = '\0';

    for ( door = 0 ; door < 6 ; door++ )
    {
	if ( track_list[ door ] < 1 )
	    continue;

	if ( track_list[ door ] == 1 )
	    sprintf( buf1, "You notice tracks leading %s.", dir_name[ door ] );
	else
	    sprintf( buf1, "You notice %d tracks leading %s.", track_list[ door ], dir_name[ door ] );

	strcat( buf, buf1 );

	if ( current == door )
	    strcat( buf, " (most current)\n\r" );
	else
	    strcat( buf, "\n\r" );
    }

    send_to_char( buf, ch );
    check_improve(ch,gsn_track,TRUE,1);

    return;
}

void do_hunt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * room = ch->in_room;
    EXIT_DATA * pexit;
    CHAR_DATA * vch;
    TRACK_DATA * tracks;
    int skill;
    int door;
    int current=-1;
    int count;
    long time=0;
    bool found=FALSE;
    bool fPass;

    if ( ch->desc == NULL || ch->desc->inbuf[0] != '\0' )
    {
	ch->hunting[0] = '\0';
	return;
    }

    if ( ( skill = get_skill( ch, gsn_hunt ) ) <= 0 )
    {
        send_to_char( "You found no tracks.\n\r", ch );
	ch->hunting[0] = '\0';
        return;
    }

    skill += ( IS_SET( ch->affected_by_2, AFF_SENSES ) ? ch->level : 0 );

    /* get argument and see if it contains anything */
    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	if ( ch->hunting[0] == '\0' )
	{
            send_to_char("What do you want to hunt?\n\r", ch );
            return;
	}
	else
	    strncpy( arg, ch->hunting,30 );
    }
    else
    {
	strncpy(ch->hunting, arg,30);
    }

    if ( get_char_room( ch, arg ) )
    {
	send_to_char( "You are in the same room as it.\n\r", ch );
	ch->hunting[0] = '\0';
	return;
    }

    /* Take the time to look for tracks */
    WAIT_STATE( ch, 3 * PULSE_PER_SECOND );

    for ( door = 0 ; door < 6 ; door++ )
    {
	pexit = room->exit[ door ];

	if ( pexit == NULL || pexit->tracks == NULL )
	    continue;

	count = 0;

	for ( tracks = pexit->tracks ; tracks != NULL ; tracks = tracks->next )
	{
	    count++;
	    for ( vch = char_list ; vch != NULL && vch != tracks->ch ; vch = vch->next );

	    if ( IS_IMMORTAL(ch) )
		fPass = TRUE;
	    else
	        fPass = skill - tracks->track_penalty - ( count * 5 ) > number_percent();

	    if ( vch == NULL 
	      || ( !is_name( arg, vch->name ) && fPass )
	      || ( is_name( arg, vch->name ) && !fPass ) )
		continue;

	    found = TRUE;
	    if ( IS_IMMORTAL(ch) )
		fPass = TRUE;
	    else
		fPass = skill / 2 - tracks->track_penalty - ( count * 5 ) > number_percent();

	    if ( tracks->ctime > time && fPass )
	    {
		time = tracks->ctime;
		current = door;
	    }
	}
    }

    if ( !found )
    {
	send_to_char( "You found no tracks.\n\r", ch );
        check_improve(ch,gsn_hunt,FALSE,1);
        ch->hunting[0] =  '\0';
	return;
    }

    if ( current == -1 )
    {
	send_to_char( "You cannot tell which tracks are most current.\n\r", ch );
        check_improve(ch,gsn_hunt,FALSE,1);
        ch->hunting[0] = '\0';
	return;
    }

    act( "You follow tracks leading $T!", ch, NULL, dir_name[ current ], TO_CHAR);
    check_improve(ch,gsn_hunt,TRUE,1);
    move_char( ch, current, TRUE );	    
    if ( room == ch->in_room )
    {
        ch->hunting[0] = '\0';
        return;
    }

    return;
}

void do_steer( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *in_room;
    ROOM_INDEX_DATA *pRoom;
    EXIT_DATA *pexit;
    int door;

    if (  argument[0] == '\0' )
    {
	send_to_char( "Stear in what direction?\n\r", ch );
	return;
    }

         if ( !str_cmp( argument, "n" ) || !str_cmp( argument, "north" ) ) door = 0;
    else if ( !str_cmp( argument, "e" ) || !str_cmp( argument, "east"  ) ) door = 1;
    else if ( !str_cmp( argument, "s" ) || !str_cmp( argument, "south" ) ) door = 2;
    else if ( !str_cmp( argument, "w" ) || !str_cmp( argument, "west"  ) ) door = 3;
    else if ( !str_cmp( argument, "u" ) || !str_cmp( argument, "up"    ) ) door = 4;
    else if ( !str_cmp( argument, "d" ) || !str_cmp( argument, "down"  ) ) door = 5;
    else
    {
	send_to_char( "You can't steer that way!\n\r", ch );
	return;
    }
	
    if ( ch->inside_area == NULL ) /* oops! Summon, Or just starting the game.. This may be NULL But we need to set */
	{
	  for ( victim = char_list; victim != NULL; victim = victim->next )
	  {
	   if ( victim->moveable != NULL && victim->moveable->area_start_room != 0 ) /* This is a possible moveable area we need to check */
	     {
	       pRoom = get_room_index( victim->moveable->area_start_room );
	       if ( ch->in_room->area == pRoom->area ) /* If so, we must have the right moveable area */
		{
		ch->inside_area = inside_area_alloc();
		strncpy( ch->inside_area->mob_name, victim->name, 30 );
		strncpy( ch->inside_area->short_descr, victim->short_descr, 80 );
 		ch->inside_area->start_room = victim->moveable->area_start_room; 
		break;
		}
	     } 
	  }
	}
	victim = NULL;
	if (ch->inside_area == NULL ) /* This should never happen */
    	{
		send_to_char( "This is not your day is it?\n\r", ch );
		return;
    	}

    for ( victim = char_list; victim != NULL ; victim = victim->next )
    {
	if ( is_name( ch->inside_area->mob_name, victim->name ) )
	    break;
    }
	if (victim == NULL ) /* This should never happen */
    	{
		send_to_char( "And the world turns backwards!\n\r", ch );
		return;
    	}
	
    in_room = victim->in_room;
    if ( ( pexit   = in_room->exit[door] ) == NULL
    ||   ( to_room = pexit->u1.to_room   ) == NULL 
    ||	 !can_see_room(ch,pexit->u1.to_room) )
    {
	send_to_char( "You can't steer that way!\n\r", ch );
	act( "$n is steering wildly!", ch, NULL, NULL, TO_ROOM );
	return;
    }
	
    if ( victim->valid_sector[pexit->u1.to_room->sector_type] == FALSE )
    {
	sprintf(buf,"%s isn't allowed that way\n\r",victim->short_descr);
	send_to_char( buf, ch );
	act( "$n is steering wildly!", ch, NULL, NULL, TO_ROOM );
	return;
    }
 
    sprintf(buf, "You steer %s %s\n\r",victim->short_descr, dir_name[door]);
    send_to_char(buf,ch);
    sprintf(buf, "%s steers %s %s",IS_NPC(ch) ? ch->short_descr : ch->name, victim->short_descr, dir_name[door]);
    act( buf, ch, NULL, NULL, TO_ROOM );

    move_char(victim,door,FALSE); 

    in_room = ch->in_room;
    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
    {
	rch->in_room = victim->in_room;
	do_look( rch, "auto" );
	rch->in_room = in_room;
    }

    return;
}

bool can_enter_gate(CHAR_DATA *ch, OBJ_DATA *obj)
{
   OBJ_DATA *to_obj;
   CHAR_DATA *to_mob;
   ROOM_INDEX_DATA *to_room;

   if ( IS_SET( obj->gate_data->flags, GATE_TO_OBJ ) )
   {
   if ( ( to_obj = (OBJ_DATA *)obj->gate_data->leads_to ) == NULL )
	return FALSE;
   if ( to_obj->in_room == NULL )
	return FALSE;
   if ( to_obj->in_room->area->visible > ch->level )
	return FALSE;
   if ( IS_SET( to_obj->in_room->room_flags_2, ROOM_STARTING ) 
      && !IS_SET(ch->in_room->room_flags_2, ROOM_STARTING ) )
	return FALSE;
   if ( IS_SET( to_obj->in_room->room_flags, ROOM_SAFE ) )
	return FALSE;
   if ( IS_SET( to_obj->in_room->room_flags, ROOM_PRIVATE ) )
	return FALSE;
   if ( IS_SET( to_obj->in_room->room_flags, ROOM_NO_RECALL ) )
	return FALSE;
   return TRUE;
   }
   if ( IS_SET( obj->gate_data->flags, GATE_TO_MOB ) )
   {
   if ( ( to_mob = (CHAR_DATA *)obj->gate_data->leads_to) == NULL )
	return FALSE;
   if ( to_mob->in_room == NULL )
	return FALSE;
   if ( to_mob->in_room->area->visible > ch->level )
	return FALSE;
   if ( IS_SET( to_mob->in_room->room_flags_2, ROOM_STARTING ) )
	return FALSE;
   if ( IS_SET( to_mob->in_room->room_flags, ROOM_SAFE ) )
	return FALSE;
   if ( IS_SET( to_mob->in_room->room_flags, ROOM_PRIVATE ) )
	return FALSE;
   if ( IS_SET( to_mob->in_room->room_flags, ROOM_NO_RECALL ) )
	return FALSE;
   return TRUE;
   }
   if ( ( to_room = get_room_index( obj->gate_data->vnum  ) ) == NULL )
	return FALSE;
   if ( to_room == NULL )
	return FALSE;
   if ( to_room->area->visible > ch->level )
	return FALSE;
   if ( IS_SET( to_room->room_flags_2, ROOM_STARTING ) )
	return FALSE;
   if ( IS_SET( to_room->room_flags, ROOM_SAFE ) )
	return FALSE;
   if ( IS_SET( to_room->room_flags, ROOM_PRIVATE ) )
	return FALSE;
   if ( IS_SET( to_room->room_flags, ROOM_NO_RECALL ) )
	return FALSE;
   return TRUE;

	
}
/***************************************************************************
 *   File: graph.c                                       Part of CircleMUD *
 *  Usage: various graph algorithms                                        *
 *                                                                         *
 *  All rights reserved.  See license.doc for complete information.        *
 *                                                                         *
 *  Copyright (C) 1993 by the Trustees of the Johns Hopkins University     *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 ***************************************************************************/


#define TRACK_THROUGH_DOORS 

/*
 * You can define or not define TRACK_THOUGH_DOORS, above, depending on
 *  whether or not you want track to find paths which lead through closed
 *  or hidden doors.
 */
BFS_QUEUE *queue_head = NULL, *queue_tail = NULL;

BFS_ROOM *list_head = NULL, *list_tail = NULL;

/* Utility macros */
#define MARK( room )          ( SET_BIT( ( room )->room_flags_2, ROOM_BFS_MARK))
#define UNMARK( room )        ( REMOVE_BIT( ( room )->room_flags_2, ROOM_BFS_MARK))
#define IS_MARKED( room )     ( IS_SET( ( room )->room_flags_2, ROOM_BFS_MARK))
#define TOROOM( room, y )     ( ( room )->exit[ ( y ) ]->u1.to_room )
#define IS_CLOSED( room, y )  ( IS_SET( ( room )->exit[( y )]->exit_info, \
                                EX_CLOSED))

#ifdef TRACK_THROUGH_DOORS
#define VALID_EDGE( room, y )                                              \
                              ( ( room )->exit[( y )] &&                   \
			      ( TOROOM(( room ), ( y ) ) != NULL ) &&   \
			      ( !IS_MARKED( TOROOM( ( room ), ( y ) ) ) ) )
#else
#define VALID_EDGE( room , y )                                             \
                              ( ( room )->exit[ ( y ) ] &&                 \
			      ( TOROOM( ( room ), ( y ) ) != NULL ) &&     \
                              ( !IS_CLOSED( (room), ( y ) ) ) &&           \
                              ( !IS_MARKED( TOROOM( ( room ), ( y ) ) ) ) )
#endif

void list_enqueue( ROOM_INDEX_DATA *room )
{
   BFS_ROOM *curr;

   curr = alloc_bfs_room();
   curr->room = room;
   curr->next = NULL;

   if( list_tail != NULL )
   {
      list_tail->next = curr;
      list_tail = curr;
   }
   else
      list_head = list_tail = curr;

   return;
}

void bfs_enqueue( ROOM_INDEX_DATA *room, sh_int dir, sh_int depth )
{
   BFS_QUEUE * curr;

   curr = alloc_bfs_queue();
   curr->room = room;
   curr->dir = dir;
   curr->depth = depth + 1;
   curr->next = NULL;

   if( queue_tail != NULL )
   {
      queue_tail->next = curr;
      queue_tail = curr;
   }
   else
      queue_head = queue_tail = curr;

   list_enqueue( room );

   return;
}

void bfs_dequeue(void)
{
   BFS_QUEUE *curr;

   curr = queue_head;

   if( (queue_head = queue_head->next) == NULL )
      queue_tail = NULL;

   free_bfs_queue(curr);
   return;
}

void list_dequeue( void )
{
   BFS_ROOM *curr;

   curr = list_head;

   if( (list_head = list_head->next) == NULL )
      list_tail = NULL;

   UNMARK( curr->room );
   free_bfs_room(curr);
   return;
}

void bfs_clear_queue(void) 
{
   while( queue_head != NULL )
      bfs_dequeue();
   return;
}

void list_clear_queue( void )
{
   while( list_head != NULL )
      list_dequeue();
}

/* find_first_step: given a source room and a target room, find the first
   step on the shortest path from the source to the target.

   Intended usage: in mobile_activity, give a mob a dir to go if they're
   tracking another mob or a PC.  Or, a 'track' skill for PCs.
*/


int find_first_step( ROOM_INDEX_DATA *src, ROOM_INDEX_DATA *target)
{
   int curr_dir;

   if ( src == NULL || target == NULL ) {
      bug("Illegal value passed to find_first_step (graph.c)", 0 );
      return BFS_ERROR;
   }

   if (src == target)
      return BFS_ALREADY_THERE;

   queue_head = queue_tail = NULL;
   list_head = list_tail = NULL;

   MARK( src );
   list_enqueue( src );

   /* first, enqueue the first steps, saving which direction we're going. */
   for (curr_dir = 0; curr_dir < 6; curr_dir++)
      if ( VALID_EDGE( src, curr_dir ) ) 
      {
         MARK( TOROOM( src, curr_dir ) );
         bfs_enqueue( TOROOM( src, curr_dir ), curr_dir, 0 );
      }

   /* now, do the classic BFS. */
   while( queue_head )
   {
      if( queue_head->depth >= 200 )
      {
         bfs_clear_queue();
         list_clear_queue();
         return BFS_NO_PATH;
      }
      if( queue_head->room == target ) 
      {
	 curr_dir = queue_head->dir;
	 bfs_clear_queue();
         list_clear_queue();
	 return curr_dir;
      }
      else
      {
         for( curr_dir = 0; curr_dir < 6; curr_dir++ )
         {
            if (VALID_EDGE( queue_head->room, curr_dir ) ) 
            {
               MARK( TOROOM( queue_head->room, curr_dir ) );
	       bfs_enqueue( TOROOM( queue_head->room, curr_dir  ),
                queue_head->dir, queue_head->depth );
            }
         }
         bfs_dequeue();
      }
   }
   list_clear_queue();
   return BFS_NO_PATH;
}

