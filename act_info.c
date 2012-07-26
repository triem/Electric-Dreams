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
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "merc.h"
#include "interp.h"

extern int top_quest;
/* for do_count */
int max_on = 0;

char * const scan_msg[3] =
{
	"very near","close by","far away"
};

char *	const	where_name	[] =
{
    "<used as light>        ",
    "<worn on finger>       ",
    "<worn on finger>       ",
    "<worn around neck>     ",
    "<worn around neck>     ",
    "<worn on body>         ",
    "<worn on head>         ",
    "<worn on legs>         ",
    "<worn on feet>         ",
    "<worn on hands>        ",
    "<worn on arms>         ",
    "<worn as shield>       ",
    "<worn about body>      ",
    "<worn about waist>     ",
    "<worn around wrist>    ",
    "<worn around wrist>    ",
    "<wielded>              ",
    "<held>                 ",
    "<wielded in two hands> ",
    "<wielded also>         ",
    "<worn on ear>          ",
    "<worn on ear>          "
};

void do_quest(CHAR_DATA *ch, char *argument)
{
    QUEST_DATA * quest_data;
    CHAR_QUESTS *char_quests;
    CHAR_DATA *victim;
    BUFFER *output;
    int Hash_loc,x;
    bool found = FALSE;
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    TRIGGER_INDEX_DATA * pTrigIndex;

    argument = one_argument( argument, arg1 );
    victim = ch;

    if ( arg1[0] != '\0' && IS_IMMORTAL(ch))
    {
    	if ( ( victim = get_char_world( ch, arg1 ) ) != NULL )
    	{
	    if ( IS_NPC(victim ) )
		victim = ch;
	    else
    		argument = one_argument( argument, arg1 );
        }
    	else
    	    victim = ch;
    }
    argument = one_argument( argument, arg2 );

    if ( is_number(arg1 ))
    {
	for ( quest_data = quest_data_list ; quest_data != NULL ; quest_data = quest_data->next )
	{
	    if ( quest_data->quest_number == atoi(arg1) )
	    {
		found = TRUE;
		break;
	    }
	}
	if (!found)
	{
	    send_to_char("Quest not found\n\r",ch);
	    return;
	}	
	if (IS_IMMORTAL(ch) )
	if ( arg2[0] == '\0' || arg1[0] == '\0' )
	{
	    send_to_char("Syntax:  Quest name <name>\n\r",ch);
	    send_to_char("Syntax:  Quest desc <desc>\n\r",ch);
	    send_to_char("Syntax:  Quest # reset\n\r",ch);
	    return;
	}	
	if (!str_cmp(arg2,"name" ) && IS_IMMORTAL(ch) )
	{
	    strncpy( quest_data->name, argument, 30 );
	    send_to_char("New name set\n\r",ch);
	    SET_BIT(changed_system, CHANGED_QUESTS);
	    return;
	}
	if (!str_cmp(arg2,"desc" )  && IS_IMMORTAL(ch))
	{
	    strncpy( quest_data->description, argument, 512 );
	    send_to_char("New desc set\n\r",ch);
	    SET_BIT(changed_system, CHANGED_QUESTS);
	    return;
	}
	if (!str_cmp(arg2,"reset" ) )
	{
	    char_quests = get_char_quest( victim, atoi(arg1) );
	    if ( !char_quests )
	    {
	        send_to_char("Quest has not been started\n\r",ch);
		return;
	    }
	    if ( IS_SET(char_quests->flags, QUEST_COMPLETED ) && !IS_IMMORTAL(ch) )
	    {
	        send_to_char("You have already completed that quest.\n\r",ch);
	        send_to_char("You cannot reset a completed quest.\n\r",ch);
		return;
	    }
	
	    char_quests->flags = 0;
	    send_to_char("Quest reset!\n\r",ch);
	    return;
	}


	/* must not have found keyword */
	if (IS_IMMORTAL(ch) )
	{
	    send_to_char("Syntax:  Quest name <name>\n\r",ch);
	    send_to_char("Syntax:  Quest desc <desc>\n\r",ch);
	    return;
	}	
	else
	{
	    send_to_char("Syntax:  Quest # reset\n\r",ch);
	    return;
	}	
	
	
    }
    if ( IS_IMMORTAL(ch ) && arg1[0] != '\0' && !str_cmp(arg1, "scan" ))
    {
	if (!str_cmp(arg1, "scan" ) )
	for (Hash_loc = 0; Hash_loc < MAX_KEY_HASH; Hash_loc++)
	{	
	    for (pTrigIndex = trig_index_hash[Hash_loc]; pTrigIndex != NULL; pTrigIndex= pTrigIndex->next)
	    {
		if (pTrigIndex->quests != 0 )
		{
		    found = FALSE;
		    for ( quest_data = quest_data_list ; quest_data != NULL ; quest_data = quest_data->next )
		    {
			if ( quest_data->quest_number == pTrigIndex->quests )
			{
			    found = TRUE;
			    break;
			}
		    }
		    if (!found)
		    {
			quest_data = quest_data_alloc(); 	
			quest_data->quest_number = pTrigIndex->quests;
			
			quest_data->next       = quest_data_list;
			quest_data_list    = quest_data;
			SET_BIT(changed_system, CHANGED_QUESTS);
		    }
		    /* add the triggers  quest_data */
		}
	    }
	} 
        send_to_char("Done\n\r",ch);
	return;
    }
    if (arg1[0] == '\0' || victim != ch) 
    {
	output = new_buf();
	if ( victim != ch )
	{
	    sprintf(buf, "Quests for %s\n\r", victim->name);
	    add_buf(output, buf);
	}
	for ( x = 1 ; x <= top_quest ; x++ )
	{
	for ( quest_data = quest_data_list ; quest_data != NULL ; quest_data = quest_data->next )
	{
	  if ( x == quest_data->quest_number )
	  {
	    char_quests = get_char_quest( victim, quest_data->quest_number );

	    if ( char_quests && char_quests->flags != 0)
	    {
		sprintf(buf,"Quest %3d  %-30s  %-12s\n\r", quest_data->quest_number, quest_data->name, IS_SET(char_quests->flags, QUEST_COMPLETED ) ? "Completed" : "In-complete");
		add_buf(output, buf);
	    }
	    else
	    {
		sprintf(buf,"Quest %3d  %-30s  %-12s\n\r", quest_data->quest_number, quest_data->name, "Not Started");
		add_buf(output, buf);
	    }
	  }
	}
	}
	page_to_char(buf_string(output),ch);
	free_buf(output);
	return;
    }
    return;
}
CHAR_QUESTS * get_char_quest(CHAR_DATA *ch, int quest_number )
{
    CHAR_QUESTS * char_quests;
    for ( char_quests = ch->char_quests; char_quests != NULL; char_quests = char_quests->next )
    {
	if ( char_quests->quest_number == quest_number )
	   return char_quests;
    }
    return NULL;
}

char *format_obj_to_char( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort )
{
    static char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    buf[0] = '\0';
    buf2[0] = '\0';
    if ( IS_SET(ch->plr2, PLR_VNUMS_ON )       )
    {
	sprintf(buf2, "{ %ld }",obj->pIndexData->vnum);
	strcat(buf, buf2);
    } 
    if ( IS_OBJ_STAT(obj, ITEM_SHADOWFORM)     )   strcat( buf, "`K(Shadowform) `W"     );
    if ( ( IS_AFFECTED(ch, AFF_DETECT_MAGIC) || IS_IMMORTAL( ch ) )
         && IS_OBJ_STAT(obj, ITEM_MAGIC)  )   strcat( buf, "`B(Magical) `W"   );
    if ( IS_OBJ_STAT(obj, ITEM_GLOW)      )   strcat( buf, "(Glowing) "   );
    if ( IS_OBJ_STAT(obj, ITEM_HUM)       )   strcat( buf, "(Humming) "   );
    if ( obj->in_room != NULL && IS_OBJ_STAT( obj, ITEM_NO_SHOW ) && IS_IMMORTAL( ch ) )  strcat( buf, "(No_Show) " );
    if ( obj->in_room != NULL && IS_OBJ_STAT( obj, ITEM_BURIED ) && IS_IMMORTAL( ch ) )  strcat( buf, "(Buried) " );
    if ( obj->light_data != NULL )
    {
	if ( IS_SET( obj->light_data->flags, LIGHT_LIT|LIGHT_MAGIC_LIT )
	  && IS_SET( obj->light_data->flags, LIGHT_BURNING ) )
	    strcat( buf, "`R(Burning) `W" );
	else if ( IS_SET( obj->light_data->flags, LIGHT_LIT ) )
	    strcat( buf, "(`YLit) `W" );
	else if ( IS_SET( obj->light_data->flags, LIGHT_MAGIC_LIT ) )
	    strcat( buf, "`C(Shining) `W" );
    }

    if ( fShort )
    {
	if ( obj->short_descr[0] != '\0' )
	    strcat( buf, obj->short_descr );
    if ( obj->wear_data != NULL 
	&& ( obj->wear_data->weapon_data != NULL || obj->wear_data->ac != 0 ) )
    {
    	if ( obj->condition == 100 )
	    strcat( buf, "	`w(Perfect Condition)" );
    	else if ( obj->condition > 85 )
	    strcat( buf, "	`w(Great Condition)" );
    	else if ( obj->condition > 70 )
	    strcat( buf, "	`w(Good Condition)" );
    	else if ( obj->condition > 55 )
	    strcat( buf, "	`w(Getting Worn)" );
    	else if ( obj->condition > 40 )
	    strcat( buf, "	`w(Worn Condition)" );
    	else if ( obj->condition > 25 )
	    strcat( buf, "	`w(Bad Condition)" );
    	else
	    strcat( buf, "	`w(Worthless Condition)" );
    	}

    	strcat( buf, "\n\r" );
    }
    else
    {
	if ( obj->description[0] != '\0' )
	    strcat( buf, obj->description );
    }

    return buf;
}

void show_char_to_char_0( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];

    buf[0] = '\0';
    buf2[0] = '\0';

    if ( IS_NPC( victim ) && IS_SET( victim->act, ACT_MOUNT ) && IS_AFFECTED( victim, AFF_MOUNTED ) && victim->master != ch )
	return;

    if ( IS_SET( victim->act2, ACT2_MOB_NO_SEE ) && !IS_IMMORTAL( ch ) )
	return;

    if ( room_is_dark(ch->in_room, ch ) && IS_AFFECTED(ch, AFF_INFRARED ) )
	send_to_char("`r", ch);
    else
    {
    	sprintf(buf,"`w"); 
    	send_to_char( buf, ch); 
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) && IS_SET(ch->plr2, PLR_VNUMS_ON)  )
    {
	sprintf(buf2, "{ %ld }", victim->pIndexData->vnum);
	strcat(buf, buf2);
    }

    if ( IS_AFFECTED(victim, AFF_SHADOWFORM)   ) 		strcat( buf, "(Shadowform) "      );
    if ( IS_SET( victim->plr, PLR_WIZINVIS ) && !IS_NPC( victim ) ) 	strcat( buf, "(Wizi) "	     );
    if ( IS_AFFECTED(victim, AFF_HIDE)		) 		strcat( buf, "(Hide) "       );
    if ( IS_AFFECTED_2(victim, AFF_FADE)	) 		strcat( buf, "(Faded) "       );
    if ( IS_AFFECTED(victim, AFF_CONTROL_MIND)  ) 		strcat( buf, "(Charmed) "    );
    if ( IS_AFFECTED(victim, AFF_DIMENSION_WALK)) 		strcat( buf, "(Translucent) ");
    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE)	) 		strcat( buf, "(Pink Aura) "  );
    if ( !IS_NPC( victim ) && !IS_NPC( ch ) && IS_SET( victim->plr, PLR_PK ) && IS_SET( ch->plr, PLR_PK ) ) strcat( buf, "(PK) " );
    if ( IS_AFFECTED(ch, AFF_DETECT_WORLD)    	)
    {
        strcat( buf, "("   );
        strcat( buf, world_name[ race_table[victim->race][victim->sex].world ] );
        strcat( buf, ") "  );
    }
    if ( IS_AFFECTED(victim, AFF_SANCTUARY)   	) 		strcat( buf, "(White Aura) " );
    if ( !IS_NPC(victim) && IS_SET(victim->plr, PLR_THIEF  ) )	strcat( buf, "(THIEF) "      );
    if ( !IS_NPC(victim) && IS_SET(victim->plr2, PLR_PLAYER_HELPER  ) ) strcat( buf, "(PH) "      );
    if ( IS_SET(victim->act2, ACT2_MOB_NO_SEE) 	)		strcat ( buf, "(Mob No See) ");
    if ( victim->position == victim->start_pos && victim->long_descr[0] != '\0' )
    {
	strcat( buf, victim->long_descr );
        if ( victim->mount != NULL && IS_AFFECTED(victim->mount,AFF_MOUNTED) )
        {
	    buf[strlen(buf)-2]='\0';
	    strcat(buf, ", while mounted on ");
	    strcat(buf, PERS( victim->mount, ch ) );
   	    strcat(buf, ".\n\r" );
        }
	send_to_char( buf, ch );
	return;
    }

    strcat( buf, PERS( victim, ch ) );
    if ( !IS_NPC(victim) && !IS_SET(ch->comm, COMM_BRIEF) )
	strcat( buf, victim->pcdata->title );

    switch ( victim->position )
    {
    case POS_DEAD:     strcat( buf, " `wis DEAD!" );               break;
    case POS_MORTAL:   strcat( buf, " `wis mortally wounded." );   break;
    case POS_INCAP:    strcat( buf, " `wis incapacitated." );      break;
    case POS_STUNNED:  strcat( buf, " `wis lying here stunned." ); break;
    case POS_SLEEPING: strcat( buf, " `wis sleeping here." );      break;
    case POS_RESTING:  strcat( buf, " `wis resting here." );       break;
    case POS_SITTING:  strcat( buf, " `wis sitting here." );	 break;
    case POS_STANDING: strcat( buf, " `wis here." );               break;
    case POS_FIGHTING:
	strcat( buf, " `wis here, fighting " );
	if ( victim->fighting == NULL )
	    strcat( buf, "thin air?" );
	else if ( victim->fighting == ch )
	    strcat( buf, "YOU!" );
	else if ( victim->in_room == victim->fighting->in_room )
	{
	    strcat( buf, PERS( victim->fighting, ch ) );
	    strcat( buf, "." );
	}
	else
	    strcat( buf, "somone who left?" );
	break;
    }

    if ( victim->mount != NULL && IS_AFFECTED(victim->mount,AFF_MOUNTED) )
    {
	buf[strlen(buf)-1]='\0';
	strcat(buf, ", while mounted on ");
	strcat(buf, PERS( victim->mount, ch ) );
	strcat(buf, "." );
    }
    strcat( buf, "\n\r" );
    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    return;
}

void show_char_to_char_1( CHAR_DATA *victim, CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int iWear;
    int percent;
    bool found;
    buf[0] = '\0';
    if ( can_see( victim, ch ) )
    {
	if (ch == victim)
	    act( "$n looks at $mself.",ch,NULL,NULL,TO_ROOM);
	else if ( ch->height * 11 / 12 > victim->height )
	{
	    act( "$n looks down at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks down at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
	else if ( ch->height < victim->height * 11 / 12 )
	{
	    act( "$n looks up at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks up at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
	else 
	{
	    act( "$n looks at you.", ch, NULL, victim, TO_VICT    );
	    act( "$n looks at $N.",  ch, NULL, victim, TO_NOTVICT );
	}
    }

    if ( victim->description[0] != '\0' )
    {
	send_to_char("`w", ch);
	send_to_char( victim->description, ch );
    }
    else
    {
	send_to_char("`Y", ch);
	act( "You see nothing special about $M.", ch, NULL, victim, TO_CHAR );
    }
    if (IS_SET(victim->act2, ACT2_MOVEABLE_AREA ) )
	{send_to_char("`w", ch);
	return;
	}

    if ( victim->max_hit > 0 )
	percent = ( 100 * victim->hit ) / victim->max_hit;
    else
	percent = -1;

    send_to_char("`G", ch);
    strcpy( buf, PERS(victim, ch) );

    if ( IS_AFFECTED( victim, AFF_BERSERK ) && !IS_IMMORTAL( ch ) )
	strcat( buf, " `Gis filled with rage.\n\r" );
    else if (percent >= 100) 
{	strcat( buf, " `Gis in excellent condition.\n\r"); }
    else if (percent >= 90) 
{
	strcat( buf, " `ghas a few scratches.\n\r");}
    else if (percent >= 75) 
{	strcat( buf," `Chas some small wounds and bruises.\n\r");}
    else if (percent >=  50) 
{	strcat( buf, " `chas quite a few wounds.\n\r");}
    else if (percent >= 30)
{	strcat( buf, " `Yhas some big nasty wounds and scratches.\n\r");}
    else if (percent >= 15)
{	strcat ( buf, " `ylooks pretty hurt.\n\r"); }
    else if (percent >= 0 )
{	strcat (buf, " `Ris in awful condition.\n\r");}
    else
{
	strcat(buf, " `ris bleeding to death.\n\r");}

    buf[0] = UPPER(buf[0]);
    send_to_char( buf, ch );
    /* Show the mood of the mob */
    if (IS_NPC(victim) && !IS_SET(victim->act2, ACT2_MOVEABLE_AREA ) )
    {
    sprintf(buf,"`Y%s %s`w\n\r",victim->short_descr ,mood_table[victim->mood].mood);
    send_to_char(buf,ch);
    }
 
    strcpy( buf, PERS(victim, ch) );
    if (is_affected(victim, gsn_fire_shield ) )
    {
	strcat (buf, " `ris encased in magical fire.\n\r");
	send_to_char(buf,ch);
    }

    if (is_affected(victim,gsn_water_shield ) )
    {
	strcat (buf, " `Bis surrounded by a large bubble of water.\n\r");
	send_to_char(buf,ch);
    }

    if (is_affected(victim,gsn_earth_shield ) )
    {
	strcat (buf, " `yis encased in a hard earthly crust.\n\r");
	send_to_char(buf,ch);
    }

    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( victim, iWear ) ) != NULL
	&&   can_see_obj( ch, obj ) )
	{
	    if ( !found )
	    {
		send_to_char( "\n\r", ch );
		cact( "$N `wis using:", ch, NULL, victim, TO_CHAR, "`C" );
		found = TRUE;
	    }
		send_to_char( "`y", ch);
	    send_to_char( where_name[iWear], ch );
		send_to_char( "`W", ch);
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "`w", ch );
	}
    }
/* blonde */
    if ( victim != ch
    &&   !IS_NPC(ch)
    &&   number_percent( ) < ch->pcdata->learned[gsn_peek] )
    {
	send_to_char( "\n\r`CYou peek at the inventory:`w\n\r", ch );
	check_improve(ch,gsn_peek,TRUE,4);
	send_to_char("`W", ch );
	show_list_to_char( victim->carrying, ch, TRUE, TRUE );
	send_to_char( "`w", ch );
    }

    return;
}

void show_char_to_char( CHAR_DATA *list, CHAR_DATA *ch )
{
    CHAR_DATA *rch;

    for ( rch = list; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch == ch )
	    continue;

	if ( !IS_NPC(rch)
	&&  !can_see(ch, rch) )
	    continue;

	if (ch->inside_area != NULL )
	if ( !str_cmp(ch->inside_area->mob_name, rch->name) )
	continue;

	if ( can_see( ch, rch ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room , ch)
	&& IS_AFFECTED(ch, AFF_INFRARED ) )
	    show_char_to_char_0( rch, ch );
	else if ( room_is_dark( ch->in_room, ch )
	&& IS_AFFECTED(rch, AFF_INFRARED ) )
	    send_to_char( "`rYou see glowing eyes.\n\r", ch );
    }

    return;
} 

