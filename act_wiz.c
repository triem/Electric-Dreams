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
#include <sys/time.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "merc.h"

#define MAX_NEST 100

bool check_snoop_by( CHAR_DATA * ch, CHAR_DATA * victim );
void do_set_room_template( CHAR_DATA *ch, char * argument )
{
    ROOM_INDEX_DATA * pRoomtemp;
    
    if ( ( pRoomtemp = get_room_index( atoi( argument ) ) ) == NULL )
    {
	send_to_char( "Room not found\n\r", ch );
	send_to_char( "Syntax: set_room_template <room vnum>\n\r", ch );
	return;
    }
    ch->room_template = atoi( argument );
    send_to_char( "Done!\n\r", ch );
    return;
}
void do_cpu( CHAR_DATA *ch, char * argument )
{
    int x;
    char buf[MAX_STRING_LENGTH];
    for ( x = 30; x > 0; x-- )
    {
	sprintf(buf, "%d  Cpu Usage %d\n\r",x, per_cpu_usage[x]);
	send_to_char(buf,ch);
    }
}
void do_xyzzy( CHAR_DATA *ch, char * argument )
{
    if (!IS_OPTIVISION(ch ) )
    {
	send_to_char("Huh?\n\r",ch);
	return;
    }
    if (!str_cmp("enable", argument ) )
    {
	system("/usr4/test/area/enable");
	send_to_char("Done!  Turn it off as soon as you login!!!\n\r",ch);
	return;
    }
    if (!str_cmp("disable", argument ) )
    {
	system("/usr4/test/area/disable");
	send_to_char("Done!  Login disableed!\n\r",ch);
	return;
    }
    send_to_char("Syntax: xyzzy enable\n\r",ch);
    send_to_char("Syntax: xyzzy disable\n\r",ch);
}
void do_system( CHAR_DATA *ch, char * argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
     
    argument = one_argument(argument, arg);
    if ( arg[0] == '\0' )
    {
	sprintf(buf,"Autosave helps    %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_HELPS) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_HELPS) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave quests   %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_QUESTS) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_QUESTS) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave socials  %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_SOCIALS) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_SOCIALS) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave triggers %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_TRIGGERS) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_TRIGGERS) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave clans    %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_CLANS) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_CLANS) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave guilds   %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_GUILDS) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_GUILDS) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave races    %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_RACES) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_RACES) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave logons   %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_LOGONS) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_LOGONS) ? "Yes" : "No");
	send_to_char(buf,ch);
	sprintf(buf,"Autosave random   %3s  pending %s\n\r",
	    IS_SET(autosave_system, AUTOSAVE_RANDOM) ? "Yes" : "No",
	    IS_SET(changed_system, CHANGED_RANDOM) ? "Yes" : "No");
	send_to_char(buf,ch);
	
    }
    if (!strncmp("helps",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_HELPS);
	send_to_char("Helps Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("quests",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_QUESTS);
	send_to_char("Quests Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("logons",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_LOGONS);
	send_to_char("Logons Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("random",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_RANDOM);
	send_to_char("Random Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("clans",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_CLANS);
	send_to_char("Clans Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("races",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_RACES);
	send_to_char("Races Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("socials",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_SOCIALS);
	send_to_char("Socials Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("guilds",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_GUILDS);
	send_to_char("Guilds Autosave Toggled\n\r",ch);
	return;
    }
    if (!strncmp("triggers",arg, 3))
    {
	TOGGLE_BIT(autosave_system, AUTOSAVE_TRIGGERS);
	send_to_char("Triggers Autosave Toggled\n\r",ch);
	return;
    }
}
void reset_events( CHAR_DATA *ch, char * argument)
{
    char buf[ MAX_INPUT_LENGTH ];
    long flag;

    if ( argument[0] == '\0' )
    {
	send_to_char("Invalid flag\n\r",ch);
	send_to_char("reset_events <flag>\n\r",ch);
	sprintf(buf,"%s\n\r",get_event_flags( ALL_FLAGS ) );
	send_to_char(buf,ch);
	return;
    }

    if ( ( flag = get_flags_event( argument ) ) == 0 ) 
    {
	send_to_char("Invalid flag\n\r",ch);
	sprintf(buf,"%s\n\r",get_event_flags( ALL_FLAGS ) );
	send_to_char(buf,ch);
	return;
    }

    event_reset(flag, ch->in_room->area);

    send_to_char("Events reset\n\r",ch);
    return;

}
char * 	opti_wiz_list [ MAX_OPTIWIZ ] = 
{
	"reverie",
	"sands",
	"fork"
};

void do_select( CHAR_DATA *ch, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA * pObj;
    OBJ_DATA * obj;
    int  type;
    long vnum;
    int i;

    one_argument( argument, arg );

    if ( ( type = weapon_name_type( arg ) ) < 1 )
    {
	send_to_char( "Invalid weapon type.\n\r", ch );
	send_to_char( "Valid types:  sword, axe, dagger, flail, mace, polearm, spear, whip, staff, hand to hand\n\r", ch );
	return;
    }

    for ( i = 1; i < MAX_WEAPON_TYPE ; i++ )
	if ( ch->pcdata->learned[ gsn_weapon_power[i][0] ] != 0 )
	{
	    send_to_char( "Sorry you can only select 1 weapon.\n\r", ch );
	    return;
	}

    switch( type )
    {
	default:
	case WEAPON_SWORD:	vnum = 1505;	break;
	case WEAPON_FLAIL:	vnum = 1505;	break;
	case WEAPON_SPEAR:	vnum = 15902;	break;
	case WEAPON_MACE:	vnum = 1506;	break;
	case WEAPON_WHIP:	vnum = 1505;	break;
	case WEAPON_POLEARM:	vnum = 30004;	break;
	case WEAPON_DAGGER:	vnum = 15900;	break;
	case WEAPON_AXE:	vnum = 1503;	break;
    }

    if ( ( pObj = get_obj_index( vnum ) ) == NULL || ( obj = create_object( pObj ) ) == NULL )
    {
	send_to_char( "Cannot find weapon, try another.\n\r", ch );
	return;
    }
    
    ch->pcdata->learned[ gsn_weapon_power[ type ][ 0 ] ] = 40;
    approve_object(obj);
    obj_to_char( obj, ch );
    equip_char( ch, obj, WEAR_WIELD );
    send_to_char( "Weapon selected.\n\r", ch );
    reset_char(ch);
}

void backup_area( CHAR_DATA * ch, char * argument, int num )
{
    char buf[ MAX_INPUT_LENGTH ];
    char filename[ MAX_INPUT_LENGTH ];
    char * name;
    FILE * fp;
    int n;

    filename[ 0 ]= '\0';
    if ( num > 0 )
    {
        sprintf( buf, "find /usr6/live -mtime -%d -maxdepth 1 -mindepth 1 -print >/tmp/backup.out", num );
	system( buf );
	if ( ( fp = fopen( "/tmp/backup.out", "r" ) ) == NULL || feof( fp ) )
	{
	    send_to_char( "Backup failed.\n\r", ch );
	    return;
	}
	name = fread_word( fp );
	if ( fp )
	fclose( fp );

	strcpy( filename, name );
	strcat( filename, "/Rom2/area/" );
	strcat( filename, argument );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	{
	    send_to_char( "Cannot find backup area file for that date.\n\r", ch );
	    return;
	}
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of area file.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/rooms" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	{
	    send_to_char( "Cannot find backup rooms for that date.\n\r", ch );
	    return;
	}
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of rooms.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/mobiles" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	{
	    send_to_char( "Cannot find backup mobiles for that date.\n\r", ch );
	    return;
	}
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of mobiles.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/objects" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	{
	    send_to_char( "Cannot find backup objects for that date.\n\r", ch );
	    return;
	}
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of objects.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/resets" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	{
	    send_to_char( "Cannot find backup resets for that date.\n\r", ch );
	    return;
	}
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of resets.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/shops" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	{
	    send_to_char( "Cannot find backup shops for that date.\n\r", ch );
	    return;
	}
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of shops.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

        reboot_timer = 305;
	reboot_pause = FALSE;
	send_to_char( "Backup complete.\n\r", ch );
	return;
    }

    for ( n = 1 ; n < 14 ; n++ )
    {
        sprintf( buf, "find /usr6/live -mtime -%d -maxdepth 1 -mindepth 1 -print >/tmp/backup.out", n );
	system( buf );
	if ( ( fp = fopen( "/tmp/backup.out", "r" ) ) == NULL || feof( fp ) )
	    continue;

	name = fread_word( fp );
	if ( fp )
	fclose( fp );

	strcpy( filename, name );
	strcat( filename, "/Rom2/area/" );
	strcat( filename, argument );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	     continue;
	if ( fp )
	fclose( fp );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/mobiles" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	    continue;
	if ( fp )
	fclose( fp );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/objects" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	    continue;
	if ( fp )
	fclose( fp );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/resets" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	    continue;
	if ( fp )
	fclose( fp );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/shops" );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	    continue;
	if ( fp )
	fclose( fp );

	strcpy( filename, name );
	strcat( filename, "/Rom2/area/" );
	strcat( filename, argument );
	sprintf( buf, "Copying backup of area file.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/rooms" );
	sprintf( buf, "Copying backup of rooms.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/mobiles" );
	sprintf( buf, "Copying backup of mobiles.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/objects" );
	sprintf( buf, "Copying backup of objects.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/resets" );
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of resets.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

	strcpy( filename, name );
	strcat( filename, "/Rom2/savearea/" );
	strcat( filename, argument );
	strcat( filename, "/shops" );
	sprintf( buf, "Copying backup of shops.\n\r" );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );

        reboot_timer = 305;
	reboot_pause = FALSE;
	send_to_char( "Backup complete.\n\r", ch );
	return;
    }

    send_to_char( "Cannot find backup file.", ch );
}

bool backup_file( CHAR_DATA * ch, char * argument, int num )
{
    char buf[ MAX_INPUT_LENGTH ];
    char filename[ MAX_INPUT_LENGTH ];
    char * name;
    FILE * fp;
    int n;

    filename[ 0 ]= '\0';
    if ( num > 0 )
    {
        sprintf( buf, "find /usr6/live -mtime -%d -maxdepth 1 -mindepth 1 -print >/tmp/backup.out", num );
	system( buf );
	if ( ( fp = fopen( "/tmp/backup.out", "r" ) ) == NULL || feof( fp ) )
	{
	    send_to_char( "Backup failed.\n\r", ch );
	    return FALSE;
	}
	name = fread_word( fp );
	if ( fp )
	fclose( fp );
	strcpy( filename, name );
	strcat( filename, "/Rom2/" );
	strcat( filename, argument );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	{
	    send_to_char( "Cannot find backup file for that date.\n\r", ch );
	    return FALSE;
	}
	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of %s.\n\r", argument );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );
	send_to_char( "Backup complete.\n\r", ch );
	return TRUE;
    }

    for ( n = 1 ; n < 14 ; n++ )
    {
        sprintf( buf, "find /usr6/live -mtime -%d -maxdepth 1 -mindepth 1 -print >/tmp/backup.out", n );
	system( buf );
	if ( ( fp = fopen( "/tmp/backup.out", "r" ) ) == NULL || feof( fp ) )
	    continue;

	name = fread_word( fp );
	if ( fp )
	fclose( fp );
	strcpy( filename, name );
	strcat( filename, "/Rom2/" );
	strcat( filename, argument );
	if ( ( fp = fopen( filename, "r" ) ) == NULL )
	    continue;

	if ( fp )
	fclose( fp );
	sprintf( buf, "Copying backup of %s.\n\r", argument );
	send_to_char( buf, ch );
	sprintf( buf, "cp %s ../%s", filename, argument );
	system( buf );
	send_to_char( "Backup complete.\n\r", ch );
	return TRUE;
    }

    send_to_char( "Cannot find backup file.", ch );
    return FALSE;
}

void do_backup( CHAR_DATA * ch, char * argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char file_name[ MAX_INPUT_LENGTH ];
    CHAR_DATA * victim;
    AREA_DATA * pArea;
    DESCRIPTOR_DATA * d;
    int num = -1;
    int value;

    file_name[ 0 ] = '\0';

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[ 0 ] == '\0' || ( *argument != '\0' && !is_number( argument ) && ( num = atoi( argument ) ) <= 0 ) )
    {
	send_to_char( "Syntax: backup player <name>\n\r", ch );
	send_to_char( "               area <area filename>\n\r", ch );
	send_to_char( "               rooms <area filename>\n\r", ch );
	send_to_char( "               mobs <area filename>\n\r", ch );
	send_to_char( "               objects <area filename>\n\r", ch );
	send_to_char( "               resets <area filename>\n\r", ch );
	send_to_char( "               shops <area filename>\n\r", ch );
	return;
    }
    else if ( !str_cmp( arg1, "player" ) )
    {
	char name[ MAX_INPUT_LENGTH ];
	LOGON_DATA * logon_data;
	int sn;
	bool fUser = FALSE;

	if ( ( victim = get_char_world( ch, arg2 ) ) == NULL )
	{
	   send_to_char( "They are not here..\n\r", ch );
	   return;
	}

	if ( victim == ch ) fUser = TRUE;

	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Use backup mobile, to backup a mobile.\n\r", ch );
	    return;
	}

	d = victim->desc;
	logon_data = victim->logon_data;

	name[ 0 ] = '\0';
	strcpy( name, capitalize( arg2 ) );
	strcpy( file_name, "player/" );
	strcat( file_name, name );
	if ( backup_file( ch, file_name, num ) )
	{
	    extract_char( victim, TRUE );
	    load_char_obj( d, name );
	    victim = d->character;
	    if ( fUser )
		ch = victim;
	    send_to_char( "Your character has been backed up.\n\r", victim );
	    victim->logon_data = logon_data;
 	    strncpy( victim->logon_data->exit, "Logged-IN", 30 );
	    victim->next	= char_list;
	    char_list		= victim;
	    d->connected	= CON_PLAYING;
	    reset_char(victim);
	    if ( victim->in_room != NULL )
	        char_to_room( victim, victim->in_room );
	    else if ( IS_IMMORTAL(victim) )
	        char_to_room( victim, get_room_index( ROOM_VNUM_CHAT ) );
	    else
	        char_to_room( victim, get_room_index( ROOM_VNUM_TEMPLE ) );

	    for ( sn = 0; sn < MAX_SKILL; sn++ )
		if (skill_table[sn].name != NULL )
		if ( race_table[ victim->race ][ victim->sex ].gained[sn] == TRUE )
		if ( victim->pcdata->learned[sn] == 0 )
		 victim->pcdata->learned[sn] = race_table[ victim->race ][ victim->sex ].starting[sn];
	    victim->regen_timer	= current_time;

	    if (victim->pet != NULL)
	        char_to_room(victim->pet,victim->in_room);

	    if (victim->mount != NULL)
                if ( victim->mount->in_room == NULL )
	    	    char_to_room(victim->mount,victim->in_room);
	}
	return;
    }
    else if ( !str_prefix( arg1, "rooms" ) )
    {
	if ( arg2[ 0 ] == '\0' )
	    pArea = ch->in_room->area;
	else if ( !is_number( arg2 ) || ( value = atoi( arg2 ) ) < 1 )
	{
	    send_to_char( "Invalid area number.\n\r", ch );
	    return;
	}
	else if ( ( pArea = get_area_data( value ) ) == NULL )
	{
	    send_to_char( "That area does not exist.\n\r", ch );
	    return;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "Sorry you cannot edit that area.\n\r", ch );
	    return;
	}
	
	strcpy( file_name, "savearea/" );
	strcat( file_name, pArea->filename );	
	strcat( file_name, "/rooms" );

	if ( backup_file( ch, file_name, num ) )
	{
            reboot_timer = 305;
	    reboot_pause = FALSE;
	}
	return;
    }
    else if ( !str_prefix( arg1, "mobiles" ) || !str_cmp( arg1, "mobs" ) )
    {
	if ( arg2[ 0 ] == '\0' )
	    pArea = ch->in_room->area;
	else if ( !is_number( arg2 ) || ( value = atoi( arg2 ) ) < 1 )
	{
	    send_to_char( "Invalid area number.\n\r", ch );
	    return;
	}
	else if ( ( pArea = get_area_data( value ) ) == NULL )
	{
	    send_to_char( "That area does not exist.\n\r", ch );
	    return;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "Sorry you cannot edit that area.\n\r", ch );
	    return;
	}
	
	strcpy( file_name, "savearea/" );
	strcat( file_name, pArea->filename );	
	strcat( file_name, "/mobiles" );

	if ( backup_file( ch, file_name, num ) )
	{
            reboot_timer = 305;
	    reboot_pause = FALSE;
	}
	return;
    }
    else if ( !str_prefix( arg1, "objects" ) )
    {
	if ( arg2[ 0 ] == '\0' )
	    pArea = ch->in_room->area;
	else if ( !is_number( arg2 ) || ( value = atoi( arg2 ) ) < 1 )
	{
	    send_to_char( "Invalid area number.\n\r", ch );
	    return;
	}
	else if ( ( pArea = get_area_data( value ) ) == NULL )
	{
	    send_to_char( "That area does not exist.\n\r", ch );
	    return;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "Sorry you cannot edit that area.\n\r", ch );
	    return;
	}
	
	strcpy( file_name, "savearea/" );
	strcat( file_name, pArea->filename );	
	strcat( file_name, "/objects" );

	if ( backup_file( ch, file_name, num ) )
	{
            reboot_timer = 305;
	    reboot_pause = FALSE;
	}
	return;
    }
    else if ( !str_prefix( arg1, "resets" ) )
    {
	if ( arg2[ 0 ] == '\0' )
	    pArea = ch->in_room->area;
	else if ( !is_number( arg2 ) || ( value = atoi( arg2 ) ) < 1 )
	{
	    send_to_char( "Invalid area number.\n\r", ch );
	    return;
	}
	else if ( ( pArea = get_area_data( value ) ) == NULL )
	{
	    send_to_char( "That area does not exist.\n\r", ch );
	    return;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "Sorry you cannot edit that area.\n\r", ch );
	    return;
	}
	
	strcpy( file_name, "savearea/" );
	strcat( file_name, pArea->filename );	
	strcat( file_name, "/resets" );

	if ( backup_file( ch, file_name, num ) )
	{
            reboot_timer = 305;
	    reboot_pause = FALSE;
	}
	return;
    }
    else if ( !str_prefix( arg1, "shops" ) )
    {
	if ( arg2[ 0 ] == '\0' )
	    pArea = ch->in_room->area;
	else if ( !is_number( arg2 ) || ( value = atoi( arg2 ) ) < 1 )
	{
	    send_to_char( "Invalid area number.\n\r", ch );
	    return;
	}
	else if ( ( pArea = get_area_data( value ) ) == NULL )
	{
	    send_to_char( "That area does not exist.\n\r", ch );
	    return;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "Sorry you cannot edit that area.\n\r", ch );
	    return;
	}
	
	strcpy( file_name, "savearea/" );
	strcat( file_name, pArea->filename );	
	strcat( file_name, "/shops" );

	if ( backup_file( ch, file_name, num ) )
	{
            reboot_timer = 305;
	    reboot_pause = FALSE;
	}
	return;
    }
    else if ( !str_prefix( arg1, "area" ) )
    {
	if ( arg2[ 0 ] == '\0' )
	    pArea = ch->in_room->area;
	else if ( !is_number( arg2 ) || ( value = atoi( arg2 ) ) < 1 )
	{
	    send_to_char( "Invalid area number.\n\r", ch );
	    return;
	}
	else if ( ( pArea = get_area_data( value ) ) == NULL )
	{
	    send_to_char( "That area does not exist.\n\r", ch );
	    return;
	}

	if ( !IS_BUILDER( ch, pArea ) )
	{
	    send_to_char( "Sorry you cannot edit that area.\n\r", ch );
	    return;
	}
	
	backup_area( ch, pArea->filename, num );
	return;	    
    }

    do_backup( ch, "" );
}

void do_wed( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA * ch1, * ch2;
    OBJ_DATA *ring;

    if ( *argument == '\0' )
    {
	send_to_char( "Syntax: wed <player> <player>\n\r", ch );
	return;
    }

    argument = one_argument( argument, buf );

    if ( ( ch1 = get_char_room( ch, buf ) ) == NULL || IS_NPC( ch1 ) )
    {
	send_to_char( "Invalid player.\n\r", ch );
	return;
    }

    argument = one_argument( argument, buf );

    if ( ( ch2 = get_char_room( ch, buf ) ) == NULL || IS_NPC( ch2 ) )
    {
	send_to_char( "Invalid player.\n\r", ch );
	return;
    }

    if ( ch1 == ch2 )
    {
	send_to_char( "You must wed to different people.\n\r", ch );
	return;
    }

    if ( ch1 == ch || ch2== ch )
    {
	send_to_char( "You cannot wed yourself.\n\r", ch );
	return;    
    }

    if ( *ch1->mate != '\0' )
    {
	act("$N is already married.\n\r", ch, NULL, ch1, TO_CHAR );
	return;
    }

    if ( *ch2->mate != '\0' )
    {
	act("$N is already married.\n\r", ch, NULL, ch2, TO_CHAR );
	return;
    }

    strncpy( ch1->mate, ch2->name, 12 );
    strncpy( ch2->mate, ch1->name, 12 );

    if ( ch1->pcdata != NULL )
	strncpy( ch1->pcdata->title, " has just been married.", 80 );

    if ( ch2->pcdata != NULL )
	strncpy( ch2->pcdata->title, " has just been married.", 80 );

    ring = create_object( get_obj_index( OBJ_VNUM_WED_RING ) );
    if ( ring != NULL )
    {
	sprintf( buf, "%s's wedding ring", capitalize( ch1->name ) );
	strncpy( ring->short_descr, buf, 80 );
	strncpy( ring->description,  buf, 80 );
	approve_object(ring);
	obj_to_char( ring, ch1 );
    }

    ring = create_object( get_obj_index( OBJ_VNUM_WED_RING ) );
    if ( ring != NULL )
    {
	sprintf( buf, "%s's wedding ring", capitalize( ch2->name ) );
	strncpy( ring->short_descr, buf, 80 );
	strncpy( ring->description, buf, 80 );
	approve_object(ring);
	obj_to_char( ring, ch2 );
    }

    sprintf( buf, "%s has married you to %s.\n\r", ch->name, ch1->name );
    send_to_char( buf, ch2 );
    sprintf( buf, "%s has married you to %s.\n\r", ch->name, ch2->name );
    send_to_char( buf, ch1 );
    sprintf( buf, "You have married %s to %s.\n\r", ch1->name, ch2->name );
    send_to_char( buf, ch );
    sprintf( buf, "%s has married %s to %s.\n\r", ch->name, ch1->name, ch2->name );
    act( buf, ch, ch1, ch2, TO_NOTVICT );
}

void do_simulate( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    int value, x, i, sn;

    if ( !is_number( argument ) )
    {
	send_to_char( "Syntax: simulate <level>\n\r", ch );
	return;
    }


    value = atoi( argument );
    if ( value == 0 ) value = ch->level;  

    if ( value < 1 || value > MAX_LEVEL ) 
    {
	send_to_char( "Invalid level.\n\r", ch );
	return;
    }

    ch->pcdata->condition[COND_THIRST] = 48;
    ch->pcdata->condition[COND_FULL] = 48;
    ch->pcdata->condition[COND_DRUNK] = 0;
    ch->level		= value;
    ch->hit		= ch->level * 10 + ch->level *10 ;   
    ch->max_hit		= ch->hit;
    ch->pcdata->perm_hit = ch->max_hit;
    ch->move		= ch->level *15 + 10;
    ch->max_move	= ch->level *15 + 10;
    ch->pcdata->perm_move = ch->max_move;
    ch->exp = exp_table[ ch->level ].exp_at_level;
    ch->pkills = 0;
    ch->died = 0;
    ch->kills = 0;
    for (i = 0; i < MAX_STATS; i++)
    {
	ch->perm_stat[i] = race_table[ch->race][ch->sex].stats[i];
	ch->mod_stat[i]=0;
    }
    for (x = 0; x < ch->level; x++)
    for (i = 0; i < MAX_STATS; i++)
    {
	if ( ch->perm_stat[i] < 25 )
	if ( number_range(0,3) == 0 )
	ch->perm_stat[i] += 1;
    }
    ch->affected_by = race_table[ch->race][ch->sex].aff;
    ch->affected_by_2 = race_table[ch->race][ch->sex].aff2;
    ch->pcdata->perm_aff = race_table[ch->race][ch->sex].aff;
    ch->pcdata->perm_aff_2 = race_table[ch->race][ch->sex].aff2;
    ch->imm_flags	= race_table[ch->race][ch->sex].imm;
    ch->res_flags	= race_table[ch->race][ch->sex].res;
    ch->vuln_flags	= race_table[ch->race][ch->sex].vuln;
    ch->form	= race_table[ch->race][ch->sex].form;
    ch->size = race_table[ch->race][ch->sex].size;
    ch->height = get_height_size( ch->size );
    ch->weight = get_weight_size( ch->size );
    strncpy( ch->pcdata->prompt, "`yHp %c`y/`w%H `yMv %d`y/`w%V`y-> ", 160 );
    for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
    {
	ch->pcdata->learned[sn] = 0;
	if ( ch->level >= LEVEL_IMMORTAL )
	    ch->pcdata->learned[ sn ] = 100;
	else
	    if ( race_table[ ch->race ][ ch->sex ].skill_level[ sn ] < ch->level
	      && race_table[ ch->race ][ ch->sex ].rating[ sn ] > 0 )
	    ch->pcdata->learned[ sn ] = URANGE( 0, ( ( ch->level - race_table[ ch->race ][ ch->sex ].skill_level[ sn ] + 1 ) * 35 ) / race_table[ ch->race ][ ch->sex ].rating[ sn ], 100 );
	else
	    ch->pcdata->learned[ sn ] = 0;
    }
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        ch->max_mana[i] = race_table[ ch->race ][ ch->sex ].mana_die[ i ] * race_table[ ch->race ][ ch->sex ].mana_dice[ i ] + race_table[ ch->race ][ ch->sex ].mana_bonus[ i ];
	if ( ch->level > 1 )
	    ch->max_mana[i] *= 7*ch->level;
	else
	    ch->max_mana[i] *= 10*ch->level;
        ch->mana[i]	= ch->max_mana[ i ];
	ch->pcdata->perm_mana[i] = ch->max_mana[i];
    }

    sprintf( buf, "You are now level %d!\n\r", value );
    send_to_char( buf, ch );
}

void do_rlist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    int race;
    for ( race = 0; race < top_race ; race++ )
    {
	sprintf( buf, " %2d - %15s %19s %15s %2d %15s\n\r", race, race_list[ race ].name,
		race_list[ race ].filename, race_list[ race ].builders,
		race_list[ race ].security, race_table[race][1].name);
	send_to_char( buf, ch );
    }
    return;
}

void do_glist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    int guild;
    for ( guild = 0; guild < top_guild ; guild++ )
    {
	sprintf( buf, " %2d - %15s %20s %20s %2d\n\r", guild, guild_table[ guild ].name,
		guild_table[ guild ].filename, guild_table[ guild ].builders,
		guild_table[ guild ].security);
	send_to_char( buf, ch );
    }
    return;
}


void do_saveraces( CHAR_DATA *ch, char *argument )
{
    if ( is_number( argument ) )
	save_race( atoi( argument ) );
    else if ( !str_cmp( argument, "list" ) )
	save_rlist();
    else
	save_races();
}
void do_saveguilds( CHAR_DATA *ch, char *argument )
{
    if ( is_number( argument ) )
	save_guild( atoi( argument ) );
    else if ( !str_cmp( argument, "list" ) )
	save_glist();
    else
	save_guilds();
}

