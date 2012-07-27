/*
 * Our function prototypes.
 * One big lump ... this is every function in Dreams.
 */

#define CD	CHAR_DATA
#define MID	MOB_INDEX_DATA
#define OD	OBJ_DATA
#define OID	OBJ_INDEX_DATA
#define RID	ROOM_INDEX_DATA
#define SF	SPEC_FUN
#define VD      VARIABLE_DATA /* scripts */
#define TD      TRIGGER_DATA /* scripts */

/* 
 * act_comm.c 
 */
char * speak_lang 	args( ( CHAR_DATA *ch, CHAR_DATA *victim, char *argument, sh_int sn ) );
char *	remove_re_subject args( ( char * arg ) );
void	 note_attach	args( ( CHAR_DATA *ch ) );
void	 do_awaymode 	args( (CHAR_DATA *ch, char *argument) );
void	 note_remove	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
void	 save_notes	args( ( void ) );
void	 note_add	args( ( NOTE_DATA *pnote ) );
void	 note_delete	args( ( NOTE_DATA *pnote ) );
void	 do_note	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_delet	args( ( CHAR_DATA *ch, char *argument) );
void	 do_delete	args( ( CHAR_DATA *ch, char *argument) );
void	 do_channels	args( ( CHAR_DATA *ch, char *argument) );
void	 do_deaf	args( ( CHAR_DATA *ch, char *argument) );
void	 do_quiet 	args( ( CHAR_DATA *ch, char * argument) );
void	 do_auction	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_auction_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_think	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_think_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_ooc		args( ( CHAR_DATA *ch, char *argument ) );
void	 do_ooc_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn) );
void	 do_answer	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_answer_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_pray	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_immtalk	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_helpme	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_language	args( ( CHAR_DATA *ch, char *argument, int sn ) );
void	 do_say	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_say_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_shout	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_shout_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_clan	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_clan_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_tell	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_mate	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_tell_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_reply	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_reply_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_yell	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_yell_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_emote	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_bug		args( ( CHAR_DATA *ch, char *argument ) );
void	 do_idea	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_typo	args( ( CHAR_DATA *ch, char *argument ) );
void	 auto_rent 	args( (CHAR_DATA *ch) );
void	 do_rent	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_qui		args( ( CHAR_DATA *ch, char *argument ) );
void	 do_quit	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_save	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_follow	args( ( CHAR_DATA *ch, char *argument ) );
void	 nuke_pets	args( ( CHAR_DATA *ch ) );
void	 add_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	 remove_follower	args( ( CHAR_DATA *ch, CHAR_DATA *master ) );
void	 stop_follower	args( ( CHAR_DATA *ch ) );
void	 die_to_followers	args( ( CHAR_DATA *ch ) );
void	 do_found	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_defect	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_appoint	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_disband	args( ( CHAR_DATA *ch, char *argument ) );
void	 add_to_group	args( ( CHAR_DATA *ch, CHAR_DATA *leader ) );
void	 remove_from_group	args( ( CHAR_DATA *ch ) );
void	 do_order	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_group	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_split	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_split	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_gtell	args( ( CHAR_DATA *ch, char *argument ) );
void	 do_gtell_race	args( ( CHAR_DATA *ch, char *argument, sh_int sn ) );
void	 do_impchat 	args( (CHAR_DATA *ch, char *argument) );
void	 do_infochan 	args( (CHAR_DATA *ch, char *argument) );
bool	 is_read_note	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
bool	 is_note_to	args( ( CHAR_DATA *ch, NOTE_DATA *pnote ) );
bool	 is_language	args( ( int sn ) );
bool	 is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );
bool	 is_same_group	args( ( CHAR_DATA *ach, CHAR_DATA *bch ) );

/* 
 * act_info.c 
 */
char *	format_obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch, bool fShort ) );
void 	show_char_to_char_0	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	show_char_to_char_1	args( ( CHAR_DATA *victim, CHAR_DATA *ch ) );
void 	show_char_to_char	args( ( CHAR_DATA *list, CHAR_DATA *ch ) );
void 	do_topten	args( (CHAR_DATA *ch, char *argument) );
void 	do_setinfo	args( (CHAR_DATA *ch, char *argument) );
void 	do_scroll	args( (CHAR_DATA *ch, char *argument) );
void 	do_socials	args( (CHAR_DATA *ch, char *argument) );
void 	do_news		args( (CHAR_DATA *ch, char *argument) );
void 	do_motd		args( (CHAR_DATA *ch, char *argument) );
void 	do_quest	args( (CHAR_DATA *ch, char *argument) );
void 	do_imotd	args( (CHAR_DATA *ch, char *argument) );
void 	do_rules	args( (CHAR_DATA *ch, char *argument) );
void 	do_story	args( (CHAR_DATA *ch, char *argument) );
void 	do_history	args( (CHAR_DATA *ch, char *argument) );
void 	do_autolist	args( (CHAR_DATA *ch, char *argument) );
void 	do_autoassist	args( (CHAR_DATA *ch, char *argument) );
void 	do_autodirs	args( (CHAR_DATA *ch, char *argument) );
void 	do_automap	args( (CHAR_DATA *ch, char *argument) );
void 	do_autoexit	args( (CHAR_DATA *ch, char *argument) );
void 	do_roomstats	args( (CHAR_DATA *ch, char *argument) );
void 	do_sectortype	args( (CHAR_DATA *ch, char *argument) );
void 	do_autogold	args( (CHAR_DATA *ch, char *argument) );
void 	do_autoloot	args( (CHAR_DATA *ch, char *argument) );
void 	do_autosac	args( (CHAR_DATA *ch, char *argument) );
void 	do_autosplit	args( (CHAR_DATA *ch, char *argument) );
void 	do_brief	args( (CHAR_DATA *ch, char *argument) );
void 	do_set_color	args( (CHAR_DATA *ch, char *argument) );
void 	do_compact	args( (CHAR_DATA *ch, char *argument) );
void 	do_prompt	args( (CHAR_DATA *ch, char *argument) );
void 	do_combine	args( (CHAR_DATA *ch, char *argument) );
void 	do_noloot	args( (CHAR_DATA *ch, char *argument) );
void 	do_nomiss	args( (CHAR_DATA *ch, char *argument) );
void 	do_nofollow	args( (CHAR_DATA *ch, char *argument) );
void 	do_nosummon	args( (CHAR_DATA *ch, char *argument) );
void 	do_look		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_read 	args( (CHAR_DATA *ch, char *argument ) );
void 	do_examine	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_exits	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_worth	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_map		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_scan		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_score	args( ( CHAR_DATA *ch , char *argument ) );
void 	do_time		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_weather	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_help		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_who		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_count 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_inventory	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_compare	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_show_obj_aff	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_equipment	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_credits	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_where	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_consider	args( ( CHAR_DATA *ch, char *argument ) );
void 	set_title	args( ( CHAR_DATA *ch, char *title ) );
void 	do_title	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_description	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_report	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_show_skills	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_find		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_wimpy	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_password	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_logondata	args( (CHAR_DATA *ch, char *argument) );
void 	do_affects	args( (CHAR_DATA *ch, char *argument ) );
void 	do_level	args( ( CHAR_DATA *ch, char *argument) );
void 	do_whogroup	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_wizlist	args( (CHAR_DATA *ch, char *argument) );
void 	do_finger	args( (CHAR_DATA *ch, char *argument) );
void 	show_list_to_char	args( ( OBJ_DATA *list, CHAR_DATA *ch, bool fShort, bool fShowNothing ) );
bool 	check_blind	args( ( CHAR_DATA *ch ) );
void 	check_sensitive	args( ( CHAR_DATA *ch ) );
CHAR_QUESTS *get_char_quest args( ( CHAR_DATA *ch, int quest_number ) );
void    get_clanrank args( ( CHAR_DATA *ch, char *argument) );
void    do_vitals   args( ( CHAR_DATA *ch, char *argument ) );
void    change_attach   args( ( CHAR_DATA *ch ) );
void    change_add  args( ( CHANGE_DATA *change ) );
void    change_remove   args( ( CHANGE_DATA *change ) );
void    check_new_changes   args( ( CHAR_DATA *ch ) );
void    do_changes  args( ( CHAR_DATA *ch, char *argument ) );

/*
 * forge.c
 */
void	do_forge_golem	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_forge_weapon	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_forge_armor	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_extract	args( ( CHAR_DATA *ch, char * argument ) );
void 	list_minerals	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_fset		args( ( CHAR_DATA *ch, char * argument ) );

/* 
 * act_move.c 
 */