void check_sensitive( CHAR_DATA *ch )
{
    ch->in_room->light_adjustment = get_light_adjustment( ch, ch->in_room );

    if ( IS_IMMORTAL(ch) )
	return;

    if ( ( ch->in_room->light_adjustment < 75 
    || IS_SET(ch->imm_flags, IMM_LIGHT) 
    || IS_SET(ch->res_flags, RES_LIGHT) )
    && ( IS_SET(ch->affected_by_2,AFF_LIGHT_SENSITIVE) ) )
    {
    	REMOVE_BIT(ch->affected_by_2, AFF_LIGHT_SENSITIVE);
    	send_to_char("The light is less severe and you can see again.\n\r",ch );
    }
    else if ( ch->in_room->light_adjustment >= 75 )
    {
	switch(check_immune(ch,DAM_LIGHT))
        {
            case(IS_IMMUNE):
		break;
            case(IS_RESISTANT):
            	break;
            case(IS_VULNERABLE):
            	SET_BIT(ch->affected_by_2, AFF_LIGHT_SENSITIVE);
            	send_to_char("The light is too bright!  You can't see anything!\n\r", ch);
            	break;
    	}
    }
    else if ( ch->in_room->light_adjustment > 50 && ch->in_room->light_adjustment < 75 )
    {
	switch(check_immune(ch,DAM_LIGHT))
        {
            case(IS_IMMUNE):
		break;
            case(IS_RESISTANT):
            	break;
            case(IS_VULNERABLE):
		send_to_char("The light is beginning to make your eyes water.\n\r", ch);
		break;
	}
    }
    return;
}

bool check_blind( CHAR_DATA *ch )
{

    if (!IS_NPC(ch) && IS_SET(ch->plr,PLR_HOLYLIGHT))
	return TRUE;

    if ( IS_AFFECTED(ch, AFF_BLIND) )
    { 
	send_to_char( "You can't see a thing!\n\r", ch ); 
	return FALSE; 
    }
/*
    if ( IS_SET(ch->affected_by_2, AFF_LIGHT_SENSITIVE) )
    {
	return FALSE;
    }
*/
    return TRUE;
}

void do_topten(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];
    int i;
    bool found = FALSE;

    argument = one_argument( argument, buf );
    if ( !str_cmp( buf, "saying" ) )
    {
	if ( strlen( argument ) > 40 )
	{
	    send_to_char( "Saying is larger then 40 characters.\n\r", ch );
	    return;
	}

	if ( is_name( top_ten_list[ 0 ][ 0 ].name, ch->name ) )
	{
	    found = TRUE;
	    strncpy( top_ten_list[ 0 ][ 0 ].saying, argument, 80 );
	}

	if ( is_name( top_ten_list[ 0 ][ 1 ].name, ch->name ) )
	{
	    found = TRUE;
	    strncpy( top_ten_list[ 0 ][ 1 ].saying, argument, 80 );
	}

	if ( is_name( top_ten_list[ 0 ][ 2 ].name, ch->name ) )
	{
	    found = TRUE;
	    strncpy( top_ten_list[ 0 ][ 2 ].saying, argument, 80 );
	}

	if ( !found )
	{
	    send_to_char( "You are not on the top of the list.\n\r", ch );
	    return;
	}
	else
	{
	    send_to_char( "Saying set.\n\r", ch );
	    save_top_ten( );
	    return;
	}
    }

    send_to_char("                      - Top Ten List -\n\r\n\r",ch);
    sprintf( buf, " Best Pkiller - %s  (%d players killed)  '%s'\n\r",top_ten_list[ 0 ][ 0 ].name, top_ten_list[ 0 ][ 0 ].value, top_ten_list[ 0 ][ 0 ].saying );
    send_to_char( buf, ch );
    sprintf( buf, " Best Killer - %s  (%d mobs killed)  '%s'\n\r",top_ten_list[ 0 ][ 1 ].name, top_ten_list[ 0 ][ 1 ].value, top_ten_list[ 0 ][ 1 ].saying );
    send_to_char( buf, ch );
    sprintf( buf, " %-20s %-20s\n\r\n\r","Pkiller","Killer");
    send_to_char( buf, ch );

    for ( i = 1 ; i < 10 ; i++ )
    {
	sprintf(  buf, " %-10s %5d     %-10s %5d\n\r",
	top_ten_list[ i ][ 0 ].name,
	top_ten_list[ i ][ 0 ].value,
	top_ten_list[ i ][ 1 ].name,
	top_ten_list[ i ][ 1 ].value);
	send_to_char( buf, ch );
    }
}

#define MAX_INFO 17

void do_setinfo(CHAR_DATA *ch, char *argument)
{
int c1,c2;
char buf[MAX_STRING_LENGTH];
char keyword[MAX_INPUT_LENGTH];

const char *yesno[]={"No", "Yes"};
const char *info_name[MAX_INFO]=
 {"Login",
  "Quit",
  "Rent",
  "Disconnect",
  "Reconnect",
  "Death",
  "Bug",
  "Permission",
  "Reset",
  "Snoops",
  "Corpse",
  "Level",
  "Obj_check",
  "Zonesave",
  "Imm_power", 
  "Coders", 
  "helps"}; 
bool info_type[MAX_INFO]=
 { 1,
   1,
   1,
   1,
   1,
   0,
   1,
   1,
   1,
   1,
   1,
   0,
   1,
   1,
   1,
   1,
   1};
const char *info_desc[MAX_INFO]=
 {"someone enters the game",
  "someone quits",
  "someone rents",
  "someone loses their link",
  "someone reconnects their link",
  "someone dies",
  "a bug is logged",
  "permission is given to edit a zone",
  "a zone resets",
  "someone snoops",
  "someone corpse is saved",
  "someone levels",
  "a object should be checked",
  "a zone is saved (automatic)",
  "Imm power log",  
  "Coders",
  "Help not found"};  
  long info_val[MAX_INFO]=
 {INFOACT_LOGIN,
  INFOACT_QUIT,
  INFOACT_RENT,
  INFOACT_LINK,
  INFOACT_RECONNECT,
  INFOACT_DEATH,
  INFOACT_BUG,
  INFOACT_PERMISSION,
  INFOACT_RESET,
  INFOACT_SNOOPS,
  INFOACT_CORPSE,
  INFOACT_LEVEL,
  INFOACT_OBJ_CHECK,
  INFOACT_SAVE_ZONE,
  INFOACT_IMP_LOG,   
  INFOACT_CODER,
  INFOACT_HELP};   


   
if (argument[0]==0)
  {
  send_to_char("Keyword    Trigger                            Set\n\r",ch);
  send_to_char("---------- ---------------------------------- ---\n\r",ch);
  for (c1=0;c1<MAX_INFO;c1++)
    {
    if ( !IS_IMMORTAL(ch) && info_type[c1] == TRUE )
	continue;

    if (IS_SET(ch->infoact,info_val[c1])) c2=1; else c2=0;
    sprintf(buf,"%-10s %-34s %-3s\n\r",info_name[c1],info_desc[c1],yesno[c2]);
    send_to_char(buf,ch);
    }
  return;
  }

argument=one_argument(argument,keyword);

if (!strcasecmp(argument,"No")||!strcasecmp(argument,"N")) c2=0;
  else if (!strcasecmp(argument,"Yes")||!strcasecmp(argument,"Y")) c2=1;
    else if (!strcasecmp(argument,"Toggle")||!strcasecmp(argument,"T")) c2=2; 
      else  c2 = 2;

for (c1=0;c1<MAX_INFO;c1++)
  if (!strcasecmp(keyword,info_name[c1]) && (info_type[c1] == FALSE || IS_IMMORTAL(ch)))
    {
    switch (c2)
      {
      case 0: REMOVE_BIT(ch->infoact,info_val[c1]);    break;
      case 1: SET_BIT(ch->infoact,info_val[c1]);       break;
      case 2: TOGGLE_BIT(ch->infoact,info_val[c1]);    break;
      }
    if (IS_SET(ch->infoact,info_val[c1])) 
      sprintf(buf,"Info WILL report when %s.\n\r",info_desc[c1]);
     else
      sprintf(buf,"Info WILL NOT report when %s.\n\r", info_desc[c1]);
    send_to_char(buf,ch);
    return;
    }
send_to_char("There is no info section with that keyword.\n\r",ch);
return;
}

/* changes your scroll */
void do_scroll(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    int lines;

    one_argument(argument,arg);
    
    if (arg[0] == '\0')
    {
	if (ch->lines == 0)
	    send_to_char("You do not page long messages.\n\r",ch);
	else
	{
	    sprintf(buf,"You currently display %d lines per page.\n\r",
		    ch->lines + 2);
	    send_to_char(buf,ch);
	}
	return;
    }

    if (!is_number(arg))
    {
	send_to_char("You must provide a number.\n\r",ch);
	return;
    }

    lines = atoi(arg);

    if (lines == 0)
    {
        send_to_char("Paging disabled.\n\r",ch);
        ch->lines = 0;
        return;
    }

    if (lines < 10 || lines > 100)
    {
	send_to_char("You must provide a reasonable number.\n\r",ch);
	send_to_char("Choose 0 for no paging\n\r",ch);
	return;
    }

    sprintf(buf,"Scroll set to %d lines.\n\r",lines);
    send_to_char(buf,ch);
    ch->lines = lines - 2;
}

/* RT does socials */
void do_socials(CHAR_DATA *ch, char *argument)
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    int iSocial;
    int col;
    bool found = FALSE; 
    col = 0;
    output = new_buf();
  if (argument[0] == '\0') 
    {
      for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
      { 
	sprintf(buf,"%-12s",social_table[iSocial].name);
	add_buf(output,buf);
	if (++col % 6 == 0)
	 add_buf(output,"\n\r");
       }
        send_to_char( "`Y", ch);
	page_to_char(buf_string(output),ch);
	free_buf(output);
        send_to_char( "`w", ch);
     return;
     }
      for (iSocial = 0; social_table[iSocial].name[0] != '\0'; iSocial++)
	if ( is_name(argument, social_table[iSocial].name ) )
	 {
	  found = TRUE;
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].name);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_no_arg);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_no_arg);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_found);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_found);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].vict_found);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_not_found);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].char_auto);
	  add_buf(output, buf);
	  sprintf(buf,"%-12s\n\r",social_table[iSocial].others_auto);
	  add_buf(output, buf);
	 } 
     if (!found)
	{
	send_to_char("Social not found\n\r",ch);
	free_buf(output);
	return;
	}
	send_to_char( "`Y", ch);
	page_to_char(buf_string(output),ch);
	free_buf(output);
	send_to_char( "`w", ch);
    return;
}

 
/* RT Commands to replace news, motd, imotd, etc from ROM */

void do_news(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"news");
}

void do_motd(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"motd");
}

void do_imotd(CHAR_DATA *ch, char *argument)
{  
    do_help(ch,"imotd");
}

void do_rules(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"rules");
}

void do_story(CHAR_DATA *ch, char *argument)
{
    do_help(ch,"story");
}

void do_history(CHAR_DATA *ch, char *argument)
{
     int x;
     char buf[MAX_INPUT_LENGTH];

    sprintf(buf, " !-> %s\n\r",ch->desc->inlast[0]);
    send_to_char(buf, ch);
     for ( x = 1; x < MAX_HISTORY; x++ )
    {
	sprintf(buf, "%2d-> %s\n\r",x, ch->desc->inlast[x]);
	send_to_char(buf, ch);
    }
}

/* RT this following section holds all the auto commands from ROM, as well as
   replacements for config */

void do_autolist(CHAR_DATA *ch, char *argument)
{
    if ( IS_NPC( ch ) )
	return;

    /* lists most player flags */
    if (IS_NPC(ch))
      return;

    send_to_char("   action     status\n\r",ch);
    send_to_char("---------------------\n\r",ch);
 
    send_to_char("autoassist     ",ch);
    if (IS_SET(ch->plr,PLR_AUTOASSIST))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch); 

    send_to_char("autodirs       ",ch);
    if (IS_SET(ch->plr,PLR_AUTODIRS))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoexit       ",ch);
    if (IS_SET(ch->plr,PLR_AUTOEXIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("automap        ",ch);
    if (IS_SET(ch->plr,PLR_AUTOMAP))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autogold       ",ch);
    if (IS_SET(ch->plr,PLR_AUTOGOLD))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autoloot       ",ch);
    if (IS_SET(ch->plr,PLR_AUTOLOOT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosac        ",ch);
    if (IS_SET(ch->plr,PLR_AUTOSAC))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("autosplit      ",ch);
    if (IS_SET(ch->plr,PLR_AUTOSPLIT))
        send_to_char("ON\n\r",ch);
    else
        send_to_char("OFF\n\r",ch);

    send_to_char("infochan       ",ch);
    if (IS_SET(ch->comm,COMM_INFO))
	send_to_char("ON  ( Use 'infoset' to set ) \n\r",ch);
    else
	send_to_char("OFF ( Use 'Infoset' to set ) \n\r",ch);

    send_to_char("combine items  ",ch);
    if (IS_SET(ch->comm,COMM_COMBINE))
	send_to_char("ON\n\r",ch);
    else
	send_to_char("OFF\n\r",ch);

    if (!IS_SET(ch->plr,PLR_CANLOOT))
	send_to_char("noloot         ON  ( Your corpse is safe from thieves. )\n\r",ch);
    else 
        send_to_char("noloot         OFF ( Your corpse may be looted. )\n\r",ch);

    if (IS_SET(ch->plr,PLR_NOSUMMON))
	send_to_char("nosummon       ON  ( You cannot be summoned. )\n\r",ch);
    else
	send_to_char("nosummon       OFF ( You can be summoned. )\n\r",ch);
   
    if (IS_SET(ch->plr,PLR_NOFOLLOW))
	send_to_char("nofollow       ON  ( You do not welcome followers. )\n\r",ch);
    else
	send_to_char("nofollow       OFF ( You accept followers. )\n\r",ch);

    if ( IS_SET(ch->plr,PLR_AUTOPROMPT) )
	send_to_char("autoprompt     ON  ( Your prompt will switch into and out of combat )\n\r", ch );
    else
	send_to_char("autoprompt     OFF ( Your prompt stays the same )\n\r", ch );

    if ( IS_SET(ch->plr2,PLR_NOMISS) )
	send_to_char("nomiss         ON  ( You don't see misses. )\n\r", ch );
    else
	send_to_char("nomiss         OFF ( You see all misses. )\n\r", ch );
}

void do_autoassist(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
    
    if (IS_SET(ch->plr,PLR_AUTOASSIST))
    {
      send_to_char("Autoassist removed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTOASSIST);
    }
    else
    {
      send_to_char("You will now assist when needed.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTOASSIST);
    }
}

void do_autodirs(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;

    if (IS_SET(ch->plr,PLR_AUTODIRS))
    {
      send_to_char("Exit directions will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTODIRS);
    }
    else
    {
      send_to_char("Exit directions will now be displayed.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTODIRS);
    }
}


void do_automap(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_AUTOMAP))
    {
      send_to_char("Map will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTOMAP);
    }
    else
    {
      send_to_char("Map will now be displayed.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTOMAP);
    }
}
void do_autoexit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_AUTOEXIT))
    {
      send_to_char("Exits will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTOEXIT);
    }
    else
    {
      send_to_char("Exits will now be displayed.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTOEXIT);
    }
}

void do_roomstats(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_ROOMSTATS))
    {
      send_to_char("Roomstats will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_ROOMSTATS);
    }
    else
    {
      send_to_char("Roomstats will now be displayed.\n\r",ch);
      SET_BIT(ch->plr,PLR_ROOMSTATS);
    }
}

void do_sectortype(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_SECTORSTATS))
    {
      send_to_char("Sector stats will no longer be displayed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_SECTORSTATS);
    }
    else
    {
      send_to_char("Sector stats will now be displayed.\n\r",ch);
      SET_BIT(ch->plr,PLR_SECTORSTATS);
    }
}

void do_autogold(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_AUTOGOLD))
    {
      send_to_char("Autogold removed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTOGOLD);
    }
    else
    {
      send_to_char("Automatic gold looting set.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTOGOLD);
    }
}

void do_autoloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_AUTOLOOT))
    {
      send_to_char("Autolooting removed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTOLOOT);
    }
    else
    {
      send_to_char("Automatic corpse looting set.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTOLOOT);
    }
}

void do_autosac(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_AUTOSAC))
    {
      send_to_char("Autosacrificing removed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTOSAC);
    }
    else
    {
      send_to_char("Automatic corpse sacrificing set.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTOSAC);
    }
}

void do_autosplit(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_AUTOSPLIT))
    {
      send_to_char("Autosplitting removed.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_AUTOSPLIT);
    }
    else
    {
      send_to_char("Automatic gold splitting set.\n\r",ch);
      SET_BIT(ch->plr,PLR_AUTOSPLIT);
    }
}

void do_brief(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_BRIEF))
    {
      send_to_char("Full descriptions activated.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_BRIEF);
    }
    else
    {
      send_to_char("Short descriptions activated.\n\r",ch);
      SET_BIT(ch->comm,COMM_BRIEF);
    }
}

void do_set_color(CHAR_DATA *ch, char *argument)
{
    TOGGLE_BIT(ch->plr, PLR_ANSI);
    ch->color = !ch->color;
    if (!ch->color)
    {
	send_to_char( "`Y", ch);
	send_to_char("Color is now off.\n\r",ch);
	send_to_char( "`w", ch);
    }
    else
    {
	send_to_char( "`Y", ch);
	send_to_char("Color is now on.\n\r",ch);
	send_to_char( "`w", ch);
    }
}
void do_compact(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMPACT))
    {
      send_to_char("Compact mode removed.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMPACT);
    }
    else
    {
      send_to_char("Compact mode set.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMPACT);
    }
}

void do_prompt(CHAR_DATA *ch, char *argument)
{
   if (!IS_NPC(ch)) 
   {
        if( !str_cmp( argument, "default" ) )
            strncpy( ch->pcdata->prompt, "`yHp %c`y/`w%H `yMv %d`y/`w%V`y-> ", 160 );
   	else if( !str_cmp( argument, "combat" ) )
           strncpy( ch->pcdata->prompt, "`gTank:`w %l  `rEnemy:`w %q%#`yHp %c`y/`w%H `yMv %d`y/`w%V`y-> ", 160 );
	else if ( !str_cmp( argument, "combat2" ) )
	    strncpy( ch->pcdata->prompt, "`GTank:`w %b  `rEnemy:`w %B%#`yHp %c`y/`w%H `yMv %d`y/`w%V`y-> ", 160 );
   	else if( !str_cmp( argument, "on" ) )
	{
	   if (ch->pcdata->Oprompt[0] != '\0')
	   	strncpy( ch->pcdata->prompt, ch->pcdata->Oprompt, 160 );
	   else
	   	strncpy( ch->pcdata->prompt, "`gTank:`w %l  `rEnemy:`w %e%#[%W] <%i`y/`W%H `wHP %n`y/`W%M `wMP %w`y/`W%V `wMV `y>", 160 );
	}
   	else if( !str_cmp( argument, "off" ) )
	{
	   ch->pcdata->prompt[0] = '\0';
	}
   	else if( !strncmp( argument, "original",3 ) )
	{
	   if (ch->pcdata->Oprompt[0] != '\0')
	   	strncpy( ch->pcdata->prompt, ch->pcdata->Oprompt, 160 );
	   else
	   {
	   	send_to_char("`YOriginal prompt not set.`w\n\r", ch);
	   	return;
	   }
		
	}

	else if (argument[0] != '\0')
	{
	   strncpy( ch->pcdata->prompt, argument, 160 );
	   strncpy( ch->pcdata->Oprompt, argument, 160 );
	}

	else
	{
		send_to_char("`Gprompt combat`w\n\r", ch);
           	send_to_char("Example: ``wgTank:``ww %l  ``wrEnemy:``ww %q%#``wBHp %c``wB/``ww%H ``wBMv %d``wB/``ww%V``wB-> \n\r",ch);
		send_to_char("`Gprompt default`w\n\r", ch);
                send_to_char("Example:  ``wBHp %c``wB/``ww%H ``wBMv %d``wB/``ww%V``wB-> \n\r",ch);
		send_to_char("`Gprompt original`w\n\r", ch);
		send_to_char("`Gprompt on`w\n\r", ch);
		send_to_char("`Gprompt off`w\n\r", ch);
		send_to_char("`GType 'help prompt' for more info.`w\n\r", ch);
		return;
	}
     send_to_char("`YOk.`w\n\r", ch);
     return;
   }
   else send_to_char("Mobiles may not change thier prompts.\n\r",ch);
}

void do_combine(CHAR_DATA *ch, char *argument)
{
    if (IS_SET(ch->comm,COMM_COMBINE))
    {
      send_to_char("Long inventory selected.\n\r",ch);
      REMOVE_BIT(ch->comm,COMM_COMBINE);
    }
    else
    {
      send_to_char("Combined inventory selected.\n\r",ch);
      SET_BIT(ch->comm,COMM_COMBINE);
    }
}

void do_noloot(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_CANLOOT))
    {
      send_to_char("Your corpse is now safe from thieves.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_CANLOOT);
    }
    else
    {
      send_to_char("Your corpse may now be looted.\n\r",ch);
      SET_BIT(ch->plr,PLR_CANLOOT);
    }
}

