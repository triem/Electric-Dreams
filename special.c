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
#include <time.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"
#include "magic.h"


/*
 * The following special functions are available for mobiles.
 */
DECLARE_SPEC_FUN(	spec_breath_any		);
DECLARE_SPEC_FUN(	spec_breath_acid	);
DECLARE_SPEC_FUN(	spec_breath_fire	);
DECLARE_SPEC_FUN(	spec_breath_gas		);
DECLARE_SPEC_FUN(	spec_breath_lightning	);
DECLARE_SPEC_FUN(	spec_cast_adept		);
DECLARE_SPEC_FUN(	spec_cast_lithdor	);
DECLARE_SPEC_FUN(	spec_cast_nenkemen	);
DECLARE_SPEC_FUN(	spec_cast_maegmenel	);
DECLARE_SPEC_FUN(	spec_cast_undead	);
DECLARE_SPEC_FUN(	spec_fido		);
DECLARE_SPEC_FUN(	spec_guard		);
DECLARE_SPEC_FUN(	spec_janitor		);
DECLARE_SPEC_FUN(	spec_mayor		);
DECLARE_SPEC_FUN(	spec_coach		);
DECLARE_SPEC_FUN(	spec_viking_ship	);
DECLARE_SPEC_FUN(	spec_poison		);
DECLARE_SPEC_FUN(	spec_thief		);
DECLARE_SPEC_FUN(	spec_dung		);
DECLARE_SPEC_FUN(	spec_egg		);
DECLARE_SPEC_FUN(	spec_assassin		);
DECLARE_SPEC_FUN(	spec_thug		);
DECLARE_SPEC_FUN(	spec_nightmare		);
DECLARE_SPEC_FUN(	spec_guard_dog		);
/*
 * Given a name, return the appropriate spec fun.
 */
SPEC_FUN *spec_lookup( const char *name )
{
    if ( !str_cmp( name, "spec_breath_any"	  ) ) return spec_breath_any;
    if ( !str_cmp( name, "spec_breath_acid"	  ) ) return spec_breath_acid;
    if ( !str_cmp( name, "spec_breath_fire"	  ) ) return spec_breath_fire;
    if ( !str_cmp( name, "spec_breath_gas"	  ) ) return spec_breath_gas;
    if ( !str_cmp( name, "spec_breath_lightning"  ) ) return
							spec_breath_lightning;
    if ( !str_cmp( name, "spec_cast_adept"	  ) ) return spec_cast_adept;
    if ( !str_cmp( name, "spec_cast_lithdor"	  ) ) return spec_cast_lithdor;
    if ( !str_cmp( name, "spec_cast_nenkemen"	  ) ) return spec_cast_nenkemen;
    if ( !str_cmp( name, "spec_cast_maegmenel"	  ) ) return spec_cast_maegmenel;
    if ( !str_cmp( name, "spec_cast_undead"	  ) ) return spec_cast_undead;
    if ( !str_cmp( name, "spec_fido"		  ) ) return spec_fido;
    if ( !str_cmp( name, "spec_janitor"		  ) ) return spec_janitor;
    if ( !str_cmp( name, "spec_mayor"		  ) ) return spec_mayor;
    if ( !str_cmp( name, "spec_coach"		  ) ) return spec_coach;
    if ( !str_cmp( name, "spec_viking_ship"	  ) ) return spec_viking_ship;
    if ( !str_cmp( name, "spec_poison"		  ) ) return spec_poison;
    if ( !str_cmp( name, "spec_thief"		  ) ) return spec_thief;
 	if ( !str_cmp( name, "spec_dung"          ) ) return spec_dung;
 	if ( !str_cmp( name, "spec_egg"           ) ) return spec_egg;
 	if ( !str_cmp( name, "spec_assassin"      ) ) return spec_assassin;
 	if ( !str_cmp( name, "spec_thug"          ) ) return spec_thug;
 	if ( !str_cmp( name, "spec_nightmare"     ) ) return spec_nightmare;
	if ( !str_cmp( name, "spec_guard"	  ) ) return spec_guard;
	if ( !str_cmp( name, "spec_guard_dog"	  ) ) return spec_guard_dog;
    return 0;
}



