/* Code specifically for the new skill system */

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
#include "merc.h"
#include "magic.h"

//extern const int mana_stat[MAX_ELEMENT_TYPE];
//extern const int mana_gain_table[26];

void do_skillz( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
    int i,c,sn,num;
    int total[ 6 ][ 3 ];
    int groups[MAX_GROUP];
    int tspells = 0;
    int tskills = 0;

    if ( *argument != '\0' )
    {
	for ( i = 0 ; i < 5 ; i++ )
	    for ( c = 0 ; c < 3 ; c++ )
		if ( !str_prefix( argument, skill_table[ gsn_element_power[ i ][ c ] ].name ) )
		{
		    buf[0] = '\0';
		    num = 0;
		    for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
		    {
			if ( skill_table[ sn ].spell_fun != spell_null 
			  && skill_table[ sn ].element[ i ].level == c 
			  && skill_table[ sn ].element[ ( i + 1 ) % 5 ].level < 0 
			  && skill_table[ sn ].element[ ( i + 2 ) % 5 ].level < 0 
			  && skill_table[ sn ].element[ ( i + 3 ) % 5 ].level < 0 
			  && skill_table[ sn ].element[ ( i + 4 ) % 5 ].level < 0 )
			{
			    if ( num == 0 )
			    {
				sprintf( buf1, "  %s -\n\r\n\r", capitalize( skill_table[ gsn_element_power[ i ][ c ] ].name ) );
				send_to_char( buf1, ch );
			    }

			    sprintf( buf1, "  %22s", skill_table[ sn ].name );
			    strcat( buf, buf1 );

			    if ( num++ % 3 == 2 )
			    {
				strcat( buf, "\n\r" );
				send_to_char( buf, ch );
				buf[0] = '\0';
			    }
			}
		    }

		    if ( num == 0 )
		    {
			send_to_char( "Sorry no spells with that power.\n\r", ch );
			return;
		    }

		    if ( buf[0] != '\0' )
		    {
			send_to_char( buf, ch );
			send_to_char( "\n\r", ch );
		    }
		    return;
		}

/* skill groups */

        for  ( i = 0 ; i < MAX_GROUP ; i++ )      
	{
	    if ( !str_prefix( argument, group_name[ i ] ) )
	    {
		buf[0] = '\0';
		num = 0;
		for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
		{
		    if ( skill_table[ sn ].group == i )
		    {
			if ( num == 0 )
			{
			    sprintf( buf1, "  %s -\n\r\n\r", capitalize( group_name[ i ] ) );
			    send_to_char( buf1, ch );
			}

			sprintf( buf1, "  %22s", skill_table[ sn ].name );
			strcat( buf, buf1 );

			if ( num++ % 3 == 2 )
			{
			    strcat( buf, "\n\r" );
			    send_to_char( buf, ch );
			    buf[ 0 ] = '\0';
			}
		    }
		}

		if ( num == 0 )
		{
		    send_to_char( "No skills found in this group.\n\r", ch );
		    return;
		}

		if ( buf[0] != '\0' )
		{
		    send_to_char( buf, ch );
		    send_to_char( "\n\r", ch );
		}

		return;
	    }

	}

/* 5 power spells */

	c = 1;
	if ( !strncmp( argument, "minor", 5 ) )
	{
	    c = 0;
            argument = one_argument( argument, buf );
	}
	else if ( !strncmp( argument, "major", 5 ) )
	{
	    c = 2;
	    argument = one_argument( argument, buf );
	}

		if ( !str_prefix( argument, "5" ) || !str_prefix( argument, "five" ) )
		{
		    buf[0] = '\0';
		    num = 0;
		    for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
		    {
			if ( skill_table[ sn ].spell_fun != spell_null 
			  && skill_table[ sn ].element[ 0 ].level == c 
			  && skill_table[ sn ].element[ 1 ].level == c 
			  && skill_table[ sn ].element[ 2 ].level == c 
			  && skill_table[ sn ].element[ 3 ].level == c 
			  && skill_table[ sn ].element[ 4 ].level == c )
			{
			    if ( num == 0 )
			    {
				sprintf( buf1, " %s Five Spell Powers -\n\r\n\r",
				    ( c == 0 ? "Minor" : ( c == 1 ? "" : "Major" ) ) );
				send_to_char( buf1, ch );
			    }

			    sprintf( buf1, "  %22s", skill_table[ sn ].name );
			    strcat( buf, buf1 );

			    if ( num++ % 3 == 2 )
			    {
				strcat( buf, "\n\r" );
				send_to_char( buf, ch );
				buf[0] = '\0';
			    }
			}
		    }

		    if ( num == 0 )
		    {
			send_to_char( "Sorry no spells with that power.\n\r", ch );
			return;
		    }

		    if ( buf[0] != '\0' )
		    {
			send_to_char( buf, ch );
			send_to_char( "\n\r", ch );
		    }
		    return;
		}


	send_to_char( "Invalid group or spell power.\n\r", ch );
	return;
    }

    for ( i = 0 ; i < 6 ; i++ )
	for ( c = 0 ; c < 3 ; c++ )
	    total[ i ][ c ] = 0;

    for ( i = 0 ; i < MAX_GROUP ; i++ )
	groups[ i ] = 0;

    for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
    {
	groups[ skill_table[ sn ].group ]++;

	if ( skill_table[ sn ].spell_fun != spell_null )
	    tspells++;
	else
	    tskills++;
    }


    for ( i = 0 ; i < 5 ; i++ )
    {
	for ( c = 0 ; c < 3 ; c++ )
	{
	    for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
		if ( skill_table[ sn ].spell_fun != spell_null
		  && skill_table[ sn ].element[ i ].level == c 
		  && skill_table[ sn ].element[ ( i + 1 ) % 5 ].level < 0 
		  && skill_table[ sn ].element[ ( i + 2 ) % 5 ].level < 0 
		  && skill_table[ sn ].element[ ( i + 3 ) % 5 ].level < 0 
		  && skill_table[ sn ].element[ ( i + 4 ) % 5 ].level < 0 )
		    total[ i ][ c ]++;
	}
    }

    for ( c = 0 ; c < 3 ; c++ )
      for ( sn = 1 ; sn < MAX_SKILL ; sn++  )
	if ( skill_table[ sn ].spell_fun != spell_null
	  && skill_table[ sn ].element[ 0 ].level == c 
	  && skill_table[ sn ].element[ 1 ].level == c 
	  && skill_table[ sn ].element[ 2 ].level == c 
	  && skill_table[ sn ].element[ 3 ].level == c 
	  && skill_table[ sn ].element[ 4 ].level == c )
	    total[ 5 ][ c ]++;

     send_to_char( " Level |   Fire    |   Water   |   Earth   |   Wind    |  Spirit   | All 5\n\r", ch );
     send_to_char( "-------+-----------+-----------+-----------+-----------+-----------+-----------\n\r", ch );
     send_to_char( "       |           |           |           |           |           |\n\r", ch );

     for ( i = 0 ; i < 3 ; i++ )
     {
	sprintf( buf, " %5d | %9d | %9d | %9d | %9d | %9d | %9d\n\r",
		i+1,
		total[ 0 ][ i ],
		total[ 1 ][ i ],
		total[ 2 ][ i ],
		total[ 3 ][ i ],
		total[ 4 ][ i ],
		total[ 5 ][ i ] );
	send_to_char( buf, ch );
    }

     send_to_char( "       |           |           |           |           |           |\n\r", ch );
     send_to_char( "-------+-----------+-----------+-----------+-----------+-----------+-----------\n\r", ch );

	sprintf( buf, " Total | %9d | %9d | %9d | %9d | %9d | %9d\n\r",
		total[ 0 ][ 0 ]  + total[ 0 ][ 1 ] + total[ 0 ][ 2 ],
		total[ 1 ][ 0 ]  + total[ 1 ][ 1 ] + total[ 1 ][ 2 ],
		total[ 2 ][ 0 ]  + total[ 2 ][ 1 ] + total[ 2 ][ 2 ],
		total[ 3 ][ 0 ]  + total[ 3 ][ 1 ] + total[ 3 ][ 2 ],
		total[ 4 ][ 0 ]  + total[ 4 ][ 1 ] + total[ 4 ][ 2 ],
		total[ 5 ][ 0 ]  + total[ 5 ][ 1 ] + total[ 5 ][ 2 ]);
	send_to_char( buf, ch );


     sprintf( buf, "\n\r" );
     for ( i = 0 ; i < 5 ; i++ )
     {
	one_argument( group_name[ i ] , buf1 );
	sprintf( buf, "%s %10s |", buf, capitalize( buf1 ) );
     }

     one_argument( group_name[ 10 ] , buf1 );
     sprintf( buf, "%s %10s\n\r", buf, capitalize( buf1 ) );

     send_to_char( buf, ch );
     send_to_char( "------------+------------+------------+------------+------------+------------\n\r", ch );
     send_to_char( "            |            |            |            |            |            |\n\r", ch );
     sprintf( buf, " %10d | %10d | %10d | %10d | %10d | %10d\n\r",
	groups[ 0 ], groups[ 1 ], groups[ 2 ], groups[ 3 ], groups[ 4 ], groups[ 10 ] );
     send_to_char( buf, ch );


     sprintf( buf, "\n\r" );
     for ( i = 5 ; i < 10 ; i++ )
     {
	one_argument( group_name[ i ] , buf1 );
	sprintf( buf, "%s %10s |", buf, capitalize( buf1 ) );
     }

     strcat( buf, "\n\r" );
     send_to_char( buf, ch );
     send_to_char( "------------+------------+------------+------------+------------+------------\n\r", ch );
     send_to_char( "            |            |            |            |            |            |\n\r", ch );
     sprintf( buf, " %10d | %10d | %10d | %10d | %10d |\n\r",
	groups[ 5 ], groups[ 6 ], groups[ 7 ], groups[ 8 ], groups[ 9 ]);
     send_to_char( buf, ch );


     sprintf( buf, "\n\r\n\rTotal spells: %d\n\rTotal skills: %d + ( 10 languages, 15 spell powers, 33 weapon skills powers)\n\rTotal : %d\n\r",
		tspells, tskills - 58, tspells + tskills );
     send_to_char( buf, ch );

}

