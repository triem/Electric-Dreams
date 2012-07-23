/**************************************************************************c
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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "merc.h"
#include <ctype.h>

/*
 * Control the fights going on.
 * Called periodically by update_handler.
 */
void violence_update( void )
{
    CHAR_DATA *ch;
    CHAR_DATA *ch_next;
    CHAR_DATA *victim;

    for ( ch = char_list; ch != NULL; ch = ch->next )
    {
	ch_next	= ch->next;

	if ( ( victim = ch->fighting ) == NULL || ch->in_room == NULL )
	    continue;

	if ( IS_SET( ch->plr, PLR_FLEE ) && ch->wait <= 0 )
	    flee_char( ch );

	script_update(ch, TRIG_COMBAT );

	if ( IS_AWAKE(ch) && ch->in_room == victim->in_room && !IS_SET( ch->plr, PLR_FLEE ) )
	    multi_hit( ch, victim, TYPE_UNDEFINED );
	else
	    stop_fighting( ch, FALSE );

	if ( ( victim = ch->fighting ) == NULL )
	    continue;

	check_assist(ch,victim);
    }

    return;
}

void do_apply( CHAR_DATA *ch, char *argument )
{
    int cost;

    if ( IS_NPC( ch ) )
	return;

    if ( IS_SET( ch->plr, PLR_PK ) )
    {
        cost = 100000 * ch->level;
        if ( cost > get_money_total( ch, ch->in_room->area->world ) )
        {
            send_to_char("You cannot afford to disable PK!\n\r",ch);
            return;
        }

	REMOVE_BIT( ch->plr, PLR_PK );
        send_to_char( "You have disabled PK.\n\r", ch );
        get_char_money( ch, cost, ch->in_room->area->world );
	return;
    }

    cost = 10000 * ch->level;
    if ( cost > get_money_total( ch, ch->in_room->area->world ) )
    {
	send_to_char( "You cannot afford to enable PK!\n\r", ch );
	return;
    }

    get_char_money( ch, cost, ch->in_room->area->world );
    SET_BIT( ch->plr, PLR_PK );
    send_to_char( "You are now PK enabled.\n\r", ch );
    return;    
}

/* for auto assisting */
void check_assist( CHAR_DATA *ch, CHAR_DATA *victim )
{
    CHAR_DATA *rch, *rch_next;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch_next )
    {
	rch_next = rch->next_in_room;
	
	if ( IS_AWAKE( rch ) && rch->fighting == NULL )
	{
	    /* quick check for ASSIST_PLAYER */
	    if ( !IS_NPC(ch) && IS_NPC(rch) 
	      && IS_SET( rch->off_flags, ASSIST_PLAYERS )
	      && rch->level + 6 > victim->level )
	    {
		do_emote( rch, "screams and attacks!" );
		multi_hit( rch, victim, TYPE_UNDEFINED );
		continue;
	    }
	    
	    if ( !IS_NPC( ch ) && IS_NPC( rch ) 
	      && is_same_group( ch, rch ) )
	    {
		multi_hit( rch, victim, TYPE_UNDEFINED );
		continue;
	    }

	    /* PCs next */
	    if ( !IS_NPC( ch ) || IS_AFFECTED( ch, AFF_CONTROL_MIND ) )
	    {
		if ( ( ( !IS_NPC( rch ) && IS_SET( rch->plr, PLR_AUTOASSIST ) )
		  || IS_AFFECTED( rch, AFF_CONTROL_MIND ) ) 
		  && is_same_group( ch, rch ) )
		    multi_hit( rch, victim, TYPE_UNDEFINED );
		
		continue;
	    }
  	
	    /* now check the NPC cases */
 	    if ( IS_NPC( ch ) && !IS_AFFECTED( ch, AFF_CONTROL_MIND ) )
	    {
		if ( ( IS_NPC( rch ) && IS_SET( rch->off_flags, ASSIST_ALL ) )
		  || ( IS_NPC( rch ) && rch->race == ch->race 
		    && IS_SET( rch->off_flags, ASSIST_RACE ) )  
		  || ( rch->pIndexData == ch->pIndexData 
		    && IS_SET( rch->off_flags, ASSIST_VNUM ) )
		  || is_same_group( ch, rch ) )
	   	{
		    CHAR_DATA *vch;
		    CHAR_DATA *target;
		    int number;

		    if ( number_bits( 1 ) == 0 )
			continue;
		
		    target = NULL;
		    number = 0;
		    for ( vch = ch->in_room->people; vch; vch = vch->next_in_room )
		    {
			if ( can_see( rch, vch )
			  && is_same_group( vch, victim )
			  && number_range( 0, number ) == 0 )
			{
			    target = vch;
			    number++;
			}
		    }

		    if ( target != NULL )
		    {
			do_emote( rch, "screams and attacks!" );
			multi_hit( rch, target, TYPE_UNDEFINED );
		    }
		}	
	    }
	}
    }
}

/*
 * Do one group of attacks.
 */
void multi_hit( CHAR_DATA *ch, CHAR_DATA *victim, int dt )
{
    /* decremtnt wait */
    if ( ch->desc == NULL )
    	ch->wait = UMAX( 0, ch->wait - PULSE_VIOLENCE );

    if ( ch->position < POS_RESTING )
     	return; /*If they are stunned, or bitten ... they are gonners */

    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
     	stop_fighting( ch, FALSE );
     	stop_fighting( victim, FALSE );
     	return;
    }

    if ( IS_SET( ch->affected_by_2, AFF_STUN ) )
    {
     	send_to_char( "`YYou are STUNNED!`w\n\r", ch );
     	act( "`Y$N is STUNNED!`w", ch, NULL, ch, TO_ROOM );
     	REMOVE_BIT( ch->affected_by_2, AFF_STUN );
     	return;
    }

    if ( IS_NPC( ch ) )
    { 
	mob_hit( ch, victim, dt );
     	return; /*Mobs dont' get special attacks......yet :) */
    }             
   
    switch ( ch->specattack )
    { 
       case 0  : multi_hit_standard ( ch, victim, dt ); break;
    }
} 

void multi_hit_standard ( CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    OBJ_DATA * wield;
    int chance, skill, combination_chance, fast_chance, 
	shola_kick_chance, shola_punch_chance, high_kick_chance, 
	knee_punch_chance, sweep_chance, toss_chance, sweep, toss;

    if ( ( wield = get_eq_char( ch, WEAR_WIELD_R ) ) == NULL )
	if ( ( wield = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) == NULL )
	    wield = get_eq_char( ch, WEAR_WIELD_L );

    one_hit( ch, victim, wield, dt );

    if (ch->fighting != victim)
	return;

    if (IS_AFFECTED(ch,AFF_HASTE) && !IS_AFFECTED_2(ch,AFF_SLOW))
	one_hit(ch,victim, wield, dt);
    
    if ( IS_SET(ch->off_flags, OFF_FAST) && !IS_AFFECTED(ch, AFF_HASTE) && !IS_AFFECTED_2(ch, AFF_SLOW) )
	one_hit(ch,victim,wield,dt);

    if ( IS_SET(ch->off_flags, OFF_SECOND) )
	one_hit( ch, victim, wield, dt);

    if ( IS_SET( ch->off_flags, OFF_THIRD ) )
	one_hit( ch, victim, wield, dt );

    if ( ch->fighting != victim || dt == gsn_shadowstrike )
	return;

    chance = get_skill(ch,gsn_second_attack);
		/* npc's don't need weapons to get this attack */
    if ( IS_NPC(ch) && chance > 1 && number_percent( ) < chance )
    {
	one_hit( ch, victim, wield, dt );
	if ( ch->fighting != victim )
	return;
    }
    else if ( !IS_NPC(ch) && chance > 1 && number_percent( ) < chance
    && ( get_eq_char( ch, WEAR_WIELD_R ) != NULL
    ||  get_eq_char( ch, WEAR_WIELD_L ) != NULL
    ||  get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) != NULL ) )
    {
	one_hit( ch, victim, wield, dt );
	check_improve(ch,gsn_second_attack,TRUE,5);
	if ( ch->fighting != victim )
	    return;
    }

    chance = get_skill(ch,gsn_third_attack)/2;
		/* npc's don't need weapons to get this attack */
    if ( IS_NPC(ch) && chance > 1 && number_percent( ) < chance )
    {
	one_hit( ch, victim, wield, dt );
	if ( ch->fighting != victim )
	return;
    }
    else if ( !IS_NPC(ch) && chance > 1 && number_percent( ) < chance
    && ( get_eq_char( ch, WEAR_WIELD_R ) != NULL
    || get_eq_char( ch, WEAR_WIELD_L ) != NULL
    || get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) != NULL ) )
    {
	one_hit( ch, victim, wield, dt );
	check_improve(ch,gsn_third_attack,TRUE,6);
	if ( ch->fighting != victim )
	    return;
    }

    toss_chance = get_skill( ch, gsn_shola_toss ) + get_curr_stat( ch, STAT_STR ) - ( get_curr_stat( victim, STAT_STR ) / 2 ) - ( victim->level / 4 );
    sweep_chance = get_skill( ch, gsn_leg_sweep ) + get_curr_stat( ch, STAT_DEX ) - ( get_curr_stat( victim, STAT_DEX ) / 2 ) - ( victim->level / 4 );
    fast_chance = get_skill(ch,gsn_fast)/2 + (ch->level/2) - (victim->level/4);
    shola_kick_chance = get_skill(ch,gsn_shola_kick)/2 + (ch->level/2) - (victim->level/4);
    shola_punch_chance = get_skill(ch,gsn_shola_punch)/2 + (ch->level/2) - (victim->level/4);
    high_kick_chance = get_skill(ch,gsn_high_kick)/2 + (ch->level/2) - (victim->level/4);
    knee_punch_chance = get_skill(ch,gsn_knee_punch)/2 + (ch->level/2) - (victim->level/4);
    combination_chance = get_skill(ch, gsn_combination);
    sweep = 0;
    toss = 0;
    if ( IS_SET( ch->affected_by_2, AFF_SLOW ) )
	combination_chance -= 15;

    if ( IS_SET( ch->affected_by, AFF_HASTE ) )
	combination_chance += 15;

    if ( get_skill( ch, gsn_combination ) > 0 )
    {
    	while ( combination_chance > 1 && number_percent() < combination_chance  )
    	{
	    if ( ch->fighting != victim )
	    	return;
            switch ( number_range(1, 10) )
     	    {
	    	case 1:
		    if ( get_skill( ch, gsn_shola_kick ) > 0 && number_percent() < shola_kick_chance && get_eq_char( ch, WEAR_FEET ) == NULL )
		    {
		    	one_hit( ch, victim, wield, gsn_shola_kick );
		     	check_improve(ch,gsn_shola_kick,TRUE,4);
			if ( number_percent() < ( shola_kick_chance / 3 * 2 ) )
			{
	    		    if ( ch->fighting != victim )
	    			return;
		    	    one_hit( ch, victim, wield, gsn_shola_kick );
		    	    check_improve(ch,gsn_shola_kick,TRUE,4);
			}
    			combination_chance -= 15;
		    }
		    else if ( get_eq_char( ch, WEAR_FEET ) == NULL )
		    	check_improve(ch,gsn_shola_kick,FALSE,4);			
		    break;
	    	case 2:
		    if ( get_eq_char( ch, WEAR_HANDS ) == NULL && get_skill( ch, gsn_shola_punch ) > 0 && number_percent( ) < shola_punch_chance
		    && get_eq_char( ch, WEAR_WIELD_R ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_L ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) == NULL )
		    {
		    	one_hit( ch, victim, wield, gsn_shola_punch );
		    	check_improve(ch,gsn_shola_punch,TRUE,4);
			if ( number_percent( ) < ( shola_punch_chance / 3 * 2 ) )
			{
	    		    if ( ch->fighting != victim )
	    			return;
		    	    one_hit( ch, victim, wield, gsn_shola_punch );
		    	    check_improve(ch,gsn_shola_punch,TRUE,4);
			}
    			combination_chance -= 15;
		    }
		    else if ( get_eq_char( ch, WEAR_WIELD_R ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_L ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) == NULL )
		    	check_improve(ch,gsn_shola_punch,FALSE,4);			
		    break;
	    	case 3:
		    if ( get_skill( ch, gsn_high_kick ) > 0 && number_percent( ) < high_kick_chance && get_eq_char( ch, WEAR_FEET ) == NULL )
		    {
		    	one_hit( ch, victim, wield, gsn_high_kick );
		     	check_improve(ch,gsn_high_kick,TRUE,4);
    			combination_chance -= 15;
		    }
		    else if ( get_eq_char( ch, WEAR_FEET ) == NULL )
			check_improve(ch,gsn_high_kick,FALSE,4);
		    break;
	    	case 4:
		    if ( get_skill( ch, gsn_knee_punch ) > 0 && number_percent( ) < knee_punch_chance )
		    {
		    	one_hit( ch, victim, wield, gsn_knee_punch );
		    	check_improve(ch,gsn_knee_punch,TRUE,4);
    			combination_chance -= 15;
		    }
		    else
			check_improve(ch,gsn_knee_punch,FALSE,4);
		    break;
	    	case 5:
		    if ( get_skill( ch, gsn_fast ) > 0 && number_percent( ) < fast_chance
		    && get_eq_char( ch, WEAR_WIELD_R ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_L ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) == NULL )
		    {
		    	one_hit( ch, victim, wield, dt );
		    	check_improve(ch,gsn_fast,TRUE,4);
    			combination_chance -= 15;
		    }
		    else if ( get_eq_char( ch, WEAR_WIELD_R ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_L ) == NULL
    		    && get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) == NULL )
		    	check_improve(ch,gsn_fast,FALSE,4);			
		    break;
		case 6:
		    if ( sweep == 0 && !IS_AFFECTED( victim, AFF_LEVITATING ) && !IS_AFFECTED( victim, AFF_FLYING ) && get_skill( ch, gsn_leg_sweep ) > 0 && get_eq_char( ch, WEAR_FEET ) == NULL )
		    {
			if ( number_percent( ) < sweep_chance )
			{
			    send_to_char("`WYou tumble to the ground as your feet are swept from beneath you.\n\r", victim);
			    send_to_char("`WYou sweep your opponents feet from beneath them and they tumble to the ground.\n\r", ch);
	    		    WAIT_STATE(victim, PULSE_VIOLENCE); 
		    	    check_improve(ch,gsn_leg_sweep,TRUE,4);
			}
			else
			    check_improve( ch, gsn_leg_sweep, FALSE, 4 );

			sweep = 1;
    			combination_chance -= 15;
		    }
		    break;
		case 7:
		    if ( toss == 0 && !IS_AFFECTED( victim, AFF_LEVITATING ) && !IS_AFFECTED( victim, AFF_FLYING ) && get_skill( ch, gsn_shola_toss ) > 0 && number_percent( ) < toss_chance )
		    {
			WAIT_STATE(ch, PULSE_VIOLENCE);
	    		WAIT_STATE(victim, 2 * PULSE_VIOLENCE); 
			act("`Y$n tosses you over their shoulder!`w",ch,NULL,victim,TO_VICT);
			act("`YYou toss $N over your shoulder!`w",ch,NULL,victim,TO_CHAR);
	  		act("`Y$n tosses $N over their shoulder!`w",ch,NULL,victim,TO_NOTVICT);
		    	check_improve(ch,gsn_shola_toss,TRUE,4);
    			combination_chance -= 15;
			toss = 1;
			if ( number_percent( ) < ( toss_chance / 3 ) )
			{
	    		    if ( ch->fighting != victim )
	    			return;
			    act("`Y$n knocks the air from your lungs!`w",ch,NULL,victim,TO_VICT);
			    act("`YYou knock the air from $N's lungs!`w",ch,NULL,victim,TO_CHAR);
	  		    act("`Y$n knocks the air from $N's lungs!.`w",ch,NULL,victim,TO_NOTVICT);
     			    SET_BIT(victim->affected_by_2, AFF_STUN);
			}
		    }
		    break;
	    	default:
		    combination_chance -= number_range(5,10);
		    break;
	    }
    	}
	check_improve(ch,gsn_combination,TRUE,4);
    }
    else if ( get_skill( ch, gsn_fast ) > 0 && number_percent( ) < fast_chance 
    && get_eq_char( ch, WEAR_WIELD_R ) == NULL
    && get_eq_char( ch, WEAR_WIELD_L ) == NULL
    && get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) == NULL )
    {
	one_hit( ch, victim, wield, dt );
	check_improve(ch,gsn_fast,TRUE,4);
    }

    if ( ( skill = get_skill( ch, gsn_dual_wield ) ) <= 0 
	|| wield == NULL
	|| ( wield->wear_data != NULL && wield->wear_data->wear_loc != WEAR_WIELD_R )
	|| ( wield = get_eq_char( ch, WEAR_WIELD_L ) ) == NULL )
        return;

    check_improve(ch,gsn_dual_wield,TRUE,2);
    skill += get_curr_stat( ch, STAT_DEX );
    skill = skill / 2;

    chance = skill;
    if ( chance > 1 && number_percent( ) < chance )
    {
	one_hit( ch, victim, wield, dt );
	if ( ch->fighting != victim )
	    return;
    }

    chance = skill;
    if (chance > 1 && number_percent( ) < chance && IS_AFFECTED( ch, AFF_HASTE ) && !IS_AFFECTED_2( ch, AFF_SLOW ) )
    {
	one_hit( ch, victim, wield, dt );
	if ( ch->fighting != victim )
	    return;
    }

    return;
}

/* procedure for all mobile attacks */
void mob_hit (CHAR_DATA *ch, CHAR_DATA *victim, int dt)
{
    CHAR_DATA *vch, *vch_next;
    OBJ_DATA *wield;

    if (IS_SET(ch->act, ACT_WIMPY) )
    {
	if (victim->position <= POS_STUNNED )
	return;
    }

    if ( IS_SET(ch->affected_by_2, AFF_STUN) )
    {
     	send_to_char( "`YYou are STUNNED!`w\n\r", ch);
        act("`Y$N is STUNNED!",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->affected_by_2, AFF_STUN);
	return;
    }	

    if ( ( wield = get_eq_char( ch, WEAR_WIELD_R ) ) == NULL )
	if ( ( wield = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) == NULL )
	    wield = get_eq_char( ch, WEAR_WIELD_L );

    multi_hit_standard( ch, victim, dt );

    if (ch->fighting != victim)
	return;

    /* Area attack -- BALLS nasty! */
 
    if (IS_SET(ch->off_flags,OFF_AREA_ATTACK))
    {
	for (vch = ch->in_room->people; vch != NULL; vch = vch_next)
	{
	    vch_next = vch->next_in_room;
	    if ((vch != victim && vch->fighting == ch))
		one_hit(ch,vch, wield, dt);
	}
    }

    if (ch->fighting != victim || dt == gsn_shadowstrike)
	return;

}
	