/*
 * And now for object spec_funs
 */
DECLARE_OBJ_SPEC_FUN(	spec_blood_thirsty		);
DECLARE_OBJ_SPEC_FUN(	spec_object_protection		);


OBJ_SPEC_FUN *obj_spec_lookup( const char *name )
{
 	if ( !str_cmp( name, "spec_blood_thirsty"      ) ) return spec_blood_thirsty;
 	if ( !str_cmp( name, "spec_object_protection"  ) ) return spec_object_protection;
    return 0;
}


/*
 * Core procedure for dragons.
 */
bool dragon( CHAR_DATA *ch, char *spell_name )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    if ( ( sn = skill_lookup( spell_name ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}

/*
 * Special procedures for mobiles.
 */
bool spec_breath_any( CHAR_DATA *ch )
{
    if ( ch->position != POS_FIGHTING )
	return FALSE;

    switch ( number_bits( 3 ) )
    {
    case 0: return spec_breath_fire		( ch );
    case 1:
    case 2: return spec_breath_lightning	( ch );
    case 3: return spec_breath_gas		( ch );
    case 4: return spec_breath_acid		( ch );
    case 5:
    case 6:
    case 7: return spec_breath_fire		( ch ); 
    }

    return FALSE;
}

bool spec_breath_acid( CHAR_DATA *ch )
{
    return dragon( ch, "acid breath" );
}

bool spec_breath_fire( CHAR_DATA *ch )
{
    return dragon( ch, "fire breath" );
}

bool spec_breath_gas( CHAR_DATA *ch )
{
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    if ( ( sn = skill_lookup( "gas breath" ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, NULL );
    return TRUE;
}



bool spec_breath_lightning( CHAR_DATA *ch )
{
    return dragon( ch, "lightning breath" );
}



bool spec_cast_adept( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim != ch && can_see( ch, victim ) && number_bits( 1 ) == 0 
	     && !IS_NPC(victim) && victim->level < 11
	     && roll_dice(1,30)<=get_curr_stat(ch,STAT_CHR) )
	    break;

    }

    if ( victim == NULL )
	return FALSE;

    switch ( number_bits( 4 ) )
    {
    case 0:
	act( "$n utters the word 'abrazak'.", ch, NULL, NULL, TO_ROOM );
	spell_armor( skill_lookup( "armor" ), ch->level, ch, victim );
	return TRUE;

    case 1:
	act( "$n utters the word 'kfari'.", ch, NULL, NULL, TO_ROOM );
	spell_courage( skill_lookup( "courage" ), ch->level, ch, victim );
	return TRUE;

    case 2:
	act( "$n utters the word 'judicandus noselacri'.", ch, NULL, NULL, TO_ROOM );
	spell_remove_blindness( skill_lookup( "remove blindness" ),
	    ch->level, ch, victim );
	return TRUE;

    case 3:
	act( "$n utters the word 'judicandus dies'.", ch, NULL, NULL, TO_ROOM );
	spell_cure_light( skill_lookup( "cure light" ),
	    ch->level, ch, victim );
	return TRUE;

    case 4:
	act( "$n utters the words 'judicandus sausabru'.", ch, NULL, NULL, TO_ROOM );
	spell_remove_poison( skill_lookup( "remove poison" ),
	    ch->level, ch, victim );
	return TRUE;

    case 5:
	act( "$n utters the words 'candusima'.", ch, NULL, NULL, TO_ROOM );
	spell_refresh( skill_lookup( "refresh" ), ch->level, ch, victim );
	return TRUE;

    }

    return FALSE;
}

bool check_is_dispelled( CHAR_DATA *ch, int sn , int target )
{

    if ( !str_cmp( skill_table[sn].name, "remove blindness" ) )
    {
        if ( is_affected( ch, skill_lookup( "blinding light" ) ) )
	    return FALSE;
	else
	    return TRUE;
    }
    else if ( !str_cmp( skill_table[sn].name, "remove poison" ) )
    {
        if ( is_affected( ch, skill_lookup( "poison" ) ) )
	    return FALSE;
	else
	    return TRUE;
    }
    else if ( !str_cmp( skill_table[sn].name, "cure disease" ) )
    {
        if ( is_affected( ch, skill_lookup( "plague" ) ) )
	    return FALSE;
	else
	    return TRUE;
    }
    else if ( !str_cmp( skill_table[sn].name, "remove curse" ) )
    {
        if ( is_affected( ch, skill_lookup( "curse" ) ) )
	    return FALSE;
	else
	    return TRUE;
    }
    else if ( !str_cmp( skill_table[sn].name, "remove aura" ) )
    {
	    return TRUE;
    }
    else if ( !str_cmp( skill_table[sn].name, "dispel magic" ) )
    {
        if ( ( IS_SET( target, TAR_CHAR_OFFENSIVE ) && (
	       is_affected( ch, skill_lookup( "blinding light" ) ) 
	    || is_affected( ch, skill_lookup( "calm spirit" ) ) 
	    || is_affected( ch, skill_lookup( "control mind" ) ) 
	    || is_affected( ch, skill_lookup( "desert fist" ) ) 
	    || is_affected( ch, skill_lookup( "curse" ) ) 
	    || is_affected( ch, skill_lookup( "faerie fire" ) ) 
	    || is_affected( ch, skill_lookup( "entangle" ) ) 
	    || is_affected( ch, skill_lookup( "sleep" ) ) 
	    || is_affected( ch, skill_lookup( "weaken" ) ) ) )
	|| ( IS_SET( target, TAR_CHAR_DEFENSIVE ) && (
	       is_affected( ch, skill_lookup( "armor" ) ) 
	    || is_affected( ch, skill_lookup( "courage" ) ) 
	    || is_affected( ch, skill_lookup( "detect shadowform" ) ) 
	    || is_affected( ch, skill_lookup( "detect hidden" ) ) 
	    || is_affected( ch, skill_lookup( "fly" ) ) 
	    || is_affected( ch, skill_lookup( "levitate" ) ) 
	    || is_affected( ch, skill_lookup( "frenzy" ) ) 
	    || is_affected( ch, skill_lookup( "empower" ) ) 
	    || is_affected( ch, skill_lookup( "haste" ) ) 
	    || is_affected( ch, skill_lookup( "infravision" ) ) 
	    || is_affected( ch, skill_lookup( "shadowform" ) ) 
	    || is_affected( ch, skill_lookup( "mass shadowform" ) ) 
	    || is_affected( ch, skill_lookup( "dimension walk" ) ) 
	    || is_affected( ch, skill_lookup( "protection" ) ) 
	    || is_affected( ch, gsn_circle_of_protection ) 
	    || is_affected( ch, skill_lookup( "fire shield" ) ) 
	    || is_affected( ch, skill_lookup( "earth shield" ) ) 
	    || is_affected( ch, skill_lookup( "wind shield" ) ) 
	    || is_affected( ch, skill_lookup( "water shield" ) ) 
	    || is_affected( ch, skill_lookup( "spirit shield" ) ) 
	    || is_affected( ch, skill_lookup( "stone skin" ) ) ) ) )
	    return FALSE;
	else
	    return TRUE;
    }

    return TRUE;
}

bool spec_cast_lithdor( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  2; spell = "sand jambiya";     	break;
	case  2: min_level =  10; spell = "shatter";     	break;
	case  3: min_level =  15; spell = "desert fist";    	break;
	case  1: min_level =  23; spell = "sand blast";     	break;
	case  4: min_level =  26; spell = "sand storm"; 	break;
	default: min_level =  28; spell = "poison";          	break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}

bool spec_cast_nenkemen( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;
 
    if ( ch->position != POS_FIGHTING )
        return FALSE;
 
    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
        v_next = victim->next_in_room;
        if ( victim->fighting == ch && number_bits( 2 ) == 0 )
            break;
    }
 
    if ( victim == NULL )
        return FALSE;
    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  2; spell = "flying dagger"; 	break;
	case  1: min_level =  2; spell = "energy tap";         	break;
	case  2: min_level =  7; spell = "pain touch"; 		break;
	case  3: min_level = 12; spell = "wind vortex";     	break;
	case  4: min_level = 15; spell = "wind blade";		break;
	case  5: min_level = 30; spell = "water blast";		break;
	default: min_level = 36; spell = "maelstrom";   	break;
	}

	if ( ch->level >= min_level )
	    break;
    }
 
    if ( ( sn = skill_lookup( spell ) ) < 0 )
        return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_maegmenel( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;

    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  1; spell = "ice shards"; 		break;
	case  1: min_level =  2; spell = "energy tap";         	break;
	case  2: min_level =  4; spell = "pain touch"; 		break;
	case  3: min_level =  6; spell = "mind blast";     	break;
	case  4: min_level =  8; spell = "arrows of fire";    	break;
	case  5: min_level = 11; spell = "pyrotechnics";	break;
	case  6: min_level = 13; spell = "disintigrate";   	break;
	case  7: min_level = 14; spell = "energy drain";	break;
	default: min_level = 15; spell = "sunfire";         	break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}