void do_cost( CHAR_DATA *ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    long x;
    long cost = 0;
    int guilds = 0;

    for ( x = 0 ; x < MAX_GUILD ; x++ )
	if ( IS_SET( ch->guilds, guild_table[ x ].flag ) )
	    guilds++;

    if ( guilds >= 3 && ch->guilds != 1)
    {
	send_to_char( "You cannot join more then 3 guilds.\n\r", ch );
	return;
    }
    else if ( ch->level < 25 && guilds >= 1 && ch->guilds != 1 )
    {
	send_to_char( "You cannot join another guild until level 25.\n\r", ch );
	return;
    }
    else if ( ch->level < 35 && guilds >= 2 && ch->guilds != 1)
    {
	send_to_char( "You cannot join another guild until level 35.\n\r", ch );
	return;
    }

    if ( ch->guilds <= 1)
	cost = 0;
    else
    	cost = 1000 * ch->level + 10000 * guilds * guilds;

    sprintf( buf, "It will cost you %ld %s to join this guild.\n\r", cost, coin_names[ ch->in_room->area->world ] );
    send_to_char( buf, ch );
    return;
}

void do_join( CHAR_DATA *ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * pRoom;
    EXIT_DATA * pexit;
    long x;
    long cost = 0;
    long guild = 0;
    int guilds = 0;
    int i;
    int door;

    for ( x = 0 ; x < MAX_GUILD ; x++ )
	if ( IS_SET( ch->guilds, guild_table[ x ].flag ) )
	    guilds++;

    if ( guilds >= 1 && ch->guilds != 1 )
	{
	send_to_char( "You can't join more than 1 guild at this time.\n\r", ch );
	return;
	}

    if ( guilds >= 3 && ch->guilds != 1)
    {
	send_to_char( "You cannot join more then 3 guilds.\n\r", ch );
	return;
    }
    else if ( ch->level < 25 && guilds >= 1 && ch->guilds != 1)
    {
	send_to_char( "You cannot join another guild until level 25.\n\r", ch );
	return;
    }
    else if ( ch->level < 35 && guilds >= 2 && ch->guilds != 1)
    {
	send_to_char( "You cannot join another guild until level 35.\n\r", ch );
	return;
    }

    for ( door = 0 ; door < 6 && guild == 0 ; door++ )
    {
	if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
	  && ( pRoom = pexit->u1.to_room ) != NULL
	  && pRoom->guilds != 0 )
	{
	    guilds = 0;
	    for ( x = 1 ; x != 0 ; x *= 2 )
		if ( IS_SET( pRoom->guilds, x ) )
		    guilds++;

	    if ( guilds == 1 )
		guild = pRoom->guilds;
	}
    }

    if ( guild == 0 )
    {
	send_to_char( "Please notify nearest immortal, no guild set in rooms around here.\n\r", ch );
	return;
    }

    if ( IS_SET( ch->guilds, guild ) )
    {
	send_to_char( "You are already a member of this guild.\n\r", ch );
	return;
    }

    if ( ch->guilds <= 1 )
    	cost = 0;
    else
	cost = 1000 * ch->level + 10000 * guilds * guilds;

    if ( get_money_total( ch, ch->in_room->area->world ) < cost )
    {
        sprintf( buf, "It will cost you %ld %s to join this guild.\n\r", cost, coin_names[ ch->in_room->area->world ] );
        send_to_char( buf, ch );
        return;
    }

    for( i = 0 ; i < MAX_GUILD && guild_table[ i ].flag != guild ;i++ );

    if ( i >= MAX_GUILD )
    {
	send_to_char( "Invalid Guild to join.\n\r", ch );
	return;
    }

    get_char_money( ch, cost, ch->in_room->area->world );

    sprintf( buf, "%s has joined %s!\n\r", capitalize( ch->name),guild_table[ i ].title );
    send_to_guild( buf, guild );

    if ( ch->guilds == 1 )
	REMOVE_BIT ( ch->guilds, DEFAULT );

    SET_BIT( ch->guilds, guild );

