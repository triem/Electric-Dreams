/*
 * Nimud scripts.  
 *      This script parser was begun on 5/7/94 by Herb Gilliland
 * (aka Locke of The Isles).  It was written to allow a more advanced mud
 * software to come into play.  The next_command() and multi_interpret()
 * routines were derived from the MOBprograms in Merc 2.2, and the work of
 * the buggy code of Natasha.  I found the whole MOBprograms to be bugged
 * to hell, and, since I myself didn't want to remain with the primitive
 * and crude script language that I developed, I wanted to make something
 * that was really useful.  I must thank the writers of MudOS (LpMUD) and
 * all MOOs, MUSEs, MUSHes and MUCKs, this really is based on the ideas
 * made by them.
 *                                      Enjoy.
 * See adjoining text for information on the syntax.
 */
/* Update began 10/01/95 by Galadrail and Quin
 * The tier of scripts remains the same but have evolved into independent
 * triggers.  OLC or any online managment of a mud has become a standard.
 * It was therefore nessasary to continue this approach with scripts.
 * Basicly, each trigger is an independent state: having its own vnum.
 * Each room, mob or object can have the same trigger and even multiple
 * triggers handled by a simple list.
 */ 

#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

/*
 * Local procedures.
 */
int	FORCE_LEVEL = MAX_LEVEL;
extern long	top_variable_data;
int get_dir( char *arg )
{
    int door = MAX_DIR;

       if ( !str_cmp( arg, "n" ) || !str_cmp( arg, "north" ) ) door = DIR_NORTH;
  else if ( !str_cmp( arg, "e" ) || !str_cmp( arg, "east"  ) ) door = DIR_EAST;
  else if ( !str_cmp( arg, "s" ) || !str_cmp( arg, "south" ) ) door = DIR_SOUTH;
  else if ( !str_cmp( arg, "w" ) || !str_cmp( arg, "west"  ) ) door = DIR_WEST;
  else if ( !str_cmp( arg, "u" ) || !str_cmp( arg, "up"    ) ) door = DIR_UP;
  else if ( !str_cmp( arg, "d" ) || !str_cmp( arg, "down"  ) ) door = DIR_DOWN;
    return door;
}


/*
 * This little command helps a user debug the scripts online.  It can give
 * information about a single trigger, or general info on the mob's current
 * state of execution.
 * [ON MAY - 31 96 MEM-Patch Applied by starburst]
 */

void do_script( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    TRIGGER_DATA *trig;
    VARIABLE_DATA *var;
    ROOM_INDEX_DATA *room;
    bool fRoom = FALSE;
    victim = NULL; 
    obj = NULL; 
    room = ch->in_room;
    argument = one_argument( argument, arg );
    one_argument( argument, arg2 );

    if ( arg[0] == '\0' )
    {
    send_to_char( "Script <mob name>\n\r", ch );
    send_to_char( "Script <mob name> <trigger name>\n\r", ch );
    send_to_char( "Script <obj name>\n\r", ch );
    send_to_char( "Script <obj name> <trigger name>\n\r", ch );
    send_to_char( "Script room\n\r", ch );
    send_to_char( "Script room  <trigger name>\n\r", ch );
	return;
    }

    fRoom = !str_cmp("room",arg );

    if (!fRoom)
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "Sorry, they aren't here.\n\r", ch );
	    return;
	}
    }

    if (!fRoom)
    if (victim != NULL)
    if (IS_NPC(victim) )
    {
    sprintf( buf, "Name: %s\n\r", victim->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %ld  Sex: %s  Room: %ld\n\r",
    victim->pIndexData->vnum,
    victim->sex == SEX_MALE    ? "male"   :
    victim->sex == SEX_FEMALE  ? "female" : "neuter",
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );
    }
    if (!fRoom)
    if (victim == NULL)
    {
    sprintf( buf, "Name: %s\n\r", obj->name );
    send_to_char( buf, ch );

    sprintf( buf, "Vnum: %ld In room %ld\n\r",
    obj->pIndexData->vnum,
	obj->in_room == NULL    ?        0 : obj->in_room->vnum
	);
    send_to_char( buf, ch );
    }

   if ( arg2[0] != '\0' )
   {
   for ( trig =  fRoom ? room->triggers : victim ? victim->triggers : obj->triggers ; trig != NULL; trig = trig->next )
   {
       if ( !str_prefix( trig->name, arg2 ) )
            break;
   }
   if ( trig == NULL )  send_to_char( "No such trigger.\n\r", ch );
   else
   {
       SCRIPT_DATA *scr;

       for ( scr = trig->script; scr != NULL; scr = scr->next )
       {
            if ( scr == trig->current )
                sprintf( buf, "##\n\r%s   <--\n\r", scr->command );
           else sprintf( buf, "##\n\r%s\n\r",       scr->command );
           send_to_char( buf, ch );
       }
   }
   }
   else
   {
   for ( trig = fRoom ? room->triggers : victim ? victim->triggers : obj->triggers; (trig != NULL);  trig = trig->next )
   {
      if ( trig != NULL )
      {
          sprintf( buf, "Trigger '%s' Vnum %d  key '%s'\n\r'%s' w/tra/typ: %d %d %d\n\rAttempts %6ld Activated %6ld Timer %d  %s %s %s %s\n\r",
                 trig->name,
                 trig->vnum,
                 trig->keywords,
                 trig->current != NULL ? trig->current->command :"not running",
                 trig->waiting,
                 trig->tracer,
                 trig->trigger_type,
                 trig->attempts,
                 trig->activated,
                 trig->timer,
                 IS_SET(trig->bits, SCRIPT_ADVANCE) ? "" : "stopped",
                 IS_SET(trig->bits, SCRIPT_HALT)    ? "halted" : "", 
                 IS_SET(trig->flags, TRIG_INACTIVE)    ? "Inactive" : "Active", 
                 IS_SET(trig->flags, SCRIPT_HALT_RESET)    ? "halt-reset" : "" );
          send_to_char( buf, ch );
      }
   }

   if ( fRoom ? room->variables != NULL : victim ? victim->variables != NULL : obj->variables != NULL ) send_to_char( "Variables:\n\r", ch );

   for ( var = fRoom ? room->variables : victim ? victim->variables : obj->variables; (var != NULL);  var = var->next )
   {
      if ( var != NULL )
      {
          sprintf( buf, "'%s' = '%s'\n\r",
                 var->name,
                 var->value );
          send_to_char( buf, ch );
      }
   }
   if (!fRoom)
   if (victim != NULL)
   if (IS_NPC(victim))
   {
   sprintf( buf, "Current spec tracer set to %d\n\r",
                  victim->spec_tracer);
   send_to_char( buf, ch );
   }
   }

   send_to_char( "\n\r", ch );
   return;
}

void do_tset_char(CHAR_DATA * ch, char *argument )
{
     do_tset(argument);
}

void do_tset(char *argument)
{
    TRIGGER_DATA * trigger;
    TRIGGER_INDEX_DATA * pTrig;
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument, arg);

    if (argument[0] == '\0' || arg[0] == '\0' )
    	return;

    if ( !str_cmp( "active", argument ) ) 
    {
	if ( ( pTrig = get_trig_index( atoi( arg ) ) ) == NULL )
	    return;

	for ( trigger = trigger_list; trigger != NULL; trigger = trigger->next_in_list )
	{
	    if ( trigger->vnum == pTrig->vnum )
	    {
	      	REMOVE_BIT( pTrig->flags, TRIG_INACTIVE );
	      	REMOVE_BIT( trigger->flags, TRIG_INACTIVE );
	    }
	}
    }

    if ( !str_cmp( "inactive", argument ) ) 
    {
	if ( ( pTrig = get_trig_index( atoi( arg ) ) ) == NULL ) 
	    return;

	for ( trigger = trigger_list; trigger != NULL; trigger = trigger->next_in_list )
	{
	    if ( trigger->vnum == pTrig->vnum )
	    {
	    	SET_BIT( pTrig->flags, TRIG_INACTIVE );
	    	SET_BIT( trigger->flags, TRIG_INACTIVE );
	    }
	}
    }
}
/*
 * This is a quick and dirty utility for adding a few fields to a trigger
 * quickly.  When a script is triggered, information is sent into variables
 * using this procedure.  The arg, from, to are preceded by the command
 * string, thus:
 * act_trigger( bill, "Argyle", "Red", NULL, NULL );
 * would put the following variables on the mob bill:
 * %Argyle-arg%      = Red
 */