void do_bonus ( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA * pobj;
    OBJ_DATA * obj;
    MOB_INDEX_DATA * pmob;
    CHAR_DATA * mob;
    char name[31];
    static BHOST_LIST * host_list = NULL;
    BHOST_LIST * list;
    BHOST_LIST * list_next;
    int value;
    int sn=0;
    int i;
    int type;
    bool found=FALSE;

    if ( *argument != 'a' || str_cmp( argument, "auto" ) )
    {

    while ( host_list != NULL && host_list->time <= current_time )
    {
	list = host_list;
	host_list = host_list->next;
	free_bhost_list(list);
    }

    list_next = host_list;

    for ( list = host_list ; list != NULL && list_next != NULL ; list = list_next )
    {
	list_next = list->next;

	if ( list_next != NULL && list_next->time <= current_time )
	{
	    list->next = list_next->next;
	    free_bhost_list(list_next);
	    list_next = list->next;
	}

    }

    list = host_list;

    while( list != NULL )
    {
	if ( !str_cmp( list->host, ch->desc->host ) )
	{
	    list->time = current_time + 4500;
	    return;
	}
	list = list->next;
    }	

    list = bhost_list_alloc();
    list->time = current_time + 1500;
    strncpy( list->host, ch->desc->host, 15 );
    list->next = host_list;
    host_list = list;

    }

    if ( !is_number( argument ) || !IS_IMMORTAL( ch ) )
        value = number_range( 1, 20 );
    else
	value = atoi( argument );
   switch ( value )
   {
/* Money */
   case 1:
   case 2:
	sprintf( buf, "You inherited %d %s from your parents to help you in your adventures.\n\r", 5000, coin_names[ race_table[ ch->race ][ ch->sex ].world ] );
	send_to_char( buf, ch );
	ch->gold[ race_table[ ch->race ][ ch->sex ].world ] += 5000;
	break;
/* Armor */
   case 3:
   case 4:
	value = ( number_range( 0, 1 ) ? 18098 : 18100 );
	if ( ( pobj = get_obj_index( value ) ) == NULL )
	    do_bonus( ch, "auto" );
	else
	{
	    strcpy( name, ch->name );
	    obj = create_object( pobj );
	    sprintf( buf, obj->short_descr, name );
	    strncpy( obj->short_descr, buf, 80 );
	    sprintf( buf, obj->description, name );
	    strncpy( obj->description, buf, 80 );
	    act( "You inherit $p from your father.", ch, obj, obj, TO_CHAR );
	    approve_object(obj);
	    obj_to_char( obj, ch );
	}
	break;
/* Weapon */
   case 5:
   case 6:
	value = ( ch->sex == SEX_FEMALE ? 18101 : ( number_range( 0, 1 ) ? 18096 : 18097 ) );
	if ( ( pobj = get_obj_index( value ) ) == NULL )
	    do_bonus( ch, "auto" );
	else
	{
	    strcpy( name, ch->name );
	    obj = create_object( pobj );
	    sprintf( buf, obj->short_descr, name );
	    strncpy( obj->short_descr, buf, 80 );
	    sprintf( buf, obj->description, name );
	    strncpy( obj->description, buf, 80 );
	    if ( ch->sex == SEX_FEMALE )
	        act( "You inherit $p from your mother.", ch, obj, obj, TO_CHAR );
	    else
	        act( "You inherit $p from your father.", ch, obj, obj, TO_CHAR );
	    approve_object(obj);
	    obj_to_char( obj, ch );
	}
	break;
/* Potion */
   case 7:
   case 8:
	value = number_range( 18112, 18172 );
	if ( ( pobj = get_obj_index( value ) ) == NULL )
	    do_bonus( ch, "auto" );
	else
	{
	    obj = create_object( pobj );
	    act( "You inherit $p from your parents.", ch, obj, obj, TO_CHAR );
	    approve_object(obj);
	    obj_to_char( obj, ch );
	}
	break;
/*  skill % */
   case 9:
   case 10:
	for ( sn = 0 ; sn < MAX_SKILL && !found ; sn++ )
	    if ( race_table[ ch->race ][ ch->sex ].skill_level[ sn ] < 49
	      && race_table[ ch->race ][ ch->sex ].rating[ sn ] > 0
	      && ch->pcdata->learned[ sn ] < 25 )
		found = TRUE;

	if ( !found )
	{
	    do_bonus( ch, "auto" );
	    return;
	}

	found = FALSE;

	while ( !found )
	{
	   sn = number_range( 0, MAX_SKILL - 1 );
	   if ( race_table[ ch->race ][ ch->sex ].skill_level[ sn ] < 49 
	     && race_table[ ch->race ][ ch->sex ].rating[ sn ] > 0
	     && ch->pcdata->learned[ sn ] < 25 )
		found = TRUE;
	}

	if ( skill_table[ sn ].spell_fun == spell_null )
	    sprintf( buf, "As a child you were taught the art of %s.", skill_table[ sn ].name );
	else
	    sprintf( buf, "As a child you were taught the art of casting %s.", skill_table[ sn ].name );

	ch->pcdata->learned[ sn ] = number_range( 25, race_table[ ch->race ][ ch->sex ].adept[ sn ] );
	send_to_char( buf, ch );
	break;
/* mount */
   case 11:
   case 12:
	if ( ( pmob = get_mob_index( 25015 ) ) == NULL )
	    do_bonus( ch, "auto" );
	else
	{
	    send_to_char( "Your parents give you a mount to help you in your adventures.\n\r", ch );
	    mob = create_mobile( pmob ); 
	    char_to_room( mob, ch->in_room );
	    add_follower( mob, ch);
	    mob->leader = ch;
	    ch->mount = mob;
	    ch->pcdata->learned[ gsn_riding ] = 85;
	}
	break;
/* bag of holding */
   case 13:
   case 14:
	if ( ( pobj = get_obj_index( 18099 ) ) == NULL )
	    do_bonus( ch, "auto" );
	else
	{
	    strcpy( name, ch->name );
	    obj = create_object( pobj );
	    sprintf( buf, obj->short_descr, name );
	    strncpy( obj->short_descr, buf, 80 );
	    sprintf( buf, obj->description, name );
	    strncpy( obj->description, buf, 80 );
	    act( "You inherit $p from your parents.", ch, obj, obj, TO_CHAR );
	    approve_object(obj);
	    obj_to_char( obj, ch );
	}
	break;
/* hp/mana/move */
   case 15:
   case 16:
	i = number_range( 10, 25 );
	switch ( number_range( 0, 2 ) )
	{
	case 0:
	    sprintf( buf, "As a hardy child you tended to have more hit points then your friends.\n\r" );
	    send_to_char( buf, ch );
	    ch->max_hit += i;
	    ch->hit = ch->max_hit;
	    ch->pcdata->perm_hit = ch->max_hit;
	    break;
	case 1:
	    type = number_range( 0, MAX_ELEMENT_TYPE );
	    sprintf( buf, "You found at an early age you had a very high magic potential.\n\r" );
	    send_to_char( buf, ch );
	    ch->max_mana[ type ] += i;
	    ch->mana[ type ] = ch->max_mana[ type ];
	    ch->pcdata->perm_mana[ type ] = ch->max_mana[ type ];
	    break;
	case 2:
	    sprintf( buf, "The young explorer in you had always been on the move.\n\r" );
	    send_to_char( buf, ch );
	    ch->max_move += 3 * i;
	    ch->move = ch->max_move;
	    ch->pcdata->perm_move = ch->max_move;
	    break;
	}
	break;
/* trains/practices */
   case 17:
   case 18:
	{
	    send_to_char( "As a child you always enjoyed practicing new skills.\n\r", ch );
	    ch->practice += number_range( 4, 8 );
	}
	break;
/* stats */
   case 19:
   case 20:
	for ( i = 0 ; i < MAX_STATS && !found ; i++ )
	    if ( ch->perm_stat[ i ] < race_table[ ch->race ][ ch->sex ].max_stats[ i ] )
		found = TRUE;

	if ( !found )
	{
	    do_bonus( ch, "auto" );
	    return;
	}


	while ( ch->perm_stat[ i = number_range( 0, MAX_STATS - 1 ) ] >= race_table[ ch->race ][ ch->sex ].max_stats[ i ] );

	switch ( i )
	{
	case STAT_STR:
	    send_to_char( "As a child you were always stronger then your friends.\n\r", ch );
	    break;
	case STAT_DEX:
	    send_to_char( "As a child you had surprisingly good balance.\n\r", ch );
	    break;
	case STAT_INT:
	    send_to_char( "As a child you were always more intelligent then your friends.\n\r", ch );
	    break;
	case STAT_WIS:
	    send_to_char( "As a child you were always wiser then your friends.\n\r", ch );
	    break;
	case STAT_CON:
	    send_to_char( "You were a very healthy child.\n\r", ch );
	    break;
	case STAT_CHR:
	    send_to_char( "As a child you found it easy to make new friends.\n\r", 
ch );
	    break;
	}
	ch->perm_stat[ i ] = UMIN( ch->perm_stat[ i ] + number_range( 1, 2 ), race_table[ ch->race ][ ch->sex ].max_stats[ i ] );
	break;

   }

}


/* equips a character */
void do_outfit ( CHAR_DATA *ch, char *argument )
{
    OBJ_DATA *obj;
    char buf[MAX_STRING_LENGTH];
    
    return;

    if (ch->outfit_timer > 0 )
	{
	   sprintf(buf,"Wait a bit and try again.  We are currently out of stock\n\r");
	   send_to_char(buf,ch);
	   return;
	}
    ch->outfit_timer = 2;
    if (ch->level > 5 || IS_NPC(ch))
    {
	send_to_char("Find it yourself!\n\r",ch);
	return;
    }

    if ( ( obj = get_eq_char( ch, WEAR_LIGHT ) ) == NULL 
      && !has_obj_vnum( ch, OBJ_VNUM_BEGIN_LIGHT )
      && !obj_vnum_in_room( ch->in_room, OBJ_VNUM_BEGIN_LIGHT ) )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_BEGIN_LIGHT) );
	obj->wear_data->cost = 0;
	approve_object(obj);
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_LIGHT );
    }
 
    if ( ( obj = get_eq_char( ch, WEAR_WIELD ) ) == NULL 
      && !has_obj_vnum( ch, race_table[ch->race][ch->sex].weapon )
      && !obj_vnum_in_room( ch->in_room, race_table[ch->race][ch->sex].weapon ) )
    { 
        obj = create_object( get_obj_index(race_table[ch->race][ch->sex].weapon));
	approve_object(obj);
        obj_to_char( obj, ch );
        equip_char( ch, obj, WEAR_WIELD );
    }

    if (!has_obj_vnum( ch, OBJ_VNUM_BEGIN_LIGHTER ) 
      && !has_obj_vnum( ch, OBJ_VNUM_BEGIN_LIGHTER )
      && !obj_vnum_in_room( ch->in_room, OBJ_VNUM_BEGIN_LIGHTER ) )
    {
        obj = create_object( get_obj_index(OBJ_VNUM_BEGIN_LIGHTER) );
	obj->wear_data->cost = 0;
	approve_object(obj);
        obj_to_char( obj, ch );
    }

    ch->hands_free = 0;

    send_to_char("You have been equipped by the gods.\n\r",ch);
}

void do_cpr( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    DESCRIPTOR_DATA *d;
    bool found;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Retrieve who's corpse?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	found = FALSE;
	for ( d = descriptor_list ; d != NULL ; d = d->next )
	{
	    if ( d->character != NULL 
	      && can_see( ch, d->character )
	      && d->connected <= CON_PLAYING
	      && d->character->pcdata != NULL
	      && d->character->pcdata->corpse != NULL )
	    {
		found = TRUE;
		strcpy( arg, d->character->name );
		do_cpr( ch, arg );
	    }
	}

	if ( !found )
	    send_to_char( "No corpses to retrieve.\n\r", ch );

	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }

    if ( IS_NPC( victim ) || victim->pcdata == NULL )
    {
	send_to_char( "You can only retrieve player corpses.\n\r", ch );
	return;
    }

    if ( victim->pcdata->corpse == NULL )
    {
	act( "$N doesn't have a corpse.\n\r", ch, NULL, victim, TO_CHAR );
	return;
    }

    if ( victim->pcdata->corpse->carried_by != NULL )
    {
	obj_from_char( victim->pcdata->corpse,"K" );
	act( "$p disappears.", victim->pcdata->corpse->carried_by, victim->pcdata->corpse, victim->pcdata->corpse, TO_CHAR );
    }
    else if ( victim->pcdata->corpse->in_obj != NULL )
	obj_from_obj( victim->pcdata->corpse );
    else if ( victim->pcdata->corpse->in_room != NULL )
    {
	obj_from_room( victim->pcdata->corpse );
    }

	obj_to_room( victim->pcdata->corpse, victim->in_room );

    act( "$N's corpse has been retrieved.", ch, NULL, victim, TO_CHAR );
    act( "Your corpse appears before you.", victim, NULL, NULL, TO_CHAR );
    act( "$n's corpse appears in the room.", victim, NULL, ch, TO_NOTVICT );

}

void do_saveclans( CHAR_DATA *ch, char *argument )
{

	save_clans( ); /* anything else? */ /* NO. */

}
void do_makeclan( CHAR_DATA *ch, char *argument )
{
    CLAN_DATA *clan;

    if ( strlen( argument ) == 0 )
    {
    	send_to_char( "You must give the new clan a name\n\r",ch);
	return;
    }

    clan = clan_data_alloc();
    strncpy( clan->title, argument, 80 );
    strncpy( clan->name, argument, 30 );
    clan->leader_name[0] = '\0';
    clan->sponser_name[0] = '\0';
    clan->leader = NULL;
    clan->sponser = NULL;
    clan->treasury = 0;
    clan->recall_room = 2;
    clan->donate_room = 2;
    strncpy ( clan->rank1, "Neophyte", 30);
    clan->rank1[31] = '\0';
    strncpy ( clan->rank2, "Underling", 30);
    clan->rank2[31] = '\0';      
    strncpy ( clan->rank3, "Apprentice", 30);
    clan->rank3[31] = '\0';      
    strncpy ( clan->rank4, "Scout", 30);
    clan->rank4[31] = '\0';      
    strncpy ( clan->rank5, "Tracker", 30);
    clan->rank5[31] = '\0';      
    strncpy ( clan->rank6, "Squire", 30);
    clan->rank6[31] = '\0';      
    strncpy ( clan->rank7, "Knight", 30);
    clan->rank7[31] = '\0';      
    strncpy ( clan->rank8, "Overlord", 30);
    clan->rank8[31] = '\0';      
    strncpy ( clan->rank9, "Arch-Bishop", 30);
    clan->rank9[31] = '\0';      
    strncpy ( clan->rank10, "Supreme Grand Vizir", 30);
    clan->rank10[31] = '\0';      
    clan->next = clan_list;
    clan_list = clan;

    send_to_char("done. Use set to change values.\n\r",ch);
    SET_BIT(changed_system, CHANGED_CLANS);
}

void do_clist( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_INPUT_LENGTH];
    CLAN_DATA *clan;
    bool found = FALSE;

    send_to_char( "Clans -\n\r\n\r",ch);
    for ( clan = clan_list ; clan != NULL ; clan = clan->next )
    {
	if ( IS_SET( clan->clan_info, CLAN_UNLISTED ) && !IS_IMMORTAL( ch ) && ch->pcdata->pcclan->clan != clan )
	    continue;

	found = TRUE;
	sprintf(buf,"%s - %s\n\r",clan->title,clan->leader_name);
	send_to_char( buf, ch );
    }

    if ( !found )
	send_to_char( "No clans right now.\n\r",ch);
}

void do_cshow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CLAN_DATA * clan = NULL;
    if (ch->pcdata->pcclan != NULL)
        clan = ch->pcdata->pcclan->clan;
    one_argument( argument, arg );
    if ( IS_IMMORTAL( ch ) && arg[0] == '\0' )
    {
	send_to_char("Invalid clan name.\n\r",ch);
	return;
    }
    else if ( IS_IMMORTAL( ch ) )
	clan = clan_lookup( arg );

    if ( clan == NULL )
    {
	send_to_char("Huh?",ch);
	return;
    }

    sprintf( arg, "Clan title: %s\n\r",clan->title);
    send_to_char( arg, ch );

    if ( IS_IMMORTAL( ch ) )
    {
	sprintf( arg, "Clan names: %s\n\r",clan->name);
	send_to_char( arg, ch );
    }

    sprintf( arg, "Clan Leader: %s    Clan Sponser: %s\n\rTreasury: %ld\n\r", clan->leader_name, clan->sponser_name, clan->treasury );
    send_to_char( arg, ch );

    if ( IS_IMMORTAL( ch ) )   
    {
	sprintf( arg, "Clan Flags: %s\n\r", clan_flag_name( clan->clan_info ) );
	send_to_char( arg, ch );
	sprintf( arg, "Recall Room: %ld    Donation Room: %ld\n\r", clan->recall_room, clan->donate_room );
	send_to_char( arg, ch );
	sprintf( arg, "Ranks: %s %s %s %s %s\n\r", clan->rank1, clan->rank2, clan->rank3, clan->rank4, clan->rank5);
	send_to_char( arg, ch );
	sprintf( arg, "       %s %s %s %s %s\n\r", clan->rank6, clan->rank7, clan->rank8, clan->rank9, clan->rank10);
	send_to_char( arg, ch);
    }

}
     
void do_recruit( CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char rank[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CLAN_DATA *clan;
    PC_CLAN_DATA *new_pcclan;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Who are you trying to recruit?", ch );
	return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL || 
       IS_NPC(victim))
    {
        send_to_char("They are not on.\n\r",ch);
        return;
    }
    if ( IS_IMMORTAL(ch))
    {
    	if ( arg2[0] == '\0' )
	{
            send_to_char("Syntax:\n\r",ch);
            send_to_char("  recruit <name> <clan> \n\r",ch);
	    return;
	}
    	if ( victim->pcdata->pcclan != NULL )
        {
            send_to_char("They already belong to a clan!\n\r",ch);
            return;
	}
	clan = clan_lookup(arg2);
        if ( clan == NULL )
        {
            send_to_char("Not a valid clan name, use clist to see names.",ch);
            return;
        }
    }
    else
    {
	if ( ch->pcdata->pcclan == NULL )
	{
	    send_to_char("You cannot recruit when you are not in a clan! \n", ch);
	    return;
	}
	if ( !strstr(ch->name, ch->pcdata->pcclan->clan->leader->name))
	{
	    send_to_char("Only the leader of a clan may recruit! \n", ch);
	    return;
	}
	if ( ch->in_room != victim->in_room )
	{
	    send_to_char("You must be in the same room to recruit someone. \n", ch);
	    return;
	}
    	if ( victim->pcdata->pcclan != NULL )
    	{
            send_to_char("They already belong to a clan!\n\r",ch);
            return;
	}
	
	clan = ch->pcdata->pcclan->clan;
    }
    /* At this point we have a valid victim and valid clan */
    new_pcclan = pcclan_data_alloc();
    new_pcclan->clan = clan;
    new_pcclan->clanrank = 1;
    victim->pcdata->pcclan = new_pcclan;
    get_clanrank(victim, rank);
    sprintf(buf, "Welcome to %s!\n\r", victim->pcdata->pcclan->clan->title);
    send_to_char(buf, victim);
    sprintf(buf, "Your rank is now: %s\n\r", rank);
    send_to_char(buf, victim);
    sprintf(buf, "%s has now joined %s.\n\r", victim->name, victim->pcdata->pcclan->clan->name );
    send_to_char(buf, ch);

    return;
}

void do_blackball( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
   
    if (arg[1] == '\0')
    {
	send_to_char("Who do you wish to blackball?\n\r", ch);
	return;
    }
    if ( ( victim = get_char_world( ch, arg ) ) == NULL ||
       IS_NPC(victim))
    {
        send_to_char("They are not on.\n\r",ch);
        return;
    }
    if ( victim->pcdata->pcclan == NULL )
    {
 	send_to_char("They are not in a clan.\n\r", ch);
	return;
    }
    send_to_char("You have been blackballed!!!\n\r", victim);
    sprintf(buf, "You are no longer a member of %s!\n\r", victim->pcdata->pcclan->clan->title);
    send_to_char(buf, victim);
    sprintf(buf, "%s is no longer a member of %s.\n\r", victim->name, victim->pcdata->pcclan->clan->name);
    send_to_char(buf, ch);
    free_pcclan_data(victim->pcdata->pcclan);
    victim->pcdata->pcclan = NULL;
}

void do_clanrank( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    char rank[MAX_INPUT_LENGTH];
    int  newclanrank;
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
        send_to_char( "Who are you trying to recruit?", ch );
        return;
    }
    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL ||
       IS_NPC(victim))
    {
        send_to_char("They are not on.\n\r",ch);
        return;
    }

    if ( IS_IMMORTAL(ch))
    {
        if ( arg2[0] == '\0' )
        {
            send_to_char("Syntax:\n\r",ch);
            send_to_char("  clanrank <name> <rank> \n\r",ch);
            return;
        } 
    }
    if (victim->pcdata->pcclan == NULL)
    {
        act("$n does not belong to a clan.", victim, NULL, ch, TO_CHAR);
	return;
    }
    if ( ( newclanrank = atoi( arg2 ) ) < 1 || newclanrank > 10 )
    {
        send_to_char( "Clanrank must be 1 to 10.\n\r", ch );
        return;
    }
    if ( victim->pcdata->pcclan->clanrank == newclanrank )
    {
	send_to_char( "No change in rank. \n\r", ch );
	return;
    }
    if ( victim->pcdata->pcclan->clanrank < newclanrank )
  	send_to_char( "You have been promoted within your clan! \n\r", victim );
    if ( victim->pcdata->pcclan->clanrank > newclanrank )
        send_to_char( "You have been demoted within your clan! \n\r", victim );  
    victim->pcdata->pcclan->clanrank = newclanrank;
    get_clanrank(victim, rank);
    sprintf(buf, "Your rank is now: %s\n\r", rank);
    send_to_char(buf, victim);
    sprintf(buf, "Rank is now: %s\n\r", rank);
    send_to_char(buf, ch);

    return;
}

/* RT nochannels command, for those spammers */
void do_nochannels( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
 
    one_argument( argument, arg );
 
    if ( arg[0] == '\0' )
    {
        send_to_char( "Nochannel whom?", ch );
        return;
    }
 
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
        send_to_char( "They aren't here.\n\r", ch );
        return;
    }
 
    if(!check_immortal_order(ch, victim) )
    {
        send_to_char( "You failed.\n\r", ch );
        return;
    }
 
    if ( IS_SET(victim->comm, COMM_NOCHANNELS) )
    {
        REMOVE_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "The gods have restored your channel priviliges.\n\r", 
		      victim );
        send_to_char( "NOCHANNELS removed.\n\r", ch );
    }
    else
    {
        SET_BIT(victim->comm, COMM_NOCHANNELS);
        send_to_char( "The gods have revoked your channel priviliges.\n\r", 
		       victim );
        send_to_char( "NOCHANNELS set.\n\r", ch );
    }
 
    return;
}

void do_bamfin( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];

    if ( !IS_NPC(ch) )
    {
	if (argument[0] == '\0')
	{
	    sprintf(buf,"Your poofin is %s\n\r",ch->pcdata->bamfin);
	    send_to_char(buf,ch);
	    return;
	}

	if ( strstr(argument,ch->name) == NULL)
	{
	    send_to_char("You must include your name.\n\r",ch);
	    return;
	}
	     
	strncpy( ch->pcdata->bamfin, argument, 160 );

        sprintf(buf,"Your poofin is now %s\n\r",ch->pcdata->bamfin);
        send_to_char(buf,ch);
    }
    return;
}



void do_bamfout( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
 
    if ( !IS_NPC(ch) )
    {
        if (argument[0] == '\0')
        {
            sprintf(buf,"Your poofout is %s\n\r",ch->pcdata->bamfout);
            send_to_char(buf,ch);
            return;
        }
 
        if ( strstr(argument,ch->name) == NULL)
        {
            send_to_char("You must include your name.\n\r",ch);
            return;
        }
 
        strncpy( ch->pcdata->bamfout, argument, 160 );
 
        sprintf(buf,"Your poofout is now %s\n\r",ch->pcdata->bamfout);
        send_to_char(buf,ch);
    }
    return;
}

void do_deny( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Deny whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }


    if(!check_immortal_order(ch, victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    SET_BIT(victim->plr, PLR_DENY);
    send_to_char( "You are denied access!\n\r", victim );
    send_to_char( "OK.\n\r", ch );
    save_char_obj(victim);
    do_quit( victim, "" );

    return;
}



void do_disconnect( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Disconnect whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	act( "$N doesn't have a descriptor.", ch, NULL, victim, TO_CHAR );
	return;
    }

    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d == victim->desc )
	{
	    close_socket( d );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    bug( "Do_disconnect: desc not found.", 0 );
    send_to_char( "Descriptor not found!\n\r", ch );
    return;
}

void pardon_death( CHAR_DATA * ch )
{
    AFFECT_DATA * af;
    AFFECT_DATA * af_next;

    for ( af = ch->affected ; af != NULL ; af = af_next )
    {
	af_next = af->next;
	if ( af->type == -2 )
	    affect_remove( ch, af );
    }
}

void do_pardon( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char( "Syntax: pardon <character> <killer|thief|debt|death|ph>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "death" ) )
    {
	pardon_death( victim );
	send_to_char( "Death has been pardoned.\n\r", ch );
	send_to_char( "Your death has been pardoned.\n\r", victim );
    }

    if ( !str_cmp( arg2, "debt" ) )
    {
	if ( victim->pcdata != NULL && victim->pcdata->debt != NULL )
	{
	    free_debt_data( victim->pcdata->debt );
	    victim->pcdata->debt = NULL;
	    send_to_char( "Debt has been pardoned.\n\r", ch );
	    send_to_char( "Your debt has been pardoned.\n\r", victim );
    	}
	return;
    }

    if ( !str_cmp( arg2, "thief" ) )
    {
	if ( IS_SET(victim->plr, PLR_THIEF) )
	{
	    REMOVE_BIT( victim->plr, PLR_THIEF );
	    send_to_char( "Thief flag removed.\n\r", ch );
	    send_to_char( "You are no longer a THIEF.\n\r", victim );
	}
	return;
    }
    if ( !str_cmp( arg2, "ph" ) )
    {
	if ( IS_SET(victim->plr2, PLR_PLAYER_HELPER) )
	{
	    REMOVE_BIT( victim->plr2, PLR_PLAYER_HELPER );
	    send_to_char( "Player helper flag removed.\n\r", ch );
	    send_to_char( "You are no longer a player helper.\n\r", victim );
	}
	else 
	{
	    SET_BIT( victim->plr2, PLR_PLAYER_HELPER );
	    send_to_char( "Player helper flag added.\n\r", ch );
	}
	return;
    }
    send_to_char( "Syntax: pardon <character> <killer|thief|debt|ph>.\n\r", ch );
    return;
}



void do_donate (CHAR_DATA *ch, char *argument)
{
   char arg[MAX_INPUT_LENGTH];
   OBJ_DATA *obj;
   OBJ_DATA *obj2;
   CLAN_DATA *clan;
   ROOM_INDEX_DATA *to_room;
   int coin_type,value;
   int count = 0;

   argument = one_argument( argument, arg );
   if (arg[0] == '\0')
   {
	send_to_char("What would you like to donate?\n\r",ch);
	return;
   }
 
   if ( is_number( arg ) )
   {
      value = atoi(arg);
      argument = one_argument( argument, arg );
      coin_type = get_coin_type( arg );
      if ( coin_type == -1 )
 	coin_type = race_table[ ch->race ][ ch->sex ].world; 
      else
         one_argument( argument, arg );

      if ( value > get_money_total( ch, coin_type ) )
      {
	 send_to_char("You do not have that much.\n\r",ch);
	 return;
      }
      if ( !ch->pcdata )
      {
	   /* Added 08092000 by Galadrail:   */
	   /* Crashed i guess from an imp switching to a mob.  No pcdata then */
	   send_to_char("You must be a character to donate\n\r",ch);
	   return;
	
      }

      clan = clan_lookup( arg );
      if ( clan == NULL )
      {
         if ( ch->pcdata->pcclan == NULL )
         {
	   send_to_char("Invalid coin name or clan name\n\r",ch);
	   return;
         }
         else
	   clan = ch->pcdata->pcclan->clan;
      }
      get_char_money( ch, value, coin_type );
      clan->treasury+=value;
      if ( clan->leader != NULL )
	 act("$n has donated some money to the clan.", clan->leader, NULL, ch, TO_CHAR);
      if ( clan->sponser != NULL )
	 act("$n has donated some money to the clan.", clan->sponser, NULL, ch, TO_CHAR);
      return;
   }

   if ( ( obj = get_obj_carry (ch, arg) ) == NULL )
   {
      send_to_char ("You don't have that to donate. \n\r",ch);
      return;
   }

   if ( !can_drop_obj (ch, obj, FALSE) )
   {
      send_to_char ("It doesn't want to be donated. \n\r",ch);
      return;
   }

   if ( ch->pcdata->pcclan != NULL )
   {
	if ( (  to_room = get_room_index( ch->pcdata->pcclan->clan->donate_room ) ) == NULL )
	  if ( ( to_room = get_room_index( race_table[ ch->race ][ ch->sex ].donate ) ) == NULL )
	  to_room = get_room_index( 2 );
   }
   else
   if ( ( to_room = get_room_index( race_table[ ch->race ][ ch->sex ].donate ) ) == NULL )
    to_room = get_room_index( 2 );

   for ( obj2 = ch->in_room->contents ; obj2 != NULL ; obj2 = obj2->next_content ) count++;
   if ( count >= 400 )
   {
	send_to_char("There is no room to donate anything.\n\r", ch );
	return;
   }

   if ( obj->wear_data != NULL )
       obj->wear_data->cost=0;
   obj->owner[0] = '\0';
   obj->donate_timer = 50;
   obj_from_char ( obj, "L" );
   obj_to_room( obj, to_room );
   act ("$n donates $p to anyone who can get it.\n\r",ch,obj,NULL,TO_ROOM );
   act ("You donate $p to the mud welfare system.\n\r", ch, obj, NULL, TO_CHAR);
   act("The Universe opens up and somethings falls to the floor.\n\r",to_room->people, NULL, NULL, TO_ROOM );
   act("The Universe opens up and somethings falls to the floor.\n\r",to_room->people, NULL, NULL, TO_CHAR );
   return;
 }

void do_echo( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Global echo what?\n\r", ch );
	return;
    }
    
    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected <= CON_PLAYING)
	{
    	    if(!check_immortal_order(ch, d->character) )
		send_to_char( "`Wglobal> ",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "`w\n\r",   d->character );
        }
    }

    return;
}

void do_wecho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "World echo what?\n\r", ch );
	return;
    }
    
    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected <= CON_PLAYING 
	  && d->character->in_room->area->world == ch->in_room->area->world )
	{
    	    if(!check_immortal_order(ch, d->character) )
		send_to_char( "`Wwecho> ",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "`w\n\r",   d->character );
        }
    }

    return;
}