/*  Add any starting bonuses for guilds here */

    sprintf( buf, "You have joined %s.\n\r", guild_table[ i ].title );
    send_to_char( buf, ch );

}

/* used to get new skills */
void do_gain(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char * args;
    CHAR_DATA *trainer;
    int sn = 0;
    int cost;
    int i;
    bool found, trig;

    if (IS_NPC(ch))
	return;

    args = argument;
    /* find a trainer */
    for ( trainer = ch->in_room->people; 
	  trainer != NULL; 
	  trainer = trainer->next_in_room)
    {
	if ( trainer == ch )
	continue;
	if ( !can_see(ch,trainer) )
	    continue;
	if (IS_NPC(trainer) && IS_SET(trainer->act,ACT_GAIN) && trainer->pIndexData->gainer_data != NULL )
	    break;
    }

    if (trainer == NULL )
    {
	send_to_char("You can't do that here.\n\r",ch);
	return;
    }

    argument=one_argument(argument,arg);
    one_argument(argument,arg2);

    if (arg[0] == '\0')
    {
	int col;
	
	col = 0;


	if ( IS_NPC( trainer ) )
	{
	    sprintf( buf, "Cost in %s\n\r", coin_names[ ch->in_room->area->world ] );
	    send_to_char( buf, ch );
	}
        sprintf(buf, "%-18s %-6s %-9s   %-18s %-6s %-9s\n\r\n\r",
                     "skill","pracs",( !IS_NPC( trainer ) ? "chance" : "cost" ),"skill","pracs",( !IS_NPC( trainer ) ? "chance" : "cost" ) );
        send_to_char(buf,ch);
 
        for (sn = 0; sn < MAX_SKILL; sn++)
        {
            if (skill_table[sn].name == NULL)
                break;

	    found = FALSE;
	    
	    for ( i = 0 ; i < MAX_PREREQUISITE ; i++ )
		if ( skill_table[ sn ].prerequisite[ i ].pgsn != NULL && ch->pcdata->learned[ *skill_table[ sn ].prerequisite[ i ].pgsn ] < skill_table[ sn ].prerequisite[ i ].percent )
		    found = TRUE;

	    if ( found )
		continue;

	    if ( skill_table[ sn ].spell_fun != spell_null )
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		    if ( skill_table[ sn ].element[ i ].level >= 0 
		      && ch->pcdata->learned[ gsn_element_power[ i ][ skill_table[ sn ].element[ i ].level ] ] < 1 )
			found = TRUE;

	    if ( found )
		continue;

            if ( ch->pcdata->learned[sn]
	    ||  ( skill_table[ sn ].spell_fun == spell_null
		&& get_skill_level( ch, sn) > ch->level )
            ||  get_skill_cost( ch, sn ) < 1 )
		continue;

	    if ( ( IS_NPC( trainer ) && trainer->pIndexData->gainer_data->skill[sn] )
	      || ( !IS_NPC( trainer ) && trainer->pcdata->teaching[ sn ] ) )
            {
                sprintf(buf,"%-18s  %-5d %-9ld   ", skill_table[sn].name, get_skill_cost( ch, sn ),
		    ( IS_NPC( trainer ) ? get_gain_cost( ch, sn) :
			get_skill( trainer, sn ) - get_gain_cost( ch, sn ) * 2 + get_curr_stat( ch, STAT_INT ) ) );
                send_to_char(buf,ch);
                if (++col % 2 == 0)
                    send_to_char("\n\r",ch);
            }
        }
        if (col % 2 != 0)
            send_to_char("\n\r",ch);
	return;
    }

    /* else add a group/skill */
    sn = skill_lookup(args);
    if (sn > -1)
    {
        found = FALSE;
        for ( i = 0 ; i < MAX_PREREQUISITE ; i++ )
	    if ( skill_table[ sn ].prerequisite[ i ].pgsn != NULL && ch->pcdata->learned[ *skill_table[ sn ].prerequisite[ i ].pgsn ] < skill_table[ sn ].prerequisite[ i ].percent )
	        found = TRUE;

	if ( skill_table[ sn ].spell_fun != spell_null )
	    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	        if ( skill_table[ sn ].element[ i ].level >= 0 
	          && ch->pcdata->learned[ gsn_element_power[ i ][ skill_table[ sn ].element[ i ].level ] ] < 1 )
		    found = TRUE;


        if ( ( IS_NPC( trainer ) && !trainer->pIndexData->gainer_data->skill[sn] )
	  || ( !IS_NPC( trainer ) && !trainer->pcdata->teaching ) 
	  || found )
        {
            act("Sorry, you cannot learn that from $N.",
                ch,NULL,trainer,TO_CHAR);
            return;
        }
 
        if (ch->pcdata->learned[sn])
        {
            act("You already know it!",
                ch,NULL,trainer,TO_CHAR);
            return;
        }
 
        if ( get_skill_cost( ch, sn ) <= 0 )
        {
            act("That is beyond your powers.",
                ch,NULL,trainer,TO_CHAR);
            return;
        }
 
        if (ch->practice < get_skill_cost( ch, sn ) 
	    || ( skill_table[ sn ].spell_fun == spell_null
	       && get_skill_level( ch, sn ) > ch->level ) )
        {
            act("You are not yet ready for it.",
                ch,NULL,trainer,TO_CHAR);
            return;
        }

	cost = get_gain_cost( ch, sn );
	if ( IS_NPC( trainer ) && get_money_total( ch, ch->in_room->area->world ) < ( cost ) )
	{
	    act("You cannot afford to gain it.",
		ch,NULL,trainer,TO_CHAR);
	    return;
	}

	if ( IS_NPC( trainer ) )
	    get_char_money( ch, cost, ch->in_room->area->world ); 
	else if ( number_percent() > get_skill( trainer, sn ) - get_gain_cost( ch, sn ) * 2 + get_curr_stat( ch, STAT_INT ) )
	{
	     act("$n failed to learn $t from $N.", ch, skill_table[sn].name, trainer, TO_NOTVICT );
	     act("$n failed to learn $t from you.", ch, skill_table[sn].name, trainer, TO_VICT );
	     act("You failed to learn $t from $N.", ch, skill_table[sn].name, trainer, TO_CHAR );
	     ch->practice -= get_skill_cost( ch, sn );
	     return;
	}

        /* add the skill */
	if ( skill_table[sn].spell_fun != spell_null )
	    ch->pcdata->learned[sn] = UMAX( 40, race_table[ ch->race ][ ch->sex ].starting[ sn ] );
	else
	    ch->pcdata->learned[sn] = UMAX( 25, race_table[ ch->race ][ ch->sex ].starting[ sn ] );
        act("$N trains you in the art of $t",
            ch,skill_table[sn].name,trainer,TO_CHAR);
        ch->practice -= get_skill_cost( ch, sn );

	trig = do_trigger( trainer, ch, NULL, NULL, TRIG_GAINS, TRIG_TYPE_MOB);
        return;
    }

    act("Syntax: gain or gain <skill name>",ch,NULL,trainer,TO_CHAR);
}
    