bool spec_cast_undead( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    char *spell;
    int sn;

    if ( ch->position != POS_FIGHTING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;
	if ( victim->fighting == ch && number_bits( 2 ) == 0 )
	    break;
    }

    if ( victim == NULL )
	return FALSE;

    for ( ;; )
    {
	int min_level;

	switch ( number_bits( 4 ) )
	{
	case  0: min_level =  0; spell = "curse";          break;
	case  1: min_level =  3; spell = "weaken";         break;
	case  2: min_level =  6; spell = "desert fist";    break;
	case  3: min_level =  9; spell = "blinding light"; break;
	case  4: min_level = 12; spell = "poison";         break;
	case  5: min_level = 15; spell = "energy drain";   break;
	case  6: min_level = 18; spell = "harm";           break;
	case  7: min_level = 18; spell = "harm";           break;
	case  8: min_level = 20; spell = "plague";	   break;
	default: min_level = 18; spell = "harm";           break;
	}

	if ( ch->level >= min_level )
	    break;
    }

    if ( ( sn = skill_lookup( spell ) ) < 0 )
	return FALSE;
    (*skill_table[sn].spell_fun) ( sn, ch->level, ch, victim );
    return TRUE;
}

bool spec_dung( CHAR_DATA *ch )
{
    if ( roll_dice(1,60) != 1 )
	return FALSE;

    act( "$n searches for a bush, unable to find one, $n squats down right here.\n\r", ch, NULL, NULL, TO_ROOM);

    obj_to_room( create_object( get_obj_index( OBJ_VNUM_DUNG ) ) , ch->in_room ); 
    
    return TRUE;
}


