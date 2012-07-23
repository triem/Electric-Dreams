/* New commands for rooms by Quin, to be used with triggers by Galadrail */
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

typedef	struct	room_cmd_type		ROOM_CMD_TYPE;

struct room_cmd_type
{
    char * const	name;
    ROOM_FUN *		room_fun;
};

void do_room( CHAR_DATA * ch, char * argument )
{
    room_interpret( ch->in_room, argument );
}

void room_rand_stat( ROOM_INDEX_DATA * pRoom, char * argument )
{
    CHAR_DATA * ch;
    char arg[MAX_INPUT_LENGTH];
    int stat,stat2;
    int value;
    int i;

    argument = one_argument( argument, arg );

    for ( ch = pRoom->people ; ch != NULL && !is_name( arg, ch->name ) ; ch = ch->next_in_room );

    if ( ch == NULL )
	return;

    for ( i = number_range( 2, 5 ) ; i > 0 ; i-- )
    {
	value = UMAX( 1, number_range( 0, 2 ) );
	stat = number_range( 0, 5 );
	while( ( stat2 = number_range( 0, 5 ) ) == stat );
	ch->perm_stat[ stat ]  += value;
	ch->perm_stat[ stat2 ] -= value;
    }
}

void room_change( ROOM_INDEX_DATA * pRoom, char * argument )
{
    CHAR_DATA * ch;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    int stat = STAT_STR;
    int coin_type;
    int value;
    int flag;
    int sn;

    argument = one_argument( argument, arg );

    for ( ch = pRoom->people ; ch != NULL && !is_name( arg, ch->name ) ; ch = ch->next_in_room );

    if ( ch == NULL )
	return;

    argument = one_argument( argument, arg );

    argument = one_argument( argument, arg2 );

    if ( arg2[0] == '\0' )
	value = 0;
    else if ( !is_number( arg2 ) )
	return;
    else
	value = atoi( arg2 );

    argument = one_argument( argument, arg2 );

    if ( !str_cmp( arg, "str" ) ) stat = STAT_STR;
    else if ( !str_cmp( arg, "dex" ) ) stat = STAT_DEX;
    else if ( !str_cmp( arg, "wis" ) ) stat = STAT_WIS;
    else if ( !str_cmp( arg, "con" ) ) stat = STAT_CON;
    else if ( !str_cmp( arg, "int" ) ) stat = STAT_INT;
    else if ( !str_cmp( arg, "chr" ) ) stat = STAT_CHR;
    else if ( !str_cmp( arg, "fire" ) )
    {
	ch->max_mana[ ELEMENT_FIRE ] = UMAX( 0 , ch->max_mana[ ELEMENT_FIRE ] + value );
	ch->pcdata->perm_mana[ ELEMENT_FIRE ] = ch->max_mana[ ELEMENT_FIRE ];
	ch->mana[ ELEMENT_FIRE ] = ch->max_mana[ ELEMENT_FIRE ];
	return;
    }
    else if ( !str_cmp( arg, "water" ) )
    {
	ch->max_mana[ ELEMENT_WATER ] = UMAX( 0 , ch->max_mana[ ELEMENT_WATER ] + value );
	ch->mana[ ELEMENT_WATER ] = ch->max_mana[ ELEMENT_WATER ];
	ch->pcdata->perm_mana[ ELEMENT_WATER ] = ch->max_mana[ ELEMENT_WATER ];
	return;
    }
    else if ( !str_cmp( arg, "earth" ) )
    {
	ch->max_mana[ ELEMENT_EARTH ] = UMAX( 0 , ch->max_mana[ ELEMENT_EARTH ] + value );
	ch->mana[ ELEMENT_EARTH ] = ch->max_mana[ ELEMENT_EARTH ];
	ch->pcdata->perm_mana[ ELEMENT_EARTH ] = ch->max_mana[ ELEMENT_EARTH ];
	return;
    }
    else if ( !str_cmp( arg, "wind" ) )
    {
	ch->max_mana[ ELEMENT_WIND ] = UMAX( 0 , ch->max_mana[ ELEMENT_WIND ] + value );
	ch->mana[ ELEMENT_WIND ] = ch->max_mana[ ELEMENT_WIND ];
	ch->pcdata->perm_mana[ ELEMENT_WIND ] = ch->max_mana[ ELEMENT_WIND ];
	return;
    }
    else if ( !str_cmp( arg, "spirit" ) )
    {
	ch->max_mana[ ELEMENT_SPIRIT ] = UMAX( 0 , ch->max_mana[ ELEMENT_SPIRIT ] + value );
	ch->mana[ ELEMENT_SPIRIT ] = ch->max_mana[ ELEMENT_SPIRIT ];
	ch->pcdata->perm_mana[ ELEMENT_SPIRIT ] = ch->max_mana[ ELEMENT_SPIRIT];
	return;
    }
    else if ( !str_cmp( arg, "hit" ) || !str_cmp( arg, "hp" ) )
    {
	ch->max_hit = UMAX( 1, ch->max_hit + value );
	ch->hit = ch->max_hit;
	ch->pcdata->perm_hit = ch->max_hit;
	return;
    }
    else if ( !str_cmp( arg, "move" ) || !str_cmp( arg, "mv" ) )
    {
	ch->max_move = UMAX( 1, ch->max_move + value );
	ch->move = ch->max_move;
	ch->pcdata->perm_move = ch->max_move;
	return;
    }
    else if ( ( coin_type = get_coin_type( arg ) ) != -1 )
    {
	ch->gold[ coin_type ] = UMAX( 0, ch->gold[ coin_type ] + value );
	return;
    }
    else if ( !str_cmp( arg, "gold" ) )
    {
	ch->gold[ race_table[ ch->race ][ ch->sex ].world ] += value;
	return;
    }
    else if ( !strncmp( arg, "practices", 4 ) )
    {
	ch->practice = UMAX( 0, ch->practice + value );
	return;
    }
    else if ( !strncmp( arg, "vulnerable", 4 ) )
    {
	flag = vuln_name_bit( arg2 );
	if ( value < 0 )
	    REMOVE_BIT( ch->perm_vuln, flag );
	else if ( value > 0 )
	    SET_BIT( ch->perm_vuln, flag );
	else
	    TOGGLE_BIT( ch->perm_vuln, flag );
	return;
    }
    else if ( !strncmp( arg, "resistance", 3 ) )
    {
	flag = res_name_bit( arg2 );
	if ( value < 0 )
	    REMOVE_BIT( ch->perm_res, flag );
	else if ( value > 0 )
	    SET_BIT( ch->perm_res, flag );
	else
	    TOGGLE_BIT( ch->perm_res, flag );
	return;
    }
    else if ( !strncmp( arg, "immune", 3 ) )
    {
	flag = imm_name_bit( arg2 );
	if ( value < 0 )
	    REMOVE_BIT( ch->perm_imm, flag );
	else if ( value > 0 )
	    SET_BIT( ch->perm_imm, flag );
	else
	    TOGGLE_BIT( ch->perm_imm, flag );
	return;
    }
    else if ( !strncmp( arg, "affect", 3 ) )
    {
	if ( affect_name_bit( arg2 ) != 0)
	{
		flag = affect_name_bit( arg2 );
		if ( value < 0 )
		{
		    REMOVE_BIT( ch->affected_by, flag );
		    REMOVE_BIT( ch->pcdata->perm_aff, flag );
		}
		else if ( value > 0 )
		{
		    SET_BIT( ch->affected_by, flag );
		    SET_BIT( ch->pcdata->perm_aff, flag );
		}
		else
		{
		    TOGGLE_BIT( ch->affected_by, flag );
		    TOGGLE_BIT( ch->pcdata->perm_aff, flag );
		}
	}
	else if ( affect_name_bit_2( arg2 ) != 0)
	{
		flag = affect_name_bit_2( arg2 );
		if ( value < 0 )
		{
		    REMOVE_BIT( ch->affected_by_2, flag );
		    REMOVE_BIT( ch->pcdata->perm_aff_2, flag );
		}
		else if ( value > 0 )
		{
		    SET_BIT( ch->affected_by_2, flag );
		    SET_BIT( ch->pcdata->perm_aff_2, flag );
		}
		else
		{
		    TOGGLE_BIT( ch->affected_by_2, flag );
		    TOGGLE_BIT( ch->pcdata->perm_aff_2, flag );
		}
	}
	else
	{
		send_to_char( "affect_name_bit not found.\n\r", ch );
	}
	return;
    }
    else if ( !strncmp( arg, "skills", 3 ) )
    {
	if ( value < 0 )
	return;
	if ( ( sn = skill_lookup( arg2 ) ) > 0 )
	{
	    ch->pcdata->learned[sn] = value;
	}
    return;
    }
    else return;

    ch->perm_stat[ stat ] += value;
}

