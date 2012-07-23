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
#include <sys/time.h>
#include <sys/msg.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include <string.h>

void do_awaymode (CHAR_DATA *ch, char *argument)
{
   if (!IS_SET(ch->tmpact,TMPACT_AWAY) && argument[0] == '\0')
     {
      send_to_char("Away is allready off.\n\rSyntax: away <MESSAGE> to turn away on.\n\rSyntax: away to turn if off or get this help.\n\r",ch);
      return;
     }
   if (argument[0] == '\0')
     {
     send_to_char ("Away mode OFF\n\r",ch);
     REMOVE_BIT (ch->tmpact,TMPACT_AWAY);
     ch->awaymessage[0] = '\0'; 
     }
   else
     {
     send_to_char ("Away mode ON\n\r",ch);
     SET_BIT (ch->tmpact,TMPACT_AWAY);
     strncpy(ch->awaymessage,argument,40);
     }
}

/* RT code to delete yourself */

void do_delet( CHAR_DATA *ch, char *argument)
{
    send_to_char("You must type the full command to delete yourself.\n\r",ch);
}

void do_delete( CHAR_DATA *ch, char *argument)
{
   char strsave[MAX_INPUT_LENGTH];
   char buf[MAX_INPUT_LENGTH];
   char buf2[MAX_INPUT_LENGTH];
   DESCRIPTOR_DATA *d;
   if (IS_NPC(ch))
	return;
  
   if (ch->pcdata->confirm_delete)
   {
	if (argument[0] != '\0')
	{
	    send_to_char("Delete status removed.\n\r",ch);
	    ch->pcdata->confirm_delete = FALSE;
	    return;
	}
	else
	{
	    sprintf(buf2, "%s has just deleted\n\r",ch->name);
	    info_channel(ch,buf2,INFOACT_IMP_LOG);
    	    sprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial(ch->name), "/",capitalize( ch->name ) ); 
	    if ( ch->home != 0 )
	    {
	        sprintf( buf, "rm -r %s%ld", SAVEHOMES_DIR, ch->home );
	        system( buf );
	    }

	    if (ch->logon_data != NULL )
     	    	strncpy( ch->logon_data->exit, "Delete", 30 );

            save_char_obj( ch );
            d = ch->desc;
            extract_char( ch, TRUE );
            if ( d != NULL )
            	close_socket( d );

	    return;
 	}
    }

    if (argument[0] != '\0')
    {
	send_to_char("Just type delete. No argument.\n\r",ch);
	return;
    }

    send_to_char("Type delete again to confirm this command.\n\r",ch);
    send_to_char("WARNING: this command is irreversible.\n\r",ch);
    send_to_char("Typing delete with an argument will undo delete status.\n\r",
	ch);
    ch->pcdata->confirm_delete = TRUE;
}

/* RT code to display channel status */

void do_channels( CHAR_DATA *ch, char *argument)
{
    /* lists all channels and their status */
    send_to_char("   channel     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);
 
    send_to_char("think         ",ch);
    if (!IS_SET(ch->comm,COMM_NOTHINK))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("helpme         ",ch);
    if (!IS_SET(ch->comm,COMM_NOHELP))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("auction        ",ch);
    if (!IS_SET(ch->comm,COMM_NOAUCTION))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    if (IS_IMMORTAL(ch))
    {
      send_to_char("pray           ",ch);
      if (!IS_SET(ch->comm,COMM_NOPRAY))
        send_to_char("ON\n\r",ch);
      else
        send_to_char("OFF\n\r",ch);
    }

    send_to_char("OOC            ",ch);
    if (!IS_SET(ch->comm,COMM_NOOOC))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    if (IS_IMMORTAL(ch))
    {
      send_to_char("god channel    ",ch);
      if(!IS_SET(ch->comm,COMM_NOWIZ))
        send_to_char("ON\n\r",ch);
      else
        send_to_char("OFF\n\r",ch);
    }
      send_to_char ("Info Channel   ",ch);
      if (IS_SET(ch->comm,COMM_INFO))
         send_to_char ("ON\n\r",ch);
      else
         send_to_char ("OFF\n\r",ch);

    send_to_char("shouts         ",ch);
    if (!IS_SET(ch->comm,COMM_DEAF))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);

    send_to_char("quiet mode     ",ch);
    if (IS_SET(ch->comm,COMM_QUIET))
      send_to_char("ON\n\r",ch);
    else
      send_to_char("OFF\n\r",ch);
   
    if (ch->lines != PAGELEN)
    {
	char buf[100];
	if (ch->lines)
	{
	    sprintf(buf,"You display %d lines of scroll.\n\r",ch->lines+2);
	    send_to_char(buf,ch);
 	}
	else
	    send_to_char("Scroll buffering is off.\n\r",ch);
    }

    if (IS_SET(ch->comm,COMM_NOSHOUT))
      send_to_char("You cannot shout.\n\r",ch);
  
    if (IS_SET(ch->comm,COMM_NOTELL))
      send_to_char("You cannot use tell.\n\r",ch);
 
    if (IS_SET(ch->comm,COMM_NOCHANNELS))
     send_to_char("You cannot use channels.\n\r",ch);

    if (IS_SET(ch->comm,COMM_NOEMOTE))
      send_to_char("You cannot show emotions.\n\r",ch);
    
}

/* RT deaf blocks out all shouts */

void do_deaf( CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_NOSHOUT))
    {
      send_to_char("The gods have taken away your ability to shout.\n\r",ch);
      return;
    }
    
   if (IS_SET(ch->comm,COMM_DEAF))
   {
     send_to_char("You can now hear shouts again.\n\r",ch);
     REMOVE_BIT(ch->comm,COMM_DEAF);
   }
   else 
   {
     send_to_char("From now on, you won't hear shouts.\n\r",ch);
     SET_BIT(ch->comm,COMM_DEAF);
   }
}

/* RT quiet blocks out all communication */

void do_quiet ( CHAR_DATA *ch, char * argument)
{
    if (IS_SET(ch->comm,COMM_QUIET))
    {
      send_to_char("Quiet mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_QUIET);
    }
   else
   {
     send_to_char("From now on, you will only hear says and emotes.\n\r",ch);
     SET_BIT(ch->comm,COMM_QUIET);
   }
}

/* RT auction rewritten in ROM style */
void do_auction( CHAR_DATA *ch, char *argument )
{
    do_auction_race(ch,argument, gsn_common );
    return;
}

void do_auction_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    char *said;

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOAUCTION))
      {
	send_to_char("Auction channel is now ON.\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOAUCTION);
      }
      else
      {
	send_to_char("Auction channel is now OFF.\n\r",ch);
	SET_BIT(ch->comm,COMM_NOAUCTION);
      }
    }
    else  /* auction message sent, turn auction on if it is off */
    {
	if (IS_SET(ch->comm,COMM_QUIET))
	{
	  send_to_char("You must turn off quiet mode first.\n\r",ch);
	  return;
	}

	if (IS_SET(ch->comm,COMM_NOCHANNELS))
	{
	  send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
	  return;

	REMOVE_BIT(ch->comm,COMM_NOAUCTION);
      }

      check_improve(ch,sn,TRUE,5);
      said=argument;
      argument = speak_lang( ch, ch, said, sn );
      sprintf( buf, "`GYou auction '%s'`w\n\r", argument );
      send_to_char( buf, ch );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected <= CON_PLAYING &&
	     d->character != ch &&
	     ( IS_IMMORTAL( victim ) || IS_IMMORTAL( ch ) || ( 
	     victim->in_room->area->world == ch->in_room->area->world ) ) &&
	     !IS_SET(victim->comm,COMM_NOAUCTION) &&
	     !IS_SET(victim->comm,COMM_QUIET) )
	{
            argument = speak_lang( ch, ch, said, sn );
	    cact_new("$n auctions '$t'",
		    ch,argument,victim,TO_VICT,POS_DEAD,"`r");
            check_improve(victim,sn,TRUE,3);
 	}
    }
  }
}

void do_think( CHAR_DATA *ch, char *argument )
{
    do_think_race( ch, argument, gsn_common );
    return;
}

void do_think_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    char *said;
 
    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOTHINK))
      {
        send_to_char("Thought channel is now ON.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOTHINK);
      }
      else
      {
        send_to_char("Thought channel is now OFF.\n\r",ch);
        SET_BIT(ch->comm,COMM_NOTHINK);
      }
    }
    else  /* think message sent, turn think on if it isn't already */
    {
        if (IS_SET(ch->comm,COMM_QUIET))
        {
          send_to_char("You must turn off quiet mode first.\n\r",ch);
          return;
        }
 
        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
          send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
 
       	}

      REMOVE_BIT(ch->comm,COMM_NOTHINK);
 