/* RT spells and skills show the players spells (or skills) */

void do_spells(CHAR_DATA *ch, char *argument)
{
    int sn,x;
    bool found = FALSE;
    char buf[MAX_STRING_LENGTH];
    BUFFER * output;
    if (IS_NPC(ch))
    {
	SKILL_LIST * skills;

	if ( ch->skills == NULL )
	{
	    send_to_char( "You have no skills.\n\r", ch );
	    return;
 	}

	send_to_char( "Spells:\n\r\n\r", ch );

	for ( skills = ch->skills ; skills != NULL ; skills = skills->next )
	{
	    sprintf( buf, "    %s\n\r",  skill_table[ skills->sn ].name );
	    send_to_char( buf, ch );
	}

        return;
    }

    /* initilize data */
    output = new_buf();
    x = 0;
    for (sn = 1; sn < MAX_SKILL; sn++)
    {
      	if (skill_table[sn].name == NULL )
        	break;

    if (skill_table[sn].spell_fun != spell_null)
    {


        if (ch->pcdata->learned[sn] > 0 || ch->pcdata->skill_mod[sn] > 0 )
	{
        found = TRUE;
	  x++;
	  if ( skill_table[ sn ].group == 5 )
              sprintf(buf,"`R%-20s`w  %3d%%  ",skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 6 )
              sprintf(buf,"`B%-20s`w  %3d%%  ",skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 7 )
              sprintf(buf,"`y%-20s`w  %3d%%  ",skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 8 )
              sprintf(buf,"`C%-20s`w  %3d%%  ",skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 9 )
              sprintf(buf,"`W%-20s`w  %3d%%  ",skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 10 )
              sprintf(buf,"`K%-20s`w  %3d%%  ",skill_table[sn].name, ch->pcdata->learned[sn]);
	  else
              sprintf(buf,"%-20s  %3d%%  ",skill_table[sn].name, ch->pcdata->learned[sn]);

	add_buf(output, buf);
	if ( ch->pcdata->skill_mod[sn] != 0 )
	    sprintf(buf, "%2d%%   ",ch->pcdata->skill_mod[sn]);
	else
	  sprintf(buf, "      ");
	  add_buf(output, buf);
	if ( x % 2 == 0)
	add_buf(output,"\n\r");
	}
      }
    }

    /* return results */

    if (!found)
    {
      send_to_char("You know no spells.\n\r",ch);
      free_buf(output);
      return;
    }
    else
    {
    send_to_char("Name               Learned  Mod   Name               Learned  Mod\n\r",ch); 
    page_to_char(buf_string(output),ch);
    free_buf(output);
    }

}