void act_trigger( CHAR_DATA *mob,
                  char *command, char *argument, char *from, char *to )
{
    char buf[MAX_STRING_LENGTH];

    if ( !HAS_SCRIPT(mob) || command == NULL )
        return;

    if ( from != NULL )
    {
        sprintf( buf, "%%%s-from%%", command );
        assign_variable( buf, from, mob );
    }

    if ( to != NULL )
    {
        sprintf( buf, "%%%s-to%%", command );
        assign_variable( buf, to, mob );
    }

    if ( argument != NULL)
    {
        sprintf( buf, "%%%s-arg%%", command );
        assign_variable( buf, argument, mob );
    }
    return;
}
void act_trigger_room( ROOM_INDEX_DATA *room,
                  char *command, char *argument, char *from, char *to )
{
    char buf[MAX_STRING_LENGTH];

    if ( !HAS_SCRIPT_ROOM(room) || command == NULL )
        return;

    if ( from != NULL )
    {
        sprintf( buf, "%%%s-from%%", command );
        assign_variable_room( buf, from, room );
    }

    if ( to != NULL )
    {
        sprintf( buf, "%%%s-to%%", command );
        assign_variable_room( buf, to, room );
    }

    if ( argument != NULL)
    {
        sprintf( buf, "%%%s-arg%%", command );
        assign_variable_room( buf, argument, room );
    }
    return;
}
void act_trigger_obj( OBJ_DATA *obj,
                  char *command, char *argument, char *from, char *to )
{
    char buf[MAX_STRING_LENGTH];

    if ( !HAS_SCRIPT_OBJ(obj) || command == NULL )
        return;

    if ( from != NULL )
    {
        sprintf( buf, "%%%s-from%%", command );
        assign_variable_obj( buf, from, obj );
    }

    if ( to != NULL )
    {
        sprintf( buf, "%%%s-to%%", command );
        assign_variable_obj( buf, to, obj );
    }

    if ( argument != NULL)
    {
        sprintf( buf, "%%%s-arg%%", command );
        assign_variable_obj( buf, argument, obj );
    }
    return;
}


/*
 * This searches a list of variables for a variable with a certain name,
 * returning NULL if the variable happens to not currently exist.
 */
VARIABLE_DATA *get_variable( char *var_name, CHAR_DATA *mob )
{
    VARIABLE_DATA *var;

    for ( var = mob->variables;  var != NULL;  var = var->next )
    {
        if ( !str_cmp( var_name, var->name ) )
        {
            return var;
        }
    }

    return NULL;
}
VARIABLE_DATA *get_variable_room( char *var_name, ROOM_INDEX_DATA *room )
{
    VARIABLE_DATA *var;

    for ( var = room->variables;  var != NULL;  var = var->next )
    {
        if ( !str_cmp( var_name, var->name ) )
        {
            return var;
        }
    }

    return NULL;
}
VARIABLE_DATA *get_variable_obj( char *var_name, OBJ_DATA *obj )
{
    VARIABLE_DATA *var;

    for ( var = obj->variables;  var != NULL;  var = var->next )
    {
        if ( !str_cmp( var_name, var->name ) )
        {
            return var;
        }
    }

    return NULL;
}



/*
 * Unlike the above, this takes a variable name and returns a value string
 * no matter if the variable exists or not.  This is for func evaluations
 * and if-thens mostly.
 */
char *varval( char *var_name, CHAR_DATA *mob )
{
    VARIABLE_DATA *var;

    var = get_variable( var_name, mob );
    if ( var == NULL ) return &str_empty[0];

    return var->value;
}
char *varval_room( char *var_name, ROOM_INDEX_DATA *room )
{
    VARIABLE_DATA *var;

    var = get_variable_room( var_name, room );
    if ( var == NULL ) return &str_empty[0];

    return var->value;
}
char *varval_obj( char *var_name, OBJ_DATA *obj )
{
    VARIABLE_DATA *var;

    var = get_variable_obj( var_name, obj );
    if ( var == NULL ) return &str_empty[0];

    return var->value;
}

/*
 * Assigns or creates a variable with a value.   Translates interior variables.
 */
void assign_variable( char *var_name, char *value, CHAR_DATA *mob )
{
    VARIABLE_DATA *var;

    if ( ( var = get_variable( var_name, mob ) ) != NULL )
        strncpy( var->value, translate_variables( value, mob ), 80 );
    else
    {
        if ( variable_free == NULL )
        {
            var     = (VARIABLE_DATA *) alloc_perm( sizeof(*var) );
            top_variable_data++;
        }
        else
        {
            var     = variable_free;
            variable_free = variable_free->next;
        }
        var->next        = mob->variables;
        mob->variables   = var;
        strncpy( var->name, var_name, 80 );
        strncpy( var->value, translate_variables( value, mob ), 80 );
    }
    return;
}
void assign_variable_room( char *var_name, char *value, ROOM_INDEX_DATA *room )
{
    VARIABLE_DATA *var;

    if ( ( var = get_variable_room( var_name, room ) ) != NULL )
        strncpy( var->value, translate_variables_room( value, room ), 80 );
    else
    {
        if ( variable_free == NULL )
        {
            var     = (VARIABLE_DATA *) alloc_perm( sizeof(*var) );
            top_variable_data++;
        }
        else
        {
            var     = variable_free;
            variable_free = variable_free->next;
        }
        var->next        = room->variables;
        room->variables   = var;
        strncpy( var->name, var_name, 80 );
        strncpy( var->value, translate_variables_room( value, room ), 80 );
    }
    return;
}
void assign_variable_obj( char *var_name, char *value, OBJ_DATA *obj )
{
    VARIABLE_DATA *var;

    if ( ( var = get_variable_obj( var_name, obj ) ) != NULL )
        strncpy( var->value, translate_variables_obj( value, obj ), 80 );
    else
    {
        if ( variable_free == NULL )
        {
            var     = (VARIABLE_DATA *) alloc_perm( sizeof(*var) );
            top_variable_data++;
        }
        else
        {
            var     = variable_free;
            variable_free = variable_free->next;
        }
        var->next        = obj->variables;
        obj->variables   = var;
        strncpy( var->name, var_name, 80 );
        strncpy( var->value, translate_variables_obj( value, obj ), 80 );
    }
    return;
}

