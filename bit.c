#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"

char * element_type_name( int element )
{
    if ( element == ELEMENT_FIRE	) return "`Rfire`w";
    if ( element == ELEMENT_WATER	) return "`Bwater`w";
    if ( element == ELEMENT_EARTH	) return "`Gearth`w";
    if ( element == ELEMENT_WIND	) return "`Cwind`w";
    if ( element == ELEMENT_SPIRIT	) return "`Wspirit`w";
    return "none";
}

char * mineral_type_name( int mineral )
{
    if ( mineral == MINERAL_STEEL	) return "`wSteel`w";
    if ( mineral == MINERAL_GOLD	) return "`YGold`w";
    if ( mineral == MINERAL_SILVER	) return "`wSilver`w";
    if ( mineral == MINERAL_IRON	) return "`KIron`w";
    if ( mineral == MINERAL_BRONZE	) return "`yBronze`w";
    if ( mineral == MINERAL_COPPER	) return "`yCopper`w";
    if ( mineral == MINERAL_RUBY	) return "`RRuby`w";
    if ( mineral == MINERAL_SAPPHIRE	) return "`BSapphire`w";
    if ( mineral == MINERAL_EMERALD	) return "`gEmerald`w";
    if ( mineral == MINERAL_ALABASTER	) return "`wAlabaster`w";
    if ( mineral == MINERAL_ADAMANTITE	) return "`wAdamantite`w";
    if ( mineral == MINERAL_TITANIUM	) return "`WTitanium`w";
    if ( mineral == MINERAL_PLATINUM	) return "`KPlatinum`w";
    if ( mineral == MINERAL_OBSIDIAN	) return "`bObsidian`w";
    if ( mineral == MINERAL_MITHRIL	) return "`CMithril`w";
    if ( mineral == MINERAL_DIAMOND	) return "`WDiamond`w";
    return "none";
}

char * bit_type_name( int bit_type, long bit_vector )
{
    if ( bit_type == BIT_IMM		) return imm_bit_name(bit_vector);
    if ( bit_type == BIT_AFFECT		) return affect_bit_name(bit_vector);
    if ( bit_type == BIT_VULN		) return vuln_bit_name(bit_vector);
    if ( bit_type == BIT_AFFECT2	) return affect_bit_name_2(bit_vector);
    if ( bit_type == BIT_RES		) return res_bit_name(bit_vector);
    return "none";
}
char * get_bit_name( int bit_type )
{
    if ( bit_type == BIT_IMM		) return "immunity";
    if ( bit_type == BIT_AFFECT		) return "affect";
    if ( bit_type == BIT_VULN		) return "vulnerbility";
    if ( bit_type == BIT_AFFECT2	) return "affect";
    if ( bit_type == BIT_RES		) return "resistance";
    return "none";
}

char * get_quest_name( int bit_type )
{
    if ( bit_type == STEP1		) return "step1";
    if ( bit_type == STEP2		) return "step2";
    if ( bit_type == STEP3		) return "step3";
    if ( bit_type == STEP4		) return "step4";
    if ( bit_type == STEP5		) return "step5";
    if ( bit_type == STEP6		) return "step6";
    if ( bit_type == STEP7		) return "step7";
    if ( bit_type == STEP8		) return "step8";
    if ( bit_type == STEP9		) return "step9";
    if ( bit_type == STEP10		) return "step10";
    if ( bit_type == STEP11		) return "step11";
    if ( bit_type == STEP12		) return "step12";
    if ( bit_type == STEP13		) return "step13";
    if ( bit_type == STEP14		) return "step14";
    if ( bit_type == STEP15		) return "step15";
    if ( bit_type == STEP16		) return "step16";
    if ( bit_type == STEP17		) return "step17";
    if ( bit_type == STEP18		) return "step18";
    if ( bit_type == STEP19		) return "step19";
    if ( bit_type == STEP20		) return "step20";
    if ( bit_type == QUEST_COMPLETED	) return "completed";
    return "none";
}


int get_element_type( char * arg )
{
    if ( !str_cmp( arg, "fire"	) ) return ELEMENT_FIRE;
    if ( !str_cmp( arg, "water"	) ) return ELEMENT_WATER;
    if ( !str_cmp( arg, "earth"	) ) return ELEMENT_EARTH;
    if ( !str_cmp( arg, "wind"	) ) return ELEMENT_WIND;
    if ( !str_cmp( arg, "spirit") ) return ELEMENT_SPIRIT;
    return -1;
}

int get_mineral_type( char * arg )
{
    if ( !str_cmp( arg, "steel" 	) ) return MINERAL_STEEL;
    if ( !str_cmp( arg, "gold" 		) ) return MINERAL_GOLD;
    if ( !str_cmp( arg, "silver" 	) ) return MINERAL_SILVER;
    if ( !str_cmp( arg, "iron" 		) ) return MINERAL_IRON;
    if ( !str_cmp( arg, "copper" 	) ) return MINERAL_COPPER;
    if ( !str_cmp( arg, "bronze" 	) ) return MINERAL_BRONZE;
    if ( !str_cmp( arg, "diamond" 	) ) return MINERAL_DIAMOND;
    if ( !str_cmp( arg, "ruby" 		) ) return MINERAL_RUBY;
    if ( !str_cmp( arg, "sapphire" 	) ) return MINERAL_SAPPHIRE;
    if ( !str_cmp( arg, "emerald" 	) ) return MINERAL_EMERALD;
    if ( !str_cmp( arg, "obsidian" 	) ) return MINERAL_OBSIDIAN;
    if ( !str_cmp( arg, "platinum" 	) ) return MINERAL_PLATINUM;
    if ( !str_cmp( arg, "mithril" 	) ) return MINERAL_MITHRIL;
    if ( !str_cmp( arg, "adamantite" 	) ) return MINERAL_ADAMANTITE;
    if ( !str_cmp( arg, "alabaster" 	) ) return MINERAL_ALABASTER;
    if ( !str_cmp( arg, "titanium" 	) ) return MINERAL_TITANIUM;
    return -1;
}

