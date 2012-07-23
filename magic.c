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
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"

/*
 * Lookup a skill by name.
 */
int skill_lookup( const char *name )
{
    char arg[5][MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char *skill;
    char *argument;
    int sn,i,args;
    bool found;

    if ( name[0] == '\0' )
    {
    bug( "Skill_lookup: NULL Name", 0 );
    return -1;
    }	
    argument = (char *) name;
    for ( i = 0; *( argument = one_argument( argument, arg[i] ) ) != '\0' && i < 4; i++ );

    args = i;
    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	found = TRUE;
	if ( ( skill = skill_table[ sn ].name ) == NULL )
	    continue;

  	for ( i = 0 ; i <= args ; i++ )
	{
	    skill = one_argument( skill, buf );
	    if ( LOWER(arg[i][0]) != LOWER(buf[0]) 
		|| str_prefix( arg[i], buf ) )
		found = FALSE;
	}
	if ( found )
	    return sn;
    }

    if ( args == 0 )
    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	found = TRUE;

	if ( ( skill = skill_table[ sn ].name ) == NULL )
	    continue;

  	for ( i = 0 ; arg[0][i] != '\0' ; i++ )
	{
	    skill = one_argument( skill, buf );
	    if ( LOWER(arg[0][i]) != LOWER(buf[0]) )
		found = FALSE;
	}
	if ( found )
	    return sn;
    }

    return -1;
}



/*
 * Lookup a skill by slot number.
 * Used for object loading.  
 * Not any more.. G

int slot_lookup( int slot )
{
    extern bool fBootDb;
    int sn;

    if ( slot <= 0 )
	return -1;

    for ( sn = 0; sn < MAX_SKILL; sn++ )
    {
	if ( slot == skill_table[sn].slot )
	    return sn;
    }

    if ( fBootDb )
    {
	bug( "Slot_lookup: bad slot %d.", slot );
	abort( );
    }

    return -1;
}
*/

void say_spell( CHAR_DATA *ch, void *victim, int sn, bool pass )
{
    if ( victim == NULL )
	victim = ch;

    if ( pass )
    {
        cact(skill_table[sn].char_pass,ch,victim,victim,TO_CHAR,"`W");
	if ( !IS_SET( skill_table[sn].target, TAR_IGNORE|TAR_OBJ_INV))
	    cact(skill_table[sn].other_pass,ch,victim,victim,TO_NOTVICT,"`W");
	else 
	    cact(skill_table[sn].other_pass,ch,victim,victim,TO_ROOM,"`W");
	if ( victim != ch && !IS_SET( skill_table[sn].target, TAR_CHAR_SELF|TAR_IGNORE|TAR_OBJ_INV))
	    cact(skill_table[sn].vict_pass,ch,victim,victim,TO_VICT,"`W");
    }
    else
    {
        cact(skill_table[sn].char_fail,ch,victim,victim,TO_CHAR,"`W");
	if ( !IS_SET( skill_table[sn].target, TAR_IGNORE|TAR_OBJ_INV))
	    cact(skill_table[sn].other_fail,ch,victim,victim,TO_NOTVICT,"`W");
	else 
	    cact(skill_table[sn].other_fail,ch,victim,victim,TO_ROOM,"`W");
	if ( victim != ch && !IS_SET( skill_table[sn].target, TAR_CHAR_SELF|TAR_IGNORE|TAR_OBJ_INV))
	    cact(skill_table[sn].vict_fail,ch,victim,victim,TO_VICT,"`W");
    }	
    return;
}

/*
 * Compute a saving throw.
 * Positive apply's make saving throw better.
 */
bool saves_spell( int level, CHAR_DATA *victim )
{
    int save;

    save = 50 + ( victim->level - level + victim->saving_throw );
    if (IS_AFFECTED(victim,AFF_BERSERK))
	save -= victim->level/2;

    save = URANGE( 5, save, 95 );
    return number_percent( ) < save;
}

/* RT save for dispels */

bool saves_dispel( int dis_level, int spell_level, int duration)
{
    int save;
    
    if (duration == -1)
      spell_level += 5;  
      /* very hard to dispel permanent effects */

    save = 50 + (spell_level - dis_level) * 5;
    save = URANGE( 5, save, 95 );
    return number_percent( ) < save;
}

/* co-routine for dispel magic and cancellation */

bool check_dispel( int dis_level, CHAR_DATA *victim, int sn)
{
    AFFECT_DATA *af;

    if (is_affected(victim, sn))
    {
        for ( af = victim->affected; af != NULL; af = af->next )
        {
            if ( af->type == sn )
            {
                if (!saves_dispel(dis_level,af->level,af->duration))
                {
                    affect_strip(victim,sn);
        	    if ( skill_table[sn].msg_off )
        	    {
            		send_to_char( skill_table[sn].msg_off, victim );
            		send_to_char( "\n\r", victim );
        	    }

		    return TRUE;
		}
		else
		    af->level--;
            }
        }
    }
    return FALSE;
}

/*
 * The kludgy global is for spells who want more stuff from command line.
 */
char *target_name;
void do_cast( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    char argument1[MAX_INPUT_LENGTH];
    char argument2[MAX_INPUT_LENGTH];
    char argument3[MAX_INPUT_LENGTH];
    char argument4[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    char *name;
    void *vo;
    int penalty = 0;
    int i;
    int sn = 0;
    int elevel;
    int max = 0 ;
    int level = 0;
    int total = 0;
    int num = 0;
    int chance = 100;
    int number_per;
    int skill;
    bool found = FALSE;
    buf2[0] = '\0';

    strcpy( argument1, argument );
    argument = one_argument(argument, arg1);
    strcpy( argument2, argument );
    argument = one_argument(argument, arg2);
    strcpy( argument3, argument );
    argument = one_argument(argument, arg3);
    strcpy( argument4, argument );
    one_argument(argument, arg4);

    if (arg4[0] != '\0')
    {
        strcpy(buf2, arg1);
        strcat(buf2, " ");
        strcat(buf2, arg2);
        strcat(buf2, " ");
        strcat(buf2, arg3);
        strcat(buf2, " ");
        target_name = argument4;

        sn = skill_lookup( buf2 );
    }

    if ( sn < 1 && arg3[0] != '\0')
    {
        strcpy(buf2, arg1);
        strcat(buf2, " ");
        strcat(buf2, arg2);
        strcat(buf2, " ");
        strcat(buf2, arg3);
        target_name = argument4;

        sn = skill_lookup( buf2 );
    }

    if ( sn < 1 && arg2[0] != '\0')
    {
        strcpy(buf2, arg1);
        strcat(buf2, " ");
        strcat(buf2, arg2);
        sn = skill_lookup( buf2 );
        target_name = argument3;
    }

    if (sn < 1 && arg1[0] != '\0')
    {
        sn = skill_lookup( arg1 );
        target_name = argument2;
    }

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Cast which what where?\n\r", ch );
        return;
    }

    if ( sn < 1 )
    {
        send_to_char( "You don't know any spells by that name.\n\r", ch );
        return;
    }

    REMOVE_BIT( ch->affected_by, AFF_HIDE );
    REMOVE_BIT( ch->affected_by_2, AFF_FADE );

    if ( ch->position < skill_table[sn].minimum_position || IS_AFFECTED( ch, AFF_BERSERK ) )
    {
        send_to_char( "You can't concentrate enough.\n\r", ch );
        return;
    }

    if ( IS_SET( ch->in_room->room_flags, ROOM_SILENCE ) &&
        !IS_SET( skill_table[sn].target, SPELL_SILENT ) )
    {
        send_to_char( "You are unable to say the spell.\n\r", ch );
        return;
    }

    if ( !IS_NPC( ch ) )
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        if ( ( elevel = skill_table[ sn ].element[ i ].level ) < 0 )
            continue;

        if ( skill_table[ sn ].element[ i ].mana > max )
            max = skill_table[ sn ].element[ i ].mana;

        if ( get_skill( ch, gsn_element_power[ i ][ elevel ] ) <= 0 )
        {
            sprintf(buf, "You do not have enough %s power to cast this spell.\n\r", element_type_name(i) );
            send_to_char(buf, ch);
            return;
        }

        if ( ch->mana[ i ] < get_mana_cost( ch, sn, i ) )
        {
            send_to_char( "You do not have enough mana to cast this spell.\n\r", ch );
            return;
        }
    }

    if ( IS_NPC( ch ) )
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        if ( ch->mana[ i ] < get_mana_cost( ch, sn, i ) )
        {
            send_to_char( "You do not have enough mana to cast this spell.\n\r", ch );
            return;
        }
    }

    if ( !IS_NPC( ch ) )
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        if ( skill_table[ sn ].element[ i ].level < 0
          || max != skill_table[ sn ].element[ i ].mana )
            continue;
 chance += get_skill( ch, gsn_element_power[ i ][ skill_table[ sn ].element[ i ].level ] );
 total += get_skill( ch, gsn_element_power[ i ][ 0 ] ) + get_skill( ch, gsn_element_power[ i ][ 1 ] ) + get_skill( ch, gsn_element_power[ i ][ 2 ] );
 num++;
    }

    if ( chance == 0 || num == 0 )
        chance = get_skill(ch, sn);
    else
        chance = chance / num;

    if ( num == 0 || total == 0 )
        level = ch->level / 10 + 1;
    else
        level = total / ( 6 * num );

    if ( IS_NPC( ch ) )
    {
        level = ch->level;
        chance = get_skill( ch, sn );
    }

    /*
     * Locate targets.
     */
    victim      = NULL;
    obj         = NULL;
    vo          = NULL;
    target_name = strlen( target_name )  > 1 ? target_name : NULL;
    if ( IS_SET( skill_table[sn].target, TAR_CHAR_OTHER ) )
    {
        bool Valid=FALSE;

        name = ( target_name != NULL ? target_name : NULL );
/*
        while( *name != '\0' && !Valid )
            if ( !isalpha( name++ ) )
                Valid=TRUE;
*/
        if ( name )
             Valid = TRUE;
        if ( !target_name )
             Valid = FALSE;

        if ( !Valid )
        {
            if  ( ( victim = ch->fighting ) == NULL && IS_SET( skill_table[sn].target, TAR_CHAR_OFFENSIVE|TAR_CHAR_DAMAGING ) )
            {
                send_to_char("You didn't specify a target!\n\r",ch);
                return;
            }
            else
            {
                if (victim == NULL) /* If victim = ch->fighting from previous, we donot want to reassign the victim */
                    victim = ch;
                else if ( !can_see( ch, victim ) )
                {
                    send_to_char( "You cannot find them.\n\r", ch );
                    return;
                }
            }
        }
        else
        {
            if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
            {
                send_to_char( "They aren't here.\n\r", ch );
                return;
            }
        }

        if ( !IS_NPC(ch) )
        {
            if ( victim != NULL )
                if ( is_safe_spell( ch, victim, FALSE ) && victim != ch)
                {
                    send_to_char("Not on that target.\n\r",ch);
                    return;
                }
        }

        if ( IS_AFFECTED(ch, AFF_CONTROL_MIND) && ch->master == victim )
        {
            send_to_char( "You can't do that on your own follower.\n\r", ch );
            return;
        }

        if ( IS_SET( skill_table[sn].target, TAR_CHAR_DAMAGING ) )
        {
            if ( victim == ch )
            {
                send_to_char( "You can't cast that on yourself!\n\r", ch );
                return;
            }
        }

        vo = (void *) victim;
    }
    else if ( IS_SET( skill_table[sn].target, TAR_CHAR_SELF ) )
    {
        if ( target_name != NULL && !is_name( target_name, ch->name ) )
        {
            send_to_char( "You cannot cast this spell on another.\n\r", ch );
            return;
        }
        vo = (void *) ch;
    }
    else if ( IS_SET( skill_table[sn].target, TAR_IGNORE ) )
        vo = NULL;
    else if ( IS_SET( skill_table[sn].target, TAR_OBJ_INV ) )
    {
        if ( target_name == NULL )
        {
            send_to_char( "What should the spell be cast upon?\n\r", ch );
            return;
        }

        if ( ( obj = get_obj_carry( ch, target_name ) ) == NULL )
        {
            send_to_char( "You are not carrying that.\n\r", ch );
            return;
        }

        vo = (void *) obj;
    }
    else
    {
        if ( target_name == NULL )
        {
            victim = ch;
        }
        else
        {
            if ( ( victim = get_char_room( ch, target_name ) ) == NULL )
            {
                send_to_char( "They aren't here.\n\r", ch );
                return;
            }
        }

        vo = (void *) victim;
    }

    if ( !IS_NPC( ch ) && !IS_IMMORTAL( ch ) )
    for ( i = 0 ; i < MAX_COMPONENT ; i++ )
    {
        if ( skill_table[ sn ].component[ i ].vnum == 0 )
            continue;

        if ( IS_SET( skill_table[ sn ].component[ i ].flags, COMP_RARE ) )
        {
            if ( skill_table[ sn ].component[ i ].vnum == COMP_RARE )
            {
                found = FALSE;
                for ( obj = ch->carrying ; obj != NULL && !found ; obj = obj->next_content )
                {
                    if ( obj->edible_data != NULL
                      && !IS_SET( obj->edible_data->flags, EDIBLE_FOOD )
                      && obj->edible_data->type == 0
                      && obj->edible_data->amount > 0 )
                    {
                        found = TRUE;

                    if ( IS_SET( skill_table[ sn ].component[ i ].flags, COMP_RARE ) )
                        obj->edible_data->amount = 0;
                    }
                }

                if ( !found )
                    penalty += skill_table[ sn ].component[ i ].vnum_2;
            }
        }
        else
        {
            found = FALSE;
            for ( obj = ch->carrying ; obj != NULL && !found ; obj = obj->next_content )
                if ( obj->pIndexData->vnum == skill_table[ sn ].component[ i ].vnum )
                {
                    found = TRUE;
                    if ( IS_SET( skill_table[ sn ].component[ i ].flags, COMP_RARE ) )
                        extract_obj( obj );
                }
            if ( !found )
                penalty += skill_table[ sn ].component[ i ].vnum_2;
        }
    }

    WAIT_STATE( ch, skill_table[sn].beats );
    number_per = number_percent();

/*  I use this for testing mobiles and their percentage casting chances
    bug( "Character = : %d.", ch->pIndexData->vnum );
    bug( "Skill     = : %d.", sn );
    bug( "Chance    = : %d.", chance );
    bug( "Penalty   = : %d.", penalty );
    bug( "Get Skill = : %d.", get_skill( ch, sn ) ); */
    if ( IS_AFFECTED( ch, AFF_COLD ) && number_per > get_curr_stat( ch, STAT_CON ) * 3 )
    {
        send_to_char( "Aachoo!\n\r", ch );
        act( "$n `wsneezes.", ch, NULL, NULL, TO_ROOM );
        for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
            if ( skill_table[ sn ].element[ i ].level >= 0 )
                ch->mana[ i ] -= number_range( 0, get_mana_cost( ch, sn, i ) );
    }
    else if ( penalty >= 100 || number_per > ( skill = get_skill( ch, sn ) - penalty )
        || number_per > chance )
    {
        say_spell( ch, vo, sn, FALSE );
/*      send_to_char( "You failed.\n\r", ch ); */
/*  I use this for testing mobiles and their percentage casting chances
    bug( "Character = : %d.", ch->pIndexData->vnum );
    bug( "Skill     = : %d.", sn );
    bug( "Number_per= : %d.", number_per );
    bug( "Chance    = : %d.", chance );
    bug( "Penalty   = : %d.", penalty );
    bug( "Get Skill = : %d.", get_skill( ch, sn ) );
    bug( "Get level = : %d.", get_skill_level( ch, sn ) ); */
        if ( !IS_NPC( ch ) && skill_table[sn].spell_fun != spell_null )
            check_improve(ch,sn,FALSE,2);
        for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
            if ( skill_table[ sn ].element[ i ].level >= 0 )
            {
                if ( !IS_NPC( ch ) )
                    check_improve(ch, gsn_element_power[ i ][ skill_table[ sn ].element[ i ].level ], FALSE, 2 );
                ch->mana[ i ] -= number_range( 0, get_mana_cost( ch, sn, i ) );
            }
    }
    else
    {
        for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
            if ( skill_table[ sn ].element[ i ].level >= 0 )
                ch->mana[ i ] -= get_mana_cost( ch, sn, i);

        if ( victim != NULL && !is_affected( victim, sn ) && !IS_NPC(ch) )
            check_mana_improve( ch, sn );

        say_spell( ch, vo, sn, TRUE );
        (*skill_table[sn].spell_fun) ( sn, level, ch, vo );
        if ( !IS_NPC( ch ) && skill_table[sn].spell_fun != spell_null )
            check_improve(ch,sn,TRUE,URANGE( 1, get_skill_cost( ch, sn ) -1, 10 ) );

        if ( !IS_NPC( ch ) )
            for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
                if ( skill_table[ sn ].element[ i ].level >= 0 )
                    check_improve(ch, gsn_element_power[ i ][ skill_table[ sn ].element[ i ].level ], TRUE, URANGE( 1, get_skill_cost( ch, sn ) -1, 10 ) );

    }

     if (victim == NULL)
        return;
    if ( IS_SET( skill_table[sn].target, START_FIGHTING )
    &&   victim != ch
    &&   victim->master != ch)
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;

        for ( vch = ch->in_room->people; vch; vch = vch_next )
        {
            vch_next = vch->next_in_room;
            if ( victim == vch && victim->fighting == NULL )
            {
                multi_hit( victim, ch, TYPE_UNDEFINED );
                break;
            }
        }
    }

    return;
}