bool spec_egg( CHAR_DATA *ch )
{
    OBJ_DATA *obj;

    if ( roll_dice(1,60) != 1 )
	return FALSE;

    act( "$n has just laid an egg.\n\r", ch, NULL, NULL, TO_ROOM);

    obj = create_object( get_obj_index( 1 ) );
    obj->timer = number_range( 3, 20 );
    obj_to_room( obj , ch->in_room ); 
    
    return TRUE;
}

bool spec_assassin( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *to_room;
    CHAR_DATA *victim;

    if ( ch->position != POS_STANDING && ch->fighting != NULL  )
    {
	do_stand( ch, "" );
	return TRUE;
    }

    if ( ch->hunting[0] == '\0' || ( victim = get_char_world( ch, ch->hunting ) ) == NULL 
  	|| ch->position != POS_STANDING || ch->fighting != NULL 
	|| ( ch->in_room->area == victim->in_room->area && ch->in_room != victim->in_room ) )
	return FALSE;

    if ( ch->in_room == victim->in_room )
    {
	do_say( ch, "Your soul is mine!" );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }
    for ( to_room = get_room_index( number_range( 2, top_room_index ) ) ;
	to_room == NULL || !can_see_room( ch , to_room ) || to_room->area->world != victim->in_room->area->world ;
	to_room = get_room_index( number_range( 2, top_room_index ) ) );

/* Assasins shouldn't be seen teleporting, uncomment if desired */

    act( "$n searches for $N, and then disappears.\n\r", ch, NULL, victim,TO_ROOM); 
    char_from_room( ch );
    char_to_room( ch, to_room );
    act( "$n slowly fades into existence.\n\r", ch, NULL, victim, TO_ROOM);

    return TRUE;
}