/* this is here for the lower level charactors */
      if ( ch->level < 5 )
	{
	if (ch->gos_timer > 0 )
	    {
		sprintf(buf,"Until level 5, you are restricted on the number of thoughts per tic.\n\rTry using shout.\n\r");
		send_to_char(buf,ch);
		return;
	    }
	ch->gos_timer = 1;
	}
      check_improve(ch,sn,TRUE,5);
      said=argument;
      argument = speak_lang( ch, ch, said, sn );
      if ( ch->in_room->area->world == 0 )
      	sprintf( buf, "`GYou think '%s'`w\n\r", argument );
      else if ( ch->in_room->area->world == 1 )
      	sprintf( buf, "`MYou think '%s'`w\n\r", argument );
      else if ( ch->in_room->area->world == 2 )
      	sprintf( buf, "`YYou think '%s'`w\n\r", argument );
      else 
      	sprintf( buf, "`mYou think '%s'`w\n\r", argument );

      send_to_char( buf, ch );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
        CHAR_DATA *victim;
 
        victim = d->original ? d->original : d->character;
 
        if ( d->connected <= CON_PLAYING &&
             d->character != ch &&
	     ( IS_IMMORTAL( victim ) || ( 
	     victim->in_room->area->world == ch->in_room->area->world ) ) &&
             !IS_SET(victim->comm,COMM_NOTHINK) &&
             !IS_SET(victim->comm,COMM_QUIET) )
        {
            argument = speak_lang( ch, victim, said, sn );
	    if ( ch->in_room->area->world == 0 )
		sprintf( buf, "`G" );
	    else if ( ch->in_room->area->world == 1 )
		sprintf( buf, "`M" );
	    else if ( ch->in_room->area->world == 2 )
		sprintf( buf, "`Y" );
	    else 
		sprintf( buf, "`m" );

            cact_new( "$n thinks '$t'", 
		   ch,argument, victim, TO_VICT,POS_SLEEPING, buf );
            check_improve(victim,sn,TRUE,3);
        }
      }
    }
}

/* RT ooc channel */
void do_ooc( CHAR_DATA *ch, char *argument )
{
    do_ooc_race(ch,argument, gsn_common);
    return;
} 

void do_ooc_race( CHAR_DATA *ch, char *argument, sh_int sn)
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    char *said;
 
    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOOOC))
      {
        send_to_char("OOC channel is now ON.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOOOC);
      }
      else
      {
        send_to_char("OOC channel is now OFF.\n\r",ch);
        SET_BIT(ch->comm,COMM_NOOOC);
      }
    }
    else  /* ooc sent, turn ooc on if it isn't already */
    {
        if (IS_SET(ch->comm,COMM_QUIET))
        {
          send_to_char("You must turn off quiet mode first.\n\r",ch);
          return;
        }
 
        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
          send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
	}
 
        REMOVE_BIT(ch->comm,COMM_NOOOC);
 
      check_improve(ch,sn,TRUE,5);
      said=argument;
      argument = speak_lang( ch, ch, said, sn );
      sprintf( buf, "`cYou ooc: '%s'`w\n\r", argument );
      send_to_char( buf, ch );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
        CHAR_DATA *victim;
 
        victim = d->original ? d->original : d->character;
 
        if ( d->connected <= CON_PLAYING &&
             d->character != ch &&
             !IS_SET(victim->comm,COMM_NOOOC) &&
             !IS_SET(victim->comm,COMM_QUIET) )
        {
            argument = speak_lang( ch, victim, said, sn );
 	    cact_new("$n ooc: '$t'",
	 	  ch,argument,victim,TO_VICT,POS_SLEEPING, "`c");
            check_improve(victim,sn,TRUE,3);
        }
    }
  }
}

/* RT answer channel - uses same line as questions */
void do_answer( CHAR_DATA *ch, char *argument )
{
    do_answer_race(ch,argument, gsn_common );
    return;
}

void do_answer_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
     char *said;
 
    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if (argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOOOC))
      {
        send_to_char("Q/A channel is now ON.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOOOC);
      }
      else
      {
        send_to_char("Q/A channel is now OFF.\n\r",ch);
        SET_BIT(ch->comm,COMM_NOOOC);
      }
    }
    else  /* answer sent, turn Q/A on if it isn't already */
    {
        if (IS_SET(ch->comm,COMM_QUIET))
        {
          send_to_char("You must turn off quiet mode first.\n\r",ch);
          return;
        }
 
        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
          send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
	}
 
        REMOVE_BIT(ch->comm,COMM_NOOOC);
 
      check_improve(ch,sn,TRUE,5);
      said=argument;
      argument = speak_lang( ch, ch, said, sn );
      sprintf( buf, "`yYou answer '%s'`w\n\r", argument );
      send_to_char( buf, ch );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
        CHAR_DATA *victim;
 
        victim = d->original ? d->original : d->character;
 
        if ( d->connected <= CON_PLAYING &&
             d->character != ch &&
	     ( IS_IMMORTAL( victim ) || IS_IMMORTAL( ch ) || ( 
	     victim->in_room->area->world == ch->in_room->area->world ) ) &&
             !IS_SET(victim->comm,COMM_NOOOC) &&
             !IS_SET(victim->comm,COMM_QUIET) )
        {
            argument = speak_lang( ch, victim, said, sn );
	    cact_new("$n answers '$t'",
		  ch,argument,victim,TO_VICT,POS_SLEEPING,"`c");
            check_improve(victim,sn,TRUE,3);
        }
    }
  }
}


/* RT pray channel */
void do_pray( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
 
    if (argument[0] == '\0' && IS_IMMORTAL(ch) )
    {
      if (IS_SET(ch->comm,COMM_NOPRAY))
      {
        send_to_char("Prayer channel is now ON.\n\r",ch);
        REMOVE_BIT(ch->comm,COMM_NOPRAY);
      }
      else
      {
        send_to_char("Prayer channel is now OFF.\n\r",ch);
        SET_BIT(ch->comm,COMM_NOPRAY);
      }
    }
    else if (argument[0] == '\0' )
    {
    	send_to_char("Yes but what do you want to pray??\n\r",ch);
	return;
    }
    else  /* prayer sent, turn pray on if it isn't already */
    {
 
        if (IS_SET(ch->comm,COMM_NOCHANNELS))
        {
          send_to_char("The gods have revoked your channel priviliges.\n\r",ch);
          return;
	}
 
        REMOVE_BIT(ch->comm,COMM_NOPRAY);
 
      sprintf( buf, "`CYou pray: '%s'`w\n\r", argument );
      send_to_char( buf, ch );
      sprintf( buf, "$n prays: '%s'", argument );
      for ( d = descriptor_list; d != NULL; d = d->next )
      {
        CHAR_DATA *victim;
 
        victim = d->original ? d->original : d->character;
 
        if ( d->connected <= CON_PLAYING &&
             d->character != ch &&
	     IS_IMMORTAL(d->character) &&
             !IS_SET(victim->comm,COMM_NOPRAY) )
        {
	    cact_new("$n prays: '$t'",
		    ch,argument,d->character,TO_VICT,POS_DEAD,"`w");
        }
      }
    }
}

void do_immtalk( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOWIZ))
      {
	send_to_char("Immortal channel is now ON\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOWIZ);
      }
      else
      {
	send_to_char("Immortal channel is now OFF\n\r",ch);
	SET_BIT(ch->comm,COMM_NOWIZ);
      } 
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOWIZ);

    cact_new("$n: $t",ch,argument,NULL,TO_CHAR,POS_DEAD,"`Y");
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected <= CON_PLAYING && 
	     IS_IMMORTAL(d->character) && 
             !IS_SET(d->character->comm,COMM_NOWIZ) )
	{
	    cact_new("$n: $t",ch,argument,d->character,TO_VICT,POS_DEAD,"`Y");
	}
    }

    return;
}

void do_helpme( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    bool found;

    if ( argument[0] == '\0' )
    {
      if (IS_SET(ch->comm,COMM_NOHELP))
      {
	send_to_char("Helpme channel is now ON\n\r",ch);
	REMOVE_BIT(ch->comm,COMM_NOHELP);
      }
      else
      {
	send_to_char("Helpme channel is now OFF\n\r",ch);
	SET_BIT(ch->comm,COMM_NOHELP);
      } 
      return;
    }

    REMOVE_BIT(ch->comm,COMM_NOHELP);

    for ( d = descriptor_list; d != NULL ; d = d->next )
    {
	if ( d->connected <= CON_PLAYING
	  && can_see( d->character, ch )
	  && IS_SET( d->character->plr2, PLR_PLAYER_HELPER ) )
	    found = TRUE;
    }

    sprintf( buf, "$n: %s", argument );
    cact_new("$n: $t",ch,argument,NULL,TO_CHAR,POS_DEAD,"`G");
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected <= CON_PLAYING && 
	     (!IS_SET(d->character->comm, COMM_NOHELP) 
	     && IS_SET(d->character->plr2, PLR_PLAYER_HELPER) ) )
	{
	    cact_new("$n: $t",ch,argument,d->character,TO_VICT,POS_DEAD,"`G");
	}
    }

    return;
}