/*
 * Hit one guy once.
 */
void one_hit( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA * wielding, int dt )
{
    OBJ_DATA *wield;
    int dam;
    int chance;
    int diceroll;
    int roll;
    int sn,skill;
    int crit_bonus = 0;
    int dam_type;
    char buf[MAX_STRING_LENGTH];
    sn = -1;
    wield = wielding;

    /* just in case */
    if (victim == ch || ch == NULL || victim == NULL)
	return;
    
    /*
     * Can't beat a dead char!
     * Guard against weird room-leavings.
     */
    if ( victim->position == POS_DEAD || ch->in_room != victim->in_room )
	return;

    if ( IS_SET(ch->affected_by_2, AFF_STUN) )
    {
     	send_to_char( "`YYou are STUNNED!`w\n\r", ch );
        act("`Y$N is STUNNED!",ch,NULL,NULL,TO_ROOM);
	REMOVE_BIT(ch->affected_by_2, AFF_STUN); 
	return;
    }

    if ( wield == NULL || wield->wear_data == NULL || wield->wear_data->weapon_data == NULL )
	wield = NULL;

    if ( dt == TYPE_UNDEFINED )
    {
	dt = TYPE_HIT;
	if ( wield != NULL )
	    dt += wield->wear_data->weapon_data->dam_type;
	else 
	    dt += ch->dam_type;
    }

    if (dt < TYPE_HIT)
    	if (wield != NULL)
    	    dam_type = attack_table[wield->wear_data->weapon_data->dam_type].damage;
    	else
    	    dam_type = attack_table[ch->dam_type].damage;
    else
    	dam_type = attack_table[dt - TYPE_HIT].damage;

    if (dam_type == -1)
	dam_type = DAM_BASH;

    /* get the weapon skill */ /*here Quin*/
    skill = get_weapon_skill(ch,wield);

    if ( dt < TYPE_HIT && dt > 50 )
	skill += get_skill( ch, dt ) - 100;

    chance = skill;
    chance += GET_HITROLL(ch);

    if ( ch->mount != NULL && IS_AFFECTED( ch->mount, AFF_MOUNTED) )
	chance += ( IS_AFFECTED(ch->mount, AFF_FLYING|AFF_LEVITATING) ? get_skill( ch,gsn_riding_air ) : get_skill( ch, gsn_riding ) ) / 5;

    if (dt == gsn_shadowstrike)
	chance += 150;

    chance += GET_AC(victim);

    chance += ch->level;
    chance -= victim->level;

    if ( !can_see( ch, victim ) )
	chance -= 40;

    if ( victim->position < POS_FIGHTING)
	chance += 10;
 
    if (victim->position < POS_RESTING)
	chance += 30;

    /*
     * The moment of excitement!
     */
    roll = number_percent();

    if ( IS_SET( victim->plr, PLR_FLEE ) && !is_affected( ch, gsn_retreat ) )
	roll += skill / 40;

    if ( roll <= 1 )
    {
	AFFECT_DATA af;

	/* Fumble Table */
	/* higher value = the less of a fumble */
	switch ( URANGE( 1, number_range( 1, 4 ) + ( skill - 100 ) / 150 , 4 ) )
 	{
	default:
	case 1:
	    if ( wield != NULL )
	    {
		act( "`Y$p slips from $n's grasp.`w", ch, wield, wield, TO_ROOM );
		act( "`Y$p slips from your grasp.`w", ch, wield, wield, TO_CHAR );
		obj_from_char( wield, "P" );
		obj_to_room( wield, ch->in_room );
		if (IS_NPC(ch) )
		SET_BIT(ch->act2, ACT2_WEAPON_LOST );
	    }
	    else
	    {
		act( "$n's loses their balance!", ch, NULL, NULL, TO_ROOM );
		act( "You lose your balance and almost fall to the ground!", ch, NULL, NULL, TO_CHAR );
	       	af.type         = 0;
	       	af.level        = ch->level;
        	af.duration     = number_range(1,2);
        	af.location     = APPLY_DEX;
        	af.modifier     = number_range( -5, -2 );
        	affect_to_char(ch,&af);
	    }
	    break;
	case 2:
	    if ( wield != NULL )
	    {
		act( "$n swings wildly with $p and damage themselves.", ch, wield, wield, TO_ROOM );
		act( "You swing wildly with $p and it hits your leg.", ch, wield, wield, TO_CHAR );
	    	damage( ch, ch, number_range(1,ch->level), dt, dam_type );
	    }
	    else
	    {
		act( "$n swings wildly and hits themselves!", ch, NULL, NULL, TO_ROOM );
		act( "You swing wildly and hit yourself!", ch, NULL, NULL, TO_CHAR );
	    	damage( ch, ch, number_range(1,ch->level), dt, dam_type );
	    }
	    break;
	case 3:
	    act( "$n trips over $s own left foot and falls flat on $s face.", ch, NULL, NULL, TO_ROOM );
	    act( "You trip over your own left foot and fall flat on your face.", ch, NULL, NULL, TO_CHAR );
	    set_position( ch, POS_RESTING );
	    WAIT_STATE(ch,PULSE_VIOLENCE); 
	    break;
	case 4:
	    act( "$n slips and falls to the ground.", ch, victim, victim, TO_ROOM );
	    act( "You slip and fall to the ground.", ch, victim, victim, TO_CHAR );
	    set_position( ch, POS_RESTING );
	    WAIT_STATE(ch,PULSE_VIOLENCE);
	    break;
	}
	return;
    }
    else if ( roll >= 100 )
    {
	crit_bonus = 100;

	/* Critical Hit Table */
	/* higher value = greater hit */
	switch ( URANGE( 1, number_range( 1, 4 ) + ( skill - 100 ) / 150 , 4 ) )
	{
	default:
	case 1:
	    act( "$n carves $s initials in $N's armor.", ch, victim, victim, TO_ROOM );
	    act( "You carves your initials in $N's armor.", ch, victim, victim, TO_CHAR );
	    crit_bonus = 125;
    	    check_armor_damage( victim );
	    break;
	case 2:
	    /* extra attack */
	    one_hit(ch, victim, wield, dt );
	    break;
	case 3:
	    act( "$n gets a critical hit on $N!", ch, victim, victim, TO_NOTVICT );
	    act( "You get a critical hit on $N!", ch, victim, victim, TO_CHAR );
	    act( "$n gets a critical hit on you!", ch, victim, victim, TO_VICT );
	    crit_bonus = 150;
	    break;
	case 4:
	    act( "$n hits $N in the crotch! *OUCH*", ch, victim, victim, TO_NOTVICT );
	    act( "$n hits you in the crotch! *OUCH*", ch, victim, victim, TO_VICT );
	    act( "You hit $N in the crotch! *OUCH*", ch, victim, victim, TO_CHAR );
	    crit_bonus = 200;
	    break;
	}
    }
    else if ( ( roll < 6 || roll > chance ) && roll < 95 ) /* miss */
    {
        if ( victim->mount != NULL && IS_AFFECTED( victim->mount, AFF_MOUNTED )&& ( number_percent() >= ( IS_AFFECTED( victim->mount, AFF_FLYING|AFF_LEVITATING )? 
           get_skill( victim, gsn_riding_air ) : get_skill( victim, gsn_riding ) )/3 
           + victim->mount->level / 3 ) )
	{
	    one_hit( ch, victim->mount, wield, dt);
	}
	else
	    damage( ch, victim, 0, dt, dam_type );
	    tail_chain( );
	    return;
    }

    /*
     * Hit.
     * Calc damage.
     */
    if ( IS_NPC(ch) && wield == NULL && ( dt >= TYPE_HIT || dt < 50 ) )
	    dam = roll_dice(ch->damage[DICE_NUMBER],ch->damage[DICE_TYPE]);
    else
    {
	get_weapon_improve(ch, wield);

	if (sn != -1)
	    check_improve(ch,sn,TRUE,5);

	if ( wield != NULL )
	{
	    if ( wield->wear_data->weapon_data->dice > 100 || wield->wear_data->weapon_data->die > 100)
	    {
		sprintf(buf,"Dice: %d:  more then 100!  Vnum %ld",wield->wear_data->weapon_data->dice,wield->pIndexData->vnum);
		bug(buf,0); 
	    	dam = 1;
	    }
	    else
	    	dam = roll_dice(wield->wear_data->weapon_data->dice,wield->wear_data->weapon_data->die) * skill/75;

	    if (get_eq_char(ch,WEAR_SHIELD) == NULL)  /* no shield = more */
		dam = dam * 21/20;

	    if ( wield->wear_data != NULL && wield->wear_data->wear_loc == WEAR_WIELD_TWO_HANDED && is_bastard( ch, wield ) )
		 dam = dam * 3 / 2;

    if ( get_skill(ch,gsn_weapon_enhancement) > 0 )
    {
        diceroll = number_percent();
        if (diceroll <= get_skill(ch,gsn_weapon_enhancement))
        {
            check_improve(ch,gsn_weapon_enhancement,TRUE,8);
            dam += dam * diceroll/100;
        }
    }

	}
	else if ( dt == gsn_shola_kick || dt == gsn_shola_punch || dt == gsn_high_kick || dt == gsn_knee_punch )
	    dam = roll_dice( get_skill( ch, dt ) / 15, 6 ) + ( ch->level / 4 );
	else
	    dam = number_range( skill * skill / 2000, skill * skill / 1000 );
    }

    /*
     * Bonuses.
     */
    if ( crit_bonus > 0 )
        dam = dam * crit_bonus / 100; 

    if ( ch->mount != NULL && IS_AFFECTED(ch->mount, AFF_MOUNTED) )
	dam += ch->mount->level / 10 + ( IS_AFFECTED(ch->mount, AFF_FLYING|AFF_LEVITATING) ? get_skill( ch, gsn_riding_air ) : get_skill( ch, gsn_riding ) ) / 10;

    if ( !IS_AWAKE(victim) )
	dam *= 2;
     else if (victim->position < POS_FIGHTING)
	dam = dam * 3 / 2;

    if ( IS_AFFECTED( ch, AFF_BERSERK ) )
	dam = dam * 3 / 2;

    if ( dt == gsn_shadowstrike && wield != NULL) 
    {
    	if ( wield->wear_data->weapon_data->type != 2 )
		dam *= 2 + ch->level / 10; 
	else 
		dam *= 2 + ch->level / 8;
    }

    if ( ( dt == gsn_quick_strike && wield == NULL ) )
	dam *= 2 + ch->level / 12;

    dam += GET_DAMROLL(ch);
    if ( dam <= 0 )
	dam = 1;

    if ( wield != NULL && wield->wear_data != NULL 
	&& wield->wear_data->weapon_data != NULL )
    {
	if ( IS_SET(wield->wear_data->weapon_data->flags, WEAPON_SHARP) )
	    dam += 1;

        if ( wield->wear_data->magic_data != NULL )
     	    if ( wield->wear_data->magic_data->spells != NULL )
    	    	if ( number_range( 1, 12 ) == 5 )
            	    obj_cast_spell( ch, victim, wield, wield->wear_data->magic_data->spells, MAGIC_WAND );

    	if ( IS_SET(wield->wear_data->weapon_data->flags, WEAPON_FLAMING) )
	{
	    if ( IS_SET(victim->imm_flags, IMM_FIRE) )
	    {
	    	damage(ch, victim, dam, dt, dam_type); 
    	    	tail_chain( );
    	    	return;
	    }
	    else if ( IS_SET(victim->res_flags, RES_FIRE) )
	    	dam += number_range( 2, 8 );
	    else if ( IS_SET(victim->vuln_flags, VULN_FIRE) )
	    	dam += number_range( 16, 22 );
	    else if ( !IS_SET(victim->imm_flags, IMM_FIRE) )
		dam += number_range( 8, 14 );
	    damage(ch, victim, dam, dt, DAM_FIRE); 
    	    tail_chain( );
    	    return;
	}
    	else if ( IS_SET(wield->wear_data->weapon_data->flags, WEAPON_VORPAL ) )
	{
	    if ( number_range( 0, 10 ) <= 1 )
	    {
	    	dam = dam * 3 / 2;
	    	damage(ch, victim, dam, dt, DAM_VORPAL); 
    	    	tail_chain( );
    	    	return;
	    }
	}
   	 else if ( IS_SET(wield->wear_data->weapon_data->flags, WEAPON_FROST) )
	{
	    if ( IS_SET(victim->imm_flags, IMM_WATER) )
	    {
	    	damage(ch, victim, dam, dt, dam_type); 
    	    	tail_chain( );
    	    	return;
	    }
	    else if ( IS_SET(victim->res_flags, RES_WATER) )
	    	dam += number_range( 2, 8 );
	    else if ( IS_SET(victim->vuln_flags, VULN_WATER) )
	    	dam += number_range( 16, 22 );
	    else if ( !IS_SET(victim->imm_flags, IMM_WATER) )
		dam += number_range( 8, 14 );
	    damage(ch, victim, dam, dt, DAM_WATER); 
    	    tail_chain( );
    	    return;
	}
	else if ( IS_SET(wield->wear_data->weapon_data->flags, WEAPON_VAMPIRIC) )
	{
	    ch->hit = UMIN( ch->hit + ( dam / 5 ), ch->max_hit);
	    damage(ch, victim, dam, dt, DAM_SPIRIT); 
    	    tail_chain( );
    	    return;
	}
	else if ( IS_SET(wield->wear_data->weapon_data->flags, WEAPON_POISON) )
	{
	    damage(ch, victim, dam, dt, DAM_POISON);
	    tail_chain( );
	    return;
	}
    }
    damage( ch, victim, dam, dt, dam_type );
    tail_chain( );
    return;
}


/* this is for fire shield damage only, can be expanded for more but as
is it MUST be called from bool damage because it doesn't make many of
the checks needed when applying damage */

bool fire_damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type )
{
  bool immune;
  int skill;
  if ( victim->position == POS_DEAD )
    return FALSE;

  if (dam_type == -1)
    dam_type = DAM_BASH;

  /*
   * Damage modifiers.
   */
  if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
    dam /= 2;
  if ( ( skill = get_skill( victim, gsn_endurance ) ) > 0 )
  {
    if ( number_percent( ) <= skill )
    {
      dam -= dam / 4 * skill / 100;
      check_improve(victim, gsn_endurance, TRUE, 10 );
    }
  }

  immune = FALSE;	
  dam_message( ch, victim, dam, dt, 0, immune );

  if ( !IS_NPC(victim) )
  {
    check_hp_improve( victim, dam );
    check_armor_damage( victim );
  }

  victim->hit -= dam;
  if ( !IS_NPC(victim) && IS_IMMORTAL(victim) && victim->hit < 1 )
    victim->hit = 1;
	
  return TRUE;
}

/*
 * Inflict damage from a hit.
 */