/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes, parenthesis (barring ) ('s) and percentages.
 */
/*char *first_arg( char *argument, char *arg_first, bool fCase )
{
    char cEnd;

    while ( *argument == ' ' )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"'
      || *argument == '%'  || *argument == '(' )
    {
        if ( *argument == '(' )
        {
            cEnd = ')';
            argument++;
        }
        else cEnd = *argument++;
    }

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
    if ( fCase ) *arg_first = LOWER(*argument);
            else  *arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( *argument == ' ' )
	argument++;

    return argument;
}
*/

/*
 * Translates all variables in a string, note the usage of replace_string()..
 * which is as good as any. ( I changed it to string_replace() same thing )
*/
char *translate_variables( char *argument, CHAR_DATA *mob )
{
    VARIABLE_DATA *var;
    char pstr[MAX_STRING_LENGTH];

    for ( var = mob->variables; var != NULL; var = var->next )
    {
        while ( strstr(argument, var->name) )
	{
	    strcpy( pstr, string_replace( argument, var->name, var->value ) );
            strcpy( argument, pstr );
	}
    }

    return argument;
} 
char *translate_variables_room( char *argument, ROOM_INDEX_DATA *room )
{
    VARIABLE_DATA *var;
    char pstr[MAX_STRING_LENGTH];

    for ( var = room->variables; var != NULL; var = var->next )
    {
        while ( strstr(argument, var->name) )
	{
	    strcpy( pstr, string_replace( argument, var->name, var->value ) );
            strcpy( argument, pstr );
	}
    }

    return argument;
} 
char *translate_variables_obj( char *argument, OBJ_DATA *obj )
{
    VARIABLE_DATA *var;
    char pstr[MAX_STRING_LENGTH];

    for ( var = obj->variables; var != NULL; var = var->next )
    {
        while ( strstr(argument, var->name) )
	{
	    strcpy( pstr, string_replace( argument, var->name, var->value ) );
            strcpy( argument, pstr );
	}
    }

    return argument;
} 
/*
 * Used to get sequential lines of a multi line string (separated by "\n\r")
 * Thus its like one_argument(), but a trifle different. It is destructive
 * to the multi line string argument, and thus clist must not be shared.
 */
char *next_command( char *argument,  char *arg_first )
{
    if ( *argument == '\n' )
    {
       *arg_first = ' ';
       arg_first++;
       *arg_first = '\0';

    return argument;
    }
    while ( *argument != '\0' )
    {
    if ( *argument == '\n' )
	{
        argument++;
    	    if ( *argument == '\r' )
               argument++;
	break;
	}
    *arg_first = *argument;
    arg_first++;
	argument++;
    }
    *arg_first = '\0';

    return argument;
}


/*
 * Evaluates multiple line command parses and does them all in one pulse.
 * Kinda kludgy, but useful.
 */
void multi_interpret ( CHAR_DATA *mob, char *com_list )
{
    char buf[MAX_STRING_LENGTH];

    while ( com_list[0] != '\0' )
    {
        com_list = next_command( com_list, buf );
        interpret( mob, buf );
    }
    return;
}


/*
 * Takes a plain and simple 1 + 2 + 4 * 2 and does a infix left to right
 * evaluation, returning the final numeric.
 */
int expression( char *ex, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char  op[MAX_STRING_LENGTH];
    int num;
    int val;

    if ( ex == NULL ) return 0;

    ex = first_arg( ex, buf, FALSE );
    if ( buf[0] == '%' ) strcpy( buf, varval( buf, ch ) );
    num = atoi( buf );

    while ( ex[0] != '\0' )
    {
        ex = first_arg( ex, op, TRUE );
        ex = first_arg( ex, buf, TRUE );
        if ( buf[0] == '%' ) strcpy( buf, varval( buf, ch ) );

        val = atoi( buf );

        switch( op[0] )
        {
            case '+':  num = num + val;  break;
            case '-':  num = num - val;  break;
            case '/':  num = num / val;  break;
            case '*':  num = num * val;  break;
            case '&':  num = num & val;  break;
            case '|':  num = num | val;  break;
            case '^':  num = num ^ val;  break;
             default: break;
        }
    }

    return num;
}


/*
 * This procedure is the 'let' routine. It assigns variables on a mobile,
 * allowing the translation of expressions with variables in them.
 */
void eval_assign( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char  var[MAX_STRING_LENGTH];
    char   op[MAX_STRING_LENGTH];
    char  val[MAX_STRING_LENGTH];
    char  num[MAX_STRING_LENGTH];
    char nval[MAX_STRING_LENGTH];
    
    argument = one_argument( argument, var );
    argument = first_arg( argument, op, TRUE );
    one_argument( argument, val );
    strcpy( val, argument );
    
      if ( !str_cmp( op, "==" ) ) assign_variable( var, val, ch );
 else if ( !str_cmp( op, "="  ) )
      {
          sprintf( num, "%d", expression( val, ch )  );
          assign_variable( var, num, ch );
      }
 else if ( !str_cmp( op, "+=" ) )
      {
          int iVal;
          int iOld;

          iVal = atoi( val );
          iOld = get_variable( var, ch ) == NULL ? 0 : atoi( get_variable( var, ch )->value );
          sprintf( val, "%d", iVal + iOld );
          assign_variable( var, val, ch );
      }
 else if ( !str_cmp( op, "-=" ) )
      {
          int iVal;
          int iOld;

          iVal = atoi( val );
          iOld = get_variable( var, ch ) == NULL ? 0 : atoi( get_variable( var, ch )->value );
          sprintf( val, "%d", iVal - iOld );
          assign_variable( var, val, ch );
      }
 else if ( !str_cmp( op, "|=" ) )
      {
          int iVal;
          int iOld;

          iVal = atoi( val );
          iOld = get_variable( var, ch ) == NULL ? 0 : atoi( get_variable( var, ch )->value );
          sprintf( val, "%d", iVal | iOld );
          assign_variable( var, val, ch );
      }
 else if ( !str_cmp( op, "&=" ) )
      {
          int iVal;
          int iOld;

          iVal = atoi( val );
          iOld = get_variable( var, ch ) == NULL ? 0 : atoi( get_variable( var, ch )->value );
          sprintf( val, "%d", iVal & iOld );
          assign_variable( var, val, ch );
      }
 else if ( !str_cmp( op, "^=" ) )
      {
          int iVal;
          int iOld;

          iVal = atoi( val );
          iOld = get_variable( var, ch ) == NULL ? 0 : atoi( get_variable( var, ch )->value );
          sprintf( val, "%d", iVal ^ iOld );
          assign_variable( var, val, ch );
      }
 else if ( !str_cmp( op, "++" ) )
      {
          sprintf( nval, "%d", atoi( get_variable( var, ch )->value ) + 1 );
          assign_variable( var, nval, ch );
      }
 else if ( !str_cmp( op, "--" ) )
      {
          sprintf( nval, "%d", atoi( get_variable( var, ch )->value ) - 1 );
          assign_variable( var, nval, ch );
      }

    return;
}
         
         
/*
 * the 'int' command -> does an interpret at the script level
 */
void eval_interp( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char *cmd;

    cmd = translate_variables( argument, ch );
    FORCE_LEVEL = MAX_LEVEL;   /* change as desired */
    interpret( ch, cmd );
    return;
}
void eval_interp_room( ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger, char *argument )
{
    char *cmd;

    cmd =  translate_variables_room( argument, room );
    FORCE_LEVEL = MAX_LEVEL;   /* change as desired */
    room_interpret( room, cmd );
    return;
}
void eval_interp_obj( OBJ_DATA *obj, TRIGGER_DATA *trigger, char *argument )
{
    char *cmd;

    cmd = translate_variables_obj( argument, obj );
    FORCE_LEVEL = MAX_LEVEL;   /* change as desired */
    obj_interpret( obj, cmd );
    return;
}

/*
 * the 'int' command -> does an interpret at the script level
 */
void eval_oed( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char cmd[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];

    argument = one_argument( argument, buf );

    strcpy( cmd, translate_variables( argument, ch ) );
    FORCE_LEVEL = LEVEL_BUILDER;   /* change as desired */
    switch ( UPPER(buf[0]) )
    {
        default: break;
        case 'a': aedit( ch, cmd ); break;
        case 'r': redit( ch, cmd ); break;
        case 'o': oedit( ch, cmd ); break;
        case 'm': medit( ch, cmd ); break;
    }

    FORCE_LEVEL = MAX_LEVEL;   /* change as desired */
    return;
}

/*
 * This procedure does a few different 'halt' functions:
 *
 * 1)   triggers  -> will halt the single trigger until mob dies
 * 2)   all       -> halts the whole mob
 * 3)   statement -> aborts a single 'Sc' statement from continuing
 * 4)   once      -> abort it this time
 */
bool eval_halt( CHAR_DATA *ch, TRIGGER_DATA *trigger, char* argument )
{
    char arg[MAX_STRING_LENGTH];

    one_argument( argument, arg );

     if ( !str_cmp( arg, "once" ) )
     {
        trigger->current = NULL;
        return FALSE;
     }
else if ( !str_cmp( arg, "trigger" ) )
     {
        trigger->current = NULL;
        SET_BIT( trigger->bits, SCRIPT_HALT );
        return FALSE;
     }
else if ( !str_cmp( arg, "reset" ) )
     {
        trigger->current = NULL;
        SET_BIT( trigger->flags, SCRIPT_HALT_RESET );
        return FALSE;
     }
else if ( !str_cmp( arg, "all" ) )
     {
        TRIGGER_DATA *Trig;
	if ( ch != NULL )
	{
        SET_BIT( ch->act2, ACT2_HALT );
         for ( Trig = ch->triggers; Trig != NULL; Trig = Trig->next )
           Trig->current = NULL;
        return FALSE;
	}
	else
	{
        trigger->current = NULL;
        SET_BIT( trigger->bits, SCRIPT_HALT );
        return FALSE;
	}
     }
else if ( !str_cmp( arg, "statement" ) )  return FALSE;

     return TRUE;
}

/*
 *  This procedure sets the pulse-wait state on mobs ( num = 2*arg1 )
 */
void eval_wait( CHAR_DATA *ch, ROOM_INDEX_DATA * room, OBJ_DATA *obj,  TRIGGER_DATA *trigger, char* argument )
{
    char arg[MAX_STRING_LENGTH];
    int waittime;
    int world =0;

    one_argument( argument, arg );

    if ( ch )
    	world = ch->in_room->area->world;

    if ( room )
    	world = room->area->world;

    if ( obj )
    {
	if ( obj->carried_by )
	    world = obj->carried_by->in_room->area->world;
	if ( obj->in_obj )
	    world = obj->in_obj->in_room->area->world;
	if ( obj->in_room )
	    world = obj->in_room->area->world;
    }

	if ( arg[0] == '\0' )
        return;

    if ( is_number( arg ) )
    {
       trigger->waiting = atoi( arg );
       return;
    }
    else
    if ( !str_cmp( arg, "time" ) )
    {
       argument = one_argument( argument, arg );
       if ( !is_number( arg ) )
          return;
       waittime = atoi( arg );
       if ( waittime != time_info[ world ].hour ) trigger->tracer--;
    }

   return;
}


/*
 *  This goto evaluates things one of two ways:
 *  1)  Will go to a specific script entry (counting 0 as first)
 *  2)  Go to a label ( define labels as the first part of a script ': label')
 */
void eval_goto( CHAR_DATA *ch, TRIGGER_DATA *trigger, char* argument )
{
    char arg[MAX_STRING_LENGTH];
    SCRIPT_DATA *scr = trigger->script;
 
    one_argument( argument, arg );

    if ( is_number( arg ) )
    {
        int count = 1;
        int jump_loc;

        jump_loc = atoi( arg );

        while ( count < jump_loc && scr != NULL )
        {
             count++;
             scr = scr->next;
        }
    }
    else
    {
        char arg2[MAX_STRING_LENGTH];
        char *cmd;

        while( scr != NULL )
        {
            cmd = one_argument( scr->command, arg2 );
           if ( !str_cmp( arg2, ":" ) )
            {
               cmd = one_argument( cmd, arg2 );
               if ( !str_cmp( arg2, arg ) )
                   break;
            }
	    scr = scr->next;
        }
    }

    if ( scr == NULL ) 
    {
        trigger->current = NULL;
        return;
    }
    trigger->current = scr; 
    script_interpret( ch, trigger );
    return;
}

/*
 * Assigns a single value on a mobile:
 * usage:
 * mfunc <mobile/variable of mobile's name>  <value/variable w/ value> <what>
 */
void eval_msign( CHAR_DATA *ch, ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char arg4[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    int val,sn,val2;
    victim = NULL;    
    argument = one_argument( argument, arg1 );    /* mob to assign */
    argument = one_argument( argument, arg2 );    /* value to assign */
    argument = one_argument( argument, arg3 );    /* deliminator */
    argument = one_argument( argument, arg4 );    /* value 2 to assign */

    if (ch != NULL )
    {
	if ( arg1[0] == '%' ) strcpy( arg1, varval( arg1, ch ) );
	if ( arg2[0] == '%' ) strcpy( arg2, varval( arg2, ch ) );

	victim = get_char_world( ch, arg1 );
    }
    else
    {
	if ( arg1[0] == '%' ) strcpy( arg1, varval_room( arg1, room ) );
	if ( arg2[0] == '%' ) strcpy( arg2, varval_room( arg2, room ) );

	for (rch = room->people; rch != NULL; rch = rch->next_in_room)
	{
	    if (!is_name(arg1, rch->name) )
	    continue;
	    victim = rch;
	}
    }
    val = atoi( arg2 );
    val2 = atoi( arg4 );

    if ( victim == NULL )
    {
        eval_halt( ch, trigger, "once" );
        return;
    }
else if ( !str_cmp(arg3, "position") )   victim->position    = val;
/*else if ( !str_cmp(arg3, "gold" ) || !str_cmp( arg3, "money" ) )
     {
         if ( val > 0 ) create_amount( val, victim, NULL, NULL );
         else if ( val < 0 ) sub_coins( abs(val), victim );
     }
*/
else if ( !str_cmp(arg3, "hunting" ) )	strncpy(victim->hunting,arg2,30);
else if ( !str_cmp(arg3, "spell" ) )	
{
    if ( ( sn = skill_lookup( arg2 ) ) < 1 || sn > MAX_SKILL )
	return;

    if (ch->pcdata->learned[sn])
	return;
    if ( skill_table[sn].spell_fun != spell_null )
	victim->pcdata->learned[sn] = UMAX( 40, race_table[ victim->race ][ victim->sex ].starting[ sn ] );
    else
	victim->pcdata->learned[sn] = UMAX( 25, race_table[ victim->race ][ victim->sex ].starting[ sn ] );
}
else if ( !str_cmp(arg3, "dislikes" ) )	strncpy(victim->dislikes, arg2,30 );
else if ( !str_cmp(arg3, "prefix" ) )	
	{
	strncpy( victim->prefix, arg2, 30 );
	send_to_char("\n\r",victim);
	}
else if ( !str_cmp(arg3, "quest") )   set_quest( victim, val, 0, arg4 == NULL ? 0 : get_quest_flags(arg4 ) );
else if ( !str_cmp(arg3, "mood" ) )	victim->mood     	= val;
else if ( !str_cmp(arg3, "quest_points" ) )set_quest( victim, val, val2, 0 );
else if ( !str_cmp(arg3, "race" ) )	victim->race     	= val;
else if ( !str_cmp(arg3, "kills" ) )	victim->kills     	= val;
else if ( !str_cmp(arg3, "pkills" ) )	victim->pkills     	= val;
else if ( !str_cmp(arg3, "wimpy" ) )	victim->wimpy     	= val;
else if ( !str_cmp(arg3, "spectracer"))  victim->spec_tracer = val;
/*else if ( !str_cmp(arg3, "bounty" ) )    victim->bounty      = val;*/
/*else if ( !str_cmp(arg3, "owed" ) )      victim->owed        = val;*/
else if ( !str_cmp(arg3, "act" ) )       victim->act         = val;
else if ( !str_cmp(arg3, "act2" ) )      victim->act2        = val;
else if ( !str_cmp(arg3, "affected_by")) victim->affected_by = val;
/*else if ( !str_cmp(arg3, "learntime" ) ) victim->learn_time  = val;*/
else if ( !str_cmp(arg3, "sex" ) )       victim->sex         = val;
else if ( !str_cmp(arg3, "practice" ) )  victim->practice    = val;
else if ( !str_cmp(arg3, "practice_plus" ) )  victim->practice    += val;
else if ( !str_cmp(arg3, "race" ) )      victim->race        = val;
else if ( !str_cmp(arg3, "level" ) )     victim->level       = val;
else if ( !str_cmp(arg3, "hit" ) )       victim->hit         = val;
else if ( !str_cmp(arg3, "maxhit" ) )       victim->max_hit         = val;
else if ( !str_cmp(arg3, "move" ) )      victim->max_move        = val;
else if ( !str_cmp(arg3, "maxmove" ) )      victim->move        = val;
else if ( !str_cmp(arg3, "str") && !IS_NPC(ch)) victim->perm_stat[0] = val;
else if ( !str_cmp(arg3, "int") && !IS_NPC(ch)) victim->perm_stat[1] = val;
else if ( !str_cmp(arg3, "wis") && !IS_NPC(ch)) victim->perm_stat[2] = val;
else if ( !str_cmp(arg3, "dex") && !IS_NPC(ch)) victim->perm_stat[3] = val;
else if ( !str_cmp(arg3, "con") && !IS_NPC(ch)) victim->perm_stat[4] = val;
else if ( !str_cmp(arg3, "chr") && !IS_NPC(ch)) victim->perm_stat[5] = val;

    return;
}



void eval_osign( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int val;
    
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '%' ) strcpy( arg1, varval( arg1, ch ) );
    if ( arg2[0] == '%' ) strcpy( arg2, varval( arg2, ch ) );
    obj = get_obj_world( ch, arg1 );
    val = atoi( arg2 );

    if ( obj == NULL )
    {
        eval_halt( ch, trigger, "once" );
        return;
    }
else if ( !str_cmp(arg3, "cost" ) )        obj->wear_data->cost        = val;
else if ( !str_cmp(arg3, "armor" ) )       obj->wear_data->ac		= val;
else if ( !str_cmp(arg3, "size" ) )        obj->wear_data->size        = val;
else if ( !str_cmp(arg3, "wear_flags" ) )  obj->wear_data->wear_flags  = val;
else if ( !str_cmp(arg3, "wear_loc" ) )    obj->wear_data->wear_loc    = val;
else if ( !str_cmp(arg3, "weight" ) )      obj->wear_data->weight      = val;
else if ( !str_cmp(arg3, "timer" ) )       obj->timer       = val;
else if ( !str_cmp(arg3, "extra_flags" ) ) obj->extra_flags = val;

    return;
}

void eval_wsign( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    int val;
    int world=ch->in_room->area->world;    
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg2[0] == '%' ) strcpy( arg2, varval( arg2, ch ) );
    val = atoi( arg2 );

     if ( !str_cmp( arg3, "hour" ) )         time_info[ world ].hour           = val;
else if ( !str_cmp( arg3, "day" ) )          time_info[ world ].day            = val;
else if ( !str_cmp( arg3, "month" ) )        time_info[ world ].month          = val;
else if ( !str_cmp( arg3, "year" ) )         time_info[ world ].year           = val;
else if ( !str_cmp( arg3, "sunlight" ) )     weather_info[ world ].sunlight    = val;
else if ( !str_cmp( arg3, "mmhg" ) )         weather_info[ world ].mmhg        = val;
/*else if ( !str_cmp( arg3, "winddir" ) )      weather_info.winddir     = val;*/
/*else if ( !str_cmp( arg3, "windspeed" ) )    weather_info.windspeed   = val;*/
/*else if ( !str_cmp( arg3, "temperature" ) )  weather_info.temperature = val;*/
else if ( !str_cmp( arg3, "weather" ) )      weather_info[ world ].sky         = val;

    return;
}