extern const int rev_dir [];
sh_int	get_direction_number 	args( ( const char *dir ) );
int 	find_first_step		args( ( ROOM_INDEX_DATA *src, ROOM_INDEX_DATA *target) );
int	find_door	args( ( CHAR_DATA *ch, char *arg ) );
void 	do_disarm_trap	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_dig		args( ( CHAR_DATA *ch, char * argument ) );
void 	do_mine		args( ( CHAR_DATA *ch, char * argument ) );
void 	do_cover	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_absorb	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_search	args( ( CHAR_DATA *ch, char * argument ) );
void 	move_char	args( ( CHAR_DATA *ch, int door, bool follow ) );
void 	do_north	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_east		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_south	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_west		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_up		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_down		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_hold_breath	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mount	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_dismount	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_open		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_close	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_run		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_lock		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_unlock	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_pick		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_stand	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rest		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sit 		args( (CHAR_DATA *ch, char *argument ) );
void 	do_sleep	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_wake		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sneak	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_hide		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_fade		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_visible	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_recall	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_train	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_gohome 	args( (CHAR_DATA *ch, char *argument) );
void 	do_enter 	args( (CHAR_DATA *ch, char *argument) );
void 	do_board 	args( (CHAR_DATA *ch, char *argument) );
void 	do_exit 	args( (CHAR_DATA *ch, char *argument) );
void 	make_track	args( ( CHAR_DATA * ch, EXIT_DATA * pexit ) );
void 	next_track	args( (char *arg, int spaces , TRACK_TYPE *tracks )  );
void 	do_track	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_hunt		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_steer	args( ( CHAR_DATA *ch, char *argument ) );
void 	list_enqueue	args( ( ROOM_INDEX_DATA *room ) );
void 	bfs_enqueue	args( ( ROOM_INDEX_DATA *room, sh_int dir, sh_int depth ) );
void 	bfs_dequeue	args( (void) );
void 	list_dequeue	args( ( void ) );
void 	bfs_clear_queue	args( (void)  );
void 	list_clear_queue	args( ( void ) );
bool 	is_door	args( ( EXIT_DATA *pexit ) );
bool 	is_locked	args( ( LOCK_DATA * lock ) );
bool 	is_bolted	args( ( LOCK_DATA * lock ) );
bool 	is_locked_door	args( ( ROOM_INDEX_DATA *in_room, sh_int door ) );
bool 	has_key		args( ( CHAR_DATA *ch, LOCK_DATA * lock ) );
bool 	can_find_door	args( ( CHAR_DATA *ch, sh_int doorflags) );
bool 	can_enter_gate	args( ( CHAR_DATA *ch, OBJ_DATA *obj) );

/* 
 * act_obj.c 
 */
DECLARE_SPEC_FUN( spec_guard_dog	);
int	get_coin_type	args( ( char * arg ) );
long    get_money_total args( ( CHAR_DATA *ch, int coin_type ) );
CHAR_DATA *	find_keeper	args( ( CHAR_DATA *ch ) );
int	get_cost	args( ( CHAR_DATA *keeper, OBJ_DATA *obj, bool fBuy, CHAR_DATA *ch ) );
long	haggle_cost	args( ( CHAR_DATA *keeper, long cost ) );
void 	get_home_key	args( ( CHAR_DATA *ch,  char * argument) );
void 	get_char_money	args( ( CHAR_DATA *ch, long cost, int coin_type ) );
void 	transfer_money	args( ( CHAR_DATA *ch, CHAR_DATA *victim, long cost, int coin_type )  );
void 	get_obj		args( ( CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *container ) );
void 	do_bury		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_throw	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_borrow	args( ( CHAR_DATA *ch, char*argument ) );
void 	do_cook		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_poison_blade	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_mrepair	args( ( CHAR_DATA * ch, CHAR_DATA * smithy, char * arg ) );
void 	do_repair	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_pay		args( ( CHAR_DATA *ch, char*argument ) );
void 	do_tame_animal	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_empty	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_get		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_put		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_drop		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_give		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_fill		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_drink	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_eat		args( ( CHAR_DATA *ch, char *argument ) );
void 	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );
void 	do_wear		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_remove	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sacrifice	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_quaff	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_recite	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_brandish	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_zap		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_steal	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_copy		args( ( CHAR_DATA *ch, char *argument ) );
void 	buy_obj		args( ( CHAR_DATA * ch, CHAR_DATA * keeper, OBJ_DATA * obj, int number )  );
void 	do_resize	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_enchant	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_buy		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_refill	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_extinguish	args( ( CHAR_DATA *ch, char *argument ) );
void 	extinguish_obj 	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void 	do_light	args( ( CHAR_DATA *ch, char *argument ) );
void 	light_obj 	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void 	do_refuel	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_list		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sell		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_value	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_first_aid 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rename	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_pull		args( (CHAR_DATA *ch, char *argument ) );
void 	do_sign		args( (CHAR_DATA *ch, char *argument) );
bool 	is_bastard	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool 	is_knife_sized	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool 	is_one_handed	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool 	is_two_handed	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool 	can_loot	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
bool 	remove_obj	args( ( CHAR_DATA *ch, int iWear, bool fReplace ) );
bool	can_wear_size	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
void 	wear_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool fReplace ) );

/* 
 * act_special.c
 */
void 	do_beeper 	args( (CHAR_DATA *ch, char *argument) );
void 	do_balance 	args( (CHAR_DATA *ch, char *argument) );
void 	do_deposit 	args( (CHAR_DATA *ch, char *argument) );
void 	do_exchange 	args( (CHAR_DATA *ch, char *argument) );
void 	do_withdraw 	args( (CHAR_DATA *ch, char *argument) );
void 	do_cointrans 	args( (CHAR_DATA *ch, char *argument) );
void 	do_bankset 	args( (CHAR_DATA *ch, char *argument) );
void 	do_wheelofwonder 	args( (CHAR_DATA *ch, char *argument)  );

/* 
 * act_wiz.c 
 */
DECLARE_OBJ_SPEC_FUN(spec_blood_thirsty );
void 	reset_events	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_select	args( ( CHAR_DATA *ch, char * argument ) );
void 	backup_area	args( ( CHAR_DATA * ch, char * argument, int num ) );
void 	do_backup	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_wed		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_simulate	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rlist	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_glist	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_saveraces	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_saveguilds	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_bonus 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_outfit 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_cpr		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_saveclans	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_makeclan	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_clist	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_cshow	args( ( CHAR_DATA *ch, char *argument ) );
void	do_recruit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_blackball	args( ( CHAR_DATA *ch, char *argument ) );
void   	do_clanrank	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_nochannels	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_bamfin	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_bamfout	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_deny		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_disconnect	args( ( CHAR_DATA *ch, char *argument ) );
void 	pardon_death	args( ( CHAR_DATA * ch ) );
void 	do_pardon	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_donate 	args( (CHAR_DATA *ch, char *argument) );
void 	do_echo		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_wecho	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_aecho	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_recho	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_pecho	args( ( CHAR_DATA *ch, char *argument ) );
void 	trans_obj	args( ( CHAR_DATA * ch, char * arg, ROOM_INDEX_DATA *location ) );
void 	do_transfer	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_at		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_goto		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_stat 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rstat	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sstat	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_ostat	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mstat	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_vnum		args( (CHAR_DATA *ch, char *argument) );
void 	do_areastat 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_fixexit 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mfind	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_hfind	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rfind	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_ofind	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mwhere	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_reboo	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_reboot	args( ( CHAR_DATA *ch, char *argument ) );
void 	reboot		args( ( void ) );
void 	do_shutdow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_shutdown	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_snoop	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_switch	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_return	args( ( CHAR_DATA *ch, char *argument ) );
void 	recursive_clone	args( (CHAR_DATA *ch, OBJ_DATA *obj, OBJ_DATA *clone) );
void 	do_clone	args( (CHAR_DATA *ch, char *argument ) );
void 	do_load		args( (CHAR_DATA *ch, char *argument ) );
void 	do_mload	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_oload	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_purge_area	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_purge	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_advance	args( ( CHAR_DATA *ch, char *argument ) );
void    restore_character   args( ( CHAR_DATA *ch ) );
void 	do_restore	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_freeze	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_log		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_noemote	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_noshout	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_notell	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_peace	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_ban		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_allow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_wizlock	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_newlock	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_slookup	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_settitle	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_setgroup	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_setguild	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_set		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sset		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_cset		args( ( CHAR_DATA *ch, char *argument) );
void 	do_mset		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_string	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_oset		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rset		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sockets	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_force	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_invis	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_wizpass	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_holylight	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_areasave	args( ( AREA_DATA  *pArea, char *argument, CHAR_DATA *ch ) );
void 	make_appartment	args( ( CHAR_DATA * ch, ROOM_INDEX_DATA * in_room, int direction, int number )  );
void 	do_buy_appartment 	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_makehome 	args( (CHAR_DATA *ch, char *argument) );
void 	do_sellhome 	args( (CHAR_DATA *ch, char *argument) );
void 	do_sane 	args( ( CHAR_DATA *ch, char *argument )  );
void 	do_saveout 	args( ( CHAR_DATA *ch, char *argument) );
void 	do_nuke 	args( ( CHAR_DATA *ch, char *argument) );
void 	do_virtual	args( ( CHAR_DATA *ch, char *argument) );
void 	do_system	args( ( CHAR_DATA *ch, char *argument) );
void 	do_set_room_template	args( ( CHAR_DATA *ch, char *argument) );
void 	do_cpu		args( ( CHAR_DATA *ch, char *argument) );
void 	do_xyzzy	args( ( CHAR_DATA *ch, char *argument) );
void 	do_makegrid	args( ( CHAR_DATA *ch, char *argument) );
void 	do_vrtual		args( ( CHAR_DATA *ch, AREA_DATA *pArea) );
void 	do_jail		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_prefi 	args( (CHAR_DATA *ch, char *argument) );
void 	do_prefix 	args( (CHAR_DATA *ch, char *argument) );
bool 	backup_file	args( ( CHAR_DATA * ch, char * argument, int num ) );
bool 	permission 	args( ( CHAR_DATA *ch ) );
bool 	obj_check 	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
bool 	is_optivision	args( ( CHAR_DATA *ch ) );
bool 	make_floor	args( ( ROOM_INDEX_DATA * room, int floor_dir ) );
bool 	make_first_floor	args( ( ROOM_INDEX_DATA * room, int dir ) );
bool 	find_empty_floor	args( ( CHAR_DATA * ch, ROOM_INDEX_DATA * in_room, int floor_dir, int floor_num ) );