bool damage( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type )
{
    CHAR_DATA *fch;
    CHAR_DATA *fch_next;
    OBJ_DATA *corpse;
    bool immune, trig;	 
    bool fCrit = FALSE;
    int skill;

   if ( victim->position == POS_DEAD )
	return FALSE;

    if ( dam > 10000 )
    {
	fCrit = TRUE;
	dam -= 10000;
    }

    if ( victim != ch )
    {
	/*
	 * Certain attacks are forbidden.
	 * Most other attacks are returned.
	 */
	if ( is_safe( ch, victim ) )
	    return FALSE;

	if ( victim->position > POS_STUNNED )
	{
	    if ( victim->fighting == NULL )
		set_fighting( victim, ch );
	    if (victim->timer <= 4)
		set_position( victim, POS_FIGHTING );
	}

	if ( victim->position > POS_STUNNED )
	{
	    if ( ch->fighting == NULL )
		set_fighting( ch, victim );

	    /*
	     * If victim is controlled, ch might attack victim's master.
	     */
	    if ( IS_NPC(ch)
	    &&   IS_NPC(victim)
	    &&   IS_AFFECTED(victim, AFF_CONTROL_MIND)
	    &&   victim->master != NULL
	    &&   victim->master->in_room == ch->in_room
	    &&   number_bits( 3 ) == 0 )
	    {
		stop_fighting( ch, FALSE );
		multi_hit( ch, victim->master, TYPE_UNDEFINED );
		return FALSE;
	    }
	}

	/*
	 * More control stuff.
	 */
	if ( victim->master == ch )
	    stop_follower( victim );
    }

    /*
     * Inviso attacks ... not.
     */
    if ( IS_AFFECTED(ch, AFF_SHADOWFORM) )
    {
	affect_strip( ch, gsn_shadowform );
	affect_strip( ch, gsn_mass_shadowform );
	REMOVE_BIT( ch->affected_by, AFF_SHADOWFORM );
	cact( "$n fades into existence.", ch, NULL, NULL, TO_ROOM, "`W" );
    }

    /*
     * Damage modifiers.
     */
    if ( IS_AFFECTED(victim, AFF_SANCTUARY) )
	dam /= 2;

    if ( ( skill = get_skill( victim, gsn_endurance ) ) > 0 )
    	if ( number_percent( ) <= skill )
    	{
	    dam -= dam / 4 * skill / 100;
    	    check_improve(victim, gsn_endurance, TRUE, 10 );
	}

    immune = FALSE;
    /*
     * Check for parry, and dodge.
     */
    if ( dt >= TYPE_HIT 
	&& ch != victim 
	&& !fCrit 
	&& !IS_AFFECTED( victim, AFF_BERSERK )
	&& ( !IS_SET( victim->plr, PLR_FLEE )
	  || is_affected( ch, gsn_retreat ) ) )
    {
	
	switch( number_range( 1, 24 ) )
	{
	    case 1:
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	    	    return FALSE;
		break;
	    case 2:
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		break;
	    case 3:
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		break;
	    case 4:
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		break;
	    case 5:
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		break;
	    case 6:
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		break;
	    case 7:
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		break;
	    case 8:
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		break;
	    case 9:
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		break;
	    case 10:
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		break;
	    case 11:
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		break;
	    case 12:
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		break;
	    case 13:
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		break;
	    case 14:
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		break;
	    case 15:
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		break;
	    case 16:
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		break;
	    case 17:
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		break;
	    case 18:
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		break;
	    case 19:
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		break;
	    case 20:
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		break;
	    case 21:
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		break;
	    case 22:
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		break;
	    case 23:
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		break;
	    case 24:
		if ( check_wind_over_sand( ch, victim ) )
	       	    return FALSE;
        	if ( check_parry( ch, victim ) )
	      	    return FALSE;
        	if ( check_block( ch, victim ) )
	    	    return FALSE;
		if ( check_dodge( ch, victim ) )
	    	    return FALSE;
		break;
	}
    }

    switch(check_immune(victim,dam_type))
    {
	case(IS_IMMUNE):
	    immune = TRUE;
	    dam = 0;
	    break;
	case(IS_RESISTANT):	
	    dam -= dam/2;
	    break;
	case(IS_VULNERABLE):
	    dam += dam/2;
	    break;
    }
    if (dam_type == -1)
	dam_type = DAM_BASH;

    dam_message( ch, victim, dam, dt, dam_type, immune );

    if (dam == 0)
	return FALSE;

    /*
     * Hurt the victim.
     * Inform the victim of his new state.
     */
    victim->hit -= dam;

    if ( !IS_NPC(victim) && IS_IMMORTAL(victim) && victim->hit < 1 )
	victim->hit = 1;

    if ( !IS_NPC(victim) )
        check_hp_improve( victim, dam );

    check_armor_damage( victim );
/* check for fire_shield damage and that the damage is not from a spell */
    if ( (dam_type != DAM_FIRE) && (dam_type != DAM_WATER) 
	&& (dam_type != DAM_SPIRIT) && (dam_type != DAM_EARTH) 
	&& (dam_type != DAM_WIND) && (dam_type != DAM_ENERGY) 
	&& (dam_type != DAM_HARM) 
	&& (dam_type != DAM_ACID) 
	&& (dam_type != DAM_MENTAL) )
    {
    	if (is_affected( victim, gsn_fire_shield ) )
    	{
	    int fire_dam = dam;

	    if ( !IS_NPC(victim) )
	        fire_dam = dam * 2;

		if (IS_SET(ch->imm_flags, IMM_FIRE) )
		{
			send_to_char("`WYou are uninjured by your opponents shield of fire.\n\r", ch);
			send_to_char("`WYour opponent is uninjured by your shield of fire.\n\r", victim);
		}
		else if (IS_SET(ch->res_flags, RES_FIRE) && !IS_SET(ch->res_flags, VULN_FIRE))
		{	
	 		fire_damage(victim,ch,number_range(2, fire_dam / 2),gsn_fire_shield, DAM_FIRE);
		}
		else if (IS_SET(ch->vuln_flags, VULN_FIRE) && !IS_SET(ch->res_flags, RES_FIRE))
		{
			fire_damage(victim,ch,number_range(2, fire_dam * 2 ),gsn_fire_shield,DAM_FIRE); 
		}
		else
			fire_damage(victim,ch,number_range(2, fire_dam ),gsn_fire_shield,DAM_FIRE); 
	}
    }

    update_pos( victim );
    switch( victim->position )
    {
    case POS_MORTAL:
	cact( "$n is mortally wounded, and will die soon, if not aided.",
	    victim, NULL, NULL, TO_ROOM, "`r" );
	send_to_char( 
	    "You are mortally wounded, and will die soon, if not aided.\n\r",
	    victim );
	break;

    case POS_INCAP:
	cact( "$n is incapacitated and may die, if not aided.",
	    victim, NULL, NULL, TO_ROOM, "`R" );
	send_to_char(
	    "You are incapacitated and may die, if not aided.\n\r",
	    victim );
	break;

    case POS_STUNNED:
	cact( "$n is stunned, but will probably recover.",
	    victim, NULL, NULL, TO_ROOM, "`Y" );
	send_to_char("You are stunned, but will probably recover.\n\r",
	    victim );
	break;

    case POS_DEAD:
	/*  cact( "$n is DEAD!!", victim, 0, 0, TO_ROOM, "`R" );
	 *  moved farther down in the code for script handling
	 *  Galadrail.
         */
	send_to_char( "You have been KILLED!!\n\r\n\r", victim );
	break;

    default:
	if ( dam > victim->max_hit / 4 )
	    send_to_char( "That really did HURT!\n\r", victim );
	if ( victim->hit < victim->max_hit / 4 )
	    send_to_char( "You sure are BLEEDING!\n\r", victim );
	break;
    }

    /*
     * Sleep spells and extremely wounded folks.
     */
    if ( !IS_AWAKE(victim) )
	stop_fighting( victim, FALSE );

    /*
     * Payoff for killing things.
     */
    if ( victim->position == POS_DEAD )
    {
	if ( !IS_NPC(victim))
	{
	    if ( ch->group != NULL )
	    ch->group->pkills++;
	    ch->pkills ++;
	    check_top_ten( ch, ch->pkills, 0 );
	    if (LOGON_ON)
	    if (ch->logon_data != NULL)
	    ch->logon_data->pkills++;
	    trig = do_trigger( ch, ch, NULL, NULL, TRIG_KILLS_PLAYER, TRIG_TYPE_MOB);
	    trig = do_trigger( ch->in_room, ch, NULL, NULL, TRIG_KILLS_PLAYER, TRIG_TYPE_ROOM);
	}
	if ( IS_NPC(victim))
	{
	    if ( ch->group != NULL )
	    ch->group->mob_kills++;
	    ch->kills ++;
	    check_top_ten( ch, ch->kills,  1 );
	    if (LOGON_ON)
	    if (ch->logon_data != NULL)
	    ch->logon_data->kills++;
	    trig = do_trigger( ch, ch, NULL, NULL, TRIG_KILLS_MOB, TRIG_TYPE_MOB);
	    trig = do_trigger( ch->in_room, ch, NULL, NULL, TRIG_KILLS_MOB, TRIG_TYPE_ROOM);
	}


	trig = do_trigger( victim->in_room, ch, NULL, NULL, TRIG_DIES, TRIG_TYPE_ROOM);
        if ( HAS_SCRIPT(victim) )
	{
	TRIGGER_DATA *pTrig;
	
	for ( pTrig = IS_NPC(victim) ? victim->triggers : victim->triggers ; pTrig != NULL; pTrig = pTrig->next )
	    {
		if ( pTrig->trigger_type == TRIG_DIES
		&& pTrig->current == NULL  
		&& !IS_SET(victim->act2, ACT2_HALT ) )
           
		{
		pTrig->current = pTrig->script;

		/* 
		 * In order for the mob to be able to execute any commands
		 * from a script.  Mobs at this point still look to interpret
		 * to find the minimum position needed.
		 */
                victim->position = POS_STANDING; 

		act_trigger( victim, pTrig->name, NULL, IS_NPC(ch) ? ch->short_descr : ch->name, NULL );
		script_interpret(victim, pTrig );
		if (IS_SET(pTrig->bits, SCRIPT_ADVANCE) )
			pTrig->script = pTrig->script->next; 

		/*
		 * Now that the script is complete we can set them back
		 * to dead
		 */
                victim->position = POS_DEAD;

		}
	    }
	}

	  cact( "$n is DEAD!!", victim, 0, 0, TO_ROOM, "`R" );

	if ( victim->master != NULL && victim->master->mount == victim
	    && IS_AFFECTED( victim, AFF_MOUNTED ) )
	{
	    if ( number_percent( ) > get_skill( victim->master, ( IS_AFFECTED( victim, AFF_FLYING|AFF_LEVITATING ) ? gsn_riding_air : gsn_riding ) ) )
	    {
		send_to_char( "As your mount dies you are caught under it.\n\r",victim->master);
    		damage( victim, victim->master, roll_dice( 2 * victim->size + 1, 10 ) * ( IS_AFFECTED( victim, AFF_FLYING|AFF_LEVITATING) ? 4 : 1 ), 0, DAM_BASH );
	    }
	    else
		send_to_char("You manage to get out of the way of your dying mount.\n\r",victim->master);
	    victim->master->mount = NULL;
	    REMOVE_BIT(victim->master->affected_by, AFF_MOUNTED );
 	}
	
	if ( IS_NPC( victim ) )
            group_gain( ch, victim );

	if ( !IS_NPC(victim) )
	{

	    sprintf( log_buf, "%s killed by %s",
		victim->name,
		(IS_NPC(ch) ? ch->short_descr : ch->name) );
	    log_string( log_buf );
	    info_channel ( victim, log_buf , INFOACT_DEATH);
	    if ( IS_NPC( ch ) )
	    	victim->exp -= (victim->exp - exp_table[victim->level].exp_at_level) / 10;
	}

	if ( !IS_NPC( ch ) && !IS_NPC( victim ) )
	    raw_kill( victim, FALSE, TRUE );
	else
	    raw_kill( victim, FALSE, FALSE );

	if ( is_name( ch->hunting, victim->name ) )
	{
	    ch->hunting[0] = '\0';
	}

	strncpy(victim->dislikes, ch->name,30);
	for ( fch = char_list ; fch != NULL; fch = fch_next )
	{
	    fch_next = fch->next;
	    if ( is_name( fch->dislikes ,victim->name ) )
	    {
            	fch->dislikes[0] = '\0';
		
	    	if ( IS_NPC(fch) && ( fch->spec_fun == spec_thug || fch->spec_fun == spec_nightmare || fch->spec_fun == spec_assassin ) )
	    	{
		    extract_char( fch, TRUE );
		    return TRUE;
	    	}
	    }
	}
        /* RT new auto commands */

	if( ch->group && IS_NPC( victim ) && victim->in_room == ch->group->treasurer->in_room )
	{
	    do_get( ch->group->treasurer, "all corpse" );
	}
	else if ( !IS_NPC(ch) && IS_NPC(victim) )
	{
	   corpse = get_obj_list( ch, "corpse", ch->in_room->contents ); 

	    if ( corpse == NULL || corpse->cont_data == NULL )
		return TRUE;

	    if ( IS_SET(ch->plr, PLR_AUTOLOOT) && 
		corpse->cont_data->contents ) /* exists and not empty */
		do_get( ch, "all corpse" );

 	    if (IS_SET(ch->plr,PLR_AUTOGOLD)
		&& corpse->cont_data->contents  /* exists and not empty */
		&& !IS_SET(ch->plr,PLR_AUTOLOOT))
	      do_get(ch, "coins corpse");
            
	    if ( IS_SET(ch->plr, PLR_AUTOSAC) && IS_NPC(victim))
		do_sacrifice( ch, "corpse" );
	}

	return TRUE;
    }

    if ( victim == ch )
	return TRUE;

    /*
     * Take care of link dead people.
     */
    if ( !IS_NPC(victim) && victim->desc == NULL )
    {
	if ( number_range( 0, victim->wait ) == 0 )
	{
	    do_recall( victim, "" );
	    return TRUE;
	}
    }

    /*
     * Wimp out?
     */
    if ( IS_NPC(victim) && dam > 0 && victim->wait < PULSE_VIOLENCE / 2)
    {
	if ( ( IS_SET(victim->act, ACT_WIMPY) && number_bits( 2 ) == 0
	&&   victim->hit < victim->max_hit / 5) 
	||   ( IS_AFFECTED(victim, AFF_CONTROL_MIND) && victim->master != NULL
	&&     victim->master->in_room != victim->in_room ) )
	    do_flee( victim, "" );
    }

    if ( !IS_NPC(victim)
    &&   victim->hit > 0
    &&   victim->hit <= victim->wimpy
    &&   victim->wait < PULSE_VIOLENCE / 2 )
	do_flee( victim, "" );

    tail_chain( );
    return TRUE;
}

void check_top_ten( CHAR_DATA *ch, int number, int type )
{
    char name[17], temp_name[17];
    bool found = FALSE;
    int i,value,temp_int;

    if ( IS_NPC( ch ) )
	return;

    value = number;
    strncpy( name, ch->name, 16 );
    temp_name[0] = '\0';

/* see if they are already on the top ten, if so move all the ones bellow up */
    for ( i = 0 ; i < 10 && !is_name( name, top_ten_list[ i ][ type ].name ) ; i++ );
    for ( ; i < 9 ; i++ )
	{
	    top_ten_list[ i ][ type ].value = top_ten_list[ i+1 ][ type ].value;
   	    strncpy( top_ten_list[ i ][ type ].name, top_ten_list[ i+1 ][ type ].name, 30 );
	}
/* insert the name and value into the right place on the list. */
    for ( i = 0 ; i < 10 ; i++ )
    { 
	if ( value >= top_ten_list[ i ][ type ].value )
	{  
	    if ( i == 0 )
	    {
		top_ten_list[ i ][ type ].saying[0] = '\0';
	    }
	    found = TRUE;
	    temp_int = top_ten_list[ i ][ type ].value;	    
	    top_ten_list[ i ][ type ].value = value;
	    value = temp_int;
	    strncpy( temp_name, top_ten_list[ i ][ type ].name, 16 );
	    strncpy( top_ten_list[ i ][ type ].name, name, 30 );
	    strncpy( name, temp_name, 16 );
  	}
    }

    if ( found )
	save_top_ten( );
}

bool is_safe(CHAR_DATA *ch, CHAR_DATA *victim )
{
    /* no killing in shops hack */
    if ( IS_NPC( victim ) && victim->pIndexData->pShop != NULL )
    {
	send_to_char( "The shopkeeper wouldn't like that.\n\r", ch ); 
        return TRUE;
    }

    /* no killing healers, adepts, etc */
    if ( IS_NPC( victim ) 
    && ( IS_SET( victim->act, ACT_TRAIN )
    ||   IS_SET( victim->act, ACT_IS_HEALER ) ) )
    {
	send_to_char( "I don't think the gods would approve.\n\r", ch );
	return TRUE;
    }

    /* no fighting in safe rooms */
    if ( IS_SET( ch->in_room->room_flags, ROOM_SAFE ) )
    {
	send_to_char( "Not in this room.\n\r", ch );
	return TRUE;
    }

/* crashed here....what if victim->fighting == NULL...can this happen? */
    if ( victim->fighting == ch )
	return FALSE;

    if (IS_NPC(ch))
    {
 	/* controlled mobs and pets cannot attack players */
	if (!IS_NPC(victim) && (IS_AFFECTED(ch,AFF_CONTROL_MIND) 
	||  IS_SET(ch->act,ACT_PET) || IS_SET(ch->act,ACT_MOUNT)))
	    return TRUE;

      	return FALSE;
     }
     else /* Not NPC */
     {	
	if (IS_IMMORTAL(ch))
	    return FALSE;

	/* no pets */
	if (IS_NPC(victim) && ( IS_SET(victim->act,ACT_PET) || IS_SET( victim->act,ACT_MOUNT)))
	{
            act("But $N looks so cute and cuddly...",ch,NULL,victim,TO_CHAR);
            return TRUE;
	}

	/* no controlled mobs unless char is the the owner */
	if (IS_AFFECTED(victim,AFF_CONTROL_MIND) && ch != victim->master)
	{
            send_to_char("You don't own that monster.\n\r",ch);
	    return TRUE;
	}

        if (!IS_NPC(victim) 
           && IS_SET(victim->plr,PLR_THIEF) )
            return FALSE; /*Lets Kill those undesireable ones*/

	if ( !IS_NPC( victim ) && ( victim->level <= 15 || IS_SET( ch->in_room->area->area_flags, AREA_PKSAFE ) ) )
	{ 
	    send_to_char( "You can't kill them here.\n\r", ch );
	    return TRUE;    
	}

	//if ( !IS_NPC( victim ) && !IS_NPC( ch ) )
	//{
	 //   send_to_char("Player killing disabled due to instability.\n\r", ch );
	  //  return TRUE;
	//}

	/* if both players are set PLR_PK, then PK OK */
	if ( !IS_NPC( victim ) && !IS_NPC( ch ) && IS_SET( victim->plr, PLR_PK )
	     && IS_SET( ch->plr, PLR_PK ) && ch->group == NULL && victim->group == NULL )
	    return FALSE;
	

        /* no player killing if not in a clan */
	if ( !IS_NPC( victim ) && !IS_NPC( ch ) )
	{
	    if ( IS_SET( ch->in_room->area->area_flags, AREA_PKSAFE ) )
		return TRUE;
	    else if ( ch->pcdata->pcclan == NULL )
		return TRUE;
	    else if ( victim->pcdata->pcclan == NULL )
		return TRUE;
	    else if ( ch->pcdata->pcclan->clan->name == victim->pcdata->pcclan->clan->name )
            	return TRUE;      
   	}

	return FALSE;
    }
}

bool is_safe_spell(CHAR_DATA *ch, CHAR_DATA *victim, bool area ) /*DONE*/
{
    /* can't zap self (crash bug) */
    if (ch == victim)
	return TRUE;

    /* immortals not hurt in area attacks */
    if (IS_IMMORTAL(victim) &&  area)
	return TRUE;

    if (IS_SET(victim->act2, ACT2_MOVEABLE_AREA ) )
	return TRUE;

    /* no killing in shops hack */
    if (IS_NPC(victim) && victim->pIndexData->pShop != NULL)
        return TRUE;

    /* no killing healers, adepts, etc */
    if (IS_NPC(victim)
    && (IS_SET(victim->act,ACT_TRAIN)
    ||  IS_SET(victim->act,ACT_IS_HEALER)))
	return TRUE;

    /* no fighting in safe rooms */
    if (IS_SET(ch->in_room->room_flags,ROOM_SAFE))
        return TRUE;

    if (victim->fighting == ch)
	return FALSE;

    /*Make Shure we can still those undesireable ones*/
    if ( !IS_NPC(ch) && IS_SET(victim->plr,PLR_THIEF) )
       return FALSE; 

    if (IS_NPC(ch))
    {
        /* controlled mobs and pets cannot attack players */
        if (!IS_NPC(victim) && (IS_AFFECTED(ch,AFF_CONTROL_MIND)
            || IS_SET(ch->act,ACT_PET) || IS_SET(ch->act,ACT_MOUNT)))
            return TRUE;
	
	/* area affects don't hit other mobiles */
        if (IS_NPC(victim) && area)
            return TRUE;
 
        return FALSE;
    }
    else /* Not NPC */
    {
        if (IS_IMMORTAL(ch) && !area)
            return FALSE;
 
        /* no pets */
        if (IS_NPC(victim) && ( IS_SET(victim->act,ACT_PET) || IS_SET(victim->act,ACT_MOUNT)))
            return TRUE;
 
        /* no controlled mobs unless char is the the owner */
        if (IS_AFFECTED(victim,AFF_CONTROL_MIND) && ch != victim->master)
            return TRUE;
     
	/* if both players are set PLR_PK, then PK OK */
	if ( !IS_NPC( victim ) && !IS_NPC( ch ) && IS_SET( victim->plr, PLR_PK )
	     && IS_SET( ch->plr, PLR_PK ) && ch->group == NULL && victim->group == NULL )
	    return FALSE;

        /* no player killing if not in a clan */
	if ( !IS_NPC( victim ) && !IS_NPC( ch ) )
	{
	    if ( IS_SET( ch->in_room->area->area_flags, AREA_PKSAFE ) )
		return TRUE;
	    else if ( ch->pcdata->pcclan == NULL )
		return TRUE;
	    else if ( victim->pcdata->pcclan == NULL )
		return TRUE;
	    else if ( ch->pcdata->pcclan->clan->name == victim->pcdata->pcclan->clan->name )
            	return TRUE;      
   	}

	/* cannot use spells if not in same group */
	if ( victim->fighting != NULL && !is_same_group( ch,victim->fighting ) )
	    return TRUE;
 
        return FALSE;
    }
}

