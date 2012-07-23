/*************************************************************************
Locke code from the isles.  And a few modifications by Quin and Galadrail.
*************************************************************************/
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
#include "magic.h"


void redit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    ROOM_INDEX_DATA *pRoomIndex = NULL;
    ROOM_INDEX_DATA *pRoomtemp = NULL;
    ROOM_INDEX_DATA *pRoomIndex_new;
    MOB_INDEX_DATA *pMob;
    OBJ_INDEX_DATA *pObj;
    EXTRA_DESCR_DATA *ed;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    long  value, vnum;
    int  door;
    long x;
    int value_int;

    strcpy( arg, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    pRoom = ch->in_room;
    pArea = pRoom->area;


    if ( !IS_BUILDER( ch, pArea ) )
    {
        send_to_char( "REdit:  Insufficient security to modify room.\n\r", ch );
    }


    if ( !str_cmp( arg1, "show" ) || arg1[0] == '\0' )
    {
        sprintf( buf, "%ld", pRoom->vnum );
        do_rshow( ch, buf );
        return;
   }

    if ( !str_cmp( arg1, "done" ) )
    {
        ch->desc->pEdit = NULL;
        ch->desc->connected = CON_PLAYING;
	ch->edit_menu = 0;
	ch->edit_buffer[0] = '\0';
        return;
    }

    if ( !str_cmp( arg1, "??" ) )
    if (edit_function(ch, REDIT_ANY, SECURITY_9))
    {
	sprintf(buf,"`C                 Secondary Commands Menu\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bdone`w         Exit Redit and return to normal commands\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bshow`w         Displays current room status ( rshow )\n\r");
	send_to_char(buf,ch);

	if (ch->edit_menu == REDIT_EVENT_MENU )
	{
	send_to_char("`Blist        `w List all events in this room\n\r",ch);
	send_to_char("`w                  Syntax: list\n\r",ch);
	}
	if (ch->edit_menu == REDIT_DOOR_MENU )
	{
	send_to_char("`BMTC         `w Message to char on exit\n\r",ch);
	send_to_char("`w                  Syntax: mtc <string>\n\r",ch);
	send_to_char("`BMTR         `w Message to room on exit\n\r",ch);
	send_to_char("`w                  Syntax: mtr <string>\n\r",ch);
	send_to_char("`BMAR         `w Message to room on exit at toroom\n\r",ch);
	send_to_char("`w                  Syntax: mar <string>\n\r",ch);
	send_to_char("`BDesc        `w Description of exit\n\r",ch);
	send_to_char("`w                  Syntax: desc <direction> <string>\n\r",ch);
	send_to_char("`BSpell       `w Trap spells on door\n\r", ch );
	send_to_char("`w                  Syntax: spell <direction> <level> <spell>\n\r",ch);
	send_to_char("`BTflags      `w Trap flags on door\n\r", ch );
	send_to_char("`w                  Syntax: spell <direction> <trap flag> \n\r",ch);
	send_to_char("`BKey         `w Key vnum ( of object ) on lock\n\r", ch );
	send_to_char("`w                  Syntax: key <direction> <obj vnum> \n\r",ch);
	send_to_char("`BDelete      `w Delete a exit\n\r", ch );
	send_to_char("`w                  Syntax: delete <direction> \n\r",ch);
	send_to_char("`w                  Syntax: delete <direction> [both]\n\r",ch);
	}

	if (ch->edit_menu == REDIT_MENU )
	{
	sprintf(buf,"`Bremove_trig `w Remove a trigger from this object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: remove_trigger <trigger vnum>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Badd_trigger `w Add a trigger to this object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: add_trigger <trigger vnum>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bmakeroom    `w Make a new room in specified direction\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: makeroom <direction>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: makeroom <direction> <vnum>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Blink        `w Links an exit to a specified vnum\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: link <direction> <vnum>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: link <direction> <vnum> [1way]\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bdescription `w Enter the editor for the room description\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: description\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bformat      `w Format the current room description\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: format\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bloadtoroom  `w Load current note to room description\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: loadtoroom\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bloadtonote  `w Load current room description to note\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: loadtonote\n\r");
	send_to_char(buf,ch);
	}

    return;
    }

    if ( !str_cmp( arg1, "?" ) )
    if (edit_function(ch, REDIT_ANY, SECURITY_9))
    {
	sprintf(buf,"`C                      Command Redit Menu\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`B?`w         Show current menu, status and available commands\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`B??`w        Show secondary commands for this menu\n\r");
	send_to_char(buf,ch);
	if (ch->edit_menu > 2 )
	{
	sprintf(buf,"`BBack`w      Back one menu\n\r");
	send_to_char(buf,ch);
	}

	if (ch->edit_menu == REDIT_EXTRA_MENU )
	{
	    if ( pRoom->extra_descr != NULL )
	    {
	        send_to_char( "Extra description keywords: `G", ch );
	
	        for ( ed = pRoom->extra_descr; ed != NULL; ed = ed->next )
	        {
	        send_to_char( "'", ch );
	            send_to_char( ed->keyword, ch );
	        send_to_char( "'", ch );
	        }
	        send_to_char( "\n\r", ch );
	    }
	    else
	        send_to_char( "`GNone\n\r", ch );
	send_to_char("`BDelete      `w Delete extra description from this room\n\r", ch );
	send_to_char("`w                  Syntax: delete <keyword> \n\r",ch);
	send_to_char("`BAdd         `w Add extra description to this room\n\r", ch );
	send_to_char("`w                  Syntax: add <keyword> \n\r",ch);
	send_to_char("`BEdit        `w Edit extra description on this room\n\r", ch );
	send_to_char("`w                  Syntax: edit <keyword> \n\r",ch);
	send_to_char("`BFormat      `w Format extra description on this room\n\r", ch );
	send_to_char("`w                  Syntax: format <keyword> \n\r",ch);
	}
	if (ch->edit_menu == REDIT_SPEC_MENU )
	{
	send_to_char("\n\r",ch);
	do_specroom(ch,"");
	send_to_char("`BAdd         `w Add a special process to this room\n\r", ch );
	send_to_char("`w                  Syntax: add <special process number> \n\r",ch);
	send_to_char("`BRemove      `w Remove a special process from this room\n\r", ch );
	send_to_char("`w                  Syntax: remove <special process number> \n\r",ch);
	}
	if (ch->edit_menu == REDIT_EVENT_MENU )
	{
	send_to_char("`BCreate      `w Create a new event on this room\n\r", ch );
	send_to_char("`w                  Syntax: create \n\r",ch);
	send_to_char("`BRemove      `w Remove event from this room\n\r", ch );
	send_to_char("`w                  Syntax: remove <event vnum> \n\r",ch);
	send_to_char("`BEvent       `w Change values for events on this room\n\r", ch );
	send_to_char("`w                  Syntax: event <vnum> chance <percent>\n\r",ch);
	send_to_char("`w                  Syntax: event <vnum> flags <event_flags>\n\r",ch);
	send_to_char("`w                  Syntax: event <vnum> object <obj_vnum>\n\r",ch);
	send_to_char("`w                  Syntax: event <vnum> mobile <mob_vnum>\n\r",ch);
	send_to_char("`BEvent       `w Create new events with default chance and flags\n\r", ch );
	send_to_char("`w                  Syntax: event mobile <mob_vnum>\n\r",ch);
	send_to_char("`w                  Syntax: event object <obj_vnum>\n\r",ch);
	send_to_char("`Blist        `w List all events in this room or area\n\r",ch);
	send_to_char("`w                  Syntax: list\n\r",ch);
	send_to_char("`w                  Syntax: list all\n\r",ch);
	send_to_char("`Bclean       `w Clean all bad  events in this room or area\n\r",ch);
	send_to_char("`w                  Syntax: clean\n\r",ch);
	send_to_char("\n\r",ch);
	redit(ch,"list");
	}
	if (ch->edit_menu == REDIT_DOOR_MENU )
	{
	    for ( door = 0; door < MAX_DIR; door++ )
	    {
		SPELL_LIST *spell;
		EXIT_DATA *pexit;
		bool found = FALSE;
		LOCK_DATA *lock_list;
	
		if ( ( pexit = pRoom->exit[door] ) )
		{
		    sprintf( buf,
			    "`wExit: %5s.  To: %ld\n\r`BName `G     %s `BFlags `G    %s\n\r",
			    dir_name[door],
			    pexit->u1.to_room ? pexit->u1.to_room->vnum : 0,
			    (pexit->keyword[0] != '\0') ? pexit->keyword : "",
			    get_exit_flag_string( pexit->exit_info_default ));
		    send_to_char( buf,ch );
		    if ( pexit->description )
		    {
			sprintf( buf,"`BDesc   `G   %s\n\r", pexit->description ? pexit->description : ""  );
			send_to_char( buf, ch );
		    }
		    if ( pexit->mes_to_char )
		    {
			sprintf( buf,"`BMTC     `G  %s\n\r",pexit->mes_to_char ? pexit->mes_to_char : "");	  
			send_to_char(buf, ch);
		    }
		    if ( pexit->mes_to_room )
		    {
			sprintf( buf,"`BMTR      `G %s\n\r",pexit->mes_to_room ? pexit->mes_to_room : "");	  
			send_to_char(buf, ch);
		    }
		    if ( pexit->mes_at_room )
		    {
			sprintf( buf,"`BMAR      `G %s\n\r",pexit->mes_at_room ? pexit->mes_at_room : "");	  
			send_to_char( buf, ch);
		    }
		    if ( pexit->trap != NULL )
		    {
			sprintf( buf,"`BTflags   `G %s\n\r", get_flags_trap( pexit->trap->flags ) );
		        send_to_char( buf,ch );
			sprintf( buf,"`BSpells \n\r");
		        send_to_char( buf,ch );
	
			for ( spell = pexit->trap->spells ; spell != NULL ; spell = spell->next )
			{
			    found = TRUE;
			    sprintf( buf, "   Level %d spell of %s\n\r", spell->level, skill_table[ spell->spell ].name );
		       	    send_to_char( buf,ch );
			}
	 	    }
		    for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_list->next )
		    {
			sprintf( buf, "`BKey    `G   %ld  `BLflags `g %s\n\r", lock_list->vnum,
			get_flags_lock(lock_list->lock_info_default) );
			send_to_char(buf,ch);
		    }
		    if ( pexit->u1.to_room != NULL && pexit->u1.to_room->exit[ rev_dir[ door ] ] != NULL
			&& pexit->u1.to_room->exit[ rev_dir[ door ] ]->u1.to_room == pRoom
	 		&& ( lock_list = pexit->u1.to_room->exit[ rev_dir[ door ] ]->lock ) != NULL )
		    {
			send_to_char( "  - Otherside - \n\r", ch );
		        for ( ; lock_list != NULL ; lock_list = lock_list->next )
		        {
			sprintf( buf, "`BKey    `G   %ld  `BLflags `g %s\n\r", lock_list->vnum,
			get_flags_lock(lock_list->lock_info_default) );
			send_to_char(buf,ch);
		        }
		    }
		}
	    }
	}
	if (ch->edit_menu == REDIT_MENU )
	{
	sprintf(buf,"`B1`w         Extra Data           %s\n\r`w",
	    !pRoom->extra_descr ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B2`w         Door Data            %s\n\r`w",
	    !pRoom->exit ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B3`w         Special Pros         %s\n\r`w",
	    !pRoom->spro ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B4`w         Events               \n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BName      `G%s\n\r`w", pRoom->name);
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: Name <string>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BFlags     `G%s %s\n\r`w", room_bit_name(pRoom->room_flags), room_bit_name_2(pRoom->room_flags_2));
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: flag <flag name>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BGuild     `G%s\n\r`w", guild_bit_name(pRoom->guilds));
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: Guild <guild name>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BLight     `G%d\n\r`w", pRoom->light);
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: Light <value>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BSector    `G%s\n\r`w", sector_bit_name(pRoom->sector_type));
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: Sector <sector name>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BSize      `G%d\n\r`w", pRoom->size);
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: Size <value>\n\r`w");
	send_to_char(buf,ch);
	}
        return;
    }


    if ( !IS_BUILDER( ch, pArea ) )
    {
        interpret( ch, arg );
        return;
    }

    if (!strncmp( arg1, "extra_descr", 8 ) || !strncmp(arg1, "1", 1) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = REDIT_EXTRA_MENU ;
	strncpy( ch->edit_buffer, "Extra->", 80 );
	return;
    }
    if (!strncmp( arg1, "event_data", 8 ) || !strncmp(arg1, "4", 1) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = REDIT_EVENT_MENU ;
	strncpy( ch->edit_buffer, "Events->", 80 );
	return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-" ) )
    if (edit_function(ch, REDIT_EXTRA_MENU, SECURITY_9))
    {
		ch->edit_menu = REDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-" ) )
    if (edit_function(ch, REDIT_EVENT_MENU, SECURITY_9))
    {
		ch->edit_menu = REDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!strncmp( arg1, "door_data", 8 ) || !strncmp(arg1, "2", 1) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = REDIT_DOOR_MENU ;
	strncpy( ch->edit_buffer, "Doors->", 80 );
	return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
		ch->edit_menu = REDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!strncmp( arg1, "spec_pros", 8 ) || !strncmp(arg1, "3", 1) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = REDIT_SPEC_MENU ;
	strncpy( ch->edit_buffer, "Special->", 80 );
	return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-" ) )
    if (edit_function(ch, REDIT_SPEC_MENU, SECURITY_9))
    {
		ch->edit_menu = REDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }

    if ( !str_cmp( arg1, "xyzzy" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_1))
    {
	argument = one_argument( argument, arg1);

	if ( get_exit_flag_number( arg1) == 0 )
	{
	    send_to_char("Invalid door flag\n\r",ch);
	    return;
	}

	for ( x = pArea->lvnum ; x < pArea->uvnum ; x++ )
	    if ( ( pRoomIndex = get_room_index(x) ) != NULL )
		for ( door = 0; door < 6; door++ )
		    if ( pRoomIndex->exit[door] != NULL )
			SET_BIT(pRoomIndex->exit[door]->exit_info_default, get_exit_flag_number( arg1 ));

	send_to_char( "Done!\n\r", ch );
	return;
    }

    if ( !strncmp( arg1, "flags", 3 ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	bool fRoom1 = FALSE;
	bool fRoom2 = FALSE;
	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);
	argument = one_argument( argument, arg3);
	
	if ( room_name_bit( arg1 ) != ROOM_NONE )
	    fRoom1 = TRUE;
	else
	if ( room_name_bit_2( arg1 ) != ROOM_NONE )
	    fRoom2 = TRUE;
	if ( !fRoom1 && !fRoom2 )
	{
	    send_to_char( "Syntax: flag < room_flag >\n\r", ch );
	    send_to_char( "Syntax: flag < room_flag > [ all ] {toggle|remove|set}\n\r", ch );
	    sprintf( buf, "%s %s\n\r",
	    room_bit_name( ALL_FLAGS ),
	    room_bit_name_2( ALL_FLAGS ) );
	    send_to_char(buf, ch);
	    return;
	}	
	if ( arg2[0] != '\0' && arg3[0] != '\0' );
	{
	    if ( !str_cmp(arg2, "all" )  )		
	    {
		if ( !str_cmp( arg3, "toggle" ) )
		{
		    for ( x = pArea->lvnum ; x < pArea->uvnum ; x++ )
		    {
			if ( ( pRoomIndex = get_room_index(x) ) != NULL )
			{
			    if (fRoom1)
			    TOGGLE_BIT(pRoomIndex->room_flags, room_name_bit( arg1 ));
			    if (fRoom2)
			    TOGGLE_BIT(pRoomIndex->room_flags_2, room_name_bit_2( arg1));
			    if ( (pRoomIndex->sector_type == SECT_UNDERGROUND )
				|| IS_SET( pRoomIndex->room_flags, ROOM_INDOORS ) )
		    		pRoomIndex->size = 3;
			    else
		    		pRoomIndex->size = 5;
			}
		    }
		SET_BIT( pArea->area_flags, AREA_CHANGED );
		send_to_char( "Room flags toggled.\n\r", ch );
		return;
		}
		if ( !str_cmp( arg3, "remove" ) )
		{
		    for ( x = pArea->lvnum ; x < pArea->uvnum ; x++ )
		    {
			if ( ( pRoomIndex = get_room_index(x) ) != NULL )
			{
			    if (fRoom1)
			    REMOVE_BIT(pRoomIndex->room_flags, room_name_bit( arg1 ));
			    if (fRoom2)
			    REMOVE_BIT(pRoomIndex->room_flags_2, room_name_bit_2( arg1));
			    if ( ( pRoomIndex->sector_type == SECT_UNDERGROUND )
				|| IS_SET( pRoomIndex->room_flags, ROOM_INDOORS ) )
		    		pRoomIndex->size = 3;
			    else
		    		pRoomIndex->size = 5;
			}
		    }
		SET_BIT( pArea->area_flags, AREA_CHANGED );
		send_to_char( "Room flags removed.\n\r", ch );
		return;
		}
		if ( !str_cmp( arg3, "set" ) )
		{
		    for ( x = pArea->lvnum ; x < pArea->uvnum ; x++ )
		    {
			if ( ( pRoomIndex = get_room_index(x) ) != NULL )
			{
			    if (fRoom1)
			    SET_BIT(pRoomIndex->room_flags, room_name_bit( arg1 ));
			    if (fRoom2)
			    SET_BIT(pRoomIndex->room_flags_2, room_name_bit_2( arg1));
			    if ( ( pRoomIndex->sector_type == SECT_UNDERGROUND )
				|| IS_SET( pRoomIndex->room_flags, ROOM_INDOORS ) )
		    		pRoomIndex->size = 3;
			    else
		    		pRoomIndex->size = 5;
			}
		    }
		SET_BIT( pArea->area_flags, AREA_CHANGED );
		send_to_char( "Room flags set.\n\r", ch );
		return;
		}
	    }
	}

	if (fRoom1)
	TOGGLE_BIT(pRoom->room_flags, room_name_bit( arg1 ));
	if (fRoom2)
	TOGGLE_BIT(pRoom->room_flags_2, room_name_bit_2( arg1));

	if ( IS_SET( pRoom->room_flags, ROOM_INDOORS )
	    || ( pRoom->sector_type == SECT_UNDERGROUND ) )
	    pRoom->size = 3;
	else
	    pRoom->size = 5;

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Room flag toggled.\n\r", ch );
        return;
    }

    if ( !strncmp( arg1, "sector", 3 ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	int y = 0;
	argument = one_argument( argument, arg1);
	argument = one_argument( argument, arg2);
	 
	if ( sector_number( arg1 ) == SECT_MAX )
	{
        send_to_char( "Syntax: sector <sector_name>\n\r", ch );
        send_to_char( "Syntax: sector <sector_name> [all] \n\r", ch );
	    
	    for ( x= 0; x < MAX_SECTORS; x++)
	    {
		sprintf(buf, "Sector:%-15s   ",sector_bit_name(x));
		y++;
		if (y > 1)
		{
		    strcat(buf,"\n\r");
		    y = 0;
		}	 
	    send_to_char( buf, ch );
	    }
	    return;
	}
	
	if ( arg2[0] != '\0' );
	{
	    if ( !str_cmp(arg2, "all" )  )		
	    {
		    for ( x = pArea->lvnum ; x < pArea->uvnum ; x++ )
		    {
			if ( ( pRoomIndex = get_room_index(x) ) != NULL )
			{
        		    pRoomIndex->sector_type  = sector_number( arg1 );

			    if ( ( pRoomIndex->sector_type == SECT_UNDERGROUND)
				|| IS_SET( pRoomIndex->room_flags,ROOM_INDOORS ) )
				pRoomIndex->size = 3;
			    else
				pRoomIndex->size = 5;
			}
		    }

		SET_BIT( pArea->area_flags, AREA_CHANGED );
		send_to_char( "Sector type set for all rooms in this area.\n\r", ch );
		return;
	    }
	}

        pRoom->sector_type  = sector_number( arg1 );
	if ( IS_SET( pRoom->room_flags, ROOM_INDOORS )
	|| ( pRoom->sector_type == SECT_UNDERGROUND ) )
	    pRoom->size = 3;
	else
	    pRoom->size = 5;

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Sector type set.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "link" ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	bool oneway = FALSE;
        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
        argument = one_argument( argument, arg3 );
	
	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Invalid direction\n\r", ch );
	    send_to_char( "Syntax: link [direction] [vnum] \n\r", ch );
	    send_to_char( "Syntax: link [direction] [vnum] [1way]\n\r", ch );
	    return;
	}
	if ( pRoom->exit[door] != NULL )
	{
	    send_to_char( "REdit:  An exit is already there!\n\r", ch );
	    send_to_char( "Syntax: link [direction] [vnum] \n\r", ch );
	    send_to_char( "Syntax: link [direction] [vnum] [1way]\n\r", ch );
	    return;
	}

	if ( arg2[0] == '\0' || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: link [direction] [vnum] \n\r", ch );
	    send_to_char( "Syntax: link [direction] [vnum] [1way]\n\r", ch );
	    return;
	}
	oneway = !str_cmp( arg3, "1way" );
	value = atoi( arg2 );

	if ( ( pRoomIndex = get_room_index( value ) ) == NULL )
	{
	    send_to_char( "REdit:  Cannot link to non-existant room.\n\r", ch );
	    return;
	}

	if ( !IS_BUILDER( ch, pRoomIndex->area ) )
	{
	    send_to_char( "REdit:  Remote side not created, not a builder there.\n\r", ch );
	    return;
	}

	if ( pRoomIndex->exit[rev_dir[door]] != NULL && !oneway)
	{
	    send_to_char( "REdit:  Remote side's exit already exists.\n\r", ch );
	    return;
	}

	make_exit(pRoom, pRoomIndex, door);
	sprintf(buf, "Room %ld direction %s linked to Room %ld\n\r",pRoom->vnum, dir_name[door], pRoomIndex->vnum );
	send_to_char(buf, ch);
	if (!oneway )
	{
	    make_exit(pRoomIndex, pRoom, rev_dir[door]);
	    sprintf(buf, "Room %ld direction %s linked to Room %ld\n\r",pRoomIndex->vnum, dir_name[rev_dir[door]], pRoom->vnum );
	    send_to_char(buf, ch);
	}
	return;
    } 
    if ( !strncmp( arg1, "dup_temp" , 4) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	{
	    if ( ( pRoomtemp = get_room_index( ch->room_template ) ) == NULL )
	    {
		send_to_char( "REdit:  Room template not found.  Use set_room_template <room vnum>.\n\r", ch );
		return;
	    }
	}
	    strncpy( pRoom->name, pRoomtemp->name, 80 );
	    pRoom->room_flags = pRoomtemp->room_flags;
	    pRoom->room_flags_2 = pRoomtemp->room_flags_2;
	    pRoom->guilds = pRoomtemp->guilds;
	    pRoom->light_adjustment = pRoomtemp->light_adjustment;
	    pRoom->light = pRoomtemp->light;
	    pRoom->size = pRoomtemp->size;
	    pRoom->sector_type = pRoomtemp->sector_type;
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Done.\n\r", ch );
	return;
    }
    if ( !strncmp( arg1, "makeroom" , 4) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
	
	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Invalid direction\n\r", ch );
	    send_to_char( "Syntax: makeroom <direction>\n\r", ch );
	    send_to_char( "Syntax: makeroom <direction> [vnum]\n\r", ch );
	    send_to_char( "creates a new room and links the exits.\n\r", ch );
	    return;
	}
	if ( pRoom->exit[door] != NULL )
	{
	    send_to_char( "REdit:  An exit is already there!\n\r", ch );
	    send_to_char( "Syntax: makeroom <direction>\n\r", ch );
	    send_to_char( "Syntax: makeroom <direction> [vnum]\n\r", ch );
	    send_to_char( "creates a new room and links the exits.\n\r", ch );
	    return;
	}
	if ( !strncmp(arg2,"template",4))
	{
	    if ( ( pRoomtemp = get_room_index( ch->room_template ) ) == NULL )
	    {
		send_to_char( "REdit:  Room template not found.  Use set_room_template <room vnum>.\n\r", ch );
		return;
	    }
	}
	else if ( arg2[0] != '\0' && !is_number( arg2 ) )
	{
	    send_to_char( "REdit:  Invalid vnum!\n\r", ch );
	    send_to_char( "Syntax: makeroom <direction>\n\r", ch );
	    send_to_char( "Syntax: makeroom <direction> [vnum]\n\r", ch );
	    return;
	}
	if ( arg2[0] != '\0' )
	    value = atoi( arg2 );
	else
	    value = 0;

	if ( ( pRoomIndex_new = create_room( ch, pArea, value ) ) == NULL )
	    return; 
	if ( pRoomtemp )
	{
	    strncpy( pRoomIndex_new->name, pRoomtemp->name, 80 );
	    pRoomIndex_new->description = str_dup(pRoomtemp->description);
	    pRoomIndex_new->room_flags = pRoomtemp->room_flags;
	    pRoomIndex_new->room_flags_2 = pRoomtemp->room_flags_2;
	    pRoomIndex_new->guilds = pRoomtemp->guilds;
	    pRoomIndex_new->light_adjustment = pRoomtemp->light_adjustment;
	    pRoomIndex_new->light = pRoomtemp->light;
	    pRoomIndex_new->size = pRoomtemp->size;
	    pRoomIndex_new->sector_type = pRoomtemp->sector_type;
	}
	
	make_exit(pRoom, pRoomIndex_new, door);
	make_exit(pRoomIndex_new, pRoom, rev_dir[door]);
	return;
    }


    if ( !str_cmp( arg1, "delete" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
	bool fBoth = FALSE; 

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Syntax: delete <direction>.\n\r", ch );
	    send_to_char( "This will unlink and remove the exit from this side only!.\n\r", ch );
	    send_to_char( "Syntax: delete <direction> [both].\n\r", ch );
	    send_to_char( "This will unlink and remove the exit from both sides.\n\r", ch );
	    return;
	}
	   	 
	
	if ( pRoom->exit[door] == NULL )
	{
	    send_to_char( "REdit:  Cannot delete a non-existant exit.\n\r", ch );
	    return;
	}
	if ( fBoth )
	{
	    if (pRoom->exit[door]->u1.to_room != NULL )
	    {
		free_exit( pRoom->exit[door]->u1.to_room->exit[rev_dir[door]] );
		pRoom->exit[door]->u1.to_room->exit[rev_dir[door]] = NULL;
                send_to_char( "Exit other side unlinked.\n\r", ch );
	    }
	}

            free_exit( pRoom->exit[door] );
            pRoom->exit[door] = NULL;

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Exit unlinked.\n\r", ch );
            return;
        }

       

    if ( !str_cmp( arg1, "spell" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
	EXIT_DATA *pexit;
	int level,sn;

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
        argument = one_argument( argument, arg3 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Usage: spell <direction> <level> <spell> \n\r", ch );
	    send_to_char( "(negative level removes previous spells)\n\r", ch );
	    return;
	}

	pexit = pRoom->exit[door];

	if ( arg2[0] == '\0' || arg3[0] == '\0' )
	{
	    send_to_char( "Usage: spell <direction> <level> <spell> \n\r", ch );
	    send_to_char( "(negative level removes previous spells)\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( arg3 ) ) == -1 || skill_table[ sn ].spell_fun == spell_null )
	{
	    send_to_char( "Invalid spell!\n\r", ch );
	    return;
	}

	if ( pexit == NULL )
	{
	    send_to_char( "That is not an exit.\n\r", ch );
	    return;
	}

	if ( pexit->trap == NULL )
	    pexit->trap = trap_alloc();

	if ( ( level = atoi( arg2 ) ) < 0 )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spell_next;

	    if ( pexit->trap->spells == NULL )
	    {
		send_to_char( "That exit contains no spells to erase.\n\r", ch );
		return;
	    }

	    if ( pexit->trap->spells->spell == sn )
	    {
		spell = pexit->trap->spells;
		pexit->trap->spells = pexit->trap->spells->next;
		free_spell( spell );

		if ( pexit->trap->spells == NULL )
		{
		    free_trap_data( pexit->trap );
		    pexit->trap = NULL;
		    send_to_char( "Trap removed.\n\r", ch );
		}
		else
	            send_to_char( "Trap spell removed.\n\r", ch);
	        SET_BIT( pArea->area_flags, AREA_CHANGED );
	        return;
	    }
	    else
	        for ( spell = pexit->trap->spells , spell_next = spell->next ; spell_next != NULL ; spell = spell_next )
	        {
		    spell_next = spell->next;

		    if ( spell_next->spell == sn )
		    {
			spell->next = spell_next->next;
			free_spell( spell_next );
		        SET_BIT( pArea->area_flags, AREA_CHANGED );
		        send_to_char( "Trap spell removed.\n\r", ch);
		        return;
		    }
	        }	

	    send_to_char( "Spell not found.\n\r", ch );
	    return;
	}
	else
	{
	    SPELL_LIST * spell;
	
	    spell = spell_alloc();
	    spell->level = level;
	    spell->spell = sn;
	    spell->next = pexit->trap->spells;
	    pexit->trap->spells = spell;
	}
	
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Trap spell added.\n\r", ch);
        return;
    }

    if ( !strncmp( arg1, "tflags",4 ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
	EXIT_DATA *pexit;

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
            send_to_char( "Syntax: tflags <direction> <trap flag>\n\r", ch );
	    sprintf( buf, "\n\r       %s\n\r", get_flags_trap( ALL_FLAGS ) );
	    send_to_char( buf, ch );
	    return;
	}
	pexit = pRoom->exit[door];

        if ( arg2[0] == '\0' || ( value = get_trap_flags( arg2 ) ) == 0 )
        {
            send_to_char( "Syntax: tflags <direction> <trap flag>\n\r", ch );
	    sprintf( buf, "\n\r       %s\n\r", get_flags_trap( ALL_FLAGS ) );
	    send_to_char( buf, ch );
            return;
        }

	if ( pexit == NULL )
	{
	    send_to_char( "That is not an exit and cannot be trapped.\n\r", ch );
	    return;
	}

	if ( pexit->trap == NULL )
	    pexit->trap = trap_alloc();

	TOGGLE_BIT( pexit->trap->flags, value );
	send_to_char( "Flag toggled.\n\r", ch );
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return;
    }

    if ( !strncmp( arg1, "lflags",4 ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
	EXIT_DATA *pexit;
	LOCK_DATA *lock_list;
	long key_vnum;

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
        argument = one_argument( argument, arg3 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
            send_to_char( "Syntax: lflags <direction> <key_vnum> <lock_flags>\n\r", ch );
	    sprintf( buf, "\n\r       %s\n\r", get_flags_lock( ALL_FLAGS ) );
	    send_to_char( buf, ch );
	    return;
	}
	pexit = pRoom->exit[door];

	key_vnum = atoi(arg2);

        if ( get_obj_index( key_vnum ) == NULL )
        {
            send_to_char( "Key not found\n\r", ch );
            return;
        }

        if ( arg3[0] == '\0' || ( value = get_lock_flags( arg3 ) ) == 0 )
        {
            send_to_char( "Syntax: lflags <direction> <key_vnum> <lock_flags>\n\r", ch );
	    sprintf( buf, "\n\r       %s\n\r", get_flags_lock( ALL_FLAGS ) );
	    send_to_char( buf, ch );
            return;
        }

	if ( pexit == NULL )
	{
	    send_to_char( "That is not an exit\n\r", ch );
	    return;
	}
	if ( pexit->lock == NULL )
	{
	    send_to_char( "That is not a lock there\n\r", ch );
	    return;
	}
	if ( ( lock_list = get_lock_data(key_vnum , pexit->lock) ) == NULL )
	{
              send_to_char ("Lock not found for the key\n\r",ch);
	      return;
	}

	TOGGLE_BIT( lock_list->lock_info_default, value );
	send_to_char( "Flag toggled.\n\r", ch );
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return;
    }

    if ( !str_cmp( arg1, "key" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
	    LOCK_DATA * lock;
	    LOCK_DATA * lock_next;

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Syntax:  key [direction] [vnum]\n\r", ch );
	    return;
	}

            if ( arg2[0] == '\0' || !is_number( arg2 ) )
            {
                send_to_char( "Syntax:  key [direction] [vnum]\n\r", ch );
                return;
            }

            if ( pRoom->exit[door] == NULL )
            {
                send_to_char( "REdit:  Exit does not exists.\n\r", ch );
                return;
            }

	    if ( ( value = atoi( arg2 ) ) < 0 )
	    {
		value *= -1;

		if ( pRoom->exit[door]->lock != NULL
		  && pRoom->exit[door]->lock->vnum == value )
		{
		    lock = pRoom->exit[door]->lock;
		    pRoom->exit[door]->lock = pRoom->exit[door]->lock->next;
		    free_lock_data( lock );

	            SET_BIT( pArea->area_flags, AREA_CHANGED );
		    send_to_char( "Lock removed.\n\r", ch );
		    return;	
		}

		for ( lock = pRoom->exit[door]->lock ; lock != NULL ; lock = lock_next )
		{
		    lock_next = lock->next;

		    if ( lock_next != NULL && lock_next->vnum == value )
		    {
			lock->next = lock_next->next;

			free_lock_data( lock_next );

	                SET_BIT( pArea->area_flags, AREA_CHANGED );
		        send_to_char( "Lock removed.\n\r", ch );
		        return;	
		    }
		}

		send_to_char( "Lock not found.\n\r", ch );
		return;
	    }

            if ( get_obj_index( value ) == NULL )
            {
                send_to_char( "REdit:  Cannot use a non-existant object as a key.\n\r", ch );
                return;
            }

	    lock = lock_alloc();
            lock->vnum = value;
            lock->lock_info = 0;
            lock->lock_info_default = 0;
	    lock->pick_penalty = 0;
	    lock->next = pRoom->exit[door]->lock;
            pRoom->exit[door]->lock = lock;

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Exit key set.\n\r", ch );
            return;
    }
    if ( !strncmp( arg1, "step", 3 ))
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
 	pArea = ch->in_room->area;

	if ( pArea->lvnum <= 0 || pArea->uvnum <= 0 )
	{ 
	    send_to_char( "Can't find begin vnum for the area.\n\r", ch );
	    return;
	}
	
	for ( vnum = pRoom->vnum; vnum <= pArea->uvnum; vnum++ )
	{
	    if ( vnum > pRoom->vnum )
	    if ( (pRoomIndex = get_room_index( vnum ) ) != NULL )
	    {
		break;
	    }
	}
	
	if ( pRoomIndex )
	{
	    char_from_room(ch);
	    char_to_room(ch, pRoomIndex);
	    ch->desc->pEdit = (void *)pRoomIndex;
	    pRoom = (ROOM_INDEX_DATA *)ch->desc->pEdit;
	    pArea = pRoom->area;
            sprintf( buf, "%ld", pRoomIndex->vnum );
            do_look( ch, "" );
	}
	else 
	{ 
	    send_to_char( "End of rooms in area.\n\r", ch );
	    return;
	}
	
	return;
    }



    if ( !str_cmp( arg1, "mes_to_char" ) || !str_cmp(arg1, "mtc" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
        argument = one_argument( argument, arg1 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Syntax:  mtc [direction] [string]\n\r", ch );
	    return;
	}
            if ( argument[0] == '\0' )
            {
                send_to_char( "Syntax: mtc [direction] [string]\n\r", ch );
                return;
            }

            if ( pRoom->exit[door] == NULL )
            {
                send_to_char( "REdit:  Cannot change mes_to_char on a non-existant exit.\n\r", ch );
                return;
            }

            strncpy( pRoom->exit[door]->mes_to_char, argument, 80 );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Exit mes_to_char set.\n\r", ch );
            return;
    }

    if ( !str_cmp( arg1, "mes_at_room" ) || !str_cmp( arg1, "mar" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
        {
        argument = one_argument( argument, arg1 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Syntax:  mar [direction] [string]\n\r", ch );
	    return;
	}
            if ( argument[0] == '\0' )
            {
                send_to_char( "Syntax:  mar [direction] [string]\n\r", ch );
                return;
            }

            if ( pRoom->exit[door] == NULL )
            {
                send_to_char( "REdit:  Cannot change mes_at_room on a non-existant exit.\n\r", ch );
                return;
            }

            strncpy( pRoom->exit[door]->mes_at_room, argument, 80 );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Exit mes_at_room set.\n\r", ch );
            return;
    }

    if ( !str_cmp( arg1, "mes_to_room" ) || !str_cmp( arg1, "mtr" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
        argument = one_argument( argument, arg1 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Syntax:  mtr [direction] [string]\n\r", ch );
	    return;
	}
            if ( argument[0] == '\0' )
            {
                send_to_char( "Syntax: mtr [direction] [string]\n\r", ch );
                return;
            }

            if ( pRoom->exit[door] == NULL )
            {
                send_to_char( "REdit:  Cannot change mes_to_room on a non-existant exit.\n\r", ch );
                return;
            }

            strncpy( pRoom->exit[door]->mes_to_room, argument, 80 );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Exit mes_to_room set.\n\r", ch );
            return;
    }
    if ( !str_cmp( arg1, "name" ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
        argument = one_argument( argument, arg1 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Syntax:  name [direction] [string]\n\r", ch );
	    return;
	}
            if ( argument[0] == '\0' )
            {
                send_to_char( "Syntax:  name [direction] [string]\n\r", ch );
                return;
            }

            if ( pRoom->exit[door] == NULL )
            {
                send_to_char( "REdit:  Exit does not exists.\n\r", ch );
                return;
            }

            strncpy( pRoom->exit[door]->keyword, argument, 30 );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Exit name set.\n\r", ch );
            return;
    }


    if ( !str_cmp( arg1, "desc" ) || !strncmp( arg1, "description", 5 ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  || arg2[0] == '\0' )
	{
	    send_to_char( "Syntax:  desc [direction] [string]\n\r", ch );
	    return;
	}

        if ( pRoom->exit[door] == NULL )
        {
            send_to_char( "REdit:  Exit does not exists.\n\r", ch );
            return;
        }

  	strncpy( pRoom->exit[door]->description, arg2, 80 );
	send_to_char( "Exit Description Added.\n\r", ch );
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        return;
    }


    if ( !strncmp( arg1, "flags", 3 ) )
    if (edit_function(ch, REDIT_DOOR_MENU, SECURITY_9))
    {
       	argument = one_argument( argument, arg1 );
       	argument = one_argument( argument, arg2 );

	if ( ( door = get_direction_number_1( arg1 ) ) == MAX_DIR  )
	{
	    send_to_char( "Syntax:  flag [direction] <flag name>\n\r", ch );
	    sprintf(buf,"%s\n\r",get_exit_flag_string( ALL_FLAGS));
	    send_to_char(buf,ch);
	    return;
	}
            if ( pRoom->exit[door] == NULL )
            {
            send_to_char( "Sorry, there is not an exit in that direction\n\r", ch );
            return;
            }

        if ( get_exit_flag_number( arg2 ) == EX_NONE )
	{
	    send_to_char( "Syntax: flag [direction] <flag name>\n\r", ch );
	    sprintf(buf,"%s\n\r",get_exit_flag_string( ALL_FLAGS));
	    send_to_char(buf,ch);
	    return;
	}

            TOGGLE_BIT(pRoom->exit[door]->exit_info_default, get_exit_flag_number( arg2 ));
            TOGGLE_BIT(pRoom->exit[door]->exit_info, get_exit_flag_number( arg2 ));

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Exit flag toggled.\n\r", ch );
            return;
	
    }
	if (!strncmp(arg1,"add_trigger",5))
        if (edit_function(ch, REDIT_MENU, SECURITY_9))
	{ 
		TRIGGER_LIST_DATA *trigger_list_data;
		TRIGGER_INDEX_DATA *trigger;
		int s;
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("Syntax: add_trigger <vnum>\n\r",ch); 
			 send_to_char ("This is used to add a trigger to the room.\n\r",ch); 
			 send_to_char ("Rooms can have multimple triggers.  Use vnum trigger or tlist to find\n\r",ch);
			 send_to_char ("the one(s) you wish to add.\n\r",ch);
			 send_to_char("Current Triggers vnums:\n\r",ch);
			 for ( trigger_list_data = pRoom->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			 {
			 sprintf(buf," %d",trigger_list_data->vnum);
			 send_to_char(buf,ch);
			 }
			 send_to_char("\n\r",ch);
			 return;
		}
		s = atoi (arg2);
		if ( ( trigger = get_trig_index( s ) ) == NULL )
		{
		sprintf(buf,"trigger vnum %d NOT added\n\r",s);
		send_to_char (buf,ch);					    
		sprintf(buf,"vnum %d not found \n\r",s);
		send_to_char (buf,ch);					    
		return;
		}
		if ( trigger->script_type != TRIG_TYPE_ROOM )
		{
		sprintf(buf,"trigger vnum %d NOT added\n\r",s);
		send_to_char (buf,ch);					    
		sprintf(buf,"vnum %d is not a room trigger.  Set the Stype in tedit\n\r",s);
		send_to_char (buf,ch);					    
		return;
		}
		trigger_list_data = trigger_list_alloc();
		trigger_list_data->vnum = s;
		trigger_list_data->next = pRoom->trigger_list;
		pRoom->trigger_list = trigger_list_data;
		sprintf(buf,"trigger vnum %d added\n\r",s);
		send_to_char (buf,ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	}
	if (!strncmp(arg1,"remove_trigger",8))
        if (edit_function(ch, REDIT_MENU, SECURITY_9))
	{ 
		TRIGGER_LIST_DATA *trigger_list_data;
		int s, y = 0, n, xx[100];
		bool found = FALSE;
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("Syntax: remove_trigger <vnum>\n\r",ch); 
			 send_to_char ("This is used to remove a trigger from the room.\n\r",ch); 
			 send_to_char("Current Triggers vnums:\n\r",ch);
			 for ( trigger_list_data = pRoom->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			 {
			 sprintf(buf," %d",trigger_list_data->vnum);
			 send_to_char(buf,ch);
			 }
			 send_to_char("\n\r",ch);
			 return;
		}
		s = atoi (arg2);
		 for ( trigger_list_data = pRoom->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
	    	    {
	
			    if ( trigger_list_data->vnum == s )
			    {
				found = TRUE;
				continue;
			    }
			    else
			    {
			        xx[y] = trigger_list_data->vnum;
				y++;
			    }
	    	    }	
		pRoom->trigger_list = NULL;

		for ( n = 0 ; n < y ; n++ )
		   {
	        	trigger_list_data = trigger_list_alloc();
			trigger_list_data->vnum = xx[n];
			trigger_list_data->next = pRoom->trigger_list;
			pRoom->trigger_list = trigger_list_data; 

		   }
		if (!found)	
	    	send_to_char( "Trigger not found.\n\r", ch );
		else
		{
		sprintf(buf,"trigger vnum %d removed\n\r",s);
		send_to_char (buf,ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		}
	    	return;
	}

    if ( !strncmp( arg1, "guilds", 3 ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	long guild;

	if ( arg2[0] == '\0' )
	{
	    send_to_char( " Syntax: guilds <guild name>\n\r", ch );
	    sprintf(buf,"%s\n\r", guild_bit_name(ALL_FLAGS) );
	    send_to_char(buf,ch);
	    return;
	}

        if ( ( guild = guild_name_bit( arg2 ) ) < 1 )
	{
	    send_to_char( "Invalid guild name.\n\r", ch );
	    send_to_char( " Syntax: guilds <guild name>\n\r", ch );
	    sprintf(buf,"%s\n\r", guild_bit_name(ALL_FLAGS) );
	    send_to_char(buf,ch);
	    return;
	}
	
	TOGGLE_BIT( pRoom->guilds, guild );
	send_to_char( "Guild flag toggled.\n\r", ch );
	return;
    }


        if ( !str_cmp( arg1, "add" ) )
        if (edit_function(ch, REDIT_EXTRA_MENU, SECURITY_9))
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax: add [keyword]\n\r", ch );
                return;
            }

            ed                  =   new_extra_descr();
            strncpy( ed->keyword, arg2, 30 );
            ed->description     =   &str_empty[0];
            ed->next            =   pRoom->extra_descr;
            pRoom->extra_descr  =   ed;

            string_append( ch, &ed->description );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }


        if ( !str_cmp( arg1, "edit" ) )
        if (edit_function(ch, REDIT_EXTRA_MENU, SECURITY_9))
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax: edit [keyword]\n\r", ch );
                return;
            }

            for ( ed = pRoom->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
            }

            if ( ed == NULL )
            {
                send_to_char( "REdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            string_append( ch, &ed->description );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }


        if ( !strncmp( arg1, "append", 3 ) )
        if (edit_function(ch, REDIT_EXTRA_MENU, SECURITY_9))
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax: edit [keyword]\n\r", ch );
                return;
            }

            for ( ed = pRoom->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
            }

            if ( ed == NULL )
            {
                send_to_char( "REdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            string_append( ch, &ed->description );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }


        if ( !str_cmp( arg1, "delete" ) )
        if (edit_function(ch, REDIT_EXTRA_MENU, SECURITY_9))
        {
            EXTRA_DESCR_DATA *ped;

            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax: delete [keyword]\n\r", ch );
                return;
            }

            ped = NULL;

            for ( ed = pRoom->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
                ped = ed;
            }

            if ( ed == NULL )
            {
                send_to_char( "REdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            if ( ped == NULL )
            {
                pRoom->extra_descr = ed->next;
            }
            else
            {
                ped->next = ed->next;
            }

            free_extra_descr( ed );

            send_to_char( "Extra description deleted.\n\r", ch );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }


        if ( !str_cmp( arg1, "format" ) )
        if (edit_function(ch, REDIT_EXTRA_MENU, SECURITY_9))
        {
            EXTRA_DESCR_DATA *ped;

            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax: format [keyword]\n\r", ch );
                return;
            }

            ped = NULL;

            for ( ed = pRoom->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
                ped = ed;
            }

            if ( ed == NULL )
            {
                send_to_char( "REdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            ed->description = format_string( ed->description );

            send_to_char( "Extra description formatted.\n\r", ch );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }

    if ( !strncmp( arg1, "snooped_by" , 4) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	ROOM_SNOOP_LIST * room_snoop_list;
	ROOM_SNOOP_LIST * room_snoop_list_next;
	value = atoi( arg2 );

	if ( ( pRoomIndex = get_room_index( value ) ) == NULL )
	{
	    send_to_char( "That room does not exist.\n\r", ch );
	    return;
 	}

        if ( !IS_BUILDER( ch, pRoomIndex->area ) )
	{
	    send_to_char( "You are not a builder for that area.\n\r", ch );
	    return;
	}

	if ( pRoom == pRoomIndex )
	{
	    send_to_char( "You cannot have a room snooping itself.\n\r", ch );
	    return;
	}

        if ( pRoom->snoop_by != NULL && pRoom->snoop_by->room == pRoomIndex )
	{
	    room_snoop_list = pRoom->snoop_by;
	    pRoom->snoop_by = pRoom->snoop_by->next;
	    free_room_snoop_list( room_snoop_list );
	    sprintf( buf, "This room is no longer snooped by room %ld.\n\r", value );
	    send_to_char( buf, ch );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
	}

	for ( room_snoop_list = pRoom->snoop_by ; room_snoop_list != NULL ; room_snoop_list = room_snoop_list_next )
	{
	    room_snoop_list_next = room_snoop_list->next;

	    if ( room_snoop_list_next != NULL && room_snoop_list_next->room == pRoomIndex )
	    {
		room_snoop_list->next = room_snoop_list_next->next;
		free_room_snoop_list( room_snoop_list_next );
		sprintf( buf, "This room is no longer snooped by room %ld.\n\r", value );
		send_to_char( buf, ch );
                SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}

	room_snoop_list = room_snoop_list_alloc();
	room_snoop_list->room = pRoomIndex;
	room_snoop_list->next = pRoom->snoop_by;
	pRoom->snoop_by = room_snoop_list;

	sprintf( buf, "This room is now snooped by room %ld.\n\r", value );
	send_to_char( buf, ch );
	return;
    }

    if ( !str_cmp( arg1, "name" ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  name [name]\n\r", ch );
            return;
        }

        strncpy( pRoom->name, arg2, 80 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Name set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "desc" ) || !strncmp( arg1, "description", 5 ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' )
        {
            string_append( ch, &pRoom->description );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }

        if ( arg2[0] == '+' )
        {
            string_append( ch, &pRoom->description );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }

        send_to_char( "Syntax:  desc    - line edit\n\r", ch );
        send_to_char( "         desc +  - line append\n\r",ch );
        return;
    }


    if ( !str_cmp( arg1, "format" ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
        pRoom->description = format_string( pRoom->description );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "String formatted.\n\r", ch );
        return;
    }
    if ( !str_cmp( arg1, "loadtoroom" ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	if (ch->pnote == NULL) 
	{
 	    send_to_char ("You must make the note first.  Use note enter\n\r",ch);
	    return;
	}
	send_to_char ("Coping Note data to Room Description.\n\r",ch);
	ch->in_room->description = str_dup(ch->pnote->text);
	send_to_char ("Done\n\r",ch);
	send_to_char ("Remember to do a 'note clear'\n\r",ch);
	SET_BIT(ch->in_room->area->area_flags, AREA_CHANGED );
	return;
    }
    if ( !str_cmp( arg1, "loadtonote" ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
	send_to_char ("Loading Room description to note.\n\r",ch); 
	note_attach(ch);
	ch->pnote->text = str_dup( ch->in_room->description);
	send_to_char ("Done.",ch);
	SET_BIT(ch->in_room->area->area_flags, AREA_CHANGED );
	return;
    } 


    if ( !str_cmp( arg1, "size" ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {

	if ( arg2[0] == '\0' )
	{
	    if ( IS_SET(pRoom->room_flags, ROOM_INDOORS)
		|| pRoom->sector_type == SECT_UNDERGROUND )
		pRoom->size = 3;
	    else
		pRoom->size = 5;

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Size set.\n\r", ch);
	    return;
	}

	if ( !is_number(arg2) )
	{
	    send_to_char("Set size to what?\n\r", ch);
	    return;
	}

	if ( atoi( arg2 ) < 0 || atoi( arg2 ) > 5 )
	{
	    send_to_char("Size must be between 0 and 5.\n\r", ch);
	    return;
	}

	value_int = atoi( arg2);
	pRoom->size = value_int;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Size set.\n\r", ch);

	return;
    }

    if ( !strncmp( arg1, "remove", 3 ) )
    if (edit_function(ch, REDIT_SPEC_MENU, SECURITY_9))
    {
	SPECPRO_LIST  *s;
	SPECPRO_LIST  *spec;

        argument = one_argument( argument, arg1 );

        if ( arg1[0] == '\0' || !is_number( arg1 ) )
        {
            send_to_char( "Syntax:  remove [special process number]\n\r", ch );
	    do_specroom(ch,"");
            return;
        }

	spec = NULL;

	for ( s = pRoom->spro; s != NULL; s = s->next )
	{
	    if ( atoi(arg1) == s->num)
		break;
	    spec = s;
	}
	if ( s == NULL )
	{
            send_to_char( "Special not found\n\r", ch );
	    return;
	}
	if ( spec == NULL )
	{
	    pRoom->spro = s->next;
	}
	else
	{
	    spec->next = s->next;
	}
	free_spec_pro( s );

	send_to_char ("Spec Pro removed from Room.\n\r",ch);
	SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	return;
 
    }
    if ( !strncmp( arg1, "remove", 3 ) )
    if (edit_function(ch, REDIT_EVENT_MENU, SECURITY_9))
    {
	EVENT_DATA  *event;
	EVENT_DATA *event_data;
	bool fAll = FALSE;

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );

        if ( arg1[0] == '\0' || !is_number( arg1 ) )
        {
            send_to_char( "Syntax:  remove [event number]\n\r", ch );
	    redit(ch,"list");
            return;
        }

	fAll = !str_cmp(arg2, "all");
	event_data = NULL;

	for ( event = pArea->events; event != NULL; event = event->next )
	{

	 /*
	  * Make sure they are in the right room 
	  */

	    if ( event->room_vnum == pRoom->vnum || fAll )
	    if ( event->vnum == atoi(arg1) )
		break; 
	    event_data = event;
	
	}
	if ( event == NULL )
	{
            send_to_char( "Event not found\n\r", ch );
	    return;
	}
	if ( event_data == NULL )
	{
	    pArea->events = event->next;
	}
	else
	{
	    event_data->next = event->next;
	}
	if (event->mob)
	event->mob->event = NULL;
	if (event->obj)
	event->obj->event = NULL;
	free_event_data( event );

	send_to_char ("Event removed from Room.\n\r",ch);
	SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	return;
 
    }
    if ( !strncmp( arg1, "events", 4 ) )
    if (edit_function(ch, REDIT_EVENT_MENU, SECURITY_9))
    {
	EVENT_DATA   *event;
	bool found = FALSE;

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
        argument = one_argument( argument, arg3 );

        if ( arg1[0] != '\0' && arg2[0] != '\0' && is_number( arg2 ) )
        {
	if ( !strncmp( arg1, "mobile",3) )
	{
	    if ( ( pMob = get_mob_index( atoi( arg2 ) ) ) == NULL ) 	
	    {
	        send_to_char("Invalid vnum for mobile\n\r",ch);
	        return;
	    }
	    
	    if ( pArea != pMob->load_with_area && pMob->load_with_area->areanum != 3 )
	    {
	    	send_to_char("Sorry!  Mob is not from this area. \n\r",ch);
	    	send_to_char("Duplicate the mob to this area if you wish.\n\r",ch);
	    	send_to_char("Event Not created!",ch);
	    	return;
	    }
	    event = event_alloc();
	    event->next = pArea->events;
	    pArea->events = event;
	    event->type = EVENT_ROOM;
	    event->room_vnum = pRoom->vnum;
	    event->area = pArea;
	    event->flags = EVENT_ON_BOOT|EVENT_ON_RESET;
	    event->chance = 100;
	    sprintf(buf,"New event with vnum %ld created\n\r",event->vnum);
	    send_to_char(buf,ch);
	    SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	    event->mob_vnum = pMob->vnum;
	    send_to_char("Event for mob set\n\r",ch);
	    return;
	}
	if ( !strncmp( arg1, "object",3) )
	{
	    if ( ( pObj = get_obj_index( atoi( arg2 ) ) ) == NULL ) 	
	    {
	        send_to_char("Invalid vnum for object\n\r",ch);
	        return;
	    }
	    if ( pArea != pObj->in_area && pObj->in_area->areanum != 3 )
	    {
	    	send_to_char("Sorry!  Obj is not from this area. \n\r",ch);
	    	send_to_char("Duplicate the obj to this area if you wish.\n\r",ch);
	    	send_to_char("Event Not created!",ch);
	    	return;
	    }
	    event = event_alloc();
	    event->next = pArea->events;
	    pArea->events = event;
	    event->type = EVENT_ROOM;
	    event->room_vnum = pRoom->vnum;
	    event->area = pArea;
	    event->flags = EVENT_ON_BOOT|EVENT_ON_RESET;
	    sprintf(buf,"New event with vnum %ld created\n\r",event->vnum);
	    send_to_char(buf,ch);
	    SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	    event->obj_vnum = pObj->vnum;
	    event->chance = pObj->points == 0 ? 100 : pObj->points;
	    send_to_char("Event for obj set\n\r",ch);
	    return;
	}
        }

        if ( arg1[0] == '\0' || !is_number( arg1 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  event <vnum> chance <percent>\n\r", ch );
            send_to_char( "Syntax:  event <vnum> flags <event flags>\n\r", ch );
            send_to_char( "Syntax:  event <vnum> mobile <mob_vnum>\n\r", ch );
            send_to_char( "Syntax:  event <vnum> object <obj_vnum>\n\r", ch );
	    send_to_char("Flags\n\r",ch);
	    sprintf(buf,"%s\n\r",get_event_flags( ALL_FLAGS ) );
	    send_to_char(buf,ch);
            return;
        }
	for ( event = pArea->events; event != NULL; event = event->next )
	{

	 /*
	  * Make sure they are in the right room 
	  */

	    if ( event->room_vnum == pRoom->vnum)
	    if ( event->vnum == atoi(arg1) )
	    {
		found = TRUE;
		break; 
	    }
	
	}
	if ( !found )
	{
	    send_to_char("Event vnum not found for this room\n\r",ch);
	    return;
	}
	if ( !strncmp( arg2, "mobile",3) )
	{
	    if ( atoi( arg3 ) == 0 )
	    {
		event->mob_vnum = 0;
		send_to_char("Event for mobile set to 0\n\r",ch);
	        SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
		return;
	    }
	    if ( ( pMob = get_mob_index( atoi( arg3 ) ) ) == NULL ) 	
	    {
	        send_to_char("Invalid vnum for mobile\n\r",ch);
	        return;
	    }
	    if ( pArea != pMob->load_with_area )
	    {
	    	send_to_char("Sorry!  Mob is not from this area. \n\r",ch);
	    	send_to_char("Duplicate the mob to this area if you wish.\n\r",ch);
	    	send_to_char("Event Not created!",ch);
	    	return;
	    }
	    event->mob_vnum = pMob->vnum;
	    send_to_char("Event for mob set\n\r",ch);
	    SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	    return;
	}
	if ( !strncmp( arg2, "object",3) )
	{
	    if ( atoi( arg3 ) == 0 )
	    {
		event->obj_vnum = 0;
		send_to_char("Event for object set to 0\n\r",ch);
	        SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
		return;
	    }
	    if ( ( pObj = get_obj_index( atoi( arg3 ) ) ) == NULL ) 	
	    {
	        send_to_char("Invalid vnum for object\n\r",ch);
	        return;
	    }
	    if ( pArea != pObj->in_area )
	    {
	    	send_to_char("Sorry!  Obj is not from this area. \n\r",ch);
	    	send_to_char("Duplicate the obj to this area if you wish.\n\r",ch);
	    	send_to_char("Event Not created!",ch);
	    	return;
	    }
	    event->obj_vnum = pObj->vnum;
	    send_to_char("Event for obj set\n\r",ch);
	    SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	    return;
	}
	if ( !strncmp( arg2, "chance", 3 ) )
	{
	    if ( arg3[0] == '\0' )
	    {
		send_to_char("Chance is between 1 and 100\n\r",ch);
		return;
	    }
	    event->chance = atoi(arg3);
	    send_to_char("Chance set\n\r",ch);
	    SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	    return;
	}
	if ( !strncmp( arg2, "flags", 3 ) )
	{
	    if ( arg3[0] == '\0' )
	    {
		send_to_char("Flags\n\r",ch);
		sprintf(buf,"%s\n\r",get_event_flags( ALL_FLAGS ) );
		send_to_char(buf,ch);
		return;
	    }
	    if ( ( get_flags_event( arg3 ) ) == 0 ) 
	    {
		send_to_char("Invalid flag\n\r",ch);
		sprintf(buf,"%s\n\r",get_event_flags( ALL_FLAGS ) );
		send_to_char(buf,ch);
		return;
	    }
	    TOGGLE_BIT(event->flags, get_flags_event( arg3 ));
	    send_to_char("Flag Toggled\n\r",ch);
	    SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	    return;
	}
	return;
    }
    if ( !str_cmp( arg1, "create" ) )
    if (edit_function(ch, REDIT_EVENT_MENU, SECURITY_9))
    {
	EVENT_DATA   *event;

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );

	event = event_alloc();
	event->next = pArea->events;
	pArea->events = event;
	event->type = EVENT_ROOM;
	event->room_vnum = pRoom->vnum;
	event->area = pArea;
	event->flags = EVENT_ON_BOOT|EVENT_ON_RESET;
	sprintf(buf,"New event with vnum %ld created\n\r",event->vnum);
	send_to_char(buf,ch);
	SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	return;
    }
    if ( !str_cmp( arg1, "list" ) )
    if (edit_function(ch, REDIT_EVENT_MENU, SECURITY_9))
    {
 	bool fAll;
	EVENT_DATA   *event;
	BUFFER *output;
	output = new_buf();
        argument = one_argument( argument, arg1 );

	fAll = !str_cmp(arg1, "all");

	for ( event = pArea->events; event != NULL; event = event->next )
	{
	    if ( event->type == EVENT_ROOM )
	    if ( event->room_vnum == pRoom->vnum || (fAll && pRoom->area == event->area))
	    {
		sprintf(buf,"`G%-4ld `wChance `G%-3d `wMob `G%-5ld `wObj `G%-5ld `wRoom `G%-5ld `wFlags `G%s`w\n\r",
		event->vnum, event->chance, event->mob_vnum, event->obj_vnum, event->room_vnum, get_event_flags(event->flags));
		add_buf(output, buf);
	    }
	}
	page_to_char(buf_string(output),ch);
	free_buf(output);
	return;
    }
    if ( !str_cmp( arg1, "cleanfixitsomeday" ) )
    if (edit_function(ch, REDIT_EVENT_MENU, SECURITY_9))
    {
 	bool fAll;
	EVENT_DATA   *event;
	EVENT_DATA   *event_next;
        argument = one_argument( argument, arg1 );

	fAll = !str_cmp(arg1, "all");

        for ( event = pArea->events ; event != NULL ; event = event_next )
	{
	    event_next = event->next;
	    if (!event_next)	    
		break;

	    if ( event_next->type == EVENT_ROOM && IS_SET(event_next->flags, EVENT_INVALID))
	    if ( event_next->room_vnum == pRoom->vnum || (fAll && pRoom->area == event_next->area))
	    {
			
		event->next = event_next->next;
		if (event_next->mob)
		event_next->mob->event = NULL;
		if (event_next->obj)
		event_next->obj->event = NULL;
		free_event_data( event_next );
	    }
	}
	send_to_char("Events cleaned\n\r",ch);
	return;
    }
    if ( !str_cmp( arg1, "add" ) )
    if (edit_function(ch, REDIT_SPEC_MENU, SECURITY_9))
    {
	SPECPRO_LIST  *s;

        argument = one_argument( argument, arg1 );

        if ( arg1[0] == '\0' || !is_number( arg1 ) )
        {
            send_to_char( "Syntax:  add [special process number]\n\r", ch );
	    list_spec_pros(ch,"");
            return;
        }
 
	value = atoi (arg1);
	if (value < 0 || value >= MAX_SPECPRO)
	{ 
	    send_to_char ("Invalid Spec Pro Number.\n\r",ch);
	    list_spec_pros(ch,"");
	    return;
	}
	s = specpro_list_alloc();
	s->next = pRoom->spro;
	pRoom->spro = s;
	s->sp = &spec_pro_list[value];
	s->num = s->sp->spn;
	send_to_char ("Spec Pro Added to Room.\n\r",ch);
	SET_BIT(pRoom->area->area_flags, AREA_CHANGED );
	return;
    }
    if ( !str_cmp( arg1, "light" ) )
    if (edit_function(ch, REDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' || !is_number( arg2 ) )
        {
            send_to_char( "Syntax:  light [number]\n\r", ch );
            return;
        }

	if ( atoi( arg2 ) < 0 || atoi( arg2 ) > 100 )
	{
	    send_to_char("Light must be between 0 and 100.\n\r", ch);
	    return;
	}

        value_int = atoi( arg2 );
        pRoom->light = value_int;

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Light set.\n\r", ch);
        return;
    }


    interpret( ch, arg );
    return;
}
void do_redit( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoom;
    AREA_DATA *pArea;
    int value;
    long vnum;
    char arg1[MAX_STRING_LENGTH];
    if ( IS_NPC(ch) ) return;

    argument = one_argument( argument, arg1 );

    pRoom = ch->in_room;

    if ( is_number( arg1 ) )
    {
        value = atoi( arg1 );
        if ( ( pRoom = get_room_index( value ) ) == NULL )
        {
            send_to_char( "REdit:  That vnum does not exist.\n\r", ch );
            return;
        }
    }
    else
    {
        if ( !str_cmp( arg1, "create" ) )
        {
            value = atoi( argument );
	    if ( argument[0] == '\0' )
 	    {
		for ( vnum = pRoom->area->lvnum ; free_room[ vnum ] ; vnum++ );

		if ( vnum <= pRoom->area->uvnum )
		    value = vnum;
	    }

            if ( value == 0 )
            {
                send_to_char( "Syntax:  redit create [vnum]\n\r", ch );
                return;
            }

	    pArea = pRoom->area;

            if ( pArea->lvnum > value || pArea->uvnum < value )
            {
                send_to_char( "REdit:  That vnum is not assigned to this area.\n\r", ch );
                return;
            }

            if ( !IS_BUILDER( ch, pRoom->area ) )
            {
                send_to_char( "REdit:  Vnum in an area you cannot build in.\n\r", ch );
                return;
            }

            if ( get_room_index( value ) != NULL )
            {
                send_to_char( "REdit:  Room vnum already exists.\n\r", ch );
                return;
            }
	
	    if ( ( pRoom = create_room( ch, pArea, value ) ) == NULL )
		return;

            SET_BIT( pRoom->area->area_flags, AREA_CHANGED );
 	    if (ch->inside_area != NULL )
	    free_inside_area_data(ch->inside_area);
	    ch->inside_area = NULL;
            char_from_room( ch );
            char_to_room( ch, pRoom );
        }
    }

    ch->desc->connected = CON_REDITOR;
    ch->edit_menu = REDIT_MENU;
    ch->edit_buffer[0] = '\0';
    return;
}
ROOM_INDEX_DATA *create_room( CHAR_DATA *ch, AREA_DATA *pArea, long vnum )
{ 
    ROOM_INDEX_DATA * pRoomIndex;
    char buf[MAX_STRING_LENGTH];
    int iHash;
  
    if ( vnum < 1 )
    if ( pArea->lvnum == 0 || pArea->uvnum == 0 )
    {
	send_to_char("The vnum's for this area have not been set.\n\r",ch);
	send_to_char("Room NOT created.\n\r",ch);
	return NULL;
    }

    if ( vnum < 1 )
    for ( vnum = pArea->lvnum; free_room[vnum]; vnum++ ); /*Searches the vnum list till it finds a free slot*/ 

    if (vnum > pArea->uvnum || vnum < pArea->lvnum)
    {
	send_to_char("Vnum out of range for area.  This area must be out of rooms.\n\r",ch);
	send_to_char("Room NOT created.\n\r",ch);
	return NULL;	
    }

    if (free_room[vnum])
    {
	send_to_char("Vnum already in use\n\r",ch);
	send_to_char("Room NOT created.\n\r",ch);
	return NULL;	
    }

    free_room[vnum] = 1;

    pRoomIndex			= pRoomIndex_alloc();
    pRoomIndex->vnum		= vnum;
    pRoomIndex->area		= pArea;
    strncpy( pRoomIndex->name, "A formless void", 80 );
    pRoomIndex->description	= str_dup ("Waves of chaos congeal, twist, bend to form in the begining of \n\r some area. This most definitly is the work of a God's hand.\n\r");
    iHash			= vnum % MAX_KEY_HASH;
    pRoomIndex->next		= room_index_hash[iHash];
    room_index_hash[iHash]	= pRoomIndex;

    if (LOGON_ON)
    if (ch->logon_data != NULL)
	ch->logon_data->rooms++;

    sprintf (buf,"Room Number [%ld] created\n\r",vnum);
    send_to_char (buf,ch);
    SET_BIT(ch->in_room->area->area_flags, AREA_CHANGED );
    return pRoomIndex;
}

void make_exit(ROOM_INDEX_DATA *pRoom, ROOM_INDEX_DATA *pRoomIndex, int dir ) 
{
    EXIT_DATA *pExit; 

    if ( pRoom->exit[dir] == NULL )
	pExit = new_exit();    
    else
	pExit = pRoom->exit[dir];

    pExit->keyword[0]		= '\0';
    pExit->description[0]	= '\0';
    pExit->exit_info		= 0; 
    pExit->exit_info_default	= 0; 
    pExit->u1.vnum		= pRoomIndex->vnum;
    pExit->u1.to_room		= pRoomIndex; 

    pRoom->exit[dir] 		= pExit;
}

LOCK_DATA  *get_lock_data( int vnum, LOCK_DATA * lock)
{
LOCK_DATA *lock_next;
LOCK_DATA *lock_list;
		for ( lock_list = lock ; lock_list != NULL; lock_list = lock_next)
		 {
		  lock_next = lock_list->next;
		  if (lock_list->vnum == vnum)
		  return lock_list; 
		  }		   
	return NULL;
}




