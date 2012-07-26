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
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include "merc.h"
 
#if !defined(macintosh)
extern  int     _filbuf         args( (FILE *) );
#endif



/*
 * Array of containers read for proper re-nesting of objects.
 */
#define MAX_NEST	100
static	OBJ_DATA *	rgObjNest	[MAX_NEST];


extern long char_index;
extern long obj_index;
extern long top_pcdata;
WIZLIST_DATA *wizlist_data_list;

/* Courtesy of Yaz of 4th Realm */
char *initial( const char *str )
{
    static char strint [ MAX_STRING_LENGTH ];

    strint[0] = LOWER( str[ 0 ] );
    return strint;

}
/*
 * Save a character and inventory.
 * Would be cool to save NPC's too for quest purposes,
 *   some of the infrastructure is provided.
 */
void save_char_obj( CHAR_DATA *ch )
{
    char strsave[MAX_INPUT_LENGTH];
    char buf[MAX_STRING_LENGTH];
    FILE *fp;

    if ( IS_NPC( ch ) )
	return;

    if ( ch->level < 2 )
	return;

    if ( ch->desc != NULL && ch->desc->original != NULL )
	ch = ch->desc->original;

    /* create god log */
    if ( IS_IMMORTAL( ch ) )
    {
        bool found = FALSE;
	WIZLIST_DATA *wizlist;

	for ( wizlist = wizlist_data_list ; wizlist != NULL ; wizlist = wizlist->next )
	{
	    if ( !str_cmp( ch->name, wizlist->name ) )
	    {
		strncpy( wizlist->title, ch->pcdata->imm_title, 30 );
		wizlist->order = IS_OPTIVISION( ch ) ? 1 : IS_IMP( ch ) ? 2 : IS_IMMORTAL( ch ) ? 3 : 4;
	    	found = TRUE;
	    }
	}

	if ( !found )
	{
	    wizlist = wizlist_data_alloc();
	    wizlist->next = wizlist_data_list;
	    wizlist_data_list = wizlist;
	    strncpy( wizlist->name, capitalize(ch->name), 30 );
	    strncpy( wizlist->title, ch->pcdata->imm_title, 30 );
	    wizlist->order = IS_OPTIVISION(ch) ? 1 : IS_IMP(ch) ? 2 : IS_IMMORTAL(ch) ? 3 : 4;
	}	
    }

    sprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial(ch->name), "/", capitalize( ch->name ) );
    if ( ( fp = fopen( PLAYER_TEMP, "w" ) ) == NULL )
    {
	bug( "Save_char_obj: fopen", 0 );
	perror( strsave );
    }
    else
    {
	fwrite_char( ch, fp );
	if ( ch->carrying != NULL )
	    fwrite_obj( ch, ch->carrying, fp, 0 );

	if ( ch->pcdata->corpse != NULL )
	    fwrite_corpse( ch, ch->pcdata->corpse, fp, 0 );

	/* save the pets */
	if ( ch->pet != NULL && ch->pet->in_room == ch->in_room )
	    fwrite_pet( ch->pet, fp, "#PET\n" );

	if ( ch->mount != NULL && ch->mount->in_room == ch->in_room )
	    fwrite_pet( ch->mount, fp, "#MOUNT\n" );

	fprintf( fp, "#END\n" );
    }

    if ( fp )
    	fclose( fp );

     /* move the file */
    sprintf( buf, "mv -f %s %s", PLAYER_TEMP, strsave );
    system( buf );
    sprintf( buf, "chmod u=rw,g=rw,o=r %s", strsave );
    system( buf ); 
    return;
}

/*
 * Write the char.
 */
void fwrite_char( CHAR_DATA *ch, FILE *fp )
{
    CHAR_DATA * mob;
    READ_NOTES * read_notes;
    AFFECT_DATA *paf;
    CHAR_QUESTS *char_quests;
    int sn, i;
    bool fThug = FALSE;
    bool fNight = FALSE;

    fprintf( fp, "#%s\n", IS_NPC(ch) ? "MOB" : "PLAYER"	);

    fprintf( fp, "Name %s~\n",	ch->name		);
    fprintf( fp, "Vers %d\n",   11			);
    if (ch->mate[0] != '\0')
      	fprintf( fp, "Mate  %s~\n",	ch->mate	);
    if (ch->short_descr[0] != '\0')
      	fprintf( fp, "ShD  %s~\n",	ch->short_descr	);
    if( ch->long_descr[0] != '\0' )
	fprintf( fp, "LnD  %s~\n",	ch->long_descr	);
    if (ch->description != NULL )
    	fprintf( fp, "Desc %s~\n",	ch->description	);
    fprintf( fp, "Race %s~\n", race_list[ch->race].name );
    fprintf( fp, "Sex  %d\n",	ch->sex			);
    fprintf( fp, "Levl %d\n",	ch->level		);
    for ( char_quests = ch->char_quests; char_quests != NULL ; char_quests = char_quests->next )
    {
	fprintf( fp, "Char_quests %d %ld\n",char_quests->quest_number, char_quests->flags);	
    }
    if ( ch->pnote != NULL )
    {
	fprintf( fp, "No_sender %s~\n", ch->pnote->sender );
	fprintf( fp, "No_list %s~\n", ch->pnote->to_list );
	fprintf( fp, "No_subject %s~\n", ch->pnote->subject );
	fprintf( fp, "No_text %s~\n", ch->pnote->text );
    }
    
    fprintf( fp, "Bday %d\n", ch->birth_day );
    fprintf( fp, "Bmonth %d\n", ch->birth_month );
    fprintf( fp, "Color %d\n",	ch->color		);
    fprintf( fp, "Colours       %d %d %d %d %d %d %d %d %d %d %d\n",
        ch->colors[0],
        ch->colors[1],
        ch->colors[2],
        ch->colors[3],
        ch->colors[4],
        ch->colors[5],
        ch->colors[6],
        ch->colors[7],
        ch->colors[8],
        ch->colors[9],
        ch->colors[10]);
    fprintf( fp, "Plyd %ld\n",
	ch->played + (int) (current_time - ch->logon)	);
    for ( read_notes = ch->pcdata->read_notes ; read_notes != NULL ; read_notes = read_notes->next )
        fprintf( fp, "Read %d\n", (int) read_notes->date_stamp );
    fprintf( fp, "Changes %d\n", (int) ch->pcdata->last_change );
    fprintf( fp, "Scro %d\n", 	ch->lines		);
    fprintf( fp, "Room %ld\n",
        (  ch->in_room == get_room_index( ROOM_VNUM_LIMBO )
        && ch->was_in_room != NULL )
            ? ch->was_in_room->vnum
            : ch->in_room == NULL ? 3001 : ch->in_room->vnum );

    fprintf( fp, "HV  %d %d %d %d\n",
	ch->hit, ch->max_hit, ch->move, ch->max_move );
    fprintf( fp, "Mana   %d %d   %d %d   %d %d   %d %d   %d %d\n",
	ch->mana[ 0 ],	 ch->max_mana[ 0 ],
	ch->mana[ 1 ],	 ch->max_mana[ 1 ],
	ch->mana[ 2 ],	 ch->max_mana[ 2 ],
	ch->mana[ 3 ],	 ch->max_mana[ 3 ],
	ch->mana[ 4 ],	 ch->max_mana[ 4 ]);
    fprintf( fp, "Prompt %s~\n",	ch->pcdata->prompt );
    if (ch->pcdata->Oprompt[0] != '\0' )
    	fprintf( fp, "Oprompt %s~\n",	ch->pcdata->Oprompt );
   
    /* castle code */
    fprintf( fp, "Castle %d\n",		ch->castle_area );
    fprintf( fp, "InfoAct %ld\n",	ch->infoact );
    fprintf( fp, "Coins %ld %ld %ld %ld\n",	ch->gold[0], ch->gold[1], ch->gold[2], ch->gold[3] );
    fprintf( fp, "Exp  %ld\n",	ch->exp			);
    fprintf( fp, "Kills  %d\n",	ch->kills		);
    fprintf( fp, "Pkills  %d\n",	ch->pkills	);
    fprintf( fp, "Died  %d\n",	ch->died	);
    if (ch->imm_flags != 0)
	fprintf( fp, "Immune %ld\n", 	ch->imm_flags );
    if (ch->res_flags != 0)
	fprintf( fp, "Res %ld\n", 	ch->res_flags );
    if (ch->vuln_flags != 0)
	fprintf( fp, "Vuln %ld\n", 	ch->vuln_flags );
    if (ch->perm_imm != 0)
	fprintf( fp, "Perm_imm %ld\n",	ch->perm_imm );
    if (ch->perm_res != 0)
	fprintf( fp, "Perm_res %ld\n",	ch->perm_res );
    if (ch->perm_vuln != 0)
	fprintf( fp, "Perm_vuln %ld\n",	ch->perm_vuln );
    if (ch->plr != 0)
	fprintf( fp, "Plr  %ld\n",   ch->plr		);
    if (ch->cmd_groups != 0)
	fprintf( fp, "CMD_Groups  %ld\n",   ch->cmd_groups	);
    if (ch->guilds != 0)
	fprintf( fp, "Guilds  %ld\n",   ch->guilds	);
    if (ch->quest_points != 0)
	fprintf( fp, "Quests_points  %d\n",   ch->quest_points	);
    if (ch->room_template != 0)
	fprintf( fp, "Room_template  %ld\n",   ch->room_template	);
    if (ch->plr2 != 0)
	fprintf( fp, "Plr2  %ld\n",   ch->plr2		);
    if (ch->act != 0)
	fprintf( fp, "Act  %ld\n",   ch->act		);
    if (ch->act2 != 0)
	fprintf( fp, "Act2  %ld\n",   ch->act2		);
    if (ch->affected_by != 0)
	fprintf( fp, "AfBy %ld\n",	ch->affected_by	);
    if (ch->affected_by != 0)
	fprintf( fp, "AfBy2 %ld\n",	ch->affected_by_2);
    if ( ch->pcdata->perm_aff != 0 )
	fprintf( fp, "Perm_aff %ld\n",	ch->pcdata->perm_aff );
    if ( ch->pcdata->perm_aff_2 != 0 )
	fprintf( fp, "Perm_aff_2 %ld\n",	ch->pcdata->perm_aff_2 );
    fprintf( fp, "Comm %ld\n",   ch->comm		);
	if ( ch->pcdata->debt != NULL )
	fprintf( fp, "Debt %ld %d %ld %ld %d\n",
		ch->pcdata->debt->amount,
		ch->pcdata->debt->type,
		ch->pcdata->debt->due,	
		ch->pcdata->debt->vnum,
		ch->pcdata->debt->timer);
    for ( mob = char_list ; mob != NULL ; mob = mob->next )
    {
	if ( fThug && fNight )
	    break;

	if ( !fNight && mob->pIndexData != NULL && mob->pIndexData->spec_fun == spec_nightmare && !str_cmp( ch->name, mob->hunting ) )
	{
	    fprintf( fp, "Night %d\n",	mob->level );
	    fNight = TRUE;
	}

	if ( !fThug && mob->pIndexData != NULL && mob->pIndexData->spec_fun == spec_thug && !str_cmp( ch->name, mob->hunting ) )
	{
	    fprintf( fp, "Thug %d\n",	mob->level );
	    fThug = TRUE;
	}
    }
    if (ch->invis_level != 0)
	fprintf( fp, "Invi %d\n", 	ch->invis_level	);
    fprintf( fp, "Pos  %d\n",	
	ch->position == POS_FIGHTING ? POS_STANDING : ch->position );
    if (ch->practice != 0)
    	fprintf( fp, "Prac %d\n",	ch->practice	);
    if (ch->saving_throw != 0)
	fprintf( fp, "Save  %d\n",	ch->saving_throw);
    if (ch->hitroll != 0)
	fprintf( fp, "Hit   %d\n",	ch->hitroll	);
    if (ch->damroll != 0)
	fprintf( fp, "Dam   %d\n",	ch->damroll	);
    fprintf( fp, "AC %d\n", ch->armor );	
    fprintf( fp, "Height  %d\n",	ch->height	);
    fprintf( fp, "Weight  %d\n",	ch->weight	);
    if (ch->wimpy !=0 )
	fprintf( fp, "Wimp  %d\n",	ch->wimpy	);
    fprintf( fp, "Attr %d %d %d %d %d %d\n",
	ch->perm_stat[STAT_STR],
	ch->perm_stat[STAT_INT],
	ch->perm_stat[STAT_WIS],
	ch->perm_stat[STAT_DEX],
	ch->perm_stat[STAT_CON],
	ch->perm_stat[STAT_CHR] );

    fprintf (fp, "AMod %d %d %d %d %d %d\n",
	ch->mod_stat[STAT_STR],
	ch->mod_stat[STAT_INT],
	ch->mod_stat[STAT_WIS],
	ch->mod_stat[STAT_DEX],
	ch->mod_stat[STAT_CON],
	ch->mod_stat[STAT_CHR] );

    if ( IS_NPC(ch) )
    {
	fprintf( fp, "Vnum %ld\n",	ch->pIndexData->vnum	);
    }
    else
    {
	fprintf( fp, "Pass %s~\n",	ch->pcdata->pwd		);
	if (strlen(ch->pcdata->bamfin) != 0)
	    fprintf( fp, "Bin  %s~\n",	ch->pcdata->bamfin);
	if (strlen(ch->pcdata->bamfout) != 0)
		fprintf( fp, "Bout %s~\n",	ch->pcdata->bamfout);
	fprintf( fp, "Titl %s~\n",	ch->pcdata->title	);
	fprintf( fp, "TSex %d\n",	ch->pcdata->true_sex	);
	fprintf( fp, "Dreams %d\n",	ch->pcdata->dreams	);
	fprintf( fp, "Nmares %d\n",	ch->pcdata->nightmares	);
	fprintf( fp, "LLev %ld\n",	ch->pcdata->last_level	);
	fprintf( fp, "HVP %d %d\n", ch->pcdata->perm_hit, 
						   ch->pcdata->perm_move);
	fprintf( fp, "Manap %d %d %d %d %d\n", ch->pcdata->perm_mana[ 0 ],
	    ch->pcdata->perm_mana[ 1 ],
	    ch->pcdata->perm_mana[ 2 ],
	    ch->pcdata->perm_mana[ 3 ],
	    ch->pcdata->perm_mana[ 4 ]);
	fprintf( fp, "Cond %d %d %d\n",
	    ch->pcdata->condition[0],
	    ch->pcdata->condition[1],
	    ch->pcdata->condition[2] );

	for ( i = 0; i < MAX_MINERAL; i++ )
	    fprintf( fp, "Minerals %d\n", ch->pcdata->minerals[i] );

	for ( i = 0; i < MAX_MINERAL; i++ )
	    fprintf( fp, "Forge %d\n", ch->pcdata->forge[i] );

	for ( sn = 0; sn < MAX_SKILL; sn++ )
	{
	    if ( skill_table[sn].name != NULL && ch->pcdata->learned[sn] > 0 )
	    {
		fprintf( fp, "Ski %d %d '%s'\n",
		    ch->pcdata->learned[sn], ( ch->pcdata->teaching[sn] ? 1 : 0 ) ,skill_table[sn].name );
	    }
	    if ( skill_table[sn].name != NULL && ch->pcdata->skill_mod[sn] != 0 )
	    {
		fprintf( fp, "Sm %d '%s'\n",
		    ch->pcdata->skill_mod[sn], skill_table[sn].name );
	    }
	}

    }

    for ( paf = ch->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < -2 || paf->type > MAX_SKILL)
	    continue;
	
	if ( paf->type < 1 )
	fprintf( fp, "AffS %d %3d %3d %3d %3d %ld %d %d\n",
	    paf->type,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector,
	    paf->bit_type,
	    paf->location2
	    );
	else
	fprintf( fp, "AffO '%s' %3d %3d %3d %3d %ld %d %d\n",
	    skill_table[paf->type].name,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector,
	    paf->bit_type,
	    paf->location2
	    );
    }
    fprintf( fp, "Bank %ld\n", ch->bank);
    fprintf( fp, "Home %ld\n", ch->home); 
    fprintf( fp, "Security %d\n", ch->pcdata->security);
    fprintf( fp, "Imm_title %s~\n", ch->pcdata->imm_title);
    if ( ch->pcdata->pcclan != NULL )
    {
       fprintf( fp, "Clan %s~\n",   ch->pcdata->pcclan->clan->name  );
       fprintf( fp, "ClanRank %d\n",ch->pcdata->pcclan->clanrank );
    }
    fprintf( fp, "End\n\n" );
    return;
}

