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

/*
 * You can define verbose if you want details printed out when
 *  resets are saved. 
 */

/*
 * #define VERBOSE 
 */

/*
 * #define SKILLSN(sn) (sn != -1 ? skill_table[sn].slot : 0)
 */

/*
 * Globals
 */
int			CiSocial;
extern int		social_count;
char			sedit_in_use[31];

HELP_DATA *get_help_data( int vnum );

AREA_DATA *get_area_data( int vnum )
{
    AREA_DATA *pArea;

    for (pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
        if (pArea->areanum == vnum)
            return pArea;
    }

    return NULL;
}



AREA_DATA *get_vnum_area( long vnum )
{
    AREA_DATA *pArea;

    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
        if ( vnum >= pArea->lvnum
          && vnum <= pArea->uvnum )
            return pArea;
    }

    return NULL;
}

char *area_bit_name( int area_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( area_flags & AREA_CHANGED      )   strcat( buf, " CHANGED" );
        else                                strcat( buf, " changed" );
    if ( area_flags & AREA_ADDED        )   strcat( buf, " ADDED" );
        else                                strcat( buf, " added" );
    if ( area_flags & AREA_AUTOSAVE     )   strcat( buf, " AUTOSAVE" );
        else                                strcat( buf, " autosave" );
    if ( area_flags & AREA_PKSAFE	)   strcat( buf, " PKSAFE" );
	else				    strcat( buf, " pksafe" );
    return buf+1;
}



int get_area_flags_number( char *argument )
{
    if ( !str_cmp( argument, "changed" ) )      return AREA_CHANGED;
    if ( !str_cmp( argument, "added" ) )        return AREA_ADDED;
    if ( !str_cmp( argument, "no_teleport" ) )    return AREA_NO_TELEPORT;
    if ( !str_cmp( argument, "pksafe" ) )	return AREA_PKSAFE;
    return AREA_NONE;
}



void sedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    strcpy( arg, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );
    pArea = ch->in_room->area;
    

    if ( !str_cmp( arg1, "show" ) || arg1 == '\0' )
    {
        sprintf( buf, "%s", social_table[CiSocial].name );
        do_sshow( ch, buf );
        sprintf( buf, "%d", CiSocial);
	send_to_char(buf,ch);	
        return;
    }


    if ( !str_cmp( arg1, "done" ) )
    {
	if ( !str_cmp(social_table[CiSocial].name,"new_social_name") )
	{
		send_to_char("Social not saved!\n\r", ch); 
		social_count--;
	}
	sedit_in_use[0] = '\0';
        ch->desc->pEdit = NULL;
        ch->desc->connected = CON_PLAYING;
        return;
    }


    if ( !str_cmp( arg1, "?" ) )
    {
        do_help( ch, "sedit" );
        return;
    }

    if ( !str_cmp( arg1, "create" ) )
    {
	CiSocial = social_count++;
	  strcpy(social_table[CiSocial].name,"new_social_name"); 
	  social_table[CiSocial].char_no_arg[0] = '\0';
	  social_table[CiSocial].others_no_arg[0] = '\0';
	  social_table[CiSocial].char_found[0] = '\0';
	  social_table[CiSocial].others_found[0] = '\0';
	  social_table[CiSocial].vict_found[0] = '\0';
	  social_table[CiSocial].char_not_found[0] = '\0';
	  social_table[CiSocial].char_auto[0] = '\0';
	  social_table[CiSocial].others_auto[0] = '\0';
	  social_table[CiSocial].type = 0;
	  social_table[CiSocial].damage = 0;
	  social_table[CiSocial].item = 0;
	  social_table[CiSocial].action = 0;
	  social_table[CiSocial].mood = 0;
          SET_BIT(changed_system, CHANGED_SOCIALS);
       return;
    }



    if ( !str_cmp( arg1, "mood" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  type [#mood]\n\r", ch );
            send_to_char( "Increase or Decrease the Mobiles mood.\n\r", ch );
            send_to_char( "-1 to Decrease -- Makes mob in a worse mood\n\r", ch );
            send_to_char( " 0 to Neither  -- Mobs mood does not change\n\r", ch );
            send_to_char( " 1 to Increase -- Makes mob in a better mood\n\r", ch );
            return;
        }

        social_table[CiSocial].mood = atoi( arg2 );
        send_to_char( "Mood set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "action" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  type [#action]\n\r", ch );
            return;
        }

        social_table[CiSocial].action = atoi( arg2 );
        send_to_char( "Action set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "item" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  type [#item] ( vnum of object )\n\r", ch );
            send_to_char( "Gives mob or player this item.\n\r", ch );
            send_to_char( "Example: rose (char_name)\n\r", ch );
            send_to_char( "The charactor now has a rose in his/her inventory.\n\r", ch );
            send_to_char( "It is a good idea to use useless items here as they\n\r", ch );
            send_to_char( "can be done over and over and by anyone.\n\r", ch );
            return;
        }

        social_table[CiSocial].item = atoi( arg2 );
        send_to_char( "Item set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "damage" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  type [#damage]\n\r", ch );
            send_to_char( "Currently there are just too valid choices.\n\r", ch );
            send_to_char( "0 do nothing\n\r", ch );
            send_to_char( "1 Will decrease charactors hit points by one.\n\r", ch );
            send_to_char( "This is only possible when a mob does a social to\n\r", ch );
            send_to_char( "the charactor.  A reaction to a negative social from a charactor\n\r", ch );
            send_to_char( "Or just a pissed of mob out bonking charactors.\n\r", ch );
            return;
        }

        social_table[CiSocial].damage = atoi( arg2 );
        send_to_char( "Damage set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "type" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  type [#type]\n\r", ch );
            send_to_char( "0   Neutral\n\r", ch );
            send_to_char( "5   Greeting negitive\n\r", ch );
            send_to_char( "25  Greeting positive\n\r", ch );
            send_to_char( "50  Offensive negitive \n\r", ch );
            send_to_char( "100 Offensive positive \n\r", ch );
            send_to_char( "200 Sexual \n\r", ch );
            send_to_char( "250 Reaction negative\n\r", ch );
            send_to_char( "300 Reaction positive\n\r", ch );
            send_to_char( "\n\r", ch );
            return;
        }

        social_table[CiSocial].type = atoi( arg2 );
        send_to_char( "Type set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "others_auto" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   others_auto [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].others_auto, arg2, 80 );
        send_to_char( "Others_auto  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "char_auto" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   char_auto [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].char_auto, arg2, 80 );
        send_to_char( "Char_auto  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "vict_not_found" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   vict_not_found [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].char_not_found, arg2, 80 );
        send_to_char( "Vict_not_found  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "vict_found" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   vict_found [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].vict_found, arg2, 80 );
        send_to_char( "Vict_found  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "others_found" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   others_found [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].others_found, arg2, 80 );
        send_to_char( "Others_found  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "char_found" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   char_found [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].char_found, arg2, 80 );
        send_to_char( "Char_found  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "others_no_arg" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   others_no_arg [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].others_no_arg, arg2, 80 );
        send_to_char( "Others_no_arg  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "char_no_arg" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   char_no_arg [$String]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].char_no_arg, arg2, 80 );
        send_to_char( "Char_no_arg  set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    if ( !str_cmp( arg1, "name" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   name [$name]\n\r", ch );
               return;
        }

        strncpy( social_table[CiSocial].name, arg2, 20 );

        send_to_char( "Name set.\n\r", ch );
        SET_BIT(changed_system, CHANGED_SOCIALS);
        return;
    }

    interpret( ch, arg );
    return;
}
void hedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    HELP_DATA *pHelp;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    pHelp = (HELP_DATA *)ch->desc->pEdit;
    strcpy( arg, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );
    pArea = ch->in_room->area;


    if ( !str_cmp( arg1, "show" ) || arg1 == '\0' )
    {
        sprintf( buf, "%d", pHelp->vnum );
        do_hshow( ch, buf );
        return;
    }


    if ( !str_cmp( arg1, "done" ) )
    {
        ch->desc->pEdit = NULL;
        ch->desc->connected = CON_PLAYING;
        return;
    }


    if ( !str_cmp( arg1, "?" ) )
    {
        do_help( ch, "hedit" );
        return;
    }


    if ( !IS_BUILDER( ch, pArea ) )
    {
        interpret( ch, arg );
        return;
    }



    if ( !str_cmp( arg1, "create" ) )
    {
        pHelp               =   new_help();
	help_last->next	    =	pHelp;
        ch->desc->pEdit     =   (void *)pHelp;
	help_last	    =	pHelp;
	pHelp->next	    =	NULL;
        sprintf( buf, "%d", pHelp->vnum );
        do_hshow( ch, buf );
	SET_BIT(changed_system, CHANGED_HELPS);
       return;
    }



    if ( !strncmp( arg1, "keywords",3 ) )
    {
	if (pHelp->vnum == DEFAULT_HELP_VNUM)
	{
        	sprintf( buf, "%d", DEFAULT_HELP_VNUM );
		do_hshow( ch, buf);
		return;
	}
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   keyword [$keyword]\n\r", ch );
               return;
        }

        strncpy( pHelp->keyword,  arg2, 50 );

        send_to_char( "Keyword set.\n\r", ch );
	SET_BIT(changed_system, CHANGED_HELPS);
        return;
    }

    if ( !str_cmp( arg1, "level" ) )
    {
	if (pHelp->vnum == DEFAULT_HELP_VNUM)
	{
        	sprintf( buf, "%d", DEFAULT_HELP_VNUM );
		do_hshow( ch, buf);
		return;
	}
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  level [#level]\n\r", ch );
            return;
        }

        pHelp->level = atoi( arg2 );

        send_to_char( "Level set.\n\r", ch );
	SET_BIT(changed_system, CHANGED_HELPS);
        return;
    }


    if ( !strncmp( arg1, "text",3 ) )
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  text    - line edit\n\r", ch );
            send_to_char( "         text +  - line append\n\r",ch );
	    send_to_char( "         text n  - append current note\n\r",ch );
	    send_to_char( "Entering Editor\n\r",ch );
            string_append( ch, &pHelp->text );
	    SET_BIT(changed_system, CHANGED_HELPS);
            return;
        }

        if ( arg2[0] == '+' )
        {
            string_append( ch, &pHelp->text );
	    SET_BIT(changed_system, CHANGED_HELPS);
            return;
        }

        if ( arg2[0] == 'n' )
        {
	    if ( ch->pnote == NULL || strlen( ch->pnote->text ) == 0 )
	    {
		send_to_char("Note empty\n\r", ch );
		return;
	    }
	    pHelp->text = str_dup( ch->pnote->text );
	    SET_BIT(changed_system, CHANGED_HELPS);
            return;
        }

        send_to_char( "Syntax:  text    - line edit\n\r", ch );
        send_to_char( "         text +  - line append\n\r",ch );
	send_to_char( "         text n  - append current note\n\r",ch );
        return;
    }



    interpret( ch, arg );
    return;
}



void aedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    int  value,season;
    int value1,value2,value3;
    long vnum;
    int iHash;
    bool found = FALSE;
    pArea = (AREA_DATA *)ch->desc->pEdit;
    strcpy( arg, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    if ( !IS_BUILDER( ch, pArea ) )
    {
        send_to_char( "AEdit:  Insufficient security to modify area.\n\r", ch );
    }


    if ( !str_cmp( arg1, "show" ) || arg1[0] == '\0' )
    {
        sprintf( buf, "%d", pArea->areanum );
        do_ashow( ch, buf );
        return;
    }


    if ( !str_cmp( arg1, "done" ) )
    {
        ch->desc->pEdit = NULL;
        ch->desc->connected = CON_PLAYING;
        return;
    }


    if ( !str_cmp( arg1, "?" ) )
    {
        do_help( ch, "aedit" );
        return;
    }


    if ( !IS_BUILDER( ch, pArea ) )
    {
        interpret( ch, arg );
        return;
    }


    if ( ( value = get_area_flags_number( arg1 ) ) != AREA_NONE )
    {
        TOGGLE_BIT(pArea->area_flags, value);

        send_to_char( "Flag toggled.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "create" ) )
    {
	int num_of_rooms;
        long x;	
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax: create <number of rooms>\n\r", ch );
            return;
        }

	one_argument( arg2, arg );
	num_of_rooms = atoi( arg );
	if ( !is_number( arg ) || num_of_rooms < 0 || num_of_rooms > 1000)
	{
	    send_to_char( "Invalid Number of rooms\n\r", ch );
	    return;
	}

    for ( x = 100; x < MAX_VNUM; x+=100 )
    {
    found = FALSE;
    	for ( vnum = x -1; vnum < num_of_rooms + x -1; vnum++ )
    	{
		if ( get_vnum_area( vnum ) != NULL )
		{
		found = TRUE;
		break;
		}
    	}
	if (!found)
	break;	
    }
    if ( found )
	{
	send_to_char( "No more rooms!\n\rArea not created.\n\r", ch );
	return;
	}
	
        pArea               =   new_area();
        area_last->next     =   pArea;
        ch->desc->pEdit     =   (void *)pArea;
        area_last	    =   pArea;
	pArea->next	    =   NULL;	
	strncpy( pArea->name, "New Area", 80 );
	sprintf( buf, "area%d.are", pArea->areanum );
	strncpy( pArea->filename, buf, 30 );
        pRoom               = pRoomIndex_alloc();
        strncpy( pRoom->name, "A formless void", 80 );
        pRoom->description	= str_dup ("Waves of chaos congeal, twist, bend to from in the begining of \n\r some area. This most definitly is the work of God's hand.\n\r");
	for (vnum = x ; free_room[vnum]; vnum++ );
	pRoom->vnum = vnum;
	free_room[vnum] = 1;
        pRoom->area             = pArea;

        iHash                   = vnum % MAX_KEY_HASH;
        pRoom->next             = room_index_hash[iHash];
        room_index_hash[iHash]  = pRoom;
        ch->desc->pEdit         = (void *)pRoom;
	if (LOGON_ON)
	if (ch->logon_data != NULL )
	ch->logon_data->rooms++;
	SET_BIT( pArea->area_flags, AREA_AUTOSAVE );
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Area / Room created.\n\r", ch );
	if (ch->inside_area != NULL )
	free_inside_area_data(ch->inside_area);
	ch->inside_area = NULL;
        char_from_room( ch );
        char_to_room( ch, pRoom );
        sprintf( buf, "%d", pArea->areanum ); /* re-read the area data */
        do_aedit( ch, buf );
	pArea->lvnum = x;
	pArea->uvnum = x + num_of_rooms -1; 
        return;
    }



    if ( !str_cmp( arg1, "visible" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   visible [#level]\n\r", ch );
               return;
        }

        pArea->visible = atoi( arg2 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Visible Level set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "name" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   name [$name]\n\r", ch );
               return;
        }

        strncpy( pArea->name, arg2, 80 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Name set.\n\r", ch );
        return;
    }

    if ( !strncmp( arg1, "credits", 4 ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   credits [$credits]\n\r", ch );
               return;
        }

        strncpy( pArea->credits, arg2, 30 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Credits set.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "name" ) )
    {
        if ( arg2[0] == '\0' )
        {
               send_to_char( "Syntax:   name [$name]\n\r", ch );
               return;
        }

        strncpy( pArea->name, arg2, 80 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Name set.\n\r", ch );
        return;
    }



    if ( !str_cmp( arg1, "filename" ) || !str_cmp( arg1, "file" ) )
    {
        if ( argument[0] == '\0' )
        {
            send_to_char( "Syntax:  filename [$file]\n\r", ch );
            send_to_char( "or       file     [$file]\n\r", ch );
            return;
        }

        strncpy( pArea->filename, arg2, 30 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Filename set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "age" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  age [#age]\n\r", ch );
            return;
        }

        pArea->age = atoi( arg2 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Age set.\n\r", ch );
        return;
    }

    if ( !strncmp( arg1, "wander_time", 3 ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  wander_time [#wander_time]\n\r", ch );
            send_to_char( "Wander_time    Chance of wandering\n\r", ch );
            send_to_char( "0                    100\n\r", ch );
            send_to_char( "1                    50\n\r", ch );
            send_to_char( "2                    33\n\r", ch );
            send_to_char( "3                    25\n\r", ch );
            send_to_char( "4                    20\n\r", ch );
            send_to_char( "100/(x+1)\n\r", ch );
            return;
        }

        pArea->wander_time = atoi( arg2 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Wander Time set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "world" ) )
    {
	argument = one_argument(argument, arg1);
        if ( arg1[0] == '\0' )
        {
            send_to_char( "Syntax:  world <name>\n\r", ch );
            return;
        }
	for ( value = 0; value < MAX_WORLD; value++)
	if ( !strncmp(arg1, world_name_olc[value], 3) )
	{
	    found = TRUE;
	    break;
	}

        if ( !found )
        {
            send_to_char( "Invalid world\n\r", ch );
	    for ( value = 0; value < MAX_WORLD; value++)
	    {
		send_to_char(world_name[value], ch);
		send_to_char(" ", ch);
	    }
	    send_to_char("\n\r", ch);
            return;
        }
        pArea->world = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "World set.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "chance" ) )
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  chance <season> <cloudless> <cloudy> <raining>\n\r", ch );
	    send_to_char( "Expample: chance summer 25 25 25\n\r   (means equal probabilty of all weather type).\n\r", ch );
            return;
        }

	argument = one_argument( arg2, arg );
	if ( !str_cmp( arg , "spring" ) ) season = 0;
	else if ( !str_cmp( arg , "summer" ) ) season = 1;
	else if ( !str_cmp( arg , "fall" ) ) season = 2;
	else if ( !str_cmp( arg , "winter" ) ) season = 3;
	else
	{
	    send_to_char( "Invalid season.\n\r", ch );
	    return;
	}

	argument = one_argument( argument, arg1 );
	if ( !is_number( arg1 ) )
	{
	    send_to_char( "Invalid value.\n\r", ch );
	    return;
	}
	value1 = atoi( arg1 );
	argument = one_argument( argument, arg1 );
	if ( !is_number( arg1 ) )
	{
	    send_to_char( "Invalid value.\n\r", ch );
	    return;
	}
	value2 = atoi( arg1 );
	argument = one_argument( argument, arg1 );
	if ( !is_number( arg1 ) )
	{
	    send_to_char( "Invalid value.\n\r", ch );
	    return;
	}
	value3 = atoi( arg1 );

    	if ( value1 + value2 + value3 > 100 )
	{
	    send_to_char( "Total probability cannot exceed 100%.\n\r", ch );
	    return;
	}

        pArea->weather->chance[ season ][ 0 ] = 0;
        pArea->weather->chance[ season ][ 1 ] = value1;
        pArea->weather->chance[ season ][ 2 ] = value2+value1;
        pArea->weather->chance[ season ][ 3 ] = value3+value2+value1;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Storm type set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "storm" ) )
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  storm <season> <storm type>\n\r", ch );
            return;
        }

	argument = one_argument( arg2, arg );
	if ( !str_cmp( arg , "spring" ) ) season = 0;
	else if ( !str_cmp( arg , "summer" ) ) season = 1;
	else if ( !str_cmp( arg , "fall" ) ) season = 2;
	else if ( !str_cmp( arg , "winter" ) ) season = 3;
	else
	{
	    send_to_char( "Invalid season.\n\r", ch );
	    return;
	}
	one_argument( argument, arg1 );
	value = get_storm_type( arg1 );

    	if ( value == -1 )
	{
	    send_to_char( "Invalid storm type.\n\r", ch );
	    return;
	}

        pArea->weather->storm_type[ season ] = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Storm type set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "speed" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  speed #\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        pArea->weather->speed = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Weather speed set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "spring" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  spring #\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        pArea->weather->spring = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Spring starting month set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "summer" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  summer #\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        pArea->weather->summer = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Summer starting month set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "fall" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  fall #\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        pArea->weather->fall = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Fall starting month set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "winter" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  winter #\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        pArea->weather->winter = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Winter starting month set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "max_age" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  max_age [#]\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        pArea->max_age = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Max age set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "max_age_empty" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  max_age_empty [#]\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        pArea->max_age_empty = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Max age while empty set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "security" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  security [#level]\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        if ( IS_NPC(ch) || value < GET_PC(ch,security,0) || value > 9 )
        {
            if ( GET_PC(ch,security,9) != 9 )
            {
                sprintf( buf, "Valid security range is %d-9.\n\r", PC(ch,security) );
                send_to_char( buf, ch );
            }
            else
            {
                send_to_char( "Valid security is 9 only.\n\r", ch );
            }
            return;
        }

        pArea->security = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );

        send_to_char( "Security set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "builder" ) )
    {
        argument = one_argument( argument, arg2 );

        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  builder [$name]\n\r", ch );
            return;
        }

        arg2[0] = UPPER( arg2[0] );

        if ( strstr( pArea->builders, arg2 ) != '\0' )
        {
            strncpy( pArea->builders, string_replace( pArea->builders, arg2, "\0" ), 30 );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Builder removed.\n\r", ch );
            return;
        }
        else
        {
            buf[0] = '\0';
            if ( strstr( pArea->builders, "None" ) != '\0' )
                strncpy( pArea->builders, string_replace( pArea->builders, "None", "\0" ), 30 );

            if (pArea->builders[0] != '\0' )
            {
                strcat( buf, pArea->builders );
                strcat( buf, " " );
            }
            strcat( buf, arg2 );
            strncpy( pArea->builders, string_proper( str_dup( buf ) ), 30 );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Builder added.\n\r", ch );
            return;
        }
    }


    if ( !str_cmp( arg1, "areanum" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  areanum [#]\n\r", ch );
            return;
        }

        value = atoi( arg2 );

 /* check for areanum already in use... Fix this Galadrail!
        {
            send_to_char( "AEdit:  Areanum already assigned.\n\r", ch );
            return;
	} */

        pArea->areanum = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED ); 

        send_to_char( "Area number set.\n\r", ch );
        return;
    }

    if ( !str_cmp( arg1, "lvnum" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  lvnum [#lower]\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        if ( get_vnum_area( value ) != NULL
          && get_vnum_area( value ) != pArea )
        {
            send_to_char( "AEdit:  Vnum range already assigned.\n\r", ch );
            return;
        }

        pArea->lvnum = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );

        send_to_char( "Lower vnum set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "uvnum" ) )
    {
        if ( !is_number( arg2 ) || arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  uvnum [#upper]\n\r", ch );
            return;
        }

        value = atoi( arg2 );

        if ( get_vnum_area( value ) != NULL
          && get_vnum_area( value ) != pArea )
        {
            send_to_char( "AEdit:  Vnum range already assigned.\n\r", ch );
            return;
        }

        pArea->uvnum = value;
        SET_BIT( pArea->area_flags, AREA_CHANGED );

        send_to_char( "Upper vnum set.\n\r", ch );
        return;
    }


    if ( !str_cmp( arg1, "vnum" ) )
    {
       argument = one_argument( argument, arg1 );
       strcpy( arg2, argument );

       if ( !is_number( arg1 ) || arg1[0] == '\0'
         || !is_number( arg2 ) || arg2[0] == '\0' )
       {
            send_to_char( "Syntax:  vnum [#lower] [#upper]\n\r", ch );
            return;
        }

        value = atoi( arg1 );

        if ( get_vnum_area( value ) != NULL
          && get_vnum_area( value ) != pArea )
        {
            send_to_char( "AEdit:  Lower vnum already assigned.\n\r", ch );
            return;
        }

        pArea->lvnum = value;
        send_to_char( "Lower vnum set.\n\r", ch );

        value = atoi( arg2 );

        if ( get_vnum_area( value ) != NULL
          && get_vnum_area( value ) != pArea )
        {
            send_to_char( "AEdit:   Upper vnum already assigned.\n\r", ch );
            return;
        }

        pArea->uvnum = value;
        send_to_char( "Upper vnum set.\n\r", ch );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        return;
    }

    interpret( ch, arg );
    return;
}



int get_direction_number_1( char *arg )
{
    if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) return DIR_NORTH;
    if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) return DIR_EAST;
    if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) return DIR_SOUTH;
    if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) return DIR_WEST;
    if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) return DIR_UP;
    if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) return DIR_DOWN;
    return MAX_DIR;
}



int get_exit_flag_number( char *arg )
{
    if ( !str_cmp( arg, "door" ) )              return EX_ISDOOR;
    if ( !str_cmp( arg, "isdoor" ) )            return EX_ISDOOR;
    if ( !str_cmp( arg, "closed" ) )            return EX_CLOSED;
    if ( !str_cmp( arg, "bashed" ) )            return EX_BASHED;
    if ( !str_cmp( arg, "bashproof" ) )         return EX_BASHPROOF;
    if ( !str_cmp( arg, "passproof" ) )         return EX_PASSPROOF;
    if ( !str_cmp( arg, "wall" ) ) 	        return EX_WALL;
    if ( !str_cmp( arg, "illusion" ) )          return EX_ILLUSION;
    if ( !str_cmp( arg, "hidden" ) )            return EX_HIDDEN;
    if ( !str_cmp( arg, "secret" ) )            return EX_SECRET;
    return EX_NONE;
}


char *get_exit_flag_string_1( int exit_dir )
{
    static char buf[512];

    buf[0] = '\0';
    if ( exit_dir & 1    ) strcat( buf, " closed"    );
    if ( exit_dir & 2    ) strcat( buf, " locked"    );
    return ( buf[0] != '\0' ) ? buf + 1 : str_dup("open");
}

char *get_exit_flag_string( int exit_dir )
{
    static char buf[512];

    buf[0] = '\0';
    if ( exit_dir & EX_ISDOOR    ) strcat( buf, " isdoor"    );
    if ( exit_dir & EX_CLOSED    ) strcat( buf, " closed"    );
    if ( exit_dir & EX_BASHED    ) strcat( buf, " bashed"    );
    if ( exit_dir & EX_BASHPROOF ) strcat( buf, " bashproof" );
    if ( exit_dir & EX_PASSPROOF ) strcat( buf, " passproof" );
    if ( exit_dir & EX_HIDDEN    ) strcat( buf, " hidden" );
    if ( exit_dir & EX_SECRET    ) strcat( buf, " secret" );
    if ( exit_dir & EX_WALL      ) strcat( buf, " wall" );
    if ( exit_dir & EX_ILLUSION  ) strcat( buf, " illusion" );
    return ( buf[0] != '\0' ) ? buf + 1 : str_dup("none");
}

int get_mob_sex_number( char* arg )
{
    if ( !str_cmp( arg, "neuter" ) )            return SEX_NEUTER;
    if ( !str_cmp( arg, "male" ) )              return SEX_MALE;
    if ( !str_cmp( arg, "female" ) )            return SEX_FEMALE;
    return SEX_NONE;
}



SPEC_FUN *get_mob_spec( char* arg )
{
    char buf[MAX_STRING_LENGTH];

    buf[0] = '\0';
    strcat( buf, "spec_" );
    strcat( buf, arg );

    return spec_lookup( buf );
}


void maedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    MOB_INDEX_DATA *pMob2;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    strcpy( arg, argument );
    strcpy( arg3, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    pMob2 = (MOB_INDEX_DATA *)ch->desc->pEdit; 
    pArea = pMob2->load_with_area;

    SET_BIT(pMob2->affected_by,AFF_NEW_MOB);
    if ( !str_cmp( arg1, "show" ) || arg1[0] == '\0' )
    {
        sprintf( buf, "%ld", pMob2->vnum );
        do_mashow( ch, buf );
        return;
    }


    if ( !str_cmp( arg1, "done" ) )
    {
        ch->desc->pEdit = NULL;
        ch->desc->connected = CON_PLAYING;
        return;
    }

    if ( !str_cmp( arg1, "?" ) )
    {
        do_help( ch, "maedit" );
        return;
    }

        if (!strncmp(arg1,"area_start_room",8))
	{ 
		long s;
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("Set the vnum of this to the start room of the area\n\r",ch); 
			 send_to_char ("for boarding the area.\n\r",ch);
			 if (pMob2->moveable == NULL )
			 pMob2->moveable = moveable_alloc();			 
			 sprintf(buf,"Current area_start_room number:\n\r%ld\n\r",pMob2->moveable->area_start_room);
			 send_to_char(buf,ch);
			 return;
		}
		s = atoi (arg2);
		if (pMob2->moveable == NULL )
		pMob2->moveable = moveable_alloc();
		pMob2->moveable->area_start_room = s;
		sprintf(buf,"The board room for %s will now be vnum %ld\n\r",pMob2->short_descr,pMob2->moveable->area_start_room);
		send_to_char (buf,ch);					    
		return;
	}

    if ( !str_cmp( arg1, "board_msg" ) )
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  board_msg [string]\n\r", ch );
	    if (pMob2->moveable == NULL )
	    pMob2->moveable = moveable_alloc();
	    sprintf(buf,"Mobile Boarding: %s",pMob2->moveable->board_msg);
	    send_to_char(buf,ch);
            return;
        }

	if (pMob2->moveable == NULL )
	pMob2->moveable = moveable_alloc();
        strncpy( pMob2->moveable->board_msg, arg2, 15 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Boarding message set\n\r", ch);
        return;
    }


    if ( !str_cmp( arg1, "leave_msg" ) )
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  leave_msg [string]\n\r", ch );
	    if (pMob2->moveable == NULL )
	    pMob2->moveable = moveable_alloc();
	    sprintf(buf,"Mobile Leaving: %s",pMob2->moveable->leave_msg);
	    send_to_char(buf,ch);
            return;
        }

	if (pMob2->moveable == NULL )
	pMob2->moveable = moveable_alloc();
        strncpy( pMob2->moveable->leave_msg, arg2, 15 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Leaving message set\n\r", ch);
        return;
    }


    if ( !str_cmp( arg1, "move_msg" ) )
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  move_msg [string]\n\r", ch );
            send_to_char( "Syntax:  move_msg clear ( to clear the string )\n\r", ch );
	    if (pMob2->moveable == NULL )
	    pMob2->moveable = moveable_alloc();
	    sprintf(buf,"Mobile Moving: %s",pMob2->moveable->move_msg);
	    send_to_char(buf,ch);
            return;
        }

	if (pMob2->moveable == NULL )
	pMob2->moveable = moveable_alloc();
	if ( !str_cmp( arg2, "clear") )
	    pMob2->moveable->move_msg[0] = '\0';
	else
            strncpy( pMob2->moveable->move_msg, arg2, 15 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Moving message set\n\r", ch);
        return;
    }

    if ( !str_cmp( arg1, "moveable_area" ) )
    {

           TOGGLE_BIT( pMob2->act2, ACT2_MOVEABLE_AREA );

           SET_BIT( pArea->area_flags, AREA_CHANGED );
           send_to_char( "Moveable status flag toggled.\n\r", ch);
           return;
    }

    interpret( ch, arg );
    return;
}