void room_rset( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[ MAX_INPUT_LENGTH ];
    CHAR_DATA * ch;
    int a,b,i,sn;
    int race;

    argument = one_argument( argument, arg );

    for ( ch = pRoom->people ; ch != NULL && !is_name( arg, ch->name ) ; ch = ch->next_in_room ) ;

    if ( ch == NULL || IS_NPC( ch ) )
	return;

    argument = one_argument( argument, arg );

    if ( !str_cmp( arg, "random" ) )
	for ( race = number_range( 0, top_race - 1 ) ; !race_table[ race ][ ch->sex ].pc_race ; race = number_range( 0, top_race - 1 ) );
    else if ( ( race = race_lookup( arg ) ) < 0 )
	return;

        ch->race = race;
	/* initialize stats */
	for (i = 0; i < MAX_STATS; i++)
	{
	    ch->perm_stat[i] = race_table[race][ch->sex].stats[i];
	    ch->mod_stat[i]=0;
	}
	ch->affected_by = race_table[race][ch->sex].aff;
	ch->pcdata->perm_aff = race_table[race][ch->sex].aff;
	ch->imm_flags	= race_table[race][ch->sex].imm;
	ch->res_flags	= race_table[race][ch->sex].res;
	ch->vuln_flags	= race_table[race][ch->sex].vuln;
	ch->form	= race_table[race][ch->sex].form;
	ch->size = race_table[race][ch->sex].size;
	ch->height = get_height_size( ch->size );
	ch->weight = get_weight_size( ch->size );
        strncpy( ch->pcdata->prompt, "`yHp %c`y/`w%H `yMv %d`y/`w%V`y-> ", 160 );

	    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	    {
	        ch->max_mana[i] = race_table[ ch->race ][ ch->sex ].mana_die[ i ] * race_table[ ch->race ][ ch->sex ].mana_dice[ i ] + race_table[ ch->race ][ ch->sex ].mana_bonus[ i ];
		ch->max_mana[i] *= 20;
	        ch->mana[i] = ch->max_mana[ i ];
	        ch->pcdata->perm_mana[i] = ch->max_mana[ i ];
	    }

	    for ( sn = 0; sn < MAX_SKILL; sn++ )
		if (skill_table[sn].name != NULL )
		{
		    if ( race_table[ ch->race ][ ch->sex ].gained[sn] == TRUE )
			ch->pcdata->learned[sn] = race_table[ ch->race ][ ch->sex ].starting[sn];
		    else
			ch->pcdata->learned[sn] = 0;
		}
	    obj_to_char(create_object(get_obj_index(race_table[ch->race][ch->sex].map)),ch); 
	    obj_to_char(create_object(get_obj_index(OBJ_VNUM_TENT)),ch); 
	    obj_to_char(create_object(get_obj_index(OBJ_VNUM_HANDBOOK)),ch); 
	    equip_char(ch,create_object(get_obj_index(OBJ_VNUM_LIGHT_BALL)),WEAR_LIGHT);

	    for( a = 0 ; a < top_race ; a++ )
		for ( b = 0 ; b < 3 ; b++ )
		    if ( race_table[a][b].world == race_table[ ch->race ][ ch->sex ].world )
			ch->pcdata->learned[ race_table[a][b].language ] = 50;

	    ch->pcdata->learned[ gsn_common ] = 100;
            if ( race_table[ ch->race ][ ch->sex ].world == 0 )
                ch->pcdata->learned[ gsn_swim ] = 100;

	    if ( ch->race == 8 )
		ch->pcdata->learned[ gsn_sneak ] = 100;

	    ch->pcdata->learned[ race_table[ ch->race ][ ch->sex ].language ] = 100;
	    ch->comm = COMM_PROMPT|COMM_COMBINE;
	    do_bonus(ch,"");
}

