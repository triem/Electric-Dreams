/*************************************************************************
Locke code from the isles.  And many modifications by Reverie and Galadrail.
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
#include "magic.h"

void update_approve_data(CHAR_DATA *ch, AREA_DATA *pArea, OBJ_INDEX_DATA *pObj )
{
    /*
     * Update the changed flag for autosaves
     */
    SET_BIT( pArea->area_flags, AREA_CHANGED );

    /* 
     * The object was just modified.. Needs to be approved again
     */
    REMOVE_BIT( pObj->approve_data->flags, APPROVED);

    /*
     * Ok who modified the object and when?
     */
    strncpy( pObj->approve_data->modifier, ch->name, 30 );
    pObj->approve_data->modified_date = current_time;

    return;
}

void do_oshow( CHAR_DATA *ch, char *argument )
{
    char buf1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    OBJ_INDEX_DATA *obj;
    OBJ_DATA *obj1 = NULL;
    int chance;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
    send_to_char( "OShow what?\n\r", ch );
        return;
    }

    if ( is_number( arg ) )
    {
	if ( ( obj = get_obj_index( atoi( arg ) ) ) == NULL )
	{
	    send_to_char( "That object doesn't exist.\n\r", ch );
	    return;
	}
    }
    else if ( ( obj1 = get_obj_world( ch, argument ) ) == NULL || ( obj = obj1->pIndexData ) == NULL )
    {
	send_to_char( "That object doesn't exist.\n\r", ch );
	return;
    }

    sprintf(buf,"`R****************************************************\n\r");
    send_to_char(buf, ch);
    sprintf( buf,"`R* `wVnum: `G%ld \n\r`R* `wName: `G%s\n\r",
	obj->vnum,obj->name);
    send_to_char(buf, ch);
    sprintf( buf,"`R* `wArea: `G(%d) '%s'\n\r",
    	obj->in_area->areanum,
    	obj->in_area->name );
    send_to_char( buf, ch );
    sprintf( buf, "`R* `wShort description: `G%s\n\r`R* `wLong description: `G%s",
        obj->short_descr, obj->description );
    send_to_char( buf, ch );
    if ( obj->wear_data != NULL )
    {
    sprintf( buf, "`R* `wWear flags: `G%s `wExtra flags: `G%s `wSpec flags: `G%s `wShop Type: `G%s\n\r",
        ( obj->wear_data != NULL ? wear_bit_name_1(obj->wear_data->wear_flags) : "None" ),
        extra_bit_name( obj->extra_flags ), special_bit_name( obj->special_flags ),shop_type_name( obj->shop_type ) );
    send_to_char( buf, ch );
    sprintf( buf, "`R* `wCondition: `G%d `wMaterial: `G%s `wTimer: `G%d `wCost: `G%ld `wWeight: `G%d `wSize: `G%d\n\r",
	obj->condition, material_type_name( obj->material ),
	obj->timer, obj->wear_data->cost, obj->wear_data->weight,
	obj->wear_data->size );
    send_to_char( buf, ch );
    if ( obj->wear_data->ac != 0 )
    {
	sprintf( buf, "`R* `wArmor:`G %d\n\r", obj->wear_data->ac ); 
	    send_to_char( buf, ch );
    }
    if ( obj->wear_data->weapon_data != NULL )
    {
	sprintf( buf, "`R* `wWeapon: Damage - `G%dd%d `wType - `G%s `wDamage type - `G%s `wWeapon flags - `G%s\n\r",
	    obj->wear_data->weapon_data->dice,
	    obj->wear_data->weapon_data->die,
	    get_weapon_name(obj->wear_data->weapon_data->type),
	    get_type_weapon(obj->wear_data->weapon_data->dam_type),
	    weapon_bit_name(obj->wear_data->weapon_data->flags) );
	send_to_char( buf, ch );
    }
    if ( obj->wear_data->magic_data != NULL )
    {
    SPELL_LIST *spell;
	sprintf( buf, "`R*`w%s, magical flags : `G%s `wwith `G%d(%d) `wcharges of:\n\r",
	    magical_type_name( obj->wear_data->magic_data->type ),
	    magical_flag_name( obj->wear_data->magic_data->flags ),
	    obj->wear_data->magic_data->charges,
	    obj->wear_data->magic_data->max_charges);
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
    }
    if ( obj->cont_data != NULL )
    {
	LOCK_DATA * lock;

	sprintf( buf, "`R* `wCapacity: `G%d `wContainer Flags: `G%s\n\r",
	    obj->cont_data->capacity,
	    get_flags_container( obj->cont_data->flags ) );
	send_to_char( buf, ch );

  	if ( obj->cont_data->trap != NULL )
	{
	    SPELL_LIST * spell;

	    sprintf( buf, "`R*     `wTrap - `G%s\n\r", get_flags_trap(obj->cont_data->trap->flags ) );
	    send_to_char( buf, ch );
	    for ( spell = obj->cont_data->trap->spells ; spell != NULL ; spell = spell->next )
	    {
		sprintf( buf, "`R*          `wLevel `G%d `wspell of `G%s\n\r", spell->level, skill_table[ spell->spell ].name );
	 	send_to_char( buf, ch );
	    }
	}
	for ( lock = obj->cont_data->lock ; lock != NULL ; lock = lock->next )
	{
	    sprintf( buf, "`R*     `wLock - Key: `G%ld `wPick Penalty: `G%d `wFlags: `G%s\n\r",
		lock->vnum, lock->pick_penalty, get_flags_lock( lock->lock_info ) );
	    send_to_char( buf, ch );
	}
    }

    if ( obj->edible_data != NULL )
    {
	SPELL_LIST * spell;

	sprintf( buf, "`R* `w%s with `G%d(%d) `whours of %s. `wFlags - `G%s\n\r", 
	    ( IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) ? "Food" : "Drink" ),
	    obj->edible_data->amount, obj->edible_data->max_amount,
	    ( IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) ? "Food" : liq_table[ obj->edible_data->type ].liq_name ),
	    edible_flag_name( obj->edible_data->flags ) );
	send_to_char( buf, ch );

        if ( obj->edible_data->spells != NULL )
	{
	    sprintf( buf, "`R*  `wWith %s type spells:\n\r",
	        ( IS_SET( obj->edible_data->flags, FOOD_EDIBLE ) ? "pill" : "potion" ) );
	    send_to_char( buf, ch );
	}

	for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell->next )
	{
	    sprintf( buf, "`R*    `wLevel `G%d `wspell of `G%s.\n\r",
	    	spell->level, skill_table[ spell->spell ].name );
	    send_to_char( buf, ch );
	}
    }

    if ( obj->gate_data != NULL )
    {
	MOB_INDEX_DATA *mob;
   	OBJ_INDEX_DATA *obj2;
	ROOM_INDEX_DATA *room;

	sprintf( buf, "`R* `wGate flags: `G%s %ld\n\r",
	    gate_flag_name( obj->gate_data->flags ),obj->gate_data->flags );
	send_to_char( buf, ch );
	sprintf( buf, "`R* `wLeads to: `G%s (%ld) `w(average timer `G%d `w) vnum `G%ld\n\r",
	    ( IS_SET( obj->gate_data->flags, GATE_TO_MOB ) && ( mob = get_mob_index( obj->gate_data->vnum ) ) != NULL ? mob->short_descr :
	    ( IS_SET( obj->gate_data->flags, GATE_TO_OBJ ) && ( obj2 = get_obj_index( obj->gate_data->vnum ) ) != NULL ? obj2->short_descr :
	    (!IS_SET( obj->gate_data->flags, GATE_TO_MOB|GATE_TO_OBJ ) && ( room = get_room_index( obj->gate_data->vnum ) ) != NULL ? room->name :
	    "none" ) ) ), obj->gate_data->vnum,
	    obj->gate_data->ave_timer ,obj->gate_data->vnum);
	send_to_char( buf, ch );
    }

    if ( obj->light_data != NULL )
    {
	sprintf( buf, "`R* `wLight: Fuel - `G%d `wMax fuel - `G%d `wBrightness - `G%d `wFlags: `G%s\n\r",
	    obj->light_data->fuel,
	    obj->light_data->max_fuel,
	    obj->light_data->brightness,
	    light_flag_name( obj->light_data->flags ) );
	send_to_char( buf, ch );
    }

    if ( obj->extra_descr != NULL )
    {
        EXTRA_DESCR_DATA *ed;

        send_to_char( "`R* `wExtra description keywords: `G", ch );

        for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
        {
        send_to_char( "'", ch );
            send_to_char( ed->keyword, ch );
        send_to_char( "'", ch );
        }
        send_to_char( "\n\r", ch );
    }
    if ( obj->wear_data !=NULL )
    for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->location != APPLY_AFFECT )
           sprintf( buf, "`R* `wAffects `G%s `wby `G%d.\n\r",
	   affect_loc_name( paf ), paf->modifier );
	else
	   sprintf( buf, "`R* `wApplies `G%s `wof `G%s.\n\r",
	   get_bit_name(paf->bit_type), affect_loc_name( paf ) );
        send_to_char( buf, ch );
    }

    buf1[0] = '\0';
    if ( obj->wear_data == NULL )
       strcpy( buf1, "\n\r`R* `wNo Wear Data" );
    else if (IS_OPTIVISION(ch))
    {
	if ( obj->points < 0 )
	    strcpy( buf1, "`Gway too wimpy" );
	else if ( obj->points >= 0 && obj->points < 4 )
	    strcpy( buf1, "`Gvery wimpy" );
	else if ( obj->points >= 4 && obj->points < 8 )
	    strcpy( buf1, "`Gwimpy" );
	else if ( obj->points >= 8 && obj->points < 12 )
	    strcpy( buf1, "`Gaverage" );
	else if ( obj->points >= 12 && obj->points < 15 )
	    strcpy( buf1, "`Gpowerful" );
	else if ( obj->points >= 15 && obj->points < 20 )
	    strcpy( buf1, "`Gvery powerful" );
	else
	    strcpy( buf1, "`Gway too powerful" );
    }
    if ( obj->points < 11 )
	chance = URANGE(1, 100 - ( obj->points * 3 / 2 ), 100 );
    else
	chance = URANGE(1, 120 - ( obj->points * 8 / 2 ), 100 );
    sprintf( buf, "`R* `wPoints: `G%d[max chance for reset] (%s) `wmPoints: `G%d\n\r",
	chance, buf1, obj->mpoints );
    send_to_char(buf, ch);

    sprintf( buf, "`R* `wApproved? `G%s (%s) %s",
    	IS_SET(obj->approve_data->flags, APPROVED) ? "YES" : 
	IS_SET(obj->approve_data->flags, APPROVE_HOLD) ? "On Hold" : "NO", 
    	strlen(obj->approve_data->approver) > 1  ? obj->approve_data->approver : "System",
    	(char *) ctime( &obj->approve_data->approved_date));
    send_to_char(buf, ch);
    sprintf( buf, "`R* `wModified `G(%s) %s",
    	strlen(obj->approve_data->modifier) > 1 ? obj->approve_data->modifier : "System",
    	(char *) ctime( &obj->approve_data->modified_date));
    send_to_char(buf, ch);
    sprintf( buf, "`R* `wCreater `G(%s) %s",
    	strlen(obj->approve_data->creater) > 1  ? obj->approve_data->creater  : "System",
    	(char *) ctime( &obj->approve_data->creation_date));
    send_to_char(buf, ch);
    sprintf(buf,"`R****************************************************\n\r");
    send_to_char(buf, ch);
    return;
}

bool edit_function(CHAR_DATA *ch, int menu, int security)
{
    if (IS_NPC(ch) )
	return FALSE;
    if ( ch->pcdata->security > security )
    {
	SET_BIT(ch->edit_flags,SECURITY_CHECK);
	return FALSE;
    }
    if ( menu == OEDIT_ANY )
	return TRUE;
    if ( menu == ch->edit_menu )
	return TRUE;
    return FALSE;
}