void do_area_reset( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    char arg [MAX_INPUT_LENGTH];
    bool fAll = FALSE;
    
    one_argument(argument, arg);

    if (*arg != '\0')
    fAll = !str_cmp(arg, "all");

    if (!fAll)
    {
    pArea = ch->in_room->area;
    reset_area( pArea );
    event_reset( EVENT_ON_RESET, pArea);
    send_to_char( "Area reset.\n\r", ch );
    return;
    }
    else
    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
	reset_area( pArea );
	event_reset( EVENT_ON_RESET, pArea);
    }
    send_to_char( "All Areas reset.\n\r", ch );
    return;
}

void do_hedit( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    int value;
    if ( IS_NPC(ch) ) return;

    if ( is_number( argument ) )
    {
        value = atoi( argument );
        if ( ( pHelp = get_help_data( value ) ) == NULL )
        {
            send_to_char( "That area vnum does not exist.\n\r", ch );
            return;
        }
     }
     else
        if ( ( pHelp = get_help_data( DEFAULT_HELP_VNUM ) ) == NULL )
        {
            send_to_char( "ERROR, DEFAULT_HELP_VNUM\n\r", ch );
            return;
        }

    ch->desc->pEdit = (void *)pHelp;
    ch->desc->connected = CON_HEDITOR;
    return;
}

void do_sedit( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf1 [MAX_STRING_LENGTH];
    char buf [MAX_INPUT_LENGTH];
    bool found = FALSE;
    int iSocial;
    if ( IS_NPC(ch) ) return;
    
    strcpy( arg1, argument );

    if ( sedit_in_use[0] != '\0' )
    {
       if(str_cmp(sedit_in_use,ch->name))
       {
       sprintf(buf,"%s is in sedit now.  Please try again later.\n\r",sedit_in_use);
       send_to_char(buf,ch);
       return;
       }
    }
	

    if ( arg1[0] == '\0' )
    {
       send_to_char( "Syntax: sedit <social name>\n\r", ch );
       send_to_char( "Syntax: sedit create\n\r", ch );
       return;
    }

    if ( is_number( arg1 ) )
    {
            send_to_char( "Use the social name insted\n\r", ch );
            return;
    }

    buf1[0] = '\0';

      for (iSocial = 0; iSocial < social_count && !found ; iSocial++)
	if ( is_name(arg1, social_table[iSocial].name ) )
	 {
	  found = TRUE;
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].name);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_no_arg);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_no_arg);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].vict_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_not_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_auto);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_auto);
	  strcat(buf1, buf);
	  CiSocial = iSocial;
	  strcpy( sedit_in_use, ch->name);
	 } 

	if(!found)
	 {
            send_to_char( "Social not found\n\r", ch );
            return;
	 }

	send_to_char(buf1,ch);

    ch->desc->pEdit = (void *) (ch->in_room->area);
    ch->desc->connected = CON_SEDITOR;
    return;
}
HELP_DATA *get_help_data( int vnum )
{
    HELP_DATA *pHelp;

    for (pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
        if (pHelp->vnum == vnum)
            return pHelp;
    }

    return NULL;
}

void do_aedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    int value;

    if ( IS_NPC(ch) ) return;
    pArea = ch->in_room->area;

    if ( is_number( argument ) )
    {
        value = atoi( argument );
        if ( ( pArea = get_area_data( value ) ) == NULL )
        {
            send_to_char( "That area vnum does not exist.\n\r", ch );
            return;
        }
    }
    else
    {
        if ( !str_cmp( argument, "reset" ) )
        {
            reset_area( pArea );
            send_to_char( "Area reset.\n\r", ch );
            return;
        }
    }

    ch->desc->pEdit = (void *)pArea;
    ch->desc->connected = CON_AEDITOR;
    return;
}


void do_maedit( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob2;
    AREA_DATA *pArea;
    int value;
    char arg1[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );

    if ( is_number( arg1 ) )
    {
        value = atoi( arg1 );
        if ( ( pMob2 = get_mob_index( value ) ) == NULL )
        {
            send_to_char( "MAEdit:  That vnum does not exist.\n\r", ch );
            return;
        }
        pArea = pMob2->load_with_area;
          if ( !IS_BUILDER( ch, pArea ) )
           {
           send_to_char( "MAEdit:  Vnum in an area you cannot build in.\n\r", ch );
           return;
           }

        ch->desc->pEdit = (void *)pMob2;
        ch->desc->connected = CON_MAEDITOR;
        return;
    }

    send_to_char( "MAEdit:  There is no default moveable area to edit.\n\r", ch );
    return;
}