void do_slist(CHAR_DATA *ch, char *argument)
{
    int sn,lev,x,y;
    bool found = FALSE;
    char buf[MAX_STRING_LENGTH];
    BUFFER * output;
    if (IS_NPC(ch))
    {
	SKILL_LIST * skills;

	if ( ch->skills == NULL )
	{
	    send_to_char( "You have no skills.\n\r", ch );
	    return;
 	}

	send_to_char( "Skills:\n\r\n\r", ch );

	for ( skills = ch->skills ; skills != NULL ; skills = skills->next )
	{
	    sprintf( buf, "    %s\n\r",  skill_table[ skills->sn ].name );
	    send_to_char( buf, ch );
	}

        return;
    }

    /* initilize data */
    output = new_buf();
    x = 0;
    for ( y = 1; y < MAX_LEVEL; y++)
    {
    for (sn = 1; sn < MAX_SKILL; sn++)
    {
      	if (skill_table[sn].name == NULL )
        	break;

    if (skill_table[sn].spell_fun == spell_null)
    {
	if ( get_skill( ch, sn ) <= 0 && ( get_skill_level( ch, sn ) > MAX_LEVEL || get_skill_level( ch, sn ) < 0 ) ) 
	    continue;

        found = TRUE;
        lev = get_skill_level( ch, sn);
	if ( lev != y )
	   continue;

        if (ch->pcdata->learned[sn] >= 0)
	{
	  x++;
          sprintf(buf,"%2d  %-20s ",lev,skill_table[sn].name);
	add_buf(output, buf);
	  sprintf(buf, "      ");
	  add_buf(output, buf);
	if ( x % 2 == 0)
	add_buf(output,"\n\r");
	}
      }
    }
    }

    /* return results */

    if (!found)
    {
      send_to_char("You know no skills.\n\r",ch);
      free_buf(output);
      return;
    }
    else
    {
    send_to_char("Lv  Name                   Lv  Name               \n\r",ch); 
    page_to_char(buf_string(output),ch);
    free_buf(output);
    }

}