bool spec_guard_dog( CHAR_DATA * ch )
{
    CHAR_DATA * rch;
    CHAR_DATA * victim = NULL;
    int num = 0;

    for ( rch = ch->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	if ( !IS_NPC( rch ) && ch->in_room->vnum == rch->home )
	    return FALSE;

    for ( rch = ch->in_room->people ; rch != NULL ; rch =rch->next_in_room )
    {
	if ( IS_NPC( rch ) || !can_see( ch, rch ) || is_safe( ch, rch ) || IS_IMMORTAL( rch ) )
	    continue;

	if ( number_range( 0, num++ ) == 0 )	
	    victim = rch;
    }

    if ( victim != NULL )
    {
	act( "$n growls at you.", ch, victim, victim, TO_VICT );
	act( "$n growls at $N.", ch, victim, victim, TO_NOTVICT );
	act( "You growl at $N.", ch, victim, victim, TO_CHAR );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }
    
    return FALSE;
}

bool spec_thug( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *to_room;
    CHAR_DATA *victim;

    if ( ch->fighting != NULL )
	return FALSE;

    if ( ch->position != POS_STANDING )
    {
	do_stand( ch, "" );
	return TRUE;
    }

    if ( ch->hunting[0] == '\0' || ( victim = get_char_world( ch, ch->hunting ) ) == NULL 
  	|| ch->position != POS_STANDING || ch->fighting != NULL 
	|| ( ch->in_room->area == victim->in_room->area && ch->in_room != victim->in_room ) )
	return FALSE;

    if ( victim->pcdata == NULL || victim->pcdata->debt == NULL )
    {
	extract_char( ch, TRUE );
	return TRUE;
    }

    if ( ch->in_room == victim->in_room )
    {
	do_say( ch, "Pay your debt!" );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    for ( to_room = get_room_index( number_range( 2, top_room_index ) ) ;
	to_room == NULL || !can_see_room( ch , to_room ) || to_room->area->world != victim->in_room->area->world ;
	to_room = get_room_index( number_range( 2, top_room_index ) ) );

/* Assasins shouldn't be seen teleporting, uncomment if desired
    act( "$n searches for $N, and then disappears.\n\r", ch, NULL, victim,TO_ROOM); 
*/
    char_from_room( ch );
    char_to_room( ch, to_room );
/*
    act( "$n slowly fades into existence.\n\r", ch, NULL, victim, TO_ROOM);
*/

    return TRUE;
}

bool spec_nightmare( CHAR_DATA *ch )
{
    ROOM_INDEX_DATA *to_room;
    CHAR_DATA *victim;

    if ( ch->fighting != NULL )
	return FALSE;

    if ( ch->position != POS_STANDING )
    {
	do_stand( ch, "" );
	return TRUE;
    }

    if ( ch->hunting[0] == '\0' || ( victim = get_char_world( ch, ch->hunting ) ) == NULL 
  	|| ch->position != POS_STANDING || ch->fighting != NULL 
	|| ( ch->in_room->area == victim->in_room->area && ch->in_room != victim->in_room ) )
	return FALSE;

    if ( victim->pcdata == NULL )
    {
	extract_char( ch, TRUE );
	return TRUE;
    }

    if ( !IS_SET( ch->in_room->room_flags, ROOM_SAFE ) && ch->in_room == victim->in_room )
    {
	do_say( ch, "I must have your soul...the Master requires it!" );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    for ( to_room = get_room_index( number_range( 2, top_room_index ) ) ;
	to_room == NULL || !can_see_room( ch , to_room ) || to_room->area->world != victim->in_room->area->world ;
	to_room = get_room_index( number_range( 2, top_room_index ) ) );
 
    act( "$n searches for $N, and then disappears.\n\r", ch, NULL, victim,TO_ROOM); 
    char_from_room( ch );
    char_to_room( ch, to_room );
    if (ch->in_room->area == victim->in_room->area)
	send_to_char("You feel a familar presence of fear.\n\r",victim);

    act( "$n slowly fades into existence.\n\r", ch, NULL, victim, TO_ROOM);
    return TRUE;
}


bool spec_fido( CHAR_DATA *ch )
{
    OBJ_DATA *corpse;
    OBJ_DATA *c_next;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( corpse = ch->in_room->contents; corpse != NULL; corpse = c_next )
    {
	c_next = corpse->next_content;
	if ( corpse->cont_data == NULL || !is_name( "corpse", corpse->name ) )
	    continue;

	act( "$n savagely devours a corpse.", ch, NULL, NULL, TO_ROOM );
	for ( obj = corpse->cont_data->contents; obj; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    obj_from_obj( obj );
	    obj_to_room( obj, ch->in_room );
	}
	extract_obj( corpse );
	return TRUE;
    }

    return FALSE;
}



bool spec_guard( CHAR_DATA *ch )
{
    char buf[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    CHAR_DATA *ech;
    char *crime;
    int max_evil;

    if ( !IS_AWAKE(ch) || ch->fighting != NULL )
	return FALSE;

    max_evil = 300;
    ech      = NULL;
    crime    = "";

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( !IS_NPC(victim) && IS_SET(victim->plr, PLR_THIEF) )
	    { crime = "THIEF"; break; }

	if ( !IS_NPC(victim) && victim->fighting != NULL
	&&   victim->fighting != ch && IS_NPC(victim->fighting))
	{
	    ech      = victim;
	}
    }

    if ( victim != NULL )
    {
	sprintf( buf, "%s is a %s!  YOU! STOP!!",
	    victim->name, crime );
	do_yell( ch, buf );
	multi_hit( ch, victim, TYPE_UNDEFINED );
	return TRUE;
    }

    if ( ech != NULL )
    {
	act( "$n screams 'YOU! STOP!!",
	    ch, NULL, NULL, TO_ROOM );
	multi_hit( ch, ech, TYPE_UNDEFINED );
	return TRUE;
    }

    return FALSE;
}



bool spec_janitor( CHAR_DATA *ch )
{
    OBJ_DATA *trash;
    OBJ_DATA *trash_next;

    if ( !IS_AWAKE(ch) )
	return FALSE;

    for ( trash = ch->in_room->contents; trash != NULL; trash = trash_next )
    {
	trash_next = trash->next_content;
	if ( trash->wear_data == NULL || !IS_SET( trash->wear_data->wear_flags, ITEM_TAKE ) 
  	  || !can_loot(ch,trash))
	    continue;
	if ( trash->wear_data->cost < 10 )
	{
	    act( "$n picks up some trash.", ch, NULL, NULL, TO_ROOM );
	    obj_from_room( trash );
	    obj_to_char( trash, ch );
	    return TRUE;
	}
    }

    return FALSE;
}



bool spec_viking_ship( CHAR_DATA *ch )
{
    static const char open_path[] =
	"111111222222222222222222222222!";

    static const char close_path[] =
	"0000000000000000000000003333333300000000022222222211.";

    static const char *path;
    static int pos;
    static bool move;
/* s=2 n=0 e=1 w=3 */
    if ( !move )
    {
	    move = TRUE;
	    path = open_path;
	    pos  = 0;
    }

    set_position( ch, POS_STANDING );

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0', FALSE );
	break;

    case 'A':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Driver yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"New Traal Square and the Fountain!\n\r");	
	break;

    case 'B':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Drive yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"Adventurers Hall and Lockers!'\n\r");	
	break;

    case 'C':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Drive yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"Market Square!\n\r");	
	break;

    case 'Z':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Drive yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"Temple of Kalmari!\n\r");	
	break;

    case '!' :
	 path = close_path;
	 move = TRUE;
	 pos  = 0;
	break;

    case '.' :
	 path = open_path;
	 move = TRUE;
	 pos  = 0;
	break;
    }

    pos++;
    return FALSE;
}