void do_nomiss(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr2,PLR_NOMISS))
    {
      send_to_char("You now see all misses in combat.\n\r",ch);
      REMOVE_BIT(ch->plr2,PLR_NOMISS);
    }
    else
    {
      send_to_char("You now don't see any misses in combat.\n\r",ch);
      SET_BIT(ch->plr2,PLR_NOMISS);
    }
}

void do_nofollow(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
      return;
 
    if (IS_SET(ch->plr,PLR_NOFOLLOW))
    {
      send_to_char("You now accept followers.\n\r",ch);
      REMOVE_BIT(ch->plr,PLR_NOFOLLOW);
    }
    else
    {
      send_to_char("You no longer accept followers.\n\r",ch);
      SET_BIT(ch->plr,PLR_NOFOLLOW);
      die_to_followers( ch );
    }
}

void do_nosummon(CHAR_DATA *ch, char *argument)
{
    if (IS_NPC(ch))
    {
      if (IS_SET(ch->imm_flags,IMM_SUMMON))
      {
	send_to_char("You are no longer immune to summon.\n\r",ch);
	REMOVE_BIT(ch->imm_flags,IMM_SUMMON);
      }
      else
      {
	send_to_char("You are now immune to summoning.\n\r",ch);
	SET_BIT(ch->imm_flags,IMM_SUMMON);
      }
    }
    else
    {
      if (IS_SET(ch->plr,PLR_NOSUMMON))
      {
        send_to_char("You are no longer immune to summon.\n\r",ch);
        REMOVE_BIT(ch->plr,PLR_NOSUMMON);
      }
      else
      {
        send_to_char("You are now immune to summoning.\n\r",ch);
        SET_BIT(ch->plr,PLR_NOSUMMON);
      }
    }
}

void do_look( CHAR_DATA *ch, char *argument )
{
    char buf  [MAX_STRING_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    EXIT_DATA *pexit;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    OBJ_DATA *container;
    ROOM_INDEX_DATA *pRoom;
    char *pdesc;
    int door, chance;
    int number,count;
    bool found = FALSE, trig;
   ROOM_INDEX_DATA *location;
   ROOM_INDEX_DATA *original;
   SPECPRO_LIST *s;
    if ( ch->desc == NULL )
	return;
    if ( ch->position < POS_SLEEPING )
    {
	send_to_char( "You can't see anything but stars!\n\r", ch );
	return;
    }

    if ( ch->position == POS_SLEEPING )
    {
	send_to_char( "You see beautiful white sheep jumping over a picket fence!\n\r", ch );
	return;
    }
/*
    check_sensitive( ch );
*/
    if ( !check_blind( ch ) )
	return;

    if ( !IS_NPC(ch)
    &&   !IS_SET(ch->plr, PLR_HOLYLIGHT)
    &&   room_is_dark( ch->in_room, ch ) )
    {
	
	if ( IS_SET(ch->in_room->room_flags, ROOM_DARK ) )
	{
	    send_to_char( "It's darker then the darkest night.\n\r", ch );
	    return;
	}

	if ( !IS_SET(ch->affected_by, AFF_DARK_VISION ) )
	{
	    send_to_char( "It is pitch black ... \n\r", ch );
	    if (IS_AFFECTED(ch, AFF_INFRARED) )
		show_char_to_char( ch->in_room->people,   ch );

	    return;
	}
    }

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    number = number_argument(arg1,arg3);
    count = 0;
    if ( arg1[0] == '\0' || !str_cmp( arg1, "auto" ) )
    {
	/* 'look' or 'look auto' */
        if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_ROOMSTATS) )
        {
            /* send_to_char("\n\r",ch); */
            do_show_roomstats( ch, "" );
        }
        if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_SECTORSTATS) )
        {
            /* send_to_char("\n\r",ch); */
            do_show_sector_type( ch, "" );
        }
	if ( ch->in_room->sector_type == SECT_CITY
 	|| ch->in_room->sector_type == SECT_NONE )
 	{
	    if ( ch->in_room->area->world  == 0 )
 	    	send_to_char("`G", ch);
	    else if (  ch->in_room->area->world == 1 )
 	    	send_to_char("`m", ch);
	    else if (  ch->in_room->area->world == 2 )
 	    	send_to_char("`y", ch);
	    else if ( ch->in_room->area->world == 3 )
 	    	send_to_char("`W", ch);
	}
	else if ( ch->in_room->sector_type == SECT_FIELD )
 	    send_to_char("`g", ch);
	else if ( ch->in_room->sector_type == SECT_HILLS )
 	    send_to_char("`g", ch);
	else if ( ch->in_room->sector_type == SECT_AIR )
 	    send_to_char("`w", ch);
	else if ( ch->in_room->sector_type == SECT_LAVA )
 	    send_to_char("`R", ch);
	else if ( ch->in_room->sector_type == SECT_SWAMP )
 	    send_to_char("`y", ch);
	else if ( ch->in_room->sector_type == SECT_SNOW )
 	    send_to_char("`w", ch);
	else if ( ch->in_room->sector_type == SECT_DIRT )
 	    send_to_char("`c", ch);
	else if ( ch->in_room->sector_type == SECT_SAND )
 	    send_to_char("`C", ch);
	else if ( ch->in_room->sector_type == SECT_GRASS )
 	    send_to_char("`g", ch);
	else if ( ch->in_room->sector_type == SECT_ICE )
 	    send_to_char("`w", ch);
	else if ( ch->in_room->sector_type == SECT_DESERT )
 	    send_to_char("`y", ch);
	else if ( ch->in_room->sector_type == SECT_MOUNTAIN )
 	    send_to_char("`w", ch);
	else if ( ch->in_room->sector_type == SECT_FOREST )
 	    send_to_char("`y", ch);
	else if ( ch->in_room->sector_type == SECT_UNDERWATER )
 	    send_to_char("`b", ch);
	else if ( ch->in_room->sector_type == SECT_OCEAN_BOTTOM )
	    send_to_char("`G", ch);
	else if ( ch->in_room->sector_type == SECT_UNDERGROUND )
 	    send_to_char("`W", ch);
	else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
 	    send_to_char("`C", ch);
	else if ( ch->in_room->sector_type == SECT_WATER_NOSWIM )
 	    send_to_char("`C", ch);
	else if ( ch->in_room->sector_type == SECT_STREAM )
 	    send_to_char("`C", ch);
	else
	{
	    sprintf(buf,"`G");
	    send_to_char( buf, ch); 
	}
	send_to_char( ch->in_room->name, ch );
	send_to_char( "`g",ch );
	send_to_char( "\n\r",ch );
	if ( arg1[0] == '\0'
	|| ( !IS_NPC(ch) && !IS_SET(ch->comm, COMM_BRIEF) ) )
	{
	if ( ch->in_room->sector_type == SECT_CITY
 	|| ch->in_room->sector_type == SECT_NONE )
 	{
	    if (  ch->in_room->area->world == 0 )
 	    	send_to_char("`B", ch);
	    else if (  ch->in_room->area->world == 1 )
 	    	send_to_char("`c", ch);
	    else if (  ch->in_room->area->world == 2 )
 	    	send_to_char("`Y", ch);
	    else if (  ch->in_room->area->world == 3 )
 	    	send_to_char("`K", ch);
	}
	else if ( ch->in_room->sector_type == SECT_FIELD )
 	    send_to_char("`G", ch);
	else if ( ch->in_room->sector_type == SECT_HILLS )
 	    send_to_char("`G", ch);
	else if ( ch->in_room->sector_type == SECT_AIR )
 	    send_to_char("`C", ch);
	else if ( ch->in_room->sector_type == SECT_LAVA )
 	    send_to_char("`r", ch);
	else if ( ch->in_room->sector_type == SECT_SWAMP )
 	    send_to_char("`g", ch);
	else if ( ch->in_room->sector_type == SECT_SNOW )
 	    send_to_char("`w", ch);
	else if ( ch->in_room->sector_type == SECT_ICE )
	    send_to_char("`C", ch );
	else if ( ch->in_room->sector_type == SECT_DIRT )
 	    send_to_char("`y", ch);
	else if ( ch->in_room->sector_type == SECT_SAND )
 	    send_to_char("`W", ch);
	else if ( ch->in_room->sector_type == SECT_GRASS )
 	    send_to_char("`G", ch);
	else if ( ch->in_room->sector_type == SECT_DOCK )
 	    send_to_char("`y", ch);
	else if ( ch->in_room->sector_type == SECT_DESERT )
 	    send_to_char("`Y", ch);
	else if ( ch->in_room->sector_type == SECT_MOUNTAIN )
 	    send_to_char("`K", ch);
	else if ( ch->in_room->sector_type == SECT_FOREST )
 	    send_to_char("`g", ch);
	else if ( ch->in_room->sector_type == SECT_UNDERWATER )
 	    send_to_char("`B", ch);
	else if ( ch->in_room->sector_type == SECT_OCEAN_BOTTOM )
	    send_to_char("`B", ch);
	else if ( ch->in_room->sector_type == SECT_UNDERGROUND )
 	    send_to_char("`K", ch);
	else if ( ch->in_room->sector_type == SECT_WATER_SWIM )
 	    send_to_char("`B", ch);
	else if ( ch->in_room->sector_type == SECT_WATER_NOSWIM )
 	    send_to_char("`B", ch);
	else if ( ch->in_room->sector_type == SECT_STREAM )
 	    send_to_char("`B", ch);
	else
	{
	    sprintf(buf,"`g");
	    send_to_char( buf, ch); 
	}
	    send_to_char( "  ",ch);
	    send_to_char( ch->in_room->description, ch );
	}
	
	if ( IS_SET(ch->in_room->room_flags,ROOM_SAFE) )
        send_to_char( "  `MYou feel safe here.`w\n\r",ch);
    
    if ( IS_SET(ch->in_room->room_flags,ROOM_MANAGEN) 
      && IS_SET(ch->in_room->room_flags,ROOM_HPGEN)
      && IS_SET(ch->in_room->room_flags,ROOM_MVGEN) )
        send_to_char( "  `CYou feel restful here.`w\n\r",ch);

        for ( s = ch->in_room->spro ; s != NULL ; s = s->next )
	if ( s->sp->type == 'X' || s->sp->type == 'Y' ) 
        { 
            if ( ch->inside_area == NULL ) /* oops! Summon, Or just starting the game.. This may be NULL But we need to set */
 	    {
	  	for ( victim = char_list; victim != NULL; victim = victim->next )
	  	{
	   	    if ( victim->moveable != NULL )
	    	        if ( victim->moveable->area_start_room != 0 ) /* This is a possible moveable area we need to check */
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

            if ( ch->inside_area != NULL ) /* Moveable has no start room or is nolonger in the game */
      		if ( ( location = find_location(ch, ch->inside_area->mob_name) )  != NULL )
        	{
		    original = ch->in_room;
        	    char_from_room ( ch );
       	 	    char_to_room( ch, location );
		    if ( s->sp->type == 'X' ) /* leaveable room */
	 	    {
	 		send_to_char( "Travelling through ",ch);
         		send_to_char( ch->in_room->name, ch); 
	 		send_to_char( "\n\r",ch);
	 	    }

		    if ( s->sp->type == 'Y' ) /* steering room */
	 	    {
	 		send_to_char( "From this room you can see:\n\r ",ch);
         		send_to_char( ch->in_room->description, ch); 
			show_char_to_char( ch->in_room->people,   ch );
        		if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_AUTOEXIT) )
        	    	{
		   	    /* do_scan(ch, "moveable" ); */
        	    	    do_exits( ch, "moveable" );
        		}
	 	    }

		    char_from_room ( ch );
		    char_to_room ( ch, original );
		}
      	}

	sprintf( buf, "`W" );
	send_to_char( buf, ch ); 
	if ( ch->in_room->contents )
	    show_list_to_char( ch->in_room->contents, ch, FALSE, FALSE );

	sprintf( buf, "`W" );
	send_to_char( buf, ch ); 

	if ( ch->in_room->people )
	    show_char_to_char( ch->in_room->people, ch );

        if ( !IS_NPC( ch ) && IS_SET( ch->plr, PLR_AUTODIRS ) )
            do_exits( ch, "auto" );

        if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_AUTOMAP) )
            do_map( ch, "" );

        if ( !IS_NPC(ch) && IS_SET(ch->plr, PLR_AUTOEXIT) )
            do_exits( ch, "" );
 
	for( obj = ch->in_room->contents ; obj != NULL; obj = obj->next_content )
	{
	    if ( IS_SET(obj->extra_flags, ITEM_BURIED ) )
	    { 
		chance = get_skill( ch, gsn_awareness ) + (IS_SET(ch->affected_by_2, AFF_SENSES ) ? ch->level : 0 );
		if (number_percent() < chance)
		{
		    check_improve(ch,gsn_awareness,TRUE,1);
		    send_to_char("`yThere is some freshly dug earth here.\n\r",ch);
		    break;
		}
		else
		{
		    check_improve(ch,gsn_awareness,FALSE,1);
		    break;
		}
	    }
	    
	}
 	send_to_char( "`w", ch);

    if ( arg1[0] == '\0' )
    trig = do_trigger( ch->in_room, ch, NULL, NULL, TRIG_LOOK, TRIG_TYPE_ROOM);

    return;
    }

    if ( !str_cmp( arg1, "i" ) || !str_cmp( arg1, "in" ) )
    {
	/* 'look in' */
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Look in what?\n\r", ch );
	    return;
	}

	if ( ( obj = get_obj_here( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "You do not see that here.\n\r", ch );
	    return;
	}
        trig = do_trigger( obj, ch, NULL, NULL, TRIG_LOOK, TRIG_TYPE_OBJ);

        container = obj;

	if ( obj->gate_data != NULL )
	{
	    ROOM_INDEX_DATA * to_room;
	    found = TRUE;

	    to_room = gate_leads_to( obj->gate_data );
	
	    if ( to_room == NULL )
	    {
		/* This is possible, the gate could have been to
		   a mob that was killed. Toast the gate */

		act( "`wThe $p `wis extremely unstable!.", ch, obj, NULL, TO_CHAR );
	        act ("$p `wfades away as $n `wtouches it.",ch,obj,NULL,TO_ROOM);	
	        act ("$p `wfades away as you touch it.",ch,obj,NULL,TO_CHAR);	
		extract_obj(obj);
		return;
	    }

	    if ( !IS_SET( obj->gate_data->flags, GATE_OPAQUE ) )
		act( "You cannot see through $p.", ch, obj, NULL, TO_CHAR );
	    else 
	    {
		sprintf( buf, "%s\n\r", to_room->name );
		send_to_char( buf, ch );
	    }
	}

	if ( obj->edible_data != NULL && !IS_SET( obj->edible_data->flags, EDIBLE_FOOD ) )
	{
	    found = TRUE;
	    if ( obj->edible_data->amount <= 0 )
		act( "$p `wis empty.", ch, obj, NULL, TO_CHAR );
	    else
	    {
	        sprintf( buf, "It's %s full of a %s liquid.\n\r",
		obj->edible_data->amount <     obj->edible_data->max_amount / 4
		    ? "less than" :
		obj->edible_data->amount < 3 * obj->edible_data->max_amount / 4
		    ? "about"     : "more than",
		liq_table[obj->edible_data->type].liq_color
		);

	        send_to_char( buf, ch );
	    }
	}

	if ( obj->cont_data != NULL )
	{
	    found = TRUE;

	    if ( IS_SET( obj->cont_data->flags, CONT_CLOSED) )
		send_to_char( "It is closed.\n\r", ch );
	    else
	    {
	    	act( "$p contains:", ch, obj, NULL, TO_CHAR );
	    	show_list_to_char( obj->cont_data->contents, ch, TRUE, TRUE );
	    }
	}
	if(!found)
	send_to_char( "You can't look in that!\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) != NULL )
    {
	show_char_to_char_1( victim, ch );
        trig = do_trigger( victim, ch, NULL, NULL, TRIG_LOOK, TRIG_TYPE_MOB);
	return;
    }

    for ( obj = ch->carrying; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( can_see_obj( ch, obj ) )
	{
        trig = do_trigger( obj, ch, NULL, NULL, TRIG_LOOK, TRIG_TYPE_OBJ);
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    {
	    	if (++count == number)
	    	{
		    page_to_char( pdesc, ch );
		    return;
	    	}
	    	else 
		    continue;
	    }
 	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
 	    if ( pdesc != NULL )
	    {
 	    	if (++count == number)
 	    	{	
		    page_to_char( pdesc, ch );
		    return;
	     	}
		else 
		    continue;
	    }

	    if ( is_name( arg3, obj->name ) )
	    	if (++count == number)
	    	{
	    	    send_to_char( obj->description, ch );
	    	   return;
	    	}
	}
    }

    for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
    {
	obj_next = obj->next_content;
	if ( can_see_obj( ch, obj ) )
	{
        trig = do_trigger( obj, ch, NULL, NULL, TRIG_LOOK, TRIG_TYPE_OBJ);
	    pdesc = get_extra_descr( arg3, obj->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    send_to_char( pdesc, ch );
		    return;
	    	}

	    pdesc = get_extra_descr( arg3, obj->pIndexData->extra_descr );
	    if ( pdesc != NULL )
	    	if (++count == number)
	    	{
		    page_to_char( pdesc, ch );
		    return;
	    	}
	}

	if ( is_name( arg3, obj->name ) )
	    if (++count == number)
	    {
	    	send_to_char( obj->description, ch );
	    	return;
	    }
    }
    
    if (count > 0 && count != number)
    {
    	if (count == 1)
    	    sprintf(buf,"You only see one %s here.\n\r",arg3);
    	else
    	    sprintf(buf,"You only see %d %s's here.\n\r",count,arg3);
    	
    	send_to_char(buf,ch);
    	return;
    }
    pdesc = get_extra_descr( arg1, ch->in_room->extra_descr );
    if ( pdesc != NULL )
    {
	send_to_char( "`W", ch);
	page_to_char( pdesc, ch );
	send_to_char( "`w", ch);
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
	send_to_char( "You do not see that here.\n\r", ch );
	return;
    }

    /* 'look direction' */
    if ( ( pexit = ch->in_room->exit[door] ) == NULL ) 
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }

    if ( !can_find_door( ch, pexit->exit_info ) )
    {
	send_to_char( "Nothing special there.\n\r", ch );
	return;
    }
    if ( pexit->description != NULL && pexit->description[0] != '\0' )
	send_to_char( pexit->description, ch );
    else if ( !IS_SET( pexit->exit_info, EX_CLOSED ) )
    {
	if ( pexit->u1.to_room != NULL && can_see_room( ch, pexit->u1.to_room ) && ( IS_SET( ch->plr, PLR_HOLYLIGHT ) || !room_is_dark( pexit->u1.to_room, ch ) ) )
	{
	    send_to_char( pexit->u1.to_room->name, ch );
	    send_to_char("\n\r",ch);
	}
	else if ( pexit->u1.to_room != NULL )
	    send_to_char( "You can't see into that room.\n\r",ch );
	else
	    send_to_char( "Nothing special there.\n\r", ch );
    }

    if ( pexit->keyword[0] != '\0'
    &&   pexit->keyword[0] != ' '  )
    {
	if ( IS_SET(pexit->exit_info, EX_CLOSED) )
	{
	    act( "The $d is closed.", ch, NULL, pexit->keyword, TO_CHAR );
	}
	else if ( IS_SET(pexit->exit_info, EX_ISDOOR) )
	{
	    act( "The $d is open.",   ch, NULL, pexit->keyword, TO_CHAR );
	}
    }

    return;
}