/*
 * Cast spells at targets using a magical object.
 */
void obj_cast_spell( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, SPELL_LIST * spells , int type ) 
{
    void *vo;
    SPELL_LIST * spell;
    int sn,level;
    

    for ( spell = spells ; spell != NULL ; spell = spell->next )
    {
	sn = spell->spell;
	level = spell->level;

	if ( type == MAGIC_STAFF )
	{

	    if ( IS_SET( skill_table[sn].target , TAR_IGNORE|TAR_CHAR_SELF ) && victim != ch )
  	    	continue;

	    if ( IS_SET( skill_table[sn].target, TAR_CHAR_DAMAGING|TAR_CHAR_OFFENSIVE ) && ( IS_NPC(ch) ? IS_NPC(victim) : !IS_NPC(victim) ) )
		    continue;

	    if ( IS_SET( skill_table[sn].target, TAR_CHAR_CURATIVE|TAR_CHAR_DEFENSIVE ) && ( IS_NPC(ch) ? !IS_NPC(victim) : IS_NPC(victim) ) )
		    continue;

	}

        if ( sn >= MAX_SKILL || skill_table[sn].spell_fun == 0 )
        {
	    bug( "Obj_cast_spell: bad sn %d.", sn );
	    return;
        }

        if ( IS_SET( skill_table[sn].target, TAR_CHAR_OTHER ) )
        {
	    if ( victim == NULL )
	        victim = ch->fighting;

	    if ( victim == NULL )
	    {
	        send_to_char( "You can't do that.\n\r", ch );
	        return;
	    }

	    if (is_safe_spell(ch,victim,FALSE) && ch != victim)
	    {
	        send_to_char("Something isn't right...\n\r",ch);
	        return;
	    }

	    vo = (void *) victim;
        }
        else if ( IS_SET( skill_table[sn].target, TAR_CHAR_SELF ) )
	    vo = (void *) ch;
        else if ( IS_SET( skill_table[sn].target, TAR_IGNORE ) )
	    vo = NULL;
        else if ( IS_SET( skill_table[sn].target, TAR_OBJ_INV ) )
        {
	    if ( obj == NULL )
	    {
	        send_to_char( "You can't do that.\n\r", ch );
	        return;
	    }
	    vo = (void *) obj;
        }
        else
        {
	    if ( victim == NULL )
	         victim = ch;

	    vo = (void *) victim;
        }

        target_name = "";
        (*skill_table[sn].spell_fun) ( sn, level, ch, vo );
        if ( IS_SET( skill_table[sn].target, START_FIGHTING )
	&&   victim != NULL
        &&   victim != ch
        &&   victim->master != ch )
        {
	    CHAR_DATA *vch;
	    CHAR_DATA *vch_next;

	    for ( vch = ch->in_room->people; vch; vch = vch_next )
	    {
	        vch_next = vch->next_in_room;
	        if ( victim == vch && victim->fighting == NULL )
	        {
		    multi_hit( victim, ch, TYPE_UNDEFINED );
		    break;
	        }
	    }
	}
    }

    return;
}

void trap_cast_spell( CHAR_DATA *ch, TRAP_DATA * trap ) 
{
    CHAR_DATA * vch = NULL;
    CHAR_DATA * victim;
    SPELL_LIST * spell;
    int sn,level;
    int count = 0;
    bool fArea = FALSE;

    if ( trap == NULL || IS_SET( trap->flags, TRAP_SPRUNG ) )
	return;

    act( "You have set off a trap!", ch, NULL, NULL, TO_CHAR );
    act( "$n `whas set off a trap!", ch, NULL, NULL, TO_ROOM );

    if (!IS_SET(trap->flags, TRAP_AUTO_RESET ) )
    SET_BIT( trap->flags, TRAP_SPRUNG );

    for ( victim = ch->in_room->people ; victim != NULL ; victim = victim->next_in_room )
    {
	if  ( ( !IS_NPC( ch ) && IS_NPC( victim ) )
	  || ( IS_NPC( ch ) && !IS_NPC( victim ) ) )
	    continue;

	if ( number_range( 0, count ) == 0 )
	    vch = victim;

	count++;
    }

    for ( victim = ch->in_room->people ; victim != NULL ; victim = victim->next_in_room )
    {
	if ( ( !IS_NPC( ch ) && IS_NPC( victim ) )
	  || ( IS_NPC( ch ) && !IS_NPC( victim ) ) )
	    continue;

   	if ( !IS_SET( trap->flags, TRAP_AREA|TRAP_RANDOM_AREA ) )
	{
	    if ( IS_SET( trap->flags, TRAP_RANDOM_ONE ) )
	    {
		if ( victim != vch )
		    continue;
	    }
	    else
		if ( victim != ch )
		    continue;
	}
	else if ( IS_SET( trap->flags, TRAP_RANDOM_AREA ) && number_percent() > 50 )
	    continue;

        for ( spell = trap->spells ; spell != NULL ; spell = spell->next )
        {   
	    sn = spell->spell;
	    level = spell->level;

            if ( sn >= MAX_SKILL || sn <= 0 || skill_table[sn].spell_fun == 0 )
            {
	        bug( "trap_cast_spell: bad sn %d.", sn );
	        return;
            }

	    if ( IS_SET( skill_table[sn].target, TAR_AREA ) && fArea )
		continue;

            if ( IS_SET( skill_table[sn].target, TAR_OBJ_INV ) )
            {
	        bug( "trap_cast_spell: bad target TAR_OBJ_INV.", sn );
	        return;
            }

            target_name = "";
            (*skill_table[sn].spell_fun) ( sn, level, ch, ( void * ) victim );

	}

	fArea = FALSE;
    }

    return;
}



/*
 * Spell functions.
 */
void spell_water_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    if (victim == NULL)
 	return;

    dam = roll_dice( level / 2, 10 );
    if ( saves_spell( level, victim ) )
	dam /= 2;

    act("$n `wwater blast peels your skin.",ch, NULL, victim, TO_VICT);
    act("your water blast peels $N's `wskin.",ch, NULL, victim, TO_CHAR);
    damage( ch, victim, dam, sn,DAM_WATER );
    return;
}

void spell_shatter( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam,num;
    OBJ_DATA *obj;

    if ( saves_spell( level, victim ) )
	return;

    if ( IS_SET(victim->imm_flags, IMM_FIRE) )
	return;

    if ( IS_SET(victim->res_flags, RES_FIRE) && number_range(0, 2) > 0 )
	return;

    for ( num = 0 ; get_eq_char( victim, num ) == NULL && num < MAX_WEAR; num++); 

    if ( get_eq_char( victim, num ) == NULL )
	return;

    while ( ( obj = get_eq_char( victim , roll_dice( 1 , MAX_WEAR ) -1 ) ) == NULL ); 

    if ( IS_SET( obj->extra_flags, ITEM_MAGIC ) && number_range( 0, 10 ) > 0 )
	return;

    act("$P `wexplodes! and you are hurt.",victim, NULL, obj, TO_CHAR);
    act("$p `wexplodes! and $N `wis hurt.",ch, obj, victim, TO_CHAR);
    dam = roll_dice( obj->points * 5, 2 );
    extract_obj(obj);
    damage( ch, victim, dam, sn, 6 );
    return;
}