bool spec_coach( CHAR_DATA *ch )
{
    static const char open_path[] =
	"2A1B32C222222111000333333000001111110033322222Z!";

    static const char close_path[] =
	"2A1B32C222222111000333333000001111110033322222Z.";

    static const char *path;
    static int pos;
    static bool move;

    if ( !move )
    {
	    move = TRUE;
	    path = open_path;
	    pos  = 0;
    }

    set_position( ch, POS_STANDING );

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
	move_char( ch, path[pos] - '0', FALSE );
	break;

    case 'A':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Driver yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"New Traal Square and the Fountain!\n\r");	
	break;

    case 'B':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Drive yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"Adventurers Hall and Lockers!'\n\r");	
	break;

    case 'C':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Drive yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"Market Square!\n\r");	
	break;

    case 'Z':
	act( "The Stage Coach Drive yells 'All a board!'.", ch, NULL, NULL, TO_ROOM );
	send_to_room(BEGIN_ROOM_COACH,"The Stage Coach Drive yells\n\r");	
	send_to_room(BEGIN_ROOM_COACH,"Temple of Kalmari!\n\r");	
	break;

    case '!' :
	 path = close_path;
	 move = TRUE;
	 pos  = 0;
	break;

    case '.' :
	 path = open_path;
	 move = TRUE;
	 pos  = 0;
	break;
    }

    pos++;
    return FALSE;
}