/* RT added back for the hell of it */
void do_read (CHAR_DATA *ch, char *argument )
{
    do_look(ch,argument);
}

void do_examine( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Examine what?\n\r", ch );
	return;
    }

    do_look( ch, arg );

    if ( ( obj = get_obj_here( ch, arg ) ) != NULL )
    {
	if ( obj->cont_data != NULL )
	{
	    send_to_char( "When you look inside, you see:\n\r", ch );
	    sprintf( buf, "in %s", arg );
	    do_look( ch, buf );
	}
    }

    return;
}



/*
 * Thanks to Zrin for auto-exit part.
 */
void do_exits( CHAR_DATA *ch, char *argument )
{
    extern char * const dir_name[];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    bool found;
    bool fAuto;
    bool fMoveable;
    int door;
    buf[0] = '\0';
    fAuto  = !str_cmp( argument, "auto" );
    fMoveable  = !str_cmp( argument, "moveable" );
/*
    check_sensitive( ch );
*/
    if ( !check_blind( ch ) )
	return;

    strcpy( buf, fAuto || fMoveable ? "[Exits:" : "Obvious exits:\n\r" );

    found = FALSE;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room)
        &&   can_find_door(ch,pexit->exit_info) 
	&&   (!IS_SET(pexit->exit_info, EX_SECRET) || 
	     !IS_SET(pexit->exit_info, EX_CLOSED)  ||
	      IS_SET(ch->plr, PLR_HOLYLIGHT) ) )
	{
	    found = TRUE;
	    if ( fAuto )
	    {
		if (!IS_SET(pexit->exit_info,EX_CLOSED))
                 {strcat( buf, " `g" );
		  strcat( buf, dir_name[door] );}
	    }
	    else if ( fMoveable )
	    {
		if (!IS_SET(pexit->exit_info,EX_CLOSED))
                 {strcat( buf, " `g" );
		  strcat( buf, dir_name[door] );}
	    }
	    else
	    {
		char buf1[MAX_INPUT_LENGTH];

		if ( IS_IMMORTAL( ch ) && IS_SET(ch->plr,PLR_HOLYLIGHT) )
		    sprintf( buf1, "[%5ld] %s", pexit->u1.to_room->vnum, pexit->u1.to_room->name );
		else
		    strcpy( buf1, pexit->u1.to_room->name );

		sprintf( buf + strlen(buf), "`g%-5s - %s  %s%s%s%s%s%s%s\n\r",
		    capitalize( dir_name[door] ), (room_is_dark( pexit->u1.to_room, ch ) 
			&& !IS_SET(ch->plr,PLR_HOLYLIGHT)
			&& !IS_SET(ch->affected_by, AFF_DARK_VISION) )
			?  "Too dark to tell" : 
			(!IS_SET(pexit->exit_info, EX_CLOSED)) 
                          ? buf1 : 
			"(Closed Doorway)",  
			 (IS_SET(pexit->exit_info, EX_HIDDEN) && ( ( IS_IMMORTAL(ch) && IS_SET(ch->plr,PLR_HOLYLIGHT) ) || IS_SET( pexit->exit_info, EX_CLOSED ) ) ) ? 
			 "(Hidden)" : "" ,
			 (IS_SET(pexit->exit_info, EX_BASHED)) ? 
			 "(Bashed)" : "" ,
			 (IS_SET(pexit->exit_info, EX_BASHPROOF) && IS_SET(ch->plr, PLR_HOLYLIGHT )) ? 
			 "(Bashproof)" : "" ,
			 (IS_SET(pexit->exit_info, EX_PASSPROOF) && IS_SET(ch->plr, PLR_HOLYLIGHT )) ? 
			 "(Passproof)" : "" ,
			 (IS_SET(pexit->exit_info, EX_WALL) && IS_SET(ch->plr, PLR_HOLYLIGHT )) ? 
			 "(Wall)" : "" ,
			 (IS_SET(pexit->exit_info, EX_ILLUSION) && IS_SET(ch->plr, PLR_HOLYLIGHT )) ? 
			 "(Illusion)" : "" ,
			 (IS_SET(pexit->exit_info, EX_SECRET)) ? 
			 "(Secret)" : "" 
		   );
	    }
	}
    }

    if ( !found )
	strcat( buf, fAuto || fMoveable ? " none" : "None.\n\r" );
    if ( fMoveable )
	{
	strcat( buf, "]\n\r" );
        send_to_char( buf, ch );
	return;
	}
    if ( fAuto )
	strcat( buf, "]\n\r" );
    sprintf(buf2,"`g");
    send_to_char( buf2, ch); 
    send_to_char( buf, ch );
    return;
}

void do_worth( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if (IS_NPC(ch))
    {
   	sprintf(buf,"You have:\n\r%ld Nenkemen crowns.\n\r%ld Maegmenel duckets\n\r%ld Lithdor wheels\n\r%ld greckles.\n\r",ch->gold[0],ch->gold[1],ch->gold[2],ch->gold[3]);
	send_to_char(buf,ch);
	return;
    }

    sprintf(buf,"You have:\n\r%ld Nenkemen crowns.\n\r%ld Maegmenel duckets\n\r%ld Lithdor wheels\n\r%ld greckles.\n\r",ch->gold[0],ch->gold[1],ch->gold[2],ch->gold[3]);
    send_to_char(buf,ch);

    return;
}

void do_map( CHAR_DATA *ch, char *argument )
{   
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *to_room;
    ROOM_INDEX_DATA *to_room_in;
    EXIT_DATA *pexit;
    char buf[MAX_INPUT_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char buf3[MAX_STRING_LENGTH];
    int door;
    int x;
    sh_int map_view[5];
    buf1[0] = '\0';
    room = ch->in_room;
    to_room = ch->in_room;
     for ( x = 0; x < 6; x++)
	map_view[x] = 0;
    for ( door = 0; door <= 5; door++ )
    {
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   can_see_room(ch,pexit->u1.to_room)
        &&   can_find_door(ch,pexit->exit_info) 
	&&   (!IS_SET(pexit->exit_info, EX_SECRET) || 
	     !IS_SET(pexit->exit_info, EX_CLOSED)  ||
	      IS_SET(ch->plr, PLR_HOLYLIGHT) ) )
	{
		to_room_in = pexit->u1.to_room;
		sprintf(buf,"%s\n\r",to_room_in->name);
		strcat(buf1,buf);
		map_view[door] = 1;	
	}
    }
sprintf(buf3,"%s%s%s%s%s%s%s%s",
map_view[0] == 1 ? "          N\n\r"        : "\n\r",
map_view[4] == 1 ? map_view[0] == 1 ? "          |  U\n\r" : "            U\n\r" : map_view[0] == 1 ?  "          |\n\r" : "\n\r",
map_view[4] == 1 ? map_view[0] == 1 ? "          | /\n\r" : "           /\n\r" : map_view[0] == 1 ?  "          |\n\r" : "\n\r",
map_view[3] == 1 ? "    W --- *"            : "          *",
map_view[1] == 1 ? " --- E\n\r"  : "\n\r",
map_view[5] == 1 ? map_view[2] == 1 ? "          | \\ \n\r"     : "            \\ \n\r" : map_view[2] == 1 ? "          |\n\r" : "\n\r",
map_view[5] == 1 ? map_view[2] == 1 ? "          |  D\n\r"     : "             D\n\r": map_view[2] == 1 ? "          |\n\r" : "\n\r",
map_view[2] == 1 ? "          S\n\r"        : "\n\r");
send_to_char(buf3,ch);
/*send_to_char(buf1,ch); */
    return;
}

void do_scan( CHAR_DATA *ch, char *argument )
{   
    CHAR_DATA *vch;
    ROOM_INDEX_DATA *room;
    ROOM_INDEX_DATA *to_room;
    EXIT_DATA *pexit;
    OBJ_DATA *obj;
    char buf[MAX_INPUT_LENGTH];
    int door;
    int num;
    int item_count;
    bool found = FALSE;
    bool fMoveable;

    if ( ch->position == POS_FIGHTING)
    {
	send_to_char("You are in battle!\n\r", ch);
	return;
    }

    fMoveable  = !str_cmp( argument, "moveable" );

    if (!fMoveable)
    act("$n looks through all the exits.\n\r",ch, NULL, NULL , TO_ROOM);

    if (!fMoveable)
    for ( vch = ch->in_room->people ; vch != NULL ; vch = vch->next_in_room )
    {
	if ( can_see( ch, vch ) && ch != vch )
	{
	    found = TRUE;
	    sprintf( buf, "%s `wis right here.\n\r", ( IS_NPC(vch) ? vch->short_descr : vch->name ) );
	    send_to_char( buf, ch );
	}
    }

    room = ch->in_room;
    for ( door = 0 ; door < 6 ; door ++ )
    {
        to_room = room;
	for ( num = 0 ; num < ( race_table[ ch->race ][ ch->sex ].scan_dist < 10 ? race_table[ ch->race ][ ch->sex ].scan_dist : 10 ) ; num++ )
	{
    	    if ( ( pexit = to_room->exit[ door ] ) != NULL && pexit->u1.to_room != NULL 
	      && can_see_room( ch, pexit->u1.to_room )
	      && !IS_SET( pexit->exit_info, EX_CLOSED )
	      && !IS_SET( pexit->exit_info, EX_WALL )
	      && !IS_SET( pexit->exit_info, EX_ILLUSION ) )
	    {
	        if ( !IS_AFFECTED( ch, AFF_INFRARED ) && !IS_AFFECTED( ch, AFF_DARK_VISION) && room_is_dark( pexit->u1.to_room, ch ) && !IS_IMMORTAL( ch ) )
		{
		    sprintf( buf, "All you can see is darkness %s to the %s.\n\r",scan_msg[ num ], dir_name[ door ] );
		    send_to_char( buf, ch );
		    num = 10; 
		}
		else
        {
	        for( vch = pexit->u1.to_room->people ; vch != NULL ; vch = vch->next_in_room )
	        {
		    if ( IS_AFFECTED( vch, AFF_HIDE ) && !IS_IMMORTAL(ch) )
			continue;
		    if ( IS_AFFECTED_2( vch, AFF_FADE ) && !IS_IMMORTAL(ch) )
			continue;

		    if ( can_see( ch, vch ) && ch != vch )
		    {
			found = TRUE;
			if (fMoveable)
		        sprintf( buf, "You see %s `wis %s %s of you.\n\r", ( IS_NPC(vch) ? vch->short_descr : vch->name ) , scan_msg[ num ], dir_name[ door ] );
			else
		        sprintf( buf, "%s `wis %s %s.\n\r", ( IS_NPC(vch) ? vch->short_descr : vch->name ) , scan_msg[ num ], dir_name[ door ] );
		        send_to_char( buf, ch );
		    }
		    else if ( ch == vch )
		    {
			found = TRUE;
			sprintf( buf, "You see yourself %s %s?\n\r",scan_msg[ num ], dir_name[ door ] );
			send_to_char( buf, ch );
		    }
	        }

            item_count = 0;
            for( obj = pexit->u1.to_room->contents ; obj != NULL ; obj = obj->next_content )
            {
                if ( ( obj->wear_data == NULL || obj->wear_data->wear_loc == WEAR_NONE ) 
                &&  can_see_obj( ch, obj ) && obj->short_descr[0] != '\0'
                && ( !IS_SET( obj->extra_flags, ITEM_NO_SHOW ) || IS_IMMORTAL( ch ) ) 
                && ( !IS_SET( obj->extra_flags, ITEM_BURIED )  || IS_IMMORTAL( ch ) ) )
                {
                    item_count++;
                    if ( item_count > MAX_SCAN_ITEMS )
                        break;
                }
            }
            if ( item_count <= MAX_SCAN_ITEMS )
            {
                for( obj = pexit->u1.to_room->contents ; obj != NULL ; obj = obj->next_content )
                {
                    if ( ( obj->wear_data == NULL || obj->wear_data->wear_loc == WEAR_NONE ) 
                    &&  can_see_obj( ch, obj ) && obj->short_descr[0] != '\0'
                    && ( !IS_SET( obj->extra_flags, ITEM_NO_SHOW ) || IS_IMMORTAL( ch ) ) 
                    && ( !IS_SET( obj->extra_flags, ITEM_BURIED )  || IS_IMMORTAL( ch ) ) )
                    {
                        sprintf( buf, "%s `wis %s %s.\n\r", obj->short_descr, scan_msg[ num ], dir_name[ door ] );
                        send_to_char( buf, ch );
                    }
                }
            }
            else
            {
                sprintf( buf, "a pile of objects are %s %s.\n\r", scan_msg[ num ], dir_name[ door ] );
                send_to_char( buf, ch );
            }
        }
	        to_room = pexit->u1.to_room;
	    }
	    else
		num = 10;
	}	
    }

    if ( !found && !fMoveable)
	send_to_char("You don't see anyone.\n\r",ch);

    return;
}



char *	const	day_name	[MAX_WORLD][7] =
{
    {"Ebb Tide", "Neap Tide", "Dal Tide", "Wan Tide", "Sun Tide",
    "Pass Tide", "Greb Tide",},
    {"the First Day", "the Second Day", "the Third day", "the Fourth Day", 
     "the Fifth Day", "the Sixth Day", "the Seventh Day",},
    {"the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun",},
    {"the Moon", "the Bull", "Deception", "Thunder", "Freedom",
    "the Great Gods", "the Sun",}
};

char *	const	month_name	[MAX_WORLD][35] =
{
    {"Dark Clouds", "Many Rains", "Rain", "High Winds",
    "Light", "Trade Winds", "Growth", "Nature", "Fertility", "the Moon",
    "the Sun", "Heat", "Soft Breezes", "the Darkening Skies", "Cool Winds",
    "Long Shadows", "Approaching Darkness", "Dark Waters"},
    {"Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "the Iron Cross", "the Silver Tear", 
    "Blood Ties", "the Sun", "the Heat", "the Battle", "the Blood Moon", 
    "the Shadows", "the Ruby Chalice", "the Lost Quest",
    "the Great Evil"},
    {"Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"},
    {"Winter", "the Winter Wolf", "the Frost Giant", "the Old Forces",
    "the Grand Struggle", "the Spring", "Nature", "Futility", "the Dragon",
    "the Sun", "the Heat", "the Battle", "the Dark Shades", "the Shadows",
    "the Long Shadows", "the Ancient Darkness", "the Great Evil"}
};

/* Code for the new score. old score follows this code */

