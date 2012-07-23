/*************************************************************************
Galadrail's tedit, with special thanks to Locke.
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
extern long	top_trigger_data;
extern long	top_script_data;
extern long	top_trigger_index;
void tedit( CHAR_DATA *ch, char *argument )
{
    TRIGGER_INDEX_DATA *pTrigIndex;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int iHash, x = 0;
    int value;
    long vnum;
    SCRIPT_DATA *scr;
    if (IS_NPC(ch) ) return;
    strcpy( arg, argument );
    strcpy( arg3, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    pTrigIndex = (TRIGGER_INDEX_DATA *)ch->desc->pEdit; 

    if ( !str_cmp( arg1, "show" ) || arg1[0] == '\0' )
    {
        sprintf( buf, "%d", pTrigIndex->vnum );
        do_tshow( ch, buf );
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
        do_help( ch, "tedit" );
        return;
    }

    if (!strncmp(arg1,"create",3)) 
    {
    one_argument( argument, arg1 );
	if (arg1[0] == '\0')
	{ 
	 send_to_char ("Syntax: create trigger\n\r",ch); 
	 return;
	}

	pTrigIndex		= pTrigIndex_alloc();
	for ( vnum = 1 ; free_trig_list[vnum]; vnum++ );

        pTrigIndex->vnum 	= vnum;
	free_trig_list[vnum] 	= 1;
	
        iHash			= pTrigIndex->vnum % MAX_KEY_HASH;
	pTrigIndex->next	= trig_index_hash[iHash];
	trig_index_hash[iHash]	= pTrigIndex;
	strncpy( pTrigIndex->builders, ch->name, 30 );	
	strncpy( pTrigIndex->name, "NONE", 30 );	
	send_to_char("Trigger Created\n\r",ch);
	sprintf(buf,"%d",pTrigIndex->vnum);
	do_tedit(ch,buf);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"key_sim",7)) 
    {

	if (argument[0] == '\0')
	{ 
	 send_to_char ("New key_sim?\n\r",ch); 
	 return;
	}
        if (!str_cmp(argument,"none") || !str_cmp(argument, "null" ) ) 
	{
	    pTrigIndex->key_sim[0] = '\0';
	SET_BIT(changed_system, CHANGED_TRIGGERS);
 	return;	
	}
	else
	{
	strncpy( pTrigIndex->key_sim, argument, 30 );	
	send_to_char("New key_sim set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
	}
    }
    if ( !strncmp( arg1, "key_words" , 5) )
    {
        argument = one_argument( argument, arg2 );

        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  key_words [$name]\n\r", ch );
            return;
        }

        if ( strstr( pTrigIndex->key_words, arg2 ) != '\0' )
        {
            strncpy( pTrigIndex->key_words, string_replace( pTrigIndex->key_words, arg2, "\0" ), 30 );
            send_to_char( "Key_word removed.\n\r", ch );
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
            return;
        }
        else
        {
            buf[0] = '\0';
            if ( strstr( pTrigIndex->key_words, "None" ) != '\0' )
            {
                strncpy( pTrigIndex->key_words, string_replace( pTrigIndex->key_words, "None", "\0" ), 30 );
            }
            if (pTrigIndex->key_words[0] != '\0' )
            {
                strcat( buf, pTrigIndex->key_words );
                strcat( buf, " " );
            }
            strcat( buf, arg2 );
            strncpy( pTrigIndex->key_words, buf, 30 ) ;

            send_to_char( "Key_word added.\n\r", ch );
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
            return;
        }
    }
    if (!strncmp(arg1,"key_string",7)) 
    {

	if (argument[0] == '\0')
	{ 
	 send_to_char ("New key_string?\n\r",ch); 
	 return;
	}
        if (!str_cmp(argument,"none") || !str_cmp(argument, "null" ) ) 
	{
	    pTrigIndex->key_string[0] = '\0';	
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
 	    return;	
	}
	strncpy( pTrigIndex->key_string, argument, 30 );	
	send_to_char("New key_string set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;

    }
    if (!strncmp(arg1,"player",3)) 
    {
	TOGGLE_BIT(pTrigIndex->flags, TRIG_PC);
	send_to_char("Toggled\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"active",3)) 
    {
	TOGGLE_BIT(pTrigIndex->flags, TRIG_INACTIVE);
	send_to_char("Toggled\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"instant",3)) 
    {
	TOGGLE_BIT(pTrigIndex->flags, TRIG_INSTANT);
	send_to_char("Toggled\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"reset",3)) 
    {
	TOGGLE_BIT(pTrigIndex->flags, SCRIPT_HALT_RESET);
	send_to_char("Toggled\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"inactive",3)) 
    {
	TOGGLE_BIT(pTrigIndex->flags, TRIG_INACTIVE);
	send_to_char("Toggled\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"mobile",4)) 
    {
	TOGGLE_BIT(pTrigIndex->flags, TRIG_MOB);
	send_to_char("Toggled\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if ( !strncmp( arg1, "builders", 3 ) )
    {
        argument = one_argument( argument, arg2 );

        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  builder [$name]\n\r", ch );
            return;
        }

        arg2[0] = UPPER( arg2[0] );

        if ( strstr( pTrigIndex->builders, arg2 ) != '\0' )
        {
            strncpy( pTrigIndex->builders, string_replace( pTrigIndex->builders, arg2, "\0" ), 30 );
            send_to_char( "Builder removed.\n\r", ch );
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
            return;
        }
        else
        {
            buf[0] = '\0';
            if ( strstr( pTrigIndex->builders, "None" ) != '\0' )
                strncpy( pTrigIndex->builders, string_replace( pTrigIndex->builders, "None", "\0" ), 30 );

            if (pTrigIndex->builders[0] != '\0' )
            {
                strcat( buf, pTrigIndex->builders );
                strcat( buf, " " );
            }
            strcat( buf, arg2 );
            strncpy( pTrigIndex->builders, string_proper( str_dup( buf ) ), 30 );

            send_to_char( "Builder added.\n\r", ch );
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
            return;
        }
    }

    if (!strncmp(arg1,"name",2)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0') 
	{
	 send_to_char ("Syntax: Name <name>\n\r",ch); 
	 return;
	}
	strncpy( pTrigIndex->name, arg2, 30 );	
	send_to_char("New name set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"type",2)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0' || ( value = get_trig_type( arg2 ) ) == -1 ) 
	{
	 send_to_char ("Syntax: Type <type>\n\r",ch); 
	 strcpy( buf, "Types: " );
	 for ( x = 0 ; x < MAX_TRIG_TYPE ; x++ )
	 {
	    strcat( buf, " " );
	    strcat( buf, trig_type_name( x ) );
	 }
	 send_to_char( format_string( str_dup (buf) ), ch );
	 return;
	}
	pTrigIndex->trigger_type = value;	
	send_to_char("New type set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"quest_step_pre",12)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0' )
	{
	 send_to_char ("Syntax: Quest_step_pre <step>\n\r",ch); 
	 return;
	}
	if (!str_cmp(arg2, "none" ) )
	{
	    pTrigIndex->step_pre = 0;	
	    send_to_char("Quest pre step set to none.\n\r",ch);
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
	    return;
	}
	if (  (value = get_quest_flags( arg2 ) ) == -1 ) 
	{
	 send_to_char ("Syntax: Quest_step_pre <step>\n\r",ch); 
	 return;
	}
	pTrigIndex->step_pre = value;	
	send_to_char("New quest pre step set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"quest_step",7)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0' )
	{
	 send_to_char ("Syntax: Quest_step <step>\n\r",ch); 
	 return;
	}
	if (!str_cmp(arg2, "none" ) )
	{
	    pTrigIndex->step_pre = 0;	
	    send_to_char("Quest pre step set to none.\n\r",ch);
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
	    return;
	}

	if  (  (value = get_quest_flags( arg2 ) ) == -1 ) 
	{
	 send_to_char ("Syntax: Quest_step <step>\n\r",ch); 
	 return;
	}

	pTrigIndex->step = value;	
	send_to_char("New quest step set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"quest_pre",7)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0' )
	{
	 send_to_char ("Syntax: Quests_pre <quest number>\n\r",ch); 
	 return;
	}
        x = atoi( arg2 );
	if ( x > 200 || x < 0 )
	{
	 send_to_char ("Invalid Quest number\n\r",ch); 
	 return;
	}
	pTrigIndex->quests_pre = x;	
	send_to_char("New quest prerequisite set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!str_cmp(arg1,"quest")) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0' )
	{
	 send_to_char ("Syntax: Quests <quest number>\n\r",ch); 
	 return;
	}
        x = atoi( arg2 );
	if ( x > 200 || x < 1 )
	{
	 send_to_char ("Invalid Quest number\n\r",ch); 
	 return;
	}
	pTrigIndex->quests = x;	
	send_to_char("New quest set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"security",3)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0') 
	{
	 send_to_char ("Syntax: Security <security>\n\r",ch); 
	 return;
	}
        x = atoi( arg2 );
	if ( x > 9 || x < 1 )
	{
	 send_to_char ("Invalid Security\n\r",ch); 
	 return;
	}
	pTrigIndex->security = x;	
	send_to_char("New security set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"timer",3)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0') 
	{
	 send_to_char ("Syntax: Timer <number>\n\r",ch); 
	 return;
	}
        x = atoi( arg2 );
	if ( x > 32000 || x < 0 )
	{
	 send_to_char ("Invalid Timer\n\r",ch); 
	 return;
	}
	pTrigIndex->timer = x;	
	send_to_char("New timer set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"gets_vnum",4)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0') 
	{
	 send_to_char ("Syntax: gets_vnum <obj vnum>\n\r",ch); 
	 return;
	}
        x = atoi( arg2 );
	if ( x > MAX_VNUM || x < 0 )
	{
	 send_to_char ("Invalid vnum\n\r",ch); 
	 return;
	}
	pTrigIndex->gets_vnum = x;	
	send_to_char("New gets_vnum set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"mob_vnum",4)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0') 
	{
	 send_to_char ("Syntax: mob_vnum <mob vnum>\n\r",ch); 
	 return;
	}
        x = atoi( arg2 );
	if ( x > MAX_VNUM || x < 0 )
	{
	 send_to_char ("Invalid vnum\n\r",ch); 
	 return;
	}
	pTrigIndex->mob_vnum = x;	
	send_to_char("New mob_vnum set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }
    if (!strncmp(arg1,"chance",3)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0') 
	{
	 send_to_char ("Syntax: chance < % chance of loading>\n\r",ch); 
	 return;
	}
        x = atoi( arg2 );
	if ( x > 100 || x < 1 )
	{
	 send_to_char ("Invalid chance\n\r",ch); 
	 return;
	}
	pTrigIndex->chance = x;	
	send_to_char("New chance set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"stype",2)) 
    {
     one_argument( argument, arg2 );

	if (arg2[0] == '\0') 
	{
	 send_to_char ("Syntax: Stype <stype>\n\r",ch); 
	 return;
	}
        if (!strncmp(arg2,"mobile",3)) 
	pTrigIndex->script_type = TRIG_TYPE_MOB;
	else
        if (!strncmp(arg2,"object",3)) 
	pTrigIndex->script_type = TRIG_TYPE_OBJ;
	else
        if (!strncmp(arg2,"room",3)) 
	pTrigIndex->script_type = TRIG_TYPE_ROOM;
	else
	pTrigIndex->script_type = atoi(arg2);	
	send_to_char("New script type set.\n\r",ch);
	SET_BIT(changed_system, CHANGED_TRIGGERS);
	return;
    }

    if (!strncmp(arg1,"command",2)) 
    {
  
	argument = one_argument( argument, arg2 );

	if ( arg2[0] != '\0' )
	{
	    for ( scr = pTrigIndex->script; scr != NULL; scr = scr->next ) 
	    {
	    	x++;
		if ( x == atoi(arg2))
		    break;
	    }
	    if ( scr == NULL ) 
	    {
		if ( pTrigIndex->script != NULL ) 
		{
		    for (scr = pTrigIndex->script; scr->next != NULL; scr = scr->next)
			;
		    scr->next = (SCRIPT_DATA *) alloc_perm(sizeof(*scr) );
		    top_script_data++;
		    scr = scr->next;
		}
		else 
		{
		    pTrigIndex->script = (SCRIPT_DATA *) alloc_perm(sizeof(*scr) );
		    top_script_data++;
		    scr = pTrigIndex->script;
		}
	        scr->command = &str_empty[0];
	        scr->next = NULL;
	        send_to_char( "Creating new command\n\r", ch );
	    }	
	    string_append(ch, &scr->command); 
	    SET_BIT(changed_system, CHANGED_TRIGGERS);
	    return;
	}  
	else
	{
	    for ( scr = pTrigIndex->script; scr != NULL; scr = scr->next )
	    {
		x++;
		sprintf( buf, "##\n\r%d)-> %s\n\r",x,       scr->command );
		send_to_char( buf, ch );
	    }
	    return;
	} 
    }
    interpret( ch, arg );
    return;
}



void do_tedit( CHAR_DATA *ch, char *argument )
{
    TRIGGER_INDEX_DATA *pTrigIndex;
    int value, iHash, vnum;
    char arg1[MAX_STRING_LENGTH];

    argument = one_argument( argument, arg1 );

    if ( is_number( arg1 ) ) 
    {
    value = atoi( arg1 );

    	if ( ( pTrigIndex = get_trig_index( value ) ) == NULL ) 
	{
            send_to_char( "TEdit:  That vnum does not exist.\n\r", ch );
            return;
        }
        if ( !IS_BUILDER_TRIG( ch, pTrigIndex ) ) 
	{
        send_to_char( "TEdit:  Sorry, your not the builder of this trigger\n\r", ch );
        return;
        }

        ch->desc->pEdit = (void *)pTrigIndex;
        ch->desc->connected = CON_TEDITOR;
        return;
    }
    else
    {
	if (!strncmp(arg1,"create",3)) 
	{
	pTrigIndex = pTrigIndex_alloc();
	for ( vnum = 1 ; free_trig_list[vnum]; vnum++ );

        pTrigIndex->vnum 	= vnum; 
	free_trig_list[vnum] 	= 1;
        iHash			= pTrigIndex->vnum % MAX_KEY_HASH;
	pTrigIndex->next	= trig_index_hash[iHash];
	trig_index_hash[iHash]	= pTrigIndex;
	strncpy( pTrigIndex->builders, ch->name, 30 );
	
        ch->desc->pEdit = (void *)pTrigIndex;
        ch->desc->connected = CON_TEDITOR;
        return;
	}	
    }

    send_to_char( "TEdit: There is no default trigger to edit.\n\r", ch );
    send_to_char( "TEdit: Tedit <vnum>\n\r", ch );
    send_to_char( "TEdit: Tedit create\n\r", ch );
    return;
}


void do_tshow( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    TRIGGER_INDEX_DATA *pTrigIndex;
    SCRIPT_DATA *scr;
    int x = 0;

    buf[0] = '\0';
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
    send_to_char( "Tshow what trigger?\n\r", ch );
        return;
    }

    if ( ( pTrigIndex = get_trig_index( atoi( arg ) ) ) == NULL )
    {
    send_to_char( "Invalid trigger index VNUM.\n\r", ch );
        return;
    }


    sprintf( buf, "Name: %s\n\r", pTrigIndex->name );
    send_to_char( buf, ch );
    sprintf( buf, "Vnum: %d\n\r", pTrigIndex->vnum );
    send_to_char( buf, ch );
    sprintf( buf, "Builders: %s\n\r", pTrigIndex->builders );
    send_to_char( buf, ch );
    sprintf( buf, "Key_words: %s\n\r", pTrigIndex->key_words );
    send_to_char( buf, ch );
    sprintf( buf, "Key_string: %s\n\r", pTrigIndex->key_string );
    send_to_char( buf, ch );
    sprintf( buf, "Key_sim: %s\n\r", pTrigIndex->key_sim );
    send_to_char( buf, ch );
    sprintf( buf, "Type: %s\n\r", trig_type_name( pTrigIndex->trigger_type ) );
    send_to_char( buf, ch );
    sprintf( buf, "Quest: %d\n\r", pTrigIndex->quests );
    send_to_char( buf, ch );
    sprintf(buf, "Quest_step  %s\n\r", get_quest_name(pTrigIndex->step) );
    send_to_char( buf, ch );
    sprintf(buf, "Quest_step_pre  %s\n\r", get_quest_name(pTrigIndex->step_pre) );
    send_to_char( buf, ch );
    sprintf( buf, "Quest_Prerequisite: %d\n\r", pTrigIndex->quests_pre );
    send_to_char( buf, ch );
    sprintf( buf, "Stype: %s\n\r", pTrigIndex->script_type == TRIG_TYPE_MOB ? "Mobile" : pTrigIndex->script_type == TRIG_TYPE_OBJ ? "Object" : pTrigIndex->script_type == TRIG_TYPE_ROOM ? "Room" : "None" );
    send_to_char( buf, ch );
    sprintf( buf, "Chance: %d\n\r", pTrigIndex->chance );
    send_to_char( buf, ch );
    sprintf( buf, "Gets_vnum: %ld\n\r", pTrigIndex->gets_vnum );
    send_to_char( buf, ch );
    sprintf( buf, "Mob_vnum: %ld\n\r", pTrigIndex->mob_vnum );
    send_to_char( buf, ch );
    sprintf( buf, "Timer: %d\n\r", pTrigIndex->timer );
    send_to_char( buf, ch );
    sprintf( buf, "Trigger acts on Players %s : Mobiles %s : Active %s Reset %s Instant %s\n\r",
	IS_SET(pTrigIndex->flags, TRIG_PC) ? "YES" : "NO ", 
	IS_SET(pTrigIndex->flags, TRIG_MOB) ? "YES" : "NO ", 
	IS_SET(pTrigIndex->flags, TRIG_INACTIVE) ? "NO" : "YES ",
	IS_SET(pTrigIndex->flags, SCRIPT_HALT_RESET) ? "YES" : "NO ",
	IS_SET(pTrigIndex->flags, TRIG_INSTANT	) ? "YES" : "NO ");
    send_to_char( buf, ch);

    for ( scr = pTrigIndex->script; scr != NULL; scr = scr->next )
    {
	x++;
	sprintf( buf, "##\n\r%d)-> %s\n\r",x,       scr->command );
	send_to_char( buf, ch );
    }

return;

}

void do_trigger_list( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    TRIGGER_INDEX_DATA *pTrigIndex;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int vnum;
    output = new_buf();
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    sprintf( buf, "[%4s] [%15s] [%3s] [%10s] [%12s] [%4s] [%s]\n\r",
       "Vnum", "Name", "Sec", "Type", "Keywords", "Stype" ,"Builders");
    send_to_char( buf, ch );
    

    for ( vnum = 0; vnum < MAX_KEY_HASH ; vnum++ )
    {
	for ( pTrigIndex = trig_index_hash[vnum]; pTrigIndex != NULL ; pTrigIndex = pTrigIndex->next )
	{
	    if ( *arg2 != '\0' 
		&& is_number(arg2) 
	        && atoi(arg2) == pTrigIndex->quests )
	    {
		sprintf( buf, "[%4d] [%15s] [%3d] [%10s] [%12s] [%4d] [%s]\n\r",
		    pTrigIndex->vnum,
		    pTrigIndex->name,
		    pTrigIndex->security,
		    trig_type_name( pTrigIndex->trigger_type ),
		    strlen( pTrigIndex->key_sim ) > 2 ? pTrigIndex->key_sim : 
		    strlen( pTrigIndex->key_words ) > 2 ? pTrigIndex->key_words :
		    strlen( pTrigIndex->key_string ) > 2 ? pTrigIndex->key_string : " " ,
		    pTrigIndex->script_type,
		    pTrigIndex->builders );
		add_buf(output, buf);	
	    }
	    else if ( *arg1 == '\0'
		|| strstr( pTrigIndex->builders, capitalize(arg1) )  != NULL
		|| !strncmp( pTrigIndex->name, arg1, 4) 
		|| !strncmp( trig_type_name( pTrigIndex->trigger_type ), arg1, 4 ) 
		|| !strncmp( pTrigIndex->key_sim, arg1, 4)
		|| !strncmp( pTrigIndex->key_words, arg1, 4)
		|| !strncmp( pTrigIndex->key_string, arg1, 4) )
	    {
		sprintf( buf, "[%4d] [%15s] [%3d] [%10s] [%12s] [%4d] [%s]\n\r",
		    pTrigIndex->vnum,
		    pTrigIndex->name,
		    pTrigIndex->security,
		    trig_type_name( pTrigIndex->trigger_type ),
		    strlen( pTrigIndex->key_sim ) > 2 ? pTrigIndex->key_sim : 
		    strlen( pTrigIndex->key_words ) > 2 ? pTrigIndex->key_words :
		    strlen( pTrigIndex->key_string ) > 2 ? pTrigIndex->key_string : " " ,
		    pTrigIndex->script_type,
		    pTrigIndex->builders );
		add_buf(output, buf);	
	    }
	}
    } 
    page_to_char(buf_string(output),ch);
    free_buf(output);
    return;
}
