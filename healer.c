/* Healer code written for Merc 2.0 muds by Alander 
   direct questions or comments to rtaylor@cie-2.uoregon.edu
   any use of this code must include this header */

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


void do_heal(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *mob;
    OBJ_DATA *obj;
    SPELL_FUN *spell;
    char *words;	
    int cost,sn;
    int type;
    int coin_type;

    /* check for healer */
    for ( mob = ch->in_room->people; mob; mob = mob->next_in_room )
    {
        if ( IS_NPC(mob) && IS_SET(mob->act, ACT_IS_HEALER) )
            break;
    }
 
    if ( mob == NULL )
    {
        send_to_char( "You can't do that here.\n\r", ch );
        return;
    }

    coin_type = race_table[ mob->race ][ mob->sex ].world; 

    one_argument(argument,arg);

    if (arg[0] == '\0')
    {
        /* display price list */
	sprintf( buf, "$N says 'I offer the following spells:' (prices in %s)",
	    coin_names[ coin_type ] );
	act(buf,ch,NULL,mob,TO_CHAR);
	send_to_char("  light:   cure light wounds     250 coins\n\r",ch);
	send_to_char("  serious: cure serious wounds   800 coins\n\r",ch);
	send_to_char("  critic:  cure critical wounds  1000 coins\n\r",ch);
	send_to_char("  heal:    healing spell         5000 coins\n\r",ch);
	send_to_char("  blind:   remove blindness      500 coins\n\r",ch);
	send_to_char("  disease: cure disease          1500 coins\n\r",ch);
	send_to_char("  poison:  remove poison         2500 coins\n\r",ch); 
	send_to_char("  uncurse: remove curse          5000 coins\n\r",ch);
	send_to_char("  refresh: restore movement      500 coins\n\r",ch);
	send_to_char("  mana:    restore mana          1000 coins\n\r",ch);
	if ( ch->level < MAX_LEVEL || IS_IMMORTAL( ch ) )
	{
	    sprintf( buf, "  cpr: corpse retrieval      %-7d coins\n\r",
	      ( ch->level < 5 ? 0 : ch->level * ( UMIN( 1, ch->level / 2 ) ) * ( UMIN( 1, ch->level / 3 ) ) * 2 ) );
	    send_to_char( buf, ch );
	}
	send_to_char("Type heal <type> to be healed.\n\r",ch);
	return;
    }

    switch (arg[0])
    {
	case 'l' :
	    spell = spell_cure_light;
	    sn    = gsn_cure_light;
	    words = "judicandus dies";
	    cost  = 250;
	    break;

	case 's' :
	    spell = spell_cure_serious;
	    sn    = gsn_cure_serious;
	    words = "judicandus gzfuajg";
	    cost  = 800;
	    break;

	case 'c' :
	    if ( arg[1] == 'r' )
	    {
	        spell = spell_cure_critical;
	        sn    = gsn_cure_critical;
	        words = "judicandus qfuhuqar";
	        cost  = 1000;
	        break;
	    }
	    else if ( arg[1] == 'p' )
	    {
		if ( ch->level  >= MAX_LEVEL )
		{
		    act("$N says 'You are too high a level for this services.'",
	                 ch,NULL,mob,TO_CHAR);
		    return;
		}

		if ( IS_NPC(ch))
		{
		    act("$N says 'Sorry you don't have a corpse to retrieve.'",
	                 ch,NULL,mob,TO_CHAR);
		    return;
		}
		if (ch->pcdata->corpse == NULL )
		{
		    for ( obj = object_list; obj; obj = obj->next)
		    {
			if ( obj->pIndexData->vnum == 11)
			{
			    sprintf(buf, "corpse of %s",ch->name);
			    if (!str_cmp(buf,obj->short_descr) )
			    {
				ch->pcdata->corpse = obj;
				break;
			    }
			}
		    }
		}

		if (ch->pcdata->corpse == NULL )
		{
		    act("$N says 'Sorry you don't seem to have a corpse to retrieve.'",
		    ch,NULL,mob,TO_CHAR);
		    return;
		}

    		coin_type = race_table[ mob->race ][ mob->sex ].world; 
		if ( ch->level > 5 )
	            cost = ch->level * ( UMIN( 1, ch->level / 2 ) ) * ( UMIN( 1, ch->level / 3 ) ) * 2;
		else
		    cost = 0;

    		if (cost > get_money_total( ch, coin_type ) )
    		{
		    act("$N says 'You do not have enough gold for my services.'",
	                 ch,NULL,mob,TO_CHAR);
		    return;
    		}

    		WAIT_STATE(ch,PULSE_VIOLENCE);
    		transfer_money( ch, mob, cost, coin_type );
	    	act("$n dances around and chants 'Suihtrup'.",mob,NULL,NULL,TO_ROOM);

		if ( number_percent() > ch->level )
  		{
    		    if ( ch->pcdata->corpse->carried_by != NULL )
    		    {
		  	obj_from_char( ch->pcdata->corpse, "T" );
			act( "$p disappears.", ch->pcdata->corpse->carried_by, ch->pcdata->corpse, ch->pcdata->corpse, TO_CHAR );
    		    }
    		    else if ( ch->pcdata->corpse->in_obj != NULL )
			obj_from_obj( ch->pcdata->corpse );
    		    else if ( ch->pcdata->corpse->in_room != NULL )
    		    {
		 	obj_from_room( ch->pcdata->corpse );
    		    }	
		obj_to_room( ch->pcdata->corpse, ch->in_room );
		act("$p appears in the room.\n\r", mob, ch->pcdata->corpse, NULL, TO_ROOM );
		}
		else
		{
		   act("$n's spell fails.", mob, NULL, ch, TO_VICT);
		}

		return;
	    }

	case 'h' :
	    spell = spell_heal;
	    sn = gsn_heal;
	    words = "pzar";
	    cost  = 5000;
	    break;

	case 'b' :
	    spell = spell_remove_blindness;
	    sn    = gsn_remove_blindness;
      	    words = "judicandus noselacri";		
            cost  = 500;
	    break;

    	case 'd' :
	    spell = spell_cure_disease;
	    sn    = gsn_cure_disease;
	    words = "judicandus eugzagz";
	    cost = 1500;
	    break;

	case 'p' :
	    spell = spell_remove_poison;
	    sn    = gsn_remove_poison;
	    words = "judicandus sausabru";
	    cost  = 2500;
	    break;
	
	case 'u' :
	    spell = spell_remove_curse; 
	    sn    = gsn_remove_curse;
	    words = "candussido judifgz";
	    cost  = 5000;
	    break;

	case 'r' :
	    spell =  spell_refresh;
	    sn    = gsn_refresh;
	    words = "candusima"; 
	    cost  = 500;
	    break;

	case 'm' :
	    spell = NULL;
	    sn = -1;
	    words = "energizer";
	    cost = 1000;
	    break;

	default :
	    act("$N says 'Type 'heal' for a list of spells.'",
	        ch,NULL,mob,TO_CHAR);
	    return;
    }

    coin_type = race_table[ mob->race ][ mob->sex ].world; 

    if (cost > get_money_total( ch, coin_type ) )
    {
	act("$N says 'You do not have enough gold for my services.'",
	    ch,NULL,mob,TO_CHAR);
	return;
    }

    WAIT_STATE(ch,PULSE_VIOLENCE);
    transfer_money( ch, mob, cost, coin_type );
  
    if (spell == NULL)  /* restore mana trap...kinda hackish */
    {
	for ( type = 0 ; type < MAX_ELEMENT_TYPE ; type++ )
	{
	    ch->mana[ type ] += roll_dice(2,8) + mob->level / 4;
	    ch->mana[ type ] = UMIN(ch->mana[ type ],ch->max_mana[ type ]);
	}
	send_to_char("A warm glow passes through you.\n\r",ch);
	return;
     }

     if (sn == -1)
	return;
     else
	say_spell( mob, ( void * ) ch, sn, TRUE );
    
     spell(sn,55,mob,ch);
}