bool is_language( int sn )
{
    if ( sn == gsn_belegfea 
      || sn == gsn_mirros
      || sn == gsn_common
      || sn == gsn_aranna
      || sn == gsn_iavartor
      || sn == gsn_milrochdil
      || sn == gsn_tarnanthalion
      || sn == gsn_manlhach
      || sn == gsn_nenkhilmen
      || sn == gsn_sulkano )
	return TRUE;

    return FALSE;
}

char * say_drunk( char * argument )
{
    char buf  [MAX_STRING_LENGTH];
    int iSyl;
    int length;

    struct syl_type
    {
	char *	old;
	char *	newsyl;
    };

/* Muhalo this is the table you need to edit */

    static const struct syl_type syl_table[] =
    {
	{ " ",		" "		},
	{ "ar",		"abra"		},
	{ "au",		"kada"		},
	{ "bless",	"fido"		},
	{ "blind",	"nose"		},
	{ "bur",	"mosa"		},
	{ "cu",		"judi"		},
	{ "de",		"oculo"		},
	{ "en",		"unso"		},
	{ "light",	"dies"		},
	{ "lo",		"hi"		},
	{ "mor",	"zak"		},
	{ "move",	"sido"		},
	{ "ness",	"lacri"		},
	{ "ning",	"illa"		},
	{ "per",	"duda"		},
	{ "ra",		"gru"		},
	{ "fresh",	"ima"		},
	{ "re",		"candus"	},
	{ "son",	"sabru"		},
	{ "tect",	"infra"		},
	{ "tri",	"cula"		},
	{ "ven",	"nofo"		},
	{ "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
	{ "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
	{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
	{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
	{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
	{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
	{ "y", "l" }, { "z", "k" },
	{ "", "" }
    };

    buf[0]	= '\0';
    for ( ; *argument != '\0'; argument += length )
    {
	for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, argument ) )
	    {
		strcat( buf, syl_table[iSyl].newsyl );
		break;
	    }
	}

	if ( length == 0 )
	{
	    strncat( buf, argument, 1 );
	    length = 1;
	}
    }

    return  str_dup(buf);
}

char * say_cold( char * argument )
{
    char buf  [MAX_STRING_LENGTH];
    int iSyl;
    int length;

    struct syl_type
    {
	char *	old;
	char *	newsyl;
    };

    static const struct syl_type syl_table[] =
    {
	{ " ",		" "		},
	{ "ar",		"abra"		},
	{ "au",		"kada"		},
	{ "bless",	"fido"		},
	{ "blind",	"nose"		},
	{ "bur",	"mosa"		},
	{ "cu",		"judi"		},
	{ "de",		"oculo"		},
	{ "en",		"unso"		},
	{ "light",	"dies"		},
	{ "lo",		"hi"		},
	{ "mor",	"zak"		},
	{ "move",	"sido"		},
	{ "ness",	"lacri"		},
	{ "ning",	"illa"		},
	{ "per",	"duda"		},
	{ "ra",		"gru"		},
	{ "fresh",	"ima"		},
	{ "re",		"candus"	},
	{ "son",	"sabru"		},
	{ "tect",	"infra"		},
	{ "tri",	"cula"		},
	{ "ven",	"nofo"		},
	{ "a", "a" }, { "b", "b" }, { "c", "q" }, { "d", "e" },
	{ "e", "z" }, { "f", "y" }, { "g", "o" }, { "h", "p" },
	{ "i", "u" }, { "j", "y" }, { "k", "t" }, { "l", "r" },
	{ "m", "w" }, { "n", "i" }, { "o", "a" }, { "p", "s" },
	{ "q", "d" }, { "r", "f" }, { "s", "g" }, { "t", "h" },
	{ "u", "j" }, { "v", "z" }, { "w", "x" }, { "x", "n" },
	{ "y", "l" }, { "z", "k" },
	{ "", "" }
    };

    buf[0]	= '\0';
    for ( ; *argument != '\0'; argument += length )
    {
	for ( iSyl = 0; (length = strlen(syl_table[iSyl].old)) != 0; iSyl++ )
	{
	    if ( !str_prefix( syl_table[iSyl].old, argument ) )
	    {
		strcat( buf, syl_table[iSyl].newsyl );
		break;
	    }
	}

	if ( length == 0 )
	{
	    strncat( buf, argument, 1 );
	    length = 1;
	}
    }

    return str_dup( buf );
}

char * make_language( char * arg, int chance )
{
    char buf[MAX_INPUT_LENGTH];
    int num;
    bool understood;

    understood = ( number_percent() < chance );

    buf[0] = '\0';
    while ( *arg != '\0' )
    {
	if ( *arg == ' ' )
	{
  	    strcat(buf," ");
	    understood = ( number_percent() < chance );
	}
	else if ( understood || LOWER( *arg ) < 'a' || LOWER( *arg ) >'z')
	    sprintf(buf,"%s%c",buf,*arg);
	else 
	{
	    for ( num = number_range( -1, 3 ) ; num > 0 ; num-- )
	    	sprintf(buf,"%s%c",buf,number_range( 0, 25 )+ ( LOWER( *arg ) == *arg ? 'a' : 'A' ) );
	}
	arg++;
    }
    return  str_dup(buf);
}

char * speak_lang( CHAR_DATA * ch, CHAR_DATA * victim, char * argument, sh_int sn )
{
    char buf[MAX_INPUT_LENGTH];
    char * arg;
    if ( !is_language( sn ) )
    {
	sprintf(buf,"Bad language spoken: %d ", sn );
	bug( buf, 0 );
	sn = gsn_common;
    }
    if (!ch) 
    {
	bug( "Speak Lang, NULL ch", 0 );
	return "";
    }

    if ( ( IS_IMMORTAL( ch ) && IS_SET( ch->plr, PLR_HOLYLIGHT) ) 
      || ( IS_IMMORTAL( victim ) && IS_SET( victim->plr, PLR_HOLYLIGHT ) )
      || is_name( victim->name, ch->mate )
      || IS_AFFECTED( ch, AFF_TONGUES ) || IS_AFFECTED( victim, AFF_TONGUES ) )
	return argument;

    arg = argument;
    if ( ch != victim )
    {
	if ( get_skill( ch, sn ) > 1 && get_skill( victim, sn ) > 1 )
            arg = make_language( arg, 
	           ( get_skill( ch, sn ) + get_skill( victim, sn ) ) / 2 + get_curr_stat( victim, STAT_INT ) );
	else
	    arg = make_language( arg, 0 );
    }
/*
    if ( !IS_NPC( ch ) && ch->pcdata->condition[ COND_DRUNK ] > 10 )
	arg = say_drunk( arg );
    else if ( !IS_NPC( ch ) && IS_AFFECTED( ch, AFF_COLD ) )
	arg = say_cold( arg );
*/
    return arg;
}

void do_language( CHAR_DATA *ch, char *argument, int sn )
{
    char arg[MAX_INPUT_LENGTH];

    if ( *argument == '\'' || *argument =='"' )
    {
	do_say_race(ch,++argument,sn);
        return;
    }

    one_argument(argument,arg);

    if (strlen(argument) == 0)
    {
	send_to_char("What do you want to say?",ch);
        return;
    }

    switch( arg[0] )
    {
    case 'y':
      if ( !strncmp( arg, "yell", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_yell_race(ch,argument,sn);
	break;
      }
    case 'o':
      if ( !strncmp( arg, "ooc", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_ooc_race(ch,argument,sn);
	break;
      }
    case 'r':
      if ( !strncmp( arg, "reply", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_reply_race(ch,argument,sn);
	break;
      }
    case 'i':
      if ( !strncmp( arg, "immtalk", strlen(arg) ) && IS_IMMORTAL(ch) )
      {
  	argument=one_argument(argument,arg);
	do_immtalk(ch,argument);
	break;
      }
    case ':':
      if ( arg[0]==':' && IS_IMMORTAL(ch))
      {
	do_immtalk(ch,++argument);
	break;
      }
    case ';':
      if ( arg[0]==';' )
      {
	do_gtell_race(ch,++argument,sn);
	break;
      }
    case 'p':
      if ( !strncmp( arg, "pray", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_pray(ch,argument);
	break;
      }
    case 't':
      if ( !strncmp( arg, "tell", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_tell_race(ch,argument,sn);
	break;
      }
    case 's':
      if ( !strncmp( arg, "shout", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_shout_race(ch,argument,sn);
	break;
      }
    case 'g':
      if ( !strncmp( arg, "think", strlen(arg) )&& ch->level > 4 )
      {
  	argument=one_argument(argument,arg);
	do_think_race(ch,argument,sn);
	break;
      }
      if ( !strncmp( arg, "gtell", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_gtell_race(ch,++argument,sn);
	break;
      }
    case '.':
      if ( arg[0]=='.' && ch->level > 4 )
      {
	do_think_race(ch,++argument,sn);
	break;
      }
    case 'c':
      if ( !strncmp( arg, "clan", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_clan_race(ch,argument,sn);
	break;
      }
    case 'a':
      if ( !strncmp( arg, "answer", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_answer_race(ch,argument,sn);
	break;
      }
      if ( !strncmp( arg, "auction", strlen(arg) ))
      {
  	argument=one_argument(argument,arg);
	do_auction_race(ch,argument,sn);
	break;
      }
    default:
        if ( !strncmp( arg, "say", strlen(arg) ))
    	  argument=one_argument(argument,arg);
 	do_say_race(ch,argument,sn);
	break;
    }
    return;
}

void do_say( CHAR_DATA *ch, char *argument )
{
    do_say_race(ch,argument, gsn_common );
    return;
}

void do_say_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *said;
    char c;
    int i;
    bool trig;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Say what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    for ( i = strlen( argument ) - 1 ; i < 0 && argument[ i ] == ' ' ; i-- );
    c = argument[ i ];

    said=argument;
    argument = speak_lang( ch, ch, said, sn );
    sprintf( buf, "`CYou %s '$T'`w", ( c == '!' ? "exclaim" : ( c == '?' ? "ask" : "say" ) ) );
    act( buf, ch, NULL, argument, TO_CHAR );
    check_improve(ch,sn,TRUE,5);

    for ( victim=ch->in_room->people ; victim != NULL ;victim=victim->next_in_room)
    {
    	for ( obj = victim->carrying; obj != NULL; obj = obj->next_content )
    	{ 
	    trig = do_trigger( obj, ch, NULL, argument, TRIG_SAY, TRIG_TYPE_OBJ);
    	}
        if (victim == ch )
	   continue;

	argument = speak_lang( ch, victim, said, sn );
	sprintf( buf, "`C$n %s '$t'`w", ( c == '!' ? "exclaims" : ( c == '?' ? "asks" : "says" ) ) );
	act( buf,ch, argument, victim ,TO_VICT );
        check_improve(victim,sn,TRUE,3);

	trig = do_trigger( victim, ch, NULL, argument, TRIG_SAY, TRIG_TYPE_MOB);
    }
    trig = do_trigger( ch->in_room, ch, NULL, argument, TRIG_SAY, TRIG_TYPE_ROOM);
    for ( obj = ch->in_room->contents; obj != NULL; obj = obj->next_content )
    { 
	trig = do_trigger( obj, ch, NULL,argument, TRIG_SAY, TRIG_TYPE_OBJ);
    }
    send_to_char("`w",ch);
    return;
}


void do_shout( CHAR_DATA *ch, char *argument )
{
    do_shout_race(ch,argument, gsn_common );
    return;
}

void do_shout_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    DESCRIPTOR_DATA *d;
    char *said;

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOSHOUT) )
    {
	send_to_char( "You can't shout.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_DEAF))
    {
	send_to_char( "Deaf people can't shout.\n\r",ch);
        return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Shout what?\n\r", ch );
	return;
    }

    check_improve(ch,sn,TRUE,5);
    said=argument;
    argument = speak_lang( ch, ch, said, sn );	
    cact( "You shout '$T'", ch, NULL, argument, TO_CHAR, "`g" );
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected <= CON_PLAYING &&
	     d->character != ch &&
	     d->character->in_room->area->world == ch->in_room->area->world &&
	     !IS_SET(victim->comm, COMM_DEAF) &&
	     !IS_SET(victim->comm, COMM_QUIET) ) 
	{
            argument = speak_lang( ch, victim, said, sn );	
	    cact("$n shouts '$t'",ch,argument,victim,TO_VICT, "`g" );
            check_improve(victim,sn,TRUE,3);
	}
    }
    return;
}

void do_clan( CHAR_DATA *ch, char *argument )
{
    do_clan_race(ch,argument, gsn_common );
    return;
}

void do_clan_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    DESCRIPTOR_DATA *d;
    char *said;

    if ( ch->pcdata->pcclan == NULL )
    {
	send_to_char("You must be in a clan to use the clan channel\n\r",ch);
	return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_DEAF))
    {
	send_to_char( "Deaf people can't use the clan channel.\n\r",ch);
        return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your clan what?\n\r", ch );
	return;
    }

    said=argument;
    argument = speak_lang( ch, ch, said, sn );
    cact("Clan- $n: $t",ch,argument,NULL,TO_CHAR, "`g" );
    check_improve(ch,sn,TRUE,5);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	CHAR_DATA *victim;

	victim = d->original ? d->original : d->character;

	if ( d->connected <= CON_PLAYING &&
	     d->character != ch &&
	     d->character->pcdata->pcclan->clan == ch->pcdata->pcclan->clan &&
	     !IS_SET(victim->comm, COMM_DEAF) &&
	     !IS_SET(victim->comm, COMM_QUIET) ) 
	{

      	    argument = speak_lang( ch, ch, said, sn );
    	    cact("Clan- $n: $t",ch,argument,d->character,TO_VICT, "`g" );
      	    check_improve(victim,sn,TRUE,3);
	}
    }

    return;
}

void do_tell( CHAR_DATA *ch, char *argument )
{
    do_tell_race(ch,argument, gsn_common );
    return;
}

void do_mate( CHAR_DATA *ch, char *argument)
{
    char *said;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }


    if ( ch->pcdata != NULL && ( ch->pcdata->hand != NULL
      || ch->pcdata->deck != NULL
      || ch->pcdata->discarded != NULL
      || ch->pcdata->played != NULL ) )
    {
	send_to_char( "No private messages while playing cards.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char( "You must turn off quiet mode first.\n\r", ch);
	return;
    }


    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your mate what?\n\r", ch );
	return;
    }

    /*
     * Can tell to PC's anywhere, but NPC's only in same room.
     * -- Furey
     */
    if ( strlen(ch->mate) == 0 )
    {
	send_to_char( "Your not married!\n\r", ch );
	return;
    }
	
    if ( ( victim = get_char_world( ch, ch->mate ) ) == NULL
    || ( IS_NPC(victim) && victim->in_room != ch->in_room ) )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
            cact("$N seems to have misplaced $S link...try again later.",
	    ch,NULL,victim,TO_CHAR, "`R");
	return;
    }

    if ( !IS_AWAKE(victim) && !IS_IMMORTAL( ch ) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }
  
    if ( IS_SET(victim->comm,COMM_QUIET) )
    {
	cact( "$E is not receiving you!.", ch, 0, victim, TO_CHAR, "`R" );
  	return;
    }
    if ( IS_SET(victim->comm,COMM_NO_REC_TELL) )
    {
	cact( "$E is not receiving tells!.", ch, 0, victim, TO_CHAR, "`R" );
  	return;
    }

    if (IS_SET(victim->tmpact,TMPACT_AWAY))
         {sprintf (buf,"%s is Away => %s\n\rThey may miss you.\n\r",victim->name,victim->awaymessage);
          cact(buf,ch,NULL,NULL,TO_CHAR,"`R");
          send_to_char ("\a",victim);
         }

    said = argument;
    cact( "Beloved-> '$t'", ch, argument, victim, TO_CHAR, "`B");
    cact_new("Beloved $n-> '$t'",ch,argument,victim,TO_VICT,POS_DEAD, "`B" );
    return;
}

void do_tell_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    char arg[MAX_INPUT_LENGTH];
    char *said;
    CHAR_DATA *victim;
    char buf[MAX_STRING_LENGTH];
    char c;
    int i;
    bool trig;

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if ( ch->pcdata != NULL && ( ch->pcdata->hand != NULL
      || ch->pcdata->deck != NULL
      || ch->pcdata->discarded != NULL
      || ch->pcdata->played != NULL ) )
    {
	send_to_char( "No private messages while playing cards.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOTELL) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_QUIET) )
    {
	send_to_char( "You must turn off quiet mode first.\n\r", ch);
	return;
    }

    argument = one_argument( argument, arg );

    for ( i = strlen( argument ) - 1 ; i < 0 && argument[ i ] == ' ' ; i-- );
    c = argument[ i ];

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Tell whom what?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
            cact("$N seems to have misplaced $S link...try again later.",
	    ch,NULL,victim,TO_CHAR, "`R");
	return;
    }

    if ( !IS_IMMORTAL( ch ) && !IS_IMMORTAL( victim ) && victim->in_room->area->world != ch->in_room->area->world )
    {
	send_to_char( "They aren't here, perhaps another means of communication is needed.\n\r", ch );
	return;
    }

    if ( !IS_AWAKE(victim) && !IS_IMMORTAL( ch ) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }
  
    if ( IS_SET(victim->comm,COMM_QUIET) )
    {
	cact( "$E is not receiving tells.", ch, 0, victim, TO_CHAR, "`R" );
  	return;
    }

    if ( IS_SET(victim->comm, COMM_NO_REC_TELL) )
    {
	cact( "$E is not receiving tells.", ch, 0, victim, TO_CHAR, "`R" );
	return;
    }

    victim->reply	= ch;
    ch->reply		= victim;
    if (IS_SET(victim->tmpact,TMPACT_AWAY))
         {sprintf (buf,"%s is Away => %s\n\rThey may miss your tell.\n\r",victim->name,victim->awaymessage);
          cact(buf,ch,NULL,NULL,TO_CHAR,"`R");
          send_to_char ("\a",victim);
         }

    check_improve(ch,sn,TRUE,5);
    check_improve(victim,sn,TRUE,3);
    said = argument;
    argument = speak_lang( ch, ch, said, sn );
    sprintf( buf, "`GYou %s $N '$t'`w",( c == '!' ? "exclaim to" : ( c == '?' ? "ask" : "tell" ) ) );
    act( buf, ch, argument, victim, TO_CHAR );
    argument = speak_lang( ch, victim, said, sn );
    sprintf( buf, "`G$n %s you '$t'`w", ( c == '!' ? "exclaims to" : ( c == '?' ? "asks" : "tells" ) ) );
    act_tell( buf, ch, argument, victim, TO_VICT );

    trig = do_trigger( victim, ch, NULL, argument, TRIG_TELL, TRIG_TYPE_MOB);
    return;
}

void do_reply( CHAR_DATA *ch, char *argument )
{
    do_reply_race(ch,argument, gsn_common );
    return;
}

void do_reply_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *said;
    char c;
    int i;
    bool trig;

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }


    if ( ch->pcdata != NULL && ( ch->pcdata->hand != NULL
      || ch->pcdata->deck != NULL
      || ch->pcdata->discarded != NULL
      || ch->pcdata->played != NULL ) )
    {
	send_to_char( "No private messages while playing cards.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOTELL) )
    {
	send_to_char( "Your message didn't get through.\n\r", ch );
	return;
    }

    if ( ( victim = ch->reply ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL && !IS_NPC(victim))
    {
        cact("$N seems to have misplaced $S link...try again later.",
            ch,NULL,victim,TO_CHAR, "`R");
        return;
    }

    if ( !IS_IMMORTAL(ch) && !IS_AWAKE(victim) )
    {
	act( "$E can't hear you.", ch, 0, victim, TO_CHAR );
	return;
    }

    if ( IS_SET(victim->comm,COMM_QUIET) && !IS_IMMORTAL(ch))
    {
        act( "$E is not receiving tells.", ch, 0, victim, TO_CHAR );
        return;
    }

    if ( IS_SET(ch->comm, COMM_NO_REC_TELL) )
    {
	cact( "$E is not receiving tells.", ch, 0, victim, TO_CHAR, "`R" );
	return;
    }

    for ( i = strlen( argument ) - 1 ; i < 0 && argument[ i ] == ' ' ; i-- );
    c = argument[ i ];

    check_improve(ch,sn,TRUE,5);
    said = argument;
    argument = speak_lang( ch, ch, said, sn );
    sprintf( buf, "`GYou %s $N '$t'`w",( c == '!' ? "exclaim to" : ( c == '?' ? "ask" : "tell" ) ) );
    act( buf, ch, argument, victim, TO_CHAR );
    argument = speak_lang( ch, victim, said, sn );
    sprintf( buf, "`G$n %s you '$t'`w", ( c == '!' ? "exclaims to" : ( c == '?' ? "asks" : "tells" ) ) );
    cact_new( buf,ch,argument,victim,TO_VICT,POS_DEAD, "`R" );
    check_improve(victim,sn,TRUE,3);
    victim->reply	= ch;
    trig = do_trigger( victim, ch, NULL, argument, TRIG_TELL, TRIG_TYPE_MOB);
    return;
}

void do_yell( CHAR_DATA *ch, char *argument )
{
    do_yell_race(ch,argument, gsn_common );
    return;
}

void do_yell_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    DESCRIPTOR_DATA *d;
    char *said;

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "You are unable to speak.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->comm, COMM_NOSHOUT) )
    {
        send_to_char( "You can't yell.\n\r", ch );
        return;
    }
 
    if ( argument[0] == '\0' )
    {
	send_to_char( "Yell what?\n\r", ch );
	return;
    }

    check_improve(ch,sn,TRUE,5);
    said=argument;
    argument = speak_lang( ch, ch, said, sn );
    cact("You yell '$t'",ch,argument,NULL,TO_CHAR,"`Y");
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->connected <= CON_PLAYING
	&&   d->character != ch
	&&   d->character->in_room != NULL
	&&   d->character->in_room->area == ch->in_room->area 
        &&   !IS_SET(d->character->comm,COMM_QUIET) )
	{
            argument = speak_lang( ch, d->character, said, sn );
	    cact("$n yells '$t'",ch,argument,d->character,TO_VICT,"`Y");
            check_improve(d->character,sn,TRUE,3);
	}
    }

    return;
}


void do_emote( CHAR_DATA *ch, char *argument )
{

    if ( !IS_NPC(ch) && ( IS_SET(ch->comm, COMM_NOEMOTE) ) )
    {
	send_to_char( "You can't show your emotions.\n\r", ch );
	return;
    }

    if ( argument[0] == '\0' )
    {
	send_to_char( "Emote what?\n\r", ch );
	return;
    }

    {
    	cact( "$n $T", ch, NULL, argument, TO_ROOM, "`C" );
    	cact( "$n $T", ch, NULL, argument, TO_CHAR, "`C" );
    }
    return;
}

void do_bug( CHAR_DATA *ch, char *argument )
{
    append_file( ch, BUG_FILE, argument );
    send_to_char( "Bug logged.\n\r", ch );
    return;
}

void do_idea( CHAR_DATA *ch, char *argument )
{
    append_file( ch, IDEA_FILE, argument );
    send_to_char( "Idea logged. This is NOT an identify command.\n\r", ch );
    return;
}



void do_typo( CHAR_DATA *ch, char *argument )
{
    append_file( ch, TYPO_FILE, argument );
    send_to_char( "Typo logged.\n\r", ch );
    return;
}

void auto_rent (CHAR_DATA *ch)
{ DESCRIPTOR_DATA *d;
  char buf[MAX_STRING_LENGTH];

  sprintf(buf,"%s has been Autorented.",ch->name);
  log_string (buf);
  info_channel( ch, buf, INFOACT_RENT); 
  /*item_save_charge (ch);*/ 
  do_recall(ch, "");

  if (ch->logon_data != NULL )
	strncpy( ch->logon_data->exit, "Auto-Rent", 30 );

  save_char_obj(ch);
  d = ch->desc;
  extract_char (ch,TRUE);
  if (d != NULL )
    close_socket (d);
  return;
}

void do_rent( CHAR_DATA *ch, char *argument )
{  
    char buf[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    OBJ_DATA *obj;
    SPECPRO_LIST *s;

    if ( ch->level < 2 )
    {
     send_to_char ("You must be level 2 in order to rent. Or delete your character\n\r",ch); 
     return;
    }

    for ( s = ch->in_room->spro; s != NULL && str_cmp("rent",s->sp->trigger) ; s = s->next );

    if ( (obj=get_obj_carry_vnum(ch,OBJ_VNUM_TENT) ) && s == NULL )
    {
    	extract_obj(obj);
    }
    else 
     	send_to_char ("Rent is free! We look forward to your return!\n\r",ch); 

     act ("$n has left the game.",ch,NULL,ch,TO_ROOM);
     sprintf (buf,"%s has rented.",ch->name);
     log_string(buf);
     info_channel( ch, buf, INFOACT_RENT); 

     if (ch->logon_data != NULL )
     	strncpy( ch->logon_data->exit, "Rent", 30 );

     save_char_obj(ch);
     d = ch->desc;
     extract_char (ch, TRUE);
     if (d != NULL) close_socket(d); 
     return;          
}



void do_qui( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to QUIT, you have to spell it out.\n\r", ch );
    return;
}

void do_quit( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) )
	return;


    if (!IS_IMMORTAL(ch) && !ch->home )
       {send_to_char ("You must rent at the hotel or delete your character.\n\r",ch);
        return;
       }
    else if (ch->in_room->vnum != ch->home && !IS_IMMORTAL(ch) )
       {send_to_char ("You must be home first.\n\r",ch);
        return;
       }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "No way! You are fighting.\n\r", ch );
	return;
    }

    if ( ch->position  < POS_STUNNED  )
    {
	send_to_char( "You're not DEAD yet.\n\r", ch );
	return;
    }

    if (!IS_IMMORTAL(ch))
       {send_to_char( "Come dream with us again soon.\n\r",ch);}
    else
       {send_to_char( "We will try to survive without you, but alas...it will be hard.\n\r",ch);}

    act( "$n has left the game.", ch, NULL, NULL, TO_ROOM );
    sprintf( buf, "%s has quit.", ch->name );
    log_string( buf );
    info_channel ( ch, buf,INFOACT_QUIT );
    if (ch->logon_data != NULL )
    {
	if ( ch->logon_data->exit[0] != '\0' )
	    strncpy( ch->logon_data->exit, "Quit", 30 );
    }
    /*
     * After extract_char the ch is no longer valid!
     */
    save_char_obj( ch );
    d = ch->desc;
    extract_char( ch, TRUE );
    if ( d != NULL )
	close_socket( d );

    return;
}