/*
 * Grabs world stuff/random numbers.
 * Usage:
 * wfunc %variable-to-go-in% <what> [<min random> <max random>]
 */
void eval_wfunc( CHAR_DATA *ch, ROOM_INDEX_DATA * pRoom, OBJ_DATA *obj, TRIGGER_DATA *trigger, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    ROOM_INDEX_DATA * room;
    int world; 
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg3 );
    room = NULL;
    if ( ch )
	room = ch->in_room;
    if ( pRoom )
	room = pRoom;
    if ( obj )
    {
	if ( obj->carried_by )
	    room = obj->carried_by->in_room;
	if ( obj->in_obj )
	    room = obj->in_obj->in_room;
	if ( obj->in_room )
	    room = obj->in_room;
	if ( !room )
	{
        bug( "Obj not in room scr.c wfunc", 0 );
	return;
	}
    }
     world = room->area->world;

     if ( !str_cmp( arg3, "hour" ) )
                   sprintf( buf, "%d", time_info[ world ].hour );
else if ( !str_cmp( arg3, "day" ) )
                   sprintf( buf, "%d", time_info[ world ].day );
else if ( !str_cmp( arg3, "month" ) )
                   sprintf( buf, "%d", time_info[ world ].month );
else if ( !str_cmp( arg3, "year" ) )
                   sprintf( buf, "%d", time_info[ world ].year );
else if ( !str_cmp( arg3, "sunlight" ) )
                   sprintf( buf, "%d", weather_info[ world ].sunlight );
else if ( !str_cmp( arg3, "mmhg" ) )
                   sprintf( buf, "%d", weather_info[ world ].mmhg );
/*else if ( !str_cmp( arg3, "winddir" ) )
                   sprintf( buf, "%d", weather_info.winddir );
else if ( !str_cmp( arg3, "windspeed" ) )
                   sprintf( buf, "%d", weather_info.windspeed );
else if ( !str_cmp( arg3, "temperature" ) )
                   sprintf( buf, "%d", weather_info.temperature );*/
else if ( !str_cmp( arg3, "weather" ) )
                   sprintf( buf, "%d", weather_info[ world ].sky );
else if ( !str_cmp( arg3, "topmob" ) )
                   sprintf( buf, "%ld", top_mob_index );
else if ( !str_cmp( arg3, "topobj" ) )
                   sprintf( buf, "%ld", top_obj_index );

else if ( !str_cmp( arg3, "toproom" ) )
                   sprintf( buf, "%ld", top_vnum_room );