void do_sshow( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf  [MAX_STRING_LENGTH];
    char buf1  [MAX_STRING_LENGTH];
    int iSocial;
    bool found = FALSE;
    buf1[0] = '\0';
    strcpy( arg1, argument );
    if (arg1[0] == '\0')
	{
	send_to_char("Syntax: sshow keyword\n\r",ch);
	return;
	}        
    if ( is_number( arg1 ) )
	{
	send_to_char("Use social name\n\r",ch);
	return;
	}        
      for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
	if ( is_name(arg1, social_table[iSocial].name ) )
	 {
	  found = TRUE;
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].name);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_no_arg);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_no_arg);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].vict_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_not_found);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_auto);
	  strcat(buf1, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_auto);
	  strcat(buf1, buf);
	  sprintf(buf,"Type	%-5d\n\r",social_table[iSocial].type);
	  strcat(buf1, buf);
	  sprintf(buf,"damage	%-5d\n\r",social_table[iSocial].damage);
	  strcat(buf1, buf);
	  sprintf(buf,"Item	%-5ld\n\r",social_table[iSocial].item);
	  strcat(buf1, buf);
	  sprintf(buf,"Action	%-5d\n\r",social_table[iSocial].action);
	  strcat(buf1, buf);
	  sprintf(buf,"Mood	%-5d\n\r",social_table[iSocial].mood);
	  strcat(buf1, buf);
          if(!str_cmp(sedit_in_use,ch->name))
	  CiSocial = iSocial;
	 } 
     if (!found)
	{
	send_to_char("Social not found\n\r",ch);
	return;
	}
	send_to_char(buf1,ch);
    return;
}
void do_hshow( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf  [MAX_STRING_LENGTH];
    HELP_DATA *pHelp;
    int vnum;
    strcpy( arg1, argument );

    if ( is_number( arg1 ) )
        vnum = ( atoi( arg1 ) );
    else
	vnum = -1;
        
for ( pHelp = help_first; pHelp != NULL;  pHelp = pHelp->next )
  {
    if ( pHelp->vnum == vnum || ( vnum == -1 && is_name( arg1, pHelp->keyword ) ) )
    {

    	sprintf( buf, "Keyword: '%s'\n\r", pHelp->keyword );
    	send_to_char( buf, ch );

    	sprintf( buf, "Vnum: %d\n\r", pHelp->vnum );
    	send_to_char( buf, ch );

    	sprintf( buf, "Level: %d\n\r", pHelp->level);
    	send_to_char( buf, ch );
	if ( pHelp->vnum == DEFAULT_HELP_VNUM )
	{
    	sprintf( buf, "%s\n\r", pHelp->text);
    	send_to_char( buf, ch );
	}
    }
  }
    return;
}

char * const storm_type[MAX_SKY] =
{
    "Cloudless",
    "Cloudy",
    "Rain",
    "Lightning",
    "Snow",
    "Blizzard",
    "Rain",
    "Huricane",
    "Windy",
    "Tornado",    "Windy",
    "Sandstorm"

};

void do_ashow( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char buf  [MAX_STRING_LENGTH];
    AREA_DATA *pArea;
    int Hash_loc, total_objects_in_area, obj_approved, obj_approve_hold;
    OBJ_INDEX_DATA *pObjIndex;

    strcpy( arg1, argument );

    if ( is_number( arg1 ) )
        pArea = get_area_data( atoi( arg1 ) );
    else
        pArea = ch->in_room->area;
        
    if (!pArea)
      pArea = ch->in_room->area;

    sprintf( buf, "Name: '%s'\n\r", pArea->name );
    send_to_char( buf, ch );

    sprintf( buf, "Credits: '%s'\n\r", pArea->credits );
    send_to_char( buf, ch );

    sprintf( buf, "File: '%s'\n\r", pArea->filename );
    send_to_char( buf, ch );

    sprintf( buf, "Age: %d; Players: %d  Wander Time %d\n\r", pArea->age, pArea->nplayer, pArea->wander_time );
    send_to_char( buf, ch );

    sprintf( buf, "Visible level: %d\n\r", pArea->visible);
    send_to_char( buf, ch );

    sprintf( buf, "Weather mmhg: %d change %d\n\r", pArea->weather->mmhg, pArea->weather->change );
    send_to_char( buf, ch );

    sprintf( buf, "Chance : %d (Cloudless) %d (Cloudy) %d (%s) %d (%s)   Season : %d\n\r",
	pArea->weather->chance[ pArea->weather->season ][ 1 ],
	pArea->weather->chance[ pArea->weather->season ][ 2 ]-pArea->weather->chance[ pArea->weather->season ][ 1 ],
	pArea->weather->chance[ pArea->weather->season ][ 3 ]-pArea->weather->chance[ pArea->weather->season ][ 2 ],
	storm_type[ pArea->weather->storm_type[ pArea->weather->season ] - 1 ],
	100-pArea->weather->chance[ pArea->weather->season ][ 3 ],
	storm_type[ pArea->weather->storm_type[ pArea->weather->season ] ],
	pArea->weather->season);
    send_to_char( buf, ch );

    sprintf( buf, "Max age: %d; Max age while empty: %d\n\r", pArea->max_age, pArea->max_age_empty );
    send_to_char( buf, ch );

    sprintf( buf, "World: %d  [%s]\n\r",  pArea->world, pArea->world <= MAX_WORLD ? world_name[pArea->world] : "Set the world again please"  );
    send_to_char( buf, ch );

    sprintf( buf, "Security: %d; Builders '%s'\n\r", pArea->security, pArea->builders );
    send_to_char( buf, ch );

    sprintf( buf, "Areanum: %d\n\r", pArea->areanum);
    send_to_char( buf, ch );

    sprintf( buf, "Vnums: %ld-%ld\n\r", pArea->lvnum, pArea->uvnum );
    send_to_char( buf, ch );
   

    total_objects_in_area = 0;
    obj_approved = 0;
    obj_approve_hold = 0;

    for (Hash_loc = 0; Hash_loc < MAX_KEY_HASH; Hash_loc++)
    {
	for (pObjIndex = obj_index_hash[Hash_loc]; pObjIndex != NULL; pObjIndex= pObjIndex->next)
	{
	    if (pObjIndex->in_area == pArea)
	    {
		total_objects_in_area++;
		if (IS_SET(pObjIndex->approve_data->flags, APPROVED ) )
		obj_approved++;
		else
		if (IS_SET(pObjIndex->approve_data->flags, APPROVE_HOLD ) )
		obj_approve_hold++;
	    }
	}
    }
    sprintf( buf, "Total %5d Approved %5d On hold %5d Need Approving %5d\n\r",
	total_objects_in_area, obj_approved, obj_approve_hold, total_objects_in_area - obj_approved );	
    send_to_char( buf, ch );
    sprintf( buf, "Flags: %s\n\r", area_bit_name( pArea->area_flags ) );
    send_to_char( buf, ch );

    return;
}



void do_rshow( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA        *obj;
    CHAR_DATA       *rch;
    ROOM_INDEX_DATA *location;
    LOCK_DATA 	    *lock_list;
    ROOM_SNOOP_LIST *room_snoop_list;
    char             buf  [ MAX_STRING_LENGTH ];
    char             buf1 [ MAX_STRING_LENGTH ];
    char             arg  [ MAX_INPUT_LENGTH  ];
    int              door;

    rch = get_char( ch );

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( !location )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( ch->in_room != location && room_is_private( location ) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    buf1[0] = '\0';

    sprintf( buf, "Name: '%s'\n\rArea: [%3d] '%s'\n\r",
	    location->name,
	    location->area->areanum,
	    location->area->name );
    strcat( buf1, buf );

    sprintf( buf,
	    "Vnum: %ld\n\rSector: %s\n\rLight: %d Adjusted Light: %d Size: %d\n\r",
	    location->vnum,
	    sector_bit_name( location->sector_type ),
	    location->light, get_light_adjustment ( ch, location ), location->size );
    strcat( buf1, buf );

    sprintf( buf,
	    "Room flags: %s %s\n\rDescription:\n\r%s",
	    room_bit_name( location->room_flags ),
	    room_bit_name_2( location->room_flags_2 ),
	    location->description );
    strcat( buf1, buf );

    if ( location->guilds != 0 )
    {
	sprintf( buf, "Guilds: %s\n\r", guild_bit_name( location->guilds ) );
	strcat( buf1, buf );
    }

    if ( location->extra_descr )
    {
	EXTRA_DESCR_DATA *ed;

	strcat( buf1, "Extra description keywords: '" );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    strcat( buf1, ed->keyword );
	    if ( ed->next )
		strcat( buf1, " " );
	}
	strcat( buf1, "'\n\r" );
    }

    if ( location->snoop_by != NULL )
    {
	strcat( buf1, "Snooped by: " );
	for ( room_snoop_list = location->snoop_by ; room_snoop_list != NULL ; room_snoop_list = room_snoop_list->next )
	{
	    sprintf( buf, " %ld", room_snoop_list->room->vnum );
	    strcat( buf1, buf );
	}

	strcat( buf1, "\n\r" );
    }

    strcat( buf1, "Characters:" );

    /* Yes, we are reusing the variable rch.  - Kahn */
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	strcat( buf1, " " );
	one_argument( rch->name, buf );
	strcat( buf1, buf );
    }

    strcat( buf1, "\n\rObjects:   " );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	strcat( buf1, " " );
	one_argument( STR(obj, name), buf );
	strcat( buf1, buf );
    }
    strcat( buf1, "\n\r" );

    for ( door = 0; door < MAX_DIR; door++ )
    {
	SPELL_LIST *spell;
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) )
	{
	    sprintf( buf,
		    "Door: %5s.  To: %ld.  Key: (unused) Keyword '%s'\n\rExit flags: %s",
		    dir_name[door],
		    pexit->to_room ? pexit->to_room->vnum : 0,
		    (pexit->keyword[0] != '\0') ? pexit->keyword : "",
		    get_exit_flag_string( pexit->exit_info ));
	    strcat( buf1, buf );
	    sprintf(buf, "  Reset to %s\n\r",get_exit_flag_string( pexit->exit_info_default ) );
	    strcat(buf1, buf);
	    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	    {
		sprintf( buf, "%s", pexit->description );
		strcat( buf1, buf );
	    }
	    if ( pexit->mes_to_char[0] != '\0' )
	    {
		sprintf( buf,"MTC->%s\n\r",pexit->mes_to_char);	  
		strcat( buf1, buf);
	    }
	    if ( pexit->mes_to_room[0] != '\0' )
	    {
		sprintf( buf,"MTR->%s\n\r",pexit->mes_to_room);	  
		strcat( buf1, buf);
	    }
	    if ( pexit->mes_at_room[0] != '\0' )
	    {
		sprintf( buf,"MAR->%s\n\r",pexit->mes_at_room);	  
		strcat( buf1, buf);
	    }
	    if ( pexit->keyword[0] == '\0' || strlen(pexit->keyword) == 0 
	 	|| !IS_SET( pexit->exit_info_default, EX_ISDOOR ) )
		continue;
	    if ( pexit->trap != NULL )
	    {
		sprintf( buf, "Trap flags : %s\n\n", get_flags_trap( pexit->trap->flags ) );
	        strcat( buf1, buf );

		for ( spell = pexit->trap->spells ; spell != NULL ; spell = spell->next )
		{
		    sprintf( buf, "   Level %d spell of %s\n\r", spell->level, skill_table[ spell->spell ].name );
	       	    strcat( buf1, buf );
		}
 	    }
	    for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_list->next )
	    {
		sprintf( buf, "Key: %ld   Lock info : ", lock_list->vnum );
	 	if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		    strcat( buf, "(locked) ");
		else
		    strcat( buf, "(unlocked) " );
	 	if (  IS_SET( lock_list->lock_info, LOCK_PICKPROOF ) )
		    strcat( buf, "(pick proof) ");
	 	if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		    strcat( buf, "(dead bolt) ");
		strcat( buf, "\n\r");
	       	strcat( buf1, buf );
	    }
	    if ( pexit->u1.to_room != NULL && pexit->u1.to_room->exit[ rev_dir[ door ] ] != NULL
		&& pexit->u1.to_room->exit[ rev_dir[ door ] ]->u1.to_room == location
 		&& ( lock_list = pexit->u1.to_room->exit[ rev_dir[ door ] ]->lock ) != NULL )
	    {
		send_to_char( "  - Otherside - \n\r", ch );
	        for ( ; lock_list != NULL ; lock_list = lock_list->next )
	        {
		    sprintf( buf, "Key: %ld   Lock info : ", lock_list->vnum );
	     	    if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
			    strcat( buf, "(locked) ");
	   	    else
		    	strcat( buf, "(unlocked) " );
	   	    if (  IS_SET( lock_list->lock_info, LOCK_PICKPROOF ) )
		    	strcat( buf, "(pick proof) ");
	 	    if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		    	strcat( buf, "(dead bolt) ");
		    strcat( buf, "\n\r");
	       	    strcat( buf1, buf );
	        }
	    }
	}
    }

    send_to_char( buf1, ch );
    return;
}


void do_mashow( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *victim;
    int x,y = 0;
    buf[0] = '\0';
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
    send_to_char( "MaShow whom?\n\r", ch );
        return;
    }

    if ( ( victim = get_mob_index( atoi( arg ) ) ) == NULL )
    {
    send_to_char( "Invalid mob index VNUM.\n\r", ch );
        return;
    }

    if ( victim->moveable == NULL )
    victim->moveable = moveable_alloc();

    sprintf( buf, "Name: '%s'\n\rArea %d: '%s'\n\r",
    victim->player_name,
    victim->load_with_area == NULL ? -1        : victim->load_with_area->areanum,
    victim->load_with_area == NULL ? "No Area" : victim->load_with_area->name );
    send_to_char( buf, ch );

    if (IS_SET(victim->act2, ACT2_MOVEABLE_AREA ) )
    sprintf(buf,"Moveable area ? : ( YES )\n\r");
    else
    sprintf(buf,"Movealbe area ? : ( NO )\n\r");
    send_to_char( buf, ch );

    if (IS_SET(victim->act2, ACT2_SHIP ) )
    sprintf(buf,"Moveable acts like a ship ? : ( YES )\n\r");
    else
    sprintf(buf,"Movealbe acts like a ship ? : ( NO )\n\r");
    send_to_char( buf, ch );

    if (IS_SET(victim->act2, ACT2_OPEN_LAND ) )
    sprintf(buf,"Moveable stays in open sectors ? : ( YES )\n\r");
    else
    sprintf(buf,"Movealbe stays in open sectors ? : ( NO )\n\r");
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %ld\n\rSex:  %s\n\r",
    victim->vnum,
    victim->sex == SEX_MALE    ? "male"   :
    victim->sex == SEX_FEMALE  ? "female" : "neuter" );
    send_to_char( buf, ch );

    sprintf( buf,
    "Level: %d\n\r",
    victim->level  );
    send_to_char( buf, ch );

    sprintf( buf, "Leaving Msg: %s\n\rBoarding Msg: %s\n\rMoving Msg: %s\n\r Area Start room: %ld\n\r",
    victim->moveable->leave_msg,
    victim->moveable->board_msg,
    victim->moveable->move_msg,
    victim->moveable->area_start_room );
    send_to_char( buf, ch );
    for ( x= 0; x < MAX_SECTORS; x++)
    {
	sprintf(buf, "Sector:%-15s %-5s  ",sector_bit_name(x),victim->valid_sector[x]  > 0 ? "TRUE" : "FALSE" );
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

void set_obj_index_points( OBJ_INDEX_DATA *obj )
{
    int points = 0;
    SPELL_LIST * spell;
    AFFECT_DATA * af;

    if ( obj->wear_data != NULL )
    {
	if ( obj->wear_data->weapon_data != NULL )
	{
	    points += (( obj->wear_data->weapon_data->die * obj->wear_data->weapon_data->dice) * 2 + 4) / 4;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_BASTARD ) ) points += points / 4;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_FLAMING ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_FROST ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_VAMPIRIC ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_POISON ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_SHARP ) ) points += 2;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_VORPAL ) ) points += 10;
	}

	if ( obj->wear_data->affected != NULL )
	{
	    int mod;

	    for ( af = obj->wear_data->affected ; af != NULL ; af = af->next )
	    {
		if ( IS_SET( af->bitvector, AFF_BLIND		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_SHADOWFORM	) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_DETECT_WORLD	) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_DETECT_SHADOWFORM) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_DETECT_MAGIC	) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_DETECT_HIDDEN	) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_TONGUES		) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_SANCTUARY	) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_FAERIE_FIRE	) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_INFRARED	) ) points += 4;
		if ( IS_SET( af->bitvector, AFF_CURSE		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_BREATHE_UNDERWATER ) ) points += 8;
		if ( IS_SET( af->bitvector, AFF_SNEAK		) ) points += 8;
		if ( IS_SET( af->bitvector, AFF_HIDE		) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_SLEEP		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_LEVITATING	) ) points += 8;
		if ( IS_SET( af->bitvector, AFF_FLYING		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_DIMENSION_WALK	) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_HASTE		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_CALM_SPIRIT	) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_PLAGUE		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_DARK_VISION	) ) points += 6;
		if ( IS_SET( af->bitvector, AFF_BERSERK		) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_SWIM		) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_REGENERATION	) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_COLD	 	) ) points -= 5;
		if ( IS_SET( af->bitvector, AFF_FADE		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_SENSES		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_SLOW		) ) points -= 10;
		if ( IS_SET( af->bitvector, AFF_LIGHT_SENSITIVE ) ) points -= 5;

		mod = af->modifier;
		switch ( af->location )
		{
		    default:						break;
		    case APPLY_NONE:					break;
		    case APPLY_STR:           points	+= 4 * mod;	break;
		    case APPLY_DEX:           points	+= 4 * mod;	break;
		    case APPLY_INT:           points	+= 4 * mod;	break;
		    case APPLY_WIS:           points	+= 4 * mod;	break;
		    case APPLY_CON:           points	+= 4 * mod;	break;
		    case APPLY_CHR:           points	+= 2 * mod;	break;
		    case APPLY_SEX:           				break;
		    case APPLY_LEVEL:					break;
		    case APPLY_AGE:	     				break;
		    case APPLY_HEIGHT:					break;
		    case APPLY_WEIGHT:					break;
		    case APPLY_MANA:          points	+= mod / 20;	break;
		    case APPLY_MANA_FIRE:	      points	+= mod / 30;	break;
		    case APPLY_MANA_WATER:	      points	+= mod / 30;	break;
		    case APPLY_MANA_EARTH:	      points	+= mod / 30;	break;
		    case APPLY_MANA_WIND:	      points	+= mod / 30;	break;
		    case APPLY_MANA_SPIRIT:	      points	+= mod / 30;	break;
		    case APPLY_HIT:           points	+= mod / 10;	break;
		    case APPLY_MOVE:          points	+= mod / 15;	break;
		    case APPLY_GOLD:					break;
		    case APPLY_EXP:					break;
		    case APPLY_AC:	      points	+= -1 * mod;	break;
		    case APPLY_HITROLL:       points	+= mod * 2;	break;
		    case APPLY_DAMROLL:       points	+= mod * 3;	break;
		    case APPLY_SAVING_SPELL:  points	+= mod * -1;	break;
		    case APPLY_AFFECT:				        break;
		    }
	    }
	}
	if ( obj->wear_data->magic_data != NULL )
	{
	    int charges = obj->wear_data->magic_data->charges;
	    int magic_points = 0;

	    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
	    {
		if ( !IS_SET( skill_table[ spell->spell ].target, TAR_CHAR_OFFENSIVE|TAR_CHAR_DAMAGING ) 
		  || obj->wear_data->weapon_data != NULL 
		  || obj->wear_data->magic_data->type == MAGIC_WAND 
		  || obj->wear_data->magic_data->type == MAGIC_STAFF)
		    magic_points += ( spell->level / 2 ) + ( charges / 2 );
		else 
		    magic_points -= ( spell->level / 2 ) + ( charges / 2 );
	    }
	
	    magic_points = ( magic_points < 0 ? -1 * magic_points : magic_points );

	    points += magic_points;
	}

	points += obj->wear_data->ac;

	if ( IS_SET( obj->extra_flags, ITEM_BUCKLER	) ) points += 5;
	if ( IS_SET( obj->extra_flags, ITEM_SHADOWFORM	) ) points += 2;
	if ( IS_SET( obj->extra_flags, ITEM_NODROP	) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ANTI_NENKEMEN ) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ANTI_LITHDOR ) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ANTI_MAEGMENEL ) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_NOREMOVE	) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ROT_DEATH	) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_NO_SAVE	) ) points -= 4;
	if ( IS_SET( obj->extra_flags, ITEM_NO_LOCATE	) ) points -= 0;
	if ( IS_SET( obj->extra_flags, ITEM_NO_IDENTIFY	) ) points -= 0;
	if ( IS_SET( obj->extra_flags, ITEM_FLOAT	) ) points += 4;

    }
    else
	return;

    if ( obj->edible_data != NULL )
    {

	if ( IS_SET( obj->edible_data->flags, FOOD_POISONED ) )
	    points -= 4;

	for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell->next )
	{
	    if ( !IS_SET( skill_table[ spell->spell ].target, TAR_CHAR_OFFENSIVE|TAR_CHAR_DAMAGING ) )
		points += ( spell->level / 2 ) + ( obj->edible_data->max_amount / 2 );
	    else
		points -= ( spell->level / 2 ) + ( obj->edible_data->max_amount / 2 );
	}
    }    

    obj->mpoints = points;

    if ( obj->light_data != NULL )
    {
	if ( obj->light_data->max_fuel == -1 )
	{
	    /* unlimited lighter */
	    if ( !IS_SET( obj->light_data->flags, LIGHT_LIT|LIGHT_MAGIC_LIT|LIGHT_LIGHTABLE ) && IS_SET( obj->light_data->flags, LIGHT_BURNING ) )
		points += 2;
	    else
		points += obj->light_data->brightness / 15;
	}
	else
	{
	    /* non working light */
	    if ( !IS_SET( obj->light_data->flags, LIGHT_LIGHTABLE|LIGHT_LIT|LIGHT_MAGIC_LIT ) )
		points += 0;
	    else
	    {
		if ( IS_SET( obj->light_data->flags, LIGHT_REFUELABLE ) )
		    points += ( IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) ? 5 : 2 ) * ( obj->light_data->fuel / 100 ) * ( obj->light_data->brightness / 10 );
		else
		    points += ( IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) ? 3 : 1 ) * ( obj->light_data->fuel / 100 ) * ( obj->light_data->brightness / 10 );
	    }
	}
    }
    if ( obj->cont_data != NULL )
    {
	int cont_points;

	cont_points = obj->cont_data->capacity / 20;
	
	if ( IS_SET( obj->cont_data->flags, CONT_DEVOURING	) ) points += -5;
	if ( IS_SET( obj->cont_data->flags, CONT_WATER_PROOF	) ) points += 1;
	if ( IS_SET( obj->cont_data->flags, CONT_PERM_SAVE	) ) points += 10;
	if ( IS_SET( obj->cont_data->flags, CONT_HOLDING	) ) points += 1;	
	if ( IS_SET( obj->cont_data->flags, CONT_EXTRA_HOLDING	) ) points += 5;
	if ( IS_SET( obj->cont_data->flags, CONT_SUPER_HOLDING	) ) points += 10;

	points += cont_points;
    }

    obj->points = points;

}

