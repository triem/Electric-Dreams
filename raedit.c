/***************************************************************************
*                                                                          *
*  Race Edit code By Quin                                                  *
*         Special thanks to Locke of the Isles                             *
*                                                                          *
*                                                                          *
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

DECLARE_SPELL_FUN( spell_null );
char * res_bit_name( long bit );
char * vuln_bit_name( long bit );

char * sex_name[4] =
{
	"Neuter",
	"Male",
	"Female",
	"All"
};

void do_raedit( CHAR_DATA *ch , char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    int race;
    if(IS_NPC(ch) ) return;

    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Which Race do you wish to edit?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "add" ) || !str_cmp( arg, "create" ) )
    {
	argument = one_argument( argument, arg );
	if ( arg[0] == '\0' )
	{
	    send_to_char( "You must suply a name for the new race\n\r", ch );
	    return;
	}
	else if ( top_race == MAX_RACE )
	{
	    send_to_char( "No room for new race.\n\r", ch );
	    return;
	}
	else
	{
	    race = top_race++;
	    default_race( race );
	    race_list[ race ].race_flags = RACE_CHANGED;
	    strncpy( race_list[ race ].name, arg, 15 );
	    sprintf( buf, "%s.rce", arg );
	    strncpy( race_list[ race ].filename, buf, 20 );
	    race_list[ race ].security = UMAX(1, ch->pcdata->security );
	    strncpy( race_list[ race ].builders, ch->name, 30 );
	    if ( ch->desc->editing == NULL )
		ch->desc->editing = editing_data_alloc();
	    ch->desc->editing->vnum = race ;
	    ch->desc->connected = CON_RAEDITOR;
	    return;
    	}
    }

    if ( ( race = race_lookup( arg ) ) == -1 )
    {
	if ( !is_number( arg ) || ( race = atoi( arg ) ) < 0 || race >= top_race )
	{
	    send_to_char(  "That is not a valid race.\n\r", ch );
	    return;
	}
    }

    if ( ch->desc->editing == NULL )
	ch->desc->editing = editing_data_alloc();
    ch->desc->editing->vnum = race ;
    ch->desc->connected = CON_RAEDITOR;

}   

void do_rashow( CHAR_DATA *ch, char *argument, int race , int sex )
{
    char buf1[MAX_SKILL*80];
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int i,age,sn;

    argument = one_argument( argument, arg );

    if ( !str_prefix( arg, "Base" ) )
    {
	sprintf( buf, "Name: %s  World: %s  Who Name: %s  Language: %s Gold: %d\n\r",
	    race_table[ race ][ sex ].name,
	    world_name[ race_table[ race ][ sex ].world ],
	    race_table[ race ][ sex ].who_name,
	    skill_table[ race_table[ race ][ sex ].language ].name, 
	    race_table[ race ][ sex ].gold);
	send_to_char( buf, ch );
	sprintf( buf, "Starting -  Str: %2d  Int: %2d  Wis: %2d  Dex: %2d  Con: %2d Chr: %2d\n\r",
	    race_table[ race ][ sex ].stats[ 0 ],
	    race_table[ race ][ sex ].stats[ 1 ],
	    race_table[ race ][ sex ].stats[ 2 ],
	    race_table[ race ][ sex ].stats[ 3 ],
	    race_table[ race ][ sex ].stats[ 4 ],
	    race_table[ race ][ sex ].stats[ 5 ]);
	send_to_char( buf, ch );
	sprintf( buf, "Max -       Str: %2d  Int: %2d  Wis: %2d  Dex: %2d  Con: %2d Chr: %2d\n\r",
	    race_table[ race ][ sex ].max_stats[ 0 ],
	    race_table[ race ][ sex ].max_stats[ 1 ],
	    race_table[ race ][ sex ].max_stats[ 2 ],
	    race_table[ race ][ sex ].max_stats[ 3 ],
	    race_table[ race ][ sex ].max_stats[ 4 ],
	    race_table[ race ][ sex ].max_stats[ 5 ]);
	send_to_char( buf, ch );
	sprintf( buf, "%s-  Size: %2d  Damage: %s  Scan dist: %2d\n\r",
	    (race_table[ race ][ sex ].pc_race?"Pc":"Npc"),
	    race_table[ race ][ sex ].size,
	    get_type_weapon(race_table[ race ][ sex ].dam_type),
	    race_table[ race ][ sex ].scan_dist);
	send_to_char( buf, ch );
	sprintf( buf, "Hp multiplier (npc): %d  Damage per level: %dd%d + %d\n\r",
	    race_table[ race ][ sex ].hp_mult,
	    race_table[ race ][ sex ].dam_dice,
	    race_table[ race ][ sex ].dam_die,
	    race_table[ race ][ sex ].dam_bonus);
	send_to_char( buf, ch );

	strcpy( buf1, "Mana:" );
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	{
	    sprintf( buf, "  %s %dd%d + %d",
		element_type_name( i ),
		race_table[ race ][ sex ].mana_dice[ i ],
		race_table[ race ][ sex ].mana_die[ i ],
		race_table[ race ][ sex ].mana_bonus[ i ] );
	    strcat( buf1, buf );
	}
	strcat( buf1, "\n\r" );
	send_to_char( buf1, ch );

	sprintf( buf, "Ac per level (npc only):\n\r  Pierce - %dd%d + %d   Bash - %dd%d + %d   Slash - %dd%d + %d   Magic - %dd%d + %d\n\r",
	    race_table[ race ][ sex ].ac_dice[0],
	    race_table[ race ][ sex ].ac_die[0],
	    race_table[ race ][ sex ].ac_bonus[0],
	    race_table[ race ][ sex ].ac_dice[1],
	    race_table[ race ][ sex ].ac_die[1],
	    race_table[ race ][ sex ].ac_bonus[1],
	    race_table[ race ][ sex ].ac_dice[2],
	    race_table[ race ][ sex ].ac_die[2],
	    race_table[ race ][ sex ].ac_bonus[2],
	    race_table[ race ][ sex ].ac_dice[3],
	    race_table[ race ][ sex ].ac_die[3],
	    race_table[ race ][ sex ].ac_bonus[3]);
	send_to_char( buf, ch );
	sprintf( buf, "Weapon: %ld  Map: %ld  Starting Age: %3d  Aging Rate: %2d  \n\rThac0_0: %2d    Thac0_32: %2d\n\r",
	    race_table[ race ][ sex ].weapon,
	    race_table[ race ][ sex ].map,
	    race_table[ race ][ sex ].start_age,
	    race_table[ race ][ sex ].year_per_hour,
	    race_table[ race ][ sex ].thac0_00,
	    race_table[ race ][ sex ].thac0_32);
	send_to_char( buf, ch );
	sprintf( buf, "Drunk:  (%d)  Hunger: (%d)  Thirst: (%d)\n\r",
	    race_table[ race ][ sex ].max_condition[ 0 ],
	    race_table[ race ][ sex ].max_condition[ 1 ],
	    race_table[ race ][ sex ].max_condition[ 2 ]);
	send_to_char( buf, ch );
	sprintf( buf, "Begin Room: %ld  Die Room: %ld  Donation Room: %ld\n\r",
	    race_table[ race ][ sex ].begin,
	    race_table[ race ][ sex ].die,
	    race_table[ race ][ sex ].donate);
	send_to_char( buf, ch );
	sprintf( buf, "Act: %s\n\r", act_bit_name(race_table[ race ][ sex ].act) );
	send_to_char( buf, ch );
	sprintf( buf, "Aff: %s\n\r", affect_bit_name(race_table[ race ][ sex ].aff) );
	send_to_char( buf, ch );
	sprintf( buf, "Aff2: %s\n\r", affect_bit_name_2(race_table[ race ][ sex ].aff2) );
	send_to_char( buf, ch );
	sprintf( buf, "Off: %s\n\r", off_bit_name(race_table[ race ][ sex ].off) );
	send_to_char( buf, ch );
	sprintf( buf, "Imm: %s\n\r", imm_bit_name(race_table[ race ][ sex ].imm) );
	send_to_char( buf, ch );
	sprintf( buf, "Res: %s\n\r", res_bit_name(race_table[ race ][ sex ].res) );
	send_to_char( buf, ch );
	sprintf( buf, "Vuln: %s\n\r", vuln_bit_name(race_table[ race ][ sex ].vuln) );
	send_to_char( buf, ch );
	sprintf( buf, "Form: %s\n\r", form_bit_name(race_table[ race ][ sex ].form) );
	send_to_char( buf, ch );
	return;
    }
    if ( !str_prefix( arg, "Age" ) || !str_prefix( arg, "Max" ) )
    {
	sprintf( buf1, " Age  %10s %10s %10s %10s %10s %10s\n\r\n\r",
		"Str","Int","Wis","Dex","Con","Chr");
	for ( age = 0 ; age < 40 ; age++ )
	{
	    sprintf( buf, " %4d %10d %10d %10d %10d %10d %10d\n\r", age*5,
		race_table[ race ][ sex ].age_stats[ age ][ 0 ],
		race_table[ race ][ sex ].age_stats[ age ][ 1 ],
		race_table[ race ][ sex ].age_stats[ age ][ 2 ],
		race_table[ race ][ sex ].age_stats[ age ][ 3 ],
		race_table[ race ][ sex ].age_stats[ age ][ 4 ],
		race_table[ race ][ sex ].age_stats[ age ][ 5 ] );
	    strcat( buf1, buf );
	}	
	page_to_char( buf1, ch );
	return;
    }
    if ( !str_prefix( arg, "Skills" ) )
    {
	sprintf( buf1, " %-18s %5s %5s %5s %5s\n\r\n\r","Skill","Level","Cost","Adept","Start");
	for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
	{
	    if ( skill_table[ sn ].spell_fun != spell_null )
		continue;

	    if ( race_table[ race ][ sex ].skill_level[ sn ] >= LEVEL_IMMORTAL ||
		 race_table[ race ][ sex ].rating[ sn ] < 1 )
		continue;

	    sprintf( buf," %-18s %5d %5d %5d %5d %5d\n\r", skill_table[ sn ].name,
		 race_table[ race ][ sex ].skill_level[ sn ],
		 race_table[ race ][ sex ].rating[ sn ],
		 race_table[ race ][ sex ].adept[ sn ],
		 race_table[ race ][ sex ].starting[ sn ],
		 race_table[ race ][ sex ].gained[ sn ]);
	     strcat( buf1, buf );
	}
	page_to_char( buf1, ch );
	return;
    }
    if ( !str_prefix( arg, "Spells" ) )
    {
	sprintf( buf1, " %-18s %5s %5s %5s %5s\n\r\n\r","Spell","Level","Cost","Adept","Start" );
	for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
	{
	    if ( skill_table[ sn ].spell_fun == spell_null )
		continue;

	    if ( race_table[ race ][ sex ].skill_level[ sn ] >= LEVEL_IMMORTAL ||
		 race_table[ race ][ sex ].rating[ sn ] < 1 )
		continue;

	    sprintf( buf," %-18s %5d %5d %5d %5d %5d\n\r", skill_table[ sn ].name,
		 race_table[ race ][ sex ].skill_level[ sn ],
		 race_table[ race ][ sex ].rating[ sn ],
		 race_table[ race ][ sex ].adept[ sn ],
		 race_table[ race ][ sex ].starting[ sn ],
		 race_table[ race ][ sex ].gained[ sn ]);
	     strcat( buf1, buf );
	}
	page_to_char( buf1, ch );
	return;
    }
    send_to_char( "Syntax: show <Base/Max/Skills> <sex> \n\r", ch );
}

int get_stat( char * argument )
{
    if ( !str_prefix( argument, "all") ) 
	return MAX_STATS;
    if ( !str_prefix( argument, "strength") ) 
	return STAT_STR;
    if ( !str_prefix( argument, "intelligence") ) 
	return STAT_INT;
    if ( !str_prefix( argument, "wisdom") ) 
	return STAT_WIS;
    if ( !str_prefix( argument, "dexterity") ) 
	return STAT_DEX;
    if ( !str_prefix( argument, "constitution") ) 
	return STAT_CON;
    if ( !str_prefix( argument, "charisma") ) 
	return STAT_CHR;

    return -1;
}

int get_sex( char * argument )
{
    if ( !str_prefix( argument, "all" ) )
	return 4;
    else if ( !str_prefix( argument, "male" ) )
	return 2;
    else if ( !str_prefix( argument, "female" ) )
	return 3;
    else if ( !str_prefix( argument, "neuter" ) )
	return 1;
    else
        return -1;
}

int get_world( char * argument )
{
    int world;

    if ( is_number( argument ) && ( world = atoi(argument) ) > 0 && world < MAX_WORLD )
	return world;

    for ( world = 0 ; world < MAX_WORLD ; world++ )
	if ( !str_prefix( argument, world_name[ world ] ) )
	    return world;

    return -1;
}

/* raeditor starts hereh */
void raedit( CHAR_DATA *ch, char * argument )
{
    OBJ_INDEX_DATA * obj;
    char * arguments;
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    char arg4[MAX_INPUT_LENGTH];
    char arg5[MAX_INPUT_LENGTH];
    char arg6[MAX_INPUT_LENGTH];
    char arg7[MAX_INPUT_LENGTH];
    int race,sex,i,c;
    int value1,value2,value3,value4,value5;
    int type;

    race = ch->desc->editing->vnum;

    arguments = argument;
    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	/* online help */
	send_to_char( "show - shows current racial values\n\r", ch );
	send_to_char( "done - exits the editor\n\r", ch );
	send_to_char( "save - saves current race\n\r", ch );
	send_to_char( "name - changes the racial name, or the sex name\n\r", ch );
	send_to_char( "world - sets the world the race is from\n\r", ch );
	send_to_char( "size - sets how large a person from that race would be\n\r", ch );
	send_to_char( "str/int/wis/dex/con/cha - sets the starting and max attribute\n\r", ch );
	send_to_char( "drunk/hunger/thirst - sets the max of each state, and the rate\n\r", ch );
	send_to_char( "hp - sets the amount of hp for each level\n\r", ch );
	send_to_char( "dam - sets the amount of damage an npc of that race does for each level\n\r", ch );
	send_to_char( "bash/slash/pierce/magic - sets the AC for each level\n\r", ch );
	send_to_char( "skills - sets the starting skills %s\n\r", ch );
	send_to_char( "group - sets the starting group\n\r", ch );
	send_to_char( "thac0 - sets the chance to hit AC 0\n\r", ch );
	send_to_char( "begin/die/donate - sets the room for which each of these are done\n\r", ch );
	send_to_char( "weapon/map - sets the starting weapon or map\n\r", ch );
	send_to_char( "points - sets the starting creation points\n\r", ch );
	send_to_char( "multiplier - sets the experience multiplier\n\r", ch );
	send_to_char( "age - sets the starting age, and the aging rate\n\r", ch );
	send_to_char( "gold - sets the starting gold per level (average )\n\r", ch );
	send_to_char( "scan - sets the maximum scan distance\n\r", ch );
	send_to_char( "dam_type - sets the damage type without a weapon\n\r", ch );
	send_to_char( "builders - adds/removes a builder\n\r", ch );
	send_to_char( "filename - changes the race file name\n\r", ch );
	send_to_char( "security - sets the security level\n\r", ch );
	send_to_char( "npc/pc - sets if the race is available to players or not\n\r", ch );
	send_to_char( "Also:\n\r\n\r     aff_bit, off_bit, form, imm_bit res_bit, vuln_bit, act_bit\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "show" ) )
    {
        argument = one_argument( argument, arg1 );
        argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: show <Base/Max/Skills/Spells> [sex]\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( i = 1 ; i < 4 ; i++ )
	    {
		sprintf( buf, "\n\r%s - %s:\n\r", race_list[ race ].name,
			sex_name[ i ]);
		send_to_char( buf, ch );
		do_rashow( ch, arg1, race, i );
	    }
	}
	else
  	    do_rashow( ch, arg1, race, sex );
	return;
    }

    if ( !str_cmp( arg, "done" ) )
    {
	if ( IS_SET( race_list[ race ].race_flags, RACE_CHANGED ) )
	{
	    send_to_char( "The race has been changed, don't forget to save it.\n\r", ch );
	}
	ch->desc->connected = CON_PLAYING;
	return;
    }

    if ( ch->pcdata->security > race_list[ race ].security &&
	!is_name( ch->name, race_list[ race ].builders ) ) 
    {
        interpret( ch, arguments );
	return;
    }

    if ( !str_cmp( arg, "save" ) )
    {
	save_rlist();
	save_race( race );
	send_to_char( "Race saved.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "world" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax:  <world> [sex] \n\r", ch );
	    send_to_char( "Values:  Lithdor Maegmenel Nenkemen Dark\n\r", ch );
	    return;
	}
	else if ( ( i = get_world( arg1 ) ) == -1 )
	{
	    send_to_char( "Invalid world.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].world = i;
	    }
	    send_to_char( "World set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].world = i;
	    send_to_char( "World set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "size" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 || !is_number( arg1 ) )
	{
	    send_to_char( "Syntax: size <size> [sex]\n\r", ch );
	    send_to_char( "Values: 0-5\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 1 || i > 5 )
	{
	    send_to_char( "Invalid size.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].size = i;
	    }
	    send_to_char( "Size set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].size = i;
	    send_to_char( "Size set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "age" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( ( sex = get_sex( arg3 ) ) == -1 || !is_number( arg1 ) 
	   || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: age <starting age> <aging rate> [sex]\n\r", ch );
	    send_to_char( "Values: starting age (0-100 years)  aging rate (1-10 hours per year)\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 0 || i > 100 ||
	          ( c = atoi( arg2 ) ) < 1 || c > 10 )
	{
	    send_to_char( "Invalid age value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].start_age = i;
		race_table[ race ][ sex ].year_per_hour = c;
	    }
	    send_to_char( "Age values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].start_age = i;
	    race_table[ race ][ sex ].year_per_hour = c;
	    send_to_char( "Age values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "drunk" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( ( sex = get_sex( arg2 ) ) == -1 || !is_number( arg1 ) ) 
	{
	    send_to_char( "Syntax: drunk <drunk rate> <max drunk> [sex]\n\r", ch );
	    send_to_char( "  drunk rate - the rate at which the drunk state is lost.\n\r", ch );
	    send_to_char( "  max drunk - the maximum amount of alchohol that can be drunk.\n\r", ch );
	    return;
	}
	else if ( ( c = atoi( arg1 ) ) < 10 || c > 200 )
	{
	    send_to_char( "Invalid drunk value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
		race_table[ race ][ sex ].max_condition[0] = c;

	    send_to_char( "Drunk values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].max_condition[0] = c;
	    send_to_char( "Drunk values set.\n\r", ch );
	}

	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "hunger" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( ( sex = get_sex( arg2 ) ) == -1 || !is_number( arg1 ) )
	{
	    send_to_char( "Syntax: hunger <hunger rate> <max food> [sex]\n\r", ch );
	    send_to_char( "  hunger rate - the rate at which a person get hungry.\n\r", ch );
	    send_to_char( "  max food - the maximum amount of food that can be eaten.\n\r", ch );
	    return;
	}
 	else if ( ( c = atoi( arg2 ) ) < 10 || c > 200 )
	{
	    send_to_char( "Invalid hunger value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
		race_table[ race ][ sex ].max_condition[1] = c;

	    send_to_char( "Hunger values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].max_condition[1] = c;
	    send_to_char( "Hunger values set.\n\r", ch );
	}

	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "thirst" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( ( sex = get_sex( arg2 ) ) == -1 || !is_number( arg1 ) )
	{
	    send_to_char( "Syntax: thirst <thirst rate> <max drink> [sex]\n\r", ch );
	    send_to_char( "  thirst rate - the rate at which a person gets thirsty.\n\r", ch );
	    send_to_char( "  max drink - the maximum amount of drink that can be drunk.\n\r", ch );
	    return;
	}
	else if ( ( c = atoi( arg1 ) ) < 10 || c > 200 )
	{
	    send_to_char( "Invalid thirst value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
		race_table[ race ][ sex ].max_condition[2] = c;

	    send_to_char( "Thirst values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].max_condition[2] = c;
	    send_to_char( "Thirst values set.\n\r", ch );
	}

	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "strength" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( ( sex = get_sex( arg3 ) ) == -1 || !is_number( arg1 ) 
	   || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: strength <starting strength> <max trainable strength> [sex]\n\r", ch );
	    send_to_char( "Values: 3-25\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 3 || i > 25 ||
	          ( c = atoi( arg2 ) ) < 3 || c > 25 )
	{
	    send_to_char( "Invalid strength value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].stats[STAT_STR] = i;
		race_table[ race ][ sex ].max_stats[STAT_STR] = c;
	    }
	    send_to_char( "Strength values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].stats[STAT_STR] = i;
	    race_table[ race ][ sex ].max_stats[STAT_STR] = c;
	    send_to_char( "Strength values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "intelligence" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( ( sex = get_sex( arg3 ) ) == -1 || !is_number( arg1 ) 
	   || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: intelligence <starting intelligence> <max trainable intelligence> [sex]\n\r", ch );
	    send_to_char( "Values: 3-25\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 3 || i > 25 ||
	          ( c = atoi( arg2 ) ) < 3 || c > 25 )
	{
	    send_to_char( "Invalid intelligence value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].stats[STAT_INT] = i;
		race_table[ race ][ sex ].max_stats[STAT_INT] = c;
	    }
	    send_to_char( "Intelligence values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].stats[STAT_INT] = i;
	    race_table[ race ][ sex ].max_stats[STAT_INT] = c;
	    send_to_char( "Intelligence values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "wisdom" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( ( sex = get_sex( arg3 ) ) == -1 || !is_number( arg1 ) 
	   || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: wisdom <starting wisdom> <max trainable wisdom> [sex]\n\r", ch );
	    send_to_char( "Values: 3-25\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 3 || i > 25 ||
	          ( c = atoi( arg2 ) ) < 3 || c > 25 )
	{
	    send_to_char( "Invalid wisdom value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].stats[STAT_WIS] = i;
		race_table[ race ][ sex ].max_stats[STAT_WIS] = c;
	    }
	    send_to_char( "Wisdom values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].stats[STAT_WIS] = i;
	    race_table[ race ][ sex ].max_stats[STAT_WIS] = c;
	    send_to_char( "Wisdom values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "dexterity" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( ( sex = get_sex( arg3 ) ) == -1 || !is_number( arg1 ) 
	   || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: dexterity <starting dexterity> <max trainable dexterity> [sex]\n\r", ch );
	    send_to_char( "Values: 3-25\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 3 || i > 25 ||
	          ( c = atoi( arg2 ) ) < 3 || c > 25 )
	{
	    send_to_char( "Invalid dexterity value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].stats[STAT_DEX] = i;
		race_table[ race ][ sex ].max_stats[STAT_DEX] = c;
	    }
	    send_to_char( "Dexterity values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].stats[STAT_DEX] = i;
	    race_table[ race ][ sex ].max_stats[STAT_DEX] = c;
	    send_to_char( "Dexterity values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "constitution" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( ( sex = get_sex( arg3 ) ) == -1 || !is_number( arg1 ) 
	   || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: constitution <starting constitution> <max trainable constitution> [sex]\n\r", ch );
	    send_to_char( "Values: 3-25\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 3 || i > 25 ||
	          ( c = atoi( arg2 ) ) < 3 || c > 25 )
	{
	    send_to_char( "Invalid constitution value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].stats[STAT_CON] = i;
		race_table[ race ][ sex ].max_stats[STAT_CON] = c;
	    }
	    send_to_char( "Constitution values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].stats[STAT_CON] = i;
	    race_table[ race ][ sex ].max_stats[STAT_CON] = c;
	    send_to_char( "Constitution values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "charisma" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( ( sex = get_sex( arg3 ) ) == -1 || !is_number( arg1 ) 
	   || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: charisma <starting charisma> <max trainable charisma> [sex]\n\r", ch );
	    send_to_char( "Values: 3-25\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 3 || i > 25 ||
	          ( c = atoi( arg2 ) ) < 3 || c > 25 )
	{
	    send_to_char( "Invalid charisma value.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].stats[STAT_CHR] = i;
		race_table[ race ][ sex ].max_stats[STAT_CHR] = c;
	    }
	    send_to_char( "Charisma values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].stats[STAT_CHR] = i;
	    race_table[ race ][ sex ].max_stats[STAT_CHR] = c;
	    send_to_char( "Charisma values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "thac0" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg3 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: thac0 <at level 0> <at level 32> [sex]\n\r", ch );
	    send_to_char( "Values: 20-0\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 0 || i > 20 
	   || ( c = atoi( arg2 ) ) < 0 || c > 20 || i < c )
	{
	    send_to_char( "Invalid Thac0 values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].thac0_00 = i;
		race_table[ race ][ sex ].thac0_32 = c;
	    }
	    send_to_char( "Thac0 values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].thac0_00 = i;
	    race_table[ race ][ sex ].thac0_32 = c;
	    send_to_char( "Thac0 values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }
/*
    if ( !str_cmp( arg, "hp" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg4 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Syntax: hp <number of dice> <die number> <bonus> [sex]\n\r", ch );
	    send_to_char( "Example: hp 2 4 1 male - is 2d4+1 hp per level\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 1 
		||( value2 = atoi( arg2 ) ) < 1
		||( value3 = atoi( arg3 ) ) < 0 )
	{
	    send_to_char( "Invalid hp values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 sex++ )
	    {
		race_table[ race ][ sex ].hp_dice = value1;
		race_table[ race ][ sex ].hp_die = value2;
		race_table[ race ][ sex ].hp_bonus = value3;
	    }
	    send_to_char( "hp values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].hp_dice = value1;
	    race_table[ race ][ sex ].hp_die = value2;
	    race_table[ race ][ sex ].hp_bonus = value3;
	    send_to_char( "hp values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }
*/

    if ( ( type = get_element_type( arg ) ) != -1 )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg4 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Syntax: mana_type <number of dice> <die number> <bonus> [sex]\n\r", ch );
	    send_to_char( "Example: fire 2 4 1 male - is 2d4+1  fire mana per level\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 1 
		||( value2 = atoi( arg2 ) ) < 1
		||( value3 = atoi( arg3 ) ) < 0 )
	{
	    send_to_char( "Invalid mana values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].mana_dice[ type ] = value1;
		race_table[ race ][ sex ].mana_die[ type ] = value2;
		race_table[ race ][ sex ].mana_bonus[ type ] = value3;
	    }
	    send_to_char( "mana values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].mana_dice[ type ] = value1;
	    race_table[ race ][ sex ].mana_die[ type ] = value2;
	    race_table[ race ][ sex ].mana_bonus[ type ] = value3;
	    send_to_char( "mana values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "bash" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg4 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Syntax: bash <number of dice> <die number> <bonus> [sex]\n\r", ch );
	    send_to_char( "Example: ac 2 4 1 male - is 2d4+1 ac per level verses bash(npc only)\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 1 
		||( value2 = atoi( arg2 ) ) < 1
		||( value3 = atoi( arg3 ) ) < 0 )
	{
	    send_to_char( "Invalid ac values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].ac_dice[1] = value1;
		race_table[ race ][ sex ].ac_die[1] = value2;
		race_table[ race ][ sex ].ac_bonus[1] = value3;
	    }
	    send_to_char( "Ac values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].ac_dice[1] = value1;
	    race_table[ race ][ sex ].ac_die[1] = value2;
	    race_table[ race ][ sex ].ac_bonus[1] = value3;
	    send_to_char( "Ac values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "pierce" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg4 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Syntax: pierce <number of dice> <die number> <bonus> [sex]\n\r", ch );
	    send_to_char( "Example: ac 2 4 1 male - is 2d4+1 ac per level verses pierce(npc only)\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 1 
		||( value2 = atoi( arg2 ) ) < 1
		||( value3 = atoi( arg3 ) ) < 0 )
	{
	    send_to_char( "Invalid ac values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].ac_dice[0] = value1;
		race_table[ race ][ sex ].ac_die[0] = value2;
		race_table[ race ][ sex ].ac_bonus[0] = value3;
	    }
	    send_to_char( "Ac values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].ac_dice[0] = value1;
	    race_table[ race ][ sex ].ac_die[0] = value2;
	    race_table[ race ][ sex ].ac_bonus[0] = value3;
	    send_to_char( "Ac values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "slash" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg4 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Syntax: slash <number of dice> <die number> <bonus> [sex]\n\r", ch );
	    send_to_char( "Example: ac 2 4 1 male - is 2d4+1 ac per level verses slash(npc only)\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 1 
		||( value2 = atoi( arg2 ) ) < 1
		||( value3 = atoi( arg3 ) ) < 0 )
	{
	    send_to_char( "Invalid ac values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].ac_dice[2] = value1;
		race_table[ race ][ sex ].ac_die[2] = value2;
		race_table[ race ][ sex ].ac_bonus[2] = value3;
	    }
	    send_to_char( "Ac values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].ac_dice[2] = value1;
	    race_table[ race ][ sex ].ac_die[2] = value2;
	    race_table[ race ][ sex ].ac_bonus[2] = value3;
	    send_to_char( "Ac values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "magic" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg4 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Syntax: magic <number of dice> <die number> <bonus> [sex]\n\r", ch );
	    send_to_char( "Example: ac 2 4 1 male - is 2d4+1 ac per level verses magic(npc only)\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 1 
		||( value2 = atoi( arg2 ) ) < 1
		||( value3 = atoi( arg3 ) ) < 0 )
	{
	    send_to_char( "Invalid ac values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].ac_dice[3] = value1;
		race_table[ race ][ sex ].ac_die[3] = value2;
		race_table[ race ][ sex ].ac_bonus[3] = value3;
	    }
	    send_to_char( "Ac values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].ac_dice[3] = value1;
	    race_table[ race ][ sex ].ac_die[3] = value2;
	    race_table[ race ][ sex ].ac_bonus[3] = value3;
	    send_to_char( "Ac values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "dam" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg4 ) ) == -1 || 
	    !is_number( arg1 ) || !is_number( arg2 ) || !is_number( arg3 ) )
	{
	    send_to_char( "Syntax: dam <number of dice> <die number> <bonus> [sex]\n\r", ch );
	    send_to_char( "Example: dam 2 4 1 male - is 2d4+1 damage per level(npc only)\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 1 
		||( value2 = atoi( arg2 ) ) < 1
		||( value3 = atoi( arg3 ) ) < 0 )
	{
	    send_to_char( "Invalid damage values.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].dam_dice = value1;
		race_table[ race ][ sex ].dam_die = value2;
		race_table[ race ][ sex ].dam_bonus = value3;
	    }
	    send_to_char( "Damage values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].dam_dice = value1;
	    race_table[ race ][ sex ].dam_die = value2;
	    race_table[ race ][ sex ].dam_bonus = value3;
	    send_to_char( "Damage values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "scan" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 || !is_number( arg1 ) )
	{
	    send_to_char( "Syntax: scan <scan distance> [sex]\n\r", ch );
	    send_to_char( "Values: 1-3\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 1 || i > 3 )
	{
	    send_to_char( "Invalid scan distance.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].scan_dist = i;
	    }
	    send_to_char( "Scan distance set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].scan_dist = i;
	    send_to_char( "Scan distance set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "dam_type" ) || !str_cmp( arg, "dt" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: dam_type <damage type> [sex]\n\r", ch );
	    sprintf( buf, "Values: \n\r     " );
	    for ( i = 0 ; i < 34 ; i++ )
	    {
		strcat( buf, get_type_weapon( i ) );
		strcat( buf, " ");
		if ( i % 7 == 0 )
		    strcat( buf, "\n\r" );
	    }
	    strcat( buf, "\n\r" );
	    send_to_char( buf, ch );
	    return;
	}
	else if ( ( i = get_weapon_type( arg1 ) ) > 50 )
	{
	    send_to_char( "Invalid damage type.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].dam_type = i;
	    }
	    send_to_char( "Damage type set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].dam_type = i;
	    send_to_char( "Damage type set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "gold" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( !is_number( arg1 ) || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: gold <value> [sex]\n\r", ch );
	    return;
	}
	else if ( ( i = atoi( arg1 ) ) < 0 || i > 1000 )
	{
	    send_to_char( "Syntax: gold <value> [sex]\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].gold = i;
	    }
	    send_to_char( "Gold set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].gold = i;
	    send_to_char( "Gold set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "begin" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( !is_number( arg1 ) || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: begin <room vnum> [sex]\n\r", ch );
	    return;
	}
	else if ( get_room_index( i = atoi( arg1 ) ) == NULL )
	{
	    send_to_char( "Invalid room vnum.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].begin = i;
	    }
	    send_to_char( "Begining room set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].begin = i;
	    send_to_char( "Begining room set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "die" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( !is_number( arg1 ) || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: die <room vnum> [sex]\n\r", ch );
	    return;
	}
	else if ( get_room_index( i = atoi( arg1 ) ) == NULL )
	{
	    send_to_char( "Invalid room vnum.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].die = i;
	    }
	    send_to_char( "Dying room set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].die = i;
	    send_to_char( "Dying room set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "donate" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( !is_number( arg1 ) || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: donate <room vnum> [sex]\n\r", ch );
	    return;
	}
	else if ( get_room_index( i = atoi( arg1 ) ) == NULL )
	{
	    send_to_char( "Invalid room vnum.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].donate = i;
	    }
	    send_to_char( "Donation room set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].donate = i;
	    send_to_char( "Donation room set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "map" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( !is_number( arg1 ) || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: map <map vnum> [sex]\n\r", ch );
	    return;
	}
	else if ( ( obj = get_obj_index( i = atoi( arg1 ) ) ) == NULL )
	{
	    send_to_char( "Invalid starting map.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].map = i;
	    }
	    send_to_char( "Starting map set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].map = i;
	    send_to_char( "Starting map set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "weapon" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( !is_number( arg1 ) || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: weapon <weapon vnum> [sex]\n\r", ch );
	    return;
	}
	else if ( ( obj = get_obj_index( i = atoi( arg1 ) ) ) == NULL 
	    || obj->wear_data == NULL || obj->wear_data->weapon_data == NULL )
	{
	    send_to_char( "Invalid starting weapon.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].weapon = i;
	    }
	    send_to_char( "Starting weapon set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].weapon = i;
	    send_to_char( "Starting weapon set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "language" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: language <language> [sex]\n\r", ch );
	    send_to_char( "Values:\n\r\n\rbelegfea aranna mirros iavartor milrochdil tarnanthalion\n\rmanlhach nenkhilmen sulkano\n\r", ch );
	    return;
	}
	else if ( ( i = skill_lookup( arg1 ) ) < 1 || (
	    i != gsn_belegfea &&
	    i != gsn_common   &&
	    i != gsn_aranna   &&
	    i != gsn_mirros   &&
	    i != gsn_iavartor &&
	    i != gsn_milrochdil &&
	    i != gsn_tarnanthalion &&
	    i != gsn_manlhach &&
	    i != gsn_nenkhilmen &&
	    i != gsn_sulkano ) )
	{
	    send_to_char( "Invalid default language.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].language = i;
	    }
	    send_to_char( "Default language set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].language = i;
	    send_to_char( "Default language set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "max" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
	if ( !is_number( arg1 ) || !is_number( arg2 ) )
	{
	    send_to_char( "Syntax: max <age> <value> [attribute] [sex]\n\r", ch );
	    send_to_char( "Example: max 80 20 strength male - set the max strength for an 80 year old male to 20.\n\r", ch );
	    send_to_char( " This max is even with items - 'Sorry items can only help you look younger'\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg1 ) ) < 0 || value1 > 195 || value1 % 5 != 0 )
	{
	    send_to_char( "Invalid age (should be a denomination of 5).\n\r", ch );
	    return;
	}
	else if ( ( value2 = atoi( arg2 ) ) < 3 || value2 > 25 )
    	{
	    send_to_char( "Attribute value out of range.\n\r", ch );
	    return;
	}

	if ( ( i = get_stat( arg3 ) ) < 0 )
	{
	    if ( ( sex = get_sex( arg3 ) ) < 0 )
	    {
		send_to_char( "Invalid attribute or sex.\n\r", ch );
		return;
	    }
	    value1 = MAX_STATS;
	}
	else if ( ( sex = get_sex( arg4 ) ) < 0 )
	{
	    send_to_char( "Invalid sex.\n\r", ch );
	    return;
	}
	if ( sex == 4 ) 
	{
	    if ( i == MAX_STATS )
	    {
		for ( sex = 1 ; sex < 4 ; sex++ )
		    for ( i = 0 ; i < MAX_STATS ; i++ )
			race_table[ race ][ sex ].age_stats[ value1 / 5 ][ i ] = value2;	
	    }	
	    else
	    {
		for ( sex = 1 ; sex < 4 ; sex++ )
		    race_table[ race ][ sex ].age_stats[ value1 / 5 ][ i ] = value2;
	    }
	    send_to_char( "Max attribute set.\n\r", ch );
	}
	else
	{
	    if ( i == MAX_STATS )
	    {
	        for ( i = 0 ; i < MAX_STATS ; i++ )
		    race_table[ race ][ sex ].age_stats[ value1 / 5 ][ i ] = value2;	
	    }	
	    else
	    {
	        race_table[ race ][ sex ].age_stats[ value1 / 5 ][ i ] = value2;
	    }
	    send_to_char( "Max attribute set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "skill" ) || !str_cmp( arg, "spell" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
	argument = one_argument( argument, arg3 );
	argument = one_argument( argument, arg4 );
	argument = one_argument( argument, arg5 );
	argument = one_argument( argument, arg6 );
	argument = one_argument( argument, arg7 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg7 ) ) == -1 
	 || !is_number( arg2 ) || !is_number( arg3 ) || !is_number( arg4 ) || !is_number( arg5 ) || !is_number( arg6 ) )
	{
	    send_to_char( "Syntax: skill <skill name> <skill level> <skill cost> <adept %> starting %> [sex]\n\r", ch );
	    send_to_char( "  Skill level - the level the skill can be obtained and practiced.\n\r", ch );
	    send_to_char( "  Skill cost - the cost to gain the skill, also how hard it is to practice.\n\r               ( -1 for not available )\n\r", ch );
	    send_to_char( "  Adept % - the maximum precent the skill can be practiced.\n\r", ch );
	    send_to_char( "  Starting % - the starting precent a player will have in that skill.\n\r", ch );
	    send_to_char( "  Gained ( 0 or 1 )  Skill/Spell need not be gained\n\r", ch );
	    send_to_char( "  *Note* - npc races use these values also to determine each mobs skills.\n\r", ch );
	    return;
	}
	else if ( ( i = skill_lookup( arg1 ) ) < 0 )
	{
	    send_to_char( "Invalid skill name.\n\r", ch );
	    return;
	}
	else if ( ( value1 = atoi( arg2 ) ) < 0 || value1 > MAX_LEVEL )
	{
	    send_to_char( "Invalid skill level.\n\r", ch );
	    return;
	}
	else if ( ( value2 = atoi( arg3 ) ) == 0 || value2 < -1 )
	{
	    send_to_char( "Invalid skill cost.\n\r", ch );
	    return;
 	}
	else if ( ( value3 = atoi( arg4 ) ) < 1 || value3 > 100 )
	{
	    send_to_char( "Invalid adept %.\n\r", ch );
	    return;
	}
	else if ( ( value4 = atoi( arg5 ) ) < 0 || value4 > 100 )
	{
	    send_to_char( "Invalid starting %.\n\r", ch );
	    return;
	}
	else if ( ( value5 = atoi( arg6 ) ) < 0 || value5 > 1 )
	{
	    send_to_char( "Invalid gained  ( 0 or 1 ).\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].skill_level[ i ] = value1;
		race_table[ race ][ sex ].rating[ i ] = value2;
		race_table[ race ][ sex ].adept[ i ] = value3;
		race_table[ race ][ sex ].starting[ i ] = value4;
		race_table[ race ][ sex ].gained[ i ] = value5;
	    }
	    send_to_char( "Skill values set.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].skill_level[ i ] = value1;
	    race_table[ race ][ sex ].rating[ i ] = value2;
	    race_table[ race ][ sex ].adept[ i ] = value3;
	    race_table[ race ][ sex ].starting[ i ] = value4;
	    race_table[ race ][ sex ].gained[ i ] = value5;
	    send_to_char( "Skill values set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
    	return;
    }

    if ( !str_prefix( arg, "who_name" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: who_name <name> [sex]\n\r", ch );
	    send_to_char( "   No more then 5 letters long.\n\r", ch );
	    return;
	}
	else if ( strlen( arg1 ) > 5 )
	{
	    send_to_char( "Who name too long.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		strcpy( race_table[ race ][ sex ].who_name, arg1 );
	    }
	    send_to_char( "Who name set.\n\r", ch );
	}
	else
	{
	    strcpy( race_table[ race ][ sex ].who_name, arg1 );
	    send_to_char( "Who name set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "act_bit" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: act_bit <act> [sex]\n\r", ch );
	    sprintf( buf, "%s",act_bit_name( ALL_FLAGS ) );
	    sprintf(buf1, "Values:\n\r\n\r%s\n\r", format_string( str_dup( buf ) ) );
	    send_to_char( buf1, ch );
	    return;
	}
	else if ( ( i = act_name_bit( arg1 ) ) < 1 )
	{
	    send_to_char( "Invalid act flag.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		TOGGLE_BIT( race_table[ race ][ sex ].act, i );
	    }
	    send_to_char( "Act bit toggled.\n\r", ch );
	}
	else
	{
	    TOGGLE_BIT( race_table[ race ][ sex ].act, i );
	    send_to_char( "Act bit toggled.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "off_bit" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: off_bit <off flag> [sex]\n\r", ch );
	    sprintf( buf, "%s",off_bit_name( ALL_FLAGS ) );
	    sprintf(buf1, "Values:\n\r\n\r%s\n\r", format_string( str_dup ( buf ) ));
	    send_to_char( buf1, ch );
	    return;
	}
	else if ( ( i = off_name_bit( arg1 ) ) < 1 )
	{
	    send_to_char( "Invalid offensive flag.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		TOGGLE_BIT( race_table[ race ][ sex ].off, i );
	    }
	    send_to_char( "Offensive flag toggled.\n\r", ch );
	}
	else
	{
	    TOGGLE_BIT( race_table[ race ][ sex ].off, i );
	    send_to_char( "Offensive flag toggled.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "imm_bit" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: imm_bit <immune flag> [sex]\n\r", ch );
	    sprintf( buf, "%s",imm_bit_name( ALL_FLAGS ) );
	    sprintf(buf1, "Values:\n\r\n\r%s\n\r", format_string( str_dup ( buf ) ));
	    send_to_char( buf1, ch );
	    return;
	}
	else if ( ( i = imm_name_bit( arg1 ) ) < 1 )
	{
	    send_to_char( "Invalid immune flag.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		TOGGLE_BIT( race_table[ race ][ sex ].imm, i );
	    }
	    send_to_char( "Immune bit toggled.\n\r", ch );
	}
	else
	{
	    TOGGLE_BIT( race_table[ race ][ sex ].imm, i );
	    send_to_char( "Immune bit toggled.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "res_bit" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: res_bit <resistance flag> [sex]\n\r", ch );
	    sprintf( buf, "%s",res_bit_name( ALL_FLAGS ) );
	    sprintf(buf1, "Values:\n\r\n\r%s\n\r", format_string( str_dup ( buf ) ));
	    send_to_char( buf1, ch );
	    return;
	}
	else if ( ( i = res_name_bit( arg1 ) ) < 1 )
	{
	    send_to_char( "Invalid resistance flag.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		TOGGLE_BIT( race_table[ race ][ sex ].res, i );
	    }
	    send_to_char( "Resistance bit toggled.\n\r", ch );
	}
	else
	{
	    TOGGLE_BIT( race_table[ race ][ sex ].res, i );
	    send_to_char( "Resistance bit toggled.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "vuln_bit" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: vuln_bit <vulnerable flag> [sex]\n\r", ch );
	    sprintf( buf, "%s",vuln_bit_name( ALL_FLAGS ) );
	    sprintf(buf1, "Values:\n\r\n\r%s\n\r", format_string( str_dup ( buf ) ));
	    send_to_char( buf1, ch );
	    return;
	}
	else if ( ( i = vuln_name_bit( arg1 ) ) < 1 )
	{
	    send_to_char( "Invalid vulnerability flag.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		TOGGLE_BIT( race_table[ race ][ sex ].vuln, i );
	    }
	    send_to_char( "Vulnerability bit toggled.\n\r", ch );
	}
	else
	{
	    TOGGLE_BIT( race_table[ race ][ sex ].vuln,i );
	    send_to_char( "Vulnerability bit toggled.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "form" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: form <form flag> [sex]\n\r", ch );
	    sprintf( buf, "%s",form_bit_name( ALL_FLAGS ) );
	    sprintf(buf1, "Values:\n\r\n\r%s\n\r", format_string( str_dup ( buf ) ));
	    send_to_char( buf1, ch );
	    return;
	}
	else if ( ( i = form_name_bit( arg1 ) ) < 1 )
	{
	    send_to_char( "Invalid form flag.\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		TOGGLE_BIT( race_table[ race ][ sex ].form, i );
	    }
	    send_to_char( "Form flag toggled.\n\r", ch );
	}
	else
	{
	    TOGGLE_BIT( race_table[ race ][ sex ].form, i );
	    send_to_char( "Form flag toggled.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }
    if ( !str_prefix( arg, "affected" ) || !str_cmp( arg, "aff_bit" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: affected <affect> [sex]\n\r", ch );
	    sprintf( buf, "%s",affect_bit_name( ALL_FLAGS ) );
	    sprintf(buf1, "Values:\n\r\n\r%s\n\r", format_string( str_dup ( buf ) ));
	    send_to_char( buf1, ch );
	    sprintf( buf, "%s",affect_bit_name_2( ALL_FLAGS ) );
	    sprintf(buf1, "\n\r%s\n\r", format_string( str_dup ( buf ) ));
	    send_to_char( buf1, ch );
	    return;
	}
	else if ( ( i = affect_name_bit( arg1 ) ) < 1 )
	{
	 if ( ( i = affect_name_bit_2( arg1 ) ) < 1 )
	 {
	    send_to_char( "Invalid affect flag.\n\r", ch );
	    return;
	 }
	}
	if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		if ( affect_name_bit( arg1 ) > 1 )
		TOGGLE_BIT( race_table[ race ][ sex ].aff, i );
		else
		TOGGLE_BIT( race_table[ race ][ sex ].aff2, i );
	    }
	    send_to_char( "Affect flag toggled.\n\r", ch );
	}
	else
	{
	    if ( affect_name_bit( arg1 ) > 1 )
		TOGGLE_BIT( race_table[ race ][ sex ].aff, i );
	    else
		TOGGLE_BIT( race_table[ race ][ sex ].aff2, i );
	    send_to_char( "Affect flag toggled.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !IS_OPTIVISION(ch) )
    {
        interpret( ch, arguments );
	return;
    }

    if ( !str_cmp( arg, "pc" ) )
    {
	argument = one_argument( argument, arg1 );
    	if ( ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: pc [sex]\n\r", ch );
	    send_to_char( "   Allows players to select the (race/sex)\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].pc_race = TRUE;
	    }
	    send_to_char( "Players can choose the race.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].pc_race = TRUE;
	    send_to_char( "Players of that sex can choose the race.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "npc" ) )
    {
	argument = one_argument( argument, arg1 );
    	if ( ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: npc [sex]\n\r", ch );
	    send_to_char( "   Doesn't Allow players to select the (race/sex)\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
		race_table[ race ][ sex ].pc_race = FALSE;
	    }
	    send_to_char( "Players cannot choose the race.\n\r", ch );
	}
	else
	{
	    race_table[ race ][ sex ].pc_race = FALSE;
	    send_to_char( "Players of that sex cannot choose the race.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "builders" ) )
    {
	argument = one_argument( argument, arg1 );
	if ( arg1[0] == '\0' )
	{
	     send_to_char( "Syntax: builders <builder name>\n\r", ch);
	     return;
	}
	if ( !is_name( arg1, race_list[ race ].builders ) )
	{
	     strncpy( race_list[ race ].builders, string_replace( race_list[ race ].builders, arg1, "\0" ), 30 );
	     send_to_char( "Builder removed from list.\n\r", ch );
	}
	else
	{
	     buf[0] = '\0';
	     strcat( buf, race_list[ race ].builders ) ;
	     strcat( buf, " " );
	     strcat( buf, arg1 );
	     strncpy( race_list[ race ].builders, buf, 30 );
	     send_to_char( "Builder added to list.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_prefix( arg, "filename" ) )
    {
	argument = one_argument( argument, arg1 );
	if ( arg1[0] == '\0' )
	{
	    send_to_char( "Syntax: filename <name>\n\r", ch );
	    return;
	}
	else
	{
	    strncpy( race_list[ race ].filename, arg1, 20 );
	    send_to_char( "Filename set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "security" ) )
    {
	argument = one_argument( argument, arg1 );
	if ( arg1[0] == '\0' )
	{
	    send_to_char( "Syntax: security <number>\n\r", ch );
	    return;
	}
	else
	{
	    race_list[ race ].security = atoi( arg1 );
	    send_to_char( "Secutiry rating set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    if ( !str_cmp( arg, "name" ) )
    {
	argument = one_argument( argument, arg1 );
	argument = one_argument( argument, arg2 );
    	if ( arg1[0] == '\0' || ( sex = get_sex( arg2 ) ) == -1 )
	{
	    send_to_char( "Syntax: name <name> [sex] \n\r", ch );
	    send_to_char( "Example: name woman female (ie a human race)\n\r", ch );
	    return;
	}
	else if ( sex == 4 )
	{
	    for ( sex = 1 ; sex < 4 ; sex++ )
	    {
	    strncpy( race_table[ race ][ sex ].name, arg1, 15 );
	    send_to_char( "Race name set.\n\r", ch );
	    }
	}
	else
	{
	    strncpy( race_table[ race ][ sex ].name, arg1, 15 );
	    send_to_char( "Race sex name set.\n\r", ch );
	}
	SET_BIT( race_list[ race ].race_flags, RACE_CHANGED );
	return;
    }

    interpret( ch, arguments );
}