void do_save( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC( ch ) )
	return;

    if ( ch->level < 2 )
    {
	send_to_char( "You must be level 2 in order to save your charactor.\n\r", ch );
	return;
    }

    save_char_obj( ch );
    send_to_char( "Saving...There will be a small delay while your charactor is stored.\n\r", ch );

    if ( !IS_IMMORTAL( ch ) )
    	WAIT_STATE(ch,5 * PULSE_VIOLENCE);

    return;
}

void do_follow( CHAR_DATA *ch, char *argument )
{
/* RT changed to allow unlimited following and follow the NOFOLLOW rules */
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Follow whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CONTROL_MIND) && ch->master != NULL )
    {
	act( "But you'd rather follow $N!", ch, NULL, ch->master, TO_CHAR );
	return;
    }

    if ( victim == ch )
    {
	if ( ch->master == NULL )
	{
	    send_to_char( "You already follow yourself.\n\r", ch );
	    return;
	}
	if ( ch->group )
		do_defect( ch, "" );
	stop_follower(ch);
	return;
    }

    if ( IS_SET(victim->plr,PLR_NOFOLLOW) && !IS_IMMORTAL(ch) ) 
    {
	act("$N doesn't seem to want any followers.\n\r",
             ch,NULL,victim, TO_CHAR);
        return;
    }

    if ( IS_NPC( ch ) )
        REMOVE_BIT(ch->plr,PLR_NOFOLLOW);
    
    if ( ch->master != NULL )
	stop_follower( ch );

    add_follower( ch, victim );
    return;
}