void oedit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    AREA_DATA *pArea_to;
    OBJ_INDEX_DATA *pObj2;
    OBJ_INDEX_DATA *pObj;
    EXTRA_DESCR_DATA *ed;
    MOB_INDEX_DATA *pMobIndex;
    AFFECT_DATA *pAf;
    AFFECT_DATA *paf_next;
    char arg[MAX_STRING_LENGTH];
    char arg_all[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    int  value = 0;
    bool found = FALSE;
    bool display = FALSE;
    int  iHash;
    long vnum;
    int  x = 0;
    int  skill;
    arg2[0] = '\0';
    strcpy( arg, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );
    strcpy( arg_all, argument );

    pObj2 = (OBJ_INDEX_DATA *)ch->desc->pEdit;
    pObj = NULL;
    pArea = pObj2->in_area;

    set_obj_index_points( pObj2 );

    if ( !str_cmp( arg1, "show" ) || arg1[0] == '\0' )
    {
        sprintf( buf, "%ld", pObj2->vnum );
	set_obj_index_points( pObj2 );
        do_oshow( ch, buf );
        return;
    }


    if ( !str_cmp( arg1, "done" ) )
    {
	set_obj_index_points( pObj2 );
        ch->desc->pEdit = NULL;
        ch->desc->connected = CON_PLAYING;
	ch->edit_menu = 0;
	ch->edit_buffer[0] = '\0';
        return;
    }
    if ( !str_cmp( arg1, "?" ) )
    if (edit_function(ch, OEDIT_ANY, SECURITY_9))
    {
	sprintf(buf,"`C                      Command Oedit Menu\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`B?`w         Show current menu, status and available commands\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`B??`w        Show secondary commands for this menu\n\r");
	send_to_char(buf,ch);
	if (ch->edit_menu > 2 )
	{
	sprintf(buf,"`BBack`w      Back one menu\n\r");
	send_to_char(buf,ch);
	}

	if (ch->edit_menu == OEDIT_EDIBLE_MENU )
	{
  	if ( pObj2->edible_data == NULL )
		send_to_char("This object does not have any edible_data.\n\rTo make this object food or drinkable set the flag type with the command  `Bflag`w.\n\r\n\r", ch);
	    {
		SPELL_LIST * spell;

	       if ( pObj2->edible_data != NULL )
		{
		sprintf( buf, "`BFlags     `G%s`w\n\r", 
		    edible_flag_name( pObj2->edible_data->flags ) );
		send_to_char( buf, ch );

		sprintf( buf, "`BType      `G%s`w\n\r", 
		     IS_SET( pObj2->edible_data->flags, FOOD_EDIBLE ) ? "Food" : liq_table[ pObj2->edible_data->type ].liq_name);
		send_to_char( buf, ch );

		sprintf( buf, "`BCharges   `G%d(%d)`w\n\r", 
		    pObj2->edible_data->amount, pObj2->edible_data->max_amount);
		send_to_char( buf, ch );
	        }
		else
		{
		send_to_char("`BFlags     `GNone\n\r",ch);
		send_to_char("`w               Syntax: flag <edible flag name>\n\r",ch);
		send_to_char("`BCharges   `G0(0)\n\r",ch);
		send_to_char("`w               Syntax: hours <amount> <max amount>\n\r",ch);
		send_to_char("`BType      `GNone\n\r",ch);
		send_to_char("`w               Syntax: type <edible item type>\n\r",ch);
		}

	        if ( pObj2->edible_data != NULL )
		{
	            if ( pObj2->edible_data->spells != NULL )
		    {
			sprintf( buf,"`w(%s)\n\r",
		            ( IS_SET( pObj2->edible_data->flags, FOOD_EDIBLE ) ? "pill" : "potion" ) );
		    	send_to_char( buf, ch );
		    }
		    else
			send_to_char("`BSpells    `GNone`w\n\r",ch);
		}

	        if ( pObj2->edible_data != NULL )
	        if ( pObj2->edible_data->spells != NULL )
		for ( spell = pObj2->edible_data->spells ; spell != NULL ; spell = spell->next )
		{
		    sprintf( buf,"`BLevel `G%d `BSpell `wof `G%s.\n\r",
		    	spell->level, skill_table[ spell->spell ].name );
		    send_to_char( buf, ch );
		}
	    send_to_char( "`w            Syntax: spell <level> <spell> \n\r", ch );
	    send_to_char( "`w            (negative level removes previous spells)\n\r", ch );
	    }
	}
	
	if (ch->edit_menu == OEDIT_EXTRA_MENU )
	{
	    if ( pObj2->extra_descr != NULL )
	    {
	        send_to_char( "Extra description keywords: `G", ch );
	
	        for ( ed = pObj2->extra_descr; ed != NULL; ed = ed->next )
	        {
	        send_to_char( "'", ch );
	            send_to_char( ed->keyword, ch );
	        send_to_char( "'", ch );
	        }
	        send_to_char( "\n\r", ch );
	    }
	    else
	        send_to_char( "`GNone\n\r", ch );
            send_to_char( "`w    Syntax: add    [keyword]\n\r", ch );
            send_to_char( "            delete [keyword]\n\r", ch );
            send_to_char( "            edit   [keyword]\n\r", ch );
            send_to_char( "            format [keyword]\n\r", ch );
	}

	if (ch->edit_menu == OEDIT_CONTAINER_LOCK_MENU )
	{
  	if ( pObj2->cont_data->lock == NULL )
		send_to_char("This object does not have any locks.\n\rTo to add locks use the command `Block`w.\n\r\n\r", ch);
	{
	    LOCK_DATA *lock;

	    if ( pObj2->cont_data->lock )
	    for ( lock = pObj2->cont_data->lock ; lock != NULL ; lock = lock->next )
	    {
		sprintf( buf, "`BLock      `wKey vnum `G%ld `BPick Penalty: `G%d `BFlags: `G%s\n\r",
		lock->vnum, lock->pick_penalty, get_flags_lock( lock->lock_info ) );
		send_to_char( buf, ch );
		found = TRUE;
	    }
	    if (!found || !pObj2->cont_data->lock)
	    send_to_char("`BLocks    `GNone\n\r",ch);
            send_to_char( "`w            Syntax: lock <key vnum>\n\r", ch );
	    send_to_char( "`w            (negative key vnum to remove lock)\n\r", ch );
            send_to_char( "`w            Syntax: flag <key vnum> <lock flags>\n\r", ch );
            send_to_char( "`w            Syntax: pick <key vnum> <pick penalty>\n\r", ch );
	}
	}

	if (ch->edit_menu == OEDIT_CONTAINER_TRAP_MENU )
	{
  	if ( pObj2->cont_data->trap == NULL )
		send_to_char("This object does not have any traps.\n\rTo to add traps use the command `Bspell`w.\n\r\n\r", ch);
	{
	    SPELL_LIST * spell;

	    sprintf( buf,"`BFlags     `G%s\n\r", pObj2->cont_data->trap != NULL ? get_flags_trap(pObj2->cont_data->trap->flags ): "None" );
	    send_to_char( buf, ch );
	    send_to_char("`w             Syntax: flag <trap flag>\n\r",ch);
	    if ( pObj2->cont_data->trap )
	    for ( spell = pObj2->cont_data->trap->spells ; spell != NULL ; spell = spell->next )
	    {
		sprintf( buf, "`BSpell     `wLevel `G%d `wspell of `G%s\n\r", spell->level, skill_table[ spell->spell ].name );
	 	send_to_char( buf, ch );
		found = TRUE;
	    }
	    if (!found || !pObj2->cont_data->trap)
	    send_to_char("`BSpells   `GNone\n\r",ch);
	    send_to_char( "`w            Syntax: spell <level> <spell> \n\r", ch );
	    send_to_char( "`w            (negative level removes previous spells)\n\r", ch );
	}

	}

	if (ch->edit_menu == OEDIT_GATE_MENU )
	{
	    if ( pObj2->gate_data == NULL )
		send_to_char("This object is not a gate.  To make it a gate\n\rset the vnum with the command `Bvnum`w.\n\r\n\r", ch);
	        sprintf(buf,"`BFlags     `G%s`w\n\r",pObj2->gate_data == NULL ? "None"
		: gate_flag_name( pObj2->gate_data->flags ) ); 
	        send_to_char(buf, ch);
		send_to_char("`w             Syntax: flag <gate flag>\n\r",ch);
	        sprintf(buf,"`BVnum      `G%ld`w\n\r",pObj2->gate_data == NULL ? 0 : pObj2->gate_data->vnum );
	        send_to_char(buf, ch);
		send_to_char("`w             Syntax: vnum  <value>\n\r",ch);
	        sprintf(buf,"`BAtimer    `G%d`w\n\r",pObj2->gate_data == NULL ? 0 : pObj2->gate_data->ave_timer );
	        send_to_char(buf, ch);
		send_to_char("`w             Syntax: atimer <average timer>\n\r",ch);
	}
	if (ch->edit_menu == OEDIT_LIGHT_MENU )
	{
	    if ( pObj2->light_data == NULL )
		send_to_char("This object is not a light.  To make it a light\n\rset the fuel amounts with the command `Bfuel`w.\n\r\n\r", ch);
	        sprintf(buf,"`BFlags     `G%s`w\n\r",pObj2->light_data == NULL ? "None"
		: light_flag_name( pObj2->light_data->flags ) ); 
	        send_to_char(buf, ch);
		send_to_char("`w             Syntax: flag <light flag>\n\r",ch);
	        sprintf(buf,"`BFuel      `G%d  `gMax fuel  `G%d`w\n\r",pObj2->light_data == NULL ? 0 : pObj2->light_data->fuel,
		pObj2->light_data == NULL ? 0 : pObj2->light_data->max_fuel );
	        send_to_char(buf, ch);
		send_to_char("`w             Syntax: fuel <value> <max value>\n\r",ch);
	        sprintf(buf,"`BBright    `G%d`w\n\r",pObj2->light_data == NULL ? 0 : pObj2->light_data->brightness );
	        send_to_char(buf, ch);
		send_to_char("`w             Syntax: brightness  <percent>\n\r",ch);
	}

	if (ch->edit_menu == OEDIT_WEAR_MENU )
	{
	    if ( pObj2->wear_data == NULL )
		send_to_char("This object does not have any wear flags.  To make it a wearable\n\radd a wear flag with the command `Bflag`w.\n\r\n\r", ch);
		sprintf(buf,"`B1`w         Weapon Data            %s\n\r`w",
		    !pObj2->wear_data ? "No" : !pObj2->wear_data->weapon_data ? "No" : "Yes");
		send_to_char(buf,ch);
		sprintf(buf,"`B2`w         Magic Data             %s\n\r`w",
		    !pObj2->wear_data ? "No" : !pObj2->wear_data->magic_data ? "No" : "Yes");
		send_to_char(buf,ch);
		sprintf(buf,"`B3`w         Affect Data            %s\n\r`w",
		    !pObj2->wear_data ? "No" : !pObj2->wear_data->affected ? "No" : "Yes");
		send_to_char(buf,ch);
		sprintf(buf,"`BFlags     `G%s\n\r",pObj2->wear_data != NULL ? wear_bit_name_1(pObj2->wear_data->wear_flags) : "None");
		send_to_char(buf,ch);
		sprintf(buf,"`w                Syntax: Flag <flag name>\n\r`w");
		send_to_char(buf,ch);
		sprintf(buf,"`BSize      `G%d\n\r",
			pObj2->wear_data != NULL ? pObj2->wear_data->size : 0);
		send_to_char(buf,ch);
		sprintf(buf,"`w                Syntax: Size <value>\n\r`w");
		send_to_char(buf,ch);
		sprintf(buf,"`BCost      `G%ld\n\r",
			pObj2->wear_data != NULL ? pObj2->wear_data->cost : 0);
		send_to_char(buf,ch);
		sprintf(buf,"`w                Syntax: Cost <value>\n\r`w");
		send_to_char(buf,ch);
		sprintf(buf,"`BWeight    `G%d\n\r",
		pObj2->wear_data != NULL ? pObj2->wear_data->weight : 0);
		send_to_char(buf,ch);
		sprintf(buf,"`w                Syntax: Weight <value>\n\r`w");
		send_to_char(buf,ch);
		sprintf(buf,"`BAC        `G%d\n\r",
		pObj2->wear_data != NULL ? pObj2->wear_data->ac : 0 );
		send_to_char(buf,ch);
		sprintf(buf,"`w                Syntax: AC <value>\n\r`w");
		send_to_char(buf,ch);
	}

	if (ch->edit_menu == OEDIT_WEAR_WEAPON_MENU )
	{
	    if ( pObj2->wear_data->weapon_data == NULL )
		send_to_char("This object is not a weapon.  To make it a weapon\n\rset the `Bdamage`w on this object`w.\n\r\n\r", ch);
		sprintf(buf,"`BDam_type  `G%s\n\r",
			pObj2->wear_data->weapon_data != NULL ? get_type_weapon(pObj2->wear_data->weapon_data->dam_type) : "None" ); 
		send_to_char(buf,ch);
		send_to_char( "`w             Syntax: dam_type [number]\n\r", ch );
		sprintf(buf,"`BDamage    `GDice %d Die %d\n\r",
			pObj2->wear_data->weapon_data != NULL ? pObj2->wear_data->weapon_data->dice : 0,
			pObj2->wear_data->weapon_data != NULL ? pObj2->wear_data->weapon_data->die : 0 ); 
		send_to_char(buf,ch);
		send_to_char( "`w             Syntax: damage [dice] [die]\n\r", ch );
		sprintf(buf,"`BType      `G%s\n\r",
			pObj2->wear_data->weapon_data != NULL ? get_weapon_name(pObj2->wear_data->weapon_data->type) : "None" ); 
		send_to_char(buf,ch);
		send_to_char( "`w             Syntax: type [weapon type]\n\r", ch );
		sprintf(buf,"`BFlags     `G%s\n\r",
			pObj2->wear_data->weapon_data != NULL ? weapon_bit_name( pObj2->wear_data->weapon_data->flags) : "None" ); 
		send_to_char(buf,ch);
		send_to_char( "`w             Syntax: flag [weapon flag]\n\r", ch );
	}

	if (ch->edit_menu == OEDIT_WEAR_MAGIC_MENU )
	{
	    SPELL_LIST * spell;
	    if ( pObj2->wear_data->magic_data == NULL )
		send_to_char("This object does not have any Magic_data.\n\rTo give this object magic_data\n\rset the charges with the command  `Bcharges`w on this object`w.\n\r\n\r", ch);
		sprintf(buf,"`BFlag      `G%s\n\r`w",
		pObj2->wear_data->magic_data != NULL ? magical_flag_name(pObj2->wear_data->magic_data->flags) : "None");
		send_to_char(buf,ch);
	        send_to_char( "             Syntax: flag <magical item flag> \n\r", ch );
		sprintf(buf,"`BType      `G%s\n\r`w",
		pObj2->wear_data->magic_data != NULL ? magical_type_name(pObj2->wear_data->magic_data->type) : "None");
		send_to_char(buf,ch);
	        send_to_char( "             Syntax: flag <magical item flag> \n\r", ch );
		sprintf(buf,"`BCharges   Current `G%d `BMax `G%d\n\r`w",
		pObj2->wear_data->magic_data != NULL ? pObj2->wear_data->magic_data->max_charges : 0,
		pObj2->wear_data->magic_data != NULL ? pObj2->wear_data->magic_data->charges : 0);
		send_to_char(buf,ch);
	        send_to_char( "             Syntax: charges <current> <max> \n\r", ch );
		sprintf(buf,"`BSpells\n\r");
		send_to_char(buf,ch);
		if ( pObj2->wear_data->magic_data != NULL ) 
		{
		if (pObj2->wear_data->magic_data->spells == NULL )
		    send_to_char( "`GNone.\n\r`w", ch );
		else
		    for ( spell = pObj2->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
		    {
			sprintf( buf,"          `wLevel `G%2d `wspell of `G%s`w.\n\r",
			    spell->level, skill_table[ spell->spell ].name );
			send_to_char( buf, ch );
		    }
		}
	   	else
		    send_to_char( "`GNone`w\n\r", ch );
	    send_to_char( "`w            Syntax: spell <level> <spell> \n\r", ch );
	    send_to_char( "`w            (negative level removes previous spells)\n\r", ch );
	}

	if (ch->edit_menu == OEDIT_WEAR_AFFECT_MENU )
	{
		send_to_char("`BAffects\n\r",ch);
		found = FALSE;
		for ( pAf = pObj2->wear_data->affected; pAf != NULL; pAf = pAf->next )
		{
			if ( pAf->location != APPLY_AFFECT )
			{
			    sprintf(buf,"          `G%-12s `wby `G%3d\n\r",
			   affect_loc_name( pAf ), pAf->modifier );
		            send_to_char( buf, ch );
			    found = TRUE;
			}
		}
		    if (!found)
			send_to_char("`G          None\n\r",ch);
                send_to_char("`w               Syntax: affect <affect_name_loc> <modify number>\n\r", ch );
		found = FALSE;
		send_to_char("`BApplies\n\r",ch);
		for ( pAf = pObj2->wear_data->affected; pAf != NULL; pAf = pAf->next )
		{
			if ( pAf->location == APPLY_AFFECT )
			{
		sprintf(buf,"          `G%-12s `wof `G%s\n\r",
			   get_bit_name(pAf->bit_type),affect_loc_name( pAf) );
		        send_to_char( buf, ch );
			found = TRUE;
			}
		}
		    if (!found)
			send_to_char("`G          None\n\r",ch);
                send_to_char( "`w               Syntax: applies  <bit name> bit_type\n\r", ch );
	}

	if (ch->edit_menu == OEDIT_CONTAINER_MENU )
	{
  	if ( pObj2->cont_data == NULL )
		send_to_char("This object is not a container.\n\rTo make this object a container set the capacity with the command  `Bcapacity`w.\n\r\n\r", ch);
	sprintf(buf,"`B1`w         Trap Data                 %s\n\r`w",
		    !pObj2->cont_data ? "No" : !pObj2->cont_data->trap ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B2`w         Lock Data                 %s\n\r`w",
		    !pObj2->cont_data ? "No" : !pObj2->cont_data->lock ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`BFlags     `G%s\n\r`w",pObj2->cont_data != NULL ? get_flags_container(pObj2->cont_data->flags) : "None" );
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: flag <flag name>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BCapacity  `G%d\n\r`w", pObj2->cont_data != NULL ? pObj2->cont_data->capacity : 0);
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: capacity <value>\n\r`w");
	send_to_char(buf,ch);
	
	}

	if (ch->edit_menu == OEDIT_MENU )
	{
	sprintf(buf,"`B1`w         Wear Data            %s\n\r`w",
	    !pObj2->wear_data ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B2`w         Light Data           %s\n\r`w",
	    !pObj2->light_data ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B3`w         Container Data       %s\n\r`w",
	    !pObj2->cont_data ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B4`w         Gate Data            %s\n\r`w",
	    !pObj2->gate_data ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B5`w         Edible Data          %s\n\r`w",
	    !pObj2->edible_data ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B6`w         Extra Descriptions   %s\n\r`w",
	    !pObj2->extra_descr ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`B7`w         Inside Data          %s\n\r`w",
	    !pObj2->inside ? "No" : "Yes");
	send_to_char(buf,ch);
	sprintf(buf,"`BName      `G%s\n\r`w", pObj2->name);
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: Name <string>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BLong      `G%s`w", strlen(pObj2->description) < 2 ? "\n\r" : pObj2->description);
	send_to_char(buf,ch);  
	sprintf(buf,"`w                Syntax: Long <string>`w\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`BShort     `G%s\n\r`w", pObj2->short_descr);
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: short <string>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BFlags     `G%s\n\r`w", extra_bit_name(pObj2->extra_flags));
	send_to_char(buf,ch);
	sprintf(buf,"`BSflags    `G%s\n\r`w", special_bit_name(pObj2->special_flags));
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: flag <flag name>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BShop      `G%s\n\r`w", shop_type_name(pObj2->shop_type));
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: shop <shop name>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BMaterial  `G%s\n\r`w", material_type_name(pObj2->material));
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: material <material name>\n\r`w");
	send_to_char(buf,ch);
	sprintf(buf,"`BCondition `G%d\n\r`w", pObj2->condition);
	send_to_char(buf,ch);
	sprintf(buf,"`w                Syntax: condition <percent>\n\r`w");
	send_to_char(buf,ch);
	}
	

    display = TRUE;
    }

    if (!strncmp( arg1, "extra_descr", 8 ) || !strncmp(arg1, "6", 1) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = OEDIT_EXTRA_MENU ;
	strncpy( ch->edit_buffer, "Extra->", 80 );
	return;
    }
    if (!strncmp( arg1, "inside_menu", 8 ) || !strncmp(arg1, "7", 1) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = OEDIT_INSIDE_MENU ;
	strncpy( ch->edit_buffer, "Inside->", 80 );
	return;
    }
    if (!strncmp( arg1, "lights", 3 ) || !strncmp(arg1, "2", 1) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = OEDIT_LIGHT_MENU ;
	strncpy( ch->edit_buffer, "Lights->", 80 );
	return;
    }

    if (!strncmp( arg1, "weardata", 3 ) || !strncmp(arg1, "1", 1 ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = OEDIT_WEAR_MENU ;
	strncpy(ch->edit_buffer, "Wear->", 80 );
	return;
    }
    if (!strncmp( arg1, "trap_data", 4 ) || !strncmp(arg1, "1", 1 ) )
    if (edit_function(ch, OEDIT_CONTAINER_MENU, SECURITY_9))
    {
	    if ( !pObj2->cont_data )
	    {
		send_to_char ("This object does not have any container data.\n\r ", ch) ;
		send_to_char ("It must be a container before you can add traps..\n\r ", ch) ;
		return;
	    }
	
	ch->edit_menu = OEDIT_CONTAINER_TRAP_MENU ;
	strncpy( ch->edit_buffer, "Container->Traps", 80 );
	return;
    }
    if (!strncmp( arg1, "lock_data", 4 ) || !strncmp(arg1, "2", 1 ) )
    if (edit_function(ch, OEDIT_CONTAINER_MENU, SECURITY_9))
    {
	    if ( !pObj2->cont_data )
	    {
		send_to_char ("This object does not have any container data.\n\r ", ch) ;
		send_to_char ("It must be a container before you can add locks..\n\r ", ch) ;
		return;
	    }
	
	
	ch->edit_menu = OEDIT_CONTAINER_LOCK_MENU ;
	strncpy( ch->edit_buffer, "Container->Locks", 80 );
	return;
    }
    if (!strncmp( arg1, "container_data", 9 ) || !strncmp(arg1, "3", 1 ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = OEDIT_CONTAINER_MENU ;
	strncpy( ch->edit_buffer, "Container->", 80 );
	return;
    }
    if (!strncmp( arg1, "edible_data", 6 ) || !strncmp(arg1, "5", 1 ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = OEDIT_EDIBLE_MENU ;
	strncpy( ch->edit_buffer, "Edible->", 80 );
	return;
    }
    if (!strncmp( arg1, "gate_data", 7 ) || !strncmp(arg1, "4", 1 ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	
	ch->edit_menu = OEDIT_GATE_MENU ;
	strncpy( ch->edit_buffer ,"Gates->", 80 );
	return;
    }

    if (!strncmp( arg1, "weapon_data", 4 ) || !strncmp( arg1, "1" ,1 ) )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
	    if ( !pObj2->wear_data )
	    {
		send_to_char ("This object does not have any wear data.\n\r ", ch) ;
		send_to_char ("It must have a wear location added first.\n\r ", ch) ;
		send_to_char ("Example: take and wield\n\r ", ch) ;
		return;
	    }
	    ch->edit_menu = OEDIT_WEAR_WEAPON_MENU ;
	    strncpy( ch->edit_buffer, "Wear->Weapon->", 80 );
	    return;
    }
    if (!strncmp( arg1, "magic_data", 4 ) || !strncmp( arg1, "2", 1) )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
	    if ( !pObj2->wear_data )
	    {
		send_to_char ("This object does not have any wear data.\n\r ", ch) ;
		send_to_char ("It must have a wear location added first.\n\r ", ch) ;
		send_to_char ("Example: take and wield\n\r ", ch) ;
		return;
	    }
	    ch->edit_menu = OEDIT_WEAR_MAGIC_MENU ;
	    strncpy( ch->edit_buffer, "Wear->Magic->", 80 );
	    return;
    }
    if (!strncmp( arg1, "affect_data", 8 ) || !strncmp( arg1, "3", 1) )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
	    if ( !pObj2->wear_data )
	    {
		send_to_char ("This object does not have any wear data.\n\r ", ch) ;
		send_to_char ("It must have a wear location added first.\n\r ", ch) ;
		send_to_char ("Example: take and wield\n\r ", ch) ;
		return;
	    }
	    ch->edit_menu = OEDIT_WEAR_AFFECT_MENU ;
	    strncpy( ch->edit_buffer, "Wear->Affects->", 80 );
	    return;
    }
    if ( !strncmp( arg1, "skilllist",6 ) )
    if (edit_function(ch, OEDIT_WEAR_AFFECT_MENU, SECURITY_9))
    {
        
		for ( skill = 0; skill < MAX_SKILL; skill++ )
		{
		    if ( !skill_table[skill].name)
			continue;
		    if ((skill_table[skill].spell_fun) == spell_null)
			{
			 x++;
			 if (x == 1) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			 if (x == 2) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x == 3)
				{
	    			sprintf(buf,"%2d : %-17s\n\r",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x > 2) x = 0;
			}

		}


        send_to_char( "\n\r", ch);
        return;
    }
    if ( !str_cmp( arg1, "delaffect" ) || !str_cmp( arg1, "remaffect" ) )
    if (edit_function(ch, OEDIT_WEAR_AFFECT_MENU, SECURITY_9))
    {
	if ( pObj2->wear_data == NULL )
	{
	    send_to_char( "That cannot be worn.\n\r", ch );
	    return;
	}
	if ( arg2[0] == '\0' )
	{
	send_to_char("syntax: delaffect <name>\n\r",ch);
	return;
	}

	if (pObj2->wear_data->affected == NULL  )
	{
	send_to_char("No affects to remove\n\r",ch);
	return;
	}	

	pAf = pObj2->wear_data->affected;

	if ( !str_cmp(arg2, affect_loc_name( pAf ) ) )
	{
	    pObj2->wear_data->affected = pAf->next;
	    free_affect(pAf);
       	    send_to_char( "Affect removed.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}

	for ( pAf = pObj2->wear_data->affected; pAf != NULL; pAf = paf_next )
	{
	    paf_next = pAf->next;
	    if (paf_next && !str_cmp(arg2, affect_loc_name( paf_next ) ) )
	    {
		pAf->next = paf_next->next;
		free_affect(paf_next);
        	send_to_char( "Affect removed.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
		return;
	    }
	}
        send_to_char( "No affect found.\n\r", ch );
        return;
    }

    if ( !strncmp( arg1, "applies", 3 ) || !strncmp(arg1, "apply", 3 ) )
    if (edit_function(ch, OEDIT_WEAR_AFFECT_MENU, SECURITY_9))
    {

	if ( pObj2->wear_data == NULL )
	{
	    send_to_char( "That cannot be worn.\n\r", ch );
	    return;
	}

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );


        if ( arg1[0] == '\0' || arg2[0] == '\0' )
	{
		send_to_char( "Syntax: apply <bit name> [bit_type]\n\r", ch );
		send_to_char( "bit_type `BBIT_IMM`w\n\r", ch );
		sprintf( buf, "`YBit_names: `w%s\n\r", imm_bit_name( ALL_FLAGS ) );
		strcpy( buf, format_string( str_dup( buf ) ) );
		send_to_char( buf, ch );
		send_to_char( "bit_type `BBIT_RES`w\n\r", ch );
		sprintf( buf, "`YBit_names: `w%s\n\r", res_bit_name( ALL_FLAGS ) );
		strcpy( buf, format_string( str_dup( buf ) ) );
		send_to_char( buf, ch );
		send_to_char( "bit_type `BBIT_VULN`w\n\r", ch );
		sprintf( buf, "`YBit_names: `w%s\n\r", vuln_bit_name( ALL_FLAGS ) );
		strcpy( buf, format_string( str_dup( buf ) ) );
		send_to_char( buf, ch );
		send_to_char( "bit_type `BBIT_AFFECT`w\n\r", ch );
		sprintf( buf, "`YBit_names: `w%s\n\r", affect_bit_name( ALL_FLAGS ) );
		strcpy( buf, format_string( str_dup( buf ) ) );
		send_to_char( buf, ch );
		send_to_char( "bit_type `BBIT_AFFECT2`w\n\r", ch );
		sprintf( buf, "`YBit_names: `w%s\n\r", affect_bit_name_2( ALL_FLAGS ) );
		strcpy( buf, format_string( str_dup( buf ) ) );
		send_to_char( buf, ch );
		return;
	}  
    	    if ( !str_cmp( arg2, "bit_affect" ) )
	    {
		if ( affect_name_bit( arg1 ) != 0 )
		{
            	    pAf             =   new_affect(FALSE);
	   	    pAf->location = APPLY_AFFECT;
            	    pAf->modifier   =   0;
		    pAf->bitvector  =   affect_name_bit( arg1 );
		    pAf->bit_type   = BIT_AFFECT;
		}
		else
	        {
		    send_to_char( "Invalid affect bit\n\r", ch );
		    return;
	        }
	    }
	    else
    	    if ( !str_cmp( arg2, "bit_affect2" ) )
	    {
		if ( affect_name_bit_2( arg1 ) != 0 )
		{
            	    pAf             =   new_affect(FALSE);
	   	    pAf->location = APPLY_AFFECT; 
            	    pAf->modifier   =   0;
		    pAf->bitvector  =   affect_name_bit_2( arg1 );
		    pAf->bit_type   = BIT_AFFECT2;
		}
		else
	        {
		    send_to_char( "Invalid affect2 bit\n\r", ch );
		    return;
	        }
	    }
	    else
    	    if ( !str_cmp( arg2, "bit_vuln" ) )
	    {
		if (vuln_name_bit( arg1 ) != 0 )
		{
            	    pAf             =   new_affect(FALSE);
	   	    pAf->location = APPLY_AFFECT;
            	    pAf->modifier   =   0;
		    pAf->bitvector  =   vuln_name_bit( arg1 );
		    pAf->bit_type   = BIT_VULN;
		}
		else
	        {
		    send_to_char( "Invalid vuln bit\n\r", ch );
		    return;
	        }
	    }
	    else
    	    if ( !str_cmp( arg2, "bit_res" ) )
	    {
		if (res_name_bit( arg1 ) != 0 )
		{
            	    pAf             =   new_affect(FALSE);
	   	    pAf->location = APPLY_AFFECT;
            	    pAf->modifier   =   0;
		    pAf->bitvector  =   res_name_bit( arg1 );
		    pAf->bit_type   = BIT_RES;
		}
		else
	        {
		    send_to_char( "Invalid res bit\n\r", ch );
		    return;
	        }
	    }
	    else
    	    if ( !str_cmp( arg2, "bit_imm" ) )
	    {
		if (imm_name_bit( arg1 ) != 0 )
		{
            	    pAf             =   new_affect(FALSE);
	   	    pAf->location = APPLY_AFFECT;
            	    pAf->modifier   =   0;
		    pAf->bitvector  =   imm_name_bit( arg1 );
		    pAf->bit_type   = BIT_IMM;
		}
		else
	        {
		    send_to_char( "Invalid imm bit\n\r", ch );
		    return;
	        }
	    }
	    else
	    {
            	send_to_char( "Invalid bit\n\r", ch );
            	return;
	    }

        pAf->duration   =   -1;
        pAf->type       =   -1;
        pAf->next       =   pObj2->wear_data->affected;
        pObj2->wear_data->affected  =   pAf;

        if ( !IS_SET(pObj2->extra_flags, ITEM_MAGIC) )
	    SET_BIT(pObj2->extra_flags, ITEM_MAGIC);

        send_to_char( "Affect added.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "affects" , 4 ) )
    if (edit_function(ch, OEDIT_WEAR_AFFECT_MENU, SECURITY_9))
    {

	if ( pObj2->wear_data == NULL )
	{
	    send_to_char( "That cannot be worn.\n\r", ch );
	    return;
	}

        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );

        if ( arg1[0] == '\0' || ( affect_name_loc( arg1 ) == 0 ) )
        {
            send_to_char( "Syntax: affect <affect_name_loc> <modify number>\n\r", ch );
	    for ( x = 1; x < APPLY_MAX; x++ )
	    {
	    sprintf(buf, "%s", affect_name(x) );
	    send_to_char(buf,ch);
	    }
	    send_to_char("\n\r",ch);
            return;
        }


        if ( affect_name_loc(arg1) == APPLY_AFFECT )
	{
            send_to_char( "Syntax: affect   <affect_name_loc> <modify number>\n\r", ch );
            send_to_char( "Syntax: applies  <bit name> bit_type\n\r", ch );
	    return;
	    
	}
	else 
	{
	    if ( !is_number(arg2) )
		{
            	send_to_char( "Invalid mod number\n\r", ch );
            	return;
		}
            pAf             =   new_affect(FALSE);
	    pAf->location = affect_name_loc(arg1);
            pAf->modifier   =   atoi( arg2 );
            pAf->bitvector  =   0;
	}
        pAf->duration   =   -1;
        pAf->type       =   -1;
        pAf->next       =   pObj2->wear_data->affected;
        pObj2->wear_data->affected  =   pAf;

        if ( !IS_SET(pObj2->extra_flags, ITEM_MAGIC) )
	    SET_BIT(pObj2->extra_flags, ITEM_MAGIC);

        send_to_char( "Affect added.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_INSIDE_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_CONTAINER_TRAP_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_CONTAINER_MENU ;
		strncpy( ch->edit_buffer, "Container->", 80 );
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_CONTAINER_LOCK_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_CONTAINER_MENU ;
		strncpy( ch->edit_buffer ,"Container->", 80 );
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_EXTRA_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_EDIBLE_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_CONTAINER_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_WEAR_AFFECT_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_WEAR_MENU ;
		strncpy( ch->edit_buffer, "Wear->", 80 );
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_GATE_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_CONTAINER_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_MENU ;
	        ch->edit_buffer[0] = '\0';
		return;
    }

    if ( !strncmp( arg1, "spelllist",6 ) )
    if (edit_function(ch, OEDIT_EDIBLE_MENU, SECURITY_9))
    {
        
		for ( skill = 0; skill < MAX_SKILL; skill++ )
		{
		    if ((skill_table[skill].spell_fun) != spell_null)
			{
			 x++;
			 if (x == 1) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			 if (x == 2) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x == 3)
				{
	    			sprintf(buf,"%2d : %-17s\n\r",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x > 2) x = 0;
			}

		}


        send_to_char( "\n\r", ch);
        return;
    }
    if ( !strncmp( arg1, "spelllist",6 ) )
    if (edit_function(ch, OEDIT_WEAR_MAGIC_MENU, SECURITY_9))
    {
        
		for ( skill = 0; skill < MAX_SKILL; skill++ )
		{
		    if ((skill_table[skill].spell_fun) != spell_null)
			{
			 x++;
			 if (x == 1) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			 if (x == 2) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x == 3)
				{
	    			sprintf(buf,"%2d : %-17s\n\r",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x > 2) x = 0;
			}

		}


        send_to_char( "\n\r", ch);
        return;
    }
    if ( !strncmp( arg1, "spelllist",6 ) )
    if (edit_function(ch, OEDIT_CONTAINER_TRAP_MENU, SECURITY_9))
    {
        
		for ( skill = 0; skill < MAX_SKILL; skill++ )
		{
		    if ((skill_table[skill].spell_fun) != spell_null)
			{
			 x++;
			 if (x == 1) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			 if (x == 2) 
				{
		    		sprintf(buf,"%2d : %-17s  ",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x == 3)
				{
	    			sprintf(buf,"%2d : %-17s\n\r",skill,skill_table[skill].name);
	    			send_to_char(buf,ch);
				}
			if (x > 2) x = 0;
			}

		}


        send_to_char( "\n\r", ch);
        return;
    }
    if ( !str_cmp( arg1, "charges" ) )
    if (edit_function(ch, OEDIT_WEAR_MAGIC_MENU, SECURITY_9))
    {
        argument = one_argument( arg2, buf );
	strcpy( arg2, buf );
	strcpy( arg3, argument );

	if ( arg3[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Usage: charges <current> <max> \n\r", ch );
	    return;
	}

	if ( pObj2->wear_data == NULL )
	{
	    send_to_char( "That cannot be used as a magical item.\n\r", ch );
	    return;
	}

	if ( pObj2->wear_data->magic_data == NULL )
	    pObj2->wear_data->magic_data = magic_alloc();

	pObj2->wear_data->magic_data->charges = atoi( arg2 );
	pObj2->wear_data->magic_data->max_charges = atoi( arg3 );

	if ( pObj2->wear_data->magic_data->charges == 0
	  && pObj2->wear_data->magic_data->max_charges == 0 )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spell_next;
	
	    for ( spell = pObj2->wear_data->magic_data->spells ; spell != NULL ; spell = spell_next )
	    {
		spell_next = spell->next;
		free_spell( spell );
	    }

	    free_magic_data( pObj2->wear_data->magic_data );
	    pObj2->wear_data->magic_data = NULL;

            send_to_char( "Magical ability removed.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
            return;
	}

        send_to_char( "Charges set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "spells" , 3) )
    if (edit_function(ch, OEDIT_WEAR_MAGIC_MENU, SECURITY_9))
    {
	int level,sn;

        argument = one_argument( arg2, buf );
	strcpy( arg2, buf );
	strcpy( arg3, argument );

	if ( arg3[0] == '\0' || arg2[0] == '\0' )
	{
	    send_to_char( "Usage: spell <level> <spell> \n\r", ch );
	    send_to_char( "(negative level removes previous spells)\n\r", ch );
	    send_to_char( "`BSpelllist `w for a list of possible spells.\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( arg3 ) ) == -1 )
	{
	    send_to_char( "Invalid spell!\n\r", ch );
	    return;
	}

	if ( pObj2->wear_data == NULL )
	{
	    send_to_char( "That cannot be used as a magical item.\n\r", ch );
	    return;
	}

	if ( pObj2->wear_data->magic_data == NULL )
	    pObj2->wear_data->magic_data = magic_alloc();

	if ( ( level = atoi( arg2 ) ) < 0 )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spell_next;

	    if ( pObj2->wear_data->magic_data->spells == NULL )
	    {
		send_to_char( "This items contains no spells to erase.\n\r", ch );
		return;
	    }

	    if ( pObj2->wear_data->magic_data->spells->spell == sn )
	    {
		spell = pObj2->wear_data->magic_data->spells;
		pObj2->wear_data->magic_data->spells = pObj2->wear_data->magic_data->spells->next;
		free_spell( spell );
	        send_to_char( "Magical spell removed.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
	        return;
	    }
	    else
	        for ( spell = pObj2->wear_data->magic_data->spells , spell_next = spell->next ; spell_next != NULL ; spell = spell_next )
	        {
		    spell_next = spell->next;

		    if ( spell_next->spell == sn )
		    {
			spell->next = spell_next->next;
			free_spell( spell_next );
		        send_to_char( "Magical spell removed.\n\r", ch);
			update_approve_data(ch, pArea, pObj2);
		        return;
		    }
	        }	

	    send_to_char( "Spell not found.\n\r", ch );
	    return;
	}
	else
	{
	    SPELL_LIST * spell;
	
	    spell = spell_alloc();
	    spell->level = level;
	    spell->spell = sn;
	    spell->next = pObj2->wear_data->magic_data->spells;
	    pObj2->wear_data->magic_data->spells = spell;
	}
	
        send_to_char( "Magical spell added.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "type", 3 ) )
    if (edit_function(ch, OEDIT_WEAR_MAGIC_MENU, SECURITY_9))
    {
       if (pObj2->wear_data == NULL)
        {
         send_to_char ("That object cannot be used magically! \n \r ", ch) ;
             return ;
        }
	if ( arg2[0] == '\0' )
	{
	    for ( x = 1; x < MAGIC_TYPE_MAX; x++ )
		send_to_char(magical_type_name(x),ch);
	    send_to_char("\n\r",ch);
	    send_to_char("syntax: type < magic type>\n\r",ch);
	    return; 
	}
       if (pObj2->wear_data->magic_data == NULL)
	    pObj2->wear_data->magic_data = magic_alloc();

	if ( get_magical_type ( arg2 ) == 0 )
	{
	     send_to_char("Invalid magical type.\n\r",ch);
	    for ( x = 1; x < MAGIC_TYPE_MAX; x++ )
		send_to_char(magical_type_name(x),ch);
	    send_to_char("\n\r",ch);
	    send_to_char("syntax: type < magic type>\n\r",ch);
	}
	
        pObj2->wear_data->magic_data->type = get_magical_type( arg2 );

        send_to_char( "Magical type set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "flags", 4 ) )
    if (edit_function(ch, OEDIT_WEAR_MAGIC_MENU, SECURITY_9))
    {

	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Usage: flag <magical item flag> \n\r", ch );
	    sprintf(buf,"Flags: %s\n\r",magical_flag_name(ALL_FLAGS) );
	    send_to_char(buf,ch);
	    return;
	}

	if ( pObj2->wear_data == NULL )
	{
	    send_to_char( "That cannot be used as a magical item.\n\r", ch );
	    return;
	}

	if ( get_magical_flag( arg2 ) == 0 )
	{
	    send_to_char( "Invalid magical item flag.\n\r", ch );
	    return;
	}

	if ( pObj2->wear_data->magic_data == NULL )
	    pObj2->wear_data->magic_data = magic_alloc();

	pObj2->wear_data->magic_data->flags ^= get_magical_flag( arg2 );

        send_to_char( "Magical flag toggled.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_WEAR_MAGIC_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_WEAR_MENU ;
		strncpy( ch->edit_buffer, "Wear->", 80 );
		return;
    }


    if ( !strncmp( arg1, "damage", 4 ) )
    if (edit_function(ch, OEDIT_WEAR_WEAPON_MENU, SECURITY_9))
    {
  
		argument = one_argument( argument, arg1 );
		argument = one_argument( argument, arg2 );

		if ( arg2[0] == '\0' || !is_number( arg2 ) || 
		    arg1[0] == '\0' || !is_number( arg1 ) )
		{
		    send_to_char( "Syntax:damage [dice] [die]\n\r", ch );
		    return;
		}

		if ( pObj2->wear_data == NULL )
		{
		    send_to_char( "That cannot be worn.\n\r", ch );
		    return;
		}

		if ( pObj2->wear_data->weapon_data == NULL )
		    pObj2->wear_data->weapon_data = weapon_alloc();

		pObj2->wear_data->weapon_data->dice = atoi( arg1 );
		pObj2->wear_data->weapon_data->die  = atoi( arg2 );

		if ( pObj2->wear_data->weapon_data->dice == 0
		    && pObj2->wear_data->weapon_data->die == 0 )
		{
		    free_weapon_data( pObj2->wear_data->weapon_data );
		    pObj2->wear_data->weapon_data = NULL;

		    update_approve_data(ch, pArea, pObj2);
		    send_to_char( "Weapon ability removed.\n\r", ch );
		    return;
		}

		send_to_char( "Weapon damage set.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
		return;
    }
    if ( !str_cmp( arg1, "events" ) )
    if (edit_function(ch, OEDIT_ANY, SECURITY_9))
    {
	EVENT_DATA   *event;
	BUFFER *output;
	output = new_buf();

	for ( event = pArea->events; event != NULL; event = event->next )
	{
	    if ( event->obj_vnum == pObj2->vnum)
	    {
		sprintf(buf,"`G%-4ld `wChance `G%-3d `wMob `G%-5ld `wObj `G%-5ld `wRoom `G%-5ld `wFlags `G%s`w\n\r",
		event->vnum, event->chance, event->mob_vnum, event->obj_vnum, event->room_vnum, get_event_flags(event->flags));
		add_buf(output, buf);
	    }
	}
	page_to_char(buf_string(output),ch);
	free_buf(output);
	return;
    }
    if ( !str_cmp( arg1, "equip" ) )
    if (edit_function(ch, OEDIT_ANY, SECURITY_9))
    {
	EQUIP_DATA *equip;
	BUFFER *output;
	output = new_buf();
	for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
	{
	    for ( pMobIndex = mob_index_hash[vnum]; pMobIndex != NULL; pMobIndex = pMobIndex->next )
	    {
		for ( equip = pMobIndex->equip; equip != NULL; equip = equip->next )
		{
		    if ( equip->obj_vnum == pObj2->vnum )
		    {
			sprintf(buf, "`wMob `G%-5ld  `wChance `G%-3d  `wFlags `G%s\n\r",
			pMobIndex->vnum, equip->chance, get_equip_flags(equip->flags) );
			add_buf(output, buf);
		    }
		}
	    }
	}

	page_to_char(buf_string(output),ch);
	free_buf(output);
	return;
    }
    if ( !strncmp( arg1, "inside", 3 ) || !str_cmp( arg1, "?") )
    if (edit_function(ch, OEDIT_INSIDE_MENU, SECURITY_9))
    {
	INSIDE_DATA * inside;
	INSIDE_DATA * inside_data;
	int flag;
    	if ( !strncmp( arg1, "?", 3 ) )
	{
	    display = TRUE; 
	    send_to_char("`BInside      `w Load objects or mobs inside this object\n\r",ch);	
	    send_to_char("`w                  Syntax: inside <vnum> chance <chance>\n\r",ch);	
	    send_to_char("`w                  Syntax: inside <vnum> flags <flags>\n\r",ch);	
	    send_to_char("`w                  Syntax: inside <vnum> remove\n\r",ch);	
	    send_to_char("`w                  Syntax: inside <vnum> \n\r",ch);	
	    send_to_char("`w                  Syntax: inside list \n\r",ch);	
	    display = TRUE;
	}
	else
	{
        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
        argument = one_argument( argument, arg3 );
	if ( arg1[0] == '\0' )
	{
	    send_to_char("Syntax:inside <vnum> chance <chance>\n\r",ch);	
	    send_to_char("Syntax:inside <vnum> flags <flags>\n\r",ch);	
	    send_to_char("Syntax:inside <vnum> remove\n\r",ch);	
	    send_to_char("Syntax:inside <vnum> \n\r",ch);	
	    send_to_char("Syntax:inside list \n\r",ch);	
	    sprintf(buf,get_equip_flags(ALL_FLAGS)); 
	    send_to_char(buf,ch);
	    send_to_char("\n\r",ch);
	    for ( inside = pObj2->inside; inside != NULL ; inside = inside->next )
	    {
		sprintf(buf, "`wObj `G%-5ld  `wMob `G%-5ld `wChance `G%-3d  `wFlags `G%s\n\r",
		inside->obj_vnum, inside->mob_vnum,  inside->chance, get_equip_flags(inside->flags) );
		send_to_char(buf,ch);
	    }
	    return;
	}
    	if ( !strncmp( arg1, "list", 3 ) )
	{
	    for ( inside = pObj2->inside; inside != NULL ; inside = inside->next )
	    {
		sprintf(buf, "`wObj `G%-5ld  `wMob `G%-5ld `wChance `G%-3d  `wFlags `G%s\n\r",
		inside->obj_vnum, inside->mob_vnum,  inside->chance, get_equip_flags(inside->flags) );
		send_to_char(buf,ch);
	    }
	    return;
	}
	if ( arg2[0] == '\0' && is_number(arg1) )
	{
	    inside = inside_alloc();
	    inside->obj_vnum = atoi(arg1); 
	    inside->flags = EQUIP_ON_BOOT|EQUIP_ON_CREATION;
	    inside->next = pObj2->inside;
	    pObj2->inside = inside; 
	    send_to_char("New inside Created\n\r",ch);	
	    update_approve_data(ch, pArea, pObj2);
	    return; 
	}
	if ( !strncmp( arg2, "remove", 3 ) )
	{
	    inside_data = NULL;

	    for ( inside = pObj2->inside; inside != NULL ; inside = inside->next )
	    {
		if ( inside->obj_vnum == atoi( arg1 ) )
		    break;
		inside_data = inside;
	    }
	    if ( inside == NULL )
	    {
		send_to_char("Not found!\n\r",ch);	
		return;
	    }
	    if ( inside_data == NULL )
	    {
		pObj2->inside = inside->next;
	    }
	    else
	    {
		inside_data->next = inside->next;
	    }

	    free_inside_data( inside );

	    send_to_char( "Inside removed.\n\r", ch );
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}
	if ( arg3[0] == '\0' )
	{
	    send_to_char("Syntax:inside <vnum> chance <chance>\n\r",ch);	
	    send_to_char("Syntax:inside <vnum> flags <flags>\n\r",ch);	
	    send_to_char("Syntax:inside <vnum> remove\n\r",ch);	
	    sprintf(buf,get_equip_flags(ALL_FLAGS)); 
	    send_to_char(buf,ch);
	    return;
	}

	found = FALSE;
	for ( inside = pObj2->inside; inside != NULL ; inside = inside->next )
	{
	    if ( inside->obj_vnum == atoi( arg1 ) )
	    {
		found = TRUE;
		break;
	    }
	}

	if ( inside == NULL || !found )
	{
	    inside = inside_alloc();
	    inside->obj_vnum = atoi(arg1); 
	    inside->flags = EQUIP_ON_BOOT|EQUIP_ON_CREATION;
	    inside->next = pObj2->inside;
	    pObj2->inside = inside; 
	    send_to_char("New inside Created\n\r",ch);	
	}

	if ( !strncmp( arg2, "flags", 3 ) )
	{
	    if ( (flag = get_flags_equip( arg3 ) ) == 0 )
	    {
		send_to_char("Syntax:inside <vnum> chance <chance>\n\r",ch);	
		send_to_char("Syntax:inside <vnum> flags <flags>\n\r",ch);	
		send_to_char("Syntax:inside <vnum> remove\n\r",ch);	
		sprintf(buf,get_equip_flags(ALL_FLAGS)); 
		send_to_char(buf,ch);
		return;
	    }
	    TOGGLE_BIT( inside->flags, flag);
	    send_to_char( "Inside flags toggled.\n\r", ch );
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}
	if ( !strncmp( arg2, "chance", 3 ) )
	{
	    inside->chance = atoi(arg3);
	    send_to_char( "Inside flags toggled.\n\r", ch );
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}
	send_to_char( "Inside added.\n\r", ch );
	update_approve_data(ch, pArea, pObj2);
	return;
	}
	
    }

    if ( !str_cmp( arg1, "type" ) )
    if (edit_function(ch, OEDIT_WEAR_WEAPON_MENU, SECURITY_9))
    {
		if ( arg2[0] == '\0' )
		{
		    send_to_char( "Syntax: type [weapon type]\n\r", ch );
		    buf[0] = '\0';
		    for ( value = 0 ; value < 9 ; value++ )
		    {
			if ( value % 4 == 0 )
			{
			    send_to_char( buf, ch );
			    send_to_char( "\n\r", ch );
			    buf[0] = '\0';
			}
			sprintf( buf, "%s %15s", buf, get_weapon_name( value ) );
		    }
		    send_to_char( buf, ch );
		    send_to_char( "\n\r", ch );
		    return;
		}

		if ( pObj2->wear_data == NULL )
		{
		    send_to_char( "That cannot be worn.\n\r", ch );
		    return;
		}

		if ( pObj2->wear_data->weapon_data == NULL )
		    pObj2->wear_data->weapon_data = weapon_alloc();

		value = get_name_weapon( arg2 );
		pObj2->wear_data->weapon_data->type = value;

		send_to_char( "Weapon type set.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
		return;
    }

    if ( !strncmp( arg1, "flags" , 4 ) )
    if (edit_function(ch, OEDIT_WEAR_WEAPON_MENU, SECURITY_9))
    {
		if ( arg2[0] == '\0' )
		{
		    send_to_char( "Syntax: flags [flag]\n\r", ch );
		    sprintf( buf, "%s\n\r", weapon_bit_name( ALL_FLAGS ) );
		    strcpy( buf, format_string( str_dup( buf ) ) );
		    send_to_char( buf, ch );
		    return;
		}

		if ( pObj2->wear_data == NULL )
		{
		    send_to_char( "That cannot be worn.\n\r", ch );
		    return;
		}

		if ( pObj2->wear_data->weapon_data == NULL )
		    pObj2->wear_data->weapon_data = weapon_alloc();

		value = weapon_name_bit( arg2 );
		pObj2->wear_data->weapon_data->flags ^= value;

		send_to_char( "Weapon flag toggled.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
		return;
    }

    if ( !str_cmp( arg1, "dt" ) || !str_cmp( arg1, "dam_type" ) )
    if (edit_function(ch, OEDIT_WEAR_WEAPON_MENU, SECURITY_9))
    {
		if ( arg2[0] == '\0' )
		{
		    send_to_char( "Syntax: dam_type [type]\n\r", ch );
		    buf[0] = '\0';
		    for ( value = 0 ; value < 33 ; value++ )
		    {
			if ( value % 4 == 0 )
			{
			    send_to_char( buf, ch );
			    send_to_char( "\n\r", ch );
			    buf[0] = '\0';
			}
			sprintf( buf, "%s %15s", buf, get_type_weapon( value ) );
		    }
		    send_to_char( buf, ch );
		    send_to_char( "\n\r", ch );
		    return;
		}

		if ( pObj2->wear_data == NULL )
		{
		    send_to_char( "That cannot be worn.\n\r", ch );
		    return;
		}

		if ( pObj2->wear_data->weapon_data == NULL )
		    pObj2->wear_data->weapon_data = weapon_alloc();

		value = get_weapon_type( arg2 );
		pObj2->wear_data->weapon_data->dam_type = value;

		send_to_char( "Weapon damage type set.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
		return;
    }

    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_WEAR_WEAPON_MENU, SECURITY_9))
    {
		ch->edit_menu = OEDIT_WEAR_MENU ;
		strncpy( ch->edit_buffer, "Wear->", 80 );
		return;
    }

    if ( !str_cmp( arg1, "weight" ) )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
	    if ( pObj2->wear_data == NULL )
	    {
		send_to_char( "It cannot be worn.\n\r", ch );
		return;
	    }

	    if ( arg2[0] == '\0' || !is_number( arg2 ) )
	    {
		send_to_char( "Syntax:  weight [number]\n\r", ch );
		return;
	    }

	    value = atoi( arg2 );
	    pObj2->wear_data->weight = value;

	    send_to_char( "Weight set.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}

    if ( !str_cmp( arg1, "ac" ) )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
	    if ( pObj2->wear_data == NULL )
	    {
		send_to_char( "It cannot be worn\n\r", ch );
		return;
	    }

	    if ( arg2[0] == '\0' )
	    {
		send_to_char( "Syntax: ac < armor class >\n\r", ch );
		return;
	    }

	    pObj2->wear_data->ac = atoi(arg2);
	    update_approve_data(ch, pArea, pObj2);
	    return;
    }

    if ( !str_cmp( arg1, "cost" ) )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
	    if ( pObj2->wear_data == NULL )
	    {
		send_to_char( "That cannot be worn.\n\r", ch );
		return;
	    }
	    if ( arg2[0] != '\0' && str_cmp( arg2, "default" ) && !is_number(arg2) )
	    {
		send_to_char( "Syntax:  cost [number]\n\rSyntax:  cost default\n\r", ch );
		return;
	    }

	    if ( arg2[0] == '\0' || !str_cmp( arg2, "default" ) )
	    {
		if ( pObj2->points < 1 )
		    value = number_range(1, 100) + 20;
		else if ( pObj2->points >= 1 && pObj2->points <= 5 )
		    value = pObj2->points * 50 / 3 * 2 + number_range(1, 100);
		else if ( pObj2->points >= 6 && pObj2->points <= 10 )
		    value = pObj2->points * 100 / 3 * 2 + number_range(1, 100);
		else if ( pObj2->points >= 11 && pObj2->points <= 15 )
		    value = pObj2->points * 1500 / 3 * 2 + number_range(1, 1000);
		else if ( pObj2->points >= 16 && pObj2->points <= 20 )
		    value = pObj2->points * 10500 / 3 * 2 + number_range(1, 1000);
		else if ( pObj2->points >= 21 && pObj2->points <= 25 )
		    value = pObj2->points * 50000 / 3 * 2 + number_range(1, 1000);
		else if ( pObj2->points >= 26 )
		    value = pObj2->points * 100000 / 3 * 2 + number_range(1, 1000);
		else value = 0;
	    }

	    if ( is_number( arg2 ) )
		value = atoi( arg2 );

	    pObj2->wear_data->cost = value;

	    send_to_char( "Cost set.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
    }

    if ( !strncmp( arg1, "flags" ,4) )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
 	if ( arg2[0] == '\0' )
	{

		sprintf(buf,"Flags   %s\n\r",wear_bit_name_1(ALL_FLAGS) );
		send_to_char(buf,ch);
		return;
	    
	}
	
	if ( ( value = wear_name_bit( arg2 ) ) != ITEM_WEAR_NONE )
	{
	    if ( pObj2->wear_data == NULL )
		pObj2->wear_data = wear_alloc();

	    TOGGLE_BIT(pObj2->wear_data->wear_flags, value);

	    if ( pObj2->wear_data->wear_flags == 0 )
	    {
		AFFECT_DATA *paf;

		if ( pObj2->wear_data->magic_data != NULL )
		{
		    SPELL_LIST * spell;
		    SPELL_LIST * spell_next;

		    for ( spell = pObj2->wear_data->magic_data->spells ; spell != NULL ; spell = spell_next )
		    {
			spell_next = spell->next;
			free_spell( spell );
		    }

		    free_magic_data( pObj2->wear_data->magic_data );
		    pObj2->wear_data->magic_data = NULL;
		}

		if ( pObj2->wear_data->weapon_data != NULL )
		{
		    free_weapon_data( pObj2->wear_data->weapon_data );
		    pObj2->wear_data->weapon_data = NULL;
		}

		for ( paf = pObj2->wear_data->affected ; paf != NULL ; paf = paf_next )
		{
		    paf_next = paf->next;
		    free_affect( paf );
		}

		pObj2->wear_data->affected = NULL;
	
		free_wear_data( pObj2->wear_data );
		pObj2->wear_data = NULL;

		send_to_char( "Wear ability removed.", ch );
		update_approve_data(ch, pArea, pObj2);
		return;
	    }

	    send_to_char( "Wear flag toggled.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}
	   return;
    }

    if (!strncmp(arg1,"size",4))
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    { 
	    int s;
	    if (pObj2->wear_data == NULL)
	    {
		send_to_char ("This object does not have any wear data.\n\r ", ch) ;
		send_to_char ("It must have a wear location added first.\n\r ", ch) ;
		send_to_char ("Example: take or hold\n\r ", ch) ;
		return ;
	    }
	    if (arg2[0] == '\0')
	    { 
		send_to_char ("\n\r",ch); 
		send_to_char ("The size of the object has many game effects, and should be chosen\n\r",ch); 
		send_to_char ("carefully.  The acceptable sizes are as fillows:\n\r",ch);
		send_to_char ("0.TINY		Small birds and anything smaller.\n\r",ch);
		send_to_char ("1.SMALL		Large birds (ducks and up) to manlhach.\n\r",ch);
		send_to_char ("2.MEDIUM		Sulkano,Iavartor,Milrochdil\n\r",ch);
		send_to_char ("3.LARGE 		Belegfea,Mirros,Aranna,Tarnanthalion,Nenkhilmen.\n\r",ch);
		send_to_char ("4.HUGE		Giants, small dragons, and wyverns.\n\r",ch);
		send_to_char ("5.GIANT		For VERY large critters, i.e. dragons, titans.\n\r",ch);
		sprintf(buf,"Current Size:\n\r%d\n\r",pObj2->wear_data->size);
		send_to_char(buf,ch);
		return;
	    }
	    s = atoi (arg2);
	    if ((s < 0) || (s > 5))
	    {	
		send_to_char ("Size Out of Range\n\r",ch); return;
	    }
	    pObj2->wear_data->size = s;
	    send_to_char ("Size set\n\r",ch);					    
	    update_approve_data(ch, pArea, pObj2);
	    return;
    }

    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_WEAR_MENU, SECURITY_9))
    {
	    ch->edit_menu = OEDIT_MENU;
	    ch->edit_buffer[0] = '\0';
	    return;
    }

    if ( !strncmp( arg1, "flags" , 4) )
    if (edit_function(ch, OEDIT_LIGHT_MENU, SECURITY_9))
    {
	    int flag;

	    if ( arg2[0] == '\0' )
	    {
		send_to_char( "Usage: flag <light flag> \n\r", ch );
		sprintf( buf, "Flags:\n\r       %s\n\r", light_flag_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
		return;
	    }

	    if ( ( flag = get_light_flag( arg2 ) ) == 0 )
	    {
		send_to_char( "Invalid light flag.\n\r", ch );
		sprintf( buf, "Flags:\n\r       %s\n\r", light_flag_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
		return;
	    }

	    if ( pObj2->light_data == NULL )
		pObj2->light_data = light_alloc();

	    pObj2->light_data->flags ^= flag;

	    send_to_char( "Light flag toggled.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
    }

    if ( !strncmp( arg1, "brightness", 5 ) )
    if (edit_function(ch, OEDIT_LIGHT_MENU, SECURITY_9))
    {
	    if ( pObj2->light_data == NULL )
	    {
		send_to_char( "It cannot be used as a light (try fuel).\n\r", ch );
		return;
	    }

	    if ( arg2[0] == '\0' || !is_number( arg2 ) )
	    {
		send_to_char( "Syntax:  brightness [percentage]\n\r", ch );
		return;
	    }

	    value = atoi( arg2 );
	    pObj2->light_data->brightness = value;
	
	    send_to_char( "Brightness set.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
    }

    if ( !str_cmp( arg1, "fuel" ) )
    if (edit_function(ch, OEDIT_LIGHT_MENU, SECURITY_9))
    {
	    int value1;
	    int value2;

	    argument = one_argument( arg2, buf );
	    strcpy( arg2, buf );
	    strcpy( arg3, argument );

	    if ( arg2[0] == '\0' || !is_number( arg2 ) || !is_number( arg3 ) )
	    {
		send_to_char( "Syntax: fuel <amount> <max amount>\n\r", ch );
		send_to_char( "     0  0 = remove light\n\r", ch );
		send_to_char( "    -1 -1 = indefinite light\n\r", ch );
		return;
	    }

	    if ( pObj2->light_data == NULL )
		pObj2->light_data = light_alloc();

	    value1 = atoi( arg2 );
	    value2 = atoi( arg3 );

	    if ( value1 == 0 && value2 == 0 )
	    {	
		free_light_data( pObj2->light_data );
		pObj2->light_data = NULL;
		send_to_char( "Light removed.\n\r", ch );
		update_approve_data(ch, pArea, pObj2);
		return;
	    }

	    pObj2->light_data->fuel = value1;
	    pObj2->light_data->max_fuel = value2;
	    send_to_char( "Light Fuel amounts set.\n\r", ch );
	    update_approve_data(ch, pArea, pObj2);
	    return; 
    }

    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, OEDIT_LIGHT_MENU, SECURITY_9))
    {
	    ch->edit_menu = OEDIT_MENU;
	    ch->edit_buffer[0] = '\0';
	    return;
    }

    if ( !str_cmp( arg1, "??" ) )
    if (edit_function(ch, OEDIT_ANY, SECURITY_9))
    {

	sprintf(buf,"`C                 Secondary Commands Menu\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bdone`w         Exit Oedit and return to normal commands\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bshow`w         Displays current object status ( oshow )\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bapprove  `w    Toggles approve status for object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: approve\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bapp_hold `w    Toggles approve hold status for object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: app_hold\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bcreate   `w    Creates a new object in this area\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: create\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bremove_trig `w Remove a trigger from this object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: remove_trigger <trigger vnum>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Badd_trigger `w Add a trigger to this object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: add_trigger <trigger vnum>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bevents      `w List all events for this object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: events\n\r");
	send_to_char(buf,ch);

	if (ch->edit_menu == OEDIT_WEAR_AFFECT_MENU )
	{
	sprintf(buf,"`Bdelaffect`w    Deletes affects or applies or spells\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: delaffect <name>\n\r");
	send_to_char(buf,ch);
	}
	if (ch->edit_menu == OEDIT_MENU )
	{
	sprintf(buf,"`Bmove`w         Move this object to another area.\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: move <area number>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bclear`w        WARNING!  RESETS OBJECT TO DEFAULTS!\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: clear object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bduplicate`w    Makes a copy of this object.\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: duplicate object\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: duplicate object <area number>\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bnext`w         Oedit the next object that needs approving.\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`w                  Syntax: next [hold ( next on hold ) ]\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`Bstep`w         Oedit the next object in this area.\n\r");
	sprintf(buf,"`w                  Syntax: step\n\r");
	send_to_char(buf,ch);
	}

        return;
    }
    if ( !strncmp( arg1, "step", 3 ))
    if (edit_function(ch, OEDIT_MENU, SECURITY_1))
    {
 	pArea = ch->in_room->area;

	if ( pArea->lvnum <= 0 || pArea->uvnum <= 0 )
	{ 
	    send_to_char( "Can't find begin vnum for the area.\n\r", ch );
	    return;
	}
	
	for ( vnum = pObj2->vnum; vnum <= pArea->uvnum; vnum++ )
	{
	    if ( vnum > pObj2->vnum )
	    if ( (pObj = get_obj_index( vnum ) ) != NULL )
	    {
		break;
	    }
	}
	
	if ( pObj && pObj2)
	{
	    ch->desc->pEdit = (void *)pObj;
	    pObj2 = (OBJ_INDEX_DATA *)ch->desc->pEdit;
	    pArea = pObj2->in_area;
	    set_obj_index_points( pObj2 );
            sprintf( buf, "%ld", pObj2->vnum );
            do_oshow( ch, buf );
	}
	else 
	{ 
	    send_to_char( "End of objects in area.\n\r", ch );
	    return;
	}
	
	return;
    }

    if ( !strncmp( arg1, "next", 3 ))
    if (edit_function(ch, OEDIT_MENU, SECURITY_1))
    {
	bool fHold = FALSE;
 	pArea = ch->in_room->area;

	if ( pArea->lvnum <= 0 || pArea->uvnum <= 0 )
	{ 
	    send_to_char( "Can't find begin vnum for the area.\n\r", ch );
	    return;
	}
	
	fHold = !str_cmp( arg2, "hold");
	if (!fHold)
	for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
	{
	    if ( (pObj = get_obj_index( vnum ) ) != NULL )
	    {
		if (!IS_SET( pObj->approve_data->flags, APPROVED ) )
		if (!IS_SET( pObj->approve_data->flags, APPROVE_HOLD ) )
		break;
	    }
	}
	if (fHold)
	for ( vnum = pArea->lvnum; vnum <= pArea->uvnum; vnum++ )
	{
	    if ( (pObj = get_obj_index( vnum ) ) != NULL )
	    {
		if (IS_SET( pObj->approve_data->flags, APPROVE_HOLD ) )
		break;
	    }
	}
	
	if ( pObj )
	{
	    ch->desc->pEdit = (void *)pObj;
	    pObj2 = (OBJ_INDEX_DATA *)ch->desc->pEdit;
	    pArea = pObj2->in_area;
	    set_obj_index_points( pObj2 );
            sprintf( buf, "%ld", pObj2->vnum );
            do_oshow( ch, buf );
	}
	else 
	{ 
	    send_to_char( "End of objects in area.\n\r", ch );
	    return;
	}
	
	return;
    }
    if ( !strncmp( arg1, "condition", 4 ))
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	int s;
        if (arg2[0] == '\0' || !is_number( arg2 ) || ( s = atoi( arg2 ) ) < 0
	  || s > 100 )
	{ 
	    send_to_char( "Syntax: condition <percent>\n\r", ch );
	    return;
	}
	pObj2->condition = s;
	send_to_char ("Condition set\n\r",ch);
	update_approve_data(ch, pArea, pObj2);
	return;
    }
    if (!strncmp(arg1,"add_trigger",5))
    if (edit_function(ch, OEDIT_ANY, SECURITY_9))
    { 
		TRIGGER_LIST_DATA *trigger_list_data;
		TRIGGER_INDEX_DATA *trigger;
		int s;
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("Syntax: add_trigger <vnum>\n\r",ch); 
			 send_to_char ("This is used to add a trigger to the obj.\n\r",ch); 
			 send_to_char ("Objects can have multimple triggers.  Use vnum trigger or tlist to find\n\r",ch);
			 send_to_char ("the one(s) you wish to add.\n\r",ch);
			 send_to_char("Current Triggers vnums:\n\r",ch);
			 for ( trigger_list_data = pObj2->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			 {
			 sprintf(buf," %d",trigger_list_data->vnum);
			 send_to_char(buf,ch);
			 }
			 send_to_char("\n\r",ch);
			 return;
		}
		s = atoi (arg2);
		if ( ( trigger = get_trig_index( s ) ) == NULL )
		{
		sprintf(buf,"trigger vnum %d NOT added\n\r",s);
		send_to_char (buf,ch);					    
		sprintf(buf,"vnum %d not found \n\r",s);
		send_to_char (buf,ch);					    
		return;
		}
		if ( trigger->script_type != TRIG_TYPE_OBJ )
		{
		sprintf(buf,"trigger vnum %d NOT added\n\r",s);
		send_to_char (buf,ch);					    
		sprintf(buf,"vnum %d is not a obj type trigger.  Use tedit and set the stype\n\r",s);
		send_to_char (buf,ch);					    
		return;
		}
		trigger_list_data = trigger_list_alloc();
		trigger_list_data->vnum = s;
		trigger_list_data->next = pObj2->trigger_list;
		pObj2->trigger_list = trigger_list_data;
		sprintf(buf,"trigger vnum %d added\n\r",s);
		send_to_char (buf,ch);					    
		update_approve_data(ch, pArea, pObj2);
		return;
    }
    if (!strncmp(arg1,"remove_trigger",8))
    if (edit_function(ch, OEDIT_ANY, SECURITY_9))
    { 
		TRIGGER_LIST_DATA *trigger_list_data;
		int s, y = 0, n, xx[100];
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("Syntax: remove_trigger <vnum>\n\r",ch); 
			 send_to_char ("This is used to remove a trigger from the object.\n\r",ch); 
			 send_to_char("Current Triggers vnums:\n\r",ch);
			 for ( trigger_list_data = pObj2->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			 {
			 sprintf(buf," %d",trigger_list_data->vnum);
			 send_to_char(buf,ch);
			 }
			 send_to_char("\n\r",ch);
			 return;
		}
		s = atoi (arg2);
		 for ( trigger_list_data = pObj2->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
	    	    {
	
			    if ( trigger_list_data->vnum == s )
			    {
				found = TRUE;
				continue;
			    }
			    else
			    {
			        xx[y] = trigger_list_data->vnum;
				y++;
			    }
	    	    }	
		pObj2->trigger_list = NULL;

		for ( n = 0 ; n < y ; n++ )
		   {
	        	trigger_list_data = trigger_list_alloc();
			trigger_list_data->vnum = xx[n];
			trigger_list_data->next = pObj2->trigger_list;
			pObj2->trigger_list = trigger_list_data; 

		   }
		if (!found)	
	    	send_to_char( "Trigger not found.\n\r", ch );
		else
		{
		sprintf(buf,"trigger vnum %d removed\n\r",s);
		send_to_char (buf,ch);					    
		update_approve_data(ch, pArea, pObj2);
		}
	    	return;
    }

    if ( !strncmp( arg1, "duplicate", 3 ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_5))
    {
	EXTRA_DESCR_DATA *ed_new; 
	argument = one_argument(argument, arg1);
	argument = one_argument(argument, arg2);
        if ( arg1[0] == '\0' )
        {
            send_to_char( "Syntax:  duplicate object\n\r", ch );
            send_to_char( "Syntax:  duplicate object <area>\n\r", ch );
            send_to_char( "Warning! This will make a copy of this object\n\r", ch );
            return;
        }

	if ( str_cmp( arg1, "object" ) )
        {
            send_to_char( "Syntax:  duplicate object\n\r", ch );
            send_to_char( "Syntax:  duplicate object <area>\n\r", ch );
            send_to_char( "Warning! This will make a copy of this object\n\r", ch );
            return;
        }
	pArea_to = pArea;
   	if ( is_number( arg2 ) )
	{
        value = atoi( arg2 );
        if ( ( pArea_to = get_area_data( value ) ) == NULL )
	{
            send_to_char( "Invalid Area Number\n\r", ch );
            return;
	}
        if ( !IS_BUILDER( ch, pArea_to ) )
        {
            send_to_char( "OEdit:  Vnum in an area you cannot build in.\n\r", ch );
            return;
        }
	}
 
	    pObj = new_obj_index();
	    pObj->in_area	= pArea_to; 
	    strncpy( pObj->name, pObj2->name, 30 );
	    strncpy( pObj->short_descr, pObj2->short_descr, 80 );
	    strncpy( pObj->description, pObj2->description, 80 );
	    pObj->extra_flags  = pObj2->extra_flags;
	    pObj->special_flags = pObj2->special_flags;
	    pObj->spec_fun	= pObj2->spec_fun;
	    pObj->gate_data 	= NULL;
	    pObj->light_data	= NULL;
	    if ( pObj2->light_data != NULL )
	    {
		pObj->light_data = light_alloc();
		pObj->light_data->flags = pObj2->light_data->flags;
	 	pObj->light_data->fuel = pObj2->light_data->fuel;
		pObj->light_data->max_fuel = pObj2->light_data->max_fuel;
		pObj->light_data->brightness = pObj2->light_data->brightness;
	    }
	    pObj->cont_data	= NULL;
	    if ( pObj2->cont_data != NULL )
	    {
		SPELL_LIST * spells;
		SPELL_LIST * spell;
		LOCK_DATA * lock_list;
		LOCK_DATA * lock;
		RESET_DATA * reset;
		RESET_DATA * pReset;
		RESET_DATA * last_reset=NULL;
	
		pObj->cont_data = cont_alloc();
		pObj->cont_data->capacity  = pObj2->cont_data->capacity;
		pObj->cont_data->flags = pObj2->cont_data->flags;
	
		if ( pObj2->cont_data->trap != NULL )
		{
		    pObj->cont_data->trap = trap_alloc();
		    pObj->cont_data->trap->flags = pObj2->cont_data->trap->flags;
	
		    for ( spells = pObj2->cont_data->trap->spells ; spells != NULL ; spells = spells->next )
		    {
			spell = spell_alloc();
			spell->level = spells->level;
			spell->spell = spells->spell;
			spell->next = pObj->cont_data->trap->spells;
			pObj->cont_data->trap->spells = spell;
		    }
		}
	
		for ( lock_list = pObj2->cont_data->lock ; lock_list != NULL ; lock_list = lock_list->next )
		{
		    lock = lock_alloc();
		    lock->vnum = lock_list->vnum;
		    lock->lock_info = lock_list->lock_info;
		    lock->pick_penalty = lock_list->pick_penalty;
		    lock->next = pObj->cont_data->lock;
		    pObj->cont_data->lock = lock;
		}
	
		for ( reset = pObj2->cont_data->inside ; reset != NULL ; reset = reset->next )
		{
		    pReset = reset_alloc();
		    pReset->command	= reset->command;
		    pReset->arg1	= reset->arg1;
		    pReset->arg2	= reset->arg2;
		    pReset->arg3	= reset->arg3;
		    pReset->vnum	= reset->vnum;
		    pReset->chance	= reset->chance;
		    pReset->next	= NULL;
		    if ( last_reset == NULL )
		        pObj->cont_data->inside = pReset;
		    else
			last_reset->next = pReset;
	
		    last_reset = pReset;
		}
	    }
	    if ( pObj2->gate_data != NULL )
	    {
		GATE_DATA * gate;
	
		gate = gate_alloc();
		gate->flags = pObj2->gate_data->flags;
		gate->vnum  = pObj2->gate_data->vnum;
		gate->ave_timer = pObj2->gate_data->ave_timer;
		pObj->gate_data = gate;
	    }
	    pObj->edible_data = NULL;
	    if ( pObj2->edible_data != NULL )
	    {
		SPELL_LIST * spell;
		SPELL_LIST * spells;
	
		pObj->edible_data = edible_alloc();
		pObj->edible_data->type = pObj2->edible_data->type;
		pObj->edible_data->flags = pObj2->edible_data->flags;
		pObj->edible_data->amount = pObj2->edible_data->amount;
		pObj->edible_data->max_amount = pObj2->edible_data->max_amount;
	
		for ( spells = pObj2->edible_data->spells ; spells != NULL ; spells = spells->next )
		{
		    spell = spell_alloc();
		    spell->spell = spells->spell;
		    spell->level = spells->level;
		    spell->next  = pObj->edible_data->spells;
		    pObj->edible_data->spells = spell;
		}
	    }
	    pObj->wear_data	= NULL;
	    if ( pObj2->wear_data != NULL )
	    {
		pObj->wear_data 	 = wear_alloc();
		pObj->wear_data->wear_flags = pObj2->wear_data->wear_flags;
		pObj->wear_data->wear_loc = WEAR_NONE;	
		pObj->wear_data->weight = pObj2->wear_data->weight;
		pObj->wear_data->size   = pObj2->wear_data->size;
	        pObj->wear_data->cost   = pObj2->wear_data->cost;
		pObj->wear_data->ac     = pObj2->wear_data->ac;

		pObj->wear_data->magic_data = NULL;
		if ( pObj2->wear_data->magic_data != NULL )
		{
		    SPELL_LIST * spell;
		    SPELL_LIST * spells;
	
		    pObj->wear_data->magic_data = magic_alloc();
		    pObj->wear_data->magic_data->flags = pObj2->wear_data->magic_data->flags;
		    pObj->wear_data->magic_data->charges = pObj2->wear_data->magic_data->charges;
		    pObj->wear_data->magic_data->max_charges = pObj2->wear_data->magic_data->max_charges;
		    pObj->wear_data->magic_data->type = pObj2->wear_data->magic_data->type;
		    pObj->wear_data->magic_data->spells = NULL;
	
		    for ( spells = pObj2->wear_data->magic_data->spells ; spells != NULL ; spells = spells->next )
		    {
			spell = spell_alloc();
			spell->next = pObj->wear_data->magic_data->spells;
			spell->level = spells->level;
			spell->spell = spells->spell;
			pObj->wear_data->magic_data->spells = spell;
		    }
	
		}

		pObj->wear_data->weapon_data = NULL;
		if ( pObj2->wear_data->weapon_data != NULL )
		{
		    pObj->wear_data->weapon_data = weapon_alloc();
		    pObj->wear_data->weapon_data->dice = pObj2->wear_data->weapon_data->dice;
		    pObj->wear_data->weapon_data->die = pObj2->wear_data->weapon_data->die;
		    pObj->wear_data->weapon_data->type = pObj2->wear_data->weapon_data->type;
		    pObj->wear_data->weapon_data->flags = pObj2->wear_data->weapon_data->flags;
		    pObj->wear_data->weapon_data->dam_type = pObj2->wear_data->weapon_data->dam_type;
		}
	    }
	    pObj->condition    = pObj2->condition;
	    pObj->material     = pObj2->material;
	    pObj->timer        = pObj2->timer;
	    pObj->shop_type    = pObj2->shop_type;
	 
	  
	    /* extended desc */

	    pObj->extra_descr = NULL;
	    for (ed = pObj2->extra_descr; ed != NULL; ed = ed->next)
	    {
	        ed_new              = new_extra_descr();
 
	        strncpy( ed_new->keyword, ed->keyword, 30 );
	        ed_new->description     = ed->description;
	        ed_new->next            = pObj->extra_descr;
	        pObj->extra_descr      = ed_new;
	    }
 
	for (vnum = pArea_to->lvnum ; free_obj_list[vnum]; vnum++ );
	    pObj->vnum = vnum;
	free_obj_list[vnum] = 1;
 
        iHash                  = vnum % MAX_KEY_HASH;
        pObj->next             = obj_index_hash[iHash];
        obj_index_hash[iHash]  = pObj;
	sprintf(buf, "Done!  Vnum %ld", vnum );
        send_to_char(buf, ch);
	pObj->approve_data->creation_date = current_time;
	strncpy( pObj->approve_data->creater, ch->name, 30 );
	pObj->approve_data->flags = pObj2->approve_data->flags;
	strncpy( pObj->approve_data->approver, pObj2->approve_data->approver, 30 );
	pObj->approve_data->approved_date = pObj2->approve_data->approved_date;
	SET_BIT( pArea_to->area_flags, AREA_CHANGED );
	return;
    }

    if ( !strncmp( arg1, "material", 3 ))
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	int s;
        if (arg2[0] == '\0' || ( s = get_material_type( arg2 ) ) == 0 )
	{ 
	    send_to_char( "Syntax: material <material type>\n\r", ch );
	    strcpy( buf, "Materials: " );
	    for ( x = 1 ; x < MAX_MATERIAL ; x++ )
	    {
		strcat( buf, material_type_name( x ) );
		strcat( buf, " " );
	    }
	    send_to_char( format_string( str_dup( buf ) ), ch );
	    return;
	}
	pObj2->material = s;
	send_to_char ("Material set\n\r",ch);
	update_approve_data(ch, pArea, pObj2);
	return;
    }


    if ( !strncmp( arg1, "flags",4 ) )
    if (edit_function(ch, OEDIT_GATE_MENU, SECURITY_9))
    {
	int gflag;

	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Usage: flag <gate flag> \n\r", ch );
	    sprintf( buf, "Flags:\n\r       %s\n\r", gate_flag_name( ALL_FLAGS ) );
	    send_to_char( buf, ch );
	    return;
	}

	if ( ( gflag = get_gate_flag( arg2 ) ) == 0 )
	{
	    send_to_char( "Invalid gate flag.\n\r", ch );
	    return;
	}

	if ( pObj2->gate_data == NULL )
	    pObj2->gate_data = gate_alloc();

   	if ( gflag == GATE_TO_MOB || gflag == GATE_TO_OBJ )
	{
	    pObj2->gate_data->leads_to = NULL;
	    pObj2->gate_data->vnum = 0;
	}

	pObj2->gate_data->flags ^= gflag;

        send_to_char( "Gate flag toggled.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !str_cmp( arg1, "vnum" ) )
    if (edit_function(ch, OEDIT_GATE_MENU, SECURITY_9))
    {
	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Usage: vnum <vnum> \n\r", ch );
	    send_to_char( "  vnum - The vnum which the gate leads to\n\r", ch );
	    return;
	}

	if ( !is_number( arg2 ) || (vnum = atoi(arg2)) < 0 || vnum > MAX_VNUM )
	{
	    send_to_char( "Invalid vnum.\n\r", ch );
	    return;
	}

	if ( pObj2->gate_data == NULL )
	    pObj2->gate_data = gate_alloc();

	if ( vnum == 0 )
	{
	    free_gate_data( pObj2->gate_data );
	    pObj2->gate_data = NULL;
	    send_to_char( "Gate ability removed.\n\r", ch );
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}

	if ( ( IS_SET( pObj2->gate_data->flags, GATE_TO_MOB ) && get_mob_index( vnum ) == NULL )
	  || ( IS_SET( pObj2->gate_data->flags, GATE_TO_OBJ ) && get_obj_index( vnum ) == NULL )
	  || (!IS_SET( pObj2->gate_data->flags, GATE_TO_MOB|GATE_TO_OBJ ) && get_room_index( vnum ) == NULL ) )
	{
	    send_to_char( "Vnum does not exist.\n\r", ch );
	    return;
	}

	pObj2->gate_data->vnum = vnum;

        send_to_char( "Gate vnum set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !str_cmp( arg1, "atimer" ) )
    if (edit_function(ch, OEDIT_GATE_MENU, SECURITY_9))
    {
	int atimer;

	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Usage: atimer <average timer> \n\r", ch );
	    send_to_char( "  atimer - The average timer the gate stays before moving. (-1 doesn't move )\n\r", ch );
	    return;
	}

	if ( !is_number( arg2 ) )
	{
	    send_to_char( "Invalid vnum.\n\r", ch );
	    return;
	}

	if ( pObj2->gate_data == NULL )
	    pObj2->gate_data = gate_alloc();

	if ( ( atimer = atoi( arg2 ) ) < 1 )
	    atimer = -1;

	pObj2->gate_data->ave_timer = atimer;

        send_to_char( "Average gate timer set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "flags",4 ) )
    if (edit_function(ch, OEDIT_EDIBLE_MENU, SECURITY_9))
    {

	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Usage: flag <edible item flag> \n\r", ch );
	    sprintf( buf, "Flags:\n\r       %s\n\r", edible_flag_list_name( ALL_FLAGS ) );
	    send_to_char( buf, ch );
	    return;
	}

	if ( pObj2->edible_data == NULL )
	    pObj2->edible_data = edible_alloc();

	if ( !str_cmp( arg2, "drink" ) )
	    strcpy( arg2, "food" ); 

	if ( get_edible_flag( arg2 ) == 0 )
	{
	    send_to_char( "Invalid edible item flag.\n\r", ch );
	    return;
	}

	pObj2->edible_data->flags ^= get_edible_flag( arg2 );

        send_to_char( "Edible item flag toggled.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !str_cmp( arg1, "type" ) )
    if (edit_function(ch, OEDIT_EDIBLE_MENU, SECURITY_9))
    {
	int liquid;

	if ( arg2[0] == '\0' )
	{
	    send_to_char( "Usage: type <edible item type> \n\r", ch );

	    sprintf( buf, "  %20s %15s %10s %10s %10s\n\r\n\r",
		"Name","Color","Alcohol","Food","Drink");
	    send_to_char( buf, ch );

	    for ( liquid = 0 ; liquid < LIQ_MAX ; liquid++ )
	    {
		sprintf( buf, "  %20s %15s %10d %10d %10d\n\r",
		    liq_table[ liquid ].liq_name,
		    liq_table[ liquid ].liq_color,
		    liq_table[ liquid ].liq_affect[0],
		    liq_table[ liquid ].liq_affect[1],
		    liq_table[ liquid ].liq_affect[2]);
		send_to_char( buf, ch );
	    }
	    return;
	}

	if ( pObj2->edible_data == NULL )
	    pObj2->edible_data = edible_alloc();

	if ( get_edible_type( arg2 ) == -1 )
	{
	    send_to_char( "Invalid edible item type.\n\r", ch );
	    return;
	}

	pObj2->edible_data->type = get_edible_type( arg2 );

        send_to_char( "Edible item type set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "charges", 4 ) )
    if (edit_function(ch, OEDIT_EDIBLE_MENU, SECURITY_9))
    {
        argument = one_argument( arg2, buf );
	strcpy( arg2, buf );
	strcpy( arg3, argument );

	if ( arg3[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Usage: charges <current> <max> \n\r", ch );
	    return;
	}

	if ( pObj2->edible_data == NULL )
	    pObj2->edible_data = edible_alloc();

	pObj2->edible_data->amount = atoi( arg2 );
	pObj2->edible_data->max_amount = atoi( arg3 );

	if ( pObj2->edible_data->amount == 0
	  && pObj2->edible_data->max_amount == 0 )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spell_next;
	
	    for ( spell = pObj2->edible_data->spells ; spell != NULL ; spell = spell_next )
	    {
		spell_next = spell->next;
		free_spell( spell );
	    }

	    free_edible_data( pObj2->edible_data );
	    pObj2->edible_data = NULL;

            send_to_char( "Edible ability removed.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
            return;
	}

        send_to_char( "Edible charges set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }
    
    if ( !strncmp( arg1, "spells",4 ) )
    if (edit_function(ch, OEDIT_CONTAINER_TRAP_MENU, SECURITY_9))
    {
	int level,sn;

        argument = one_argument( arg2, buf );
	strcpy( arg2, buf );
	strcpy( arg3, argument );

	if ( arg3[0] == '\0' || arg2[0] == '\0' )
	{
	    send_to_char( "Usage: spell <level> <spell> \n\r", ch );
	    send_to_char( "(negative level removes previous spells)\n\r", ch );
	    send_to_char( "`BSpelllist `w for a list of possible spells.\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( arg3 ) ) == -1 || skill_table[ sn ].spell_fun == spell_null )
	{
	    send_to_char( "Invalid spell!\n\r", ch );
	    return;
	}

	if ( pObj2->cont_data == NULL )
	{
	    send_to_char( "That is not a container and cannot be trapped.\n\r", ch );
	    return;
	}

	if ( pObj2->cont_data->trap == NULL )
	    pObj2->cont_data->trap = trap_alloc();

	if ( ( level = atoi( arg2 ) ) < 0 )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spell_next;

	    if ( pObj2->cont_data->trap->spells == NULL )
	    {
		send_to_char( "This items contains no spells to erase.\n\r", ch );
		return;
	    }

	    if ( pObj2->cont_data->trap->spells->spell == sn )
	    {
		spell = pObj2->cont_data->trap->spells;
		pObj2->cont_data->trap->spells = pObj2->cont_data->trap->spells->next;
		free_spell( spell );

		if ( pObj2->cont_data->trap->spells == NULL )
		{
		    free_trap_data ( pObj2->cont_data->trap );
		    pObj2->cont_data->trap = NULL;
		    send_to_char( "Trap removed.\n\r", ch );
		}
		else
	            send_to_char( "Trap spell removed.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
	        return;
	    }
	    else
	        for ( spell = pObj2->cont_data->trap->spells , spell_next = spell->next ; spell_next != NULL ; spell = spell_next )
	        {
		    spell_next = spell->next;

		    if ( spell_next->spell == sn )
		    {
			spell->next = spell_next->next;
			free_spell( spell_next );
		        send_to_char( "Trap spell removed.\n\r", ch);
			update_approve_data(ch, pArea, pObj2);
		        return;
		    }
	        }	

	    send_to_char( "Spell not found.\n\r", ch );
	    return;
	}
	else
	{
	    SPELL_LIST * spell;
	
	    spell = spell_alloc();
	    spell->level = level;
	    spell->spell = sn;
	    spell->next = pObj2->cont_data->trap->spells;
	    pObj2->cont_data->trap->spells = spell;
	}
	
	update_approve_data(ch, pArea, pObj2);
        send_to_char( "Trap spell added.\n\r", ch);
        return;
    }
    
    if ( !strncmp( arg1, "spells", 4 ) )
    if (edit_function(ch, OEDIT_EDIBLE_MENU, SECURITY_9))
    {
	int level,sn;

        argument = one_argument( arg2, buf );
	strcpy( arg2, buf );
	strcpy( arg3, argument );

	if ( arg3[0] == '\0' || arg2[0] == '\0' )
	{
	    send_to_char( "Usage: spell <level> <spell> \n\r", ch );
	    send_to_char( "(negative level removes previous spells)\n\r", ch );
	    send_to_char( "`BSpelllist `w for a list of possible spells.\n\r", ch );
	    return;
	}

	if ( ( sn = skill_lookup( arg3 ) ) == -1 )
	{
	    send_to_char( "Invalid spell!\n\r", ch );
	    return;
	}

	if ( pObj2->edible_data == NULL )
	{
	    send_to_char( "That cannot be eaten/drunk.\n\r", ch );
	    return;
	}

	if ( ( level = atoi( arg2 ) ) < 0 )
	{
	    SPELL_LIST * spell;
	    SPELL_LIST * spell_next;

	    if ( pObj2->edible_data->spells == NULL )
	    {
		send_to_char( "This items contains no spells to erase.\n\r", ch );
		return;
	    }

	    if ( pObj2->edible_data->spells->spell == sn )
	    {
		spell = pObj2->edible_data->spells;
		pObj2->edible_data->spells = pObj2->edible_data->spells->next;
		free_spell( spell );
	        send_to_char( "Spell removed.\n\r", ch);
		update_approve_data(ch, pArea, pObj2);
	        return;
	    }
	    else
	        for ( spell = pObj2->edible_data->spells , spell_next = spell->next ; spell_next != NULL ; spell = spell_next )
	        {
		    spell_next = spell->next;

		    if ( spell_next->spell == sn )
		    {
			spell->next = spell_next->next;
			free_spell( spell_next );
		        send_to_char( "Spell removed.\n\r", ch);
			update_approve_data(ch, pArea, pObj2);
		        return;
		    }
	        }	

	    send_to_char( "Spell not found.\n\r", ch );
	    return;
	}
	else
	{
	    SPELL_LIST * spell;
	
	    spell = spell_alloc();
	    spell->level = level;
	    spell->spell = sn;
	    spell->next = pObj2->edible_data->spells;
	    pObj2->edible_data->spells = spell;

	}
	
        send_to_char( "Spell added.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "flags" , 4) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	    if ( arg2[0] == '\0' )
	    {
		send_to_char( "Usage: flag <extra flag> \n\r", ch );
		sprintf( buf, "Flags:\n\r       %s\n\r", extra_bit_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
		return;
	    }

	    if ( ( value = extra_name_bit( arg2 ) ) == 0 )
	    {
		send_to_char( "Invalid Extra flag.\n\r", ch );
		sprintf( buf, "Flags:\n\r       %s\n\r", extra_bit_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
		return;
	    }


	    pObj2->extra_flags ^= value;

	    send_to_char( "Extra flag toggled.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
    }

    if ( !strncmp( arg1, "sflags" , 4) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	    if ( arg2[0] == '\0' )
	    {
		send_to_char( "Usage: sflag <special flag> \n\r", ch );
		sprintf( buf, "Sflags:\n\r       %s\n\r", special_bit_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
		return;
	    }

	    if ( ( value = special_name_bit( arg2 ) ) == 0 )
	    {
		send_to_char( "Invalid Special flag.\n\r", ch );
		sprintf( buf, "Sflags:\n\r       %s\n\r", special_bit_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
		return;
	    }


	    pObj2->special_flags ^= value;

	    send_to_char( "Special flag toggled.\n\r", ch);
	    update_approve_data(ch, pArea, pObj2);
	    return;
    }


    if ( !strncmp( arg1, "move",6 ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_1))
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  move_object [area number]\n\r", ch );
            return;
        }
   	if ( is_number( arg2 ) )
        value = atoi( arg2 );
	else
        {
            send_to_char( "Syntax:  move_object [area number]\n\r", ch );
            return;
        }
        if ( ( pArea_to = get_area_data( value ) ) == NULL )
	{
            send_to_char( "Invalid Area Number\n\r", ch );
            return;
	}
        if ( !IS_BUILDER( ch, pArea_to ) )
        {
            send_to_char( "OEdit:  Vnum in an area you cannot build in.\n\r", ch );
            return;
        }

        pObj2->in_area = pArea_to;
	
        strcpy(arg1, "objs" );
        do_areasave( pArea, arg1, ch ); /* must save to update in memory and disk*/
        do_areasave( pArea_to, arg1, ch );

        SET_BIT( pArea_to->area_flags, AREA_CHANGED );
        send_to_char( "Object moved!\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }


    if ( !str_cmp( arg1, "clear" ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  clear object\n\r", ch );
            send_to_char( "Warning! This will clear all values!\n\r", ch );
            return;
        }

	if ( str_cmp( arg2, "object" ) )
        {
            send_to_char( "Syntax:  clear object\n\r", ch );
            send_to_char( "Warning! This will clear all values!\n\r", ch );
            return;
        }

    	pObj2->extra_descr   =   NULL;
    	strncpy( pObj2->name, "no name", 30 );
    	strncpy( pObj2->short_descr, "(no short description)", 80 );
    	strncpy( pObj2->description, "(no description)", 80 );
    	pObj2->extra_flags   =   0;
    	pObj2->special_flags =   0;
    	pObj2->wear_data     =   NULL;
    	pObj2->gate_data     =   NULL;
    	pObj2->edible_data   =   NULL;
    	pObj2->light_data    =   NULL;
    	pObj2->cont_data     =   NULL;
    	pObj2->count         =   0;
    	pObj2->timer	     =   0;

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Object Cleared!\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
	strncpy( pObj2->approve_data->creater, "Cleared OBJ", 30 );
        return;
    }

    if ( !str_cmp( arg1, "name" ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  name [string]\n\r", ch );
            return;
        }

        strncpy( pObj2->name, arg2, 30 );

        send_to_char( "Name set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }


    if ( !str_cmp( arg1, "short" ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  short [string]\n\r", ch );
            return;
        }

        strncpy( pObj2->short_descr, arg2, 80 );

        send_to_char( "Short description set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }


    if ( !strncmp( arg1, "shop_type" ,4) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
	    int type;

            if ( ( type = shop_name_type( arg2 ) ) == 0 )
            {
                send_to_char( "Syntax:  shop_type [shop type]\n\r", ch );
		sprintf( buf, "\n\r       %s\n\r", shop_type_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
                return;
            }

	    TOGGLE_BIT( pObj2->shop_type, type ); 

            send_to_char( "Shop type toggled.\n\r", ch);
            update_approve_data(ch, pArea, pObj2);
            return;
    }
    if ( !str_cmp( arg1, "long" ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  long [string]\n\r", ch );
            return;
        }
        
        strcat( arg2, "\n\r" );

        strncpy( pObj2->description, arg2, 80 );

        send_to_char( "Long description set.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }
    if ( !str_cmp( arg1, "longfix" ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
        sprintf(buf,pObj2->description); 
        strcat( buf, "\n\r" );

        strncpy( pObj2->description, buf, 80 );

        send_to_char( "Long description fixed.\n\r", ch);
	update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !str_cmp( arg1, "ireset" ) || !str_cmp( arg1, "inside" ) || !str_cmp( arg1, "reset" ) )
    if (edit_function(ch, OEDIT_CONTAINER_MENU, SECURITY_9))
    {
	if ( pObj2->cont_data == NULL  )
	{
	    send_to_char( "It is not a container.\n\r", ch );
	    return;
	}

	do_mob_inside( ch, pObj2, arg_all );
	return;
    }

    if ( !str_cmp( arg1, "capacity" ) )
    if (edit_function(ch, OEDIT_CONTAINER_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' || !is_number( arg2 ) )
        {
            send_to_char( "Syntax: capacity <weight>\n\r", ch );
	    send_to_char( "    0 weight = remove container\n\r", ch );
            return;
        }

	if ( pObj2->cont_data == NULL )
	    pObj2->cont_data = cont_alloc();

	value = atoi( arg2 );

	if ( value == 0 )
	{
	    LOCK_DATA * lock;
	    LOCK_DATA * lock_next;

	    for ( lock = pObj2->cont_data->lock ; lock != NULL ; lock = lock_next )
	    {
		lock_next = lock->next;
		free_lock_data( lock );
	    }

	    free_cont_data( pObj2->cont_data );
	    pObj2->cont_data = NULL;
	    send_to_char( "Container removed.\n\r", ch );
	    update_approve_data(ch, pArea, pObj2);
	    return;
	}

	if ( value < 0 )
	{
	    send_to_char( "Must have a capacity greater then 0.\n\r", ch );
	    return;
	}
	if ( value > 32000 )
	{
	    send_to_char( "Must have a capacity of less then 32001.\n\r", ch );
	    return;
	}

	pObj2->cont_data->capacity = value;
	send_to_char( "Capacity set.\n\r", ch );
	update_approve_data(ch, pArea, pObj2);
	return; 
    }

    if ( !str_cmp( arg1, "lock" ) )
    if (edit_function(ch, OEDIT_CONTAINER_LOCK_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' || !is_number( arg2 ) )
        {
            send_to_char( "Syntax: lock <key vnum>\n\r", ch );
	    send_to_char( "    (negative key vnum to remove lock)\n\r", ch );
            return;
        }

	if ( pObj2->cont_data == NULL )
	{
	    send_to_char( "It is not a container.\n\r", ch );
	    return;
	}

	value = atoi( arg2 );

	if ( value < 0 )
	{
	    LOCK_DATA * lock;
	    LOCK_DATA * lock_next;

	    if ( pObj2->cont_data->lock == NULL )
	    {
	 	send_to_char( "Lock not found.\n\r", ch );
		return;
	    }

	    value *= -1;
	    for( ; pObj2->cont_data->lock != NULL && pObj2->cont_data->lock->vnum == value ; )
	    {
		lock_next = pObj2->cont_data->lock->next;
		free_lock_data( pObj2->cont_data->lock );
		found = TRUE;
		pObj2->cont_data->lock = lock_next;
	    }

	    for ( lock = pObj2->cont_data->lock ; lock != NULL && lock->next != NULL ; )
	    {
		lock_next = lock->next;

		if ( lock_next->vnum == value )
		{
		    lock->next = lock_next->next;
		    free_lock_data( lock_next );
		    found = TRUE;
		}
		else
		    lock = lock_next;
	    }

	    if ( !found )
	    {
	 	send_to_char( "Lock not found.\n\r", ch );
		return;
	    }
	    else
	    {
	        send_to_char( "Lock removed.\n\r", ch );
		update_approve_data(ch, pArea, pObj2);
	        return;
	    }
	}

	if ( get_obj_index( value ) == NULL )
	{
	    send_to_char( "Invalid object vnum.\n\r", ch );
	    return;
	}
	else
	{
	    LOCK_DATA * lock;

	    lock = lock_alloc();
	    lock->vnum = value;
	    lock->next = pObj2->cont_data->lock;
	    pObj2->cont_data->lock = lock;
	    send_to_char( "Key vnum set.\n\r", ch );
	    update_approve_data(ch, pArea, pObj2);
	    return; 
	}
    }

    if ( !strncmp( arg1, "flags" ,4) )
    if (edit_function(ch, OEDIT_CONTAINER_LOCK_MENU, SECURITY_9))
    {
	LOCK_DATA * lock;

	argument = one_argument( arg2, buf );
	strcpy( arg2, buf );
	strcpy( arg3, argument );
        if ( arg2[0] == '\0' || !is_number( arg2 ) 
	  || arg3[0] == '\0' || ( value = get_lock_flags( arg3 ) ) == 0 )
        {
            send_to_char( "Syntax: flags <key vnum> <lock flag>\n\r", ch );
	    sprintf( buf, "\n\r       %s\n\r", get_flags_lock( ALL_FLAGS ) );
	    send_to_char( buf, ch );
            return;
        }

	if ( pObj2->cont_data == NULL )
	{
	    send_to_char( "It is not a container.\n\r", ch );
	    return;
	}

	vnum = atoi( arg2 );
	for ( lock = pObj2->cont_data->lock ; lock != NULL ; lock = lock->next )
	    if ( lock->vnum == vnum )
	    {
		TOGGLE_BIT( lock->lock_info, value );
		found = TRUE;
	    }

	if ( !found )
	{
	    send_to_char( "Key vnum not found.\n\r", ch );
	    return;
	}

	send_to_char( "Flag toggled.\n\r", ch );
	update_approve_data(ch, pArea, pObj2);
	return;
    }

    if ( !str_prefix( arg1, "pick_penalty" ) )
    if (edit_function(ch, OEDIT_CONTAINER_LOCK_MENU, SECURITY_9))
    {
	LOCK_DATA * lock;

	argument = one_argument( arg2, buf );
	strcpy( arg2, buf );
	strcpy( arg3, argument );
        if ( arg2[0] == '\0' || !is_number( arg2 ) 
	  || arg3[0] == '\0' || !is_number( arg3 ) )
        {
            send_to_char( "Syntax: pick_penalty <key vnum> <pick penalty>\n\r", ch );
            return;
        }

	if ( pObj2->cont_data == NULL )
	{
	    send_to_char( "It is not a container.\n\r", ch );
	    return;
	}

	vnum = atoi( arg2 );
	value = atoi( arg3 );
	for ( lock = pObj2->cont_data->lock ; lock != NULL ; lock = lock->next )
	    if ( lock->vnum == vnum )
	    {
		lock->pick_penalty = value;
		found = TRUE;
	    }

	if ( !found )
	{
	    send_to_char( "Key vnum not found.\n\r", ch );
	    return;
	}

	send_to_char( "Pick penalty set.\n\r", ch );
	update_approve_data(ch, pArea, pObj2);
	return;
    }

    if ( !strncmp( arg1, "flags" ,4) )
    if (edit_function(ch, OEDIT_CONTAINER_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' || ( value = get_container_flags( arg2 ) ) == 0 )
        {
            send_to_char( "Syntax: flags <container flag>\n\r", ch );
	    sprintf( buf, "%s\n\r", get_flags_container( ALL_FLAGS ) );
	    send_to_char( buf, ch );
            return;
        }

	if ( pObj2->cont_data == NULL )
	    pObj2->cont_data = cont_alloc();

	TOGGLE_BIT( pObj2->cont_data->flags, value );
	send_to_char( "Flag toggled.\n\r", ch );
	update_approve_data(ch, pArea, pObj2);
	return;
    }

    if ( !strncmp( arg1, "flags" ,4) )
    if (edit_function(ch, OEDIT_CONTAINER_TRAP_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' || ( value = get_trap_flags( arg2 ) ) == 0 )
        {
            send_to_char( "Syntax: flag <trap flag>\n\r", ch );
	    sprintf( buf, "\n\r       %s\n\r", get_flags_trap( ALL_FLAGS ) );
	    send_to_char( buf, ch );
            return;
        }

	if ( pObj2->cont_data == NULL )
	{
	    send_to_char( "That is not a container and cannot be trapped.\n\r", ch );
	    return;
	}

	if ( pObj2->cont_data->trap == NULL )
	    pObj2->cont_data->trap = trap_alloc();

	TOGGLE_BIT( pObj2->cont_data->trap->flags, value );
	send_to_char( "Flag toggled.\n\r", ch );
        update_approve_data(ch, pArea, pObj2);
	return;
    }

    if ( !str_cmp( arg1, "timer" ) )
    if (edit_function(ch, OEDIT_MENU, SECURITY_9))
    {
        if ( arg2[0] == '\0' || !is_number( arg2 ) )
        {
            send_to_char( "Syntax:  timer [number]\n\r", ch );
            return;
        }

        value = atoi( arg2 );
        pObj2->timer = value;

        send_to_char( "Timer set.\n\r", ch);
        update_approve_data(ch, pArea, pObj2);
        return;
    }

    if ( !strncmp( arg1, "approve", 4 ) && ( IS_IMP( ch ) || !str_cmp( ch->name, "Ron" ) ) )
    if (edit_function(ch, OEDIT_ANY, SECURITY_1))
    {
	TOGGLE_BIT(pObj2->approve_data->flags, APPROVED);
	REMOVE_BIT(pObj2->approve_data->flags, APPROVE_HOLD);

        send_to_char( "Approved status Toggled\n\r", ch);
	sprintf(buf,"Object is %s\n\r",IS_SET(pObj2->approve_data->flags, APPROVED) ? "APPROVED" : "UNapproved");
   	send_to_char(buf,ch);
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	strncpy( pObj2->approve_data->approver, ch->name, 30 );
	pObj2->approve_data->approved_date = current_time;
        return;
    }

    if ( !strncmp( arg1, "app_hold", 5 ) )
    if (edit_function(ch, OEDIT_ANY, SECURITY_1))
    {
	REMOVE_BIT(pObj2->approve_data->flags, APPROVED);
	TOGGLE_BIT(pObj2->approve_data->flags, APPROVE_HOLD);

        send_to_char( "Approved status HOLD Toggled\n\r", ch);
	sprintf(buf,"Object is %s\n\r",IS_SET(pObj2->approve_data->flags, APPROVE_HOLD) ? "HOLD" : "Not on Hold");
   	send_to_char(buf,ch);
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        return;
    }


    if ( !str_cmp( arg1, "create" ) )
    if (edit_function(ch, OEDIT_ANY, SECURITY_9))
    {
        value = atoi( arg2 );
        if ( arg2[0] == '\0' || value == 0 )
        {
            send_to_char( "Syntax:  oedit create [vnum]\n\r", ch );
            return;
        }

        pArea = ch->in_room->area;

        if ( IS_OPTIVISION(ch) )
        {
            send_to_char( "OEdit:  That vnum is not assigned an area.\n\r", ch );
            return;
        }

        if ( !IS_BUILDER( ch, pArea ) )
        {
            send_to_char( "OEdit:  Vnum in an area you cannot build in.\n\r", ch );
            return;
        }

        if ( get_obj_index( value ) != NULL )
        {
            send_to_char( "OEdit:  Object vnum already exists.\n\r", ch );
            return;
        }
        
        pObj2                    = new_obj_index();
        pObj2->vnum              = value;
        pObj2->in_area              = pArea;
        
     /*   if ( value > top_vnum_obj ) top_vnum_obj = value; */

        iHash                   = value % MAX_KEY_HASH;
        pObj2->next              = obj_index_hash[iHash];
        obj_index_hash[iHash]   = pObj2;
	ch->edit_menu = 1;
        ch->desc->pEdit = (void *)pObj2;

	if (LOGON_ON)
	if (ch->logon_data != NULL )
	ch->logon_data->objects++;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Object Created.\n\r", ch );
        pObj2->approve_data->creation_date = current_time;
        strncpy( pObj2->approve_data->creater, ch->name, 30 ); 
        update_approve_data(ch, pArea, pObj2);
        return;
    }


        if (edit_function(ch, OEDIT_EXTRA_MENU, SECURITY_9))
        if ( !str_cmp( arg1, "add" ) )
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax:  add [keyword]\n\r", ch );
                return;
            }

            ed                  =   new_extra_descr();
            strncpy( ed->keyword, arg2, 30 );
            ed->next            =   pObj2->extra_descr;
            pObj2->extra_descr   =   ed;

            string_append( ch, &ed->description );

            update_approve_data(ch, pArea, pObj2);
            return;
        }


        if (edit_function(ch, OEDIT_EXTRA_MENU, SECURITY_9))
        if ( !str_cmp( arg1, "edit" ) )
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax:  edit [keyword]\n\r", ch );
                return;
            }

            for ( ed = pObj2->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
            }

            if ( ed == NULL )
            {
                send_to_char( "OEdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            string_append( ch, &ed->description );

            update_approve_data(ch, pArea, pObj2);
            return;
        }


        if (edit_function(ch, OEDIT_EXTRA_MENU, SECURITY_9))
        if ( !str_cmp( arg1, "append" ) )
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax:  edit [keyword]\n\r", ch );
                return;
            }

            for ( ed = pObj2->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
            }

            if ( ed == NULL )
            {
                send_to_char( "OEdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            string_append( ch, &ed->description );

            update_approve_data(ch, pArea, pObj2);
            return;
        }


        if (edit_function(ch, OEDIT_EXTRA_MENU, SECURITY_9))
        if ( !str_cmp( arg1, "delete" ) )
        {
            EXTRA_DESCR_DATA *ped;

            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax:  delete [keyword]\n\r", ch );
                return;
            }

            ped = NULL;

            for ( ed = pObj2->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
                ped = ed;
            }

            if ( ed == NULL )
            {
                send_to_char( "OEdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            if ( ped == NULL )
            {
                pObj2->extra_descr = ed->next;
            }
            else
            {
                ped->next = ed->next;
            }

            free_extra_descr( ed );

            send_to_char( "Extra description deleted.\n\r", ch );
            update_approve_data(ch, pArea, pObj2);
            return;
        }


        if (edit_function(ch, OEDIT_EXTRA_MENU, SECURITY_9))
        if ( !str_cmp( arg1, "format" ) )
        {
            EXTRA_DESCR_DATA *ped;

            if ( arg2[0] == '\0' )
            {
                send_to_char( "Syntax:  format [keyword]\n\r", ch );
                return;
            }

            ped = NULL;

            for ( ed = pObj2->extra_descr; ed != NULL; ed = ed->next )
            {
                if ( is_name( arg2, ed->keyword ) )
                    break;
                ped = ed;
            }

            if ( ed == NULL )
            {
                send_to_char( "OEdit:  Extra description keyword not found.\n\r", ch );
                return;
            }

            ed->description = format_string( ed->description );

            send_to_char( "Extra description formatted.\n\r", ch );
            update_approve_data(ch, pArea, pObj2);
            return;
        }
    if ( IS_SET(ch->edit_flags, SECURITY_CHECK) )
    {
       send_to_char( "Security check failed.  This command requires special security.\n\r", ch );
	REMOVE_BIT(ch->edit_flags, SECURITY_CHECK);
	return;	
    }
    if (!display )
    interpret( ch, arg );
    return;
}
void do_oedit( CHAR_DATA *ch, char *argument )
{
    OBJ_INDEX_DATA *pObj2;
    AREA_DATA *pArea;
    int value;
    long vnum;
    int iHash;
    char arg1[MAX_STRING_LENGTH];

    if ( IS_NPC(ch) ) return;
    argument = one_argument( argument, arg1 );

    if ( is_number( arg1 ) )
    {
        value = atoi( arg1 );
        if ( ( pObj2 = get_obj_index( value ) ) == NULL )
        {
            send_to_char( "OEdit:  That vnum does not exist.\n\r", ch );
            return;
        }
	pArea = pObj2->in_area;
            if ( !IS_BUILDER( ch, pArea ) )
            {
                send_to_char( "OEdit:  Vnum in an area you cannot build in.\n\r", ch );
                return;
            }

        ch->desc->pEdit = (void *)pObj2;
        ch->desc->connected = CON_OEDITOR;
	ch->edit_menu = OEDIT_MENU;
	ch->edit_buffer[0] = '\0';
        return;
    }
    else
    {
        if ( !str_cmp( arg1, "create" ) )
        {
	    pArea = ch->in_room->area;

            if ( !IS_BUILDER( ch, ch->in_room->area ) )
            {
                send_to_char( "You cannot build in this area.\n\r", ch );
                return;
            }
            pObj2                    = new_obj_index();

	    for (vnum = pArea->lvnum ; free_obj_list[vnum]; vnum++ );
 	    pObj2->vnum = vnum;
	    free_obj_list[vnum] = 1;


            pObj2->in_area              = pArea;
            
        
            iHash                   = vnum % MAX_KEY_HASH;
            pObj2->next             = obj_index_hash[iHash];
            obj_index_hash[iHash]   = pObj2;
            ch->desc->pEdit = (void *)pObj2;
	    if (LOGON_ON)
	    if (ch->logon_data != NULL )
	    ch->logon_data->objects++;
	    pObj2->approve_data->creation_date = current_time;
	    strncpy( pObj2->approve_data->creater, ch->name, 30 );
            ch->desc->connected = CON_OEDITOR;
	    ch->edit_menu = OEDIT_MENU;
	    ch->edit_buffer[0] = '\0';
            update_approve_data(ch, pArea, pObj2);
            return;
        }
    }

    send_to_char( "OEdit:  There is no default object to edit.\n\r", ch );
    send_to_char( "OEdit:  Oedit create \n\r", ch );
    send_to_char( "OEdit:  Oedit <vnum> \n\r", ch );
    return;
}