void set_obj_points( OBJ_DATA *obj )
{
    int points = 0;
    SPELL_LIST * spell;
    AFFECT_DATA * af;

    if ( obj->wear_data != NULL )
    {
	if ( obj->wear_data->weapon_data != NULL )
	{
	    points += (( obj->wear_data->weapon_data->die * obj->wear_data->weapon_data->dice) * 2 + 4) / 4;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_BASTARD ) ) points += points / 4;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_FLAMING ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_FROST ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_VAMPIRIC ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_POISON ) ) points += 10;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_SHARP ) ) points += 2;
	    if ( IS_SET( obj->wear_data->weapon_data->flags, WEAPON_VORPAL ) ) points += 10;
	}

	if ( obj->wear_data->affected != NULL )
	{
	    int mod;

	    for ( af = obj->wear_data->affected ; af != NULL ; af = af->next )
	    {
		if ( IS_SET( af->bitvector, AFF_BLIND		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_SHADOWFORM	) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_DETECT_WORLD	) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_DETECT_SHADOWFORM) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_DETECT_MAGIC	) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_DETECT_HIDDEN	) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_TONGUES		) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_SANCTUARY	) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_FAERIE_FIRE	) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_INFRARED	) ) points += 4;
		if ( IS_SET( af->bitvector, AFF_CURSE		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_BREATHE_UNDERWATER ) ) points += 8;
		if ( IS_SET( af->bitvector, AFF_SNEAK		) ) points += 8;
		if ( IS_SET( af->bitvector, AFF_HIDE		) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_SLEEP		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_LEVITATING	) ) points += 8;
		if ( IS_SET( af->bitvector, AFF_FLYING		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_DIMENSION_WALK	) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_HASTE		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_CALM_SPIRIT	) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_PLAGUE		) ) points += -5;
		if ( IS_SET( af->bitvector, AFF_DARK_VISION	) ) points += 6;
		if ( IS_SET( af->bitvector, AFF_BERSERK		) ) points += 10;
		if ( IS_SET( af->bitvector, AFF_SWIM		) ) points += 2;
		if ( IS_SET( af->bitvector, AFF_REGENERATION	) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_COLD	 	) ) points -= 5;
		if ( IS_SET( af->bitvector, AFF_FADE		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_SENSES		) ) points += 15;
		if ( IS_SET( af->bitvector, AFF_SLOW		) ) points -= 10;
		if ( IS_SET( af->bitvector, AFF_LIGHT_SENSITIVE ) ) points -= 5;

		mod = af->modifier;
		switch ( af->location )
		{
		    default:						break;
		    case APPLY_NONE:					break;
		    case APPLY_STR:           points	+= 4 * mod;	break;
		    case APPLY_DEX:           points	+= 4 * mod;	break;
		    case APPLY_INT:           points	+= 4 * mod;	break;
		    case APPLY_WIS:           points	+= 4 * mod;	break;
		    case APPLY_CON:           points	+= 4 * mod;	break;
		    case APPLY_CHR:           points	+= 2 * mod;	break;
		    case APPLY_SEX:           				break;
		    case APPLY_LEVEL:					break;
		    case APPLY_AGE:	     				break;
		    case APPLY_HEIGHT:					break;
		    case APPLY_WEIGHT:					break;
		    case APPLY_MANA:          points	+= mod / 20;	break;
		    case APPLY_MANA_FIRE:	      points	+= mod / 30;	break;
		    case APPLY_MANA_WATER:	      points	+= mod / 30;	break;
		    case APPLY_MANA_EARTH:	      points	+= mod / 30;	break;
		    case APPLY_MANA_WIND:	      points	+= mod / 30;	break;
		    case APPLY_MANA_SPIRIT:	      points	+= mod / 30;	break;
		    case APPLY_HIT:           points	+= mod / 10;	break;
		    case APPLY_MOVE:          points	+= mod / 15;	break;
		    case APPLY_GOLD:					break;
		    case APPLY_EXP:					break;
		    case APPLY_AC:	      points	+= -1 * mod;	break;
		    case APPLY_HITROLL:       points	+= mod * 2;	break;
		    case APPLY_DAMROLL:       points	+= mod * 3;	break;
		    case APPLY_SAVING_SPELL:  points	+= mod * -1;	break;
		    case APPLY_AFFECT:				        break;
		    }
	    }
	}
	if ( obj->wear_data->magic_data != NULL )
	{
	    int charges = obj->wear_data->magic_data->charges;
	    int magic_points = 0;

	    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
	    {
		if ( !IS_SET( skill_table[ spell->spell ].target, TAR_CHAR_OFFENSIVE|TAR_CHAR_DAMAGING ) 
		  || obj->wear_data->weapon_data != NULL 
		  || obj->wear_data->magic_data->type == MAGIC_WAND 
		  || obj->wear_data->magic_data->type == MAGIC_STAFF)
		    magic_points += ( spell->level / 2 ) + ( charges / 2 );
		else 
		    magic_points -= ( spell->level / 2 ) + ( charges / 2 );
	    }
	
	    magic_points = ( magic_points < 0 ? -1 * magic_points : magic_points );

	    points += magic_points;
	}

	points += obj->wear_data->ac;

	if ( IS_SET( obj->extra_flags, ITEM_BUCKLER	) ) points += 5;
	if ( IS_SET( obj->extra_flags, ITEM_SHADOWFORM	) ) points += 2;
	if ( IS_SET( obj->extra_flags, ITEM_NODROP	) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ANTI_NENKEMEN ) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ANTI_LITHDOR ) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ANTI_MAEGMENEL ) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_NOREMOVE	) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_ROT_DEATH	) ) points -= 2;
	if ( IS_SET( obj->extra_flags, ITEM_NO_SAVE	) ) points -= 4;
	if ( IS_SET( obj->extra_flags, ITEM_NO_LOCATE	) ) points -= 0;
	if ( IS_SET( obj->extra_flags, ITEM_NO_IDENTIFY	) ) points -= 0;
	if ( IS_SET( obj->extra_flags, ITEM_FLOAT	) ) points += 4;
    }
    else
	return;

    if ( obj->edible_data != NULL )
    {

	if ( IS_SET( obj->edible_data->flags, FOOD_POISONED ) )
	    points -= 4;

	for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell->next )
	{
	    if ( !IS_SET( skill_table[ spell->spell ].target, TAR_CHAR_OFFENSIVE|TAR_CHAR_DAMAGING ) )
		points += ( spell->level / 2 ) + ( obj->edible_data->max_amount / 2 );
	    else
		points -= ( spell->level / 2 ) + ( obj->edible_data->max_amount / 2 );
	}
    }    

    obj->mpoints = points;

    if ( obj->light_data != NULL )
    {
	if ( obj->light_data->max_fuel == -1 )
	{
	    /* unlimited lighter */
	    if ( !IS_SET( obj->light_data->flags, LIGHT_LIT|LIGHT_MAGIC_LIT|LIGHT_LIGHTABLE ) && IS_SET( obj->light_data->flags, LIGHT_BURNING ) )
		points += 2;
	    else
		points += obj->light_data->brightness / 15;
	}
	else
	{
	    /* non working light */
	    if ( !IS_SET( obj->light_data->flags, LIGHT_LIGHTABLE|LIGHT_LIT|LIGHT_MAGIC_LIT ) )
		points += 0;
	    else
	    {
		if ( IS_SET( obj->light_data->flags, LIGHT_REFUELABLE ) )
		    points += ( IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) ? 5 : 2 ) * ( obj->light_data->fuel / 100 ) * ( obj->light_data->brightness / 10 );
		else
		    points += ( IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) ? 3 : 1 ) * ( obj->light_data->fuel / 100 ) * ( obj->light_data->brightness / 10 );
	    }
	}
    }
    if ( obj->cont_data != NULL )
    {
	int cont_points;

	cont_points = obj->cont_data->capacity / 20;
	
	if ( IS_SET( obj->cont_data->flags, CONT_DEVOURING	) ) points += -5;
	if ( IS_SET( obj->cont_data->flags, CONT_WATER_PROOF	) ) points += 1;
	if ( IS_SET( obj->cont_data->flags, CONT_PERM_SAVE	) ) points += 10;
	if ( IS_SET( obj->cont_data->flags, CONT_HOLDING	) ) points += 1;	
	if ( IS_SET( obj->cont_data->flags, CONT_EXTRA_HOLDING	) ) points += 5;
	if ( IS_SET( obj->cont_data->flags, CONT_SUPER_HOLDING	) ) points += 10;

	points += cont_points;
    }

    obj->points = points;

}

void display_inside( CHAR_DATA *ch, OBJ_INDEX_DATA * obj, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf1[MAX_BUF];
    char arg[MAX_STRING_LENGTH];
    RESET_DATA *pReset;
    MOB_INDEX_DATA *pMob2 = NULL;
    int iReset = 0;
    int number1 = 0;
    int number2 = 0;
    bool fAll;
    argument = one_argument( argument, arg);
    argument = one_argument( argument, arg1);
    argument = one_argument( argument, arg2);
    fAll = !str_cmp(arg, "all");

    if (fAll)
    {
	if (arg1[0] == '\0' || arg2[0] == '\0' )
	{
	send_to_char("Syntax: all <start #> <end #>\n\r",ch);
	return;
	}

    	number1 = atoi(arg1);
    	number2 = atoi(arg2);

	if ( number1 > number2 )
	{
	send_to_char("Cannot show resets backwards.\n\r",ch);
	return;
	}
	
	if ( number2 - number1 > 200 )
	{
	send_to_char("Do you really need to see that many at once?\n\r",ch);
	return;
	}
	
    }
    buf1[0] = '\0';

    for ( pReset = obj->cont_data->inside; pReset; pReset = pReset->next )
    {
	OBJ_INDEX_DATA  *pObj2;
	MOB_INDEX_DATA  *pMobIndex;
	OBJ_INDEX_DATA  *pObjIndex;
	OBJ_INDEX_DATA  *pObjToIndex;
	++iReset;
	if ( fAll)
	if (iReset < number1 || iReset > number2 )
	continue;	

	switch ( pReset->command )
	{
	default:
	    sprintf( buf, "Bad reset command: %c.", pReset->command );
	log_string(buf);
	    break;

	case 'M':
	    if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Load Mobile - Bad Mob %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

            pMob2 = pMobIndex;

            sprintf( buf, "%3d-> Loads %s (%ld) (max %d)%s [%d]\n\r",iReset,
                       pMob2->short_descr, pReset->arg1,
                       pReset->arg2,
		       ( pReset->vnum == 0 ? "" : ( pReset->vnum == -1 ? " as mounted" : " as grouped" ) ),
			pReset->chance );
		strcat( buf1, buf );

	    break;
	case 'O':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Load Object - Bad Object %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

            pObj2       = pObjIndex;

            sprintf( buf, "%3d-> Loads %s (%ld) (max %d) [%d]\n\r",iReset, 
                          pObj2->short_descr, pReset->arg1,
			  pReset->arg2, pReset->chance );
		strcat( buf1, buf );
	    break;

	case 'P':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Put Object - Bad Object %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

            pObj2       = pObjIndex;

	    if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )
	    {
                sprintf( buf, "Put Object - Bad To Object %ld\n\r", pReset->arg3 );
	log_string(buf);
                continue;
	    }

            sprintf( buf, "%3d-> Loads %s (%ld) inside %s (%ld) Max [%d] [%d]\n\r", iReset,
                          pObj2->short_descr,
                          pReset->arg1,
                          pObjToIndex->short_descr,
                          pReset->arg3,pReset->arg2, pReset->chance );
		strcat( buf1, buf );
	    break;

	case 'G':
	case 'E':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Give/Equip Object - Bad Object %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

            pObj2       = pObjIndex;

	    if ( !pMob2 )
	    {
                sprintf( buf, "Give/Equip Object - No Previous Mobile\n\r" );
	log_string(buf);
                continue;
	    }

            if ( pMob2->pShop && pReset->command == 'G' )
	{
            	sprintf( buf, "%3d-> Loads %s (%ld) in the inventory of %s (%ld) shopkeeper [%d]\n\r", iReset,
            	              pObj2->short_descr, pReset->arg1,
            	              pMob2->short_descr, pMob2->vnum, pReset->chance );
		strcat( buf1, buf );
	}
            else
	   {
            sprintf( buf, "%3d-> Loads %s (%ld) %s of %s (%ld) [%d]\n\r", iReset,
                          pObj2->short_descr, pReset->arg1,
                          (pReset->command == 'G') ? wear_loc_name( WEAR_NONE ) 
                                                   : wear_loc_name( pReset->arg3 ),
                          pMob2->short_descr, pMob2->vnum, pReset->chance );
		strcat( buf1, buf );
	   }
	    break;

        /*
         * Doors are automatically set as what they look like when the
         * database is edited.  I would like to do this will all the
         * reset information.
         */
	case 'D':
	    
	   if ((pReset->arg1) == (ch->in_room->vnum) || fAll)
	   {
	    sprintf( buf, "%3d-> The %s door is reset to %s [%d]\n", 
                iReset,
                dir_name[ pReset->arg2 ],
                get_exit_flag_string_1( pReset->arg3 ), pReset->chance );
		strcat( buf1, buf );
	   }
	    break;

	case 'R':
            sprintf( buf, "%3d-> Exits are randomized. [%d]\n\r",
                         iReset, pReset->chance );
		strcat( buf1, buf );
	    break;
	}
    }
            sprintf( buf, "\n\rNext available reset -> %3d\n\r", iReset+1);
		strcat( buf1, buf );
	page_to_char(buf1,ch);
    return;
}