void do_aecho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Area echo what?\n\r", ch );
	return;
    }
    
    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected <= CON_PLAYING 
	  && d->character->in_room->area == ch->in_room->area )
	{
    	    if(!check_immortal_order(ch, d->character) )
		send_to_char( "`Waecho> ",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "`w\n\r",   d->character );
        }
    }

    return;
}



void do_recho( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    
    if ( argument[0] == '\0' )
    {
	send_to_char( "Local echo what?\n\r", ch );

	return;
    }

    for ( d = descriptor_list; d; d = d->next )
    {
	if ( d->connected <= CON_PLAYING
	&&   d->character->in_room == ch->in_room )
	{
    	    if(!check_immortal_order(ch, d->character) )
                send_to_char( "local> ",d->character);
	    send_to_char( argument, d->character );
	    send_to_char( "\n\r",   d->character );
	}
    }

    return;
}

void do_pecho( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    argument = one_argument(argument, arg);
 
    if ( argument[0] == '\0' || arg[0] == '\0' )
    {
	send_to_char("Personal echo what?\n\r", ch); 
	return;
    }
   
    if  ( (victim = get_char_world(ch, arg) ) == NULL )
    {
	send_to_char("Target not found.\n\r",ch);
	return;
    }

    if (victim->level >= ch->level && !IS_OPTIVISION(ch))
        send_to_char( "personal> ",victim);

    send_to_char(argument,victim);
    send_to_char("\n\r",victim);
    send_to_char( "personal> ",ch);
    send_to_char(argument,ch);
    send_to_char("\n\r",ch);
}


ROOM_INDEX_DATA *find_location( CHAR_DATA *ch, char *arg )
{
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    if ( is_number(arg) )
	return get_room_index( atoi( arg ) );

    if ( ( victim = get_char_world( ch, arg ) ) != NULL )
	return victim->in_room;

    if ( ( obj = get_obj_world( ch, arg ) ) != NULL )
	return obj->in_room;

    return NULL;
}


void trans_obj( CHAR_DATA * ch, char * arg, ROOM_INDEX_DATA *location )
{
    OBJ_DATA * obj;

    if ( ( obj = get_obj_world( ch, arg ) ) == NULL )
    {
	send_to_char( "You cannot find that person or object.\n\r", ch );
	return;
    }

    if ( obj->carried_by != NULL )
    {
	obj_from_char( obj, "M" );
	act( "$p disappears.", obj->carried_by, obj, obj, TO_CHAR );
    }
    else if ( obj->in_obj != NULL )
	obj_from_obj( obj );
    else if ( obj->in_room != NULL )
    {
	obj_from_room( obj );
    }

    if ( obj->wear_data != NULL && location == ch->in_room )
	obj_to_char( obj, ch );
    else
	obj_to_room( obj, location );

    act( "$p transfered.", ch, obj, obj, TO_CHAR );
}