void do_score( CHAR_DATA *ch , char *argument )
{
CHAR_DATA *victim;
CHAR_QUESTS *quests;
char buf[MAX_STRING_LENGTH];
char buf1[MAX_STRING_LENGTH];
char birth[MAX_INPUT_LENGTH];
char codedhits[20];  /* Used to create color coded hp's */
char codedmana0[20];  /* Used to create color coded mana */
char codedmana1[20];  /* Used to create color coded mana */
char codedmana2[20];  /* Used to create color coded mana */
char codedmana3[20];  /* Used to create color coded mana */
char codedmana4[20];  /* Used to create color coded mana */
char codedmove[20];  /* Used to create color coded move */
char pos[80];  /* Used Position */
char suf[5];
OBJ_DATA * wield;
int skill = 0;
int temp;
int day;
int quests_completed = 0;

temp = ch->max_hit;
victim = ch;
buf[0] = '\0';
buf1[0] = '\0';
if ( IS_IMMORTAL( ch ) && argument[0] != '\0' )
    victim = get_char_world( ch, argument );

/* Color coded hp */
if ( !victim )
{
    send_to_char( "Not found\n\r", ch );
    return;
}

if ( victim->hit < ( temp / 3 ) ) 
    strcpy( codedhits, "`R" );
else if ( victim->hit < ( ( temp / 3 ) * 2 ) ) 
    strcpy( codedhits, "`Y" );
else 
    strcpy( codedhits, "`G" );

/* how may quests completed */
for ( quests = victim->char_quests; quests != NULL; quests = quests->next )
if ( IS_SET( quests->flags, QUEST_COMPLETED ) )
    quests_completed++;

/* Color coded mana */

temp = victim->max_mana[0];
if ( victim->mana[0] < ( temp / 3 ) ) 
    strcpy( codedmana0, "`R" );
else if ( victim->mana[0] < ( ( temp / 3 ) * 2 ) ) 
    strcpy( codedmana0, "`Y" );
else 
    strcpy( codedmana0, "`G" );

temp = victim->max_mana[1];
if ( victim->mana[1] < ( temp / 3 ) ) 
    strcpy( codedmana1, "`R" );
else if ( victim->mana[1] < ( ( temp / 3 ) * 2 ) ) 
    strcpy( codedmana1, "`Y" );
else 
    strcpy( codedmana1, "`G" );

temp = victim->max_mana[2];
if ( victim->mana[2] < ( temp / 3 ) ) 
    strcpy( codedmana2, "`R" );
else if ( victim->mana[2] < ( ( temp / 3 ) * 2 ) ) 
    strcpy( codedmana2, "`Y" );
else 
    strcpy( codedmana2, "`G" );

temp = victim->max_mana[3];
if ( victim->mana[3] < ( temp / 3 ) ) 
    strcpy( codedmana3, "`R" );
else if ( victim->mana[3] < ( ( temp / 3 ) * 2 ) ) 
    strcpy( codedmana3, "`Y" );
else 
    strcpy( codedmana3, "`G" );

temp = victim->max_mana[4];
if ( victim->mana[4] < ( temp / 3 ) ) 
    strcpy( codedmana4, "`R" );
else if ( victim->mana[4] < ( ( temp / 3 ) * 2 ) ) 
    strcpy( codedmana4, "`Y" );
else 
    strcpy( codedmana4, "`G" );

temp = victim->max_move;
/* Color coded move */
if ( victim->move < ( temp / 3 ) ) 
    strcpy( codedmove, "`R" );
else if ( victim->move < ( ( temp / 3 ) * 2 ) ) 
    strcpy( codedmove, "`Y" );
else 
    strcpy( codedmove, "`G" );

/* skill bonus to hit */
    if ( ( wield = get_eq_char( victim, WEAR_WIELD_R ) ) == NULL )
	if ( ( wield = get_eq_char( victim, WEAR_WIELD_TWO_HANDED ) ) == NULL )
	    wield = get_eq_char( victim, WEAR_WIELD_L );

    if ( wield == NULL || wield->wear_data == NULL || wield->wear_data->weapon_data == NULL )
	wield = NULL;

    skill = get_weapon_skill( victim, wield );

/* Birthday Code */

    day = victim->birth_day + 1;

         if ( day > 4 && day <  20 ) strncpy( suf, "th", 4 );
    else if ( day % 10 ==  2       ) strncpy( suf, "nd", 4 );
    else if ( day % 10 ==  3       ) strncpy( suf, "rd", 4 );
    else                             strncpy( suf, "th", 4 );

    sprintf( birth, "Born: `G%d%s of the Month of %s", day, suf,
	month_name[ race_table[ victim->race ][ victim->sex ].world  ][ victim->birth_month ] );

    if ( IS_AFFECTED( victim, AFF_BERSERK ) )
    {
	strcpy( codedhits,"`G" );
	strcpy( pos, "`RYou are filled with rage.");
    }
    else
    switch ( victim->position )
    {
    case POS_DEAD:     
	strcpy(pos, "`RYou are DEAD!!");
	break;
    case POS_MORTAL:
	strcpy(pos, "`rYou are mortally wounded.");
	break;
    case POS_INCAP:
	strcpy(pos, "`YYou are incapacitated.");
	break;
    case POS_STUNNED:
	strcpy(pos, "`yYou are stunned.");
	break;
    case POS_SLEEPING:
	strcpy(pos, "`WYou are sleeping.");
	break;
    case POS_RESTING:
	strcpy(pos, "`WYou are resting.");
	break;
    case POS_STANDING:
	strcpy( pos,"`WYou are standing.");
	break;
    case POS_FIGHTING:
	strcpy(pos,  "`WYou are fighting.");
	break;
    }
/* This is fairly simple to implement, if you wish to do so.. just have
   a %s in the sprintf command which corresponds to the variable codedhit
   instead of for example ch->hit 
   this might not look good in practice, but it seems a good idea right now
   as players can, at a glace, see what roughly their health is 
  the color coded hp is implemented, and the source is above if you want
  to do something with it - it can be taken out if we dont
  want it..... oh ya and unless you didnt notice, 
  i have added an implemented spell affects underneath the table.. feel free to
  do some cosmetics on that */

/* Basic layout for the new do_score. Still a bit unstable but should be
   Ready to go as soon as I port the color codes and variables over */
send_to_char( "\n\r", ch );
sprintf( buf, "`B(:)=========================================================================(:)\n\r" );
strcat( buf1, buf );
sprintf( buf, "|=| `C%20s`B -- Attributes  (`MElectric Dreams`B)                   |=|\n\r", victim->name );
strcat( buf1, buf );
sprintf( buf, "(:)==========================(:)=========================(:)================(:)\n\r" );
strcat( buf1, buf );
sprintf( buf, "|/| `RStrength:`B        `G%2d `B<`G%2d`B> |/| `RRace:   `G%-14s`B  |=|    `CQuests      `B|/|\n\r", victim->perm_stat[STAT_STR], get_curr_stat(victim, STAT_STR), race_list[victim->race].name );
strcat( buf1, buf );
sprintf( buf, "|=| `RDexterity:`B       `G%2d `B<`G%2d`B> |=| `RLevel:  `G%-14d`B  |=| `RCompleted:   `G%-2d`B|=|\n\r",victim->perm_stat[STAT_DEX], get_curr_stat(victim, STAT_DEX), victim->level, quests_completed );
strcat( buf1, buf );
sprintf( buf, "|/| `RConstitution:`B    `G%2d `B<`G%2d`B> |/| `RHeight: `C%-3d`G' `C%-3d`G%c`B       |=| `RPoints:      `G%-2d`B|/|\n\r",victim->perm_stat[STAT_CON], get_curr_stat(victim, STAT_CON), victim->height/12,victim->height%12,34,victim->quest_points );
strcat( buf1, buf );
sprintf( buf, "|=| `RIntelligence:`B    `G%2d `B<`G%2d`B> |=| `RWeight: `C%-4d `Gpounds`B     |=|                `B|=|\n\r", victim->perm_stat[STAT_INT], get_curr_stat(victim, STAT_INT), victim->weight );
strcat( buf1, buf );
sprintf( buf, "|/| `RWisdom:`B          `G%2d `B<`G%2d`B> |/| `RAge:    `C%-14d`B  |=|                `B|/|\n\r", victim->perm_stat[STAT_WIS], get_curr_stat(victim, STAT_WIS),get_age(victim) );
strcat( buf1, buf );
sprintf( buf, "|=| `RCharisma:`B        `G%2d `B<`G%2d`B> |=| `RPrac:   `C%-14d`B  |=|                `B|=|\n\r",victim->perm_stat[STAT_CHR], get_curr_stat(victim, STAT_CHR), victim->practice );
strcat( buf1, buf );
sprintf( buf, "(:)=====================(:)==(:)=========================(:)================(:)\n\r" );
strcat( buf1, buf );
sprintf( buf, "|/|    `CStatistics`B       |/|  `CAdventures and Valuables`B    |/|     `CCombat`B     |/|\n\r" );
strcat( buf1, buf );
sprintf( buf, "|=|                     |=|                              |=|                |=|\n\r" );
strcat( buf1, buf );
sprintf( buf, "|/| `RHp:     %s%4d / `G%4d `B|/| `RCrowns:    `C%15ld`B   |/| `RHit:`B     `G%5d`B |/|\n\r",codedhits, ( IS_AFFECTED( victim, AFF_BERSERK ) ? victim->max_hit : victim->hit ), victim->max_hit, victim->gold[0], GET_HITROLL(victim) );
strcat( buf1, buf );
sprintf( buf, "|=| `RMove:   %s%4d / `G%4d `B|=| `RDuckets:   `C%15ld`B   |=| `RDam:`B     `G%5d`B |=|\n\r", codedmove, victim->move, victim->max_move, victim->gold[1], GET_DAMROLL(victim) );
strcat( buf1, buf );
sprintf( buf, "|/| `RFire:   %s%4d / `G%4d `B|/| `RWheels:    `C%15ld`B   |/| `RAC:`B      `G%5d`B |/|\n\r", codedmana0, victim->mana[0], victim->max_mana[0], victim->gold[2], GET_AC(victim) );
strcat( buf1, buf );
sprintf( buf, "|=| `BWater:  %s%4d / `G%4d `B|=| `RGreckles:  `C%15ld`B   (:)================(:)\n\r", codedmana1, victim->mana[1], victim->max_mana[1], victim->gold[3] );
strcat( buf1, buf );
sprintf( buf, "|/| `GEarth:  %s%4d / `G%4d `B|/| `RItems:         `G%4d / %4d`B   `RWeight:`G%4ld / %4ld`B |/|\n\r", codedmana2, victim->mana[2], victim->max_mana[2], victim->carry_number, can_carry_n(victim), victim->carry_weight, UMIN( 9999, can_carry_w(victim) ) );
strcat( buf1, buf );
sprintf( buf, "|=| `CWind:   %s%4d / `G%4d `B|=| `RGuild(s): `G%-30s        `B|=|\n\r", codedmana3, victim->mana[3], victim->max_mana[3], guild_bit_name_2( victim->guilds ) );
strcat( buf1, buf );
sprintf( buf, "|/| `WSpirit: %s%4d / `G%4d `B|/| `R%-49s`B |/|\n\r", codedmana4, victim->mana[4], victim->max_mana[4], birth );
strcat( buf1, buf );
sprintf( buf, "(:)=====================(:)=================================================(:)\n\r" );
strcat( buf1, buf );
sprintf( buf, "|/| `RYou need `G%-10ld `Rexperience points to level `B                         (:)\n\r", exp_table[ victim->level + 1 ].exp_at_level - victim->exp );
strcat( buf1, buf );
send_to_char( buf1, ch );
sprintf( buf, "`B(:)=========================================================================(:)`w\n\r%s\n\r", pos );
    send_to_char( buf, ch );
    
    if ( !IS_NPC( victim ) )
    {
        if ( victim->pcdata->condition[COND_DRUNK] > 10 )
            send_to_char( "`WYou are drunk.`w\n\r", ch );
        if ( victim->pcdata->condition[COND_FULL] == 0 && !IS_IMMORTAL( victim ) )
            send_to_char( "`WYou are hungry.`w\n\r", ch );
        if ( victim->pcdata->condition[COND_THIRST] == 0 && !IS_IMMORTAL( victim ) )
            send_to_char( "`WYou are thirsty.`w\n\r", ch );
    }

    if ( victim->affected != NULL )
 	send_to_char( "`BYou have affects:  `YType affects\n\r", ch );
}

char * const season_name [ 4 ] =
{
    "spring","summer","fall","winter"
};

void do_time( CHAR_DATA *ch, char *argument )
{
    extern char str_boot_time[];
    char buf[MAX_STRING_LENGTH];
    char suf[5], day_sect[11];
    int day, time_of_day, noon;
    int world=ch->in_room->area->world;

    day     = time_info[ world ].day + 1;
    if ( ch->in_room->area->time_zone + time_info[ world ].hour > time_table[ world ].hours_day )
	day++;

    noon = time_table[ world ].hours_day / 2;
    if ( time_table[ world ].hours_day % 2 != 0 )
	noon++;

    time_of_day = get_hour( ch->in_room->area );

         if ( time_of_day < time_table[ world ].sunrise ) strncpy( day_sect, "morning", 10 );
    else if ( time_of_day < noon ) strncpy( day_sect, "day", 10 );
    else if ( time_of_day < ( time_table[ world ].sunset + noon )/2 ) strncpy( day_sect, "afternoon", 10 );
    else if ( time_of_day < time_table[ world ].sunset + 1 ) strncpy( day_sect, "evening", 10 );
    else	strncpy( day_sect, "night", 10 );

         if ( day > 4 && day <  20 ) strncpy( suf, "th", 4 );
    else if ( day % 10 ==  1       ) strncpy( suf, "st", 4 );
    else if ( day % 10 ==  2       ) strncpy( suf, "nd", 4 );
    else if ( day % 10 ==  3       ) strncpy( suf, "rd", 4 );
    else                             strncpy( suf, "th", 4 );

    sprintf( buf, "It is a %s %s, the %d%s of the Month of %s\n\rthe Day of %s.\n\r",
	season_name[ ch->in_room->area->weather->season ],
	day_sect,
	day, suf,
	month_name[ world ][ time_info[ world ].month + ( day / time_table[ world ].days_month ) ],
	day_name[ world ][ day % time_table[ world ].days_week ]);

    send_to_char( buf, ch );

    if ( IS_IMMORTAL( ch ) )
    {
        sprintf( buf, "Electric Dreams started up at %s\rThe system time is %s\r",
	str_boot_time,
	(char *) ctime( &current_time ) );
	send_to_char( buf, ch );
    }

    return;
}

void do_weather( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    bool fWeather_off;
    bool fWeather_on;

    static char * const sky_look[ MAX_SKY ] =
    {
	"The sky is cloudless",
	"The sky is cloudy",
	"Rain falls steady from the sky",
	"The sky is lit by flashes of lightning", 
	"The sky is filled with snow",
	"A blizzard rages all around you",
	"Strong winds and heavy rain pounds against you",
	"A huricane rages all around you",
	"Strong winds blow all about you", 
	"A large tornado can be seen in the distance",
	"Strong winds blow all about you",
	"The sandstorm howls in your ear"
    };

    fWeather_off  = !str_cmp( argument, "off" );
    fWeather_on  = !str_cmp( argument, "on" );

    if (IS_IMMORTAL(ch) )
    {
     if (fWeather_off)
	{
	SET_BIT(ch->plr, PLR_WEATHER_OFF);
	send_to_char("Weather will no longer be displayed\n\r",ch);
	return;
	}
     if (fWeather_on)
	{
	REMOVE_BIT(ch->plr, PLR_WEATHER_OFF);
	send_to_char("Weather will now be displayed\n\r",ch);
	return;
	}
    }

    if ( !IS_OUTSIDE(ch) )
    {
	send_to_char( "You can't see the weather indoors.\n\r", ch );
	return;
    }

    if ( IS_UNDERWATER( ch ) );
    {
	send_to_char( "You can't see the weather underwater.\n\r",ch);
	return;
    }

    sprintf( buf, "%s and %s.\n\r",
	sky_look[ ch->in_room->area->weather->sky ],
	ch->in_room->area->weather->change <= 0
	? "a warm southerly breeze blows"
	: "a cold northern gust blows"
	);

    send_to_char( buf, ch );
    return;
}

void do_help( CHAR_DATA *ch, char *argument )
{
    HELP_DATA *pHelp;
    char buf[MAX_STRING_LENGTH];
    char argall[MAX_INPUT_LENGTH],argone[MAX_INPUT_LENGTH];
    bool found = FALSE;

    if ( argument[0] == '\0' )
    {
    	if ( IS_SET(ch->in_room->room_flags_2, ROOM_STARTING ) )
	    argument = "creation";
    	else
	    argument = "summary";
    }

    /* this parts handles help a b so that it returns help 'a b' */
    argall[0] = '\0';
    while (argument[0] != '\0' )
    {
	argument = one_argument(argument,argone);
	if (argall[0] != '\0')
	    strcat(argall," ");
	strcat(argall,argone);
    }

    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	if (  pHelp->level > 1 && !IS_IMMORTAL(ch))
	    continue;

	if ( is_name_2( argall, pHelp->keyword ) )
	{

	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		send_to_char( pHelp->keyword, ch );
		send_to_char( "\n\r", ch );
	    }

	    /*
	     * Strip leading '.' to allow initial blanks.
	     */
	    if ( pHelp->text[0] == '.' )
		page_to_char( pHelp->text+1, ch );
	    else
		page_to_char( pHelp->text  , ch );
	    return;
	}
    }

    send_to_char( "Possibles\n\r", ch );
    for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next )
    {
	if (  pHelp->level > 1 && !IS_IMMORTAL(ch))
	    continue;

	if ( is_part_name( argall, pHelp->keyword ) )
	{

	    if ( pHelp->level >= 0 && str_cmp( argall, "imotd" ) )
	    {
		send_to_char( pHelp->keyword, ch );
		send_to_char( "\n\r", ch );
	        found = TRUE;
	    }

	}
    }
    if (!found)
    {
    send_to_char( "No help on that word.\n\r", ch );
    sprintf(buf,"No help for: %s (%s)",argall, ch->name);
    log_string(buf);
    info_channel( ch, buf, INFOACT_HELP);
    }
    return;
}


/*
 * simplified who command, by Reverie (resolves huge memory problem)
 */
void do_who( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    DESCRIPTOR_DATA *d;
    int iRace;
    int iLevelLower;
    int iLevelUpper;
    int nNumber;
    int nMatch;
    bool fWorldRestrict;
    bool rgfRace[MAX_RACE];
    bool fRaceRestrict;
    bool fImmortalOnly;
    char colorbuf[15];

    if ( IS_NPC( ch ) )
        return;

    if ( IS_IMMORTAL( ch ) )
        fWorldRestrict = FALSE;
    else
        fWorldRestrict = TRUE;

    /*
     * Set default arguments.
     */
    iLevelLower    = 0;
    iLevelUpper    = MAX_LEVEL;
    fRaceRestrict = FALSE;
    fImmortalOnly  = FALSE;
    for ( iRace = 0; iRace < top_race; iRace++ )
        rgfRace[iRace] = FALSE;

    /*
     * Parse arguments.
     */
    nNumber = 0;
    for ( ;; )
    {
        char arg[MAX_STRING_LENGTH];

        argument = one_argument( argument, arg );
        if ( arg[0] == '\0' )
            break;

        if ( is_number( arg ) )
        {
            switch ( ++nNumber )
            {
                case 1: iLevelLower = atoi( arg ); break;
                case 2: iLevelUpper = atoi( arg ); break;
                default:
                    send_to_char( "Only two level numbers allowed.\n\r", ch );
                    return;
            }
        }
        else
        {
            /*
             * Look for immortals to turn on.
             */
            if ( arg[0] == 'i' && arg[1] == 'm' )
                fImmortalOnly = TRUE;
            else if ( arg[0] == 'a' && arg[1] == 'l' )
                fWorldRestrict = FALSE;
            else
            {
                for ( iRace = 0; iRace < top_race ; iRace++ )
                    if ( !strncmp( arg, race_list[ iRace ].name, strlen(arg) ) )
                    {
                        rgfRace[iRace] = TRUE;
                        fRaceRestrict = TRUE;
                    }
            }
        }
    }

    /*
     * Now show matching chars.
     */
    nMatch = 0;
    buf[0] = '\0';
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
        CHAR_DATA *wch;

        /*
         * Check for match against restrictions.
         */
        if ( d->connected > CON_PLAYING )
            continue;

        wch   = ( d->original != NULL ) ? d->original : d->character;
        if ( wch->level < iLevelLower
        ||   wch->level > iLevelUpper
        || !can_see( ch, wch )
        || ( fImmortalOnly  && !IS_IMMORTAL(ch) )
        || ( fWorldRestrict && ch->in_room->area->world != wch->in_room->area->world )
        || ( fRaceRestrict && !rgfRace[wch->race] ) )
            continue;

        nMatch++;

        if (ch->color)
        {
            switch (wch->level / 10)
            {
                case 0:
                    {
                        strncpy( colorbuf,"`W", 14);
                        break;
                    }
                case 1:
                    {
                        strncpy (colorbuf,"`w", 14);
                        break;
                    }
                case 2:
                    {
                        strncpy (colorbuf,"`y", 14);
                        break;
                    }
                case 3:
                    {
                        strncpy (colorbuf,"`Y", 14);
                        break;
                    }
                case 4:
                    {
                        strncpy (colorbuf,"`G", 14);
                        break;
                    }
                default:
                    {
                        strncpy (colorbuf,"`R", 14);
                        break;
                    }
            }

        }

        if ( IS_IMMORTAL( wch ) )
            sprintf( buf, "%s[%s] %s%s`w%s%s%s\n\r", ( ( ch->color ) ? colorbuf : "" ), 
		wch->pcdata->imm_title, 
		wch->name, 
		wch->pcdata->title, 
		( IS_SET( wch->plr, PLR_WIZINVIS ) ? " (INVIS)" : "" ),
		( d->connected < CON_PLAYING ? " (BUSY)" : "" ), 
		( IS_SET( wch->tmpact, TMPACT_AWAY ) ? " (AWAY)" : "" ) );
        else
            sprintf( buf, "%s[ %d %s] %s%s`w%s%s\n\r", 
		( ( ch->color ) ? colorbuf : "" ),
 		wch->level, 
		race_table[ wch->race ][ wch->sex ].who_name, 
		wch->name, wch->pcdata->title, 
		( d->connected < CON_PLAYING ? " (BUSY)" : "" ), 
		( IS_SET( wch->tmpact, TMPACT_AWAY ) ? " (AWAY)" : "" ) );

        send_to_char( buf, ch );
    }

    if ( ch->color )
        strncpy ( colorbuf, "`W", 14 );

    sprintf( buf2, "%s\n\rPlayers found: %d\n\r",((ch->color) ?  colorbuf : ""), nMatch );
    send_to_char( buf2, ch );
    send_to_char("`w",ch);
    return;
}