/* 
 * comm.c 
 */
void	do_hotreboot	args( (CHAR_DATA *ch, char * argument) );
void	do_hotreboo	args( (CHAR_DATA *ch, char * argument) );
void    copyover_recover        args((void));
char 	*do_color	args( (char *plaintext, bool color) );
extern  int	mud_port;	/* the port the mud is on */
extern  int	LOGON_NUMBER;
extern  int	top_descriptor;
extern  char *help_greeting[MAX_GREETING];
void 	game_loop_unix	args( ( int control ) );
void 	new_descriptor	args( ( int control ) );
void 	close_socket	args( ( DESCRIPTOR_DATA *dclose ) );
void 	read_from_buffer	args( ( DESCRIPTOR_DATA *d, bool color ) );
void 	write_to_buffer	args( ( DESCRIPTOR_DATA *d, const char *txt, long length ) );
void 	nanny	args( ( DESCRIPTOR_DATA *d, char *argument ) );
void 	stop_idling	args( ( CHAR_DATA *ch ) );
void 	send_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void 	send_to_char_trig	args( ( char *txt, CHAR_DATA *ch ) );
void 	send_to_room	args( ( int room_number, char *message ) );
void 	send_to_area	args( ( AREA_DATA *area, char *message ) );
void 	send_to_guild	args( ( char *message, long guild ) );
void 	send_weather	args( ( AREA_DATA *area, char *message ) );
void 	send_to_clan	args( ( CLAN_DATA *clan, char *message ) );
void 	send_to_world	args( ( int world_number, char *message ) );
void 	info_channel 	args( (CHAR_DATA *ch, char *message, long infoitem) );
void 	set_color	args( ( const char *color, CHAR_DATA *ch ) );
void 	page_to_char	args( ( const char *txt, CHAR_DATA *ch ) );
void 	show_string	args( (struct descriptor_data *d, char *input) );
void 	fix_sex		args( (CHAR_DATA *ch) );
bool 	read_from_descriptor	args( ( DESCRIPTOR_DATA *d, bool color ) );
void 	read_from_buffer	args( ( DESCRIPTOR_DATA *d, bool color ) );
bool 	process_output	args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
bool 	write_to_descriptor	args( ( int desc, char *txt, long length, bool color ) );
bool 	check_parse_name	args( ( char *name ) );
bool 	check_reconnect	args( ( DESCRIPTOR_DATA *d, char *name, bool fConn ) );
bool 	check_playing	args( ( DESCRIPTOR_DATA *d, char *name ) );


void	cact		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type,
			     char *color ) );
void	act_tell	args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	act		args( ( const char *format, CHAR_DATA *ch,
			    const void *arg1, const void *arg2, int type ) );
void	cact_new	args( ( const char *format, CHAR_DATA *ch, 
			    const void *arg1, const void *arg2, int type,
			    int min_pos, char *color) );
void	act_new		args( ( const char *format, CHAR_DATA *ch, 
			    const void *arg1, const void *arg2, int type,
                            	int min_pos) );


/* 
 * db.c 
 */
extern bool fBootDb;
extern int		newmobs;
extern int		social_count;
extern int		newobjs;
extern MOB_INDEX_DATA 	* mob_index_hash          [MAX_KEY_HASH];
extern OBJ_INDEX_DATA 	* obj_index_hash          [MAX_KEY_HASH];
extern long		top_mob_index;
extern long		top_obj_index;
extern int  		top_affect;
extern int		top_ed; 
CD *	create_mobile	args( ( MOB_INDEX_DATA *pMobIndex ) );
OD *	create_object	args( ( OBJ_INDEX_DATA *pObjIndex ) );
char *	get_extra_descr	args( ( const char *name, EXTRA_DESCR_DATA *ed ) );
MID *	get_mob_index	args( ( long vnum ) );
TRIGGER_INDEX_DATA *	get_trig_index	args( ( int vnum ) );
OID *	get_obj_index	args( ( long vnum ) );
RID *	get_room_index	args( ( long vnum ) );
char	fread_letter	args( ( FILE *fp ) );
int	fread_number	args( ( FILE *fp ) );
long 	fread_flag	args( ( FILE *fp ) );
char *	fread_string	args( ( FILE *fp ) );
char *	fread_string_ns	args( ( FILE *fp ) );
char *  fread_string_eol args(( FILE *fp ) );
char *	fread_word	args( ( FILE *fp ) );
long	flag_convert	args( ( char letter) );
char *	str_dup		args( ( const char *str ) );
char *	str_dup_ns	args( ( const char *str ) );
int	number_fuzzy	args( ( int number ) );
int	number_range	args( ( int from, int to ) );
int	number_percent	args( ( void ) );
int	number_door	args( ( void ) );
int	number_bits	args( ( int width ) );
long    number_mm       args( ( void ) );
int	roll_dice	args( ( int number, int size ) );
int	interpolate	args( ( int level, int value_00, int value_32 ) );
int	str_cmp		args( ( const char *astr, const char *bstr ) );
char *	capitalize	args( ( const char *str ) );
char *  initial		args( ( const char *str ) );
void 	srandom		args( (unsigned int) );
void 	maxfilelimit	args( () );
void	save_quests	args( ( void ) );
void 	boot_db		args( ( void ) );
void 	load_helps_new	args( () );
void 	load_trigger_index	args( (void) );
void 	load_event_data	args( ( AREA_DATA *pArea ) );
void 	load_resets_new	args( ( AREA_DATA *pArea ) );
void 	load_obj_new	args( ( AREA_DATA *pArea ) );
void 	load_mob_new	args( ( AREA_DATA *pArea ) );
void 	load_rooms_new	args( ( AREA_DATA *pArea ) );
void 	load_shops_new	args( ( AREA_DATA *pArea ) );
void 	load_specials	args( ( FILE *fp ) );
void 	load_notes	args( ( void ) );
void    load_changes args( ( void ) );
void    save_changes args( ( void ) );
void 	load_clans	args( ( void ) );
void 	imp_log_string	args( ( const char *str ) );
void 	load_counter	args( ( void ) );
void 	load_logon	args( ( void ) );
void 	load_random	args( ( void ) );
void 	load_quests	args( ( void ) );
void 	load_topten	args( ( void ) );
void 	save_clans	args( ( void ) );
void 	save_logon	args( ( void ) );
void 	save_random	args( ( void ) );
void 	load_rlist	args( ( ) );
void 	save_top_ten	args( ( void ) );
void 	save_rlist	args( ( ) );
void 	save_races	args( ( void ) );
void 	save_race	args( ( int race ) );
void 	load_race	args( ( int race ) );
void 	default_race	args( ( int race ) );
void 	load_races	args( ( void ) );
void 	save_glist	args( ( ) );
void 	load_glist	args( ( ) );
void 	load_guild	args( ( int guild ) );
void 	load_guilds	args( ( void ) );
void 	default_guild	args( ( int guild) );
void 	save_guilds	args( ( void ) );
void 	save_guild	args( ( int guild ) );
void 	fix_exits	args( ( void ) );
void 	area_update	args( ( void ) );
void 	reset_area	args( ( AREA_DATA *pArea ) );
void 	hatch		args( ( OBJ_DATA * cont ) );
void 	do_load_room_triggers	args( ( CHAR_DATA *ch, char *argument ) );
void 	load_room_triggers	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void	pick_skills	args( ( CHAR_DATA * ch ) );
void 	clone_mobile	args( (CHAR_DATA *parent, CHAR_DATA *clone) );
void 	clone_object	args( (OBJ_DATA *parent, OBJ_DATA *clone) );
void 	clear_char	args( ( CHAR_DATA *ch ) );
void 	clear_obj	args( ( OBJ_DATA *obj ) );
void 	free_char	args( ( CHAR_DATA *ch ) );
void 	free_char_quests	args( ( CHAR_QUESTS *quests ) );
void 	free_random_data	args( ( GLOBAL_RANDOM_DATA *grandom ) );
void 	fread_to_eol	args( ( FILE *fp ) );
void 	*alloc_mem	args( ( long sMem ) );
void 	free_mem	args( ( void *pMem, long sMem ) );
void 	*alloc_perm	args( ( long sMem ) );
void 	free_string	args( ( char *pstr ) );
void 	do_areas	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_memory	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_dump		args( ( CHAR_DATA *ch, char *argument ) );
void 	init_mm		args( ( ) );
void 	smash_tilde	args( ( char *str ) );
void 	append_file	args( ( CHAR_DATA *ch, char *file, char *str ) );
void 	coder_log	args( ( const char *str, int param ) );
void 	bug		args( ( const char *str, int param ) );
void 	bug2		args( ( const char *str, int param ) );
void 	log_string	args( ( const char *str ) );
void 	tail_chain	args( ( void ) );
void 	save_rooms	args( (AREA_DATA *pArea, char *arg) );
void 	save_mobs	args( (AREA_DATA *pArea, char *arg) );
void 	save_objs	args( (AREA_DATA *pArea, char *arg) );
void 	default_weather	args( ( AREA_DATA *pArea ) );
void 	new_load_area	args( ( ) );
void 	assign_area_vnum	args( ( long vnum ) );
void 	save_triggers	args( (CHAR_DATA *ch, char *argument) );
void 	save_events	args( (AREA_DATA *pArea, char *argument) );
void 	load_socials	args( ( ) );
void 	load_wizlist	args( ( void ) );
void 	load_finger	args( ( void ) );
void 	save_wizlist	args( ( void ) );
void 	save_finger	args( ( void ) );
bool 	str_prefix	args( ( const char *astr, const char *bstr ) );
bool 	str_infix	args( ( const char *astr, const char *bstr ) );
bool 	str_suffix	args( ( const char *astr, const char *bstr ) );
/* 
 * fight.c
 */