/* write a pet */
void fwrite_pet( CHAR_DATA *pet, FILE *fp, char *type)
{
    AFFECT_DATA *paf;
    
    fprintf(fp,type);
    
    fprintf(fp,"Vnum %ld\n",pet->pIndexData->vnum);
    
    fprintf(fp,"Name %s~\n", pet->name);
    if (pet->short_descr != pet->pIndexData->short_descr)
    	fprintf(fp,"ShD  %s~\n", pet->short_descr);
    if (pet->long_descr != pet->pIndexData->long_descr)
    	fprintf(fp,"LnD  %s~\n", pet->long_descr);
    if (pet->description != pet->pIndexData->description)
    	fprintf(fp,"Desc %s~\n", pet->description);
    if (pet->race != pet->pIndexData->race)
    	fprintf(fp,"Race %s~\n", race_list[pet->race].name);
    fprintf(fp,"Sex  %d\n", pet->sex);
    if (pet->level != pet->pIndexData->level)
    	fprintf(fp,"Levl %d\n", pet->level);
    fprintf(fp, "HV  %d %d %d %d\n",
    	pet->hit, pet->max_hit, pet->move, pet->max_move);   
    fprintf(fp,"Mana   %d %d   %d %d   %d %d   %d %d   %d %d\n",
	pet->mana[ 0 ],	pet->max_mana[ 0 ],
	pet->mana[ 1 ],	pet->max_mana[ 1 ],
	pet->mana[ 2 ],	pet->max_mana[ 2 ],
	pet->mana[ 3 ],	pet->max_mana[ 3 ],
	pet->mana[ 4 ],	pet->max_mana[ 4 ] );
    fprintf(fp,"Coins %ld %ld %ld %ld\n",pet->gold[0],pet->gold[1],pet->gold[2],pet->gold[3]);
    if (pet->exp > 0)
    	fprintf(fp, "Exp  %ld\n", pet->exp);
    	fprintf(fp, "Kills  %d\n", pet->kills);
    if (pet->act != pet->pIndexData->act)
    	fprintf(fp, "Act  %ld\n", pet->act);
    if (pet->affected_by != pet->pIndexData->affected_by)
    	fprintf(fp, "AfBy %ld\n", pet->affected_by);
    if (pet->affected_by_2 != pet->pIndexData->affected_by_2)
    	fprintf(fp, "AfBy2 %ld\n", pet->affected_by_2);
    if (pet->comm != 0)
    	fprintf(fp, "Comm %ld\n", pet->comm);
    fprintf(fp,"Pos  %d\n", pet->position = POS_FIGHTING ? POS_STANDING : pet->position);
    if (pet->saving_throw != 0)
    	fprintf(fp, "Save %d\n", pet->saving_throw);
    if (pet->hitroll != pet->pIndexData->hitroll)
    	fprintf(fp, "Hit  %d\n", pet->hitroll);
    if (pet->height != pet->pIndexData->height )
	fprintf(fp, "Height %d\n", pet->height );
    if (pet->weight != pet->pIndexData->weight )
	fprintf(fp, "Weight %d\n", pet->weight );
    if (pet->damroll != pet->pIndexData->damage[DICE_BONUS])
    	fprintf(fp, "Dam  %d\n", pet->damroll);
    fprintf(fp, "AC %d\n", pet->armor );
    fprintf(fp, "Attr %d %d %d %d %d %d\n",
    	pet->perm_stat[STAT_STR], pet->perm_stat[STAT_INT],
    	pet->perm_stat[STAT_WIS], pet->perm_stat[STAT_DEX],
    	pet->perm_stat[STAT_CON], pet->perm_stat[STAT_CHR]);
    fprintf(fp, "AMod %d %d %d %d %d %d\n",
    	pet->mod_stat[STAT_STR], pet->mod_stat[STAT_INT],
    	pet->mod_stat[STAT_WIS], pet->mod_stat[STAT_DEX],
    	pet->mod_stat[STAT_CON], pet->mod_stat[STAT_CHR]);
    
    for ( paf = pet->affected; paf != NULL; paf = paf->next )
    {
    	if (paf->type < -2 || paf->type >= MAX_SKILL)
    	    continue;
 	if ( paf->type < 1 ) 
    	fprintf(fp, "AffS %d %3d %3d %3d %3d %ld %d %d\n",
	   paf->type,
    	    paf->level, paf->duration, paf->modifier,paf->location,
    	    paf->bitvector,
    	    paf->bit_type,
	    paf->location2);
	else
    	fprintf(fp, "AffO '%s' %3d %3d %3d %3d %ld %d %d\n",
	    skill_table[paf->type].name,
    	    paf->level, paf->duration, paf->modifier,paf->location,
    	    paf->bitvector,
    	    paf->bit_type,
	    paf->location2);
    }
    
    fprintf(fp,"End\n");
    return;
}

void load_perm_rooms( void )
{
    char buf[MAX_INPUT_LENGTH];
    int iNest;
    long vnum;
    FILE *fp;
    ROOM_INDEX_DATA *pRoomIndex;

   for (vnum = 1; vnum < MAX_KEY_HASH; vnum++ )
   {
	
	for ( pRoomIndex = room_index_hash[vnum]; pRoomIndex != NULL; pRoomIndex = pRoomIndex->next )
	{
	    if ( IS_SET(pRoomIndex->room_flags_2, ROOM_SAVE ) )
	    {
    	     sprintf( buf, "%s%ld", SAVEHOMES_DIR,pRoomIndex->vnum );
	     if ( ( fp = fopen(buf, "r" )  ) == NULL )
    	     {
		/* No worries, Nothing in the room to load */
		continue;
    	     }
	     else
	     {
	        for ( iNest = 0 ; iNest < MAX_NEST; iNest++ )
	    	rgObjNest[iNest] = NULL;
	        read_perm_rooms( fp );
	     }
	     if ( fp )
	     fclose(fp);
	    }
	}
    }
}

#if defined(KEY)
#undef KEY
#endif