else if ( !str_cmp( arg3, "exitinfo" ) )
     {
         char arg4[MAX_STRING_LENGTH];
         char arg5[MAX_STRING_LENGTH];
         int door;

         argument = one_argument( argument, arg4 );
         if ( arg4[0] == '%' ) 
	 {
	     if ( ch )
	     strcpy( arg4, varval( arg4, ch ) );
	     if ( pRoom )
	     strcpy( arg4, varval_room( arg4, room ) );
	     if ( obj )
	     strcpy( arg4, varval_obj( arg4, obj ) );
	 }
         argument = one_argument( argument, arg5 );
         if ( arg5[0] == '%' ) 
	 {
	     if ( ch )
	     strcpy( arg4, varval( arg5, ch ) );
	     if ( pRoom )
	     strcpy( arg4, varval_room( arg5, room ) );
	     if ( obj )
	     strcpy( arg4, varval_obj( arg5, obj ) );
	 }
         door = get_dir( arg4 );
         if ( room->exit[door] == NULL )
             sprintf( buf, "FALSE" );
         else if ( arg5[0] == '\0' )
             sprintf( buf, "FALSE" );
	 else if (IS_SET(room->exit[door]->exit_info, get_exit_flag_number(arg5) ) )
         sprintf( buf, "TRUE" );
	 else
         sprintf( buf, "FALSE" );

     }
else if ( !str_cmp( arg3, "random" ) )
     {
         char arg4[MAX_STRING_LENGTH];
         char arg5[MAX_STRING_LENGTH];

         argument = one_argument( argument, arg4 );
         if ( arg4[0] == '%' ) 
	 {
	     if ( ch )
	     strcpy( arg4, varval( arg4, ch ) );
	     if ( pRoom )
	     strcpy( arg4, varval_room( arg4, room ) );
	     if ( obj )
	     strcpy( arg4, varval_obj( arg4, obj ) );
	 }
         argument = one_argument( argument, arg5 );
         if ( arg5[0] == '%' ) 
	 {
	     if ( ch )
	     strcpy( arg5, varval( arg5, ch ) );
	     if ( pRoom )
	     strcpy( arg5, varval_room( arg5, room ) );
	     if ( obj )
	     strcpy( arg5, varval_obj( arg5, obj ) );
	 }

         sprintf( buf, "%d", number_range(atoi( arg4 ), atoi( arg5 )) );
     }
else {
         int door = 0;

	 if ( ch )
         door = get_dir( varval( arg3, ch ) );
	 if ( obj )
         door = get_dir( varval_obj( arg3, obj ) );
	 if ( pRoom )
         door = get_dir( varval_room( arg3, room ) );
         sprintf( buf, "%ld", room->exit[door] != NULL
                          && room->exit[door]->to_room != NULL ?
                             room->exit[door]->to_room->vnum :
                             ROOM_VNUM_TEMPLATE );
     }

    if ( ch )
    assign_variable( arg1,  buf, ch );
    if ( pRoom )
    assign_variable_room( arg1,  buf, room );
    if ( obj )
    assign_variable_obj( arg1,  buf, obj );
    return;
}


void eval_mfunc( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    
    argument = one_argument( argument, arg1 );   /* mob name */
    argument = one_argument( argument, arg2 );   /* variable to put val in */
    argument = one_argument( argument, arg3 );   /* deliminator */

    if ( arg1[0] == '%' ) strcpy( arg1, varval( arg1, ch ) );
    
   /* victim = !str_cmp( arg1, "self" ) ? ch : get_char_world( ch, arg1 ); */
    victim =  get_char_world( ch, arg1 ); 

    if ( victim == NULL )
    {
        eval_halt( ch, trigger, "once" );
        return;
    }
else if ( !str_cmp(arg3, "position") )  
                    sprintf( buf, "%d", victim->position );
/*else if ( !str_cmp(arg3, "gold" ) )    
                    sprintf( buf, "%d", tally_coins( victim ) );
*/
else if ( !str_cmp(arg3, "name" ) )
         	sprintf( buf, "%s", victim->name);
else if ( !str_cmp(arg3, "vnum" ) )
		if (IS_NPC(victim))
         	sprintf( buf, "%ld", victim->pIndexData->vnum);
		else
         	sprintf( buf, "%d", 0);
else if ( !str_cmp(arg3, "hunting" ) )
                    sprintf( buf, "%s", victim->hunting );
else if ( !str_cmp(arg3, "room" ) )
                    sprintf( buf, "%ld", victim->in_room->vnum );
/*else if ( !str_cmp(arg3, "specproc" ) )
                    sprintf( buf, "%s", victim->spec_name );*/
else if ( !str_cmp(arg3, "spectracer" ) )
                    sprintf( buf, "%d", victim->spec_tracer );
/*else if ( !str_cmp(arg3, "owed" ) )
                    sprintf( buf, "%d", victim->owed );*/
/*else if ( !str_cmp(arg3, "bounty" ) )
                    sprintf( buf, "%d", victim->bounty );*/
else if ( !str_cmp(arg3, "act" ) )
                    sprintf( buf, "%ld", victim->act );
else if ( !str_cmp(arg3, "act2" ) )
                    sprintf( buf, "%ld", victim->act2 );
else if ( !str_cmp(arg3, "affected_by" ) )
                    sprintf( buf, "%ld", victim->affected_by );
/*else if ( !str_cmp(arg3, "learntime" ) )
                    sprintf( buf, "%d", victim->learn_time );*/
else if ( !str_cmp(arg3, "hitroll" ) )
                    sprintf( buf, "%d", GET_HITROLL(victim) );
else if ( !str_cmp(arg3, "damroll" ) )
                    sprintf( buf, "%d", GET_DAMROLL(victim) );
else if ( !str_cmp(arg3, "age" ) )
                    sprintf( buf, "%d", get_age(victim) );
else if ( !str_cmp(arg3, "sex" ) )
                    sprintf( buf, "%d", victim->sex );
else if ( !str_cmp(arg3, "race" ) )
                    sprintf( buf, "%d", victim->race );
else if ( !str_cmp(arg3, "level" ) )
                    sprintf( buf, "%d", victim->level );
else if ( !str_cmp(arg3, "hit" ) )
                    sprintf( buf, "%d", victim->hit );
else if ( !str_cmp(arg3, "maxhit" ) )
                    sprintf( buf, "%d", victim->max_hit );
else if ( !str_cmp(arg3, "practice" ) )
                    sprintf( buf, "%d", victim->practice );
else if ( !str_cmp(arg3, "move" ) )
                    sprintf( buf, "%d", victim->move );
else if ( !str_cmp(arg3, "maxmove" ) )
                    sprintf( buf, "%d", victim->max_move );
else if ( !str_cmp(arg3, "str" ) )
                    sprintf( buf, "%d", get_curr_stat( victim, STAT_STR ) );
else if ( !str_cmp(arg3, "int" ) )
                    sprintf( buf, "%d", get_curr_stat( victim, STAT_INT ) );
else if ( !str_cmp(arg3, "dex" ) )
                    sprintf( buf, "%d", get_curr_stat( victim, STAT_DEX ) );
else if ( !str_cmp(arg3, "wis" ) )
                    sprintf( buf, "%d", get_curr_stat( victim, STAT_WIS ) );
else if ( !str_cmp(arg3, "con" ) )
                    sprintf( buf, "%d", get_curr_stat( victim, STAT_CON ) );
else if ( !str_cmp(arg3, "chr" ) )
                    sprintf( buf, "%d", get_curr_stat( victim, STAT_CHR ) );
else return;

    assign_variable( arg2,  buf, ch );
    return;
}

void eval_ocreate( CHAR_DATA *ch, ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *obj;
    OBJ_DATA *obj2;
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    victim = NULL;    
    argument = one_argument( argument, arg1 );     /* object vnum */
    argument = one_argument( argument, arg2 );     /* deliminator */
    argument = one_argument( argument, arg3 );     /* victim */

    obj = get_obj_index( atoi(arg1) );
    if (ch != NULL )
    {
	if ( arg3[0] == '%' ) strcpy( arg3, varval( arg3, ch ) );

	victim = get_char_world( ch, arg3 );
    }
    else
    {
	if ( arg3[0] == '%' ) strcpy( arg3, varval_room( arg3, room ) );

	for (rch = room->people; rch != NULL; rch = rch->next_in_room)
	{
	    if (!is_name(arg3, rch->name) )
	    continue;
	    victim = rch;
	}
    }
    if ( victim == NULL )
     {
        eval_halt( ch, trigger, "once" );
        return;
     }
     if ( obj == NULL ) sprintf( buf, "NOTHING" );
else if ( !str_cmp(arg2, "room" ) )
     {
	obj2 = create_object(obj);
	obj_to_room(obj2,victim->in_room);
     }
else if ( !strncmp(arg2, "inventory",3 ) )
     {
	obj2 = create_object(obj);
	obj_to_char(obj2,victim);	
     }
else if ( wear_name_loc( arg2 ) != WEAR_NONE )
	{
	obj2 = create_object(obj);
	obj_to_char(obj2,victim);	
	equip_char(victim, obj2, wear_name_loc( arg2 ) );
	}
else return;

    return;
}