bool spec_mayor( CHAR_DATA *ch )
{
    static const char open_path[] =
	"W3a3003b33000c111d0d111Oe333333Oe22c222112212111a1S.";

    static const char close_path[] =
	"W3a3003b33000c111d0d111CE333333CE22c222112212111a1S.";

    static const char *path;
    static int pos;
    static bool move;

    if ( !move )
    {
	if ( time_info[ ch->in_room->area->world ].hour ==  time_table[ ch->in_room->area->world ].sunrise )
	{
	    path = open_path;
	    move = TRUE;
	    pos  = 0;
	}

	if ( time_info[ ch->in_room->area->world ].hour == time_table[ ch->in_room->area->world ].sunset + 1 )
	{
	    path = close_path;
	    move = TRUE;
	    pos  = 0;
	}
    }

    if ( ch->fighting != NULL )
	return spec_cast_lithdor( ch );
    if ( !move || ch->position < POS_SLEEPING )
	return FALSE;

    switch ( path[pos] )
    {
    case '0':
    case '1':
    case '2':
    case '3':
        set_position( ch, POS_STANDING );
	move_char( ch, path[pos] - '0', FALSE );
	break;

    case 'W':
	act( "$n awakens and groans loudly.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'S':
        set_position( ch, POS_STANDING );
	act( "$n lies down and falls asleep.", ch, NULL, NULL, TO_ROOM );
	break;

    case 'a':
        set_position( ch, POS_STANDING );
	act( "$n says 'Hello Honey!'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'b':
        set_position( ch, POS_STANDING );
	act( "$n says 'What a view!  I must do something about that dump!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'c':
        set_position( ch, POS_STANDING );
	act( "$n says 'Vandals!  Youngsters have no respect for anything!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'd':
	act( "$n says 'Good day, citizens!'", ch, NULL, NULL, TO_ROOM );
	break;

    case 'e':
	act( "$n says 'I hereby declare the city of Midgaard open!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'E':
	act( "$n says 'I hereby declare the city of Midgaard closed!'",
	    ch, NULL, NULL, TO_ROOM );
	break;

    case 'O':
/*	do_unlock( ch, "gate" ); */
	do_open( ch, "gate" );
	break;

    case 'C':
	do_close( ch, "gate" );
/*	do_lock( ch, "gate" ); */
	break;

    case '.' :
	move = FALSE;
	break;
    }

    pos++;
    return FALSE;
}



bool spec_poison( CHAR_DATA *ch )
{
    CHAR_DATA *victim;

    if ( ch->position != POS_FIGHTING
    || ( victim = ch->fighting ) == NULL
    ||   number_percent( ) > 2 * ch->level )
	return FALSE;

    act( "You bite $N!",  ch, NULL, victim, TO_CHAR    );
    act( "$n bites $N!",  ch, NULL, victim, TO_NOTVICT );
    act( "$n bites you!", ch, NULL, victim, TO_VICT    );
    spell_poison( gsn_poison, ch->level, ch, victim );
    return TRUE;
}



bool spec_thief( CHAR_DATA *ch )
{
    CHAR_DATA *victim;
    CHAR_DATA *v_next;
    long gold;
    int coin_type;

    if ( ch->position != POS_STANDING )
	return FALSE;

    for ( victim = ch->in_room->people; victim != NULL; victim = v_next )
    {
	v_next = victim->next_in_room;

	if ( IS_NPC(victim)
	||  IS_IMMORTAL(victim) 
	||   number_bits( 5 ) != 0 
	||   !can_see(ch,victim))
	    continue;

	if ( IS_AWAKE(victim) && number_range( 0, ch->level ) == 0 )
	{
	    act( "You discover $n's hands in your wallet!",
		ch, NULL, victim, TO_VICT );
	    act( "$N discovers $n's hands in $S wallet!",
		ch, NULL, victim, TO_NOTVICT );
	    return TRUE;
	}
	else
	{
	    for( coin_type = 0 ; coin_type < 3 ; coin_type++ )
	    {
	    	gold = victim->gold[ coin_type ] * UMIN(number_range( 1, 20 ),ch->level) / 100;
	    	gold = UMIN( gold, ch->level * ch->level * 20 );
		ch->gold[ coin_type ]     += gold;
	        victim->gold[ coin_type ] -= gold;
	    }
	}
    }

    return FALSE;
}

bool spec_blood_thirsty( OBJ_DATA *obj )
{
    CHAR_DATA *victim;
    CHAR_DATA *rch;
    CHAR_DATA *vch;

    victim = get_obj_carried_by( obj );

    if ( victim == NULL || roll_dice(1,6) != 1 )
	return FALSE;

    if ( victim->fighting != NULL && victim->fighting->hit < victim->fighting->max_hit / 4 )
    {
        act( "$p says 'Ahh, blood atlast!'\n\r", victim, obj, NULL, TO_ROOM);
        act( "$p says 'Ahh, blood atlast!'\n\r", victim, obj, NULL, TO_CHAR);
	return TRUE;
    }
    else
    {
	vch = NULL;
	for ( rch = victim->in_room->people ; rch != NULL ; rch = rch->next_in_room )
	{
	    if ( rch == victim )
		continue;

 	    if ( !IS_NPC( rch ) && !IS_NPC( victim ) && victim->level <= 10 )
		continue;

 	    if ( number_range( 0, 2 ) < 2 )
		vch = rch;
	}
	
	if ( vch != NULL )
	{
	    act( "$p says 'Give me $N's blood!'\n\r", victim, obj, vch, TO_ROOM);
	    act( "$p says 'Give me $N's blood!'\n\r", victim, obj, vch, TO_CHAR);
	}
	else
	{
	    act( "$p says 'Give me blood!'\n\r", victim, obj, NULL, TO_ROOM);
	    act( "$p says 'Give me blood!'\n\r", victim, obj, NULL, TO_CHAR);
	}
    }

    return TRUE;
}

bool spec_object_protection( OBJ_DATA *obj )
{
    CHAR_DATA *victim;

    victim = get_obj_carried_by( obj );

    if ( victim == NULL )
	return FALSE;

    if ( obj->owner[0] == '\0' )
    {
	if ( !IS_NPC(victim) )
	{
	    strncpy(obj->owner , victim->name,30 );
	}
	return FALSE;
    }

    if ( is_name( obj->owner, victim->name ) )
	return FALSE;

    act( "$p says 'Help I'm being stolen by $n!'\n\r", victim, obj, NULL, TO_ROOM);
    act( "$p says 'Help I'm being stolen by $n!'\n\r", victim, obj, NULL, TO_CHAR);

    return TRUE;
}