void display_resets( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char buf1[MAX_BUF];
    char arg[MAX_STRING_LENGTH];
    AREA_DATA  *pArea = ch->in_room->area;
    RESET_DATA *pReset;
    MOB_INDEX_DATA *pMob2 = NULL;
    int iReset = 0;
    int mob_load_room = 0 ;
    int obj_load_room  = 0;
    int number1 = 0;
    int number2 = 0;
    bool fAll, fMob, fObj, fDoor;
    argument = one_argument( argument, arg);
    argument = one_argument( argument, arg1);
    argument = one_argument( argument, arg2);
    fAll = !str_cmp(arg, "all");
    fMob = !strncmp(arg, "mobile", 3);
    fObj = !strncmp(arg, "object", 3);
    fDoor = !strncmp(arg, "door", 3);

    if (fAll)
    {
	if (arg1[0] == '\0' || arg2[0] == '\0' )
	{
	send_to_char("Syntax: all <start #> <end #>\n\r",ch);
	return;
	}

    	number1 = atoi(arg1);
    	number2 = atoi(arg2);

	if ( number1 > number2 )
	{
	send_to_char("Cannot show resets backwards.\n\r",ch);
	return;
	}
	
	if ( number2 - number1 > 200 )
	{
	send_to_char("Do you really need to see that many at once?\n\r",ch);
	return;
	}
	
    }
    buf1[0] = '\0';

    for ( pReset = pArea->reset_first; pReset; pReset = pReset->next )
    {
	OBJ_INDEX_DATA  *pObj2;
	MOB_INDEX_DATA  *pMobIndex;
	OBJ_INDEX_DATA  *pObjIndex;
	OBJ_INDEX_DATA  *pObjToIndex;
	ROOM_INDEX_DATA *pRoomIndex;
	++iReset;
	if ( fAll)
	if (iReset < number1 || iReset > number2 )
	continue;	

	switch ( pReset->command )
	{
	default:
	    sprintf( buf, "Bad reset command: %c.", pReset->command );
	log_string(buf);
	    break;

	case 'M':
	mob_load_room = 0; 
	    if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Load Mobile - Bad Mob %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

	    if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
	    {
                sprintf( buf, "Load Mobile - Bad Room %ld\n\r", pReset->arg3 );
	log_string(buf);
                continue;
	    }
            pMob2 = pMobIndex;
	   if ( (pRoomIndex->vnum) == (ch->in_room->vnum) || fAll ||  ( fMob && is_name(arg1, pMob2->player_name) ))
	    {
            sprintf( buf, "%3d-> Loads %s (%ld) in %s (%ld) (max %d)%s [%d]",iReset,
                       pMob2->short_descr, pReset->arg1, pRoomIndex->name,
                       pReset->arg3, pReset->arg2,
		       ( pReset->vnum == 0 ? "" : ( pReset->vnum == -1 ? " as mounted" : " as grouped" ) ),
			pReset->chance );
		mob_load_room = ch->in_room->vnum;
		strcat( buf1, buf );
	    /*
	     * Check for pet shop.
	     */
	    {
		ROOM_INDEX_DATA *pRoomIndexPrev;

		pRoomIndexPrev = get_room_index( pRoomIndex->vnum - 1 );
		if ( pRoomIndexPrev
		    && IS_SET( pRoomIndexPrev->room_flags, ROOM_PET_SHOP ) )
                    sprintf( buf, " as a pet.\n\r" );
		else
                    sprintf( buf, "\n\r" );
		strcat( buf1, buf );
	    }
	   }
	    break;
	case 'O':
	obj_load_room = 0; 
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Load Object - Bad Object %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

            pObj2       = pObjIndex;

	    if ( !( pRoomIndex = get_room_index( pReset->arg3 ) ) )
	    {
                sprintf( buf, "Load Object - Bad Room %ld\n\r", pReset->arg3 );
	log_string(buf);
                continue;
	    }

	   if ( (pReset->arg3) == (ch->in_room->vnum) || fAll || ( fObj && is_name(arg1,pObj2->short_descr ) ) )
	   {
            sprintf( buf, "%3d-> Loads %s (%ld) in %s (%ld) (max %d) [%d]\n\r",iReset, 
                          pObj2->short_descr, pReset->arg1,
                          pRoomIndex->name, pReset->arg3, pReset->arg2, pReset->chance );
		strcat( buf1, buf );
		obj_load_room = ch->in_room->vnum;
	   }
	    break;

	case 'P':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Put Object - Bad Object %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

            pObj2       = pObjIndex;

	    if ( !( pObjToIndex = get_obj_index( pReset->arg3 ) ) )
	    {
                sprintf( buf, "Put Object - Bad To Object %ld\n\r", pReset->arg3 );
	log_string(buf);
                continue;
	    }
	   if ( (obj_load_room) == (ch->in_room->vnum) || fAll || ( fObj && is_name(arg1,pObj2->short_descr ) ) )
	    {
            sprintf( buf, "%3d-> Loads %s (%ld) inside %s (%ld) Max [%d] [%d]\n\r", iReset,
                          pObj2->short_descr,
                          pReset->arg1,
                          pObjToIndex->short_descr,
                          pReset->arg3,pReset->arg2, pReset->chance );
		strcat( buf1, buf );
	    }
	    break;

	case 'G':
	case 'E':
	    if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Give/Equip Object - Bad Object %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

            pObj2       = pObjIndex;

	    if ( !pMob2 )
	    {
                sprintf( buf, "Give/Equip Object - No Previous Mobile\n\r" );
	log_string(buf);
                continue;
	    }

            if ( pMob2->pShop && pReset->command == 'G' )
	{
	    if ( ( mob_load_room ) == (ch->in_room->vnum) || fAll ||  (fObj && is_name(arg1, pObj2->short_descr ) ))
	    {
            	sprintf( buf, "%3d-> Loads %s (%ld) in the inventory of %s (%ld) shopkeeper [%d]\n\r", iReset,
            	              pObj2->short_descr, pReset->arg1,
            	              pMob2->short_descr, pMob2->vnum, pReset->chance );
		strcat( buf1, buf );
	   obj_load_room = ch->in_room->vnum;
	    }
	}
            else
	    if ( ( mob_load_room ) == (ch->in_room->vnum) || fAll || (fObj && is_name(arg1, pObj2->short_descr ) ))
	   {
            sprintf( buf, "%3d-> Loads %s (%ld) %s of %s (%ld) [%d]\n\r", iReset,
                          pObj2->short_descr, pReset->arg1,
                          (pReset->command == 'G') ? wear_loc_name( WEAR_NONE ) 
                                                   : wear_loc_name( pReset->arg3 ),
                          pMob2->short_descr, pMob2->vnum, pReset->chance );
		strcat( buf1, buf );
	   obj_load_room = ch->in_room->vnum;
	   }
	    break;

        /*
         * Doors are automatically set as what they look like when the
         * database is edited.  I would like to do this will all the
         * reset information.
         */
	case 'D':
	    
	    if ( ( pReset->arg1 ) == (ch->in_room->vnum) || fAll || ( fDoor && is_name(arg1, dir_name[pReset->arg2] ) ))
	   {
            pRoomIndex = get_room_index( pReset->arg1 );
	    sprintf( buf, "%3d-> The %s door of %s is reset to %s [%d]\n", 
                iReset,
                dir_name[ pReset->arg2 ],
                pRoomIndex->name,
                get_exit_flag_string_1( pReset->arg3 ), pReset->chance );
		strcat( buf1, buf );
	   }
	    break;

	case 'R':
	    if ( !( pRoomIndex = get_room_index( pReset->arg1 ) ) )
	    {
                sprintf( buf, "Randomize Exits - Bad Room %ld\n\r", pReset->arg1 );
	log_string(buf);
                continue;
	    }

	if (( pReset->arg1 ) == ( ch->in_room->vnum) )
	  {
            sprintf( buf, "%3d-> Exits are randomized in %s (%ld) [%d]\n\r",
                         iReset, pRoomIndex->name, pReset->arg1, pReset->chance );
		strcat( buf1, buf );
	  }
	    break;
	}
    }
            sprintf( buf, "\n\rNext available reset -> %3d\n\r", iReset+1);
		strcat( buf1, buf );
	page_to_char(buf1,ch);
    return;
}


void add_reset( AREA_DATA *area, RESET_DATA *pReset, int reset_index )
{
    RESET_DATA *reset;
    int iReset = 0;

    if ( area->reset_first == NULL )
    {
        area->reset_first = pReset;
        area->reset_last  = pReset;
        pReset->next      = NULL;
        return;
    }

    reset_index--;

    if ( reset_index == 0 )
    {
        pReset->next = area->reset_first;
        area->reset_first = pReset;
        return;
    }
    SET_BIT( area->area_flags, AREA_CHANGED );

    for ( reset = area->reset_first; reset->next != NULL; reset = reset->next )
    {
        if ( ++iReset == reset_index ) break;
    }

    pReset->next = reset->next;
    reset->next  = pReset;
    if ( pReset->next == NULL ) area->reset_last = pReset;
    return;
}



int get_max_dir( CHAR_DATA *ch, int arg )
{
    char buf[MAX_INPUT_LENGTH];

    if ( arg < 0 || arg >= MAX_DIR )
    {
	sprintf( buf, "Doors range from 0 to %d.\n\r", MAX_DIR-1 );
	send_to_char( buf, ch );
	return -1;
    }
    else
	return arg;
}

 

void do_resets( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    char arg5[MAX_INPUT_LENGTH];
    char arg6[MAX_INPUT_LENGTH];
    char arg7[MAX_INPUT_LENGTH];
    char arg_all[MAX_INPUT_LENGTH];
    RESET_DATA *pReset = NULL;
    MOB_INDEX_DATA  *pMobIndex;
    OBJ_INDEX_DATA  *pObjIndex;
    int door, chance_max;
    char buf[MAX_STRING_LENGTH];
    strcpy(arg_all, argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );
    argument = one_argument( argument, arg5 );
    argument = one_argument( argument, arg6 );
    argument = one_argument( argument, arg7 );

    if ( arg2[0] == '\0' || !str_cmp(arg1, "all") || !strncmp(arg1, "mobile",3 ) || !strncmp(arg1, "object", 3) || !str_cmp(arg1, "door"))
    {
        if ( ch->in_room->area->reset_first != NULL )
        {
            send_to_char( "Resets: in room->\n\r", ch );
            display_resets( ch, arg_all );
        }
        else
            send_to_char( "No resets in this area.\n\r", ch );
    }

    if ( !IS_BUILDER( ch, ch->in_room->area ) )
    {
        send_to_char( "Resets: Invalid security for editing this area.\n\r", ch );
        return;
    }

    if ( is_number( arg1 ) )
    {
        AREA_DATA *pArea = ch->in_room->area;

        if ( !str_cmp( arg2, "delete" ) )
        {
            int insert_loc = atoi( arg1 );

            if ( ch->in_room->area->reset_first == NULL )
            {
                send_to_char( "No resets in this area.\n\r", ch );
                return;
            }

            if ( insert_loc-1 <= 0 )
            {
                pReset = pArea->reset_first;
                pArea->reset_first = pArea->reset_first->next;
                if ( pArea->reset_first == NULL )
                    pArea->reset_last = NULL;
            }
            else
            {
                int iReset = 0;
                RESET_DATA *prev = NULL;

                for ( pReset = pArea->reset_first; pReset != NULL; pReset = pReset->next )
                {
                    if ( ++iReset == insert_loc )   break;
                    prev = pReset;
                }

                if ( pReset == NULL )
                {
                    send_to_char( "Reset not found.\n\r", ch );
                    return;
                }

                if ( prev != NULL )  prev->next = prev->next->next;
                else pArea->reset_first = pArea->reset_first->next;

                for ( pArea->reset_last = pArea->reset_first;
                    pArea->reset_last->next != NULL;
                    pArea->reset_last = pArea->reset_last->next );
            }

            free_reset_data( pReset );
            send_to_char( "Reset deleted.\n\r", ch );
        }
        else
            if ( (!str_cmp( arg2, "mob" ) && is_number( arg3 ))
              || (!str_cmp( arg2, "obj" ) && is_number( arg3 )) 
              || (!str_cmp( arg2, "door" ) )
              || (!str_cmp( arg2, "random" ) ) )
            {
                pReset = reset_alloc();
	        if ( !str_cmp( arg2, "random") )
	        {
	            if ( arg3[0] == '\0' || !is_number( arg3 ) )
		        pReset->chance = 100;
	            else
		        pReset->chance = atoi( arg3 );
                    pReset->arg1    = ch->in_room->vnum;
                    pReset->arg2    = MAX_DIR;
                    pReset->command = 'R';
		
	        }
	        else if ( !str_cmp( arg2, "door") )
	        {
	            if ( ( door = get_direction_number_1(arg3) ) == MAX_DIR ) 
		    {
 	                sprintf( buf, "Bad door: Use, north, south, east...\n\rReset not set.\n\r");
 			send_to_char(buf,ch);
			return;
		    }
	            if ( !str_cmp( arg4, "open" ) )
	    	        pReset->arg3 = 0;	
	            else if ( !str_cmp( arg4, "closed" ) )
	    	        pReset->arg3 = 1;	
	            else if ( !str_cmp( arg4, "locked" ) )
	    	        pReset->arg3 = 2;	
	            else
		    {
 	                sprintf( buf, "Bad door Type: Use, open, closed, locked\n\rReset not set.\n\r");
 			send_to_char(buf,ch);
			return;
		    }	
	            if ( arg5[0] == '\0' || !is_number( arg5 ) )
		        pReset->chance = 100;
	            else
		        pReset->chance = atoi( arg5 );
                    pReset->arg1    = ch->in_room->vnum;
	            pReset->arg2    = door;
                    pReset->command = 'D';
	        }
		else if ( !str_cmp( arg2, "mob" ) )
	     	{
              	    pReset->arg1    = atoi( arg3 );
		    if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
 	 	    {
 	            	sprintf( buf, "Load Mobile Reset - Bad Mobile %ld\n\r", pReset->arg1 );
 			send_to_char(buf,ch);
 	               return;
		    }
/* check for mob == to ch->In_area */
		    if ( pMobIndex->load_with_area->areanum == 1 )
		    {
			send_to_char("loading from Global.are\n\r",ch);
		    } 
		    else
		        if ( ch->in_room->area != pMobIndex->load_with_area && !IS_OPTIVISION(ch) )
 	 	        {
 	                    sprintf( buf, "Load Mobile Reset - %ld\n\r", pReset->arg1 );
 	                    sprintf( buf, "Mobile does not belong to this area.\n\rReset not set.\n\r");
 			    send_to_char(buf,ch);
 	                    return;
		        }
                 {
             pReset->command = 'M';
	     if ( arg5[0] == '\0' || is_number( arg5 ) )
		pReset->vnum = 0;
	     else if ( !strncmp( arg5, "grouped" , strlen(arg5) ) )
 	     {
		pReset->vnum = atoi( arg6 );
		argument = one_argument(argument, arg5 );		
	     }
	     else if ( !strncmp( arg5, "mounted", strlen( arg5 ) ) )
	     {
		pReset->vnum = -1;
		argument = one_argument(argument, arg5 );		
	     }
             pReset->arg2    = is_number( arg4 ) ? atoi( arg4 ) : 5; /* Max # */
             pReset->arg3    = ch->in_room->vnum;
	     if ( arg5[0] == '\0' || !is_number( arg5 ) )
		pReset->chance = 100;
	     else
		pReset->chance = atoi( arg5 );
             }
	    }
            else
            if ( !str_cmp( arg2, "obj" ) )
            {
            pReset->arg1    = atoi( arg3 );
  /* blonde */ 
		  if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
 	 	    {
 	               sprintf( buf, "Give/Equip Object - Bad Object %ld\n\r", pReset->arg1 );
 			send_to_char(buf,ch);
 	               return;
		    }
/* check for object == to char->area */
		if ( pObjIndex->in_area->areanum == 1 )
		   {
			send_to_char("loading from Global.are\n\r",ch);
		   } 
		else if ( ch->in_room->area != pObjIndex->in_area && !IS_OPTIVISION(ch) )
 	 	    {
 	               sprintf( buf, "Load Object Reset - %ld\n\r", pReset->arg1 );
 	               send_to_char( "Object does not belong to this area.\n\rReset not set.\n\r",ch);
 			send_to_char(buf,ch);
 	               return;
		    }
            if ( !strncmp( arg4, "inside",3 ))
	    {
	      pReset->arg3 = atoi( arg5); 
		  if ( !( pObjIndex = get_obj_index( pReset->arg3 ) ) )
 	 	    {
 	               sprintf( buf, "Object inside of: NULL or  - Bad Object %ld\n\r", pReset->arg1 );
 			send_to_char(buf,ch);
 	               return;
		    }
		if ( ch->in_room->area != pObjIndex->in_area && !IS_OPTIVISION(ch) )
 	 	    {
 	               sprintf( buf, "Load Object Reset - %ld\n\r", pReset->arg1 );
 	               send_to_char( "Object does not belong to this area.\n\r",ch);
 	               send_to_char( "Please make sure you have a reset for the object\n\r you wish to load inside of in this room\n\rReset not set.\n\r",ch);
 			send_to_char(buf,ch);
 	               return;
		    }
                  pReset->command = 'P';
	     if ( arg6[0] == '\0' || !is_number( arg6 ) )
		pReset->arg2 = 1;
	     else
		pReset->arg2 = atoi( arg6 );
	     if ( pObjIndex->points < 11 )
	         chance_max = URANGE(1, 100 - ( pObjIndex->points * 3 / 2 ), 100 );
	     else
	         chance_max = URANGE(1, 120 - ( pObjIndex->points * 8 / 2 ), 100 );
	     if ( arg7[0] == '\0' || !is_number( arg7 ) )
		pReset->chance = chance_max;
	     else if (chance_max < atoi( arg7 ) )
	     {
		if ( IS_OPTIVISION(ch) )
		    pReset->chance = URANGE(1, atoi( arg7 ), 100);
		else
		{
		    send_to_char( "This item is too powerful to be reset at that percent.\n\r", ch);
		    return;
		}
	     }
	     else
		pReset->chance = URANGE(1, atoi( arg7 ), 100);
	     
            }
       else if ( !strncmp( arg4, "room",3 ) )
       {
           pReset->command = 'O';
           pReset->arg2     = UMAX(1,atoi(arg5));
           pReset->arg3     = ch->in_room->vnum;
	     if ( pObjIndex->points < 11 )
	         chance_max = URANGE(1, 100 - ( pObjIndex->points * 3 / 2 ), 100 );
	     else
	         chance_max = URANGE(1, 120 - ( pObjIndex->points * 8 / 2 ), 100 );
	   if ( arg6[0] == '\0' || !is_number( arg6 ) )
		pReset->chance = chance_max;
	   else if ( chance_max < atoi( arg6 ) )
	   {
		if ( IS_OPTIVISION(ch) )
		    pReset->chance = atoi( arg6 );
		else
		{
		    send_to_char( "This item is too powerful to be reset at that percent.\n\r", ch);
		    return;
		}
	   }
	   else
		pReset->chance = URANGE(1, atoi( arg6 ), 100);
       	}
       	else
       	{
            pReset->arg3     = wear_name_loc( arg4 );
            if ( pReset->arg3 == WEAR_NONE )
	    {
	    	if ( !strncmp( arg4, "inventory",3 ))
                pReset->command = 'G';
            else
	    {
            	do_help( ch, "wear_name_loc" );
                return;
	     }
	}
	else
            pReset->command = 'E';

	if ( pObjIndex->points < 11 )
	    chance_max = URANGE(1, 100 - ( pObjIndex->points * 3 / 2 ), 100 );
	else
	    chance_max = URANGE(1, 120 - ( pObjIndex->points * 8 / 2 ), 100 );

	if ( arg5[0] == '\0' || !is_number( arg5 ) )
	    pReset->chance = chance_max;
	else if ( chance_max < atoi( arg5 ) )
	{
	    if ( IS_OPTIVISION(ch) )
		pReset->chance = atoi( arg5 );
	    else
	    {
		send_to_char( "This item is too powerful to be reset at that percent.\n\r", ch);
		return;
	    }
	}
	else
	    pReset->chance = URANGE(1, atoi( arg5 ), 100);
    }
}

            add_reset( ch->in_room->area, pReset, atoi( arg1 ) );
            send_to_char( "Reset added.\n\r", ch );
        }
        else
        {
        send_to_char( "Syntax: RESET <number> OBJ <vnum> <location> [ %%chance ]\n\r", ch );
        send_to_char( "        RESET <number> OBJ <vnum> INSide <vnum> [<max #>] [ %%chance ]\n\r", ch );
        send_to_char( "        RESET <number> OBJ <vnum> ROOm [<max #>] [ %%chance]\n\r", ch );
        send_to_char( "        RESET <number> OBJ <vnum> INVentory [ %%chance ]\n\r", ch );
        send_to_char( "        RESET <number> MOB <vnum> [<max #>] [mounted / [grouped <vnum>] ] [ %%chance ]\n\r", ch );
        send_to_char( "        RESET <number> DOOR (north, south ...) (locked,open,closed) [ %%chance ]\n\r", ch );
        send_to_char( "        RESET <number> RANDOM [ %%chance ]\n\r", ch );
        send_to_char( "        RESET <number> DELETE\n\r", ch );
        send_to_char( "        RESET room (Display Resets in current room)\n\r", ch );
        send_to_char( "        RESET all (Display Resets in current area)\n\r", ch );
        send_to_char( "        RESET mob <name> (Display mob Resets in current area)\n\r", ch );
        send_to_char( "        RESET obj <name> (Display obj Resets in current area)\n\r", ch );
        send_to_char( "        RESET door <closed/open/locked..> (Display door Resets in current area)\n\r", ch );
        }
    }

    return;
}