/* nukes controlled monsters and pets */
void nuke_pets( CHAR_DATA *ch )
{    
    CHAR_DATA *pet;
    CHAR_DATA *mount;

    if ((pet = ch->pet) != NULL)
    {
    	stop_follower(pet);
    	if (pet->in_room != NULL)
    	    act("$N slowly fades away.",ch,NULL,pet,TO_NOTVICT);
    	extract_char(pet,TRUE);
    }
    if ((mount = ch->mount) != NULL)
    {
    	stop_follower(mount);
    	if (mount->in_room != NULL)
    	    act("$N slowly fades away.",ch,NULL,mount,TO_NOTVICT);
    	extract_char(mount,TRUE);
    }

    ch->pet = NULL;
    ch->mount = NULL;
    return;
}

void add_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    CHAR_DATA *trav_ch;
    char buf[MAX_STRING_LENGTH];
    
    if ( ch->master != NULL )
    {
	sprintf( buf, "Add_follower: non-null master : %s", ch->name );
	bug( buf, 0 );
	return;
    }

    ch->master        = master;

    if( master->follower == NULL )
    {
	master->follower = ch;
	ch->next_follower = NULL;
    }
    else
    {
	for(trav_ch=master->follower;
	    trav_ch->next_follower;
	    trav_ch=trav_ch->next_follower );
	
	trav_ch->next_follower = ch;
	ch->next_follower = NULL;
    }

    if ( can_see( master, ch ) )
	act( "$n now follows you.", ch, NULL, master, TO_VICT );

    act( "You now follow $N.",  ch, NULL, master, TO_CHAR );

    if( master->group && !ch->group )
    {
	if( IS_SET(master->group->flags, GROUP_IS_PUBLIC ) )
	{
    	if ( ch->level - master->level < -10
    	||   ch->level - master->level >  10 )
    	{
		act( "$N cannot join $n's group.",     ch, NULL, master, TO_NOTVICT );
		act( "You cannot join $n's group.",    ch, NULL, master, TO_VICT    );
		act( "$N cannot join your group.",     ch, NULL, master, TO_CHAR    );
		return;
    	}
	    add_to_group( ch, master );
	}
    }
    return;
}