#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void read_perm_rooms( FILE *fp )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    AFFECT_DATA *af;
    AFFECT_DATA *af_next;
    ROOM_INDEX_DATA *in_room;
    char buf[MAX_INPUT_LENGTH];
    LOCK_DATA *lock_list;
    char *word;
    char temp_name[31];
    int iNest;
    bool fMatch = FALSE;
    bool faffect = TRUE;
    bool fNest;
    bool fVnum;
    bool first;
    bool make_new;    /* update object */

    fVnum = FALSE;
    obj = NULL;
    first = TRUE;  /* used to counter fp offset */
    make_new = FALSE;
    in_room = NULL;

    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    while ( !str_cmp( word, "Guard" ) )
    {
	long vnum;
	MOB_INDEX_DATA * pGuard;
	CHAR_DATA * guard;
	ROOM_INDEX_DATA * pRoom;

	vnum = fread_number( fp );
	if ( ( pGuard = get_mob_index( vnum ) ) == NULL )
	{
	    log_string( "Fread_room (perm save): Guard vnum not found" );
	    fread_to_eol( fp );
	    word = fread_word( fp );
	    continue;
	}
	guard = create_mobile( pGuard );
	vnum = fread_number( fp );
	pRoom = get_room_index( vnum );
	char_to_room( guard, pRoom );

        word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    }
    if (!str_cmp(word,"Room" ))
    {
	in_room = get_room_index(fread_number( fp ));
        word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    }

    if ( !str_cmp( word, "Vnum" ) )
    {
        long vnum;
	first = FALSE; 
 
        vnum = fread_number( fp );
        if ( get_obj_index( vnum )  == NULL )
	{
            bug( "Fread_Perm Rooms: bad vnum %ld.", vnum );
	}
        else
	{
	    obj = create_object(get_obj_index(vnum));
	}
	    
    } 

    if (obj == NULL) 
    {
    	if ( obj_free == NULL )
    	{
	    obj		= (OBJ_DATA *) alloc_perm( sizeof(*obj) );
	    obj_index++;
    	}
    	else
    	{
	    obj		= obj_free;
	    obj_free	= obj_free->next;
    	}

    	*obj		= obj_zero;
    	obj->name[0]		= '\0';
    	obj->short_descr[0]	= '\0';
    	obj->description[0]	= '\0';
    }
    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	if (first)
	    first = FALSE;
	else
	    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '#':
	    if ( !str_cmp( word, "#O" ) )
	    {
	        fMatch = TRUE;
	        read_perm_rooms( fp );
		return;
	    }
	    break;
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if ( !str_cmp( word, "AC" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Affect" ) || !str_cmp(word,"Aff")
	    ||   !str_cmp( word, "AffD"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}

		    paf		= new_affect(FALSE);

		if (!str_cmp(word, "AffD"))
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_rooms: unknown skill.",0);
		    else
		   	paf->type = sn;
		}
		else 
		    paf->type	= fread_number( fp );
		  paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->location2	= fread_number( fp ); 
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffN" ) || !str_cmp( word, "AffO") )
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}

		    paf		= new_affect(FALSE);
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_rooms: unknown skill.",0);
		    else
		   	paf->type = sn;
		}

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp ); 
		if (!str_cmp( word, "AffN" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffR" ) || !str_cmp( word, "AffS") )
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}

		    paf		= new_affect(FALSE);

		paf->type	= fread_number( fp );
		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp ); 
		if (!str_cmp( word, "AffR" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    break; 

	case 'B':
	    break;

	case 'C':
	    KEY( "Condition",	obj->condition,		fread_number( fp ) );
	    if ( !str_cmp( word, "Cost" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->cost = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'D':
	    if ( !str_cmp( word, "Description" ) || !str_cmp( word, "Desc" ) )
	    {
	    	strncpy( obj->description,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'E':

	    if ( !str_cmp( word, "Edible" ) )
	    {
		if ( obj->edible_data == NULL )
		    obj->edible_data = edible_alloc();

		obj->edible_data->flags = fread_number( fp );
		obj->edible_data->amount = fread_number( fp );
		obj->edible_data->max_amount = fread_number( fp );
		obj->edible_data->type = fread_number( fp );

		fMatch = TRUE;
		break;
		
	    }
	    if ( !str_cmp( word, "Espell" ) )
	    {
		int level;
		int sn;

		level = fread_number( fp );
		sn     = skill_lookup( fread_string( fp ) );
		if ( sn < 0 )
		{
		    bug( "Fread_room: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->edible_data == NULL )
			obj->edible_data = edible_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->edible_data->spells;
 		    obj->edible_data->spells = spell;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Enchanted"))
	    {
		obj->enchanted = TRUE;
	 	fMatch 	= TRUE;
		break;
	    }

	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
	    KEY( "ExtF",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe"))
	    {
		EXTRA_DESCR_DATA *ed;

		    ed			= new_extra_descr();

		strncpy( ed->keyword, fread_string( fp ), 30 );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || !fVnum || obj->pIndexData == NULL)
		{
		    bug( "Fread_rooms: incomplete object.", 0 );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
		{
		    if (make_new)
		    {
			int wear;
			
			if ( obj->wear_data == NULL )
			{
			    wear = obj->wear_data->wear_loc;
			    extract_obj(obj);

			    obj = create_object(obj->pIndexData);
		 	    obj->wear_data->wear_loc = wear;
			}
			else
			{
			    extract_obj(obj);

			    obj = create_object(obj->pIndexData);
			}
		    }
		    if ( iNest == 0 || rgObjNest[iNest-1] == NULL )
		    {
			obj_to_room( obj, in_room );
		    }
		    else
		    {
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    }
		    if ( feof( fp ) )
			return;
		    else
			fMatch = TRUE;
		}
	    }
	    break;

	case 'I':
	    break;

	case 'L':
	    if ( !str_cmp( word, "Light" ) )
	    {
		if ( obj->light_data == NULL )
		    obj->light_data = light_alloc();

		obj->light_data->flags = fread_number( fp );
		obj->light_data->fuel = fread_number( fp );
		obj->light_data->max_fuel = fread_number( fp );
		obj->light_data->brightness = fread_number( fp );

		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Lock" ) )
	    {
		fMatch = TRUE;
		lock_list = lock_alloc();
		lock_list->vnum = fread_number( fp );
		lock_list->lock_info = fread_number( fp );
		lock_list->pick_penalty = 0;
		if ( obj->cont_data == NULL )
		    break;
		lock_list->next = obj->cont_data->lock;
		obj->cont_data->lock = lock_list;
	    }
	    if ( !str_cmp( word, "Locks" ) )
	    {
		fMatch = TRUE;
		lock_list = lock_alloc();
		lock_list->vnum = fread_number( fp );
		lock_list->lock_info = fread_number( fp );
		lock_list->pick_penalty = fread_number( fp );
		if ( obj->cont_data == NULL )
		    break;
		lock_list->next = obj->cont_data->lock;
		obj->cont_data->lock = lock_list;
	    }
	    break;
	   
	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
	    	strncpy( obj->name,		fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_rooms: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    if ( !str_cmp( word, "NoMagical" ) )
	    {
		SPELL_LIST * spell;

		if ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL )
		{
		    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = obj->wear_data->magic_data->spells )
		    {
			obj->wear_data->magic_data->spells = spell->next;
			free_spell( spell );
		    }
		    free_magic_data( obj->wear_data->magic_data );
		    obj->wear_data->magic_data = NULL;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "NoIreset" ) )
	    {
		RESET_DATA * ireset;

		if ( obj->cont_data != NULL && obj->cont_data->inside != NULL )
		{
		    for ( ireset = obj->cont_data->inside ; ireset != NULL ; ireset = obj->cont_data->inside )
		    {
			obj->cont_data->inside = ireset->next;
			free_reset_data( ireset );
		    }
		}
		fMatch = TRUE;
		break;
	    }
	    break;

   	case 'M':
	    if ( !str_cmp( word, "Magical" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->magic_data == NULL )
		    obj->wear_data->magic_data = magic_alloc();

		obj->wear_data->magic_data->type = fread_number( fp );
		obj->wear_data->magic_data->flags = fread_number( fp );
		obj->wear_data->magic_data->charges = fread_number( fp );
		obj->wear_data->magic_data->max_charges = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Mate",	obj->material,		fread_number( fp ) );
	    break;
   	case 'O':
	    if ( !str_cmp( word,"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL)
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    if ( !str_cmp( word,"Owner" ) )
	    {
		strncpy( temp_name, fread_string( fp ), 30 );	
		strncpy(obj->owner, temp_name, 30);
		fMatch = TRUE;
		break;
	    }
	    break;
   	case 'R':
	    KEY( "Repaired",	obj->repaired,		fread_number( fp ) );
            if (!str_cmp(word,"Room" ))
    	    {
		in_room = get_room_index(fread_number( fp ));
		fMatch = TRUE;
    	    }
	    break;

	case 'P':
	    break;
	case 'S':
	    if ( !str_cmp( word, "ShortDescr" ) )
	    {
	    	strncpy( obj->short_descr, fread_string( fp ) , 80 );
		fMatch= TRUE;
		break;
	    }
	    KEY( "SPFL",	obj->special_flags,	fread_number( fp ) );
	    if ( !str_cmp( word, "ShD" ) )
	    {
	    	strncpy( obj->short_descr, fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Slash" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Size" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->size = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Stop" ) )
		return;
	    if ( !str_cmp( word, "Spell" ) )
	    {
		int level;
		int sn;

		level = fread_number( fp );
		sn     = skill_lookup( fread_string( fp ) );
/*		if ( sn < 0 )
		{
		    bug( "Fread_perm_rooms: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->wear_data == NULL )
			obj->wear_data = wear_alloc();
		    if ( obj->wear_data->magic_data == NULL )
			obj->wear_data->magic_data = magic_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->wear_data->magic_data->spells;
 		    obj->wear_data->magic_data->spells = spell;
		} */
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		long vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( "Fread_perm_rooms: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    if ( !str_cmp( word, "WearFlags" ) || !str_cmp( word, "WeaF" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "WearLoc" ) || !str_cmp( word, "Wear" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_loc = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Weight" ) || !str_cmp( word, "Wt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->weight = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Wtype" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wflags" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dam_type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdamage" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dice = fread_number( fp );
		obj->wear_data->weapon_data->die  = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	}

	if ( !fMatch )
	{
	    sprintf( buf, "Read_homes: no match : %s", word );
	    bug( buf, 0 );
	    fread_to_eol( fp );
	}
    }
}


void save_perm_rooms( ROOM_INDEX_DATA *pRoomIndex )
{
    CHAR_DATA * ch;
    char buf[MAX_INPUT_LENGTH];
    FILE *fp;
    OBJ_DATA *obj;
    OBJ_DATA *obj_next;

    sprintf(buf, "%s/%ld", SAVEHOMES_DIR, pRoomIndex->vnum );
    if ( ( fp = fopen(buf , "w" ) ) == NULL )
    {
        if ( ( fp = fopen( buf, "w" ) ) == NULL )
	{	
	    bug( "Unable to open room save file", 0 );
   	    return;
	}
    }

    for ( ch = pRoomIndex->people; ch != NULL; ch = ch->next_in_room )
	if ( IS_NPC( ch ) && ch->spec_fun == spec_guard_dog )
	   fprintf( fp, "Guard %ld %ld\n", ch->pIndexData->vnum, pRoomIndex->vnum );

for ( obj = pRoomIndex->contents; obj != NULL; obj = obj_next )
  {
    obj_next = obj->next_content;
    fwrite_corpse( NULL, obj, fp, 0 );
  }
    fprintf( fp, "Stop\n" );
    if ( fp )
    fclose( fp );
    return;
}



/*
 * Write an object and its contents.
 */
void fwrite_obj( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;

    /*
     * Slick recursion to write lists backwards,
     *   so loading them will load in forwards order.
     */
    if ( obj->next_content != NULL )
	fwrite_obj( ch, obj->next_content, fp, iNest );

    /*
     * Castrate storage characters.
     */
    if ( obj->wear_data == NULL )
	return;

    if ( IS_SET( obj->extra_flags, ITEM_NO_SAVE ) )
	return;

    fprintf( fp, "#O\n" );
    fprintf( fp, "Vnum %ld\n",   obj->pIndexData->vnum        );
    if (obj->enchanted)
	fprintf( fp,"Enchanted\n");
    fprintf( fp, "Nest %d\n",	iNest	  	     );

    /* these data are only used if they do not match the defaults */

    if ( obj->name != obj->pIndexData->name)
    	fprintf( fp, "Name %s~\n",	obj->name		     );
    if ( obj->owner[0] != '\0' )
    	fprintf( fp, "Owner %s~\n",	obj->owner		     );
    if ( obj->short_descr != obj->pIndexData->short_descr)
        fprintf( fp, "ShD  %s~\n",	obj->short_descr	     );
    if ( obj->description != obj->pIndexData->description)
        fprintf( fp, "Desc %s~\n",	obj->description	     );
    if ( obj->extra_flags != obj->pIndexData->extra_flags)
        fprintf( fp, "ExtF %ld\n",	obj->extra_flags	     );
    if ( obj->special_flags != obj->pIndexData->special_flags)
        fprintf( fp, "SPFL %ld\n",	obj->special_flags	     );
    if ( obj->material != obj->pIndexData->extra_flags)
	fprintf( fp, "Mate %d\n",	obj->material		     );
    if ( obj->wear_data != NULL )
    {

    if ( obj->pIndexData->wear_data == NULL ||
      obj->wear_data->wear_flags != obj->pIndexData->wear_data->wear_flags)
        fprintf( fp, "WeaF %ld\n",	obj->wear_data->wear_flags   );

    if ( obj->pIndexData->wear_data == NULL ||
      obj->wear_data->weight != obj->pIndexData->wear_data->weight)
        fprintf( fp, "Wt   %d\n",	obj->wear_data->weight		     );

    if ( obj->pIndexData->wear_data == NULL ||
	obj->wear_data->size != obj->pIndexData->wear_data->size)
        fprintf( fp, "Size %d\n",	obj->wear_data->size		     );

    if ( obj->pIndexData->wear_data == NULL ||
	obj->wear_data->ac != obj->pIndexData->wear_data->ac)
        fprintf( fp, "AC %d\n",	obj->wear_data->ac	 );
/*
	if ( obj->wear_data->magic_data != NULL )
	{
	    SPELL_LIST * spell;

	    fprintf( fp, "Magical %d %d %d %d\n", 
		obj->wear_data->magic_data->type,
		obj->wear_data->magic_data->flags,
		obj->wear_data->magic_data->charges,
		obj->wear_data->magic_data->max_charges);

	    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
		fprintf( fp, "Spell %d %s~\n", spell->level, skill_table[ spell->spell ].name );

	}
	else
	    fprintf( fp, "NoMagical\n" );
*/
	if ( obj->wear_data->weapon_data != NULL )
	{
	    fprintf( fp, "Wtype %d\n", obj->wear_data->weapon_data->type );
	    fprintf( fp, "Wflags %ld\n", obj->wear_data->weapon_data->flags );
	    fprintf( fp, "Wdt %d\n", obj->wear_data->weapon_data->dam_type );
	    fprintf( fp, "Wdamage %d %d\n", obj->wear_data->weapon_data->dice,
		obj->wear_data->weapon_data->die );
	}

    }
    if ( obj->condition != obj->pIndexData->condition )
        fprintf( fp, "Condition   %d\n",	obj->condition	     );
    if ( obj->repaired != 0 )
        fprintf( fp, "Repaired %d\n", obj->repaired );

    /* variable data */

    fprintf( fp, "Wear %d\n",  ( obj->wear_data != NULL ? obj->wear_data->wear_loc : WEAR_NONE ) );
    if (obj->timer != 0)
        fprintf( fp, "Time %d\n",	obj->timer	     );
    if ( obj->wear_data != NULL )
        fprintf( fp, "Cost %ld\n",	obj->wear_data->cost		     );
    if ( obj->light_data != NULL )
	fprintf( fp, "Light %ld %d %d %d\n",
	    obj->light_data->flags, obj->light_data->fuel, obj->light_data->max_fuel, obj->light_data->brightness );

    if ( obj->edible_data != NULL )
    {

	fprintf( fp, "Edible %ld %d %d %d\n",
	    obj->edible_data->flags, obj->edible_data->amount, obj->edible_data->max_amount, obj->edible_data->type );
/*
	for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell->next )
	    fprintf( fp, "Espell %d %s~\n", spell->level, skill_table[ spell->spell ].name );
*/
    }

    if ( obj->cont_data != NULL && obj->cont_data->inside == NULL 
      && obj->pIndexData->cont_data != NULL && obj->pIndexData->cont_data->inside != NULL )
	fprintf( fp, "NoIreset\n" );

    if ( obj->wear_data != NULL )
    for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < -2 || paf->type >= MAX_SKILL)
	    continue;
	if (paf->type < 1 )
	fprintf( fp, "AffS %d %d %d %d %d %ld %d %d\n",
	    paf->type,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector,
	    paf->bit_type,
	    paf->location2
	    );
	else
	fprintf( fp, "AffO '%s' %d %d %d %d %ld %d %d\n",
	    skill_table[paf->type].name,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector,
	    paf->bit_type,
	    paf->location2
	    );
    }

    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
    {
	fprintf( fp, "ExDe %s~ %s~\n",
	    ed->keyword, ed->description );
    }

    fprintf( fp, "End\n\n" );

    if ( obj->cont_data != NULL && obj->cont_data->contents != NULL )
	fwrite_obj( ch, obj->cont_data->contents, fp, iNest + 1 );

    return;
}
/*
 * Write a corpse and its contents.
 */
void fwrite_corpse( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest )
{
    ROOM_INDEX_DATA *in_room;
    EXTRA_DESCR_DATA *ed;
    AFFECT_DATA *paf;
    char buf[MAX_INPUT_LENGTH];

    if ( ( get_obj_index( obj->pIndexData->vnum ) ) == NULL )
    {
	sprintf( buf,"Bad vnum for item: %s ( %ld )\n\r", obj->name, obj->pIndexData->vnum );
	bug( buf, 0);
	return;
    }

    if ( ch != NULL )
        fprintf( fp, "#CORPSE\n" );
    else 
        fprintf( fp, "#O\n" );

    if ( ( in_room = get_in_room_obj( obj ) ) != NULL )
        fprintf( fp, "Room %ld\n",   in_room->vnum        );
    else
    {
	fprintf( fp, "Room %ld\n",   race_table[ ch->race ][ ch->sex ].begin );
	sprintf( buf,"Bad room for corpse save: %s\n\r", ch->name );
	bug( buf, 0);
    }
    
    fprintf( fp, "Vnum %ld\n",   obj->pIndexData->vnum        );
    if (obj->enchanted)
	fprintf( fp,"Enchanted\n");

    fprintf( fp, "Nest %d\n",	iNest	  	     );

    /* these data are only used if they do not match the defaults */

    if ( obj->name != obj->pIndexData->name)
    	fprintf( fp, "Name %s~\n",	obj->name		     );

    if ( obj->owner[0] != '\0' )
	fprintf( fp, "Owner %s~\n",	obj->owner		     );

    if ( obj->short_descr != obj->pIndexData->short_descr)
        fprintf( fp, "ShD  %s~\n",	obj->short_descr	     );

    if ( obj->description != obj->pIndexData->description)
        fprintf( fp, "Desc %s~\n",	obj->description	     );

    if ( obj->extra_flags != obj->pIndexData->extra_flags)
        fprintf( fp, "ExtF %ld\n",	obj->extra_flags	     );

    if ( obj->special_flags != obj->pIndexData->special_flags)
        fprintf( fp, "SPFL %ld\n",	obj->special_flags	     );

    if ( obj->material != obj->pIndexData->extra_flags)
	fprintf( fp, "Mate %d\n",	obj->material		     );
	
    if ( obj->wear_data != NULL )
    {
    	if ( obj->pIndexData->wear_data == NULL ||
      	    obj->wear_data->wear_flags != obj->pIndexData->wear_data->wear_flags)
        	fprintf( fp, "WeaF %ld\n",	obj->wear_data->wear_flags   );

    	if ( obj->pIndexData->wear_data == NULL ||
      	    obj->wear_data->weight != obj->pIndexData->wear_data->weight)
        	fprintf( fp, "Wt   %d\n",	obj->wear_data->weight		     );

    	if ( obj->pIndexData->wear_data == NULL ||
	    obj->wear_data->size != obj->pIndexData->wear_data->size)
        	fprintf( fp, "Size %d\n",	obj->wear_data->size		     );

    	if ( obj->pIndexData->wear_data == NULL ||
	    obj->wear_data->ac != obj->pIndexData->wear_data->ac )
        	fprintf( fp, "AC %d\n",	obj->wear_data->ac		     );

	if ( obj->wear_data->magic_data != NULL )
	{
	    SPELL_LIST * spell;

	    fprintf( fp, "Magical %d %ld %d %d\n", 
		obj->wear_data->magic_data->type,
		obj->wear_data->magic_data->flags,
		obj->wear_data->magic_data->charges,
		obj->wear_data->magic_data->max_charges);

	    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = spell->next )
		fprintf( fp, "Spell %d %s~\n", spell->level, skill_table[ spell->spell ].name );

	}
	else
	    fprintf( fp, "NoMagical\n" );

	if ( obj->wear_data->weapon_data != NULL )
	{
	    fprintf( fp, "Wtype %d\n", obj->wear_data->weapon_data->type );
	    fprintf( fp, "Wflags %ld\n", obj->wear_data->weapon_data->flags );
	    fprintf( fp, "Wdt %d\n", obj->wear_data->weapon_data->dam_type );
	    fprintf( fp, "Wdamage %d %d\n", obj->wear_data->weapon_data->dice,
		obj->wear_data->weapon_data->die );
	}
    }

    if ( obj->condition != obj->pIndexData->condition)
        fprintf( fp, "Condition   %d\n",	obj->condition		     );

    if ( obj->repaired != 0 )
	fprintf( fp, "Repaired    %d\n", 	obj->repaired	);

    if ( obj->cont_data != NULL && obj->cont_data->inside == NULL 
      && obj->pIndexData->cont_data != NULL && obj->pIndexData->cont_data->inside != NULL )
	fprintf( fp, "NoIreset\n" );

    /* variable data */

    fprintf( fp, "Wear %d\n",   ( obj->wear_data != NULL ? obj->wear_data->wear_loc : WEAR_NONE ) );
    if (obj->timer != 0)
        fprintf( fp, "Time %d\n",	obj->timer	     );

    if ( obj->wear_data != NULL )
        fprintf( fp, "Cost %ld\n",	obj->wear_data->cost		     );

    if ( obj->light_data != NULL )
	fprintf( fp, "Light %ld %d %d %d\n",
	    obj->light_data->flags, obj->light_data->fuel, obj->light_data->max_fuel, obj->light_data->brightness );

    if ( obj->edible_data != NULL )
    {
	SPELL_LIST * spell;

	fprintf( fp, "Edible %ld %d %d %d\n",
	    obj->edible_data->flags, obj->edible_data->amount, obj->edible_data->max_amount, obj->edible_data->type );

	for ( spell = obj->edible_data->spells ; spell != NULL ; spell = spell->next )
	    fprintf( fp, "Espell %d %s~\n", spell->level, skill_table[ spell->spell ].name );
    }

    if ( obj->wear_data != NULL )
    for ( paf = obj->wear_data->affected; paf != NULL; paf = paf->next )
    {
	if (paf->type < -2 || paf->type >= MAX_SKILL)
	    continue;
	if (paf->type < 1 )
	fprintf( fp, "AffS %d %d %d %d %d %ld %d %d\n",
	    paf->type,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector,
	    paf->bit_type,
	    paf->location2
	    );
	else
	fprintf( fp, "AffO '%s' %d %d %d %d %ld %d %d\n",
	    skill_table[paf->type].name,
	    paf->level,
	    paf->duration,
	    paf->modifier,
	    paf->location,
	    paf->bitvector,
	    paf->bit_type,
	    paf->location2
	    );
    }

    for ( ed = obj->extra_descr; ed != NULL; ed = ed->next )
    {
	fprintf( fp, "ExDe %s~ %s~\n",
	    ed->keyword, ed->description );
    }

    fprintf( fp, "End\n\n" );

    if ( obj->cont_data != NULL && obj->cont_data->contents != NULL )
		fwrite_obj( NULL, obj->cont_data->contents, fp, iNest + 1 );

    return;
}


/*
 * Load a char and inventory into a new ch structure.
 */
bool load_char_obj( DESCRIPTOR_DATA *d, char *name )

{
    static PC_DATA pcdata_zero;
    char strsave[MAX_INPUT_LENGTH];
    CHAR_DATA *ch;
    FILE *fp;
    bool found;
    int stat;
    int sn;
    int i;

    if ( char_free == NULL )
    {
	ch				= (CHAR_DATA * ) alloc_perm( sizeof(*ch) );
	char_index++;
    }
    else
    {
	ch				= char_free;
	char_free			= char_free->next;
    }
    clear_char( ch );

    if ( pcdata_free == NULL )
    {
	ch->pcdata			= (PC_DATA *) alloc_perm( sizeof(*ch->pcdata) );
	top_pcdata++;
    }
    else
    {
	ch->pcdata			= pcdata_free;
	pcdata_free			= pcdata_free->next;
    }
    *ch->pcdata				= pcdata_zero;
    d->character			= ch; 
    ch->desc				= d;
    ch->mob_next_update			= NULL;
    ch->pcdata->deck			= NULL;
    ch->pcdata->hand			= NULL;
    ch->pcdata->played			= NULL;
    ch->pcdata->discarded		= NULL;
    strncpy( ch->name, name, 30 );
    ch->description = &str_empty[0];
    strncpy( ch->pcdata->prompt, "<%h/%Hhp %v/%Vmv> ", 160 );
    ch->pcdata->Oprompt[0]		= '\0';
    ch->mate[0]	 			= '\0';
    ch->short_descr[0] 			= '\0';
    ch->long_descr[0] 			= '\0';
    ch->in_room 			= NULL;
    ch->was_in_room 			= NULL;
    ch->version				= 10;
    ch->hunting[0]			= '\0';
    ch->dislikes[0]			= '\0';
    ch->race				= 1;
    ch->died				= 0;
    ch->infoact				= 0;
    ch->pkills				= 0;
    ch->kills				= 0;
    ch->pet				= 0;
    ch->mount				= NULL;
    ch->played				= 0;
    ch->logon				= current_time;
    ch->affected_by			= 0;
    ch->affected_by_2			= 0;
    ch->armor				= 0;
    /* castle code */
    ch->castle_area			= 0;
    ch->imm_flags			= 0;
    ch->vuln_flags			= 0;
    ch->res_flags			= 0;
    ch->perm_imm			= 0;
    ch->perm_vuln			= 0;
    ch->perm_res			= 0;
    ch->hands_free			= 2;
    ch->act				= 0;
    ch->plr				= PLR_NOSUMMON;
    ch->cmd_groups			= CMD_GROUP_PLAYER;
    ch->guilds				= DEFAULT;
    ch->quest_points			= 0;
    ch->char_quests			= NULL;
    ch->room_template			= 0;
    ch->plr2				= 0;
    ch->act2				= 0;
    ch->comm				= COMM_COMBINE 
					| COMM_PROMPT;
    ch->invis_level			= 0;
    ch->practice			= 0;
    ch->regen_timer			= current_time;
    ch->position			= POS_STANDING;
    ch->lines				= 40;
    ch->hitroll				= 0;
    ch->damroll				= 0;
    ch->move				= 0;
    ch->hit				= 0;
    for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
    {
        ch->mana[i] 			= 0;
	ch->max_mana[i]			= 0;
        ch->pcdata->perm_mana[i]	= 0;
    }
    ch->wimpy			 	= 0;
    ch->color			        = 0;
    ch->timer			        = 0;
    ch->gos_timer		        = 0;
    ch->outfit_timer		        = 0;
    ch->gold[0]			        = 0;
    ch->gold[1]			        = 0;
    ch->gold[2]			        = 0;
    ch->gold[3]			        = 0;
    ch->saving_throw			= 0;
    ch->edit_flags			= 0;
    ch->edit_menu			= 0;
    ch->birth_day			= 0;
    ch->birth_month			= 0;
    RST_PROMPT(ch);
    for ( sn = 0 ; sn < MAX_SKILL ; sn++ )
    {
	ch->pcdata->learned[ sn  ] = 0;
	ch->pcdata->teaching[ sn ] = FALSE;
	ch->pcdata->skill_mod[ sn ] = 0;
    }
    for ( i = 0 ; i < MAX_MINERAL ; i++ )
	ch->pcdata->minerals[ i ] = 0;

    for ( i = 0 ; i < MAX_MINERAL ; i++ )
	ch->pcdata->forge[ i ] = 0;

    ch->pcdata->home_timer		= 0;
    ch->pcdata->confirm_delete		= FALSE;
    ch->pcdata->pwd[0]			= '\0';
    ch->pcdata->bamfin[0]		= '\0';
    ch->pcdata->bamfout[0]		= '\0';
    ch->pcdata->title[0]		= '\0';
    ch->pcdata->imm_title[0]		= '\0';
    for (stat =0; stat < MAX_STATS; stat++)
	ch->perm_stat[stat]		= 13;
    ch->pcdata->corpse			= NULL;
    ch->pcdata->debt			= NULL;
    ch->pcdata->read_notes		= NULL;
    ch->pcdata->last_change     = current_time;
    ch->pcdata->perm_hit		= 0;
    ch->pcdata->perm_move		= 0;
    ch->pcdata->perm_aff		= race_table[ch->race][ch->sex].aff;
    ch->pcdata->perm_aff_2		= race_table[ch->race][ch->sex].aff2;
    ch->pcdata->true_sex		= 0;
    ch->pcdata->dreams			= 0;
    ch->pcdata->nightmares		= 0;
    ch->pcdata->pcclan         		= NULL;
    ch->pcdata->last_level		= 0;
    ch->pcdata->condition[COND_THIRST]	= 48; 
    ch->pcdata->condition[COND_FULL]	= 48;
    ch->pcdata->security		= 9;
    ch->bank				= 0;
    ch->home				= 0;   
    ch->inside_area			= NULL;   
    ch->exp				= 0;   
    ch->size				= 2;
    ch->height				= 0;
    ch->weight				= 0;
    found = FALSE;

/*  #if defined(unix)

     decompress if .gz file exists  

    sprintf( strsave, "%s%s%s%s%s", PLAYER_DIR, initial(name>, "/", capitalize(name),".gz");
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
        if ( fp )
	fclose(fp);
	sprintf(buf,"gzip -dfq %s",strsave);
	system(buf);
    }
    #endif
 */

    sprintf( strsave, "%s%s%s%s", PLAYER_DIR, initial(name), "/", capitalize( name ) );
    if ( ( fp = fopen( strsave, "r" ) ) != NULL )
    {
	int iNest;

	for ( iNest = 0; iNest < MAX_NEST; iNest++ )
	    rgObjNest[iNest] = NULL;

	found = TRUE;
	for ( ; ; )
	{
	    char letter;
	    char *word;

	    letter = fread_letter( fp );
	    if ( letter == '*' )
	    {
		fread_to_eol( fp );
		continue;
	    }

	    if ( letter != '#' )
	    {
		bug( "Load_char_obj: # not found.", 0 );
		break;
	    }

	    word = fread_word( fp );
	    if      ( !str_cmp( word, "PLAYER" ) ) fread_char ( ch, fp );
	    else if ( !str_cmp( word, "OBJECT" ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "CORPSE" ) ) fread_corpse ( ch, fp );
	    else if ( !str_cmp( word, "O"      ) ) fread_obj  ( ch, fp );
	    else if ( !str_cmp( word, "PET"    ) ) fread_pet  ( ch, fp, "PET" );
	    else if ( !str_cmp( word, "MOUNT"    ) ) fread_pet  ( ch, fp, "MOUNT" );
	    else if ( !str_cmp( word, "END"    ) ) break;
	    else
	    {
		bug( "Load_char_obj: bad section.", 0 );
		break;
	    }
	}
        if ( fp )
	fclose( fp );
    }

    /* initialize race */
    if (found)
    {
	FINGER_DATA * finger;
	bool fMatch = FALSE;

	for ( finger = finger_list ; finger != NULL ; finger = finger->next )
	    if ( !str_cmp( ch->name, finger->name ) )
		fMatch = TRUE;

	if ( !fMatch )
	{
	    finger = finger_alloc();
	    strncpy( finger->name, ch->name, 30 );
	    finger->race = ch->race;
	    finger->sex = ch->sex;
	    finger->level = ( IS_IMMORTAL( ch ) ? 61 : ch->level );
	    strncpy( finger->title, IS_IMMORTAL( ch ) ? ch->pcdata->imm_title : ch->pcdata->title, 80 );
	    finger->next = finger_list;
	    finger_list = finger;
	}

	ch->size = race_table[ch->race][ch->sex].size;
	if ( ch->height	== 0 )
	    ch->height = get_height_size( ch->size );
	if ( ch->weight	== 0 )
	    ch->weight = get_weight_size( ch->size );
	ch->dam_type = race_table[ch->race][ch->sex].dam_type; /*punch */
	ch->affected_by = ch->affected_by|ch->pcdata->perm_aff;
	ch->affected_by_2 = ch->affected_by_2|ch->pcdata->perm_aff_2;
	ch->perm_imm	|= race_table[ ch->race ][ ch->sex ].imm;
	ch->perm_res	|= race_table[ ch->race ][ ch->sex ].res;
	ch->perm_vuln	|= race_table[ ch->race ][ ch->sex ].vuln;
	ch->imm_flags	= ch->imm_flags | ch->perm_imm;
	ch->res_flags	= ch->res_flags | ch->perm_res;
	ch->vuln_flags	= ch->vuln_flags | ch->perm_vuln;
	ch->form	= race_table[ch->race][ch->sex].form;
    }

    /* fix levels */
    return found;
}



/*
 * Read in a char.
 */

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}

void fread_char( CHAR_DATA *ch, FILE *fp )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    int temp_int;
    char *temp;
    bool fMatch;
    int mineral = 0;
    int forge = 0;

    for ( ; ; )
    {
	word   = feof( fp ) ? str_dup("End") : fread_word( fp );
	fMatch = FALSE;
	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    KEY( "Act",		temp_int,			fread_number( fp ) );
	    KEY( "Act2",	ch->act2,		fread_number( fp ) );
	    KEY( "AffectedBy",	ch->affected_by,	fread_number( fp ) );
	    KEY( "AfBy",	ch->affected_by,	fread_number( fp ) );
	    KEY( "AfBy2",	ch->affected_by_2,	fread_number( fp ) );
	    KEY( "Arch",	temp_int,			fread_number( fp ) );

	    if (!str_cmp( word, "AC"))
	    {
		ch->armor = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if (!str_cmp(word,"ACs"))
	    {
		int i;
		int t = 0;

		for (i = 0; i < 4; i++)
		   t += fread_number(fp);

		ch->armor = t/4;
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Affect" ) || !str_cmp( word, "Aff" ) 
	    ||   !str_cmp( word, "AffD"))
	    {
		AFFECT_DATA *paf;

		    paf		= new_affect(TRUE);

	  	if (!str_cmp(word,"AffD"))
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_char: unknown skill.",0);
		    else
			paf->type = sn;
	 	}
		else  /* old form */
		    paf->type	= fread_number( fp );
		if (ch->version == 0)
		  paf->level = ch->level;
		else
		  paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		if ( ch->version > 7 )
		paf->location2	= fread_number( fp );
		affect_to_char(ch, paf);
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffN" ) || !str_cmp( word, "AffO" ) )
	    {
		AFFECT_DATA *paf;
		    int sn;

		    paf		= new_affect(TRUE);
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_char: unknown skill.",0);
		    else
			paf->type = sn;

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp );
		if (!str_cmp( word, "AffN" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_char(ch, paf);
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffR" ) || !str_cmp( word, "AffS" ) )
	    {
		AFFECT_DATA *paf;

		    paf		= new_affect(TRUE);

		paf->type	= fread_number( fp );
		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp );
		if (!str_cmp( word, "AffR" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_char(ch, paf);
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrMod"  ) || !str_cmp(word,"AMod"))
	    {
		int stat;
		for (stat = 0; stat < MAX_STATS; stat ++)
		   ch->mod_stat[stat] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "AttrPerm" ) || !str_cmp(word,"Attr"))
	    {
		int stat;

		for (stat = 0; stat < MAX_STATS; stat++)
		{
		    ch->perm_stat[stat] = fread_number(fp);
		}		    	
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'B':
	    KEY( "Bank",	ch->bank,		fread_number( fp ) );
	    if (!str_cmp( word, "Bamfin" ) || !str_cmp( word, "Bin" ) )
	    {
	    	strncpy( ch->pcdata->bamfin,	fread_string( fp ), 160 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Bamfout" ) || !str_cmp( word, "Bout" ) )
	    {
	    	strncpy( ch->pcdata->bamfout,	fread_string( fp ), 160 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Bday",	ch->birth_day,		fread_number( fp ) );
	    KEY( "Bmonth",	ch->birth_month,	fread_number( fp ) );
		break;

	case 'C':
	    /* castle code */
	    if ( !str_cmp( word, "Castle" ) )
	    {
		ch->castle_area = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Cata",	temp_int,			fread_number( fp ) );
        KEY( "Changes", ch->pcdata->last_change, fread_number( fp ) );
	    if ( !str_cmp( word, "Clan" ) )
	    {
		ch->pcdata->pcclan = pcclan_data_alloc();
		ch->pcdata->pcclan->clan = clan_lookup(fread_string( fp ) );
		if ( ch->pcdata->pcclan->clan != NULL && is_name( ch->name, ch->pcdata->pcclan->clan->leader_name ) )
		    ch->pcdata->pcclan->clan->leader = ch;
		if ( ch->pcdata->pcclan->clan != NULL && is_name( ch->name, ch->pcdata->pcclan->clan->sponser_name ) )
		    ch->pcdata->pcclan->clan->sponser = ch;
		fMatch = TRUE;
		break;
	    }
	    if (!str_cmp(word, "Char_quests"))
	    {
		CHAR_QUESTS * char_quests;

		char_quests = char_quests_data_alloc();
		char_quests->quest_number = fread_number(fp);
		char_quests->flags = fread_number(fp);
		char_quests->next = ch->char_quests;
		ch->char_quests = char_quests;
		fMatch = TRUE;        
		break;
	    }
	    KEY( "Clrcs",	temp_int,			fread_number( fp ) );
	    KEY( "Color",	ch->color,		fread_number( fp ) );

            if ( !str_cmp( word, "Colors" ) )
            {
            ch->colors[0] = fread_number( fp );
            ch->colors[1] = fread_number( fp );
            ch->colors[2] = fread_number( fp );
            ch->colors[3] = fread_number( fp );
            ch->colors[4] = fread_number( fp );
            ch->colors[5] = fread_number( fp );
            ch->colors[6] = fread_number( fp );
            ch->colors[7] = fread_number( fp );
            ch->colors[8] = fread_number( fp );
            ch->colors[9] = fread_number( fp );
            fMatch = TRUE;
            break;
            }

            if ( !str_cmp( word, "Colours" ) )
            {
            ch->colors[0] = fread_number( fp );
            ch->colors[1] = fread_number( fp );
            ch->colors[2] = fread_number( fp );
            ch->colors[3] = fread_number( fp );
            ch->colors[4] = fread_number( fp );
            ch->colors[5] = fread_number( fp );
            ch->colors[6] = fread_number( fp );
            ch->colors[7] = fread_number( fp );
            ch->colors[8] = fread_number( fp );
            ch->colors[9] = fread_number( fp );
            ch->colors[10] = fread_number( fp );
            fMatch = TRUE;
            break;
            }

	    if( !str_cmp( word, "City" ) )
	    {
	        fread_number( fp );
		fMatch = TRUE;
		break;
	    }
            if ( ch->pcdata->pcclan != NULL )
                KEY( "ClanRank",      ch->pcdata->pcclan->clanrank,   fread_number( fp ) );

	    if( !str_cmp( word, "Coin" ) )
	    {
	        ch->gold[0]=fread_number( fp );
	        ch->gold[1]=fread_number( fp );
	        ch->gold[2]=fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if( !str_cmp( word, "Coins" ) )
	    {
	        ch->gold[0]=fread_number( fp );
	        ch->gold[1]=fread_number( fp );
	        ch->gold[2]=fread_number( fp );
	        ch->gold[3]=fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Condition" ) || !str_cmp(word,"Cond"))
	    {
		ch->pcdata->condition[0] = fread_number( fp );
		ch->pcdata->condition[1] = fread_number( fp );
		ch->pcdata->condition[2] = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    KEY("Comm",		ch->comm,		fread_number( fp ) ); 
	    KEY( "CMD_Groups",	ch->cmd_groups,		fread_number( fp ) );
          
	    break;

	case 'D':
	    KEY( "Damroll",	ch->damroll,		fread_number( fp ) );
	    KEY( "Dam",		ch->damroll,		fread_number( fp ) );
	    KEY( "Description",	ch->description,	fread_string( fp ) );
	    KEY( "Dreams",	ch->pcdata->dreams,     fread_number( fp ) );
	    KEY( "Desc",	ch->description,	fread_string( fp ) );
	    if (!str_cmp(word,"Debt"))
	    {
		ch->pcdata->debt = debt_data_alloc();

		ch->pcdata->debt->amount = fread_number(fp);
		ch->pcdata->debt->type = fread_number(fp);
		ch->pcdata->debt->due = fread_number(fp);
		ch->pcdata->debt->vnum = fread_number(fp);
		ch->pcdata->debt->timer = fread_number(fp);
		fMatch = TRUE;
		break;
	    }
	    KEY( "Died",	ch->died,		fread_number( fp ) );
	    break;

	case 'E':
	    if ( !str_cmp( word, "End" ) )
		return;
	    if ( !str_cmp( word, "Exp" ) )
	    {
		long old_to_level;
		ch->exp = fread_number(fp);
		if ( ch->version < 9 )
		{
		    old_to_level = exp_table_old[ch->level +1].exp_at_level - ch->exp;
		    ch->exp = exp_table[ch->level+1].exp_at_level - old_to_level;
		}
		fMatch = TRUE;
		break;
	    }
	    KEY( "Eperm",	temp_int,			fread_number( fp ) );
	    KEY( "Eperm1",	temp_int,			fread_number( fp ) );
	    break;

	case 'F':
	    if ( !str_cmp( word, "Forge" ) )
	    {
		ch->pcdata->forge[forge] = fread_number( fp );
		forge++;
		fMatch = TRUE;
		break;
	    }
	    KEY( "Fland",	temp_int,			fread_number( fp ) );
	    KEY( "Frmers",	temp_int,			fread_number( fp ) );
	    break;

	case 'G':
	    KEY( "Gold",	ch->gold[ race_table[ ch->race ][ ch->sex ].world ],		fread_number( fp ) );
	    KEY( "Grain",	temp_int,		fread_number( fp ) );
	    KEY( "Guilds",	ch->guilds, 	fread_number( fp ) );
	    break;
            if ( !str_cmp( word, "Group" )  || !str_cmp(word,"Gr"))
            {
                fread_word( fp ) ;
                fMatch = TRUE;
            }
	    break;

	case 'H':
	    KEY( "Hands_free",	ch->hands_free,		fread_number( fp ) );
	    KEY( "Hitroll",	ch->hitroll,		fread_number( fp ) );
	    KEY( "Height",	ch->height,		fread_number( fp ) );
	    KEY( "Hit",		ch->hitroll,		fread_number( fp ) );
            KEY( "Home",	ch->home,		fread_number( fp ) ); 
	    KEY( "Hrse",	temp_int,		fread_number( fp ) );
	    if ( !str_cmp( word, "HpManaMove" ) || !str_cmp(word,"HMV"))
	    {
		int i, mana, max_mana;

		ch->hit		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		mana 		= fread_number( fp )/2;
		max_mana	= fread_number( fp )/2;
		if ( ch->version > 6 )
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		{
		    ch->mana[i] = mana;
		    ch->max_mana[i] = max_mana;
		}
		else
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		{
		    ch->mana[i] = mana + ch->level * 30;
		    ch->max_mana[i] = max_mana + ch->level * 30;
		}
		
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp(word,"HV"))
	    {
		ch->hit		= fread_number( fp );
		ch->max_hit	= fread_number( fp );
		ch->move	= fread_number( fp );
		ch->max_move	= fread_number( fp );
		fMatch = TRUE;
		break;
	    }

            if ( !str_cmp( word, "HpManaMovePerm" ) || !str_cmp(word,"HMVP"))
            {
		int i, mana;

                ch->pcdata->perm_hit	= fread_number( fp );
		mana = fread_number( fp ) / 3;
		if ( ch->version > 6 )
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
                    ch->pcdata->perm_mana[i] = mana;
		else
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
                    ch->pcdata->perm_mana[i] = mana + ch->level * 30;
                ch->pcdata->perm_move   = fread_number( fp );
                fMatch = TRUE;
                break;
            }
      
            if ( !str_cmp(word,"HVP"))
            {
                ch->pcdata->perm_hit	= fread_number( fp );
                ch->pcdata->perm_move   = fread_number( fp );
                fMatch = TRUE;
                break;
            }
      
	    break;

	case 'I':
	    KEY( "InfoAct",	ch->infoact,		fread_number( fp ) );
	    KEY( "InvisLevel",	ch->invis_level,	fread_number( fp ) );
	    KEY( "Invi",	ch->invis_level,	fread_number( fp ) );
	    if ( !str_cmp( word, "Imm_title" ) )
	    {
	    	strncpy( ch->pcdata->imm_title,	fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Immune",	ch->imm_flags,		fread_number( fp ) );
	    break;

	case 'K':
	    KEY( "Kills",		ch->kills,	fread_number( fp ) );
	    break;

	case 'L':
	    KEY( "Labs",	temp_int,			fread_number( fp ) );
	    KEY( "LastLevel",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "LLev",	ch->pcdata->last_level, fread_number( fp ) );
	    KEY( "Level",	ch->level,		fread_number( fp ) );
	    KEY( "Lev",		ch->level,		fread_number( fp ) );
	    KEY( "Levl",	ch->level,		fread_number( fp ) );
	    KEY( "Locker",	temp_int,			fread_number( fp ) );
	if ( !str_cmp( word, "LongDescr" ) || !str_cmp( word, "LnD" ) )
	{
	    strncpy( ch->long_descr,		fread_string( fp ), 80 );
	    fMatch = TRUE;
	    break;
	}
	    break;

	case 'M':
	    KEY( "Metals",	temp_int,			fread_number( fp ) );
	    KEY( "Miners", 	temp_int,			fread_number( fp ) );
	    KEY( "Mnts",	temp_int,			fread_number( fp ) );
	    if ( !str_cmp( word, "Mate" ) )
	    {
	    	strncpy( ch->mate,		fread_string( fp ), 12 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Minerals" ) )
	    {
		ch->pcdata->minerals[mineral] = fread_number( fp );
		mineral++;
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Mana" ) )
	    {
		int i;

		if ( ch->version > 6 )
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		{
		    ch->mana[i] = fread_number( fp );
		    ch->max_mana[i] = fread_number( fp );
		}
		else
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		{
		    ch->mana[i] = fread_number( fp );
		    ch->max_mana[i] = fread_number( fp );
		    ch->max_mana[i] = ch->max_mana[i] + ch->level * 30 ;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Manap" ) )
	    {
		int i;

		if (ch->version > 6 )
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		    ch->pcdata->perm_mana[i] = fread_number( fp );
		else
		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		{
		    ch->pcdata->perm_mana[i] = fread_number( fp );
		    ch->pcdata->perm_mana[i] = ch->pcdata->perm_mana[i] + ch->level * 30;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
		strncpy( ch->name, fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Nmares",	ch->pcdata->nightmares,   fread_number( fp ) );
	    if ( !str_cmp( word, "Night" ) )
	    {
		int level;
		
		level = fread_number( fp );

		create_nightmare_mob( ch, level );
	    }
	    if ( !str_cmp( word, "Note" ) )
	    {
		NOTE_DATA * pnote;
		READ_NOTES * read_notes;
		time_t	date;

		date = fread_number( fp );
		for ( pnote = note_list ; pnote != NULL ; pnote = pnote->next )
		{
		    if ( pnote->date_stamp > date )
			continue;;

		     read_notes = read_notes_alloc();
		     read_notes->date_stamp = pnote->date_stamp;
		     read_notes->next = ch->pcdata->read_notes;
		     ch->pcdata->read_notes = read_notes;
		}
	        fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "No_sender" ) )
	    {
		note_attach( ch );
		strncpy( ch->pnote->sender, fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "No_list" ) )
	    {
		note_attach( ch );
		strncpy( ch->pnote->to_list, fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "No_subject" ) )
	    {
		note_attach( ch );
		strncpy( ch->pnote->subject, fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "No_text" ) )
	    {
		note_attach( ch );
		ch->pnote->text		= fread_string( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'O':
	    if ( !str_cmp( word, "Oprompt" ) )
	    {
		strncpy( ch->pcdata->Oprompt, fread_string( fp ), 160 );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'P':
	    if ( !str_cmp( word, "Prompt" ) )
	    {
		strncpy( ch->pcdata->prompt, fread_string( fp ), 160 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Pwd" ) || !str_cmp( word, "Pass" ) )
	    {
	    	strncpy( ch->pcdata->pwd,	fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Perm_aff",	ch->pcdata->perm_aff,	fread_number( fp ) );
	    KEY( "Perm_aff_2",	ch->pcdata->perm_aff_2,	fread_number( fp ) );
	    KEY( "Perm_imm", 	ch->perm_imm,		fread_number( fp ) );
	    KEY( "Perm_res", 	ch->perm_res,		fread_number( fp ) );
	    KEY( "Perm_vuln", 	ch->perm_vuln,		fread_number( fp ) );
	    KEY( "Pike",	temp_int,			fread_number( fp ) );
	    KEY( "Pkills",	ch->pkills,		fread_number( fp ) );
	    KEY( "Played",	ch->played,		fread_number( fp ) );
	    KEY( "Plyd",	ch->played,		fread_number( fp ) );
	    KEY( "Plr",		ch->plr,		fread_number( fp ) );
	    KEY( "Plr2",	ch->plr2,		fread_number( fp ) );
	    KEY( "Points",	temp_int,			fread_number( fp ) );
	    KEY( "Pnts",	temp_int,			fread_number( fp ) );
	   /* KEY( "Position",	ch->position,		fread_number( fp ) ); */
	    if ( !str_cmp( word, "Pos" ) )
	    {
		ch->position = fread_number (fp );
		if ( ch->position > POS_STANDING )
		ch->position = POS_STANDING;
		fMatch = TRUE;
		break;
	    }	

	   /* KEY( "Pos",		ch->position,		fread_number( fp ) );*/
	    KEY( "Practice",	ch->practice,		fread_number( fp ) );
	    KEY( "Prac",	ch->practice,		fread_number( fp ) );
	    break;
	case 'Q':
	    KEY( "Quests_points",	ch->quest_points, 	fread_number( fp ) );
	    break;

	case 'R':
	    KEY( "Raids",	temp_int,			fread_number( fp ) );
	    KEY( "Res",		ch->res_flags,		fread_number( fp ) );
	    if ( !str_cmp( word, "Race" ) )
	    {
		if ( ( ch->race = race_lookup(fread_string( fp ) ) ) == -1 )
		    ch->race = 4;
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Read" ) )
	    {
		READ_NOTES * read_notes;
		NOTE_DATA * pnote;
		time_t date;
		bool found = FALSE;

		date = fread_number( fp );
		for ( pnote = note_list ; pnote != NULL && !found ; pnote = pnote->next )
		    if ( pnote->date_stamp == date )
			found = TRUE;

		if ( found )
		{
		    read_notes = read_notes_alloc();
		    read_notes->date_stamp = date;
		    read_notes->next = ch->pcdata->read_notes;
		    ch->pcdata->read_notes = read_notes;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Room" ) )
	    {
		ch->in_room = get_room_index( fread_number( fp ) );
		if ( ch->in_room == NULL )
		    ch->in_room = get_room_index( ROOM_VNUM_LIMBO );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Room_template",	ch->room_template, 	fread_number( fp ) );

	    break;

	case 'S':
	    KEY( "SavingThrow",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Save",	ch->saving_throw,	fread_number( fp ) );
	    KEY( "Scro",	ch->lines,		fread_number( fp ) );
	    KEY( "Sex",		ch->sex,		fread_number( fp ) );
	    if ( !str_cmp( word, "ShortDescr" ) )
	    {
	    	strncpy( ch->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "ShD" ) )
	    {
	    	strncpy( ch->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "Smths",	temp_int,		fread_number( fp ) );
	    KEY( "Spies",	temp_int,		fread_number( fp ) );

	    if ( !str_cmp( word, "Security" ) )
	    {
		ch->pcdata->security = fread_number (fp );
		fMatch = TRUE;
		if ( ch->version < 9 ) /* Need to reset everyone */
		ch->pcdata->security = 9;
		break;
	    }	
	    if ( !str_cmp( word, "Sector" ) )
	    {
		fread_number (fp );
		fread_number (fp );
		fread_number (fp );
		fread_number (fp );
		fMatch = TRUE;
		break;
	    }	

	    if ( !str_cmp( word, "Skill" ) || !str_cmp(word,"Sk"))
	    {
		int sn;
		int value;

		value = fread_number( fp );
		temp = fread_word( fp ) ;
		sn = skill_lookup(temp);
		if ( sn < 0 )
		{
		    fprintf(stderr,"%s",temp);
		    bug( "Fread_char: unknown skill. ", 0 );
		}
		else
		    ch->pcdata->learned[sn] = value;
		fMatch = TRUE;
	    }

	    if ( !str_cmp(word,"Ski"))
	    {
		int sn;
		int value,teach;

		value = fread_number( fp );
		teach = fread_number( fp );
		temp = fread_word( fp ) ;
		sn = skill_lookup(temp);
		if ( sn < 0 )
		{
		    fprintf(stderr,"%s",temp);
		    bug( "Fread_char: unknown skill. ", 0 );
		}
		else
		{
		    ch->pcdata->teaching[sn] = ( teach == 1 );
		    ch->pcdata->learned[sn] = value;
		}
		fMatch = TRUE;
	    }

	    if ( !str_cmp(word,"Sku"))
	    {
		int sn;
		int value;

		value = fread_number( fp );
		fread_number( fp );
		temp = fread_word( fp ) ;
		sn = skill_lookup(temp);
		if ( sn < 0 )
		{
		    fprintf(stderr,"%s",temp);
		    bug( "Fread_char: unknown skill. ", 0 );
		}
		else
		{
		    ch->pcdata->learned[sn] = value;
		}
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "Sm" ) )
	    {
		int sn;
		int value;

		value = fread_number( fp );
		temp = fread_word( fp ) ;
		sn = skill_lookup(temp);
		if ( sn < 0 )
		{
		    fprintf(stderr,"%s",temp);
		    bug( "Fread_char: unknown skill. ", 0 );
		}
		else
		    ch->pcdata->skill_mod[sn] = value;
		fMatch = TRUE;
	    }

	    break;

	case 'T':
	    KEY( "Tmpls",	temp_int,			fread_number( fp ) );
            KEY( "TrueSex",     ch->pcdata->true_sex,  	fread_number( fp ) );
	    KEY( "TSex",	ch->pcdata->true_sex,   fread_number( fp ) );
	    if ( !str_cmp( word, "Thug" ) )
	    {
		int level;

		level = fread_number( fp );

		create_thug( ch, level );
	    }
	    if ( !str_cmp( word, "Trai" ) )
	    {
		ch->practice +=- fread_number( fp ) * 3 / 2;
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Title" )  || !str_cmp( word, "Titl"))
	    {
		strncpy( ch->pcdata->title, fread_string( fp ), 80 );
    		if (ch->pcdata->title[0] != '.' && ch->pcdata->title[0] != ',' 
		&&  ch->pcdata->title[0] != '!' && ch->pcdata->title[0] != '?')
		{
		    sprintf( buf, " %s", ch->pcdata->title );
		    strncpy( ch->pcdata->title, buf, 80 );
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'V':
	    KEY( "Version",     ch->version,		fread_number ( fp ) );
	    KEY( "Vers",	ch->version,		fread_number ( fp ) );
	    KEY( "Vuln",	ch->vuln_flags,		fread_number( fp ) );
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		ch->pIndexData = get_mob_index( fread_number( fp ) );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    KEY( "Wimpy",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Weight",	ch->weight,		fread_number( fp ) );
	    KEY( "Wimp",	ch->wimpy,		fread_number( fp ) );
	    KEY( "Wizs",	temp_int,			fread_number( fp ) );
	    break;
	}

	if ( !fMatch )
	{
	    bug( "Fread_char: no match.", 0 );
	    fread_to_eol( fp );
	}
    }
}

/* load a pet from the forgotten reaches */
void fread_pet( CHAR_DATA *ch, FILE *fp, char *type )
{
    char *word;
    CHAR_DATA *pet;
    bool fMatch;

    /* first entry had BETTER be the vnum or we barf */
    word = feof(fp) ? str_dup("END") : fread_word(fp);
    if (!str_cmp(word,"Vnum"))
    {
    	long vnum;
    	
    	vnum = fread_number(fp);
    	if (get_mob_index(vnum) == NULL)
	{
    	    bug("Fread_pet: bad vnum %d.",vnum);
	    pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
	}
    	else
    	    pet = create_mobile(get_mob_index(vnum));
    }
    else
    {
        bug("Fread_pet: no vnum in file.",0);
        pet = create_mobile(get_mob_index(MOB_VNUM_FIDO));
    }
    
    for ( ; ; )
    {
    	word 	= feof(fp) ? str_dup("END") : fread_word(fp);
    	fMatch = FALSE;
    	
    	switch (UPPER(word[0]))
    	{
    	case '*':
    	    fMatch = TRUE;
    	    fread_to_eol(fp);
    	    break;
    		
    	case 'A':
    	    KEY( "Act",		pet->act,		fread_number(fp));
    	    KEY( "Act2",	pet->act2,		fread_number(fp));
    	    KEY( "AfBy",	pet->affected_by,	fread_number(fp));
    	    KEY( "AfBy2",	pet->affected_by_2,	fread_number(fp));
	    KEY( "AC",		pet->armor,		fread_number(fp));    	    
    	    if (!str_cmp(word,"ACs"))
    	    {
    	    	int i;
    	    	int t = 0;

    	    	for (i = 0; i < 4; i++)
		    t += fread_number( fp );

		pet->armor = t/4;
    	    	fMatch = TRUE;
    	    	break;
    	    }
    	    if (!str_cmp(word,"AffD"))
    	    {
    	    	AFFECT_DATA *paf;
    	    	int sn;
    	    	
    	    	    paf = new_affect(TRUE);
    	    	
    	    	sn = skill_lookup(fread_word(fp));
    	     	if (sn < 0)
    	     	    bug("Fread_char: unknown skill.",0);
    	     	else
    	     	   paf->type = sn;
    	     	   
    	     	paf->level	= fread_number(fp);
    	     	paf->duration	= fread_number(fp);
    	     	paf->modifier	= fread_number(fp);
    	     	paf->location	= fread_number(fp);
    	     	paf->bitvector	= fread_number(fp);
		if ( ch->version > 7 )
		paf->location2  = fread_number(fp);
		affect_to_char(pet, paf);
    	     	fMatch		= TRUE;
    	     	break;
    	    }
    	    if (!str_cmp(word,"AffN") || !str_cmp(word, "AffO" ))
    	    {
    	    	AFFECT_DATA *paf;
    	    	int sn;
    	    	
    	    	    paf = new_affect(TRUE);
    	    	
    	    	sn = skill_lookup(fread_word(fp));
    	     	if (sn < 0)
    	     	    bug("Fread_char: unknown skill.",0);
    	     	else
    	     	   paf->type = sn;
    	    	
    	     	paf->level	= fread_number(fp);
    	     	paf->duration	= fread_number(fp);
    	     	paf->modifier	= fread_number(fp);
    	     	paf->location	= fread_number(fp);
    	     	paf->bitvector	= fread_number(fp);
    	     	paf->bit_type	= fread_number(fp);
		paf->location2  = fread_number(fp);
		if (!str_cmp( word, "AffN" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_char(pet, paf);
    	     	fMatch		= TRUE;
    	     	break;
    	    }
    	    if (!str_cmp(word,"AffR") || !str_cmp(word, "AffS" ))
    	    {
    	    	AFFECT_DATA *paf;
    	    	
    	    	    paf = new_affect(TRUE);
    	    	
    	     	paf->type	= fread_number(fp);
    	     	paf->level	= fread_number(fp);
    	     	paf->duration	= fread_number(fp);
    	     	paf->modifier	= fread_number(fp);
    	     	paf->location	= fread_number(fp);
    	     	paf->bitvector	= fread_number(fp);
    	     	paf->bit_type	= fread_number(fp);
		paf->location2  = fread_number(fp);
		if (!str_cmp( word, "AffR" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_char(pet, paf);
    	     	fMatch		= TRUE;
    	     	break;
    	    }
    	    if (!str_cmp(word,"AMod"))
    	    {
    	     	int stat;
    	     	
    	     	for (stat = 0; stat < MAX_STATS; stat++)
    	     	    pet->mod_stat[stat] = fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	    }
    	     
    	    if (!str_cmp(word,"Attr"))
    	    {
    	         int stat;
    	         
    	         for (stat = 0; stat < MAX_STATS; stat++)
    	             pet->perm_stat[stat] = fread_number(fp);
    	         fMatch = TRUE;
    	         break;
    	    }
    	    break;
    	     
    	 case 'C':
	    if ( !str_cmp( word, "Coin" ) )
	    {
		pet->gold[0] = fread_number(fp);
		pet->gold[1] = fread_number(fp);
		pet->gold[2] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Coins" ) )
	    {
		pet->gold[0] = fread_number(fp);
		pet->gold[1] = fread_number(fp);
		pet->gold[2] = fread_number(fp);
		pet->gold[3] = fread_number(fp);
		fMatch = TRUE;
		break;
	    }

    	     KEY( "Comm",	pet->comm,		fread_number(fp));
    	     break;
    	     
    	 case 'D':
    	     KEY( "Dam",	pet->damroll,		fread_number(fp));
    	     KEY( "Desc",	pet->description,	fread_string(fp));
    	     break;
    	     
    	 case 'E':
    	     if (!str_cmp(word,"End"))
	     {
		pet->leader = ch;
		add_follower( pet, ch );
		if (!str_cmp(type,"PET"))
		    ch->pet = pet;
		if (!str_cmp(type,"MOUNT"))
		    ch->mount = pet;
    	     	return;
	     }
    	     KEY( "Exp",	pet->exp,		fread_number(fp));
    	     break;
    	     
    	 case 'G':
    	     KEY( "Gold",	pet->gold[ race_table[ pet->race ][ pet->sex ].world ],		 fread_number(fp));
    	     break;
    	     
    	 case 'H':
    	     KEY( "Hit",	pet->hitroll,		fread_number(fp));
    	     KEY( "Height",	pet->height,		fread_number(fp));
    	     
    	     if (!str_cmp(word,"HMV"))
    	     {
		int i, mana, max_mana;

    	     	pet->hit	= fread_number(fp);
    	     	pet->max_hit	= fread_number(fp);
		mana = fread_number( fp );
		max_mana = fread_number( fp );
		for( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		{
		    pet->mana[i] = mana;
		    pet->max_mana[i] = max_mana;
		}
    	     	pet->move	= fread_number(fp);
    	     	pet->max_move	= fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	     }
    	     if (!str_cmp(word,"HV"))
    	     {
    	     	pet->hit	= fread_number(fp);
    	     	pet->max_hit	= fread_number(fp);
    	     	pet->move	= fread_number(fp);
    	     	pet->max_move	= fread_number(fp);
    	     	fMatch = TRUE;
    	     	break;
    	     }
    	     break;
    	     
    	 case 'K':
    	     KEY( "Kills",	pet->kills,		fread_number(fp));
    	     break;

     	case 'L':
    	     KEY( "Levl",	pet->level,		fread_number(fp));
	     if ( !str_cmp( word, "LnD" ) )
	     {
    	     	strncpy( pet->long_descr,	fread_string(fp), 80 );
		fMatch = TRUE;
		break;
	     }
    	     break;
    	     
	case 'M' :
	    if ( !str_cmp( word, "Mana" ) )
	    {
		int i;

		for ( i = 0 ; i < MAX_ELEMENT_TYPE ; i++ )
		{
		    pet->mana[i] = fread_number( fp );
		    pet->max_mana[i] = fread_number( fp );
		}
		fMatch = TRUE;
		break;
	    }
	    break;
    	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
    	    	strncpy( pet->name,		fread_string(fp), 30 );
		fMatch = TRUE;
		break;
	    }
    	     break;
    	     
    	case 'P':
    	     KEY( "Pos",	pet->position,		fread_number(fp));
    	     break;
    	     
	case 'R':
    	    KEY( "Race",	pet->race, race_lookup(fread_string(fp)));
    	    break;
 	    
    	case 'S' :
    	    KEY( "Save",	pet->saving_throw,	fread_number(fp));
    	    KEY( "Sex",		pet->sex,		fread_number(fp));
	    if ( !str_cmp( word, "ShD" ) )
	    {
    	    	strncpy( pet->short_descr,	fread_string(fp), 80 );
		fMatch = TRUE;
		break;
	    }
    	    break;
    	case 'W' :
    	    KEY( "Weight",	pet->weight,	fread_number(fp));
    	    
    	if ( !fMatch )
    	{
    	    bug("Fread_pet: no match.",0);
    	    fread_to_eol(fp);
    	}
    	
    	}
    }
    
}



void fread_corpse( CHAR_DATA *ch, FILE *fp )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    ROOM_INDEX_DATA *in_room;
    AFFECT_DATA *af;
    AFFECT_DATA *af_next;
    bool faffect = TRUE;
    char *word;
    char temp_name[31];
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    bool first;
    bool make_new;    /* update object */
    
    fVnum = FALSE;
    obj = NULL;
    first = TRUE;  /* used to counter fp offset */
    make_new = FALSE;
    in_room = NULL;

    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    if (!str_cmp(word,"Room" ))
    {
	if ( ( in_room = get_room_index(fread_number( fp ) ) ) == NULL 
	  && ( in_room = get_room_index( race_table[ ch->race ][ ch->sex ].begin ) ) == NULL )
	{
	 bug("fread_corpse:  Room is invalid (limbo used)",0);
	 in_room = get_room_index( ROOM_VNUM_LIMBO);
	}
        word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    }
    if (!str_cmp(word,"Vnum" ))
    {
        long vnum;
	first = FALSE;  /* fp will be in right place */
 
        vnum = fread_number( fp );
        if (  get_obj_index( vnum )  == NULL )
	{
            bug( "Fread_corpse: bad vnum %d.", vnum );
	}
        else
	{
	    obj = create_object(get_obj_index(vnum));
	}
	    
    }

    if (obj == NULL)  /* either not found or old style */
    {
    	if ( obj_free == NULL )
    	{
	    obj		= (OBJ_DATA *) alloc_perm( sizeof(*obj) );
	    obj_index++;
    	}
    	else
    	{
	    obj		= obj_free;
	    obj_free	= obj_free->next;
    	}

    	*obj		= obj_zero;
    	obj->name[0]		= '\0';
    	obj->short_descr[0]	= '\0';
    	obj->description[0]	= '\0';
    }

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	if (first)
	    first = FALSE;
	else
	    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	 case 'A':
	    if ( !str_cmp( word, "AC" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Affect" ) || !str_cmp(word,"Aff")
	    ||   !str_cmp( word, "AffD"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}

		    paf		= new_affect(FALSE);

		if (!str_cmp(word, "AffD"))
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_obj: unknown skill.",0);
		    else
		   	paf->type = sn;
		}
		else 
		    paf->type	= fread_number( fp );
		if (ch->version == 0)
		  paf->level = 20;
		else
		  paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		if ( ch->version > 7 ) 
		paf->location2	= fread_number( fp );
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffN" ) || !str_cmp(word,"AffO"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}
		    paf		= new_affect(FALSE);
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_obj: unknown skill.",0);
		    else
		   	paf->type = sn;
		}

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp );
		if (!str_cmp( word, "AffN" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffR" ) || !str_cmp(word,"AffS") )
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}
		    paf		= new_affect(FALSE);

		paf->type	= fread_number( fp );
		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp );
		if (!str_cmp( word, "AffR" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    break;

	case 'B':
	    break;
	case 'C':
	    KEY( "Condition",	obj->condition,		fread_number( fp ) );
	    if ( !str_cmp( word, "Cost" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->cost = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'D':
	    if ( !str_cmp( word, "Description" ) || !str_cmp( word, "Desc" ) )
	    {
	    	strncpy( obj->description,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'E':

	    if ( !str_cmp( word, "Edible" ) )
	    {
		if ( obj->edible_data == NULL )
		    obj->edible_data = edible_alloc();

		obj->edible_data->flags = fread_number( fp );
		obj->edible_data->amount = fread_number( fp );
		obj->edible_data->max_amount = fread_number( fp );
		obj->edible_data->type = fread_number( fp );

		fMatch = TRUE;
		break;
		
	    }
	    if ( !str_cmp( word, "Espell" ) )
	    {
		int level;
		int sn;

		level = fread_number( fp );
		sn     = skill_lookup( fread_string( fp ) );
		if ( sn < 0 )
		{
		    bug( "Fread_perm_rooms: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->edible_data == NULL )
			obj->edible_data = edible_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->edible_data->spells;
 		    obj->edible_data->spells = spell;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Enchanted"))
	    {
		obj->enchanted = TRUE;
	 	fMatch 	= TRUE;
		break;
	    }

	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
	    KEY( "ExtF",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe"))
	    {
		EXTRA_DESCR_DATA *ed;

		    ed			= new_extra_descr();

		strncpy( ed->keyword, fread_string( fp ), 30 );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || !fVnum || obj->pIndexData == NULL)
		{
		    bug( "Fread_corpse: incomplete object.", 0 );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
		{
		    if (make_new)
		    {
			int wear;
			
			if ( obj->wear_data != NULL )
			{
			    wear = obj->wear_data->wear_loc;
			    extract_obj(obj);

			    obj = create_object(obj->pIndexData);
			    obj->wear_data->wear_loc = wear;
			}
			else
			{
			    extract_obj( obj );
			    obj = create_object( obj->pIndexData);
			}
		    }
		    if ( iNest == 0 || rgObjNest[iNest] == NULL )
		    {
			strncpy(obj->owner , ch->name,30 );
			obj_to_room( obj, in_room );
			ch->pcdata->corpse = obj;
		    }
		    else
		    {
			strncpy(obj->owner , ch->name,30 );
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    }
		    return;
		}
	    }
	    break;

	case 'I':
	    break;

	case 'L':
	    if ( !str_cmp( word, "Light" ) )
	    {
		if ( obj->light_data == NULL )
		    obj->light_data = light_alloc();

		obj->light_data->flags = fread_number( fp );
		obj->light_data->fuel = fread_number( fp );
		obj->light_data->max_fuel = fread_number( fp );
		obj->light_data->brightness = fread_number( fp );

		fMatch = TRUE;
		break;
	    }
	    break;

	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
	    	strncpy( obj->name,		fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_corpse: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "NoIreset" ) )
	    {
		RESET_DATA * ireset;

		if ( obj->cont_data != NULL && obj->cont_data->inside != NULL )
		{
		    for ( ireset = obj->cont_data->inside ; ireset != NULL ; ireset = obj->cont_data->inside )
		    {
			obj->cont_data->inside = ireset->next;
			free_reset_data( ireset );
		    }
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "NoMagical" ) )
	    {
		SPELL_LIST * spell;

		if ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL )
		{
		    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = obj->wear_data->magic_data->spells )
		    {
			obj->wear_data->magic_data->spells = spell->next;
			free_spell( spell );
		    }
		    free_magic_data( obj->wear_data->magic_data );
		    obj->wear_data->magic_data = NULL;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'M':
	    if ( !str_cmp( word, "Magical" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->magic_data == NULL )
		    obj->wear_data->magic_data = magic_alloc();

		obj->wear_data->magic_data->type = fread_number( fp );
		obj->wear_data->magic_data->flags = fread_number( fp );
		obj->wear_data->magic_data->charges = fread_number( fp );
		obj->wear_data->magic_data->max_charges = fread_number( fp );
		obj->wear_data->magic_data->spells = NULL;
		fMatch = TRUE;
		break;
	    }
	    KEY( "Mate",	obj->material,		fread_number( fp ) );
	    break;

   	case 'O':
	    if ( !str_cmp( word,"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL )
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    if ( !str_cmp( word,"Owner" ) )
	    {
		strncpy( temp_name, fread_string( fp ), 30 );	
		strncpy(obj->owner, temp_name, 30);
		fMatch = TRUE;
		break;
	    }
	    break;
		    
	case 'R':
	    KEY( "Repaired",	obj->repaired,		fread_number( fp ) );
	    break;
	case 'P':
	    break;

	case 'S':
	    if ( !str_cmp( word, "ShortDescr" ) )
	    {
	    	strncpy( obj->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "SPFL",	obj->special_flags,	fread_number( fp ) );
	    if ( !str_cmp( word, "ShD" ) )
	    {
	    	strncpy( obj->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Slash" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Size" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->size = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Spell" ) )
	    {
		int level;
		int sn;

		level = fread_number( fp );
		sn     = skill_lookup( fread_string( fp ) );
		if ( sn < 0 )
		{
		    bug( "Fread_corpse: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->wear_data == NULL )
			obj->wear_data = wear_alloc();
		    if ( obj->wear_data->magic_data == NULL )
			obj->wear_data->magic_data = magic_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->wear_data->magic_data->spells;
 		    obj->wear_data->magic_data->spells = spell;
		}
		fMatch = TRUE;
		break;
	    }

	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		long vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( "Fread_corpse: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    if ( !str_cmp( word, "WearFlags" ) || !str_cmp( word, "WeaF" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "WearLoc" ) || !str_cmp( word, "Wear" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_loc = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Weight" ) || !str_cmp( word, "Wt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->weight = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wtype" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wflags" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dam_type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdamage" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dice = fread_number( fp );
		obj->wear_data->weapon_data->die  = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	}

	if ( !fMatch )
	{
	    bug( "Fread_corpse: no match.", 0 );
	    fread_to_eol( fp );
	}
    }

    if ( str_cmp( "End",fread_word( fp ) ) )
	fread_obj( ch, fp );
}

void fread_obj( CHAR_DATA *ch, FILE *fp )
{
    static OBJ_DATA obj_zero;
    AFFECT_DATA *af;
    AFFECT_DATA *af_next;
    OBJ_DATA *obj;
    bool faffect = TRUE;
    char buf[MAX_INPUT_LENGTH];
    char temp_name[31];
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    bool Owned;
    bool first;
    bool make_new;    /* update object */
    
    fVnum = FALSE;
    obj = NULL;
    first = TRUE;  /* used to counter fp offset */
    make_new = FALSE;

    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    if (!str_cmp(word,"Vnum" ))
    {
        long vnum;
	first = FALSE;  /* fp will be in right place */
 
        vnum = fread_number( fp );
        if (  get_obj_index( vnum )  == NULL )
	{
            bug( "Fread_obj: bad vnum %d.", vnum );
	}
        else
	{
	    obj = create_object(get_obj_index(vnum));
	}
	    
    }

    if (obj == NULL)  /* either not found or old style */
    {
    	if ( obj_free == NULL )
    	{
	    obj		= (OBJ_DATA *) alloc_perm( sizeof(*obj) );
	    obj_index++;
    	}
    	else
    	{
	    obj		= obj_free;
	    obj_free	= obj_free->next;
    	}

    	*obj			= obj_zero;
    	obj->name[0]		= '\0';
    	obj->short_descr[0]	= '\0';
    	obj->description[0]	= '\0';
    }

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;
    Owned		= FALSE;

    for ( ; ; )
    {
	if (first)
	    first = FALSE;
	else
	    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
	fMatch = FALSE;
	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	 case 'A':
	    if ( !str_cmp( word, "AC" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Affect" ) || !str_cmp(word,"Aff")
	    ||   !str_cmp( word, "AffD"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}

		    paf		= new_affect(FALSE);

		if (!str_cmp(word, "AffD"))
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_obj: unknown skill.",0);
		    else
		   	paf->type = sn;
		}
		else 
		    paf->type	= fread_number( fp );
		if (ch->version == 0)
		  paf->level = 20;
		else
		  paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		if ( ch->version > 7 ) 
		paf->location2	= fread_number( fp );
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffN" ) || !str_cmp(word,"AffO"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}
		    paf		= new_affect(FALSE);
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_obj: unknown skill.",0);
		    else
		   	paf->type = sn;
		}

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp );
		if (!str_cmp( word, "AffN" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffR" ) || !str_cmp(word,"AffS"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}
		    paf		= new_affect(FALSE);

		paf->type	= fread_number( fp );
		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp );
		if (!str_cmp( word, "AffR" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    break;

	case 'B':
	    break;

	case 'C':
	    KEY( "Condition",	obj->condition,		fread_number( fp ) );
	    if ( !str_cmp( word, "Cost" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->cost = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'D':
	    if ( !str_cmp( word, "Description" ) || !str_cmp( word, "Desc" ) )
	    {
	    	strncpy( obj->description,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'E':

	    if ( !str_cmp( word, "Edible" ) )
	    {
		if ( obj->edible_data == NULL )
		    obj->edible_data = edible_alloc();

		obj->edible_data->flags = fread_number( fp );
		obj->edible_data->amount = fread_number( fp );
		obj->edible_data->max_amount = fread_number( fp );
		obj->edible_data->type = fread_number( fp );

		fMatch = TRUE;
		break;
		
	    }
	    if ( !str_cmp( word, "Espell" ) )
	    {
		int level;
		int sn;

		level = fread_number( fp );
		sn     = skill_lookup( fread_string( fp ) );
		if ( sn < 0 )
		{
		    bug( "Fread_perm_rooms: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->edible_data == NULL )
			obj->edible_data = edible_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->edible_data->spells;
 		    obj->edible_data->spells = spell;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Enchanted"))
	    {
		obj->enchanted = TRUE;
	 	fMatch 	= TRUE;
		break;
	    }

	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
	    KEY( "ExtF",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe"))
	    {
		EXTRA_DESCR_DATA *ed;

		    ed			= new_extra_descr();

		strncpy( ed->keyword, fread_string( fp ), 30 );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		if ( !fNest || !fVnum || obj->pIndexData == NULL)
		{
		    bug( "Fread_obj: incomplete object.", 0 );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
		{
		    if (make_new)
		    {
			int wear;
			
			if ( obj->wear_data != NULL )
			{
			    wear = obj->wear_data->wear_loc;
			    extract_obj(obj);

			    obj = create_object(obj->pIndexData);
			    obj->wear_data->wear_loc = wear;
			}
			else
			{
			    extract_obj( obj );
			    obj = create_object( obj->pIndexData);
			}
		    }
		    if ( iNest == 0 || rgObjNest[iNest] == NULL )
		    {
			if ( !Owned )
			{
			    strncpy(obj->owner , ch->name,30 );
			}
			obj_to_char( obj, ch );
		    }
		    else
		    {
			if ( !Owned )
			{
			    strncpy(obj->owner , ch->name,30 );
			}
			obj_to_obj( obj, rgObjNest[iNest-1] );
		    }
		    return;
		}
	    }
	    break;

	case 'I':
	    break;

	case 'L':
	    if ( !str_cmp( word, "Light" ) )
	    {
		if ( obj->light_data == NULL )
		    obj->light_data = light_alloc();

		obj->light_data->flags = fread_number( fp );
		obj->light_data->fuel = fread_number( fp );
		obj->light_data->max_fuel = fread_number( fp );
		obj->light_data->brightness = fread_number( fp );

		fMatch = TRUE;
		break;
	    }
	    break;

	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
	    	strncpy( obj->name,		fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_obj: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    if ( !str_cmp( word, "NoIreset" ) )
	    {
		RESET_DATA * ireset;

		if ( obj->cont_data != NULL && obj->cont_data->inside != NULL )
		{
		    for ( ireset = obj->cont_data->inside ; ireset != NULL ; ireset = obj->cont_data->inside )
		    {
			obj->cont_data->inside = ireset->next;
			free_reset_data( ireset );
		    }
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "NoMagical" ) )
	    {
		SPELL_LIST * spell;

		if ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL )
		{
		    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = obj->wear_data->magic_data->spells )
		    {
			obj->wear_data->magic_data->spells = spell->next;
			free_spell( spell );
		    }
		    free_magic_data( obj->wear_data->magic_data );
		    obj->wear_data->magic_data = NULL;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'M':
	    if ( !str_cmp( word, "Magical" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->magic_data == NULL )
		    obj->wear_data->magic_data = magic_alloc();

		obj->wear_data->magic_data->type = fread_number( fp );
		obj->wear_data->magic_data->flags = fread_number( fp );
		obj->wear_data->magic_data->charges = fread_number( fp );
		obj->wear_data->magic_data->max_charges = fread_number( fp );
		obj->wear_data->magic_data->spells = NULL;
		fMatch = TRUE;
		break;
	    }
	    KEY( "Mate",	obj->material,		fread_number( fp ) );
	    break;

   	case 'O':
	    if ( !str_cmp( word,"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL )
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    if ( !str_cmp( word,"Owner" ) )
	    {
		strncpy( temp_name, fread_string( fp ), 30 );	
		strncpy(obj->owner, temp_name, 30);
		fMatch = TRUE;
		Owned = TRUE;
		break;
	    }
	    break;
	case 'R':
	    KEY( "Repaired",	obj->repaired,		fread_number( fp ) );
	    break;		    
	case 'P':
	    break;

	case 'S':
	    if ( !str_cmp( word, "ShortDescr" ) )
	    {
	    	strncpy( obj->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "SPFL",	obj->special_flags,	fread_number( fp ) );
	    if ( !str_cmp( word, "ShD" ) )
	    {
	    	strncpy( obj->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Slash" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Size" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->size = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Spell" ) )
	    {
		char * sname;
		int level;
		int sn;

		level = fread_number( fp );
		sname = fread_string( fp );
		sn     = skill_lookup( sname );
		if ( sn < 0 )
		{
		    bug( "Fread_object: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->wear_data == NULL )
			obj->wear_data = wear_alloc();
		    if ( obj->wear_data->magic_data == NULL )
			obj->wear_data->magic_data = magic_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->wear_data->magic_data->spells;
 		    obj->wear_data->magic_data->spells = spell;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		long vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( "Fread_obj: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    if ( !str_cmp( word, "WearFlags" ) || !str_cmp( word, "WeaF" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "WearLoc" ) || !str_cmp( word, "Wear" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_loc = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Weight" ) || !str_cmp( word, "Wt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->weight = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wtype" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wflags" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dam_type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdamage" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dice = fread_number( fp );
		obj->wear_data->weapon_data->die  = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	}

	if ( !fMatch )
	{
	    sprintf( buf, "Fread_obj: no match - %s ", word );
	    bug( buf, 0 );
	    fread_to_eol( fp );
	}
    }
}

void fread_obj_2( OBJ_DATA *container, FILE *fp )
{
    static OBJ_DATA obj_zero;
    OBJ_DATA *obj;
    AFFECT_DATA *af;
    AFFECT_DATA *af_next;
    char *word;
    int iNest;
    bool fMatch;
    bool fNest;
    bool fVnum;
    bool first;
    bool faffect = TRUE;
    bool make_new;    /* update object */
   int done = 0;    

    for ( ; ; )
  {
 done = 0;
    fVnum = FALSE;
    obj = NULL;
    first = TRUE;  /* used to counter fp offset */
    make_new = FALSE;
    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
    if (!str_cmp(word,"Vnum" ))
    {
        long vnum;
	first = FALSE;  /* fp will be in right place */
 
        vnum = fread_number( fp );
        if (  get_obj_index( vnum )  == NULL )
	{
            bug( "Fread_obj2: bad vnum %d.", vnum );
	}
        else
	{
	    obj = create_object(get_obj_index(vnum)); 
	}
	    
    }

    if (obj == NULL)  /* either not found or old style */
    {
    	if ( obj_free == NULL )
    	{
	    obj		= (OBJ_DATA *) alloc_perm( sizeof(*obj) );
	    obj_index++;
    	}
    	else
    	{
	    obj		= obj_free;
	    obj_free	= obj_free->next;
    	}

    	*obj		= obj_zero;
    	obj->name[0]		= '\0';
    	obj->short_descr[0]	= '\0';
    	obj->description[0]	= '\0';
    }

    fNest		= FALSE;
    fVnum		= TRUE;
    iNest		= 0;

    for ( ; ; )
    {
	if (done) 
	break;
	if (first)
	    first = FALSE;
	else
	    word   = feof( fp ) ? str_dup("End") : fread_word( fp );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;

	case 'A':
	    if ( !str_cmp( word, "AC" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Affect" ) || !str_cmp(word,"Aff")
	    ||   !str_cmp( word, "AffD"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}

		    paf		= new_affect(FALSE);

		if (!str_cmp(word, "AffD"))
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_obj2: unknown skill.",0);
		    else
		   	paf->type = sn;
		}
		else 
		    paf->type	= fread_number( fp );
		  paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->location2	= fread_number( fp ); 
		
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffN" ) || !str_cmp(word,"AffO"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}
		    paf		= new_affect(FALSE);
		{
		    int sn;
		    sn = skill_lookup(fread_word(fp));
		    if (sn < 0)
			bug("Fread_obj2: unknown skill.",0);
		    else
		   	paf->type = sn;
		}

		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp ); 
		if (!str_cmp( word, "AffN" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    if ( !str_cmp( word, "AffR" ) || !str_cmp(word,"AffS"))
	    {
		AFFECT_DATA *paf;
		if ( faffect )
		{
		    obj->wear_data->affected = NULL;
		    for ( af = obj->wear_data->affected ; af != NULL ; af = af_next )
		    {
			af_next = af->next;
			free_affect ( af );
		    }
		    faffect = FALSE;
		}
		fMatch = TRUE;
		if ( obj->wear_data == NULL )
		{
		    fread_to_eol( fp );
		    break;
		}
		    paf		= new_affect(FALSE);

		paf->type	= fread_number( fp );
		paf->level	= fread_number( fp );
		paf->duration	= fread_number( fp );
		paf->modifier	= fread_number( fp );
		paf->location	= fread_number( fp );
		paf->bitvector	= fread_number( fp );
		paf->bit_type	= fread_number( fp );
		paf->location2	= fread_number( fp ); 
		if (!str_cmp( word, "AffR" ) )
		{
		    if (paf->bit_type == 1 )
		    {
		    paf->bit_type = BIT_AFFECT2;
		    paf->bitvector = AFF_SLOW;
		    }
		    else
		    {
		    if (paf->bitvector > 0 )
		    paf->bit_type = BIT_AFFECT;
		    }
		}
		affect_to_obj(obj, paf);
		fMatch		= TRUE;
		break;
	    }
	    break; 

	case 'B':
	    break;

	case 'C':
	    KEY( "Condition",	obj->condition,		fread_number( fp ) );
	    if ( !str_cmp( word, "Cost" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->cost = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'D':
	    if ( !str_cmp( word, "Description" ) || !str_cmp( word, "Desc" ) )
	    {
	    	strncpy( obj->description,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'E':

	    if ( !str_cmp( word, "Edible" ) )
	    {
		if ( obj->edible_data == NULL )
		    obj->edible_data = edible_alloc();

		obj->edible_data->flags = fread_number( fp );
		obj->edible_data->amount = fread_number( fp );
		obj->edible_data->max_amount = fread_number( fp );
		obj->edible_data->type = fread_number( fp );

		fMatch = TRUE;
		break;
		
	    }
	    if ( !str_cmp( word, "Espell" ) )
	    {
		int level;
		int sn;

		level = fread_number( fp );
		sn     = skill_lookup( fread_string( fp ) );
		if ( sn < 0 )
		{
		    bug( "Fread_perm_rooms: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->edible_data == NULL )
			obj->edible_data = edible_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->edible_data->spells;
 		    obj->edible_data->spells = spell;
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Enchanted"))
	    {
		obj->enchanted = TRUE;
	 	fMatch 	= TRUE;
		break;
	    }

	    KEY( "ExtraFlags",	obj->extra_flags,	fread_number( fp ) );
	    KEY( "ExtF",	obj->extra_flags,	fread_number( fp ) );

	    if ( !str_cmp( word, "ExtraDescr" ) || !str_cmp(word,"ExDe"))
	    {
		EXTRA_DESCR_DATA *ed;

		    ed			= new_extra_descr();

		strncpy( ed->keyword, fread_string( fp ), 30 );
		ed->description		= fread_string( fp );
		ed->next		= obj->extra_descr;
		obj->extra_descr	= ed;
		fMatch = TRUE;
	    }

	    if ( !str_cmp( word, "End" ) )
	    {
		done = 1;
		if ( !fNest || !fVnum || obj->pIndexData == NULL)
		{
		    bug( "Fread_obj2: incomplete object.", 0 );
		    obj->next = obj_free;
		    obj_free  = obj;
		    return;
		}
		else
		{
			obj = create_object(obj->pIndexData);
		    if ( iNest == 0 || rgObjNest[iNest] == NULL )
			{
			obj_to_obj( obj, container );
			}
		    else
			obj_to_obj( obj, container);
		}
	    }
	    break;
	case 'I':
	    break;

	case 'L':
	    if ( !str_cmp( word, "Light" ) )
	    {
		if ( obj->light_data == NULL )
		    obj->light_data = light_alloc();

		obj->light_data->flags = fread_number( fp );
		obj->light_data->fuel = fread_number( fp );
		obj->light_data->max_fuel = fread_number( fp );
		obj->light_data->brightness = fread_number( fp );

		fMatch = TRUE;
		break;
	    }
	    break;

	case 'N':
	    if ( !str_cmp( word, "Name" ) )
	    {
	    	strncpy( obj->name,		fread_string( fp ), 30 );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Nest" ) )
	    {
		iNest = fread_number( fp );
		if ( iNest < 0 || iNest >= MAX_NEST )
		{
		    bug( "Fread_obj2: bad nest %d.", iNest );
		}
		else
		{
		    rgObjNest[iNest] = obj;
		    fNest = TRUE;
		}
		fMatch = TRUE;
	    }
	    if ( !str_cmp( word, "NoIreset" ) )
	    {
		RESET_DATA * ireset;

		if ( obj->cont_data != NULL && obj->cont_data->inside != NULL )
		{
		    for ( ireset = obj->cont_data->inside ; ireset != NULL ; ireset = obj->cont_data->inside )
		    {
			obj->cont_data->inside = ireset->next;
			free_reset_data( ireset );
		    }
		}
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "NoMagical" ) )
	    {
		SPELL_LIST * spell;

		if ( obj->wear_data != NULL && obj->wear_data->magic_data != NULL )
		{
		    for ( spell = obj->wear_data->magic_data->spells ; spell != NULL ; spell = obj->wear_data->magic_data->spells )
		    {
			obj->wear_data->magic_data->spells = spell->next;
			free_spell( spell );
		    }
		    free_magic_data( obj->wear_data->magic_data );
		    obj->wear_data->magic_data = NULL;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

   	case 'M':/*
	    if ( !str_cmp( word, "Magical" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->magic_data == NULL )
		    obj->wear_data->magic_data = magic_alloc();

		obj->wear_data->magic_data->type = fread_number( fp );
		obj->wear_data->magic_data->flags = fread_number( fp );
		obj->wear_data->magic_data->charges = fread_number( fp );
		obj->wear_data->magic_data->max_charges = fread_number( fp );
		obj->wear_data->magic_data->spells = NULL;
		fMatch = TRUE;
		break;
	    }*/
	    break;

   	case 'O':
	    if ( !str_cmp( word,"Oldstyle" ) )
	    {
		if (obj->pIndexData != NULL)
		    make_new = TRUE;
		fMatch = TRUE;
	    }
	    break;
	case 'R':
	    KEY( "Repaired",	obj->repaired,		fread_number( fp ) );
	    break;
	case 'P':
	    break;		    

	case 'S':
	    if ( !str_cmp( word, "ShortDescr" ) )
	    {
	    	strncpy( obj->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    KEY( "SPFL",	obj->special_flags,	fread_number( fp ) );
	    if ( !str_cmp( word, "ShD" ) )
	    {
	    	strncpy( obj->short_descr,	fread_string( fp ), 80 );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Slash" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->ac = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Size" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->size = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "Stop" ) )
		return;
	    if ( !str_cmp( word, "Spell" ) )
	    {
		int level;
		int sn;

		level = fread_number( fp );
		sn     = skill_lookup( fread_string( fp ) );
		if ( sn < 0 )
		{
		    bug( "Fread_object2: unknown skill.", 0 );
		}
		else
		{
		    SPELL_LIST * spell;

		    if ( obj->wear_data == NULL )
			obj->wear_data = wear_alloc();
		    if ( obj->wear_data->magic_data == NULL )
			obj->wear_data->magic_data = magic_alloc();

		    spell = spell_alloc();
		    spell->level = level;
		    spell->spell = sn;
		    spell->next = obj->wear_data->magic_data->spells;
 		    obj->wear_data->magic_data->spells = spell;
		}
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'T':
	    KEY( "Timer",	obj->timer,		fread_number( fp ) );
	    KEY( "Time",	obj->timer,		fread_number( fp ) );
	    break;

	case 'V':
	    if ( !str_cmp( word, "Vnum" ) )
	    {
		long vnum;

		vnum = fread_number( fp );
		if ( ( obj->pIndexData = get_obj_index( vnum ) ) == NULL )
		    bug( "Fread_obj2: bad vnum %d.", vnum );
		else
		    fVnum = TRUE;
		fMatch = TRUE;
		break;
	    }
	    break;

	case 'W':
	    if ( !str_cmp( word, "WearFlags" ) || !str_cmp( word, "WeaF" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }

	    if ( !str_cmp( word, "WearLoc" ) || !str_cmp( word, "Wear" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->wear_loc = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Weight" ) || !str_cmp( word, "Wt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();

		obj->wear_data->weight = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wtype" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wflags" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->flags = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdt" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dam_type = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    if ( !str_cmp( word, "Wdamage" ) )
	    {
		if ( obj->wear_data == NULL )
		    obj->wear_data = wear_alloc();
		if ( obj->wear_data->weapon_data == NULL )
		    obj->wear_data->weapon_data = weapon_alloc();

		obj->wear_data->weapon_data->dice = fread_number( fp );
		obj->wear_data->weapon_data->die  = fread_number( fp );
		fMatch = TRUE;
		break;
	    }
	    break;

	}

	if ( !fMatch )
	{
	    bug( "Fread_obj2: no match.", 0 ); 
	    fread_to_eol( fp );
	}
    }
  }
}