/*
 * Container hatching
 */
void do_mob_inside( CHAR_DATA * ch, OBJ_INDEX_DATA * obj, char * argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    char arg5[MAX_INPUT_LENGTH];
    char arg6[MAX_INPUT_LENGTH];
    char arg7[MAX_INPUT_LENGTH];
    char arg_all[MAX_INPUT_LENGTH];
    RESET_DATA *pReset = NULL;
    MOB_INDEX_DATA  *pMobIndex;
    OBJ_INDEX_DATA  *pObjIndex;
    int door;
    char buf[MAX_STRING_LENGTH];

    if ( obj->cont_data == NULL )
    {
	sprintf( buf, "%s is not a container.\n\r", obj->short_descr );
	send_to_char( buf, ch );
	return;
    }

    strcpy(arg_all, argument);
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );
    argument = one_argument( argument, arg4 );
    argument = one_argument( argument, arg5 );
    argument = one_argument( argument, arg6 );
    argument = one_argument( argument, arg7 );

    if ( arg2[0] == '\0' || !str_cmp(arg1, "all") )
    {
        if ( obj->cont_data->inside )
        {
            send_to_char( "Inside Container->\n\r", ch );
            display_inside( ch, obj, arg_all );
        }
        else
            send_to_char( "Nothing inside the container.\n\r", ch );
    }

    if ( is_number( arg1 ) )
    {

        if ( !str_cmp( arg2, "delete" ) )
        {
        int insert_loc = atoi( arg1 );

        if ( obj->cont_data->inside == NULL )
        {
            send_to_char( "Nothing inside the container.\n\r", ch );
            return;
        }

        {
            int iReset = 0;
            RESET_DATA *prev = NULL;

            for ( pReset = obj->cont_data->inside; pReset != NULL; pReset = pReset->next )
            {
                if ( ++iReset == insert_loc )   break;
                prev = pReset;
            }

            if ( pReset == NULL )
            {
                send_to_char( "Inside reset not found.\n\r", ch );
                return;
            }

            if ( prev != NULL )  prev->next = prev->next->next;
                            else obj->cont_data->inside = obj->cont_data->inside->next;

        }

        free_reset_data( pReset );
        send_to_char( "Inside reset deleted.\n\r", ch );
	SET_BIT( obj->in_area->area_flags, AREA_CHANGED );
        }
        else
        if ( (!str_cmp( arg2, "mob" ) && is_number( arg3 ))
          || (!str_cmp( arg2, "obj" ) && is_number( arg3 )) 
          || (!str_cmp( arg2, "door" ) )
          || (!str_cmp( arg2, "random" ) ) )
        {
            pReset = reset_alloc();
	    if ( !str_cmp( arg2, "random") )
	    {
	     if ( arg3[0] == '\0' || !is_number( arg3 ) )
		pReset->chance = 100;
	     else
		pReset->chance = atoi( arg3 );
             pReset->arg2    = MAX_DIR;
             pReset->command = 'R';
		
	    }
	    else if ( !str_cmp( arg2, "door") )
	    {
	     if ( ( door = get_direction_number_1(arg3) ) == MAX_DIR ) 
		{
 	                sprintf( buf, "Bad door: Use, north, south, east...\n\rInside reset not set.\n\r");
 			send_to_char(buf,ch);
			return;
		}
	     if ( !str_cmp( arg4, "open" ) )
	    	pReset->arg3 = 0;	
	     else if ( !str_cmp( arg4, "closed" ) )
	    	pReset->arg3 = 1;	
	     else if ( !str_cmp( arg4, "locked" ) )
	    	pReset->arg3 = 2;	
	     else
		{
 	                sprintf( buf, "Bad door Type: Use, open, closed, locked\n\rInside Reset not set.\n\r");
 			send_to_char(buf,ch);
			return;
		}	
	     if ( arg5[0] == '\0' || !is_number( arg5 ) )
		pReset->chance = 100;
	     else
		pReset->chance = atoi( arg5 );
	     pReset->arg2    = door;
             pReset->command = 'D';
	    }

            else if ( !str_cmp( arg2, "mob" ) )
	    {
              pReset->arg1    = atoi( arg3 );
		if ( !( pMobIndex = get_mob_index( pReset->arg1 ) ) )
 	 	    {
 	               sprintf( buf, "Load Mobile Inside Reset - Bad Mobile %ld\n\r", pReset->arg1 );
 			send_to_char(buf,ch);
 	               return;
		    }
		if ( pMobIndex->load_with_area->areanum == 1 )
		   {
			send_to_char("loading from Global.are\n\r",ch);
		   } 
		else
		if ( obj->in_area != pMobIndex->load_with_area && !IS_OPTIVISION(ch) )
 	 	    {
 	               sprintf( buf, "Mobile does not belong to this area.\n\rInside reset not set.\n\r");
 			send_to_char(buf,ch);
 	               return;
		    }
             {
             pReset->command = 'M';
	     if ( arg5[0] == '\0' || is_number( arg5 ) )
		pReset->vnum = 0;
	     else if ( !strncmp( arg5, "grouped" , strlen(arg5) ) )
 	     {
		pReset->vnum = atoi( arg6 );
		argument = one_argument(argument, arg5 );		
	     }
	     else if ( !strncmp( arg5, "mounted", strlen( arg5 ) ) )
	     {
		pReset->vnum = -1;
		argument = one_argument(argument, arg5 );		
	     }
             pReset->arg2    = is_number( arg4 ) ? atoi( arg4 ) : 5; /* Max # */
	     if ( arg5[0] == '\0' || !is_number( arg5 ) )
		pReset->chance = 100;
	     else
		pReset->chance = atoi( arg5 );
             }
	    }
            else
            if ( !str_cmp( arg2, "obj" ) )
            {
            pReset->arg1    = atoi( arg3 );
  /* blonde */ 
		  if ( !( pObjIndex = get_obj_index( pReset->arg1 ) ) )
 	 	    {
 	               sprintf( buf, "Give/Equip Object - Bad Object %ld\n\r", pReset->arg1 );
 			send_to_char(buf,ch);
 	               return;
		    }
		if ( pObjIndex->in_area->areanum == 1 )
		   {
			send_to_char("loading from Global.are\n\r",ch);
		   } 
		else if ( obj->in_area != pObjIndex->in_area && !IS_OPTIVISION(ch) )
 	 	    {
 	               sprintf( buf, "Load Object Reset - %ld\n\r", pReset->arg1 );
 	               send_to_char( "Object does not belong to this area.\n\rInside reset not set.\n\r",ch);
 			send_to_char(buf,ch);
 	               return;
		    }
            if ( !strncmp( arg4, "inside",3 ))
	    {
	      pReset->arg3 = atoi( arg5); 
		  if ( !( pObjIndex = get_obj_index( pReset->arg3 ) ) )
 	 	    {
 	               sprintf( buf, "Object inside of: NULL or  - Bad Object %ld\n\r", pReset->arg1 );
 			send_to_char(buf,ch);
 	               return;
		    }
		if ( obj->in_area != pObjIndex->in_area && !IS_OPTIVISION(ch) )
 	 	    {
 	               sprintf( buf, "Load Object Inside Reset - %ld\n\r", pReset->arg1 );
 	               send_to_char( "Object does not belong to this area.\n\r",ch);
 	               send_to_char( "Please make sure you have a reset for the object\n\r you wish to load inside of in this room\n\rInside reset not set.\n\r",ch);
 			send_to_char(buf,ch);
 	               return;
		    }
                  pReset->command = 'P';
	     if ( arg6[0] == '\0' || !is_number( arg6 ) )
		pReset->arg2 = 1;
	     else
		pReset->arg2 = atoi( arg6 );
	     if ( arg7[0] == '\0' || !is_number( arg7 ) )
		pReset->chance = 100;
	     else
		pReset->chance = atoi( arg7 );
	     
            }
       else if ( !strncmp( arg4, "room",3 ) )
       {
           pReset->command = 'O';
           pReset->arg2     = UMAX(1,atoi(arg5));
	     if ( arg6[0] == '\0' || !is_number( arg6 ) )
		pReset->chance = 100;
	     else
		pReset->chance = atoi( arg6 );
       }
       else
       {
           pReset->arg3     = wear_name_loc( arg4 );
           if ( pReset->arg3 == WEAR_NONE )
	     {
	     
            if ( !strncmp( arg4, "inventory",3 ))
                pReset->command = 'G';
             else
		{
                 do_help( ch, "wear_name_loc" );
                 return;
	        }
	      }
	    else
               pReset->command = 'E';
	     if ( arg5[0] == '\0' || !is_number( arg5 ) )
		pReset->chance = 100;
	     else
		pReset->chance = atoi( arg5 );
       }
            }

	    if ( obj->cont_data->inside == NULL )
	    {
		pReset->next = NULL;
		obj->cont_data->inside = pReset;
	    }
	    else
	    {
		int iReset=0,reset_index;

		reset_index = atoi( arg1 ) - 1;

    		if ( reset_index == 0 )
    		{
    		    pReset->next = obj->cont_data->inside;
    		    obj->cont_data->inside = pReset;
    		}
		else
		{
		    RESET_DATA * reset;

		    for ( reset = obj->cont_data->inside; reset->next != NULL; reset = reset->next )
    		    {
    		        if ( ++iReset == reset_index ) break;
    		    }

    		    pReset->next = reset->next;
    		    reset->next  = pReset;
		}
	    }
            send_to_char( "Inside Reset added.\n\r", ch );
	    SET_BIT( obj->in_area->area_flags, AREA_CHANGED );
        }
        else
        {
        send_to_char( "Syntax: IRESET <number> OBJ <vnum> <location> [ %%chance ]\n\r", ch );
        send_to_char( "        IRESET <number> OBJ <vnum> INSide <vnum> [<max #>] [ %%chance ]\n\r", ch );
        send_to_char( "        IRESET <number> OBJ <vnum> ROOm [<max #>] [ %%chance]\n\r", ch );
        send_to_char( "        IRESET <number> OBJ <vnum> INVentory [ %%chance ]\n\r", ch );
        send_to_char( "        IRESET <number> MOB <vnum> [<max #>] [mounted / [grouped <vnum>] ] [ %%chance ]\n\r", ch );
        send_to_char( "        IRESET <number> DOOR (north, south ...) (locked,open,closed) [ %%chance ]\n\r", ch );
        send_to_char( "        IRESET <number> RANDOM [ %%chance ]\n\r", ch );
        send_to_char( "        IRESET <number> DELETE\n\r", ch );
        send_to_char( "        IRESET all (Display all inside resets)\n\r", ch );
        }
    }

    return;
}

void do_autosave(CHAR_DATA *ch, char *argument)
{
AREA_DATA *pArea;
char buf[MAX_STRING_LENGTH];

pArea = ch->in_room->area;

     if ( IS_SET( pArea->area_flags, AREA_AUTOSAVE ) )
	{
	 REMOVE_BIT(pArea->area_flags, AREA_AUTOSAVE);
	 sprintf(buf,"Autosave for %s is now off\n\r",pArea->name);
	 send_to_char(buf,ch);
	 return;
	}
     if ( !IS_SET( pArea->area_flags, AREA_AUTOSAVE ) )
	{
	 SET_BIT(pArea->area_flags, AREA_AUTOSAVE); 
	 sprintf(buf,"Autosave for %s is now on\n\r",pArea->name);
	 send_to_char(buf,ch);
	}
return;	
}


void save_area_list()
{
    FILE *fp;
    AREA_DATA *pArea;
    if ( ( fp = fopen( "area.lst", "w" ) ) == NULL )
    {
	bug( "Save_area_list: fopen", 0 );
	perror( "area.lst" );
    }
    else
    {
	for( pArea = area_first; pArea != NULL; pArea = pArea->next )
	{
	    if (!IS_SET(pArea->area_flags, AREA_VIRTUAL ) ) /* don't save the virtual area */
	    fprintf( fp, "%s\n", pArea->filename );
	}

	fprintf( fp, "$\n" );
	if ( fp )
	fclose( fp );
    }

    return;
}



char *fix_string( const char *str )
{
    static char strfix[MAX_STRING_LENGTH];
    int i;
    int o;

    if ( str == NULL )
        return '\0';

    for ( o = i = 0; str[i+o] != '\0'; i++ )
    {
        if (str[i+o] == '\r' || str[i+o] == '~')
            o++;
        strfix[i] = str[i+o];
    }
    strfix[i] = '\0';
    return strfix;
}

void save_social(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    FILE *fp;
   int iSocial; 

/* Save the socials in the socials directory */

    sprintf(buf, "%s", SOCIALSAVE_FILE);
	if ( ( fp= fopen(buf, "w") ) == NULL )
	{
                sprintf( buf, "Save_social: Unable to open file! social.are");
                bug( buf, 0 );
		return;
	}

      for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
	 {
	  fprintf(fp,"%s %d %d %ld %d %d\n",social_table[iSocial].name,social_table[iSocial].type, social_table[iSocial].damage,social_table[iSocial].item,social_table[iSocial].action,social_table[iSocial].mood);
	  fprintf(fp,"%s\n",social_table[iSocial].char_no_arg[0] == '\0' ? "(null)" : social_table[iSocial].char_no_arg );
	  fprintf(fp,"%s\n",social_table[iSocial].others_no_arg[0] == '\0' ? "(null)" : social_table[iSocial].others_no_arg );
	  fprintf(fp,"%s\n",social_table[iSocial].char_found[0] == '\0' ? "(null)" : social_table[iSocial].char_found) ;
	  fprintf(fp,"%s\n",social_table[iSocial].others_found[0] == '\0' ? "(null)" : social_table[iSocial].others_found );
	  fprintf(fp,"%s\n",social_table[iSocial].vict_found[0] == '\0' ? "(null)" : social_table[iSocial].vict_found );
	  fprintf(fp,"%s\n",social_table[iSocial].char_not_found[0] == '\0' ? "(null)" : social_table[iSocial].char_not_found );
	  fprintf(fp,"%s\n",social_table[iSocial].char_auto[0] == '\0' ? "(null)" : social_table[iSocial].char_auto );
	  fprintf(fp,"%s\n",social_table[iSocial].others_auto[0] == '\0' ? "(null)": social_table[iSocial].others_auto );
          fprintf(fp,"\n" );
	 } 

    fprintf( fp, "#0\n#$\n" );
   if ( fp )
   fclose(fp);
if (ch != NULL )
   send_to_char("Social's saved.\n\r",ch);
    else
    {
	sprintf( buf, "Autosave:  Socials" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	REMOVE_BIT( changed_system, CHANGED_SOCIALS );
    }
    return;
}

void save_help(CHAR_DATA *ch, char *argument)
{
    HELP_DATA *pHelp;
    char buf[MAX_STRING_LENGTH];
    FILE *fp;
    
/* save all the helps in the help directory */

    sprintf(buf, "%s", HELPSAVE_FILE);
	if ( ( fp= fopen(buf, "w") ) == NULL )
	{
                sprintf( buf, "Save_helps: Unable to open file! help.are");
                bug( buf, 0 );
		return;
	}

    for ( pHelp = help_first; pHelp; pHelp = pHelp->next )
    {
       fprintf( fp, "%d %d %s~\n",pHelp->level,pHelp->vnum,   pHelp->keyword); 
	if (!str_cmp( pHelp->keyword, "greeting" ) || !str_cmp(pHelp->keyword, "motd" ) || !str_cmp(pHelp->keyword, "hmotd" ) || !str_cmp(pHelp->keyword, "imotd") || !str_cmp(pHelp->keyword, "race help")
	 || !str_cmp(pHelp->keyword, "group header" ) || !str_cmp(pHelp->keyword, "menu choice" ) )
         fprintf( fp, "%s~",pHelp->text); 
	else
         fprintf( fp, "%s~\n",pHelp->text); 
        
    }

    fprintf( fp, "0 0 $~\n" );
   if ( fp )
   fclose(fp);
    if (ch != NULL )
	send_to_char("Helps Saved.\n\r",ch);
    else
    {
	sprintf( buf, "Autosave:  Helps" );
	info_channel(NULL, buf, INFOACT_SAVE_ZONE );
	REMOVE_BIT( changed_system, CHANGED_HELPS );
    }
    return;
}

void save_specials( FILE *fp, AREA_DATA *pArea )
{
    int iHash;
    MOB_INDEX_DATA *pMobIndex;
    
    fprintf( fp, "#SPECIALS\n" );

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pMobIndex = mob_index_hash[iHash]; pMobIndex; pMobIndex = pMobIndex->next )
        {
            if ( pMobIndex && pMobIndex->load_with_area == pArea && pMobIndex->spec_fun )
            {
                fprintf( fp, "M %ld %s Load to: %s\n", pMobIndex->vnum,
                                                      pMobIndex->spec_name,
                                                      pMobIndex->short_descr );
            }
        }
    }

    fprintf( fp, "S\n\n\n\n" );
    return;
}