void remove_follower( CHAR_DATA *ch, CHAR_DATA *master )
{
    CHAR_DATA *trav_ch;

    if( master->follower == ch )
	master->follower = ch->next_follower;
    else
    {
	for(trav_ch=master->follower;trav_ch->next_follower != ch;
	    trav_ch=trav_ch->next_follower);
	
	trav_ch->next_follower = ch->next_follower;
    }
}

void stop_follower( CHAR_DATA *ch )
{
    if ( ch->master == NULL )
    {
	bug( "Stop_follower: null master.", 0 );
	return;
    }

    if ( IS_AFFECTED(ch, AFF_CONTROL_MIND) )
    {
	REMOVE_BIT( ch->affected_by, AFF_CONTROL_MIND );
	affect_strip( ch, gsn_control_mind );
    }
    if ( IS_AFFECTED( ch, AFF_MOUNTED ) )
    {
	REMOVE_BIT(ch->affected_by, AFF_MOUNTED);
    }
    if ( can_see( ch->master, ch ) && ch->in_room != NULL)
    {
	act( "$n stops following you.",     ch, NULL, ch->master, TO_VICT    );
    	act( "You stop following $N.",      ch, NULL, ch->master, TO_CHAR    );
    }
    
    remove_follower( ch, ch->master );

    ch->master = NULL;
    return;
}

void die_to_followers( CHAR_DATA *ch )
{
    CHAR_DATA *fch;
    
    if( ch->follower == NULL )
	return;
    
    for(fch=ch->follower;fch;fch=fch->next_follower)
    {
	stop_follower( fch );
    }
}