void room_teleport( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * ch;
    ROOM_INDEX_DATA * to_room;
    long vnum;

    argument = one_argument( argument, arg );

    for ( ch = pRoom->people; ch != NULL && !is_name( arg, ch->name ) ; ch = ch->next_in_room );

    if ( ch == NULL )
	return;

    argument = one_argument( argument, arg );

    if ( !is_number( arg ) || ( vnum = atoi( arg ) ) < 1 )
	return;

    for ( to_room = room_index_hash[ vnum % MAX_KEY_HASH ] ; to_room != NULL && to_room->vnum != vnum ; to_room = to_room->next );

    if ( to_room == NULL )
	return;

    char_from_room( ch );
    char_to_room( ch, to_room );
    do_look( ch, "auto" );
}

void room_at( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * at_room;
    long vnum;

    argument = one_argument( argument, arg );

    if ( !is_number( arg ) || ( vnum = atoi( arg ) ) < 1 )
	return;

    for ( at_room = room_index_hash[ vnum % MAX_KEY_HASH ] ; at_room != NULL && at_room->vnum != vnum ; at_room = at_room->next );

    if ( at_room != NULL )
	room_interpret( at_room, argument );
}

void room_techo( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * ch;
    bool trig;

    argument = one_argument( argument, arg );

    for ( ch = pRoom->people ; ch != NULL ; ch = ch->next_in_room )
	if ( is_name( arg, ch->name ) )
	{
	    send_to_char( argument, ch );
	    send_to_char( "\n\r", ch );
	    trig = do_trigger( ch, ch, NULL, argument, TRIG_TROOM, TRIG_TYPE_MOB);
	    return;
	}

    for ( ch = char_list ; ch != NULL ;  ch = ch->next )
	if ( is_name( arg, ch->name ) )
	{
	    send_to_char( argument, ch );
  	    send_to_char( "\n\r", ch );
	    trig = do_trigger( ch, ch, NULL, argument, TRIG_TROOM, TRIG_TYPE_MOB);
	    return;
	} 

}