/*
 * Check for parry.
 */
bool check_parry( CHAR_DATA *ch, CHAR_DATA *victim )
{
    CHAR_DATA *rch;
    char buf[MAX_INPUT_LENGTH];
    int chance;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( victim->fighting != ch )  /* can only parry the person they are */
	return FALSE;		   /* directly fighting */

    if ( get_eq_char( victim, WEAR_WIELD_R ) == NULL
    && get_eq_char( victim, WEAR_WIELD_L ) == NULL
    && get_eq_char( victim, WEAR_WIELD_TWO_HANDED ) == NULL )
	return FALSE;

    chance = get_skill( victim, gsn_parry ) / 2;

    if ( number_percent( ) >= chance + victim->level - ch->level )
	return FALSE;

    if ( !IS_SET( victim->plr2, PLR_NOMISS ) )
        cact( "You parry $n's attack.",  ch, NULL, victim, TO_VICT, "`G"    );
    if ( !IS_SET( ch->plr2, PLR_NOMISS ) )
        cact( "$N parries your attack.", ch, NULL, victim, TO_CHAR, "`G"    );

    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
    {
	if ( rch == ch || rch == victim )
	     continue;

        sprintf( buf, "%s parries $N's attack.", PERS( victim, rch ) );

	if ( !IS_SET( rch->plr2, PLR_NOMISS ) )
    	     cact( buf, rch, NULL, ch, TO_CHAR, "`G"    );
    }

    check_improve(victim,gsn_parry,TRUE,6);

    if ( get_skill( victim, gsn_counter_strike ) > 1 )
    {
	chance = get_skill( victim, gsn_counter_strike ) / 2;

	if ( number_percent( ) <= chance + victim->level - ch->level )
	{
        	damage(victim,ch,number_range(2, 2 * victim->level + chance / 2), gsn_counter_strike, DAM_BASH);
    	    	check_improve(victim, gsn_counter_strike, TRUE, 4);		
	}
	else
		check_improve(victim, gsn_counter_strike, FALSE, 4);
    }

    return TRUE;
}


/*
 * Check for block.
 */
bool check_block( CHAR_DATA *ch, CHAR_DATA *victim )
{
    CHAR_DATA *rch;
    char buf[MAX_INPUT_LENGTH];
    int chance;
    OBJ_DATA *shield;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    if ( (shield = get_eq_char( victim, WEAR_SHIELD ) ) == NULL )
        return FALSE;

    chance = get_skill( victim, gsn_shield_block ) / 2;

    if ( IS_SET( shield->extra_flags, ITEM_BUCKLER ) )
	chance -= 20;

    if ( number_percent( ) >= chance + victim->level - ch->level )
	return FALSE;

    if ( !IS_SET( victim->plr2, PLR_NOMISS ) )
        cact( "You block $n's `Gattack with $p`G.",  ch, shield, victim, TO_VICT, "`G"    );
    if ( !IS_SET( ch->plr2, PLR_NOMISS ) )
        cact( "$N `Gblocks your attack with $p`G.", ch, shield, victim, TO_CHAR, "`G"    );

    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
    {
	if ( rch == ch || rch == victim )
	     continue;

        sprintf( buf, "%s `Gblocks $N's `Gattack with $p`G.", PERS( victim, rch ) );

	if ( !IS_SET( rch->plr2, PLR_NOMISS ) )
    	     cact( buf, rch, shield, ch, TO_CHAR, "`G"    );
    }
    check_improve(victim,gsn_shield_block,TRUE,2);
/* thats right, i said leave it out
    if ( number_percent( ) <= (IS_OBJ_STAT(shield,ITEM_MAGIC)?1:3) )
    {
        cact( "Your $p `Gshatters from the mighty blow.",  ch, shield, victim, TO_VICT, "`G"    );
        cact( "$N's $p `Gshatters from the mighty blow.",  ch, shield, victim, TO_NOTVICT, "`G"    );
	extract_obj(shield);
    }
*/
    return TRUE;
}


/*
 * Check for dodge.
 */
bool check_dodge( CHAR_DATA *ch, CHAR_DATA *victim )
{
    CHAR_DATA *rch;
    char buf[MAX_INPUT_LENGTH];
    int chance;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    chance = get_skill( victim, gsn_dodge ) / 2;

    if ( victim->fighting != ch )
	chance -= 30;

    if ( chance <= 0 || number_percent( ) >= chance + victim->level - ch->level )
        return FALSE;

    if ( !IS_SET( victim->plr2, PLR_NOMISS ) )
        cact( "You dodge $n's attack.", ch, NULL, victim, TO_VICT, "`G"    );
    if ( !IS_SET( ch->plr2, PLR_NOMISS ) )
        cact( "$N dodges your attack.", ch, NULL, victim, TO_CHAR, "`G"    );

    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
    {
	if ( rch == ch || rch == victim )
	     continue;

        sprintf( buf, "%s dodges $N's attack.", PERS( victim, rch ) );
	

	if ( !IS_SET( rch->plr2, PLR_NOMISS ) )
    	     cact( buf, rch, NULL, ch, TO_CHAR, "`G"    );
    }

    check_improve(victim,gsn_dodge,TRUE,6);
    return TRUE;
}

/* check for wind over sand */

bool check_wind_over_sand( CHAR_DATA *ch, CHAR_DATA *victim )
{
    CHAR_DATA *rch;
    char buf[MAX_INPUT_LENGTH];
    int chance;

    if ( !IS_AWAKE(victim) )
	return FALSE;

    chance = get_skill( victim, gsn_wind_over_sand ) / 2;

    if ( chance <= 0 )
	return FALSE;

    if ( victim->fighting != ch )
	chance -= 20;

    if ( chance <= 0 || number_percent( ) >= chance + victim->level - ch->level )
        return FALSE;

    if ( !IS_SET( victim->plr2, PLR_NOMISS ) )
        cact( "You flow around $n's attack.", ch, NULL, victim, TO_VICT, "`G"    );
    if ( !IS_SET( ch->plr2, PLR_NOMISS ) )
        cact( "$N flows around your attack.", ch, NULL, victim, TO_CHAR, "`G"    );

    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
    {
	if ( rch == ch || rch == victim )
	     continue;

        sprintf( buf, "%s flows around $N's attack.", PERS( victim, rch ) );

	if ( !IS_SET( rch->plr2, PLR_NOMISS ) )
    	     cact( buf, rch, NULL, ch, TO_CHAR, "`G"    );
    }

    check_improve(victim,gsn_wind_over_sand,TRUE,6);
    return TRUE;
}



/*
 * Set position of a victim.
 */
void update_pos( CHAR_DATA *victim )
{
    if ( victim->hit > 0 )
    {
    	if ( victim->position <= POS_STUNNED )
	    set_position( victim, POS_STANDING );
	return;
    }

    if ( IS_NPC(victim) && victim->hit < 1 )
    {
	set_position( victim, POS_DEAD );
	return;
    }

    if ( victim->hit <= -5 )
    {
	set_position( victim, POS_DEAD );
	return;
    }

    if ( victim->hit <= -3 ) set_position( victim, POS_INCAP );
    else    set_position( victim, POS_STUNNED );

    return;
}



/*
 * Start fights.
 */
void set_fighting( CHAR_DATA *ch, CHAR_DATA *victim )
{
    if ( ch->fighting != NULL )
    {
	bug( "Set_fighting: already fighting", 0 );
	return;
    }
    
    if ( IS_AFFECTED(ch, AFF_SLEEP) )
	affect_strip( ch, gsn_sleep );

    ch->fighting = victim;
    strncpy(ch->dislikes, victim->name,30);
    set_position( ch, POS_FIGHTING );
    ch->happy_hour = 0;

    return;
}



/*
 * Stop fights.
 */
void stop_fighting( CHAR_DATA *ch, bool fBoth )
{
    CHAR_DATA *fch;

    for ( fch = char_list; fch != NULL; fch = fch->next )
    {
	if ( fch == ch || ( fBoth && fch->fighting == ch ) )
	{
	    switch (fch->specattack) {
                case 1 : fch->specattack = 0; break; 
                default : fch->specattack = fch->specattack; 
            }
            fch->fighting	= NULL;
	    set_position( fch, IS_NPC(fch) ? ch->default_pos : POS_STANDING );
	    update_pos( fch );
	    REMOVE_BIT(fch->affected_by_2, AFF_STUN);
	}
    }

    return;
}

/*
 * Make a corpse out of a character.
 */
void make_corpse( CHAR_DATA *ch, bool empty )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *corpse;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;
    char name[81];

    if ( IS_NPC(ch) )
    {
	if ( IS_SET( ch->form, FORM_INSTANT_DECAY ) )
	    return;

	strcpy( name, ch->short_descr);
	corpse = create_object( get_obj_index( OBJ_VNUM_CORPSE_NPC ) );
	corpse->timer	= number_range( 3, 6 );
	if ( ch->gold[0] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[0] , 0 ), corpse );
	    ch->gold[0] = 0;
	}

	if ( ch->gold[1] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[1] , 1 ), corpse );
	    ch->gold[1] = 0;
	}

	if ( ch->gold[2] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[2] , 2 ), corpse );
	    ch->gold[2] = 0;
	}

	if ( ch->gold[3] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[3] , 3 ), corpse );
	    ch->gold[3] = 0;
	}
	if ( corpse->wear_data != NULL )
	    corpse->wear_data->cost = 0;
    }
    else
    {
	strcpy( name, ch->name );
	corpse = create_object( get_obj_index( OBJ_VNUM_CORPSE_PC ) );
	strncpy( corpse->owner, ch->name, 30 );
	if ( ch->gold[0] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[0] , 0 ), corpse );
	    ch->gold[0] = 0;
	}

	if ( ch->gold[1] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[1] , 1 ), corpse );
	    ch->gold[1] = 0;
	}

	if ( ch->gold[2] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[2] , 2 ), corpse );
	    ch->gold[2] = 0;
	}

	if ( ch->gold[3] > 0 && !empty )
	{
	    obj_to_obj( create_money( ch->gold[3] , 3 ), corpse );
	    ch->gold[3] = 0;
	}

	if ( corpse->wear_data != NULL )
	    corpse->wear_data->cost = 0;
    }

    sprintf( buf, corpse->short_descr, name );
    strncpy( corpse->short_descr, buf, 80 );
    sprintf( buf, corpse->description, name );
    strncpy( corpse->description, buf, 80 );

    if ( !empty )
        for ( obj = ch->carrying; obj != NULL; obj = obj_next )
        {
	    obj_next = obj->next_content;
	    if ( !IS_SET( obj->extra_flags, ITEM_STAY_ON_DEATH ) )
	    {
	    	obj_from_char( obj, "N" );
	    	if (IS_SET(obj->extra_flags,ITEM_ROT_DEATH))
	            obj->timer = number_range(5,10);

	     	REMOVE_BIT(obj->extra_flags,ITEM_VIS_DEATH);
	    	REMOVE_BIT(obj->extra_flags,ITEM_ROT_DEATH);
	        obj_to_obj( obj, corpse );
	    }
	    else
	    	continue;
        }

    obj_to_room( corpse, ch->in_room );
    if ( !IS_NPC( ch ) )
	ch->pcdata->corpse = corpse;	

    return;
}

void raw_kill( CHAR_DATA *victim, bool empty, bool pk )
{
    int i;
    AFFECT_DATA af;
    AFFECT_DATA *paf;
    AFFECT_DATA *paf_next;

    stop_fighting( victim, TRUE );
    if ( pk == FALSE )
        make_corpse( victim, empty );

    if ( !IS_NPC( victim ) )
    {
	victim->died++;
	check_top_ten( victim, victim->died, 2 );
    }
    else
    {
	victim->pIndexData->killed++;
	kill_table[URANGE(0, victim->level, MAX_LEVEL-1)].killed++;
	extract_char( victim, TRUE );
	return;
    }

    set_position( victim, POS_RESTING );
    victim->hit		= victim->pcdata->perm_hit;
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
        victim->mana[i]	=  victim->pcdata->perm_mana[i];
    victim->move	=  victim->pcdata->perm_move;
    victim->pcdata->condition[ COND_FULL ] = race_table[ victim->race ][ victim->sex ].max_condition[ COND_FULL ];
    victim->pcdata->condition[ COND_THIRST ] = race_table[ victim->race ][ victim->sex ].max_condition[ COND_THIRST ];

    if ( empty )
	return;

    extract_char( victim, FALSE );
    for ( paf = victim->affected; paf != NULL; paf = paf_next)
    {
	paf_next = paf->next;
	affect_remove( victim, paf );
    }
    victim->affected_by	= victim->pcdata->perm_aff;
    if ( !empty )
     	    victim->armor= 100;

    if ( victim->level >= 10 && pk == FALSE )
    {
	af.type = -2;
	af.level = 60;
	af.duration = -1;
	af.bitvector = 0;

	af.location = APPLY_STR;
	af.modifier = -2 - ( victim->level / 10 );
	affect_to_char( victim, &af );

	af.location = APPLY_DEX;
	af.modifier = -2 - ( victim->level / 10 );
	affect_to_char( victim, &af );

	af.location = APPLY_INT;
	af.modifier = -2 - ( victim->level / 10 );
	affect_to_char( victim, &af );

	af.location = APPLY_WIS;
	af.modifier = -2 - ( victim->level / 10 );
	affect_to_char( victim, &af );

	af.location = APPLY_CON;
	af.modifier = -2 - ( victim->level / 10 );
	affect_to_char( victim, &af );

	af.location = APPLY_CHR;
	af.modifier = -2 - ( victim->level / 10 );
	affect_to_char( victim, &af );

	af.location = APPLY_HIT;
	af.modifier = 0 - ( victim->max_hit / 4 );
	affect_to_char( victim, &af );

	af.location = APPLY_HITROLL;
	af.modifier = 0 - ( victim->level / 4 );
	affect_to_char( victim, &af );

	af.location = APPLY_DAMROLL;
	af.modifier = 0 - ( victim->level / 4 );
	affect_to_char( victim, &af );

	af.location = APPLY_MANA_FIRE;
	af.modifier = 0 - ( victim->mana[ ELEMENT_FIRE ] / 2 );
	affect_to_char( victim, &af );

	af.location = APPLY_MANA_WATER;
	af.modifier = 0 - ( victim->mana[ ELEMENT_WATER ] / 2 );
	affect_to_char( victim, &af );

	af.location = APPLY_MANA_EARTH;
	af.modifier = 0 - ( victim->mana[ ELEMENT_EARTH ] / 2 );
	affect_to_char( victim, &af );

	af.location = APPLY_MANA_WIND;
	af.modifier = 0 - ( victim->mana[ ELEMENT_WIND ] / 2 );
	affect_to_char( victim, &af );

	af.location = APPLY_MANA_SPIRIT;
	af.modifier = 0 - ( victim->mana[ ELEMENT_SPIRIT ] / 2 );
	affect_to_char( victim, &af );
    }

    /* RT added to prevent infinite deaths */
    REMOVE_BIT(victim->plr, PLR_THIEF);
    REMOVE_BIT(victim->plr, PLR_BOUGHT_PET);
    return;
}

void group_gain( CHAR_DATA *ch, CHAR_DATA *victim )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *gch;
    CHAR_DATA *lch;
    int xp;
    int members;
    int group_levels;

    /*
     * Monsters don't get kill xp's.
     * P-killing doesn't help either.
     * Dying of mortal wounds or poison doesn't give xp to anyone!
     */
    if ( IS_NPC(ch) || victim == ch || victim->spec_fun == spec_thug )
	return;
    
    members = 0;
    group_levels = 0;
    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	if ( is_same_group( gch, ch ) )
        {
	    members++;
	    group_levels += gch->level;
	}
    }

    if (ch->group != NULL )
    	if ( members == 0 )
    	{
	    bug( "Group_gain: members.", members );
	    members = 1;
	    group_levels = ch->level ;
    	}

    lch = (ch->leader != NULL) ? ch->leader : ch;

    for ( gch = ch->in_room->people; gch != NULL; gch = gch->next_in_room )
    {
	OBJ_DATA *obj;
	OBJ_DATA *obj_next;

	if ( !is_same_group( gch, ch ) || IS_NPC(gch))
	    continue;

	xp = xp_compute( gch, victim, group_levels );  
	sprintf( buf, "`MYou receive %d experience points.\n\r", xp );
	send_to_char( buf, gch );
	gain_exp( gch, xp );
	if (LOGON_ON)
	    if (ch->logon_data != NULL)
		ch->logon_data->exp += xp;

	send_to_char("`w",ch);

	for ( obj = ch->carrying; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if ( obj->wear_data != NULL && obj->wear_data->wear_loc == WEAR_NONE )
		continue;

	    if ( ( IS_OBJ_STAT(obj, ITEM_ANTI_NENKEMEN)  && race_table[ ch->race ][ch->sex].world == 0 )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_MAEGMENEL)  && race_table[ ch->race ][ ch->sex ].world == 1 )
	    ||   ( IS_OBJ_STAT(obj, ITEM_ANTI_LITHDOR)  && race_table[ ch->race ][ ch->sex ].world == 2 ) )
	    {
		cact( "You are zapped by $p.", ch, obj, NULL, TO_CHAR, "`W" );
		cact( "$n is zapped by $p.",   ch, obj, NULL, TO_ROOM, "`W" );
		obj_from_char( obj, "O" );
		obj_to_room( obj, ch->in_room );
	    }
	}
    }

    return;
}



/*
 * Compute xp for a kill.
 * Edit this function to change xp computations.
 */