void 	violence_update	args( ( void ) );
void    do_apply	args( ( CHAR_DATA *ch, char *argument ) );
void	flee_char	args( ( CHAR_DATA *ch ) );
void 	check_assist	args( (CHAR_DATA *ch,CHAR_DATA *victim) );
void 	multi_hit	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt ) );
void 	multi_hit_standard 	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dt) );
void 	mob_hit 	args( (CHAR_DATA *ch, CHAR_DATA *victim, int dt) );
void 	one_hit		args( ( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA * wielding, int dt ) );
void 	check_top_ten	args( ( CHAR_DATA *ch, int number, int type ) );
void 	check_killer	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	update_pos	args( ( CHAR_DATA *victim ) );
void 	set_fighting	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	stop_fighting	args( ( CHAR_DATA *ch, bool fBoth ) );
void 	make_corpse	args( ( CHAR_DATA *ch, bool empty ) );
void 	raw_kill	args( ( CHAR_DATA *victim, bool empty, bool pk ) );
void 	group_gain	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	dam_message	args( ( CHAR_DATA *ch, CHAR_DATA *victim,int dam,int dt, int dam_type, bool immune ) );
void 	disarm	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
void 	do_berserk	args( ( CHAR_DATA *ch, char *argument) );
void 	do_tarvals_wrath	args( ( CHAR_DATA *ch, char *argument) );
void 	do_blur		args( ( CHAR_DATA *ch, char *argument) );
void 	do_breathfire	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_bash		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_dirt		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_perception	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_lore		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_trip		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_tail		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_slam		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_kill		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_ki_force	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_stun 	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_murde	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_hkill	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_murder	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_shadowstrike	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_strike	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_double_strike	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_precision_strike	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_quick_strike	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_retreat	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_flee		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rescue	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_disarm	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_thrust	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_jab		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_whirl	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_kick		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sla		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_intimidate	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_shriek	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_slay		args( ( CHAR_DATA *ch, char *argument ) );
bool 	fire_damage	args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type ) );
bool 	damage		args( ( CHAR_DATA *ch, CHAR_DATA *victim, int dam, int dt, int dam_type ) );
bool 	is_safe		args( (CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	is_safe_spell	args( (CHAR_DATA *ch, CHAR_DATA *victim, bool area ) );
bool 	check_parry	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	check_block	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	check_dodge	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	check_wind_over_sand	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
int 	xp_compute	args( ( CHAR_DATA *gch, CHAR_DATA *victim, int total_levels ) );

/*
 * healer.c
 */
void 	do_heal	args( (CHAR_DATA *ch, char *argument) );

/* 
 * handler.c 
 */
bool	check_wander args( ( CHAR_DATA *ch , EXIT_DATA *pexit, int door) );
void 	equip_mob args( (CHAR_DATA *mob, long flags) );
void    set_quest args( ( CHAR_DATA * ch, int number, int points, long flags ) );
void	load_inside_obj args( ( OBJ_DATA *obj, long flags ) );
void	approve_object	args( ( OBJ_DATA *pObj ) );
int direction_room( CHAR_DATA *ch, ROOM_INDEX_DATA * pRoom );
LIGHT_DATA * light_alloc( void );
TRIGGER_DATA * trigger_alloc( void );
TRIGGER_LIST_DATA * trigger_list_alloc( void );
TRIGGER_INDEX_DATA * pTrigIndex_alloc( void );
RESET_DATA * reset_alloc( void );
CONT_DATA * cont_alloc( void );
READ_NOTES * read_notes_alloc( void );
DEBT_DATA * debt_data_alloc( void );
BET_DATA * bet_data_alloc( void );
BAN_DATA * ban_data_alloc( void );
SKILL_LIST * skill_list_alloc( void );
SPECPRO_LIST * specpro_list_alloc( void );
BHOST_LIST * bhost_list_alloc( void );
GROUP_DATA * group_data_alloc( void );
EDITING_DATA * editing_data_alloc( void );
NOTE_DATA * note_data_alloc( void );
WEATHER_DATA * weather_data_alloc( void );
GAINER_DATA * gainer_data_alloc( void );
CLAN_DATA * clan_data_alloc( void );
PC_CLAN_DATA * pcclan_data_alloc( void );
TRAP_DATA * trap_alloc( void );
TRACK_DATA * track_alloc( void );
TRACK_TYPE * track_type_alloc( void );
GATE_DATA * gate_alloc( void );
EDIBLE_DATA * edible_alloc( void );
MAGIC_DATA * magic_alloc( void );
SPELL_LIST * spell_alloc( void );
WEAR_DATA * wear_alloc( void );
EQUIP_DATA * equip_alloc( void );
INSIDE_DATA * inside_alloc( void );
LOCK_DATA * lock_alloc( void );
WEAPON_DATA * weapon_alloc( void );
ROOM_INDEX_DATA * pRoomIndex_alloc( void );
INSIDE_AREA_DATA * inside_area_alloc( void );
MOVEABLE_DATA * moveable_alloc( void );
HELP_DATA * new_help( void );
EXIT_DATA * new_exit( void );
SHOP_DATA * new_shop( void );
AFFECT_DATA * new_affect( bool MoBile);
OBJ_INDEX_DATA * new_obj_index( void );
MOB_INDEX_DATA * alloc_pMobIndex( void );
EXTRA_DESCR_DATA * new_extra_descr( void );
AREA_DATA * new_area( void );
AREA_DATA * get_vnum_area( long vnum );
LOGON_DATA * logon_data_alloc( void );
GLOBAL_RANDOM_DATA * random_data_alloc( void );
QUEST_DATA  * quest_data_alloc( void );
EVENT_DATA * event_alloc( void );
CHAR_QUESTS * char_quests_data_alloc( void );
WIZLIST_DATA * wizlist_data_alloc( void );
FINGER_DATA * finger_alloc( void );
ROOM_INDEX_DATA * gate_leads_to( GATE_DATA * gate );
BFS_QUEUE * alloc_bfs_queue( void );
BFS_ROOM * alloc_bfs_room( void );
CHANGE_DATA * change_data_alloc( void );
long	spell_affect_bit args( (int sn) ); 
long	spell_affect_bit_2 args( (int sn) ); 
int 	check_immune	args( (CHAR_DATA *ch, int dam_type) );
int 	material_lookup args( ( const char *name) );
int	race_lookup	args( ( const char *name) );
CLAN_DATA * clan_lookup	args( ( char *name) );
int 	get_eq_light	args( ( CHAR_DATA *ch ) );
int 	get_eq_magic_light	args( ( CHAR_DATA *ch ) );
sh_int 	get_light_adjustment	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
sh_int 	get_hour	args( ( AREA_DATA *pArea) );
sh_int 	get_sky_type 	args( ( AREA_DATA *pArea) );
sh_int 	get_season	args( ( AREA_DATA *pArea) );
sh_int	get_height_size args( ( int size ) );
sh_int	get_weight_size args( ( int size ) );
int	get_skill_cost	args( ( CHAR_DATA *ch, int sn ) );
int	get_skill_level	args( ( CHAR_DATA *ch, int sn ) );
long	get_gain_cost	args( ( CHAR_DATA *ch, int sn ) );
int	get_mana_cost	args( ( CHAR_DATA *ch, int sn, int i ) );
int	get_skill	args( ( CHAR_DATA *ch, int sn ) );
int	get_weapon_skill args(( CHAR_DATA *ch, OBJ_DATA * obj ) );
int     get_age         args( ( CHAR_DATA *ch ) );
int	get_curr_stat	args( ( CHAR_DATA *ch, int pstat ) );
int 	get_max_train	args( ( CHAR_DATA *ch, int pstat ) );
int	can_carry_n	args( ( CHAR_DATA *ch ) );
long	can_carry_w	args( ( CHAR_DATA *ch ) );
CD *    get_obj_carried_by args( ( OBJ_DATA *obj ) );
ROOM_INDEX_DATA * get_in_room_obj args( ( OBJ_DATA *obj ) );
char *	cut_name	args( ( char *str, char *namelist ) );
int	apply_ac	args( ( OBJ_DATA *obj, int iWear, int type ) );
OD *	get_eq_char	args( ( CHAR_DATA *ch, int iWear ) );
int	count_obj_list	args( ( OBJ_INDEX_DATA *obj, OBJ_DATA *list ) );
int	count_obj	args( ( OBJ_INDEX_DATA *obj ) );
CD *	get_char_room	args( ( CHAR_DATA *ch, char *argument ) );
CD *	get_char	args( ( CHAR_DATA *ch ) );
CD *	get_char_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_type	args( ( OBJ_INDEX_DATA *pObjIndexData ) );
OD *	get_obj_list	args( ( CHAR_DATA *ch, char *argument,
			    OBJ_DATA *list ) );
OD *	get_obj_carry_vnum	args ( (CHAR_DATA *ch, long arg) );
OD *	get_obj_carry	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_wear	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_here	args( ( CHAR_DATA *ch, char *argument ) );
OD *	get_obj_world	args( ( CHAR_DATA *ch, char *argument ) );
OD *	create_money	args( ( int amount, int coin_type ) );
int	get_obj_weight	args( ( OBJ_DATA *obj ) );
char *  shop_type_name  args( ( int arg ) );
int 	shop_name_type	args( ( char * arg ) );
int	get_shop_type	args( ( int arg ) );
char *  clan_flag_name	args( ( int clan_info ) );
char *	affect_loc_name	args( ( AFFECT_DATA * paf ) );
char *	affect_name	args( (  int vector ) );
char *	affect_bit_name	args( ( long vector ) );
char *	affect_bit_name_2	args( ( long vector ) );
char *	extra_bit_name	args( ( long extra_flags ) );
char *	special_bit_name	args( ( long special_flags ) );
char *	room_bit_name	args( ( int room_flags ) );
char *	room_bit_name_2	args( ( int room_flags_2 ) );
char *	room_bit_name_1	args( ( int room_flags ) );
char *  sector_bit_name args( ( int sector_type ) );
int  	wear_bit_location	args( ( int wear_flags ) );
int  	wear_bit_location_2	args( ( int wear_flags ) );
char * 	wear_bit_name	args( ( int wear_flags ) );
char *	act_bit_name	args( ( long act_flags ) );
char *	act2_bit_name	args( ( long act2_flags ) );
char *	off_bit_name	args( ( int off_flags ) );
char *  imm_bit_name	args( ( int imm_flags ) );
char *  guild_bit_name	args( ( int guild_flags ) );
char *  guild_bit_name_2	args( ( int guild_flags ) );
int	guild_name_bit	args( ( char* buf ) );	
char *  res_bit_name	args( ( long res_flags ) );
char *  vuln_bit_name	args( ( long vuln_flags ) );
char * 	adv_flag_bit_name args( ( int adv_flags ) );
char * 	form_bit_name	args( ( int form_flags ) );
char *	weapon_bit_name	args( ( int weapon_flags ) );
char *  comm_bit_name	args( ( int comm_flags ) );
void 	next_rtrack	args( ( CHAR_DATA * ch, ROOM_INDEX_DATA * pRoom, int spaces , TRACK_TYPE *tracks ) );
void 	do_direction	args( ( CHAR_DATA *ch, char * argument ) );
void 	create_nightmare_mob	args( ( CHAR_DATA *ch, int nlevel ) );
void 	create_thug	args( ( CHAR_DATA *ch, int tlevel ) );
void 	send_thug	args( ( CHAR_DATA *ch ) );
void 	reset_char	args( (CHAR_DATA *ch) );
void	affect_apply_room args( ( ROOM_INDEX_DATA * room, AFFECT_DATA *paf ) );
bool	affect_modify_room args( ( ROOM_INDEX_DATA * room, AFFECT_DATA *paf, bool fAdd ) );
void 	affect_to_char	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void 	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
void 	affect_to_obj	args( (OBJ_DATA *obj, AFFECT_DATA *paf) );
void 	affect_remove	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void 	affect_remove_obj	args( ( OBJ_DATA *obj, AFFECT_DATA *paf) );
void 	affect_remove_room	args( ( ROOM_INDEX_DATA *room, AFFECT_DATA *paf) );
void 	affect_strip	args( ( CHAR_DATA *ch, int sn ) );
void 	affect_join	args( ( CHAR_DATA *ch, AFFECT_DATA *paf ) );
void 	char_from_room	args( ( CHAR_DATA *ch ) );
void 	char_to_obj 	args( (CHAR_DATA *ch, OBJ_DATA *obj) );
void 	char_to_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
void 	add_to_shop	args( ( OBJ_DATA * obj, CHAR_DATA * ch ) );
void 	obj_to_char	args( ( OBJ_DATA *obj, CHAR_DATA *ch ) );
void 	obj_from_char	args( ( OBJ_DATA *obj, char *argument ) );
void	check_armor_damage	args( ( CHAR_DATA *victim ) );
void 	equip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj, int iWear ) );
void 	unequip_char	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
void 	obj_from_room	args( ( OBJ_DATA *obj ) );
void 	obj_to_room	args( ( OBJ_DATA *obj, ROOM_INDEX_DATA *pRoomIndex ) );
void 	obj_to_obj	args( ( OBJ_DATA *obj, OBJ_DATA *obj_to ) );
void 	obj_from_obj	args( ( OBJ_DATA *obj ) );
bool 	do_trigger	args( ( void  *vo, CHAR_DATA *ch, OBJ_DATA *object, char *argument,  int TRIGGER, int TRIG_TYPE ) );
bool 	check_immortal_order	args( (CHAR_DATA *ch, CHAR_DATA *vch) );
bool 	is_part_name	args( ( const char *str, char *namelist ) );
bool 	is_name	args( ( const char *str, char *namelist ) );
bool 	is_name_3	args( ( const char *str, char *namelist ) );
bool 	can_do_trigger	args( (TRIGGER_DATA *trig, CHAR_DATA *ch) );
bool 	is_trig_keyword	args( (TRIGGER_DATA *trig, char *argument ) );
bool 	is_name_prefix	args( ( const char *str, char *namelist ) );
bool 	is_name_2	args( ( const char *str, char *namelist ) );
void 	affect_modify	args( ( CHAR_DATA *ch, AFFECT_DATA *paf, bool fAdd ) );
bool 	is_affected	args( ( CHAR_DATA *ch, int sn ) );
bool 	is_affected_bit	args( ( CHAR_DATA *ch, int sn ) );
bool 	is_affected_bit_2	args( ( CHAR_DATA *ch, int sn ) );
bool 	obj_vnum_in_room	args( ( ROOM_INDEX_DATA *in_room, long vnum ) );
bool 	has_obj_vnum	args( ( CHAR_DATA *ch, long vnum ) );
bool 	room_is_dark	args( ( ROOM_INDEX_DATA *pRoomIndex, CHAR_DATA *ch ) );
bool 	room_is_private	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
bool 	check_room_size	args( ( ROOM_INDEX_DATA *pRoomIndex, CHAR_DATA *ch ) );
bool 	can_see_room	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *pRoomIndex ) );
bool 	can_see		args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	in_same_plane	args( ( CHAR_DATA *ch, CHAR_DATA *victim ) );
bool 	can_see_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj ) );
bool 	can_drop_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, bool inapartment ) );
bool 	check_for_dream	args( ( CHAR_DATA *ch ) );
bool 	do_trigger	args( ( void  *vo, CHAR_DATA *ch, OBJ_DATA *object, char *argument,  int TRIGGER, int TRIG_TYPE ) );
bool 	check_string 	args( (char *name) );