void do_count ( CHAR_DATA *ch, char *argument )
{
    int count;
    DESCRIPTOR_DATA *d;
    char buf[MAX_STRING_LENGTH];

    count = 0;

    if ( !str_cmp(argument, "auto" ) )
    {
    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected <= CON_PLAYING )
	    count++;
    max_on = UMAX(count,max_on);
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
        if ( d->connected <= CON_PLAYING && can_see( ch, d->character ) )
	    count++;

    if (max_on == count)
        sprintf(buf,"There are %d characters on, the most so far today.\n\r",
	    count);
    else
	sprintf(buf,"There are %d characters on, the most on today was %d.\n\r",
	    count,max_on);
    send_to_char(buf,ch);
}

void do_inventory( CHAR_DATA *ch, char *argument )
{   
    char buf[MAX_STRING_LENGTH];
    sprintf(buf,"`W");
    send_to_char( buf, ch);  
    send_to_char( "You are carrying:\n\r", ch );
    show_list_to_char( ch->carrying, ch, TRUE, TRUE );
    send_to_char("`w", ch);
    return;
}

void show_obj_aff( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    OBJ_DATA *obj;
    int iWear;
    bool found;
   
    found = FALSE;
    buf2[0] = '\0';

    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;

	if ( ( IS_SET( obj->extra_flags, ITEM_NO_IDENTIFY ) && !IS_IMMORTAL( ch ) ) 
	    || obj->wear_data->affected == NULL )
	    continue;

        for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
        {
	    if ( paf->location == APPLY_AFFECT )
	    {
		if ( paf->bit_type == BIT_AFFECT )
		{
		    if ( !IS_AFFECTED( ch, paf->bitvector ) && !IS_AFFECTED_2( ch, paf->bitvector ) ) 
			continue;
		    else
		    {
	    	    	sprintf( buf, "%s `w: %s of %s\n\r", obj->short_descr, 
           	    	    get_bit_name(paf->bit_type), affect_loc_name( paf ) );
	    	    	strcat( buf2, buf );
		    }
		}
		else if ( paf->bit_type == BIT_IMM && !IS_SET( ch->imm_flags, paf->bitvector ) )
		    continue;
		else if ( paf->bit_type == BIT_RES && !IS_SET( ch->res_flags, paf->bitvector ) )
		    continue;
		else if ( paf->bit_type == BIT_VULN && !IS_SET( ch->vuln_flags, paf->bitvector ) )
		    continue;
		else
	    	{
	    	    sprintf( buf, "%s `w: %s of %s\n\r", obj->short_descr, 
           	    	get_bit_name(paf->bit_type), affect_loc_name( paf ) );
	    	    strcat( buf2, buf );
	    	}
	    }
	}

	if ( buf2[0] != '\0' )
	    found = TRUE;
    }

    if ( found )
    {
	send_to_char( "\n\rYou are affected by the following objects:\n\r", ch );
	send_to_char( buf2, ch );
    }

    return;
}

void do_equipment( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    int iWear;
    bool found;
    
    send_to_char( "`CYou are using:`w\n\r", ch );
    found = FALSE;
    for ( iWear = 0; iWear < MAX_WEAR; iWear++ )
    {
	if ( ( obj = get_eq_char( ch, iWear ) ) == NULL )
	    continue;
	
	send_to_char("`y", ch);
 	send_to_char( where_name[iWear], ch );
	if ( can_see_obj( ch, obj ) )
	{   
	    send_to_char("`W", ch);
	    send_to_char( format_obj_to_char( obj, ch, TRUE ), ch );
	    send_to_char( "`w", ch );
	}
	else
	 {   
	    send_to_char( "`Gsomething.`w\n\r", ch );
	 }
	found = TRUE;
    }

    if ( !found )
	send_to_char( "Nothing.\n\r", ch );
    
    return;
}



void do_compare( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    OBJ_DATA *obj1;
    OBJ_DATA *obj2;
    int value1;
    int value2;
    char *msg;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Compare what to what?\n\r", ch );
	return;
    }

    if ( ( obj1 = get_obj_carry( ch, arg1 ) ) == NULL )
    {
	send_to_char( "You do not have that item.\n\r", ch );
	return;
    }

    if (arg2[0] == '\0')
    {
	for (obj2 = ch->carrying; obj2 != NULL; obj2 = obj2->next_content)
	{
	    if (obj2->wear_data != NULL && obj2->wear_data->wear_loc != WEAR_NONE
	    &&  can_see_obj(ch,obj2)
	    &&  obj1->wear_data != NULL
	    &&  (obj1->wear_data->wear_flags & obj2->wear_data->wear_flags & ~ITEM_TAKE) != 0 )
		break;
	}

	if (obj2 == NULL)
	{
	    send_to_char("You aren't wearing anything comparable.\n\r",ch);
	    return;
	}
    } 

    else if ( (obj2 = get_obj_carry(ch,arg2) ) == NULL )
    {
	send_to_char("You do not have that item.\n\r",ch);
	return;
    }

    msg		= NULL;
    value1	= 0;
    value2	= 0;

    if ( obj1 == obj2 )
    {
	msg = "You compare $p to itself.  It looks about the same.";
    }
    else
    {
     	if ( obj1->wear_data != NULL && obj2->wear_data != NULL )
	{
	    value1 = obj1->wear_data->ac;
	    value2 = obj2->wear_data->ac;

	    if ( obj1->wear_data->weapon_data != NULL && obj2->wear_data->weapon_data != NULL )
	    {
		value1 = (1 + obj1->wear_data->weapon_data->die) * obj1->wear_data->weapon_data->dice;
		value2 = (1 + obj2->wear_data->weapon_data->die) * obj2->wear_data->weapon_data->dice;
	    }
	}
    }

    if ( msg == NULL )
    {
	     if ( value1 == value2 ) msg = "$p and $P look about the same.";
	else if ( value1  > value2 ) msg = "$p looks better than $P.";
	else                         msg = "$p looks worse than $P.";
    }

    act( msg, ch, obj1, obj2, TO_CHAR );
    return;
}



void do_credits( CHAR_DATA *ch, char *argument )
{
    do_help( ch, "diku" );
    return;
}



void do_where( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Players near you:\n\r", ch );
	found = FALSE;
	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->connected <= CON_PLAYING
	    && ( victim = d->character ) != NULL
	    &&   !IS_NPC(victim)
	    &&   victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   can_see( ch, victim ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    victim->name, victim->in_room->name );
		send_to_char( buf, ch );
	    }
	}
	if ( !found )
	    send_to_char( "None\n\r", ch );
    }
    else
    {
	found = FALSE;
	for ( victim = char_list; victim != NULL; victim = victim->next )
	{
	    if ( victim->in_room != NULL
	    &&   victim->in_room->area == ch->in_room->area
	    &&   !IS_AFFECTED(victim, AFF_HIDE)
	    &&   !IS_AFFECTED_2(victim, AFF_FADE)
	    &&   !IS_AFFECTED(victim, AFF_SNEAK)
	    &&   can_see( ch, victim )
	    &&   is_name( arg, victim->name ) )
	    {
		found = TRUE;
		sprintf( buf, "%-28s %s\n\r",
		    PERS(victim, ch), victim->in_room->name );
		send_to_char( buf, ch );
		break;
	    }
	}
	if ( !found )
	    act( "You didn't find any $T.", ch, NULL, arg, TO_CHAR );
    }

    return;
}




void do_consider( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    char *msg;
    int diff, level;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Consider killing whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They're not here.\n\r", ch );
	return;
    }

    if (is_safe(ch,victim))
    {
	send_to_char("Don't even think about it.\n\r",ch);
	return;
    }

    level = victim->level;

    if ( victim->skills != NULL || victim->innate_skills != NULL )
	level += 2;

    diff = level - ch->level;

         if ( diff <= -10 ) msg = "They don't have a chance.";
    else if ( diff <=  -5 ) msg = "It would be an unfair fight, at your advantage.";
    else if ( diff <=  -2 ) msg = "$N would have a hard time keeping up with you.";
    else if ( diff <=   1 ) msg = "You are just as skilled as your opponent.";
    else if ( diff <=   4 ) msg = "$N would be a good test of your skills.";
    else if ( diff <=   9 ) msg = "$N would be more than a challenge.";
    else                    msg = "Your opponent is obviously more skilled than you.";

    act( msg, ch, NULL, victim, TO_CHAR );
    return;
}

void set_title( CHAR_DATA *ch, char *title )
{
    char buf[MAX_STRING_LENGTH];
    FINGER_DATA * finger;

    if ( IS_NPC(ch) )
    {
	bug( "Set_title: NPC.", 0 );
	return;
    }

    if ( title[0] != '.' && title[0] != ',' && title[0] != '!' && title[0] != '?' )
    {
	buf[0] = ' ';
	strcpy( buf+1, title );
    }
    else
    {
	strcpy( buf, title );
    }

    strncpy( ch->pcdata->title, buf, 80 );

    for ( finger = finger_list ; finger != NULL ; finger = finger->next )
	if ( !str_cmp( finger->name, ch->name ) && !IS_IMMORTAL( ch ) )
	{
	    strncpy( finger->title, buf, 80 );
	} 

   return;
}



void do_title( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Change your title to what?\n\r", ch );
	return;
    }

    if ( strlen(argument) > 45 )
	argument[45] = '\0';

    set_title( ch, argument );
    send_to_char( "Ok.\n\r", ch );
}

void do_description( CHAR_DATA *ch, char *argument )
{

        if ( argument[0] == '\0' )
        {
            string_append( ch, &ch->description );
            return;
        }

        if ( argument[0] == '+' )
        {
            string_append( ch, &ch->description );
            return;
        }

        send_to_char( "Syntax:  desc     line edit\n\r", ch );
        send_to_char( "         desc +   line append\n\r",ch );
        return;
}

void do_report( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
    int i;

    buf1[0] = '\0';
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        sprintf( buf, "%s %d/%d ",element_type_name( i ), ch->mana[ i ], ch->max_mana[ i ] );
	strcat( buf1, buf );
    }

    sprintf( buf,
	"You say I have [HP %d/%d] [MV %d/%d]\n\r%s\n\r",
	ch->hit,  ch->max_hit,
	ch->move, ch->max_move,
	buf1   );

    send_to_char( buf, ch );

    sprintf( buf, "$n says I have [HP %d/%d] [MV %d/%d]\n\r%s",
	ch->hit,  ch->max_hit, 
	ch->move, ch->max_move,
	buf1 );

    act( buf, ch, NULL, NULL, TO_ROOM );

    return;
}

void do_show_skills( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *mob;
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char arg[MAX_INPUT_LENGTH];
    int sn;
    int col = 0;

    if ( IS_NPC(ch) )
	return;

    argument = one_argument(argument,arg);

    output = new_buf();

    if ( arg[0] == '\0' )
    {
	sprintf(buf,"In current world:\n\r");
	add_buf(output, buf);
	for ( mob = char_list; mob != NULL; mob = mob->next )
	{
	    if (mob->in_room->area->world == ch->in_room->area->world )
	    	if (IS_NPC(mob) && mob->pIndexData->gainer_data != NULL )
	    	{
		    col = 0;
		    sprintf(buf,"\n\r`R%-25s (%5ld) (Room %5ld) (Area %2d)`w\n\r",mob->short_descr, mob->pIndexData->vnum,mob->in_room->vnum, mob->in_room->area->areanum );
		    add_buf(output, buf);
		    sprintf(buf,"------------------------------------------------------------------------\n\r");
		    add_buf(output, buf);
		    for ( sn = 1; sn < MAX_SKILL; sn++)
			if (mob->pIndexData->gainer_data != NULL )
			    if (mob->pIndexData->gainer_data->skill[sn] )
			    {
	    			sprintf(buf,"%-22s",skill_table[sn].name);
				add_buf(output, buf);
		  	  	if (++col % 3 == 0)
		    	    	{
				    sprintf(buf,"\n\r");
				    add_buf(output, buf);
		    		}
			    }
	    	}
	}
    }
    else if (!strncmp(arg,"area",2))
    {
	sprintf(buf,"In current area:\n\r");
	add_buf(output, buf);
	for ( mob = char_list; mob != NULL; mob = mob->next )
	{
	    if (mob->in_room->area == ch->in_room->area )
	    	if (IS_NPC(mob) && mob->pIndexData->gainer_data != NULL )
	    	{
		    col = 0;
		    sprintf(buf,"\n\r`R%-25s (%5ld) (Room %5ld) (Area %2d)`w\n\r",mob->short_descr, mob->pIndexData->vnum,mob->in_room->vnum, mob->in_room->area->areanum );
		    add_buf(output, buf);
		    sprintf(buf,"------------------------------------------------------------------------\n\r");
		    add_buf(output, buf);
		    for ( sn = 1; sn < MAX_SKILL; sn++)
			if (mob->pIndexData->gainer_data != NULL )
			    if (mob->pIndexData->gainer_data->skill[sn] )
			    {
	    			sprintf(buf,"%-22s",skill_table[sn].name);
				add_buf(output, buf);
		    		if (++col % 3 == 0)
		    		{
				    sprintf(buf,"\n\r");
				    add_buf(output, buf);
		    		}
			    }
	    	}
	}
    }
    else if (!strncmp(arg,"mobile",2))
    {
	if ( ( mob = get_char_world(ch, argument) ) != NULL )
	{
	    if (IS_NPC(mob) && mob->pIndexData->gainer_data != NULL )
	    {
		col = 0;
		sprintf(buf,"\n\r`R%-25s (%5ld) (Room %5ld) (Area %2d)`w\n\r",mob->short_descr, mob->pIndexData->vnum,mob->in_room->vnum, mob->in_room->area->areanum );
		add_buf(output, buf);
		sprintf(buf,"------------------------------------------------------------------------\n\r");
		add_buf(output, buf);
		for ( sn = 1; sn < MAX_SKILL; sn++)
		    if (mob->pIndexData->gainer_data != NULL )
			if (mob->pIndexData->gainer_data->skill[sn] )
			{
	    		    sprintf(buf,"%-22s",skill_table[sn].name);
			    add_buf(output, buf);
		    	    if (++col % 3 == 0)
		    	    {
				sprintf(buf,"\n\r");
				add_buf(output, buf);
		    	    }
			}
	    }
	}    
    }
    else
    {
    	send_to_char("Syntax: show_skills\n\r", ch);
    	send_to_char("Syntax: show_skills area\n\r", ch);
    	send_to_char("Syntax: show_skills mob <name>\n\r", ch);
    	free_buf(output);
    	return;
    }

    page_to_char(buf_string(output), ch);
    free_buf(output);
    return;
}

