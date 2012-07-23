/*************************************************************************
Locke code from the isles.  And a few modifications by Quin and Galadrail.
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

void medit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    AREA_DATA *pArea_to;
    MOB_INDEX_DATA *pMob2;
    MOB_INDEX_DATA *pMobIndex;
    char arg[MAX_STRING_LENGTH];
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];
    char arg3[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    char buf3[MAX_BUF];
    int sn,number;
    int iHash,value,i = 0,x,y = 0;
    bool display = FALSE;
    long vnum;
    char t[MAX_STRING_LENGTH];
    arg2[0] = '\0';
    strcpy( arg, argument );
    strcpy( arg3, argument );
    argument = one_argument( argument, arg1 );
    strcpy( arg2, argument );

    pMob2 = (MOB_INDEX_DATA *)ch->desc->pEdit; 
    pArea = pMob2->load_with_area;
    pMobIndex = NULL;

    SET_BIT(pMob2->affected_by,AFF_NEW_MOB);
    if ( !str_cmp( arg1, "show" ) || arg1[0] == '\0' )
    {
        sprintf( buf, "%ld", pMob2->vnum );
        do_mshow( ch, buf );
        return;
    }


    if ( !str_cmp( arg1, "done" ) )
    {
        ch->desc->pEdit = NULL;
        ch->desc->connected = CON_PLAYING;
	ch->edit_menu = 0;
	ch->edit_buffer[0] = '\0';
        return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
    {
		ch->edit_menu = MEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
    {
		ch->edit_menu = MEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_SKILLS, SECURITY_9))
    {
		ch->edit_menu = MEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_SPECIALS, SECURITY_9))
    {
		ch->edit_menu = MEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_ADVANCED, SECURITY_9))
    {
		ch->edit_menu = MEDIT_MENU ;
		ch->edit_buffer[0] = '\0';
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_FORM_MOVE, SECURITY_9))
    {
		ch->edit_menu = MEDIT_FORM ;
		strncpy( ch->edit_buffer, "Form->", 80 );
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_FORM_COMBAT, SECURITY_9))
    {
		ch->edit_menu = MEDIT_FORM ;
		strncpy( ch->edit_buffer, "Form->", 80 );
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_FORM_MAGIC, SECURITY_9))
    {
		ch->edit_menu = MEDIT_FORM ;
		strncpy( ch->edit_buffer, "Form->", 80 );
		return;
    }
    if (!str_cmp( arg1, "back" ) || !str_cmp(arg1, "-") )
    if (edit_function(ch, MEDIT_FORM_AFFECTS, SECURITY_9))
    {
		ch->edit_menu = MEDIT_FORM ;
		strncpy( ch->edit_buffer, "Form->", 80 );
		return;
    }

    if ( !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_ANY, SECURITY_9))
    {
	sprintf(buf,"`C                      Command Medit Menu\n\r");
	send_to_char(buf,ch);
	sprintf(buf,"`B?`w            Show current menu, status and available commands\n\r");
	send_to_char(buf,ch);
	if (ch->edit_menu > 2 )
	{
	sprintf(buf,"`BBack ( - )`w   Back one menu\n\r");
	send_to_char(buf,ch);
	}
    }

    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "1", 1) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B1           `w Form and Parts\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_FORM ;
	strncpy( ch->edit_buffer, "Form->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "2", 1) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B2           `w Events and Equips\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_EVENTS ;
	strncpy( ch->edit_buffer, "Events->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "3", 1) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B3           `w Skills\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_SKILLS ;
	strncpy( ch->edit_buffer, "Skills->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "4", 1) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B4           `w Specials\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_SPECIALS ;
	strncpy( ch->edit_buffer, "Specials->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "5", 1) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B5           `w Advanced Mob\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_ADVANCED ;
	strncpy( ch->edit_buffer, "Advanced->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "1", 1) )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B1           `w Movement\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_FORM_MOVE ;
	strncpy( ch->edit_buffer, "Form->Movement->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "2", 1) )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B2           `w Combat\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_FORM_COMBAT ;
	strncpy( ch->edit_buffer, "Form->Combat->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "3", 1) )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B3           `w Affects\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_FORM_AFFECTS ;
	strncpy( ch->edit_buffer, "Form->Affects->", 80 );
	return;
	}
    }
    if (!str_cmp( arg1, "?" ) || !strncmp(arg1, "4", 1) )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`B4           `w Magic\n\r",ch);
	}
	else
	{
	
	ch->edit_menu = MEDIT_FORM_MAGIC ;
	strncpy( ch->edit_buffer, "Form->Magic->", 80 );
	return;
	}
    }

    if ( !strncmp( arg1, "events", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
    {
	EVENT_DATA   *event;
	BUFFER *output;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BEvents       `w Create or List events for this mobile\n\r",ch);
	    send_to_char("`w                  Syntax: events\n\r",ch);
	    send_to_char("`w                  Syntax: events new\n\r",ch);
	}
	else
	{
	output = new_buf();

        argument = one_argument( argument, arg1 );
	if ( !strncmp( arg1, "new",3) )
	{
	    if ( pArea != ch->in_room->area )
	    {
	    	send_to_char("Sorry!  Mob is not from this area. \n\r",ch);
	    	send_to_char("Duplicate the mob to this area if you wish.\n\r",ch);
	    	send_to_char("Event Not created!",ch);
	    	return;
	    }
	    event = event_alloc();
	    event->next = pArea->events;
	    pArea->events = event;
	    event->type = EVENT_ROOM;
	    event->room_vnum = ch->in_room->vnum;
	    event->area = ch->in_room->area;
	    event->flags = EVENT_ON_BOOT|EVENT_ON_RESET;
	    event->chance = 100;
	    sprintf(buf,"New event with vnum %ld created\n\r",event->vnum);
	    send_to_char(buf,ch);
	    SET_BIT(ch->in_room->area->area_flags, AREA_CHANGED );
	    event->mob_vnum = pMob2->vnum;
	    send_to_char("Event for mob set\n\r",ch);
	    return;
	}

	for ( event = pArea->events; event != NULL; event = event->next )
	{
	    if ( event->mob_vnum == pMob2->vnum)
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
    }
    
    if ( !strncmp( arg1, "revents", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
    {
	EVENT_DATA   *event;
	BUFFER *output;

 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BRevents       `wList events for this room\n\r",ch);
	    send_to_char("`w                  Syntax: revents\n\r",ch);
	}
	else
	{
	output = new_buf();

	for ( event = pArea->events; event != NULL; event = event->next )
	{
	    if ( event->room_vnum == ch->in_room->vnum)
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
    }
    if ( !strncmp( arg1, "equips", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
    {
	EQUIP_DATA * equip;
	EQUIP_DATA * equip_data;
        OBJ_INDEX_DATA *pObjIndex;
	int flag;
	bool found = FALSE;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BEquip         `wEquip the mobile with objects\n\r",ch);
	    send_to_char("`w                  Syntax: equip <vnum> chance <chance>\n\r",ch);	
	    send_to_char("`w                  Syntax: equip <vnum> flags <flags>\n\r",ch);	
	    send_to_char("`w                  Syntax: equip <vnum> remove\n\r",ch);	
	    send_to_char("`w                  Syntax: equip <vnum> \n\r",ch);	
	    send_to_char("`w                  Syntax: equip list \n\r",ch);	
	}
	else
	{
        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
        argument = one_argument( argument, arg3 );
	if ( arg1[0] == '\0' )
	{
	    send_to_char("Syntax:equip <vnum> chance <chance>\n\r",ch);	
	    send_to_char("Syntax:equip <vnum> flags <flags>\n\r",ch);	
	    send_to_char("Syntax:equip <vnum> remove\n\r",ch);	
	    send_to_char("Syntax:equip <vnum> \n\r",ch);	
	    send_to_char("Syntax:equip list \n\r",ch);	
	    sprintf(buf,get_equip_flags(ALL_FLAGS)); 
	    send_to_char(buf,ch);
	    send_to_char("\n\r",ch);
	    for ( equip = pMob2->equip; equip != NULL ; equip = equip->next )
	    {
		sprintf(buf, "`wObj `G%-5ld  `wChance `G%-3d  `wFlags `G%s\n\r",
		equip->obj_vnum, equip->chance, get_equip_flags(equip->flags) );
		send_to_char(buf,ch);
	    }
	    return;
	}
    	if ( !strncmp( arg1, "list", 3 ) )
	{
	    for ( equip = pMob2->equip; equip != NULL ; equip = equip->next )
	    {
		sprintf(buf, "`wObj `G%-5ld  `wChance `G%-3d  `wFlags `G%s\n\r",
		equip->obj_vnum, equip->chance, get_equip_flags(equip->flags) );
		send_to_char(buf,ch);
	    }
	    return;
	}
	if ( arg2[0] == '\0' && is_number(arg1) )
	{
	    if ( ( pObjIndex = get_obj_index( atoi(arg1) ) ) == NULL )
	    {
		send_to_char("Invalid obj index\n\r",ch);
		return;
	    }
		
	    equip = equip_alloc();
	    equip->obj_vnum = atoi(arg1); 
	    equip->flags = EQUIP_ON_BOOT|EQUIP_ON_CREATION;
	    equip->next = pMob2->equip;
	    pMob2->equip = equip; 
	    send_to_char("New equip Created\n\r",ch);	
	    if ( pObjIndex->points < 11 )
		equip->chance = URANGE(1, 100 - ( pObjIndex->points * 3 / 2 ), 100);
	    else
		equip->chance = URANGE(1, 120 - ( pObjIndex->points * 8 / 2 ), 100); 
            SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return; 
	}
	if ( !strncmp( arg2, "remove", 3 ) )
	{
	    equip_data = NULL;

	    for ( equip = pMob2->equip; equip != NULL ; equip = equip->next )
	    {
		if ( equip->obj_vnum == atoi( arg1 ) )
		    break;
		equip_data = equip;
	    }
	    if ( equip == NULL )
	    {
		send_to_char("Not found!\n\r",ch);	
		return;
	    }
	    if ( equip_data == NULL )
	    {
		pMob2->equip = equip->next;
	    }
	    else
	    {
		equip_data->next = equip->next;
	    }

	    free_equip_data( equip );

	    send_to_char( "Equip removed.\n\r", ch );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return;
	}
	if ( arg3[0] == '\0' )
	{
	    send_to_char("Syntax:equip <vnum> chance <chance>\n\r",ch);	
	    send_to_char("Syntax:equip <vnum> flags <flags>\n\r",ch);	
	    send_to_char("Syntax:equip <vnum> remove\n\r",ch);	
	    sprintf(buf,get_equip_flags(ALL_FLAGS)); 
	    send_to_char(buf,ch);
	    return;
	}

	found = FALSE;
	for ( equip = pMob2->equip; equip != NULL ; equip = equip->next )
	{
	    if ( equip->obj_vnum == atoi( arg1 ) )
	    {
		found = TRUE;
		break;
	    }
	}

	if ( equip == NULL || !found )
	{
	    if ( ( pObjIndex = get_obj_index( atoi(arg1) ) ) == NULL )
	    {
		send_to_char("Invalid obj index\n\r",ch);
		return;
	    }
	    equip = equip_alloc();
	    equip->obj_vnum = atoi(arg1); 
	    equip->flags = EQUIP_ON_BOOT|EQUIP_ON_CREATION;
	    equip->next = pMob2->equip;
	    pMob2->equip = equip; 
	    send_to_char("New equip Created\n\r",ch);	
	}

	if ( !strncmp( arg2, "flags", 3 ) )
	{
	    if ( (flag = get_flags_equip( arg3 ) ) == 0 )
	    {
		send_to_char("Syntax:equip <vnum> chance <chance>\n\r",ch);	
		send_to_char("Syntax:equip <vnum> flags <flags>\n\r",ch);	
		send_to_char("Syntax:equip <vnum> remove\n\r",ch);	
		sprintf(buf,get_equip_flags(ALL_FLAGS)); 
		send_to_char(buf,ch);
		return;
	    }
	    TOGGLE_BIT( equip->flags, flag);
	    send_to_char( "Equip flags toggled.\n\r", ch );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return;
	}
	if ( !strncmp( arg2, "chance", 3 ) )
	{
	    equip->chance = atoi(arg3);
	    send_to_char( "Equip flags toggled.\n\r", ch );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
	    return;
	}
	send_to_char( "Equip added.\n\r", ch );
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return;
	}
	
    }
    if ( !strncmp( arg1, "step", 3 ) || !str_cmp( arg1, "??" ) )
    if (edit_function(ch, MEDIT_ANY, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "??" ) )
	{
	    send_to_char("`BStep         `wMedit the next mobile in this area\n\r",ch);
	    send_to_char("`w                  Syntax: step\n\r",ch);	
	}
	else
	{
 	pArea = ch->in_room->area;

	if ( pArea->lvnum <= 0 || pArea->uvnum <= 0 )
	{ 
	    send_to_char( "Can't find begin vnum for the area.\n\r", ch );
	    return;
	}
	
	for ( vnum = pMob2->vnum; vnum <= pArea->uvnum; vnum++ )
	{
	    if ( vnum > pMob2->vnum )
	    if ( (pMobIndex = get_mob_index( vnum ) ) != NULL )
	    {
		break;
	    }
	}
	
	if ( pMobIndex )
	{
	    ch->desc->pEdit = (void *)pMobIndex;
	    pMob2 = (MOB_INDEX_DATA *)ch->desc->pEdit;
	    pArea = pMobIndex->area;
            sprintf( buf, "%ld", pMobIndex->vnum );
            do_mshow( ch, buf );
	}
	else 
	{ 
	    send_to_char( "End of mobiles in area.\n\r", ch );
	    return;
	}
	
	return;
	}
    }
    if ( !strncmp( arg1, "duplicate", 3 ) || !str_cmp( arg1, "??" ) )
    if (edit_function(ch, MEDIT_ANY, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "??" ) )
	{
	    send_to_char("`BDuplicate    `wDuplicate or copy the mobile\n\r",ch);
	    send_to_char("`w                  Syntax: duplicate\n\r",ch);	
	    send_to_char("`w                  Syntax: duplicate <area number>\n\r",ch);	
	}
	else
	{
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
            	    send_to_char( "Medit:  Vnum in an area you cannot build in.\n\r", ch );
              	    return;
            	}
	    }

   	    pMobIndex 			= alloc_pMobIndex();  
    	    pMobIndex->spec_fun 	= pMob2->spec_fun;
    	    strncpy( pMobIndex->spec_name, pMob2->spec_name, 30 );
    	    pMobIndex->pShop       	= pMob2->pShop;
    	    pMobIndex->gainer_data	= pMob2->gainer_data;
    	    pMobIndex->load_with_area 	= pArea_to;
    	    pMobIndex->trigger_list	= pMob2->trigger_list;
	    /* castle code */
	    pMobIndex->castle_area	= pMob2->castle_area;
    	    strncpy( pMobIndex->movement_msg, pMob2->movement_msg, 15 );
    	    strncpy( pMobIndex->player_name, pMob2->player_name, 30 ); 
    	    strncpy( pMobIndex->short_descr , pMob2->short_descr, 80 );
    	    strncpy( pMobIndex->long_descr, pMob2->long_descr, 80 );
    	    pMobIndex->description  	= pMob2->description;
    	    pMobIndex->follower         = pMob2->follower;
    	    pMobIndex->race         	= pMob2->race;
    	    pMobIndex->sex          	= pMob2->sex;
    	    pMobIndex->level        	= pMob2->level;
    	    pMobIndex->off_flags        = pMob2->off_flags;
    	    pMobIndex->imm_flags        = pMob2->imm_flags;
    	    pMobIndex->res_flags        = pMob2->res_flags;
    	    pMobIndex->vuln_flags       = pMob2->vuln_flags;
    	    pMobIndex->default_pos      = pMob2->default_pos;
    	    pMobIndex->start_pos        = pMob2->start_pos;
    	    pMobIndex->mood         	= pMob2->mood;
    	    pMobIndex->form         	= pMob2->form;
    	    pMobIndex->material         = pMob2->material;
    	    pMobIndex->size         	= pMob2->size;
    	    pMobIndex->weight		= pMob2->weight;
    	    pMobIndex->height		= pMob2->height;
    	    pMobIndex->act          	= pMob2->act;	/* NPC */
    	    pMobIndex->dam_type         = pMob2->dam_type;	/* NPC */
    	    pMobIndex->affected_by  	= pMob2->affected_by;
    	    pMobIndex->affected_by_2  	= pMob2->affected_by_2;
    	    pMobIndex->hitroll		= pMob2->hitroll;
    	    pMobIndex->hit[0]		= pMob2->hit[0];
    	    pMobIndex->hit[1]		= pMob2->hit[1];
    	    pMobIndex->hit[2]		= pMob2->hit[2];
    	    pMobIndex->hit[3]		= pMob2->hit[3];
    	    for ( i = 0 ; i < MAX_SECTORS ; i++ )	
		pMobIndex->valid_sector[ i ] = pMob2->valid_sector[ i ];
 
   	    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    	    {
    	        pMobIndex->mana[0][i]	= pMob2->mana[0][i];
    	        pMobIndex->mana[1][i]	= pMob2->mana[1][i];
    	        pMobIndex->mana[2][i]	= pMob2->mana[2][i];
    	        pMobIndex->mana[3][i]	= pMob2->mana[3][i];
    	    }

    	    pMobIndex->damage[0]	= pMob2->damage[0];
    	    pMobIndex->damage[1]	= pMob2->damage[1];
    	    pMobIndex->damage[2]	= pMob2->damage[2];
    	    pMobIndex->damage[3]	= pMob2->damage[3];
    	    pMobIndex->armor		= pMob2->armor;
    	    pMobIndex->act2	        = pMob2->act2;
    	    pMobIndex->gold[0]      	= pMob2->gold[0];
    	    pMobIndex->gold[1]     	= pMob2->gold[1];
    	    pMobIndex->gold[2]      	= pMob2->gold[2];
    	    pMobIndex->gold[3]      	= pMob2->gold[3];
	    for (vnum = pArea_to->lvnum ; free_mob_list[vnum]; vnum++ );
	        pMobIndex->vnum 	= vnum;

	    free_mob_list[vnum] 	= 1;
            iHash                   	= vnum % MAX_KEY_HASH;
            pMobIndex->next         	= mob_index_hash[iHash];
            mob_index_hash[iHash]   	= pMobIndex;
	    sprintf(buf, "Done!  Vnum %ld", vnum );
            send_to_char(buf, ch);
            SET_BIT( pArea_to->area_flags, AREA_CHANGED );
	    return;
	}
    }

    if ( !strncmp( arg1, "guilds", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SKILLS, SECURITY_9))
    {
	long guild;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BGuilds       `G%s`w\n\r",guild_bit_name(pMob2->guilds));
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: guilds <guild name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{

	if ( arg2[0] == '\0' )
	{
	    send_to_char( " Syntax: guilds <guild name>\n\r", ch );
	    sprintf(buf, "Guilds: `G%s`w\n\r",guild_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
	    return;
	}

        if ( ( guild = guild_name_bit( arg2 ) ) <= 1 )
	{
	    send_to_char( "Invalid guild name.\n\r", ch );
	    return;
	}
	
	TOGGLE_BIT( pMob2->guilds, guild );
	send_to_char( "Guild flag toggled.\n\r", ch );
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return;
	}
    }
 
    if ( !strncmp( arg1, "sector", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_MOVE, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BSector       `wToggle valid sectors for the mobile to go\n\r",ch);
	    send_to_char("`w                  Syntax: sector <sector name>\n\r",ch);	
	    send_to_char("`w                  Syntax: sector land ( Default land sectors )\n\r",ch);	
	    send_to_char("`w                  Syntax: sector water ( Default water sectors )\n\r",ch);	
	    send_to_char("`w                  Syntax: sector default\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
    	for ( x= 0; x < MAX_SECTORS; x++)
    	{
		sprintf(buf, "Sector:%-15s %-5s  ",sector_bit_name(x),pMob2->valid_sector[x]  > 0 ? "TRUE" : "FALSE" );
		y++;
		if (y > 1)
		{
		    strcat(buf,"\n\r");
		    y = 0;
		}	 
    		send_to_char( buf, ch );
    	}
            send_to_char( "\n\rSyntax:  sector <sector_type> \n\r", ch );
            return;
        }

	if ( !str_cmp( arg2, "default" ) )
	{
	for (x = 0; x < MAX_SECTORS; x++ )
	{
	    pMob2->valid_sector[x] = FALSE;
	    TOGGLE_BIT(pMob2->valid_sector[x],x );
	}
        send_to_char( "Defaults loaded for sectors\n\r", ch );
	pMob2->valid_sector[0] = TRUE;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return;
	}
	if ( !str_cmp( arg2, "land" ) )
	{
	for (x = 0; x < MAX_SECTORS; x++ )
	{
	    pMob2->valid_sector[x] = FALSE;
	}
        send_to_char( "Land defaults loaded for sectors\n\r", ch );
	pMob2->valid_sector[0] = TRUE;
	pMob2->valid_sector[1] = TRUE;
	pMob2->valid_sector[2] = TRUE;
	pMob2->valid_sector[3] = TRUE;
	pMob2->valid_sector[4] = TRUE;
	pMob2->valid_sector[5] = TRUE;
	pMob2->valid_sector[9] = TRUE;
	pMob2->valid_sector[10] = TRUE;
	pMob2->valid_sector[14] = TRUE;
	pMob2->valid_sector[15] = TRUE;
	pMob2->valid_sector[16] = TRUE;
	pMob2->valid_sector[19] = TRUE;
	pMob2->valid_sector[20] = TRUE;
	pMob2->valid_sector[21] = TRUE;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return;
	}
	if ( !str_cmp( arg2, "water" ) )
	{
	for (x = 0; x < MAX_SECTORS; x++ )
	{
	    pMob2->valid_sector[x] = FALSE;
	}
        send_to_char( "Water defaults loaded for sectors\n\r", ch );
	pMob2->valid_sector[6] = TRUE;
	pMob2->valid_sector[7] = TRUE;
	pMob2->valid_sector[8] = TRUE;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return;
	}
    	if ( sector_number( arg2 ) == SECT_MAX  )
    	{
            send_to_char( "Invalid sector type\n\r", ch );
            return;
	}
	else
    	if ( sector_number( arg2 ) == SECT_NONE  ) /* Can't toggle a 0 bit */
    	{
	   if ( pMob2->valid_sector[0] == TRUE )
		pMob2->valid_sector[0] = FALSE ;
	   else
		pMob2->valid_sector[0] = TRUE ;
	}
	else
        {
	    if ( pMob2->valid_sector[sector_number(arg2)] > 0 )
		pMob2->valid_sector[sector_number(arg2)] = FALSE ;
	    else
		pMob2->valid_sector[sector_number(arg2)] = TRUE ;
        }

           SET_BIT( pArea->area_flags, AREA_CHANGED );
           send_to_char( "Sector flag toggled.\n\r", ch);
           return;
	}
    }

    if ( !strncmp( arg1, "setskill", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SKILLS, SECURITY_9))
    {
	SKILL_LIST * skills;
	SKILL_LIST * next_skill;

 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BSetskill     `wToggle skills for the mobile\n\r",ch);
	    send_to_char("`w                  Syntax: setskill <skill name>\n\r",ch);	
	    send_to_char("`w                  Syntax: setskill show\n\r",ch);	
	    display = TRUE;
	}
	else
	{
	if ( !str_cmp(arg2, "show") )
	{
	x=0;
	for ( skills = pMob2->innate_skills ; skills != NULL ; skills = skills->next )
	{
	x++;
		sprintf( buf, "%-20s ", skill_table[ skills->sn ].name );		
		send_to_char( buf, ch );
		if (x % 3 == 0 )
		    send_to_char( "\n\r", ch );
	}
	return;	
	}

	if ( *argument != '\0' )
	    sn = skill_lookup( argument );	    
	else
	{
	    for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
	    {
		sprintf( buf, "%-20s ", skill_table[ sn ].name );		
		send_to_char( buf, ch );
		if ( sn % 3 == 0 )
		    send_to_char( "\n\r", ch );
	    }
	    send_to_char("Syntax:    setskill [skill_name]\n\r",ch);
	    return;
	}

	if ( sn < 1 || sn > MAX_SKILL)
	{
	    send_to_char( "Invalid Skill\n\r", ch );
	    return;
	}

        if ( ( skills = pMob2->innate_skills ) != NULL && pMob2->innate_skills->sn == sn )
        {
            	pMob2->innate_skills = skills->next;
            	free_skill_list( skills);
	    	SET_BIT( pArea->area_flags, AREA_CHANGED );
            	send_to_char( "Skill removed.\n\r", ch );
            	return;
        }

	for ( skills = pMob2->innate_skills ; skills != NULL ; skills = next_skill )
	{
		next_skill = skills->next;
		if (next_skill != NULL && next_skill->sn == sn) 
		{
			skills->next = next_skill->next;
			free_skill_list( next_skill);
			SET_BIT( pArea->area_flags, AREA_CHANGED );
			send_to_char("Skill removed!\n\r", ch);
			return;
		}
	}

	skills = skill_list_alloc();
	skills->sn = sn;
	skills->next = pMob2->innate_skills;
	pMob2->innate_skills = skills;
        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Skill added!\n\r", ch);
        return;
	}
    }	

    if ( !strncmp( arg1, "setprac", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SKILLS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BSetprac      `GSkills %d  Spells %d`w\n\r",pMob2->prac_skills, pMob2->prac_spells);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: setprac skill <number>\n\r",ch);	
	    send_to_char("`w                  Syntax: setprac spell <number>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        argument = one_argument( argument, arg1 );
        strcpy( arg2, argument );
	if (arg1[0] == '\0')
	{
		send_to_char("Syntax: setprac skill <number>\n\r", ch);
		send_to_char("Syntax: setprac spell <number>\n\r", ch);
		return;
	}	
	if (!str_cmp( arg1, "skill" ) )
	{
		if (arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char("Syntax: setprac skill <number>\n\r", ch);
			send_to_char("Syntax: setprac spell <number>\n\r", ch);
			return;
		}	
		pMob2->prac_skills = atoi( arg2 );
	        SET_BIT( pArea->area_flags, AREA_CHANGED );
		send_to_char("Skill practices set.\n\r", ch);
		return;
	}
	if (!str_cmp( arg1, "spell" ) )
	{
		if (arg2[0] == '\0' || !is_number( arg2 ) )
		{
			send_to_char("Syntax: practice skill <number>\n\r", ch);
			send_to_char("Syntax: practice spell <number>\n\r", ch);
			return;
		}	
		pMob2->prac_spells = atoi( arg2 );
		SET_BIT( pArea->area_flags, AREA_CHANGED );
		send_to_char("Spell practices set.\n\r", ch);
		return;
	}
	}
    }		
		

    if ( !strncmp( arg1, "shop", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SPECIALS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`Bshop         `wMake this mobile a shop\n\r",ch);
	    send_to_char("`w                  Syntax: shop hours [#opening] [#closing]\n\r",ch);	
	    send_to_char("`w                  Syntax: shop profit [#buying] [#selling]\n\r",ch);	
	    send_to_char("`w                  Syntax: shop type <shop type>\n\r",ch);	
	    send_to_char("`w                  Syntax: shop delete\n\r",ch);	
	    display = TRUE;
    if ( pMob2->pShop != NULL )
    {
        SHOP_DATA *pShop;

        pShop = pMob2->pShop;

        sprintf( buf, "Shop data (for %ld):\n\r    Will buy at %d%%, and sell at %d%%.\n\r",
                      pShop->keeper, pShop->profit_buy, pShop->profit_sell );
        send_to_char( buf, ch );
        sprintf( buf, "    Opens at %d, closes at %d.\n\r",
                      pShop->open_hour, pShop->close_hour );
        send_to_char( buf, ch );

	sprintf( buf, "    Type - %s\n\r", shop_type_name( pShop->type ) );
	send_to_char( buf, ch );
/*    	sprintf( buf, "     Owner - %s\n\r", pShop->owner );
    	send_to_char( buf, ch );
 */
    }
	}
	else
	{
        argument = one_argument( argument, arg1 );
        strcpy( arg2, argument );

        if ( arg1[0] == '\0' )
        {
            send_to_char( "Syntax:  shop hours [#opening] [#closing]\n\r", ch );
            send_to_char( "         shop profit [#buying%] [#selling%]\n\r", ch );
            send_to_char( "         shop type [shop type]\n\r", ch );
            send_to_char( "         shop delete\n\r", ch );
/*	    send_to_char( "	    shop ownable\n\r" , ch ); */
            return;
        }

/*
	if( !str_cmp( arg1, "ownable" ) )
	{
		
		if( pMob2->pShop->owner != '\0' )
		{
			send_to_char( "This shop has an owner, unable to change ownability data.\n\r" , ch );
			return;
		}
	
		
		if( pMob2->pShop->ownable )
			pMob->pShop->ownable = FALSE;
		else
			pMob->pShop->ownable = TRUE;
			
		send_to_char( "Shop data set.\n\r" , ch );
		return;
		
	}
*/
	
	 if ( !str_cmp( arg1, "hours" ) )
        {
            argument = one_argument( argument, arg1 );
            strcpy( arg2, argument );

            if ( arg1[0] == '\0' || !is_number( arg1 )
              || arg2[0] == '\0' || !is_number( arg2 ) )
            {
                send_to_char( "Syntax:  shop hours [#opening] [#closing]\n\r", ch );
                return;
            }

            if ( pMob2->pShop == NULL )
            {
                pMob2->pShop         = new_shop();
                pMob2->pShop->keeper = pMob2->vnum;
                shop_last->next     = pMob2->pShop;
            }

            pMob2->pShop->open_hour = atoi( arg1 );
            pMob2->pShop->close_hour = atoi( arg2 );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Shop hours set.\n\r", ch);
            return;
        }


        if ( !str_cmp( arg1, "profit" ) )
        {
            argument = one_argument( argument, arg1 );
            strcpy( arg2, argument );

            if ( arg1[0] == '\0' || !is_number( arg1 )
              || arg2[0] == '\0' || !is_number( arg2 ) )
            {
                send_to_char( "Syntax:  shop profit [#buying%] [#selling%]\n\r", ch );
                return;
            }

            if ( pMob2->pShop == NULL )
            {
                pMob2->pShop         = new_shop();
                pMob2->pShop->keeper = pMob2->vnum;
                shop_last->next     = pMob2->pShop;
            }

            pMob2->pShop->profit_buy     = atoi( arg1 );
            pMob2->pShop->profit_sell    = atoi( arg2 );

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Shop profit set.\n\r", ch);
            return;
        }

        if ( !str_cmp( arg1, "type" ) )
        {
	    int type;

            if ( ( type = shop_name_type( arg2 ) ) == 0 )
            {
                send_to_char( "Syntax:  shop type [shop type]\n\r", ch );
		sprintf( buf, "\n\r       %s\n\r", shop_type_name( ALL_FLAGS ) );
		send_to_char( buf, ch );
                return;
            }

            if ( pMob2->pShop == NULL )
            {
                pMob2->pShop         = new_shop();
                pMob2->pShop->keeper = pMob2->vnum;
                shop_last->next     = pMob2->pShop;
            }

	    TOGGLE_BIT( pMob2->pShop->type, type ); 

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Shop type toggled.\n\r", ch);
            return;
        }

        if ( !str_cmp( arg1, "delete" ) )
        {
            if ( pMob2->pShop == NULL )
            {
                send_to_char( "REdit:  Cannot delete a shop that is non-existant.\n\r", ch );
                return;
            }

            free_shop( pMob2->pShop );
            pMob2->pShop = NULL;

            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "Shop deleted.\n\r", ch);
            return;
        }

        send_to_char( "Syntax:  shop hours [#opening] [#closing]\n\r", ch );
        send_to_char( "         shop profit [#buying%] [#selling%]\n\r", ch );
        send_to_char( "         shop type [#] [item type]\n\r", ch );
        send_to_char( "         shop delete\n\r", ch );
        return;
	}
    }

    /* castle code */
    if ( !strncmp( arg1, "castle", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SPECIALS, SECURITY_1))
    {
	int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BCastle     `G%d`w\n\r",pMob2->castle_area);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: castle <area number>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
            if ( arg2[0] == '\0' || !is_number( arg2 ) )
            {
            	send_to_char( "This is used for castle code mobs ONLY. If 0 then no castle area set.\n\r", ch );
            	send_to_char( "If you don't know what castle code is then don't touch this.\n\r", ch );
            	send_to_char( "Syntax:  castle [area number]\n\r", ch );
	    	sprintf( buf, "Mobile castle area: `G%d`w", pMob2->castle_area );
	    	send_to_char( buf, ch );
            	return;
            }
 
	    s = atoi( arg2 );
	    pMob2->castle_area = s;
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            send_to_char( "castle area set.\n\r", ch);
            return;
	}
    }

    if ( !strncmp( arg1, "move_msg", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_MOVE, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BMove_msg     `G%s`w\n\r",pMob2->movement_msg);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: move_msg <string>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "When the mobile moves this will show to the characters.\n\r", ch );
            send_to_char( "If you do not have a movement message, the default will be used.\n\r", ch );
            send_to_char( "Syntax:  move_msg [string]\n\r", ch );
	    sprintf(buf,"Mobile name: `G%s`w",pMob2->movement_msg);
	    send_to_char(buf,ch);
            return;
        }

        strncpy( pMob2->movement_msg, arg2, 15 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "move_msg set.\n\r", ch);
        return;
	}
    }

    if ( !strncmp( arg1, "name", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BName         `G%s\n\r`w",pMob2->player_name);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: name <string>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "In most cases a mobile should have more then one name.\n\r", ch );
            send_to_char( "It is used by both players and other mobiles to reference by.\n\r", ch );
            send_to_char( "A cat could have kitten, cat and feline.\n\r", ch );
            send_to_char( "\n\r", ch );
            send_to_char( "Syntax:  name [string]\n\r", ch );
	    sprintf(buf,"Current Mobile name: `G%s`w",pMob2->player_name);
	    send_to_char(buf,ch);
            return;
        }

        strncpy( pMob2->player_name, arg2, 30 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Name set.\n\r", ch);
        return;
	}
    }

    if ( !strncmp( arg1, "race", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
	int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BRace         `G%s\n\r`w",race_table[pMob2->race][pMob2->sex].name);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: race <race name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
	    do_rlist(ch,"");
            send_to_char( "Syntax:  race [race name]\n\r", ch );
            return;
        }

	if ( ( s = race_lookup( arg2 ) ) == -1 )
	{
	    send_to_char( "Invalid race\n\r",ch);
	    return;
	}

	pMob2->race = s;

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Race set.\n\r", ch);
        return;
	}
    }

    if ( !strncmp( arg1, "default", 4 ) || !str_cmp( arg1, "??" ) )
    if (edit_function(ch, MEDIT_ANY, SECURITY_9))
    {
	int sex = pMob2->sex;
 	int race = pMob2->race;
	int total = 0;

 	if ( !str_cmp( arg1, "??" ) )
	{
	    send_to_char("`BDefault     `wSet the mob back to default\n\r",ch);
	    send_to_char("`w                  Syntax: default\n\r",ch);	
	    display = TRUE;
	}
	else
	{
	pMob2->size = race_table[ race ][ sex ].size;
 	pMob2->dam_type = race_table[ race ][ sex ].dam_type;
	pMob2->form = race_table[ race ][ sex ].form;
	pMob2->affected_by = race_table[ race ][ sex ].aff;
	pMob2->affected_by_2 = race_table[ race ][ sex ].aff2;
	pMob2->off_flags = race_table[ race ][ sex ].off;
	pMob2->imm_flags = race_table[ race ][ sex ].imm;
	pMob2->res_flags = race_table[ race ][ sex ].res;
	pMob2->vuln_flags = race_table[ race ][ sex ].vuln;
	pMob2->act = race_table[ race ][ sex ].act;
        if ( pMob2->level == 1 )
        {
            pMob2->hit[ 0 ] = 2;
            pMob2->hit[ 1 ] = 2;
            pMob2->hit[ 2 ] = 10;
        }
        else if ( pMob2->level == 2 )
        {
            pMob2->hit[ 0 ] = 2;
            pMob2->hit[ 1 ] = 2;
            pMob2->hit[ 2 ] = 15;
        }
        else if ( pMob2->level == 3 )
        {
            pMob2->hit[ 0 ] = 2;
            pMob2->hit[ 1 ] = 2;
            pMob2->hit[ 2 ] = 20;
        }
        else
        {
	   pMob2->hit[ 0 ] = pMob2->level;
	   pMob2->hit[ 1 ] = ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / ( 3 * pMob2->level + 3 ) - 1;
	   pMob2->hit[ 2 ] = ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / 3;
	}
	pMob2->damage[ 0 ] = ( ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / ( 4 * pMob2->level + 3 ) ) / 3 + 1;
	pMob2->damage[ 1 ] = ( ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / ( 4 * pMob2->level + 3 ) ) / 3 + 3;
	pMob2->damage[ 2 ] = pMob2->level / 5;
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	{
	    pMob2->mana[ 0 ][ i ] = race_table[ race ][ sex ].mana_dice[ i ] * pMob2->level;
	    pMob2->mana[ 1 ][ i ] = race_table[ race ][ sex ].mana_die[ i ];
	    pMob2->mana[ 2 ][ i ] = race_table[ race ][ sex ].mana_bonus[ i ] * pMob2->level + 100;
	}
	number = number_range(25,75) * race_table[ race ][ sex ].gold * pMob2->level / 100;
	pMob2->gold[ race_table[ race ][ sex ].world ] = number;
	    total=100;
	    for ( x = 0 ; x < pMob2->level ; x++ )	
		total-= roll_dice( 2, 3 ) + 2;
	    pMob2->armor = total;

	send_to_char ("Race default values set\n\r",ch);
        SET_BIT( pArea->area_flags, AREA_CHANGED );
	return; 
	}
    }

	if (!str_cmp(arg1,"gold") || !strncmp(arg1,"crowns", 4) || !strncmp(arg1, "duckets", 4)
	 || !strncmp(arg1, "wheels", 4) || !strncmp(arg1, "greckles", 4 ) || !str_cmp( arg1, "?" )) 
    if (edit_function(ch, MEDIT_SPECIALS, SECURITY_9))
	{
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BGold        `wDefault for current world of mobile\n\r",ch);
	    send_to_char("`BWheels      `wSet the amount of Wheels\n\r",ch);
	    send_to_char("`BGreckles    `wSet the amount of Greckles\n\r",ch);
	    send_to_char("`BCrowns      `wSet the amount of Crowns\n\r",ch);
	    send_to_char("`BDuckets     `wSet the amount of Duckets\n\r",ch);
	    send_to_char("`w                  Syntax: {gold type} <amount>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			send_to_char ("\n\r",ch);
			send_to_char ("Most mobiles should carry no more than 100 coins per level,\n\r",ch);
			send_to_char ("and usually much less unless they are supposted to be wealthy.\n\r",ch);
			send_to_char ("Non-intelligent mobiles should usually have no gold.\n\r",ch);
			sprintf(buf,"Current Crowns %ld  Duckets %ld  Wheels %ld  Greckles %ld:\n\r",
			pMob2->gold[0],pMob2->gold[1],pMob2->gold[2],pMob2->gold[3]);
			send_to_char(buf,ch);
			return;
		}
		s = atoi(arg2);
		if (s < 0) 
		{ 
			send_to_char ("Mobs cannot have negative gold amounts\n\r",ch); return;
		}
		pMob2->gold[ !strncmp(arg1,"crowns", 4 ) ? 0 : 
		!strncmp(arg1,"duckets", 4) ? 1 : 
		!strncmp(arg1, "wheels", 4) ? 2 : 
		!strncmp(arg1, "greckles", 4 ) ? 3 : 
		race_table[pMob2->race][pMob2->sex].world] = s;
		send_to_char ("Gold set\n\r",ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}
    if ( !strncmp( arg1, "start_pos", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
	{ 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BStart_pos     `G%s`w\n\r",get_pos_name(pMob2->start_pos));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: start_pos <position>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
 
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("Syntax: start_pos <position>\n\r",ch); 
			 send_to_char ("Positions:   sleeping, resting, fighting, standing\n\r",ch); 
			 return;
		}
		s = position_value(arg2);
		if ( s == -1 ) 
		{
		    send_to_char( "Invalid position.\n\r", ch );
		    return;
		}
		pMob2->start_pos = s;
		send_to_char ("Start Position set\n\r",ch);
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	}
	}
    if ( !strncmp( arg1, "default_pos", 7 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
	{ 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BDefault_pos   `G%s`w \n\r",get_pos_name(pMob2->default_pos));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: default_pos <position>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("Syntax: default_pos <position>\n\r",ch); 
			 send_to_char ("Positions:   sleeping, resting, fighting, standing\n\r",ch); 
			 return;
		}
		s = position_value(arg2);
		if ( s == -1 ) 
		{
		    send_to_char( "Invalid position.\n\r", ch );
		    return;
		}
		pMob2->default_pos = s;
		send_to_char ("Default Position set\n\r",ch);	
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}
    if ( !strncmp( arg1, "follower", 7 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_MOVE, SECURITY_9))
	{ 
		long s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`Bfollower     `G%ld`w\n\r",pMob2->follower);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: follower <mobile vnum>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("If you wish this mob to follow ( group ) with another\n\r",ch); 
			 send_to_char ("mobile, simple set the follower value to the vnum of that mobile.\n\r",ch);
			 sprintf(buf,"Current Follower number:\n\r`G%ld`w\n\r",pMob2->follower);
			 send_to_char(buf,ch);
			 return;
		}
		s = atoi (arg2);
		pMob2->follower = s;
		sprintf(buf,"%s will now follow vnum %ld\n\r",pMob2->short_descr,pMob2->follower);
		send_to_char (buf,ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}
    if ( !strncmp( arg1, "add_trigger", 7 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
	{ 
		TRIGGER_LIST_DATA *trigger_list_data;
		TRIGGER_INDEX_DATA *trigger;
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`Badd_trigger   `wTo add a trigger to the mobile\n\r",ch);
	    send_to_char("`w                  Syntax: add_trigger <trigger vnum>\n\r",ch);	
			 send_to_char("Current Triggers vnums:\n\r",ch);
			 for ( trigger_list_data = pMob2->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			 {
			 sprintf(buf," `G%d`w",trigger_list_data->vnum);
			 send_to_char(buf,ch);
			 }
			 send_to_char("\n\r",ch);
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("Syntax: add_trigger <vnum>\n\r",ch); 
			 send_to_char ("This is used to add a trigger to the mob.\n\r",ch); 
			 send_to_char ("Mobiles can have multimple triggers.  You vnum trigger or tlist to find\n\r",ch);
			 send_to_char ("the one(s) you wish to add.\n\r",ch);
			 send_to_char("Current Triggers vnums:\n\r",ch);
			 for ( trigger_list_data = pMob2->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
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
		if ( trigger->script_type != TRIG_TYPE_MOB )
		{
		sprintf(buf,"trigger vnum %d NOT added\n\r",s);
		send_to_char (buf,ch);					    
		sprintf(buf,"vnum %d is not a mob type trigger.  Use tedit and set the stype\n\r",s);
		send_to_char (buf,ch);					    
		return;
		}
		trigger_list_data = trigger_list_alloc();
		trigger_list_data->vnum = s;
		trigger_list_data->next = pMob2->trigger_list;
		pMob2->trigger_list = trigger_list_data;
		sprintf(buf,"trigger vnum %d added\n\r",s);
		send_to_char (buf,ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}
    if ( !strncmp( arg1, "remove_trigger", 9 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_EVENTS, SECURITY_9))
	{ 
		TRIGGER_LIST_DATA *trigger_list_data;
		int s, n, xx[100];
		bool found = FALSE;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BRemove_trigger`wTo remove a trigger from the mobile\n\r",ch);
	    send_to_char("`w                  Syntax: remove_trigger <trigger vnum>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			 send_to_char ("\n\r",ch); 
			 send_to_char ("Syntax: remove_trigger <vnum>\n\r",ch); 
			 send_to_char ("This is used to remove a trigger from the mob.\n\r",ch); 
			 send_to_char("Current Triggers vnums:\n\r",ch);
			 for ( trigger_list_data = pMob2->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
			 {
			 sprintf(buf," %d",trigger_list_data->vnum);
			 send_to_char(buf,ch);
			 }
			 send_to_char("\n\r",ch);
			 return;
		}
		s = atoi (arg2);
		 for ( trigger_list_data = pMob2->trigger_list; trigger_list_data != NULL; trigger_list_data = trigger_list_data->next )
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
		pMob2->trigger_list = NULL;

		for ( n = 0 ; n < y ; n++ )
		   {
	        	trigger_list_data = trigger_list_alloc();
			trigger_list_data->vnum = xx[n];
			trigger_list_data->next = pMob2->trigger_list;
			pMob2->trigger_list = trigger_list_data; 

		   }
		if (!found)	
	    	send_to_char( "Trigger not found.\n\r", ch );
		else
		{
		sprintf(buf,"trigger vnum %d removed\n\r",s);
		send_to_char (buf,ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		}
	    	return;
	    }
	}
    if ( !strncmp( arg1, "size", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
	{ 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BSize         `G%d`w\n\r",pMob2->size);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: size <size>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			send_to_char ("\n\r",ch); 
			send_to_char ("The size of the mobile has many game effects, and should be chosen\n\r",ch); 
			send_to_char ("carefully.  The acceptable sizes are as fillows:\n\r",ch);
			send_to_char ("0.TINY           Small birds and anything smaller.\n\r",ch);
			send_to_char ("1.SMALL          Large birds (ducks and up) to manlhach.\n\r",ch);
			send_to_char ("2.MEDIUM         Sulkano,Iavartor,milrochdil.\n\r",ch);
			send_to_char ("3.LARGE          Belegfea,Mirros,Aranna,Tarnanthalion,Nenkhilmen\n\r",ch);
			send_to_char ("4.HUGE           Giants, small dragons, and wyverns.\n\r",ch);
			send_to_char ("5.GIANT          For VERY large critters, i.e. dragons, titans.\n\r",ch);
			sprintf(buf,"Current Size:\n\r`G%d`w\n\r",pMob2->size);
			send_to_char(buf,ch);
			return;
		}
		s = atoi (arg2);
	 	if ((s < 0) || (s > 5))
		{	
			send_to_char ("Size Out of Range\n\r",ch); return;
		}
		pMob2->size = s;
	send_to_char ("Size set\n\r",ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}
    if ( !strncmp( arg1, "hit_points", 5 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_COMBAT, SECURITY_9))
	{ 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BHit_points  `G %d %d %d`w\n\r",pMob2->hit[0], pMob2->hit[1], pMob2->hit[2]);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: hit_pts <dice> <die> <bonus>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			send_to_char ("\n\r",ch); 
			send_to_char ("Hit Pts are expressed as ndn+n (so many d-whatevers + a bonus) \n\r",ch); 
			send_to_char ("The hit points will determine how much raw damage your monster \n\r",ch);
			send_to_char ("can take before expiring.  Use the chart below to determine\n\r",ch);
			send_to_char ("a relitive hit dice for the level of mobile you are creating.\n\r",ch);
			send_to_char ("Example:  A level 12 would have approx 2d9+150 Hit Pts.\n\r",ch);
			send_to_char ("Use the format: 0 0 0   Where 2 9 150 would be the above example.\n\r",ch); 
			send_to_char ("level	Hit Pts	  		level	Hit Pts	\n\r",ch);
			send_to_char ("1	2d6+10	  	 	31	6d12+928	\n\r",ch);
			send_to_char ("3	2d6+35	  		33	10d10+1100	\n\r",ch);
			send_to_char ("5	2d6+60	  		35	10d10+1300	\n\r",ch); 
			send_to_char ("7	2d6+85	  		37	10d10+1500	\n\r",ch);
			send_to_char ("9	2d6+110	  		39	15d10+1700	\n\r",ch);
			send_to_char ("11	2d8+134	  		41	25d10+2000 	\n\r",ch);
			send_to_char ("13	2d10+170 		43	25d10+2500 \n\r",ch); 
			send_to_char ("15	3d9+208	 		45	25d10+3000	\n\r",ch);
			send_to_char ("17	3d9+258	 		47	25d10+3500	\n\r",ch);
			send_to_char ("19	3d9+308	 		49	50d10+4000	\n\r",ch);
			send_to_char ("21	4d10+360 		51	50d10+5000	\n\r",ch); 
			send_to_char ("23	5d10+450 		53	50d10+6000	\n\r",ch);
			send_to_char ("25	5d10+500 		55	50d10+7000\n\r",ch);
			send_to_char ("27	5d10+650 		57	50d10+8000\n\r",ch);
			send_to_char ("29	6d12+778 		59	50d10+9000		\n\r",ch); 
			sprintf(buf,"Current Hit Points:\n\r`G%d  %d  %d`w\n\r",pMob2->hit[0],pMob2->hit[1],pMob2->hit[2]);
			send_to_char(buf,ch);
		return;
		}
		for (argument=one_argument(argument,t),s = 0; ((s < 3) && (t[0]!='\0')); s++,argument=one_argument(argument,t))
		{ 
			pMob2->hit[s] = atoi(t); 
		}
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
	send_to_char ("Hit Points set\n\r",ch);					    
		return;
	    }
	}
    if ( !strncmp( arg1, "dam_roll", 5 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_COMBAT, SECURITY_9))
	{ 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf ,"`BDam_roll     `G%d  %d  %d`w\n\r",pMob2->damage[0],pMob2->damage[1],pMob2->damage[2]);
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: Dam_roll <dice> <die> <bonus>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			send_to_char ("\n\r",ch); 
			send_to_char ("The damaging power of a mobile is expressed as ndn+n, just like hit points\n\r",ch); 
			send_to_char ("and mana.  It is very important to stay close to the values recommended below.\n\r",ch);
			send_to_char ("The dice will be rolled whenever the mobile lands a blow with or without\n\r",ch);
			send_to_char ("a weapon.\n\r",ch);
			send_to_char ("Example:  A level 12 would have approx 1d10+3 damage points.\n\r",ch);
			send_to_char ("Use the format: 0 0 0   Where 1 10 3 would be the above example.\n\r",ch); 
			send_to_char ("level	damage		level	Damage\n\r",ch);
			send_to_char ("1	1d4+0 		31	4d6+9\n\r",ch);
			send_to_char ("3	1d6+0		33	6d4+9\n\r",ch);
			send_to_char ("5	1d6+1		35	4d7+11\n\r",ch); 
			send_to_char ("7	1d8+1		37	3d10+12\n\r",ch);
			send_to_char ("9	1d8+2		39	5d6+13\n\r",ch);
			send_to_char ("11	1d10+2		41	4d8+15\n\r",ch);
			send_to_char ("13	2d5+3		43	3d12+15\n\r",ch); 
			send_to_char ("15	2d6+3		45	8d4+16\n\r",ch);
			send_to_char ("17	3d4+4		47	6d6+17\n\r",ch);
			send_to_char ("19	2d7+5		49	4d10+18\n\r",ch);
			send_to_char ("21	4d4+5		51	5d8+20\n\r",ch); 
			send_to_char ("23	3d6+6		53	6d7+21\n\r",ch);
			send_to_char ("25	2d10+7		55	6d8+23\n\r",ch);
			send_to_char ("27	5d4+7		57	6d8+24\n\r",ch);
			send_to_char ("29	2d12+8		59	8d6+26\n\r",ch); 
			 sprintf(buf,"Current Damage Roll:`G%d  %d  %d`w\n\r",pMob2->damage[0],pMob2->damage[1],pMob2->damage[2]);
			 send_to_char(buf,ch);
			 return;
		}
		for (argument=one_argument(argument,t),s = 0; ((s < 3) && (t[0]!='\0')); s++,argument=one_argument(argument,t))
		{ 
			pMob2->damage[s] = atoi(t); 
		}
	send_to_char ("Damage Roll set\n\r",ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
			return;
	    }
	}
    if ( !strncmp( arg1, "ac", 2 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_COMBAT, SECURITY_9))
	{ 
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BAc           `G%d\n\r",pMob2->armor);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: AC < armor class >\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			send_to_char ("The armor class works just like for players.\n\r",ch); 
			send_to_char ("Follow the chart as closely as possible.\n\r",ch); 
			send_to_char ("Keeping in mind some mobiles may have higher armor class ( easier to hit )\n\r",ch); 
			send_to_char ("and other less ( harder to hit ).\n\r",ch); 
			send_to_char ("level	Ave Armor	Level	Ave Armor\n\r",ch);
			send_to_char ("1	93		31	-132	\n\r",ch);
			send_to_char ("3	79		33	-146	\n\r",ch);
			send_to_char ("5	65		35	-160	\n\r",ch); 
			send_to_char ("7	46		37	-174	\n\r",ch);
			send_to_char ("9	32		39	-188	\n\r",ch);
			send_to_char ("11	18		41	-207	\n\r",ch);
			send_to_char ("13	4		43	-221	\n\r",ch); 
			send_to_char ("15	-10		45	-235	\n\r",ch);
			send_to_char ("17	-24		47	-249	\n\r",ch);
			send_to_char ("19	-38		49	-263	\n\r",ch);
			send_to_char ("21	-57		51	-277	\n\r",ch); 
			send_to_char ("23	-71	 	53	-291	\n\r",ch);
			send_to_char ("25	-85		55	-305	\n\r",ch);
			send_to_char ("27	-99		57	-319	\n\r",ch);
			send_to_char ("29	-113		59	-333	\n\r",ch); 
			sprintf(buf,"Current Armor Class:\n\r%d\n\r",pMob2->armor );
			send_to_char(buf,ch);
			return;
		}
	        argument=one_argument( argument, t );
		pMob2->armor = atoi(t);
	send_to_char ("Armor Class set\n\r",ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}
    if ( !strncmp( arg1, "fire", 4 ) || !strncmp( arg1, "water", 4 ) || 
    !strncmp( arg1, "wind", 4 ) ||  !strncmp( arg1, "spirit", 4 ) ||
    !strncmp( arg1, "earth", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_MAGIC, SECURITY_9))
	{ 
	if ( !str_cmp(arg1, "wind" ) )
		i = 3;
	if ( !str_cmp(arg1, "water" ) )
		i = 1;
	if ( !str_cmp(arg1, "earth" ) )
		i = 2;
	if ( !str_cmp(arg1, "spirit" ) )
		i = 4;
	if ( !str_cmp(arg1, "fire" ) )
		i = 0;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BFire         `G%d %d %d`w\n\r",pMob2->mana[0][0], pMob2->mana[1][0], pMob2->mana[2][0]);
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: fire <dice> <die> <bonus>\n\r",ch);	
	    sprintf(buf, "`BEarth        `G%d %d %d`w\n\r",pMob2->mana[0][2], pMob2->mana[1][2], pMob2->mana[2][2]);
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: earth <dice> <die> <bonus>\n\r",ch);	
	    sprintf(buf, "`BSpirit       `G%d %d %d`w\n\r",pMob2->mana[0][4], pMob2->mana[1][4], pMob2->mana[2][4]);
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: spirit <dice> <die> <bonus>\n\r",ch);	
	    sprintf(buf, "`BWater        `G%d %d %d`w\n\r",pMob2->mana[0][1], pMob2->mana[1][1], pMob2->mana[2][1]);
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: water <dice> <die> <bonus>\n\r",ch);	
	    sprintf(buf, "`BWind         `G%d %d %d`w\n\r",pMob2->mana[0][3], pMob2->mana[1][3], pMob2->mana[2][3]);
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: wind <dice> <die> <bonus>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		int s;
		if (arg2[0] == '\0')
		{ 
			send_to_char ("\n\r",ch); 
			send_to_char ("Mana dice are expressed as ndn+n.  This  \n\r",ch); 
			send_to_char ("value will determine how many spells your mobile can cast in combat\n\r",ch);
			send_to_char ("non-spellcasters have a mana value as well, as their mana can be tapped by\n\r",ch);
			send_to_char ("the energy drain spell.  A good guideline is 100 mana for creatures with\n\r",ch);
			send_to_char ("no (zero zip, completely none) magical talent,  100 + 1d10/level mana for\n\r",ch);
			send_to_char ("normal mobiles.  Particlarly powerful spell casters may have more mana\n\r",ch);
			send_to_char ("but not too much more. Use the format 0 0 0.  Example: 12 10 100 for level 12\n\r",ch);
			sprintf(buf,"Current %s mana:%d  %d  %d\n\r", element_type_name( i ), pMob2->mana[0][i],pMob2->mana[1][i],pMob2->mana[2][i]);
			send_to_char(buf,ch);
			return;
		}
		for (argument=one_argument(argument,t),s = 0; ((s < 4) && (t[0]!='\0')); s++,argument=one_argument(argument,t))
		{ 
			pMob2->mana[s][i] = atoi(t); 
		}
	send_to_char ("Mana set\n\r",ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}
    if ( !strncmp( arg1, "dam_type", 5 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_COMBAT, SECURITY_9))
	{ 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BDam_type     `G%d`w\n\r",pMob2->dam_type);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: Dam_type <dam type>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			buf3[0] = '\0';
			send_to_char ("A mobiles damage type defines three things: what message is seen when\n\r",ch);
			send_to_char ("it attacks in combat, what armor class is used to defend against the attack,\n\r",ch);
			send_to_char ("and what type of resistance or vulnerablility affects the damage received\n\r",ch);
			send_to_char ("from the attack.\n\r",ch);
			y=0;
			for ( x = 0; x < MAX_DAMAGE_TYPE; x++ )
			{
			     sprintf(buf, "%2d  %-13s  ",x, attack_table[x].name);
			     strcat(buf3, buf);
			y++;
			if ( y > 3 )
			{
			y = 0;
			strcat(buf3, "\n\r");
		        }
			}
			send_to_char (buf3, ch);
			send_to_char ("\n\r",ch);
			send_to_char ("*note* If the damage type you desire is not listed. Leave me a note with a\n\r",ch);
			send_to_char ("the NAME and a DESCRIPTION of how it should work.  Galadrail\n\r",ch);
			send_to_char ("\n\r",ch);
			sprintf(buf,"Current Damage Type:\n\r`G%d`w\n\r",pMob2->dam_type);
			send_to_char(buf,ch);
			return;
		}
		s = atoi (arg2);
		pMob2->dam_type = s;
	send_to_char ("Damage Type set\n\r",ch);					    
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	    }
	}

    if ( !strncmp( arg1, "hit_roll", 5 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_COMBAT, SECURITY_9))
     { 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`Bhit_roll     `G%d`w\n\r",pMob2->hitroll);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: hit_roll <value>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		if (arg2[0] == '\0')
		{ 
			send_to_char ("\n\r",ch); 
			send_to_char ("This statistic is normally 0, but in mobiles with  \n\r",ch); 
			send_to_char ("exceptional (and I do mean exceptional) combat abilities it may be\n\r",ch);
			send_to_char ("higher.  Clumsy mobiles may deserve a -1 or -2.  The main effect of\n\r",ch);
			send_to_char ("changing the to hit bonus for a mobile is making it more ( a positive\n\r",ch);
			send_to_char ("or less (a minus) likely to land a blow in combat.\n\r",ch);
			sprintf(buf,"Current Hitroll:\n\r`G%d`w\n\r",pMob2->hitroll);
			send_to_char(buf,ch);
			return;
		}
		s = atoi (arg2);
		pMob2->hitroll = s;
        	send_to_char( "Hitroll set.\n\r", ch);
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	 }
     }
    if ( !strncmp( arg1, "mood", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SPECIALS, SECURITY_9))
     { 
		int s;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BMood         `wSet the mood of the mobile\n\r",ch);
	    send_to_char("`w                  Syntax: mood <value>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
		buf2[0] = '\0';
		if (arg2[0] == '\0')
		{ 
			send_to_char ("The mood of a mob does absolutly nothing.\n\r",ch); 
			send_to_char ("What should a mob do differntly if it is having a bad day? \n\r",ch); 
			send_to_char ("Or perhaps it is having a good day.\n\r",ch);
			send_to_char ("Leave notes if you have any ideas.\n\r",ch);
			send_to_char ("\n\r",ch);
			send_to_char ("Socials can change the mood of the mob.\n\r",ch);
			send_to_char ("Heres a list of moods that can be set.\n\r",ch);
			for ( x = 0; x < MAX_MOOD; x++ )
			{
			sprintf(buf,"\n\r%d - %s",mood_table[x].type, mood_table[x].mood);
			strcat(buf2,buf);
			}
			send_to_char(buf2,ch);
			sprintf(buf,"\n\rCurrently This mob is %s\n\r", mood_table[pMob2->mood].mood);
			send_to_char(buf,ch);
			return;
		}
		s = atoi (arg2);
		if ( s < 0 || s > MAX_MOOD )
		{
        	send_to_char( "Invalid mood\n\r", ch);
		return;
		}

		pMob2->mood = s;
        	send_to_char( "Mood set.\n\r", ch);
        	SET_BIT( pArea->area_flags, AREA_CHANGED );
		return;
	}
     }

    if ( !strncmp( arg1, "short", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BShort        `G%s\n\r`w",pMob2->short_descr);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: short <string>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "The short description is what is seen by players\n\r", ch );
            send_to_char( "and other mobiles when the mobile moves or does some action.\n\r", ch );
            send_to_char( "The first letter should not be capatalized.\n\r", ch );
            send_to_char( "a small cat.\n\r", ch );
            send_to_char( "Syntax:  short [string]\n\r", ch );
	    sprintf(buf,"Mobile short: `G%s`w",pMob2->short_descr);
	    send_to_char(buf,ch);
            return;
        }

        strncpy( pMob2->short_descr, arg2, 80 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Short description set.\n\r", ch);
        return;
	}
    }

    if ( !strncmp( arg1, "move_mobile", 8 ) || !str_cmp( arg1, "??" ) )
    if (edit_function(ch, MEDIT_ANY, SECURITY_1))
    {
 	if ( !str_cmp( arg1, "??" ) )
	{
	    send_to_char("`Bmove_mobile  `wMove this mobile to another area\n\r",ch);
	    send_to_char("`w                  Syntax: move_mobile <area number>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  move_mobile [area number]\n\r", ch );
            return;
        }
   	if ( is_number( arg2 ) )
        value = atoi( arg2 );
	else
        {
            send_to_char( "Syntax:  move_mobile [area number]\n\r", ch );
            return;
        }
        if ( ( pArea_to = get_area_data( value ) ) == NULL )
	{
            send_to_char( "Invalid Area Number\n\r", ch );
            return;
	}
        if ( !IS_BUILDER( ch, pArea_to ) )
        {
            send_to_char( "MEdit:  Vnum in an area you cannot build in.\n\r", ch );
            return;
        }

        pMob2->load_with_area = pArea_to;
	
        strcpy(arg1, "mobs" );
        do_areasave( pArea, arg1, ch ); /* must save to update in memory and disk*/
        do_areasave( pArea_to, arg1, ch );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        SET_BIT( pArea_to->area_flags, AREA_CHANGED );
        send_to_char( "Mobile moved!\n\r", ch);
        return;
	}
    }

    if ( !strncmp( arg1, "long", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BLong         `G%s`w",pMob2->long_descr);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: long <string>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "This is what the player sees when looking in the room\n\r", ch );
            send_to_char( "and the mobile is in it also.\n\r", ch );
            send_to_char( "Capitalize the first letter.\n\r", ch );
            send_to_char( "A small white cat has a black stripe along its back.\n\r", ch );
            send_to_char( "Syntax:  long [string]\n\r", ch );
	    sprintf(buf,"Mobile long: `G%s`w",pMob2->long_descr);
	    send_to_char(buf,ch);
            return;
        }

        strcat( arg2, "\n\r" );
        strncpy( pMob2->long_descr, arg2, 80 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Long description set.\n\r", ch);
        return;
	}
    }