/* 
 * interp.c 
 */
int	number_argument	args( ( char *argument, char *arg ) );
char *	one_argument	args( ( char *argument, char *arg_first ) );
char *	one_argument_any args( ( char *argument, char *arg_first ) );
char *  one_argument_case args( ( char *argument, char *arg ) );
void 	interpret	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_commands	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_wizhelp	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_specroom 	args( (CHAR_DATA *ch, char *argument) );
void 	do_show_roomstats 	args( (CHAR_DATA *ch, char *argument) );
void 	do_show_sector_type 	args( (CHAR_DATA *ch, char *argument) );
void 	do_color_check 	args( (CHAR_DATA *ch, char *argument) );
bool 	can_do_command	args( (CHAR_DATA *ch, int cmd) );
bool 	check_social	args( ( CHAR_DATA *ch, char *command, char *argument ) );
bool 	is_number 	args( ( char *arg ) );

/* 
 * magic.c 
 */
int 	mana_cost 	(CHAR_DATA *ch, int min_mana, int level);
int	skill_lookup	args( ( const char *name ) );
int	slot_lookup	args( ( int slot ) );
void 	say_spell	args( ( CHAR_DATA *ch, void *victim, int sn, bool pass ) );
void 	do_cast		args( ( CHAR_DATA *ch, char *argument ) );
void 	obj_cast_spell	args( ( CHAR_DATA *ch, CHAR_DATA *victim, OBJ_DATA *obj, SPELL_LIST * spells , int type )  );
void 	trap_cast_spell	args( ( CHAR_DATA *ch, TRAP_DATA * trap )  );
void 	spell_water_blast	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_shatter	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_armor	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_tongues	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_courage	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_blinding_light	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_sand_blast	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_chain_lightning	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_calm_spirit	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_remove_aura	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_energy_tap	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_mind_blast	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_pain_touch	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_minor_sacrifice	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_sacrifice	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_major_sacrifice	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_maelstrom	args( (int sn, int level, CHAR_DATA *ch, void *vo) );
void 	spell_control_mind	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_desert_fist	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_arrows_of_fire	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_create_minor_portal	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_create_major_portal	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_continual_light	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_illuminate	args( (int sn,int level, CHAR_DATA *ch, void *vo) );
void 	spell_create_ration	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_find_spring	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_create_water	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_remove_blindness	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_cure_critical	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_cure_disease	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_cure_light	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_remove_poison	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_cure_serious	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_curse	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_demonfire	args( (int sn, int level, CHAR_DATA *ch, void *vo) );
void 	spell_detect_world	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_detect_hidden	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_detect_shadowform	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_senses	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_detect_magic	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_detect_poison	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_dispel_magic	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_pyrotechnics	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_boulder_toss	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_enchant_armor	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_enchant_weapon	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_energy_drain	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_sunfire 	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_sand_storm	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_faerie_fire	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_reveal	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_fly	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_levitate	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_frenzy	args( (int sn, int level, CHAR_DATA *ch, void *vo) );
void 	spell_planar_travel	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_empower	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_disintigrate	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_entangle	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_haste	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_slow	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_heal	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_holy_word	args( (int sn, int level, CHAR_DATA *ch, void *vo) );
void 	spell_identify	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_infravision	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_shadowform	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_wind_blade	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_locate_object	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_flying_dagger	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_ice_shards	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_sand_jambiya	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_mass_healing	args( (int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_group_healing	args( (int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_mass_shadowform	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_null	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_dimension_walk	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_plague	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_poison	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_breathe_underwater	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_refresh	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_remove_curse	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_circle_of_protection args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_protection_from_fire	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_protection_from_water	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_protection_from_wind	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_protection_from_earth	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_protection_from_spirit	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_fire_shield	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_water_shield	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_earth_shield	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_wind_shield	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_spirit_shield	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_element_protection	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_wind_vortex	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_sleep	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_stone_skin	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_summon	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_teleport	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_ventriloquate	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_weaken	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_return_to_safety	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_acid_breath	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_fire_breath	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_gas_breath	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_lightning_breath	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	spell_high_explosive	args( ( int sn, int level, CHAR_DATA *ch, void *vo ) );
void 	say_spell	args( ( CHAR_DATA *ch, void *victim, int sn, bool pass ) );
bool 	saves_spell	args( ( int level, CHAR_DATA *victim ) );
bool 	saves_dispel	args( ( int dis_level, int spell_level, int duration) );
bool 	check_dispel	args( ( int dis_level, CHAR_DATA *victim, int sn) );

/* 
 * save.c 
 */
void 	save_char_obj	args( ( CHAR_DATA *ch ) );
void 	fwrite_char	args( ( CHAR_DATA *ch, FILE *fp ) );
void 	fwrite_pet	args( ( CHAR_DATA *pet, FILE *fp, char *type) );
void 	load_perm_rooms	args( ( void ) );
void 	read_perm_rooms	args( ( FILE *fp ) );
void 	save_perm_rooms	args( ( ROOM_INDEX_DATA *pRoomIndex ) );
void 	fwrite_obj	args( ( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest ) );
void 	fwrite_corpse	args( ( CHAR_DATA *ch, OBJ_DATA *obj, FILE *fp, int iNest ) );
void 	fread_char	args( ( CHAR_DATA *ch, FILE *fp ) );
void 	fread_pet	args( ( CHAR_DATA *ch, FILE *fp, char *type ) );
void 	fread_corpse	args( ( CHAR_DATA *ch, FILE *fp ) );
void 	fread_obj	args( ( CHAR_DATA *ch, FILE *fp ) );
void 	fread_obj_2	args( ( OBJ_DATA *container, FILE *fp ) );
bool 	load_char_obj	args( ( DESCRIPTOR_DATA *d, char *name ) );

/* 
 * skills.c 
 */
void 	do_teach	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_cost		args( ( CHAR_DATA *ch, char * argument ) );
void 	do_slist	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_skillz	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_join 	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_gain 	args( ( CHAR_DATA *ch, char *argument) );
void 	do_spells	args( ( CHAR_DATA *ch, char *argument) );
void 	do_skills	args( ( CHAR_DATA *ch, char *argument) );
void 	check_improve	args( ( CHAR_DATA *ch, int sn, bool success, int multiplier ) );
void 	check_hp_improve args( ( CHAR_DATA *victim, int tot_damage ) );
void 	check_mv_improve args( ( CHAR_DATA *ch, int mv ) );
void 	check_mana_improve args( ( CHAR_DATA *ch, int sn ) );
void	get_weapon_improve args( ( CHAR_DATA *ch, OBJ_DATA * obj ) );

/* 
 * special.c 
 */
SF *	spec_lookup	args( ( const char *name ) );
OBJ_SPEC_FUN *	obj_spec_lookup	args( ( const char *name ) );
DECLARE_SPEC_FUN(spec_assassin );
bool 	dragon		args( ( CHAR_DATA *ch, char *spell_name ) );
bool 	spec_breath_any	args( ( CHAR_DATA *ch ) );
bool 	spec_breath_acid	args( ( CHAR_DATA *ch ) );
bool 	spec_breath_fire	args( ( CHAR_DATA *ch ) );
bool 	spec_breath_frost	args( ( CHAR_DATA *ch ) );
bool 	spec_breath_gas	args( ( CHAR_DATA *ch ) );
bool 	spec_breath_lightning	args( ( CHAR_DATA *ch ) );
bool 	spec_cast_adept	args( ( CHAR_DATA *ch ) );
bool 	check_is_dispelled	args( ( CHAR_DATA *ch, int sn , int target ) );
bool 	spec_cast_lithdor	args( ( CHAR_DATA *ch ) );
bool 	spec_cast_nenkemen	args( ( CHAR_DATA *ch ) );
bool 	spec_cast_maegmenel	args( ( CHAR_DATA *ch ) );
bool 	spec_cast_undead	args( ( CHAR_DATA *ch ) );
bool 	spec_executioner	args( ( CHAR_DATA *ch ) );
bool 	spec_executioner	args( ( CHAR_DATA *ch ) );
bool 	spec_dung	args( ( CHAR_DATA *ch ) );
bool 	spec_egg	args( ( CHAR_DATA *ch ) );
bool 	spec_assassin	args( ( CHAR_DATA *ch ) );
bool 	spec_guard_dog	args( ( CHAR_DATA * ch ) );
bool 	spec_thug	args( ( CHAR_DATA *ch ) );
bool 	spec_nightmare	args( ( CHAR_DATA *ch ) );
bool 	spec_fido	args( ( CHAR_DATA *ch ) );
bool 	spec_guard	args( ( CHAR_DATA *ch ) );
bool 	spec_janitor	args( ( CHAR_DATA *ch ) );
bool 	spec_viking_ship	args( ( CHAR_DATA *ch ) );
bool 	spec_coach	args( ( CHAR_DATA *ch ) );
bool 	spec_mayor	args( ( CHAR_DATA *ch ) );
bool 	spec_poison	args( ( CHAR_DATA *ch ) );
bool 	spec_thief	args( ( CHAR_DATA *ch ) );
bool 	spec_blood_thirsty	args( ( OBJ_DATA *obj ) );
bool 	spec_object_protection	args( ( OBJ_DATA *obj ) );

/*
 * mob_commands.c
 */
void 	do_mpasound	args( ( CHAR_DATA *ch, char *argument ) );
void 	next_mtrack	args( (char *arg, int spaces , TRACK_TYPE *tracks )  );
void 	do_mpkill	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpjunk	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpechoaround	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpset_exit_flags	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpdig	args( ( CHAR_DATA *ch, char * argument ) );
void 	do_mpechoat	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpecho	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpmload	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpoload	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mppurge	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpgoto	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpat	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mptransfer	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mpforce	args( ( CHAR_DATA *ch, char *argument ) );
bool 	do_mphunt	args( ( CHAR_DATA *ch, char *argument ) );
bool 	do_mphunt_old	args( ( CHAR_DATA *ch, char *argument ) );

/*
 * obj_commands.c
 */
void 	do_object	args( ( CHAR_DATA * ch, char * argument ) );
void 	obj_techo	args( ( OBJ_DATA * obj, char * argument ) );
void 	obj_say	args( ( OBJ_DATA * obj, char * argument ) );
void 	obj_echo	args( ( OBJ_DATA * obj, char * argument ) );
void 	obj_echo_2	args( ( OBJ_DATA * obj, char * argument ) );
void 	obj_interpret	args( ( OBJ_DATA * obj, char * argument ) );

/*
 * room_commands.c
 */
void 	do_room	args( ( CHAR_DATA * ch, char * argument ) );
void 	room_rand_stat	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_change	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_rset	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_teleport	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_at	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_techo	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_echo	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_echo_2	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_refresh	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_unlock	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_lock	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_open	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_close	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_move	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_move_2	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );
void 	room_interpret	args( ( ROOM_INDEX_DATA * pRoom, char * argument ) );


/* 
 * update.c 
 */
void 	reset_doors	args( ( AREA_DATA *pArea) );
void 	event_reset	args( ( long flags, AREA_DATA *pArea) );
void 	check_rust	args( ( OBJ_DATA * obj, int amount ) );
void 	weather_affect	args( ( CHAR_DATA *ch ) );
void 	advance_level	args( ( CHAR_DATA *ch ) );
void 	gain_exp	args( ( CHAR_DATA *ch, int gain ) );
void 	set_position	args( ( CHAR_DATA * ch, int position ) );
void 	check_regen	args( ( CHAR_DATA * ch ) );
void 	hit_gain	args( ( CHAR_DATA *ch ) );
void 	mana_gain	args( ( CHAR_DATA *ch ) );
void 	move_gain	args( ( CHAR_DATA *ch ) );
void 	gain_condition	args( ( CHAR_DATA *ch, int iCond, int value ) );
void 	remove_track	args( ( EXIT_DATA * pexit ) );
void 	wear_best	args( ( CHAR_DATA *ch ) );
void 	web_who_update 	args( ( void ) );
void 	room_update_trigger	args( ( void ) );
void 	room_update	args( ( void ) );
void 	mobile_update	args( ( void ) );
void 	weather_update	args( ( void ) );
void 	char_dream_update	args( () );
void 	char_forge_update	args( () );
void 	char_update	args( ( void ) );
void    regen_update    args( ( void ) );
void 	obj_update_trigger	args( ( void ) );
void 	trigger_update	args( ( void ) );
void 	obj_update	args( ( void ) );
void 	aggr_update	args( ( void ) );
void 	update_handler	args( ( void ) );
void 	do_task	args( ( CHAR_DATA *ch ) );
bool 	use_area_affect	args( ( CHAR_DATA *ch, int target ) );
bool 	mob_has_mana	args( ( CHAR_DATA * ch, int sn ) );
int 	get_skill_mob	args( ( CHAR_DATA *ch, CHAR_DATA * victim, int target, bool fArea )  );
bool 	mob_skill	args( ( CHAR_DATA *ch ) );

/* 
 * gamble.c 
 */
void 	setup_race	args( ( HORSE_RACE_TYPE * race ) );
void 	free_card	args( ( CARD_DATA * card ) );
CARD_DATA * alloc_card	args( ( void ) );
void 	clear_deck	args( ( ROOM_INDEX_DATA * pRoom ) );
void 	sort_hand	args( ( CHAR_DATA * ch, bool fSuit ) );
void 	do_deck		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_deal		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_draw		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_sort		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_play		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_discard	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_junk		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_hand		args( ( CHAR_DATA * ch, char * argument ) );
void 	upper_lower_triple args( ( CHAR_DATA * ch, char * argument, int type ) );
void 	do_upper	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_lower	args( ( CHAR_DATA * ch, char * argument ) );
void 	do_triple	args( ( CHAR_DATA * ch, char * argument ) );
void 	display_horses	args( ( HORSE_RACE_TYPE * race ) );
void 	race_update	args( ( void ) );
void 	do_list_horses	args( ( CHAR_DATA *ch, char * argument ) );
void 	setup_race	args( ( HORSE_RACE_TYPE * race ) );
void 	do_bet		args( ( CHAR_DATA * ch, char * argument ) );
void 	do_collect	args( ( CHAR_DATA * ch, char *argument ) );

/* 
 * string.c 
 */
char * 	rot_string	args( ( char *string ) );
char *  format_string   args( ( char *oldstring /*, bool fSpace */ ) );	 /* OLC */
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ) );	/* OLC */
char *	string_replace	args( ( char * orig, char * old, char * newstr ) );	/* OLC */
char *	string_proper	args( ( char * argument ) );	/* OLC */
char *	string_unpad	args( ( char * argument ) );	/* OLC */
int	arg_count	args( ( char *argument ) );	/* OLC */
bool    check_string	args( ( char * name  ) );
void 	string_edit	args( ( CHAR_DATA *ch, char **pString ) );
void 	do_rot_string	args( ( CHAR_DATA *ch, char *argument ) );
void 	string_append	args( ( CHAR_DATA *ch, char **pString ) );
void 	string_append_2	args( ( CHAR_DATA *ch, char **pString ) );
void 	string_add	args( ( CHAR_DATA *ch, char *argument ) );

/* 
 * olc.c 
 */
AREA_DATA * get_area_data args( ( int vnum ) );
int     get_direction_number_1 args( ( char *arg ) );
void 	sedit		args( ( CHAR_DATA *ch, char *argument ) );
void 	hedit		args( ( CHAR_DATA *ch, char *argument ) );
void 	aedit		args( ( CHAR_DATA *ch, char *argument ) );
void 	redit		args( ( CHAR_DATA *ch, char *argument ) );
void 	maedit		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_area_reset	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_hedit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sedit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_aedit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_redit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_maedit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_sshow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_hshow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_ashow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_rshow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mashow	args( ( CHAR_DATA *ch, char *argument ) );
void 	set_obj_index_points	args( ( OBJ_INDEX_DATA *obj ) );
void 	set_obj_points	args( ( OBJ_DATA *obj ) );
void 	do_oshow	args( ( CHAR_DATA *ch, char *argument ) );
void 	display_inside	args( ( CHAR_DATA *ch, OBJ_INDEX_DATA * obj, char *argument ) );
void 	display_resets	args( ( CHAR_DATA *ch, char *argument ) );
void 	add_reset	args( ( AREA_DATA *area, RESET_DATA *pReset, int reset_index ) );
void 	do_resets	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mob_inside	args( ( CHAR_DATA * ch, OBJ_INDEX_DATA * obj, char * argument ) );
void 	do_autosave	args( (CHAR_DATA *ch, char *argument) );
void 	save_area_list	args( () );
void 	save_social	args( (CHAR_DATA *ch, char *argument) );
void 	save_help	args( (CHAR_DATA *ch, char *argument) );
void 	save_specials	args( ( FILE *fp, AREA_DATA *pArea ) );
void 	save_resets	args( ( AREA_DATA *pArea, char *arg ) );
void 	do_save_resets	args( ( CHAR_DATA *ch, char *arg ) );
void 	save_shops	args( ( AREA_DATA *pArea, char *arg ) );
void 	save_area	args( ( AREA_DATA *pArea, CHAR_DATA *ch, char *arg2 ) );
void 	do_asave	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_arealist	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_trigger_list	args( ( CHAR_DATA *ch, char *argument ) );
void 	send_to_all_char	args( ( const char *text ) );
void 	tedit		args( ( CHAR_DATA *ch, char *argument ) );
void 	gedit		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_tedit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_gedit	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_tshow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_gshow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_trigger_list	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_raedit	args( ( CHAR_DATA *ch , char * argument ) );
void 	do_rashow	args( ( CHAR_DATA *ch, char *argument, int race , int sex ) );
void 	raedit		args( ( CHAR_DATA *ch, char * argument ) );
void 	oe_makegrid 	args( (CHAR_DATA *ch, char *argument) );
void 	do_security 	args( (CHAR_DATA *ch, char *argument) );

/* medit.c */
int	get_mob_sex_number	args( ( char* arg ) );
SPEC_FUN *get_mob_spec	args( ( char* arg ) );
void 	medit		args( ( CHAR_DATA *ch, char *argument ) );
void 	do_mshow	args( ( CHAR_DATA *ch, char *argument ) );
void 	do_medit	args( ( CHAR_DATA *ch, char *argument ) );

/* oedit.c */
void 	do_oedit	args( ( CHAR_DATA *ch, char *argument ) );
void 	oedit		args( ( CHAR_DATA *ch, char *argument ) );
bool    edit_function   args( ( CHAR_DATA *ch, int menu, int security) );
void    update_approve_data args( ( CHAR_DATA *ch, AREA_DATA *pArea, OBJ_INDEX_DATA *pObj ) );

/* redit.c */
ROOM_INDEX_DATA *create_room args( ( CHAR_DATA *ch, AREA_DATA *pArea, long vnum ) );
void    make_exit	args( (ROOM_INDEX_DATA *pRoom, ROOM_INDEX_DATA *pRoomIndex, int dir ) );
LOCK_DATA  *get_lock_data args( (int vnum, LOCK_DATA * lock) );

/* bit.c */
int	position_value		args( ( char * arg ) );
char *  element_type_name	args( ( int arg ) );
char *  mineral_type_name	args( ( int arg ) );
char *  bit_type_name		args( ( int arg, long bit_vector ) );
char *  cmd_group_type_name	args( ( int arg ) );
int	get_element_type	args( ( char * arg ) );
int	get_mineral_type	args( ( char * arg ) );
int	get_cmd_group_type	args( ( char * arg ) );
int	get_quest_flags		args( ( char * arg ) );
char *  get_quest_name		args( ( int arg ) );
char *  trig_type_name		args( ( int arg ) );
char *  target_type_name	args( ( int arg ) );
int	get_trig_type		args( ( char * arg ) );
char *	material_type_name	args( ( int arg ) );
int	get_material_type	args( ( char * arg ) );
char *  note_flag_name		args( ( int flag ) );
int	get_note_flag		args( ( char * arg ) );
char *  get_pos_name		args( ( int pos ) );
char *  light_flag_name		args( ( int flag ) );
int	get_light_flag		args( ( char * arg ) );
char *  get_exit_flag_string	args( ( int exit_dir ) );
char *  get_exit_flag_string_1	args( ( int exit_dir ) );
char *  gate_flag_name		args( ( int flag ) );
int	get_gate_flag		args( ( char * arg ) );
char *  edible_flag_name	args( ( int flag ) );
char *  edible_flag_list_name	args( ( int flag ) );
int	get_edible_flag		args( ( char * arg ) );
int	get_edible_type		args( ( char * arg ) );
char *  magical_flag_name	args( ( int arg ) );
char *  magical_type_name	args( ( int arg ) );
int	get_magical_flag	args( ( char *arg ) );
int	get_magical_type	args( ( char *arg ) );
char *	room_bit_name		args( ( int room_flags ) );	/* OLC */
char *	room_bit_name_2		args( ( int room_flags_2 ) );	/* OLC */
int	room_name_bit		args( ( char *buf ) );		/* OLC */
int	room_name_bit_2		args( ( char *buf ) );		/* OLC */
int	sector_number		args( ( char *argument ) );	/* OLC */
int	get_storm_type		args( ( char *arg ) );
char *	sector_name		args( ( int sect ) );		/* OLC */
int	item_name_type		args( ( char *name ) ); 	/* OLC */
char *	item_type_name		args( ( int item_type ) );	/* OLC */
int	extra_name_bit		args( ( char* buf ) );		/* OLC */
int	special_name_bit	args( ( char* buf ) );		/* OLC */
char *	extra_bit_name		args( ( long extra_flags ) ); 	/* OLC */
int	wear_name_bit		args( ( char* buf ) );		/* OLC */
char *	wear_bit_name_1		args( ( int wear ) );		/* OLC */
int	wear_location_flag	args( ( int wear ) );
long	act_name_bit		args( ( char* buf ) );		/* OLC */
long	act2_name_bit		args( ( char* buf ) );		/* OLC */
long	off_name_bit		args( ( char* buf ) );		/* OLC */
int	vuln_name_bit		args( ( char* buf ) );		/* OLC */
int	form_name_bit		args( ( char* buf ) );		/* OLC */
int	res_name_bit		args( ( char* buf ) );		/* OLC */
int	imm_name_bit		args( ( char* buf ) );		/* OLC */
long 	affect_name_bit		args( ( char* buf ) );		/* OLC */
long 	affect_name_bit_2	args( ( char* buf ) );		/* OLC */
int	affect_name_loc		args( ( char* name ) );		/* OLC */
int     wear_name_loc   	args( ( char *buf ) );		/* OLC */
char *	wear_loc_name		args( ( int wearloc ) );	/* OLC */
char *	act_bit_name		args( ( long act_flags ) );		/* OLC */
char *	get_bit_name		args( ( int bit_name ) );		/* OLC */
int	get_weapon_type		args( ( char *arg ) );		/* OLC */
int     weapon_name_type	args( ( char * arg ) );
char *	get_type_weapon		args( ( int arg ) );		/* OLC */
char *	get_weapon_name		args( ( int arg ) );		/* OLC */
int 	get_name_weapon		args( ( char *arg ) );
int	weapon_name_bit		args( ( char *arg ) );
int	get_container_flags	args( ( char *arg ) );		/* OLC */
int	get_trap_flags		args( ( char *arg ) );		/* OLC */
int	get_flags_event		args( ( char *arg ) );		/* OLC */
int	get_flags_equip		args( ( char *arg ) );		/* OLC */
char *	get_event_flags		args( ( int arg ) );		/* OLC */
char *	get_equip_flags		args( ( int arg ) );		/* OLC */
char *	get_flags_container	args( ( int arg ) );		/* OLC */
char *	get_flags_trap		args( ( int arg ) );		/* OLC */
int	get_lock_flags		args( ( char *arg ) );		/* OLC */
char *	get_flags_lock		args( ( int arg ) );		/* OLC */
int	get_liquid_type		args( ( char *arg ) );		/* OLC */
char *	get_type_liquid		args( ( int arg ) );		/* OLC */

/*
 * const.c
 */
void	list_spec_pros 	args( (CHAR_DATA *ch, char *argument) );

/* 
 * scr.c 
 */
int     get_dir         args( ( char *arg ) );
char *  translate_variables      args( ( char *argument, CHAR_DATA *mob ) );
char *  translate_variables_obj  args( ( char *argument, OBJ_DATA *obj ) );
char *  translate_variables_room  args( ( char *argument, ROOM_INDEX_DATA *room ) );
VD *    get_variable    args( ( char *var_name, CHAR_DATA *mob ) );
char *  first_arg       args( ( char *argument, char *arg_first, bool fCase ));
void    act_trigger     args( ( CHAR_DATA *mob, char *command,
                            char *argument, char *from, char *to ) );
void    act_trigger_obj    args( ( OBJ_DATA *obj, char *command,
                            char *argument, char *from, char *to ) );
void    act_trigger_room  args( ( ROOM_INDEX_DATA *room, char *command,
                            char *argument, char *from, char *to ) );
char *  replace_string  args( ( char *orig, char *old, char *newstr ) );
void 	do_script	args( ( CHAR_DATA *ch, char *argument ) );
bool 	script_in_process	args( ( CHAR_DATA *ch ) );
void 	do_tset_char	args( (CHAR_DATA * ch, char *argument ) );
void 	do_tset		args( (char *argument) );
void 	assign_variable	args( ( char *var_name, char *value, CHAR_DATA *mob ) );
void 	assign_variable_room	args( ( char *var_name, char *value, ROOM_INDEX_DATA *room ) );
void 	assign_variable_obj	args( ( char *var_name, char *value, OBJ_DATA *obj ) );
void 	multi_interpret 	args( ( CHAR_DATA *mob, char *com_list ) );
void 	eval_assign	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_interp	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_interp_room	args( ( ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_interp_obj	args( ( OBJ_DATA *obj, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_oed	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_wait	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, OBJ_DATA *obj, TRIGGER_DATA *trigger, char* argument ) );
void 	eval_goto	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char* argument ) );
void 	eval_msign	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_osign	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_wsign	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_wfunc	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA * pRoom, OBJ_DATA *obj, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_mfunc	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_ocreate	args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_path	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_ofunc	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_sfunc	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	eval_walkto	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument) );
void 	eval_walktomob	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument) );
void 	eval_steerto	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument) );
void 	eval_mob	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );
void 	script_interpret	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger ) );
void 	script_interpret_room	args( ( ROOM_INDEX_DATA *room, TRIGGER_DATA *trigger ) );
void 	script_interpret_obj	args( ( OBJ_DATA *obj, TRIGGER_DATA *trigger ) );
void 	script_update	args( ( CHAR_DATA *ch, int timetype ) );
void 	script_update_room	args( ( ROOM_INDEX_DATA *room, int timetype ) );
void 	script_update_obj	args( ( OBJ_DATA *obj, int timetype ) );
bool 	eval_halt	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char* argument ) );
bool 	boolchk		args( ( char *v1, char *v2, char *s ) );
bool 	eval_if		args( ( CHAR_DATA *ch, ROOM_INDEX_DATA *room, OBJ_DATA *obj, TRIGGER_DATA *trigger, char *argument ) );
bool 	eval_while	args( ( CHAR_DATA *ch, TRIGGER_DATA *trigger, char *argument ) );