void spell_spirit_talk( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_STRING_LENGTH];
    char name[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    if ( target_name )
    target_name = one_argument( target_name, name );
    else
    {
	send_to_char( "Invalid target.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, name ) ) == NULL || IS_NPC( victim ) || victim->in_room == NULL )
    {
	send_to_char( "Invalid target.\n\r", ch );
	return;
    }

    sprintf( buf, "You tell $N `G'%s'", target_name );
    act( buf, ch, NULL, victim, TO_CHAR );
    sprintf( buf, "$N's `wvoice echos in your soul '`G%s`w'", target_name );
    act( buf, victim, NULL, ch, TO_CHAR );
}


void spell_roaring_winds( int sn, int level, CHAR_DATA *ch, void *vo)
{
	ROOM_INDEX_DATA * room = ch->in_room;
	AFFECT_DATA af;
	
	af.type      =sn;
	af.level     =level;
	af.duration  =number_range( level/8, UMAX(4, level/3) );
	af.location  =APPLY_ROOM_FLAG;
	af.modifier  =0;
	af.bit_type  =0;
	af.bitvector =ROOM_SILENCE;
	affect_apply_room (room, &af);

	act( "A load roaring wind begins to blow through the room.", ch, NULL, NULL, TO_ROOM);
	act( "A load roaring wind begins to blow through the room.", ch, NULL, NULL, TO_CHAR);
	
	return;
}

void spell_whispering_wind( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_STRING_LENGTH];
    char name[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    char *said;
    char *argument;

    if ( target_name )
    target_name = one_argument( target_name, name );
    else
	return;

    if ( ( victim = get_char_world( ch, name ) ) == NULL || IS_NPC( victim ) || victim->in_room == NULL )
    {
	send_to_char( "Invalid target.\n\r", ch );
	return;
    }

    sprintf( buf, "You whipser to $N '%s'", target_name );
    act( buf, ch, NULL, victim, TO_CHAR );

    said=target_name;
    for ( vch=victim->in_room->people ; vch != NULL ; vch=vch->next_in_room)
    {
	argument = speak_lang( ch, vch, said, gsn_common );
	sprintf( buf, "`C$N`C's voice carries on the winds '$t'`w" );
	act( buf,vch, argument, ch ,TO_CHAR );
    }
}

void spell_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected.\n\r",ch);
	else
	  act("$N `wis already armored.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = URANGE( 10, level / 2, 30 );
    af.modifier  = -20;
    af.location  = APPLY_AC;
    af.bitvector = 0;
    af.bit_type = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel someone protecting you.\n\r", victim );
    if ( ch != victim )
	act("$N `wis protected by your magic.",ch,NULL,victim,TO_CHAR);
    return;
}


void spell_tongues( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (victim == NULL)
    victim = ch;
    if ( IS_AFFECTED( victim,AFF_TONGUES ) )
    {
	if (victim == ch)
	  send_to_char("You already understand everything spoken.\n\r",ch);
	else
	  act("$n `walready understands what people say.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = level*2;
    af.modifier  = 0;
    af.location  = APPLY_AFFECT;
    af.location2 = 0;
    af.bitvector = AFF_TONGUES;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "You start to understand what people are saying\n\r", victim );
    if ( ch != victim )
       send_to_char("OK.",ch);
    return;
}

void spell_dream_dust( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You already affected by dream dust.\n\r",ch);
	else
	  act("$N `walready is affected by dream dust.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = 6 + level;
    af.location  = APPLY_DREAMS;
    af.modifier  = 2 * level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_NIGHTMARES;
    af.modifier  = -4 * level / 3;
    affect_to_char( victim, &af );
    send_to_char( "You feel at ease.\n\r", victim );
    return;
}

void spell_nightmare_cloud( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You already affected by nightmare cloud.\n\r",ch);
	else
	  act("$N `walready is affected by nightmare cloud.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = 6 + level;
    af.location  = APPLY_DREAMS;
    af.modifier  = 2 * level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_NIGHTMARES;
    af.modifier  = 4 * level / 3;
    affect_to_char( victim, &af );
    send_to_char( "You feel uneasy.\n\r", victim );
    return;
}

void spell_restless_sleep( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You already affected by restless sleep.\n\r",ch);
	else
	  act("$N `walready is affected by restless sleep.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = 6 + level;
    af.location  = APPLY_DREAMS;
    af.modifier  = -4 * level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    send_to_char( "You feel restless.\n\r", victim );
    return;
}

void spell_courage( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim->position == POS_FIGHTING || is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already courageous.\n\r",ch);
	else
	  act("$N `walready has divine favor.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = 6 + level;
    af.location  = APPLY_HITROLL;
    af.modifier  = level/6;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = 0 - level/6;
    affect_to_char( victim, &af );
    send_to_char( "You feel courageous.\n\r", victim );
    if ( ch != victim )
	act("You grant $N `wthe favor of an ancient god.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_blinding_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET(victim->imm_flags, IMM_LIGHT) )
	return;

    if ( IS_SET(victim->res_flags, RES_LIGHT) )
    {
	if ( number_range(1, 5) == 3 )
	    return;
    }

    if ( IS_AFFECTED(victim, AFF_BLIND) || saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.level     = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -10 * ch->level / 8;
    af.duration  = 1+level;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location = APPLY_AFFECT;
    af.bitvector = AFF_BLIND;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "You are blinded!\n\r", victim );
    act("$n `wappears to be blinded.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_sand_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 9, 11, 12, 13,	14,	17, 20, 23, 26, 29,
	29, 29, 30, 30,	31,	31, 32, 32, 33, 33,
	34, 34, 35, 35,	36,	36, 37, 37, 38, 38,
	39, 39, 40, 40,	41,	41, 42, 42, 43, 43,
	44, 44, 45, 45,	46,	46, 47, 47, 48, 48,
	49, 49, 50, 50, 51,	51, 53, 53, 55, 60
    };
    int dam;

    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_EARTH );
    return;
}



void spell_chain_lightning( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;


    dam = roll_dice(level/2, 8);

    send_to_char( "Lightning strikes your foes!\n\r", ch );
    act( "$n `wcalls lightning to strike $s `wfoes!",
	ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;
	if ( vch->in_room == NULL )
	    continue;
	if ( vch->in_room == ch->in_room )
	{
	    if ( vch != ch && ( IS_NPC(ch) ? !IS_NPC(vch) : IS_NPC(vch) ) )
		damage( ch, vch, saves_spell( level,vch ) ? dam / 2 : dam, sn,
			DAM_WATER );
	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area
	&&   IS_OUTSIDE(vch)
	&&   !IS_UNDERWATER( vch )
	&&   IS_AWAKE(vch) )
	    send_to_char( "Lightning flashes in the sky.\n\r", vch );
    }

    return;
}

/* RT calm spell stops all fighting in the room */

void spell_calm_spirit( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    int mlevel = 0;
    int count = 0;
    int high_level = 0;    
    int chance;
    AFFECT_DATA af;

    /* get sum of all mobile levels in the room */
    for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
    {
	if (vch->position == POS_FIGHTING)
	{
	    count++;
	    if (IS_NPC(vch))
	      mlevel += vch->level;
	    else
	      mlevel += vch->level/2;
	    high_level = UMAX(high_level,vch->level);
	}
    }

    /* compute chance of stopping combat */
    chance = 4 * level - high_level + 2 * count;

    if (IS_IMMORTAL(ch)) /* always works */
      mlevel = 0;

    if (number_range(0, chance) >= mlevel)  /* hard to stop large fights */
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
   	{
	    if (!IS_NPC(vch))
		continue;

	    if (IS_NPC(vch) && (IS_SET(vch->imm_flags,IMM_MAGIC) ) )
	      	continue;

	    if (IS_AFFECTED(vch,AFF_CALM_SPIRIT) || IS_AFFECTED(vch,AFF_BERSERK)
	    ||  is_affected(vch,skill_lookup("frenzy")))
	      	continue;
	    
	    send_to_char("A wave of calm passes over you.\n\r",vch);

	    if (vch->fighting || vch->position == POS_FIGHTING)
	      stop_fighting(vch,FALSE);

	    af.type = sn;
  	    af.level = level;
	    af.duration = level/3;
	    af.location = APPLY_HITROLL;
	    af.bitvector = 0;
	    af.modifier = -5;
	    affect_to_char(vch,&af);

	    af.location = APPLY_AFFECT;
	    af.bitvector = AFF_CALM_SPIRIT;
    	    af.bit_type = BIT_AFFECT;
	    affect_to_char(vch,&af);

	    af.location = APPLY_DAMROLL;
	    affect_to_char(vch,&af);
	}
    }
}

void spell_remove_aura( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;
 
    level += 2;

    if ((!IS_NPC(ch) && IS_NPC(victim) && 
	 !(IS_AFFECTED(ch, AFF_CONTROL_MIND) && ch->master == victim) ) ||
        (IS_NPC(ch) && !IS_NPC(victim)) )
    {
	send_to_char("You failed at removing any aura.\n\r",ch);
	return;
    }

    /* unlike dispel magic, the victim gets NO save */
 
    /* begin running through the spells */
 
    if (check_dispel(level,victim,skill_lookup("armor")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("courage")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("blinding light")))
    {
        found = TRUE;
        act("$n `wis no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }

    if (check_dispel(level,victim,skill_lookup("calm spirit")))
    {
	found = TRUE;
	act("$n `wno longer looks so peaceful...",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("change sex")))
    {
        found = TRUE;
        act("$n `wlooks more like $mself again.",victim,NULL,NULL,TO_ROOM);
	send_to_char("You feel more like yourself again.\n\r",victim);
    }

    if (check_dispel(level,victim,skill_lookup("sand storm")))
    {
        found = TRUE;
        act("$n `wis no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("control mind")))
    {
        found = TRUE;
        act("$n `wregains $s free will.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("desert fist")))
    {
        found = TRUE;
        act("$n `wlooks stronger.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("curse")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect world")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect shadowform")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect magic")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("entangle")))
    {
        act("$n's `wentanglement fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("faerie fire")))
    {
        act("$n's `woutline fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("levitate")))
    {
        act("$n `wfalls to the ground!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("fly")))
    {
        act("$n `wfalls to the ground!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("frenzy")))
    {
	act("$n `wno longer looks so wild.",victim,NULL,NULL,TO_ROOM);;
	found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("empower")))
    {
        act("$n `wno longer looks so mighty.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("haste")))
    {
	act("$n `wis no longer moving so quickly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("slow")))
    {
	act("$n `wis no longer moving so slowly.",victim,NULL,NULL,TO_ROOM);
	found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("infravision")))
        found = TRUE;
 
     if (check_dispel(level,victim,skill_lookup("shadowform")))
    {
        act("$n `wfades into existance.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("mass shadowform")))
    {
        act("$n `wfades into existance.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("dimension walk")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("circle of protection")))
    {
        act("The white aura around $n's `wbody vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("fire shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("earth shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("water shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("wind shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("spirit shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("sleep")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("stone skin")))
    {
        act("$n's `wskin regains its normal texture.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("weaken")))
    {
        act("$n `wlooks stronger.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
}

void spell_mind_blast( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    damage( ch, (CHAR_DATA *) vo, roll_dice(3, 8) + level - 6, sn, DAM_MENTAL );
    if (!saves_spell(level,victim))
	    WAIT_STATE( ch, 3 );
    return;
}

void spell_pain_touch( int sn, int level, CHAR_DATA *ch, void *vo )
{
    damage( ch, (CHAR_DATA *) vo, roll_dice(2, 8) + level / 2, sn, DAM_FIRE );
    return;
}

void spell_minor_sacrifice( int sn, int level, CHAR_DATA *ch, void *vo )
{
CHAR_DATA *victim = (CHAR_DATA *) vo;
if (!saves_spell(level,victim))
{
	if ( IS_NPC(ch) )
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 8, sn, DAM_SPIRIT );
	else
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 6, sn, DAM_SPIRIT );

	ch->hit -= ch->hit / 12;
	ch->hit = UMAX(ch->hit,1);
}	
else
{
	if ( IS_NPC(ch) )
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 14, sn, DAM_SPIRIT );
	else
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 12, sn, DAM_SPIRIT );

	ch->hit -= ch->hit / 12;
	ch->hit = UMAX(ch->hit,1);
}

return;
}

void spell_sacrifice( int sn, int level, CHAR_DATA *ch, void *vo )
{
CHAR_DATA *victim = (CHAR_DATA *) vo;
if (!saves_spell(level,victim))
	{
	if ( IS_NPC(ch) )
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 5, sn, DAM_SPIRIT );
	else
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 3, sn, DAM_SPIRIT );

	ch->hit -= ch->hit / 6;
	ch->hit = UMAX(ch->hit,1);
	}	
else
	{
	if ( IS_NPC(ch) )
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 8, sn, DAM_SPIRIT );
	else
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 6, sn, DAM_SPIRIT );
	ch->hit -= ch->hit / 6;
	ch->hit = UMAX(ch->hit,1);
	}
return;
}


void spell_major_sacrifice( int sn, int level, CHAR_DATA *ch, void *vo )
{
	CHAR_DATA *victim = (CHAR_DATA *) vo;
	if ( IS_NPC(ch) )
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 4, sn, DAM_SPIRIT );
	else
	    damage( ch, (CHAR_DATA *) vo, ch->hit / 2, sn, DAM_SPIRIT );

	ch->hit -= ch->hit / 3;
	ch->hit = UMAX(ch->hit,1);
    	act("$N `Rcries out in pain as blood oozes from their wrists!",
        ch,NULL,victim,TO_ROOM);
    	act("$N `Rcries out in pain as blood oozes from their wrists!",
	ch,NULL,victim,TO_CHAR);
    	act("`RYou cry out in pain as blood begins to ooz from your wrists!",
	ch,NULL,victim,TO_VICT);  
return;
}

void spell_maelstrom(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *tmp_vict,*last_vict,*next_vict;
    bool found;
    int dam;

    /* first strike */

    act("A raging maelstrom appears from $n's `whand and smashes into $N`w.",
        ch,NULL,victim,TO_ROOM);
    act("A raging maelstrom appears from your hand and smashes into $N`w.",
	ch,NULL,victim,TO_CHAR);
    act("A raging maelstrom appears from $n's `whand and smashes into you!",
	ch,NULL,victim,TO_VICT);  

    dam = roll_dice(level,6);
    if (saves_spell(level,victim))
 	dam /= 3;
    damage(ch,victim,dam,sn,DAM_WATER);
    last_vict = victim;
    level -= 4;   /* decrement damage */

    /* new targets */
    while (level > 0)
    {
	found = FALSE;
	if (!ch->in_room)
	    return;
	for (tmp_vict = ch->in_room->people; 
	     tmp_vict != NULL; 
	     tmp_vict = next_vict) 
	{
	  next_vict = tmp_vict->next_in_room;
	  if (!is_safe_spell(ch,tmp_vict,TRUE) && tmp_vict != last_vict)
	  {
	    found = TRUE;
	    last_vict = tmp_vict;
	    act("The maelstrom rages to $n`w!",tmp_vict,NULL,NULL,TO_ROOM);
	    act("The maelstrom smashes you!",tmp_vict,NULL,NULL,TO_CHAR);
	    dam = roll_dice(level,6);
	    if (saves_spell(level,tmp_vict))
		dam /= 3;
	    damage(ch,tmp_vict,dam,sn,DAM_WATER);
	    level -= 4;  /* decrement damage */
	  }
	}   /* end target searching loop */
	
	if (!found) /* no target found, hit the caster */
	{
	  if (ch == NULL)
     	    return;

	  if (last_vict == ch) /* no double hits */
	  {
	    act("`wThe maelstrom becomes smaller, fading away.",ch,NULL,NULL,TO_ROOM);
	    act("`wThe maelstrom smashes into you and disappears.",
		ch,NULL,NULL,TO_CHAR);
	    return;
	  }
	
	  last_vict = ch;
	  act("`wThe raging maelstrom smashes into $n`w!",ch,NULL,NULL,TO_ROOM);
	  send_to_char("`wYou are struck by your own maelstrom!\n\r",ch);
	  dam = roll_dice(level,6);
	  if (saves_spell(level,ch))
	    dam /= 3;
	  damage(ch,ch,dam,sn,DAM_WATER);
	  level -= 4;  /* decrement damage */
	  if (ch == NULL) 
	    return;
	}
    /* now go back and find more targets */
    }
}
	  

void spell_control_mind( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( victim == ch )
    {
	send_to_char( "You like yourself even better!\n\r", ch );
	return;
    }

    if ( !IS_NPC( victim ) )
    {
	send_to_char( "Not on other players!\n\r", ch );
	return;
    }

    if ( IS_AFFECTED(victim, AFF_CONTROL_MIND)
    ||   IS_AFFECTED(ch, AFF_CONTROL_MIND)
    ||   level < victim->level
    ||   IS_SET(victim->imm_flags,IMM_CONTROL_MIND)
    ||   saves_spell( level, victim ) )
	return;

    if ( victim->master )
	stop_follower( victim );
    add_follower( victim, ch );
    victim->leader = ch;
    af.type      = sn;
    af.level	 = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_CONTROL_MIND;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    act( "Isn't $n `wjust so nice?", ch, NULL, victim, TO_VICT );
    if ( ch != victim )
	act("$N `wlooks at you with adoring eyes.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_desert_fist( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 4,  5,  6,  7,  8,	 9, 12, 13, 13, 13,
	14, 14, 14, 15, 15,	15, 16, 16, 16, 17,
	17, 17, 18, 18, 18,	19, 19, 19, 20, 20,
	20, 21, 21, 21, 22,	22, 22, 23, 23, 23,
	24, 24, 24, 25, 25,	25, 26, 26, 26, 27,
	27, 28, 28, 29, 29,	30, 31, 32, 33, 34 
   };
    AFFECT_DATA af;
    int dam;

    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( !saves_spell( level, victim ) )
    {
	act("$n `wis thrown back by a blast of sand.",victim,NULL,NULL,TO_ROOM);
	af.type      = sn;
        af.level     = level;
	af.duration  = 6;
	af.location  = APPLY_STR;
	af.modifier  = -1;
	af.bitvector = 0;
	affect_join( victim, &af );
    }
    else
    {
	dam /= 2;
    }

    damage( ch, victim, dam, sn, DAM_EARTH );
    return;
}



void spell_arrows_of_fire( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 1,  2,  3,  4,  5,	 6,  7,  8, 10, 15,
	20, 25, 30, 35, 40,	45, 45, 45, 46, 47,
	48, 48, 49, 50, 51,	51, 52, 53, 54, 54,
	55, 56, 57, 57, 58,	59, 60, 60, 61, 62,
	63, 63, 64, 65, 66,	66, 67, 68, 69, 69,
	70, 71, 72, 74, 75,	77, 79, 81, 83, 85
    };
    int dam;

    if ( IS_UNDERWATER( ch ) )
    {
	send_to_char("`BYour spell fizzles out.\n\r", ch);
	return;
    }
    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2,  dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    else 
	spell_blinding_light(skill_lookup("blinding light"),level/2,ch,(void *) victim);

    damage( ch, victim, dam, sn, DAM_FIRE );
    return;
}

void spell_create_minor_portal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int skill,value;
     
    obj = create_object( get_obj_index( OBJ_VNUM_GATE ));

    if ( obj == NULL )
    {
	bug( "Bad gate for spell_create_minor_portal.", 0 );
	return;
    }

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||	 IS_SET(victim->imm_flags, IMM_SUMMON )
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY) 
    ||   victim->in_room->area->visible > ch->level 
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL) 
    ||   IS_SET(victim->in_room->room_flags_2, ROOM_STARTING) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    obj->gate_data->flags = 0;
    obj->gate_data->leads_to = (void *) victim->in_room;
    obj->gate_data->vnum = victim->in_room->vnum;
    obj->timer = ch->level/2;

    skill = get_skill( ch, sn );
    value = number_range( skill/2, skill * 3 / 2 );
  
    if ( ch->in_room->area->world != victim->in_room->area->world )
	value -= 50;

    if ( IS_IMMORTAL( ch ) && IS_SET( ch->plr, PLR_HOLYLIGHT ) )
	value = 1000;

    if ( value < 20 )
    {
 	SET_BIT( obj->gate_data->flags, GATE_UNSTABLE );
	obj->gate_data->ave_timer = 5;
    }
    else if ( value < 50 )
    {
	obj->gate_data->ave_timer = ch->level / 2;	
    }
    else if ( value < 75 )
    {
	SET_BIT( obj->gate_data->flags, GATE_TO_STAY_ROOM );
	obj->timer = ch->level * 3 / 2;
	obj->gate_data->ave_timer = ch->level * 3 / 2;
    }
    else if ( value <= 95 )
    {
	SET_BIT( obj->gate_data->flags, GATE_TO_STAY_ROOM );
	obj->timer = ch->level * 2;
	obj->gate_data->ave_timer = ch->level *2;
	if ( IS_NPC( victim ) )
	{
	    SET_BIT( obj->gate_data->flags, GATE_TO_MOB );
  	    obj->gate_data->leads_to = (void *) victim; 
	    obj->gate_data->vnum = victim->pIndexData->vnum;
	}
    }
    else if ( value > 95)
    {
	obj->timer = -1;
	obj->gate_data->ave_timer = -1;
	if ( IS_NPC( victim ) )
	{
	    SET_BIT( obj->gate_data->flags, GATE_TO_MOB );
  	    obj->gate_data->leads_to = (void *) victim; 
	    obj->gate_data->vnum = victim->pIndexData->vnum;
	}
    }

    obj_to_room( obj, ch->in_room );
    act( "$p `wslowly appears in the room.", ch, obj, NULL, TO_CHAR );
    act( "$p `wslowly appears in the room.", ch, obj, NULL, TO_ROOM );

    return;
}

void spell_create_major_portal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj;
    OBJ_DATA *obj1;
    CHAR_DATA *victim;
    int skill,value;
     
    obj = create_object( get_obj_index( OBJ_VNUM_GATE ));

    if ( obj == NULL )
    {
	bug( "Bad gate for spell_create_minor_portal.", 0 );
	return;
    }

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||	 IS_SET(victim->imm_flags, IMM_SUMMON )
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags_2, ROOM_STARTING)
    ||   victim->in_room->area->visible > ch->level 
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( obj->gate_data == NULL )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    obj->gate_data->flags = 0;
    obj->gate_data->leads_to = (void *) victim->in_room;
    obj->gate_data->vnum = victim->in_room->vnum;
    obj->timer = ch->level/2;

    skill = get_skill( ch, sn );
    value = number_range( skill/2, skill * 3 / 2 );
  
    if ( ch->in_room->area->world != victim->in_room->area->world )
	value -= 25;

    if ( IS_IMMORTAL( ch ) )
	value = 1000;

    if ( value < 20 )
    {
 	SET_BIT( obj->gate_data->flags, GATE_UNSTABLE );
	obj->gate_data->ave_timer = -1;
    }
    else if ( value < 50 )
    {
	SET_BIT( obj->gate_data->flags, GATE_TO_STAY_ROOM );
	obj->timer = ch->level * 3 / 2;
	obj->gate_data->ave_timer = ch->level * 3 / 2;
    }
    else if ( value < 75 )
    {
	SET_BIT( obj->gate_data->flags, GATE_TO_STAY_ROOM );
	obj->timer = ch->level * 2;
	obj->gate_data->ave_timer = ch->level *2;
	if ( IS_NPC( victim ) )
	{
	    SET_BIT( obj->gate_data->flags, GATE_TO_MOB );
  	    obj->gate_data->leads_to = (void *) victim; 
	    obj->gate_data->vnum = victim->pIndexData->vnum;
	}
    }
    else if ( value <= 95 )
    {
	obj->timer = -1;
	obj->gate_data->ave_timer = -1;
	if ( IS_NPC( victim ) )
	{
	    SET_BIT( obj->gate_data->flags, GATE_TO_MOB );
  	    obj->gate_data->leads_to = (void *) victim; 
	    obj->gate_data->vnum = victim->pIndexData->vnum;
	}
    }
    else if ( value > 95 )
    {
	obj1 = create_object( get_obj_index( OBJ_VNUM_GATE ));

	obj1->gate_data->flags = 0;
	obj1->timer = -1;
	obj1->gate_data->ave_timer = -1;
        SET_BIT( obj1->gate_data->flags, GATE_TO_OBJ );
        obj1->gate_data->leads_to = (void *) obj; 
        obj1->gate_data->vnum = OBJ_VNUM_GATE;

	obj->timer = -1;
	obj->gate_data->ave_timer = -1;
        SET_BIT( obj->gate_data->flags, GATE_TO_OBJ );
        obj->gate_data->leads_to = (void *) obj1; 
        obj->gate_data->vnum = OBJ_VNUM_GATE;

        obj_to_room( obj1, victim->in_room );
        act( "$p `wslowly appears in the room.", victim, obj1, NULL, TO_CHAR );
        act( "$p `wslowly appears in the room.", victim, obj1, NULL, TO_ROOM );
    }

    obj_to_room( obj, ch->in_room );
    act( "$p `wslowly appears in the room.", ch, obj, NULL, TO_CHAR );
    act( "$p `wslowly appears in the room.", ch, obj, NULL, TO_ROOM );

    return;
}

void spell_continual_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
     OBJ_DATA *light ;
     
    light = create_object( get_obj_index( OBJ_VNUM_LIGHT_BALL ));
  SET_BIT(light->extra_flags, ITEM_LIT) ;
    obj_to_room( light, ch->in_room );
    act( "$n `wtwiddles $s thumbs and $p `wappears.",   ch, light, NULL, TO_ROOM );
    act( "You twiddle your thumbs and $p `wappears.", ch, light, NULL, TO_CHAR );
    return;
}

void  spell_illuminate(int sn,int level, CHAR_DATA *ch, void *vo)
{
    OBJ_DATA *obj= (OBJ_DATA *)vo ;
    
    if (obj->light_data != NULL && IS_SET( obj->light_data->flags, LIGHT_LIT|LIGHT_MAGIC_LIT ) ) 
    {
	send_to_char("Nothing seems to happen.",ch) ;
	return ;
    }

    if ( obj->light_data == NULL )
	obj->light_data = light_alloc();

    obj->light_data->fuel = 1;
    obj->light_data->max_fuel = -1;
    obj->light_data->brightness = UMIN( 75, 2 * level );
    SET_BIT( obj->light_data->flags, LIGHT_MAGIC_LIT );
    act("$p `wshines brightly!",ch,obj,NULL,TO_CHAR) ;
    return ;
}    


void spell_create_ration( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *mushroom;

    mushroom = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ));
 /*   if ( mushroom->edible_data != NULL )
    {
	mushroom->edible_data->amount = UMIN( 5 + 3 * level, 100 );
	mushroom->edible_data->max_amount = UMIN( 5 + 3 * level, 100 );
    } who put this here????? */
    obj_to_room( mushroom, ch->in_room );
    act( "$p `wsuddenly appears.", ch, mushroom, NULL, TO_ROOM );
    act( "$p `wsuddenly appears.", ch, mushroom, NULL, TO_CHAR );
    return;
}



void spell_find_spring( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *spring;

    spring = create_object( get_obj_index( OBJ_VNUM_SPRING ));
    spring->timer = level;
    obj_to_room( spring, ch->in_room );
    act( "$p `wflows from the ground.", ch, spring, NULL, TO_ROOM );
    act( "$p `wflows from the ground.", ch, spring, NULL, TO_CHAR );
    return;
}



void spell_create_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    int water;

    if ( obj->edible_data == NULL || IS_SET( obj->edible_data->flags, FOOD_EDIBLE )
      || obj->edible_data->max_amount < 1 )
    {
	send_to_char( "It is unable to hold water.\n\r", ch );
	return;
    }

    if ( obj->edible_data->type != LIQ_WATER && obj->edible_data->amount != 0 )
    {
	send_to_char( "It contains some other liquid.\n\r", ch );
	return;
    }

    water = UMIN(
		level * (weather_info[ ch->in_room->area->world ].sky >= SKY_RAINING ? 4 : 2),
		obj->edible_data->max_amount - obj->edible_data->amount
		);
  
    if ( water > 0 )
    {
	obj->edible_data->type = LIQ_WATER;
	obj->edible_data->amount += water;
	if ( !is_name( "water", obj->name ) )
	{
	    char buf[MAX_STRING_LENGTH];

	    sprintf( buf, "%s water", obj->name );
	    strncpy( obj->name, buf, 30 );
	}
	act( "$p `wis filled.", ch, obj, NULL, TO_CHAR );
    }

    return;
}



void spell_remove_blindness( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !IS_AFFECTED( victim, AFF_BLIND) )
    {
        if (victim == ch)
          send_to_char("You aren't blind.\n\r",ch);
        else
          act("$N `wdoesn't appear to be blinded.",ch,NULL,victim,TO_CHAR);
        return;
    }
 
    if ((check_dispel(level,victim,gsn_blinding_light))
	|| (check_dispel(level,victim,gsn_sand_storm)))
    {
        send_to_char( "Your vision returns!\n\r", victim );
        act("$n `wis no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
    else
        send_to_char("Spell failed.\n\r",ch);
}



void spell_cure_critical( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = roll_dice(3, 8) + level - 6;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

/* RT added to cure plague */
void spell_cure_disease( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;

    if ( !is_affected( victim, gsn_plague ) )
    {
        if (victim == ch)
          send_to_char("You aren't ill.\n\r",ch);
        else
          act("$N `wdoesn't appear to be diseased.",ch,NULL,victim,TO_CHAR);
        return;
    }
    
    if (check_dispel(level,victim,gsn_plague))
    {
	send_to_char("Your sores vanish.\n\r",victim);
	act("$n `wlooks relieves as $s sores vanish.",victim,NULL,NULL,TO_ROOM);
    }
    else
	send_to_char("Spell failed.\n\r",ch);
}



void spell_cure_light( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = roll_dice(1, 8) + level / 3;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_remove_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
 
    if ( !is_affected( victim, gsn_poison ) )
    {
        if (victim == ch)
          send_to_char("You aren't poisoned.\n\r",ch);
        else
          act("$N `wdoesn't appear to be poisoned.",ch,NULL,victim,TO_CHAR);
        return;
    }
 
    if (check_dispel(level,victim,gsn_poison))
    {
        send_to_char("A warm feeling runs through your body.\n\r",victim);
        act("$n `wlooks much better.",victim,NULL,NULL,TO_ROOM);
    }
    else
        send_to_char("Spell failed.\n\r",ch);
}

void spell_cure_serious( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int heal;

    heal = roll_dice(2, 8) + level /2 ;
    victim->hit = UMIN( victim->hit + heal, victim->max_hit );
    update_pos( victim );
    send_to_char( "You feel better!\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_CURSE) || saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -1 * (level / 15);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_CURSE;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    af.bitvector = 0;
    af.bit_type = 0;
    af.location  = APPLY_SAVING_SPELL;
    af.modifier  = level / 15;
    affect_to_char( victim, &af );

    send_to_char( "You feel unclean.\n\r", victim );
    if ( ch != victim )
	act("$N `wlooks very uncomfortable.",ch,NULL,victim,TO_CHAR);
    return;
}

void spell_detect_world( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_WORLD) )
    {
	if (victim == ch)
	  send_to_char("You can already sense creatures from other worlds.\n\r",ch);
	else
	  act("$N `wcan already detect creatures from other worlds.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_DETECT_WORLD;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_hidden( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_HIDDEN) )
    {
        if (victim == ch)
          send_to_char("You are already as alert as you can be. \n\r",ch);
        else
          act("$N `wcan already sense hidden lifeforms.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_DETECT_HIDDEN;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "Your awareness improves.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_senses( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SENSES) )
    {
        if (victim == ch)
          send_to_char("You already have heightened senses.\n\r",ch);
        else
          act("$N `walready has heightened senses.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_SENSES;
    af.bit_type = BIT_AFFECT2;
    affect_to_char( victim, &af );
    send_to_char( "You feel more aware of your surroundings.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_detect_shadowform( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_SHADOWFORM) )
    {
        if (victim == ch)
          send_to_char("You can already see shadowform.\n\r",ch);
        else
          act("$N `wcan already see things of shadowform.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_DETECT_SHADOWFORM;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}




void spell_detect_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DETECT_MAGIC) )
    {
        if (victim == ch)
          send_to_char("You can already sense magical auras.\n\r",ch);
        else
          act("$N `wcan already detect magic.",ch,NULL,victim,TO_CHAR);
        return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.modifier  = 0;
    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_DETECT_MAGIC;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes tingle.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_detect_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;

    if ( obj->edible_data != NULL )
    {
	if ( IS_SET( obj->edible_data->flags, FOOD_POISONED ) )
	    send_to_char( "You smell poisonous fumes.\n\r", ch );
	else
	    send_to_char( "It looks delicious.\n\r", ch );
    }
    else
    {
	send_to_char( "It doesn't look poisoned.\n\r", ch );
    }

    return;
}

void spell_extraction( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;
    char target[MAX_INPUT_LENGTH];

    if ( IS_NPC( ch ) )
	return;

    if ( !( target_name ) || strlen( target_name ) < 2 )
    {
	send_to_char( "You didn't specify a target!\n\r", ch );	
	return;
    }

    target_name = one_argument( target_name, target );
    if ( strlen( target_name ) < 1 )
    {
	send_to_char( "What spell do you want to remove?\n\r", ch );	
	return;
    }

    if ( ( victim = get_char_room( ch, target ) ) == NULL )
    {
	send_to_char( "They arn't here!.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) )
    {
	send_to_char( "You can only cast this on yourself or other players.\n\r", ch );
	return;
    }

    if ( saves_spell( level, victim ) )
    {
	send_to_char( "You feel a brief tingling sensation.\n\r",victim );
	send_to_char( "You failed.\n\r", ch);
	return;
    }

    /* find the spell */ 
    if ( check_dispel( level, victim, skill_lookup( target_name ) ) )
        found = TRUE;

    victim->affected_by = victim->affected_by | victim->pcdata->perm_aff;
    victim->affected_by_2 = victim->affected_by_2 | victim->pcdata->perm_aff_2;

    if ( found )
        send_to_char( "Ok.\n\r", ch );
    else
        send_to_char( "Spell failed.\n\r", ch );
}

/* modified for enhanced use */

void spell_dispel_magic( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;

    if (saves_spell(level, victim))
    {
	send_to_char( "You feel a brief tingling sensation.\n\r",victim);
	send_to_char( "You failed.\n\r", ch);
	return;
    }

    /* begin running through the spells */ 

    if (check_dispel(level,victim,skill_lookup("armor")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("courage")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("blinding light")))
    {
        found = TRUE;
        act("$n `wis no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
    if (check_dispel(level,victim,skill_lookup("sand storm")))
    {
        found = TRUE;
        act("$n `wis no longer blinded.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("calm spirit")))
    {
        found = TRUE;
        act("$n `wno longer looks so peaceful...",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("control mind")))
    {
        found = TRUE;
        act("$n `wregains $s free will.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("desert fist")))
    {
        found = TRUE;
        act("$n `wlooks warmer.",victim,NULL,NULL,TO_ROOM);
    }
 
    if (check_dispel(level,victim,skill_lookup("curse")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect world")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect hidden")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect shadowform")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("detect magic")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("entangle")))
    {
        act("$n's `wentanglement fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("faerie fire")))
    {
        act("$n's `woutline fades.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("levitate")))
    {
        act("$n `wfalls to the ground!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
    if (check_dispel(level,victim,skill_lookup("fly")))
    {
        act("$n `wfalls to the ground!",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("frenzy")))
    {
        act("$n `wno longer looks so wild.",victim,NULL,NULL,TO_ROOM);;
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("empower")))
    {
        act("$n `wno longer looks so mighty.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("haste")))
    {
        act("$n `wis no longer moving so quickly.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("slow")))
    {
        act("$n `wis no longer moving so slowly.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("infravision")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("shadowform")))
    {
        act("$n `wfades into existance.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("mass shadowform")))
    {
        act("$n `wfades into existance.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("dimension walk")))
        found = TRUE;
 
 
    if (check_dispel(level,victim,skill_lookup("circle of protection")))
    {
        act("The white aura around $n's `wbody vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (IS_AFFECTED(victim,AFF_SANCTUARY) 
	&& !saves_dispel(level, victim->level,-1)
	&& !is_affected(victim,skill_lookup("circle of protection")))
    {
	REMOVE_BIT(victim->affected_by,AFF_SANCTUARY);
        act("The white aura around $n's `wbody vanishes.",
            victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("fire shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("earth shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("water shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("wind shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (check_dispel(level,victim,skill_lookup("spirit shield")))
    {
        act("The shield protecting $n `wvanishes.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("sleep")))
        found = TRUE;
 
    if (check_dispel(level,victim,skill_lookup("stone skin")))
    {
        act("$n's `wskin regains its normal texture.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }
 
    if (check_dispel(level,victim,skill_lookup("weaken")))
    {
        act("$n `wlooks stronger.",victim,NULL,NULL,TO_ROOM);
        found = TRUE;
    }

    if (IS_NPC(victim) )
    {
    	victim->affected_by=victim->affected_by|race_table[victim->race][victim->sex].aff;
    	victim->affected_by_2=victim->affected_by_2|race_table[victim->race][victim->sex].aff2;
    }
    else 
    {
    	victim->affected_by = victim->affected_by|victim->pcdata->perm_aff;
    	victim->affected_by_2 = victim->affected_by_2|victim->pcdata->perm_aff_2;
    }


    if (found)
        send_to_char("Ok.\n\r",ch);
    else
        send_to_char("Spell failed.\n\r",ch);
}

void spell_boulder_toss( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *caster;
    int boulders, x;

    caster = ch;
    boulders = number_range( 1, ch->level / 10 );
    send_to_char( "Dozens of heavy boulders crash into the ground around you!\n\r", ch );
    act( "Dozens of heavy boulders crash into the ground around you!", NULL, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next;

	if ( vch->in_room == NULL )
	    continue;

	if ( vch->in_room == ch->in_room && vch != caster && !is_same_group( ch, vch ) && !IS_IMMORTAL( vch ) )
	{
	    for ( x = 0; x < boulders; x++ )
	    {
		damage( ch, vch, ( level * 3 / 2 ) + roll_dice( 2, level / 4 ), sn, DAM_EARTH );
	    }

	    continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "The ground vibrates and rumbles for a moment.\n\r", vch );
    }

    return;
}

void spell_pyrotechnics( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    CHAR_DATA *caster;
    caster = ch;

    if ( IS_UNDERWATER( ch ) )
    {
	send_to_char("`BYour spell fizzles out.\n\r", ch);
	return;
    }

    send_to_char( "The area flashes with a bright light, then fills with smoke!\n\r", ch );
    act( "$n `wmakes the area flash brightly, filling with smoke.", ch, NULL, NULL, TO_ROOM );

    for ( vch = char_list; vch != NULL; vch = vch_next )
    {
	vch_next	= vch->next;

	if ( vch->in_room == NULL )
	    continue;

	if ( vch->in_room == ch->in_room && vch != caster )
	{
		if ( !is_safe_spell( ch, vch, TRUE ) )
		    damage( ch, vch, level + roll_dice(2, 8), sn, DAM_FIRE );

	    	continue;
	}

	if ( vch->in_room->area == ch->in_room->area )
	    send_to_char( "A thick smoke slowly drifts by.\n\r", vch );
    }

    return;
}

void spell_enchant_armor( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf; 
    int result, fail;
    int ac_bonus, added;
    bool ac_found = FALSE;

    if (obj->wear_data == NULL || obj->wear_data->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
	return;
    }
   
    if (obj->wear_data->ac <= 0)
    {
	send_to_char( "You can only cast this on armor.\n\r", ch );
	return;
    }

    /* this means they have no bonus */
    ac_bonus = 0;
    fail = 25;	/* base 25% chance of failure */

    /* find the bonuses */

    for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_AC )
  	{
	    ac_bonus = paf->modifier;
	    ac_found = TRUE;
	    fail += 5 * (ac_bonus * ac_bonus);
	}

	else /* things get a little harder */
	    fail += 20;
    }

    /* apply other modifiers */
    fail -= level;

    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,95);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
	act("$p `wflares blindingly... and evaporates!",ch,obj,NULL,TO_CHAR);
	act("$p `wflares blindingly... and evaporates!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p `wglows brightly, then fades...oops.",ch,obj,NULL,TO_CHAR);
	act("$p `wglows brightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->wear_data->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next; 
	    paf->next = affect_free;
	    affect_free = paf;
	}
	obj->wear_data->affected = NULL;

	/* clear all flags */
	obj->extra_flags = 0;
	return;
    }

    if ( result <= fail )  /* failed, no bad result */
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    if (result <= (100 - level/5))  /* success! */
    {
	act("$p `wshimmers with a gold aura.",ch,obj,NULL,TO_CHAR);
	act("$p `wshimmers with a gold aura.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = -1;
    }
    
    else  /* exceptional enchant */
    {
	act("$p `wglows a brillant gold!",ch,obj,NULL,TO_CHAR);
	act("$p `wglows a brillant gold!",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags,ITEM_MAGIC);
	SET_BIT(obj->extra_flags,ITEM_GLOW);
	added = -2;
    }
		
    /* now add the enchantments */ 

    if (ac_found)
    {
	for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next)
	{
	    if ( paf->location == APPLY_AC)
	    {
		paf->type = sn;
		paf->modifier += added;
		paf->level = UMAX(paf->level,level);
	    }
	}
    }
    else /* add a new affect */
    {
	    paf = new_affect(FALSE);

	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_AC;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->wear_data->affected;
    	obj->wear_data->affected	= paf;
    }

}

void spell_enchant_weapon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    AFFECT_DATA *paf; 
    int result, fail;
    int added, added2;


    if (obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
    {
	send_to_char("That isn't a weapon.\n\r",ch);
	return;
    }

    if ( obj->wear_data->wear_loc != -1)
    {
	send_to_char("The item must be carried to be enchanted.\n\r",ch);
	return;
    }

    if ( IS_SET(obj->extra_flags,ITEM_MAGIC) )
    {
	send_to_char("This item cannot be enchanted.\n\r",ch);
	return;
    }

    /* this means they have no bonus */
    fail = 25;	/* base 25% chance of failure */

    for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location == APPLY_HITROLL )
  	{
	    if ( paf->modifier > 0 )
	    {
		send_to_char("This item is already enchanted.\n\r",ch);
		return;
	    }
	}

	else if (paf->location == APPLY_DAMROLL )
  	{
	    if ( paf->modifier > 0 )
	    {
		send_to_char("This item is already enchanted.\n\r",ch);
		return;
	    }
	}

	else /* things get a little harder */
	    fail += 25;
    }

    /* apply other modifiers */
    fail -= 2 * level/3;

    if (IS_OBJ_STAT(obj,ITEM_GLOW))
	fail -= 5;

    fail = URANGE(5,fail,95);

    result = number_percent();

    /* the moment of truth */
    if (result < (fail / 5))  /* item destroyed */
    {
	act("$p `wshivers violently and explodes!",ch,obj,NULL,TO_CHAR);
	act("$p `wshivers violently and explodeds!",ch,obj,NULL,TO_ROOM);
	extract_obj(obj);
	return;
    }

    if (result < (fail / 2)) /* item disenchanted */
    {
	AFFECT_DATA *paf_next;

	act("$p `wglows brightly, then fades...oops.",ch,obj,NULL,TO_CHAR);
	act("$p `wglows brightly, then fades.",ch,obj,NULL,TO_ROOM);
	obj->enchanted = TRUE;

	/* remove all affects */
	for (paf = obj->wear_data->affected; paf != NULL; paf = paf_next)
	{
	    paf_next = paf->next; 
	    paf->next = affect_free;
	    affect_free = paf;
	}
	obj->wear_data->affected = NULL;

	/* clear all flags */
	obj->extra_flags = 0;
	return;
    }

    if ( result <= fail )  /* failed, no bad result */
    {
	send_to_char("Nothing seemed to happen.\n\r",ch);
	return;
    }

    if (result <= (100 - level/5))  /* success! */
    {
	act("$p `wglows blue.",ch,obj,NULL,TO_CHAR);
	act("$p `wglows blue.",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags, ITEM_MAGIC);
	added = number_range( 1, UMAX(1, ch->level/10) );
	added2 = number_range( 1, UMAX(1, ch->level/10) );
    }
    
    else  /* exceptional enchant */
    {
	act("$p `wglows a brillant blue!",ch,obj,NULL,TO_CHAR);
	act("$p `wglows a brillant blue!",ch,obj,NULL,TO_ROOM);
	SET_BIT(obj->extra_flags,ITEM_MAGIC);
	SET_BIT(obj->extra_flags,ITEM_GLOW);
	added = number_range( 2, UMAX(2, ch->level/8) );
	added2 = number_range( 2, UMAX(2, ch->level/8) );
    }
		
    /* now add the enchantments */ 

	paf = new_affect(FALSE);
	paf->type	= sn;
	paf->level	= level;
	paf->duration	= -1;
	paf->location	= APPLY_DAMROLL;
	paf->modifier	=  added;
	paf->bitvector  = 0;
    	paf->next	= obj->wear_data->affected;
    	obj->wear_data->affected	= paf;

        paf = new_affect(FALSE); 
        paf->type       = sn;
        paf->level      = level;
        paf->duration   = -1;
        paf->location   = APPLY_HITROLL;
        paf->modifier   =  added2;
        paf->bitvector  = 0;
        paf->next       = obj->wear_data->affected;
        obj->wear_data->affected   = paf;

}



/*
 * Drain XP, MANA, HP.
 * Caster gains HP.
 */
void spell_energy_drain( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int mod_exp;
    int i;

    if ( saves_spell( level, victim ) )
    {
	send_to_char("You feel a momentary chill.\n\r",victim);  	
	return;
    }

    if ( victim->level <= 2 )
    {
	dam		 = ch->hit + 1;
    }
    else
    {
	mod_exp = ( 0 -  5 * number_range( level/2, 3 * level / 2 ) );
	gain_exp( victim, mod_exp );
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	    victim->mana[ i ] /= 3/2;
	victim->move	/= 2;
	dam		 = roll_dice(1, level);
	ch->hit		+= dam;
	ch->exp		+= mod_exp/3;
    }

    send_to_char("You feel your life slipping away!\n\r",victim);
    send_to_char("Wow....what a rush!\n\r",ch);
    damage( ch, victim, dam, sn, DAM_ENERGY );

    return;
}

void spell_sunfire ( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	  0,
	  0,   0,   0,   0,   0,	  0,   0,   0,   0,   0,
	  0,   0,   0,   0,  30,	 35,  40,  45,  50,  55,
	 60,  65,  70,  75,  80,	 82,  84,  86,  88,  90,
	 92,  94,  96,  98, 100,	102, 104, 106, 108, 110,
	112, 114, 116, 118, 120,	122, 124, 126, 128, 130,
	132, 134, 136, 138, 140,	145, 150, 155, 160, 170
    };
    int dam;

    if ( IS_UNDERWATER( ch ) )
    {
	send_to_char("`BYour spell fizzles out.\n\r", ch);
	return;
    }

    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;

    damage( ch, victim, dam, sn, DAM_FIRE );
    return;
}

void spell_sand_storm( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    int dam;

    dam = roll_dice(6, level/2);
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_EARTH );

    if ( IS_AFFECTED(victim, AFF_BLIND) || saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.level     = level;
    af.location  = APPLY_HITROLL;
    af.modifier  = -4;
    af.duration  = 1+level/2;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_BLIND;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "You are blinded by stinging sand!\n\r", victim );
    act("$n `wappears to be blinded.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_faerie_fire( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FAERIE_FIRE) )
	return;
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = 3 * (level/2);
    af.bitvector = AFF_FAERIE_FIRE;
    affect_to_char( victim, &af );

    af.location  = APPLY_AFFECT;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );

    if (IS_AFFECTED( victim, AFF_SNEAK))
       REMOVE_BIT( victim->affected_by, AFF_SNEAK) ;

    if (IS_AFFECTED( victim, AFF_HIDE))
      REMOVE_BIT( victim->affected_by, AFF_HIDE) ;

    if (IS_AFFECTED_2( victim, AFF_FADE))
      REMOVE_BIT( victim->affected_by_2, AFF_FADE) ;

    send_to_char( "You are surrounded by a pink cloud of smoke.\n\r", victim );
    act( "$n `wis surrounded by a pink cloud of smoke.", victim, NULL, NULL, TO_ROOM );
    return;
}

void spell_reveal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *ich;

    act( "$n `wconjures a cloud of purple smoke.", ch, NULL, NULL, TO_ROOM );
    send_to_char( "You conjure a cloud of purple smoke.\n\r", ch );

    for ( ich = ch->in_room->people; ich != NULL; ich = ich->next_in_room )
    {
	if ( !IS_NPC(ich) && IS_SET(ich->plr, PLR_WIZINVIS) )
	    continue;

	if ( ich == ch || saves_spell( level, ich ) )
	    continue;

	affect_strip ( ich, gsn_shadowform		);
	affect_strip ( ich, gsn_mass_shadowform		);
	affect_strip ( ich, gsn_sneak			);
	REMOVE_BIT   ( ich->affected_by, AFF_HIDE	);
	REMOVE_BIT   ( ich->affected_by_2, AFF_FADE	);
	REMOVE_BIT   ( ich->affected_by, AFF_SHADOWFORM	);
	REMOVE_BIT   ( ich->affected_by, AFF_SNEAK	);
	act( "$n `wis revealed!", ich, NULL, NULL, TO_ROOM );
	send_to_char( "You are revealed!\n\r", ich );
    }

    return;
}

void spell_fly( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_FLYING) )
    {
	if (victim == ch)
	  send_to_char("You are already flying.\n\r",ch);
	else
	  act("$N `wdoesn't need your help to fly.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = level + 3;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_FLYING;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    return;
}

void spell_levitate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_LEVITATING) )
    {
	if (victim == ch)
	  send_to_char("You are already levitating.\n\r",ch);
	else
	  act("$N `wdoesn't need your help to levitate.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = level + 5;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_LEVITATING;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "You begin to levitate.\n\r", victim );
    act( "$n `wbegins to levitate.", victim, NULL, NULL, TO_ROOM );
    return;
}

void spell_frenzy(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (is_affected(victim,sn) || IS_AFFECTED(victim,AFF_BERSERK))
    {
	if (victim == ch)
	  send_to_char("You are already in a frenzy.\n\r",ch);
	else
	  act("$N `wis already in a frenzy.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (is_affected(victim,skill_lookup("calm spirit")))
    {
	if (victim == ch)
	  send_to_char("Why don't you just relax for a while?\n\r",ch);
	else
	  act("$N `wdoesn't look like $e wants to fight anymore.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    af.type 	 = sn;
    af.level	 = level;
    af.duration	 = level / 3;
    af.modifier  = level / 6;
    af.bitvector = AFF_BERSERK;

    af.location  = APPLY_HITROLL;
    affect_to_char(victim,&af);

    af.location  = APPLY_DAMROLL;
    affect_to_char(victim,&af);

    af.modifier  = 10 * (level / 6);
    af.location  = APPLY_AC;
    affect_to_char(victim,&af);

    send_to_char("You are filled with holy wrath!\n\r",victim);
    act("$n `wgets a wild look in $s eyes!",victim,NULL,NULL,TO_ROOM);
}

/* RT ROM-style gate */
void spell_planar_travel( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;
    bool gate_pet;
    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   !can_see_room(ch,victim->in_room) 
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_SET(victim->in_room->room_flags_2, ROOM_STARTING)
    ||   victim->in_room->area->visible > ch->level 
    ||	 victim->in_room->area->world != ch->in_room->area->world
    ||   IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   victim->level >= level + 3
    ||   (!IS_NPC(victim) && IS_IMMORTAL(victim) )
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && IS_SET(victim->plr,PLR_NOSUMMON))
    ||   (IS_NPC(victim) && saves_spell( level, victim ) ) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }

    if (ch->pet != NULL && ch->in_room == ch->pet->in_room)
	gate_pet = TRUE;
    else
	gate_pet = FALSE;
    
    act("$n `wdissapears into another plane, swirling colors swarm in the area where they had once stood.",ch,NULL,NULL,TO_ROOM);
    send_to_char("You dissapear into a plane of swirling colors.\n\r",ch);
    if (ch->inside_area != NULL ) /* Don't carry the moveable area data */
	free_inside_area_data(ch->inside_area);
	ch->inside_area = NULL;
    char_from_room(ch);
    char_to_room(ch,victim->in_room);

    act("$n `whas arrived from planar travel.",ch,NULL,NULL,TO_ROOM);
    do_look(ch,"auto");

    if ( ch->mount != NULL && IS_SET( ch->mount->affected_by,AFF_MOUNTED) )
    {
        char_from_room( ch->mount );
        char_to_room( ch->mount, victim->in_room );
    }

    if (gate_pet)
    {
	act("$n `wdissapears into another plane, swirling colors swarm in the area where they had once stood.",ch->pet,NULL,NULL,TO_ROOM);
	send_to_char("You dissapear into a plane of swirling colors.\n\r",ch->pet);
	char_from_room(ch->pet);
	char_to_room(ch->pet,victim->in_room);
	act("$n `whas arrived from planar travel.",ch->pet,NULL,NULL,TO_ROOM);
	do_look(ch->pet,"auto");
    }
}

void spell_empower( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already as strong as you can get!\n\r",ch);
	else
	  act("$N `wcan't get any stronger.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level	 = level;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "Your muscles surge with heightened power!\n\r", victim );
    act("$n's `wmuscles surge with heightened power.",victim,NULL,NULL,TO_ROOM);
    return;
}

void spell_disintigrate( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;

    dam = UMAX(  20, victim->hit - roll_dice(1,4) );
    if ( saves_spell( level, victim ) )
	dam = UMIN( 50, dam / 2 );
    dam = UMIN( 100, dam );
    damage( ch, victim, dam, sn, DAM_EARTH );
    return;
}

/* RT entangle */
void spell_entangle( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
    if ( is_affected( victim, sn ) )
    {
	if (victim == ch)
	  send_to_char("You are already entangled!\n\r",ch);
 	else
	  act("$N `wis already entangled.",
	      ch,NULL,victim,TO_CHAR);
        return;
    }
    af.type      = sn;
    af.level     = level;
    if (victim == ch)
      af.duration  = level/2;
    else
      af.duration  = level/4;
    af.location  = APPLY_DEX;
    af.modifier  = -2 - (level >= 18) - (level >= 25) - (level >= 32);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You suddenly feel yourself being entangled.\n\r", 
victim );
    act("$n `wis suddenly entangled.",victim,NULL,NULL,TO_ROOM);
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}


void spell_haste( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
 
    if ( is_affected( victim, sn ) || IS_AFFECTED(victim,AFF_HASTE)
    ||   IS_SET(victim->off_flags,OFF_FAST))
    {
	if (victim == ch)
	  send_to_char("You can't move any faster!\n\r",ch);
 	else
	  act("$N `wis already moving as fast as $e can.",
	      ch,NULL,victim,TO_CHAR);
        return;
    }
    af.type      = sn;
    af.level     = level;
    if (victim == ch)
      af.duration  = level/2;
    else
      af.duration  = level/4;
    af.location  = APPLY_DEX;
    af.bitvector = 0;
    af.bit_type = BIT_AFFECT;
    af.modifier  = 1 + (level >= 18) + (level >= 25) + (level >= 32);
    affect_to_char( victim, &af );

    af.modifier  = 0;
    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_HASTE;
    af.bit_type = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "You feel yourself moving more quickly.\n\r", victim );
    act("$n `wis moving more quickly.",victim,NULL,NULL,TO_ROOM);
    if ( ch != victim )
        send_to_char( "Ok.\n\r", ch );
    return;
}

void spell_slow( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED_2(victim, AFF_SLOW) )
    {
	  act("$N `wis already moving as slow as $e can.",
	      ch,NULL,victim,TO_CHAR);
	return;
    }

    af.type      = sn;
    af.level     = level;
    af.duration  = level/3;
    af.location  = APPLY_DEX;
    af.modifier  = -1 - (level >= 18) - (level >= 25) - (level >= 32);
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.location  = APPLY_AC;
    af.modifier  = +20+(level/3);
    affect_to_char( victim, &af );
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_SLOW;
    af.bit_type	 = BIT_AFFECT2;
    affect_to_char( victim, &af );
    send_to_char( "You feel as if the world around you had become hasted.\n\r", victim );
    act( "$n `wbegins moving at an extremely slow pace.", victim, NULL, NULL, TO_ROOM );
    return;
}

void spell_heal( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->hit = UMIN( victim->hit + number_range(100,200), victim->max_hit );
    update_pos( victim );
    send_to_char( "A warm feeling fills your body.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}

/* RT really nasty high-level attack spell */
void spell_holy_word(int sn, int level, CHAR_DATA *ch, void *vo)
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int courage_num, frenzy_num, heal_num;
   
    courage_num = skill_lookup("courage");
    frenzy_num = skill_lookup("frenzy");
    heal_num = skill_lookup("heal");

    act("$n `wutters a word of divine power!",ch,NULL,NULL,TO_ROOM);
    send_to_char("You utter a word of divine power.\n\r",ch);

    if ( !IS_NPC(ch) )
    {
    	ch->move /= 2;
	ch->hit /= 2;
    }
 
    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
        vch_next = vch->next_in_room;

	if (!in_same_plane(ch, vch) )
	    continue;

 	  send_to_char("You feel more powerful.\n\r",vch);
	  spell_frenzy(frenzy_num,level,ch,(void *) vch); 
	  spell_courage(courage_num,level,ch,(void *) vch);
	  spell_heal(heal_num, level,ch,(void *) vch);
    }  
    
    send_to_char("You feel drained.\n\r",ch);
    gain_exp( ch, -1 * ch->level * 1000);
}
 
void spell_identify( int sn, int level, CHAR_DATA *ch, void *vo )
{
    OBJ_DATA *obj = (OBJ_DATA *) vo;
    char buf[MAX_STRING_LENGTH];
    AFFECT_DATA *paf;
    int skill;
    bool found;

    found = TRUE;
    if ( IS_SET( obj->extra_flags, ITEM_NO_IDENTIFY )
      && !IS_IMMORTAL( ch ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ( skill = get_skill( ch, sn ) ) <= 0 )
	skill = 100;

    sprintf(buf,"`R****************************************************\n\r");
    send_to_char(buf, ch);
    sprintf( buf, "`R* `wName: `G%s\n\r", obj->short_descr );
    send_to_char( buf, ch );

    if ( obj->wear_data != NULL )
    {
        if ( skill > number_range( 1, 100 ) )
	{
    	    sprintf( buf, "`R* `wThis item has specials: `G%s\n\r", extra_bit_name( obj->extra_flags ) );
    	    send_to_char( buf, ch );
	}
	else
	  found = FALSE;

	if ( skill > number_range( 1, 100 ) )
	{
   	    sprintf( buf, "`R* `wCondition: `G%d `wMaterial: `G%s `wValue: `G%ld `wWeight: `G%d `wSize `G%d\n\r",
            obj->condition, material_type_name( obj->material ), obj->wear_data->cost, obj->wear_data->weight,
	    obj->wear_data->size );
    	    send_to_char( buf, ch );
	}
	else
	  found = FALSE;

    	if ( obj->wear_data->ac != 0 )
    	{
	    if ( skill > number_range( 1, 100 ) )
	    {
            	sprintf( buf, "`R* `wArmor:`G %d\n\r", obj->wear_data->ac );
            	send_to_char( buf, ch );
	    }
	    else
	      found = FALSE;
    	}

    	if ( obj->wear_data->weapon_data != NULL )
    	{
	    if ( skill > number_range( 1, 100 ) )
	    {
            	sprintf( buf, "`R* `wWeapon: Damage - `G%dd%d `wType - `G%s `wDamage type - `G%s `wWeapon specials - `G%s\n\r",
            	obj->wear_data->weapon_data->dice, obj->wear_data->weapon_data->die, get_weapon_name(obj->wear_data->weapon_data->type),
            	get_type_weapon(obj->wear_data->weapon_data->dam_type), weapon_bit_name(obj->wear_data->weapon_data->flags) );
            	send_to_char( buf, ch );
	    }
	    else
	      found = FALSE;
    	}
    	if ( obj->wear_data->magic_data != NULL )
    	{
	    if ( skill > number_range( 1, 100 ) )
	    {
   	    	SPELL_LIST *spell;
            	sprintf( buf, "`R*`w%s, magical specials : `G%s `wwith `G%d(%d) `wcharges of:\n\r",
            	magical_type_name( obj->wear_data->magic_data->type ), magical_flag_name( obj->wear_data->magic_data->flags ),
            	obj->wear_data->magic_data->charges, obj->wear_data->magic_data->max_charges );
            	send_to_char( buf, ch );

       	    	if ( obj->wear_data->magic_data->spells == NULL )
            	    send_to_char( "`R*    `wNone.\n\r", ch );
            	else
            	    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
            	    {
                    	sprintf( buf, "`R*    `wLevel `G%d `wspell of `G%s.\n\r",
                    	spell->level, skill_table[ spell->spell ].name );
                    	send_to_char( buf, ch );
            	    }
	    }
	    else
	      found = FALSE;
    	}
    }

    if ( obj->cont_data != NULL )
    {
	if ( skill > number_range( 1, 100 ) )
	{
            sprintf( buf, "`R* `wCapacity: `G%d `wContainer specials: `G%s\n\r", obj->cont_data->capacity, 
	    get_flags_container( obj->cont_data->flags ) );
            send_to_char( buf, ch );
	}
	else
	    found = FALSE;

	if ( skill > number_range( 1, 100 ) )
	{
            if ( obj->cont_data->trap != NULL )
            	send_to_char( "Object has a trap!\n\r", ch );
	}
	else
	    found = FALSE;
    }

    if ( obj->edible_data != NULL )
    {
        SPELL_LIST * spell;

	if ( skill > number_range( 1, 100 ) )
	{
            sprintf( buf, "`R* `w%s with `G%d(%d) `whours of %s. `wSpecials - `G%s\n\r",
            ( IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) ? "Food" : "Drink" ),
            obj->edible_data->amount, obj->edible_data->max_amount,
            ( IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) ? "Food" : liq_table[ obj->edible_data->type ].liq_name ),
            edible_flag_name( obj->edible_data->flags ) );
            send_to_char( buf, ch );
	}
	else
	    found = FALSE;

        if ( obj->edible_data->spells != NULL )
        {
	    if ( skill > number_range( 1, 100 ) )
	    {
            	sprintf( buf, "`R*  `wWith %s type spells:\n\r",
                ( IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) ? "pill" : "potion" ) );
            	send_to_char( buf, ch );
            }
	    else
	        found = FALSE;
	}

        for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell->next )
        {
	    if ( skill > number_range( 1, 100 ) )
	    {
            	sprintf( buf, "`R*    `wLevel `G%d `wspell of `G%s.\n\r", spell->level, skill_table[ spell->spell ].name );
            	send_to_char( buf, ch );
	    }
	    else
	        found = FALSE;
        }
    }

    if ( obj->light_data != NULL )
    {
	if ( skill > number_range( 1, 100 ) )
	{
            sprintf( buf, "`R* `wLight: Fuel - `G%d `wMax fuel - `G%d `wBrightness - `G%d `wSpecials: `G%s\n\r",
            obj->light_data->fuel, obj->light_data->max_fuel, obj->light_data->brightness, light_flag_name( obj->light_data->flags ) );
            send_to_char( buf, ch );
	}
	else
	    found = FALSE;
    }

    if ( obj->wear_data !=NULL )
    	for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
    	{
	    if ( skill > number_range( 1, 100 ) )
	    {
            	if ( paf->location != APPLY_AFFECT )
           	    sprintf( buf, "`R* `wAffects `G%s `wby `G%d.\n\r", affect_loc_name( paf ), paf->modifier );
            	else
           	    sprintf( buf, "`R* `wApplies `G%s `wof `G%s.\n\r", get_bit_name(paf->bit_type), affect_loc_name( paf ) );
	    
            	send_to_char( buf, ch );
	    }
	    else
	        found = FALSE;
    	}

    if ( !found )
    {
        sprintf(buf,"`R****************************************************\n\r");
        send_to_char(buf, ch);
    }
    else
    {
        sprintf(buf,"`R****************************************************\n\r");
        send_to_char(buf, ch);
        sprintf(buf,"`R* `wYou feel certain of this items properties.\n\r");
        send_to_char(buf, ch);
        sprintf(buf,"`R****************************************************\n\r");
        send_to_char(buf, ch);
    }

    return;
}



void spell_infravision( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_INFRARED) )
    {
	if (victim == ch)
	  send_to_char("You can already see in the dark.\n\r",ch);
	else
	  act("$N `walready has infravision.\n\r",ch,NULL,victim,TO_CHAR);
	return;
    }
    act( "$n's `weyes glow red.\n\r", ch, NULL, NULL, TO_ROOM );
    af.type      = sn;
    af.level	 = level;
    af.duration  = 2 * level;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_INFRARED;
    af.bit_type  = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "Your eyes glow red.\n\r", victim );
    return;
}



void spell_shadowform( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SHADOWFORM) )
	return;

    act( "$n `wmelds into the shadows.", victim, NULL, NULL, TO_ROOM );
    af.type      = sn;
    af.level     = level;
    af.duration  = 24;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_SHADOWFORM;
    af.bit_type  = BIT_AFFECT;
    affect_to_char( victim, &af );
    send_to_char( "You become one with the shadows.\n\r", victim );
    return;
}

void spell_locate_object( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_INPUT_LENGTH];
    char buffer[4*MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    OBJ_DATA *in_obj;
    bool found;
    int number = 0, max_found;

    found = FALSE;
    number = 0;
    buffer[0] = '\0';
    max_found = IS_IMMORTAL(ch) ? 200 : 2 * level;
 
    for ( obj = object_list; obj != NULL; obj = obj->next )
    {
	if ( !can_see_obj( ch, obj ) || !is_name( target_name, obj->name ) 
    	||   (!IS_IMMORTAL(ch) 
	      && ( number_percent() > 2 * level || IS_SET( obj->extra_flags, ITEM_NO_LOCATE ) ) ) )
	    continue;

	found = TRUE;
        number++;

	for ( in_obj = obj; in_obj->in_obj != NULL; in_obj = in_obj->in_obj )
	    ;

	if ( in_obj->carried_by != NULL && can_see( ch, in_obj->carried_by ) )
	{
	    sprintf( buf, "%s carried by %s\n\r",
		obj->short_descr, PERS(in_obj->carried_by, ch) );
	}
	else
	{
	    if (IS_IMMORTAL(ch) && in_obj->in_room != NULL)
		sprintf( buf, "%s in %s [Room %ld]\n\r",
		    obj->short_descr, 
		    in_obj->in_room->name, in_obj->in_room->vnum);
	    else
	    	sprintf( buf, "%s in %s\n\r",
		    obj->short_descr, in_obj->in_room == NULL
		    	? "somewhere" : in_obj->in_room->name );
	}

	buf[0] = UPPER(buf[0]);
	strcat(buffer,buf);

    	if (number >= max_found)
	    break;
    }

    if ( !found )
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
    else if (ch->lines)
	page_to_char(buffer,ch);
    else
	send_to_char(buffer,ch);

    return;
}



void spell_flying_dagger( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 20, 20
    };
    int dam, x, hits;

    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);

    hits	= number_range( 1, ch->level / 6 );

    for ( x = 0; x < hits; x++ )
    {
    	dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    	if ( saves_spell( level, victim ) )
	    dam /= 2;
    	damage( ch, victim, dam, sn, DAM_WIND );
    }

    return;
}

void spell_ice_shards( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 20, 20
    };
    int dam, x, hits;

    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);

    hits	= number_range( 1, ch->level / 6 );

    for ( x = 0; x < hits; x++ )
    {
    	dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    	if ( saves_spell( level, victim ) )
	    dam /= 2;
    	damage( ch, victim, dam, sn, DAM_WATER );
    }

    return;
}

void spell_sand_jambiya( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const sh_int dam_each[] = 
    {
	 0,
	 3,  3,  4,  4,  5,	 6,  6,  6,  6,  6,
	 7,  7,  7,  7,  7,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 10, 10,
	10, 10, 10, 10, 10,	10, 10, 10, 20, 20
    };
    int dam, hits, x;

    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    hits	= number_range( 1, ch->level / 6 );

    for ( x = 0; x < hits; x++ )
    {
    	dam = number_range( dam_each[level] / 2, dam_each[level] * 2 );
    	if ( saves_spell( level, victim ) )
	    dam /= 2;
    	damage( ch, victim, dam, sn, DAM_EARTH );
    }
   
    return;
}

void spell_mass_healing(int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;
    
    heal_num = skill_lookup("heal");
    refresh_num = skill_lookup("refresh"); 

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ((IS_NPC(ch) && IS_NPC(gch)) ||
	    (!IS_NPC(ch) && !IS_NPC(gch)))
	{
	    spell_heal(heal_num,level,ch,(void *) gch);
	    spell_refresh(refresh_num,level,ch,(void *) gch);  
	}
    }
}
	    
void spell_group_healing(int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *gch;
    int heal_num, refresh_num;
    
    heal_num = skill_lookup("heal");
    refresh_num = skill_lookup("refresh"); 

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
    	if ( !is_same_group(gch, ch) 
    	|| ( gch == ch) )
    	continue;
	{
	    spell_heal(heal_num,level,ch,(void *) gch);
	    spell_refresh(refresh_num,level,ch,(void *) gch);  
	    update_pos(gch);
	    act("You feel a warm tingling sensation throughout your dody",ch,NULL,gch,TO_VICT);
	    act("$N `whas healed some of your wounds",ch,NULL,gch,TO_VICT);
	    act("You have healed some of $n's `wwounds",ch,NULL,gch,TO_CHAR);
	}
	return;
     }
}
	    

void spell_mass_shadowform( int sn, int level, CHAR_DATA *ch, void *vo )
{
    AFFECT_DATA af;
    CHAR_DATA *gch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( !is_same_group( gch, ch ) || IS_AFFECTED(gch, AFF_SHADOWFORM) )
	    continue;
	act( "$n `wslowly melds into the shadows.", gch, NULL, NULL, TO_ROOM );
	send_to_char( "You slowly become one with the shadows.\n\r", gch );
	af.type      = sn;
    	af.level     = level/2;
	af.duration  = 24;
	af.location  = APPLY_AFFECT;
	af.modifier  = 0;
	af.bitvector = AFF_SHADOWFORM;
    af.bit_type  = BIT_AFFECT;
	affect_to_char( gch, &af );
    }
    send_to_char( "Ok.\n\r", ch );

    return;
}



void spell_null( int sn, int level, CHAR_DATA *ch, void *vo )
{
    send_to_char( "That's not a spell!\n\r", ch );
    return;
}



void spell_dimension_walk( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_DIMENSION_WALK) )
    {
	if (victim == ch)
	  send_to_char("You are already in dimension walk.\n\r",ch);
	else
	  act("$N `wis already in dimension walk.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = number_fuzzy( level / 4 );
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_DIMENSION_WALK;
    af.bit_type  = BIT_AFFECT;
    affect_to_char( victim, &af );
    act( "$n `wsteps into another dimension.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You step into another dimension.\n\r", victim );
    return;
}

/* RT plague spell, very nasty */

void spell_plague( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if (saves_spell(level,victim) )
    {
	if (ch == victim)
	  send_to_char("You feel momentarily ill, but it passes.\n\r",ch);
	else
	  act("$N `wseems to be unaffected.",ch,NULL,victim,TO_CHAR);
	return;
    }

    af.type 	  = sn;
    af.level	  = level * 3/4;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.modifier  = -5; 
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_AFFECT;
    af.modifier  = 0; 
    af.bitvector = AFF_PLAGUE;
    af.bit_type  = BIT_AFFECT;
    affect_join(victim,&af);
   
    send_to_char
      ("You scream in agony as plague sores erupt from your skin.\n\r",victim);
    act("$n `wscreams in agony as plague sores erupt from $s skin.",
	victim,NULL,NULL,TO_ROOM);
}

void spell_poison( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( saves_spell( level, victim ) )
    {
	act("$n `wturns slightly green, but it passes.",victim,NULL,NULL,TO_ROOM);
	send_to_char("You feel momentarily ill, but it passes.\n\r",victim);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_STR;
    af.bitvector = 0;
    af.modifier  = -2;
    affect_to_char( victim, &af );
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_POISON;
    af.bit_type  = BIT_AFFECT;
    affect_join( victim, &af );
    send_to_char( "You feel very sick.\n\r", victim );
    act("$n `wlooks very ill.",victim,NULL,NULL,TO_ROOM);
    return;
}



void spell_breathe_underwater( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA *paf;
    AFFECT_DATA af;

    paf = NULL;
    if ( is_affected(victim,sn) )
    for ( paf = victim->affected; paf != NULL; paf = paf->next)
    {
	if (paf->bit_type == BIT_AFFECT)
	if (IS_SET(paf->bitvector, AFF_BREATHE_UNDERWATER ) )
	break;
    }

    if ( paf != NULL && paf->duration < 0)
    {
	if (victim == ch)
	  send_to_char("You can already breathe underwater.\n\r",ch);
	else
	  act("$N `wcan already breath underwater.",ch,NULL,victim,TO_CHAR);
	return;
    }
    else if ( paf != NULL )
    {
        paf->duration  = UMIN( ch->level * 4, paf->duration + ch->level );
	send_to_char("OK.\n\r",ch);
	return;
    }
    else
    {
        af.duration  = ch->level;
        af.type      = sn;
        af.level     = level;
        af.location  = APPLY_AFFECT;
        af.modifier  = 0;
        af.bitvector = AFF_BREATHE_UNDERWATER;
	af.bit_type  = BIT_AFFECT;
        affect_to_char( victim, &af );
        send_to_char( "You start to breathe easier.\n\r", victim );
        if ( ch != victim )
	    act("$N `wcan now breathe underwater.",ch,NULL,victim,TO_CHAR);
        return;
    }
}



void spell_refresh( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    victim->move = UMIN( victim->move + (3 * ch->level), victim->max_move );
    if (victim->max_move == victim->move)
	send_to_char("You feel fully refreshed!\n\r",victim);
    else
    	send_to_char( "You feel less tired.\n\r", victim );
    if ( ch != victim )
	send_to_char( "Ok.\n\r", ch );
    return;
}



void spell_remove_curse( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    bool found = FALSE;
    OBJ_DATA *obj;
    int iWear;

    if (check_dispel(level,victim,gsn_curse))
    {
	send_to_char("You feel better.\n\r",victim);
	act("$n `wlooks more relaxed.",victim,NULL,NULL,TO_ROOM);
    }

    for ( iWear = 0; (iWear < MAX_WEAR && !found); iWear ++)
    {
	if ((obj = get_eq_char(victim,iWear)) == NULL)
	    continue;

        if (IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE))
        {   /* attempt to remove curse */
	    if (!saves_dispel(level,obj->points * 3,0))
	    {
		found = TRUE; 
		REMOVE_BIT(obj->extra_flags,ITEM_NODROP);
		REMOVE_BIT(obj->extra_flags,ITEM_NOREMOVE);
	    	act("$p `wglows blue.",victim,obj,NULL,TO_CHAR);
		act("$p `wglows blue.",victim,obj,NULL,TO_ROOM);
	    }
	 }
    }

   for (obj = victim->carrying; (obj != NULL && !found); obj = obj->next_content)
   {
        if (IS_OBJ_STAT(obj,ITEM_NODROP) || IS_OBJ_STAT(obj,ITEM_NOREMOVE))
        {   /* attempt to remove curse */
            if (!saves_dispel(level,obj->points * 3,0))
            {
                found = TRUE;
                REMOVE_BIT(obj->extra_flags,ITEM_NODROP);
                REMOVE_BIT(obj->extra_flags,ITEM_NOREMOVE);
                act("Your $p `wglows blue.",victim,obj,NULL,TO_CHAR);
                act("$n's `w$p `wglows blue.",victim,obj,NULL,TO_ROOM);
            }
         }
    }
}

void spell_create_camp( int sn, int level, CHAR_DATA *ch, void *vo )
{
    ROOM_INDEX_DATA * room = ch->in_room;
    OBJ_DATA * obj;
    CHAR_DATA * victim;
    AFFECT_DATA af;
    int new_time = number_range( level / 8, level / 2 );

    for ( victim = ch->in_room->people ; victim != NULL ; victim = victim->next_in_room )
	if ( is_same_group( ch, victim ) )
	{
	    obj = create_object( get_obj_index( OBJ_VNUM_MUSHROOM ));
	    if ( obj->edible_data != NULL )
	    {
		obj->edible_data->amount = UMIN( 5 + 3 * level, 100 );
		obj->edible_data->max_amount = UMIN( 5 + 3 * level, 100 );
	    }
	    obj->timer = new_time + 1;
	    obj_to_char( obj, victim );

	    obj = create_object( get_obj_index( 1549 ));
	    obj->timer = new_time + 1;
	    obj_to_char( obj, victim );
	}

    af.type      = sn;
    af.level     = level;
    af.duration  = new_time;
    af.location  = APPLY_ROOM_FLAG;
    af.modifier  = 0;
    af.bit_type  = 0;
    af.bitvector = ROOM_MANAGEN;
    affect_apply_room( room, &af );
    af.bitvector = ROOM_HPGEN;
    affect_apply_room( room, &af );
    af.bitvector = ROOM_MVGEN;
    affect_apply_room( room, &af );

    act( "Large tents appear all around you, with food and water for all.", ch, NULL, NULL, TO_ROOM );
    act( "Large tents appear all around you, with food and water for all.", ch, NULL, NULL, TO_CHAR );
    return;
}

void spell_haven( int sn, int level, CHAR_DATA *ch, void *vo )
{
    ROOM_INDEX_DATA * room = ch->in_room;
    AFFECT_DATA af;

    if ( IS_SET( room->room_flags, ROOM_MANAGEN ) 
      && IS_SET( room->room_flags, ROOM_HPGEN )
      && IS_SET( room->room_flags, ROOM_MVGEN ) )
    {
	send_to_char( "The room is already a restfull haven.\n\r", ch );
	return;
    }

    af.type      = sn;
    af.level     = level;
    af.duration  = number_range( level / 8, UMAX( 4, level / 3 ) );
    af.location  = APPLY_ROOM_FLAG;
    af.modifier  = 0;
    af.bit_type  = 0;
    af.bitvector = ROOM_MANAGEN;
    affect_apply_room( room, &af );
    af.bitvector = ROOM_HPGEN;
    affect_apply_room( room, &af );
    af.bitvector = ROOM_MVGEN;
    affect_apply_room( room, &af );

    act( "A soft gentle breeze brings a calming mood to the room.", ch, NULL, NULL, TO_ROOM );
    act( "A soft gentle breeze brings a calming mood to the room.", ch, NULL, NULL, TO_CHAR );
    return;
}

void spell_sanctuary( int sn, int level, CHAR_DATA *ch, void *vo )
{
    ROOM_INDEX_DATA * room = ch->in_room;
    AFFECT_DATA af;

    if ( IS_SET( room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "The room is already a safe sanctuary.\n\r", ch );
	return;
    }

    af.type      = sn;
    af.level     = level;
    af.duration  = number_range( level / 8, UMAX( 4, level / 3 ) );
    af.location  = APPLY_ROOM_FLAG;
    af.modifier  = 0;
    af.bitvector = ROOM_SAFE;
    af.bit_type  = 0;
    affect_apply_room( room, &af );

    act( "The room is filled with a feeling of peace.", ch, NULL, NULL, TO_ROOM );
    act( "The room is filled with a feeling of peace.", ch, NULL, NULL, TO_CHAR );
    return;
}

void spell_circle_of_protection( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_AFFECTED(victim, AFF_SANCTUARY) && !IS_NPC( ch ) )
    {
	if (victim == ch)
	  send_to_char("You are already in a circle of protection.\n\r",ch);
	else
	  act("$N `wis already in a circle of protection.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = URANGE( 4, level / 5, 15 );
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_SANCTUARY;
    af.bit_type  = BIT_AFFECT;
    affect_to_char( victim, &af );

    if (IS_AFFECTED(victim, AFF_SNEAK))
       REMOVE_BIT (victim->affected_by, AFF_SNEAK) ;

    if (IS_AFFECTED( victim, AFF_HIDE))
       REMOVE_BIT(  victim->affected_by, AFF_HIDE) ;

    if (IS_AFFECTED_2( victim, AFF_FADE))
       REMOVE_BIT(  victim->affected_by_2, AFF_FADE) ;

    act( "$n `wis surrounded by a white aura.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a white aura.\n\r", victim );
    return;
}

void spell_protection_from_fire( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_PROTECTION ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from an element.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 3;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = RES_FIRE;
    af.bit_type  = BIT_RES;
    affect_to_char( victim, &af );
    send_to_char( "You feel extremely warm for a brief moment.\n\r", victim );
    return;
}

void spell_protection_from_water( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_PROTECTION ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from an element.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 3;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = RES_WATER;
    af.bit_type  = BIT_RES;
    affect_to_char( victim, &af );
    send_to_char( "You feel extremely damp for a brief moment.\n\r", victim );
    return;
}
void spell_protection_from_wind( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_PROTECTION ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from an element.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 3;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = RES_WIND;
    af.bit_type  = BIT_RES;
    affect_to_char( victim, &af );
    send_to_char( "You feel extremely light for a brief moment.\n\r", victim );
    return;
}
void spell_protection_from_earth( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_PROTECTION ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from an element.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 3;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = RES_EARTH;
    af.bit_type  = BIT_RES;
    affect_to_char( victim, &af );
    send_to_char( "You feel extremely tough for a brief moment.\n\r", victim );
    return;
}

void spell_protection_from_spirit( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_PROTECTION ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from an element.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 3;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = RES_SPIRIT;
    af.bit_type  = BIT_RES;
    affect_to_char( victim, &af );
    send_to_char( "You feel extremely clean for a brief moment.\n\r", victim );
    return;
}


void spell_fire_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_SHIELD ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = 10;
    af.location  = APPLY_AC;
    af.modifier  = -10;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n `wis surrounded by a `Rfiery shield`w.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a `Rfiery shield`w.\n\r", victim );
    return;
}

void spell_water_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_SHIELD ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = 10;
    af.location  = APPLY_AC;
    af.modifier  = -20;
    af.bitvector = 0;
    affect_to_char( victim, &af );

    af.modifier  = 0;
    af.location  = APPLY_AFFECT;
    af.bitvector = AFF_BREATHE_UNDERWATER;
    af.bit_type  = BIT_AFFECT;
    affect_to_char( victim, &af );
    act( "$n `wis surrounded by a shield of water.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a water shield.\n\r", victim );
    return;
}

void spell_earth_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_SHIELD ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = 10;
    af.location  = APPLY_AC;
    af.modifier  = -30;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location	 = APPLY_HIT;
    af.modifier  = 50;
    affect_to_char( victim, &af );
    act( "$n `wis surrounded by an earth shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by an earth shield.\n\r", victim );
    victim->hit += 50;
    return;
}

void spell_wind_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_SHIELD ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = 10;
    af.location  = APPLY_AC;
    af.modifier  = -30;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_DEX;
    af.modifier  = 3;
    affect_to_char( victim, &af );
    act( "$n `wis surrounded by a wind shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a wind shield.\n\r", victim );
    return;
}

void spell_spirit_shield( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( IS_SET( victim->spell_flags, SPELL_SHIELD ) )
    {
	if (victim == ch)
	  send_to_char("You are already shielded.\n\r",ch);
	else
	  act("$N `wis already protected by a shield.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = 10;
    af.location  = APPLY_MANA_SPIRIT;
    af.modifier  = level * 10 + 150;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    af.location  = APPLY_AC;
    af.modifier  = -10;
    affect_to_char( victim, &af );
    af.modifier	 = 0;
    af.location  = APPLY_AFFECT;
    af.bitvector = RES_SPIRIT;
    af.bit_type  = BIT_RES;
    affect_to_char( victim, &af );
    victim->mana[ELEMENT_SPIRIT] += ch->level * 10 + 150;
    act( "$n `wis surrounded by a spirit shield.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "You are surrounded by a spirit shield.\n\r", victim );
    return;
}

void spell_element_protection( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
    static const sh_int elem_each[] = 
    {
	 0,
	 1,  1,  1,  1,	 1,	1,   1,  1,  1,  1,
	 2,  2,  2,  2,	 2,	 3,  3,  3,  3,  3,
	 4,  4,  4,  4,	 5,	 5,  5,  5,  6,  6,
	 6,  6,  6,  7,	 7,	 7,  7,  7,  7,  8,
	 8,  8,  8,  8,	 8,	 8,  8,  8,  8,  8,
	 9,  9,  9,  9,  9,	 9, 10, 10, 10, 10
    };
    int elem;
    int x;
    int res;

    if (victim != ch)
    {
    	send_to_char("You cannot cast this on other people.\n\r",ch);
	return;
    }

    if ( IS_SET( victim->spell_flags, SPELL_PROTECTION ) )
    {
	if (victim == ch)
	  send_to_char("You are already protected from an element.\n\r",ch);
	else
	  act("$N `wis already protected by an element.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( is_affected( victim, skill_lookup("element protection" ) ) ) 
    {
	if (victim == ch)
	  send_to_char("You are already protected from the elements.\n\r",ch);
	else
	  act("$N `wis already protected from the elements.",ch,NULL,victim,TO_CHAR);
	return;
    }
    for (x = victim->level; x > 0; x = x - 10)
    {
    	level	= ch->level;
    	elem	= number_range( 1, elem_each[level] );
    	switch ( elem )
    	{
		case 0:
		{
			send_to_char("The forces of the elements do not respond anymore.\n\r",ch);
			return;
		}
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		{
			res = number_range(1, 5);
			switch ( res )
			{
				case 1:
				{
					if (IS_SET(victim->res_flags, RES_FIRE) )
						break;
					if (IS_SET(victim->imm_flags, IMM_FIRE) )
						break;
					if (IS_SET(victim->vuln_flags, VULN_FIRE) )
						break;
	    				af.type      = sn;
	    				af.level     = level;
	    				af.duration  = 8 + level / 2;
	    				af.location  = APPLY_AFFECT;
    					af.modifier  = 0;
    					af.bitvector = RES_FIRE;
					af.bit_type  = BIT_RES;
    					affect_to_char( victim, &af );
    					act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    					send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
					break;
				}
				case 2:
				{
					if (IS_SET(victim->res_flags, RES_WATER) )
						break;
					if (IS_SET(victim->imm_flags, IMM_WATER) )
						break;
					if (IS_SET(victim->vuln_flags, VULN_WATER) )
						break;
    					af.type      = sn;
    					af.level     = level;
    					af.duration  = 8 + level / 2;
	    				af.location  = APPLY_AFFECT;
    					af.modifier  = 0;
    					af.bitvector = RES_WATER;
					af.bit_type  = BIT_RES;
    					affect_to_char( victim, &af );
    					act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    					send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
					break;
				}
				case 3:
				{
					if (IS_SET(victim->res_flags, RES_WIND) )
						break;
					if (IS_SET(victim->imm_flags, IMM_WIND) )
						break;
					if (IS_SET(victim->vuln_flags, VULN_WIND) )
						break;
    					af.type      = sn;
    					af.level     = level;
    					af.duration  = 8 + level / 2;
	    				af.location  = APPLY_AFFECT;
    					af.modifier  = 0;
    					af.bitvector = RES_WIND;
					af.bit_type  = BIT_RES;
    					affect_to_char( victim, &af );
    					act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    					send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
					break;
				}
				case 4:
				{
					if (IS_SET(victim->res_flags, RES_EARTH) )
						break;
					if (IS_SET(victim->imm_flags, IMM_EARTH) )
						break;
					if (IS_SET(victim->vuln_flags, VULN_EARTH) )
						break;
    					af.type      = sn;
    					af.level     = level;
    					af.duration  = 8 + level / 2;
	    				af.location  = APPLY_AFFECT;
    					af.modifier  = 0;
    					af.bitvector = RES_EARTH;
					af.bit_type  = BIT_RES;
    					affect_to_char( victim, &af );
    					act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    					send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
					break;
				}
				case 5:
				{
					if (IS_SET(victim->res_flags, RES_SPIRIT) )
						break;
					if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
						break;
					if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
						break;
    					af.type      = sn;
    					af.level     = level;
    					af.duration  = 8 + level / 2;
	    				af.location  = APPLY_AFFECT;
    					af.modifier  = 0;
    					af.bitvector = RES_SPIRIT;
					af.bit_type  = BIT_RES;
    					affect_to_char( victim, &af );
    					act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    					send_to_char( "You are momentarily transparent.\n\r", victim );
					break;
				}
			} 
			break;
		}
		case 6:
		{
			res = number_range(1, 100);
			if (res <= 5)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
		    				af.type      = sn;
		    				af.level     = level;
		    				af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
	    					af.modifier  = 0;
    						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
	    					affect_to_char( victim, &af );
	    					act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
	    					send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                   	                     	if (IS_SET(victim->res_flags, RES_WATER) )
                        	                        break;
                                	        if (IS_SET(victim->imm_flags, IMM_WATER) )
                                        	        break;
                                        	if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                	break;    	
						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
	                                        if (IS_SET(victim->res_flags, RES_WIND) )
        	                                        break;
                	                        if (IS_SET(victim->imm_flags, IMM_WIND) )
                        	                        break;
                                	        if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                        	        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
	                                        if (IS_SET(victim->res_flags, RES_EARTH) )
        	                                        break;
                	                        if (IS_SET(victim->imm_flags, IMM_EARTH) )
                        	                        break;
                                	        if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                        	        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
	                                        if (IS_SET(victim->res_flags, RES_SPIRIT) )
        	                                        break;
                	                        if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                        	                        break;
                                	        if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                        	        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_FIRE;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_WATER;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_WIND;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by a strong gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by a strong gust of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_EARTH;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_SPIRIT;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel surrounded by a transparent shield.\n\r", victim );
						break;
					}
				}
			}
			if (res >= 90)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surround by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WIND;

						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_FIRE;
						af.bit_type  = BIT_VULN;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_WATER;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_WIND;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_EARTH;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_SPIRIT;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of spirit.\n\r", victim );
						break;
					}
				}
			}
			else
			{
				res = number_range(1, 5);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_FIRE;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
				}
			} 
			break;
		}
		case 7:
		{
			res = number_range(1, 100);
			if (res <= 10)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
		    				af.type      = sn;
		    				af.level     = level;
		    				af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
	    					af.modifier  = 0;
						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
	    					affect_to_char( victim, &af );
	    					act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
	    					send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_FIRE;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_WATER;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_WIND;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by a strong gust of wind.", victim, NULL, NULL, TO_ROOM );
     						send_to_char( "You are surrounded by a strong gust of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_EARTH;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_SPIRIT;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel surrounded by a transparent shield.\n\r", victim );
						break;
					}
				}
			}
			if (res >= 92)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surround by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_FIRE;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_WATER;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_WIND;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_EARTH;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_SPIRIT;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of spirit.\n\r", victim );
						break;
					}
				}
			}
			else
			{
				res = number_range(1, 5);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{

                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{

                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
				}
			} 
			break;
		}
		case 8:
		{
			res = number_range(1, 100);
			if (res <= 15)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
		    				af.type      = sn;
		    				af.level     = level;
		    				af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
	    					af.modifier  = 0;
						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
	    					affect_to_char( victim, &af );
	    					act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
	    					send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_FIRE;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_WATER;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_WIND;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by a strong gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by a strong gust of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_EARTH;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = IMM_SPIRIT;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel surrounded by a transparent shield.\n\r", victim );
						break;
					}
				}
			}
			if (res >= 94)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surround by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
						af.bitvector = VULN_FIRE;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_WATER;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_WIND;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_EARTH;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_SPIRIT;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of spirit.\n\r", victim );
						break;
					}
				}
			}
			else
			{
				res = number_range(1, 5);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
				}
			} 
			break;
		}
		case 9:
		{
			res = number_range(1, 100);
			if (res <= 20)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
		    				af.type      = sn;
		    				af.level     = level;
		    				af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
	    					af.modifier  = 0;
	    					af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
	    					affect_to_char( victim, &af );
	    					act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
	    					send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_FIRE;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_WATER;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_WIND;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by a strong gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by a strong gust of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_EARTH;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_SPIRIT;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel surrounded by a transparent shield.\n\r", victim );
						break;
					}
				}
			}
			if (res >= 96)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_FIRE;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_WATER;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_WIND;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_EARTH;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_SPIRIT;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of spirit.\n\r", victim );
						break;
					}
				}
			}
			else
			{
				res = number_range(1, 5);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
				}
			} 
			break;
		}
		case 10:
		{
		 	res = number_range(1, 100);
			if (res <= 25)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
		    				af.type      = sn;
		    				af.level     = level;
		    				af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
	    					af.modifier  = 0;
	    					af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
	    					affect_to_char( victim, &af );
	    					act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
	    					send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_FIRE;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_WATER;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_WIND;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by a strong gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by a strong gust of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_EARTH;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						act( "$n `wis surrounded by millions of particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are surrounded by millions of particles of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = IMM_SPIRIT;
						af.bit_type  = BIT_IMM;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel surrounded by a transparent shield.\n\r", victim );
						break;
					}
				}
			}
			if (res >= 98)
			{
				x = x - 15;
				res = number_range(1, 10);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
					case 6:
					{
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_FIRE;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of fire.\n\r", victim );
						break;
					}
					case 7:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_WATER;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of water.\n\r", victim );
						break;
					}
					case 8:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_WIND;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of wind.\n\r", victim );
						break;
					}
					case 9:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_EARTH;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of earth.\n\r", victim );
						break;
					}
					case 10:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = VULN_SPIRIT;
						af.bit_type  = BIT_VULN;
    						affect_to_char( victim, &af );
    						send_to_char( "You feel vulnerable to the elements of spirit.\n\r", victim );
						break;
					}
				}
			}
			else
			{
				res = number_range(1, 5);
				switch ( res )
				{
					case 1:
					{
						if (IS_SET(victim->res_flags, RES_FIRE) )
							break;
						if (IS_SET(victim->imm_flags, IMM_FIRE) )
							break;
						if (IS_SET(victim->vuln_flags, VULN_FIRE) )
							break;
	    					af.type      = sn;
	    					af.level     = level;
	    					af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_FIRE;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of fire.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of fire.\n\r", victim );
						break;
					}
					case 2:
					{
                                                if (IS_SET(victim->res_flags, RES_WATER) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WATER) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WATER) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WATER;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of water.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of water.\n\r", victim );
						break;
					}
					case 3:
					{
                                                if (IS_SET(victim->res_flags, RES_WIND) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_WIND) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_WIND) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_WIND;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis brushed by a gust of wind.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily brushed by a gust of wind.\n\r", victim );
						break;
					}
					case 4:
					{
                                                if (IS_SET(victim->res_flags, RES_EARTH) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_EARTH) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_EARTH) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_EARTH;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily surrounded by particles of earth.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily surrounded by particles of earth.\n\r", victim );
						break;
					}
					case 5:
					{
                                                if (IS_SET(victim->res_flags, RES_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->imm_flags, IMM_SPIRIT) )
                                                        break;
                                                if (IS_SET(victim->vuln_flags, VULN_SPIRIT) )
                                                        break;
    						af.type      = sn;
    						af.level     = level;
    						af.duration  = 8 + level / 2;
	    					af.location  = APPLY_AFFECT;
    						af.modifier  = 0;
    						af.bitvector = RES_SPIRIT;
						af.bit_type  = BIT_RES;
    						affect_to_char( victim, &af );
    						act( "$n `wis temporarily transparent.", victim, NULL, NULL, TO_ROOM );
    						send_to_char( "You are momentarily transparent.\n\r", victim );
						break;
					}
				}
			} 
			break;
		}
	}
    }
    return;
}