void do_save_resets( CHAR_DATA *ch, char *argument)
{
    save_resets(ch->in_room->area, argument );
    send_to_char("Resets Saved\n\r",ch);
    return;
}

void save_resets( AREA_DATA *pArea, char *arg )
{
    RESET_DATA *pReset;
    MOB_INDEX_DATA *pLastMob = NULL;
    OBJ_INDEX_DATA *pLastObj;
    OBJ_INDEX_DATA *pLastObj2;
    ROOM_INDEX_DATA *pRoom;
    EQUIP_DATA *equip;
    EVENT_DATA *event;
    INSIDE_DATA *inside;
    char buf[MAX_STRING_LENGTH]; 
    FILE *f;
    sprintf(buf, "%s%s/resets", SAVEAREAS_DIR, arg);
	if ( ( f = fopen(buf, "w") ) == NULL )
	{
                sprintf( buf, "Save_resets: Unable to open file! [%s]", pArea->filename );
                bug( buf, 0 );
		return;
	}

    for ( pReset = pArea->reset_first; pReset; pReset = pReset->next )
    {
	switch ( pReset->command )
	{
	default:
	    bug( "Save_resets: bad command %c.", pReset->command );
	    break;

#if defined( VERBOSE )
	case 'M':
            pLastMob = get_mob_index( pReset->arg1 );
	    fprintf( f, "M %d %d %d %ld %d Load %s\n", 
	        pReset->chance,
	        pReset->arg1,
                pReset->arg2,
                pReset->vnum,
                pReset->arg3,
                pLastMob->short_descr );
            break;

	case 'O':
            pLastObj = get_obj_index( pReset->arg1 );
            pRoom = get_room_index( pReset->arg3 );
	    fprintf( f, "O %d %d %d %ld %d %s loaded to %s\n", 
	        pReset->chance,
	        pReset->arg1,
		pReset->arg2,
		pReset->vnum,
                pReset->arg3,
                UPPER(pLastObj->short_descr),
                pRoom->name );
            break;

	case 'P':
            pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( f, "P %d %d %d %ld %d %s put inside %s\n", 
	        pReset->chance,
	        pReset->arg1,
                pReset->arg2,
                pReset->vnum,
                pReset->arg3,
                UPPER(get_obj_index( pReset->arg1 )->short_descr),
                pLastObj->short_descr );
            break;

	case 'G':
	    fprintf( f, "G %d %d %ld 0 %s is given to %s\n",
	        pReset->chance,
	        pReset->arg1,
	        pReset->vnum,
	        UPPER(get_obj_index( pReset->arg1 )->short_descr),
                pLastMob ? pLastMob->short_descr : "!NO_MOB!" );
            if ( !pLastMob )
            {
                sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->filename );
                bug( buf, 0 );
            }
            break;

	case 'E':
	    fprintf( f, "E %d %d 0 %ld %d %s is loaded %s of %s\n",
	        pReset->chance,
	        pReset->arg1,
	        pReset->vnum,
                pReset->arg3,
                UPPER(get_obj_index( pReset->arg1 )->short_descr),
                wear_loc_name( pReset->arg3 ),
                pLastMob ? pLastMob->short_descr : "!NO_MOB!" );
            if ( !pLastMob )
            {
                sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->filename );
                bug( buf, 0 );
            }
            break;

	case 'D':
            pRoom = get_room_index( pReset->arg1 );
	    fprintf( f, "D %d %d %d %ld %d %s\n", 
	        pReset->chance,
	        pReset->arg1,
                pReset->arg2,
                pReset->vnum,
                pReset->arg3,
                pRoom->name );
            break;

	case 'R':
            pRoom = get_room_index( pReset->arg1 );
	    fprintf( f, "R %d %d %d %ldRandomize %s\n", 
	        pReset->chance,
	        pReset->arg1,
                pReset->arg2,
                pReset->vnum,
                pRoom->name );
            break;
            }
#endif
#if !defined( VERBOSE )
	case 'M':
            pLastMob = get_mob_index( pReset->arg1 );
	    pRoom = get_room_index (pReset->arg3 );
	    fprintf( f, "M %d %ld %d %ld %ld\n", 
	        pReset->chance,
	        pReset->arg1,
                pReset->arg2,
                pReset->vnum,
                pReset->arg3 );
	event = event_alloc();
	event->next = pArea->events;
	pArea->events = event;
	event->type = EVENT_ROOM;
	event->chance = pReset->chance;
	event->room_vnum = pRoom->vnum;
	event->area = pRoom->area;
	event->mob_vnum = pReset->arg1;
	event->flags = EVENT_ON_BOOT|EVENT_ON_RESET;
	break;

	case 'O':
            pLastObj = get_obj_index( pReset->arg1 );
            pRoom = get_room_index( pReset->arg3 );
	    fprintf( f, "O %d %ld %d %ld %ld\n", 
	        pReset->chance,
	        pReset->arg1,
	        pReset->arg2,
	        pReset->vnum,
                pReset->arg3 );
	event = event_alloc();
	event->next = pArea->events;
	pArea->events = event;
	event->type = EVENT_ROOM;
	event->chance = pReset->chance;
	event->room_vnum = pRoom->vnum;
	event->area = pRoom->area;
	event->obj_vnum = pReset->arg1;
	event->flags = EVENT_ON_BOOT|EVENT_ON_RESET;
            break;

	case 'P':
            pLastObj = get_obj_index( pReset->arg1 );
            pLastObj2 = get_obj_index( pReset->arg3 );
	    fprintf( f, "P %d %ld %d %ld %ld\n", 
	        pReset->chance,
	        pReset->arg1,
	        pReset->arg2,
	        pReset->vnum,
                pReset->arg3  );
	if ( pLastObj && pLastObj2 )
	{
	    inside = inside_alloc();
	    inside->obj_vnum = pLastObj2->vnum; 
	    inside->chance = pReset->chance;
	    inside->flags = EQUIP_ON_BOOT|EQUIP_ON_CREATION;
	    inside->next = pLastObj->inside;
	    pLastObj->inside = inside; 
	}
	break;

	case 'G':
            pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( f, "G %d %ld %ld 0\n",pReset->chance, pReset->arg1 , pReset->vnum );
            if ( !pLastMob )
            {
                sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->filename );
                bug( buf, 0 );
            }
	    if ( pLastObj && pLastMob )
	    {
	    equip = equip_alloc();
	    equip->obj_vnum = pLastObj->vnum; 
	    equip->chance = pReset->chance;
	    equip->flags = EQUIP_ON_BOOT|EQUIP_ON_CREATION|EQUIP_INVENTORY;
	    equip->next = pLastMob->equip;
	    pLastMob->equip = equip; 
	    }
            break;

	case 'E':
            pLastObj = get_obj_index( pReset->arg1 );
	    fprintf( f, "E %d %ld 0 %ld %ld\n",
	        pReset->chance,
	        pReset->arg1,
	        pReset->vnum,
                pReset->arg3 );
            if ( !pLastMob )
            {
                sprintf( buf, "Save_resets: !NO_MOB! in [%s]", pArea->filename );
                bug( buf, 0 );
            }
	    if ( pLastObj && pLastMob )
	    {
	    equip = equip_alloc();
	    equip->obj_vnum = pLastObj->vnum; 
	    equip->chance = pReset->chance;
	    equip->flags = EQUIP_ON_BOOT|EQUIP_ON_CREATION;
	    equip->next = pLastMob->equip;
	    pLastMob->equip = equip; 
	    }
            break;

	case 'D':
            pRoom = get_room_index( pReset->arg1 );
	    fprintf( f, "D %d %ld %d %ld %ld \n", 
	        pReset->chance,
	        pReset->arg1,
                pReset->arg2,
                pReset->vnum,
                pReset->arg3);
            break;

	case 'R':
            pRoom = get_room_index( pReset->arg1 );
            pLastObj = get_obj_index( pReset->arg2 );
	    fprintf( f, "R %d %ld %d %ld\n", 
	        pReset->chance,
	        pReset->arg1,
                pReset->arg2,
                pReset->vnum );
            break;
            }
#endif
        }

    fprintf( f, "S\n" );
    if ( f )
    fclose(f);
    return;
}



void save_shops( AREA_DATA *pArea, char *arg )
{
    SHOP_DATA *pShopIndex;
    MOB_INDEX_DATA *pMobIndex;
    int iHash;
    char buf[MAX_STRING_LENGTH];
    FILE *f;
    sprintf(buf, "%s%s/shops", SAVEAREAS_DIR, arg);
	if ( ( f = fopen(buf, "w") ) == NULL )
	{
                sprintf( buf, "Save_shops: Unable to open file! [%s]", pArea->filename );
                bug( buf, 0 );
		return;
	}
    

    for( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for( pMobIndex = mob_index_hash[iHash]; pMobIndex; pMobIndex = pMobIndex->next )
        {
            if ( pMobIndex && pMobIndex->load_with_area == pArea && pMobIndex->pShop )
            {
                pShopIndex = pMobIndex->pShop;
                fprintf( f, "%ld ", pShopIndex->keeper );
		fprintf( f, "-1 %d ", pShopIndex->type );
                fprintf( f, "%d %d ", pShopIndex->profit_buy, pShopIndex->profit_sell );
                fprintf( f, "%d %d\n", pShopIndex->open_hour, pShopIndex->close_hour );
            /*  fprintf( f, "%s\n",  pShopIndex->owner ); */
            }
        }
    }

    fprintf( f, "0\n\n\n\n" );
    if ( f )
    fclose(f);
    return;
}

void save_area( AREA_DATA *pArea, CHAR_DATA *ch, char *arg2 )
{
    int season;
    FILE *fp;
    char buf[MAX_INPUT_LENGTH];


    if (IS_SET(pArea->area_flags, AREA_VIRTUAL ) ) /* don't save the virtual area */
	return;
    /* Create directory as needed */
    sprintf(buf, "%s%s/", SAVEAREAS_DIR, pArea->filename);
    if ( (fp = fopen(buf, "r" ) ) == NULL)
    {
	sprintf(buf, "mkdir %s%s", SAVEAREAS_DIR, pArea->filename);
	system(buf);
    }
    if ( fp )
    fclose(fp);

    sprintf(buf,"%s%s/area", SAVEAREAS_DIR, pArea->filename); 
    if ( ( fp = fopen( buf, "w" ) ) == NULL )
    {
	bug( "Open_area: fopen", 0 );
	perror( pArea->filename );
	return;
    }

    fprintf( fp, "#AREADATA\n" );
    fprintf( fp, "Name        %s~\n",        pArea->name );
    fprintf( fp, "Credits     %s~\n",        pArea->credits );
    fprintf( fp, "Builders    %s~\n",        fix_string( pArea->builders ) );
    fprintf( fp, "VNUMs       %ld %ld\n",      pArea->lvnum, pArea->uvnum );
    fprintf( fp, "AGEs        %d %d\n",      pArea->max_age, pArea->max_age_empty );
    fprintf( fp, "Wander_time %d\n",         pArea->wander_time);
    fprintf( fp, "Visible     %d\n",         pArea->visible);
    fprintf( fp, "Security    %d\n",         pArea->security );
    fprintf( fp, "Flags	      %ld\n",	     pArea->area_flags&~(AREA_CHANGED|AREA_ADDED|AREA_LOADING) );
    fprintf( fp, "World      %d\n",         pArea->world );
    fprintf( fp, "Vnum       %d\n",         pArea->vnum == 0 ? pArea->areanum : pArea->vnum );
    fprintf( fp, "Spring %d\n", pArea->weather->spring );
    fprintf( fp, "Summer %d\n", pArea->weather->summer );
    fprintf( fp, "Fall %d\n", pArea->weather->fall );
    fprintf( fp, "Winter %d\n", pArea->weather->winter );
    fprintf( fp, "Speed %d\n", pArea->weather->speed );
    /* castle code */
    fprintf( fp, "Castle %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld %ld\n", pArea->castle_data->castle, pArea->castle_data->barracks, pArea->castle_data->barracks2, pArea->castle_data->library, pArea->castle_data->library2, pArea->castle_data->smithy, pArea->castle_data->smithy2, pArea->castle_data->inn, pArea->castle_data->inn2, pArea->castle_data->armory, pArea->castle_data->magic_shop, pArea->castle_data->grainery, pArea->castle_data->bank, pArea->castle_data->track );
    fprintf( fp, "Storm" );
    for ( season = 0 ; season < 4 ; season++ )
	fprintf( fp, " %d", pArea->weather->storm_type[ season ] );
    fprintf( fp, "\n" );
    fprintf( fp, "Chance" );
    for ( season = 0 ; season < 4 ; season++ )
	fprintf( fp, " %d %d %d %d", 
		pArea->weather->chance[ season ][ 0 ],
		pArea->weather->chance[ season ][ 1 ],
		pArea->weather->chance[ season ][ 2 ],
		pArea->weather->chance[ season ][ 3 ] );
    fprintf( fp, "\n" );
    fprintf( fp, "End\n\n\n\n" );
    if ( fp )
    fclose( fp );
    do_areasave( pArea, arg2, ch );

    return;
}