void room_echo( ROOM_INDEX_DATA * pRoom, char * argument )
{
    CHAR_DATA * ch;
    for ( ch = pRoom->people ; ch != NULL ; ch = ch->next_in_room )
    {
	send_to_char( argument, ch );
	send_to_char( "\n\r", ch );
    }
}

void room_echo_2( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * ch;
    argument = one_argument( argument, arg );

    for ( ch = pRoom->people ; ch != NULL ; ch = ch->next_in_room )
    {
    	if ( is_name( arg, ch->name ) )
	    continue;

	send_to_char( argument, ch );
	send_to_char( "\n\r", ch );
    }
}

void room_refresh( ROOM_INDEX_DATA * pRoom, char * argument )
{
    OBJ_DATA  *obj_next;
    OBJ_DATA  *obj;

	for ( obj = pRoom->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))
	      extract_obj( obj );
	}
    reset_area(pRoom->area);
}

void room_unlock( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * to_room;
    EXIT_DATA * pexit;
    LOCK_DATA * lock;
    int door;
    long vnum = 0;

    argument = one_argument( argument, arg );

    for( door = 0 ; door < 6 && str_prefix( arg, dir_name[ door ] ) ; door++ );

    if ( door == 6
      || ( pexit = pRoom->exit[ door ] ) == NULL
      || ( to_room = pexit->u1.to_room ) == NULL )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' && is_number( arg ) )
	vnum = atoi( arg );

    for ( lock = pexit->lock ; lock != NULL ; lock = lock->next )
	if ( IS_SET( lock->lock_info, LOCK_LOCKED )
	  && ( vnum == 0 || lock->vnum == vnum ) )
	{
	    REMOVE_BIT( lock->lock_info, LOCK_LOCKED );
	    send_to_room( pRoom->vnum, "*Click*" );
	}

    if ( ( pexit = to_room->exit[ rev_dir[ door ] ] ) == NULL
      || pexit->u1.to_room != pRoom )
	return;

    for ( lock = pexit->lock ; lock != NULL ; lock = lock->next )
	if ( IS_SET( lock->lock_info, LOCK_LOCKED )
	  && ( vnum == 0 || lock->vnum == vnum ) )
	{
	    REMOVE_BIT( lock->lock_info, LOCK_LOCKED );
	    send_to_room( to_room->vnum, "*Click*" );
	}

}