if ( !strncmp( arg1, "description", 4 ) || !str_cmp( arg1, "?" ) )
if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BDesc         `G\n\r%s`w",pMob2->description);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: desc <string>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            string_append( ch, &pMob2->description );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }

        if ( arg2[0] == '+' )
        {
            string_append( ch, &pMob2->description );
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            return;
        }

        send_to_char( "Syntax:  desc    - line edit\n\r", ch );
        send_to_char( "         desc +  - line append\n\r",ch );
        return;
	}
    }


    if ( !strncmp( arg1, "sex", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BSex          `G%s`w\n\r", pMob2->sex == 1 ? "Neuter" : pMob2->sex == 2 ? "Male" : pMob2->sex == 3 ? "Female" : "None" );
	    send_to_char(buf,ch);
	    send_to_char("`w                  Syntax: sex <mob sex>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
    if ( get_mob_sex_number( arg2 ) != SEX_NONE )
    {
        pMob2->sex = get_mob_sex_number( arg2 );

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Sex set.\n\r", ch);
        return;
    }
    else
    {
	send_to_char("Male, female, either, neuter\n\r",ch);	
	sprintf(buf, "Current mob sex `G%s`w\n\r", pMob2->sex == 1 ? "Neuter" : pMob2->sex == 2 ? "Male" : pMob2->sex == 3 ? "Female" : "Either" );
	send_to_char(buf, ch);
    }
    return;
    }
    }

    if ( !strncmp( arg1, "level", 3 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_MENU, SECURITY_9))
    {
	int total;
	int race = pMob2->race;
	int sex = pMob2->sex;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BLevel        `G%d`w\n\r",pMob2->level);
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: level <mob level>\n\r",ch);	
	    display = TRUE;
	}
	else
	{

        if ( arg2[0] == '\0' || !is_number( arg2 ) )
        {
            send_to_char( "Syntax:  level [number]\n\r", ch );
	    sprintf(buf,"Mobile level: %d",pMob2->level);
	    send_to_char(buf,ch);
            return;
        }

        pMob2->level = atoi( arg2 );

	pMob2->hit[ 0 ] = pMob2->level;
	pMob2->hit[ 1 ] = ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / ( 3 * pMob2->level + 3 ) - 1;
	pMob2->hit[ 2 ] = ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / 3;
	pMob2->damage[ 0 ] = ( ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / ( 4 * pMob2->level + 3 ) ) / 3 + 1;
	pMob2->damage[ 1 ] = ( ( 2 * ( pMob2->level + 1 ) * ( pMob2->level + 1 ) ) / ( 4 * pMob2->level + 3 ) ) / 3 + 3;
	pMob2->damage[ 2 ] = pMob2->level / 5;
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	{
	    pMob2->mana[ 0 ][ i ] = race_table[ race ][ sex ].mana_dice[ i ] * pMob2->level;
	    pMob2->mana[ 1 ][ i ] = race_table[ race ][ sex ].mana_die[ i ];
	    pMob2->mana[ 2 ][ i ] = race_table[ race ][ sex ].mana_bonus[ i ] * pMob2->level + 100;
	}
	number = number_range(25,75) * race_table[ race ][ sex ].gold * pMob2->level / 100;
	pMob2->gold[ race_table[ race ][ sex ].world ] = number;
	    total=100;
	    for ( x = 0 ; x < pMob2->level ; x++ )	
		total-= roll_dice( 2, 3 ) + 2;
	    pMob2->armor = total;

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Level set.\n\r", ch);

        return;
	}
    }

    if ( !strncmp( arg1, "groups", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SKILLS, SECURITY_9))
    {
	char buf1[MAX_INPUT_LENGTH];
	int c,num;
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BGroup        `wGroups mobile belongs to(for skill trainers/gainers only)\n\r",ch);
	    send_to_char("`w                  Syntax: group <skill/spell group>\n\r",ch);	
	    display = TRUE;
	}
	else
	{

        if ( arg2[0] == '\0' )
        {
            send_to_char( "Syntax:  group <skill/spell group>\n\r", ch );
	    sprintf( buf, "Groups:\n\r" );
	    num = 0;
	    for ( i = 0 ; i < MAX_GROUP ; i++ )
	    {
		sprintf( buf1, "     %20s", capitalize( group_name[ i ] ) );
		strcat( buf, buf1 );
		if ( num++ % 3 == 2 )
		    strcat( buf, "\n\r" );
	    }
	
	    if ( num % 3 != 0 )
		strcat( buf, "\n\r" );
	    send_to_char( buf, ch );

	    buf[ 0 ] = '\0';
	    for ( i = 0 ; i < 5 ; i++ )
	    {
		for ( c = 0 ; c < 3 ; c++ )
		{
		    sprintf( buf1, "     %20s", skill_table[ gsn_element_power[ i ][ c ] ].name );
		    strcat( buf, buf1 );
		}
		strcat( buf, "\n\r" );
	    }

	    strcat( buf, "               Minor Five                     Five               Major Five\n\r" );
	    send_to_char( buf, ch );
            return;
        }

	if ( !IS_SET( pMob2->act, ACT_GAIN ) )
	{
	    send_to_char( "This mob does not have the gainer flag.\n\r",ch );
	    return;
	}

	if ( pMob2->gainer_data == NULL )
	{
	    pMob2->gainer_data = gainer_data_alloc();
	    for ( i = 1 ; i < MAX_SKILL ; i++ ) pMob2->gainer_data->skill[i] = FALSE;
	}

	for ( i = 0 ; i < 5 ; i++ )
	    for ( c = 0 ; c < 3 ; c++ )
		if ( !str_prefix( arg2, skill_table[ gsn_element_power[ i ][ c ] ].name ) )
		{
		    for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
		    {
			if ( skill_table[ sn ].spell_fun != spell_null 
			  && skill_table[ sn ].element[ i ].level == c 
			  && skill_table[ sn ].element[ ( i + 1 ) % 5 ].level < 0 
			  && skill_table[ sn ].element[ ( i + 2 ) % 5 ].level < 0 
			  && skill_table[ sn ].element[ ( i + 3 ) % 5 ].level < 0 
			  && skill_table[ sn ].element[ ( i + 4 ) % 5 ].level < 0 )
			{
			    pMob2->gainer_data->skill[sn]  = TRUE;
			}
		    }
	            SET_BIT( pArea->area_flags, AREA_CHANGED );
	            send_to_char( "Skill toggled.\n\r", ch);
	            return;
		}

/* skill groups */

        for  ( i = 0 ; i < MAX_GROUP ; i++ )      
	{
	    if ( !str_prefix( arg2, group_name[ i ] ) )
	    {
		for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
		    if ( skill_table[ sn ].group == i )
		    {
			    pMob2->gainer_data->skill[sn]  = TRUE;
		    }

                SET_BIT( pArea->area_flags, AREA_CHANGED );
                send_to_char( "Skill toggled.\n\r", ch);
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
		    for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
		    {
			if ( skill_table[ sn ].spell_fun != spell_null 
			  && skill_table[ sn ].element[ 0 ].level == c 
			  && skill_table[ sn ].element[ 1 ].level == c 
			  && skill_table[ sn ].element[ 2 ].level == c 
			  && skill_table[ sn ].element[ 3 ].level == c 
			  && skill_table[ sn ].element[ 4 ].level == c )
			{
			    pMob2->gainer_data->skill[sn]  = TRUE;
			}
		    }

                SET_BIT( pArea->area_flags, AREA_CHANGED );
                send_to_char( "Skill toggled.\n\r", ch);
                return;
	    }
	}
    }

    if ( !strncmp( arg1, "skills", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SKILLS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BSkill        `wSkills the mobile has(for skill gainers only)\n\r",ch);
	    send_to_char("`w                  Syntax:    skill [skill_name]\n\r",ch);
	    send_to_char("`w                  Syntax:    skill clear (clear all skills)\n\r",ch);
	    send_to_char("`w                  Syntax:    skill show (for mob)\n\r",ch);
	    display = TRUE;
	}
	else
	{
	if ( is_number( arg2 ) )
            value = atoi( arg2 );
	else if ( arg2[0] != '\0' )
	    value = skill_lookup( arg2 );	    
	else
	{
	    for ( value = 1 ; value < MAX_SKILL ; value++ )
	    {
		sprintf( buf, "%-20s ", skill_table[ value ].name );		
		send_to_char( buf, ch );
		if ( value % 3 == 0 )
		    send_to_char( "\n\r", ch );
	    }
	    send_to_char("Syntax:    skill [skill_name]\n\r",ch);
	    send_to_char("Syntax:    skill clear (clear all skills)\n\r",ch);
	    send_to_char("Syntax:    skill show (for mob)\n\r",ch);
	    return;
	}

	if ( !IS_SET( pMob2->act,ACT_GAIN ) )
	{
	    send_to_char( "This mob does not have the gainer flag.\n\r",ch );
	    return;
	}

	if (!str_cmp(arg2, "clear") )
	{
	   if ( pMob2->gainer_data == NULL )
	   pMob2->gainer_data = gainer_data_alloc();

	    for ( i = 1 ; i < MAX_SKILL ; i++ ) pMob2->gainer_data->skill[i] = FALSE;
            send_to_char( "Cleared!\n\r", ch );
            return;
	}

	if (!str_cmp(arg2, "all") )
	{
	   if ( pMob2->gainer_data == NULL )
	   pMob2->gainer_data = gainer_data_alloc();

	    for ( i = 1 ; i < MAX_SKILL ; i++ ) pMob2->gainer_data->skill[i] = TRUE;
            send_to_char( "Done!\n\r", ch );
            return;
	}
	if (!str_cmp(arg2, "show") )
	{
	   if ( pMob2->gainer_data == NULL )
	    {
		send_to_char( "None\n\r", ch );
		return;
	    }

	    for ( i = 1 ; i < MAX_SKILL ; i++ ) 
		if ( pMob2->gainer_data->skill[i] == TRUE )
	        {
		    sprintf( buf, "%-20s ", skill_table[ i ].name );		
		    send_to_char( buf, ch );
		    if ( i % 3 == 0 )
		        send_to_char( "\n\r", ch );
	        }
            return;
	}

	if ( value < 1 || value > MAX_SKILL)
	{
	    send_to_char( "Invalid Skill\n\r", ch );
	    return;
	}

	if ( pMob2->gainer_data == NULL )
	{
	    pMob2->gainer_data = gainer_data_alloc();
	    for ( i = 1 ; i < MAX_SKILL ; i++ ) pMob2->gainer_data->skill[i] = FALSE;
	}

	pMob2->gainer_data->skill[value]  = !pMob2->gainer_data->skill[value];

        SET_BIT( pArea->area_flags, AREA_CHANGED );
        send_to_char( "Skill toggled.\n\r", ch);
        return;
	}
    }


    if ( !strncmp( arg1, "imm_bit", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_AFFECTS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BImm_bit      `G%s`w\n\r",imm_bit_name(pMob2->imm_flags));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: imm_bit <imm_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            do_help( ch, "imm_name_bit" );
            send_to_char( "\n\rSyntax:  imm_bit [imm_name]\n\r", ch );
	    sprintf(buf, "Mobile immunities: %s\n\r",imm_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }

       if ( ( value = imm_name_bit( arg2 ) ) != AFFECT_NONE )
        {
           pMob2->imm_flags ^= value;
  
           SET_BIT( pArea->area_flags, AREA_CHANGED );
          send_to_char( "Immunity Type flag toggled.\n\r", ch);
          return;
        }
	}
    }
    if ( !strncmp( arg1, "forms", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BForms        `G%s\n\r`w",form_bit_name(pMob2->form));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: forms <form_bit_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "\n\rSyntax:  form [form_bit_name]\n\r", ch );
	    sprintf(buf, "Mobile Form: %s\n\r",form_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }

       if ( ( value = form_name_bit( arg2 ) ) != AFFECT_NONE )
        {
           pMob2->form ^= value;
  
           SET_BIT( pArea->area_flags, AREA_CHANGED );
          send_to_char( "Form flag toggled.\n\r", ch);
          return;
        }
	}
    }
    if ( !strncmp( arg1, "res_bit", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_AFFECTS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BRes_bit      `G%s`w\n\r",res_bit_name(pMob2->res_flags));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: res_bit <res_bit_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            do_help( ch, "res_name_bit" );
            send_to_char( "\n\rSyntax:  res_bit [res_name]\n\r", ch );
	    sprintf(buf, "Mobile resistance: %s\n\r",res_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }

       if ( ( value = res_name_bit( arg2 ) ) != AFFECT_NONE )
        {
           pMob2->res_flags ^= value;
  
           SET_BIT( pArea->area_flags, AREA_CHANGED );
          send_to_char( "Resistance Type flag toggled.\n\r", ch);
          return;
        }
	}
    }

    if ( !strncmp( arg1, "vuln_bit", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_AFFECTS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BVuln_bit     `G%s`w\n\r",vuln_bit_name(pMob2->vuln_flags));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: vuln_bit <vuln_bit_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            do_help( ch, "vuln_name_bit" );
            send_to_char( "\n\rSyntax:  vuln_bit [vuln_name]\n\r", ch );
	    sprintf(buf, "Mobile vulnerbilites: %s\n\r",vuln_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }

       if ( ( value = vuln_name_bit( arg2 ) ) != AFFECT_NONE )
        {
           pMob2->vuln_flags ^= value;
  
           SET_BIT( pArea->area_flags, AREA_CHANGED );
          send_to_char( "Vulnerabiltiy Type flag toggled.\n\r", ch);
          return;
        }
	}
    }


    if ( !strncmp( arg1, "special", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_SKILLS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    send_to_char("`BSpecial      `wAdd or remove special processes for the moible\n\r",ch);
	    send_to_char("`w                  Syntax: special <special name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            do_help( ch, "special" );
            send_to_char( "\n\rSyntax:  special [special_name]\n\r", ch );
            return;
        }

       strncpy( pMob2->spec_name, arg2, 30 );
       if ( !str_cmp( pMob2->spec_name, "none" ) )
       {
	   pMob2->spec_fun = NULL;
           send_to_char( "Special set to Null.\n\r", ch);
           return;
       }
	
       pMob2->spec_fun = spec_lookup(pMob2->spec_name);
       if (pMob2->spec_fun == NULL)
       {
	   pMob2->spec_fun = NULL;
           send_to_char( "Not a valid special\n\r", ch);
           return;
       }
           send_to_char( "Special set.\n\r", ch);
           return;
	}

   }


    if ( !strncmp( arg1, "act_bit", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_AFFECTS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BAct_bit      `G%s`w\n\r",act_bit_name(pMob2->act));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: act_bit <act_bit_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "\n\rSyntax:  act_bit [act_bit_name]\n\r", ch );
	    sprintf(buf, "Mobile act: %s\n\r",act_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }


       if ( ( value = act_name_bit( arg2 ) ) != ACT_NONE )
       {
           pMob2->act ^= value;
           SET_BIT( pMob2->act, ACT_IS_NPC );

           SET_BIT( pArea->area_flags, AREA_CHANGED );
           send_to_char( "Act flag toggled.\n\r", ch);
           return;
       }
       if ( ( value = act2_name_bit( arg2 ) ) != ACT_NONE )
       {
           pMob2->act2 ^= value;

           SET_BIT( pArea->area_flags, AREA_CHANGED );
           send_to_char( "Act2 flag toggled.\n\r", ch);
           return;
       }
	send_to_char( "Invalid act flag.\n\r", ch );
            send_to_char( "\n\rSyntax:  act_bit [act_bit_name]\n\r", ch );
	    sprintf(buf, "Mobile act: %s\n\r",act_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
	}
   }

    if ( !strncmp( arg1, "act2_bit", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_AFFECTS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BAct2_bit     `G%s`w\n\r",act2_bit_name(pMob2->act2));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: act2_bit <act2_bit_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "\n\rSyntax:  act2_bit [act2_bit_name]\n\r", ch );
	    sprintf(buf, "Mobile act2: %s\n\r",act2_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }


       if ( ( value = act2_name_bit( arg2 ) ) != ACT_NONE )
       {
           pMob2->act2 ^= value;

           SET_BIT( pArea->area_flags, AREA_CHANGED );
           send_to_char( "Act2 flag toggled.\n\r", ch);
           return;
       }
            send_to_char( "\n\rSyntax:  act2_bit [act2_bit_name]\n\r", ch );
	    sprintf(buf, "Mobile act2: %s\n\r",act2_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
	}
   }


    if ( !strncmp( arg1, "affect_bit", 6 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_AFFECTS, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BAffect_bit   `G%s`w\n\r",affect_bit_name(pMob2->affected_by));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: affect_bit <affect_bit_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "\n\rSyntax:  affect_bit [affect_bit_name]\n\r", ch );
	    sprintf(buf, "Mobile affected by: %s\n\r",affect_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }

       if ( ( value = affect_name_bit( arg2 ) ) != AFFECT_NONE )
        {
           pMob2->affected_by ^= value;
  
           SET_BIT( pArea->area_flags, AREA_CHANGED );
          send_to_char( "Affect flag toggled.\n\r", ch);
          return;
        }
       if ( ( value = affect_name_bit_2( arg2 ) ) != AFFECT_NONE )
        {
           pMob2->affected_by_2 ^= value;
  
           SET_BIT( pArea->area_flags, AREA_CHANGED );
          send_to_char( "Affect flag toggled.\n\r", ch);
          return;
        }
	}
    }

    if ( !strncmp( arg1, "off_flags", 4 ) || !str_cmp( arg1, "?" ) )
    if (edit_function(ch, MEDIT_FORM_COMBAT, SECURITY_9))
    {
 	if ( !str_cmp( arg1, "?" ) )
	{
	    sprintf(buf, "`BOff_flags    `G%s`w\n\r",off_bit_name(pMob2->off_flags));
	    send_to_char(buf, ch);
	    send_to_char("`w                  Syntax: off_flags <off_flag_name>\n\r",ch);	
	    display = TRUE;
	}
	else
	{
        if ( arg2[0] == '\0' )
        {
            send_to_char( "\n\rSyntax:  off_flag [off_name_flag]\n\r", ch );
	    sprintf(buf, "Mobile off: %s\n\r",off_bit_name(ALL_FLAGS));
	    send_to_char(buf,ch);
            return;
        }

       if ( ( value = off_name_bit( arg2 ) ) != AFFECT_NONE )
        {
           pMob2->off_flags ^= value;
  
           SET_BIT( pArea->area_flags, AREA_CHANGED );
          send_to_char( "Offensive flag toggled.\n\r", ch);
          return;
        }
	}
    }

    if (!display)
    interpret( ch, arg );
    return;
}
void do_medit( CHAR_DATA *ch, char *argument )
{
    MOB_INDEX_DATA *pMob2;
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
        if ( ( pMob2 = get_mob_index( value ) ) == NULL )
        {
            send_to_char( "MEdit:  That vnum does not exist.\n\r", ch );
            return;
        }
        pArea = pMob2->load_with_area;
          if ( !IS_BUILDER( ch, pArea ) )
           {
           send_to_char( "MEdit:  Vnum in an area you cannot build in.\n\r", ch );
           return;
           }

        ch->desc->pEdit = (void *)pMob2;
        ch->desc->connected = CON_MEDITOR;
	ch->edit_menu	= MEDIT_MENU;
    	ch->edit_buffer[0] = '\0';
        return;
    }
    else
    {
        if ( !str_cmp( arg1, "create" ) )
        {
            value = atoi( argument );
	    if ( *argument == '\0' )
	    {
		for ( vnum = ch->in_room->area->lvnum ; free_mob_list[ vnum ] ; vnum++ );

		if ( vnum <= ch->in_room->area->uvnum )
		    value = vnum;
	    }

            if ( value == 0 )
            {
                send_to_char( "Syntax:  medit create [vnum]\n\r", ch );
                return;
            }

            pArea = ch->in_room->area;

            if ( pArea->lvnum > value || pArea->uvnum < value )
            {
                send_to_char( "MEdit:  That vnum is not assigned to this area.\n\r", ch );
                return;
            }

            if ( !IS_BUILDER( ch, pArea ) )
            {
                send_to_char( "MEdit:  Vnum in an area you cannot build in.\n\r", ch );
                return;
            }

            if ( get_mob_index( value ) != NULL )
            {
                send_to_char( "MEdit:  Mobile vnum already exists.\n\r", ch );
                return;
            }

            pMob2                    = alloc_pMobIndex();
            pMob2->vnum              = value;
            pMob2->load_with_area              = pArea;
            pMob2->act               = ACT_IS_NPC;
            pMob2->sex               = SEX_MALE; /* need a default sex */
            
         /*   if ( value > top_vnum_mob ) top_vnum_mob = value; */

            iHash                   = value % MAX_KEY_HASH;
            pMob2->next              = mob_index_hash[iHash];
            mob_index_hash[iHash]   = pMob2;
            ch->desc->pEdit = (void *)pMob2;
	    free_mob_list[ value ] = TRUE;
	    if (LOGON_ON)
	    if (ch->logon_data != NULL )
	    ch->logon_data->mobiles++;
            SET_BIT( pArea->area_flags, AREA_CHANGED );
            ch->desc->connected = CON_MEDITOR;
    	    ch->edit_menu = MEDIT_MENU;
    	    ch->edit_buffer[0] = '\0';
            return;
        }
    }

    send_to_char( "MEdit:  There is no default mobile to edit.\n\r", ch );
    return;
}