void eval_path( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{

    if (*argument == '\0' )
	return;    
    strncpy( ch->task, argument, 80 );
    SET_BIT(trigger->flags, SCRIPT_PATH );
    return;
}

void eval_ofunc( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf [MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    
    argument = one_argument( argument, arg1 );     /* object name */
    argument = one_argument( argument, arg2 );     /* variable to put val in */
    argument = one_argument( argument, arg3 );     /* deliminator */

    if ( arg1[0] == '%' ) strcpy( arg1, varval( arg1, ch ) );
    
    obj = get_obj_world( ch, arg1 );

     if ( obj == NULL ) sprintf( buf, "NOTHING" );
else if ( !str_cmp(arg3, "cost" ) )
                    sprintf( buf, "%ld", obj->wear_data->cost  );
else if ( !str_cmp(arg3, "name" ) )
         sprintf( buf, "%s", obj->name);
else if ( !str_cmp(arg3, "short" ) )
         sprintf( buf, "%s",obj->short_descr );
/*else if ( !str_cmp(arg3, "action" ) )
                    sprintf( buf, "%s", STR(obj, action_descr) );*/
else if ( !str_cmp(arg3, "room" ) )
                    sprintf( buf, "%ld", obj->in_room != NULL ?
                                        obj->in_room->vnum : 0 );
else if ( !str_cmp(arg3, "inobj" ) )
                    sprintf( buf, "%s", obj->in_obj != NULL ?
                                        STR(obj->in_obj, name) : "none" );
else if ( !str_cmp(arg3, "carried_by" ) )
                    sprintf( buf, "%s", obj->carried_by != NULL ?
                                        PERS(obj->carried_by, ch) : "none" ); 
else if ( !str_cmp(arg3, "wear_flags" ) )
                    sprintf( buf, "%ld", obj->wear_data->wear_flags );
else if ( !str_cmp(arg3, "wear_loc" ) )
                    sprintf( buf, "%d", obj->wear_data->wear_loc );
else if ( !str_cmp(arg3, "weight" ) )
                    sprintf( buf, "%d", obj->wear_data->weight );
else if ( !str_cmp(arg3, "vnum" ) )
                    sprintf( buf, "%ld", obj->pIndexData->vnum );
else if ( !str_cmp(arg3, "timer" ) )
                    sprintf( buf, "%d", obj->timer );
else if ( !str_cmp(arg3, "extra_flags" ) )
                    sprintf( buf, "%ld", obj->extra_flags );
else return;

    assign_variable( arg2,  buf, ch );
    return;
}

void eval_sfunc( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
/*    char  arg[MAX_STRING_LENGTH];    */
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];    

    char *st;
    int count;
 
    argument = one_argument( argument, arg1 );    /* string */
    argument = one_argument( argument, arg2 );    /* variable to put it in */
    argument = one_argument( argument, arg3 );    /* arg deliminator */
    if ( arg1[0] == '%' ) sprintf( arg1, "%s", varval( arg1, ch ) );
    
/*    if ( !str_cmp(arg3, "money") )
      {
    sprintf( buf, "%s", name_amount( atoi( arg1 ) ) );
	  }
*/
else if ( is_number(arg3) )
  {
    st = one_argument( arg1, buf );
    for ( count = atoi( arg3 ); count > 0; count-- )
      {
        st = one_argument( st, buf );
      }
  }
 
    assign_variable( arg2,  buf, ch );
    return;
  }



bool boolchk( char *v1, char *v2, char *s )
{
    if ( !str_cmp( s, "in"       ) ) return (str_infix(v1, v2)   );
    if ( !str_cmp( s, "pre"      ) ) return (str_prefix(v1, v2)  );
    if ( !str_cmp( s, "cmp"      ) ) return (!str_cmp(v1, v2)    );
    if ( !str_cmp( s, "substr"   ) ) return (strstr(v2, v1)!=NULL);
    if ( !str_cmp( s, "num"      ) ) return (is_number(v1)       );
    if ( !str_cmp( s, ">"        ) ) return (atoi(v1) >  atoi(v2));
    if ( !str_cmp( s, "<"        ) ) return (atoi(v1) <  atoi(v2));
    if ( !str_cmp( s, ">="       ) ) return (atoi(v1) >= atoi(v2));
    if ( !str_cmp( s, "<="       ) ) return (atoi(v1) <= atoi(v2));
    if ( !str_cmp( s, "=="       ) ) return (atoi(v1) == atoi(v2));
    if ( !str_cmp( s, "!="       ) ) return (atoi(v1) != atoi(v2));
    if ( !str_cmp( s, "&"        ) ) return (atoi(v1) &  atoi(v2));
    if ( !str_cmp( s, "|"        ) ) return (atoi(v1) |  atoi(v2));
    if ( !str_cmp( s, "isset"    ) ) return (IS_SET(atoi(v1), atoi(v2)));
    if ( s[0] == '\0' )              return (atoi(v1));

    return FALSE;
}

#define C_AND     1
#define C_OR      2
#define C_NOT     4
bool eval_if( CHAR_DATA *ch, ROOM_INDEX_DATA *room, OBJ_DATA *obj, TRIGGER_DATA *trigger, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *arg;
    char v1[MAX_STRING_LENGTH];
    char v2[MAX_STRING_LENGTH];
    char  s[MAX_STRING_LENGTH];
    bool ifstatement = TRUE;
    bool prev = TRUE;
    int andornot;

    while ( argument[0] != '\0' )
    {
    argument = first_arg( argument, buf, TRUE );
    arg = buf;

    while ( arg[0] != '\0' )          /* do a single ( ) */
    {
    andornot = 0;
    arg = one_argument( arg, v1 );

    if ( !str_cmp( v1, "not" ) || !str_cmp( v1, "!" ) )
    {
        arg = one_argument( arg, v1 );
        SET_BIT(andornot, C_NOT);
        prev = ifstatement;
    }
    else
    if ( !str_cmp( v1, "or" ) || !str_cmp( v1, "||" ) )
    {
        arg = one_argument( arg, v1 );
        SET_BIT(andornot, C_OR);
        prev = ifstatement;
    }
    else
    if ( !str_cmp( v1, "and" ) || !str_cmp( v1, "&&" ) )
    {
        arg = one_argument( arg, v1 );
        SET_BIT(andornot, C_AND);
        prev = ifstatement;
    }
    else
    {
        arg = one_argument( arg, s  );
        arg = one_argument( arg, v2 );

	if ( ch != NULL )
	{
        if ( v1[0] == '%' )  strcpy(v1, varval( v1, ch ));
        if ( v2[0] == '%' )  strcpy(v2, varval( v2, ch ));
        if (  s[0] == '%' )  strcpy( s, varval( s,  ch ));
	}
	if ( room != NULL )
	{
        if ( v1[0] == '%' )  strcpy(v1, varval_room( v1, room ));
        if ( v2[0] == '%' )  strcpy(v2, varval_room( v2, room ));
        if (  s[0] == '%' )  strcpy( s, varval_room( s,  room ));
	}
	if ( obj != NULL )
	{
        if ( v1[0] == '%' )  strcpy(v1, varval_obj( v1, obj ));
        if ( v2[0] == '%' )  strcpy(v2, varval_obj( v2, obj ));
        if (  s[0] == '%' )  strcpy( s, varval_obj( s,  obj ));
	}

	if ( ch != NULL )
        if ( IS_SET(ch->act2, ACT2_TRACKSCR) )
        {
        sprintf( log_buf, "boolchk: v1'%s' v2'%s' s'%s'", v1, v2, s );
	log_string(log_buf);
    /*    NOTIFY(log_buf, LEVEL_IMMORTAL, WIZ_NOTIFY_SCRIPT); change to infochan */
        }

        ifstatement = boolchk( v1, v2, s );
    }

    if ( IS_SET(andornot, C_NOT) ) ifstatement = !ifstatement;
    if ( IS_SET(andornot, C_AND) ) ifstatement = ifstatement && prev;
    if ( IS_SET(andornot, C_OR)  ) ifstatement = ifstatement || prev;

    }
    }

    return ifstatement;
}

bool eval_while( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char *arg;
    char v1[MAX_STRING_LENGTH];
    char v2[MAX_STRING_LENGTH];
    char  s[MAX_STRING_LENGTH];
    bool ifstatement = TRUE;
    bool prev = TRUE;
    int andornot;

    while ( argument[0] != '\0' )
    {
    argument = first_arg( argument, buf, TRUE );
    arg = buf;

    while ( arg[0] != '\0' )          /* do a single ( ) */
    {
    andornot = 0;
    arg = one_argument( arg, v1 );

    if ( !str_cmp( v1, "not" ) || !str_cmp( v1, "!" ) )
    {
        arg = one_argument( arg, v1 );
        SET_BIT(andornot, C_NOT);
        prev = ifstatement;
    }
    else
    if ( !str_cmp( v1, "or" ) || !str_cmp( v1, "||" ) )
    {
        arg = one_argument( arg, v1 );
        SET_BIT(andornot, C_OR);
        prev = ifstatement;
    }
    else
    if ( !str_cmp( v1, "and" ) || !str_cmp( v1, "&&" ) )
    {
        arg = one_argument( arg, v1 );
        SET_BIT(andornot, C_AND);
        prev = ifstatement;
    }
    else
    {
        arg = one_argument( arg, s  );
        arg = one_argument( arg, v2 );

        if ( v1[0] == '%' )  strcpy(v1, varval( v1, ch ));
        if ( v2[0] == '%' )  strcpy(v2, varval( v2, ch ));
        if (  s[0] == '%' )  strcpy( s, varval( s,  ch ));

        ifstatement = boolchk( v1, v2, s );
    }

    if ( IS_SET(andornot, C_NOT) ) ifstatement = !ifstatement;
    if ( IS_SET(andornot, C_AND) ) ifstatement = ifstatement && prev;
    if ( IS_SET(andornot, C_OR)  ) ifstatement = ifstatement || prev;

    }
    }

    if ( !ifstatement ) SET_BIT(trigger->bits, SCRIPT_ADVANCE);
                else REMOVE_BIT(trigger->bits, SCRIPT_ADVANCE);

    return ifstatement;
}
#undef C_NOT
#undef C_AND
#undef C_OR


void eval_walkto( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   ROOM_INDEX_DATA *pRoom;

   one_argument( argument, buf );

   if ( (pRoom = get_room_index( atoi(buf) )) == NULL )
       return;
   ch->walkto = atoi(buf);
      SET_BIT( trigger->bits, SCRIPT_ADVANCE );
   SET_BIT(ch->act2, ACT2_WALKTO );
}
void eval_walktomob( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   MOB_INDEX_DATA  *pMob;

   one_argument( argument, buf );

   if ( (pMob = get_mob_index( atoi(buf) )) == NULL )
       return;
   ch->walkto = atoi(buf);
      SET_BIT( trigger->bits, SCRIPT_ADVANCE );
   SET_BIT(ch->act2, ACT2_WALKTO_MOB );
}

void eval_steerto( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument)
{
   char arg1[MAX_STRING_LENGTH];
   char arg2[MAX_STRING_LENGTH];

   ROOM_INDEX_DATA *pRoom;
   MOB_INDEX_DATA *pMob;

   argument = one_argument( argument, arg1 );
   one_argument( argument, arg2 );

   if ( (pRoom = get_room_index( atoi(arg2) )) == NULL )
       return;
   if ( (pMob = get_mob_index( atoi(arg1) )) == NULL )
       return;
   ch->walkto = atoi(arg2);
   ch->walkfrom = atoi(arg1);
   SET_BIT(ch->act2, ACT2_STEERING );
      SET_BIT( trigger->bits, SCRIPT_ADVANCE );
}
void eval_mob( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    LOCK_DATA *lock_list;
    LOCK_DATA *lock_next;
    
    argument = one_argument( argument, arg1 );    /* string */
    if ( arg1[0] == '%' ) strcpy( arg1, varval( arg1, ch ) );
    argument = one_argument( argument, arg2 );    /* string */
    if ( arg2[0] == '%' ) strcpy( arg2, varval( arg2, ch ) );
    argument = one_argument( argument, arg3 );    /* string */
    if ( arg3[0] == '%' ) strcpy( arg3, varval( arg3, ch ) );
    
    victim = get_char_world( ch, arg2 );

    if ( !str_cmp( arg1, "lock" ) )
    {
        int door;
        door = get_dir( arg2 );
        if ( ch->in_room->exit[door] == NULL 
          || !IS_SET(ch->in_room->exit[door]->exit_info, EX_ISDOOR) ) 
        {
            eval_halt( ch, trigger, "once" );
            return;
        }

    	    for ( lock_list = ch->in_room->exit[door]->lock ; lock_list != NULL ; lock_list = lock_next)
    	    {
		lock_next = lock_list->next;
		SET_BIT(lock_list->lock_info, LOCK_LOCKED);  
    	    }
        if ( ch->in_room->exit[door]->to_room != NULL
          && ch->in_room->exit[door]->to_room->exit[rev_dir[door]] != NULL
          && IS_SET( ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->exit_info, EX_ISDOOR ) )
    	    for ( lock_list = ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->lock ; lock_list != NULL ; lock_list = lock_next)
    	    {
		lock_next = lock_list->next;
        	SET_BIT( lock_list->lock_info, LOCK_LOCKED );
    	    }

    }
    else
    if ( !str_cmp( arg1, "unlock" ) )
    {
        int door;
        door = get_dir( arg2 );
        if ( ch->in_room->exit[door] == NULL 
          || !IS_SET(ch->in_room->exit[door]->exit_info, EX_ISDOOR) ) 
        {
            eval_halt( ch, trigger, "once" );
            return;
        }
    	    for ( lock_list = ch->in_room->exit[door]->lock ; lock_list != NULL ; lock_list = lock_next)
    	    {
		lock_next = lock_list->next;
		REMOVE_BIT(lock_list->lock_info, LOCK_LOCKED);  
    	    }
        if ( ch->in_room->exit[door]->to_room != NULL
          && ch->in_room->exit[door]->to_room->exit[rev_dir[door]] != NULL
          && IS_SET( ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->exit_info, EX_ISDOOR ) )
    	    for ( lock_list = ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->lock ; lock_list != NULL ; lock_list = lock_next)
    	    {
		lock_next = lock_list->next;
        	REMOVE_BIT( lock_list->lock_info, LOCK_LOCKED );
    	    }

    }
    else
    if ( !str_cmp( arg1, "setexit" ) )
    {
        int door;
        door = get_dir( arg2 );
        if ( ch->in_room->exit[door] == NULL )
        {
            eval_halt( ch, trigger, "once" );
            return;
        }
        if ( get_exit_flag_number( arg3 ) != EX_NONE )
        {
            TOGGLE_BIT(ch->in_room->exit[door]->exit_info, get_exit_flag_number( arg3 ));
        }

    }
    else
    if ( !str_cmp( arg1, "open" ) )
    {
        int door;
        door = get_dir( arg2 );
        if ( ch->in_room->exit[door] == NULL 
          || !IS_SET(ch->in_room->exit[door]->exit_info, EX_ISDOOR) ) 
        {
            eval_halt( ch, trigger, "once" );
            return;
        }
        REMOVE_BIT( ch->in_room->exit[door]->exit_info, EX_CLOSED );
        if ( ch->in_room->exit[door]->to_room != NULL
          && ch->in_room->exit[door]->to_room->exit[rev_dir[door]] != NULL
          && IS_SET( ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->exit_info, EX_ISDOOR ) )
        REMOVE_BIT( ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->exit_info, EX_CLOSED );
    }
    else
    if ( !str_cmp( arg1, "close" ) )
    {
        int door;
        door = get_dir( arg2 );
        if ( ch->in_room->exit[door] == NULL 
          || !IS_SET(ch->in_room->exit[door]->exit_info, EX_ISDOOR) ) 
        {
            eval_halt( ch, trigger, "once" );
            return;
        }
        SET_BIT( ch->in_room->exit[door]->exit_info, EX_CLOSED );
        if ( ch->in_room->exit[door]->to_room != NULL
          && ch->in_room->exit[door]->to_room->exit[rev_dir[door]] != NULL
          && IS_SET( ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->exit_info, EX_ISDOOR ) )
        SET_BIT( ch->in_room->exit[door]->to_room->exit[rev_dir[door]]->exit_info, EX_CLOSED );
    }
    else
    if ( !str_cmp( arg1, "trans" ) )
    {
       ROOM_INDEX_DATA *pRoom;
       
       if ( victim == NULL ||
          ( pRoom = get_room_index( atoi(arg3) ) ) == NULL )
       {
            eval_halt( ch, trigger, "once" );
            return;
       }
       char_from_room( victim );
       char_to_room( victim, pRoom );
    }
   else
    if ( !str_cmp( arg1, "stopfollower" ) )
    {
       
       
       if ( victim == NULL )
  	stop_follower( ch );
	else
       stop_follower( victim );
    }
   else
    if ( !str_cmp( arg1, "addfollower" ) )
    {
       if ( victim == NULL )
       {
            eval_halt( ch, trigger, "once" );
            return;
       }     
       do_follow( victim ,ch->name);
    }
    else
    if ( !str_cmp( arg1, "addpet" ) )
    {
       if ( victim == NULL )
       {
            eval_halt( ch, trigger, "once" );
            return;
       }     
       ch->leader = victim;
       SET_BIT(victim->plr, PLR_BOUGHT_PET);
       SET_BIT(ch->act, ACT_PET);
    }
    else
    if ( !str_cmp( arg1, "cash" ) )
    {
       int cost;

       cost = atoi(arg3);
    
       if ( victim == NULL )
       {
            eval_halt( ch, trigger, "once" );
            return;
       }     

     /*  if ( cost > 0 ) create_amount( cost, victim, NULL, NULL ); */
    }

    return;
}

void script_interpret( CHAR_DATA *ch, TRIGGER_DATA *trigger )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *cmd;
    char jumpto[MAX_STRING_LENGTH];
    bool IN_IF = FALSE;
    bool IN_JUMP = FALSE;
    buf2[0] = '\0';
    buf[0] = '\0';
    if ( !IS_NPC(ch) ) return;
    jumpto[0] = '\0';

    if ( trigger->current == NULL ) return;
    trigger->tracer++;

    cmd = one_argument_any( trigger->current->command, buf2 ); 

    while ( cmd[0] != '\0' )
    {
    cmd = next_command( cmd, buf );
      if ( IN_IF )
      {
	if (!str_cmp( buf2, "endif" ) )
	IN_IF = FALSE;
      }
      else if ( IN_JUMP )
      {
	if (!str_cmp( buf2, jumpto ) )
	  IN_JUMP = FALSE;
      }
 else if ( !str_cmp( buf2, "loop"    ) )    eval_goto   ( ch, trigger, ""  );
 else if ( !str_cmp( buf2, "let"     ) )    eval_assign ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "walkto"  ) )    eval_walkto ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "walktomob"  ) )    eval_walktomob ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "steerto"  ) )   eval_steerto ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "mob"     ) )    eval_mob    ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "ofunc"   ) )    eval_ofunc  ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "ocreate" ) )    eval_ocreate ( ch, NULL, trigger, buf );
 else if ( !str_cmp( buf2, "path"    ) )    eval_path   ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "osign"   ) )    eval_osign  ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "mfunc"   ) )    eval_mfunc  ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "msign"   ) )    eval_msign  ( ch, NULL, trigger, buf );
 else if ( !str_cmp( buf2, "wfunc"   ) )    eval_wfunc  ( ch, NULL,NULL, trigger, buf );
 else if ( !str_cmp( buf2, "wsign"   ) )    eval_wsign  ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "sfunc"   ) )    eval_sfunc  ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "jump"    ) )    eval_goto   ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "oed"     ) )    eval_oed    ( ch, trigger, buf );
 else if ( !str_cmp( buf2, "wait"     ) )    eval_wait    ( ch,NULL,NULL, trigger, buf );
 else if ( !str_cmp( buf2, "suicide" ) )
      {
           extract_char( ch, TRUE );
           ch = char_free;
           return;
      }
 else if ( !str_cmp( buf2, "halt" ) )
      {
           if (!eval_halt   ( ch, trigger, buf ))
           return;
      }
 else if ( !str_cmp( buf2, "while" )   )
      {
           if (!eval_while  ( ch, trigger, buf ))
               return;
      }
 else if ( !str_cmp( buf2, "if" )   )
      {
 	   if (!eval_if     ( ch,NULL, NULL, trigger, buf ))
	   IN_IF = TRUE;
     }
 else if ( !str_cmp( buf2, "jumpto" )   )
      {
           cmd = one_argument_any( trigger->current->command, buf2 ); 
	   strcpy( jumpto, buf );
	   IN_JUMP = TRUE;
	   IN_IF = FALSE;
     }
     else
     {
         strcat( buf2, " " );
         strcat( buf2, buf );
         eval_interp ( ch, trigger, buf2 );
     }

    cmd = one_argument_any( cmd, buf2 );
    }

    return;
}
void script_interpret_room( ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *cmd;
    bool IN_IF = FALSE;
    char jumpto[MAX_STRING_LENGTH];
    bool IN_JUMP = FALSE;
    buf2[0] = '\0';
    buf[0] = '\0';
    if ( trigger->current == NULL ) return;
    trigger->tracer++;
    jumpto[0] = '\0';

    cmd = one_argument_any( trigger->current->command, buf2 ); 

    while ( cmd[0] != '\0' )
    {
    cmd = next_command( cmd, buf );
      if ( IN_IF )
      {
	if (!str_cmp( buf2, "endif" ) )
	IN_IF = FALSE;
      }
      else if ( IN_JUMP )
      {
	if (!str_cmp( buf2, jumpto ) )
	{
	  IN_JUMP = FALSE;
	}
      }
 else if ( !str_cmp( buf2, "ocreate" ) )    eval_ocreate ( NULL, room, trigger, buf );
 else if ( !str_cmp( buf2, "msign"   ) )    eval_msign  ( NULL, room, trigger, buf );
 else if ( !str_cmp( buf2, "wfunc"   ) )    eval_wfunc  ( NULL, room, NULL, trigger, buf );
 else if ( !str_cmp( buf2, "wait"     ) )   eval_wait    ( NULL,room,NULL, trigger, buf );
 else if ( !str_cmp( buf2, "halt" ) )
      {
           if (!eval_halt   ( NULL, trigger, buf ))
           return;
      }
 else if ( !str_cmp( buf2, "while" )   )
      {
           if (!eval_while  ( NULL, trigger, buf ))
               return;
      }
 else if ( !str_cmp( buf2, "if" )   )
      {
 	   if (!eval_if     ( NULL,room, NULL, trigger, buf ))
	   IN_IF = TRUE;
     }
 else if ( !str_cmp( buf2, "jumpto" )   )
      {
           cmd = one_argument_any( trigger->current->command, buf2 ); 
	   strcpy( jumpto, buf );
	   IN_JUMP = TRUE;
	   IN_IF = FALSE;
     }
     else
     {
         strcat( buf2, " " );
         strcat( buf2, buf );
         eval_interp_room ( room, trigger, buf2 );
     }

    cmd = one_argument_any( cmd, buf2 );
    }

    return;
}
void script_interpret_obj( OBJ_DATA *obj, TRIGGER_DATA *trigger )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char *cmd;
    bool IN_IF = FALSE;
    buf2[0] = '\0';
    buf[0] = '\0';

    if ( trigger->current == NULL ) return;
    trigger->tracer++;

    cmd = one_argument_any( trigger->current->command, buf2 ); 

    while ( cmd[0] != '\0' )
    {
    cmd = next_command( cmd, buf );
      if ( IN_IF )
      {
	if (!str_cmp( buf2, "endif" ) )
	IN_IF = FALSE;
      }
 else if ( !str_cmp( buf2, "halt" ) )
      {
           if (!eval_halt   ( NULL, trigger, buf ))
           return;
      }
 else if ( !str_cmp( buf2, "while" )   )
      {
           if (!eval_while  ( NULL, trigger, buf ))
               return;
      }
 else if ( !str_cmp( buf2, "if" )   )
      {
 	   if (!eval_if     ( NULL,NULL, obj, trigger, buf ))
	   IN_IF = TRUE;
     }
 else if ( !str_cmp( buf2, "wfunc"   ) )    eval_wfunc  ( NULL, NULL,obj, trigger, buf );
 else if ( !str_cmp( buf2, "wait"     ) )   eval_wait    ( NULL,NULL,obj, trigger, buf );
     else
     {
         strcat( buf2, " " );
         strcat( buf2, buf );
         eval_interp_obj ( obj, trigger, buf2 );
     }

    cmd = one_argument_any( cmd, buf2 );
    }

    return;
}