void do_skills(CHAR_DATA *ch, char *argument)
{
    int sn,lev,x,y;
    bool found = FALSE;
    char buf[MAX_STRING_LENGTH];
    BUFFER * output;
    if (IS_NPC(ch))
    {
	SKILL_LIST * skills;

	if ( ch->skills == NULL )
	{
	    send_to_char( "You have no skills.\n\r", ch );
	    return;
 	}

	send_to_char( "Skills:\n\r\n\r", ch );

	for ( skills = ch->skills ; skills != NULL ; skills = skills->next )
	{
	    sprintf( buf, "    %s\n\r",  skill_table[ skills->sn ].name );
	    send_to_char( buf, ch );
	}

        return;
    }

    /* initilize data */
    output = new_buf();
    x = 0;
    for ( y = 1; y < MAX_LEVEL; y++)
    {
    for (sn = 1; sn < MAX_SKILL; sn++)
    {
      	if (skill_table[sn].name == NULL )
        	break;

    if (skill_table[sn].spell_fun == spell_null)
    {
	if ( get_skill_level( ch, sn ) > MAX_LEVEL || get_skill_level( ch, sn ) < 0 )
	    continue;

        found = TRUE;
        lev = get_skill_level( ch, sn);
	if ( lev != y )
	   continue;

        if (ch->pcdata->learned[sn] > 0 || ch->pcdata->skill_mod[sn] > 0 )
	{
	  x++;
	  if ( skill_table[ sn ].group == 5 )
              sprintf(buf,"%2d  `R%-20s`w  %3d%%  ",lev,skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 6 )
              sprintf(buf,"%2d  `B%-20s`w  %3d%%  ",lev,skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 7 )
              sprintf(buf,"%2d  `y%-20s`w  %3d%%  ",lev,skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 8 )
              sprintf(buf,"%2d  `C%-20s`w  %3d%%  ",lev,skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 9 )
              sprintf(buf,"%2d  `W%-20s`w  %3d%%  ",lev,skill_table[sn].name, ch->pcdata->learned[sn]);
	  else if ( skill_table[ sn ].group == 10 )
              sprintf(buf,"%2d  `K%-20s`w  %3d%%  ",lev,skill_table[sn].name, ch->pcdata->learned[sn]);
	  else
              sprintf(buf,"%2d  %-20s  %3d%%  ",lev,skill_table[sn].name, ch->pcdata->learned[sn]);

	add_buf(output, buf);

	if ( ch->pcdata->skill_mod[sn] != 0 )
	  sprintf(buf, "%2d%%   ",ch->pcdata->skill_mod[sn]);
	else
	  sprintf(buf, "      ");
	  add_buf(output, buf);
	if ( x % 2 == 0)
	add_buf(output,"\n\r");
	}
      }
    }
    }

    /* return results */
    if (!found)
    {
      send_to_char("You know no skills.\n\r",ch);
      free_buf(output);
      return;
    }
    else
    {
    	send_to_char("Lv  Name               Learned  Mod   Lv  Name               Learned  Mod\n\r",ch); 
    	page_to_char(buf_string(output),ch);
    	free_buf(output);
    }
}