int xp_compute( CHAR_DATA *gch, CHAR_DATA *victim, int total_levels )
{
    int xp,base_exp;

    base_exp = exp_table[victim->level].exp_for_mob;
    xp = base_exp;

    /* adjust for sentient skilled mobs */
    if ( IS_NPC( victim ) )
    	if ( ( victim->innate_skills != NULL || victim->skills != NULL || victim->pIndexData->prac_skills > 0 || victim->pIndexData->prac_spells > 0 )  
	   && IS_SET( victim->form, FORM_SENTIENT ) )
	{
	    if ( victim->pIndexData->prac_skills + victim->pIndexData->prac_spells >= 200 && victim->pIndexData->guilds != 0 ) 
		xp = number_range( xp * 3/2, xp * 5/2 );
	    else
		xp = number_range( xp * 3/2, xp * 2 );
	}

    /* randomize the rewards */
    xp = number_range( xp * 3/4, xp * 5/4 );

    /* adjust for grouping */
    xp = xp / total_levels * gch->level;

    if ( total_levels > 1 )
    	xp = xp * 8 / 7;

    if ( xp > exp_table[ gch->level + 1 ].exp_at_level - exp_table[ gch->level ].exp_at_level )
	xp = exp_table[ gch->level + 1 ].exp_at_level - exp_table[ gch->level ].exp_at_level;

    if (IS_SET(gch->plr2, PLR_FASTEXP ) )
	xp = xp * 10;

    return xp;
}

void dam_message( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt,int dam_type, bool immune )
{
    CHAR_DATA *rch;
    char buf[256],buf1[256], buf2[256], buf3[256];
    char prefix[256];
    const char *vs;
    const char *vp;
    const char *attack;
    char punct;

	 if ( dam ==   0 ) { vs = "miss";			vp = "misses";			}
    else if ( dam <=   8 ) { vs = "scratch";			vp = "scratches";		}
    else if ( dam <=  12 ) { vs = "graze";			vp = "grazes";			}
    else if ( dam <=  16 ) { vs = "hit";			vp = "hits";			}
    else if ( dam <=  20 ) { vs = "injure";			vp = "injures";			}
    else if ( dam <=  24 ) { vs = "wound";			vp = "wounds";			}
    else if ( dam <=  28 ) { vs = "maul";       		vp = "mauls";			}
    else if ( dam <=  32 ) { vs = "decimate";			vp = "decimates";		}
    else if ( dam <=  36 ) { vs = "devastate";			vp = "devastates";		}
    else if ( dam <=  40 ) { vs = "maim";			vp = "maims";			}
    else if ( dam <=  44 ) { vs = "MUTILATE";			vp = "MUTILATES";		}
    else if ( dam <=  48 ) { vs = "DISEMBOWEL";			vp = "DISEMBOWELS";		}
    else if ( dam <=  52 ) { vs = "DISMEMBER";			vp = "DISMEMBERS";		}
    else if ( dam <=  56 ) { vs = "MASSACRE";			vp = "MASSACRES";		}
    else if ( dam <=  60 ) { vs = "MANGLE";		     	vp = "MANGLES";			}
    else if ( dam <=  75 ) { vs = "*** DEMOLISH ***";	     	vp = "*** DEMOLISHES ***";	}
    else if ( dam <=  90 ) { vs = "*** DEVASTATE ***";	     	vp = "*** DEVASTATES ***";	}
    else if ( dam <= 105 ) { vs = "=== OBLITERATE ===";	     	vp = "=== OBLITERATES ===";	}
    else if ( dam <= 120 ) { vs = ">>> ANNIHILATE <<<";	     	vp = ">>> ANNIHILATES <<<";	}
    else if ( dam <= 135 ) { vs = "<<< ERADICATE >>>";	     	vp = "<<< ERADICATES >>>";	}
    else if ( dam <= 160 ) { vs = "<> LAPIDATE <>";	     	vp = "<> LAPIDATES <>";		}
    else if ( dam <= 185 ) { vs = "<><> TRUCIDATE <><>";     	vp = "<><> TRUCIDATES <><>";	}
    else if ( dam <= 200 ) { vs = "<><><> SLAUGHTER <><><>"; 	vp = "<><><> SLAUGHTERS <><><>";}
    else if ( dam <= 220 ) { vs = "<>=<> GORE <>=<>";	     	vp = "<>=<> GORES <>=<>";	}
    else if ( dam <= 240 ) { vs = "ooOO SPLATTER OOoo";	     	vp = "ooOO SPLATTERS OOoo";	}
    else if ( dam <= 260 ) { vs = "*=*=* BUTCHER *=*=*";     	vp = "*=*=* BUTCHERS *=*=*";	}
    else if ( dam <= 280 ) { vs = "<(*)> EVISCERATE <(*)>";  	vp = "<(*)> EVISCERATES <(*)>";	}
    else if ( dam <= 300 ) { vs = "!=! SANGUINATE !=!";	     	vp = "!=! SANGUINATES !=!";	}
    else if ( dam <= 320 ) { vs = "!!**!! IMMOLATE !!**!!";  	vp = "!!**!! IMMOLATES !!**!!";	}
    else if ( dam <= 350 ) { vs = "!!!! JUGULATE !!!!";	     	vp = "!!!! JUGULATES !!!!";	}
    else                   { vs = "do UNSPEAKABLE things to";	vp = "does UNSPEAKABLE things to";}

    punct   = (dam <= 36) ? '.' : '!';

    if ( dt == TYPE_HIT )
    {
	if (ch  == victim)
	{
	    sprintf( buf1, "%%s %s $melf%c",vp,punct);
	    sprintf( buf2, "You %s yourself%c",vs,punct);
	}
	else
	{
	    sprintf( buf1, "%%s %s $N%c",  vp, punct );
	    sprintf( buf2, "You %s $N%c", vs, punct );
	    sprintf( buf3, "$n %s you%c", vp, punct );
	}
    }
    else
    {
	if ( dt >= 0 && dt < MAX_SKILL )
	{
	    attack = skill_table[dt].noun_damage;
	}
	else if ( dt >= TYPE_HIT
	&& dt <= TYPE_HIT + MAX_DAMAGE_MESSAGE )
	{
	    prefix[0] = '\0';
	    if ( dam_type == DAM_EARTH )
		strcat( prefix, "`Gdirty`R " );
	    else if ( dam_type == DAM_FIRE )
		strcat( prefix, "`rflaming`R " );
	    else if ( dam_type == DAM_WATER )
		strcat( prefix, "`Cfreezing`R " );
	    else if ( dam_type == DAM_POISON )
		strcat( prefix, "`gpoisoned`R " );
	    else if ( dam_type == DAM_ENERGY )
		strcat( prefix, "`Cshocking`R " );
	    else if ( dam_type == DAM_ACID )
		strcat( prefix, "`Gacidic`R " );
	    else if ( dam_type == DAM_SPIRIT )
		strcat( prefix, "`Wdraining`R " );
	    else if ( dam_type == DAM_VORPAL )
		strcat( prefix, "`Yvorpal`R " );
	    strcat( prefix, attack_table[dt - TYPE_HIT].name );
	    attack	= prefix;
	}
	else
	{
	    bug( "Dam_message: bad dt %d.", dt );
	    dt  = TYPE_HIT;
	    attack  = attack_table[0].name;
	}

	if (immune)
	{
	    if (ch == victim)
	    {
		sprintf(buf1,"%%s is unaffected by $s own %s.",attack);
		sprintf(buf2,"Luckily, you are immune to that.");
	    } 
	    else
	    {
	    	sprintf(buf1,"%%s is unaffected by $N's %s!", attack);
	    	sprintf(buf2,"$N is unaffected by your %s!",attack);
	    	sprintf(buf3,"$n's %s is powerless against you.",attack);
	    }
	}
	else
	{
	    if (ch == victim)
	    {
		sprintf( buf1, "%%s's %s %s $m%c",attack,vp,punct);
		sprintf( buf2, "Your %s %s you%c",attack,vp,punct);
	    }
	    else
	    {
	    	sprintf( buf1, "%%s's %s %s $N%c", attack, vp, punct );
	    	sprintf( buf2, "Your %s %s $N%c",  attack, vp, punct );
	    	sprintf( buf3, "$n's %s %s you%c", attack, vp, punct );
	    }
	}
    }

    if (ch == victim)
    {
	if ( dam > 1 || !IS_SET( ch->plr2, PLR_NOMISS ) )
	     cact(buf2,ch,NULL,NULL,TO_CHAR, "`R");

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	     if ( rch == ch )
		continue;

	     sprintf( buf, buf1, PERS( ch, rch ) );

	     if ( dam > 1 || !IS_SET( rch->plr2, PLR_NOMISS ) )
	          cact(buf,rch,NULL,ch,TO_CHAR, "`R");
	}
    }
    else
    {
	if ( dam > 1 || !IS_SET( ch->plr2, PLR_NOMISS ) )
    	cact( buf2, ch, NULL, victim, TO_CHAR, "`R" );
	if ( dam > 1 || !IS_SET( victim->plr2, PLR_NOMISS ) )
    	cact( buf3, ch, NULL, victim, TO_VICT, "`R" );

	for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	     if ( rch == ch || rch == victim )
		continue;

	     sprintf( buf, buf1, PERS( ch, rch ) );

	     if ( dam > 1 || !IS_SET( rch->plr2, PLR_NOMISS ) )
	          cact(buf,rch,NULL,victim,TO_CHAR, "`R");
	}
    }

    return;
}



/*
 * Disarm a creature.
 * Caller must check for successful attack.
 */