void room_lock( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * to_room;
    EXIT_DATA * pexit;
    LOCK_DATA * lock;
    int door;
    long vnum = 0;

    argument = one_argument( argument, arg );

    for( door = 0 ; door < 6 && str_prefix( arg, dir_name[ door ] ) ; door++ );

    if ( door == 6
      || ( pexit = pRoom->exit[ door ] ) == NULL
      || ( to_room = pexit->u1.to_room ) == NULL )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' && is_number( arg ) )
	vnum = atoi( arg );

    for ( lock = pexit->lock ; lock != NULL ; lock = lock->next )
	if ( !IS_SET( lock->lock_info, LOCK_LOCKED )
	  && ( vnum == 0 || lock->vnum == vnum ) )
	{
	    SET_BIT( lock->lock_info, LOCK_LOCKED );
	    send_to_room( pRoom->vnum, "*Click*" );
	}

    if ( ( pexit = to_room->exit[ rev_dir[ door ] ] ) == NULL
      || pexit->u1.to_room != pRoom )
	return;

    for ( lock = pexit->lock ; lock != NULL ; lock = lock->next )
	if ( !IS_SET( lock->lock_info, LOCK_LOCKED )
	  && ( vnum == 0 || lock->vnum == vnum ) )
	{
	    SET_BIT( lock->lock_info, LOCK_LOCKED );
	    send_to_room( to_room->vnum, "*Click*" );
	}

}

void room_open( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * to_room;
    EXIT_DATA * pexit;
    int door;

    one_argument( argument, arg );

    for( door = 0 ; door < 6 && str_prefix( arg, dir_name[ door ] ) ; door++ );

    if ( door == 6
      || ( pexit = pRoom->exit[ door ] ) == NULL
      || ( to_room = pexit->u1.to_room ) == NULL )
	return;

    if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
    {
	sprintf( arg, "The %s opens.\n\r", pexit->keyword );
	send_to_room( pRoom->vnum, arg );
	REMOVE_BIT( pexit->exit_info, EX_CLOSED );
    }

    if ( ( pexit = to_room->exit[ rev_dir[ door ] ] ) == NULL
      || pexit->u1.to_room != pRoom )
	return;

    if ( IS_SET( pexit->exit_info, EX_CLOSED ) )
    {
	sprintf( arg, "The %s opens.\n\r", pexit->keyword );
	send_to_room( to_room->vnum, arg );
	REMOVE_BIT( pexit->exit_info, EX_CLOSED );
    }

}

void room_close( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * to_room;
    EXIT_DATA * pexit;
    int door;

    one_argument( argument, arg );

    for( door = 0 ; door < 6 && str_prefix( arg, dir_name[ door ] ) ; door++ );

    if ( door == 6
      || ( pexit = pRoom->exit[ door ] ) == NULL
      || ( to_room = pexit->u1.to_room ) == NULL )
	return;

    if ( !IS_SET( pexit->exit_info, EX_CLOSED ) )
    {
	sprintf( arg, "The %s closes.\n\r", pexit->keyword );
	send_to_room( pRoom->vnum, arg );
	SET_BIT( pexit->exit_info, EX_CLOSED );
    }

    if ( ( pexit = to_room->exit[ rev_dir[ door ] ] ) == NULL
      || pexit->u1.to_room != pRoom )
	return;

    if ( !IS_SET( pexit->exit_info, EX_CLOSED ) )
    {
	sprintf( arg, "The %s closes.\n\r", pexit->keyword );
	send_to_room( to_room->vnum, arg );
	SET_BIT( pexit->exit_info, EX_CLOSED );
    }

}