void get_weapon_improve( CHAR_DATA * ch, OBJ_DATA * obj )
{
    int type;

    if ( obj != NULL && obj->wear_data != NULL && obj->wear_data->weapon_data != NULL )
    {

	if ( number_range( 1, 100 ) < 2 && !IS_SET( obj->extra_flags, ITEM_MAGIC ) )
	{
	    obj->condition -= 1;	
	    act( "`C$p takes a little damage`w", ch, obj, NULL, TO_CHAR );
	}
	else if ( number_range( 1, 400 ) < 2 )
	{
	    obj->condition -= 1;	
	    act( "`C$p takes a little damage`w", ch, obj, NULL, TO_CHAR );
	}
    }

    if ( IS_NPC( ch ) )
	return;

    if ( obj == NULL || obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
    {
        if ( ch->pcdata->learned[ gsn_hand_to_hand[ 0 ] ] > 0 
	&& ch->pcdata->learned[ gsn_hand_to_hand[ 0 ] ] < 100 )
            check_improve( ch, gsn_hand_to_hand[ 0 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_hand_to_hand[ 0 ] ), 12 ) );

	if ( ch->pcdata->learned[ gsn_hand_to_hand[ 1 ] ] > 0 
	&& ch->pcdata->learned[ gsn_hand_to_hand[ 1 ] ] < 100 )
    check_improve( ch, gsn_hand_to_hand[ 1 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_hand_to_hand[ 1 ] ) + 1, 12 ) );

        if ( ch->pcdata->learned[ gsn_hand_to_hand[ 2 ] ] > 0 
	&& ch->pcdata->learned[ gsn_hand_to_hand[ 2 ] ] < 100 )
            check_improve( ch, gsn_hand_to_hand[ 2 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_hand_to_hand[ 2 ] ) + 2, 12 ) );
	return;
    } 
    type = obj->wear_data->weapon_data->type;

    if ( type < 0 || type >= MAX_WEAPON_TYPE )
    {
        if ( ch->pcdata->learned[ gsn_hand_to_hand[ 0 ] ] > 0 
	&& ch->pcdata->learned[ gsn_hand_to_hand[ 0 ] ] < 100 )
            check_improve( ch, gsn_hand_to_hand[ 0 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_hand_to_hand[ 0 ] ), 12 ) );

        if ( ch->pcdata->learned[ gsn_hand_to_hand[ 1 ] ] > 0 
	&& ch->pcdata->learned[ gsn_hand_to_hand[ 1 ] ] < 100 )
            check_improve( ch, gsn_hand_to_hand[ 1 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_hand_to_hand[ 1 ] ) + 1, 12 ) );

        if ( ch->pcdata->learned[ gsn_hand_to_hand[ 2 ] ] > 0 
	&& ch->pcdata->learned[ gsn_hand_to_hand[ 2 ] ] < 100 )
            check_improve( ch, gsn_hand_to_hand[ 2 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_hand_to_hand[ 2 ] ) + 2, 12 ) );
	return;
    }

    if ( ch->pcdata->learned[ gsn_weapon_power[ type ][ 0 ] ] > 0 
    && ch->pcdata->learned[ gsn_weapon_power[ type ][ 0 ] ] < 100 )
        check_improve( ch, gsn_weapon_power[ type ][ 0 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_weapon_power[ type ][ 0 ] ), 12 ) );

    if ( ch->pcdata->learned[ gsn_weapon_power[ type ][ 1 ] ] > 0 
    && ch->pcdata->learned[ gsn_weapon_power[ type ][ 1 ] ] < 100 )
        check_improve( ch, gsn_weapon_power[ type ][ 1 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_weapon_power[ type ][ 1 ] ) + 1, 12 ) );

    if ( ch->pcdata->learned[ gsn_weapon_power[ type ][ 2 ] ] > 0 
    && ch->pcdata->learned[ gsn_weapon_power[ type ][ 2 ] ] < 100 )
        check_improve( ch, gsn_weapon_power[ type ][ 2 ], TRUE,
URANGE( 1, get_skill_cost( ch, gsn_weapon_power[ type ][ 2 ] ) + 2, 12 ) );

    return;
}

/* checks for hit point increase */
void check_hp_improve( CHAR_DATA *victim, int tot_damage )
{
    int max,value,add_hp;
    char buf[100];
    AFFECT_DATA *af, *af_next;
    bool found = FALSE;

    if ( IS_NPC( victim ) )
	return;

    if ( victim->fighting && !IS_NPC( victim->fighting ) )
	return;

    for ( af = victim->affected ; af != NULL ; af = af_next )
    {
        af_next = af->next;
        if ( af->type == -2 )
	    found = TRUE;
    }

    if ( found == TRUE )
	return;

    /*
     * This is to prevent people from trying to gain too many hp
     * by exploiting dying alot.
     */

    if ( victim->pcdata->corpse != NULL && number_range( 1, 100 ) > 20 )
	return;

    max = ( victim->max_hit / 3 );
    value = ( get_curr_stat( victim, STAT_CON ) * 12 * tot_damage ) / victim->max_hit;

    if ( victim->pcdata->perm_hit >= 2000 )
	return;

    if ( tot_damage >= max )
	return;

    if ( victim->hit <= max )
	return;

    if ( victim->level <= 5 && value < number_range( 1, 100 ) )
	return;

    if ( victim->level <= 5 && victim->pcdata->perm_hit >= 100 )
	return;

    if ( victim->level > 5 && victim->level < 10 && value < number_range( 1, 150 ) )
	return;

    if ( victim->level >=10 && URANGE(1, value, 200) < number_range( 1, 250 ) )
        return;

    if ( victim-> level > 5 && victim->level < 10 && victim->pcdata->perm_hit >= 250 )
	return;

    if ( victim->level >= 10 && victim->level < 15 && victim->pcdata->perm_hit >= 350 )
	return;

    if ( victim->level >= 15 && victim->level < 20 && victim->pcdata->perm_hit >= 450 )
	return;

    if ( victim->level >= 20 && victim->level < 35 && victim->pcdata->perm_hit >= 600 )
	return;

    if ( victim->level >= 35 && victim->level < 40 && victim->pcdata->perm_hit >= 900 )
	return;

    if ( victim->level >= 40 && victim->level < 45 && victim->pcdata->perm_hit >= 1200 )
	return;

    if ( victim->level >= 45 && victim->level < 50 && victim->pcdata->perm_hit >= 1500 )
	return;

    if ( get_curr_stat( victim, STAT_CON ) > 18 )
      add_hp = number_range( 1, 3 );
    else
      add_hp = number_range( 1, 2 );


    sprintf( buf,"`MYou have become tougher!`w\n\r" );
    send_to_char( buf, victim );

    victim->max_hit         	+= add_hp;
    victim->pcdata->perm_hit    += add_hp;
    return;
}