void do_asave( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH] ="all";
    AREA_DATA *pArea;
    int value;
    if ( ch )
    pArea = ch->in_room->area;
    argument = one_argument( argument, arg1 );
    tail_chain();
    if ( !str_cmp( "changed", arg1 ) )
    {
        char buf[MAX_INPUT_LENGTH];

        save_area_list();

        for( pArea = area_first; pArea != '\0'; pArea = pArea->next )
        {
            if ( IS_SET(pArea->area_flags, AREA_CHANGED) && IS_SET(pArea->area_flags, AREA_AUTOSAVE ) && !IS_SET(pArea->area_flags, AREA_VIRTUAL ) )
            {
                save_area( pArea, NULL, "world" );
                sprintf( buf, "Autosave: %24s - '%s'", pArea->name, pArea->filename );
                info_channel(NULL, buf, INFOACT_SAVE_ZONE );
                REMOVE_BIT( pArea->area_flags, AREA_CHANGED );
            }
        }
        return;
    }

    if ( !str_cmp( "changed_no_message", arg1 ) )
    {

        save_area_list();

        for( pArea = area_first; pArea != '\0'; pArea = pArea->next )
        {
            if ( IS_SET(pArea->area_flags, AREA_CHANGED) && IS_SET(pArea->area_flags, AREA_AUTOSAVE ) && !IS_SET(pArea->area_flags, AREA_VIRTUAL ) )
            {
                save_area( pArea, NULL, "world" );
                REMOVE_BIT( pArea->area_flags, AREA_CHANGED );
            }
        }
        return;
    }

    pArea = ch->in_room->area;

    if ( arg1[0] == '\0' )
    {
    send_to_char( "Syntax:\n\r", ch );
    send_to_char( "  asave list          - saves the area.lst file\n\r",    ch ); 
    send_to_char( "  asave zone / all    - saves the zone you're in\n\r",     ch );
    send_to_char( "  asave changed       - saves all changed zones\n\r",    ch ); 
    send_to_char( "  asave MOBiles       - saves the mobiles in this area\n\r", ch ); 
    send_to_char( "  asave OBJects       - saves the objects in this area\n\r", ch ); 
    send_to_char( "  asave ROOms         - saves the rooms in this area\n\r", ch ); 
    send_to_char( "  asave EVEnts        - saves the events in this area\n\r", ch ); 
    send_to_char( "  asave SHOps         - saves the shops in this area\n\r", ch ); 
    send_to_char( "  asave world         - saves the world! (db dump)\n\r", ch ); 
    send_to_char( "  asave clans         - saves all the clans\n\r", ch ); 
    send_to_char( "  asave races         - saves all the races\n\r", ch ); 
    send_to_char( "  asave triggers      - saves all the triggers\n\r", ch ); 
    send_to_char( "  asave helps         - saves all the helps\n\r", ch ); 
    send_to_char( "  asave socials       - saves all the socials\n\r", ch ); 
    send_to_char( "\n\r", ch );
        return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg1 );

    if ( ( pArea = get_area_data( value ) ) == '\0' && is_number( arg1 ) )
    {
        send_to_char( "That area does not exist.\n\r", ch );
        return;
    }

    if ( is_number( arg1 ) )
    {
        save_area_list();
        if ( !IS_SET(pArea->area_flags, AREA_VIRTUAL ) )
        save_area( pArea, ch, arg2 );
        return;
    }

    if ( !str_cmp( "world", arg1 ) )
    {
        save_area_list();
        for( pArea = area_first; pArea != '\0'; pArea = pArea->next )
        {
            if ( !IS_SET(pArea->area_flags, AREA_VIRTUAL ) )
            save_area( pArea, ch, "world" );
            REMOVE_BIT( pArea->area_flags, AREA_CHANGED ); 
        }
        send_to_char( "You saved the world.\n\r", ch );
	send_to_all_char( "Database saved.\n\r" );
        return;
    }

    if ( !strncmp( arg1, "helps", 3 ) )
    {
      save_help(NULL, NULL);
      send_to_char( "helps saved.\n\r", ch );
      return;
    }

    if ( !strncmp( arg1, "socials", 3 ) )
    {
      save_social(NULL, NULL);
      send_to_char( "socials saved.\n\r", ch );
      return;
    }

    if ( !strncmp( arg1, "triggers", 3 ) )
    {
      save_triggers(NULL, NULL);
      send_to_char( "triggers saved.\n\r", ch );
      return;
    }

    if ( !strncmp( arg1, "clans" , 3) )
    {
      save_clans();
      send_to_char( "Clans saved.\n\r", ch );
      return;
    }

    if ( !strncmp( arg1, "races", 3 ) )
    {
      save_races();
      save_rlist();
      send_to_char( "Race and Race list saved.\n\r", ch );
      return;
    }

    if ( !str_cmp( arg1, "list" ) )
    {
      save_area_list();
      send_to_char( "List saved.\n\r", ch );
      return;
    }

    if ( !str_cmp( arg1, "zone" ) )
    {
    tail_chain();
      if ( IS_SET(ch->in_room->area->area_flags, AREA_VIRTUAL ) )
	{
      	send_to_char( "Virtual area:  Not saved.\n\r", ch );
	return;
	}
    tail_chain();
      save_area_list();
    tail_chain();
      save_area( ch->in_room->area, ch, arg2 );
    tail_chain();
      send_to_char( "saving zone.\n\r", ch );
      send_to_char( "Zone saved.\n\r", ch );
    tail_chain();
      REMOVE_BIT( ch->in_room->area->area_flags, AREA_CHANGED );
    tail_chain();
      return;
    }
    if ( !strncmp( arg1, "rooms",3 ) )
    {
      save_area( ch->in_room->area, ch, "rooms" );
      return;
    }
    if ( !strncmp( arg1, "objects",3 ) )
    {
      if ( IS_SET(ch->in_room->area->area_flags, AREA_VIRTUAL ) )
	{
      	send_to_char( "Virtual area:  Not saved.\n\r", ch );
	return;
	}
      save_area( ch->in_room->area, ch, "objs" );
      return;
    }
    if ( !strncmp( arg1, "mobiles",3 ) )
    {
      if ( IS_SET(ch->in_room->area->area_flags, AREA_VIRTUAL ) )
	{
      	send_to_char( "Virtual area:  Not saved.\n\r", ch );
	return;
	}
      save_area( ch->in_room->area, ch, "mobs" );
      return;
    }
    if ( !strncmp( arg1, "shops",3 ) )
    {
      if ( IS_SET(ch->in_room->area->area_flags, AREA_VIRTUAL ) )
	{
      	send_to_char( "Virtual area:  Not saved.\n\r", ch );
	return;
	}
      save_area( ch->in_room->area, ch, "shops" );
      return;
    }
    if ( !strncmp( arg1, "resets",3 ) || !strncmp(arg1, "events", 3) )
    {
      if ( IS_SET(ch->in_room->area->area_flags, AREA_VIRTUAL ) )
	{
      	send_to_char( "Virtual area:  Not saved.\n\r", ch );
	return;
	}
      save_area( ch->in_room->area, ch, "events" );
      return;
    }
    if ( !strncmp( arg1, "all",3 ) )
    {
      if ( IS_SET(ch->in_room->area->area_flags, AREA_VIRTUAL ) )
	{
      	send_to_char( "Virtual area:  Not saved.\n\r", ch );
	return;
	}
      save_area( ch->in_room->area, ch, "all" );
      send_to_char( "saving zone.\n\r", ch );
      send_to_char( "Zone saved.\n\r", ch );
      REMOVE_BIT( ch->in_room->area->area_flags, AREA_CHANGED );
      return;
    }

    do_asave( ch, "" );
    return;
}



void do_arealist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[10000];
    AREA_DATA *pArea;
    int x;
    buf1[0] = '\0';

    
    if ( !str_cmp( argument, "all") || *argument == '\0')
    {
    sprintf( buf, "[%3s] [%5s] [%5s] [%10s] %15s %3s [%s]\n\r",
       "Num", "Lvnum", "Uvnum", "World", "Filename", "Sec", "Builders" );
    send_to_char( buf, ch );

	for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
	{
       
       sprintf( buf, "[%3d] [%5ld] [%5ld] [%10s] %15s %3d [%s]\n\r",
        pArea->areanum,
        pArea->lvnum,
        pArea->uvnum,
        world_name[pArea->world],
        pArea->filename,
        pArea->security,
        pArea->builders );
	strcat(buf1,buf);
       }
    page_to_char(buf1,ch);
    return;
    }
        
    sprintf( buf, "[%3s] [%5s] [%5s] [%10s] %15s %3s [%s]\n\r",
       "Num", "Lvnum", "Uvnum", "World", "Filename", "Sec", "Builders" );
    send_to_char( buf, ch );

    for ( x = 0; x < MAX_WORLD; x++ )
    if ( !strncmp( capitalize(argument), world_name[x], 4 ) )
    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
	if ( pArea->world == x )
	{
	    sprintf( buf, "[%3d] [%5ld] [%5ld] [%10s] %15s %3d [%s]\n\r",
	    pArea->areanum,
	    pArea->lvnum,
	    pArea->uvnum,
	    world_name[pArea->world],
	    pArea->filename,
	    pArea->security,
	    pArea->builders );
	    strcat(buf1,buf);
	}
    }
	
    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
	if ( strstr( pArea->builders, capitalize( argument) )  != NULL
	  || !strncmp( pArea->name, argument, 4) 
	  || !strncmp( pArea->filename, argument, 4 ) )
	{
	    sprintf( buf, "[%3d] [%5ld] [%5ld] [%10s] %15s %3d [%s]\n\r",
	    pArea->areanum,
	    pArea->lvnum,
	    pArea->uvnum,
	    world_name[pArea->world],
	    pArea->filename,
	    pArea->security,
	    pArea->builders );
	    strcat(buf1,buf);
	}
    }

    page_to_char(buf1,ch);
    return;
}
/*
 * Write to all characters.
 */
void send_to_all_char( const char *text )
{
    DESCRIPTOR_DATA *d;

    if ( !text )
        return;
    for ( d = descriptor_list; d; d = d->next )
        if ( d->connected <= CON_PLAYING )
	    send_to_char( text, d->character );

    return;
}


CHAR_DATA *get_char( CHAR_DATA *ch )
{
    if ( !ch->pcdata )
        return ch->desc->original;
    else
        return ch;
}

/* arguements x, y, z  for a grid of rooms (east-west),(north-south),(up-down) */
void do_makegrid (CHAR_DATA *ch, char *argument)
{
  ROOM_INDEX_DATA *proom;
  ROOM_INDEX_DATA *pRoomtemp = NULL;
  ROOM_INDEX_DATA *rooms[40][40][40];
  AREA_DATA *pArea;
  EXIT_DATA *pexit;
  char buf[MAX_STRING_LENGTH]; 
  char arg[MAX_INPUT_LENGTH];
  long vnum;
  int iHash;
  int x,y,z;
  int a,b,c;
  int total = 0;
  pArea = ch->in_room->area;
   argument = one_argument (argument,arg);
   if ( arg[0] == '\0' || ( x = atoi(arg) ) == 0 || x > 40 )
   {
	send_to_char("Invalid x length\n\r",ch);
	return;
   }
  
   argument = one_argument (argument,arg);
   if ( arg[0] == '\0' || ( y = atoi(arg) ) == 0 || y > 40 )
   {
	send_to_char("Invalid y length\n\r",ch);
	return;
   }
  
   argument = one_argument (argument,arg);
   if ( arg[0] == '\0' || ( z = atoi(arg) ) == 0 || z > 40 )
   {
	send_to_char("Invalid z length\n\r",ch);
	return;
   }
   argument = one_argument (argument,arg);
	if ( !strncmp(arg,"template",4))
	{
	    if ( ( pRoomtemp = get_room_index( ch->room_template ) ) == NULL )
	    {
		send_to_char( "Makegrid:  Room template not found.  Use set_room_template <room vnum>.\n\r", ch );
		return;
	    }
	}
   total = x*y*z;
   sprintf(buf,"Total # rooms %d\n\r",total);
   send_to_char(buf,ch);
    if (pArea->lvnum == 0 || pArea->uvnum == 0 )
	{
	send_to_char("The vnum's for this area have not been set.\n\r",ch);
	send_to_char("Please set them or see Wanderer or Aravan to get them set.\n\r",ch);
	send_to_char("Grid NOT created.\n\r",ch);	
	return;
	}    
    else
	{
	for ( vnum = pArea->lvnum; free_room[vnum]; vnum++ ); /*Searches the vnum list till it finds a free slot*/ 
	if (vnum + total > pArea->uvnum)
	   {
	   send_to_char("Vnum Greater then Upper vnum for area.\n\r",ch);
	   send_to_char("Grid NOT created.\n\r",ch);
   	   sprintf(buf,"Total # rooms availiable in area %ld\n\r",pArea->uvnum - vnum);
   	   send_to_char(buf,ch);
	   return;	
	   }
	}
   for ( a = 0 ; a < x ; a++ )
 	for ( b = 0 ; b < y ; b++ )
   	    for ( c = 0 ; c < z ; c++ )
		{

		   proom 		= pRoomIndex_alloc();
		   proom->people	= NULL;
		   proom->contents	= NULL;	
		   proom->snoop_by	= NULL; 
		   proom->extra_descr	= NULL;	
		   proom->area		= ch->in_room->area;
		   if ( pRoomtemp )
		   {
		   strncpy( proom->name, pRoomtemp->name, 80 );
	    	   proom->description = str_dup(pRoomtemp->description);
	    	   proom->room_flags = pRoomtemp->room_flags;
	    	   proom->room_flags_2 = pRoomtemp->room_flags_2;
	    	   proom->guilds = pRoomtemp->guilds;
	    	   proom->light_adjustment = pRoomtemp->light_adjustment;
	    	   proom->light = pRoomtemp->light;
	  	   proom->size = pRoomtemp->size;
	    	   proom->sector_type = pRoomtemp->sector_type;
	
		   }
		   else
		   {
		   strncpy( proom->name, "A formless void", 80 );
		   proom->description	= str_dup ("Waves of chaos congeal, twist, bend to form in the begining of \n\r some area. This most definitly is the work of a God's hand.\n\r");
		   proom->room_flags 	= 0;
		   proom->sector_type 	= 0;
		   proom->light		= 0;
		   }

			for ( vnum = pArea->lvnum; free_room[vnum]; vnum++ ); /*Searches the vnum list till it finds a free slot*/ 
			proom->vnum = vnum; 
			free_room[vnum] = 1;
    
		   iHash		= vnum % MAX_KEY_HASH;
		   proom->next		= room_index_hash[iHash];
		   room_index_hash[iHash]	= proom;
   		   if (LOGON_ON)
   		   if (ch->logon_data != NULL)
		   ch->logon_data->rooms++;

		   rooms[a][b][c] = proom;
		}

  /* Set all exits */
  for ( a = 0 ; a < x ; a++ )
  {
  	for ( b = 0 ; b < y ; b++ )
	{
  	    for ( c = 0 ; c < z ; c++ )
		{

		    if ( a > 0 )
		    {

		     	pexit = new_exit();
		        pexit->description[0] = '\0';
		        pexit->keyword[0] = '\0';
		        pexit->exit_info = 1;
		        pexit->exit_info_default = 1;
			pexit->tracks = NULL;
			pexit->trap = NULL;
			pexit->mes_to_char[0] = '\0';
			pexit->mes_to_room[0] = '\0';
			pexit->mes_at_room[0] = '\0';
			pexit->lock = NULL;
			pexit->u1.vnum = rooms[a-1][b][c]->vnum;
			pexit->u1.to_room = rooms[a-1][b][c];
                        rooms[a][b][c]->exit[ DIR_EAST ] = pexit;

			/* do other side */
		     	pexit = new_exit();
		        pexit->description[0] = '\0';
		        pexit->keyword[0] = '\0';
		        pexit->exit_info = 1;
		        pexit->exit_info_default = 1;
			pexit->tracks = NULL;
			pexit->trap = NULL;
			pexit->mes_to_char[0] = '\0';
			pexit->mes_to_room[0] = '\0';
			pexit->mes_at_room[0] = '\0';
			pexit->lock = NULL;
			pexit->u1.vnum = rooms[a][b][c]->vnum;
			pexit->u1.to_room = rooms[a][b][c];
                        rooms[a-1][b][c]->exit[ DIR_WEST ] = pexit;

		    }
		    else
		    {
			rooms[a][b][c]->exit[ DIR_EAST ] = NULL;
		    }

		    if ( b > 0 )
		    {

		     	pexit = new_exit();
		        pexit->description[0] = '\0';
		        pexit->keyword[0] = '\0';
		        pexit->exit_info = 1;
		        pexit->exit_info_default = 1;
			pexit->tracks = NULL;
			pexit->trap = NULL;
			pexit->mes_to_char[0] = '\0';
			pexit->mes_to_room[0] = '\0';
			pexit->mes_at_room[0] = '\0';
			pexit->lock = NULL;
			pexit->u1.vnum = rooms[a][b-1][c]->vnum;
			pexit->u1.to_room = rooms[a][b-1][c];
                        rooms[a][b][c]->exit[ DIR_NORTH ] = pexit;

			/* do other side */
		     	pexit = new_exit();
		        pexit->description[0] = '\0';
		        pexit->keyword[0] = '\0';
		        pexit->exit_info = 1;
		        pexit->exit_info_default = 1;
			pexit->tracks = NULL;
			pexit->trap = NULL;
			pexit->mes_to_char[0] = '\0';
			pexit->mes_to_room[0] = '\0';
			pexit->mes_at_room[0] = '\0';
			pexit->lock = NULL;
			pexit->u1.vnum = rooms[a][b][c]->vnum;
			pexit->u1.to_room = rooms[a][b][c];
                        rooms[a][b-1][c]->exit[ DIR_SOUTH ] = pexit;
		    }
		    else
		    {
			rooms[a][b][c]->exit[ DIR_NORTH ] = NULL;
		    }

		    if ( c > 0 )
		    {

		     	pexit = new_exit();
		        pexit->description[0] = '\0';
		        pexit->keyword[0] = '\0';
		        pexit->exit_info = 1;
		        pexit->exit_info_default = 1;
			pexit->tracks = NULL;
			pexit->trap = NULL;
			pexit->mes_to_char[0] = '\0';
			pexit->mes_to_room[0] = '\0';
			pexit->mes_at_room[0] = '\0';
			pexit->lock = NULL;
			pexit->u1.vnum = rooms[a][b][c-1]->vnum;
			pexit->u1.to_room = rooms[a][b][c-1];
                        rooms[a][b][c]->exit[ DIR_UP ] = pexit;

			/* do other side */
		     	pexit = new_exit();
		        pexit->description[0] = '\0';
		        pexit->keyword[0] = '\0';
		        pexit->exit_info = 1;
		        pexit->exit_info_default = 1;
			pexit->tracks = NULL;
			pexit->trap = NULL;
			pexit->mes_to_char[0] = '\0';
			pexit->mes_to_room[0] = '\0';
			pexit->mes_at_room[0] = '\0';
			pexit->lock = NULL;
			pexit->u1.vnum = rooms[a][b][c]->vnum;
			pexit->u1.to_room = rooms[a][b][c];
                        rooms[a][b][c-1]->exit[ DIR_DOWN ] = pexit;
		    }
		    else
		    {
			rooms[a][b][c]->exit[ DIR_UP ] = NULL;
		    }

		    if ( c == z - 1 )
	                rooms[a][b][c]->exit[ DIR_DOWN ] = NULL;	
	            if ( b == y - 1 )
    	                rooms[a][b][c]->exit[ DIR_SOUTH ] = NULL;	
 	            if ( a == x - 1 )
        	        rooms[a][b][c]->exit[ DIR_WEST ] = NULL;	
		}
	 }
    }

    send_to_char("Done.\n\r",ch);
    sprintf (buf,"Grid starting position is [%ld].\n\r",rooms[0][0][0]->vnum);
    send_to_char (buf,ch);
     SET_BIT(ch->in_room->area->area_flags, AREA_CHANGED );
}
void do_security (CHAR_DATA *ch, char *argument)
{
  char arg1[MAX_INPUT_LENGTH];
  char arg2[MAX_INPUT_LENGTH];
  int security;
  CHAR_DATA *victim;

  argument = one_argument(argument,arg1);
  argument = one_argument(argument,arg2);

  if ((arg1[0]=='\0') || (arg2[0]=='\0'))
    { send_to_char ("Syntax: security character <security_number>\n\r",ch);
      return;
    }

  if ( ( victim = get_char_world (ch,arg1) ) == NULL )
     { send_to_char (" They are not logged in at this time.\n\r",ch);
       return; }
  security = atoi (arg2);
  if (security < 1  || security > 9)
  if (security != -11)
    { send_to_char ("Invalid Security number, Security must be between 1 and 9\n\r",ch);
      return;
    }
  if (security == -11)
	send_to_char("Optivision security activated\n\r",ch);
    victim->pcdata->security = security;
  if ( victim->pcdata->security != -11 )
  {
  sprintf (arg2,"[%s] has been given security [%d]\n\r",arg1,security);
  info_channel ( victim, arg2,INFOACT_PERMISSION);
  sprintf (arg2," You have been given security [%d]\n\r",security);
  send_to_char (arg2,victim);
   }
  return;
}