char * cmd_group_type_name( int group )
{
    static char buf[512];

    buf[0] = '\0';
    if ( group & CMD_GROUP_ADMIN		) strcat(buf,  " `Cadmin`w"		);
    if ( group & CMD_GROUP_BASIC_BUILD		) strcat(buf,  " `Cbasic_build`w"	);
    if ( group & CMD_GROUP_ADVANCE_BUILD	) strcat(buf,  " `Cadvance_build`w"	);
    if ( group & CMD_GROUP_INFO			) strcat(buf,  " `Cinfo`w"		);
    if ( group & CMD_GROUP_DEFAULT		) strcat(buf,  " `Cdefault`w"	);
    if ( group & CMD_GROUP_GAME_CONTROL		) strcat(buf,  " `Cgame_control`w"	);
    if ( group & CMD_GROUP_ADVANCE_PHELP	) strcat(buf,  " `Cadvance_phelp`w"	);
    if ( group & CMD_GROUP_BASIC_PHELP		) strcat(buf,  " `Cbasic_phelp`w"	);
    if ( group & CMD_GROUP_PLAYER		) strcat(buf,  " `Cplayer`w"	);
    if ( group & CMD_GROUP_MOBILE		) strcat(buf,  " `Cmobile`w"	);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

int get_cmd_group_type( char * arg )
{
    if ( !str_cmp( arg, "admin"		) ) return CMD_GROUP_ADMIN;
    if ( !str_cmp( arg, "basic_build"	) ) return CMD_GROUP_BASIC_BUILD;
    if ( !str_cmp( arg, "info"		) ) return CMD_GROUP_INFO;
    if ( !str_cmp( arg, "default"	) ) return CMD_GROUP_DEFAULT;
    if ( !str_cmp( arg, "game_control"	) ) return CMD_GROUP_GAME_CONTROL;
    if ( !str_cmp( arg, "advance_phelp"	) ) return CMD_GROUP_ADVANCE_PHELP;
    if ( !str_cmp( arg, "advance_build"	) ) return CMD_GROUP_ADVANCE_BUILD;
    if ( !str_cmp( arg, "basic_phelp"	) ) return CMD_GROUP_BASIC_PHELP;
    if ( !str_cmp( arg, "player"	) ) return CMD_GROUP_PLAYER;
    if ( !str_cmp( arg, "mobile"	) ) return CMD_GROUP_MOBILE;
    return -1;
}
int get_quest_flags( char * arg )
{
    if ( !str_cmp( arg, "step1"	) ) return STEP1;
    if ( !str_cmp( arg, "step2"	) ) return STEP2;
    if ( !str_cmp( arg, "step3"	) ) return STEP3;
    if ( !str_cmp( arg, "step4"	) ) return STEP4;
    if ( !str_cmp( arg, "step5"	) ) return STEP5;
    if ( !str_cmp( arg, "step6"	) ) return STEP6;
    if ( !str_cmp( arg, "step7"	) ) return STEP7;
    if ( !str_cmp( arg, "step8"	) ) return STEP8;
    if ( !str_cmp( arg, "step9"	) ) return STEP9;
    if ( !str_cmp( arg, "step10") ) return STEP10;
    if ( !str_cmp( arg, "step11") ) return STEP11;
    if ( !str_cmp( arg, "step12") ) return STEP12;
    if ( !str_cmp( arg, "step13") ) return STEP13;
    if ( !str_cmp( arg, "step14") ) return STEP14;
    if ( !str_cmp( arg, "step15") ) return STEP15;
    if ( !str_cmp( arg, "step16") ) return STEP16;
    if ( !str_cmp( arg, "step17") ) return STEP17;
    if ( !str_cmp( arg, "step18") ) return STEP18;
    if ( !str_cmp( arg, "step19") ) return STEP19;
    if ( !str_cmp( arg, "step20") ) return STEP20;
    if ( !str_cmp( arg, "completed"	) ) return QUEST_COMPLETED;
    return -1;
}


char * target_type_name( int target )
{
    static char buf[512];

    buf[0] = '\0';
    if ( target &  TAR_IGNORE		) strcat(buf, " ignore"		);
    if ( target &  TAR_CHAR_SELF	) strcat(buf, " char_self"	);
    if ( target &  TAR_CHAR_OTHER	) strcat(buf, " char_other"	);
    if ( target &  TAR_OBJ_INV		) strcat(buf, " obj_inventory"	);
    if ( target &  TAR_CHAR_OFFENSIVE	) strcat(buf, " char_offensive"	);
    if ( target &  TAR_CHAR_DEFENSIVE	) strcat(buf, " char_defensive"	);
    if ( target &  TAR_CHAR_CURATIVE	) strcat(buf, " char_curative"	);
    if ( target &  TAR_CHAR_DAMAGING	) strcat(buf, " char_damaging"	);
    if ( target &  TAR_CHAR_CANCEL	) strcat(buf, " char_cancel"	);
    if ( target &  TAR_AREA		) strcat(buf, " area"		);
    if ( target &  START_FIGHTING	) strcat(buf, " start_fighting"	);
    if ( target &  SPELL_SILENT		) strcat(buf, " silent"		);
    if ( target &  SKILL_STATIC		) strcat(buf, " static"		);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char * trig_type_name( int trig )
{
    if ( trig == TRIG_COMMAND		) return "command";
    if ( trig == TRIG_EACH_PULSE	) return "each_pulse";
    if ( trig == TRIG_COMBAT		) return "combat";
    if ( trig == TRIG_TICK_PULSE	) return "tick_pulse";
    if ( trig == TRIG_BORN		) return "born";
    if ( trig == TRIG_GETS		) return "gets";
    if ( trig == TRIG_SAY		) return "say";
    if ( trig == TRIG_TELL		) return "tell";
    if ( trig == TRIG_KILLS_PLAYER	) return "kills_player";
    if ( trig == TRIG_DIES		) return "dies";
    if ( trig == TRIG_ENTER		) return "enter";
    if ( trig == TRIG_MOVES		) return "moves";
    if ( trig == TRIG_KILLS_MOB		) return "kills_mob";
    if ( trig == TRIG_LEAVES		) return "leaves";
    if ( trig == TRIG_LOOK		) return "look";
    if ( trig == TRIG_WEAR		) return "wear";
    if ( trig == TRIG_GIVE		) return "give";
    if ( trig == TRIG_DROP		) return "drop";
    if ( trig == TRIG_WIELD		) return "wield";
    if ( trig == TRIG_TROOM		) return "troom";
    if ( trig == TRIG_DOOR_OPEN		) return "door_open";
    if ( trig == TRIG_GAINS		) return "gains";
    return "none";
}

int get_trig_type( char * arg )
{
    if ( !str_cmp( arg, "command"	) ) return TRIG_COMMAND;
    if ( !str_cmp( arg, "each_pulse"	) ) return TRIG_EACH_PULSE;
    if ( !str_cmp( arg, "each"		) ) return TRIG_EACH_PULSE;
    if ( !str_cmp( arg, "combat"	) ) return TRIG_COMBAT;
    if ( !str_cmp( arg, "tick_pulse"	) ) return TRIG_TICK_PULSE;
    if ( !str_cmp( arg, "tick"		) ) return TRIG_TICK_PULSE;
    if ( !str_cmp( arg, "born"		) ) return TRIG_BORN;
    if ( !str_cmp( arg, "gets"		) ) return TRIG_GETS;
    if ( !str_cmp( arg, "get"		) ) return TRIG_GETS;
    if ( !str_cmp( arg, "say"		) ) return TRIG_SAY;
    if ( !str_cmp( arg, "tell"		) ) return TRIG_TELL;
    if ( !str_cmp( arg, "kills_player"	) ) return TRIG_KILLS_PLAYER;
    if ( !str_cmp( arg, "pkills"	) ) return TRIG_KILLS_PLAYER;
    if ( !str_cmp( arg, "pk"		) ) return TRIG_KILLS_PLAYER;
    if ( !str_cmp( arg, "dies"		) ) return TRIG_DIES;
    if ( !str_cmp( arg, "die"		) ) return TRIG_DIES;
    if ( !str_cmp( arg, "death"		) ) return TRIG_DIES;
    if ( !str_cmp( arg, "enter"		) ) return TRIG_ENTER;
    if ( !str_cmp( arg, "moves"		) ) return TRIG_MOVES;
    if ( !str_cmp( arg, "move"		) ) return TRIG_MOVES;
    if ( !str_cmp( arg, "kills_mob"	) ) return TRIG_KILLS_MOB;
    if ( !str_cmp( arg, "mkills"	) ) return TRIG_KILLS_MOB;
    if ( !str_cmp( arg, "mk"		) ) return TRIG_KILLS_MOB;
    if ( !str_cmp( arg, "leaves"	) ) return TRIG_LEAVES;
    if ( !str_cmp( arg, "leave"		) ) return TRIG_LEAVES;
    if ( !str_cmp( arg, "looks"		) ) return TRIG_LOOK;
    if ( !str_cmp( arg, "look"		) ) return TRIG_LOOK;
    if ( !str_cmp( arg, "wear"		) ) return TRIG_WEAR;
    if ( !str_cmp( arg, "wield"		) ) return TRIG_WIELD;
    if ( !str_cmp( arg, "give"		) ) return TRIG_GIVE;
    if ( !str_cmp( arg, "gives"		) ) return TRIG_GIVE;
    if ( !str_cmp( arg, "drop"		) ) return TRIG_DROP;
    if ( !str_cmp( arg, "drops"		) ) return TRIG_DROP;
    if ( !str_cmp( arg, "troom"		) ) return TRIG_TROOM;
    if ( !str_cmp( arg, "door"		) ) return TRIG_DOOR_OPEN;
    if ( !str_cmp( arg, "door_open"	) ) return TRIG_DOOR_OPEN;
    if ( !str_cmp( arg, "gain"		) ) return TRIG_GAINS;
    if ( !str_cmp( arg, "gains"		) ) return TRIG_GAINS;
    return -1;
}
int get_material_type( char * arg )
{
    if ( !str_cmp( arg, "steel"			) ) return MAT_STEEL;
    if ( !str_cmp( arg, "mithril"		) ) return MAT_MITHRIL;
    if ( !str_cmp( arg, "obsidian"		) ) return MAT_OBSIDIAN;
    if ( !str_cmp( arg, "stainless" 		) ) return MAT_STAINLESS_STEEL;
    if ( !str_cmp( arg, "stainless_steel"	) ) return MAT_STAINLESS_STEEL;
    if ( !str_cmp( arg, "stainlesssteel"	) ) return MAT_STAINLESS_STEEL;
    if ( !str_cmp( arg, "silver"		) ) return MAT_SILVER;
    if ( !str_cmp( arg, "ruby"			) ) return MAT_RUBY;
    if ( !str_cmp( arg, "gold"			) ) return MAT_GOLD;
    if ( !str_cmp( arg, "bronze"		) ) return MAT_BRONZE;
    if ( !str_cmp( arg, "sapphire"		) ) return MAT_SAPPHIRE;
    if ( !str_cmp( arg, "copper"		) ) return MAT_COPPER;
    if ( !str_cmp( arg, "titanium"		) ) return MAT_TITANIUM;
    if ( !str_cmp( arg, "emerald"		) ) return MAT_EMERALD;
    if ( !str_cmp( arg, "adamantite"		) ) return MAT_ADAMANTITE;
    if ( !str_cmp( arg, "alabaster"		) ) return MAT_ALABASTER;
    if ( !str_cmp( arg, "diamond"		) ) return MAT_DIAMOND;
    if ( !str_cmp( arg, "paper"			) ) return MAT_PAPER;
    if ( !str_cmp( arg, "glass"			) ) return MAT_GLASS;
    if ( !str_cmp( arg, "platinum"		) ) return MAT_PLATINUM;
    if ( !str_cmp( arg, "gem"			) ) return MAT_GEM_STONE;
    if ( !str_cmp( arg, "gem_stone"		) ) return MAT_GEM_STONE;
    if ( !str_cmp( arg, "gemstone"		) ) return MAT_GEM_STONE;
    if ( !str_cmp( arg, "wood"			) ) return MAT_WOOD;
    if ( !str_cmp( arg, "leather"		) ) return MAT_LEATHER;
    if ( !str_cmp( arg, "silk"			) ) return MAT_SILK;
    if ( !str_cmp( arg, "iron"			) ) return MAT_IRON;
    if ( !str_cmp( arg, "bone"			) ) return MAT_BONE;
    if ( !str_cmp( arg, "clay"			) ) return MAT_CLAY;
    if ( !str_cmp( arg, "rock"			) ) return MAT_ROCK;
    if ( !str_cmp( arg, "coral"			) ) return MAT_CORAL;
    if ( !str_cmp( arg, "stone"			) ) return MAT_STONE;
    if ( !str_cmp( arg, "cloth"			) ) return MAT_CLOTH;
    if ( !str_cmp( arg, "canvas"		) ) return MAT_CANVAS;
    if ( !str_cmp( arg, "fish"			) ) return MAT_FISH_SKIN;
    if ( !str_cmp( arg, "fish_skin"		) ) return MAT_FISH_SKIN;
    if ( !str_cmp( arg, "fishskin"		) ) return MAT_FISH_SKIN;
    if ( !str_cmp( arg, "pearl"			) ) return MAT_PEARL;
    return 0;
}

char * material_type_name( int arg )
{
    if ( arg == MAT_STEEL		 ) return "steel";
    if ( arg == MAT_MITHRIL		 ) return "mithril";
    if ( arg == MAT_OBSIDIAN	 	 ) return "obsidian";
    if ( arg == MAT_STAINLESS_STEEL	 ) return "stainless_steel";
    if ( arg == MAT_SILVER		 ) return "silver";
    if ( arg == MAT_RUBY		 ) return "ruby";
    if ( arg == MAT_GOLD		 ) return "gold";
    if ( arg == MAT_BRONZE		 ) return "brone";
    if ( arg == MAT_EMERALD		 ) return "emerald";
    if ( arg == MAT_COPPER		 ) return "copper";
    if ( arg == MAT_TITANIUM		 ) return "titanium";
    if ( arg == MAT_SAPPHIRE		 ) return "sapphire";
    if ( arg == MAT_ADAMANTITE		 ) return "adamantite";
    if ( arg == MAT_ALABASTER		 ) return "alabaster";
    if ( arg == MAT_DIAMOND		 ) return "diamond";
    if ( arg == MAT_PAPER		 ) return "paper";
    if ( arg == MAT_GLASS		 ) return "glass";
    if ( arg == MAT_PLATINUM		 ) return "platinum";
    if ( arg == MAT_GEM_STONE		 ) return "gem_stone";
    if ( arg == MAT_WOOD		 ) return "wood";
    if ( arg == MAT_LEATHER		 ) return "leather";
    if ( arg == MAT_SILK		 ) return "silk";
    if ( arg == MAT_IRON		 ) return "iron";
    if ( arg == MAT_BONE		 ) return "bone";
    if ( arg == MAT_CLAY		 ) return "clay";
    if ( arg == MAT_ROCK		 ) return "rock";
    if ( arg == MAT_CORAL		 ) return "coral";
    if ( arg == MAT_STONE		 ) return "stone";
    if ( arg == MAT_CLOTH		 ) return "cloth";
    if ( arg == MAT_CANVAS		 ) return "canvas";
    if ( arg == MAT_FISH_SKIN		 ) return "fish_skin";
    if ( arg == MAT_PEARL		 ) return "pearl";
    return "none";
}

char * note_flag_name( int arg )
{
    static char buf[MAX_INPUT_LENGTH];

    buf[0] = '\0';
    if ( IS_SET( arg, NOTE_NO_DELETE ) ) strcat( buf, " no_delete" );
    if ( IS_SET( arg, NOTE_RECIEPT   ) ) strcat( buf, " reciept" );

    return ( buf[0] == '\0' ? str_dup("none") : &buf[1] );
}

int get_note_flag ( char * arg )
{
    if ( !str_cmp( arg, "no_delete" ) ) return NOTE_NO_DELETE;
    if ( !str_cmp( arg, "nodelete"  ) ) return NOTE_NO_DELETE;
    if ( !str_cmp( arg, "receipt"   ) ) return NOTE_RECIEPT;
    return 0;
}

int position_value( char * arg )
{
    if ( !str_prefix( arg, "mortally"		) ) return POS_MORTAL;
    if ( !str_prefix( arg, "wounded"		) ) return POS_MORTAL;
    if ( !str_prefix( arg, "incapacitated"	) ) return POS_INCAP;
    if ( !str_prefix( arg, "stunned"		) ) return POS_STUNNED;
    if ( !str_prefix( arg, "sleeping"		) ) return POS_SLEEPING;
    if ( !str_prefix( arg, "resting"		) ) return POS_RESTING;
    if ( !str_prefix( arg, "sitting"		) ) return POS_SITTING;
    if ( !str_prefix( arg, "fighting"		) ) return POS_FIGHTING;
    if ( !str_prefix( arg, "standing"		) ) return POS_STANDING;
    return -1;
}

char *get_pos_name( int pos )
{
    if ( pos == POS_MORTAL	) return "Mortally wounded";
    if ( pos == POS_INCAP	) return "Incapacitated";
    if ( pos == POS_STUNNED	) return "Stunned";
    if ( pos == POS_SLEEPING	) return "Sleeping";
    if ( pos == POS_RESTING	) return "Resting";
    if ( pos == POS_SITTING	) return "Sitting";
    if ( pos == POS_FIGHTING	) return "Fighting";
    if ( pos == POS_STANDING	) return "Standing";
    return "Dead";
}

char *shop_type_name( int arg )
{
    static char buf[MAX_INPUT_LENGTH];

    buf[0]='\0';
    if ( IS_SET( arg, SHOP_BAKER	) ) strcat( buf, " baker" );
    if ( IS_SET( arg, SHOP_MAGIC	) ) strcat( buf, " magic" );
    if ( IS_SET( arg, SHOP_BAR		) ) strcat( buf, " bar" );
    if ( IS_SET( arg, SHOP_ARMOUR	) ) strcat( buf, " armor" );
    if ( IS_SET( arg, SHOP_WEAPON	) ) strcat( buf, " weapon" );
    if ( IS_SET( arg, SHOP_JEWELLER	) ) strcat( buf, " jeweller" );
    if ( IS_SET( arg, SHOP_GROCER	) ) strcat( buf, " grocer" );
    if ( IS_SET( arg, SHOP_BOAT		) ) strcat( buf, " boat" );
    if ( IS_SET( arg, SHOP_FURNITURE	) ) strcat( buf, " furniture" );
    if ( IS_SET( arg, SHOP_BARDING	) ) strcat( buf, " barding" );
 /* if ( IS_SET( arg, SHOP_OWNABLE	) ) strcat( buf, " ownable" ); */
    return ( buf[0] == ' ' ? &buf[1] : str_dup("none") );
}


int shop_name_type(char *arg)
{
    if ( !str_cmp( arg, "baker" ) ) return SHOP_BAKER;
    if ( !str_cmp( arg, "magic" ) ) return SHOP_MAGIC;
    if ( !str_cmp( arg, "magician" ) ) return SHOP_MAGIC;
    if ( !str_cmp( arg, "bar" ) ) return SHOP_BAR;
    if ( !str_cmp( arg, "bartender" ) ) return SHOP_BAR;
    if ( !str_cmp( arg, "armour" ) ) return SHOP_ARMOUR;
    if ( !str_cmp( arg, "armourer" ) ) return SHOP_ARMOUR;
    if ( !str_cmp( arg, "armor" ) ) return SHOP_ARMOUR;
    if ( !str_cmp( arg, "armorer" ) ) return SHOP_ARMOUR;
    if ( !str_cmp( arg, "weapon" ) ) return SHOP_WEAPON;
    if ( !str_cmp( arg, "weaponsmith" ) ) return SHOP_WEAPON;
    if ( !str_cmp( arg, "jeweller" ) ) return SHOP_JEWELLER;
    if ( !str_cmp( arg, "grocer" ) ) return SHOP_GROCER;
    if ( !str_cmp( arg, "furniture" ) ) return SHOP_FURNITURE;
    if ( !str_cmp( arg, "boat" ) ) return SHOP_BOAT;
    if ( !str_cmp( arg, "barding" ) ) return SHOP_BARDING;
/*  if ( !str_cmp( arg, "ownable" ) ) return SHOP_OWNABLE; */
    return 0;
}

int item_name_type(char *name)
{
/* done */
    if ( !str_cmp( name, "scroll"	) ) return ITEM_SCROLL;
    if ( !str_cmp( name, "wand"		) ) return ITEM_WAND;
    if ( !str_cmp( name, "staff"	) ) return ITEM_STAFF;
    if ( !str_cmp( name, "weapon"	) ) return ITEM_WEAPON;
    if ( !str_cmp( name, "armor"	) ) return ITEM_ARMOR;
    if ( !str_cmp( name, "potion"	) ) return ITEM_POTION;
    if ( !str_cmp( name, "drink"	) ) return ITEM_DRINK_CON;
    if ( !str_cmp( name, "fountain"	) ) return ITEM_FOUNTAIN;
    if ( !str_cmp( name, "food"		) ) return ITEM_FOOD;
    if ( !str_cmp( name, "gate"		) ) return ITEM_GATE;
    if ( !str_cmp( name, "key"		) ) return ITEM_KEY;
    if ( !str_cmp( name, "map"		) ) return ITEM_MAP;
    if ( !str_cmp( name, "money"	) ) return ITEM_MONEY;
    if ( !str_cmp( name, "clothing"	) ) return ITEM_CLOTHING;
    if ( !str_cmp( name, "furniture"	) ) return ITEM_FURNITURE;
    if ( !str_cmp( name, "trash"	) ) return ITEM_TRASH;
    if ( !str_cmp( name, "treasure"	) ) return ITEM_TREASURE;
    if ( !str_cmp( name, "protect"	) ) return ITEM_PROTECT;
    if ( !str_cmp( name, "pc"		) ) return ITEM_PC;
    if ( !str_cmp( name, "no_food"	) ) return ITEM_NO_FOOD_REQ;
    if ( !str_cmp( name, "no_water"	) ) return ITEM_NO_WATER_REQ;
    if ( !str_cmp( name, "no_food_water") ) return ITEM_NO_FOOD_OR_WATER;
    if ( !str_cmp( name, "poisoned"	) ) return ITEM_POISONED;
    if ( !str_cmp( name, "container"	) ) return ITEM_CONTAINER;
    if ( !str_cmp( name, "corpse_npc"	) ) return ITEM_CORPSE_NPC;
    if ( !str_cmp( name, "corpse_pc"	) ) return ITEM_CORPSE_PC;
    if ( !str_cmp( name, "boat"		) ) return ITEM_BOAT;
    if ( !str_cmp( name, "barding"	) ) return ITEM_TYPE_BARDING;
    if ( !str_cmp( name, "light"	) ) return ITEM_LIGHT;
    if ( !str_cmp( name, "fuel"		) ) return ITEM_FUEL;
    if ( !str_cmp( name, "lighter"	) ) return ITEM_LIGHTER;

    return 0;
}

int special_name_bit( char* buf )
{
    if (!str_cmp( buf, "stringshare"		) ) return ITEM_SPEC_STRING        ;
    return 0;
}

int extra_name_bit( char* buf )
{
    if (!str_cmp( buf, "glow"		) ) return ITEM_GLOW        ;
    if (!str_cmp( buf, "hum"		) ) return ITEM_HUM         ;
    if (!str_cmp( buf, "no_show"	) ) return ITEM_NO_SHOW     ;
    if (!str_cmp( buf, "buckler"	) ) return ITEM_BUCKLER     ;
    if (!str_cmp( buf, "hammer"		) ) return ITEM_HAMMER      ;
    if (!str_cmp( buf, "shadowform"	) ) return ITEM_SHADOWFORM  ;
    if (!str_cmp( buf, "magic"		) ) return ITEM_MAGIC       ;
    if (!str_cmp( buf, "nodrop"		) ) return ITEM_NODROP      ;
    if (!str_cmp( buf, "anti_nenkemen"	) ) return ITEM_ANTI_NENKEMEN;
    if (!str_cmp( buf, "anti_maegmenel"	) ) return ITEM_ANTI_MAEGMENEL;
    if (!str_cmp( buf, "anti_lithdor"	) ) return ITEM_ANTI_LITHDOR;
    if (!str_cmp( buf, "noremove"	) ) return ITEM_NOREMOVE    ;
    if (!str_cmp( buf, "stay_on_death"	) ) return ITEM_STAY_ON_DEATH    ;
    if (!str_cmp( buf, "inventory"	) ) return ITEM_INVENTORY   ;
    if (!str_cmp( buf, "poisoned"	) ) return ITEM_POISONED    ;
    if (!str_cmp( buf, "nopurge"	) ) return ITEM_NOPURGE    ;
    if (!str_cmp( buf, "rot_death"	) ) return ITEM_ROT_DEATH    ;
    if (!str_cmp( buf, "vis_death"	) ) return ITEM_VIS_DEATH    ;
    if (!str_cmp( buf, "lit"		) ) return ITEM_LIT    ;
    if (!str_cmp( buf, "magic_lit"	) ) return ITEM_MAGIC_LIT    ;
    if (!str_cmp( buf, "no_save"	) ) return ITEM_NO_SAVE    ;
    if (!str_cmp( buf, "no_locate"	) ) return ITEM_NO_LOCATE    ;
    if (!str_cmp( buf, "assayed"	) ) return ITEM_ASSAYED;
    if (!str_cmp( buf, "buried"		) ) return ITEM_BURIED;
    if (!str_cmp( buf, "no_identify"	) ) return ITEM_NO_IDENTIFY    ;
    if (!str_cmp( buf, "float"		) ) return ITEM_FLOAT;
    if (!str_cmp( buf, "barding"	) ) return ITEM_BARDING;
    if (!str_cmp( buf, "mob_inside"	) ) return ITEM_MOB_INSIDE;
    if (!str_cmp( buf, "home"		) ) return ITEM_HOME;
    if (!str_cmp( buf, "newbie"		) ) return ITEM_NEWBIE;
    if (!str_cmp( buf, "pick"		) ) return ITEM_PICK;
    return 0;
}

char *gate_flag_name( int flag )
{
    static char buf[512];

    buf[0] ='\0';
    if ( IS_SET( flag, GATE_STAY_ROOM ) ) strcat( buf, " stay_room" );
    if ( IS_SET( flag, GATE_STAY_AREA ) ) strcat( buf, " stay_area" );
    if ( IS_SET( flag, GATE_STAY_WORLD ) ) strcat( buf, " stay_world" );
    if ( IS_SET( flag, GATE_TO_STAY_ROOM ) ) strcat( buf, " to_stay_room" );
    if ( IS_SET( flag, GATE_TO_STAY_AREA ) ) strcat( buf, " to_stay_area" );
    if ( IS_SET( flag, GATE_TO_STAY_WORLD ) ) strcat( buf, " to_stay_world" );
    if ( IS_SET( flag, GATE_OPAQUE ) ) strcat( buf, " opaque" );
    if ( IS_SET( flag, GATE_UNSTABLE ) ) strcat( buf, " unstable" );
    if ( IS_SET( flag, GATE_TO_OBJ ) ) strcat( buf, " to_obj" );
    if ( IS_SET( flag, GATE_TO_MOB ) ) strcat( buf, " to_mob" );

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *light_flag_name( int flag )
{
    static char buf[512];

    buf[0] ='\0';
    if ( IS_SET( flag, LIGHT_MAGIC_LIT ) ) strcat( buf, " magic_lit" );
    if ( IS_SET( flag, LIGHT_LIT ) ) strcat( buf, " lit" );
    if ( IS_SET( flag, LIGHT_BURNING ) ) strcat( buf, " burning" );
    if ( IS_SET( flag, LIGHT_LIGHTABLE ) ) strcat( buf, " lightable" );
    if ( IS_SET( flag, LIGHT_REFUELABLE ) ) strcat( buf, " refuelable" );
    if ( IS_SET( flag, LIGHT_FUEL ) ) strcat( buf, " fuel" );

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

int get_light_flag( char * arg )
{
    if ( !str_cmp( arg, "magic_lit" ) ) return LIGHT_MAGIC_LIT;
    if ( !str_cmp( arg, "lit" ) ) return LIGHT_LIT;
    if ( !str_cmp( arg, "burning" ) ) return LIGHT_BURNING;
    if ( !str_cmp( arg, "lightable" ) ) return LIGHT_LIGHTABLE;
    if ( !str_cmp( arg, "refuelable" ) ) return LIGHT_REFUELABLE;
    if ( !str_cmp( arg, "fuel" ) ) return LIGHT_FUEL;
    return 0;
}

int get_gate_flag( char * arg )
{
    if ( !str_cmp( arg, "stay_room" ) ) return GATE_STAY_ROOM;
    if ( !str_cmp( arg, "stay_area" ) ) return GATE_STAY_AREA;
    if ( !str_cmp( arg, "stay_world" ) ) return GATE_STAY_WORLD;
    if ( !str_cmp( arg, "to_stay_room" ) ) return GATE_TO_STAY_ROOM;
    if ( !str_cmp( arg, "to_stay_area" ) ) return GATE_TO_STAY_AREA;
    if ( !str_cmp( arg, "to_stay_world" ) ) return GATE_TO_STAY_WORLD;
    if ( !str_cmp( arg, "unstable" ) ) return GATE_UNSTABLE;
    if ( !str_cmp( arg, "opaque" ) ) return GATE_OPAQUE;
    if ( !str_cmp( arg, "to_mob" ) ) return GATE_TO_MOB;
    if ( !str_cmp( arg, "to_obj" ) ) return GATE_TO_OBJ;

    return 0;
}

char *edible_flag_list_name( int flag )
{
    static char buf[512];

    buf[0] ='\0';
    if ( IS_SET( flag, FOOD_POISONED ) ) strcat( buf, " poisoned" );
    if ( IS_SET( flag, FOOD_UNCOOKED ) ) strcat( buf, " uncooked" );
    if ( IS_SET( flag, FOOD_EDIBLE )   ) strcat( buf, " food" );
    if ( IS_SET( flag, FOOD_DRINK )   ) strcat( buf, " drink" );

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}
char *edible_flag_name( int flag )
{
    static char buf[512];

    buf[0] ='\0';
    if ( IS_SET( flag, FOOD_POISONED ) ) strcat( buf, " poisoned" );
    if ( IS_SET( flag, FOOD_UNCOOKED ) ) strcat( buf, " uncooked" );
    if ( IS_SET( flag, FOOD_EDIBLE )   ) strcat( buf, " food" );
	else strcat( buf, " drink" );

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

int get_edible_type( char * arg )
{
    int type;

    for ( type = 0 ; type < LIQ_MAX ; type++ )
	if ( is_name( arg, liq_table[ type ].liq_name ) )
	   return type;

    return -1;
}

int get_edible_flag( char * arg )
{
    if ( !str_cmp( arg, "poisoned" ) ) return FOOD_POISONED;
    if ( !str_cmp( arg, "food" )     ) return FOOD_EDIBLE;
    if ( !str_cmp( arg, "drink" )     ) return FOOD_DRINK;
    if ( !str_cmp( arg, "uncooked" ) ) return FOOD_UNCOOKED;

    return 0;
}

char *magical_flag_name( int arg )
{
    static char buf[512];

    buf[0] ='\0';
    if ( IS_SET( arg, ROT_USED ) ) strcat( buf, " rot_used" );

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
} 

char *magical_type_name( int arg )
{
    if ( arg == MAGIC_WAND   ) return "Wand ";
    if ( arg == MAGIC_STAFF  ) return "Staff ";
    if ( arg == MAGIC_SCROLL ) return "Scroll ";

    return "(unset magical type)";
}


int get_magical_flag( char *arg )
{
    if ( !str_cmp( arg, "rot_used" ) ) return ROT_USED;

    return 0;
}

int get_magical_type( char *arg )
{
    if (!str_cmp( arg, "staff" ) ) return MAGIC_STAFF;
    if (!str_cmp( arg, "wand" ) ) return MAGIC_WAND;
    if (!str_cmp( arg, "scroll" ) ) return MAGIC_SCROLL;

    return 0;
}

int get_storm_type( char *arg )
{
    if (!str_prefix( arg, "lightning" ) ) return SKY_LIGHTNING;
    if (!str_prefix( arg, "blizzard" ) ) return SKY_BLIZZARD;
    if (!str_prefix( arg, "huricane" ) ) return SKY_HURICANE;
    if (!str_prefix( arg, "tornado" ) ) return SKY_TORNADO;
    if (!str_prefix( arg, "sandstorm" ) ) return SKY_SANDSTORM;
    return -1;
}

/*
 * Returns the name of a wear bit.
 */
char *wear_bit_name_1( int wear )
{
    static char buf[512];

    buf[0] = '\0';
    if ( wear & ITEM_TAKE        ) strcat( buf, " take"       );
    if ( wear & ITEM_WEAR_FINGER ) strcat( buf, " finger"     );
    if ( wear & ITEM_WEAR_EAR	 ) strcat( buf,	" ear"	      );
    if ( wear & ITEM_WEAR_NECK   ) strcat( buf, " neck"       );
    if ( wear & ITEM_WEAR_BODY   ) strcat( buf, " body"       );
    if ( wear & ITEM_WEAR_HEAD   ) strcat( buf, " head"       );
    if ( wear & ITEM_WEAR_LEGS   ) strcat( buf, " legs"       );
    if ( wear & ITEM_WEAR_FEET   ) strcat( buf, " feet"       );
    if ( wear & ITEM_WEAR_HANDS  ) strcat( buf, " hands"      );
    if ( wear & ITEM_WEAR_ARMS   ) strcat( buf, " arms"       );
    if ( wear & ITEM_WEAR_SHIELD ) strcat( buf, " shield"     );
    if ( wear & ITEM_WEAR_ABOUT  ) strcat( buf, " about"      );
    if ( wear & ITEM_WEAR_WAIST  ) strcat( buf, " waist"      );
    if ( wear & ITEM_WEAR_WRIST  ) strcat( buf, " wrist"      );
    if ( wear & ITEM_WIELD       ) strcat( buf, " wield"      );
    if ( wear & ITEM_HOLD        ) strcat( buf, " hold"       );
    if ( wear & ITEM_TWO_HANDS   ) strcat( buf, " two_hands"  );
    if ( wear & ITEM_MONEY_0	 ) strcat( buf, " crowns"     );
    if ( wear & ITEM_MONEY_1	 ) strcat( buf, " duckets"    );
    if ( wear & ITEM_MONEY_2	 ) strcat( buf, " wheels"     );
    if ( wear & ITEM_MONEY_3	 ) strcat( buf, " greckles"   );
    if ( wear & ITEM_WEAR_LIGHT	 ) strcat( buf, " light"      );
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}
char *get_event_flags( int event )
{
    static char buf[512];

    buf[0] = '\0';
    if ( event & EVENT_ON_BOOT		) strcat( buf, " on_boot"	);
    if ( event & EVENT_ON_RESET		) strcat( buf, " on_reset"	);
    if ( event & EVENT_INVALID		) strcat( buf, " invalid"	);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}
int get_flags_event( char *arg )
{
    if (!str_cmp( arg, "boot" ) ) return EVENT_ON_BOOT;
    if (!str_cmp( arg, "on_boot" ) ) return EVENT_ON_BOOT;
    if (!str_cmp( arg, "reset" ) ) return EVENT_ON_RESET;
    if (!str_cmp( arg, "on_reset" ) ) return EVENT_ON_RESET;
    if (!str_cmp( arg, "invalid" ) ) return EVENT_INVALID;

    return 0;
}
char *get_equip_flags( int equip )
{
    static char buf[512];

    buf[0] = '\0';
    if ( equip & EQUIP_ON_BOOT		) strcat( buf, " on_boot"	);
    if ( equip & EQUIP_ON_RESET		) strcat( buf, " on_reset"	);
    if ( equip & EQUIP_INVALID		) strcat( buf, " invalid"	);
    if ( equip & EQUIP_INVENTORY	) strcat( buf, " inventory"	);
    if ( equip & EQUIP_REPLACE		) strcat( buf, " replace"	);
    if ( equip & EQUIP_ON_CREATION	) strcat( buf, " creation"	);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}
int get_flags_equip( char *arg )
{
    if (!str_cmp( arg, "boot" ) ) return EQUIP_ON_BOOT;
    if (!str_cmp( arg, "on_boot" ) ) return EQUIP_ON_BOOT;
    if (!str_cmp( arg, "reset" ) ) return EQUIP_ON_RESET;
    if (!str_cmp( arg, "on_reset" ) ) return EQUIP_ON_RESET;
    if (!str_cmp( arg, "invalid" ) ) return EQUIP_INVALID;
    if (!str_cmp( arg, "inventory" ) ) return EQUIP_INVENTORY;
    if (!str_cmp( arg, "inv" ) ) return EQUIP_INVENTORY;
    if (!str_cmp( arg, "creation" ) ) return EQUIP_ON_CREATION;
    if (!str_cmp( arg, "replace" ) ) return EQUIP_REPLACE;

    return 0;
}

char *clan_flag_name( int clan_info )
{
    static char buf[512];

    buf[0] = '\0';
    if ( clan_info & CLAN_UNLISTED ) strcat( buf, " unlisted" );
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

int wear_location_flag( int wear )
{
    switch( wear )
    {
               default: return 0;
        case WEAR_NONE: return 0;
       case WEAR_LIGHT: return ITEM_WEAR_LIGHT;
    case WEAR_FINGER_L: return ITEM_WEAR_FINGER;
    case WEAR_FINGER_R: return ITEM_WEAR_FINGER;
    case WEAR_EAR_L:	return ITEM_WEAR_EAR;
    case WEAR_EAR_R:	return ITEM_WEAR_EAR;
      case WEAR_NECK_1: return ITEM_WEAR_NECK;
      case WEAR_NECK_2: return ITEM_WEAR_NECK;
        case WEAR_BODY: return ITEM_WEAR_BODY;
        case WEAR_HEAD: return ITEM_WEAR_HEAD;
        case WEAR_LEGS: return ITEM_WEAR_LEGS;
        case WEAR_FEET: return ITEM_WEAR_FEET;
       case WEAR_HANDS: return ITEM_WEAR_HANDS;
        case WEAR_ARMS: return ITEM_WEAR_ARMS;
      case WEAR_SHIELD: return ITEM_WEAR_SHIELD;
       case WEAR_ABOUT: return ITEM_WEAR_ABOUT;
       case WEAR_WAIST: return ITEM_WEAR_WAIST;
     case WEAR_WRIST_L: return ITEM_WEAR_WRIST;
     case WEAR_WRIST_R: return ITEM_WEAR_WRIST;
      case WEAR_WIELD_R: return ITEM_WIELD;
      case WEAR_WIELD_L: return ITEM_WIELD;
        case WEAR_HOLD: return ITEM_HOLD;
case WEAR_WIELD_TWO_HANDED: return ITEM_TWO_HANDS;
    }
}

/*
 * Returns the location of a wear bit.
 */
int wear_bit_location( int wear )
{
    if ( wear & ITEM_WEAR_FINGER ) return WEAR_FINGER_L;
    if ( wear & ITEM_WEAR_EAR	 ) return WEAR_EAR_L;
    if ( wear & ITEM_WEAR_NECK   ) return WEAR_NECK_1;
    if ( wear & ITEM_WEAR_BODY   ) return WEAR_BODY;
    if ( wear & ITEM_WEAR_HEAD   ) return WEAR_HEAD;
    if ( wear & ITEM_WEAR_LEGS   ) return WEAR_LEGS;
    if ( wear & ITEM_WEAR_FEET   ) return WEAR_FEET;
    if ( wear & ITEM_WEAR_HANDS  ) return WEAR_HANDS;
    if ( wear & ITEM_WEAR_ARMS   ) return WEAR_ARMS;
    if ( wear & ITEM_WEAR_SHIELD ) return WEAR_SHIELD;
    if ( wear & ITEM_WEAR_ABOUT  ) return WEAR_ABOUT;
    if ( wear & ITEM_WEAR_WAIST  ) return WEAR_WAIST;
    if ( wear & ITEM_WEAR_WRIST  ) return WEAR_WRIST_L;
    if ( wear & ITEM_WIELD       ) return WEAR_WIELD;
    if ( wear & ITEM_HOLD        ) return WEAR_HOLD;
    if ( wear & ITEM_TWO_HANDS   ) return WEAR_WIELD_TWO_HANDED;
    return WEAR_NONE;
}
int wear_bit_location_2( int wear )
{
    if ( wear & ITEM_WEAR_FINGER ) return WEAR_FINGER_R;
    if ( wear & ITEM_WEAR_FINGER ) return WEAR_FINGER_R;
    if ( wear & ITEM_WEAR_EAR ) return WEAR_EAR_R;
    if ( wear & ITEM_WEAR_EAR ) return WEAR_EAR_R;
    if ( wear & ITEM_WEAR_NECK   ) return WEAR_NECK_2;
    if ( wear & ITEM_WEAR_BODY   ) return WEAR_BODY;
    if ( wear & ITEM_WEAR_HEAD   ) return WEAR_HEAD;
    if ( wear & ITEM_WEAR_LEGS   ) return WEAR_LEGS;
    if ( wear & ITEM_WEAR_FEET   ) return WEAR_FEET;
    if ( wear & ITEM_WEAR_HANDS  ) return WEAR_HANDS;
    if ( wear & ITEM_WEAR_ARMS   ) return WEAR_ARMS;
    if ( wear & ITEM_WEAR_SHIELD ) return WEAR_SHIELD;
    if ( wear & ITEM_WEAR_ABOUT  ) return WEAR_ABOUT;
    if ( wear & ITEM_WEAR_WAIST  ) return WEAR_WAIST;
    if ( wear & ITEM_WEAR_WRIST  ) return WEAR_WRIST_R;
    if ( wear & ITEM_WIELD       ) return WEAR_WIELD;
    if ( wear & ITEM_HOLD        ) return WEAR_HOLD;
    if ( wear & ITEM_TWO_HANDS   ) return WEAR_WIELD_TWO_HANDED;
    return WEAR_NONE;
}


/*
 * Returns the bit, given a certain name.
 */
int wear_name_bit( char* buf )
{
    if (!str_cmp( buf, "take"       ) ) return ITEM_TAKE;
    if (!str_cmp( buf, "finger"     ) ) return ITEM_WEAR_FINGER;
    if (!str_cmp( buf,  "ear"	    ) ) return ITEM_WEAR_EAR;
    if (!str_cmp( buf, "neck"       ) ) return ITEM_WEAR_NECK;
    if (!str_cmp( buf, "body"       ) ) return ITEM_WEAR_BODY;
    if (!str_cmp( buf, "head"       ) ) return ITEM_WEAR_HEAD;
    if (!str_cmp( buf, "legs"       ) ) return ITEM_WEAR_LEGS;
    if (!str_cmp( buf, "feet"       ) ) return ITEM_WEAR_FEET;
    if (!str_cmp( buf, "hands"      ) ) return ITEM_WEAR_HANDS;
    if (!str_cmp( buf, "arms"       ) ) return ITEM_WEAR_ARMS;
    if (!str_cmp( buf, "shield"     ) ) return ITEM_WEAR_SHIELD;
    if (!str_cmp( buf, "about"      ) ) return ITEM_WEAR_ABOUT;
    if (!str_cmp( buf, "waist"      ) ) return ITEM_WEAR_WAIST;
    if (!str_cmp( buf, "wrist"      ) ) return ITEM_WEAR_WRIST;
    if (!str_cmp( buf, "wield"      ) ) return ITEM_WIELD;
    if (!str_cmp( buf, "hold"       ) ) return ITEM_HOLD;
    if (!str_cmp( buf, "two_hands"  ) ) return ITEM_TWO_HANDS;
    if (!str_cmp( buf, "crowns"     ) ) return ITEM_MONEY_0;
    if (!str_cmp( buf, "duckets"    ) ) return ITEM_MONEY_1;
    if (!str_cmp( buf, "wheels"     ) ) return ITEM_MONEY_2;
    if (!str_cmp( buf, "greckles"   ) ) return ITEM_MONEY_3;
    if (!str_cmp( buf, "crown"      ) ) return ITEM_MONEY_0;
    if (!str_cmp( buf, "ducket"     ) ) return ITEM_MONEY_1;
    if (!str_cmp( buf, "wheel"      ) ) return ITEM_MONEY_2;
    if (!str_cmp( buf, "greckle"    ) ) return ITEM_MONEY_3;
    if (!str_cmp( buf, "light"      ) ) return ITEM_WEAR_LIGHT;
    return 0;
}


int res_name_bit( char* buf )
{
    if ( !str_cmp( buf, "control_mind"    		) ) return RES_CONTROL_MIND;
    if ( !str_cmp( buf, "magic"   			) ) return RES_MAGIC;
    if ( !str_cmp( buf, "weapon"    			) ) return RES_WEAPON;
    if ( !str_cmp( buf, "blunt"        			) ) return RES_BASH;
    if ( !str_cmp( buf, "pierce"          		) ) return RES_PIERCE;
    if ( !str_cmp( buf, "slash"      			) ) return RES_SLASH;
    if ( !str_cmp( buf, "fire" 				) ) return RES_FIRE;
    if ( !str_cmp( buf, "lightning"       		) ) return RES_LIGHTNING;
    if ( !str_cmp( buf, "acid"       			) ) return RES_ACID;
    if ( !str_cmp( buf, "poison"        		) ) return RES_POISON;
    if ( !str_cmp( buf, "energy"      			) ) return RES_ENERGY;
    if ( !str_cmp( buf, "mental"      			) ) return RES_MENTAL;
    if ( !str_cmp( buf, "disease"    			) ) return RES_DISEASE;
    if ( !str_cmp( buf, "drowning"     			) ) return RES_DROWNING;
    if ( !str_cmp( buf, "light"       			) ) return RES_LIGHT;
    if ( !str_cmp( buf, "wind"       			) ) return RES_WIND;
    if ( !str_cmp( buf, "water"       			) ) return RES_WATER;
    if ( !str_cmp( buf, "spirit"       			) ) return RES_SPIRIT;
    if ( !str_cmp( buf, "earth"       			) ) return RES_EARTH;
    return 0;
}

int vuln_name_bit( char* buf )
{
    if ( !str_cmp( buf, "magic"			) ) return VULN_MAGIC;
    if ( !str_cmp( buf, "weapon"		) ) return VULN_WEAPON;
    if ( !str_cmp( buf, "blunt"        		) ) return VULN_BASH;
    if ( !str_cmp( buf, "pierce"          	) ) return VULN_PIERCE;
    if ( !str_cmp( buf, "slash"      		) ) return VULN_SLASH;
    if ( !str_cmp( buf, "fire" 			) ) return VULN_FIRE;
    if ( !str_cmp( buf, "lightning"       	) ) return VULN_LIGHTNING;
    if ( !str_cmp( buf, "acid"       		) ) return VULN_ACID;
    if ( !str_cmp( buf, "poison"        	) ) return VULN_POISON;
    if ( !str_cmp( buf, "energy"      		) ) return VULN_ENERGY;
    if ( !str_cmp( buf, "mental"      		) ) return VULN_MENTAL;
    if ( !str_cmp( buf, "disease"    		) ) return VULN_DISEASE;
    if ( !str_cmp( buf, "drowning"     		) ) return VULN_DROWNING;
    if ( !str_cmp( buf, "light"       		) ) return VULN_LIGHT;
    if ( !str_cmp( buf, "wind"       		) ) return VULN_WIND;
    if ( !str_cmp( buf, "water"       		) ) return VULN_WATER;
    if ( !str_cmp( buf, "spirit"       		) ) return VULN_SPIRIT;
    if ( !str_cmp( buf, "earth"       		) ) return VULN_EARTH;
    return 0;
}

int imm_name_bit( char* buf )
{
    if ( !str_cmp( buf, "summon"		) ) return IMM_SUMMON;
    if ( !str_cmp( buf, "control_mind"		) ) return IMM_CONTROL_MIND;
    if ( !str_cmp( buf, "magic"			) ) return IMM_MAGIC;
    if ( !str_cmp( buf, "weapon"		) ) return IMM_WEAPON;
    if ( !str_cmp( buf, "blunt"			) ) return IMM_BASH;
    if ( !str_cmp( buf, "pierce"		) ) return IMM_PIERCE;
    if ( !str_cmp( buf, "slash"			) ) return IMM_SLASH;
    if ( !str_cmp( buf, "fire"			) ) return IMM_FIRE;
    if ( !str_cmp( buf, "lightning"		) ) return IMM_LIGHTNING;
    if ( !str_cmp( buf, "acid"			) ) return IMM_ACID;
    if ( !str_cmp( buf, "poison"		) ) return IMM_POISON;
    if ( !str_cmp( buf, "energy"		) ) return IMM_ENERGY;
    if ( !str_cmp( buf, "mental"		) ) return IMM_MENTAL;
    if ( !str_cmp( buf, "disease"		) ) return IMM_DISEASE;
    if ( !str_cmp( buf, "drowning"		) ) return IMM_DROWNING;
    if ( !str_cmp( buf, "light"			) ) return IMM_LIGHT;
    if ( !str_cmp( buf, "wind"			) ) return IMM_WIND;
    if ( !str_cmp( buf, "water"			) ) return IMM_WATER;
    if ( !str_cmp( buf, "spirit"		) ) return IMM_SPIRIT;
    if ( !str_cmp( buf, "earth"			) ) return IMM_EARTH;
    return 0;
}

int guild_name_bit( char* buf )
{
    int guild;

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
        if ( !str_cmp( buf, guild_table[ guild ].name	) ) return guild_table[ guild ].flag;
    return 0;
}

char *guild_bit_name( int guild_flags )
{
    static char buf[512];
    int guild;

    buf[0] = '\0';

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
        if (guild_flags & guild_table[ guild ].flag		)
	{
	    strcat( buf, " " );
	    strcat(buf, guild_table[ guild ].name );
	}

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *guild_bit_name_2(int guild_flags)
{
    static char buf[512];
    int guild;

    buf[0] = '\0';

    for ( guild = 0 ; guild < MAX_GUILD ; guild++ )
        if (guild_flags & guild_table[ guild ].flag		)
	{
	    strcat( buf, " " );
	    strcat(buf, guild_table[ guild ].title );
	}

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}


/*
 * Return bit vector
 */
long affect_name_bit( char* buf )
{
    if (!str_cmp( buf, "blind"         )) return AFF_BLIND;
    if (!str_cmp( buf, "shadowform"     )) return AFF_SHADOWFORM;
    if (!str_cmp( buf, "detect_world"  )) return AFF_DETECT_WORLD;
    if (!str_cmp( buf, "detect_shadowform"  )) return AFF_DETECT_SHADOWFORM;
    if (!str_cmp( buf, "detect_magic"  )) return AFF_DETECT_MAGIC;
    if (!str_cmp( buf, "detect_hidden" )) return AFF_DETECT_HIDDEN;
    if (!str_cmp( buf, "tongues"       )) return AFF_TONGUES;
    if (!str_cmp( buf, "sanctuary"     )) return AFF_SANCTUARY;
    if (!str_cmp( buf, "faerie_fire"   )) return AFF_FAERIE_FIRE;
    if (!str_cmp( buf, "infrared"      )) return AFF_INFRARED;
    if (!str_cmp( buf, "curse"         )) return AFF_CURSE;
    if (!str_cmp( buf, "hold_breath"   )) return AFF_HOLD_BREATH;
    if (!str_cmp( buf, "poison"        )) return AFF_POISON;
    if (!str_cmp( buf, "breathe_underwater" )) return AFF_BREATHE_UNDERWATER;
    if (!str_cmp( buf, "mounted"       )) return AFF_MOUNTED;
    if (!str_cmp( buf, "sneak"         )) return AFF_SNEAK;
    if (!str_cmp( buf, "hide"          )) return AFF_HIDE;
    if (!str_cmp( buf, "sleep"         )) return AFF_SLEEP;
    if (!str_cmp( buf, "control_mind"  )) return AFF_CONTROL_MIND;
    if (!str_cmp( buf, "levitating"    )) return AFF_LEVITATING;
    if (!str_cmp( buf, "flying"        )) return AFF_FLYING;
    if (!str_cmp( buf, "dimension_walk")) return AFF_DIMENSION_WALK;
    if (!str_cmp( buf, "haste"         )) return AFF_HASTE;
    if (!str_cmp( buf, "calm_spirit"   )) return AFF_CALM_SPIRIT;
    if (!str_cmp( buf, "plague"        )) return AFF_PLAGUE;
    if (!str_cmp( buf, "dark_vision"   )) return AFF_DARK_VISION;
    if (!str_cmp( buf, "berserk"       )) return AFF_BERSERK;
    if (!str_cmp( buf, "swim"          )) return AFF_SWIM;
    if (!str_cmp( buf, "regeneration"  )) return AFF_REGENERATION;
    if (!str_cmp( buf, "cold"          )) return AFF_COLD;
    if (!str_cmp( buf, "new-mob"       )) return AFF_NEW_MOB;
    return 0;
}

long affect_name_bit_2( char* buf )
{
    if (!str_cmp( buf, "slow"         )) return AFF_SLOW;
    if (!str_cmp( buf, "fade"         )) return AFF_FADE;
    if (!str_cmp( buf, "stunned"         )) return AFF_STUN;
    if (!str_prefix( buf, "senses" )) return AFF_SENSES;
    if (!str_prefix( buf, "light_sensitive" )) return AFF_LIGHT_SENSITIVE;
    if (!str_prefix( buf, "falling" )) return AFF_FALLING;
    if (!str_prefix( buf, "mana_gen" )) return AFF_MANA_GEN;
    return 0;
}

/*
 * return bit based on skill number
 */

long spell_affect_bit( int sn )
{
    if ( sn == gsn_blinding_light       ) return AFF_BLIND;
    if ( sn == gsn_sand_storm       	) return AFF_BLIND;
    if ( sn == gsn_shadowform           ) return AFF_SHADOWFORM;
    if ( sn == gsn_mass_shadowform	) return AFF_SHADOWFORM;
    if ( sn == gsn_detect_shadowform	) return AFF_DETECT_SHADOWFORM;
    if ( sn == gsn_detect_hidden	) return AFF_DETECT_HIDDEN;
    if ( sn == gsn_circle_of_protection	) return AFF_SANCTUARY;
    if ( sn == gsn_faerie_fire		) return AFF_FAERIE_FIRE;
    if ( sn == gsn_infravision		) return AFF_INFRARED;
    if ( sn == gsn_curse		) return AFF_CURSE;
    if ( sn == gsn_poison		) return AFF_POISON;
    if ( sn == gsn_breathe_underwater	) return AFF_BREATHE_UNDERWATER;
    if ( sn == gsn_sneak		) return AFF_SNEAK;
    if ( sn == gsn_hide			) return AFF_HIDE;
    if ( sn == gsn_sleep		) return AFF_SLEEP;
    if ( sn == gsn_control_mind		) return AFF_CONTROL_MIND;
    if ( sn == gsn_levitate		) return AFF_LEVITATING;
    if ( sn == gsn_fly			) return AFF_FLYING;
    if ( sn == gsn_dimension_walk	) return AFF_DIMENSION_WALK;
    if ( sn == gsn_haste		) return AFF_HASTE;
    if ( sn == gsn_calm_spirit		) return AFF_CALM_SPIRIT;
    if ( sn == gsn_plague		) return AFF_PLAGUE;
    if ( sn == gsn_berserk		) return AFF_BERSERK;
    return 0;
}

long spell_affect_bit_2( int sn )
{
    if ( sn == gsn_slow 		) return AFF_SLOW;
    if ( sn == gsn_fade 		) return AFF_FADE;
return 0;
}

int affect_name_loc( char* name )
{
    if ( !str_cmp( name, "none"          ) ) return APPLY_NONE;
    if ( !str_cmp( name, "strength"      ) ) return APPLY_STR;
    if ( !str_cmp( name, "str" 	         ) ) return APPLY_STR;
    if ( !str_cmp( name, "dexterity"     ) ) return APPLY_DEX;
    if ( !str_cmp( name, "dex"		 ) ) return APPLY_DEX;
    if ( !str_cmp( name, "intelligence"  ) ) return APPLY_INT;
    if ( !str_cmp( name, "int"	         ) ) return APPLY_INT;
    if ( !str_cmp( name, "wisdom"        ) ) return APPLY_WIS;
    if ( !str_cmp( name, "wis"	         ) ) return APPLY_WIS;
    if ( !str_cmp( name, "constitution"  ) ) return APPLY_CON;
    if ( !str_cmp( name, "con"	         ) ) return APPLY_CON;
    if ( !str_cmp( name, "charisma"      ) ) return APPLY_CHR;
    if ( !str_cmp( name, "cha"	         ) ) return APPLY_CHR;
    if ( !str_cmp( name, "sex"           ) ) return APPLY_SEX;
    if ( !str_cmp( name, "level"         ) ) return APPLY_LEVEL;
    if ( !str_cmp( name, "age"           ) ) return APPLY_AGE;
    if ( !str_cmp( name, "height"        ) ) return APPLY_HEIGHT;
    if ( !str_cmp( name, "weight"        ) ) return APPLY_WEIGHT;
    if ( !str_cmp( name, "mana"          ) ) return APPLY_MANA;
    if ( !str_cmp( name, "fire"		 ) ) return APPLY_MANA_FIRE;
    if ( !str_cmp( name, "water"	 ) ) return APPLY_MANA_WATER;
    if ( !str_cmp( name, "earth"	 ) ) return APPLY_MANA_EARTH;
    if ( !str_cmp( name, "wind"		 ) ) return APPLY_MANA_WIND;
    if ( !str_cmp( name, "spirit"	 ) ) return APPLY_MANA_SPIRIT;
    if ( !str_cmp( name, "hp"            ) ) return APPLY_HIT;
    if ( !str_cmp( name, "move"          ) ) return APPLY_MOVE;
    if ( !str_cmp( name, "moves"          ) ) return APPLY_MOVE;
    if ( !str_cmp( name, "gold"          ) ) return APPLY_GOLD;
    if ( !str_cmp( name, "experience"    ) ) return APPLY_EXP;
    if ( !str_cmp( name, "ac"            ) ) return APPLY_AC;
    if ( !str_cmp( name, "hit_roll"      ) ) return APPLY_HITROLL;
    if ( !str_cmp( name, "dam"           ) ) return APPLY_DAMROLL;
    if ( !str_cmp( name, "damage_roll"   ) ) return APPLY_DAMROLL;
    if ( !str_cmp( name, "save_vs_spell"  ) ) return APPLY_SAVING_SPELL;
    if ( !str_cmp( name, "no_water"      ) ) return APPLY_NO_WATER;
    if ( !str_cmp( name, "no_food"       ) ) return APPLY_NO_FOOD;
    if ( !str_cmp( name, "apply_affect"  ) ) return APPLY_AFFECT;
    if ( !str_cmp( name, "dreaming"	 ) ) return APPLY_DREAMS;
    if ( !str_cmp( name, "nightmares"    ) ) return APPLY_NIGHTMARES;
    return APPLY_NONE;
}



int wear_name_loc( char *buf )
{
    if ( !str_cmp( buf, "light" ) )     return WEAR_LIGHT;
    if ( !str_cmp( buf, "lfinger" ) )   return WEAR_FINGER_L;
    if ( !str_cmp( buf, "rfinger" ) )   return WEAR_FINGER_R;
    if ( !str_cmp( buf, "lear"    ) )	return WEAR_EAR_L;
    if ( !str_cmp( buf, "rear"	) ) 	return WEAR_EAR_R;       
    if ( !str_cmp( buf, "neck1" ) )     return WEAR_NECK_1;
    if ( !str_cmp( buf, "neck2" ) )     return WEAR_NECK_2;
    if ( !str_cmp( buf, "body" ) )      return WEAR_BODY;
    if ( !str_cmp( buf, "head" ) )      return WEAR_HEAD;
    if ( !str_cmp( buf, "legs" ) )      return WEAR_LEGS;
    if ( !str_cmp( buf, "feet" ) )      return WEAR_FEET;
    if ( !str_cmp( buf, "hands" ) )     return WEAR_HANDS;
    if ( !str_cmp( buf, "arms" ) )      return WEAR_ARMS;
    if ( !str_cmp( buf, "shield" ) )    return WEAR_SHIELD;
    if ( !str_cmp( buf, "about" ) )     return WEAR_ABOUT;
    if ( !str_cmp( buf, "waist" ) )     return WEAR_WAIST;
    if ( !str_cmp( buf, "lwrist" ) )    return WEAR_WRIST_L;
    if ( !str_cmp( buf, "rwrist" ) )    return WEAR_WRIST_R;
    if ( !str_cmp( buf, "wield" ) )     return WEAR_WIELD_R;
    if ( !str_cmp( buf, "lwield" ) )    return WEAR_WIELD_R;
    if ( !str_cmp( buf, "rwield" ) )    return WEAR_WIELD_L;
    if ( !str_cmp( buf, "hold" ) )      return WEAR_HOLD;
    if ( !str_cmp( buf, "two_handed" ) ) return WEAR_WIELD_TWO_HANDED;
    return WEAR_NONE;
}



/*
 * Return ascii name of wear location.
 */
char *wear_loc_name( int wearloc )
{
    switch( wearloc )
    {
               default: return "unknown";
        case WEAR_NONE: return "in the inventory";
       case WEAR_LIGHT: return "as a light";
    case WEAR_FINGER_L: return "on the left finger";
    case WEAR_FINGER_R: return "on the right finger";
    case WEAR_EAR_L:	return "through the left ear lobe";
    case WEAR_EAR_R:	return "through the right ear lobe";
      case WEAR_NECK_1: return "around the neck (1)";
      case WEAR_NECK_2: return "around the neck (2)";
        case WEAR_BODY: return "on the body";
        case WEAR_HEAD: return "over the head";
        case WEAR_LEGS: return "on the legs";
        case WEAR_FEET: return "on the feet";
       case WEAR_HANDS: return "on the hands";
        case WEAR_ARMS: return "on the arms";
      case WEAR_SHIELD: return "as a shield";
       case WEAR_ABOUT: return "about the shoulders";
       case WEAR_WAIST: return "around the waist";
     case WEAR_WRIST_L: return "on the left wrist";
     case WEAR_WRIST_R: return "on the right wrist";
      case WEAR_WIELD_R: return "as the weapon";
      case WEAR_WIELD_L: return "as the weapon";
        case WEAR_HOLD: return "held in the hands";
case WEAR_WIELD_TWO_HANDED: return "as the two handed weapon";
    }
}

int weapon_name_type( char * arg )
{
    if ( !str_cmp( arg, "exotic"	) ) return 0;
    if ( !str_cmp( arg, "sword"		) ) return 1;
    if ( !str_cmp( arg, "dagger"	) ) return 2;
    if ( !str_cmp( arg, "spear"		) ) return 3;
    if ( !str_cmp( arg, "mace"		) ) return 4;
    if ( !str_cmp( arg, "axe"		) ) return 5;
    if ( !str_cmp( arg, "flail"		) ) return 6;
    if ( !str_cmp( arg, "whip"		) ) return 7;
    if ( !str_cmp( arg, "polearm"	) ) return 8;
    if ( !str_cmp( arg, "staff"		) ) return 9;
    return -1;
}

char * get_weapon_name( int arg )
{
    switch( arg )
    {
	default: return "";
	case 0: return "exotic";
	case 1: return "sword";
	case 2: return "dagger";
	case 3: return "spear";
	case 4: return "mace";
	case 5: return "axe";
	case 6: return "flail";
	case 7: return "whip";
	case 8: return "polearm";
	case 9: return "staff";
    }
}

int get_name_weapon( char *arg )
{
    if (!str_cmp( arg, "exotic"		) ) return 0;
    if (!str_cmp( arg, "sword"		) ) return 1;
    if (!str_cmp( arg, "dagger"		) ) return 2;
    if (!str_cmp( arg, "spear"		) ) return 3;
    if (!str_cmp( arg, "mace"		) ) return 4;
    if (!str_cmp( arg, "axe"		) ) return 5;
    if (!str_cmp( arg, "flail"		) ) return 6;
    if (!str_cmp( arg, "whip"		) ) return 7;
    if (!str_cmp( arg, "polearm"	) ) return 8;
    if (!str_cmp( arg, "staff"       	) ) return 9;
    return 0;
}

int get_weapon_type( char *arg )
{
    if (!str_cmp( arg, "hit"		) ) return 0;
    if (!str_cmp( arg, "slice"		) ) return 1;
    if (!str_cmp( arg, "stab"		) ) return 2;
    if (!str_cmp( arg, "slash"		) ) return 3;
    if (!str_cmp( arg, "whip"		) ) return 4;
    if (!str_cmp( arg, "claw"		) ) return 5;
    if (!str_cmp( arg, "blast"		) ) return 6;
    if (!str_cmp( arg, "pound"		) ) return 7;
    if (!str_cmp( arg, "crush"		) ) return 8;
    if (!str_cmp( arg, "grep"		) ) return 9;
    if (!str_cmp( arg, "bite"		) ) return 10;
    if (!str_cmp( arg, "pierce"		) ) return 11;
    if (!str_cmp( arg, "suction"	) ) return 12;
    if (!str_cmp( arg, "beating"	) ) return 13;
    if (!str_cmp( arg, "digestion"	) ) return 14;
    if (!str_cmp( arg, "charge" 	) ) return 15;
    if (!str_cmp( arg, "slap"   	) ) return 16;
    if (!str_cmp( arg, "punch"  	) ) return 17;
    if (!str_cmp( arg, "wrath"  	) ) return 18;
    if (!str_cmp( arg, "magic"  	) ) return 19;
    if (!str_cmp( arg, "divine_power"	) ) return 20;
    if (!str_cmp( arg, "cleave"		) ) return 21;
    if (!str_cmp( arg, "scratch"	) ) return 22;
    if (!str_cmp( arg, "peck"		) ) return 23;
    if (!str_cmp( arg, "poke"		) ) return 24;
    if (!str_cmp( arg, "chop"		) ) return 25;
    if (!str_cmp( arg, "sting"		) ) return 26;
    if (!str_cmp( arg, "smash"		) ) return 27;
    if (!str_cmp( arg, "shocking_bite"	) ) return 28;
    if (!str_cmp( arg, "flaming_bite"	) ) return 29;
    if (!str_cmp( arg, "freezing_bite"	) ) return 30;
    if (!str_cmp( arg, "acidic_bite"	) ) return 31;
    if (!str_cmp( arg, "chomps"		) ) return 32;
    if (!str_cmp( arg, "zaps"		) ) return 33;
    return 0;
}



/*
 * Use this in dam_message in fight.c if you like.
 */
char * get_type_weapon( int arg )
{
    switch ( arg )
    {
    case 0:  return "hit";
    case 1:  return "slice";
    case 2:  return "stab";
    case 3:  return "slash";
    case 4:  return "whip";
    case 5:  return "claw";
    case 6:  return "blast";
    case 7:  return "pound";
    case 8:  return "crush";
    case 9:  return "grep";
    case 10: return "bite";
    case 11: return "pierce";
    case 12: return "suction";
    case 13: return "beating";
    case 14: return "digestion";
    case 15: return "charge";
    case 16: return "slap";
    case 17: return "punch";
    case 18: return "wrath";
    case 19: return "magic";
    case 20: return "divine_power";
    case 21: return "cleave";
    case 22: return "scratch";
    case 23: return "peck";
    case 24: return "poke";
    case 25: return "chop";
    case 26: return "sting";
    case 27: return "smash";
    case 28: return "shocking_bite";
    case 29: return "flaming_bite";
    case 30: return "freezing_bite";
    case 31: return "acidic_bite";
    case 32: return "chomp";
    case 33: return "zaps";
    }

    bug( "Get_type_weapon: unknown type %d.", arg );
    return "(unknown)";
}

int get_container_flags( char *arg )
{
    if (!str_cmp( arg, "closeable"	) ) return 1;
    if (!str_cmp( arg, "closed"		) ) return 4;
    if (!str_cmp( arg, "holding"	) ) return CONT_HOLDING;
    if (!str_cmp( arg, "extra_holding"	) ) return CONT_EXTRA_HOLDING;
    if (!str_cmp( arg, "super_holding"	) ) return CONT_SUPER_HOLDING;
    if (!str_cmp( arg, "devouring"	) ) return CONT_DEVOURING;
    if (!str_cmp( arg, "water_proof"	) ) return CONT_WATER_PROOF;
    if (!str_cmp( arg, "waterproof"	) ) return CONT_WATER_PROOF;
    return 0;
}

int get_trap_flags( char * arg )
{
    if (!str_cmp( arg, "area"		) ) return TRAP_AREA;
    if (!str_cmp( arg, "random_area"	) ) return TRAP_RANDOM_AREA;
    if (!str_cmp( arg, "random_one"	) ) return TRAP_RANDOM_ONE;
    if (!str_cmp( arg, "sprung"		) ) return TRAP_SPRUNG;
    if (!str_cmp( arg, "unlock"		) ) return TRAP_UNLOCK;
    if (!str_cmp( arg, "pick"		) ) return TRAP_PICK;
    if (!str_cmp( arg, "open"		) ) return TRAP_OPEN;
    if (!str_cmp( arg, "enter"		) ) return TRAP_ENTER;
    if (!str_cmp( arg, "auto_reset"	) ) return TRAP_AUTO_RESET;
    return 0;
}

char * get_flags_trap( int flags )
{
    static char buf[512];

    buf[0]='\0';
    if ( flags & TRAP_AREA		) strcat( buf, " area" );
    if ( flags & TRAP_RANDOM_AREA	) strcat( buf, " random_area" );
    if ( flags & TRAP_RANDOM_ONE	) strcat( buf, " random_one" );
    if ( flags & TRAP_SPRUNG		) strcat( buf, " sprung" );
    if ( flags & TRAP_UNLOCK		) strcat( buf, " unlock" );
    if ( flags & TRAP_PICK		) strcat( buf, " pick" );
    if ( flags & TRAP_OPEN		) strcat( buf, " open" );
    if ( flags & TRAP_ENTER		) strcat( buf, " enter" );
    if ( flags & TRAP_AUTO_RESET	) strcat( buf, " auto_reset" );

    if ( buf[0] == '\0' )
	return "None";

    return buf + 1;
}

int get_lock_flags( char * arg )
{
    if (!str_cmp( arg, "locked"		) ) return LOCK_LOCKED;
    if (!str_cmp( arg, "dead_bolt"	) ) return LOCK_DEAD_BOLT;
    if (!str_cmp( arg, "pick_proof"	) ) return LOCK_PICKPROOF;
    if (!str_cmp( arg, "deadbolt"	) ) return LOCK_DEAD_BOLT;
    if (!str_cmp( arg, "pickproof"	) ) return LOCK_PICKPROOF;
    return 0;
}

char * get_flags_lock( int arg )
{
    static char buf[512];

    buf[0] = '\0';
    if ( arg & LOCK_LOCKED	) strcat( buf, " locked"	);
    if ( arg & LOCK_DEAD_BOLT	) strcat( buf, " dead_bolt"	);
    if ( arg & LOCK_PICKPROOF	) strcat( buf, " pick_proof"	);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char * get_flags_container( int arg )
{
    static char buf[512];

    buf[0] = '\0';
    if ( arg & 1 ) strcat( buf, " closeable"	);
    if ( arg & 4 ) strcat( buf, " closed"	);
    if ( arg & CONT_HOLDING 	  ) strcat( buf, " holding"	  );
    if ( arg & CONT_EXTRA_HOLDING ) strcat( buf, " extra_holding" );
    if ( arg & CONT_SUPER_HOLDING ) strcat( buf, " super_holding" );
    if ( arg & CONT_DEVOURING	  ) strcat( buf, " devouring"	  );
    if ( arg & CONT_WATER_PROOF	  ) strcat( buf, " water_proof"   );
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}



int get_liquid_type( char *arg )
{
    if (!str_cmp( arg, "water"			) ) return 0;
    if (!str_cmp( arg, "beer"			) ) return 1;
    if (!str_cmp( arg, "wine"			) ) return 2;
    if (!str_cmp( arg, "ale"			) ) return 3;
    if (!str_cmp( arg, "dark_ale"		) ) return 4;
    if (!str_cmp( arg, "whisky"			) ) return 5;
    if (!str_cmp( arg, "lemonade"		) ) return 6;
    if (!str_cmp( arg, "firebreather"		) ) return 7;
    if (!str_cmp( arg, "local_specialty"	) ) return 8;
    if (!str_cmp( arg, "slime_mold_juice"	) ) return 9;
    if (!str_cmp( arg, "milk"			) ) return 10;
    if (!str_cmp( arg, "tea"			) ) return 11;
    if (!str_cmp( arg, "coffee"			) ) return 12;
    if (!str_cmp( arg, "blood"			) ) return 13;
    if (!str_cmp( arg, "salt_water"		) ) return 14;
    if (!str_cmp( arg, "cola"			) ) return 15;
    return 0;
}



char * get_type_liquid( int arg )
{
    switch ( arg )
    {
    case 0:  return "water";
    case 1:  return "beer";
    case 2:  return "wine";
    case 3:  return "ale";
    case 4:  return "dark_ale";
    case 5:  return "whisky";
    case 6:  return "lemonade";
    case 7:  return "firebreather";
    case 8:  return "local_specialty";
    case 9:  return "slime_mold_juice";
    case 10: return "milk";
    case 11: return "tea";
    case 12: return "coffee";
    case 13: return "blood";
    case 14: return "sale_water";
    case 15: return "cola";
    }

    bug( "Get_type_liquid: unknown type %d.", arg );
    return "(unknown)";
}


/*
 * Return ascii name of an item type.
 */
char *item_type_name( int item_type )
{
    switch ( item_type )
    {
    case ITEM_LIGHT:		return "light";
    case ITEM_SCROLL:		return "scroll";
    case ITEM_WAND:		return "wand";
    case ITEM_STAFF:		return "staff";
    case ITEM_WEAPON:		return "weapon";
    case ITEM_TREASURE:		return "treasure";
    case ITEM_ARMOR:		return "armor";
    case ITEM_POTION:		return "potion";
    case ITEM_FURNITURE:	return "furniture";
    case ITEM_TRASH:		return "trash";
    case ITEM_CONTAINER:	return "container";
    case ITEM_DRINK_CON:	return "drink container";
    case ITEM_KEY:		return "key";
    case ITEM_FOOD:		return "food";
    case ITEM_MONEY:		return "money";
    case ITEM_BOAT:		return "boat";
    case ITEM_CORPSE_NPC:	return "npc corpse";
    case ITEM_CORPSE_PC:	return "pc corpse";
    case ITEM_FOUNTAIN:		return "fountain";
    case ITEM_PILL:		return "pill";
    case ITEM_MAP:		return "map";
    case ITEM_PC:		return "pc";
    case ITEM_GATE:		return "gate"; 
    case ITEM_NO_FOOD_REQ:	return "no_food";
    case ITEM_NO_WATER_REQ:	return "no_water";
    case ITEM_NO_FOOD_OR_WATER:	return "no_food_water";
    case ITEM_POISONED:		return "poisoned";
    case ITEM_TYPE_BARDING:	return "barding";
    case ITEM_FUEL:		return "fuel";
    case ITEM_LIGHTER:		return "lighter";
    }

    bug( "Item_type_name: unknown type %d.", item_type );
    return "(unknown)";
}

int get_shop_type( int item_type )
{
    switch ( item_type )
    {
    case ITEM_LIGHT:		return SHOP_GROCER;
    case ITEM_SCROLL:		return SHOP_MAGIC;
    case ITEM_WAND:		return SHOP_MAGIC;
    case ITEM_STAFF:		return SHOP_MAGIC;
    case ITEM_WEAPON:		return SHOP_WEAPON;
    case ITEM_TREASURE:		return SHOP_JEWELLER;
    case ITEM_ARMOR:		return SHOP_ARMOUR;
    case ITEM_POTION:		return SHOP_MAGIC;
    case ITEM_FURNITURE:	return SHOP_FURNITURE;
    case ITEM_CONTAINER:	return SHOP_GROCER;
    case ITEM_DRINK_CON:	return SHOP_BAR;
    case ITEM_FOOD:		return SHOP_GROCER;
    case ITEM_BOAT:		return SHOP_BOAT;
    case ITEM_FOUNTAIN:		return SHOP_BAR;
    case ITEM_PILL:		return SHOP_MAGIC;
    case ITEM_TYPE_BARDING:	return SHOP_BARDING;
    case ITEM_FUEL:		return SHOP_GROCER;
    case ITEM_LIGHTER:		return SHOP_GROCER;
    }
    return 0;
}



/*
 * Return ascii name of an affect location.
 */
char *affect_loc_name( AFFECT_DATA * af )
{
    switch ( af->location )
    {
    case APPLY_NONE:		return "none";
    case APPLY_STR:		return "strength";
    case APPLY_DEX:		return "dexterity";
    case APPLY_INT:		return "intelligence";
    case APPLY_WIS:		return "wisdom";
    case APPLY_CON:		return "constitution";
    case APPLY_CHR:		return "charisma";
    case APPLY_SEX:		return "sex";
    case APPLY_LEVEL:		return "level";
    case APPLY_AGE:		return "age";
    case APPLY_MANA:		return "mana";
    case APPLY_MANA_FIRE:	return "fire";
    case APPLY_MANA_WATER:	return "water";
    case APPLY_MANA_EARTH:	return "earth";
    case APPLY_MANA_WIND:	return "wind";
    case APPLY_MANA_SPIRIT:	return "spirit";
    case APPLY_HIT:		return "hp";
    case APPLY_MOVE:		return "moves";
    case APPLY_GOLD:		return "gold";
    case APPLY_EXP:		return "experience";
    case APPLY_AC:		return "armor_class";
    case APPLY_HITROLL:		return "hit_roll";
    case APPLY_DAMROLL:		return "damage_roll";
    case APPLY_SAVING_SPELL:	return "save_vs_spell";
    case APPLY_NO_WATER:	return "water_not_needed";
    case APPLY_NO_FOOD:		return "food_not_needed"; 
    case APPLY_AFFECT:		return bit_type_name(af->bit_type,af->bitvector); 
    case APPLY_DREAMS:		return "dreaming";
    case APPLY_NIGHTMARES:	return "nightmares";
 }

    bug( "Affect_location_name: unknown location %d.", af->location );
    return "(unknown)";
}
/*
 * Return ascii name of an int.
 */
char *affect_name( int vector )
{
    switch ( vector )
    {
    case APPLY_NONE:		return "none ";
    case APPLY_STR:		return "strength ";
    case APPLY_DEX:		return "dexterity ";
    case APPLY_INT:		return "intelligence ";
    case APPLY_WIS:		return "wisdom ";
    case APPLY_CON:		return "constitution ";
    case APPLY_CHR:		return "charisma ";
    case APPLY_SEX:		return "sex ";
    case APPLY_LEVEL:		return "level ";
    case APPLY_AGE:		return "age ";
    case APPLY_MANA:		return "mana ";
    case APPLY_MANA_FIRE:	return "fire ";
    case APPLY_MANA_WATER:	return "water ";
    case APPLY_MANA_EARTH:	return "earth ";
    case APPLY_MANA_WIND:	return "wind ";
    case APPLY_MANA_SPIRIT:	return "spirit ";
    case APPLY_HIT:		return "hp ";
    case APPLY_MOVE:		return "moves ";
    case APPLY_GOLD:		return "gold ";
    case APPLY_EXP:		return "experience ";
    case APPLY_AC:		return "armor_class ";
    case APPLY_HITROLL:		return "hit_roll ";
    case APPLY_DAMROLL:		return "damage_roll ";
    case APPLY_SAVING_SPELL:	return "save_vs_spell ";
    case APPLY_NO_WATER:	return "water_not_needed ";
    case APPLY_NO_FOOD:		return "food_not_needed "; 
    case APPLY_DREAMS:		return "dreaming ";
    case APPLY_NIGHTMARES:	return "nightmares ";
 }

    return "none ";
}



/*
 * Return ascii name of an affect bit vector.
 */
char *affect_bit_name( long vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_BLIND         ) strcat( buf, " blind"         );
    if ( vector & AFF_SHADOWFORM     ) strcat( buf, " shadowform"     );
    if ( vector & AFF_DETECT_WORLD  ) strcat( buf, " detect_world"   );
    if ( vector & AFF_DETECT_SHADOWFORM  ) strcat( buf, " detect_shadowform");
    if ( vector & AFF_DETECT_MAGIC  ) strcat( buf, " detect_magic"  );
    if ( vector & AFF_DETECT_HIDDEN ) strcat( buf, " detect_hidden" );
    if ( vector & AFF_TONGUES       ) strcat( buf, " tongues"       );
    if ( vector & AFF_SANCTUARY     ) strcat( buf, " sanctuary"     );
    if ( vector & AFF_FAERIE_FIRE   ) strcat( buf, " faerie_fire"   );
    if ( vector & AFF_INFRARED      ) strcat( buf, " infrared"      );
    if ( vector & AFF_CURSE         ) strcat( buf, " curse"         );
    if ( vector & AFF_HOLD_BREATH   ) strcat( buf, " hold_breath"   );
    if ( vector & AFF_POISON        ) strcat( buf, " poison"        );
    if ( vector & AFF_BREATHE_UNDERWATER) strcat( buf, " breathe_underwater"       );
    if ( vector & AFF_MOUNTED       ) strcat( buf, " mounted"       );
    if ( vector & AFF_SLEEP         ) strcat( buf, " sleep"         );
    if ( vector & AFF_SNEAK         ) strcat( buf, " sneak"         );
    if ( vector & AFF_HIDE          ) strcat( buf, " hide"          );
    if ( vector & AFF_CONTROL_MIND  ) strcat( buf, " control_mind"  );
    if ( vector & AFF_LEVITATING    ) strcat( buf, " levitating"    );
    if ( vector & AFF_FLYING        ) strcat( buf, " flying"        );
    if ( vector & AFF_DIMENSION_WALK) strcat( buf, " dimension_walk");
    if ( vector & AFF_BERSERK	    ) strcat( buf, " berserk"	    );
    if ( vector & AFF_CALM_SPIRIT   ) strcat( buf, " calm_spirit"   );
    if ( vector & AFF_SWIM	    ) strcat( buf, " swim"	    );
    if ( vector & AFF_HASTE	    ) strcat( buf, " haste"	    );
    if ( vector & AFF_PLAGUE	    ) strcat( buf, " plague" 	    );
    if ( vector & AFF_DARK_VISION   ) strcat( buf, " dark_vision"   );
    if ( vector & AFF_COLD          ) strcat( buf, " cold"      );
    if ( vector & AFF_REGENERATION  ) strcat( buf, " regeneration"  );
    if ( vector & AFF_NEW_MOB       ) strcat( buf, " new-mob"      );
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *affect_bit_name_2( long vector )
{
    static char buf[512];

    buf[0] = '\0';
    if ( vector & AFF_SLOW	    ) strcat( buf, " slow"	    );
    if ( vector & AFF_FADE	    ) strcat( buf, " fade"	    );
    if ( vector & AFF_STUN	    ) strcat( buf, " stunned"	    );
    if ( vector & AFF_SENSES	    ) strcat( buf, " senses" 	    );
    if ( vector & AFF_LIGHT_SENSITIVE	    ) strcat( buf, " light_sensitive" );
    if ( vector & AFF_FALLING	    ) strcat( buf, " falling" );
    if ( vector & AFF_MANA_GEN	    ) strcat( buf, " mana_gen" );
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("");
}

/*
 * Return ascii name of room flags vector.
 */
char *room_bit_name( int room_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( room_flags & ROOM_DARK		) strcat( buf, " dark"		);
    if ( room_flags & ROOM_NO_MOB	) strcat( buf, " no_mob"	);
    if ( room_flags & ROOM_INDOORS	) strcat( buf, " indoors"	);
    if ( room_flags & ROOM_MANAGEN	) strcat( buf, " mana_gen"	);
    if ( room_flags & ROOM_HPGEN	) strcat( buf, " hp_gen"	);
    if ( room_flags & ROOM_MVGEN	) strcat( buf, " mv_gen"	);
    if ( room_flags & ROOM_PRIVATE	) strcat( buf, " private"	);
    if ( room_flags & ROOM_SAFE		) strcat( buf, " safe"		);
    if ( room_flags & ROOM_SOLITARY	) strcat( buf, " solitary"	);
    if ( room_flags & ROOM_NO_RECALL	) strcat( buf, " no_recall"	);
    if ( room_flags & ROOM_IMP_ONLY	) strcat( buf, " imp_only"	);
    if ( room_flags & ROOM_GODS_ONLY	) strcat( buf, " gods_only"	);
    if ( room_flags & ROOM_PET_SHOP	) strcat( buf, " pet_shop"	);
    if ( room_flags & ROOM_MOUNT_SHOP	) strcat( buf, " mount_shop"	);
    if ( room_flags & ROOM_NEWBIES_ONLY	) strcat( buf, " newbies_only"	);
    if ( room_flags & ROOM_BOTTOMLESS	) strcat( buf, " bottomless"	);
    if ( room_flags & ROOM_MOVE_UP	) strcat( buf, " move_up"	);
    if ( room_flags & ROOM_MOVE_DOWN	) strcat( buf, " move_down"	);
    if ( room_flags & ROOM_MOVE_EAST	) strcat( buf, " move_east"	);
    if ( room_flags & ROOM_MOVE_WEST	) strcat( buf, " move_west"	);
    if ( room_flags & ROOM_MOVE_NORTH	) strcat( buf, " move_north"	);
    if ( room_flags & ROOM_MOVE_SOUTH	) strcat( buf, " move_south"	);
    if ( room_flags & ROOM_THIRST	) strcat( buf, " thirst"	);
    if ( room_flags & ROOM_SILENCE	) strcat( buf, " silence"	);
    if ( room_flags & ROOM_FORGE	) strcat( buf, " forge"	);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("");
}

/*
 * Return ascii name of room flags vector.
 */
char *room_bit_name_2( int room_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( room_flags & ROOM_SAVE		) strcat( buf, " save"	);
    if ( room_flags & ROOM_NOSAVE	) strcat( buf, " nosave");
    if ( room_flags & ROOM_STARTING	) strcat( buf, " starting");
    if ( room_flags & ROOM_DELETE	) strcat( buf, " delete");
    if ( room_flags & ROOM_LIT  	) strcat( buf, " lit");
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("");
}

int room_name_bit( char* buf )
{
    if ( !str_cmp( buf, "bottomless"	) ) return ROOM_BOTTOMLESS;
    if ( !str_cmp( buf, "move_up"	) ) return ROOM_MOVE_UP;
    if ( !str_cmp( buf, "move_down"	) ) return ROOM_MOVE_DOWN;
    if ( !str_cmp( buf, "move_east"	) ) return ROOM_MOVE_EAST;
    if ( !str_cmp( buf, "move_west"	) ) return ROOM_MOVE_WEST;
    if ( !str_cmp( buf, "move_north"	) ) return ROOM_MOVE_NORTH;
    if ( !str_cmp( buf, "move_south"	) ) return ROOM_MOVE_SOUTH;
    if ( !str_cmp( buf, "thirst"	) ) return ROOM_THIRST;
    if ( !str_cmp( buf, "imp_only"	) ) return ROOM_IMP_ONLY;
    if ( !str_cmp( buf, "gods_only"	) ) return ROOM_GODS_ONLY;
    if ( !str_cmp( buf, "newbies_only"	) ) return ROOM_NEWBIES_ONLY;
    if ( !str_cmp( buf, "dark"		) ) return ROOM_DARK;
    if ( !str_cmp( buf, "no_mob"	) ) return ROOM_NO_MOB;
    if ( !str_cmp( buf, "indoors"	) ) return ROOM_INDOORS;
    if ( !str_cmp( buf, "private"	) ) return ROOM_PRIVATE;
    if ( !str_cmp( buf, "safe"		) ) return ROOM_SAFE;
    if ( !str_cmp( buf, "solitary"	) ) return ROOM_SOLITARY;
    if ( !str_cmp( buf, "pet_shop"	) ) return ROOM_PET_SHOP;
    if ( !str_cmp( buf, "pet"		) ) return ROOM_PET_SHOP;
    if ( !str_cmp( buf, "mount_shop"	) ) return ROOM_MOUNT_SHOP;
    if ( !str_cmp( buf, "mount"		) ) return ROOM_MOUNT_SHOP;
    if ( !str_cmp( buf, "no_recall"	) ) return ROOM_NO_RECALL;
    if ( !str_cmp( buf, "mana_gen"	) ) return ROOM_MANAGEN;
    if ( !str_cmp( buf, "silence"	) ) return ROOM_SILENCE;
    if ( !str_cmp( buf, "forge"		) ) return ROOM_FORGE;
    if ( !str_cmp( buf, "mana"		) ) return ROOM_MANAGEN;
    if ( !str_cmp( buf, "hp"		) ) return ROOM_HPGEN;
    if ( !str_cmp( buf, "hp_gen"	) ) return ROOM_HPGEN;
    if ( !str_cmp( buf, "mv_gen"	) ) return ROOM_MVGEN;
    if ( !str_cmp( buf, "mv"		) ) return ROOM_MVGEN;
    return 0;
}

int room_name_bit_2( char* buf )
{
    if ( !str_cmp( buf, "save"         	) ) return ROOM_SAVE;
    if ( !str_cmp( buf, "nosave"        ) ) return ROOM_NOSAVE;
    if ( !str_cmp( buf, "starting"     	) ) return ROOM_STARTING;
    if ( !str_cmp( buf, "delete"     	) ) return ROOM_DELETE;
    if ( !str_cmp( buf, "lit"     	) ) return ROOM_LIT;
    return 0;
}

/*
 * Return ascii name of room spell vector. Taken out by Rev.
 */

/*
 * Return ascii name of sector type vector.
 */
char *sector_bit_name( int sector_type )
{
    static char buf[512];

    buf[0] = '\0';
    if (( sector_type == SECT_NONE)		 ) strcat( buf, " none"		);
    if (( sector_type == SECT_CITY)		 ) strcat( buf, " city"		);
    if (( sector_type == SECT_FIELD)		 ) strcat( buf, " field"	);
    if (( sector_type == SECT_FOREST)		 ) strcat( buf, " forest"	);
    if (( sector_type == SECT_HILLS)		 ) strcat( buf, " hills"	);
    if (( sector_type == SECT_MOUNTAIN)		 ) strcat( buf, " mountain"	);
    if (( sector_type == SECT_WATER_SWIM)	 ) strcat( buf, " swim"		);
    if (( sector_type == SECT_WATER_NOSWIM)	 ) strcat( buf, " noswim "	);
    if (( sector_type == SECT_UNDERWATER)	 ) strcat( buf, " underwater"	);
    if (( sector_type == SECT_AIR)		 ) strcat( buf, " air"		);	
    if (( sector_type == SECT_DESERT)		 ) strcat( buf, " desert"	);
    if (( sector_type == SECT_LAVA)		 ) strcat( buf, " lava"		);
    if (( sector_type == SECT_PROTOTYPE)	 ) strcat( buf, " prototype"	);
    if (( sector_type == SECT_DEATH)		 ) strcat( buf, " death"	);
    if (( sector_type == SECT_SWAMP)		 ) strcat( buf, " swamp"	);
    if (( sector_type == SECT_DOCK)		 ) strcat( buf, " dock"		);
    if (( sector_type == SECT_UNDERGROUND)	 ) strcat( buf, " underground"	);
    if (( sector_type == SECT_SNOW)		 ) strcat( buf, " snow"	);
    if (( sector_type == SECT_DIRT)		 ) strcat( buf, " dirt"	);
    if (( sector_type == SECT_GRASS)		 ) strcat( buf, " grass"	);
    if (( sector_type == SECT_SAND)		 ) strcat( buf, " sand"	);
    if (( sector_type == SECT_ICE)		 ) strcat( buf, " ice"	);
    if (( sector_type == SECT_STREAM)		 ) strcat( buf, " stream"	);
    if (( sector_type == SECT_OCEAN_BOTTOM)	 ) strcat( buf, " ocean bottom"	);
    if (( sector_type == SECT_PC)		 ) strcat( buf, " pc"		);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}


int sector_number( char *argument )
{
    if ( !str_cmp( argument, "lava" ) )         return SECT_LAVA;
    if ( !str_cmp( argument, "none" ) )         return SECT_NONE;
    if ( !str_cmp( argument, "city" ) )         return SECT_CITY;
    if ( !str_cmp( argument, "field" ) )        return SECT_FIELD;
    if ( !str_cmp( argument, "forest" ) )       return SECT_FOREST;
    if ( !str_cmp( argument, "hills" ) )        return SECT_HILLS;
    if ( !str_cmp( argument, "mountain" ) )     return SECT_MOUNTAIN;
    if ( !str_cmp( argument, "swim" ) )         return SECT_WATER_SWIM;
    if ( !str_cmp( argument, "noswim" ) )       return SECT_WATER_NOSWIM;
    if ( !str_cmp( argument, "underwater" ) )   return SECT_UNDERWATER;
    if ( !str_cmp( argument, "air" ) )          return SECT_AIR;
    if ( !str_cmp( argument, "desert" ) )       return SECT_DESERT;
    if ( !str_cmp( argument, "prototype" ) )    return SECT_PROTOTYPE;
    if ( !str_cmp( argument, "death" ) )        return SECT_DEATH;
    if ( !str_cmp( argument, "swamp" ) )        return SECT_SWAMP;
    if ( !str_cmp( argument, "dock" ) )         return SECT_DOCK;
    if ( !str_cmp( argument, "underground" ) )  return SECT_UNDERGROUND;
    if ( !str_cmp( argument, "snow" ) ) 	return SECT_SNOW;
    if ( !str_cmp( argument, "dirt" ) ) 	return SECT_DIRT;
    if ( !str_cmp( argument, "grass" ) ) 	return SECT_GRASS;
    if ( !str_cmp( argument, "sand" ) ) 	return SECT_SAND;
    if ( !str_cmp( argument, "ice" ) ) 		return SECT_ICE;
    if ( !str_cmp( argument, "stream" ) ) 	return SECT_STREAM;
    if ( !str_cmp( argument, "bottom" ) ) 	return SECT_OCEAN_BOTTOM;
    if ( !str_cmp( argument, "pc" ) )           return SECT_PC;
    return SECT_MAX;
}
char *special_bit_name( long special_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( special_flags & ITEM_SPEC_STRING         ) strcat( buf, " stringshare"         );
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

/*
 * Return ascii name of extra flags vector.
 */
char *extra_bit_name( long extra_flags )
{
    static char buf[512];

    buf[0] = '\0';
    if ( extra_flags & ITEM_GLOW         ) strcat( buf, " glow"         );
    if ( extra_flags & ITEM_HUM          ) strcat( buf, " hum"          );
    if ( extra_flags & ITEM_NO_SHOW      ) strcat( buf, " no_show"      );
    if ( extra_flags & ITEM_BUCKLER      ) strcat( buf, " buckler"      );
    if ( extra_flags & ITEM_HAMMER	 ) strcat( buf, " hammer"	);
    if ( extra_flags & ITEM_SHADOWFORM   ) strcat( buf, " shadowform"   );
    if ( extra_flags & ITEM_MAGIC        ) strcat( buf, " magic"        );
    if ( extra_flags & ITEM_NODROP       ) strcat( buf, " nodrop"       );
    if ( extra_flags & ITEM_NO_SAVE      ) strcat( buf, " no_save"      );
    if ( extra_flags & ITEM_NO_LOCATE    ) strcat( buf, " no_locate"    );
    if ( extra_flags & ITEM_NO_IDENTIFY  ) strcat( buf, " no_identify"  );
    if ( extra_flags & ITEM_ASSAYED    	 ) strcat( buf, " assayed"	);
    if ( extra_flags & ITEM_BURIED	 ) strcat( buf, " buried"  );
    if ( extra_flags & ITEM_NOREMOVE     ) strcat( buf, " noremove"     ); 
    if ( extra_flags & ITEM_STAY_ON_DEATH) strcat( buf, " stay_on_death"     );
    if ( extra_flags & ITEM_INVENTORY    ) strcat( buf, " inventory"    );
    if ( extra_flags & ITEM_NOPURGE	 ) strcat( buf, " nopurge"	);
    if ( extra_flags & ITEM_VIS_DEATH	 ) strcat( buf, " vis_death"	);
    if ( extra_flags & ITEM_ROT_DEATH	 ) strcat( buf, " rot_death"	);
    if ( extra_flags & ITEM_ANTI_LITHDOR ) strcat( buf, " anti_lithdor"	);
    if ( extra_flags & ITEM_ANTI_NENKEMEN ) strcat( buf, " anti_nenkemen"	);
    if ( extra_flags & ITEM_ANTI_MAEGMENEL ) strcat( buf, " anti_maegmenel"	);
    if ( extra_flags & ITEM_FLOAT	 ) strcat( buf, " float"	);
    if ( extra_flags & ITEM_BARDING	 ) strcat( buf, " barding"	);
    if ( extra_flags & ITEM_MOB_INSIDE	 ) strcat( buf, " mob_inside"	);
    if ( extra_flags & ITEM_HOME	 ) strcat( buf, " home"		);
    if ( extra_flags & ITEM_NEWBIE	 ) strcat( buf, " newbie"	);
    if ( extra_flags & ITEM_PICK	 ) strcat( buf, " pick"	);
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

/* return ascii name of an act vector */
char *act_bit_name( long act_flags )
{
    static char buf[512];

    buf[0] = '\0';

    if (IS_SET(act_flags,ACT_IS_NPC))
    { 
 	strcat(buf," npc");
    	if (act_flags & ACT_SENTINEL 	) strcat(buf, " sentinel");
    	if (act_flags & ACT_SCAVENGER	) strcat(buf, " scavenger");
	if (act_flags & ACT_AGGRESSIVE	) strcat(buf, " aggressive");
	if (act_flags & ACT_STAY_AREA	) strcat(buf, " stay_area");
	if (act_flags & ACT_WIMPY	) strcat(buf, " wimpy");
	if (act_flags & ACT_PET		) strcat(buf, " pet");
	if (act_flags & ACT_TRAIN	) strcat(buf, " train(don't use)");
	if (act_flags & ACT_HUNTER	) strcat(buf, " hunter");
	if (act_flags & ACT_OUT_DOORS	) strcat(buf, " out_doors");
	if (act_flags & ACT_IN_DOORS	) strcat(buf, " in_doors");
	if (act_flags & ACT_GAIN	) strcat(buf, " gain(don't use)");
	if (act_flags & ACT_ATTACK_WORLD) strcat(buf, " attack_world");
	if (act_flags & ACT_REPAIR	) strcat(buf, " repair(don't use)");
	if (act_flags & ACT_NOPURGE	) strcat(buf, " no_purge");
	if (act_flags & ACT_IS_HEALER	) strcat(buf, " healer");
	if (act_flags & ACT_GAMBLE	) strcat(buf, " gamble");
	if (act_flags & ACT_MOUNT	) strcat(buf, " mount");
	if (act_flags & ACT_UPDATE_ALWAYS) strcat(buf," update_always");
    }
    else
    {
	strcat(buf," player");
	if (act_flags & PLR_BOUGHT_PET	) strcat(buf, " owner");
	if (act_flags & PLR_AUTOASSIST	) strcat(buf, " autoassist");
	if (act_flags & PLR_AUTODIRS 	) strcat(buf, " autodirs");
	if (act_flags & PLR_AUTOEXIT	) strcat(buf, " autoexit");
	if (act_flags & PLR_AUTOLOOT	) strcat(buf, " autoloot");
	if (act_flags & PLR_AUTOSAC	) strcat(buf, " autosac");
	if (act_flags & PLR_AUTOGOLD	) strcat(buf, " autogold");
	if (act_flags & PLR_AUTOSPLIT	) strcat(buf, " autosplit");
	if (act_flags & PLR_AUTOPROMPT	) strcat(buf, " autoprompt");
	if (act_flags & PLR_HOLYLIGHT	) strcat(buf, " holy_light");
	if (act_flags & PLR_WIZINVIS	) strcat(buf, " wizinvis");
	if (act_flags & PLR_CANLOOT	) strcat(buf, " loot_corpse");
	if (act_flags & PLR_NOSUMMON	) strcat(buf, " no_summon");
	if (act_flags & PLR_NOFOLLOW	) strcat(buf, " no_follow");
	if (act_flags & PLR_FREEZE	) strcat(buf, " frozen");
	if (act_flags & PLR_THIEF	) strcat(buf, " thief");
        if (act_flags & PLR_LOG		) strcat(buf, " log"); 
        if (act_flags & PLR_FREEZE	) strcat(buf, " freeze"); 
        if (act_flags & PLR_DENY	) strcat(buf, " deny"); 
        if (act_flags & PLR_ROOMSTATS	) strcat(buf, " roomstats"); 
        if (act_flags & PLR_SECTORSTATS	) strcat(buf, " sectorstats"); 
        if (act_flags & PLR_FLEE	) strcat(buf, " flee"); 
        if (act_flags & PLR_WIZPASS	) strcat(buf, " wizpass"); 
        if (act_flags & PLR_PK		) strcat(buf, " PK"); 
    }
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}


/* return ascii name of an act2 vector */
char *act2_bit_name( long act2_flags )
{
    static char buf[512];

    buf[0] = '\0';

    	if (act2_flags & ACT2_MOB_NO_SEE	) strcat(buf, " mob_no_see");
    	if (act2_flags & ACT2_MOB_NO_RANDOM	) strcat(buf, " mob_no_random");
    	if (act2_flags & ACT2_IGNORE		) strcat(buf, " ignore");
	if (act2_flags & ACT2_HALT		) strcat(buf, " halt");
	if (act2_flags & ACT2_TRACKSCR		) strcat(buf, " trackscr");
	if (act2_flags & ACT2_MOVEABLE_AREA	) strcat(buf, " moveable_area");
	if (act2_flags & ACT2_SHIP		) strcat(buf, " ship");
	if (act2_flags & ACT2_OPEN_LAND		) strcat(buf, " open_land");
	if (act2_flags & ACT2_SEE_HIDDEN	) strcat(buf, " see_hidden");
	if (act2_flags & ACT2_SEE_SECRET	) strcat(buf, " see_secret");
	if (act2_flags & ACT2_STAY_GUILD	) strcat(buf, " stay_guild");
	if (act2_flags & ACT2_ATTACK_GUILD	) strcat(buf, " attack_guild");
	if (act2_flags & ACT2_ENTER_PORTAL	) strcat(buf, " enter_portal");
	if (act2_flags & ACT2_DREAMSTATE	) strcat(buf, " dreamstate");
	if (act2_flags & ACT2_SMART_AGGRESSIVE	) strcat(buf, " smart_aggressive");
	if (act2_flags & ACT2_GUIDE		) strcat(buf, " guide");

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *comm_bit_name(int comm_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (comm_flags & COMM_QUIET		) strcat(buf, " quiet");
    if (comm_flags & COMM_DEAF		) strcat(buf, " deaf");
    if (comm_flags & COMM_NOWIZ		) strcat(buf, " no_wiz");
    if (comm_flags & COMM_NOAUCTION	) strcat(buf, " no_auction");
    if (comm_flags & COMM_NOTHINK	) strcat(buf, " no_think");
    if (comm_flags & COMM_NOOOC		) strcat(buf, " no_ooc");
    if (comm_flags & COMM_NOPRAY	) strcat(buf, " no_pray");
    if (comm_flags & COMM_INFO		) strcat(buf, " info");
    if (comm_flags & COMM_COMPACT	) strcat(buf, " compact");
    if (comm_flags & COMM_BRIEF		) strcat(buf, " brief");
    if (comm_flags & COMM_PROMPT	) strcat(buf, " prompt");
    if (comm_flags & COMM_COMBINE	) strcat(buf, " combine");
    if (comm_flags & COMM_NOEMOTE	) strcat(buf, " no_emote");
    if (comm_flags & COMM_NOSHOUT	) strcat(buf, " no_shout");
    if (comm_flags & COMM_NOTELL	) strcat(buf, " no_tell");
    if (comm_flags & COMM_NOCHANNELS	) strcat(buf, " no_channels");

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}


char *res_bit_name(long res_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (res_flags & RES_CONTROL_MIND		) strcat(buf, " control_mind");
    if (res_flags & RES_MAGIC			) strcat(buf, " magic");
    if (res_flags & RES_WEAPON			) strcat(buf, " weapon");
    if (res_flags & RES_BASH			) strcat(buf, " blunt");
    if (res_flags & RES_PIERCE			) strcat(buf, " pierce");
    if (res_flags & RES_SLASH			) strcat(buf, " slash");
    if (res_flags & RES_FIRE			) strcat(buf, " fire");
    if (res_flags & RES_LIGHTNING		) strcat(buf, " lightning");
    if (res_flags & RES_ACID			) strcat(buf, " acid");
    if (res_flags & RES_POISON			) strcat(buf, " poison");
    if (res_flags & RES_ENERGY			) strcat(buf, " energy");
    if (res_flags & RES_MENTAL			) strcat(buf, " mental");
    if (res_flags & RES_DISEASE			) strcat(buf, " disease");
    if (res_flags & RES_DROWNING		) strcat(buf, " drowning");
    if (res_flags & RES_LIGHT			) strcat(buf, " light");
    if (res_flags & RES_WIND			) strcat(buf, " wind");
    if (res_flags & RES_EARTH			) strcat(buf, " earth");
    if (res_flags & RES_SPIRIT			) strcat(buf, " spirit");
    if (res_flags & RES_WATER			) strcat(buf, " water");

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *vuln_bit_name(long vuln_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (vuln_flags & VULN_MAGIC			) strcat(buf, " magic");
    if (vuln_flags & VULN_WEAPON		) strcat(buf, " weapon");
    if (vuln_flags & VULN_BASH			) strcat(buf, " blunt");
    if (vuln_flags & VULN_PIERCE		) strcat(buf, " pierce");
    if (vuln_flags & VULN_SLASH			) strcat(buf, " slash");
    if (vuln_flags & VULN_FIRE			) strcat(buf, " fire");
    if (vuln_flags & VULN_LIGHTNING		) strcat(buf, " lightning");
    if (vuln_flags & VULN_ACID			) strcat(buf, " acid");
    if (vuln_flags & VULN_POISON		) strcat(buf, " poison");
    if (vuln_flags & VULN_ENERGY		) strcat(buf, " energy");
    if (vuln_flags & VULN_MENTAL		) strcat(buf, " mental");
    if (vuln_flags & VULN_DISEASE		) strcat(buf, " disease");
    if (vuln_flags & VULN_DROWNING		) strcat(buf, " drowning");
    if (vuln_flags & VULN_LIGHT			) strcat(buf, " light");
    if (vuln_flags & VULN_WIND			) strcat(buf, " wind");
    if (vuln_flags & VULN_EARTH			) strcat(buf, " earth");
    if (vuln_flags & VULN_SPIRIT			) strcat(buf, " spirit");
    if (vuln_flags & VULN_WATER			) strcat(buf, " water");

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *imm_bit_name(int imm_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (imm_flags & IMM_SUMMON		) strcat(buf, " summon");
    if (imm_flags & IMM_CONTROL_MIND	) strcat(buf, " control_mind");
    if (imm_flags & IMM_MAGIC		) strcat(buf, " magic");
    if (imm_flags & IMM_WEAPON		) strcat(buf, " weapon");
    if (imm_flags & IMM_BASH		) strcat(buf, " blunt");
    if (imm_flags & IMM_PIERCE		) strcat(buf, " pierce");
    if (imm_flags & IMM_SLASH		) strcat(buf, " slash");
    if (imm_flags & IMM_FIRE		) strcat(buf, " fire");
    if (imm_flags & IMM_LIGHTNING	) strcat(buf, " lightning");
    if (imm_flags & IMM_ACID		) strcat(buf, " acid");
    if (imm_flags & IMM_POISON		) strcat(buf, " poison");
    if (imm_flags & IMM_ENERGY		) strcat(buf, " energy");
    if (imm_flags & IMM_MENTAL		) strcat(buf, " mental");
    if (imm_flags & IMM_DISEASE	) strcat(buf, " disease");
    if (imm_flags & IMM_DROWNING	) strcat(buf, " drowning");
    if (imm_flags & IMM_LIGHT		) strcat(buf, " light");
    if (imm_flags & IMM_WIND			) strcat(buf, " wind");
    if (imm_flags & IMM_EARTH			) strcat(buf, " earth");
    if (imm_flags & IMM_SPIRIT			) strcat(buf, " spirit");
    if (imm_flags & IMM_WATER			) strcat(buf, " water");

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *wear_bit_name(int wear_flags)
{
    static char buf[512];

    buf [0] = '\0';
    if (wear_flags & ITEM_TAKE		) strcat(buf, " take");
    if (wear_flags & ITEM_WEAR_FINGER	) strcat(buf, " finger");
    if (wear_flags & ITEM_WEAR_EAR  	) strcat(buf, " ear");
    if (wear_flags & ITEM_WEAR_NECK	) strcat(buf, " neck");
    if (wear_flags & ITEM_WEAR_BODY	) strcat(buf, " torso");
    if (wear_flags & ITEM_WEAR_HEAD	) strcat(buf, " head");
    if (wear_flags & ITEM_WEAR_LEGS	) strcat(buf, " legs");
    if (wear_flags & ITEM_WEAR_FEET	) strcat(buf, " feet");
    if (wear_flags & ITEM_WEAR_HANDS	) strcat(buf, " hands");
    if (wear_flags & ITEM_WEAR_ARMS	) strcat(buf, " arms");
    if (wear_flags & ITEM_WEAR_SHIELD	) strcat(buf, " shield");
    if (wear_flags & ITEM_WEAR_ABOUT	) strcat(buf, " body");
    if (wear_flags & ITEM_WEAR_WAIST	) strcat(buf, " waist");
    if (wear_flags & ITEM_WEAR_WRIST	) strcat(buf, " wrist");
    if (wear_flags & ITEM_WIELD		) strcat(buf, " wield");
    if (wear_flags & ITEM_HOLD		) strcat(buf, " hold");
    if (wear_flags & ITEM_TWO_HANDS	) strcat(buf, " two_hands");
    if (wear_flags & ITEM_MONEY_0	 ) strcat( buf, " crowns"     );
    if (wear_flags & ITEM_MONEY_1	 ) strcat( buf, " duckets"    );
    if (wear_flags & ITEM_MONEY_2	 ) strcat( buf, " wheels"     );
    if (wear_flags & ITEM_MONEY_3	 ) strcat( buf, " greckles"   );
    if (wear_flags & ITEM_WEAR_LIGHT	 ) strcat( buf, " light"   );
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

char *form_bit_name(int form_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (form_flags & FORM_INSTANT_DECAY	) strcat(buf, " instant_rot");
    if (form_flags & FORM_ANIMAL	) strcat(buf, " animal");
    if (form_flags & FORM_SENTIENT	) strcat(buf, " sentient");

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

int form_name_bit( char * name )
{
    if ( !str_cmp( name, "instant_rot" ) ) return FORM_INSTANT_DECAY;
    if ( !str_cmp( name, "animal" ) ) return FORM_ANIMAL;
    if ( !str_cmp( name, "sentient" ) ) return FORM_SENTIENT;
    return 0;
}

char *weapon_bit_name(int weapon_flags)
{
    static char buf[512];

    buf[0] = '\0';
    if (weapon_flags & WEAPON_FLAMING	) strcat(buf, " flaming");
    if (weapon_flags & WEAPON_FROST	) strcat(buf, " frost");
    if (weapon_flags & WEAPON_VAMPIRIC	) strcat(buf, " vampiric");
    if (weapon_flags & WEAPON_POISON	) strcat(buf, " poisoned");
    if (weapon_flags & WEAPON_SHARP	) strcat(buf, " sharp");
    if (weapon_flags & WEAPON_VORPAL	) strcat(buf, " vorpal");
    if (weapon_flags & WEAPON_TWO_HANDS ) strcat(buf, " two_handed");
    if (weapon_flags & WEAPON_BASTARD   ) strcat(buf, " bastard");
    if (weapon_flags & WEAPON_KNIFE	) strcat(buf, " knife");

    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

int weapon_name_bit(char *arg)
{
    if ( !str_cmp( arg, "flaming"  	  ) ) return WEAPON_FLAMING;
    if ( !str_cmp( arg, "frost"    	  ) ) return WEAPON_FROST;
    if ( !str_cmp( arg, "vampiric" 	  ) ) return WEAPON_VAMPIRIC;
    if ( !str_cmp( arg, "poisoned" 	  ) ) return WEAPON_POISON;
    if ( !str_cmp( arg, "sharp"    	  ) ) return WEAPON_SHARP;
    if ( !str_cmp( arg, "vorpal"   	  ) ) return WEAPON_VORPAL;
    if ( !str_cmp( arg, "two_handed" 	  ) ) return WEAPON_TWO_HANDS;
    if ( !str_cmp( arg, "bastard"  	  ) ) return WEAPON_BASTARD;
    if ( !str_cmp( arg, "knife"	  	  ) ) return WEAPON_KNIFE;
    return 0;
}

char *off_bit_name(int off_flags)
{
    static char buf[512];

    buf[0] = '\0';

    if (off_flags & OFF_AREA_ATTACK	) strcat(buf, " area_attack");
    if (off_flags & OFF_BREATHFIRE	) strcat(buf, " breathfire");
    if (off_flags & OFF_BASH		) strcat(buf, " bash");
    if (off_flags & OFF_SECOND		) strcat(buf, " second_attack");
    if (off_flags & OFF_THIRD		) strcat(buf, " third_attack");
    if (off_flags & OFF_BERSERK		) strcat(buf, " berserk");
    if (off_flags & OFF_DISARM		) strcat(buf, " disarm");
    if (off_flags & OFF_FAST		) strcat(buf, " fast");
    if (off_flags & OFF_RESCUE		) strcat(buf, " rescue");
    if (off_flags & OFF_TRIP		) strcat(buf, " trip");
    if (off_flags & ASSIST_ALL		) strcat(buf, " assist_all");
    if (off_flags & ASSIST_RACE		) strcat(buf, " assist_race");
    if (off_flags & ASSIST_PLAYERS	) strcat(buf, " assist_players");
    if (off_flags & ASSIST_VNUM		) strcat(buf, " assist_vnum");
    return ( buf[0] != '\0' ) ? buf+1 : str_dup("none");
}

long off_name_bit( char* buf )
{
    if ( !str_cmp( buf, "breathfire"    ) ) return OFF_BREATHFIRE;
    if ( !str_cmp( buf, "bash"    ) ) return OFF_BASH;
    if ( !str_cmp( buf, "second_attack"    ) ) return OFF_SECOND;
    if ( !str_cmp( buf, "third_attack"    ) ) return OFF_THIRD;
    if ( !str_cmp( buf, "berserk"   ) ) return OFF_BERSERK;
    if ( !str_cmp( buf, "disarm"    ) ) return OFF_DISARM;
    if ( !str_cmp( buf, "fast"      ) ) return OFF_FAST;
    if ( !str_cmp( buf, "trip"        ) ) return OFF_TRIP;
    if ( !str_cmp( buf, "assist_all"         ) ) return ASSIST_ALL;
    if ( !str_cmp( buf, "assist_race"         ) ) return ASSIST_RACE;
    if ( !str_cmp( buf, "assist_players"         ) ) return ASSIST_PLAYERS;
    if ( !str_cmp( buf, "assist_vnum"       ) ) return ASSIST_VNUM;
    if ( !str_cmp( buf, "area_attack"      ) ) return OFF_AREA_ATTACK;
    return 0;
}


long act_name_bit( char* buf )
{
    if ( !str_cmp( buf, "npc"		) ) return ACT_IS_NPC;
    if ( !str_cmp( buf, "sentinel"     ) ) return ACT_SENTINEL;
    if ( !str_cmp( buf, "scavenger"    ) ) return ACT_SCAVENGER;
    if ( !str_cmp( buf, "aggressive"   ) ) return ACT_AGGRESSIVE;
    if ( !str_cmp( buf, "stay_area"    ) ) return ACT_STAY_AREA;
    if ( !str_cmp( buf, "wimpy"        ) ) return ACT_WIMPY;
    if ( !str_cmp( buf, "pet"          ) ) return ACT_PET;
    if ( !str_cmp( buf, "train"        ) ) return ACT_TRAIN;
    if ( !str_cmp( buf, "trainer"      ) ) return ACT_TRAIN;
    if ( !str_cmp( buf, "gambler"      ) ) return ACT_GAMBLE;
    if ( !str_cmp( buf, "hunter"       ) ) return ACT_HUNTER;
    if ( !str_cmp( buf, "nopurge"      ) ) return ACT_NOPURGE;
    if ( !str_cmp( buf, "out_doors"    ) ) return ACT_OUT_DOORS;
    if ( !str_cmp( buf, "in_doors"     ) ) return ACT_IN_DOORS;
    if ( !str_cmp( buf, "healer"       ) ) return ACT_IS_HEALER;
    if ( !str_cmp( buf, "gainer"       ) ) return ACT_GAIN;
    if ( !str_cmp( buf, "gain"         ) ) return ACT_GAIN;
    if ( !str_cmp( buf, "update_always") ) return ACT_UPDATE_ALWAYS;
    if ( !str_cmp( buf, "repair"       ) ) return ACT_REPAIR;
    if ( !str_cmp( buf, "mount"        ) ) return ACT_MOUNT;
    if ( !str_cmp( buf, "attack_world" ) ) return ACT_ATTACK_WORLD;
    return 0;
}


long act2_name_bit( char* buf )
{
    if ( !str_cmp( buf, "mob_no_see"	) ) return ACT2_MOB_NO_SEE;
    if ( !str_cmp( buf, "mob_no_random"	) ) return ACT2_MOB_NO_RANDOM;
    if ( !str_cmp( buf, "ignore"	) ) return ACT2_IGNORE;
    if ( !str_cmp( buf, "halt"		) ) return ACT2_HALT;
    if ( !str_cmp( buf, "trackscr"	) ) return ACT2_TRACKSCR;
    if ( !str_cmp( buf, "moveable_area"	) ) return ACT2_MOVEABLE_AREA;
    if ( !str_cmp( buf, "ship"		) ) return ACT2_SHIP;
    if ( !str_cmp( buf, "open_land"	) ) return ACT2_OPEN_LAND;
    if ( !str_cmp( buf, "see_hidden"	) ) return ACT2_SEE_HIDDEN;
    if ( !str_cmp( buf, "see_secret"	) ) return ACT2_SEE_SECRET;
    if ( !str_cmp( buf, "stay_guild"	) ) return ACT2_STAY_GUILD;
    if ( !str_cmp( buf, "attack_guild"	) ) return ACT2_ATTACK_GUILD;
    if ( !str_cmp( buf, "enter_portal"	) ) return ACT2_ENTER_PORTAL;
    if ( !str_cmp( buf, "dreamstate"	) ) return ACT2_DREAMSTATE;
    if ( !str_cmp( buf, "smart_aggressive"	) ) return ACT2_SMART_AGGRESSIVE;
    if ( !str_cmp( buf, "guide"		) ) return ACT2_GUIDE;
    return 0;
}