/*check for improvement in mana*/
void check_mana_improve( CHAR_DATA *ch, int sn )
{
    int i;
    int num,temp;
    long value, value2;
    char buf[MAX_INPUT_LENGTH];
    bool found = FALSE;
    AFFECT_DATA *af, *af_next;
    
    if ( IS_NPC( ch ) )
	return;

    if ( ch->fighting && !IS_NPC( ch->fighting ) )
	return;

    for ( af = ch->affected ; af != NULL ; af = af_next )
    {
        af_next = af->next;
        if ( af->type == -2 )
	    found = TRUE;
    }

    if ( found == TRUE )
	return;

    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
	if (skill_table[sn].element[i].mana != 0 )
	{
	    value =  get_skill( ch, gsn_element_power[ i ][ 0 ] ) +
              get_skill( ch, gsn_element_power[ i ][ 1 ] ) +
              get_skill( ch, gsn_element_power[ i ][ 2 ] );
  
	    value2 = number_range( 1, value );

	    if (value2 <= 15 )
            {
		temp = mana_gain_table[get_curr_stat( ch, mana_stat[i])];
            	num = URANGE(1, number_range( 1, temp / 2 ), 10 );
		if ( ch->max_mana[ i ] >= 5000 )
		    return;

            	ch->max_mana[ i ] += num;
            	ch->pcdata->perm_mana[i] += num;
            	sprintf(buf, "`MYou gain %d more mana!`w\n\r", num);
	    	send_to_char(buf,ch);
            }
	}
    }
}

/*check for improvement in moves*/
void check_mv_improve( CHAR_DATA *ch, int move )
{
    AFFECT_DATA *af, *af_next;
    bool found = FALSE;

    if ( IS_NPC( ch ) )
	return;

    for ( af = ch->affected ; af != NULL ; af = af_next )
    {
        af_next = af->next;
        if ( af->type == -2 )
	    found = TRUE;
    }

    if ( found == TRUE )
	return;

    if (ch->pcdata->perm_move < 1000 && ch->move > 5
	&& number_range( 1, 3000 ) <= ( get_curr_stat( ch, STAT_CON ) +
        get_curr_stat( ch, STAT_DEX ) ) * move )
    {
    	send_to_char("`MYou gain more stamina!`w\n\r", ch);
        ch->pcdata->perm_move++;
        ch->move++;
        ch->max_move++;
    }
}

/* checks for skill improvement */
void check_improve( CHAR_DATA *ch, int sn, bool success, int multiplier )
{
    int chance, number_check;
    char buf[100];

    if (IS_NPC(ch))
	return;
    chance = 1;
    if ( ( skill_table[ sn ].spell_fun == spell_null
      && ch->level < get_skill_level ( ch, sn ) )
    ||  get_skill_cost( ch, sn ) < 1
    ||  ch->pcdata->learned[sn] == 0
    ||  ch->pcdata->learned[sn] == 100)
	return;  /* skill is not known or learned to max */ 

    /* check to see if the character has a chance to learn */
    if ( ch->pcdata->learned[sn] < 50 )
    {	
    	chance = 50 * int_app[get_curr_stat(ch,STAT_INT)].learn;
    	chance /= ( multiplier * get_skill_cost( ch, sn ) );
    	chance += ch->level;
    }

    if ( ch->pcdata->learned[sn] < 75 && ch->pcdata->learned[sn] >= 50 )
    {	
    	chance = 35 * int_app[get_curr_stat(ch,STAT_INT)].learn;
    	chance /= ( multiplier * get_skill_cost( ch, sn ) );
    	chance += ch->level;
    }

    if ( ch->pcdata->learned[sn] < 100 && ch->pcdata->learned[sn] >= 75 )
    {	
    	chance = 20 * int_app[get_curr_stat(ch,STAT_INT)].learn;
    	chance /= ( multiplier * get_skill_cost( ch, sn ) * 3);
    	chance += ch->level;
    }

	
    number_check = number_range(1,1000);
    if (number_check > chance)
	return;

    /* now that the character has a CHANCE to learn, see if they really have */	

    if (success)
    {
	chance = URANGE(5,ch->pcdata->learned[sn], 95);
	if (number_percent() > chance)
	{
	    sprintf(buf,"You have become better at %s!\n\r",
		    skill_table[sn].name);
	    send_to_char(buf,ch);
	    ch->pcdata->learned[sn]++;
	    gain_exp(ch,3 * get_skill_cost( ch, sn ) );

	}
    }

    else
    {
	chance = URANGE(5,ch->pcdata->learned[sn]/2,30);
	if (number_percent() < chance)
	{
	    sprintf(buf,
		"You learn from your mistakes, and your %s skill improves.\n\r",
		skill_table[sn].name);
	    send_to_char(buf,ch);
	    ch->pcdata->learned[sn] += number_range(1,3);
	    ch->pcdata->learned[sn] = UMIN(ch->pcdata->learned[sn],100);
	    gain_exp(ch,2 * get_skill_cost( ch, sn ) );
	}
    }
}