void room_move( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * ch = NULL;
    CHAR_DATA * next;
    int door;
    bool fAll = FALSE;
    bool fAll_char = FALSE;

    argument = one_argument( argument, arg );

    if ( strncmp( arg, "all_char", 3 ) )
    {
	fAll = FALSE;
        for ( ch = pRoom->people ; ch != NULL && !is_name( arg, ch->name ) ; ch = ch->next_in_room );

        if ( ch == NULL )
	    return;
    }
    else
	fAll = TRUE; 

    if ( !str_cmp( arg, "all_char" ) )
	fAll_char = TRUE;   

    one_argument( argument, arg );

    for ( door = 0 ; door < 6 && str_prefix( arg, dir_name[ door ] ) ; door++ );

    if ( door == 6 )
	return;

    if ( fAll )
    {
	for ( ch = pRoom->people ; ch != NULL ; ch = next )
	{
	    next = ch->next_in_room;
	    if ( ( !IS_NPC( ch ) && fAll_char ) || fAll )
	    {
            	SET_BIT( ch->plr2, PLR_SPECIAL_PASS );
            	move_char( ch, door, FALSE );
            	REMOVE_BIT( ch->plr2, PLR_SPECIAL_PASS );
	    }
	}
    }
    else
    {
        SET_BIT( ch->plr2, PLR_SPECIAL_PASS );
        move_char( ch, door, FALSE );
        REMOVE_BIT( ch->plr2, PLR_SPECIAL_PASS );
    }
}

void room_move_2( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * ch = NULL;
    CHAR_DATA * next;
    int door;
    bool fAll = FALSE;
    bool fAll_char = FALSE;

    argument = one_argument( argument, arg );

    if ( strncmp( arg, "all_char", 3 ) )
    {
	fAll = FALSE;
        for ( ch = pRoom->people ; ch != NULL && !is_name( arg, ch->name ) ; ch = ch->next_in_room );
        if ( ch == NULL )
	    return;
    }
    else
	fAll = TRUE;   
    if (!str_cmp( arg, "all_char" ) )
	fAll_char = TRUE;   

    one_argument( argument, arg );

    for( door = 0 ; door < 6 && str_prefix( arg, dir_name[ door ] ) ; door++ );

    if ( door == 6 )
	return;


    if ( fAll )
    {
	for ( ch = pRoom->people ; ch != NULL ; ch = next )
	{
	    next = ch->next_in_room;
	    if ( (!IS_NPC(ch) && fAll_char && !IS_AFFECTED(ch, AFF_FLYING|AFF_LEVITATE) ) || (fAll && !IS_AFFECTED(ch, AFF_FLYING|AFF_LEVITATE) ) )
	    {
            SET_BIT( ch->plr2, PLR_SPECIAL_PASS );
            move_char( ch, door, FALSE );
            REMOVE_BIT( ch->plr2, PLR_SPECIAL_PASS );
	    }
	}
    }
    else if ( !IS_AFFECTED(ch, AFF_FLYING|AFF_LEVITATE) )
    {
        SET_BIT( ch->plr2, PLR_SPECIAL_PASS );
        move_char( ch, door, FALSE );
        REMOVE_BIT( ch->plr2, PLR_SPECIAL_PASS );
    }
}

const	struct	room_cmd_type	room_cmd_table	[] = 
{
	{ "at",		room_at		},
	{ "change",	room_change	},
	{ "close",	room_close	},
	{ "echo",	room_echo	},
	{ "echo2",	room_echo_2	},
	{ "lock",	room_lock	},
	{ "move",	room_move	},
	{ "move_2",	room_move_2	},
	{ "open",	room_open	},
	{ "rand_stat",	room_rand_stat	},
	{ "rset",	room_rset	},
	{ "techo",	room_techo	},
	{ "teleport",	room_teleport	},
	{ "unlock",	room_unlock	},
	{ "refresh",	room_refresh	},
	{ "", 		NULL } 
};    

void room_interpret( ROOM_INDEX_DATA * pRoom, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    int cmd;

    argument = one_argument( argument, arg );

    if ( arg[ 0 ] == '\0' )
	return;

    for( cmd = 0 ; room_cmd_table[ cmd ].name[ 0 ] != '\0'; cmd++ )
    {
	if ( !str_prefix( arg, room_cmd_table[ cmd ].name ) )
	{
	    (*room_cmd_table[ cmd ].room_fun)( pRoom, argument );
	    return;
	}
    }
}