void do_transfer( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;
    bool fObj = FALSE;

    argument = one_argument( argument, arg1 );
    if ( !str_prefix( arg1, "object" ) )
    {
	fObj = TRUE;
	argument = one_argument( argument, arg1 );
    }
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' )
    {
	send_to_char( "Transfer whom/what (and where)?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg1, "all" ) && !fObj )
    {
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    if ( d->connected <= CON_PLAYING
	    &&   d->character != ch
	    &&   d->character->in_room != NULL
	    &&   can_see( ch, d->character ) )
	    {
		char buf[MAX_STRING_LENGTH];
		sprintf( buf, "%s %s", d->character->name, arg2 );
		do_transfer( ch, buf );
	    }
	}
	return;
    }

    /*
     * Thanks to Grodyn for the optional location parameter.
     */
    if ( arg2[0] == '\0' )
    {
	location = ch->in_room;
    }
    else
    {
	if ( ( location = find_location( ch, arg2 ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}

	if ( room_is_private( location ) && !IS_OPTIVISION(ch))
	{
	    send_to_char( "That room is private right now.\n\r", ch );
	    return;
	}
    }

    if ( fObj || ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	trans_obj( ch, arg1, location );
	return;
    }

    if ( victim->in_room == NULL )
    {
	send_to_char( "They are in limbo.\n\r", ch );
	return;
    }

    if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );

    if ( victim->inside_area != NULL ) /* don't carry the moveable area data */
	free_inside_area_data(victim->inside_area);
	victim->inside_area = NULL;

    act( "$n disappears in a mushroom cloud.", victim, NULL, NULL, TO_ROOM );
    char_from_room( victim );
    char_to_room( victim, location );
    act( "$n `warrives from a puff of smoke.", victim, NULL, NULL, TO_ROOM );
    if ( ch != victim )
	act( "$n `whas transferred you.", ch, NULL, victim, TO_VICT );
    do_look( victim, "auto" );
    if ( victim->mount != NULL && IS_SET(victim->mount->affected_by,AFF_MOUNTED) )
    {
        char_from_room( victim->mount );
        char_to_room( victim->mount, location );
    }
    send_to_char( "Ok.\n\r", ch );
}



void do_at( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    ROOM_INDEX_DATA *original;
    CHAR_DATA *wch;
    
    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "At where what?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, arg ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) && !IS_OPTIVISION(ch))
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    original = ch->in_room;
    char_from_room( ch );
    char_to_room( ch, location );
    interpret( ch, argument );

    /*
     * See if 'ch' still exists before continuing!
     * Handles 'at XXXX quit' case.
     */
    for ( wch = char_list; wch != NULL; wch = wch->next )
    {
	if ( wch == ch )
	{
	    char_from_room( ch );
	    char_to_room( ch, original );
	    break;
	}
    }

    return;
}


void do_goto( CHAR_DATA *ch, char *argument )
{
    ROOM_INDEX_DATA *location;
    CHAR_DATA *rch;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Goto where?\n\r", ch );
	return;
    }

    if ( ( location = find_location( ch, argument ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( room_is_private( location ) && !IS_OPTIVISION(ch))
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    if ( ch->fighting != NULL )
	stop_fighting( ch, TRUE );

    if ( ch->inside_area != NULL ) /* don't carry the moveable area data */
	free_inside_area_data(ch->inside_area);
	ch->inside_area = NULL;

    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
	if ( can_see( rch, ch ) )
	{
	    if (ch->pcdata != NULL && ch->pcdata->bamfout[0] != '\0')
		act("$t",ch,ch->pcdata->bamfout,rch,TO_VICT);
	    else
		act("`M$n leaves in a swirling mist.`w",ch,NULL,rch,TO_VICT);
	}
    }

    char_from_room( ch );
    char_to_room( ch, location );


    for (rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room)
    {
        if ( can_see( rch, ch ) )
        {
            if (ch->pcdata != NULL && ch->pcdata->bamfin[0] != '\0' )
                act("$t",ch,ch->pcdata->bamfin,rch,TO_VICT);
            else
                act("$n appears in a swirling mist.",ch,NULL,rch,TO_VICT);
        }
    }

    do_look( ch, "auto" );

    if ( ch->mount != NULL && IS_SET(ch->mount->affected_by,AFF_MOUNTED) )
    {
        char_from_room( ch->mount );
        char_to_room( ch->mount, location );
    }
    return;
}

/* RT to replace the 3 stat commands */
void do_stat ( CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];
   char *string;
   OBJ_DATA *obj;
   ROOM_INDEX_DATA *location;
   CHAR_DATA *victim;

   string = one_argument(argument, arg);
   if ( arg[0] == '\0')
   {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  stat <name>\n\r",ch);
	send_to_char("  stat obj <name>\n\r",ch);
	send_to_char("  stat mob <name>\n\r",ch);
 	send_to_char("  stat room <number>\n\r",ch);
 	send_to_char("  stat spell <number> || <name>\n\r",ch);
	return;
   }

   if (!str_cmp(arg,"room"))
   {
	do_rstat(ch,string);
	return;
   }

   if (!str_cmp(arg,"spell"))
   {
	do_sstat(ch,string);
	return;
   }
   if (!str_cmp(arg,"skill"))
   {
	do_sstat(ch,string);
	return;
   }
  
   if (!str_cmp(arg,"obj"))
   {
	do_ostat(ch,string);
	return;
   }

   if(!str_cmp(arg,"char")  || !str_cmp(arg,"mob"))
   {
	do_mstat(ch,string);
	return;
   }
   
   /* do it the old way */

   obj = get_obj_world(ch,argument);
   if (obj != NULL)
   {
     do_ostat(ch,argument);
     return;
   }

  victim = get_char_world(ch,argument);
  if (victim != NULL)
  {
    do_mstat(ch,argument);
    return;
  }

  location = find_location(ch,argument);
  if (location != NULL)
  {
    do_rstat(ch,argument);
    return;
  }

  send_to_char("Nothing by that name found anywhere.\n\r",ch);
}

   



void do_rstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    LOCK_DATA *lock_list;
    SPELL_LIST *spell;
    OBJ_DATA *obj;
    CHAR_DATA *rch;
    int door;

    one_argument( argument, arg );
    location = ( arg[0] == '\0' ) ? ch->in_room : find_location( ch, arg );
    if ( location == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    if ( ch->in_room != location && room_is_private( location ) && 
	!IS_OPTIVISION(ch) )
    {
	send_to_char( "That room is private right now.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: '%s.'\n\rArea Number: %d. Area: '%s' World number: %d.\n\r Area file name: '%s'.\n\r",
	location->name,
	location->area->areanum,
	location->area->name,
	location->area->world,
	location->area->filename  );
    send_to_char( buf, ch );

    sprintf( buf,
	"Vnum: %ld.  Sector: %d.  Light: %d.\n\r",
	location->vnum,
	location->sector_type,
	location->light );
    send_to_char( buf, ch );

    sprintf( buf,
	"Room flags: %ld.\n\rDescription:\n\r%s",
	location->room_flags,
	location->description );
    send_to_char( buf, ch );

    if ( location->extra_descr != NULL )
    {
	EXTRA_DESCR_DATA *ed;

	send_to_char( "Extra description keywords: '", ch );
	for ( ed = location->extra_descr; ed; ed = ed->next )
	{
	    send_to_char( ed->keyword, ch );
	    if ( ed->next != NULL )
		send_to_char( " ", ch );
	}
	send_to_char( "'.\n\r", ch );
    }

    send_to_char( "Characters:", ch );
    for ( rch = location->people; rch; rch = rch->next_in_room )
    {
	if (can_see(ch,rch))
        {
	    send_to_char( " ", ch );
	    one_argument( rch->name, buf );
	    send_to_char( buf, ch );
	}
    }

    send_to_char( ".\n\rObjects:   ", ch );
    for ( obj = location->contents; obj; obj = obj->next_content )
    {
	send_to_char( " ", ch );
	one_argument( obj->name, buf );
	send_to_char( buf, ch );
    }
    send_to_char( ".\n\r", ch );

    for ( door = 0; door <= 5; door++ )
    {
	EXIT_DATA *pexit;

	if ( ( pexit = location->exit[door] ) != NULL )
	{
	    sprintf( buf,
		"Door: %d.  To: %ld.  Exit flags: %ld.\n\rKeyword: '%s'.  Description: %s",

		door,
		(pexit->u1.to_room == NULL ? -1 : pexit->u1.to_room->vnum),
	    	pexit->exit_info_default,
	    	pexit->keyword,
	    	pexit->description[0] != '\0'
		    ? pexit->description : "(none).\n\r" );
	    send_to_char( buf, ch );
	    if ( pexit->keyword[0] == '\0' || strlen(pexit->keyword) == 0 
	 	|| !IS_SET( pexit->exit_info_default, EX_ISDOOR ) )
		continue;
	    if ( pexit->trap != NULL )
	    {
		sprintf( buf, "Trap flags : %s\n\n", get_flags_trap( pexit->trap->flags ) );
		send_to_char( buf, ch );

		for ( spell = pexit->trap->spells ; spell != NULL ; spell = spell->next )
		{
		    sprintf( buf, "   Level %d spell of %s\n\r", spell->level, skill_table[ spell->spell ].name );
		    send_to_char( buf, ch );
		}
 	    }
	    for ( lock_list = pexit->lock ; lock_list != NULL ; lock_list = lock_list->next )
	    {
		sprintf( buf, "Key: %ld   Lock info : ", lock_list->vnum );
	 	if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		    strcat( buf, "(locked) ");
		else
		    strcat( buf, "(unlocked) " );
	 	if (  IS_SET( lock_list->lock_info, LOCK_PICKPROOF ) )
		    strcat( buf, "(pick proof) ");
	 	if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		    strcat( buf, "(dead bolt) ");
		strcat( buf, "\n\r");
		send_to_char( buf, ch );
	    }
	    if ( pexit->u1.to_room != NULL && pexit->u1.to_room->exit[ rev_dir[ door ] ] != NULL
		&& pexit->u1.to_room->exit[ rev_dir[ door ] ]->u1.to_room == location
 		&& ( lock_list = pexit->u1.to_room->exit[ rev_dir[ door ] ]->lock ) != NULL )
	    {
		send_to_char( "  - Otherside - \n\r", ch );
	        for ( ; lock_list != NULL ; lock_list = lock_list->next )
	        {
		    sprintf( buf, "Key: %ld   Lock info : ", lock_list->vnum );
	     	    if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
			    strcat( buf, "(locked) ");
	   	    else
		    	strcat( buf, "(unlocked) " );
	   	    if (  IS_SET( lock_list->lock_info, LOCK_PICKPROOF ) )
		    	strcat( buf, "(pick proof) ");
	 	    if (  IS_SET( lock_list->lock_info, LOCK_LOCKED ) )
		    	strcat( buf, "(dead bolt) ");
		    strcat( buf, "\n\r");
	 	    send_to_char( buf, ch );
	        }
	    }
	}
    }

    return;
}

void do_sstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn,x;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stat what spell?\n\r", ch );
	return;
    }
    if ( is_number(arg ) )
    {
	if ( atoi(arg) >= MAX_SKILL  || atoi(arg) < 1 )
	{
	    send_to_char( "Spell not found\n\r", ch );
	    return;
	}
	
	if ( skill_table[atoi(arg)].name == NULL )
	{
	    send_to_char( "Spell not found\n\r", ch );
	    return;
	}
	sn = atoi(arg);
    }
    else if ( !str_cmp( arg, "ungrouped" ) )
    {
	sprintf( buf, "Ungrouped skills/spells:\n\r\n\r" );

	for ( sn = 1 ; sn < MAX_SKILL ; sn++ )
	{
	    if ( skill_table[ sn ].group != 0 )
		continue;

	     strcat( buf, skill_table[ sn ].name );
	     strcat( buf, "\n\r" );
	}

	send_to_char( buf, ch );
	return;
    }
    else
    {
	sn = skill_lookup(arg);
	if ( sn < 0 || sn >= MAX_SKILL)
	{
	    send_to_char( "Sn not found\n\r", ch );
	    return;
	}
    }
    sprintf(buf,"`MName:`w %-25s     `MSN:`w %-3d\n\r",skill_table[sn].name, sn);
    send_to_char(buf,ch);
    if ( sizeof(skill_table[sn].char_pass ) > 1 )
    {
    sprintf(buf,"%s \n\r",skill_table[sn].char_pass);
    send_to_char(buf,ch);
    sprintf(buf,"%s \n\r",skill_table[sn].other_pass);
    send_to_char(buf,ch);
    sprintf(buf,"%s \n\r",skill_table[sn].vict_pass);
    send_to_char(buf,ch);
    sprintf(buf,"%s \n\r",skill_table[sn].char_fail);
    send_to_char(buf,ch);
    sprintf(buf,"%s \n\r",skill_table[sn].other_fail);
    send_to_char(buf,ch);
    sprintf(buf,"%s \n\r",skill_table[sn].vict_fail);
    send_to_char(buf,ch);
    }
    for ( x = 0; x < MAX_PREREQUISITE; x++ )
    if ( skill_table[sn].prerequisite[x].pgsn != NULL )
    {
	sprintf(buf,"`MPrerequisite:`w %s\n\r",skill_table[*skill_table[sn].prerequisite[x].pgsn].name);
	send_to_char(buf,ch);
    }
    for ( x = 0; x < MAX_COMPONENT; x++ )
    if ( skill_table[sn].component[x].vnum != 0 )
    {
	sprintf(buf,"`MComponents:`w Vnum %ld\n\r",skill_table[sn].component[x].vnum);
	send_to_char(buf,ch);
    }
    for ( x = 0; x < MAX_ELEMENT_TYPE; x++ )
    {
	sprintf(buf,"`MElements:`w %10s  `MMana`w %3d    `MLevel`w %d\n\r",element_type_name(x),skill_table[sn].element[x].mana, skill_table[sn].element[x].level );
	send_to_char(buf,ch);
    }
    sprintf(buf,"`MGroup:`w %s\n\r", group_name[ skill_table[sn].group ] );
    send_to_char( buf, ch );
    sprintf(buf,"`MTargets:`w %s\n\r",target_type_name(skill_table[sn].target) );
    send_to_char(buf,ch);
    sprintf(buf,"`MMin Pos:`w %s\n\r",get_pos_name( skill_table[sn].minimum_position ) );
    send_to_char(buf,ch);
    sprintf(buf,"`MBeats:`w %d\n\r",skill_table[sn].beats );
    send_to_char(buf,ch);
    sprintf(buf,"`MDam Mes:`w %s\n\r",skill_table[sn].noun_damage );
    send_to_char(buf,ch);
    sprintf(buf,"`MOff Mes:`w %s\n\r",skill_table[sn].msg_off );
    send_to_char(buf,ch);
return;
}



void do_ostat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    AFFECT_DATA *paf;
    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stat what?\n\r", ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, argument ) ) == NULL )
    {
	send_to_char( "Nothing like that in hell, earth, or heaven.\n\r", ch );
	return;
    }


    sprintf(buf,"`R****************************************************\n\r");
    send_to_char(buf, ch);
    sprintf( buf,"`R* `wVnum: `G%ld \n\r`R* `wName: `G%s\n\r",
        obj->pIndexData->vnum,obj->name);
    send_to_char(buf, ch);
    sprintf( buf,"`R* `wArea: `G(%d) '%s'\n\r",
        obj->pIndexData->in_area->areanum,
        obj->pIndexData->in_area->name );
    send_to_char( buf, ch );
    sprintf( buf, "`R* `wShort description: `G%s\n\r`R* `wLong description: `G%s",
        obj->short_descr, obj->description );
    send_to_char( buf, ch );
    if ( obj->wear_data != NULL )
    {
    sprintf( buf, "`R* `wWear flags: `G%s `wExtra flags: `G%s  `Spec Flag:  `G%s `wShop Type: `G%s\n\r",
        ( obj->wear_data != NULL ? wear_bit_name_1(obj->wear_data->wear_flags) : "None" ),
        extra_bit_name( obj->extra_flags ),special_bit_name(obj->special_flags), shop_type_name( obj->shop_type ) );
    send_to_char( buf, ch );
   sprintf( buf, "`R* `wCondition: `G%d `wMaterial: `G%s `wTimer: `G%d `wCost: `G%ld `wWeight: `G%d `wSize `G%d\n\r",
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
        OBJ_INDEX_DATA *obj1;
        ROOM_INDEX_DATA *room;

        sprintf( buf, "`R* `wGate flags: `G%s %ld\n\r",
            gate_flag_name( obj->gate_data->flags ),obj->gate_data->flags );
        send_to_char( buf, ch );
        sprintf( buf, "`R* `wLeads to: `G%s (%ld) `w(average timer `G%d `w) vnum `G%ld\n\r",
            ( IS_SET( obj->gate_data->flags, GATE_TO_MOB ) && ( mob = get_mob_index( obj->gate_data->vnum ) ) != NULL ? mob->short_descr :
            ( IS_SET( obj->gate_data->flags, GATE_TO_OBJ ) && ( obj1 = get_obj_index( obj->gate_data->vnum ) ) != NULL ? obj1->short_descr :
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

    sprintf( buf,
	"In room: %ld  In object: %s  Carried by: %s  Wear_loc: %d  Owner: %s\n\r",
	obj->in_room    == NULL    ?        0 : obj->in_room->vnum,
	obj->in_obj     == NULL    ? "(none)" : obj->in_obj->short_descr,
	obj->carried_by == NULL    ? "(none)" : 
	    can_see(ch,obj->carried_by) ? obj->carried_by->name
				 	: "someone",
	( obj->wear_data != NULL ? obj->wear_data->wear_loc : 0 ),
	obj->owner[0] == '\0'    ? "(none)" : obj->owner );
    send_to_char( buf, ch );

    sprintf(buf,"`R****************************************************\n\r");
    send_to_char(buf, ch);
    sprintf( buf, "%d\n\r", obj->donate_timer );
    send_to_char(buf, ch);
    return;

}



void do_mstat( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    AFFECT_DATA *paf;
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Stat whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    sprintf( buf, "Name: %s.\n\r",
	victim->name );
    send_to_char( buf, ch );
    sprintf( buf, "Vnum: %ld  Format: %s  Race: %s  Sex: %s Room: %ld\n\r",
	IS_NPC(victim) ? victim->pIndexData->vnum : 0,
	IS_NPC(victim) ? "mobile" : "pc", 
	race_table[victim->race][victim->sex].name,
	victim->sex == SEX_MALE    ? "male"   :
	victim->sex == SEX_FEMALE  ? "female" : "neuter",
	victim->in_room == NULL    ?        0 : victim->in_room->vnum
	);
    send_to_char( buf, ch );

    if ( victim->guilds != 0 )
    {
	sprintf( buf, "Guilds: %s\n\r", guild_bit_name( victim->guilds ) );
	send_to_char( buf, ch );
    }

    if (IS_NPC(victim))
    {
	sprintf(buf, "In area: '%s' (filename: '%s')\n\r",
	    victim->pIndexData->load_with_area->name,
	    victim->pIndexData->load_with_area->filename);
	send_to_char(buf, ch);
	sprintf(buf,"Count: %d  Killed: %d\n\r",
	    victim->pIndexData->count,victim->pIndexData->killed);
	send_to_char(buf,ch);
	sprintf(buf,"Mood: %s %d\n\r",
	    mood_table[victim->mood].mood,victim->mood);
	send_to_char(buf,ch);
    }

    sprintf( buf, 
   	"Str: %d(%d)  Int: %d(%d)  Wis: %d(%d)  Dex: %d(%d)  Con: %d(%d)  Chr: %d(%d)\n\r",
	victim->perm_stat[STAT_STR],
	get_curr_stat(victim,STAT_STR),
	victim->perm_stat[STAT_INT],
	get_curr_stat(victim,STAT_INT),
	victim->perm_stat[STAT_WIS],
	get_curr_stat(victim,STAT_WIS),
	victim->perm_stat[STAT_DEX],
	get_curr_stat(victim,STAT_DEX),
	victim->perm_stat[STAT_CON],
	get_curr_stat(victim,STAT_CON),
	victim->perm_stat[STAT_CHR],
	get_curr_stat(victim,STAT_CHR) );
    send_to_char( buf, ch );

    sprintf( buf, "Hp: %d/%d  Move: %d/%d  Practices: %d\n\r",
	victim->hit,         victim->max_hit,
	victim->move,        victim->max_move,
	IS_NPC(ch) ? 0 : victim->practice );
    send_to_char( buf, ch );

    sprintf( buf, "`Rfire: `G%d/%d  `Cwater: `G%d/%d  `Gearth: `G%d/%d  `Bwind: `G%d/%d  `Wspirit: `G%d/%d`w\n\r",
	victim->mana[0],	victim->max_mana[0],
	victim->mana[1],	victim->max_mana[1],
	victim->mana[2],	victim->max_mana[2],
	victim->mana[3],	victim->max_mana[3],
	victim->mana[4],	victim->max_mana[4]);
    send_to_char( buf, ch );
	
    sprintf( buf,
	"Lv: %d  %s: %ld  %s: %ld  %s: %ld  %s: %ld \n\rExp: %ld  Pkills: %d  Kills: %d  Died: %d\n\r",
	victim->level,
        coin_names[0], victim->gold[0], coin_names[1],  victim->gold[1],
        coin_names[2], victim->gold[2], coin_names[3],  victim->gold[3],

        victim->exp  , victim->pkills, victim->kills, victim->died);
    send_to_char( buf, ch );

    sprintf(buf,"Armor: %d  ", GET_AC(victim) );
    send_to_char(buf,ch);

    sprintf( buf, "Hit: %d  Dam: %d  Saves: %d  Position: %d  Wimpy: %d\n\r",
	GET_HITROLL(victim), GET_DAMROLL(victim), victim->saving_throw,
	victim->position,    victim->wimpy );
    send_to_char( buf, ch );

    if (IS_NPC(victim))
    {
	sprintf(buf, "Damage: %dd%d  Message:  %s\n\r",
	    victim->damage[DICE_NUMBER],victim->damage[DICE_TYPE],
	    attack_table[victim->dam_type].name);
	send_to_char(buf,ch);
    }
    sprintf( buf, "Fighting: %s Bank: %ld Home: %ld  Home World: %s\n\r",
	victim->fighting ? victim->fighting->name : "(none)" ,
        victim->bank,victim->home,world_name[ race_table[ victim->race ][ victim->sex ].world ]);
    send_to_char( buf, ch );
    
    if ( !IS_NPC(victim) )
    {
	sprintf( buf,
	    "Thirst: %d  Full: %d  Drunk: %d\n\r",
	    victim->pcdata->condition[COND_THIRST],
	    victim->pcdata->condition[COND_FULL],
	    victim->pcdata->condition[COND_DRUNK] );
	send_to_char( buf, ch );
    }

    sprintf( buf, "Carry number: %d  Carry weight: %ld  Size: %d\n\r",
	victim->carry_number, victim->carry_weight, victim->size );
    send_to_char( buf, ch );


    if (!IS_NPC(victim))
    {
    	sprintf( buf, 
	    "Age: %d  Played: %ld  Last Level: %ld  Timer: %d\n\r",
	    get_age(victim), 
	    (long) (victim->played + current_time - victim->logon) / 3600, 
	    victim->pcdata->last_level, 
	    victim->timer );
    	send_to_char( buf, ch );
    }

    if ( victim->pcdata != NULL && victim->pcdata->debt != NULL )
    {
	ROOM_INDEX_DATA *in_room;
	long time;

	time = ( victim->pcdata->debt->due - victim->played + victim->logon - current_time ) / 300;
	in_room = get_room_index( victim->pcdata->debt->vnum );

	sprintf( buf, "Debt : %ld %s due in %ld minutes at %s\n\r",
	    victim->pcdata->debt->amount,
	    coin_names[ victim->pcdata->debt->type ],
	    time,
	    ( in_room == NULL ? "Any loan shark" : in_room->name ) );
	send_to_char( buf, ch );
    }

    if(IS_NPC(victim) )
    sprintf(buf, "Act: %s\n\r",act_bit_name(victim->act));
    else
    sprintf(buf, "Plr: %s\n\r",act_bit_name(victim->plr));
    send_to_char(buf,ch);
    if(IS_NPC(victim) )
    {
    sprintf(buf, "%s\n\r",act2_bit_name(victim->act2));
    send_to_char(buf,ch);
    }

    if (victim->comm)
    {
    	sprintf(buf,"Comm: %s\n\r",comm_bit_name(victim->comm));
    	send_to_char(buf,ch);
    }

    if (IS_NPC(victim) && victim->off_flags)
    {
    	sprintf(buf, "Offense: %s\n\r",off_bit_name(victim->off_flags));
	send_to_char(buf,ch);
    }

    if (victim->imm_flags)
    {
	sprintf(buf, "Immune: %s\n\r",imm_bit_name(victim->imm_flags));
	send_to_char(buf,ch);
    }
 
    if (victim->res_flags)
    {
	sprintf(buf, "Resist: %s\n\r", imm_bit_name(victim->res_flags));
	send_to_char(buf,ch);
    }

    if (victim->vuln_flags)
    {
	sprintf(buf, "Vulnerable: %s\n\r", imm_bit_name(victim->vuln_flags));
	send_to_char(buf,ch);
    }

    if (victim->affected_by)
    {
	sprintf(buf, "Affected by %s\n\r", 
	    affect_bit_name(victim->affected_by));
	send_to_char(buf,ch);
    }
    if (victim->affected_by_2)
    {
	sprintf(buf, "Affected by %s\n\r", 
	    affect_bit_name_2(victim->affected_by_2));
	send_to_char(buf,ch);
    }

    sprintf( buf, "Master: %s  Leader: %s  Mount: %s  Pet: %s  ",
	victim->master      ? victim->master->name   : "(none)",
	victim->leader      ? victim->leader->name   : "(none)",
	victim->mount       ? victim->mount->name   : "(none)",
	victim->pet 	    ? victim->pet->name	     : "(none)");
    if ( victim->dislikes[0] != '\0' )
    {
	sprintf( buf1, "Dislikes: %s ", victim->dislikes );
	strcat( buf, buf1 );
    }
    if ( victim->mate[0] != '\0' )
    {
	sprintf( buf1, "Mate: %s ", victim->mate );
	strcat( buf, buf1 );
    }
    if ( victim->hunting[0] != '\0' )
    {
	sprintf( buf1, "Hunting: %s ", victim->hunting );
	strcat( buf, buf1 );
    }
    strcat( buf, "\n\r" );
    send_to_char( buf, ch );

    sprintf( buf, "Short description: %s\n\rLong  description: %s",
	victim->short_descr,
	victim->long_descr[0] != '\0' ? victim->long_descr : "(none)\n\r" );
    send_to_char( buf, ch );

    if ( IS_NPC( victim ) )
    {
	sprintf( buf, "Follower: %ld\n\r", victim->pIndexData->follower);
	send_to_char( buf, ch );
    }
    if ( !IS_NPC( victim ) )
    {
	sprintf( buf, "Corpse: %s\n\r", victim->pcdata->corpse != NULL? victim->pcdata->corpse->short_descr : "None");
	send_to_char( buf, ch );
    }

    if ( IS_NPC(victim) && victim->spec_fun != 0 )
	send_to_char( "Mobile has special procedure.\n\r", ch );

    for ( paf = victim->affected; paf != NULL; paf = paf->next )
    {
	if ( paf->bitvector != 0 )
	{
	sprintf( buf,
	    "Spell: '%s' modifies %s by %d for %d hours with bits %s, level %d.\n\r",
	    skill_table[(int) paf->type].name,
	    affect_loc_name( paf ),
	    paf->modifier,
	    paf->duration,
	    get_bit_name( paf->bit_type),
	    paf->level
	    );
	send_to_char( buf, ch );
	}
    }

    return;
}

/* ofind and mfind replaced with vnum, vnum skill also added */

void do_vnum(CHAR_DATA *ch, char *argument)
{
    char arg[MAX_INPUT_LENGTH];
    char *string;

    string = one_argument(argument,arg);
 
    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  vnum obj <name>\n\r",ch);
	send_to_char("  vnum obj area\n\r",ch);
	send_to_char("  vnum obj area_stat\n\r",ch);
	send_to_char("  vnum mob <name>\n\r",ch);
	send_to_char("  vnum mob area\n\r",ch);
	send_to_char("  vnum mob area_stat\n\r",ch);
	send_to_char("  vnum help <keyword>\n\r",ch);
	send_to_char("  vnum skill <skill or spell>\n\r",ch);
	send_to_char("  vnum room <room name>\n\r",ch);
	send_to_char("  vnum room all\n\r",ch);
	send_to_char("	vnum room area <area number (optional)>\n\r",ch);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_ofind(ch,string);
 	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    { 
	do_mfind(ch,string);
	return;
    }

    if (!str_cmp(arg,"help") || !str_cmp(arg,"helps"))
    { 
	do_hfind(ch,string);
	return;
    }
    if (!str_cmp(arg,"skill") || !str_cmp(arg,"spell"))
    {
	do_slookup(ch,string);
	return;
    }
    if (!str_cmp(arg,"room"))
    {
	do_rfind(ch,string);
	return;
    }
    send_to_char( "Vnum what? obj, mob, room...\n\r", ch );
    /* do both  */
 /*   do_mfind(ch,arg);
    do_ofind(ch,arg); */
}
void do_fixexit( CHAR_DATA *ch, char *argument )
{
    AREA_DATA *pArea;
    ROOM_INDEX_DATA *pRoom;
    int iHash, door;

    for ( pArea = area_first; pArea != NULL ; pArea = pArea->next )
	for(iHash=0; iHash<MAX_KEY_HASH; iHash++)
	    for( pRoom= room_index_hash[iHash]; pRoom!= NULL; pRoom= pRoom->next)
		for ( door = 0; door < 6; door++ )
		    if (pRoom->exit[door] != NULL )
			if ( strlen(pRoom->exit[door]->keyword) < 2 )
			    REMOVE_BIT(pRoom->exit[door]->exit_info_default, EX_ISDOOR);
	
}
void do_areastat ( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    MOB_INDEX_DATA *pMobIndex;
    ROOM_INDEX_DATA *pRoomIndex;
    OBJ_INDEX_DATA *pObjIndex;
    AREA_DATA *pArea;
    int door;
    int iHash, objs, mobs, rooms, t_objs, t_mobs, t_rooms, t_areas;
    bool fAll;
    bool fFull;
    bool fWorld;
    output = new_buf();
    argument = one_argument( argument, arg );
    fAll  = !str_cmp( arg, "all" );
    fFull  = !str_cmp( arg, "full" );
    fWorld  = !str_cmp( arg, "world" );

    t_mobs  = 0;   
    t_rooms = 0;   
    t_objs  = 0;   
    t_areas = 0;   
    for ( pArea = area_first; pArea != NULL; pArea = pArea->next )
    {
    if (!fAll && !fWorld)
    pArea = ch->in_room->area;
    if (fWorld)
	if ( pArea->world != ch->in_room->area->world ) 
	    continue;
    mobs  = 0;   
    rooms = 0;   
    objs  = 0;   
    for(iHash=0; iHash<MAX_KEY_HASH; iHash++)
	for (pMobIndex = mob_index_hash[iHash]; pMobIndex != NULL; pMobIndex = pMobIndex->next)
	    if ( pMobIndex->load_with_area == pArea )
		mobs++;

    for(iHash=0; iHash<MAX_KEY_HASH; iHash++)
	for( pRoomIndex = room_index_hash[iHash]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next)
	{
	    if ( pRoomIndex->area == pArea )
		rooms++;
	    if ( fFull )
		if ( pRoomIndex->area == pArea )
		    for ( door = 0; door < 6; door++ )
			if ( pRoomIndex->exit[door] )
			    if ( pRoomIndex->exit[door]->u1.to_room->area != pArea )
			    {
				sprintf(buf,"Connecting   to: %12s :  Room->[%5ld] TO->[%5ld]\n\r",
				pRoomIndex->exit[door]->u1.to_room->area->filename, pRoomIndex->vnum,
				pRoomIndex->exit[door]->u1.to_room->vnum );
				add_buf(output, buf);
			    } 
	    if ( fFull )
		if ( pRoomIndex->area != pArea )
		    for ( door = 0; door < 6; door++ )
			if ( pRoomIndex->exit[door] )
			    if ( pRoomIndex->exit[door]->u1.to_room->area == pArea )
			    {
				sprintf(buf,"Connecting from: %12s :  Room->[%5ld] TO->[%5ld]\n\r",
				pRoomIndex->area->filename, pRoomIndex->vnum,
				pRoomIndex->exit[door]->u1.to_room->vnum );
				add_buf(output, buf);
			    } 
	}
    for(iHash=0; iHash<MAX_KEY_HASH; iHash++)
	for (pObjIndex = obj_index_hash[iHash]; pObjIndex != NULL; pObjIndex = pObjIndex->next)
	    if ( pObjIndex->in_area == pArea )
		objs++;

   t_mobs += mobs;
   t_objs += objs;
   t_rooms += rooms;
   t_areas ++;
   sprintf( buf, "Areanum  %4d  Mobs  %4d  ",pArea->areanum, mobs); add_buf(output, buf);
   sprintf( buf, "Objs  %4d  ", objs); add_buf(output, buf);
   sprintf( buf, "Rooms  %5d  ", rooms); add_buf( output, buf );
   sprintf( buf, "Area  %12s\n\r", pArea->filename); add_buf( output, buf );
   if (!fAll && !fWorld)
   break;
   }
   sprintf( buf, "Totals   %4d  Mobs  %4d  ", t_areas, t_mobs); add_buf(output, buf);
   sprintf( buf, "Objs  %4d  ", t_objs); add_buf( output, buf );
   sprintf( buf, "Rooms  %5d\n\r", t_rooms); add_buf( output, buf );
   page_to_char(buf_string(output),ch);
   free_buf(output);
}

void do_mfind( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];    
    MOB_INDEX_DATA *pMobIndex;
    long vnum;
    int nMatch;
    bool found;
    bool fArea;
    bool fArea_stat;
    sh_int fArean;

    argument = one_argument( argument, arg );
    one_argument (argument,arg2);
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find whom?\n\r", ch );
	return;
    }

    found	= FALSE;
    nMatch	= 0;
    fArea 	= !str_cmp( arg, "area"); 
    fArea_stat 	= !str_cmp( arg, "area_stat"); 
    if (fArea_stat)
	fArea = TRUE;
    if (fArea) 
	fArean = ch->in_room->area->areanum;
    else
	fArean = 32760;
    if (!fArea && arg2[0] != '\0')
      strcat (arg,arg2);
   output = new_buf();
    for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
    {
      for ( pMobIndex = mob_index_hash[vnum]; pMobIndex != NULL; pMobIndex = pMobIndex->next )
	{
	    nMatch++;
	    if ( (fArea ? (pMobIndex->load_with_area->areanum == fArean) :
	     ( is_name( arg, pMobIndex->player_name ) ) ) )
	    {
		found = TRUE;
		if (!fArea_stat)
		{
		sprintf( buf, "`w[%5ld] %45s `wlv [%d]\n\r",
		    pMobIndex->vnum, pMobIndex->short_descr,pMobIndex->level );
		add_buf(output,buf);
		}
		else
		{
		sprintf( buf, "`w[%5ld] %25s `wlv [%2d], %20s\n\r",
		    pMobIndex->vnum, pMobIndex->short_descr,pMobIndex->level,act_bit_name(pMobIndex->act) );
		add_buf(output,buf);
		}	
	    }
	}
    }

    if ( !found )
	{
	send_to_char( "No mobiles by that name.\n\r", ch );
        free_buf(output);
	return;
	}

    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}


void do_hfind( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];    
    HELP_DATA *pHelp;
    bool found = FALSE;

    argument = one_argument( argument, arg );
    one_argument (argument,arg2);
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find what help?\n\r", ch );
	return;
    }


      strcat (arg,arg2);


	for ( pHelp = help_first; pHelp != NULL; pHelp = pHelp->next)
	{
	    if ( is_part_name(arg, pHelp->keyword) ) 
	    {
		found = TRUE;
		sprintf( buf, "`w[%5d] %45s `wlv[%d]\n\r",
		    pHelp->vnum, pHelp->keyword,pHelp->level );
		send_to_char( buf, ch );
	    }
	}
    

    if ( !found )
	send_to_char( "No helps found by that keyword.\n\r", ch );

    return;
}

void do_rfind( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex; 
    long vnum;
    int count = 0;
    bool fArea = FALSE;
    bool found, fAll = FALSE;
    AREA_DATA * pArea = ch->in_room->area;

    if ( argument[0] == '\0' )
    {
	send_to_char("Find which room?\n\r", ch );
	send_to_char("Syntax: vnum room <room name>\n\r",ch);
	send_to_char("  vnum room all\n\r",ch);
	send_to_char("	vnum room area <area number (optional)>\n\r",ch);
	send_to_char("	vnum room area <area number (optional)> <string>\n\r",ch);

	return;
    }

    one_argument( argument, arg );

    fAll = !str_cmp( arg, "all" ); 

    if ( !str_cmp( arg, "area" ) )
    {
	argument = one_argument( argument, arg );
	if ( is_number( argument ) )
	{	
	    int area;

	    area = atoi( argument );
	    if ( ( pArea = get_area_data( area ) ) == NULL )
	    {
		send_to_char( "Invalid area number.\n\r", ch );
		return;
	    }
	}
	fArea = TRUE;
    }

    found	= FALSE;

    output = new_buf();

    for ( vnum = 0; vnum < MAX_KEY_HASH; vnum++ )
    {
        for ( pRoomIndex = room_index_hash[vnum]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
	{
	    if ( fAll || fArea )
	    {
	        if ( pRoomIndex->area != pArea )
		    continue;

		if ( fAll ) 
		{
		    found = TRUE;
		    if ( count > 400 )
			break;
		    count++;
		    sprintf( buf, "[%5ld] %40s  [%3d]\n\r",
		    pRoomIndex->vnum, pRoomIndex->name, pRoomIndex->area->areanum);
		    add_buf(output,buf);
	        }
		else if (strstr( pRoomIndex->name, argument) != '\0')
		{
		    found = TRUE;
		    if ( count > 400 )
			break;
		    count++;
		    sprintf( buf, "[%5ld] %40s  [%3d]\n\r",
		    pRoomIndex->vnum, pRoomIndex->name, pRoomIndex->area->areanum);
		    add_buf(output,buf);
		}
	    }
	    else if (strstr( pRoomIndex->name, argument) != '\0')
	    {
		found = TRUE;
		if ( count > 400 )
		break;
		count++;
		sprintf( buf, "[%5ld] %40s  [%3d]\n\r",
		    pRoomIndex->vnum, pRoomIndex->name, pRoomIndex->area->areanum);
		add_buf(output,buf);
	    }
	}
    }

    if ( !found )
	send_to_char( "No rooms by that name.\n\r", ch );

    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}




void do_ofind( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    char arg2[MAX_INPUT_LENGTH];    
    char arg[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    long vnum;
    bool found;
    bool fArea;
    bool fArea_stat;
    sh_int fArean;

    argument = one_argument( argument, arg );
    one_argument (argument,arg2);
    if ( arg[0] == '\0' )
    {
	send_to_char( "Find what?\n\r", ch );
	return;
    }
    found	= FALSE;
    fArea 	= !str_cmp( arg, "area"); 
    fArea_stat 	= !str_cmp( arg, "area_stat"); 

    if (fArea_stat)
	fArea = TRUE;
    if (fArea) 
	fArean = ch->in_room->area->areanum;
    else
	fArean = 32760;
    if (!fArea && arg2[0] != '\0')
      strcat (arg,arg2);

    output = new_buf();
    /*
     * Yeah, so iterating over all vnum's takes 10,000 loops.
     * Get_obj_index is fast, and I don't feel like threading another link.
     * Do you?
     * -- Furey
     */
    for ( vnum = 0; vnum < MAX_KEY_HASH;  vnum++ )
    {
       for ( pObjIndex = obj_index_hash[vnum]; pObjIndex != NULL; pObjIndex = pObjIndex->next )
	{
	    if ( (fArea ? (pObjIndex->in_area->areanum == fArean) :
	    ( is_name( arg, pObjIndex->name ) ) ) )
	    {
		found = TRUE;
		if(!fArea_stat)
		{
		sprintf( buf, "`W[%5ld] %45s\n\r",
		    pObjIndex->vnum, pObjIndex->short_descr );
		add_buf(output,buf);
		}
		else
		{
		sprintf( buf, "`W[%5ld] %20s\n\r",
		    pObjIndex->vnum, pObjIndex->short_descr );
		add_buf(output,buf);
		}
	    }
	}
    }

    if ( !found )
	{
	send_to_char( "No objects by that name.\n\r", ch );
        free_buf(output);
	return;
	}

    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}



void do_mwhere( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    BUFFER *output;
    CHAR_DATA *victim;
    bool found;

    if ( argument[0] == '\0' )
    {
	send_to_char( "Mwhere whom?\n\r", ch );
	return;
    }

    found = FALSE;
    output = new_buf();
    for ( victim = char_list; victim != NULL; victim = victim->next )
    {
	if   (victim->in_room != NULL
	&&   is_name( argument, victim->name ) 
	&&   can_see( ch, victim) )
	{
	    found = TRUE;
	    sprintf( buf, "`w[%5ld] %-28s [%5ld] %s\n\r",
	( IS_NPC( victim ) ? victim->pIndexData->vnum : 0 ),
	( IS_NPC( victim ) ? victim->short_descr : victim->name ),
		victim->in_room->vnum,
		victim->in_room->name );
	    add_buf(output,buf);
	}
    }

    if ( !found )
	act( "You didn't find any $T.", ch, NULL, argument, TO_CHAR );
    else
    page_to_char(buf_string(output),ch);

    free_buf(output);
    return;
}



void do_reboo( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to REBOOT, spell it out.\n\r", ch );
    return;
}

void do_reboot( CHAR_DATA *ch, char *argument )
{
    sh_int timer;

    if ( *argument == '\0' )
    {
	send_to_char( "Syntac: Reboot <pause/continue/stop> <time>\n\r", ch );
	send_to_char( "any time <= 5 is in minutes\n\r", ch );
	return;
    }
    else if ( is_number( argument ) && ( timer = atoi( argument ) ) >= 0 )
    {
	if ( timer <= 5 )
	    timer *= 60;
	reboot_timer = timer + ( -1 * timer ) % 5 + 5;
	reboot_pause = FALSE;
	send_to_char( "Reboot timer set.\n\r", ch );
	return;
    }
    if ( reboot_timer <= 0 )
    {
	send_to_char( "No reboot in progress.\n\r", ch );
	return;
    }

    if ( !str_cmp( argument, "pause" ) )
    {
	reboot_pause = TRUE;
	send_to_char( "Reboot paused.\n\r", ch );
	return;
    }
    else if ( !str_cmp( argument, "continue" ) )
    {
	reboot_pause = FALSE;
	send_to_char( "Reboot continued.\n\r", ch );
	return;
    }	
    else if ( !str_cmp( argument, "stop" ) )
    {
	reboot_pause = FALSE;
	reboot_timer = 0;
	send_to_char( "Reboot stopped.\n\r", ch );
	return;
    }	

}

void reboot( void )
{
    CHAR_DATA *vch;
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;

    for ( vch = char_list; vch != NULL; vch = vch->next)
    {
	if (!IS_NPC(vch) )
	{
	    do_save( vch, "" );
	    send_to_char( "REBOOT!!!\n\r", vch );
	    if (LOGON_ON)
		if (vch->logon_data != NULL)
		    strncpy( vch->logon_data->exit, "Reboot", 30 );
	}
    }

    if (LOGON_ON)
        save_logon();

    merc_down = TRUE;

    for ( d = descriptor_list; d != NULL; d = d_next )
    {
	d_next = d->next;
    	close_socket(d);
    }
    
    return;
}



void do_shutdow( CHAR_DATA *ch, char *argument )
{
    send_to_char( "If you want to SHUTDOWN, spell it out.\n\r", ch );
    return;
}



void do_shutdown( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *vch;
    char buf[MAX_STRING_LENGTH];
    extern bool merc_down;
    DESCRIPTOR_DATA *d,*d_next;
    buf[0] = '\0';
    if (!IS_SET(ch->plr,PLR_WIZINVIS))
    {
    	sprintf( buf, "Shutdown by %s.", ch->name );
    	append_file( ch, SHUTDOWN_FILE, buf );
    }
    else
    {
    	sprintf( buf, "Electric Dreams is going down now!");
    	append_file( ch, SHUTDOWN_FILE, buf );
    }
    strcat( buf, "\n\r" );
    do_echo( ch, buf );
    do_force ( ch, "all save");
    do_save (ch, "");
    if (LOGON_ON)
    for ( vch = char_list; vch != NULL; vch = vch->next)
	{
	if (!IS_NPC(vch) )
	if (vch->logon_data != NULL)
	    strncpy( vch->logon_data->exit, "Shutdown", 30 );
	}
    if (LOGON_ON)
    save_logon();
    merc_down = TRUE;
    for ( d = descriptor_list; d != NULL; d = d_next)
    {
	d_next = d->next;
	close_socket(d);
    }
    return;
}

bool check_snoop_by( CHAR_DATA * ch, CHAR_DATA * victim )
{
    SNOOP_LIST * snoop_list;

    for ( snoop_list = ch->desc->snoop_by; snoop_list != NULL; snoop_list = snoop_list->next )
    {
        if ( snoop_list->desc->character == victim || snoop_list->desc->original == victim )
	     return TRUE;

	if ( check_snoop_by( snoop_list->desc->character, victim ) )
	     return TRUE;

    }

    return FALSE;
}

void do_snoop( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    SNOOP_LIST * snoop_list_next;
    SNOOP_LIST * snoop_list;
    DESCRIPTOR_DATA *d;
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Snoop whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim->desc == NULL )
    {
	send_to_char( "No descriptor to snoop.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Cancelling all snoops.\n\r", ch );
	for ( d = descriptor_list; d != NULL; d = d->next )
	{
	    while( d->snoop_by != NULL && d->snoop_by->desc == ch->desc )
	    {
		snoop_list = d->snoop_by;
		d->snoop_by = d->snoop_by->next;
		free_snoop_list( snoop_list );
	    }
		
	    for ( snoop_list = d->snoop_by ; snoop_list != NULL ; snoop_list = snoop_list_next )
	    {
		snoop_list_next = snoop_list->next;

		while( snoop_list_next != NULL && snoop_list_next->desc == ch->desc )
		{
		    snoop_list->next = snoop_list_next->next;
		    free_snoop_list( snoop_list_next );
		    snoop_list_next = snoop_list->next;
		}
	    }
	}
        sprintf(buf, "%s cancells snoop ",ch->name);
        info_channel ( ch, buf, INFOACT_SNOOPS);
	return;
    }

    if(!check_immortal_order(ch, victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( ch->desc != NULL && check_snoop_by( ch, victim ) )
    {
	send_to_char( "No snoop loops.\n\r", ch );
	return;
    }

    if ( victim->desc->snoop_by != NULL && victim->desc->snoop_by->desc == ch->desc )
    {
	snoop_list = victim->desc->snoop_by;
	victim->desc->snoop_by = snoop_list->next;
	free_snoop_list( snoop_list );
	sprintf( buf, "%s stops snooping %s", ch->name, victim->name );
	info_channel( ch, buf, INFOACT_SNOOPS );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    for ( snoop_list = victim->desc->snoop_by ; snoop_list != NULL ; snoop_list = snoop_list_next )
    {
	snoop_list_next = snoop_list->next;

	if ( snoop_list_next != NULL && snoop_list_next->desc == ch->desc )
	{
	    snoop_list->next = snoop_list_next->next;
	    free_snoop_list( snoop_list_next );
	    sprintf( buf, "%s stops snooping %s", ch->name, victim->name );
 	    info_channel( ch, buf, INFOACT_SNOOPS );
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    snoop_list = snoop_list_alloc();
    snoop_list->desc = ch->desc;
    snoop_list->next = victim->desc->snoop_by;
    victim->desc->snoop_by = snoop_list;

    sprintf(buf, "%s snoops %s",ch->name,victim->name);
    info_channel ( ch, buf, INFOACT_SNOOPS);
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_switch( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );
    
    if ( arg[0] == '\0' )
    {
	send_to_char( "Switch into whom?\n\r", ch );
	return;
    }

    if ( ch->desc == NULL )
	return;
    
    if ( ch->desc->original != NULL )
    {
	send_to_char( "You are already switched.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if (!IS_NPC(victim))
    {
	send_to_char("You can only switch into mobiles.\n\r",ch);
	return;
    }

    if ( victim->desc != NULL )
    {
	send_to_char( "Character in use.\n\r", ch );
	return;
    }

    ch->desc->character = victim;
    ch->desc->original  = ch;
    victim->desc        = ch->desc;
    ch->desc            = NULL;
    /* change communications to match */
    victim->comm = ch->comm;
    victim->lines = ch->lines;
    send_to_char( "Ok.\n\r", victim );
    return;
}

void do_return( CHAR_DATA *ch, char *argument )
{
    if ( ch->desc == NULL )
	return;

    if ( ch->desc->original == NULL )
    {
	send_to_char( "You aren't switched.\n\r", ch );
	return;
    }

    send_to_char( "You return to your original body.\n\r", ch );
    ch->desc->character       = ch->desc->original;
    ch->desc->original        = NULL;
    ch->desc->character->desc = ch->desc; 
    ch->desc                  = NULL;
    return;
}

bool obj_check (CHAR_DATA *ch, OBJ_DATA *obj)
{
    return TRUE;
}

/* for clone, to insure that cloning goes many levels deep */
void recursive_clone(CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone)
{
    OBJ_DATA *c_obj, *t_obj;

    if ( obj->cont_data != NULL )
    for (c_obj = obj->cont_data->contents; c_obj != NULL; c_obj = c_obj->next_content)
    {
	if (obj_check(ch,c_obj))
	{
	    t_obj = create_object(c_obj->pIndexData);
	    clone_object(c_obj,t_obj);
	    obj_to_obj(t_obj,clone);
	    recursive_clone(ch,c_obj,t_obj);
	}
    }
}

/* command that is similar to load  */
void do_clone(CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char *rest;
    CHAR_DATA *mob;
    OBJ_DATA  *obj;

    rest = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Clone what?\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	mob = NULL;
	obj = get_obj_here(ch,rest);
	if (obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	obj = NULL;
	mob = get_char_room(ch,rest);
	if (mob == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }
    else /* find both */
    {
	mob = get_char_room(ch,argument);
	obj = get_obj_here(ch,argument);
	if (mob == NULL && obj == NULL)
	{
	    send_to_char("You don't see that here.\n\r",ch);
	    return;
	}
    }

    /* clone an object */
    if (obj != NULL)
    {
	OBJ_DATA *clone;

	if (!obj_check(ch,obj))
	{
	    send_to_char(
		"Your powers are not great enough for such a task.\n\r",ch);
	    return;
	}

	clone = create_object(obj->pIndexData); 
	clone_object(obj,clone);
	if (obj->carried_by != NULL)
	    obj_to_char(clone,ch);
	else
	    obj_to_room(clone,ch->in_room);
 	recursive_clone(ch,obj,clone);

	act("$n has created $p.",ch,clone,NULL,TO_ROOM);
	act("You clone $p.",ch,clone,NULL,TO_CHAR);
	return;
    }
    else if (mob != NULL)
    {
	CHAR_DATA *clone;
	OBJ_DATA *new_obj;

	if (!IS_NPC(mob))
	{
	    send_to_char("You can only clone mobiles.\n\r",ch);
	    return;
	}


	clone = create_mobile(mob->pIndexData);
	clone_mobile(mob,clone); 
	
	for (obj = mob->carrying; obj != NULL; obj = obj->next_content)
	{
	    if (obj_check(ch,obj))
	    {
		new_obj = create_object(obj->pIndexData);
		clone_object(obj,new_obj);
		recursive_clone(ch,obj,new_obj);
		obj_to_char(new_obj,clone);
		if ( new_obj->wear_data != NULL && obj->wear_data != NULL )
		    new_obj->wear_data->wear_loc = obj->wear_data->wear_loc;
	    }
	}
	char_to_room(clone,ch->in_room);
        act("$n has created $N.",ch,NULL,clone,TO_ROOM);
        act("You clone $N.",ch,NULL,clone,TO_CHAR);
        return;
    }
}

/* RT to replace the two load commands */

void do_load(CHAR_DATA *ch, char *argument )
{
   char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  load mob <vnum>\n\r",ch);
	send_to_char("  load obj <vnum> <level>\n\r",ch);
	return;
    }

    if ( !str_prefix( arg,"nightmare" ) )
    {
	int level;

	if ( !is_number( argument ) || ( level = atoi( argument ) ) < 1 )
	{
	    send_to_char( "Must set a level higher then 1.\n\r", ch );
	    return;
	}

	create_nightmare_mob( ch, level );
	send_to_char( "You are now hunted by a nightmare.\n\r", ch );
	return;
    }

    if (!str_cmp(arg,"mob") || !str_cmp(arg,"char"))
    {
	do_mload(ch,argument);
	return;
    }

    if (!str_cmp(arg,"obj"))
    {
	do_oload(ch,argument);
	return;
    }
    /* echo syntax */
    do_load(ch,"");
}


void do_mload( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    MOB_INDEX_DATA *pMobIndex;
    CHAR_DATA *victim;
    
    one_argument( argument, arg );

    if ( arg[0] == '\0' || !is_number(arg) )
    {
	send_to_char( "Syntax: load mob <vnum>.\n\r", ch );
	return;
    }

    if ( ( pMobIndex = get_mob_index( atoi( arg ) ) ) == NULL )
    {
	send_to_char( "No mob has that vnum.\n\r", ch );
	return;
    }

    victim = create_mobile( pMobIndex );
    char_to_room( victim, ch->in_room );
    act( "$n has created $N!", ch, NULL, victim, TO_ROOM );
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_oload( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH] ,arg2[MAX_INPUT_LENGTH];
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_DATA *obj;
    int level;
    level = ch->level;
    
    argument = one_argument( argument, arg1 );
    one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || !is_number(arg1))
    {
	send_to_char( "Syntax: load obj <vnum> <level>.\n\r", ch );
	return;
    }
  
    if ( arg2[0] != '\0')  /* load with a level */
    {
	if (!is_number(arg2))
        {
	  send_to_char( "Syntax: oload <vnum> <level>.\n\r", ch );
	  return;
	}
        level = atoi(arg2);
    }

    if ( ( pObjIndex = get_obj_index( atoi( arg1 ) ) ) == NULL )
    {
	send_to_char( "No object has that vnum.\n\r", ch );
	return;
    }

    if ( ( obj = create_object( pObjIndex ) ) == NULL )
	{
	send_to_char("This object is in sorry shape.  Object not created.\n\r",ch);
	return;
	}
    if ( CAN_WEAR(obj, ITEM_TAKE) )
	{
	obj_to_char( obj, ch );
	strncpy(obj->owner ,ch->name,30);
	}
    else
	obj_to_room( obj, ch->in_room );
    act( "$n has created $p!", ch, obj, NULL, TO_ROOM );
    send_to_char( "Ok.\n\r", ch );
    return;
}


void do_purge_area( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *pRoomIndex;
    AREA_DATA *pArea;
    CHAR_DATA *vnext;
    OBJ_DATA  *obj_next;
    CHAR_DATA *victim;
    OBJ_DATA *obj;
    int iHash;
    bool fAll = FALSE;
    
    pArea = ch->in_room->area;

    one_argument(argument, arg);

    if (*arg != '\0')
    fAll = !str_cmp( arg, "all" );

    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for ( pRoomIndex  = room_index_hash[iHash];
              pRoomIndex != NULL;
              pRoomIndex  = pRoomIndex->next )
        {
	    if ( pRoomIndex->area == pArea || fAll) /* Just for this new area */
	    {
		for ( victim = pRoomIndex->people; victim != NULL; victim = vnext )
		{
		    vnext = victim->next_in_room;
		    if ( IS_NPC(victim) && !IS_SET(victim->act,ACT_NOPURGE) 
		    &&   victim != ch /* safety precaution */ )
			extract_char( victim, TRUE );
		}

		for ( obj = pRoomIndex->contents; obj != NULL; obj = obj_next )
		{
		    obj_next = obj->next_content;
		    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))
		      extract_obj( obj );
		}
	    }
	}
    }
    if ( fAll )
    send_to_char("All area's purged!\n\r",ch);
    else
    send_to_char("Area purged!\n\r",ch);
}

void do_purge( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char buf[100];
    CHAR_DATA *victim;
    OBJ_DATA *obj = NULL;
    DESCRIPTOR_DATA *d;
    bool victim_found = FALSE;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	/* 'purge' */
	CHAR_DATA *vnext;
	OBJ_DATA  *obj_next;

	for ( victim = ch->in_room->people; victim != NULL; victim = vnext )
	{
	    vnext = victim->next_in_room;
	    if ( IS_NPC(victim) && !IS_SET(victim->act,ACT_NOPURGE) 
	    &&   victim != ch /* safety precaution */ )
		extract_char( victim, TRUE );
	}

	for ( obj = ch->in_room->contents; obj != NULL; obj = obj_next )
	{
	    obj_next = obj->next_content;
	    if (!IS_OBJ_STAT(obj,ITEM_NOPURGE))
	      extract_obj( obj );
	}
	act( "$n purges the room!", ch, NULL, NULL, TO_ROOM);
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	send_to_char( "No mob by that name\n\r", ch );
    else
	victim_found = TRUE;

    if (!victim_found)
    if ( ( obj = get_obj_world(ch, arg ) )  == NULL )
    {
	send_to_char( "No objects by that name\n\r", ch );
	return;
    }

    if (victim_found)
    {
    if ( !IS_NPC(victim) )
    {

	if (ch == victim)
	{
	  send_to_char("Ho ho ho.\n\r",ch);
	  return;
	}

        if(!check_immortal_order(ch, victim) )
	{
	  send_to_char("Maybe that wasn't a good idea...\n\r",ch);
	  sprintf(buf,"%s tried to purge you!\n\r",ch->name);
	  send_to_char(buf,victim);
	  return;
	}

	act("$n disintegrates $N.",ch,0,victim,TO_NOTVICT);

    	if (victim->level > 1)
	    save_char_obj( victim );
    	d = victim->desc;
    	extract_char( victim, TRUE );
    	if ( d != NULL )
          close_socket( d );

	return;
    }

    act( "$n purges $N.", ch, NULL, victim, TO_NOTVICT );
    sprintf(buf, "%s is toast!\n\r",victim->short_descr);
    send_to_char(buf,ch);
    extract_char( victim, TRUE );
    return;
    }
 act( "$n purges $p.", ch, obj, NULL, TO_ROOM );
 sprintf(buf, "%s is toast!\n\r",obj->name);
 send_to_char(buf,ch);
 extract_obj( obj );
 return;
}



void do_advance( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int level;
    int iLevel;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || !is_number( arg2 ) )
    {
	send_to_char( "Syntax: advance <char> <level>.\n\r", ch );
	return;
    }

    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	send_to_char( "That player is not here.\n\r", ch);
	return;
    }

    if (IS_OPTIVISION(victim) )
    {
	send_to_char( "Not on other Imps.\n\r", ch);
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if ( ( level = atoi( arg2 ) ) < 1 || level > 60 )
    {
	send_to_char( "Level must be 1 to 60.\n\r", ch );
	return;
    }

    /*
     * Lower level:
     *   Reset to level 1.
     *   Then raise again.
     *   Currently, an imp can lower another imp.
     *   -- Swiftest
     */
    if ( level <= victim->level )
    {
        int temp_prac;
	int i;

	send_to_char( "Lowering a player's level!\n\r", ch );
	send_to_char( "**** OOOOHHHHHHHHHH  NNNNOOOO ****\n\r", victim );
	temp_prac = victim->practice;
	victim->level    = 1;
	victim->exp      = exp_table[victim->level].exp_at_level;
	victim->max_hit  = 10;
	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
	{
	    victim->max_mana[ i ] = race_table[ ch->race ][ ch->sex ].mana_die[ i ] * race_table[ ch->race ][ ch->sex ].mana_dice[ i ] + race_table[ ch->race ][ ch->sex ].mana_bonus[ i ];
	    victim->max_mana[ i ] *= 5;
	    victim->mana[ i ]     = victim->max_mana[ i ];
	    victim->pcdata->perm_mana[ i ] = victim->max_mana[ i ];
	}
	victim->max_move = 100;
	victim->practice = 0;
	victim->hit      = victim->max_hit;
	victim->move     = victim->max_move;
	victim->pcdata->perm_hit = victim->max_hit;
	victim->pcdata->perm_move = victim->max_move;
	victim->practice = temp_prac;
    }
    else
    {
	send_to_char( "Raising a player's level!\n\r", ch );
	send_to_char( "**** OOOOHHHHHHHHHH  YYYYEEEESSS ****\n\r", victim );
    }

    for ( iLevel = victim->level ; iLevel < level; iLevel++ )
    {
	send_to_char( "You raise a level!!  \n\r", victim );
	victim->level += 1;
    }
    victim->exp   = exp_table[victim->level].exp_at_level; 
    save_char_obj(victim);
    return;
}




void do_restore( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg1[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    DESCRIPTOR_DATA *d;
    AFFECT_DATA *af;
    AFFECT_DATA *af_next;
    int i;

    argument = one_argument( argument, arg );
    one_argument( argument, arg1 );
    if (arg[0] == '\0' || !str_cmp(arg,"room"))
    {
    /* cure room */
    	
        for (vch = ch->in_room->people; vch != NULL; vch = vch->next_in_room)
        {
            affect_strip(vch,gsn_plague);
            affect_strip(vch,gsn_poison);
            affect_strip(vch,gsn_blinding_light);
            affect_strip(vch,gsn_sand_storm);
            affect_strip(vch,gsn_sleep);
            affect_strip(vch,gsn_curse);

	    for ( af = vch->affected; af != NULL; af = af_next )
	    {
		af_next = af->next;
		if (af->type == -2 )
		  affect_remove( vch, af);
	    }
	/*
	    if (!IS_NPC(vch) )
	    {
            vch->affected_by = vch->pcdata->perm_aff; 
            vch->affected_by_2 = vch->pcdata->perm_aff_2; 
	    }
	*/
            vch->hit 	= vch->max_hit;
	    for( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
                vch->mana[i]	= vch->max_mana[i];
            vch->move	= vch->max_move;
            update_pos( vch);
            act("$n has restored you.",ch,NULL,vch,TO_VICT);
        

        }
        send_to_char("Room restored.\n\r",ch);
        return;

    }
    
    if ( IS_OPTIVISION(ch) && !str_cmp(arg,"all"))
    {
    /* cure all */
    	
        for (d = descriptor_list; d != NULL; d = d->next)
        {
	    victim = d->character;

	    if (victim == NULL || IS_NPC(victim))
		continue;
                
            affect_strip(victim,gsn_plague);
            affect_strip(victim,gsn_poison);
            affect_strip(victim,gsn_blinding_light);
            affect_strip(victim,gsn_sand_storm);
            affect_strip(victim,gsn_sleep);
            affect_strip(victim,gsn_curse);
	    for ( af = victim->affected; af != NULL; af = af_next )
	    {
		af_next = af->next;
		if (af->type == -2 )
		  affect_remove( victim, af);
	    }
	/*
	    if (!IS_NPC(victim ) )
	    {
            victim->affected_by = victim->pcdata->perm_aff; 
            victim->affected_by_2 = victim->pcdata->perm_aff_2; 
	    }
	*/
            
            victim->hit 	= victim->max_hit;
	    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
                victim->mana[i]	= victim->max_mana[i];
            victim->move	= victim->max_move;
            update_pos( victim);
	    if (victim->in_room != NULL)
                act("$n has restored you.",ch,NULL,victim,TO_VICT);
        }
	send_to_char("All active players restored.\n\r",ch);
	return;
	sprintf(imp_buf, "%s restored all active players\n\r",ch->name);
	info_channel(ch,imp_buf,INFOACT_IMP_LOG);
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    affect_strip(victim,gsn_plague);
    affect_strip(victim,gsn_poison);
    affect_strip(victim,gsn_blinding_light);
    affect_strip(victim,gsn_sand_storm);
    affect_strip(victim,gsn_sleep);
    affect_strip(victim,gsn_curse);
    for ( af = victim->affected; af != NULL; af = af_next )
    {
	af_next = af->next;
	if (af->type == -2 )
	  affect_remove( victim, af);
    }
/*
    if (!IS_NPC(victim ) )
    {
    victim->affected_by = victim->pcdata->perm_aff; 
    victim->affected_by_2 = victim->pcdata->perm_aff_2; 
    }
 */
    victim->hit  = victim->max_hit;
    for ( i  = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
        victim->mana[i] = victim->max_mana[i];
    victim->move = victim->max_move;
    update_pos( victim );
    act( "$n has restored you.", ch, NULL, victim, TO_VICT );
    send_to_char( "Ok.\n\r", ch );
    return;
}


void do_freeze( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Freeze whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if(!check_immortal_order(ch, victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->plr, PLR_FREEZE) )
    {
	REMOVE_BIT(victim->plr, PLR_FREEZE);
	send_to_char( "You can play again.\n\r", victim );
	send_to_char( "FREEZE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->plr, PLR_FREEZE);
	send_to_char( "You can't do ANYthing!\n\r", victim );
	send_to_char( "FREEZE set.\n\r", ch );
    }

    save_char_obj( victim );

    return;
}

void do_log( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Log whom?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	if ( fLogAll )
	{
	    fLogAll = FALSE;
	    send_to_char( "Log ALL off.\n\r", ch );
	}
	else
	{
	    fLogAll = TRUE;
	    send_to_char( "Log ALL on.\n\r", ch );
	}
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    /*
     * No level check, gods can log anyone.
     */
    if ( IS_SET(victim->plr, PLR_LOG) )
    {
	REMOVE_BIT(victim->plr, PLR_LOG);
	send_to_char( "LOG removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->plr, PLR_LOG);
	send_to_char( "LOG set.\n\r", ch );
    }

    return;
}



void do_noemote( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noemote whom?\n\r", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }


    if(!check_immortal_order(ch, victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOEMOTE) )
    {
	REMOVE_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can emote again.\n\r", victim );
	send_to_char( "NOEMOTE removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOEMOTE);
	send_to_char( "You can't emote!\n\r", victim );
	send_to_char( "NOEMOTE set.\n\r", ch );
    }

    return;
}



void do_noshout( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Noshout whom?\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    if(!check_immortal_order(ch, victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOSHOUT) )
    {
	REMOVE_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can shout again.\n\r", victim );
	send_to_char( "NOSHOUT removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOSHOUT);
	send_to_char( "You can't shout!\n\r", victim );
	send_to_char( "NOSHOUT set.\n\r", ch );
    }

    return;
}



void do_notell( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Notell whom?", ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( victim == ch )
    {
    if ( IS_SET(victim->comm, COMM_NO_REC_TELL) )
    {
	REMOVE_BIT(ch->comm, COMM_NO_REC_TELL);
	send_to_char( "You will now receive tells\n\r", victim );
    }
    else
    {
	SET_BIT(ch->comm, COMM_NO_REC_TELL);
	send_to_char( "You will not receive tells\n\r", victim );
    }
    return;
    }

    if(!check_immortal_order(ch, victim) )
    {
	send_to_char( "You failed.\n\r", ch );
	return;
    }

    if ( IS_SET(victim->comm, COMM_NOTELL) )
    {
	REMOVE_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can tell again.\n\r", victim );
	send_to_char( "NOTELL removed.\n\r", ch );
    }
    else
    {
	SET_BIT(victim->comm, COMM_NOTELL);
	send_to_char( "You can't tell!\n\r", victim );
	send_to_char( "NOTELL set.\n\r", ch );
    }

    return;
}



void do_peace( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA *rch;

    for ( rch = ch->in_room->people; rch != NULL; rch = rch->next_in_room )
    {
	if ( rch->fighting != NULL )
	    stop_fighting( rch, TRUE );
	if ( rch->dislikes[0] != '\0' )
	{
	rch->dislikes[0] = '\0';
	}
	if (IS_NPC(rch) && IS_SET(rch->act,ACT_AGGRESSIVE))
	    REMOVE_BIT(rch->act,ACT_AGGRESSIVE);
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}



BAN_DATA *		ban_free;
BAN_DATA *		ban_list;

void do_ban( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    BAN_DATA *pban;

    if ( ch != NULL )
    if ( IS_NPC(ch) )
	return;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	strcpy( buf, "Banned sites:\n\r" );
	for ( pban = ban_list; pban != NULL; pban = pban->next )
	{
	    strcat( buf, pban->name );
	    strcat( buf, "\n\r" );
	}
	send_to_char( buf, ch );
	return;
    }

    for ( pban = ban_list; pban != NULL; pban = pban->next )
    {
	if ( !str_cmp( arg, pban->name ) )
	{
	    if (ch != NULL )
	    send_to_char( "That site is already banned!\n\r", ch );
	    return;
	}
    }

    pban	= ban_data_alloc();

    strncpy( pban->name, arg, 30 );
    pban->next	= ban_list;
    ban_list	= pban;
    if (ch != NULL )
    send_to_char( "Ok.\n\r", ch );
    return;
}



void do_allow( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    BAN_DATA *prev;
    BAN_DATA *curr;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {
	send_to_char( "Remove which site from the ban list?\n\r", ch );
	return;
    }

    prev = NULL;
    for ( curr = ban_list; curr != NULL; prev = curr, curr = curr->next )
    {
	if ( !str_cmp( arg, curr->name ) )
	{
	    if ( prev == NULL )
		ban_list   = ban_list->next;
	    else
		prev->next = curr->next;

	    curr->next	= ban_free;
	    ban_free	= curr;
	    send_to_char( "Ok.\n\r", ch );
	    return;
	}
    }

    send_to_char( "Site is not banned.\n\r", ch );
    return;
}



void do_wizlock( CHAR_DATA *ch, char *argument )
{
    extern bool wizlock;
    wizlock = !wizlock;

    if ( wizlock )
	send_to_char( "Game wizlocked.\n\r", ch );
    else
	send_to_char( "Game un-wizlocked.\n\r", ch );

    return;
}

/* RT anti-newbie code */

void do_newlock( CHAR_DATA *ch, char *argument )
{
    extern bool newlock;
    newlock = !newlock;
 
    if ( newlock )
        send_to_char( "New characters have been locked out.\n\r", ch );
    else
        send_to_char( "Newlock removed.\n\r", ch );
 
    return;
}


void do_slookup( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int sn;

    one_argument( argument, arg );
    if ( arg[0] == '\0' )
    {
	send_to_char( "Lookup which skill or spell?\n\r", ch );
	return;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name == NULL )
		break;
	    sprintf( buf, "Sn: %3d  Skill/spell: '%s'\n\r",
		sn, skill_table[sn].name );
	    send_to_char( buf, ch );
	}
    }
    else
    {
	if ( ( sn = skill_lookup( arg ) ) < 0 )
	{
	    send_to_char( "No such skill or spell.\n\r", ch );
	    return;
	}

	sprintf( buf, "Sn: %3d  Skill/spell: '%s'\n\r",
	    sn, skill_table[sn].name );
	send_to_char( buf, ch );
    }

    return;
}
void do_settitle( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    FINGER_DATA * finger;

    if (!IS_OPTIVISION(ch) )
    {
	if (!IS_SET(ch->cmd_groups, CMD_GROUP_ADMIN ) )
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}
    }

    argument = one_argument(argument,arg);

    if (arg[0] == '\0' || argument[0] == '\0' ) 
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("settitle  <char name> <title>\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
    if (IS_NPC(victim) )
    {
	send_to_char( "Not on npc's\n\r", ch );
	return;
    }    

    if(!IS_IMMORTAL(victim) )
    {
	send_to_char( "They must be immortal first\n\r", ch );
	return;
    }    

    strncpy( victim->pcdata->imm_title, argument, 30 );

    for ( finger = finger_list ; finger != NULL ; finger = finger->next )
	if ( !str_cmp( finger->name, victim->name ) )
	    strncpy( finger->title, argument, 80 );

    send_to_char ("Immortal title set\n\r",ch);
    return;
}

void do_setgroup( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int s;

    buf[0] = '\0';

    if (!IS_OPTIVISION(ch) )
    {
	if (!IS_SET(ch->cmd_groups, CMD_GROUP_ADMIN ) )
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}
    }

    argument = one_argument(argument,arg);
    one_argument(argument,arg2);

    if (arg[0] == '\0' ) 
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  setgroup  <char name> <group name>\n\r",ch);
	send_to_char("Groups:\n\r",ch);
	sprintf(buf, cmd_group_type_name( ALL_FLAGS ) );
	strcpy( buf2, format_string( str_dup( buf ) ) );
	send_to_char( buf2, ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	
    if ( arg2[0] == '\0' ) 
    {
	sprintf(buf, "Current Groups: %s\n\r", cmd_group_type_name(victim->cmd_groups) );
	send_to_char(buf, ch);
	return;	
    }
    
    if ( ( s = get_cmd_group_type( arg2 ) ) == -1 ) 
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  setgroup  <char name> <group name>\n\r",ch);
	send_to_char("Groups:\n\r",ch);
	sprintf(buf, cmd_group_type_name( ALL_FLAGS ) );
	strcpy( buf2, format_string( str_dup( buf ) ) );
	send_to_char( buf2, ch );
	return;
    }
    else
    {
    TOGGLE_BIT(victim->cmd_groups, s);
    send_to_char ("Group toggled\n\r",ch);
    sprintf(buf, "Current Groups: %s\n\r", cmd_group_type_name(victim->cmd_groups) );
    send_to_char(buf, ch);
    }
    return;

}
void do_setguild( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    char buf2[MAX_STRING_LENGTH];
    CHAR_DATA *victim;
    int s;

    buf[0] = '\0';

    if (!IS_OPTIVISION(ch) )
    {
	if (!IS_SET(ch->cmd_groups, CMD_GROUP_ADMIN ) )
	{
	send_to_char("Huh?\n\r",ch);
	return;
	}
    }

    argument = one_argument(argument,arg);
    one_argument(argument,arg2);

    if (arg[0] == '\0' ) 
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  setguild  <char name> <guild name>\n\r",ch);
	send_to_char("Guilds:\n\r",ch);
	sprintf(buf, guild_bit_name( ALL_FLAGS ) );
	strcpy( buf2, format_string( str_dup( buf ) ) );
	send_to_char( buf2, ch );
	return;
    }

    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }
	
    if ( arg2[0] == '\0' ) 
    {
	sprintf(buf, "Current Guilds: %s\n\r", guild_bit_name(victim->guilds) );
	send_to_char(buf, ch);
	return;	
    }
    
    if ( ( s = guild_name_bit( arg2 ) ) == -1 ) 
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  setguild  <char name> <guild name>\n\r",ch);
	send_to_char("Guilds:\n\r",ch);
	sprintf(buf, guild_bit_name( ALL_FLAGS ) );
	strcpy( buf2, format_string( str_dup( buf ) ) );
	send_to_char( buf2, ch );
	return;
    }
    else
    {
    	TOGGLE_BIT(victim->guilds, s);
    	send_to_char ("Guild toggled\n\r",ch);
    	sprintf(buf, "Current Guilds: %s\n\r", guild_bit_name(victim->guilds) );
    	send_to_char(buf, ch);
    }

    return;
}

/* RT set replaces sset, mset, oset, and rset */
void do_set( CHAR_DATA *ch, char *argument )
{
    char arg[MAX_INPUT_LENGTH];

    argument = one_argument(argument,arg);

    if (arg[0] == '\0')
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set mob   <name> <field> <value>\n\r",ch);
	send_to_char("  set obj   <name> <field> <value>\n\r",ch);
	send_to_char("  set room  <room> <field> <value>\n\r",ch);
        send_to_char("  set skill <name> <spell or skill> <value>\n\r",ch);
	send_to_char("  set clan  <name> <field> <value>\n\r",ch);
	return;
    }

    if (!str_prefix(arg,"mobile") || !str_prefix(arg,"character"))
    {
	do_mset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"skill") || !str_prefix(arg,"spell"))
    {
	do_sset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"object"))
    {
	do_oset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"clan"))
    {
	do_cset(ch,argument);
	return;
    }

    if (!str_prefix(arg,"room"))
    {
	do_rset(ch,argument);
	return;
    }
    /* echo syntax */
    do_set(ch,"");
}


void do_sset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    int value;
    int sn;
    bool fAll;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set skill <name> <spell or skill> <value>\n\r", ch);
	send_to_char( "  set skill <name> all <value>\n\r",ch);  
	send_to_char("   (use the name of the skill, not the number)\n\r",ch);
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if ( IS_NPC(victim) )
    {
	send_to_char( "Not on NPC's.\n\r", ch );
	return;
    }

    fAll = !str_cmp( arg2, "all" );
    sn   = 0;
    if ( !fAll && ( sn = skill_lookup( arg2 ) ) < 0 )
    {
	send_to_char( "No such skill or spell.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }

    value = atoi( arg3 );
    if ( value < 0 || value > 100 )
    {
	send_to_char( "Value range is 0 to 100.\n\r", ch );
	return;
    }

    if ( fAll )
    {
	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL )
		victim->pcdata->learned[sn]	= value;
	}
    }
    else
    {
	victim->pcdata->learned[sn] = value;
    }

    return;
}

void do_cset( CHAR_DATA *ch, char *argument)
{
    char arg1[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    char arg3[MAX_INPUT_LENGTH];
    CLAN_DATA *clan;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    one_argument( argument, arg3 );
    if ( ( clan = clan_lookup( arg1 ) ) == NULL || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set clan <name> <field> <value>\n\r",ch);
	send_to_char("  Field being one of:\n\r",ch);
	send_to_char("    name title leader sponser flags treasury recall donate\n\r",ch);
	send_to_char("    rank1 rank2 rank3 rank4 rank5 rank6 rank7 rank8 rank9 rank10\n\r",ch);
	return;
    }

    if ( !str_cmp( arg2,"name" ) )
    {
	strncpy( clan->name ,argument, 30 );
        SET_BIT(changed_system, CHANGED_CLANS);
	return;
    }

    if ( !str_cmp( arg2,"flags" ) && !str_cmp( arg3,"unlisted") )
    {
	TOGGLE_BIT( clan->clan_info, CLAN_UNLISTED );
	send_to_char( "Unlisted flag toggled.\n\r",ch);
        SET_BIT(changed_system, CHANGED_CLANS);
	return;
    }

    if ( !str_cmp( arg2,"leader" ) )
    {
	if ( ( clan->leader = get_char_world( ch, arg3 ) ) == NULL )
        {
	    send_to_char("They are not on.\n\r",ch);
	    return;
	}
	strncpy( clan->leader_name, capitalize(arg3), 30 );
	clan->leader->pcdata->pcclan->clan = clan;
        SET_BIT(changed_system, CHANGED_CLANS);
	return;
    }

    if ( !str_cmp( arg2, "sponser" ) )
    {
	if ( ( clan->sponser = get_char_world( ch, arg3 ) ) == NULL )
        {
	    send_to_char("They are not on.\n\r",ch);
	    return;
	}
	strncpy( clan->sponser_name, capitalize(arg3), 30 );
	clan->sponser->pcdata->pcclan->clan = clan;
        SET_BIT(changed_system, CHANGED_CLANS);
	return;
    }

    if ( !str_cmp( arg2, "treasury" ) && is_number( arg3 ) )
    {
	if ( IS_IMMORTAL(ch) )
	{
	    send_to_char("You are not high enough to use this command.\n\r",ch);
	    return;
	}
	clan->treasury = atoi( arg3 );
        SET_BIT(changed_system, CHANGED_CLANS);
	return;
    }

    if ( !str_cmp( arg2, "recall" ) && is_number( arg3 ) )
    {
    	clan->recall_room = atoi( arg3 );
        SET_BIT(changed_system, CHANGED_CLANS);
	return;
    }

    if ( !str_cmp( arg2,"donate" ) && is_number( arg3 ) )
    {
	clan->donate_room = atoi( arg3 );
        SET_BIT(changed_system, CHANGED_CLANS);
	return;
    }

    if ( !str_cmp( arg2, "title" ) )
    {
    	strncpy( clan->title, argument, 80 );
        SET_BIT(changed_system, CHANGED_CLANS);
	return;    
    }

    if ( !str_cmp( arg2, "rank1" ) )
    {
       strncpy( clan->rank1, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank2" ) )
    {
       strncpy( clan->rank2, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank3" ) )
    {
       strncpy( clan->rank3, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank4" ) )
    {
       strncpy( clan->rank4, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank5" ) )
    {
       strncpy( clan->rank5, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank6" ) )
    {
       strncpy( clan->rank6, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank7" ) )
    {
       strncpy( clan->rank7, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank8" ) )
    {
       strncpy( clan->rank8, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank9" ) )
    {
       strncpy( clan->rank9, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }

    if ( !str_cmp( arg2, "rank10" ) )
    {
       strncpy( clan->rank10, argument, 30 );
       SET_BIT(changed_system, CHANGED_CLANS);
       return;
    }
    
    do_cset( ch, "" );

}


void do_mset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    char buf  [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    CHAR_DATA *vch;
    int value;
    int coin_type;
    int type;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    argument = one_argument( argument, arg3 );

    if ( arg1[0] == '\0' || arg2[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set char <name> <field> <value>\n\r",ch); 
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    str int wis dex con chr sex level\n\r",	ch );
	send_to_char( "    race coins hp move practice exp\n\r",	ch );
	send_to_char( "    fire water earth wind spirit\n\r", ch );
	send_to_char( "    thirst drunk full hitroll damroll played\n\r",		ch );
	send_to_char( "    follower mount ph direct god_who normal_who\n\r",		ch );
	send_to_char( "	   dtimer fastexp dreamstate clans vnums\n\r" , ch ); 
	return;
    }

    if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "They aren't here.\n\r", ch );
	return;
    }

    if(!check_immortal_order(ch, victim) )
    {
	send_to_char("Not on other Imps.\n\r", ch);
	return;
    }

    if ( !str_cmp( arg2, "birthday" ) )
    {
	victim->birth_day = time_info[ race_table[ victim->race ][ victim->sex ].world ].day + 1;
	victim->birth_month = time_info[ race_table[ victim->race ][ victim->sex ].world ].month;
	send_to_char( "New Birthday Set\n\r", ch );
	return;
    }

    if ( !str_cmp( arg2, "direct" ) )
    {
	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Invalid target.\n\r", ch );
	    return;
	}

	TOGGLE_BIT( victim->plr, PLR_DIRECT );
	send_to_char( "Direct flag toggled.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "fastexp" ) )
    {
	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Invalid target.\n\r", ch );
	    return;
	}

	TOGGLE_BIT( victim->plr2, PLR_FASTEXP );
	send_to_char( "Fastexp flag toggled.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "vnums" ) )
    {
	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Invalid target.\n\r", ch );
	    return;
	}

	TOGGLE_BIT( victim->plr2, PLR_VNUMS_ON );
	send_to_char( "Vnums flag toggled.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "dreamstate" ) )
    {
	if ( IS_NPC( victim ) )
	{
	TOGGLE_BIT( victim->act2, ACT2_DREAMSTATE );
	send_to_char( "Dreamstate flag toggled.\n\r", ch );
	return;
	}

	TOGGLE_BIT( victim->plr2, PLR_DREAMSTATE );
	send_to_char( "Dreamstate flag toggled.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "god_who" ) )
    {
	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Invalid target.\n\r", ch );
	    return;
	}

	TOGGLE_BIT( victim->plr2, PLR_WHO_GODS );
	send_to_char( "Who flag toggled.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "normal_who" ) )
    {
	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Invalid target.\n\r", ch );
	    return;
	}

	TOGGLE_BIT( victim->plr2, PLR_WHO_NORMAL );
	send_to_char( "Who flag toggled.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg2, "ph" ) )
    {
	if ( IS_NPC( victim ) )
	{
	    send_to_char( "Invalid target.\n\r", ch );
	    return;
	}

	TOGGLE_BIT( victim->plr2, PLR_PLAYER_HELPER );
	send_to_char( "Player helper flag toggled.\n\r", ch );
	return;
    }
    /*
     * Snarf the value (which need not be numeric).
     */
    if ( arg3[0] =='\0' )
    {
	do_mset( ch, "" );
	return;
    }
    value = is_number( arg3 ) ? atoi( arg3 ) : -1;

    /*
     * Set something.
     */

	if ( !str_prefix( arg2, "dtimer" ) )
	{
	    long minutes,seconds = 0;
	    int i;

	    if ( victim->pcdata == NULL || victim->pcdata->debt == NULL )
	    {
		send_to_char( "They do not have a debt.\n\r", ch );
		return;
	    }

	    if ( !is_number( arg3 ) )
	    {
		for ( i = 0 ; arg3[i] == ':' || arg3[i] == '\0' ; i++ );

		if ( arg3[i] == '\0' )
		{
		    send_to_char( "Invalid time.\n\r", ch );
		    return;
		}

		arg3[i] = '\0';

		if ( !is_number( arg3 ) || !is_number( &arg3[i+1] ) )
		{
		    send_to_char( "Invalid time.\n\r", ch );
		    return;
		}

		seconds = atoi( &arg3[i+1] );
		if ( ( minutes = atoi( arg3 ) ) < 0 )
		    seconds *= -1;
	    }
	    else
		minutes = atoi( arg3 );

	    victim->pcdata->debt->due = victim->played + current_time - victim->logon + ( minutes * (long) 300 + seconds * (long) 50 );

	    return;
	}


    if ( !str_cmp( arg2, "str" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_STR) )
	{
	    sprintf(buf,
		"Strength range is 3 to %d\n\r.",
		get_max_train(victim,STAT_STR));
	    send_to_char(buf,ch);
	    return;
	}

	victim->perm_stat[STAT_STR] = value;
	return;
    }

    if ( !str_cmp( arg2, "int" ) )
    {
        if ( value < 3 || value > get_max_train(victim,STAT_INT) )
        {
            sprintf(buf,
		"Intelligence range is 3 to %d.\n\r",
		get_max_train(victim,STAT_INT));
            send_to_char(buf,ch);
            return;
        }
 
        victim->perm_stat[STAT_INT] = value;
        return;
    }

    if ( !str_cmp( arg2, "wis" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_WIS) )
	{
	    sprintf(buf,
		"Wisdom range is 3 to %d.\n\r",get_max_train(victim,STAT_WIS));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_WIS] = value;
	return;
    }

    if ( !str_cmp( arg2, "dex" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_DEX) )
	{
	    sprintf(buf,
		"Dexterity ranges is 3 to %d.\n\r",
		get_max_train(victim,STAT_DEX));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_DEX] = value;
	return;
    }

    if ( !str_cmp( arg2, "chr" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_CHR) )
	{
	    sprintf(buf,
		"Charisma ranges is 3 to %d.\n\r",
		get_max_train(victim,STAT_CHR));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_CHR] = value;
	return;
    }

    if ( !str_cmp( arg2, "con" ) )
    {
	if ( value < 3 || value > get_max_train(victim,STAT_CON) )
	{
	    sprintf(buf,
		"Constitution range is 3 to %d.\n\r",
		get_max_train(victim,STAT_CON));
	    send_to_char( buf, ch );
	    return;
	}

	victim->perm_stat[STAT_CON] = value;
	return;
    }

    if ( !str_prefix( arg2, "sex" ) )
    {
	if ( value < 1 || value > 3 )
	{
	    send_to_char( "Sex range is 1 to 3.\n\r", ch );
	    return;
	}
	victim->sex = value;
	if (!IS_NPC(victim))
	    victim->pcdata->true_sex = value;
	return;
    }

    if ( !str_prefix( arg2, "mount" ) )
    {
    	if ( ( vch = get_char_world( ch, arg3 ) ) == NULL )
    	{
		send_to_char( "They aren't here.\n\r", ch );
		return;

    	}
	victim->mount = vch;
	return;
    }

    if ( !str_prefix( arg2, "clan" ) )
    {
	if ( clan_lookup( arg3 ) == NULL )
	{
	    send_to_char( "Invalid Clan.\n\r", ch );
	    return;
	}
	victim->pcdata->pcclan->clan = clan_lookup( arg3 );
	return;
    }

    if ( !str_prefix( arg2, "level" ) )
    {
	if ( !IS_NPC(victim) )
	{
	    send_to_char( "Not on PC's.\n\r", ch );
	    return;
	}

	if ( value < 0 || value > 60 )
	{
	    send_to_char( "Level range is 0 to 60.\n\r", ch );
	    return;
	}
	victim->level = value;
	return;
    }

    if ( !str_prefix( arg2, "size" ) )
    {
	if ( value < 0 || value > 4 )
	{
	    send_to_char( "Size range is 0 to 4.\n\r", ch );
	    return;
	}
	victim->size = value;
	return;
    }
    if ( !str_prefix( arg2, "exp" ) )
    {
	if ( value < 0 || value > 165795000 )
	{
	    send_to_char( "Exp range is 0 to 165795000.\n\r", ch );
	    return;
	}
	victim->exp = value;
	return;
    }
    if ( !str_prefix( arg2, "hitroll" ) )
    {
	if ( value < 0 || value > 10 )
	{
	    send_to_char( "Hitroll range is 0 to 10.\n\r", ch );
	    return;
	}
	victim->hitroll = value;
	return;
    }
    if ( !str_prefix( arg2, "damroll" ) )
    {
	if ( value < 0 || value > 10 )
	{
	    send_to_char( "Damroll range is 0 to 10\n\r", ch );
	    return;
	}
	victim->damroll = value;
	return;
    }
    if ( !str_prefix( arg2, "played" ) )
    {
	if ( value < 1 || value > 100000000 )
	{
	    send_to_char( "Played range is 1 to 100000000.\n\r", ch );
	    return;
	}
	victim->played = value;
	return;
    }

    if ( !str_prefix( arg2, "coins" ) )
    {
	one_argument(argument,buf);

	coin_type = get_coin_type( buf );
	if ( coin_type >= 0 ) 
	    victim->gold[ coin_type ] = value;
	else 
	    victim->gold[ race_table[ victim->race ][ victim->sex ].world ] = value;
	return;
    }

    if ( !str_prefix( arg2, "hp" ) )
    {
	if ( value < -10 || value > 30000 )
	{
	    send_to_char( "Hp range is -10 to 30,000 hit points.\n\r", ch );
	    return;
	}
	victim->max_hit = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_hit = value;
	return;
    }

    if ( ( type = get_element_type( arg2 ) ) != -1 )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Mana range is 0 to 30,000 mana points.\n\r", ch );
	    return;
	}
	victim->max_mana[ type ] = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_mana[ type ] = value;
	return;
    }

    if ( !str_prefix( arg2, "move" ) )
    {
	if ( value < 0 || value > 30000 )
	{
	    send_to_char( "Move range is 0 to 30,000 move points.\n\r", ch );
	    return;
	}
	victim->max_move = value;
        if (!IS_NPC(victim))
            victim->pcdata->perm_move = value;
	return;
    }

    if ( !str_prefix( arg2, "practice" ) )
    {
	if ( value < 0 || value > 250 )
	{
	    send_to_char( "Practice range is 0 to 250 sessions.\n\r", ch );
	    return;
	}
	victim->practice = value;
	return;
    }

    if ( !str_prefix( arg2, "thirst" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Thirst range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_THIRST] = value;
	return;
    }

    if ( !str_prefix( arg2, "drunk" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Drunk range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_DRUNK] = value;
	return;
    }

    if ( !str_prefix( arg2, "full" ) )
    {
	if ( IS_NPC(victim) )
	{
	    send_to_char( "Not on NPC's.\n\r", ch );
	    return;
	}

	if ( value < -1 || value > 100 )
	{
	    send_to_char( "Full range is -1 to 100.\n\r", ch );
	    return;
	}

	victim->pcdata->condition[COND_FULL] = value;
	return;
    }

    if (!str_prefix( arg2, "race" ) )
    {
	int race;

	race = race_lookup(arg3);

	if ( race == -1)
	{
	    send_to_char("That is not a valid race.\n\r",ch);
	    return;
	}

	if (!IS_NPC(victim) && !race_table[race][victim->sex].pc_race)
	{
	    send_to_char("That is not a valid player race.\n\r",ch);
	    return;
	}

	victim->race = race;
	return;
    }

    if ( !str_prefix( arg2, "follower" ) )
    {
	if ( !IS_NPC(victim) )
	{
		send_to_char( "Not on PC's.\n\r", ch );
		return;
	}

	victim->pIndexData->follower = value;
	sprintf(buf,"follower now is %ld\n\r",victim->pIndexData->follower);
	send_to_char(buf,ch);
	return;
    }

    /*
     * Generate usage message.
     */
    do_mset( ch, "" );
    return;
}

void do_string( CHAR_DATA *ch, char *argument )
{
    char type [MAX_INPUT_LENGTH];
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    CHAR_DATA *victim;
    OBJ_DATA *obj;

    argument = one_argument( argument, type );
    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( type[0] == '\0' || arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  string char <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long desc title spec\n\r",ch);
	send_to_char("  string obj  <name> <field> <string>\n\r",ch);
	send_to_char("    fields: name short long extended\n\r",ch);
	return;
    }
    
    if (!str_prefix(type,"character") || !str_prefix(type,"mobile"))
    {
    	if ( ( victim = get_char_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
    	}

	/* string something */

     	if ( !str_prefix( arg2, "name" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( "Not on PC's.\n\r", ch );
	    	return;
	    }
	    strncpy( victim->name, arg3, 30 );
	    return;
    	}
    	
    	if ( !str_prefix( arg2, "description" ) )
    	{
	    if (!IS_NPC(victim) )
	    {
		free_string(victim->description);
    	    	victim->description = str_dup(arg3);
	    }
    	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    if (!IS_NPC(victim) )
	    	strncpy( victim->short_descr, arg3, 80 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    if (!IS_NPC(victim) )
	    {
	    	strcat(arg3,"\n\r");
	    	strncpy( victim->long_descr, arg3, 80 );
	    }
	    return;
    	}

    	if ( !str_prefix( arg2, "title" ) )
    	{
	    if ( IS_NPC(victim) )
	    {
	    	send_to_char( "Not on NPC's.\n\r", ch );
	    	return;
	    }

	    set_title( victim, arg3 );
	    return;
    	}

    	if ( !str_prefix( arg2, "spec" ) )
    	{
	    if ( !IS_NPC(victim) )
	    {
	    	send_to_char( "Not on PC's.\n\r", ch );
	    	return;
	    }

	    if ( ( victim->spec_fun = spec_lookup( arg3 ) ) == 0 )
	    {
	    	send_to_char( "No such spec fun.\n\r", ch );
	    	return;
	    }

	    return;
    	}
    }
    
    if (!str_prefix(type,"object"))
    {
    	/* string an obj */
    	
   	if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    	{
	    send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	    return;
    	}
    	
        if ( !str_prefix( arg2, "name" ) )
    	{
	    strncpy( obj->name, arg3, 30 );
	    return;
    	}

    	if ( !str_prefix( arg2, "short" ) )
    	{
	    strncpy( obj->short_descr, arg3, 80 );
	    return;
    	}

    	if ( !str_prefix( arg2, "long" ) )
    	{
	    strncpy( obj->description, arg3, 80 );
	    return;
    	}

    	if ( !str_prefix( arg2, "ed" ) || !str_prefix( arg2, "extended"))
    	{
	    EXTRA_DESCR_DATA *ed;

	    argument = one_argument( argument, arg3 );
	    if ( argument == NULL )
	    {
	    	send_to_char( "Syntax: oset <object> ed <keyword> <string>\n\r",
		    ch );
	    	return;
	    }

	    arg1[0] = '\0';
	    strcat(arg1,argument);
 	    strcat(arg1,"\n\r");

	        ed			= new_extra_descr();

	    strncpy( ed->keyword, arg3, 30 );
	    ed->description	= str_dup( arg1 );
	    ed->next		= obj->extra_descr;
	    obj->extra_descr	= ed;
	    return;
    	}
    }
    
    	
    /* echo bad use message */
    do_string(ch,"");
}



void do_oset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    OBJ_DATA *obj;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char("Syntax:\n\r",ch);
	send_to_char("  set obj <object> <field> <value>\n\r",ch);
	send_to_char("  Field being one of:\n\r",				ch );
	send_to_char("    extra wear level weight cost timer\n\r",		ch );
	return;
    }

    if ( ( obj = get_obj_world( ch, arg1 ) ) == NULL )
    {
	send_to_char( "Nothing like that in heaven or earth.\n\r", ch );
	return;
    }

    /*
     * Snarf the value (which need not be numeric).
     */
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_prefix( arg2, "extra" ) )
    {
	obj->extra_flags = value;
	return;
    }

    if ( !str_prefix( arg2, "size" ) )
    {
	if ( obj->wear_data == NULL )
	{
	    send_to_char( "That cannot be worn.\n\r", ch );
	    return;
	}
	obj->wear_data->size = value;
	return;
    }

    if ( !str_prefix( arg2, "wear" ) )
    {
	if ( obj->wear_data == NULL )
	    obj->wear_data = wear_alloc();
	obj->wear_data->wear_flags = value;
	return;
    }

    if ( !str_prefix( arg2, "weight" ) )
    {
	if ( obj->wear_data == NULL )
	{
	    send_to_char( "This object can't be worn.\n\r", ch );
	    return;
	}

	obj->wear_data->weight = value;
	return;
    }

    if ( !str_prefix( arg2, "cost" ) )
    {
	if ( obj->wear_data == NULL )
	{
	    send_to_char( "That cannot be worn.\n\r", ch );
	    return;
	}

	obj->wear_data->cost = value;
	return;
    }

    if ( !str_prefix( arg2, "timer" ) )
    {
	obj->timer = value;
	return;
    }
	
    /*
     * Generate usage message.
     */
    do_oset( ch, "" );
    return;
}



void do_rset( CHAR_DATA *ch, char *argument )
{
    char arg1 [MAX_INPUT_LENGTH];
    char arg2 [MAX_INPUT_LENGTH];
    char arg3 [MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA *location;
    int value;

    argument = one_argument( argument, arg1 );
    argument = one_argument( argument, arg2 );
    strcpy( arg3, argument );

    if ( arg1[0] == '\0' || arg2[0] == '\0' || arg3[0] == '\0' )
    {
	send_to_char( "Syntax:\n\r",ch);
	send_to_char( "  set room <location> <field> <value>\n\r",ch);
	send_to_char( "  Field being one of:\n\r",			ch );
	send_to_char( "    flags sector\n\r",				ch );
	return;
    }

    if ( ( location = find_location( ch, arg1 ) ) == NULL )
    {
	send_to_char( "No such location.\n\r", ch );
	return;
    }

    /*
     * Snarf the value.
     */
    if ( !is_number( arg3 ) )
    {
	send_to_char( "Value must be numeric.\n\r", ch );
	return;
    }
    value = atoi( arg3 );

    /*
     * Set something.
     */
    if ( !str_prefix( arg2, "flags" ) )
    {
	location->room_flags	= value;
	return;
    }

    if ( !str_prefix( arg2, "sector" ) )
    {
	location->sector_type	= value;
	return;
    }

    /*
     * Generate usage message.
     */
    do_rset( ch, "" );
    return;
}



void do_sockets( CHAR_DATA *ch, char *argument )
{
    BUFFER *output;
    char buf2[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    DESCRIPTOR_DATA *d;
    int count;

    count	= 0;
    output = new_buf();    
    one_argument(argument,arg);
    for ( d = descriptor_list; d != NULL; d = d->next )
    {
	if ( d->character != NULL && can_see( ch, d->character ) 
	&& (arg[0] == '\0' || is_name(arg,d->character->name)
			   || (d->original && is_name(arg,d->original->name))))
	{
	    char cname[21];
	    count++;
	    switch( d->connected )
	    {
	    case CON_AEDITOR:		strncpy( cname, "Aediting", 20 );		break;
	    case CON_REDITOR:		strncpy( cname, "Rediting", 20 );		break;
	    case CON_MEDITOR:		strncpy( cname, "Mediting", 20 );		break;
	    case CON_OEDITOR:		strncpy( cname, "Oediting", 20 );		break;
	    case CON_HEDITOR:		strncpy( cname, "Hediting", 20 );		break;
	    case CON_SEDITOR:		strncpy( cname, "Sediting", 20 );		break;
	    case CON_RAEDITOR:		strncpy( cname, "RAediting", 20 );		break;
	    case CON_GEDITOR:		strncpy( cname, "Gediting", 20 );		break;
	    case CON_MAEDITOR:		strncpy( cname, "MAediting", 20 );		break;
	    case CON_EDITING:		strncpy( cname, "Editing", 20 );		break;
	    case CON_PLAYING:		strncpy( cname, "Mudding", 20 );		break;
	    case CON_GET_NAME:		strncpy( cname, "Entering ED", 20 );		break;
	    case CON_GET_OLD_PASSWORD:	strncpy( cname, "Get Old Passwd", 20 );		break;
	    case CON_CONFIRM_NEW_NAME:	strncpy( cname, "Confirm New Name", 20 );	break;
	    case CON_GET_NEW_PASSWORD:	strncpy( cname, "Get New Passwd", 20 );		break;
	    case CON_CONFIRM_NEW_PASSWORD:	strncpy( cname, "Confirm New Passwd", 20 );		break;
	    case CON_GET_NEW_RACE:	strncpy( cname, "Get new race", 20 );		break;
	    case CON_GET_NEW_SEX:	strncpy( cname, "Get new sex", 20 );		break;
	    case CON_DEFAULT_CHOICE:	strncpy( cname, "Default choice", 20 );		break;
	    case CON_GET_NEW_GUILDS:	strncpy( cname, "Get New Guilds ", 20 );	break;
	    case CON_CHOOSE_CREATION:	strncpy( cname, "Choose Creation ", 20 );	break;
	    case CON_ROLL_STATS:	strncpy( cname, "Rolling Statistics", 20 );	break;
	    case CON_READ_IMOTD:	strncpy( cname, "Reading IMOTD", 20 );		break;
	    case CON_READ_MOTD:		strncpy( cname, "Reading MOTD", 20 );		break;
	    case CON_BREAK_CONNECT:	strncpy( cname, "Link Dead", 20 );		break;
	    case CON_DEFAULT_COLOR:	strncpy( cname, "Default color", 20 );		break;
	    case CON_AFT_LOGIN:		strncpy( cname, "Aft_login", 20 );		break;
	    case CON_DEAD:		strncpy( cname, "Dead", 20 );			break;
	    case CON_DO_GREETING:	strncpy( cname, "Do_greating", 20 );		break;
	    default:			strncpy( cname, "*shrug* who knows", 20 );	break;
	    }
	    	
	    sprintf( buf2, "`W[`m%3d`W]`w %-10.10s logged in from `c%20.20s`w is %-15.15s\n\r",
		d->descriptor,
		d->original  ? d->original->name  :
		d->character ? d->character->name : "(none)",
		d->host,
		cname
		);
	    add_buf( output, buf2 );
	} 
}
    if (count == 0)
    {
	send_to_char("No one by that name is connected.\n\r",ch);
        free_buf(output);
	return;
    }

    sprintf( buf2, "%d user%s\n\r", count, count == 1 ? "" : "s" );
    add_buf(output,buf2);
    page_to_char( buf_string(output), ch );
    free_buf(output);
    return;
}



/*
 * Thanks to Grodyn for pointing out bugs in this function.
 */
void do_force( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    argument = one_argument( argument, arg );
    if ( arg[0] == '\0' || argument[0] == '\0' )
    {
	send_to_char( "Force whom to do what?\n\r", ch );
	return;
    }

    one_argument(argument,arg2);
  
    if (!str_cmp(arg2,"delete"))
    {
	send_to_char("That will NOT be done.\n\r",ch);
	return;
    }
	
    sprintf( buf, "$n forces you to '%s'.", argument );

    if ( !str_cmp( arg, "all" ) )
    {
	CHAR_DATA *vch;
	CHAR_DATA *vch_next;

	if (!IS_IMMORTAL(ch))
	{
	    send_to_char("Sorry, your not an immortal\n\r",ch);
	    return;
	}

	for ( vch = char_list; vch != NULL; vch = vch_next )
	{
	    vch_next = vch->next;

	    if ( !IS_NPC(vch)
	        && check_immortal_order(ch, vch)
		&& vch->prompt_num < 1 )
	    {
		act( buf, ch, NULL, vch, TO_VICT );
		interpret( vch, argument );
	    }
	}
    }
    else if (!str_cmp(arg,"players"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
 
        if (!IS_IMMORTAL(ch))
        {
            send_to_char("Your not an immortal!\n\r",ch);
            return;
        }
 
        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;
 
	    if ( !IS_NPC(vch)
		&& check_immortal_order(ch, vch)
		&& vch->prompt_num < 1 )
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else if (!str_cmp(arg,"gods"))
    {
        CHAR_DATA *vch;
        CHAR_DATA *vch_next;
 

        if (!IS_IMP(ch))
        {
            send_to_char("you failed",ch);
            return;
        }
 
        for ( vch = char_list; vch != NULL; vch = vch_next )
        {
            vch_next = vch->next;
 
		if( check_immortal_order(ch, vch)
		&& vch->prompt_num < 1 )
            {
                act( buf, ch, NULL, vch, TO_VICT );
                interpret( vch, argument );
            }
        }
    }
    else
    {
	CHAR_DATA *victim;

	if ( ( victim = get_char_world( ch, arg ) ) == NULL )
	{
	    send_to_char( "They aren't here.\n\r", ch );
	    return;
	}

	if ( victim == ch )
	{
	    send_to_char( "Aye aye, right away!\n\r", ch );
	    return;
	}

	if(!check_immortal_order(ch, victim) )
	{
	    send_to_char( "Do it yourself!\n\r", ch );
	    return;
	}

	if ( victim->prompt_num  > 0 && !IS_OPTIVISION(ch))
	{   
	    send_to_char("They are editing at present.\n\r",ch);
	    return;
	}
	act( buf, ch, NULL, victim, TO_VICT );
	interpret( victim, argument );
    }

    send_to_char( "Ok.\n\r", ch );
    return;
}

/*
 * New routines by Dionysos.
 */
void do_invis( CHAR_DATA *ch, char *argument )
{
    int level;
    char arg[MAX_STRING_LENGTH];
    

    if ( IS_NPC(ch) )
	return;

    /* RT code for taking a level argument */
    one_argument( argument, arg );

    if ( arg[0] == '\0' ) 
    /* take the default path */

      if ( IS_SET(ch->plr, PLR_WIZINVIS) )
      {
	  REMOVE_BIT(ch->plr, PLR_WIZINVIS);
	  ch->invis_level = 0;
	  ++ch->in_room->area->nplayer;
	  cact( "$n utters the words, 'goblzelarp'.",ch, NULL, NULL, TO_ROOM, "`W");
          cact( "$n slowly fades into existence.", ch, NULL, NULL, TO_ROOM, "`G");
          send_to_char( "`GYou slowly fade back into existence.`w\n\r", ch );
	  if (LOGON_ON)
	      if (ch->logon_data != NULL)
		ch->logon_data->level = ch->level;
      }
      else
      {
	  if ( !IS_SET( ch->plr, PLR_WIZINVIS ) )
	      --ch->in_room->area->nplayer;

	  SET_BIT(ch->plr, PLR_WIZINVIS);
	  ch->invis_level = ch->level;
	  cact("$n utters the words, 'engarblelerp'.",ch, NULL, NULL, TO_ROOM,"`W");
          cact("$n slowly fades out of existance.", ch, NULL, NULL, TO_ROOM,"`b");
 	  send_to_char( "`bYou slowly vanish into thin air.`w\n\r", ch );
	  if (LOGON_ON)
	      if (ch->logon_data != NULL)
		ch->logon_data->level = ch->invis_level;
      }
      else
      /* do the level thing */
      {
	  level = atoi(arg);
	  if ( level < 1 || level > 65 )
	  {
	   	send_to_char( "Out of Invis Range!\n\r", ch );
		return;
	  }

	  if (IS_IMMORTAL(ch))
	  {
	    	if (IS_OPTIVISION(ch) )
	    	{
		    ch->reply = NULL;
		    if ( !IS_SET( ch->plr, PLR_WIZINVIS ) )
			--ch->in_room->area->nplayer;

		    SET_BIT(ch->plr, PLR_WIZINVIS);
		    ch->invis_level = level;
		    if (LOGON_ON)
			if (ch->logon_data != NULL)
			    ch->logon_data->level = ch->invis_level;

		    send_to_char( "Optivision invis enabled\n\r", ch );
		    return;
	    	}

	        if (IS_IMP(ch))
	        {
	    	    if ( level < 2 || level > MAX_LEVEL + 2 )
	    	    {
		    	send_to_char("Invis level must be between 2 and 62.\n\r",ch);
		    	return;
		    }

		    ch->reply = NULL;
		    if ( !IS_SET( ch->plr, PLR_WIZINVIS ) )
			--ch->in_room->area->nplayer;

		    SET_BIT(ch->plr, PLR_WIZINVIS);
		    ch->invis_level = level;
		    cact( "$n utters the words, 'hoglarbelerp'.",ch, NULL,NULL, TO_ROOM, "`W");
		    cact( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM,"`b");
		    send_to_char( "You slowly vanish into thin air.\n\r", ch );
		    return;
	    	}
	    	else
	    	{
	    	    if ( level < 2 || level > MAX_LEVEL + 1 )
	    	    {
		    	send_to_char("Invis level must be between 2 and 61.\n\r",ch);
		    	return;
		    }

		    ch->reply = NULL;
		    if ( !IS_SET( ch->plr, PLR_WIZINVIS ) )
			--ch->in_room->area->nplayer;

		    SET_BIT(ch->plr, PLR_WIZINVIS);
		    ch->invis_level = level;
		    cact( "$n utters the words, 'hoglarbelerp'.",ch, NULL,NULL, TO_ROOM, "`W");
		    cact( "$n slowly fades into thin air.", ch, NULL, NULL, TO_ROOM,"`b");
		    send_to_char( "You slowly vanish into thin air.\n\r", ch );
	    	}
	    }
    	}

        return;
}

bool is_optivision( CHAR_DATA *ch )
{
    int i;

    for ( i = 0; i < MAX_OPTIWIZ; i++ )
    if ( !str_cmp( ch->name, capitalize( opti_wiz_list[ i ] ) ) )
	return TRUE;

    return FALSE;
}

void do_wizpass( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET(ch->plr, PLR_WIZPASS) )
    {
	REMOVE_BIT(ch->plr, PLR_WIZPASS);
	send_to_char( "Pass Through mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->plr, PLR_WIZPASS);
	send_to_char( "Pass Through mode on.\n\r", ch );
    }

    return;
}

void do_holylight( CHAR_DATA *ch, char *argument )
{
    if ( IS_NPC(ch) )
	return;

    if ( IS_SET( ch->plr, PLR_HOLYLIGHT ) && IS_IMMORTAL( ch ) && !IS_OPTIVISION( ch ) )
    {
	send_to_char( "You can't turn off holylight once turned on.\n\r", ch );
	return;
    } 

    if ( IS_SET(ch->plr, PLR_HOLYLIGHT) )
    {
	REMOVE_BIT(ch->plr, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode off.\n\r", ch );
    }
    else
    {
	SET_BIT(ch->plr, PLR_HOLYLIGHT);
	send_to_char( "Holy light mode on.\n\r", ch );
    }

    return;
}

void do_areasave( AREA_DATA  *pArea, char *argument, CHAR_DATA *ch )
{
	char buf[MAX_STRING_LENGTH];
	FILE *fp;
	/*
	 *  If the directory hasn't been made yet, it should be.
	 */
	tail_chain();

	sprintf(buf, "%s%s/", SAVEAREAS_DIR, pArea->filename);
	if ( (fp = fopen(buf, "r" ) ) == NULL)
	{
		sprintf(buf, "mkdir %s%s", SAVEAREAS_DIR, pArea->filename);
		system(buf);
	}
        if ( fp )
	fclose(fp);
	if (!str_cmp(argument, "rooms"))
	{
		save_rooms(pArea, pArea->filename);
		send_to_char("`YRooms saved.`w\n\r", ch);
		return; 

	}
	if (!str_cmp(argument, "objs"))
	{
		save_objs(pArea, pArea->filename);
		send_to_char("`YObjects saved.`w\n\r", ch);
		return;
	}
	if (!str_cmp(argument, "mobs"))
	{
		save_mobs(pArea, pArea->filename);
		send_to_char("`YMobiles saved.`w\n\r", ch);
		return;
	}
	if (!str_cmp(argument, "events"))
	{
		save_events(pArea, pArea->filename);
		send_to_char("`YEvents saved.`w\n\r", ch);
		return;
	}
	if (!str_cmp(argument, "shops"))
	{
		save_shops(pArea, pArea->filename);
		send_to_char("`YShops saved.`w\n\r", ch);
		return;
	}
	if (!str_cmp(argument, "specials"))
	{
		send_to_char("`YUse asave zone`w \n\r", ch);
		return;
	}
	if (!str_cmp(argument, "world"))
	{
		save_rooms(pArea, pArea->filename);
		save_objs(pArea, pArea->filename);
		save_mobs(pArea, pArea->filename);
		save_events(pArea, pArea->filename);
		save_shops(pArea, pArea->filename);
		return;
	}
	if (!str_cmp(argument, "all"))
	{
	tail_chain();
		save_rooms(pArea, pArea->filename);
	tail_chain();
		send_to_char("Rooms saved.\n\r", ch);
	tail_chain();
		save_objs(pArea, pArea->filename);
		send_to_char("Objects saved.\n\r", ch);
	tail_chain();
		save_mobs(pArea, pArea->filename);
		send_to_char("Mobiles saved.\n\r", ch);
	tail_chain();
		save_events(pArea, pArea->filename);
		send_to_char("Events saved.\n\r", ch);
	tail_chain();
		save_shops(pArea, pArea->filename);
		send_to_char("Shops saved.\n\r", ch);
	tail_chain();
		send_to_char("Specials saved.\n\r", ch);
	tail_chain();
		send_to_char("Mobprograms saved.\n\r", ch);
		return;
	}

	send_to_char("`YAreasave is no longer with us.\n\r",ch);
	send_to_char("  areasave rooms     : Save Rooms in current area\n\r", ch);
	send_to_char("  areasave objs      : Save Objects in current area\n\r",ch);
	send_to_char("  areasave mobs      : Save Mobiles in current area\n\r", ch);
	send_to_char("  areasave resets    : Save Resets in current area\n\r", ch);
	send_to_char("  areasave shops     : Save Shops in current area\n\r", ch);
	send_to_char("  areasave specials  : Save Specials in current area\n\r", ch);
	send_to_char("  asave zone  	   : Save everything!`w\n\r", ch);
	return;
}

bool make_floor( ROOM_INDEX_DATA * room, int floor_dir )
{
    AREA_DATA * pArea = room->area;
    EXIT_DATA * pexit;
    EXIT_DATA * pexit_here;
    EXIT_DATA * pexit_there;
    ROOM_INDEX_DATA * proom;
    ROOM_INDEX_DATA * in_room = room;
    long vnum;
    int count=0;
    int i;
    int door;
    int iHash;

    if ( str_cmp( room->name, "Large Stairwell" ) )
	return FALSE;

    for ( vnum = pArea->lvnum ; count < 12 && vnum < pArea->uvnum ; vnum++ )
	if ( !free_room[ vnum ] )
	   count++;

    if ( count < 12 )
        return FALSE;

/* build the stairwell */

    proom 		= pRoomIndex_alloc();
    proom->people	= NULL;
    proom->contents	= NULL;	
    proom->snoop_by = NULL; 
    proom->extra_descr	= NULL;	
    proom->area		= pArea;
    strncpy( proom->name,"Large Stairwell", 80 );
    proom->description	= str_dup ( "   The large wooden stairwell leads to the next floor of the apartment\n\r building.  The highly polished handrails glow from the light of the\n\r torches which line the walls.\n\r" );
    proom->room_flags 	= ROOM_INDOORS|ROOM_SAFE|ROOM_NO_MOB;
    proom->room_flags_2 = ROOM_SAVE;
    proom->sector_type 	= 0;
    proom->size 	= 3;
    proom->light	= 45;
    for ( door = 0; door <= 5; door++)
        {proom->exit[door] = NULL;}	 
   /* snarf a vnum */
    for ( vnum = pArea->lvnum ; free_room[vnum] ; vnum++ );
    proom->vnum = vnum; 
    free_room[vnum] = 1;

    pexit_here = new_exit();
    pexit_here->description[0] = '\0';
    pexit_here->keyword[0] = '\0';
    pexit_here->tracks = NULL;
    pexit_here->trap = NULL;
    strncpy( pexit_here->mes_to_char, "You climb the stairs.", 80 );
    strncpy( pexit_here->mes_to_room, "climbs the stairs.", 80 );
    strncpy( pexit_here->mes_at_room, "climbs the stairs.", 80 );
    pexit_here->lock = NULL;
    pexit_here->exit_info = 0;
    pexit_here->exit_info_default = 0;
    pexit_here->u1.vnum = vnum;
    pexit_here->u1.to_room = proom;
    in_room->exit[DIR_UP] = pexit_here;
    pexit_there = new_exit();
    pexit_there->description[0] = '\0';
    pexit_there->keyword[0] = '\0';
    pexit_there->tracks = NULL;
    pexit_there->trap = NULL;
    strncpy( pexit_there->mes_to_char, "You walk down the stairs.", 80 );
    strncpy( pexit_there->mes_to_room, "$N walks down the stairs.", 80 );
    strncpy( pexit_there->mes_at_room, "$N walks down the stairs.", 80 );
    pexit_there->lock = NULL;
    pexit_there->exit_info = 0;
    pexit_there->exit_info_default = 0;
    pexit_there->u1.vnum = in_room->vnum;
    pexit_there->u1.to_room = in_room;
    proom->exit[ DIR_DOWN ] = pexit_there;

  /* Put the room in the Database */
    iHash		= vnum % MAX_KEY_HASH;
    proom->next		= room_index_hash[iHash];
    room_index_hash[iHash]	= proom;

    in_room = proom;

    for ( i = 1 ; i < 12 ; i++ )
    {
	/* build the hallway */

    proom 		=  pRoomIndex_alloc();
    proom->people	= NULL;
    proom->contents	= NULL;	
    proom->snoop_by = NULL; 
    proom->extra_descr	= NULL;	
    proom->area		= pArea;
    if ( i != 11 )
    {
        strncpy( proom->name, "A Wide Hallway", 80 );
        proom->description	= str_dup ( "   A long brown carpet runs the length of the floor.  Torches line\n\rthe walls and illuminate the rough wooden walls.\n\r" );
    }
    else
    {
         strncpy( proom->name, "Large Stairwell", 80 );
         proom->description	= str_dup ( "   The large wooden stairwell leads to the next floor of the apartment\n\r building.  The highly polished handrails glow from the light of the\n\r torches which line the walls.\n\r" );
    } 
    proom->room_flags 	= ROOM_INDOORS|ROOM_SAFE|ROOM_NO_MOB;
    proom->room_flags_2	= ROOM_SAVE;
    proom->sector_type 	= 0;
    proom->light	= 45;
    proom->size		= 3;
    for ( door = 0; door <= 5; door++)
        {proom->exit[door] = NULL;}	 
   /* snarf a vnum */
    for ( vnum = pArea->lvnum ; free_room[vnum] ; vnum++ );
    proom->vnum = vnum; 
    free_room[vnum] = 1;

    pexit_here = new_exit();
    pexit_here->description[0] = '\0';
    pexit_here->keyword[0] = '\0';
    pexit_here->tracks = NULL;
    pexit_here->trap = NULL;
    pexit_here->mes_to_char[0] = '\0';
    pexit_here->mes_to_room[0] = '\0';
    pexit_here->mes_at_room[0] = '\0';
    pexit_here->lock = NULL;
    pexit_here->exit_info = 0;
    pexit_here->exit_info_default = 0;
    pexit_here->u1.vnum = vnum;
    pexit_here->u1.to_room = proom;
    in_room->exit[ floor_dir ] = pexit_here;
    pexit_there = new_exit();
    pexit_there->description[0] = '\0';
    pexit_there->keyword[0] = '\0';
    pexit_there->tracks = NULL;
    pexit_there->trap = NULL;
    pexit_there->mes_to_char[0] = '\0';
    pexit_there->mes_to_room[0] = '\0';
    pexit_there->mes_at_room[0] = '\0';
    pexit_there->lock = NULL;
    pexit_there->exit_info = 0;
    pexit_there->exit_info_default = 0;
    pexit_there->u1.vnum = in_room->vnum;
    pexit_there->u1.to_room = in_room;
    proom->exit[ rev_dir[ floor_dir ] ] = pexit_there;

  /* Put the room in the Database */
    iHash		= vnum % MAX_KEY_HASH;
    proom->next		= room_index_hash[iHash];
    room_index_hash[iHash]	= proom;

    in_room = proom;
    }

    for ( proom = room ; ( pexit = proom->exit[ floor_dir ] ) != NULL && pexit->u1.to_room != NULL ; proom = pexit->u1.to_room );


    pexit_here = new_exit();
    pexit_here->description[0] = '\0';
    pexit_here->keyword[0] = '\0';
    pexit_here->tracks = NULL;
    pexit_here->trap = NULL;
    strncpy( pexit_here->mes_to_char, "You climb the stairs.", 80 );
    strncpy( pexit_here->mes_to_room, "climbs the stairs.", 80 );
    strncpy( pexit_here->mes_at_room, "climbs the stairs.", 80 );
    pexit_here->lock = NULL;
    pexit_here->exit_info = 0;
    pexit_here->exit_info_default = 0;
    pexit_here->u1.vnum = vnum;
    pexit_here->u1.to_room = proom;
    in_room->exit[ DIR_DOWN ] = pexit_here;
    pexit_there = new_exit();
    pexit_there->description[0] = '\0';
    pexit_there->keyword[0] = '\0';
    pexit_there->tracks = NULL;
    pexit_there->trap = NULL;
    strncpy( pexit_there->mes_to_char, "You walk down the stairs.", 80 );
    strncpy( pexit_there->mes_to_room, "$N walks down the stairs.", 80 );
    strncpy( pexit_there->mes_at_room, "$N walks down the stairs.", 80 );
    pexit_there->lock = NULL;
    pexit_there->exit_info = 0;
    pexit_there->exit_info_default = 0;
    pexit_there->u1.vnum = in_room->vnum;
    pexit_there->u1.to_room = in_room;
    proom->exit[ DIR_UP ] = pexit_there;

    return TRUE;
}

bool make_first_floor( ROOM_INDEX_DATA * room, int dir )
{
    AREA_DATA * pArea = room->area;
    EXIT_DATA * pexit_here;
    EXIT_DATA * pexit_there;
    ROOM_INDEX_DATA * proom;
    ROOM_INDEX_DATA * in_room = room;
    int floor_dir = ( dir + 1 ) % 4;
    long vnum;
    int count=0;
    int i;
    int door;
    int iHash;

    for ( vnum = pArea->lvnum ; count < 12 && vnum < pArea->uvnum ; vnum++ )
	if ( !free_room[ vnum ] )
	   count++;

    if ( count < 12 )
        return FALSE;

    proom 		=  pRoomIndex_alloc();
    proom->people	= NULL;
    proom->contents	= NULL;	
    proom->snoop_by = NULL; 
    proom->extra_descr	= NULL;	
    proom->area		= pArea;
    strncpy( proom->name, "Large Stairwell", 80 );
    proom->description	= str_dup ( "   The large wooden stairwell leads to the next floor of the apartment\n\r building.  The highly polished handrails glow from the light of the\n\r torches which line the walls.\n\r" );
    proom->room_flags 	= ROOM_INDOORS|ROOM_SAFE|ROOM_NO_MOB;
    proom->room_flags_2	= ROOM_SAVE;
    proom->sector_type 	= 0;
    proom->size		= 3;
    proom->light	= 45;
    for ( door = 0; door <= 5; door++)
        {proom->exit[door] = NULL;}	 
   /* snarf a vnum */
    for ( vnum = pArea->lvnum ; free_room[vnum] ; vnum++ );
    proom->vnum = vnum; 
    free_room[vnum] = 1;

  /* Put the room in the Database */
    iHash		= vnum % MAX_KEY_HASH;
    proom->next		= room_index_hash[iHash];
    room_index_hash[iHash]	= proom;

    in_room = proom;

    for ( i = 1 ; i < 12 ; i++ )
    {
	/* build the hallway */

    proom 		= pRoomIndex_alloc();
    proom->people	= NULL;
    proom->contents	= NULL;	
    proom->snoop_by = NULL; 
    proom->extra_descr	= NULL;	
    proom->area		= pArea;
    if ( i != 11 )
    {
        strncpy( proom->name, "A Wide Hallway", 80 );
        proom->description	= str_dup ( "   A long brown carpet runs the length of the floor.  Torches line\n\rthe walls and illuminate the rough wooden walls.\n\r" );
    }
    else
    {
         strncpy( proom->name, "Large Stairwell", 80 );
         proom->description	= str_dup ( "   The large wooden stairwell leads to the next floor of the apartment\n\r building.  The highly polished handrails glow from the light of the\n\r torches which line the walls.\n\r" );
    } 
    proom->room_flags 	= ROOM_INDOORS|ROOM_SAFE|ROOM_NO_MOB;
    proom->room_flags_2	= ROOM_SAVE;
    proom->sector_type 	= 0;
    proom->size		= 3;
    proom->light	= 45;
    for ( door = 0; door <= 5; door++)
        {proom->exit[door] = NULL;}	 
   /* snarf a vnum */
    for ( vnum = pArea->lvnum ; free_room[vnum] ; vnum++ );
    proom->vnum = vnum; 
    free_room[vnum] = 1;

    if ( i == 5 )
    {
        pexit_here = new_exit();
        pexit_here->description[0] = '\0';
        pexit_here->keyword[0] = '\0';
        pexit_here->tracks = NULL;
        pexit_here->trap = NULL;
        pexit_here->mes_to_char[0] = '\0';
        pexit_here->mes_to_room[0] = '\0';
        pexit_here->mes_at_room[0] = '\0';
        pexit_here->lock = NULL;
        pexit_here->exit_info = 0;
        pexit_here->exit_info_default = 0;
        pexit_here->u1.vnum = vnum;
        pexit_here->u1.to_room = proom;
        room->exit[ rev_dir[ dir ] ] = pexit_here;
        pexit_there = new_exit();
        pexit_there->description[0] = '\0';
        pexit_there->keyword[0] = '\0';
        pexit_there->tracks = NULL;
        pexit_there->trap = NULL;
        pexit_there->mes_to_char[0] = '\0';
        pexit_there->mes_to_room[0] = '\0';
        pexit_there->mes_at_room[0] = '\0';
        pexit_there->lock = NULL;
        pexit_there->exit_info = 0;
        pexit_there->exit_info_default = 0;
        pexit_there->u1.vnum = room->vnum;
        pexit_there->u1.to_room = room;
        proom->exit[ dir ] = pexit_there;
    }

    pexit_here = new_exit();
    pexit_here->description[0] = '\0';
    pexit_here->keyword[0] = '\0';
    pexit_here->tracks = NULL;
    pexit_here->trap = NULL;
    pexit_here->mes_to_char[0] = '\0';
    pexit_here->mes_to_room[0] = '\0';
    pexit_here->mes_at_room[0] = '\0';
    pexit_here->lock = NULL;
    pexit_here->exit_info = 0;
    pexit_here->exit_info_default = 0;
    pexit_here->u1.vnum = vnum;
    pexit_here->u1.to_room = proom;
    in_room->exit[ floor_dir ] = pexit_here;
    pexit_there = new_exit();
    pexit_there->description[0] = '\0';
    pexit_there->keyword[0] = '\0';
    pexit_there->tracks = NULL;
    pexit_there->trap = NULL;
    pexit_there->mes_to_char[0] = '\0';
    pexit_there->mes_to_room[0] = '\0';
    pexit_there->mes_at_room[0] = '\0';
    pexit_there->lock = NULL;
    pexit_there->exit_info = 0;
    pexit_there->exit_info_default = 0;
    pexit_there->u1.vnum = in_room->vnum;
    pexit_there->u1.to_room = in_room;
    proom->exit[ rev_dir[ floor_dir ] ] = pexit_there;

  /* Put the room in the Database */
    iHash		= vnum % MAX_KEY_HASH;
    proom->next		= room_index_hash[iHash];
    room_index_hash[iHash]	= proom;

    in_room = proom;
    }
    return TRUE;
}

void make_appartment( CHAR_DATA * ch, ROOM_INDEX_DATA * in_room, int direction, int number ) 
{
    char buf[MAX_STRING_LENGTH];
    AREA_DATA * pArea = ch->in_room->area;
    OBJ_INDEX_DATA * house_key;
    EXIT_DATA * pexit_here;
    OBJ_DATA * obj;
    RESET_DATA * pReset;
    EXIT_DATA * pexit_there;
    ROOM_INDEX_DATA * proom;
    int iHash,door;
    long vnum;


    /* make a key for the front door */

    house_key = new_obj_index();
    house_key->in_area = pArea;  
    house_key->material = 0;
    house_key->condition = 100;
    house_key->extra_descr = NULL;
    house_key->extra_flags = ITEM_MAGIC;
    house_key->next = NULL;
    strncpy( house_key->name, "small apartment brass key", 30 );
    strncpy( house_key->short_descr, "A small brass apartment key", 80 );
    strncpy( house_key->description, "A small brass apartment key", 80 );
    house_key->wear_data = wear_alloc();
    house_key->wear_data->wear_flags = ITEM_TAKE|ITEM_HOLD; 
    house_key->wear_data->size = ch->size;
    house_key->wear_data->affected = NULL;
    house_key->wear_data->wear_loc = WEAR_NONE;
    house_key->wear_data->weight = 1;
    house_key->wear_data->ac = 0;
    house_key->wear_data->cost = 5000;
    for ( vnum = pArea->lvnum ; free_obj_list[vnum] && vnum < pArea->uvnum ; vnum++ );

    if ( vnum  > pArea->uvnum )
    {
	send_to_char( "Sorry no vacancy.\n\r", ch );
	return;
    }

    house_key->vnum = vnum;
    vnum = house_key->vnum % MAX_KEY_HASH;
    house_key->next = obj_index_hash[ vnum ];
    obj_index_hash[ vnum ] = house_key;
    free_obj_list[ house_key->vnum ] = 1;

    /* Now make the house */

    proom 		= pRoomIndex_alloc();
    proom->people	= NULL;
    proom->contents	= NULL;	
    proom->snoop_by = NULL; 
    proom->extra_descr	= NULL;	
    proom->area		= pArea;
    proom->size		= 3;
    sprintf(buf,"%s's Apartment", ch->name );
    strncpy( proom->name, buf, 80 );
    sprintf(buf,"%s%s%s%s%s",
	"You are in a small one room apartment.  In the corner is a bed, with a large window next to it.  A large rug covers the wooden floor, except near the door, where the floor is bare.  Many of "
	, ch->name 
	,"'s personal items can be seen throughout the room.  On the "
	, dir_name[ rev_dir[ direction ] ] 
	," side of the home is the apartment door with a rack of coat hooks next to it.");
    proom->description	=  format_string( str_dup( str_dup( buf ) ) );
    proom->room_flags 	= ROOM_INDOORS|ROOM_NO_MOB;
    proom->room_flags_2	= ROOM_SAVE;
    proom->sector_type 	= 0;
    proom->light	= 45;
    for ( door = 0; door <= 5; door++)
        {proom->exit[door] = NULL;}	 
   /* snarf a vnum */
    for ( vnum = pArea->lvnum ; free_room[vnum] && vnum < pArea->uvnum ; vnum++ );

    if ( vnum  > pArea->uvnum )
    {
	send_to_char( "Sorry no vacancy.\n\r", ch );
	return;
    }

    proom->vnum = vnum; 
    free_room[vnum] = 1;
    ch->home = vnum;    

    pexit_here = new_exit();
    sprintf(buf,"A large wooden door with the number %d on it.\n\r", number );
    strncpy( pexit_here->description, buf, 80 );
    strncpy( pexit_here->keyword, "door wooden large", 30 ); 
    pexit_here->tracks = NULL;
    pexit_here->trap = NULL;
    pexit_here->mes_to_char[0] = '\0';
    pexit_here->mes_to_room[0] = '\0';
    pexit_here->mes_at_room[0] = '\0';
    pexit_here->lock = lock_alloc();
    pexit_here->lock->next = NULL;
    pexit_here->lock->vnum = house_key->vnum;
    pexit_here->lock->lock_info = LOCK_LOCKED|LOCK_PICKPROOF;
    pexit_here->lock->lock_info_default = LOCK_LOCKED|LOCK_PICKPROOF;
    pexit_here->exit_info = EX_ISDOOR|EX_CLOSED|EX_PASSPROOF|EX_BASHPROOF;
    pexit_here->exit_info_default = EX_ISDOOR|EX_CLOSED|EX_PASSPROOF|EX_BASHPROOF;
    pexit_here->u1.vnum = vnum;
    pexit_here->u1.to_room = proom;
    in_room->exit[direction] = pexit_here;
    pexit_there = new_exit();
    sprintf(buf,"A large wooden door.\n\r");
    strncpy( pexit_there->description, buf, 80 );
    strncpy( pexit_there->keyword, "door large wooden", 30 ); 
    pexit_there->u1.vnum = in_room->vnum;
    pexit_there->tracks = NULL;
    pexit_there->trap = NULL;
    pexit_there->mes_to_char[0] = '\0';
    pexit_there->mes_to_room[0] = '\0';
    pexit_there->mes_at_room[0] = '\0';
    pexit_there->lock = lock_alloc();
    pexit_there->lock->next = NULL;
    pexit_there->lock->vnum = house_key->vnum;
    pexit_there->lock->lock_info = LOCK_LOCKED|LOCK_PICKPROOF;
    pexit_there->lock->lock_info_default = LOCK_LOCKED|LOCK_PICKPROOF;
    pexit_there->exit_info = EX_ISDOOR|EX_CLOSED|EX_PASSPROOF|EX_BASHPROOF;
    pexit_there->exit_info_default = EX_ISDOOR|EX_CLOSED|EX_PASSPROOF|EX_BASHPROOF;
    pexit_there->u1.to_room = in_room;
    proom->exit[ rev_dir[direction] ] = pexit_there;
    in_room->exit[ direction ] = pexit_here;
    pexit_here->u1.to_room = proom;

  /* Put the room in the Database */
    iHash		= vnum % MAX_KEY_HASH;
    proom->next		= room_index_hash[iHash];
    room_index_hash[iHash]	= proom;

    pReset = reset_alloc();
    pReset->arg1	= proom->vnum;
    pReset->arg2	= direction;
    pReset->arg3	= 2;
    pReset->chance 	= 100;
    pReset->command	= 'D';

    if ( pArea->reset_last == NULL )
    {
	pArea->reset_first = pReset;
	pArea->reset_last = pReset;
    }
    else
    {
	pArea->reset_last->next = pReset;
	pArea->reset_last = pReset;
    }

    /* give the owner 2 keys */
    obj = create_object( house_key );
    approve_object(obj);
    obj_to_char( obj, ch );
    obj = create_object( house_key );
    obj_to_char( obj, ch );

    get_char_money( ch, COST_HOUSE, ch->in_room->area->world );

    sprintf (buf,"You now own apartment # %d.\n\r",number);
    send_to_char(buf,ch);

    /* save the house */
    save_rooms( pArea, pArea->filename );
    save_objs( pArea, pArea->filename );
}

bool find_empty_floor( CHAR_DATA * ch, ROOM_INDEX_DATA * in_room, int floor_dir, int floor_num )
{
    ROOM_INDEX_DATA * room = in_room;
    EXIT_DATA * pexit;
    int count = -1;

    while ( ( pexit = room->exit[ floor_dir ] ) != NULL 
	  && ( room = pexit->u1.to_room ) != NULL )
    {
/* check to see if it hits the stairs on the other side. */
	if ( ( pexit = room->exit[ floor_dir ] ) == NULL
	   || pexit->u1.to_room == NULL )
	    continue;

	count += 2;

	if ( ( pexit = room->exit[ ( floor_dir + 1 ) % 4 ] ) == NULL
	  || pexit->u1.to_room == NULL )
	{
	    make_appartment( ch, room, ( floor_dir + 1 ) % 4, floor_num * 100 + count );
	    return TRUE;
	}

	if ( ( pexit = room->exit[ ( floor_dir - 1 ) % 4 ] ) == NULL
	  || pexit->u1.to_room == NULL )
	{
	    make_appartment( ch, room, ( floor_dir - 1 ) % 4, floor_num * 100 + count + 1 );
	    return TRUE;
	}
    }

    return FALSE;
}

void do_buy_appartment ( CHAR_DATA *ch, char * argument )
{
    ROOM_INDEX_DATA * in_room;
    ROOM_INDEX_DATA * room;
    EXIT_DATA * pexit=NULL;
    int floor_dir;
    int floor;
    int door;
    int coin_type = ch->in_room->area->world;
    bool found = FALSE;

    if ( get_money_total( ch, coin_type ) < COST_HOUSE )
    {
	send_to_char( "You cannot afford to buy an apartment.\n\r", ch );
	return;
    }

    if ( ch->home != 0 )
    {
	send_to_char( "You already have a home.\n\r", ch );
	return;
    }

/* find first floor */

    for ( door = 0 ; door < 4 && !found ; door++ )
	if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
	  && pexit->u1.to_room != NULL
	  && pexit->u1.to_room->area == ch->in_room->area )
	    found = TRUE;

    if ( !found )
    {
/* create a first floor */

	floor_dir = 0;
    	for ( door = 0 ; door < 4 ; door++ )
	    if ( ( pexit = ch->in_room->exit[ door ] ) != NULL
	      && pexit->u1.to_room != NULL
	      && pexit->u1.to_room->area != ch->in_room->area )
	         floor_dir = door;

	if ( !make_first_floor( ch->in_room , floor_dir )
	  || ( pexit = ch->in_room->exit[ rev_dir[ floor_dir ] ] ) == NULL
	  || pexit->u1.to_room == NULL )
	{
	    send_to_char( "Sorry no vacancy.\n\r", ch );
	    return;
	}
    }

    found = FALSE;
    in_room = pexit->u1.to_room;

/* find direction of stiars on first floor */     

    for ( door = 0 ; door < 4 && !found ; door++ )
	if ( ( pexit = in_room->exit[ door ] ) != NULL
	  && pexit->u1.to_room != NULL
	  && pexit->u1.to_room != ch->in_room )
	    found = TRUE;

    if ( !found )
    {
	send_to_char( "Sorry out of order.\n\r", ch );
	return;
    }

    door--;
    found = FALSE;
    floor_dir = rev_dir[ door ];
    in_room = pexit->u1.to_room;

    while ( ( pexit = in_room->exit[ door ] ) != NULL
	   && ( in_room = pexit->u1.to_room ) != NULL );

    floor = 1;
    while ( !find_empty_floor( ch, in_room, floor_dir, floor++ ) )
    {
	room = in_room;
	if ( ( pexit = in_room->exit[ DIR_UP ] ) == NULL 
	   || ( in_room = pexit->u1.to_room ) == NULL )
	{
	     if ( !make_floor( room, floor_dir )
	      || ( pexit = room->exit[ DIR_UP ] ) == NULL 
	      || ( in_room = pexit->u1.to_room ) == NULL )
	     {
	         send_to_char( "Sorry no rooms available.\n\r", ch );
	         return;
	     }
	}
    }
}
   
void do_makehome (CHAR_DATA *ch, char *argument)
{
    CHAR_DATA *victim;
    EXIT_DATA *pexit_here,*pexit_there;
    OBJ_DATA *obj;
    OBJ_INDEX_DATA *house_key;
    char arg[MAX_STRING_LENGTH];
    ROOM_INDEX_DATA *proom;
    AREA_DATA *pArea;
    char buf[MAX_STRING_LENGTH]; 
    long vnum;
    int iHash;
    int door;
    int direction;
    pArea = ch->in_room->area;

    argument = one_argument( argument, arg );
    if ( ( victim = get_char_world( ch, arg ) ) == NULL )
    {
	send_to_char("They are not on right now.\n\r", ch );
	return;
    }

    if ( victim->home != 0 || IS_NPC( ch ) )
    {
	send_to_char("They already have a home.\n\r", ch );
	return;
    }

    if ( COST_HOUSE > get_money_total( victim, race_table[ victim->race ][ victim->sex ].world ) )
    {
	send_to_char("They cannot afford it.\n\r", ch );
	return;
    }
    else 
 	get_char_money( victim, COST_HOUSE, race_table[ victim->race ][ victim->sex ].world );

    argument = one_argument (argument,arg);
  
    if ( (direction = get_direction_number (arg) ) == -1)
    {
	send_to_char ("That is an invalid direction.\n\r",ch);
        return;
    }

    if (ch->in_room->exit[direction] != NULL)
    { 
	send_to_char ("A room is allready there.\n\r",ch); 
	return;
    }

    /* make a key for the front door */

    house_key = new_obj_index();
    house_key->in_area = appartment_area;  
    house_key->material = 0;
    house_key->condition = 100;
    house_key->extra_descr = NULL;
    house_key->extra_flags = ITEM_MAGIC;
    house_key->next = NULL;
    strncpy( house_key->name, "small house brass key", 30 );
    strncpy( house_key->short_descr, "A small brass house key", 80 );
    strncpy( house_key->description, "A small brass house key", 80 );
    house_key->wear_data = wear_alloc();
    house_key->wear_data->wear_flags = ITEM_TAKE|ITEM_HOLD; 
    house_key->wear_data->size = victim->size;
    house_key->wear_data->affected = NULL;
    house_key->wear_data->wear_loc = WEAR_NONE;
    house_key->wear_data->weight = 1;
    house_key->wear_data->ac = 0;
    house_key->wear_data->cost = 5000;
    for ( vnum = pArea->lvnum ; free_obj_list[vnum] ; vnum++ );
    house_key->vnum = vnum;
    vnum = house_key->vnum % MAX_KEY_HASH;
    house_key->next = obj_index_hash[ vnum ];
    obj_index_hash[ vnum ] = house_key;
    free_obj_list[ house_key->vnum ] = 1;

    /* Now make the house */

    proom 		= pRoomIndex_alloc();
    proom->people	= NULL;
    proom->contents	= NULL;	
    proom->snoop_by = NULL; 
    proom->extra_descr	= NULL;	
    proom->area		= appartment_area;
    sprintf(buf,"%s's Home", victim->name );
    strncpy( proom->name, buf, 80 );
    sprintf(buf,"%s%s%s%s%s",
	"You are in a small one room house.  In the corner is a bed, with a large\n\rwindow next to it.  A large rug covers the wooden floor, except near the\n\rfireplace, where the floor is bare.  Many of "
	, capitalize( victim->name ) 
	,"'s personal items\n\rcan be seen throughout the room.  On the "
	, dir_name[ rev_dir[ direction ] ] 
	," side of the home is the front door\n\rwith a rack of coat hooks next to it.\n\r");
    proom->description	= str_dup (buf);
    proom->room_flags 	= ROOM_INDOORS|ROOM_SAFE|ROOM_NO_MOB;
    proom->room_flags_2	= ROOM_SAVE;
    proom->sector_type 	= 0;
    proom->light	= 45;
    proom->size		= 3;
    for ( door = 0; door <= 5; door++)
        {proom->exit[door] = NULL;}	 
   /* snarf a vnum */
    for ( vnum = pArea->lvnum; free_room[vnum]; vnum++ ); 
       proom->vnum = vnum; 
    free_room[vnum] = 1;
    victim->home = vnum;    

    pexit_here = new_exit();
    sprintf(buf,"A large wooden door with a small nameplate saying '%s's Home'.\n\r",capitalize( victim->name ) );
    strncpy( pexit_here->description, buf, 80 );
    strncpy( pexit_here->keyword, "door wooden large", 30 ); 
    pexit_here->tracks = NULL;
    pexit_here->trap = NULL;
    pexit_here->mes_to_char[0] = '\0';
    pexit_here->mes_to_room[0] = '\0';
    pexit_here->mes_at_room[0] = '\0';
    pexit_here->lock = lock_alloc();
    pexit_here->lock->next = NULL;
    pexit_here->lock->vnum = house_key->vnum;
    pexit_here->lock->lock_info = LOCK_LOCKED;
    pexit_here->lock->lock_info_default = LOCK_LOCKED;
    pexit_here->exit_info = EX_ISDOOR|EX_CLOSED|EX_PASSPROOF|EX_BASHPROOF;
    pexit_here->exit_info_default = EX_ISDOOR|EX_CLOSED|EX_PASSPROOF|EX_BASHPROOF;
    pexit_here->u1.vnum = vnum;
    pexit_here->u1.to_room = proom;
    ch->in_room->exit[direction] = pexit_here;
    pexit_there = new_exit();
    sprintf(buf,"A large wooden door.\n\r");
    strncpy( pexit_there->description, buf, 80 );
    strncpy( pexit_there->keyword, "door", 30 ); 
    pexit_there->u1.vnum = ch->in_room->vnum;
    pexit_there->tracks = NULL;
    pexit_there->trap = NULL;
    pexit_there->mes_to_char[0] = '\0';
    pexit_there->mes_to_room[0] = '\0';
    pexit_there->mes_at_room[0] = '\0';
    pexit_there->lock = NULL;
    pexit_there->exit_info = EX_ISDOOR|EX_CLOSED;
    pexit_there->exit_info_default = EX_ISDOOR|EX_CLOSED;
    pexit_there->u1.to_room = ch->in_room;
    proom->exit[ rev_dir[direction] ] = pexit_there;
    ch->in_room->exit[ direction ] = pexit_here;
    pexit_here->u1.to_room = proom;

  /* Put the room in the Database */
    iHash		= vnum % MAX_KEY_HASH;
    proom->next		= room_index_hash[iHash];
    room_index_hash[iHash]	= proom;

    /* give the owner 2 keys */
    obj = create_object( house_key );
    approve_object(obj);
    obj_to_char( obj, victim );
    obj = create_object( house_key );
    obj_to_char( obj, victim );

    sprintf (buf,"Home for %s has been created to [%s] vnum: %ld %ld\n\r",victim->name,arg,vnum,house_key->vnum);
    send_to_char(buf,ch);
    send_to_char( "You are now a home owner!", victim );

    /* save the house */
    save_rooms( appartment_area, appartment_area->filename );
    save_objs( appartment_area, appartment_area->filename );
    send_to_char ("Done.\n\r",ch);
}


void do_sellhome (CHAR_DATA *ch, char *argument)
{
  CHAR_DATA *victim;
  char arg[MAX_STRING_LENGTH];
  
  argument = one_argument (argument,arg);
  
  if (arg[0]=='\0')
        { send_to_char ("You Must supply a person who is to get the home.\n\r",ch);
          return;
        }
  if ( (victim = get_char_room(ch,arg)) == NULL)
        { send_to_char ("They are not here.\n\r",ch);
          return;
        }
  if (IS_NPC(victim))
       {send_to_char ("Verry funny, Mobs can't have homes.\n\r",ch);
        return;
       }
  victim->home = ch->in_room->vnum;
  send_to_char ("Welcome to your new home.\n\r",victim);
} 
void do_sane ( CHAR_DATA *ch, char *argument ) 
{ return; 
} 
 
void do_saveout ( CHAR_DATA *ch, char *argument)
{  CHAR_DATA *vch;
   char arg[MAX_STRING_LENGTH];
    
   one_argument(argument,arg);
   if (arg[0] == '\0')
       { send_to_char ("Saveout <Character>",ch);
         return;
       }
   if ( ( vch = get_char_world (ch,arg)) == NULL )
    { send_to_char ("They are allready gone.\n\r",ch); 
      return;
    }
   if (IS_NPC(ch))
    { send_to_char ("You can't Rent a mob out!!!\n\r",ch);
      return; 
    }
   auto_rent(vch);
   return;
}

void do_nuke ( CHAR_DATA *ch, char *argument)
{  CHAR_DATA *vch;
   char arg[MAX_STRING_LENGTH],file[MAX_STRING_LENGTH];
   char name[31];
   one_argument(argument,arg);
   if (arg[0] == '\0')
       { send_to_char ("Saveout <Character>",ch);
         return;
       }
   if ( ( vch = get_char_world (ch,arg)) == NULL )
    { send_to_char ("They are allready gone.\n\r",ch);
      return;
    }
   if (IS_NPC(ch))
    { send_to_char ("You can't Rent a mob out!!!\n\r",ch);
      return;
    }
    strncpy( name, vch->name, 30 );
    send_to_char ("`rEverywhere the atom bombs are dropping...\n\r",vch);
    send_to_char ("Its the end of all humanity...\n\r",vch);
    send_to_char ("No more time for laughs, get a choppin...\n\r",vch);
    send_to_char ("Its time to face your final destiny!\n\rBOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOM!!!!`w\n\r",vch);
    auto_rent(vch);
    sprintf (file,"rm ../player/%s",name);
    system(file);
    sprintf(file,"%s has been NUKED!!!!!!!!!!!!!!!!!!!!!!!",name);
    log_string(file);
    return;
}     
void do_virtual( CHAR_DATA *ch, char *argument )
{
     do_vrtual(ch, ch->in_room->area );
}

void do_vrtual( CHAR_DATA *ch, AREA_DATA *pArea)
{ 
    EVENT_DATA *new_event;
    EVENT_DATA *event;
    EQUIP_DATA *equip;
    EQUIP_DATA *equip_new;
    INSIDE_DATA *inside;
    INSIDE_DATA *inside_new;
    AREA_DATA *pArea_new;
    ROOM_INDEX_DATA *pRoomIndex;
    ROOM_INDEX_DATA *pRoom;
    MOB_INDEX_DATA *pMobIndex;
    MOB_INDEX_DATA *pMob2;
    OBJ_INDEX_DATA *pObjIndex;
    OBJ_INDEX_DATA *pObj;
    char buf[MAX_STRING_LENGTH];
    EXIT_DATA *pexit;
    int num_of_rooms, count, iHash, total_number_rooms, door, z, season, i;
    long x, vnum, vnum_old, first_room;	
    bool found = FALSE;

    if (ch != NULL)  
    pArea = ch->in_room->area; /* Current area of charactor */

    /* This will change to be sent to this function */

    num_of_rooms = pArea->uvnum - pArea->lvnum; /* How many rooms in current area? */
    if (ch != NULL)
    {
    	sprintf(buf,"Rooms: %d\n\r",num_of_rooms);
    	send_to_char(buf,ch);
    }

    for ( x = 50000; x < MAX_VNUM; x+=25 ) /* Carfull not to exceed MAX_VNUM */
    {					   /* 25 is used to increment.. may wish to increase for faster */
   	found = FALSE;
    	total_number_rooms = 0;
    	for ( vnum = x; vnum < num_of_rooms + x -1; vnum++ ) /* Find an available secton */
    	{
	    if ( get_vnum_area( vnum ) != NULL ) /* Ok loop again? */
	    {
	    	found = TRUE;
	    	break;
	    }

	    total_number_rooms++;
    	}

	if (!found) /* Ok we must have found a set of rooms to use */
	    break;	
    }

    if ( found )
    {
	if (ch != NULL)
	send_to_char( "No more rooms!\n\rArea not created.\n\r", ch );
	return;
    }
/* VIRTUAL AREA */	
    pArea_new				=   new_area();
    area_last->next			=   pArea_new;
    area_last				=   pArea_new;
    pArea_new->next			=   NULL;	
    pArea_new->lvnum			= x; /* Can't build in virtual area's */
    pArea_new->uvnum			= x + total_number_rooms; 
    sprintf(buf,"%s (Virtual)",pArea->name);
    strncpy( pArea_new->name, buf, 80 );
    sprintf(buf,"%d (Virtual)",pArea->areanum);
    strncpy( pArea_new->filename, buf, 30 );
    pArea_new->age			= pArea->age;
    pArea_new->max_age			= pArea->max_age;
    pArea_new->max_age_empty		= pArea->max_age_empty;
    pArea_new->world			= pArea->world;
    pArea_new->time_zone		= pArea->time_zone;
    pArea_new->area_flags		= pArea->area_flags;
    pArea_new->security			= -1;  /* No building allowed */
    pArea_new->visible			= pArea->visible;
    pArea_new->area_flags		= pArea->area_flags;
    pArea_new->weather->spring		= pArea->weather->spring;
    pArea_new->weather->summer		= pArea->weather->summer;
    pArea_new->weather->fall		= pArea->weather->fall;
    pArea_new->weather->winter		= pArea->weather->winter;
    pArea_new->weather->season		= pArea->weather->season;
    pArea_new->weather->speed		= pArea->weather->speed;
    pArea_new->weather->mmhg		= pArea->weather->mmhg;
    pArea_new->weather->change		= pArea->weather->change;
    pArea_new->weather->sky		= pArea->weather->sky;
    pArea_new->weather->sunlight	= pArea->weather->sunlight;
    for ( season = 0; season < MAX_SEASON; season++ )
	pArea_new->weather->storm_type[season] = pArea->weather->storm_type[season];

    for ( season = 0; season < MAX_SEASON; season++ )
	for ( z = 0; z < 4; z++ )
	    pArea_new->weather->chance[season][z] = pArea->weather->chance[season][z];
	
    first_room 				= x;
    SET_BIT(pArea_new->area_flags, AREA_VIRTUAL ); /* very important..*/
/* VIRTUAL ROOMS */
    for ( vnum_old = pArea->lvnum; vnum_old < pArea->uvnum; vnum_old++ )
    {
	if ( ( pRoomIndex = get_room_index( vnum_old ) ) != NULL ) /* Maybe more good rooms */
	{
        pRoom			= pRoomIndex_alloc();
        strncpy( pRoom->name, pRoomIndex->name, 80 ); /* ONLY one copy */
        pRoom->description	= pRoomIndex->description; 
        pRoom->extra_descr	= pRoomIndex->extra_descr; 
        pRoom->area		= pArea_new;
	pRoom->trigger_list	= pRoomIndex->trigger_list;
	load_room_triggers(NULL,pRoom);  /* add all the triggers */
	pRoom->room_flags	= pRoomIndex->room_flags;
	pRoom->room_flags_2	= pRoomIndex->room_flags_2;
	pRoom->light		= pRoomIndex->light;
	pRoom->guilds		= pRoomIndex->guilds;
	pRoom->light_adjustment	= pRoomIndex->light_adjustment;
	pRoom->sector_type 	= pRoomIndex->sector_type;
	pRoom->size 		= pRoomIndex->size;
	for (vnum = x ; free_room[vnum]; vnum++ );
	    pRoom->vnum = vnum;
	free_room[vnum] = 1;
	if ( pRoom->vnum > top_room_index ) 
	    top_room_index = pRoom->vnum;

	for ( count = 0; count < 6; count++ )
	    pRoom->exit[count] = NULL;

	for ( count = 0; count < 6; count++ )
	{
	    if ( pRoomIndex->exit[count] != NULL )
	    {
		pRoom->exit[count] = new_exit();
		strncpy( pRoom->exit[count]->description, pRoomIndex->exit[count]->description, 80 );
		strncpy( pRoom->exit[count]->keyword, pRoomIndex->exit[count]->keyword, 30 );
		strncpy( pRoom->exit[count]->mes_to_char, pRoomIndex->exit[count]->mes_to_char, 80 );
		strncpy( pRoom->exit[count]->mes_to_room, pRoomIndex->exit[count]->mes_to_room, 80 );
		strncpy( pRoom->exit[count]->mes_at_room, pRoomIndex->exit[count]->mes_at_room, 80 );
		pRoom->exit[count]->exit_info	 = pRoomIndex->exit[count]->exit_info;
		pRoom->exit[count]->exit_info_default	 = pRoomIndex->exit[count]->exit_info_default;
		if ( pRoomIndex->exit[count]->u1.to_room->area == pArea )
		    pRoom->exit[count]->u1.vnum	 = ( pRoomIndex->exit[count]->u1.to_room->vnum - pArea->lvnum ) + first_room;
		else
		    pRoom->exit[count]->u1.vnum	 =  pRoomIndex->exit[count]->u1.to_room->vnum;
	    }
	}

	if (pRoomIndex->spro != NULL )
	    pRoom->spro	= pRoomIndex->spro;

        iHash                   = vnum % MAX_KEY_HASH;
        pRoom->next             = room_index_hash[iHash];
        room_index_hash[iHash]  = pRoom;
	}

/* VIRTUALIZE MOBILES  ** NOTE ** same loop */

	if ( ( pMobIndex = get_mob_index( vnum_old ) ) != NULL ) /* Maybe more good mobs */
	{ 	
   	pMob2 				= alloc_pMobIndex();  
    	pMob2->spec_fun			= pMobIndex->spec_fun;
    	strncpy( pMob2->spec_name, pMobIndex->spec_name, 30 );
    	pMob2->pShop 			= pMobIndex->pShop;
    	pMob2->gainer_data 		= pMobIndex->gainer_data;
    	pMob2->load_with_area 		= pArea_new;
    	pMob2->trigger_list 		= pMobIndex->trigger_list;
	/* castle code */
	pMob2->castle_area 		= pMobIndex->castle_area;
    	strncpy( pMob2->movement_msg, pMobIndex->movement_msg, 15 );
    	strncpy( pMob2->player_name, pMobIndex->player_name, 30 ); 
    	strncpy( pMob2->short_descr,pMobIndex->short_descr, 80 );
    	strncpy( pMob2->long_descr, pMobIndex->long_descr, 80 );
    	pMob2->description 		= pMobIndex->description;
    	pMob2->follower 		= pMobIndex->follower;
    	pMob2->race 			= pMobIndex->race;
    	pMob2->sex 			= pMobIndex->sex;
    	pMob2->level 			= pMobIndex->level;
    	pMob2->off_flags 		= pMobIndex->off_flags;
    	pMob2->imm_flags 		= pMobIndex->imm_flags;
    	pMob2->res_flags        	= pMobIndex->res_flags;
    	pMob2->vuln_flags        	= pMobIndex->vuln_flags;
    	pMob2->default_pos        	= pMobIndex->default_pos;
    	pMob2->start_pos        	= pMobIndex->start_pos;
    	pMob2->mood         	 	= pMobIndex->mood;
    	pMob2->form         	 	= pMobIndex->form;
    	pMob2->material         	= pMobIndex->material;
    	pMob2->size         	 	= pMobIndex->size;
    	pMob2->weight		 	= pMobIndex->weight;
    	pMob2->height		 	= pMobIndex->height;
    	pMob2->act          	 	= pMobIndex->act;	/* NPC */
    	pMob2->dam_type          	= pMobIndex->dam_type;	/* NPC */
    	pMob2->affected_by  	 	= pMobIndex->affected_by;
    	pMob2->affected_by_2  	 	= pMobIndex->affected_by_2;
    	pMob2->hitroll			= pMobIndex->hitroll;
    	pMob2->hit[0]			= pMobIndex->hit[0];
    	pMob2->hit[1]			= pMobIndex->hit[1];
    	pMob2->hit[2]			= pMobIndex->hit[2];
    	pMob2->hit[3]			= pMobIndex->hit[3];
    	for ( i = 0 ; i < MAX_SECTORS ; i++ )	
		pMob2->valid_sector[ i ] = pMobIndex->valid_sector[ i ];
    	for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    	{
    	    pMob2->mana[0][i]		= pMobIndex->mana[0][i];
    	    pMob2->mana[1][i]		= pMobIndex->mana[1][i];
    	    pMob2->mana[2][i]		= pMobIndex->mana[2][i];
    	    pMob2->mana[3][i]		= pMobIndex->mana[3][i];
    	}
    	pMob2->damage[0]		= pMobIndex->damage[0];
    	pMob2->damage[1]		= pMobIndex->damage[1];
    	pMob2->damage[2]		= pMobIndex->damage[2];
    	pMob2->damage[3]		= pMobIndex->damage[3];
    	pMob2->armor			= pMobIndex->armor;
    	pMob2->act2	        	= pMobIndex->act2;
    	pMob2->gold[0]      	    	= pMobIndex->gold[0];
    	pMob2->gold[1]     		= pMobIndex->gold[1];
    	pMob2->gold[2]      	    	= pMobIndex->gold[2];
    	pMob2->gold[3]      	    	= pMobIndex->gold[3];
	for ( equip = pMobIndex->equip; equip != NULL ; equip = equip->next )
	{
	    equip_new = equip_alloc();
	    equip_new->obj_vnum		= equip->obj_vnum - pArea->lvnum + first_room;
	    equip_new->chance		= equip->chance;
	    equip_new->flags		= equip->flags;

	    equip_new->next		= pMob2->equip;
	    pMob2->equip		= equip_new;
	}
 	
	for (vnum = x ; free_mob_list[vnum]; vnum++ );
	    pMob2->vnum = vnum;
	free_mob_list[vnum] = 1;
 
        iHash                   	= vnum % MAX_KEY_HASH;
        pMob2->next             	= mob_index_hash[iHash];
        mob_index_hash[iHash]  		= pMob2;
	}
/* VIRTUALIZE OBJECTS  ** NOTE ** same loop */

	if ( ( pObjIndex = get_obj_index( vnum_old ) ) != NULL ) 
	{
	    EXTRA_DESCR_DATA *ed,*ed_new; 
 
	    pObj = new_obj_index();
	    pObj->in_area	= pArea_new; 
	    strncpy( pObj->name, pObjIndex->name, 30 );
	    strncpy( pObj->short_descr,pObjIndex->short_descr, 80 );
	    strncpy( pObj->description, pObjIndex->description, 80 );
	    pObj->extra_flags  = pObjIndex->extra_flags;
	    pObj->spec_fun	= pObjIndex->spec_fun;
	    pObj->gate_data 	= NULL;
	    pObj->light_data	= NULL;
	    if ( pObjIndex->light_data != NULL )
	    {
		pObj->light_data = light_alloc();
		pObj->light_data->flags = pObjIndex->light_data->flags;
	 	pObj->light_data->fuel = pObjIndex->light_data->fuel;
		pObj->light_data->max_fuel = pObjIndex->light_data->max_fuel;
		pObj->light_data->brightness = pObjIndex->light_data->brightness;
	    }
	    pObj->cont_data	= NULL;
	    if ( pObjIndex->cont_data != NULL )
	    {
		SPELL_LIST * spells;
		SPELL_LIST * spell;
		LOCK_DATA * lock_list;
		LOCK_DATA * lock;
		RESET_DATA * reset;
		RESET_DATA * pReset;
		RESET_DATA * last_reset=NULL;
	
		pObj->cont_data = cont_alloc();
		pObj->cont_data->capacity  = pObjIndex->cont_data->capacity;
		pObj->cont_data->flags = pObjIndex->cont_data->flags;
	
		if ( pObjIndex->cont_data->trap != NULL )
		{
		    pObj->cont_data->trap = trap_alloc();
		    pObj->cont_data->trap->flags = pObjIndex->cont_data->trap->flags;
	
		    for ( spells = pObjIndex->cont_data->trap->spells ; spells != NULL ; spells = spells->next )
		    {
			spell = spell_alloc();
			spell->level = spells->level;
			spell->spell = spells->spell;
			spell->next = pObj->cont_data->trap->spells;
			pObj->cont_data->trap->spells = spell;
		    }
		}
	
		for ( lock_list = pObjIndex->cont_data->lock ; lock_list != NULL ; lock_list = lock_list->next )
		{
		    lock = lock_alloc();
		    if ( lock_list->vnum < pArea->lvnum || lock_list->vnum > pArea->uvnum )
		    lock->vnum = lock_list->vnum;
		    else
		    lock->vnum = lock_list->vnum - pArea->lvnum + first_room;
		    lock->lock_info = lock_list->lock_info;
		    lock->pick_penalty = lock_list->pick_penalty;
		    lock->next = pObj->cont_data->lock;
		    pObj->cont_data->lock = lock;
		}
	
		for ( reset = pObjIndex->cont_data->inside ; reset != NULL ; reset = reset->next )
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
	    if ( pObjIndex->gate_data != NULL )
	    {
		GATE_DATA * gate;
	
		gate = gate_alloc();
		gate->flags = pObjIndex->gate_data->flags;
		if ( pObjIndex->gate_data->vnum < pArea->lvnum || pObjIndex->gate_data->vnum > pArea->uvnum )
		gate->vnum  = pObjIndex->gate_data->vnum;
		else
		gate->vnum  = pObjIndex->gate_data->vnum - pArea->lvnum + first_room;
		gate->ave_timer = pObjIndex->gate_data->ave_timer;
		pObj->gate_data = gate;
	    }
	    pObj->edible_data = NULL;
	    if ( pObjIndex->edible_data != NULL )
	    {
		SPELL_LIST * spell;
		SPELL_LIST * spells;
	
		pObj->edible_data = edible_alloc();
		pObj->edible_data->type = pObjIndex->edible_data->type;
		pObj->edible_data->flags = pObjIndex->edible_data->flags;
		pObj->edible_data->amount = pObjIndex->edible_data->amount;
		pObj->edible_data->max_amount = pObjIndex->edible_data->max_amount;
	
		for ( spells = pObjIndex->edible_data->spells ; spells != NULL ; spells = spells->next )
		{
		    spell = spell_alloc();
		    spell->spell = spells->spell;
		    spell->level = spells->level;
		    spell->next  = pObj->edible_data->spells;
		    pObj->edible_data->spells = spell;
		}
	    }
	    pObj->wear_data	= NULL;
	    if ( pObjIndex->wear_data != NULL )
	    {
		pObj->wear_data 	 = wear_alloc();
		pObj->wear_data->wear_flags = pObjIndex->wear_data->wear_flags;
		pObj->wear_data->wear_loc = WEAR_NONE;	
		pObj->wear_data->weight = pObjIndex->wear_data->weight;
		pObj->wear_data->size   = pObjIndex->wear_data->size;
	        pObj->wear_data->cost   = pObjIndex->wear_data->cost;
		pObj->wear_data->ac     = pObjIndex->wear_data->ac;
	

		pObj->wear_data->magic_data = NULL;
		if ( pObjIndex->wear_data->magic_data != NULL )
		{
		    SPELL_LIST * spell;
		    SPELL_LIST * spells;
	
		    pObj->wear_data->magic_data = magic_alloc();
		    pObj->wear_data->magic_data->flags = pObjIndex->wear_data->magic_data->flags;
		    pObj->wear_data->magic_data->charges = pObjIndex->wear_data->magic_data->charges;
		    pObj->wear_data->magic_data->max_charges = pObjIndex->wear_data->magic_data->max_charges;
		    pObj->wear_data->magic_data->type = pObjIndex->wear_data->magic_data->type;
		    pObj->wear_data->magic_data->spells = NULL;
	
		    for ( spells = pObjIndex->wear_data->magic_data->spells ; spells != NULL ; spells = spells->next )
		    {
			spell = spell_alloc();
			spell->next = pObj->wear_data->magic_data->spells;
			spell->level = spells->level;
			spell->spell = spells->spell;
			pObj->wear_data->magic_data->spells = spell;
		    }
	
		}

		pObj->wear_data->weapon_data = NULL;
		if ( pObjIndex->wear_data->weapon_data != NULL )
		{
		    pObj->wear_data->weapon_data = weapon_alloc();
		    pObj->wear_data->weapon_data->dice = pObjIndex->wear_data->weapon_data->dice;
		    pObj->wear_data->weapon_data->die = pObjIndex->wear_data->weapon_data->die;
		    pObj->wear_data->weapon_data->type = pObjIndex->wear_data->weapon_data->type;
		    pObj->wear_data->weapon_data->flags = pObjIndex->wear_data->weapon_data->flags;
		    pObj->wear_data->weapon_data->dam_type = pObjIndex->wear_data->weapon_data->dam_type;
		}
	    }
	    pObj->condition    = pObjIndex->condition;
	    pObj->material     = pObjIndex->material;
	    pObj->timer        = pObjIndex->timer;
	    pObj->shop_type    = pObjIndex->shop_type;
	 
	  
	    /* extended desc */

	    pObj->extra_descr = NULL;
	    for (ed = pObjIndex->extra_descr; ed != NULL; ed = ed->next)
	    {
	        ed_new              = new_extra_descr();
 
	        strncpy( ed_new->keyword, ed->keyword, 30 );
	        ed_new->description     = ed->description;
	        ed_new->next            = pObj->extra_descr;
	        pObj->extra_descr      = ed_new;
	    }
	for ( inside = pObjIndex->inside; inside != NULL; inside = inside->next )
	{
	    inside_new = inside_alloc();
	    inside_new->flags		= inside->flags;
	    if ( inside->obj_vnum != 0 )
	    inside_new->obj_vnum	= inside->obj_vnum - pArea->lvnum + first_room;
	    if ( inside->mob_vnum != 0 )
	    inside_new->mob_vnum	= inside->mob_vnum - pArea->lvnum + first_room;
	    inside_new->chance		= inside->chance;
	
	    inside_new->next		= pObj->inside;
	    pObj->inside		= inside_new;
	}
 
	for (vnum = x ; free_obj_list[vnum]; vnum++ );
	    pObj->vnum = vnum;
	free_obj_list[vnum] = 1;
 
        iHash                  = vnum % MAX_KEY_HASH;
        pObj->next             = obj_index_hash[iHash];
        obj_index_hash[iHash]  = pObj;
	}
    }
    count = 0;
    for ( event = pArea->events; event != NULL ; event = event->next )
    {
 	new_event 		= event_alloc();
	new_event->flags	= event->flags;
	new_event->type		= event->type;
	new_event->chance	= event->chance;
	new_event->room_vnum	= event->room_vnum - pArea->lvnum + first_room;
	if ( event->mob_vnum != 0 )
	new_event->mob_vnum	= event->mob_vnum  - pArea->lvnum + first_room;
	if ( event->obj_vnum != 0 )
	new_event->obj_vnum	= event->obj_vnum  - pArea->lvnum + first_room;
	new_event->area		= pArea_new;
	new_event->next		= pArea_new->events;
	pArea_new->events	= new_event;
    }
/* Add the rooms to the exits */ 
    for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
    {
        for ( pRoomIndex  = room_index_hash[iHash];
              pRoomIndex != NULL;
              pRoomIndex  = pRoomIndex->next )
        {
            bool fexit;
 
            fexit = FALSE;
	    if ( pRoomIndex->area == pArea_new) /* Just for this new area */
            for ( door = 0; door <= 5; door++ )
            {
                if ( ( pexit = pRoomIndex->exit[door] ) != NULL )
                {
                    if ( pexit->u1.vnum <= 0 
                    || get_room_index(pexit->u1.vnum) == NULL)
                        pexit->u1.to_room = NULL;
                    else
                    {
                        fexit = TRUE; 
                        pexit->u1.to_room = get_room_index( pexit->u1.vnum );
                    }
                }
            }
        }
    }
    reset_area(pArea_new);
}
void do_jail( CHAR_DATA *ch, char *argument )
{
    CHAR_DATA * victim;
    ROOM_INDEX_DATA *location;

    if ( ( victim = get_char_world( ch, argument ) ) == NULL )
    {
	send_to_char("They are not here",ch);
	return;
    }

    if ( IS_IMMORTAL(victim) )
    {
	send_to_char("You can only jail a NON-IMP!!",ch);
	return;
    }
    	if ( ( location = get_room_index( ED_JAIL_HOUSE ) ) == NULL )
	{
	    send_to_char( "No such location.\n\r", ch );
	    return;
	}
	if ( victim->in_room == NULL )
	{
	send_to_char( "They are in limbo.\n\r", ch );
	return;
	}
        if ( victim->fighting != NULL )
	stop_fighting( victim, TRUE );
    
    char_from_room( victim );
    char_to_room( victim, location );
    act("You have jailed $N!!\n\r", ch, NULL, victim, TO_CHAR);
    act("You have been jailed by $n!!\n\r", ch, NULL, victim, TO_VICT);
    act("$N has been jailed by $n!!\n\r", ch, NULL, victim, TO_ROOM);
}   

/* prefix command: it will put the string typed on each line typed */

void do_prefi (CHAR_DATA *ch, char *argument)
{
    send_to_char("You cannot abbreviate the prefix command.\r\n",ch);
    return;
}

void do_prefix (CHAR_DATA *ch, char *argument)
{
    char buf[MAX_INPUT_LENGTH];

    if (argument[0] == '\0')
    {
	if (ch->prefix[0] == '\0')
	{
	    send_to_char("You have no prefix to clear.\r\n",ch);
	    return;
	}

	send_to_char("Prefix removed.\r\n",ch);
	ch->prefix[0] = '\0';
	return;
    }

    if ( ch->prefix[0] != '\0' )
	sprintf(buf,"Prefix changed to %s.\r\n",argument);
    else
	sprintf(buf,"Prefix set to %s.\r\n",argument);

    strncpy( ch->prefix, argument, 30 );
}