void disarm( CHAR_DATA *ch, CHAR_DATA *victim )
{
    OBJ_DATA *obj;

    if ( ( obj = get_eq_char( victim, WEAR_WIELD ) ) == NULL )
        if ( ( obj = get_eq_char( victim, WEAR_WIELD_TWO_HANDED ) ) == NULL )
	    return;

    if ( IS_OBJ_STAT(obj,ITEM_NOREMOVE))
    {
	cact("$S weapon won't budge!",ch,NULL,victim,TO_CHAR, "`C");
	cact("$n tries to disarm you, but your weapon won't budge!",
	    ch,NULL,victim,TO_VICT, "`C");
	cact("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT, "`C");
	return;
    }

    cact( "$n disarms you and sends your weapon flying!", 
	 ch, NULL, victim, TO_VICT, "`M"    );
    cact( "You disarm $N!",  ch, NULL, victim, TO_CHAR, "`M"    );
    cact( "$n disarms $N!",  ch, NULL, victim, TO_NOTVICT, "`M" );

    obj_from_char( obj, "P" );
    if ( IS_OBJ_STAT(obj,ITEM_NODROP) )
	obj_to_char( obj, victim );
    else
    {
	obj_to_room( obj, victim->in_room );
	if (IS_NPC(victim) && victim->wait == 0 && can_see_obj(victim,obj))
	    get_obj(victim,obj,NULL);
    }

    return;
}

void do_berserk( CHAR_DATA *ch, char *argument)
{
    int chance, hp_percent, skill;

    if ((skill = get_skill(ch,gsn_berserk)) <= 0 )
    {
	send_to_char("You turn red in the face, but nothing happens.\n\r",ch);
	return;
    }

    if ( ch->fighting == NULL || ch->position != POS_FIGHTING)
    {
	send_to_char( "You are not fighting.\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_BERSERK) )
    {
	send_to_char("You get a little madder.\n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CALM_SPIRIT))
    {
	send_to_char("You're feeling too mellow to berserk.\n\r",ch);
	return;
    }

    chance = skill;

    hp_percent = 100 * ch->hit/ch->max_hit;
    chance += 25 - hp_percent/2;

    if (number_percent() < chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->move /= 2;

	ch->hit += ch->level * 2;
	ch->hit = UMIN(ch->hit,ch->max_hit);

	send_to_char("Your pulse races as you are consumned by rage!\n\r",ch);
	cact("$n gets a wild look in $s eyes.",ch,NULL,NULL,TO_ROOM, "`R");
	check_improve(ch,gsn_berserk,TRUE,2);

	af.type		= gsn_berserk;
	af.level	= skill / 2;
	af.duration	= UMIN( 1, skill / 10 );
	af.modifier	= UMAX(1, skill / 8);
	af.location	= APPLY_AFFECT;
	af.bitvector 	= AFF_BERSERK;
	af.bit_type	= BIT_AFFECT;
	affect_to_char(ch,&af);
	af.bitvector 	= 0;
	af.bit_type	= 0;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);

	af.modifier	= UMAX(10, 5 * (ch->level/5));
	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }
    else
    {
	WAIT_STATE(ch,3 * PULSE_VIOLENCE);
	ch->move /= 2;
	send_to_char("Your pulse speeds up, but nothing happens.\n\r",ch);
	check_improve(ch,gsn_berserk,FALSE,2);
    }
}

void do_tarvals_wrath( CHAR_DATA *ch, char *argument)
{
    int skill;

    if ((skill = get_skill(ch,gsn_tarvals_wrath)) <= 0
    || IS_AFFECTED( ch, AFF_BERSERK ) )
    {
	send_to_char("You call on Tar-Val, but nothing happens.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_tarvals_wrath))
    {
	send_to_char("You cannot call upon him more than you have.\n\r",ch);
	return;
    }

    if ( ch->move < 9 )
    {
	send_to_char("You do not have enough energy!\n\r", ch );
	return;
    }

    if (number_percent() <= skill)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch,PULSE_VIOLENCE);

	send_to_char("You briefly glow `Yyellow`w and feel consumed in energy!\n\r",ch);
	cact("$n briefly glows `Yyellow`w!",ch,NULL,NULL,TO_ROOM, "`w");
	check_improve(ch,gsn_tarvals_wrath,TRUE,2);

	af.type		= gsn_tarvals_wrath;
	af.level	= skill / 2;
	af.duration	= UMAX(1,skill/30);
	af.modifier	= UMAX(1,skill/8);
	af.bitvector 	= 0;

	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);
	
	ch->move -= 10;
	ch->move = UMAX( ch->move, 0 );
    }
    else
    {
	WAIT_STATE(ch,PULSE_VIOLENCE);
	ch->move -= 5;
	ch->move = UMAX( ch->move, 0 );

	send_to_char("Your call upon Tar-Val fails.\n\r",ch);
	check_improve(ch,gsn_tarvals_wrath,FALSE,2);
    }
}

void do_blur( CHAR_DATA *ch, char *argument)
{
    int chance;


    if ((chance = get_skill(ch,gsn_blur)) <= 0 )
    {
	send_to_char("You have not the ability to focus your energies.\n\r",ch);
	return;
    }

    if (is_affected(ch,gsn_blur))
    {
	send_to_char("You cannot focus more energy then you have already.\n\r",ch);
	return;
    }

    if (ch->move < ch->level * 3 / 2)
    {
	send_to_char("You have exhausted your energies.\n\r",ch);
	return;
    }

    /* modifiers */

    /* fighting */
    if (ch->position == POS_FIGHTING)
	chance += 10;

    if (number_percent() <= chance)
    {
	AFFECT_DATA af;

	WAIT_STATE(ch, PULSE_VIOLENCE);

	/* remove some movement points, stay above 0 */
	ch->move -= ch->level * 3;
	ch->move = UMAX(ch->move,0);

	send_to_char("You manage to focus your energies and begin to blur!\n\r",ch);
	cact("$n wavers and blurs in the air before you!",ch,NULL,NULL,TO_ROOM, "`w");
	check_improve(ch,gsn_blur,TRUE,2);

	af.type		= gsn_blur;
	af.level	= ch->level;
	af.duration	= ch->level / 10 + 3;
	af.modifier	= UMIN(-1,-(ch->level * 3 / 2) );
	af.bitvector 	= 0;

	af.location	= APPLY_AC;
	affect_to_char(ch,&af);
    }

    else
    {
	WAIT_STATE(ch,2 * PULSE_VIOLENCE);

	send_to_char("Your attempt to focus your energies fails.\n\r",ch);
	check_improve(ch,gsn_blur,FALSE,2);
    }
}

void do_breathfire( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *pill;
    int chance;
    one_argument(argument,arg);
 
    if ( ( chance = get_skill(ch,gsn_breathfire) ) <= 0 )
    {	
	send_to_char("Breathfire? What's that?\n\r",ch);
	return;
    }
    pill = NULL;
 
    if ( !IS_NPC(ch) )
    {
    	for ( pill = ch->carrying; pill != NULL && pill->pIndexData->vnum != OBJ_VNUM_BREATHFIRE ; pill = pill->next_content );

    	if ( pill == NULL )
    	{
        	send_to_char( "You do not have the right pill to breathfire.\n\r", ch );
        	return;
    	}
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("That would not be a wise thing to do.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	cact("But $N is your friend!",ch,NULL,victim,TO_CHAR, "`W");
	return;
    }

    /* modifiers */

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10; 


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4/3;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {
    
	cact("A powerfull blast of fire streams from $n towards you!",
		ch,NULL,victim,TO_VICT, "`c");
	cact("You pop a lava pill into your mouth and exhale a mighty stream of fire upon $N",ch,NULL,victim,TO_CHAR, "`c");
	cact("A stream of fire blasts from $n's mouth",
		ch,NULL,victim,TO_NOTVICT, "`c");
	check_improve(ch,gsn_breathfire,TRUE,1);

	WAIT_STATE(victim, 3 * PULSE_VIOLENCE);
	WAIT_STATE(ch,skill_table[gsn_breathfire].beats);
	damage(ch,victim,number_range(2,2 + 1 * ch->level + chance/20),gsn_breathfire,
	    DAM_FIRE);
	
    }
    else
    {
	damage(ch,victim,0,gsn_bash,DAM_FIRE);
	cact("You exhale a curl of smoke.  The lava pill failed!",
	    ch,NULL,victim,TO_CHAR, "`g");
	cact("A puff of smoke comes from $n's mouth.",
	    ch,NULL,victim,TO_NOTVICT, "`g");
	cact("$n blows smoke in your face.",
	    ch,NULL,victim,TO_VICT, "`g");
	check_improve(ch,gsn_breathfire,FALSE,1);
	set_position( ch, POS_RESTING );
	WAIT_STATE(ch,skill_table[gsn_breathfire].beats * 3/2); 
    }

    if ( !IS_NPC(ch) )
	extract_obj( pill );
}

void do_shriek( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    int skill = 1, chance = 1;

 
    if ( ( chance = get_skill(ch,gsn_shriek) ) <= 0 )
    {	
	send_to_char("Shriek? What's that?\n\r",ch);
	return;
    }
 
    victim = ch->fighting;
    if (victim != NULL)
    {
	send_to_char("You can't concentrate enough while in combat!\n\r",ch);
	return;
    }

    /* level */
    chance += ch->level / 3;

    /* now the attack */
    if (number_percent() < chance)
    {
      	AFFECT_DATA af;
	skill = get_skill( ch, gsn_shriek );
	send_to_char("You let out a piercing shriek!\n\r", ch );
	cact("$n opens their mouth and lets out an ear-piercing shriek!",
		ch,NULL,victim,TO_ROOM, "`c");
	check_improve(ch,gsn_shriek,TRUE,1);

	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
	af.type      = gsn_shriek;
    	af.level     = ch->level;
    	af.duration  = skill / 15;
    	af.location  = APPLY_ROOM_FLAG;
    	af.modifier  = 0;
    	af.bit_type  = 0;
    	af.bitvector = ROOM_SILENCE;
    	affect_apply_room( ch->in_room, &af );
    }
    else
    {
      	AFFECT_DATA af;
	af.type		= gsn_shriek;
	af.level	= ch->level;
	af.duration	= ch->level/2;
	af.modifier	= -1 * skill/8;
	af.bitvector 	= 0;
	af.location	= APPLY_HITROLL;
	affect_to_char(ch,&af);
	af.location	= APPLY_DAMROLL;
	affect_to_char(ch,&af);
	send_to_char("You open your mouth and choke and cough!\n\r", ch );
	cact("$n opens their mouth and suddenly coughs and chokes!",
	    ch,NULL,victim,TO_ROOM, "`g");
	check_improve(ch,gsn_shriek,FALSE,1);
	WAIT_STATE(ch,PULSE_VIOLENCE); 
    }
}

void do_intimidate( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int skill, chance;

    one_argument(argument,arg);
 
    if ( ( chance = get_skill(ch,gsn_intimidate) ) <= 0 )
    {	
	send_to_char("Intimidate? What's that?\n\r",ch);
	return;
    }
 
    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (victim == ch)
    {
	send_to_char("Intimidate yourself?\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	cact("But $N is your friend!",ch,NULL,victim,TO_CHAR, "`W");
	return;
    }

    /* modifiers */

    chance += get_curr_stat( ch, STAT_CHR );

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10; 

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {
      	AFFECT_DATA af;

	skill = get_skill( ch, gsn_intimidate );
	cact("$n screams at you and you jump back in fear!",
		ch,NULL,victim,TO_VICT, "`c");
	cact("\n\rYou scream at $N, and they jump back in fear!",ch,NULL,victim,TO_CHAR, "`c");
	cact("$n screams at $N, and $N jumps back in fear.",
		ch,NULL,victim,TO_NOTVICT, "`c");
	check_improve(ch,gsn_intimidate,TRUE,1);

	WAIT_STATE(ch, PULSE_VIOLENCE);
	
	af.type		= gsn_intimidate;
	af.level	= skill / 2;
	af.duration	= skill / 15;
	af.modifier	= -1 * skill/8;
	af.bitvector 	= 0;

	af.location	= APPLY_HITROLL;
	affect_to_char(victim,&af);

	af.location	= APPLY_DAMROLL;
	affect_to_char(victim,&af);
    }
    else
    {
	cact("You scream at $N but don't look impressed with your efforts!",
	    ch,NULL,victim,TO_CHAR, "`g");
	cact("$n screams at $N but $N isn't affected",
	    ch,NULL,victim,TO_NOTVICT, "`g");
	cact("$n screams at you but you are not the least bit frightened by them.",
	    ch,NULL,victim,TO_VICT, "`g");
	check_improve(ch,gsn_intimidate,FALSE,1);
	WAIT_STATE(ch,skill_table[gsn_intimidate].beats * 3/2); 
    }
}


void do_bash( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument,arg);
 
    if ( ( chance = get_skill(ch,gsn_bash) ) <= 0 )
    {	
	send_to_char("Bashing? What's that?\n\r",ch);
	return;
    }
 
    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if ( !can_see( ch, victim ) )
    {
	send_to_char( "You cannot see them.\n\r", ch );
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
	cact("You'll have to let $M get back up first.",ch,NULL,victim,TO_CHAR, "`Y");
	return;
    } 

    if ( IS_AFFECTED( victim, AFF_FLYING ) || IS_AFFECTED( victim, AFF_LEVITATING ) )
    {
	send_to_char("You can't bash someone who isn't steadily on the ground.\n\r", ch );
	return;
    }

    if (victim == ch)
    {
	send_to_char("You try to bash your brains out, but fail.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	cact("But $N is your friend!",ch,NULL,victim,TO_CHAR, "`W");
	return;
    }

    /* modifiers */

    /* size  and weight */
    chance += ch->carry_weight / 25;
    chance -= victim->carry_weight / 20;

    if (ch->size < victim->size)
	chance += (ch->size - victim->size) * 25;
    else
	chance += (ch->size - victim->size) * 10; 


    /* stats */
    chance += get_curr_stat(ch,STAT_STR);
    chance -= get_curr_stat(victim,STAT_DEX) * 4 / 3;

    /* speed */
    if (IS_SET(ch->off_flags,OFF_FAST))
	chance += 10;
    if (IS_SET(victim->off_flags,OFF_FAST))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* now the attack */
    if (number_percent() < chance)
    {
    
	cact("$n sends you sprawling with a powerful bash!",
		ch,NULL,victim,TO_VICT, "`c");
	cact("\n\rYou bash into $N, and send $M flying!",ch,NULL,victim,TO_CHAR, "`c");
	cact("$n sends $N sprawling with a powerful bash.",
		ch,NULL,victim,TO_NOTVICT, "`c");
	check_improve(ch,gsn_bash,TRUE,1);

	WAIT_STATE(victim, PULSE_VIOLENCE);
	WAIT_STATE(ch, 2 * PULSE_VIOLENCE);
	set_position( victim, POS_RESTING );
	set_position( ch, POS_RESTING );
	damage(ch,victim,number_range(2,2 + 2 * ch->size + chance/20),gsn_bash,
	    DAM_BASH); 
	
    }
    else
    {
	damage(ch,victim,0,gsn_bash,DAM_BASH);
	cact("You fall flat on your face!",
	    ch,NULL,victim,TO_CHAR, "`g");
	cact("$n falls flat on $s face.",
	    ch,NULL,victim,TO_NOTVICT, "`g");
	cact("You evade $n's bash, causing $m to fall flat on $s face.",
	    ch,NULL,victim,TO_VICT, "`g");
	check_improve(ch,gsn_bash,FALSE,1);
	set_position( ch, POS_RESTING );
	WAIT_STATE(ch,skill_table[gsn_bash].beats * 3/2); 
    }
}

void do_perception( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_DATA *obj;
    int skill = 0;
    bool found;

    found = TRUE;
    if ( *argument == '\0' )
    {
        send_to_char( "Examine what?\n\r", ch );
        return;
    }

    if ( ( obj = get_obj_carry( ch, argument ) ) == NULL )
    {
        send_to_char( "You don't have that.\n\r", ch );
        return;
    }

    skill = get_skill( ch, gsn_perception );

    if ( skill <= 0 )
    {
	send_to_char( "You don't know this skill.\n\r", ch );
	return;
    }

    sprintf(buf,"`R****************************************************\n\r");
    send_to_char(buf, ch);
    sprintf( buf, "`R* `wName: `G%s\n\r", obj->short_descr );
    send_to_char( buf, ch );

    if ( obj->wear_data != NULL )
    {
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

    check_improve(ch,gsn_perception,TRUE,1);
    return;
}

void do_lore( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    OBJ_INDEX_DATA *obj;
    int sn,x;
    int chance;

    if ( ( chance = get_skill(ch,gsn_lore) ) <= 0 )
    {	
	send_to_char("Lore? What's that?\n\r",ch);
	return;
    }
 
    if ( argument[0] == '\0' )
    {
	send_to_char( "Lore what?\n\r", ch );
	return;
    }
    send_to_char("You begin to meditate\n\r", ch);
    WAIT_STATE(ch,2 * PULSE_VIOLENCE); 
    sn = skill_lookup(argument);
    if ( sn < 0 )
    {
	send_to_char( "You did not find any spells or skills with that name \n\r", ch );
	return;
    }
    if (number_percent() < chance)
    {
    	sprintf(buf,"`MName:`w %-25s\n\r",skill_table[sn].name);
    	send_to_char(buf,ch);

    	for ( x = 0; x < MAX_PREREQUISITE; x++ )
    	    if ( skill_table[sn].prerequisite[x].pgsn != NULL )
    	    {
		sprintf(buf,"`MPrerequisite:`w %s\n\r",skill_table[*skill_table[sn].prerequisite[x].pgsn].name);
		send_to_char(buf,ch);
    	    }

    	for ( x = 0; x < MAX_COMPONENT; x++ )
    	    if ( skill_table[sn].component[x].vnum != 0 )
    	    {
	    	if ( ( obj = get_obj_index( skill_table[sn].component[x].vnum ) ) != NULL )
	    	{	
		    sprintf(buf,"`MComponents:`w %s\n\r",obj->short_descr);
		    send_to_char(buf,ch);
	    	}
    	    }

    	for ( x = 0; x < MAX_ELEMENT_TYPE; x++ )
    	{
 	    sprintf(buf,"`MElements:`w %10s  `MMana`w %3d %s\n\r",element_type_name(x), get_mana_cost(ch, sn, x), 
	    skill_table[sn].element[x].level == -1 ? "" : skill_table[sn].element[x].level == 0 ? "Minor Power" : skill_table[sn].element[x].level == 1 ? "Normal" : skill_table[sn].element[x].level == 2 ? "Major Power" : "");
	    send_to_char(buf,ch);
    	}

    	check_improve(ch,gsn_lore,TRUE,1);
    }
    else
    {
	send_to_char("Well, you are not quite sure...\n\r",ch);
	check_improve(ch,gsn_lore,FALSE,1);
    }
}

void do_dirt( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_dirt)) <= 0 )
    {
	send_to_char("You fail in blinding your opponent.\n\r",ch);
	return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't in combat!\n\r",ch);
	    return;
	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (victim->mount != NULL && IS_SET(victim->mount->affected_by,AFF_MOUNTED) )
    {
        send_to_char("Rather hard to kick dirt in his face when he is on a mount!\n\r",ch);
        return;
    }

    if (ch->mount != NULL && IS_SET(ch->mount->affected_by,AFF_MOUNTED) )
    {
        send_to_char("Rather hard to kick dirt while on a mount!\n\r",ch);
        return;
    }

    if (IS_AFFECTED(victim,AFF_BLIND))
    {
	cact("$e's already been blinded.",ch,NULL,victim,TO_CHAR, "`G");
	return;
    }

    if (victim == ch)
    {
	send_to_char("What?\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	cact("But $N is such a good friend!",ch,NULL,victim,TO_CHAR, "`g");
	return;
    }

    /* modifiers */

    /* dexterity */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_DEX);

    /* speed  */
    if ((IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
	chance += 10;
    if ((IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
	chance -= 25;

    /* level */
    chance += (ch->level - victim->level) * 2;

    /* sloppy hack to prevent false zeroes */
    if (chance % 5 == 0)
	chance += 1;

    /* terrain  */

    switch(ch->in_room->sector_type)
    {
	case(SECT_NONE):		chance -= 10;	break;
	case(SECT_CITY):		chance -= 10;	break;
	case(SECT_FIELD):		chance +=  5;	break;
	case(SECT_FOREST):				break;
	case(SECT_HILLS):				break;
	case(SECT_MOUNTAIN):		chance -= 10;	break;
	case(SECT_WATER_SWIM):		chance -= 30;	break;
	case(SECT_WATER_NOSWIM):	chance -= 30;	break;
	case(SECT_AIR):			chance -= 30;  	break;
	case(SECT_DESERT):		chance += 10;   break;
	case(SECT_DIRT):		chance += 10;   break;
	case(SECT_SAND):		chance += 10;   break;
	default: 					break;
    }

    if (chance == 0)
    {
	send_to_char("You fail at blinding your opponent.\n\r",ch);
	return;
    }

    /* now the attack */
    if (number_percent() < chance)
    {
	AFFECT_DATA af;
	send_to_char("You kick a cloud of dirt into your opponents face!\n\r", ch);
	send_to_char("You can't see a thing!\n\r",victim);
	check_improve(ch,gsn_dirt,TRUE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].beats);

	af.type 	= gsn_dirt;
	af.level 	= ch->level;
	af.duration	= 0;
	af.location	= APPLY_HITROLL;
	af.modifier	= -4;
	affect_to_char(victim,&af);
	af.location	= APPLY_AFFECT;
	af.bitvector 	= AFF_BLIND;
	af.bit_type	= BIT_AFFECT;

	affect_to_char(victim,&af);
    }
    else
    {
	damage(ch,victim,0,gsn_dirt,DAM_NONE);
	check_improve(ch,gsn_dirt,FALSE,2);
	WAIT_STATE(ch,skill_table[gsn_dirt].beats);
    }
}

void do_trip( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_trip)) <= 0 )
    {
	send_to_char("Tripping?  What's that?\n\r",ch);
	return;
    }


    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
 	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
	send_to_char("Kill stealing is not permitted.\n\r",ch);
	return;
    }
    
    if (IS_AFFECTED(victim,AFF_FLYING|AFF_LEVITATING) 
        || ( victim->mount !=NULL && IS_SET(victim->mount->affected_by,AFF_FLYING|AFF_LEVITATING) ) )
    {
	act("$S feet aren't on the ground.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
	act("$N is already down.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("You fall flat on your face!\n\r",ch);
	WAIT_STATE(ch,2 * skill_table[gsn_trip].beats);
	act("$n trips over $s own feet!",ch,NULL,NULL,TO_ROOM);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
    if ((IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
	chance += 10;
    if ((IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;


    /* now the attack */
    if (number_percent() < chance)
    {
	act("$n trips you and you go down!",ch,NULL,victim,TO_VICT);
	act("You trip $N and $N goes down!",ch,NULL,victim,TO_CHAR);
	act("$n trips $N, sending $M to the ground.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_trip,TRUE,1);

        if ( victim->mount != NULL && IS_SET( victim->mount->affected_by,AFF_MOUNTED) )
        {
	    send_to_char("You fall off your mount",victim);
	    REMOVE_BIT(victim->mount->affected_by,AFF_MOUNTED);
        }

	WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        WAIT_STATE(ch,skill_table[gsn_trip].beats);
	set_position( victim, POS_RESTING );
	damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_trip, DAM_BASH);
    }
    else
    {
	damage(ch,victim,0,gsn_trip,DAM_BASH);
	WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
	check_improve(ch,gsn_trip,FALSE,1);
    } 
}

void do_tail( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument,arg);

    if ( (chance = get_skill(ch,gsn_tail)) <= 0
    ||   !IS_NPC(ch) )
    {
	send_to_char("You wag your tail.\n\r",ch);
	return;
    }

    for ( victim = ch->in_room->people ; victim != NULL ; victim = victim->next_in_room )
    {
        chance = get_skill(ch,gsn_tail);

        if (is_safe(ch,victim)
	  || ( IS_NPC(ch) && IS_NPC(victim) ) 
	  || ( !IS_NPC(ch) && !IS_NPC(victim) ) )
	    continue;

        if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
        {
	    send_to_char("Kill stealing is not permitted.\n\r",ch);
	    continue;
        }
    
        if (IS_AFFECTED(victim,AFF_FLYING|AFF_LEVITATING) 
            || ( victim->mount !=NULL && IS_SET(victim->mount->affected_by,AFF_FLYING|AFF_LEVITATING) ) )
	continue;

        if (victim->position < POS_FIGHTING)
        {
	    act("$N is already down.",ch,NULL,victim,TO_CHAR);
	    continue;
        }

        if (victim == ch)
	    continue;

        if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
        {
	    act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
	    continue;
        }

    /* modifiers */
  	if ( victim->mount != NULL && IS_AFFECTED( victim->mount, AFF_MOUNTED ) )
	    chance += 15;

    /* size */
        if (ch->size < victim->size)
            chance += (ch->size - victim->size) * 10;  /* bigger = harder to trip */

    /* dex */
        chance += get_curr_stat(ch,STAT_DEX);
        chance -= get_curr_stat(victim,STAT_DEX) * 3 / 2;

    /* speed */
        if ((IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
 	    chance += 10;
        if ((IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
	    chance -= 20;

    /* level */
        chance += (ch->level - victim->level) * 2;


    /* now the attack */
        if (number_percent() < chance)
        {
	    act("$n trips you with $s tail and you go down!",ch,NULL,victim,TO_VICT);
	    act("You trip $N with your tail and $N goes down!",ch,NULL,victim,TO_CHAR);
	    act("$n trips $N with $s tail, sending $M to the ground.",ch,NULL,victim,TO_NOTVICT);
	    check_improve(ch,gsn_trip,TRUE,1);

            if ( victim->mount != NULL && IS_SET( victim->mount->affected_by,AFF_MOUNTED) )
            {
	        send_to_char("You fall off your mount",victim);
	        REMOVE_BIT(victim->mount->affected_by,AFF_MOUNTED);
            }

	    WAIT_STATE(victim,2 * PULSE_VIOLENCE);
            WAIT_STATE(ch,skill_table[gsn_tail].beats);
	    set_position( victim, POS_RESTING );
	    damage(ch,victim,number_range(2, 2 +  2 * victim->size),gsn_tail,
	        DAM_BASH);
        }
        else
        {
	    damage(ch,victim,0,gsn_tail,DAM_BASH);
	    WAIT_STATE(ch,skill_table[gsn_trip].beats*2/3);
        } 
    }
}


void do_slam( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument,arg);

    if   ( (chance = get_skill(ch,gsn_slam) <= 0 ) )
    {
	send_to_char("It would be wise to practice first!!\n\r",ch);
	return;
    }


    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
 	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }

    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if (is_safe(ch,victim))
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
	send_to_char("Kill stealing is not permitted.\n\r",ch);
	return;
    }
    
    if ( IS_AFFECTED( victim, AFF_FLYING ) || IS_AFFECTED( victim, AFF_LEVITATING ) )
    {
	send_to_char("You can't slam someone who isn't steadily on the ground.\n\r", ch );
	return;
    }

    if (victim->position < POS_FIGHTING)
    {
	act("$N is already down.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("Huh? slam yourself? \n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
	return;
    }

    /* modifiers */
    chance += ch->carry_weight / 25;

    /* size */
    if (ch->size < victim->size)
        chance += (ch->size - victim->size) * 10;  /* bigger = harder to slam */

    /* str */
    chance += get_curr_stat(ch,STAT_STR);

    /* speed */
    if ((IS_SET(ch->off_flags,OFF_FAST) || IS_AFFECTED(ch,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
	chance += 10;
    if ((IS_SET(victim->off_flags,OFF_FAST) || IS_AFFECTED(victim,AFF_HASTE)) && !IS_AFFECTED_2(ch,AFF_SLOW))
	chance -= 20;

    /* level */
    chance += (ch->level - victim->level) * 2;


    /* now the attack */
    if (number_percent() < chance)
    {
	act("$n slams you hard to the ground!",ch,NULL,victim,TO_VICT);
	act("You slam $N hard to the ground!",ch,NULL,victim,TO_CHAR);
	act("$n slams $N hard to the ground!.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_slam,TRUE,1);

	WAIT_STATE(victim,2 * PULSE_VIOLENCE);
	WAIT_STATE(ch,3 * PULSE_VIOLENCE);
	set_position( victim, POS_RESTING );
	damage(ch,victim,number_range(ch->level * ch->size, ( ch->level * 2 ) * ch->size),gsn_slam,
	    DAM_BASH);
    }
    else
    {
	damage(ch,victim,number_range(ch->level*ch->size/10, ch->level*ch->size/15),gsn_slam,DAM_BASH);
	WAIT_STATE(ch,skill_table[gsn_slam].beats*2/3);
	check_improve(ch,gsn_slam,FALSE,1);
    } 
}

void do_stun( CHAR_DATA *ch, char *argument )
{
    char arg[ MAX_INPUT_LENGTH ];
    CHAR_DATA *victim;
    int chance;

    one_argument(argument,arg);

    if   ( (chance = get_skill(ch,gsn_stun) <= 0 ) )
    {
	send_to_char("It would be wise to practice first!!\n\r",ch);
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
        set_position( ch, POS_STANDING );
        send_to_char("You must be in combat to use this skill.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
 	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if ( IS_SET(victim->affected_by_2, AFF_STUN) )
    {
	act("$N is already stunned.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("Huh? Stun yourself? \n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( ch->move < 21 )
    {
        send_to_char( "You haven't enough energy to focus.\n\r", ch);
        return;
    }

    ch->move -= 20;
    ch->move = UMAX( ch->move,1 );
    /* modifiers */
    /* dex and con */
    chance = get_skill(ch,gsn_stun) / 2;
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_CON) * 2 / 3;
    /* level */
    chance += (ch->level - victim->level) * 2;
    /* now the attack */
    if (number_percent() < chance)
    {
	act("`Y$n stuns you with a powerful punch!`w",ch,NULL,victim,TO_VICT);
	act("`YYou stun $N with your powerful punch!`w",ch,NULL,victim,TO_CHAR);
	act("`Y$n stuns $N with a powerful punch!.`w",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_stun,TRUE,1);

	WAIT_STATE(ch,3 * PULSE_VIOLENCE);
	    SET_BIT(victim->affected_by_2, AFF_STUN);
    }
    else
    {
	damage(ch,victim, 0 ,gsn_stun,DAM_BASH);
	WAIT_STATE(ch,skill_table[gsn_stun].beats*2/3);
	check_improve(ch,gsn_stun,FALSE,1);
    } 
}

void do_ki_force( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    CHAR_DATA *victim;
    int chance;

    one_argument(argument,arg);

    if   ( (chance = get_skill(ch,gsn_ki_force) <= 0 ) )
    {
	send_to_char("It would be wise to practice first!!\n\r",ch);
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_WIELD_R ) ) != NULL
    && ( obj = get_eq_char( ch, WEAR_WIELD_L ) ) != NULL
    && ( obj = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) != NULL )
    {
        send_to_char( "You cannot wield a weapon while attempting Ki Force.\n\r", ch );
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
        set_position( ch, POS_STANDING );
        send_to_char("You must be in combat to use this skill.\n\r",ch);
        return;
    }

    if (arg[0] == '\0')
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
 	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }
    else if ((victim = get_char_room(ch,arg)) == NULL)
    {
	send_to_char("They aren't here.\n\r",ch);
	return;
    }

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
	send_to_char("Kill stealing is not permitted.\n\r",ch);
	return;
    }
    
    if ( IS_SET(victim->affected_by_2, AFF_STUN) )
    {
	act("$N is already stunned.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if (victim == ch)
    {
	send_to_char("Huh? Ki Force yourself? \n\r",ch);
	return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) && ch->master == victim)
    {
	act("$N is your beloved master.",ch,NULL,victim,TO_CHAR);
	return;
    }

    if ( ch->move < 31 )
    {
        send_to_char( "You haven't enough energy to focus.\n\r", ch);
        return;
    }

    ch->move -= 50;
    ch->move = UMAX( ch->move,1 );
    /* modifiers */

    /* dex and con */
    chance = get_skill(ch,gsn_ki_force) / 2;
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= get_curr_stat(victim,STAT_CON) * 2 / 3;

    /* level */
    chance += (ch->level - victim->level) * 2;


    /* now the attack */
    if (number_percent() < chance)
    {
	act("`Y$n stuns you with a powerful punch!`w",ch,NULL,victim,TO_VICT);
	act("`YYou stun $N with your powerful punch!`w",ch,NULL,victim,TO_CHAR);
	act("`Y$n stuns $N with a powerful punch!.`w",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_ki_force,TRUE,1);

	WAIT_STATE(ch,3 * PULSE_VIOLENCE);
	if ( number_percent() < chance )
	{
    	    damage( ch, victim, number_range( 10, ch->level / 2 ), gsn_ki_force, DAM_BASH );
	    SET_BIT(victim->affected_by_2, AFF_STUN);
	}
	else
	    damage( ch, victim, number_range( 10, ch->level / 2 ), gsn_ki_force, DAM_BASH );
    }
    else
    {
	damage(ch,victim, 0 ,gsn_ki_force,DAM_BASH);
	WAIT_STATE(ch,skill_table[gsn_ki_force].beats*2/3);
	check_improve(ch,gsn_ki_force,FALSE,1);
    } 
}


void do_kill( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Kill whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if (IS_SET(victim->act2, ACT2_MOVEABLE_AREA ) )
    {
	send_to_char( "You can't kill that!\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "You hit yourself.  Ouch!\n\r", ch );
	ch->hit -=5;
	return;
    }

    if ( is_safe( ch, victim ) )
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }

    if ( IS_AFFECTED(ch, AFF_CONTROL_MIND) && ch->master == victim )
    {
	act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "You do the best you can!\n\r", ch );
	return;
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}



void do_murde( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to MURDER, spell it out.\n\r", ch );
    return;
}

void do_hkill( CHAR_DATA *ch, char *argument )
{
    send_to_char( "Huh?\n\r", ch );
    return;
}



void do_murder( CHAR_DATA *ch, char *argument ) /*DONE*/
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Murder whom?\n\r", ch );
	return;
    }

    if (IS_AFFECTED(ch,AFF_CONTROL_MIND) || (IS_NPC(ch) && 
       ( IS_SET(ch->act,ACT_PET) || IS_SET( ch->act,ACT_MOUNT))))
	return;

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

/* blonde fight.c Can't murder a moveable area */
if (IS_SET(victim->act2, ACT2_MOVEABLE_AREA ) )
    {
	send_to_char( "You can't Murder that!\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Suicide is a mortal sin.\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
	return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }

    if ( IS_AFFECTED(ch, AFF_CONTROL_MIND) && ch->master == victim )
    {
	act( "$N is your beloved master.", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( ch->position == POS_FIGHTING )
    {
	send_to_char( "You do the best you can!\n\r", ch );
	return;
    }

    if ( ch->level > victim->level+5 )
    {
	send_to_char( "The gods do not like you attack low level players!\n\r", ch );
	return;
    }

    if ( !IS_NPC(victim) && ch->pcdata->pcclan == NULL )
    {
	send_to_char("You cannot murder another player unless you are in a clan!\n\r", ch );
	return;
    }
 
    if ( !IS_NPC(victim) && victim->pcdata->pcclan == NULL )
    {
	send_to_char("You cannot murder a player who is not in a clan!\n\r",ch);
	return;
    }

    if ( !IS_NPC(victim) && ch->pcdata->pcclan->clan == victim->pcdata->pcclan->clan )
    {
    	sprintf(buf, "%s is in your clan! You can't start a civil war!", victim->name);
	send_to_char(buf, ch);
    }

    WAIT_STATE( ch, 1 * PULSE_VIOLENCE );
    if (IS_NPC(ch))
	sprintf(buf, "Help! I am being attacked by %s!",ch->short_descr);
    else
    	sprintf( buf, "Help!  I am being attacked by %s!", ch->name );
    do_yell( victim, buf );
    multi_hit( ch, victim, TYPE_UNDEFINED );
    return;
}

void do_double_strike( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Double Strike whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you sneak up on yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }


    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    {
        if ( ( obj = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) == NULL )
        {
	    send_to_char( "You need to wield a one handed weapon to double strike.\n\r", ch );
	    return;
        }
	else
	{
	    send_to_char( "You need to wield a weapon to double strike.\n\r", ch );
	    return;
	}
    }

    obj = get_eq_char( ch, WEAR_WIELD );
    if ( obj->wear_data->weapon_data->type != 2 )
    {
	send_to_char( "You need to wield a dagger as a weapon to double strike.\n\r", ch );
	return;
    }
    if ( victim->fighting != NULL )
    {
	send_to_char( "You can't double strike a fighting person.\n\r", ch );
	return;
    }

    if ( victim->hit < victim->max_hit && victim->position >= POS_STANDING )
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    /* add code for hiding here GALADRAIL*/

     if ( is_affected( ch, skill_lookup( "shadowform" ) ) 
	    || is_affected( ch, skill_lookup( "mass shadowform" ) ) )
	chance = 10;
     else
	chance = 0;
    WAIT_STATE( ch, skill_table[gsn_double_strike].beats );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill( ch, gsn_double_strike ) + chance )
    {
	check_improve(ch,gsn_double_strike,TRUE,1);
	one_hit( ch, victim, obj, gsn_shadowstrike );
	one_hit( ch, victim, obj, gsn_shadowstrike );
    }
    else
    {
	check_improve(ch,gsn_double_strike,FALSE,1);
	damage( ch, victim, 0, gsn_shadowstrike,DAM_NONE );
    }

    return;
}


void do_strike( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance = 1;

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You have to be in battle to use strike.\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }


    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    {
        if ( ( obj = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) == NULL )
        {
	    send_to_char( "You need to wield a one handed weapon to strike.\n\r", ch );
	    return;
        }
	else
	{
	    send_to_char( "You need to wield a weapon to strike.\n\r", ch );
	    return;
	}
    }

    obj = get_eq_char( ch, WEAR_WIELD );
    if ( obj->wear_data->weapon_data->type != 2 )
    {
	send_to_char( "You need to wield a dagger as a weapon to strike.\n\r", ch );
	return;
    }
    WAIT_STATE( ch, 3 * PULSE_VIOLENCE );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill( ch, gsn_strike ) + chance )
    {
	check_improve(ch,gsn_strike,TRUE,1);
	one_hit( ch, victim, obj, gsn_shadowstrike );
    }
    else
    {
	check_improve(ch,gsn_strike,FALSE,1);
	damage( ch, victim, 0, gsn_shadowstrike,DAM_NONE );
    }

    return;
}

void do_shadowstrike( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int chance;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Shadowstrike whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "How can you sneak up on yourself?\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( victim->fighting != NULL && !is_same_group(ch,victim->fighting)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }


    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL )
    {
        if ( ( obj = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) == NULL )
        {
	    send_to_char( "You need to wield a one handed weapon to shadowstrike.\n\r", ch );
	    return;
        }
	else
	{
	    send_to_char( "You need to wield a weapon to shadowstrike.\n\r", ch );
	    return;
	}
    }

    obj = get_eq_char( ch, WEAR_WIELD );
    if ( obj->wear_data->weapon_data->type != 2 )
    {
	send_to_char( "You need to wield a dagger as a weapon to shadowstrike.\n\r", ch );
	return;
    }
    if ( victim->fighting != NULL )
    {
	send_to_char( "You can't shadowstrike a fighting person.\n\r", ch );
	return;
    }

    if ( victim->hit < victim->max_hit && ch->position >= POS_STANDING )
    {
	act( "$N is hurt and suspicious ... you can't sneak up.",
	    ch, NULL, victim, TO_CHAR );
	return;
    }

    /* add code for hiding here GALADRAIL*/

     if ( is_affected( ch, skill_lookup( "shadowform" ) ) 
	    || is_affected( ch, skill_lookup( "mass shadowform" ) ) )
	chance = 10;
     else
	chance = 0;
    WAIT_STATE( ch, skill_table[gsn_shadowstrike].beats );
    if ( !IS_AWAKE(victim)
    ||   IS_NPC(ch)
    ||   number_percent( ) < get_skill( ch, gsn_shadowstrike ) + chance )
    {
	check_improve(ch,gsn_shadowstrike,TRUE,1);
	one_hit( ch, victim, obj, gsn_shadowstrike );
    }
    else
    {
	check_improve(ch,gsn_shadowstrike,FALSE,1);
	damage( ch, victim, 0, gsn_shadowstrike,DAM_NONE );
    }

    return;
}

void do_precision_strike( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    AFFECT_DATA af;
    int hth,chance;

    if ( get_skill(ch,gsn_precision_strike) <= 0 )
    {
        send_to_char( "You don't know how to make precise strikes.\n\r", ch );
        return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
	set_position( ch, POS_STANDING );
	send_to_char("You must be in combat to use this skill.\n\r",ch);
	return;
    }

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Precision Strike where?\n\r", ch );
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_WIELD_R ) ) != NULL
    && ( obj = get_eq_char( ch, WEAR_WIELD_L ) ) != NULL
    && ( obj = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) != NULL )
    {
        send_to_char( "You cannot wield a weapon to make precision strikes with your hands.\n\r", ch );
        return;
    }

   /* remove movement points (energy skill) */

   if ( ch->move < 31 )
   {
	send_to_char( "You haven't enough energy to focus.\n\r", ch);
	return;
   }

   ch->move -= 30;
   ch->move = UMAX( ch->move,1 );

   /* calculate chance */

   hth     = get_skill(ch,gsn_hand_to_hand[0]);
   chance  = get_skill(ch,gsn_precision_strike);
   chance += get_curr_stat(ch,STAT_DEX);
   chance -= 3 * get_curr_stat(victim,STAT_CON);
   if ( ch->level >= victim->level )
   	chance += (ch->level - victim->level) / 2;
   else
   	chance += (ch->level - victim->level) * 2;
   chance = chance * hth/80;
   
   if ( !str_cmp( arg, "temples" ) )
   {
	act("You attempt to strike at $N's temples!",ch,NULL,victim,TO_CHAR);
        act("$n attempts to strike at your temples!",ch,NULL,victim,TO_VICT);
        act("$n attempts to strike at $N's temples!",ch,NULL,victim,TO_NOTVICT);
	chance = URANGE(2, chance - 35, 98);
        WAIT_STATE( ch,2 * PULSE_VIOLENCE );
    	if ( number_percent( ) < chance )
	{
		if ( number_percent( ) < 2 * ( ch->level/6 ) && !saves_spell( ch->level, victim ) )
		{
			check_improve( ch,gsn_precision_strike,TRUE,1 );
			victim->hit = 0;
			act("`YYour strike at $N's temples renders them defenseless!`R",ch,NULL,victim,TO_CHAR);
	        	act("`Y$n`s strike at your temples renders you defenseless!`R",ch,NULL,victim,TO_VICT);
		        act("$n`s strike at $N's temples renders them defenseless!`R",ch,NULL,victim,TO_NOTVICT);
			damage( ch, victim, 3 * ( ( ch->level * 3 ) / 4 )  , gsn_precision_strike,DAM_NONE );
		}
		else
		{
			check_improve( ch,gsn_precision_strike,TRUE,1 );
			damage( ch, victim, 3 * ( ( ch->level * 3 ) / 4 )  , gsn_precision_strike,DAM_NONE );
		}
	}
    	else
    	{
		check_improve( ch,gsn_precision_strike,FALSE,1 );
		damage( ch, victim, 0, gsn_precision_strike,DAM_NONE );
    	}
	return;
   }
   else if ( !str_cmp( arg, "left" ) )
   {
	send_to_char("You make an attempt to swing at your opponents left arm!\n\r", ch);
	return;
   }
   else if ( !str_cmp( arg, "right" ) )
   {
	send_to_char("You make an attempt to swing at your opponents right arm!\n\r", ch);
	return;
   }
   else if ( !str_cmp( arg, "eyes" ) )
   {
        act("You attempt to gauge at $N's eyes!",ch,NULL,victim,TO_CHAR);
        act("$n attempts to gauge at your eyes!",ch,NULL,victim,TO_VICT);
        act("$n attempts to gauge at $N's eyes!",ch,NULL,victim,TO_NOTVICT);
        chance = URANGE(2, chance - 15, 98);
        WAIT_STATE( ch, 2 * PULSE_VIOLENCE );
        if ( number_percent( ) < chance )
	{
		if ( number_percent() < ( 2 * ( ch->level / 3 ) ) )
    		{
			if (IS_AFFECTED(victim,AFF_BLIND))
	    		{
				cact("$e's already been blinded.",ch,NULL,victim,TO_CHAR, "`G");
				return;
	    		}
	
	                act("`YYour gauging at $N's eyes blinds them!`R",ch,NULL,victim,TO_CHAR);
	                act("`Y$n`s gauge at your eyes blinds you!`R",ch,NULL,victim,TO_VICT);
	                act("$n`s gauge at $N's eyes is successful!`R",ch,NULL,victim,TO_NOTVICT);
	        	check_improve(ch,gsn_precision_strike,TRUE,1);
			damage( ch, victim, 3 *  ( ch->level / 4 )  , gsn_precision_strike,DAM_NONE );
	
	        	af.type         = gsn_precision_strike;
	        	af.level        = ch->level;
	        	af.duration     = ch->level;
	        	af.location     = APPLY_HITROLL;
	        	af.modifier     = -8;
	        	affect_to_char(victim,&af);
	        	af.location     = APPLY_AFFECT;
 		       	af.bitvector    = AFF_BLIND;
 		       	af.bit_type     = BIT_AFFECT;
		    	affect_to_char(victim,&af);
		}
		else
		{
			damage( ch, victim, 3 * ( ch->level / 4 )  , gsn_precision_strike,DAM_NONE );
		        check_improve(ch,gsn_precision_strike,FALSE,1);
		}
	}	
	else
	{
	        damage(ch,victim,0,gsn_precision_strike,DAM_NONE);
	        check_improve(ch,gsn_precision_strike,FALSE,1);
	}
	return;
   }
   else if ( !str_cmp( arg, "legs" ) )
   {
	send_to_char("You make an attempt to swing at your opponents legs!\n\r", ch);
	return;
   }
   else if ( !str_cmp( arg, "chest" ) )
   {
	chance = URANGE(2, chance - 5, 98);
	if (number_percent() < chance)
	{
        	act("`Y$n stuns you with a powerful punch to the chest!`w",ch,NULL,victim,TO_VICT);
        	act("`YYou stun $N with your powerful punch to the chest!`w",ch,NULL,victim,TO_CHAR);
        	act("`Y$n stuns $N with a powerful punch to the chest!.`w",ch,NULL,victim,TO_NOTVICT);
        	check_improve(ch,gsn_precision_strike,TRUE,1);
        	WAIT_STATE(victim,2 * PULSE_VIOLENCE);
        	WAIT_STATE(ch,3 * PULSE_VIOLENCE);
        	SET_BIT(victim->affected_by_2, AFF_STUN);
    	}
    	else
    	{
        	damage(ch,victim, 0 ,gsn_precision_strike,DAM_BASH);
		WAIT_STATE(ch,skill_table[gsn_precision_strike].beats*2/3);
        	check_improve(ch,gsn_precision_strike,FALSE,1);
    	}
	return;
   }
   else
   {
	send_to_char("Precision Strike where?\n\r", ch);
	return;
   }
   return;
}

void do_quick_strike ( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	victim = ch->fighting;
	if (victim == NULL)
	{
	    send_to_char("But you aren't fighting anyone!\n\r",ch);
	    return;
	}

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}
    }

    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "That would be very painful?\n\r", ch );
	return;
    }

    if ( ch->move < 30 )
    {
	send_to_char( "You do not have enough energy to follow through.\n\r", ch );
	return;
    }

    if ( is_safe( ch, victim ) )
      return;

    if ( victim->fighting != NULL && !is_same_group( ch, victim->fighting )
	&& !IS_NPC( ch ) )
    {
        send_to_char( "Kill stealing is not permitted.\n\r", ch );
        return;
    }

    if ( ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) != NULL )
      || ( ( obj = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) != NULL ) )
    {
	send_to_char( "You are wielding a weapon.  This skill requires you not use a weapon.\n\r", ch );
	return;
    }


    ch->move -= 30;
    ch->move = UMAX( ch->move, 0 );
    WAIT_STATE(ch,2 * PULSE_VIOLENCE);
    if ( !IS_AWAKE(victim)
      || IS_NPC(ch)
      || number_percent() < get_skill( ch, gsn_quick_strike ) )
    {
	check_improve( ch, gsn_quick_strike, TRUE, 1 );
	check_mv_improve( ch, 15 );
	multi_hit( ch, victim, gsn_quick_strike );
    }
    else
    {
	check_improve(ch,gsn_quick_strike,FALSE,1);
	damage( ch, victim, 0, gsn_quick_strike,DAM_NONE );
    }

    return;
}