/*
 * Called from the mobile_update() procedure in update.c
 * This should be called every mobile pulse or so.
 */
void script_update( CHAR_DATA *ch, int timetype )
{
   TRIGGER_DATA *trig;

   if ( !IS_NPC(ch) ) return;

    for ( trig = ch->triggers; trig != NULL; trig = trig->next )
    {

        if ( IS_SET(ch->act2, ACT2_HALT) 
	   || IS_SET(trig->bits, SCRIPT_HALT) 
	   || IS_SET(trig->flags, TRIG_INACTIVE) 
	   || IS_SET(trig->flags, SCRIPT_HALT_RESET) )
        {
            trig->current = NULL;
            continue;
        }

        if ( trig->waiting > 0 )
        {
            trig->waiting--;
            continue;
        }

	if ( IS_SET(trig->flags, SCRIPT_PATH ) )
	if ( ch->task[0] != '\0' && strlen(ch->task) > 0 )
	continue; 

	if ( IS_SET(trig->flags, SCRIPT_PATH ) )
	REMOVE_BIT(trig->flags, SCRIPT_PATH );

        if (  ( trig->trigger_type == timetype && trig->current == NULL ) ||
		IS_SET(trig->flags, TRIG_INSTANT ) )
        {
            trig->current = trig->script;
            trig->bits    = SCRIPT_ADVANCE;
        }

        if ( trig->current == NULL || !IS_SET(trig->bits, SCRIPT_ADVANCE) )
        {
            continue;
        }

        script_interpret( ch, trig );
        if ( ch != char_free
          && trig->current != NULL ) trig->current = trig->current->next;
         else trig->tracer = 0;
    }

    return;
}
void script_update_room( ROOM_INDEX_DATA *room, int timetype )
{
   TRIGGER_DATA *trig;

    for ( trig = room->triggers; trig != NULL; trig = trig->next )
    {

        if (IS_SET(trig->bits, SCRIPT_HALT) 
            || IS_SET(trig->flags, TRIG_INACTIVE) 
	    || IS_SET(trig->flags, SCRIPT_HALT_RESET))
        {
            trig->current = NULL;
            continue;
        }

        if ( trig->waiting > 0 )
        {
            trig->waiting--;
            continue;
        }


        if ( ( trig->trigger_type == timetype && trig->current == NULL ) ||
		IS_SET(trig->flags, TRIG_INSTANT ) )
        {
            trig->current = trig->script;
            trig->bits    = SCRIPT_ADVANCE;
        }

        if ( trig->current == NULL || !IS_SET(trig->bits, SCRIPT_ADVANCE) )
        {
            continue;
        }

        script_interpret_room( room, trig );

        if ( trig->current != NULL ) 
		trig->current = trig->current->next;
        else 
		trig->tracer = 0;
    }

    return;
}

void script_update_obj( OBJ_DATA *obj, int timetype )
{
   TRIGGER_DATA *trig;

    for ( trig = obj->triggers; trig != NULL; trig = trig->next )
    {

        if (IS_SET(trig->bits, SCRIPT_HALT) 
            || IS_SET(trig->flags, TRIG_INACTIVE) 
	    || IS_SET(trig->flags, SCRIPT_HALT_RESET ))
        {
            trig->current = NULL;
            continue;
        }

        if ( trig->waiting > 0 )
        {
            trig->waiting--;
            continue;
        }


        if ( ( trig->trigger_type == timetype && trig->current == NULL )
		|| IS_SET(trig->flags, TRIG_INSTANT ) )
        {
            trig->current = trig->script;
            trig->bits    = SCRIPT_ADVANCE;
        }

        if ( trig->current == NULL || !IS_SET(trig->bits, SCRIPT_ADVANCE) )
        {
            continue;
        }

        script_interpret_obj( obj, trig );

        if ( trig->current != NULL ) 
		trig->current = trig->current->next;
        else 
		trig->tracer = 0;
    }

    return;
}