void do_find( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *pRoomIndex; 
    MOB_INDEX_DATA *pMob;
    TRIGGER_LIST_DATA *trigger_list_data;
    TRIGGER_INDEX_DATA *trigger;
    long vnum;
    CHAR_DATA *mob;
    SPECPRO_LIST *s;
    SHOP_DATA *pShop;
    OBJ_DATA  *obj;
    OBJ_DATA  *in_obj;
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    bool found, fPortal;
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    int  x;
    if ( IS_NPC(ch) )
	return;

    output = new_buf();
    argument = one_argument(argument,arg);
    argument = one_argument(argument,arg1);

    if ( arg[0] == '\0' )
    {
	send_to_char("find what?\n\r", ch );
	send_to_char("find gainer\n\r", ch );
	send_to_char("find gainer world\n\r", ch );
	send_to_char("find trainer\n\r", ch );
	send_to_char("find shop\n\r", ch );
	send_to_char("find spec_pro\n\r", ch );
	send_to_char("find room\n\r", ch );
	send_to_char("find mob\n\r", ch );
	send_to_char("find object\n\r", ch );
	send_to_char("find moveable\n\r", ch );
	send_to_char("find triggers\n\r", ch );
    }
    else if (!strncmp(arg,"trainer", 4))
    {

	for ( mob = char_list; mob != NULL; mob = mob->next )
	{
	    if (IS_NPC(mob) && IS_SET(mob->act, ACT_TRAIN ) )
	    {
	    	sprintf(buf,"%30s  Area: %4d  Room: %5ld\n\r",mob->short_descr, mob->in_room->area->areanum, mob->in_room->vnum);
	    	send_to_char(buf,ch);
	    }
	}
    
    }
    else if (!strncmp(arg,"gainer", 4))
    {
	bool fWorld = FALSE;

	fWorld = !str_cmp(arg1, "world");

	for ( mob = char_list; mob != NULL; mob = mob->next )
	{
	    if (IS_NPC(mob) && IS_SET(mob->act, ACT_GAIN ) && (!fWorld || mob->in_room->area == ch->in_room->area))
	    {
	    	sprintf(buf,"%30s  Area: %4d  Room: %5ld\n\r",mob->short_descr, mob->in_room->area->areanum, mob->in_room->vnum);
	    	send_to_char(buf,ch);
	    }
	}
    
    }
    else if (!strncmp(arg,"triggers", 4))
    {
	return;
	if (*arg1 == '\0' )
	{
            free_buf(output);
	    return;
	}
	if (is_number(arg1) )
	{
	x = atoi(arg1);
    	for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
    	    for ( pMob = mob_index_hash[vnum]; pMob != NULL; pMob = pMob->next )
		for ( trigger_list_data = pMob->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
		    if (trigger_list_data->vnum == x )
			sprintf( buf, "     [Vnum:] %-5ld  [short:] %-20s\n\r",
			    pMob->vnum,  pMob->short_descr);
                	add_buf(output,buf); 
		
	}
	else
	{
    	for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
    	    for ( pMob = mob_index_hash[vnum]; pMob != NULL; pMob = pMob->next )
		for ( trigger_list_data = pMob->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
		{
		    trigger = get_trig_index(trigger_list_data->vnum );
		    if (!str_cmp(trigger->name, arg1 ) )
			sprintf( buf, "     [Vnum:] %-5ld  [short:] %-20s\n\r",
			    pMob->vnum, pMob->short_descr);
                	add_buf(output,buf); 
		}
	}
      	page_to_char(buf_string(output),ch);
	
    }
    else if (!str_cmp(arg,"shop"))
    {


    	if ( arg1[0] == '\0' )
	{
	    send_to_char( "What shop type?\n\r", ch );
            free_buf(output);
	    return;
	}

	for ( mob = char_list; mob != NULL; mob = mob->next )
	{
	    if (IS_NPC(mob) && ( pShop = mob->pIndexData->pShop )  != NULL ) 
	    {
		if ( pShop == NULL )
		break;
		
		if ( shop_name_type(arg1) == 0 )
		{
		    send_to_char("Invalid shop type\n\r",ch);
		    free_buf(output);
		    return;
		}

		if ( IS_SET( pShop->type, item_name_type(arg1) ) )
		    {		
	    		sprintf(buf,"%30s  Area: %4d  Room: %5ld\n\r",mob->short_descr, mob->in_room->area->areanum, mob->in_room->vnum);
	    		send_to_char(buf,ch);
		    }
	    }
	}
    
    }
    else if (!strncmp(arg,"spec_proc",4))
    {
    if ( arg1[0] == '\0' )
    {
	send_to_char( "Find which spec_pro?\n\r", ch );
	send_to_char( "Syntax: find spec_pro <spec_pro>\n\r",ch);
	send_to_char( "Syntax: find spec_pro all <optional>\n\r",ch);

	free_buf(output);
	return;
    }

    found	= FALSE;
    
    
    if ( !str_cmp(arg1, "all" ) )
    {
    for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
    {
        for ( pRoomIndex = room_index_hash[vnum]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
	{
	if ( pRoomIndex->spro != NULL )
	   for ( s = pRoomIndex->spro; s != NULL; s = s->next )
	   {
		found = TRUE;
		sprintf( buf, "     [Vnum:] %-15ld  [Spec_Pro:] %-24s\n\r",
		    pRoomIndex->vnum,  s->sp->trigger);
                add_buf(output,buf); 
	    }
	}
      }
      	page_to_char(buf_string(output),ch);
    }
    else
    {
    for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
    {
        for ( pRoomIndex = room_index_hash[vnum]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
	{
	if ( pRoomIndex->spro != NULL )
	   for ( s = pRoomIndex->spro; s != NULL; s = s->next )
	   if ( !str_cmp(arg1, s->sp->trigger ) )
	   {
		found = TRUE;
		sprintf( buf, "     [Vnum:] %-15ld  [Spec_Pro:] %-24s\n\r",
		    pRoomIndex->vnum,  s->sp->trigger);
                add_buf(output,buf); 
	    }
	}
      }
      	page_to_char(buf_string(output),ch);
    }    
    if ( !found )
	send_to_char( "No spec_pro's by that name.\n\r", ch );
    }
    else if (!strncmp(arg,"moveable", 4))
    {

	for ( mob = char_list; mob != NULL; mob = mob->next )
	{
	    if (IS_NPC(mob) && IS_SET( mob->act2, ACT2_MOVEABLE_AREA ) )
	    {
	    		sprintf(buf,"%30s  [Area:] %4d  [Room:] %5ld\n\r",mob->short_descr, mob->in_room->area->areanum, mob->in_room->vnum);
	    		send_to_char(buf,ch);
	    }
	}	
    }
	
    else if (!str_cmp(arg,"room"))
	{
	send_to_char("Comming soon\n\r",ch);
	}
    else if (!strncmp(arg,"object", 3))
    	{

    	if ( arg1[0] == '\0' )
	{
	    send_to_char( "What object?\n\r", ch );
	    free_buf(output);
	    return;
	}	
	
    found = FALSE;
    fPortal = !str_cmp(arg1,"portal");
    for ( obj = object_list; obj; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( arg1, obj->name ) )
	    continue;

	found = TRUE;

	for ( in_obj = obj; in_obj->in_obj; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by )
	{
	    sprintf( buf, "%s carried by %s.\n\r",
		    obj->short_descr, PERS( in_obj->carried_by, ch ) );
	    add_buf(output,buf);
	}
	else
	{
	    sprintf( buf, "%s in %s [ vnum: %ld ]\n\r",
		    obj->short_descr, in_obj->in_room == NULL ? "somewhere" : in_obj->in_room->name,
		    obj->in_room == NULL ? 0 : obj->in_room->vnum );
	    add_buf(output,buf);
	  if ( fPortal)
	  {
		sprintf( buf, "Leads to: %s (%ld) %s\n\r",
	    	( obj->gate_data->leads_to == NULL ? "None" : 
	    	( IS_SET( obj->gate_data->flags, GATE_TO_MOB ) ? ( (CHAR_DATA *) obj->gate_data->leads_to )->short_descr : 
	    	( IS_SET( obj->gate_data->flags, GATE_TO_OBJ ) ? ( (OBJ_DATA *) obj->gate_data->leads_to )->short_descr :
	    	( (ROOM_INDEX_DATA *) obj->gate_data->leads_to )->name ) ) ),
	    	obj->gate_data->vnum,
	    	obj->pIndexData->in_area->name );
	    add_buf(output,buf);
	   }
	}

    }

    if ( !found )
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	else
	page_to_char( buf_string(output), ch );
	}	
    else if (!strncmp(arg,"mobile", 3))
    {

    	if ( arg1[0] == '\0' )
	{
	    send_to_char( "What mob?\n\r", ch );
	    free_buf(output);
	    return;
	}

	for ( mob = char_list; mob != NULL; mob = mob->next )
	{
	    if (IS_NPC(mob) && is_name(arg1, mob->name) ) 
	    {
	    	sprintf(buf,"%30s  Area: %4d  Room: %5ld\n\r",mob->short_descr, mob->in_room->area->areanum, mob->in_room->vnum);
	    	add_buf(output,buf);
	    }
	}
    
     page_to_char(buf_string(output),ch);
    }
    free_buf(output);
    return;
}


/*
 * 'Wimpy' originally by Dionysos.
 */
void do_wimpy( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int wimpy;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
	wimpy = ch->max_hit / 5;
    else
	wimpy = atoi( arg );

    if ( wimpy < 0 )
    {
	send_to_char( "Your courage exceeds your wisdom.\n\r", ch );
	return;
    }

    if ( wimpy > ch->max_hit/2 )
    {
	send_to_char( "Such cowardice ill becomes you.\n\r", ch );
	return;
    }

    ch->wimpy	= wimpy;
    sprintf( buf, "Wimpy set to %d hit points.\n\r", wimpy );
    send_to_char( buf, ch );
    return;
}



void do_password( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char *pwdnew;
    char *p;

    if ( IS_NPC(ch) )
	return;

    argument = one_argument_case( argument, arg1 );
    argument = one_argument_case( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: password <old> <new>.\n\r", ch );
	return;
    }

    if ( str_cmp( arg1, ch->pcdata->pwd ))
    {
	WAIT_STATE( ch, 40 );
	send_to_char( "Wrong password.  Wait 10 seconds.\n\r", ch );
	return;
    }

    if ( strlen(arg2) < 5 )
    {
	send_to_char(
	    "New password must be at least five characters long.\n\r", ch );
	return;
    }

    /*
     * No tilde allowed because of player file format.
     */
    pwdnew = arg2;
    for ( p = pwdnew; *p != '\0'; p++ )
    {
	if ( *p == '~' )
	{
	    send_to_char(
		"New password not acceptable, try again.\n\r", ch );
	    return;
	}
    }

    strncpy( ch->pcdata->pwd, pwdnew, 30 );
    save_char_obj( ch );
    send_to_char( "Ok.\n\r", ch );
    return;
}

void do_logondata(CHAR_DATA *ch, char *argument)
{
    LOGON_DATA *logon;
    char buf[MAX_STRING_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    BUFFER *output; 
    time_t ontime;
    short  days;
    short  hours;
    short  mins; 
    bool fName = FALSE;
    bool fStatus = FALSE;
    int x = 0;

    argument = one_argument(argument,arg1);
    one_argument(argument,arg2);

    fName = !str_cmp( arg1, "name" );
    fStatus = !str_cmp( arg1, "status" );

    if ( fName )
    {
	if ( arg2[0] == '\0')
	{
	    send_to_char("What name?\n\r",ch);
	    return;
	}	
    }
    if ( fStatus )
    {
	if ( arg2[0] == '\0')
	{	
	    send_to_char("What status?\n\r",ch);
	    return;
	}
    }

    output = new_buf();

    for (logon = logon_data_list; logon != NULL; logon = logon->next)
    {
	ontime = logon->time_end - logon->time_start;		
	days = ontime / 86400;
	ontime -= (days*86400);
	hours = ontime / 3600; 
	ontime -= (hours * 3600);
	mins = ontime / 60;

	if ( mins < 0 )
	    mins = 0;

	if ( fName )
	{
	    if ( str_prefix(logon->name, arg2) )
		continue;
	}
	if ( fStatus )
	{
	    if ( str_prefix(logon->exit, arg2) )
		continue;
	}

	x++;
	if (x > 1000 )
	    break;

	if ( logon->level < 60 )
	sprintf(buf,"%-11s K %-3d P %-2d E %-8ld S %-9s H %-2d M %-2d %s %s\r"
	    ,logon->name, logon->kills, logon->pkills, logon->exp,logon->exit
	    ,hours, mins, (char *) ctime( &logon->time_end ), logon->last_ip );
	else if ( logon->level <= 62 )
	sprintf(buf,"%-11s R %-3d O %-2d M %-8d S %-9s H %-2d M %-2d %s\r"
	    ,logon->name, logon->rooms, logon->objects, logon->mobiles,logon->exit
	    ,hours, mins, (char *) ctime( &logon->time_end ) );
	else if ( is_optivision(ch) )
	sprintf(buf,"%-11s R %-3d O %-2d M %-8d S %-9s H %-2d M %-2d %s\r"
	    ,logon->name, logon->rooms, logon->objects, logon->mobiles,logon->exit
	    ,hours, mins, (char *) ctime( &logon->time_end ) );
	else
	continue;

	add_buf(output, buf);	
    }
    page_to_char(buf_string(output),ch);
    free_buf(output);
    return;
}

void do_affects(CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA *paf, *paf_last = NULL;
    char buf[MAX_STRING_LENGTH];
    
    if ( ch->affected != NULL )
    {
	send_to_char( "You are affected by the following spells:\n\r", ch );
	for ( paf = ch->affected; paf != NULL; paf = paf->next )
	{
	    if (paf_last != NULL && paf->type == paf_last->type)
		if (ch->level >= 30)
		    sprintf( buf, "                      ");
		else
		    continue;
	    else
	    { 
		if ( paf->type == -2 )
	    	sprintf( buf, "Spell: `RDEATH!  `YType: Help death");
		else
	    	sprintf( buf, "Spell: %-15s", skill_table[paf->type].name );
	    }

	    send_to_char( buf, ch );

	    if ( ch->level >= 30 )
	    {
		if (paf->bit_type > 0 && paf->modifier != 0)
		sprintf( buf,
		    ": modifies %s to %s by %d ",
		    get_bit_name(paf->bit_type),
		    affect_loc_name( paf ),
		    paf->modifier);
		else if (paf->bit_type > 0 && paf->modifier == 0)
		sprintf( buf,
		    ": modifies %s to %s ",
		    get_bit_name(paf->bit_type),
		    affect_loc_name( paf ) );
		else
		sprintf( buf,
		    ": modifies %s by %d ",
		    affect_loc_name( paf ),
		    paf->modifier);
		send_to_char( buf, ch );
		if ( paf->duration == -1 )
		    sprintf( buf, "permanently" );
		else
		    sprintf( buf, "for %d hours", paf->duration + 1);
		send_to_char( buf, ch );
	    }

	    send_to_char( "\n\r", ch );
	    paf_last = paf;
	}
    }
    else 
	send_to_char("You are not affected by any spells.\n\r",ch);

    show_obj_aff( ch, NULL );

/* Show room affects to immortals */
    if ( IS_IMMORTAL( ch ) )
    {
	for ( paf = ch->in_room->affected_by ; paf != NULL ; paf = paf->next )
	{
	    if ( paf->location == APPLY_ROOM_FLAGS )
	    {
		sprintf( buf, "Applies room flag of %s for %d hours.\n\r", room_bit_name( paf->bitvector ), paf->duration );
		send_to_char( buf, ch );
	    }
	}
    }

    return;
}
void do_level( CHAR_DATA *ch, char *argument)
{
    char buf[200];
    long value;

    if( IS_NPC( ch ) )
    {
	send_to_char( "Mobs don't gain exp yet.", ch );
	return;
    }

    value = exp_table[ch->level +1].exp_at_level - ch->exp;
   
    sprintf(buf, "`RYou need `G%ld `Rexperience points before your next level.\n\r", value);
    send_to_char( buf, ch );
} 

void do_whogroup( CHAR_DATA *ch, char *argument )
{
    GROUP_DATA 	*group;
    CHAR_DATA 	*gch;
    char	buf[MAX_STRING_LENGTH];
    bool	fAll = FALSE;

    fAll = !str_cmp( argument, "all" );

    send_to_char(
"----------------------------------------------------------------------\n\r", ch );
    send_to_char(
"=                     Groups on Electric Dreams                      =\n\r", ch );
    send_to_char(
"----------------------------------------------------------------------\n\r", ch );

    for( group = group_list ; group ; group = group->next )
    {
	if( IS_SET( group->flags, GROUP_IS_PUBLIC ) || ( IS_IMMORTAL( ch ) && fAll ) ) 
	{
	    sprintf( buf,"   \"%s\"    Pkills: %d    Mob Kills: %d\n\r", group->name, group->pkills, group->mob_kills );
	    send_to_char( buf, ch );
	    for( gch = group->leader ; gch ; gch = gch->next_in_group )
	    {
		sprintf( buf,"      %20s -- %5d -- %s\n\r", gch->name, gch->level, 
		race_list[gch->race].name );
		send_to_char( buf, ch );
	    }
	send_to_char( "----------------------------------------------------------------------\n\r",ch);
        }
    }
}

void do_finger(CHAR_DATA *ch, char * argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    FINGER_DATA * finger;

    argument = one_argument( argument, arg );

    if ( !str_cmp( arg, "email" ) )
    {
	for ( finger = finger_list ; finger != NULL ; finger = finger->next )
	    if ( !str_cmp( finger->name, ch->name ) )
	    {
		strncpy( finger->email, argument, 80 );
		send_to_char( "Email set.\n\r", ch );
		return;
	    }

	send_to_char( "Bug: player not on finger list\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "plan" ) )
    {
	for ( finger = finger_list ; finger != NULL ; finger = finger->next )
	    if ( !str_cmp( finger->name, ch->name ) )
	    {
		if ( finger->desc[0] == '\0' )
		    free_string( finger->desc );

		ch->desc->connected = CON_EDITING;
		string_append( ch, &finger->desc );
		return;
	    }

	bug( "player not on finger list\n\r",0 );
	return;
    }

    for ( finger = finger_list ; finger != NULL ; finger = finger->next )
    {
	if ( str_cmp( finger->name, arg ) )
	    continue;

	sprintf( buf, "Name: %s\n\rTitle: %s\n\rEmail: %s\n\r", finger->name, finger->title, finger->email );
	send_to_char( buf, ch );

	if ( get_char_world( ch, argument ) == NULL )
	    sprintf( buf, "Last on: %s\n\r", (char *) ctime( &finger->laston ) );
	else
	    sprintf( buf, "Currently on.\n\r" );

	send_to_char( buf, ch );

	if ( finger->level > 60 )
	    sprintf( buf, "A %s immortal.\n\r", ( finger->sex == 1 ? "neuter" : ( finger->sex == 2 ? "male" : "female" ) ) );
	else
	    sprintf( buf, "A %s level %d %s.\n\r",( finger->sex == 1 ? "neuter" : ( finger->sex == 2 ? "male" : "female" ) ), finger->level, race_table[ finger->race ][ finger->sex ].name );

	send_to_char( buf, ch );

	if ( finger->desc == NULL || finger->desc[0] == '\0' )
	    sprintf( buf, "Plan: (no plan)\n\r" );
	else
	    sprintf( buf, "Plan: %s\n\r", finger->desc );

	send_to_char( buf, ch );
	return;
    }

    send_to_char( "Player name not found.\n\r", ch );
    return;
}

void do_wizlist(CHAR_DATA *ch, char *argument)
{
    WIZLIST_DATA * wizlist;
    WIZLIST_DATA * wizlist_next;
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    FILE *fp;
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    bool found = FALSE;
    int x = 1;
    output = new_buf();

    if ( argument[0] == '\0' )
    {
	sprintf(buf, "`G               The Immortals of Electric Dreams\n\r");
	add_buf(output, buf);
    for ( x = 1; x < 5; x++ )
    for ( wizlist = wizlist_data_list; wizlist != NULL; wizlist = wizlist_next )
    {
	wizlist_next = wizlist->next;
        sprintf( buf, "%s%s%s%s", PLAYER_DIR, initial(wizlist->name), "/",
	capitalize( wizlist->name ) );
        if ( ( fp = fopen( buf, "r" ) ) == NULL )
	SET_BIT(wizlist->flags, WIZLIST_DELETED );
	if ( fp )
	fclose(fp );
	if (!IS_SET(wizlist->flags, WIZLIST_DELETED ) )
	if ( wizlist->order == x )
	{
	    sprintf(buf, "`B %-15s     -=-  %-15s %-20s\n\r",wizlist->name, wizlist->title, wizlist->desc);
	    add_buf(output, buf);
	}	
    }
    }
    else if ( IS_OPTIVISION(ch) )
    {
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
	if (arg2[0] == '\0' )
	{
	    sprintf(buf,"Syntax:title <name> <title>\n\r");
	    send_to_char(buf,ch);
	    sprintf(buf,"Syntax:desc  <name> <desc>\n\r");
	    send_to_char(buf,ch);
	    sprintf(buf,"Syntax:delete <name>\n\r");
	    send_to_char(buf,ch);
	    free_buf(output);
	    return;
	}
 	if (!strncmp( arg1, "title", 2 ) )
	{
	    for ( wizlist = wizlist_data_list; wizlist != NULL; wizlist = wizlist->next )
	    {
		if ( is_name(arg2, wizlist->name) )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if (!found)
	    {
		sprintf(buf,"Sorry, that name is not found\n\r");	
		send_to_char(buf,ch);
	    	free_buf(output);
		return;
	    }
	    else
	    {
		strncpy( wizlist->title, argument, 30 );		
		sprintf(buf, "Done!\n\r");
		send_to_char(buf,ch);
	    	free_buf(output);
		return;
	    }
	}
 	else if (!strncmp( arg1, "desc", 2 ) )
	{
	    for ( wizlist = wizlist_data_list; wizlist != NULL; wizlist = wizlist->next )
	    {
		if ( is_name(arg2, wizlist->name) )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if (!found)
	    {
		sprintf(buf,"Sorry, that name is not found\n\r");	
		send_to_char(buf,ch);
	    	free_buf(output);
		return;
	    }
	    else
	    {
		strncpy( wizlist->desc, argument, 80 );		
		sprintf(buf, "Done!\n\r");
		send_to_char(buf,ch);
	    	free_buf(output);
		return;
	    }
	}
 	else if (!strncmp( arg1, "delete", 2 ) )
	{
	    for ( wizlist = wizlist_data_list; wizlist != NULL; wizlist = wizlist->next )
	    {
		if ( is_name(arg2, wizlist->name) )
		{
		    found = TRUE;
		    break;
		}
	    }
	    if (!found)
	    {
		sprintf(buf,"Sorry, that name is not found\n\r");	
		send_to_char(buf,ch);
	    	free_buf(output);
		return;
	    }
	    else
	    {
		free_wizlist(wizlist);		
		sprintf(buf, "Done!\n\r");
		send_to_char(buf,ch);
	    	free_buf(output);
		return;
	    }
	}
    	else
    	{
	    sprintf(buf,"Syntax:title <name> <title>\n\r");
	    send_to_char(buf,ch);
	    sprintf(buf,"Syntax:delete <name>\n\r");
	    send_to_char(buf,ch);
	    free_buf(output);
	    return;
    	}
    }
    else
    {
	sprintf(buf,"No argument is used with this command\n\r");
	send_to_char(buf,ch);
	free_buf(output);
	return;
    }
sprintf(buf, "`w");
add_buf(output, buf);
page_to_char(buf_string(output), ch);
free_buf(output);

   /* do_help(ch,"wizlist"); */
}
/*
 * Show a list to a character.
 * Can coalesce duplicated items.
 */
void show_list_to_char( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char **prgpstrShow;
    int *prgnShow;
    char *pstrShow;
    OBJ_DATA *obj;
    int nShow;
    int iShow;
    int count;
    bool fCombine, comm;

    if ( ch->desc == NULL )
	return;

    /*
     * Alloc space for output lines.
     */
    output = new_buf();

    count = 0;
    for ( obj = list; obj != NULL; obj = obj->next_content )
	count++;
    prgpstrShow	= (char **) alloc_mem( count * sizeof(char *) );
    prgnShow    = (int *) alloc_mem( count * sizeof(int)    );
    nShow	= 0;

    comm = ( IS_SET(ch->comm, COMM_COMBINE ) ? TRUE : FALSE );
    if ( count > 50 )
    SET_BIT(ch->comm, COMM_COMBINE );

    /*
     * Format the list of objects.
     */
    for ( obj = list; obj != NULL; obj = obj->next_content )
    { 
	if ( ( obj->wear_data == NULL || obj->wear_data->wear_loc == WEAR_NONE ) 
	    &&  can_see_obj( ch, obj ) 
 	    && ( !IS_SET( obj->extra_flags, ITEM_NO_SHOW ) || IS_IMMORTAL( ch ) ) 
	    && ( !IS_SET( obj->extra_flags, ITEM_BURIED )  || IS_IMMORTAL( ch ) ) )
	{
	    pstrShow = format_obj_to_char( obj, ch, fShort );
	    fCombine = FALSE;

	    if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	    {
		/*
		 * Look for duplicates, case sensitive.
		 * Matches tend to be near end so run loop backwords.
		 */
		for ( iShow = nShow - 1; iShow >= 0; iShow-- )
		{
		    if ( !str_cmp( prgpstrShow[iShow], pstrShow ) )
		    {
			prgnShow[iShow]++;
			fCombine = TRUE;
			break;
		    }
		}
	    }

	    /*
	     * Couldn't combine, or didn't want to.
	     */
	    if ( !fCombine )
	    {
		prgpstrShow [nShow] = str_dup( pstrShow );
		prgnShow    [nShow] = 1;
		nShow++;
	    }
	}
    }

    /*
     * Output the formatted list.
     */
    for ( iShow = 0; iShow < nShow; iShow++ )
    {
	if (prgpstrShow[iShow][0] == '\0')
	{
	    free_string(prgpstrShow[iShow]);
	    continue;
	}

	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	{
	    if ( prgnShow[iShow] != 1 )
	    {
		sprintf( buf, "(%2d) ", prgnShow[iShow] );
		add_buf(output,buf);
	    }
	}
	add_buf(output,prgpstrShow[iShow]);
	sprintf(buf, "`W");
	add_buf(output, buf);
	free_string( prgpstrShow[iShow] );
    }

    if ( fShowNothing && nShow == 0 )
    {
	if ( IS_NPC(ch) || IS_SET(ch->comm, COMM_COMBINE) )
	send_to_char( "Nothing.\n\r", ch );
    }
    page_to_char(buf_string(output),ch);

    /*
     * Clean up.
     */
    free_buf(output);
    free_mem( prgpstrShow, count * sizeof(char *) );
    free_mem( prgnShow,    count * sizeof(int)    );
    if ( !comm ) REMOVE_BIT(ch->comm, COMM_COMBINE );

    return;
}
void get_clanrank( CHAR_DATA *ch, char *argument )
{
    char rank [MAX_INPUT_LENGTH];

    if ( ch->pcdata->pcclan == NULL )
	return;
    if ( ch->pcdata->pcclan->clanrank == 1 )
	strcpy (rank, ch->pcdata->pcclan->clan->rank1);
    if ( ch->pcdata->pcclan->clanrank == 2 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank2);
    if ( ch->pcdata->pcclan->clanrank == 3 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank3);
    if ( ch->pcdata->pcclan->clanrank == 4 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank4);
    if ( ch->pcdata->pcclan->clanrank == 5 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank5);
    if ( ch->pcdata->pcclan->clanrank == 6 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank6);
    if ( ch->pcdata->pcclan->clanrank == 7 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank7);
    if ( ch->pcdata->pcclan->clanrank == 8 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank8);
    if ( ch->pcdata->pcclan->clanrank == 9 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank9);
    if ( ch->pcdata->pcclan->clanrank == 10 )
        strcpy (rank, ch->pcdata->pcclan->clan->rank10);

    strcpy (argument, rank);
    return;
}    

void do_vitals( CHAR_DATA *ch, char *argument )
{   
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char pos[MAX_INPUT_LENGTH];
    BUFFER *output;
    bool short_out = FALSE;
    bool multi = FALSE;
    
    output = new_buf();
    argument = one_argument( argument, arg );
    
    while ( arg[0] != '\0' )
    {
        if ( !strcasecmp( arg, "-s") )
        {
            // The short option will make it print all the arguments
            // on a single line, in a tighter format.
            short_out = TRUE;
            argument = one_argument( argument, arg );
            continue;
        }
        
        if ( short_out && multi )
            add_buf(output, ", ");        
        
        if ( !str_prefix( arg, "level") )
        {
            if ( short_out )
                sprintf( buf, "level %d", ch->level );
            else
                sprintf( buf, "You are level %d.\n\r", ch->level );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "hits") || !str_prefix( arg, "hp") )
        {
            if ( short_out )
                sprintf( buf, "%d hp", ch->hit );
            else
                sprintf( buf, "You have %d hit points.\n\r", ch->hit );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "moves") || !strcasecmp( arg, "mv") )
        {
            if ( short_out )
                sprintf( buf, "%d move", ch->move );
            else
                sprintf( buf, "You have %d move.\n\r", ch->move );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "fire") )
        {
            if ( short_out )
                sprintf( buf, "%d fire", ch->mana[0] );
            else
                sprintf( buf, "You have %d fire mana.\n\r", ch->mana[0] );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "water") )
        {
            if ( short_out )
                sprintf( buf, "%d water", ch->mana[1] );
            else
                sprintf( buf, "You have %d water mana.\n\r", ch->mana[1] );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "earth") )
        {
            if ( short_out )
                sprintf( buf, "%d earth", ch->mana[2] );
            else
                sprintf( buf, "You have %d earth mana.\n\r", ch->mana[2] );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "wind") )
        {
            if ( short_out )
                sprintf( buf, "%d wind", ch->mana[3] );
            else
                sprintf( buf, "You have %d wind mana.\n\r", ch->mana[3] );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "spirit") )
        {
            if ( short_out )
                sprintf( buf, "%d spirit", ch->mana[4] );
            else
                sprintf( buf, "You have %d spirit mana.\n\r", ch->mana[4] );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "armor") || !str_prefix( arg, "ac") )
        {
            if ( short_out )
                sprintf( buf, "%d ac", GET_AC(ch) );
            else
                sprintf( buf, "You have %d armor rating.\n\r", GET_AC(ch) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "practices") || !str_prefix( arg, "pracs") )
        {
            if ( short_out )
                sprintf( buf, "%d pracs", ch->practice );
            else
                sprintf( buf, "You have %d practices.\n\r", ch->practice );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "strength") )
        {
            if ( short_out )
                sprintf( buf, "%d str", get_curr_stat(ch, STAT_STR) );
            else
                sprintf( buf, "You have %d strength.\n\r", get_curr_stat(ch, STAT_STR) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "dexterity") )
        {
            if ( short_out )
                sprintf( buf, "%d dex", get_curr_stat(ch, STAT_DEX) );
            else
                sprintf( buf, "You have %d dexterity.\n\r", get_curr_stat(ch, STAT_DEX) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "constitution") )
        {
            if ( short_out )
                sprintf( buf, "%d con", get_curr_stat(ch, STAT_CON) );
            else
                sprintf( buf, "You have %d constitution.\n\r", get_curr_stat(ch, STAT_CON) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "intelligence") )
        {
            if ( short_out )
                sprintf( buf, "%d int", get_curr_stat(ch, STAT_INT) );
            else
                sprintf( buf, "You have %d intelligence.\n\r", get_curr_stat(ch, STAT_INT) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "wisdom") )
        {
            if ( short_out )
                sprintf( buf, "%d wis", get_curr_stat(ch, STAT_WIS) );
            else
                sprintf( buf, "You have %d wisdom.\n\r", get_curr_stat(ch, STAT_WIS) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "charisma") )
        {
            if ( short_out )
                sprintf( buf, "%d cha", get_curr_stat(ch, STAT_CHR) );
            else
                sprintf( buf, "You have %d charisma.\n\r", get_curr_stat(ch, STAT_CHR) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "charisma") )
        {
            if ( short_out )
                sprintf( buf, "%d cha", get_curr_stat(ch, STAT_CHR) );
            else
                sprintf( buf, "You have %d charisma.\n\r", get_curr_stat(ch, STAT_CHR) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "hitroll") || !str_prefix( arg, "hr") )
        {
            if ( short_out )
                sprintf( buf, "%d hr", GET_HITROLL(ch) );
            else
                sprintf( buf, "You have %d hitroll.\n\r", GET_HITROLL(ch) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "damroll") || !str_prefix( arg, "dr") )
        {
            if ( short_out )
                sprintf( buf, "%d dr", GET_DAMROLL(ch) );
            else
                sprintf( buf, "You have %d damroll.\n\r", GET_DAMROLL(ch) );
            add_buf(output, buf);
        }
        else if ( !str_prefix( arg, "status") )
        {
            sprintf( pos, "%s", get_pos_name( ch->position ) );
            pos[0] = LOWER( pos[0] );
            
            if ( short_out )
                sprintf( buf, "%s", pos );
            else
                sprintf( buf, "You are %s.\n\r", pos );
            add_buf(output, buf);
                        
            if ( ch->pcdata->condition[COND_DRUNK] > 10 )
            {
                if ( short_out )
                    add_buf(output, ", drunk");
                else
                    add_buf(output, "You are drunk.\n\r");
            }
            if ( ch->pcdata->condition[COND_FULL] == 0 && !IS_IMMORTAL( ch ) )
            {
                if ( short_out )
                    add_buf(output, ", hungry");
                else
                    add_buf(output, "You are hungry.\n\r");
            }
            if ( ch->pcdata->condition[COND_THIRST] == 0 && !IS_IMMORTAL( ch ) )
            {
                if ( short_out )
                    add_buf(output, ", thirsty");
                else
                    add_buf(output, "You are thirsty.\n\r");
            }
        }
        else
        {
            sprintf( buf, "Invalid vitals argument: %s\n\r", arg );
            send_to_char( buf, ch );
            free_buf(output);
            return;
        }
        
        multi = TRUE;
        argument = one_argument( argument, arg );
    }
    
    char *str = buf_string(output);
    if ( *str == '\0' )
    {
        // There is no output buffer, this catches passing no arguments,
        // passing only "-s" or passing only invalid arguments.
        add_buf(output, "Syntax: vitals [-s] (stat) ...\n\r\n\r");
        add_buf(output, "Stats: level hp mv fire water earth wind spirit\n\r");
        add_buf(output, "  ac pracs str dex con int wis cha hr dr status\n\r");
    }
    else if ( short_out )
    {
        // The short sprints to the output buffer don't include any
        // newlines, so add a trailing one.
        add_buf(output, "\n\r");
        str = buf_string(output);
    }
    
    page_to_char(str, ch);
    free_buf(output);
    return;
}