void do_retreat( CHAR_DATA *ch, char *argument )
{
    AFFECT_DATA af;
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    char * arg;
    int door;
    int skill;
    bool found = FALSE;

    if ( ( victim = ch->fighting ) == NULL )
    {
        if ( ch->position == POS_FIGHTING )
	    set_position( ch, POS_STANDING );
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_BERSERK ) )
    {
	act( "No Surender! No Retreat! $N must die!", ch, ch->fighting, ch->fighting, TO_CHAR );
	return;
    }

    if ( ( skill = get_skill( ch, gsn_retreat ) ) < 1 )
    {
	send_to_char( "Your escape was blocked.\n\r", ch );
	return;
    }

    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	if ( rch->fighting == ch )
	{
	    found = TRUE;
	    skill -= rch->level / 8;
	}

    if ( skill < number_percent() )
    {
	send_to_char( "Your escape was blocked.\n\r", ch );
	return;
    }

    arg =  argument;

    if      ( !strncmp(arg,"north",strlen(arg) ) ) door = 0; 
    else if ( !strncmp(arg,"east" ,strlen(arg) ) ) door = 1;
    else if ( !strncmp(arg,"south",strlen(arg) ) ) door = 2;
    else if ( !strncmp(arg,"west" ,strlen(arg) ) ) door = 3;
    else if ( !strncmp(arg,"up"   ,strlen(arg) ) ) door = 4;
    else if ( !strncmp(arg,"down" ,strlen(arg) ) ) door = 5;
    else 
    {
	send_to_char("That is not a valid direction.\n\r",ch);
	return;
    }

    SET_BIT( ch->plr, PLR_FLEE );
    af.type		= gsn_retreat;
    af.level		= 100;
    af.duration		= -1;
    af.modifier		= door;
    af.location		= APPLY_NONE;
    af.location2	= APPLY_NONE;
    af.bitvector 	= 0;
    af.bit_type		= 0;
    affect_to_char(ch,&af);
    WAIT_STATE( ch, UMAX( 1, ( 100 - skill ) / 50 + 1 ) );
    send_to_char( "You call for a retreat.\n\r", ch );
    act( "$n calls for a retreat!", ch, NULL, NULL, TO_ROOM );

}

