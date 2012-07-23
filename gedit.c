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

void gedit( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObjIndex;
    bool display = FALSE;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    arg2[0] = '\0';
    strcpy( arg, argument );
    strcpy( arg3, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );


    if ( !str_cmp( arg1, "show" ) || arg1[0] == '\0' )
    {
        do_gshow( ch,"" );
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
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, GEDIT_GLOBAL, SECURITY_9))
    {
		ch->edit_menu = GEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, GEDIT_WORLD, SECURITY_9))
    {
		ch->edit_menu = GEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, GEDIT_AREA, SECURITY_9))
    {
		ch->edit_menu = GEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }

    if ( !str_cmp( arg1, "?" ) )
    if (edit_function(ch, GEDIT_ANY, SECURITY_9))
    {
	sprintf(buf,"`C                      Command Gedit Menu\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`B?`w            Show current menu, status and available commands\n\r");
	send_to_char(buf,ch);
	if (ch->edit_menu > 2 )
	{
	sprintf(buf,"`BBack ( - )`w   Back one menu\n\r");
	send_to_char(buf,ch);
	}
	display=TRUE;
    }

    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "1", 1) )
    if (edit_function(ch, GEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B1           `w Global \n\r",ch);
	}
	else
	{
	
	ch->edit_menu = GEDIT_GLOBAL ;
	strncpy( ch->edit_buffer, "Global->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "2", 1) )
    if (edit_function(ch, GEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B2           `w World\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = GEDIT_WORLD ;
	strncpy( ch->edit_buffer, "World->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "3", 1) )
    if (edit_function(ch, GEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B3           `w Area\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = GEDIT_AREA ;
	strncpy( ch->edit_buffer, "Area->", 80 );
	return;
	}
    }
    if ( !strncmp( arg1, "add", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, GEDIT_ANY, SECURITY_9))
    {
	GLOBAL_RANDOM_DATA   *grandom;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    if ( ch->edit_menu > 2 )
	    {
	    	send_to_char("`BAdd          `w Create new random\n\r",ch);
	    	send_to_char("`w                  Syntax: add <object vnum>\n\r",ch);
	    }
	}
	else
	{
	    if ( arg2[0] != '\0' && is_number(arg2) )
	    {
	    	if ( ( pObjIndex = get_obj_index( atoi(arg2) ) ) == NULL )
	    	{
		    send_to_char("Invalid obj index\n\r",ch);
		    return;
	    	}
		
	    	grandom = random_data_alloc();
	    	grandom->next = random_data_list;
	    	random_data_list = grandom;
	    	if ( ch->edit_menu == GEDIT_GLOBAL )
	    	    grandom->global = 1;
	    	if ( ch->edit_menu == GEDIT_WORLD )
	    	    grandom->world = ch->in_room->area->world;
	    	if ( ch->edit_menu == GEDIT_AREA )
	    	    grandom->area = ch->in_room->area->vnum;
	    	grandom->chance = 1000;
	    	grandom->vnum = atoi(arg2);
	    	sprintf(buf,"New random for vnum %ld created\n\r",pObjIndex->vnum);
	    	send_to_char(buf,ch);
	    	SET_BIT(changed_system, CHANGED_RANDOM);
	    	return;
	    }

	    send_to_char("`w                  Syntax: add <object vnum>\n\r",ch);
	    return;

	}
    }
    if ( !strncmp( arg1, "chance", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, GEDIT_ANY, SECURITY_9))
    {
	GLOBAL_RANDOM_DATA   *grandom;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    if ( ch->edit_menu > 2 )
	    {
	    	send_to_char("`BChance       `w Set the chance of loading: 1 in <value>\n\r",ch);
	    	send_to_char("`w                  Syntax: chance <object vnum> <value>\n\r",ch);
	    }
	}
	else
	{
    	    argument = one_argument( argument, arg1 );
    	    one_argument( argument, arg2 );
	    if ( arg1[0] == '\0' || arg2[0] == '\0' )
	    {
	    send_to_char("`w                  Syntax: chance <object vnum> <value>\n\r",ch);
	    return;
	    }
	    for ( grandom = random_data_list; grandom != NULL; grandom = grandom->next )	
	    {
		if ( atoi(arg1) == grandom->vnum )
		{
		grandom->chance = atoi(arg2);
	        sprintf(buf,"New random chance for vnum %ld set\n\r",grandom->vnum);
	        send_to_char(buf,ch);
	        SET_BIT(changed_system, CHANGED_RANDOM);
		return;
		}
	    }
	        send_to_char("Not found\n\r",ch);
		return;
	    
	}
    }
    if ( !strncmp( arg1, "max", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, GEDIT_ANY, SECURITY_9))
    {
	GLOBAL_RANDOM_DATA   *grandom;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    if ( ch->edit_menu > 2 )
	    {
	    	send_to_char("`BMax          `w Set the max to load\n\r",ch);
	    	send_to_char("`w                  Syntax: max <object vnum> <value>\n\r",ch);
	    }
	}
	else
	{
    	    argument = one_argument( argument, arg1 );
    	    one_argument( argument, arg2 );
	    if ( arg1[0] == '\0' || arg2[0] == '\0' )
	    {
	    send_to_char("`w                  Syntax: max <object vnum> <value>\n\r",ch);
	    return;
	    }
	    for ( grandom = random_data_list; grandom != NULL; grandom = grandom->next )	
	    {
		if ( atoi(arg1) == grandom->vnum )
		{
		grandom->max = atoi(arg2);
	        sprintf(buf,"New random max for vnum %ld set\n\r",grandom->vnum);
	        send_to_char(buf,ch);
	        SET_BIT(changed_system, CHANGED_RANDOM);
		return;
		}
	    }
	        send_to_char("Not found\n\r",ch);
		return;
	    
	}
    }
    if ( !strncmp( arg1, "list", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, GEDIT_ANY, SECURITY_9))
    {
	GLOBAL_RANDOM_DATA   *grandom;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    if ( ch->edit_menu > 2 )
	    {
	    	send_to_char("`BList         `w List current randomly loaded objects\n\r",ch);
	    	send_to_char("`w                  Syntax: list\n\r",ch);
	    }
	}
	else
	{
	    for ( grandom = random_data_list; grandom != NULL; grandom = grandom->next )	
	    {
		if ( ( pObjIndex = get_obj_index(grandom->vnum) ) != NULL )
		{
		    if ( ch->edit_menu == GEDIT_GLOBAL && grandom->global > 0 )
		    {
		        sprintf(buf,"%-5ld  Chance 1 in %-4d Max %-4d (%-3d)   %-30s\n\r",grandom->vnum, grandom->chance, grandom->max, grandom->count, pObjIndex->short_descr);
		        send_to_char(buf,ch);
		    }
		    if ( ch->edit_menu == GEDIT_WORLD && grandom->world == ch->in_room->area->world )
		    {
		        sprintf(buf,"%-5ld  Chance 1 in %-4d Max %-4d (%-3d)   %-30s\n\r",grandom->vnum, grandom->chance, grandom->max, grandom->count, pObjIndex->short_descr);
		        send_to_char(buf,ch);
		    }
		    if ( ch->edit_menu == GEDIT_AREA && grandom->area == ch->in_room->area->vnum ) 
		    {
		        sprintf(buf,"%-5ld  Chance 1 in %-4d Max %-4d (%-3d)   %-30s\n\r",grandom->vnum, grandom->chance, grandom->max, grandom->count, pObjIndex->short_descr);
		        send_to_char(buf,ch);
		    }
		}
	    }
	    return;
	}
    }
    if ( !strncmp( arg1, "remove", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, GEDIT_ANY, SECURITY_9))
    {
	GLOBAL_RANDOM_DATA   *grandom;
	GLOBAL_RANDOM_DATA   *random_data;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    if ( ch->edit_menu > 2 )
	    {
	    	send_to_char("`BRemove       `w remove a random\n\r",ch);
	    	send_to_char("`w                  Syntax: remove <object vnum>\n\r",ch);
	    }
	}
	else
	{
	if ( arg2[0] != '\0' && is_number(arg2) )
	{
	    random_data = NULL;

	    for ( grandom = random_data_list; grandom != NULL ; grandom = grandom->next )
	    {
		if ( grandom->vnum == atoi( arg2 ) )
		    break;
		random_data = grandom;
	    }
	    if ( grandom == NULL )
	    {
		send_to_char("Not found!\n\r",ch);	
		return;
	    }
	    if ( random_data == NULL )
	    {
		random_data_list = grandom->next;
	    }
	    else
	    {
		random_data->next = grandom->next;
	    }

	    free_random_data( grandom );

	    SET_BIT(changed_system, CHANGED_RANDOM);
	    send_to_char( "Random removed.\n\r", ch );
	    return;
	}
	    send_to_char("`w                  Syntax: remove <object vnum>\n\r",ch);
	    return;
	}
    }
    
    if (!display)
    interpret( ch, arg );
    return;
}
void do_gedit( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) ) return;

    ch->desc->connected = CON_GEDITOR;
    ch->edit_menu	= GEDIT_MENU;
    ch->edit_buffer[0] = '\0';
    return;
}
void do_gshow( CHAR_DATA *ch, char *argument )
{
    return;
}
