/* New commands for objects by Quin, to be used with triggers by Galadrail */
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

typedef	struct	obj_cmd_type		OBJ_CMD_TYPE;

struct obj_cmd_type
{
    char * const	name;
    OBJ_FUN *		obj_fun;
};

void do_object( CHAR_DATA * ch, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA * obj;

    argument = one_argument( argument, arg );

    if ( ( obj = get_obj_carry( ch, arg ) ) == NULL
      && ( obj = get_obj_here( ch, arg ) ) == NULL )
    {
	send_to_char("That object is not here.\n\r", ch );
	return;
    }

    obj_interpret( obj, argument );
}

void obj_techo( OBJ_DATA * obj, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * ch;
    ROOM_INDEX_DATA * pRoom;

    argument = one_argument( argument, arg );

    pRoom = get_in_room_obj( obj );

    for ( ch = pRoom->people ; ch != NULL ; ch = ch->next_in_room )
	if ( is_name( arg, ch->name ) )
	{
	    send_to_char( argument, ch );
	    return;
	}

    for ( ch = char_list ; ch != NULL ;  ch = ch->next )
	if ( is_name( arg, ch->name ) )
	{
	    send_to_char( argument, ch );
	    return;
	}

}

void obj_say( OBJ_DATA * obj, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * pRoom;
    CHAR_DATA *victim;
    TRIGGER_DATA *pTrig; 
    char c;
    int i;

    if ( argument[0] == '\0' )
	return;

    pRoom = get_in_room_obj( obj );

    for ( i = strlen( argument ) - 1 ; i < 0 && argument[ i ] == ' ' ; i-- );
    c = argument[ i ];

    for ( pTrig = pRoom->triggers; pTrig != NULL; pTrig = pTrig->next )
    {
        if ( pTrig->trigger_type == TRIG_SAY  
	  && is_trig_keyword(pTrig, argument)
	  && !IS_SET(pTrig->bits, SCRIPT_HALT)
	  && !IS_SET(pTrig->flags, SCRIPT_HALT_RESET)
	  && pTrig->timer < 1 
          && pTrig->current == NULL )
         {
            pTrig->current = pTrig->script;
            pTrig->bits    = SCRIPT_ADVANCE;
            act_trigger_room( pRoom, pTrig->name, argument, obj->short_descr, NULL );
         }
	
    }
	
    for ( victim= pRoom->people ; victim != NULL ;victim=victim->next_in_room)
    {
	sprintf( buf, "`C$p %s '%s'`w", ( c == '!' ? "exclaims" : ( c == '?' ? "asks" : "says" ) ), argument );
	act( buf,victim, obj, obj ,TO_CHAR );

        for ( pTrig = IS_NPC(victim) ? victim->triggers : victim->triggers; pTrig != NULL; pTrig = pTrig->next )
	        {
            if ( pTrig->trigger_type == TRIG_SAY  
	      && is_trig_keyword(pTrig, argument)
	      && !IS_SET(pTrig->bits, SCRIPT_HALT)
	      && !IS_SET(pTrig->flags, SCRIPT_HALT_RESET)
	      && !IS_SET(victim->act2, ACT2_HALT)
	      && pTrig->timer < 1 
              && pTrig->current == NULL )
            {
            pTrig->current = pTrig->script;
            pTrig->bits    = SCRIPT_ADVANCE;
            act_trigger( victim, pTrig->name, argument, obj->short_descr, NULL );
            }
        }
    }
}

void obj_echo( OBJ_DATA * obj, char * argument )
{
    CHAR_DATA * ch;
    ROOM_INDEX_DATA * pRoom;

    pRoom = get_in_room_obj( obj );

    for ( ch = pRoom->people ; ch != NULL ; ch = ch->next_in_room )
	send_to_char( argument, ch );
}

void obj_echo_2( OBJ_DATA * obj, char * argument )
{
    CHAR_DATA * ch;
    ROOM_INDEX_DATA * pRoom;

    pRoom = get_in_room_obj( obj );

    for ( ch = pRoom->people ; ch != NULL ; ch = ch->next_in_room )
	{
	send_to_char( argument, ch );
	send_to_char( "\n\r", ch );
	}
}

const	struct	obj_cmd_type	obj_cmd_table	[] = 
{
	{ "echo",	obj_echo	},
	{ "echo2",	obj_echo_2	},
	{ "say",	obj_say		},
	{ "techo",	obj_techo	},
	{ "", 		NULL } 
};    

void obj_interpret( OBJ_DATA * obj, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    int cmd;

    argument = one_argument( argument, arg );

    if ( arg[ 0 ] == '\0' || get_in_room_obj( obj ) == NULL )
	return;

    for( cmd = 0 ; obj_cmd_table[ cmd ].name[ 0 ] != '\0'; cmd++ )
    {
	if ( !str_prefix( arg, obj_cmd_table[ cmd ].name ) )
	{
	    (*obj_cmd_table[ cmd ].obj_fun)( obj, argument );
	    return;
	}
    }
}