void spell_wind_vortex( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    static const int dam_each[] = 
    {
	 0,
	 0,  0,  0,  0,  0,	 0, 20, 25, 29, 33,
	36, 39, 39, 39, 40,	40, 41, 41, 42, 42,
	43, 43, 44, 44, 45,	45, 46, 46, 47, 47,
	48, 48, 49, 49, 50,	50, 51, 51, 52, 52,
	53, 53, 54, 54, 55,	55, 56, 56, 57, 57,
	61, 65, 69, 73, 77,	81, 85, 89, 93, 97	
    };
    int dam;

    level	= UMIN(level, (signed) sizeof(dam_each)/(signed) sizeof(dam_each[0]) - 1);
    level	= UMAX(0, level);
    dam		= number_range( dam_each[level] / 2, dam_each[level] * 2 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_WIND );
    return;
}

void spell_sleep( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;
  
    if ( IS_AFFECTED(victim, AFF_SLEEP) 
    ||   level < victim->level
    ||   saves_spell( level, victim ) )
	return;

    af.type      = sn;
    af.level     = level;
    af.duration  = 4 + level;
    af.location  = APPLY_AFFECT;
    af.modifier  = 0;
    af.bitvector = AFF_SLEEP;
    af.bit_type  = BIT_AFFECT;
    affect_join( victim, &af );

    if ( IS_AWAKE(victim) )
    {
	send_to_char( "You feel very sleepy ..... zzzzzz.\n\r", victim );
	act( "$n `wgoes to sleep.", victim, NULL, NULL, TO_ROOM );
	set_position( victim, POS_SLEEPING );
    }

    return;
}