// Support functions for do_changes

void change_attach( CHAR_DATA *ch )
{
    CHANGE_DATA *change;

    if ( ch->pcdata->change_editing != NULL )
        return;

    change = change_data_alloc();

    change->next     = NULL;
    change->text     = &str_empty[0];
    strncpy( change->author, ch->name, 30 );

    ch->pcdata->change_editing = change;
    return;
}

void change_add( CHANGE_DATA *change )
{
    CHANGE_DATA * prev;
    
    if ( change->vnum < 1 )
        change->vnum = ++top_change;
    else
        top_change = UMAX( change->vnum, top_change );

    if ( change_list == NULL )
    {
        // This is the first change, woop
        change_list = change;
    }
    else if ( change->date_stamp > change_list->date_stamp )
    {
        // This is the most recent change, so just put it at the top
        change->next = change_list;
        change_list = change;
    }
    else
    {
        // There is a newer change at the top of the list, find our spot
        for ( prev = change_list; prev != NULL ; prev = prev->next )
        {
            if ( prev->next == NULL )
            {
                // We hit the end, so just stick this on there
                prev->next = change;
                break;
            }
            else if ( change->date_stamp > prev->next->date_stamp )
            {
                // We found the next one that is older than this
                change->next = prev->next;
                prev->next = change;
                break;
            }
        }
    }
    
    save_changes();
    return;
}

void change_remove( CHANGE_DATA *change )
{
    CHANGE_DATA *prev;
    
    if ( change == change_list )
        change_list = change->next;
    else
    {
        for ( prev = change_list; prev != NULL; prev = prev->next )
        {
            if ( prev->next == change )
            break;
        }

        if ( prev == NULL )
        {
            bug( "Change_remove: change not found.", 0 );
            return;
        }

        prev->next = change->next;
    }

    change->next = change_free;
    change_free = change;

    save_changes();
    return;
}

void check_new_changes( CHAR_DATA *ch )
{
    CHANGE_DATA *change;

    for ( change = change_list ; change != NULL ; change = change->next )
    {
        if ( ch->pcdata->last_change < change->date_stamp
        && ch->pcdata->security <= change->security )
        {
            send_to_char( "`YThere are new changes! Type 'changes' to view them.\n\r", ch );
            return;
        }
    }
}

void lines_to_spaces( char *str )
{
    while (*str++ != '\0')
    {
        if (*str == '\n')
            *str = ' ';
        else if (*str == '\r')
            *str = ' ';
    }
}

void do_changes( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_BUF];
    char stamp[22];
    int number;
    CHANGE_DATA *change;
    DESCRIPTOR_DATA *d;
    BUFFER *output;
    time_t new_time = 0;

    if ( IS_NPC(ch) )
        return;

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || !str_prefix( arg, "all" ) || !IS_IMMORTAL( ch ) )
    {
        // Read new or all changes, paged, most recent first
        if ( change_list == NULL )
        {
            send_to_char( "No changes posted.\n\r", ch );
            return;
        }

        output = new_buf();
        bool first = TRUE;
        for ( change = change_list ; change != NULL ; change = change->next )
        {
            if ( ( ch->pcdata->security <= change->security )
              && ( ch->pcdata->last_change < change->date_stamp
                || ( arg[0] != '\0' && !str_prefix( arg, "all" ) ) ) )
            {
                // We have permission to view this and either it is newer than
                //  we've seen or we are listing all changes
                if ( !first )
                    add_buf( output, "\n\r" );
                else
                    first = FALSE;
                strftime( stamp, 22, "%a %b %d %H:%M %Y", localtime( &change->date_stamp ) );
                sprintf( buf, "%s by %s:\n\r", stamp, change->author );
                add_buf( output, buf );
                add_buf( output, change->text );
                // If this is the first visible one, we want it to be
                //  their new last_change, but we don't want to change
                //  it until we've compared the rest of the list
                if ( !new_time )
                    new_time = change->date_stamp;
            }
            else if ( ch->pcdata->last_change >= change->date_stamp
                   && str_prefix( arg, "all" ) )
                // We've seen this before, no need to check the rest
                break;
        }

        char *str = buf_string(output);
        if ( *str == '\0' )
        {
            if ( arg[0] == '\0' )
                // Either there weren't any new changes, or
                //  they did not have the security to view them
                send_to_char( "No new changes. Try '`Wchanges all`w'.\n\r", ch );
            else
                // If we got here, then there were some changes, but
                //  they did not have the security to view them
                send_to_char( "No changes posted.\n\r", ch );
        }
        else
        {
            send_to_char( "`Y", ch );
            page_to_char( str, ch );
            send_to_char( "`w", ch );
            ch->pcdata->last_change = new_time;
        }

        free_buf( output );
        return;
    }

    // Immortal only arguments past this point

    if ( !str_prefix( arg, "add" ) )
    {
        if ( ch->pcdata->change_editing != NULL )
        {
            send_to_char( "You are already editing changes, either '`Ypost`w' or '`Rdiscard`w' them.\n\r", ch );
            return;
        }

        change_attach( ch );
        do_changes( ch, "edit" );
    }
    else if ( !str_prefix( arg, "show" ) )
    {
        change = ch->pcdata->change_editing;

        if ( change == NULL )
        {
            send_to_char( "You are not editing changes, try '`Yadd`w'.\n\r", ch );
            return;
        }

        output = new_buf();
        strftime( stamp, 22, "%a %b %d %H:%M %Y", localtime( &change->date_stamp ) );
        sprintf( buf, "%s by %s:\n\r", stamp, change->author );
        add_buf( output, buf );
        add_buf( output, change->text );
        page_to_char( buf_string( output ), ch );
        free_buf( output );
    }
    else if ( !str_prefix( arg, "edit" ) )
    {
        change = ch->pcdata->change_editing;

        if ( change == NULL )
        {
            send_to_char( "You are not editing changes, try '`Yadd`w'.\n\r", ch );
            return;
        }

        send_to_char( "`Y", ch );
        ch->desc->connected = CON_EDITING;
        string_append( ch, &change->text );
    }
    else if ( !str_prefix( arg, "post" ) )
    {
        change = ch->pcdata->change_editing;

        if ( change == NULL )
        {
            send_to_char( "You are not editing changes, try `Yadd`w.\n\r", ch );
            return;
        }

        if ( is_number( argument ) )
        {
            // A minimum security these changes are visible to
            number = atoi( argument );
            if ( number < 1 || number > 9 )
            {
                send_to_char( "Valid security level is `W1-9`w.\n\r", ch );
                return;
            }
            change->security = number;
        }
        // If no security was provided, it defaults to 9 (everyone)

        // If the author's last viewed change was the latest one,
        //  just update it so they don't need to view their own
        CHANGE_DATA *last;
        for ( last = change_list ; last != NULL ; last = last->next )
            if ( ch->pcdata->security <= last->security )
                break;
        if ( last == NULL || ch->pcdata->last_change >= last->date_stamp )
        {
            ch->pcdata->last_change = change->date_stamp;
        }

        change_add( change );
        ch->pcdata->change_editing = NULL;
        send_to_char( "Changes posted.\n\r", ch );

        // Notify players that there are new changes
        for ( d = descriptor_list ; d != NULL ; d = d->next )
        {
            if ( d->connected <= CON_PLAYING )
                check_new_changes( d->character );
        }
    }
    else if ( !str_prefix( arg, "discard" ) )
    {
        change = ch->pcdata->change_editing;

        if ( change == NULL )
        {
            send_to_char( "You are not editing changes.\n\r", ch );
            return;
        }

        change->next = change_free;
        change_free = change;
        ch->pcdata->change_editing = NULL;
        send_to_char( "Changes discarded.\n\r", ch );
    }
    else if ( !str_prefix( arg, "list" ) )
    {
        if ( change_list == NULL )
        {
            send_to_char( "No changes to list.\n\r", ch );
            return;
        }

        char sum[31]; sum[30] = '\0';
        char auth[11]; auth[10] = '\0';
        output = new_buf();
        // I don't really care that these will be in reverse vnum order,
        //  chances are if you want to delete one it will be recent
        for ( change = change_list ; change != NULL ; change = change->next )
        {
            strncpy( sum, change->text, 30 );
            lines_to_spaces( (char *)sum );
            strncpy( auth, change->author, 10 );
            strftime( stamp, 22, "%a %b %d %H:%M %Y", localtime( &change->date_stamp ) );
            sprintf( buf, "[%3d] %s by %-10s [%d]: %s\n\r",
                     change->vnum, stamp, auth, change->security, sum );
            add_buf( output, buf );
        }

        page_to_char( buf_string( output ), ch );
        free_buf( output );
    }
    else if ( !str_prefix( arg, "remove" ) )
    {
        if ( !is_number( argument ) )
        {
            send_to_char( "Which change do you want to remove?\n\r", ch );
            return;
        }

        number = atoi( argument );
        for ( change = change_list ; change != NULL ; change = change->next )
        {
            if ( change->vnum == number )
            {
                change_remove( change );
                send_to_char( "Change removed.\n\r", ch );
                return;
            }
        }
        
        // Ran out of changes
        send_to_char( "Change not found.\n\r", ch );
    }
    else
    {
        send_to_char( "Bad argument. Try '`Whelp changes`w'.\n\r", ch );
    }
    
    return;
}