void do_found( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    GROUP_DATA *new_group, *trav, *group;
    bool secret = FALSE;
    bool mobile = FALSE;
    bool visible = FALSE;

    if ( ch->group )
    {
	send_to_char( "You are already part of a group!\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( !str_cmp( arg, "secret" ) )
    {
	secret = TRUE;
	argument = one_argument( argument, arg );
    }
    
    one_argument( argument, arg );

    if ( !str_cmp( arg, "public" ) )
    {
	visible = TRUE;
	argument = one_argument( argument, arg );
    }

    if ( !str_cmp( arg, "xyZzy" ) )
    {
	mobile = TRUE;
	argument = one_argument( argument, arg );
    }

    if ( arg[0] == '\0' )
    {
	send_to_char( "You must name your group.\n\r", ch );
	return;
    }

    if ( !IS_NPC( ch ) )
    	for ( group = group_list ; group ; group = group->next )
    	{
   	    if ( !strcmp( argument, group->name ) )
	    {
	    	send_to_char( "Already a group with that name, try another.\n\r", ch );
	    	return;
	    }
    	}

    new_group = group_data_alloc();
    strncpy( new_group->name, argument, 80 );
    new_group->leader = ch;
    new_group->treasurer = ch;
    
    ch->group = new_group;
    ch->next_in_group = NULL;

    if( group_list == NULL )
    {
        group_list = new_group;
	new_group->next = NULL;
    }
    else
    {
	for( trav = group_list ; trav->next ; trav = trav->next );
	
	trav->next = new_group;
	new_group->next = NULL;
    }

    SET_BIT(ch->group->flags, GROUP_IS_PUBLIC );
    if (secret)
    {
	SET_BIT(ch->group->flags, GROUP_IS_SECRET );
	REMOVE_BIT(ch->group->flags, GROUP_IS_PUBLIC);
    }
    if (mobile)
    {
	SET_BIT(ch->group->flags, GROUP_IS_MOBILE );
	REMOVE_BIT(ch->group->flags, GROUP_IS_PUBLIC);
    }

/* Take out next lines for NPC's */    
    act( "$n has founded $t!", ch, new_group->name, NULL, TO_ROOM );
    act( "You have founded $t!", ch, new_group->name, NULL, TO_CHAR );
}

void do_defect( CHAR_DATA *ch, char *argument )
{
    if( !ch->group )
    {
	send_to_char( "But you are not a part of any group!\n\r", ch );
	return;
    }
    
    if( ch->group->leader == ch )
    {
	send_to_char( "But you are the leader!\n\r", ch );
	return;
    }

    act( "$n has defected from $t.", ch, ch->group->name, NULL, TO_ROOM );
    act( "You have defected from $t.", ch, ch->group->name, NULL, TO_CHAR );
    remove_from_group( ch );
}

void do_appoint( CHAR_DATA *ch, char *argument )
{
    char       	arg[MAX_INPUT_LENGTH];
    CHAR_DATA	*victim;

    if( ch->group == NULL )
    {
	send_to_char( "You are part of no group.\n\r", ch );
	return;
    }

    if( ch != ch->group->leader )
    {
	send_to_char( "Only the group leader can do that.\n\r", ch );
	return;
    }

    one_argument( argument, arg );
    
    if( arg[0] == '\0' )
    {
	send_to_char( "Who would you like to make treasurer?.\n\r", ch );
	return;
    }

    if( (victim = get_char_room( ch, arg )) == NULL )
    {
	send_to_char( "That person is not here.\n\r", ch );
	return;
    }

    if( !is_same_group( victim, ch ) )
    {
	send_to_char( "But that person is not in your group!\n\r", ch );
	return;
    }

    ch->group->treasurer = victim;
    
    act( "$n appoints $N as treasurer of $t!", 
	ch, ch->group->name, victim, TO_NOTVICT );
    act( "You appoint $N as treasurer of $t!",
	ch, ch->group->name, victim, TO_CHAR );
    act( "$n appoints you as treasurer of $t!",
	ch, ch->group->name, victim, TO_VICT );
}

void do_disband( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *gch;
    GROUP_DATA *trav;

    if( !ch->group )
    {
	send_to_char( "You have no group to disband.\n\r", ch );
	return;
    }
    
    if( ch != ch->group->leader )
    {
	send_to_char( "Only the group leader can do that!\n\r", ch );
	return;
    }

    act( "$t has been disbanded.", ch, ch->group->name, NULL, TO_CHAR );

    for(gch=ch->next_in_group;gch;gch=gch->next_in_group)
    {
	act( "$t has been disbanded.", ch, ch->group->name, gch, TO_VICT );
	gch->group = NULL;
	stop_follower( gch );
    }

    if( group_list == ch->group )
    {
	group_list = group_list->next;
    }
    else
    {
	for(trav=group_list;trav->next != ch->group;trav=trav->next);
	
	trav->next = ch->group->next;
    }

    free_group_data(ch->group);
    ch->group = NULL;
}

void add_to_group( CHAR_DATA *ch, CHAR_DATA *leader )
{
    CHAR_DATA	*gch;

    if( !leader->group )
    {
	bug( "add_to_group:leader->group NULL.", 0 );
	return;
    }

    if( leader->next_in_group == NULL )
    {	
	leader->next_in_group = ch;	
	ch->next_in_group = NULL;
    }
    else
    {
	for( gch = leader->next_in_group ; gch->next_in_group ; gch = gch->next_in_group );

	gch->next_in_group = ch;
	ch->next_in_group = NULL;
    }

    ch->group = leader->group;
    act( "$N joins $n's group.", leader, NULL, ch, TO_NOTVICT );
    act( "You join $n's group.", leader, NULL, ch, TO_VICT    );
    act( "$N joins your group.", leader, NULL, ch, TO_CHAR    );
}

void remove_from_group( CHAR_DATA *ch )
{
    CHAR_DATA *gch;

    if ( ch == ch->group->leader )
    {
	send_to_char( "You must use disband to leave the group.\n\r", ch );
	return;
    }

    if ( ch->group->treasurer == ch )
        ch->group->treasurer = ch->group->leader;

    for ( gch = ch->group->leader ; gch->next_in_group && gch->next_in_group != ch ; gch = gch->next_in_group );

    if ( gch->next_in_group != NULL && gch->next_in_group->next_in_group )
	gch->next_in_group = gch->next_in_group->next_in_group;
    else
	gch->next_in_group = NULL;

    ch->group = NULL;
}

void do_order( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH],arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *och;
    CHAR_DATA *och_next;
    bool found;
    bool fAll;

    argument = one_argument( argument, arg );
    one_argument(argument,arg2);

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Order whom to do what?\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_CONTROL_MIND ) )
    {
	send_to_char( "You feel like taking, not giving, orders.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "delete" ) )
    {
        send_to_char("That will NOT be done.\n\r",ch);
        return;
    }

    if ( !str_cmp( arg2, "pick" ) || !str_cmp( arg2, "pi" ) || !str_cmp( arg2, "pic" ) )
    {
	send_to_char( "Sorry, you can't order a picklock.\n\r", ch );
	return;
    }

    if (!str_cmp(arg2,"tell") || !str_cmp(arg2,"gtell") || !str_cmp(arg2,"emote")
    || !str_cmp(arg2,"think") || !str_cmp(arg2,"ooc")
    || !str_cmp(arg2,"answer") || !str_cmp(arg2,"shout") || !str_cmp(arg2,"auction")
    || !str_cmp(arg2,"say") )
    {
	send_to_char("You cannot order someone to do that.\n\r",ch);
	return;
    }

    if ( !str_cmp(arg2,"sick") && IS_OPTIVISION(ch) )
    {
	argument = one_argument( argument, arg2 );
	victim = get_char_world( ch, argument );
	if ( victim == NULL )
	{
	    send_to_char("The person you wish to be sicked upon is not here.\n\r",ch);
	    return;
	}

	sprintf( buf, " Log: %s order %s sick %s\n", ch->name, arg, victim->name );
	log_string( buf );

	if ( !str_cmp( arg, "all" ) )
	{
	    for ( och = ch->in_room->people ; och != NULL ; och = och->next_in_room )	
	    {
		if ( !IS_NPC( och ) || och == ch )
		    continue;

		act("$N tells you 'Aye, Aye Sir!'.\n\r", ch, NULL, och, TO_CHAR);
		strncpy(och->dislikes, victim->name,30);
		strncpy(och->hunting, victim->name,30);
		SET_BIT( och->act, ACT_HUNTER|ACT_UPDATE_ALWAYS );
		REMOVE_BIT( och->act, ACT_SENTINEL );
		och->spec_fun = spec_assassin;
	    }
	    return;
	}
	else
	{
	    och = get_char_room( ch, arg );
	    if ( och == NULL )
	    {
		send_to_char("They are not here.\n\r",ch);
		return;
	    }
	    act("$N tells you 'Aye, Aye Sir!'.\n\r", ch, NULL, och, TO_CHAR);
	    strncpy(och->dislikes, victim->name,30);
	    strncpy(och->hunting, victim->name,30);
	    SET_BIT( och->act, ACT_HUNTER|ACT_UPDATE_ALWAYS );
	    REMOVE_BIT( och->act, ACT_SENTINEL );
	    och->spec_fun = spec_assassin;
	}

	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	fAll   = TRUE;
	victim = NULL;
    }
    else
    {
	fAll   = FALSE;
	if ( ( victim = get_char_room( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if ( ( !IS_AFFECTED(victim, AFF_CONTROL_MIND) || victim->master != ch ) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    for ( och = ch->in_room->people; och != NULL; och = och_next )
    {
	och_next = och->next_in_room;

	if ( IS_AFFECTED(och, AFF_CONTROL_MIND)
	&&   och->master == ch 
	&& ( fAll || och == victim ) )
	{
	    found = TRUE;
	    sprintf( buf, "$n orders you to '%s'.", argument );
	    act( buf, ch, NULL, och, TO_VICT );
	    interpret( och, argument );
	}
    }

    if ( found )
	send_to_char( "Ok.\n\r", ch );
    else
	send_to_char( "You have no followers here.\n\r", ch );
    return;
}



void do_group( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    GROUP_DATA *group;
    bool fAll = FALSE;	
    
	if( ( group = ch->group ) == NULL )
	{
		send_to_char( "You are not part of any group.  See 'Help group'\n\r", ch );
		return;
	}
	
    one_argument( argument, arg );


    if ( arg[0] == '\0' )
    {
	CHAR_DATA *gch;
	CHAR_DATA *leader;

	leader = (ch->leader != NULL) ? ch->leader : ch;
	sprintf( buf, "%s's group:\n\r", PERS(leader, ch) );
	send_to_char( buf, ch );

	for ( gch = char_list; gch != NULL; gch = gch->next )
	{
	    if ( is_same_group( gch, ch ) )
	    {
		sprintf( buf,
		"[%2d] %-16s %4d/%4d hp %4d/%4d mv\n\r`Rfire %4d/%4d `Bwater %4d/%4d `Gearth %4d/%4d `Cwind %4d/%4d `Wspirit %4d/%4d`w\n\r",
		    gch->level,
		    capitalize( PERS(gch, ch) ),
		    ( IS_AFFECTED( gch, AFF_BERSERK ) ? gch->max_hit : gch->hit ), gch->max_hit,
		    gch->move,  gch->max_move,
		    gch->mana[ 0 ], gch->max_mana[ 0 ],
		    gch->mana[ 1 ], gch->max_mana[ 1 ],
		    gch->mana[ 2 ], gch->max_mana[ 2 ],
		    gch->mana[ 3 ], gch->max_mana[ 3 ],
		    gch->mana[ 4 ], gch->max_mana[ 4 ]);
		send_to_char( buf, ch );
	    }
	}
/*	sprintf( buf, "\n\rCoffers: Crowns: %ld Duckets: %ld Wheels: %ld Greckles: %ld\n\r",
	group->gold[0], group->gold[1],
	group->gold[2], group->gold[3] );
	send_to_char( buf, ch ); */
	if( IS_SET(group->flags, GROUP_IS_PUBLIC ) )
	    send_to_char( "Group is Public.", ch );
	if( IS_SET(group->flags, GROUP_IS_SECRET) )
	    send_to_char( "Group is Secret.", ch );
	send_to_char( "\n\r", ch );
	return;
    }
    fAll = !str_cmp( arg, "all" );
    if (!fAll)
    {
    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if ( ch == victim )
    {
	send_to_char( "You are already in this group\n\r", ch );
	return;
    }
	
    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }

    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
	return;
    }
    
    /*
    if (IS_AFFECTED(victim,AFF_CONTROL_MIND))
    {
        send_to_char("You can't remove controlled mobs from your group.\n\r",ch);
        return;
    }
   */
    
    if (IS_AFFECTED(ch,AFF_CONTROL_MIND))
    {
    	act("You like your master too much to leave $m!",ch,NULL,victim,TO_VICT);
    	return;
    }

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	act( "$n removes $N from $s group.",   ch, NULL, victim, TO_NOTVICT );
	act( "$n removes you from $s group.",  ch, NULL, victim, TO_VICT    );
	act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR    );
	remove_from_group( victim );
	return;
    }

    if ( ( ch->level - victim->level < -10
    ||   ch->level - victim->level >  10 )
      && !IS_NPC( victim ) )
    {
	act( "$N cannot join $n's group.",     ch, NULL, victim, TO_NOTVICT );
	act( "You cannot join $n's group.",    ch, NULL, victim, TO_VICT    );
	act( "$N cannot join your group.",     ch, NULL, victim, TO_CHAR    );
	return;
    }

      
    add_to_group( victim, ch );
    return;
    }
    else
    {
    if (IS_AFFECTED(ch,AFF_CONTROL_MIND))
    {
    	act("You like your master too much to leave $m!",ch,NULL,NULL,TO_CHAR);
    	return;
    }
    if ( ch->master != NULL || ( ch->leader != NULL && ch->leader != ch ) )
    {
	send_to_char( "But you are following someone else!\n\r", ch );
	return;
    }
    for ( victim = ch->in_room->people; victim != NULL; victim = victim->next_in_room )
    {

    if ( victim->master != ch && ch != victim )
    {
	act( "$N isn't following you.", ch, NULL, victim, TO_CHAR );
	continue;
    }
   
    if ( ch == victim )
    {
	send_to_char( "You are already in this group\n\r", ch );
	continue;
    }
    
    /*
    if (IS_AFFECTED(victim,AFF_CONTROL_MIND))
    {
        send_to_char("You can't remove controlled mobs from your group.\n\r",ch);
        continue;
    }
    */

    if ( is_same_group( victim, ch ) && ch != victim )
    {
	act( "$n removes $N from $s group.",   ch, NULL, victim, TO_NOTVICT );
	act( "$n removes you from $s group.",  ch, NULL, victim, TO_VICT    );
	act( "You remove $N from your group.", ch, NULL, victim, TO_CHAR    );
	remove_from_group( victim );
	continue;
    }

    if ( ( ch->level - victim->level < -10
    ||   ch->level - victim->level >  10 )
      && !IS_NPC( victim ) )
    {
	act( "$N cannot join $n's group.",     ch, NULL, victim, TO_NOTVICT );
	act( "You cannot join $n's group.",    ch, NULL, victim, TO_VICT    );
	act( "$N cannot join your group.",     ch, NULL, victim, TO_CHAR    );
	continue;
    }

      
    add_to_group( victim, ch );
    continue;
    }
    }
}

void do_split( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *gch;
    int members;
    int amount;
    int share;
    int extra;
    int coin_type;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Split how much?\n\r", ch );
	return;
    }
    
    amount = atoi( arg );

    if ( amount < 0 )
    {
	send_to_char( "Your group wouldn't like that.\n\r", ch );
	return;
    }

    if ( amount == 0 )
    {
	send_to_char( "You hand out zero coins, but no one notices.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( ( coin_type = get_coin_type( arg ) ) < 0 )
        coin_type = race_table[ ch->race ][ ch->sex ].world;

    if ( ch->gold[ coin_type ] < amount )
    {
	send_to_char( "You don't have that much gold.\n\r", ch );
	return;
    }
  
    members = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CONTROL_MIND))
	    members++;
    }

    if ( members < 2 )
    {
	send_to_char( "Just keep it all.\n\r", ch );
	return;
    }
	    
    share = amount / members;
    extra = amount % members;

    if ( share == 0 )
    {
	send_to_char( "Don't even bother, cheapskate.\n\r", ch );
	return;
    }

    ch->gold[ coin_type ] -= amount;
    ch->gold[ coin_type ] += share + extra;

    sprintf( buf,
	"You split %d %s.  Your share is %d %s.\n\r",
	amount, coin_names[ coin_type ], share + extra, coin_names[ coin_type ] );
    send_to_char( buf, ch );

    sprintf( buf,
	"$n split %d %s.  Your share is %d %s.\n\r",
	amount, coin_names[ coin_type ], share, coin_names[ coin_type ] );

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( gch != ch && is_same_group( gch, ch ) && !IS_AFFECTED(gch,AFF_CONTROL_MIND))
	{
	    act( buf, ch, NULL, gch, TO_VICT );
	    gch->gold[ coin_type ] += share;
	}
    }

    return;
}