void spell_stone_skin( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( ch, sn ) )
    {
	if (victim == ch)
	  send_to_char("Your skin is already as hard as a rock.\n\r",ch); 
	else
	  act("$N `wis already as hard as can be.",ch,NULL,victim,TO_CHAR);
	return;
    }
    af.type      = sn;
    af.level     = level;
    af.duration  = level;
    af.location  = APPLY_AC;
    af.modifier  = -40;
    af.bitvector = 0;
    affect_to_char( victim, &af );
    act( "$n's `wskin turns to stone.", victim, NULL, NULL, TO_ROOM );
    send_to_char( "Your skin turns to stone.\n\r", victim );
    return;
}

void spell_summon( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim;

    if ( ( victim = get_char_world( ch, target_name ) ) == NULL
    ||   victim == ch
    ||   victim->in_room == NULL
    ||   ( victim->pIndexData != NULL && victim->pIndexData->pShop != NULL )
    ||   victim->in_room->area->visible > ch->level
    ||   IS_SET(victim->in_room->room_flags, ROOM_SAFE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_PRIVATE)
    ||   IS_SET(victim->in_room->room_flags, ROOM_SOLITARY)
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    ||   (IS_NPC(victim) && IS_SET(victim->act,ACT_AGGRESSIVE|ACT_ATTACK_WORLD))
    ||   victim->level >= level + 3
 /*   ||   (!IS_NPC(victim) && !IS_IMMORTAL(ch) ) */
    ||   victim->fighting != NULL
    ||   victim->in_room->area->world != ch->in_room->area->world
    ||   (IS_NPC(victim) && IS_SET(victim->imm_flags,IMM_SUMMON))
    ||   (!IS_NPC(victim) && IS_SET(victim->plr,PLR_NOSUMMON))  
    ||   (IS_NPC(victim) && ( saves_spell( level, victim ) || saves_spell( level, victim ) ) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }
    if (victim->inside_area != NULL ) /* Don't carry the moveable area data */
	free_inside_area_data(victim->inside_area);
	victim->inside_area = NULL;
    act( "$n `wdisappears suddenly.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, ch->in_room );
    act( "$n `warrives suddenly.", victim, NULL, NULL, TO_ROOM );
    act( "$n `whas summoned you!", ch, NULL, victim,   TO_VICT );
    do_look( victim, "auto" );
    if ( victim->mount != NULL && IS_SET( victim->mount->affected_by,AFF_MOUNTED) )
    {
        char_from_room( victim->mount );
        char_to_room( victim->mount, ch->in_room );
    }
    return;
}

void spell_teleport( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    ROOM_INDEX_DATA *pRoomIndex;

    if ( victim->in_room == NULL
    ||   IS_SET(victim->in_room->room_flags, ROOM_NO_RECALL)
    || ( !IS_NPC(ch) && victim->fighting != NULL )
    || ( victim != ch
    && ( saves_spell( level, victim ) || saves_spell( level, victim ) ) ) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    for ( ; ; )
    {
	pRoomIndex = get_room_index( number_range( 0, 65535 ) );

	if ( pRoomIndex != NULL )
	if ( can_see_room(ch,pRoomIndex)
	&&   pRoomIndex->area->visible <= ch->level
	&&   ch->in_room->area->world == pRoomIndex->area->world
	&&   !IS_SET( pRoomIndex->room_flags, ROOM_NEWBIES_ONLY)
	&&   !IS_SET( pRoomIndex->room_flags, ROOM_PRIVATE)
	&&   !IS_SET( pRoomIndex->room_flags, ROOM_SOLITARY) 
	&&   !IS_SET( pRoomIndex->room_flags, ROOM_NO_RECALL)
	&&   !IS_SET( pRoomIndex->room_flags_2, ROOM_STARTING) )
	    break;
    }

    if (victim != ch)
	send_to_char("You have been teleported!\n\r",victim);

    act( "$n `wvanishes!", victim, NULL, NULL, TO_ROOM );
    if (victim->inside_area != NULL ) /* Don't carry the moveable area data */
	free_inside_area_data(victim->inside_area);
	victim->inside_area = NULL;
    char_from_room( victim );
    char_to_room( victim, pRoomIndex );
    act( "$n `wslowly fades into existence.", victim, NULL, NULL, TO_ROOM );
    do_look( victim, "auto" );
    if ( victim->mount != NULL && IS_SET( victim->mount->affected_by,AFF_MOUNTED) )
    {
        char_from_room( victim->mount );
        char_to_room( victim->mount, pRoomIndex );
    }
    return;
}

void spell_weaken( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    AFFECT_DATA af;

    if ( is_affected( victim, sn ) || saves_spell( level, victim ) )
	return;
    af.type      = sn;
    af.level     = level;
    af.duration  = level / 2;
    af.location  = APPLY_STR;
    af.modifier  = -1 * (level / 5);
    af.bitvector = 0;
    affect_to_char( victim, &af );
    send_to_char( "You feel weaker.\n\r", victim );
    act("$n `wlooks tired and weak.",victim,NULL,NULL,TO_ROOM);
    return;
}

/* RT recall spell is back */

void spell_return_to_safety( int sn, int level, CHAR_DATA *ch, void *vo )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    CHAR_DATA *fighting;
    ROOM_INDEX_DATA *location;
    
    if ( victim == NULL ||
       (IS_NPC(victim) && !IS_SET(victim->act2, ACT2_GUIDE) && !IS_SET(victim->act,ACT_PET) && !IS_SET(victim->act,ACT_MOUNT)) )
      return;

    location = NULL;
    if ( !IS_NPC( victim ) && victim->pcdata->pcclan == NULL )
    {
        if ( ( location = get_room_index( race_table[ victim->race ][ victim->sex ].begin ) ) == NULL )
            location = get_room_index( 2 );
    }
    else if ( !IS_NPC( victim ) )
    {
        location = get_room_index( victim->pcdata->pcclan->clan->recall_room );
        if ( location == NULL )
            if ( ( location = get_room_index( race_table[ victim->race ][ victim->sex ].begin ) ) == NULL )
                location = get_room_index( 2 );
    }
    else if ( IS_NPC( victim ) )
        if ( ( location = get_room_index( race_table[ victim->race ][ victim->sex ].begin ) ) == NULL )
                location = get_room_index( 2 );
   
    if ( victim->in_room == location || location == NULL )
    {
	send_to_char( "You cannot find safety.\n\r", ch );
	return;
    }

    if ( IS_SET(ch->in_room->room_flags, ROOM_NO_RECALL)
    ||   IS_AFFECTED(ch, AFF_CURSE) )
    {
	send_to_char( "God has forsaken you.\n\r", ch );
	return;
    }

    if ( ( fighting = ch->fighting ) != NULL )
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

	lose = (victim->desc != NULL) ? 25 : 50;
	gain_exp( victim, 0 - lose );
	sprintf( buf, "You recall from combat!  You lose %d exps.\n\r", lose );
	send_to_char( buf, victim );
	stop_fighting( victim, TRUE );
	
    }

    victim->move /= 2;
    act( "$n `wdisappears.", victim, NULL, NULL, TO_ROOM );
    if ( victim->inside_area != NULL )
	free_inside_area_data(victim->inside_area);

    victim->inside_area = NULL;	
    char_from_room( victim );
    char_to_room( victim, location );
    act( "$n `wappears in the room.", victim, NULL, NULL, TO_ROOM );
    do_look( victim, "auto" );
    if (victim->mount != NULL )
    {
        if ( IS_SET(victim->mount->affected_by,AFF_MOUNTED) )
	{
            char_from_room( victim->mount );
            char_to_room( victim->mount, location );
	}
	else
	    do_recall( victim->mount, "" );
    }

    if (victim->pet != NULL)
	do_recall(victim->pet,"");

    return;
}

/*
 * NPC spells.
 */
void spell_acid_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    OBJ_DATA *t_obj,*n_obj;
    int dam;
    int hpch;
    int i;
    if ( number_percent( ) < 2 * level && !saves_spell( level, victim ) )
    {
	for ( obj_lose = ch->carrying; obj_lose != NULL; obj_lose = obj_next )
	{

	    int iWear=WEAR_NONE;

	    obj_next = obj_lose->next_content;

	    if ( number_bits( 2 ) != 0 )
		continue;


	    if ( obj_lose->wear_data != NULL && !IS_SET( obj_lose->extra_flags, ITEM_MAGIC ) )
	    {
		    act( "$p `wis pitted and etched!",
			victim, obj_lose, NULL, TO_CHAR );
		    if ( ( iWear = obj_lose->wear_data->wear_loc ) != WEAR_NONE )
			for (i = 0; i < 4; i ++)
			    victim->armor -= obj_lose->wear_data->ac;
		    obj_lose->wear_data->ac = UMAX( 0,obj_lose->wear_data->ac-1 );
		    obj_lose->wear_data->cost  /= 2;
		    if ( iWear != WEAR_NONE )
			for (i = 0; i < 4; i++)
			    victim->armor += obj_lose->wear_data->ac;
	    }

	    if ( obj_lose->cont_data != NULL )
	    {
		act( "$p `wfumes and dissolves, destroying some of the contents.",
		    victim, obj_lose, NULL, TO_CHAR );

            	for (t_obj = obj_lose->cont_data->contents; t_obj != NULL; t_obj = n_obj)
            	{
                    n_obj = t_obj->next_content;
                    obj_from_obj(t_obj);

		    if (number_bits(2) == 0 || victim->in_room == NULL)
			extract_obj(t_obj);
		    else 
			obj_to_room(t_obj,victim->in_room);
		}

		extract_obj( obj_lose );
	    }
	}
    }
    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_WATER );
    return;
}