void do_mshow( CHAR_DATA *ch, char *argument )
{
    char buf1[MAX_STRING_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *victim;
    CHAR_DATA * victim1;
    EXTRA_DESCR_DATA * extra;
    TRIGGER_DATA * trig;
    SKILL_LIST * skills;
    int i;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
    send_to_char( "MShow whom?\n\r", ch );
        return;
    }

    if ( is_number( arg ) )
    {
        if ( ( victim = get_mob_index( atoi( arg ) ) ) == NULL )
        {
    	    send_to_char( "Invalid mob index VNUM.\n\r", ch );
            return;
	}
    }
    else if ( ( victim1 = get_char_world( ch, argument ) ) == NULL  || ( victim = victim1->pIndexData ) == NULL )
    {
	send_to_char( "Mob doesn't exist.\n\r", ch );
	return;
    }

    sprintf( buf, "`wName: '`c%s`w'\n\rArea %d: '`c%s`w'\n\r",
    victim->player_name,
    victim->load_with_area == NULL ? -1        : victim->load_with_area->areanum,
    victim->load_with_area == NULL ? "No Area" : victim->load_with_area->name );
    send_to_char( buf, ch );


    sprintf( buf, "`wVnum: `c%ld `wSex: `c%s  `wRace: `c%s\n\r",
    victim->vnum, 
    victim->sex == SEX_MALE    ? "male"   :
    victim->sex == SEX_FEMALE  ? "female" : "neuter",
    race_table[ victim->race ][ victim->sex ].name );
    send_to_char( buf, ch );

    sprintf( buf,
    "`wLevel: `c%d  `wSize: `c%d  `wMood: `c%s\n\r",
    victim->level, victim->size, mood_table[ victim->mood ].mood );
    send_to_char( buf, ch );

    sprintf( buf, "`wAffected by: `c%s ",
                  affect_bit_name( victim->affected_by ) );
    send_to_char( buf, ch );
    sprintf( buf, "%s\n\r",
                  affect_bit_name_2( victim->affected_by_2 ) );
    send_to_char( buf, ch );

    sprintf( buf, "`wShort description: `c%s\n\r`wLong description:\n\r`c%s",
    victim->short_descr,
    victim->long_descr );
    send_to_char( buf, ch );

    sprintf( buf, "`wDescription:\n\r`c%s", victim->description );
    send_to_char( buf, ch );

    buf[ 0 ] = '\0';
    for ( extra = victim->extra_descr ; extra != NULL ; extra = extra->next )
    {
	strcat( buf, " " );
	strcat( buf, extra->keyword );
    }

    if ( buf[ 0 ] != '\0' )
    {
	strcat( buf, "\n\r" );
	send_to_char( &buf[ 1 ], ch );
    }    

    sprintf( buf, "Triggers: " );
    for ( trig = victim->triggers ; trig != NULL ; trig = trig->next )
    {
	strcat( buf, " " );
	strcat( buf, trig->name );
    }

    if ( strlen( buf ) > 10 )
    {
	strcat( buf, "\n\r" );
	send_to_char( buf, ch );
    }

    if ( victim->guilds != 0 )
    {
	sprintf( buf, "`wGuilds: `c%s  ", guild_bit_name( victim->guilds ) );
	send_to_char( buf, ch );
    }

    /* castle code */
    if ( victim->castle_area != 0 )
    {
	sprintf( buf, "`wCastle_Area: `c%d  ", victim->castle_area );
	send_to_char( buf, ch );
    }

    if ( victim->movement_msg[0] != '\0' )
    {
	sprintf( buf, "`wMovement:  `c%s  ", victim->movement_msg );
	send_to_char( buf, ch );
    }

    if ( victim->follower > 0 )
    {
	sprintf( buf, "`wFollows: `c%ld  ", victim->follower );
	send_to_char( buf, ch );
    }

    sprintf( buf, "`wHp: `c%dd%d+%d\n\r",
	victim->hit[ 0 ],
	victim->hit[ 1 ],
	victim->hit[ 2 ]);
    send_to_char( buf, ch );

    buf1[0] = '\0';
    for( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        sprintf( buf, "  %s: %dd%d+%d",
	    element_type_name( i ),
	    victim->mana[ 0 ][ i ],
	    victim->mana[ 1 ][ i ],
	    victim->mana[ 2 ][ i ]);
	strcat( buf1, buf );
    }
    strcat( buf1, "\n\r" );
    send_to_char( &buf1[2], ch );

    sprintf( buf, "`wArmor: `c%d  ", victim->armor );
    send_to_char( buf, ch );

    sprintf( buf, "`wHitroll: `c%d  `wDamage: `c%dd%d+%d  `wDamage type: `c%s\n\r",
	victim->hitroll,
	victim->damage[ 0 ],
	victim->damage[ 1 ],
	victim->damage[ 2 ],
	get_type_weapon( victim->dam_type ) );
    send_to_char( buf, ch );

    sprintf( buf, "`wStarting Position: `c%s  `wDefault Position: `c%s\n\r",
	get_pos_name( victim->start_pos ),
	get_pos_name( victim->default_pos ) );
    send_to_char( buf, ch );

    if ( victim->spec_fun != NULL )
    {
        sprintf( buf, "Spec fun: %s\n\r",  victim->spec_name );
        send_to_char( buf, ch );
    }
    if ( victim->pShop != NULL )
    {
        SHOP_DATA *pShop;

        pShop = victim->pShop;

        sprintf( buf, "Shop data (for %ld):\n\r    Will buy at %d%%, and sell at %d%%.\n\r",
                      pShop->keeper, pShop->profit_buy, pShop->profit_sell );
        send_to_char( buf, ch );
        sprintf( buf, "    Opens at %d, closes at %d.\n\r",
                      pShop->open_hour, pShop->close_hour );
        send_to_char( buf, ch );

	sprintf( buf, "    Type - %s\n\r", shop_type_name( pShop->type ) );
	send_to_char( buf, ch );
/*    	sprintf( buf, "     Owner - %s\n\r", pShop->owner );
    	send_to_char( buf, ch );
 */
    }

    sprintf( buf, "Off_flags: %s\n\r", off_bit_name( victim->off_flags ) );
    send_to_char( buf, ch );
    sprintf( buf, "Immune: %s\n\r", imm_bit_name( victim->imm_flags ) );
    send_to_char( buf, ch );
    sprintf( buf, "Resistant: %s\n\r", res_bit_name( victim->res_flags ) );
    send_to_char( buf, ch );
    sprintf( buf, "Vulnerable: %s\n\r", vuln_bit_name( victim->vuln_flags ) );
    send_to_char( buf, ch );
    sprintf( buf, "Form: %s\n\r", form_bit_name( victim->form ) );
    send_to_char( buf, ch );
    sprintf( buf, "Act: %s %s\n\r", act_bit_name( victim->act ), act2_bit_name( victim->act2 ) );
    send_to_char( buf, ch );

	if (victim->prac_skills >= 1)
	{
	sprintf(buf, "Practices Alloted to Skills: %d\n\r",victim->prac_skills);
	send_to_char( buf, ch );
	}
	if (victim->prac_spells >= 1)
	{
	sprintf(buf, "Practices Alloted to Spells: %d\n\r",victim->prac_spells);
	send_to_char( buf, ch );
	}
    if (victim->innate_skills != NULL )
    {
	send_to_char("Current list of available skills and spells\n\r",ch);
	send_to_char("===========================================\n\r",ch);
	for ( skills = victim->innate_skills ; skills !=NULL ; skills = skills->next )
	{
		sprintf(buf, "%s  \n\r",skill_table[skills->sn].name);
		send_to_char( buf, ch );
	}
    }
    return;
}