void do_flee( CHAR_DATA *ch, char *argument )
{
    EXIT_DATA * pexit;
    int door;
    bool found = FALSE;

    for ( door = 0 ; door < 6 ; door++ )
	if ( ( pexit = ch->in_room->exit[door] ) != NULL
	&&   pexit->u1.to_room != NULL
	&&   !IS_SET(pexit->exit_info, EX_CLOSED)
	&& ( !IS_NPC(ch)
	||   !IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
	    found = TRUE;

    if ( !found )
    {
	send_to_char( "You cannot find any exits to flee through.\n\r", ch );
	return;
    }

    if ( ch->fighting == NULL )
    {
        if ( ch->position == POS_FIGHTING )
	    set_position( ch, POS_STANDING );
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

    if ( IS_AFFECTED( ch, AFF_BERSERK ) )
    {
	act( "Never! $N must die!", ch, ch->fighting, ch->fighting, TO_CHAR );
	return;
    }

    if ( IS_SET( ch->plr, PLR_FLEE ) )
    {
	send_to_char( "You are already fleeing!\n\r", ch );
	return;
    }

    send_to_char( "You turn your tail and run.\n\r", ch );
    act( "$n turns $s tail and runs for $s life.", ch, NULL, NULL, TO_ROOM );
    SET_BIT( ch->plr, PLR_FLEE );
    return;
}

void flee_char( CHAR_DATA * ch )
{
    ROOM_INDEX_DATA *was_in = ch->in_room;
    ROOM_INDEX_DATA *now_in;
    CHAR_DATA *victim = ch->fighting;
    CHAR_DATA *rch;
    EXIT_DATA *pexit;
    AFFECT_DATA * paf;
    AFFECT_DATA * paf_next;
    int door,dir,num;
    bool found = FALSE;

    if ( ch->pcdata != NULL )
	ch->pcdata->home_timer = 0;

    dir = -1;

    for ( paf = ch->affected ; paf != NULL ; paf = paf_next )
    {
	paf_next = paf->next;

	if ( paf->type == gsn_retreat )
	{
	    found = TRUE;
	    dir = paf->modifier;
	    affect_remove( ch, paf );
	}
    }

    num = 0;
    if ( !found )
    for ( door = 0; door < 6; door++ )
    {
	if ( ( pexit = was_in->exit[door] ) == NULL
	||   pexit->u1.to_room == NULL
	||   IS_SET(pexit->exit_info, EX_CLOSED)
	|| ( IS_NPC(ch)
	&&   IS_SET(pexit->u1.to_room->room_flags, ROOM_NO_MOB) ) )
	    continue;

	if ( number_range( 0, num++ ) == 0 )
	    dir = door;
    }

    if ( IS_AFFECTED( ch, AFF_BERSERK ) && ch->fighting != NULL )
    {
	act( "Never! $N must die!", ch, ch->fighting, ch->fighting, TO_CHAR );
	REMOVE_BIT( ch->plr, PLR_FLEE );
	return;
    }

    if ( dir == -1 )
    {
	send_to_char( "You couldn't find an exit.\n\r", ch );
	REMOVE_BIT( ch->plr, PLR_FLEE );
	return;
    }
	
    was_in = ch->in_room;
    move_char( ch, dir, FALSE );
    REMOVE_BIT( ch->plr, PLR_FLEE );
    if ( ( now_in = ch->in_room ) == was_in )
	return;

    ch->in_room = was_in;
    act( "$n has fled!", ch, NULL, NULL, TO_ROOM );
    ch->in_room = now_in;

	    if ( victim != NULL && IS_NPC(victim) && !IS_SET( victim->act, ACT_SENTINEL ) 
	      && number_range( 0, 26 ) > get_curr_stat( ch, STAT_CHR ) )
	    {
		for ( rch = victim->in_room->people ; rch != NULL && rch->fighting != victim ; rch = rch->next_in_room );
		if ( rch == NULL )
		{
		    strncpy(victim->dislikes, ch->name,30);
		    WAIT_STATE( victim, 32 );
		}
	    }
    
    if ( IS_NPC( ch ) )
    {
        if ( IS_SET( ch->pIndexData->affected_by, AFF_HIDE ) )
		SET_BIT( ch->affected_by, AFF_HIDE );
        if ( IS_SET( ch->pIndexData->affected_by_2, AFF_FADE ) )
		SET_BIT( ch->affected_by_2, AFF_FADE );
        if ( IS_SET( ch->pIndexData->affected_by, AFF_SHADOWFORM ) )
		(*skill_table[gsn_shadowform].spell_fun) ( gsn_shadowform, ch->level, ch, ch );
    }

    stop_fighting( ch, TRUE );
    return;
}

void do_rescue( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *fch;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Rescue whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "What about fleeing instead?\n\r", ch );
	return;
    }

    if ( !is_same_group(ch,victim)
	&& !IS_NPC( ch ) )
    {
        send_to_char("Kill stealing is not permitted.\n\r",ch);
        return;
    }

    if ( !IS_NPC(ch) && IS_NPC(victim) )
    {
	send_to_char( "Doesn't need your help!\n\r", ch );
	return;
    }

    if ( ch->fighting == victim )
    {
	send_to_char( "Too late.\n\r", ch );
	return;
    }

    if ( ( fch = victim->fighting ) == NULL )
    {
	send_to_char( "That person is not fighting right now.\n\r", ch );
	return;
    }

    WAIT_STATE( ch, skill_table[gsn_rescue].beats );
    if ( !IS_NPC(ch) && number_percent( ) > get_skill( ch, gsn_rescue ) )
    {
	send_to_char( "You fail the rescue.\n\r", ch );
	check_improve(ch,gsn_rescue,FALSE,1);
	return;
    }

    act( "You rescue $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n rescues you!", ch, NULL, victim, TO_VICT    );
    act( "$n rescues $N!",  ch, NULL, victim, TO_NOTVICT );
    check_improve(ch,gsn_rescue,TRUE,1);

    stop_fighting( fch, FALSE );
    stop_fighting( victim, FALSE );

    if ( ch->fighting == NULL )
    set_fighting( ch, fch );
    if ( fch->fighting == NULL )
    set_fighting( fch, ch );
    return;
}

void do_jab( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (  get_skill( ch, gsn_jab ) <= 0 )
    {
	send_to_char(
	    "You don't know how to jab.n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}

    WAIT_STATE(ch,2 * PULSE_VIOLENCE);
    if ( IS_NPC(ch) || number_percent( ) < get_skill( ch, gsn_jab ) )
    {
	damage( ch, victim, number_range( 20, 60 ) + GET_DAMROLL( ch ), gsn_jab,DAM_BASH );
	check_improve(ch,gsn_jab,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_jab,DAM_BASH );
	check_improve(ch,gsn_jab,FALSE,1);
    }

    return;
}

void do_thrust( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (  get_skill( ch, gsn_thrust ) <= 0 )
    {
	send_to_char(
	    "You don't know how to thrust.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}

    WAIT_STATE(ch,PULSE_VIOLENCE);
    if ( IS_NPC(ch) || number_percent( ) < get_skill( ch, gsn_thrust ) )
    {
	damage( ch, victim, number_range( 15,40 ), gsn_thrust,DAM_BASH );
	check_improve(ch,gsn_thrust,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_thrust,DAM_BASH );
	check_improve(ch,gsn_thrust,FALSE,1);
    }

    return;
}

void do_whirl( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    CHAR_DATA *vch_next;
    OBJ_DATA * wield;

    if ( ( wield = get_eq_char( ch, WEAR_WIELD_R ) ) == NULL )
        if ( ( wield = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) == NULL )
            wield = get_eq_char( ch, WEAR_WIELD_L );

    if ( IS_NPC( ch ) )
	return;

    if (  get_skill( ch, gsn_whirl ) <= 0 )
    {
	send_to_char(
	    "You don't know how to whirl.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}

    WAIT_STATE(ch,3 * PULSE_VIOLENCE);
    if ( number_percent( ) < get_skill( ch, gsn_whirl ) )
    {
    	for ( vch = ch->in_room->people; vch != NULL; vch = vch_next )
    	{
            vch_next = vch->next_in_room;

	    if ( !IS_NPC( vch ) )
		continue;

	    if ( !can_see( ch, vch ) )
		continue;

    	    one_hit( ch, vch, wield, gsn_whirl );
    	}

	check_improve(ch,gsn_whirl,TRUE,1);
    }
    else
    {
	send_to_char( "Your attempt to whirl fails.\n\r", ch );
	check_improve(ch,gsn_whirl,FALSE,1);
    }

    return;
}

void do_kick( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;

    if (  get_skill( ch, gsn_kick ) <= 0 )
    {
	send_to_char(
	    "You don't know how to kick.n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}

    WAIT_STATE(ch,PULSE_VIOLENCE);
    if ( IS_NPC(ch) || number_percent( ) < get_skill( ch, gsn_kick ) )
    {
	damage( ch, victim, number_range( 1, ch->level ), gsn_kick,DAM_BASH );
	check_improve(ch,gsn_kick,TRUE,1);
    }
    else
    {
	damage( ch, victim, 0, gsn_kick,DAM_BASH );
	check_improve(ch,gsn_kick,FALSE,1);
    }

    return;
}




void do_disarm( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    OBJ_DATA *obj2;
    int chance,hth,ch_weapon,vict_weapon,ch_vict_weapon;

    hth = 0;

    if ( *argument != '\0' && ( victim = get_char_room( ch, argument ) ) == NULL )
    {
	do_disarm_trap( ch, argument );
	return;
    }

    if ((chance = get_skill(ch,gsn_disarm)) <= 0)
    {
	send_to_char( "You don't know how to disarm opponents.\n\r", ch );
	return;
    }

    if ( ( obj2 = get_eq_char( ch, WEAR_WIELD_R ) ) == NULL 
    && ( obj2 = get_eq_char( ch, WEAR_WIELD_L ) ) == NULL
    && ( obj2 = get_eq_char( ch, WEAR_WIELD_TWO_HANDED ) ) == NULL
    &&   ((hth = get_skill(ch,gsn_hand_to_hand[0])) <= 0
    ||    (IS_NPC(ch) && !IS_SET(ch->off_flags,OFF_DISARM))))
    {
	send_to_char( "You must wield a weapon to disarm.\n\r", ch );
	return;
    }

    if ( ( victim = ch->fighting ) == NULL )
    {
	send_to_char( "You aren't fighting anyone.\n\r", ch );
	return;
    }

	if ( !can_see( ch, victim ) )
	{
	    send_to_char( "You cannot see them.\n\r", ch );
	    return;
	}

    if ( ( obj = get_eq_char( victim, WEAR_WIELD_R ) ) == NULL )
      if ( ( obj = get_eq_char( victim, WEAR_WIELD_L ) ) == NULL )
        if ( ( obj = get_eq_char( victim, WEAR_WIELD_TWO_HANDED ) ) == NULL )
        {
	    send_to_char( "Your opponent is not wielding a weapon.\n\r", ch );
	    return;
        }

    /* find weapon skills */
    ch_weapon = get_weapon_skill(ch,obj2);
    vict_weapon = get_weapon_skill(victim,obj);
    ch_vict_weapon = get_weapon_skill(ch,obj);

    /* modifiers */

    /* skill */
    if ( obj2 != NULL && obj2->wear_data != NULL && obj2->wear_data->wear_loc != WEAR_WIELD_TWO_HANDED )
	chance = chance * ch_weapon/100;
    else if ( obj2 != NULL )
	chance = chance * ch_weapon/60;
    else
	chance = chance * hth/150;

    chance += (ch_vict_weapon/2 - vict_weapon) / 2; 

    /* dex vs. strength */
    chance += get_curr_stat(ch,STAT_DEX);
    chance -= 2 * get_curr_stat(victim,STAT_STR);

    /* level */
    chance += (ch->level - victim->level) * 2;
 
    /* and now the attack */
    if (number_percent() < chance)
    {
    	WAIT_STATE( ch, skill_table[gsn_disarm].beats );
	disarm( ch, victim );
	check_improve(ch,gsn_disarm,TRUE,1);
    }
    else
    {
	WAIT_STATE(ch,skill_table[gsn_disarm].beats);
	act("You fail to disarm $N.",ch,NULL,victim,TO_CHAR);
	act("$n tries to disarm you, but fails.",ch,NULL,victim,TO_VICT);
	act("$n tries to disarm $N, but fails.",ch,NULL,victim,TO_NOTVICT);
	check_improve(ch,gsn_disarm,FALSE,1);
    }
    return;
}



void do_sla( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SLAY, spell it out.\n\r", ch );
    return;
}



void do_slay( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *victim;
    char arg[MAX_INPUT_LENGTH];

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Slay whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

	if (!check_immortal_order(ch, victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    act( "You slay $M in cold blood!",  ch, NULL, victim, TO_CHAR    );
    act( "$n slays you in cold blood!", ch, NULL, victim, TO_VICT    );
    act( "$n slays $N in cold blood!",  ch, NULL, victim, TO_NOTVICT );
    raw_kill( victim, FALSE, TRUE );
    return;
}