void spell_fire_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    OBJ_DATA *obj_lose;
    OBJ_DATA *obj_next;
    OBJ_DATA *t_obj, *n_obj;
    int dam;
    int hpch;
    if ( number_percent( ) < (level/2) && !saves_spell( level, victim ) )
    {
	for ( obj_lose = victim->carrying; obj_lose != NULL;
	obj_lose = obj_next )
	{
	    char *msg;

	    obj_next = obj_lose->next_content;
	    if ( number_bits( 2 ) != 0 )
		continue;

	    if ( IS_SET( obj_lose->extra_flags, ITEM_MAGIC ) )
		continue;

	    if ( obj_lose->cont_data != NULL )
		msg = "$p `wignites and burns!";
	    else if ( obj_lose->wear_data->magic_data != NULL && IS_SET( obj_lose->wear_data->magic_data->flags, MAGIC_SCROLL ) )
		msg = "$p `wcrackles and burns!";
	    else if ( obj_lose->wear_data->magic_data != NULL && IS_SET( obj_lose->wear_data->magic_data->flags, MAGIC_STAFF ) )
		msg = "$p `wsmokes and chars!";
	    else if ( obj_lose->wear_data->magic_data != NULL && IS_SET( obj_lose->wear_data->magic_data->flags, MAGIC_WAND ) )
		msg = "$p `wsparks and sputters!";
	    else if ( obj_lose->edible_data != NULL && IS_SET( obj_lose->edible_data->flags, FOOD_EDIBLE ) )
		msg = "$p `wblackens and crisps!";
	    else if ( obj_lose->edible_data != NULL && !IS_SET( obj_lose->edible_data->flags, FOOD_EDIBLE ) )
		msg = "$p `wbubbles and boils!";
	    else
		msg = "$p `wignites and burns!";

	    act( msg, victim, obj_lose, NULL, TO_CHAR );
	    if ( obj_lose->cont_data != NULL )
	    {
                for (t_obj = obj_lose->cont_data->contents; t_obj != NULL; t_obj = n_obj)
                {
                    n_obj = t_obj->next_content;
                    obj_from_obj(t_obj);

                    if (number_bits(2) == 0 || ch->in_room == NULL)
                        extract_obj(t_obj);
                    else
                    	obj_to_room(t_obj,ch->in_room);
		}
	    }

	    extract_obj( obj_lose );
	}
    }
    hpch = UMAX( 10, ch->hit );
    dam  = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_FIRE );
    return;
}

void spell_gas_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    int dam;
    int hpch;

    for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    {
	vch_next = vch->next_in_room;
	if ( !is_safe_spell(ch,vch,TRUE) && !is_same_group(ch,vch))
	{
	    hpch = UMAX( 10, ch->hit );
	    dam  = number_range( hpch/16+1, hpch/8 );
	    if ( saves_spell( level, vch ) )
		dam /= 2;
	    damage( ch, vch, dam, sn, DAM_POISON );
	}
    }
    return;
}

void spell_lightning_breath( int sn, int level, CHAR_DATA *ch, void *vo )
{
    CHAR_DATA *victim = (CHAR_DATA *) vo;
    int dam;
    int hpch;

    hpch = UMAX( 10, ch->hit );
    dam = number_range( hpch/16+1, hpch/8 );
    if ( saves_spell( level, victim ) )
	dam /= 2;
    damage( ch, victim, dam, sn, DAM_WATER );
    return;
}