void do_gtell( CHAR_DATA *ch, char *argument )
{
    do_gtell_race( ch, argument, gsn_common );
    return;
}

void do_gtell_race( CHAR_DATA *ch, char *argument, sh_int sn )
{
    char buf[MAX_STRING_LENGTH];
    char *said;
    CHAR_DATA *gch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Tell your group what?\n\r", ch );
	return;
    }

    if ( IS_SET( ch->comm, COMM_NOTELL ) || IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) )
    {
	send_to_char( "Your message didn't get through!\n\r", ch );
	return;
    }


    if ( ch->pcdata != NULL && ( ch->pcdata->hand != NULL
      || ch->pcdata->deck != NULL
      || ch->pcdata->discarded != NULL
      || ch->pcdata->played != NULL ) )
    {
	send_to_char( "No private messages while playing cards.\n\r", ch );
	return;
    }

    /*
     * Note use of send_to_char, so gtell works on sleepers.
     */
    check_improve(ch,sn,TRUE,5);
    said = argument;
    for ( gch =char_list; gch != NULL; gch = gch->next)
    {
	if ( !is_same_group( gch, ch ) || IS_SET( gch->in_room->room_flags, ROOM_SILENCE )  )
	    continue;

	argument = speak_lang( ch, gch, said, sn );
	sprintf( buf, "`G%s tells the group '%s'.`w\n\r", ch->name, argument );
        send_to_char( buf, gch );
        check_improve(gch,sn,TRUE,3);
    }
    return;
}

bool is_same_group( CHAR_DATA *ach, CHAR_DATA *bch )
{
    if ( ach == bch ) 
	return TRUE;

    if ( ach->group != NULL && bch->group != NULL )
    	if ( ach->group->leader == bch->group->leader ) 
	    return TRUE;

    return FALSE; 
}

void do_impchat (CHAR_DATA *ch, char *argument)
 {
   char buf[MAX_STRING_LENGTH];
   DESCRIPTOR_DATA *d;
   
   if (argument[1] == '\0') 
        {  
           send_to_char ("`RWhat do You want to char with the Other Imps about?`w\n\r",ch);
        }
  else
        {
	    sprintf( buf, "$n: %s", argument );
	    cact_new("$n: $t",ch,argument,NULL,TO_CHAR,POS_DEAD,"`r");
	    for ( d = descriptor_list; d != NULL; d = d->next )
	    {
		if ( d->connected <= CON_PLAYING && 
		     IS_IMP(d->character) && 
	             !IS_SET(d->character->comm,COMM_NOWIZ) )
		{
		    cact_new("$n: $t",ch,argument,d->character,TO_VICT,POS_DEAD,"`r");
		}
	    }
	}
 }
void do_infochan (CHAR_DATA *ch, char *argument)
{
  if (IS_SET(ch->comm,COMM_INFO))
      {
        REMOVE_BIT(ch->comm,COMM_INFO);
        send_to_char ("Info Channel OFF\n\r",ch);
      }
   else
      {
        SET_BIT(ch->comm,COMM_INFO);
        send_to_char ("Info Channel ON\n\r",ch);
       }
}
 