ROOM_INDEX_DATA *find_location 	args( ( CHAR_DATA *ch, char *arg ) );
void    send_to_all_char	args( ( const char *text ) ); 

/* 
 * recycle.c 
 */
BUFFER	*new_buf args( (void) );
BUFFER  *new_buf_size args( (long size) );
char	*buf_string args( (BUFFER *buffer) );
int 	get_exit_flag_number args( ( char *arg ) );
SNOOP_LIST * snoop_list_alloc args( ( void ) );
ROOM_SNOOP_LIST * room_snoop_list_alloc args( ( void ) );
void	free_snoop_list args( ( SNOOP_LIST *snoop_list ) );
void	free_room_snoop_list args( ( ROOM_SNOOP_LIST *room_snoop_list ) );
void 	free_reset_data	args( ( RESET_DATA *pReset ) );
void 	free_skill_list	args( ( SKILL_LIST *skill ) );
void 	free_wizlist	args( ( WIZLIST_DATA *wizlist ) );
void 	free_finger	args( ( FINGER_DATA *finger ) );
void 	free_bfs_queue	args( ( BFS_QUEUE *bfs_queue ) );
void 	free_bfs_room	args( ( BFS_ROOM *bfs_room ) );
void 	free_event_data	args( ( EVENT_DATA *event ) );
void 	free_bet_data	args( ( BET_DATA *bet_data ) );
void 	free_bhost_list	args( ( BHOST_LIST *bhost_list ) );
void 	free_debt_data	args( ( DEBT_DATA *debt_data ) );
void 	free_inside_area_data	args( ( INSIDE_AREA_DATA *inside_area ) );
void 	free_area	args( ( AREA_DATA *pArea ) );
void 	free_exit	args( ( EXIT_DATA *pExit ) );
void 	free_read_notes	args( ( READ_NOTES *pNote ) );
void 	free_spell	args( ( SPELL_LIST *pSpell ) );
void 	free_magic_data	args( ( MAGIC_DATA *pMagic ) );
void 	free_group_data	args( ( GROUP_DATA *pGroup ) );
void 	free_lock_data	args( ( LOCK_DATA *pLock ) );
void 	free_cont_data	args( ( CONT_DATA *pCont ) );
void 	free_wear_data	args( ( WEAR_DATA *pWear ) );
void 	free_equip_data	args( ( EQUIP_DATA *equip ) );
void 	free_inside_data	args( ( INSIDE_DATA *inside ) );
void 	free_edible_data	args( ( EDIBLE_DATA *pEdible ) );
void 	free_trap_data	args( ( TRAP_DATA *pTrap ) );
void 	free_gate_data	args( ( GATE_DATA *pGate ) );
void 	free_weapon_data	args( ( WEAPON_DATA *pWeapon ) );
void 	free_light_data	args( ( LIGHT_DATA *pLight ) );
void 	free_track_type	args( ( TRACK_TYPE *track ) );
void	free_pcclan_data args( ( PC_CLAN_DATA *pcclan ) );
void 	free_extra_descr	args( ( EXTRA_DESCR_DATA *pExtra ) );
void 	free_room_index	args( ( ROOM_INDEX_DATA *pRoom ) );
void 	free_affect	args( ( AFFECT_DATA* pAf ) );
void 	free_shop	args( ( SHOP_DATA *pShop ) );
void 	free_obj_index_data	args( ( OBJ_INDEX_DATA *pObj2 ) );
void 	free_mob_index_data	args( ( MOB_INDEX_DATA *pMob2 ) );
void 	free_variable	args( ( VARIABLE_DATA *var ) );
void 	free_trigger	args( ( TRIGGER_DATA *trig ) );
void 	free_trigger_list	args( ( TRIGGER_LIST_DATA *trig ) );
void 	free_script	args( ( SCRIPT_DATA *scr ) );
void 	free_spec_pro	args( ( SPECPRO_LIST *spec ) );
void 	extract_obj	args( ( OBJ_DATA *obj ) );
void 	extract_mineral	args( ( CHAR_DATA *ch ) );
void 	extract_char	args( ( CHAR_DATA *ch, bool fPull ) );
void 	free_buf	args( (BUFFER *buffer) );
void 	clear_buf	args( (BUFFER *buffer) );
bool 	add_buf		args( (BUFFER *buffer, char *string) );


#undef	CD
#undef	MID
#undef	OD
#undef	OID
#undef	RID
#undef	SF

