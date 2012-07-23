/***************************************************************************
*                                                                          *
*   Gamling code By Quin                                                   *
*                                                                          *
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

#define START_RACE 30
#define ANNOUNCE_RACE 24
#define RACE_REPORTS 8

CARD_DATA *	card_free = NULL;
extern int top_card_data;
const	char   card_name[ 13 ] =
{
    'A','2','3','4','5','6','7','8','9','0','J','Q','K'
};

const	char	suit_name[ 4 ] =
{
    'C','S','H','D'
};

const   char	suit_color[ 4 ] =
{
     'G','G','R','R'
};

int get_coin_type( char * arg );

HORSE_RACE_TYPE horse_race_list[ MAX_HORSE_RACE ] =
{
/* { { { "Horse name", horse_number, odds, current_odds, position, RACING },
	.... },  BET_DATA, timer, PAY_OFFS }  */

    { { { "Quin's Revenge", 1, 1, 0, 0, FALSE },
	{ "Galadrail's Gal", 2, 2, 0, 0, FALSE },
	{ "Aravan's Angel", 3, 3, 0, 0, FALSE },
	{ "Wanderer's Windrider", 4, 2, 0, 0, FALSE },
	{ "Talon's Tramp", 5, 3, 0, 0, FALSE },
	{ "Nikko's Nickle", 6, 4, 0, 0, FALSE },
	{ "Quin's Quarter", 7, 7, 0, 0, FALSE },
	{ "Muhalo's Majesty", 8, 8, 0, 0, FALSE },
	{ "Vivians's Vigor", 9, 9, 0, 0, FALSE },
	{ "Maz's Marvel", 10, 1, 0, 0, FALSE },
	{ "Pearce's Pinicle of Perfection", 11, 11, 0, 0, FALSE },
	{ "Sklosh's Skate", 12, 2, 0, 0, FALSE },
	{ "Dakko's Drunk", 13, 3, 0, 0, FALSE },
	{ "Arithon's Apple", 14, 4, 0, 0, FALSE },
	{ "Classic's Clunker", 5, 15, 0, 0, FALSE },
	{ "Fox's Fickle Filly", 16, 1, 0, 0, FALSE },
	{ "Strider's Stinger", 17, 17, 0, 0, FALSE },
	{ "Opium's Object", 18, 18, 0, 0, FALSE },
	{ "Taran's Terminator", 19, 19, 0, 0, FALSE },
	{ "Talon's Tickler", 20, 20, 0, 0, FALSE } },
	NULL, 0, NULL, 24022 },
    { { { "Jacobs Folly",	20, 20, 0, 0, FALSE },
	{ "Bucephalus",		19, 5, 0, 0, FALSE },
	{ "Flying Cloud",	51, 2, 0, 0, FALSE },
	{ "Mandrake",		9, 3, 0, 0, FALSE },
	{ "Apple of my Eye",	4, 3, 0, 0, FALSE },
	{ "Kiffers Run",	2, 4, 0, 0, FALSE },
	{ "Major Rusty",	99, 30, 0, 0, FALSE },
	{ "Snake-eyes",		11, 1, 0, 0, FALSE },
	{ "Buttercup",		37, 9, 0, 0, FALSE },
	{ "Tried and True",	7, 2, 0, 0, FALSE },
	{ "Greased Lightning",	84, 1, 0, 0, FALSE },
	{ "Running Wild",	48, 4, 0, 0, FALSE },
	{ "Daisy",		65, 7, 0, 0, FALSE },
	{ "Walls Lady Sue",	25, 5, 0, 0, FALSE },
	{ "Crazy Legs",		13, 4, 0, 0, FALSE },
	{ "Thunderclap",	17, 2, 0, 0, FALSE },
	{ "Down Wind",		88, 12, 0, 0, FALSE },
	{ "Dead Man's Hand",	24, 15, 0, 0, FALSE },
	{ "Comet",		5, 3, 0, 0, FALSE },
	{ "Clover in the Heather", 73, 2, 0, 0, FALSE } },
	NULL, 0, NULL, 5163 },
    { { { "White Mark",		22, 5, 0, 0, FALSE },
	{ "Empty Running",	21, 10, 0, 0, FALSE },
	{ "Star Flyer",		75, 3, 0, 0, FALSE },
	{ "Hand of Fate",	50, 4, 0, 0, FALSE },
	{ "Falcon's Time",	9, 2, 0, 0, FALSE },
	{ "Snowball",		4, 3, 0, 0, FALSE },
	{ "Black Mane",		2, 1, 0, 0, FALSE },
	{ "Wilder's Star",	97, 2, 0, 0, FALSE },
	{ "Lost Love",		69, 7, 0, 0, FALSE },
	{ "Long Leader",	1, 1, 0, 0, FALSE },
	{ "Wild Fire",		7, 1, 0, 0, FALSE },
	{ "Stolen Heart",	48, 6, 0, 0, FALSE },
	{ "Silver",		65, 4, 0, 0, FALSE },
	{ "Diamond Mark",	25, 5, 0, 0, FALSE },
	{ "Scout",		13, 3, 0, 0, FALSE },
	{ "Gene's Ticket",	17, 2, 0, 0,  FALSE },
	{ "Snow Flake",		88, 8, 0, 0,  FALSE },
	{ "Old Belle",		24, 15, 0, 0,  FALSE },
	{ "Rose's Beauty",	5, 7, 0, 0,  FALSE },
	{ "Jade Lorraine",	11, 5, 0, 0, FALSE } },
	NULL, 0, NULL, 11210 }

/*
Figure Eight - Wanton Pride - Honorbound 
*/

};

char const * position_name[ 7 ] =
{
    "first",
    "second",
    "third",
    "fourth",
    "fifth",
    "sixth",
    "last"
};

void free_card( CARD_DATA * card )
{
    card->next = card_free;
    card_free = card;
}

CARD_DATA * alloc_card( void )
{
    CARD_DATA * card;

    if ( card_free != NULL )
    {
	card = card_free;
	card_free = card->next;
    }
    else
    {
	card = (CARD_DATA *) alloc_perm( sizeof( CARD_DATA ) );
	top_card_data++;
    }

    card->next = NULL;
    card->value = 0;
    card->suit = 0;

    return card;
}

char * format_cards( CARD_DATA *cards )
{
    static char buf1[MAX_INPUT_LENGTH];
    static char buf[MAX_STRING_LENGTH];
    CARD_DATA * next_card;
    CARD_DATA * first_card;
    CARD_DATA * card;
    int i;

    buf[0] ='\0';

    for ( card = cards ; card != NULL ; card = next_card )
    {
	if ( card != cards )
	    strcat( buf, "\n\r\n\r" );

	first_card = card;
	for ( i = 0 ; i < 10 && card != NULL ; i++ ) card = card->next;
	next_card = card;
	card = first_card;

	for ( i = 0 ; i < 10 && card != NULL ; i++ )
	{
	    if ( card->value == 13 )
		sprintf( buf1, " `%cJOKERJ", suit_color[ card->suit ] );
	    else
	    sprintf( buf1, " `%c%c%c%c%c%c%c", suit_color[ card->suit ],
		card_name[ card->value ], suit_name[ card->suit ],
		card_name[ card->value ], suit_name[ card->suit ],
		card_name[ card->value ], suit_name[ card->suit ] );
	    strcat( buf, buf1 );
	    if ( i != 9 )
		strcat( buf, " " );
	    card = card->next;
	}

	strcat( buf, "\n\r" );
	card = first_card;
	for ( i = 0 ; i < 10 && card != NULL ; i++ )
	{
	    if ( card->value == 13 )
		sprintf( buf1, " `%cO    O", suit_color[ card->suit ] );
	    else
	    sprintf( buf1, " `%c%c    %c", suit_color[ card->suit ],
		suit_name[ card->suit ], card_name[ card->value ] );
	    strcat( buf, buf1 );
	    if ( i != 9 )
		strcat( buf, " " );
	    card = card->next;
	}

	strcat( buf, "\n\r" );
	card = first_card;
	for ( i = 0 ; i < 10 && card != NULL ; i++ )
	{
	    if ( card->value == 13 )
		sprintf( buf1, " `%cK `W** K", suit_color[ card->suit ] );
	    else
	    sprintf( buf1, " `%c%c `W%c%c `%c%c", suit_color[ card->suit ],
		card_name[ card->value ], card_name[ card->value ],
		suit_name[ card->suit ], suit_color[ card->suit ],
		suit_name[ card->suit ] );
	    strcat( buf, buf1 );
	    if ( i != 9 )
		strcat( buf, " " );
	    card = card->next;
	}

	strcat( buf, "\n\r" );
	card = first_card;
	for ( i = 0 ; i < 10 && card != NULL ; i++ )
	{
	    if ( card->value == 13 )
		sprintf( buf1, " `%cE    E", suit_color[ card->suit ] );
	    else
	    sprintf( buf1, " `%c%c    %c", suit_color[ card->suit ],
		suit_name[ card->suit ], card_name[ card->value ] );
	    strcat( buf, buf1 );
	    if ( i != 9 )
		strcat( buf, " " );
	    card = card->next;
	}

	strcat( buf, "\n\r" );
	card = first_card;
	for ( i = 0 ; i < 10 && card != NULL ; i++ )
	{
	    if ( card->value == 13 )
		sprintf( buf1, " `%cRJOKER", suit_color[ card->suit ] );
	    else
	    sprintf( buf1, " `%c%c%c%c%c%c%c", suit_color[ card->suit ],
		card_name[ card->value ], suit_name[ card->suit ],
		card_name[ card->value ], suit_name[ card->suit ],
		card_name[ card->value ], suit_name[ card->suit ] );
	    strcat( buf, buf1 );
	    if ( i != 9 )
		strcat( buf, " " );
	    card = card->next;
	}

    }

    return buf;
}

void clear_deck( ROOM_INDEX_DATA * pRoom )
{
    CHAR_DATA * ch;
    CARD_DATA * card;
    CARD_DATA * card_next;

    for ( ch = pRoom->people ; ch != NULL ; ch = ch->next_in_room )
    {
	if ( ch->pcdata == NULL )
	    continue;

	for ( card = ch->pcdata->discarded ; card != NULL ; card = card_next )
	{
	    card_next = card->next;
	    free_card( card );
	}

	for ( card = ch->pcdata->deck ; card != NULL ; card = card_next )
	{
	    card_next = card->next;
	    free_card( card );
	}

	for ( card = ch->pcdata->hand ; card != NULL ; card = card_next )
	{
	    card_next = card->next;
	    free_card( card );
	}

	for ( card = ch->pcdata->played ; card != NULL ; card = card_next )
	{
	    card_next = card->next;
	    free_card( card );
	}

	ch->pcdata->discarded = NULL;
	ch->pcdata->played = NULL;
	ch->pcdata->hand = NULL;
	ch->pcdata->deck = NULL;
    }
}

void sort_hand( CHAR_DATA * ch, bool fSuit )
{
    CARD_DATA * card;
    CARD_DATA * card_next;
    CARD_DATA * cards = ch->pcdata->hand;
    int suit,value;

    ch->pcdata->hand = NULL;

    for ( suit = 3 ; suit >= 0 ; suit-- )
    {
		while ( cards != NULL && cards->value == 13 && cards->suit == suit )
		{
		    card_next = cards->next;
		    cards->next = ch->pcdata->hand;
		    ch->pcdata->hand = cards;
		    cards = card_next;
		}

		for ( card = cards ; card != NULL ; card = card_next )
		{
		    card_next = card->next;

		    if ( card_next == NULL )
			continue;

		    if ( card_next->value == 13 && card_next->suit == suit )
		    {
			card->next = card_next->next;
			card_next->next = ch->pcdata->hand;
			ch->pcdata->hand = card_next;
			card_next = card;
		    }
		}
    }

    if ( fSuit )
    {
	for ( suit = 3 ; suit >= 0 ; suit-- )
	    for ( value = 13 ; value > 0 ; value-- )
	    {
		while ( cards != NULL && cards->value == value % 13 && cards->suit == suit )
		{
		    card_next = cards->next;
		    cards->next = ch->pcdata->hand;
		    ch->pcdata->hand = cards;
		    cards = card_next;
		}

		for ( card = cards ; card != NULL ; card = card_next )
		{
		    card_next = card->next;

		    if ( card_next == NULL )
			continue;

		    if ( card_next->value == value % 13 && card_next->suit == suit )
		    {
			card->next = card_next->next;
			card_next->next = ch->pcdata->hand;
			ch->pcdata->hand = card_next;
			card_next = card;
		    }
		}
	    }
    }
    else
    {
	for ( value = 13 ; value > 0 ; value-- )
	    for ( suit = 3 ; suit >= 0 ; suit-- )
	    {
		while ( cards != NULL && cards->value == value % 13 && cards->suit == suit )
		{
		    card_next = cards->next;
		    cards->next = ch->pcdata->hand;
		    ch->pcdata->hand = cards;
		    cards = card_next;
		}

		for ( card = cards ; card != NULL ; card = card_next )
		{
		    card_next = card->next;

		    if ( card_next == NULL )
			continue;

		    if ( card_next->value == value % 13 && card_next->suit == suit )
		    {
			card->next = card_next->next;
			card_next->next = ch->pcdata->hand;
			ch->pcdata->hand = card_next;
			card_next = card;
		    }
		}
	    }
    }
}

void do_deck( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CARD_DATA * card;
    CHAR_DATA * victim;
    int decks = 1;
    int num,suit,value,c;

    if ( ch->pcdata == NULL )
    {
	send_to_char( "Only players allowed to play cards.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( is_number( arg ) )
    {
	if ( ( decks = atoi( arg ) ) == 54 )
	{
	    decks = 1;
	    do_deck( ch, "1" );
	    do_deck( ch, "2 joker" );
	    return;
	}
	else if ( ( decks < 1 || decks > 5 )
	       && *argument == '\0' )
	{
	    send_to_char( "Number of decks out of range( 1 - 5 ).\n\r", ch );
	    return;
	}
    }
    if ( !str_cmp( arg, "stop" ) )
    {
	if ( ch->pcdata->deck == NULL )
	{
	    send_to_char( "You are not a dealer.\n\r", ch );
	    return;
	}

	clear_deck( ch->in_room );
	return;
    }

    for ( victim = ch->in_room->people ; victim != NULL ; victim = victim->next_in_room )
	if ( victim != ch && victim->pcdata != NULL && victim->pcdata->deck != NULL )
	{
	    send_to_char( "Sorry only 1 dealer per room.\n\r", ch );
	    return;
	}

    if ( !str_prefix( argument, "joker" ) && *argument != '\0' )
    {
	if ( ch->pcdata->deck == NULL )
        {
	    send_to_char( "Must make the deck before you add jokers.\n\r", ch );
	    return;
   	}

	if ( decks < 1 || decks > 100 )
	{
	    send_to_char( "Number of jokers is out of range.\n\r", ch );
	    return;
	}

	for ( c = 0 ; c < decks ; c++ )
	{
	    static int suits[ 4 ] = { 0, 2, 1, 3 };

	    card = alloc_card();	    
	    card->value = 13;
	    card->suit = suits[ c % 4 ];

	    card->next = ch->pcdata->deck;
	    ch->pcdata->deck = card;
	}

	sprintf( buf, "$n has added %d jokers to the deck.", decks );
	act( buf, ch, NULL, NULL, TO_ROOM );
	sprintf( buf, "You have added %d jokers to the deck.\n\r", decks );
	send_to_char( buf, ch );
	return;
    } 

    clear_deck( ch->in_room );

    if ( !str_cmp( arg, "euchre" ) )
    {
	    for ( suit = 0 ; suit < 4 ; suit++ )
		for ( value = 8 ; value < 14 ; value++ )
		{
		    card = alloc_card();
		    card->value = value % 13;
		    card->suit = suit;
		    card->next = ch->pcdata->deck;
		    ch->pcdata->deck = card;
		}
    }
    else
        for ( num = 0 ; num < decks ; num++ )
	    for ( suit = 0 ; suit < 4 ; suit++ )
		for ( value = 0 ; value < 13 ; value++ )
		{
		    card = alloc_card();
		    card->value = value;
		    card->suit = suit;
		    card->next = ch->pcdata->deck;
		    ch->pcdata->deck = card;
		}

    act( "$n prepares the deck for a game of cards.\n\r", ch, NULL, NULL, TO_ROOM );
    act( "You prepare the deck for a game of cards.\n\r", ch, NULL, NULL, TO_CHAR );
}

void do_deal( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CARD_DATA * card;
    CARD_DATA * card_next;
    CARD_DATA * delt;
    CHAR_DATA * victim;
    int cards;
    int num;
    int chance;
    bool fDiscard = FALSE;

    if ( ch->pcdata == NULL || ch->pcdata->deck == NULL )
    {
	send_to_char( "You must first have a deck to deal.(type deck)\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( !is_number( arg ) || arg[ 0 ] == '\0' )
    {
	send_to_char( "Syntax: deal <number> <player name>\n\r", ch );
	return;
    }

    cards = atoi( arg );

    argument = one_argument( argument, arg );

    if ( arg[0] == '\0' )
	victim = ch;
    else if ( !str_cmp( arg, "discard" ) )
    {
	victim = ch;
	fDiscard = TRUE;
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }
    else if ( victim->pcdata == NULL )
    {
	send_to_char( "You can only deal to players.\n\r", ch );
	return;
    }

    num = 0;
    for ( card = ch->pcdata->deck ; card != NULL ; card = card->next )
	num++;

    if ( num < cards )
    {
	sprintf( buf, "Sorrr only %d cards left.\n\r", num );
	send_to_char( buf, ch );
	return;
    }

    for ( num = 0 ; num < cards ; num++ )
    {
	if ( ch->pcdata->deck == NULL )
	{
	    send_to_char( "Not enough cards in the deck.\n\r", ch );
	    cards = num;
	    break;
	}

	delt = ch->pcdata->deck;
	chance = 0;
	for ( card = ch->pcdata->deck ; card != NULL ; card = card_next )
	{
	    card_next = card->next;

	    if ( number_range( 0, chance++ ) == 0 )
		delt = card;
	}

	if ( delt == ch->pcdata->deck )
	    ch->pcdata->deck = delt->next;
	else
	{
	    for ( card = ch->pcdata->deck ; card != NULL && card->next != delt; card = card->next );

	    if ( card != NULL )
	        card->next = delt->next;
	    else
	    {
		send_to_char("Error Error Error in dealing card.\n\r", ch );
		log_string( "Lost card in do_deal" );
		return;
	    }
	}

	if ( !fDiscard )
	{
	    delt->next = victim->pcdata->hand;
	    victim->pcdata->hand = delt;
	}
	else
	{
	    delt->next = ch->pcdata->discarded;
	    ch->pcdata->discarded = delt;
	}
    }

    if ( fDiscard )
    {
        sprintf( buf, "$n puts the top %d cards in the discard pile.", cards );
        act( buf, ch, victim, victim, TO_ROOM );
        sprintf( buf, "You put the top %d cards in the discard pile.", cards );
        act( buf, ch, victim, victim, TO_CHAR );
    }
    else
    {
        sprintf( buf, "$n deals %d cards to $N.", cards );
        act( buf, ch, victim, victim, TO_NOTVICT );
        sprintf( buf, "$n deals %d cards to you.", cards );
        act( buf, ch, victim, victim, TO_VICT );
        sprintf( buf, "You deal %d cards to $N.", cards );
        act( buf, ch, victim, victim, TO_CHAR );

        sort_hand( victim, FALSE );
    }
	
}

void do_draw( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CARD_DATA * card;
    CARD_DATA * card_next;
    CARD_DATA * delt;
    CHAR_DATA * victim;
    int cards;
    int num;
    int chance;
    bool fDiscard = FALSE;

    if ( ch->pcdata == NULL || ch->pcdata->hand == NULL )
    {
	send_to_char( "You must first have some cards to draw more.\n\r", ch );
	return;
    }

    for ( victim = ch->in_room->people ; victim != NULL && ( victim->pcdata == NULL || ( victim->pcdata->deck == NULL && victim->pcdata->discarded == NULL ) ) ; victim = victim->next_in_room );

    if ( victim == NULL )
    {
	send_to_char( "No more cards to be taken.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );

    if ( !str_cmp( arg, "discard" ) )
    {
	argument = one_argument( argument, arg );
	fDiscard = TRUE;
    }

    if ( arg[0] == '\0' )
	cards = 1;
    else if ( !is_number( arg ) )
    {
	send_to_char( "Syntax: draw <number>\n\r", ch );
	return;
    }
    else
        cards = URANGE( 0, atoi( arg ), 520 );

    if ( !str_cmp( argument, "discard" ) )
	fDiscard = TRUE;

    if ( !fDiscard )
    {

    num = 0;
    for ( card = victim->pcdata->deck ; card != NULL ; card = card->next )
	num++;

    if ( num < cards )
    {
	sprintf( buf, "Sorrr only %d cards left.\n\r", num );
	send_to_char( buf, ch );
	return;
    }

    for ( num = 0 ; num < cards ; num++ )
    {
	if ( victim->pcdata->deck == NULL )
	{
	    send_to_char( "Not enough cards in the deck.\n\r", ch );
	    cards = num;
	    break;
	}

	delt = victim->pcdata->deck;
	chance = 0;
	for ( card = victim->pcdata->deck ; card != NULL ; card = card_next )
	{
	    card_next = card->next;

	    if ( number_range( 0, chance++ ) == 0 )
		delt = card;
	}

	if ( delt == victim->pcdata->deck )
	    victim->pcdata->deck = delt->next;
	else
	{
	    for ( card = victim->pcdata->deck ; card != NULL && card->next != delt; card = card->next );

	    if ( card != NULL )
	        card->next = delt->next;
	    else
	    {
		send_to_char("Error Error Error in dealing card.\n\r", ch );
		log_string( "Lost card in do_draw" );
		return;
	    }
	}

	delt->next = ch->pcdata->hand;
	ch->pcdata->hand = delt;
    }

    sprintf( buf, "$n draws %d cards.", cards );
    act( buf, ch, victim, victim, TO_ROOM );
    sprintf( buf, "You draw %d cards.", cards );
    act( buf, ch, victim, victim, TO_CHAR );

    sort_hand( ch, FALSE );

    }	
    else
    {

        num = 0;
        for ( card = victim->pcdata->discarded ; card != NULL ; card = card->next )
	    num++;

        if ( num < cards )
        {
	    sprintf( buf, "Sorrr only %d cards left.\n\r", num );
	    send_to_char( buf, ch );
	    return;
        }

        for ( num = 0 ; num < cards ; num++ )
        {
	    card = victim->pcdata->discarded;
	    victim->pcdata->discarded = card->next;
	    card->next = ch->pcdata->hand;
	    ch->pcdata->hand = card;
        }

        sprintf( buf, "$n draws the first %d cards from the discard pile.", cards );
        act( buf, ch, victim, victim, TO_ROOM );
        sprintf( buf, "You draw the first %d cards from the discard pile.", cards );
        act( buf, ch, victim, victim, TO_CHAR );

        sort_hand( ch, FALSE );
    }	
}

void do_sort( CHAR_DATA * ch, char * argument )
{
   char arg[MAX_INPUT_LENGTH];
   bool fSuit = FALSE;

   if ( ch->pcdata == NULL || ch->pcdata->hand == NULL )
   {
	send_to_char( "You must have a hand before you can sort it.\n\r", ch );
	return;
   }

   one_argument( argument, arg );

   if ( !str_cmp( arg, "suit" ) )
	fSuit = TRUE;	

   sort_hand( ch, fSuit );
}

void do_play( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_STRING_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * victim;
    CARD_DATA * cards;
    CARD_DATA * card_next;
    CARD_DATA * discard;
    CARD_DATA * played;
    int card,c;
    int num_cards = 0;
    bool selected[ 520 ];
    bool fDiscard = FALSE;
    played = NULL;

    if ( ch->pcdata == NULL || ch->pcdata->hand == NULL )
    {
	send_to_char( "You must first have a hand to play a card.\n\r", ch );
	return;
    }

    if ( *argument == '\0' )
    {
	send_to_char( "Syntax: play <number> <number> ...etc\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( !str_cmp( arg, "all" ) )
    {
        argument = one_argument( argument, arg );

	for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = TRUE;

	if ( !str_cmp( argument, "discard" ) )
	    fDiscard = TRUE;
    }
    else if ( is_number( arg ) )
    {
        for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = FALSE;

        while ( *argument != '\0' )
        {
            argument = one_argument( argument, arg );

	    if ( !str_cmp( arg, "discard" ) )
	    {
		fDiscard = TRUE;
		continue;
	    }

	    if ( ( card = atoi( arg ) ) > 520 )
	    {
	        sprintf( buf, "Card %d is out of range.\n\r", card );
	        send_to_char( buf, ch );
	        continue;
	    }

	    selected[ card ] = TRUE;
	}
    }
    else
    {
	int value;
	int suit;

        for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = FALSE;

        while ( *argument != '\0' )
        {
            argument = one_argument( argument, arg );

	    if ( !str_cmp( arg, "discard" ) )
	    {
		fDiscard = TRUE;
		continue;
	    }

	    value = -1;
	    suit = -1;

	    for ( c = 0 ; c < 13 ; c++ )
		if ( LOWER( card_name[ c ] ) == LOWER( arg[0] ) ) value = c;

	    for ( c = 0 ; c < 4 ; c++ )
		if ( LOWER( suit_name[ c ] ) == LOWER( arg[1] ) ) suit = c;

	    if( !str_cmp( arg, "joker" ) || !str_cmp( arg, "**" ) )
	    {
		value = 13;
	   	suit = 0;
	    }

	    if ( suit == -1 || value == -1 )
	    {
		sprintf( buf, "Invalid card %s.\n\r", arg );
		send_to_char( buf, ch );
		continue;
	    }

	    card = 1;
	    for ( cards = ch->pcdata->hand ; cards != NULL && card != 0 ; cards = cards->next )
	    {
		if ( ( cards->suit == suit || value == 13 ) && cards->value == value && !selected[ card ] )
		{
		    selected[ card ] = TRUE;
		    card = -1;
		}

		card++;
	    }
	}
    }

    num_cards = 0;
    card = 1;

    while( selected[ card  ] && ch->pcdata->hand != NULL )
    {
	discard = ch->pcdata->hand;
	ch->pcdata->hand = discard->next;
	discard->next = played;
	played = discard;
	num_cards++;
	card++;
    }	

    for ( cards = ch->pcdata->hand ; cards != NULL ; cards = card_next )
    {
	card_next = cards->next;
	card++;

	if ( selected[ card ] && card_next != NULL )
	{
	    discard = card_next;
	    cards->next = card_next->next;
	    card_next = cards;
	    discard->next = played;
	    played = discard;
	    num_cards++;
	}
    }	    

    if ( played == NULL )
    {
	send_to_char( "No cards played.\n\r", ch );
	return;
    }

    act( "$n plays:", ch, NULL, NULL, TO_ROOM );
    act( "You play:", ch, NULL, NULL, TO_CHAR );

    strcpy( buf, format_cards( played ) );
    act( buf, ch, NULL, NULL, TO_ROOM );	
    act( buf, ch, NULL, NULL, TO_CHAR );	


    for ( cards = played ; cards->next != NULL ; cards = cards->next );

    if ( fDiscard )
    {
        for ( victim = ch->in_room->people ; victim != NULL && ( victim->pcdata == NULL || ( victim->pcdata->deck == NULL && victim->pcdata->discarded == NULL ) ) ; victim = victim->next_in_room );
    
        if ( victim == NULL )
        {
	    send_to_char( "Cannot find the dealer in the room, you are the new dealer.\n\r", ch );
	    victim = ch;
        }

	cards->next = victim->pcdata->discarded;
	victim->pcdata->discarded = played;
    }
    else
    {
	cards->next = ch->pcdata->played;
	ch->pcdata->played = played;
    }
}


void do_discard( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CARD_DATA * cards;
    CARD_DATA * card_next;
    CARD_DATA * discard;
    CHAR_DATA * victim;
    int card,c;
    int num_cards = 0;
    bool selected[ 520 ];
    bool fPlayed = FALSE;

    if ( ( ch->pcdata == NULL || ch->pcdata->hand == NULL )
      && *argument != '\0' )
    {
	send_to_char( "You must first have a hand to discard.\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( !str_cmp( arg, "played" ) )
    {
	argument = one_argument( argument, arg );
	one_argument( argument, arg );
	fPlayed = TRUE;
        if ( arg[0] == '\0' )
    	    strcpy( arg, "all" );
    }

    for ( victim = ch->in_room->people ; victim != NULL && ( victim->pcdata == NULL || ( victim->pcdata->deck == NULL && victim->pcdata->discarded == NULL ) ) ; victim = victim->next_in_room );
    
    if ( *argument == '\0' && !fPlayed )
    {
	if ( victim == NULL || victim->pcdata->discarded == NULL )
	{
	    send_to_char( "No discard pile to view.\n\r", ch );
	    return;
	}

	send_to_char( "Discarded:\n\r", ch );
	send_to_char( format_cards( victim->pcdata->discarded ), ch );
	act( "$n peaks at the discard pile.\n\r", ch, NULL, NULL, TO_ROOM );
	return;
    }

    if ( victim == NULL )
    {
	send_to_char( "Cannot find the dealer in the room, you are the new dealer.\n\r", ch );
	victim = ch;
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = TRUE;
    }
    else if ( is_number( arg ) )
    {
        for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = FALSE;

        while ( *argument != '\0' )
        {
            argument = one_argument( argument, arg );

	    if ( !str_cmp( arg, "played" ) )
	    {
		fPlayed = TRUE;
		continue;
	    }

	    if ( ( card = atoi( arg ) ) > 520 )
	    {
	        sprintf( buf, "Card %d is out of range.\n\r", card );
	        send_to_char( buf, ch );
	        continue;
	    }

	    selected[ card ] = TRUE;
	}
    }
    else
    {
	int value;
	int suit;

        for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = FALSE;

        while ( *argument != '\0' )
        {
            argument = one_argument( argument, arg );

	    value = -1;
	    suit = -1;

	    for ( c = 0 ; c < 13 ; c++ )
		if ( LOWER( card_name[ c ] ) == LOWER( arg[0] ) ) value = c;

	    for ( c = 0 ; c < 4 ; c++ )
		if ( LOWER( suit_name[ c ] ) == LOWER( arg[1] ) ) suit = c;


	    if( !str_cmp( arg, "joker" ) || !str_cmp( arg, "**" ) )
	    {
		value = 13;
		suit = 0;
	    }

	    if ( suit == -1 || value == -1 )
	    {
		sprintf( buf, "Invalid card %s.\n\r", arg );
		send_to_char( buf, ch );
		continue;
	    }

	    card = 1;

	    if ( !fPlayed )
	    for ( cards = ch->pcdata->hand ; cards != NULL && card != 0 ; cards = cards->next )
	    {
		if ( ( cards->suit == suit || value == 13 ) && cards->value == value && !selected[ card ] )
		{
		    selected[ card ] = TRUE;
		    card = -1;
		}

		card++;
	    }
	    else
	    for ( cards = ch->pcdata->played ; cards != NULL && card != 0 ; cards = cards->next )
	    {
		if ( ( cards->suit == suit || value == 13 ) && cards->value == value && !selected[ card ] )
		{
		    selected[ card ] = TRUE;
		    card = -1;
		}

		card++;
	    }
	}
    }

    num_cards = 0;
    card = 1;

    if ( !fPlayed )
    {

    while( selected[ card  ] && ch->pcdata->hand != NULL )
    {
	discard = ch->pcdata->hand;
	ch->pcdata->hand = discard->next;
	discard->next = victim->pcdata->discarded;
	victim->pcdata->discarded = discard;
	num_cards++;
	card++;
    }	

    for ( cards = ch->pcdata->hand ; cards != NULL ; cards = card_next )
    {
	card_next = cards->next;
	card++;

	if ( selected[ card ] && card_next != NULL )
	{
	    discard = card_next;
	    cards->next = card_next->next;
	    card_next = cards;
	    discard->next = victim->pcdata->discarded;
	    victim->pcdata->discarded = discard;
	    num_cards++;
	}
    }	    

    sprintf( buf, "$n discards %d cards, from $s hand.", num_cards );
    act( buf, ch, NULL, NULL, TO_ROOM );
    sprintf( buf, "You discard %d cards, from your hand.", num_cards );
    act( buf, ch, NULL, NULL, TO_CHAR );

    }
    else
    {

    while( selected[ card  ] && ch->pcdata->played != NULL )
    {
	discard = ch->pcdata->played;
	ch->pcdata->played = discard->next;
	discard->next = victim->pcdata->discarded;
	victim->pcdata->discarded = discard;
	num_cards++;
	card++;
    }	

    for ( cards = ch->pcdata->played ; cards != NULL ; cards = card_next )
    {
	card_next = cards->next;
	card++;

	if ( selected[ card ] && card_next != NULL )
	{
	    discard = card_next;
	    cards->next = card_next->next;
	    card_next = cards;
	    discard->next = victim->pcdata->discarded;
	    victim->pcdata->discarded = discard;
	    num_cards++;
	}
    }	    

    sprintf( buf, "$n discards %d played cards.", num_cards );
    act( buf, ch, NULL, NULL, TO_ROOM );
    sprintf( buf, "You discard %d played cards.", num_cards );
    act( buf, ch, NULL, NULL, TO_CHAR );

    }


	
}

void do_junk( CHAR_DATA * ch, char * argument )
{
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    CARD_DATA * cards;
    CARD_DATA * card_next;
    CARD_DATA * discard;
    int card,c;
    int num_cards = 0;
    bool selected[ 520 ];
    bool fPlayed = FALSE;

    if ( ch->pcdata == NULL || ch->pcdata->hand == NULL )
    {
	send_to_char( "You must first have a hand to junk a card.\n\r", ch );
	return;
    }

    one_argument( argument, arg );

    if ( !str_cmp( arg, "played" ) )
    {
	argument = one_argument( argument, arg );
	one_argument( argument, arg );
	fPlayed = TRUE;
        if ( arg[0] == '\0' )
    	    strcpy( arg, "all" );
    }

    if ( !str_cmp( arg, "all" ) )
    {
	for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = TRUE;
    }
    else if ( is_number( arg ) )
    {
        for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = FALSE;

        while ( *argument != '\0' )
        {
            argument = one_argument( argument, arg );

	    if ( !str_cmp( arg, "played" ) )
	    {
		fPlayed = TRUE;
		continue;
	    }

	    if ( ( card = atoi( arg ) ) > 520 )
	    {
	        sprintf( buf, "Card %d is out of range.\n\r", card );
	        send_to_char( buf, ch );
	        continue;
	    }

	    selected[ card ] = TRUE;
	}
    }
    else
    {
	int value;
	int suit;

        for ( c = 0 ; c < 520 ; c++ )
	    selected[ c ] = FALSE;

        while ( *argument != '\0' )
        {
            argument = one_argument( argument, arg );

	    value = -1;
	    suit = -1;

	    for ( c = 0 ; c < 13 ; c++ )
		if ( LOWER( card_name[ c ] ) == LOWER( arg[0] ) ) value = c;

	    for ( c = 0 ; c < 4 ; c++ )
		if ( LOWER( suit_name[ c ] ) == LOWER( arg[1] ) ) suit = c;

	    if( !str_cmp( arg, "joker" ) || !str_cmp( arg, "**" ) )
	    {
		value = 13;
		suit = 0;
	    }

	    if ( suit == -1 || value == -1 )
	    {
		sprintf( buf, "Invalid card %s.\n\r", arg );
		send_to_char( buf, ch );
		continue;
	    }

	    card = 1;

	    if ( !fPlayed )
	    for ( cards = ch->pcdata->hand ; cards != NULL && card != 0 ; cards = cards->next )
	    {
		if ( ( cards->suit == suit || value == 13 ) && cards->value == value && !selected[ card ] )
		{
		    selected[ card ] = TRUE;
		    card = -1;
		}

		card++;
	    }
	    else
	    for ( cards = ch->pcdata->played ; cards != NULL && card != 0 ; cards = cards->next )
	    {
		if ( ( cards->suit == suit || value == 13 ) && cards->value == value && !selected[ card ] )
		{
		    selected[ card ] = TRUE;
		    card = -1;
		}

		card++;
	    }
	}
    }

    num_cards = 0;
    card = 1;

    if ( !fPlayed )
    {

    while( selected[ card  ] && ch->pcdata->hand != NULL )
    {
	discard = ch->pcdata->hand;
	ch->pcdata->hand = discard->next;
	free_card( discard );
	num_cards++;
	card++;
    }	

    for ( cards = ch->pcdata->hand ; cards != NULL ; cards = card_next )
    {
	card_next = cards->next;
	card++;

	if ( selected[ card ] && card_next != NULL )
	{
	    discard = card_next;
	    cards->next = card_next->next;
	    card_next = cards;
	    free_card( discard );
	    num_cards++;
	}
    }	    

    sprintf( buf, "$n junks %d cards, from $s hand.", num_cards );
    act( buf, ch, NULL, NULL, TO_ROOM );
    sprintf( buf, "You junks %d cards, from your hand.", num_cards );
    act( buf, ch, NULL, NULL, TO_CHAR );

    }
    else
    {

    while( selected[ card  ] && ch->pcdata->played != NULL )
    {
	discard = ch->pcdata->played;
	ch->pcdata->played = discard->next;
	free_card( discard );
	num_cards++;
	card++;
    }	

    for ( cards = ch->pcdata->played ; cards != NULL ; cards = card_next )
    {
	card_next = cards->next;
	card++;

	if ( selected[ card ] && card_next != NULL )
	{
	    discard = card_next;
	    cards->next = card_next->next;
	    card_next = cards;
	    free_card( discard );
	    num_cards++;
	}
    }	    

    sprintf( buf, "$n junk %d played cards.", num_cards );
    act( buf, ch, NULL, NULL, TO_ROOM );
    sprintf( buf, "You junk %d played cards.", num_cards );
    act( buf, ch, NULL, NULL, TO_CHAR );

    }


	
}

void do_hand( CHAR_DATA * ch, char * argument )
{
    char arg[MAX_INPUT_LENGTH];
    CHAR_DATA * victim;

    one_argument( argument, arg );

    if ( arg[0] == '\0' )
    {

        if ( ch->pcdata == NULL )
        {
	    send_to_char( "You can't play cards.\n\r", ch );
	    return;
        }

	if ( ch->pcdata->hand != NULL )
            send_to_char( format_cards( ch->pcdata->hand ), ch );
	else
	    send_to_char( "You don't have any cards in your hand.\n\r", ch );

	if ( ch->pcdata->played != NULL )
	{
	    send_to_char( "\n\r`WPlayed:\n\r\n\r", ch );
	    send_to_char( format_cards( ch->pcdata->played ), ch );
	}
	return;
    }
    else if ( ( victim = get_char_room( ch, arg ) ) == NULL )
    {
	send_to_char( "They are not here.\n\r", ch );
	return;
    }
    else
    {

        if ( victim->pcdata == NULL )
        {
	    send_to_char( "You can't play cards.\n\r", ch );
	    return;
        }

	if ( victim->pcdata->hand != NULL )
	{
	    CARD_DATA * card;
	    int n = 0;

	    for ( card = victim->pcdata->hand ; card != NULL ; card = card->next ) n++;

	    if ( n == 1 )
		act( "$N has 1 card in $S hand.\n\r", ch, victim, victim, TO_CHAR );
	    else
	    {
		sprintf( arg, "$N has %d cards in $s hand.\n\r", n );
		act( arg, ch, victim, victim, TO_CHAR );
	    }
	}
	else
	    act( "$N has no cards in $S hand.\n\r", ch, victim, victim, TO_CHAR );

	if ( victim->pcdata->played != NULL )
	{
	    send_to_char( "\n\r`WPlayed:\n\r\n\r", ch );
	    send_to_char( format_cards( victim->pcdata->played ), ch );
	}
	return;
    }
}

void upper_lower_triple( CHAR_DATA * ch, char * argument, int type )
{
    char buf[MAX_INPUT_LENGTH];
    char arg[MAX_INPUT_LENGTH];
    int dice[ 3 ];
    int total = 0;
    int i;
    int roll_type;
    int value;
    int coin_type = 0;

    argument = one_argument ( argument, arg );
    coin_type = ( *argument == '\0' ? ch->in_room->area->world : get_coin_type( argument ) );

    if ( arg[0] == '\0' || !is_number( arg ) || coin_type == -1 )
    {
	sprintf( buf, "Syntax: %s <amount of bet> [coin type]\n\r", 
	    ( type == 0 ? "upper" :
		( type == 1 ? "lower" : "triple" ) ) );
	send_to_char( buf, ch );
	return;
    }
    
    if ( ( value = atoi( arg ) ) < 1 )
    {
	send_to_char( "Sorry your bet must be greater then 1 coin.\n\r", ch );
	return;
    }

    if ( value > ch->gold[ coin_type ] )
    {
	send_to_char( "You don't have enough to cover that bet.\n\r", ch );
	return;
    }

    if ( value > 2000000 )
    {
	send_to_char( "Sorry you cannot bet more then 2000000.\n\r", ch );
	return;
    }

    for ( i = 0 ; i < 3 ; i++ )
    {
	dice[ i ] = number_range( 1, 6 );
	total += dice[ i ];
    }

    sprintf( buf, "You roll a %d, %d and %d.\n\r", dice[ 0 ], dice[ 1 ], dice[ 2 ] );
    send_to_char( buf, ch );

    if ( dice[ 0 ] == dice[ 1 ] && dice[ 0 ] == dice[ 2 ] )
    {
	roll_type = 2;
	send_to_char( "Triple!\n\r", ch );
	if ( type == 2 )
	{
	    value *= 10;
	    sprintf( buf, "You win %d %s!\n\r", value, coin_names[ coin_type ] );
	    send_to_char( buf, ch );
	    ch->gold[ coin_type ] += value;
	    return;
	}
    }
    else if ( total < 11 )
    {
	roll_type = 1;
	send_to_char( "Lower!\n\r", ch );
    }
    else 
    {
	roll_type = 0;
	send_to_char( "Upper!\n\r", ch );
    }

    if ( type == roll_type )
    {
	sprintf( buf, "You win %d %s!", value, coin_names[ coin_type ] );
	send_to_char( buf, ch );
	ch->gold[ coin_type ] += value;
	return;
    }
    else
    {
	sprintf( buf, "You loose %d %s.\n\r", value, coin_names[ coin_type ] );
	send_to_char( buf, ch );
	ch->gold[ coin_type ] -= value;
	return;
    }
}

void do_upper( CHAR_DATA * ch, char * argument )
{
    upper_lower_triple( ch, argument, 0 );
}

void do_lower( CHAR_DATA * ch, char * argument )
{
    upper_lower_triple( ch, argument, 1 );
}

void do_triple( CHAR_DATA * ch, char * argument )
{
    upper_lower_triple( ch, argument, 2 );
}

void display_horses( HORSE_RACE_TYPE * race )
{
    int horse,odd,i;
    char buf[MAX_INPUT_LENGTH];
    char buf1[MAX_INPUT_LENGTH];
    char buf2[MAX_INPUT_LENGTH];
    
    sprintf( buf1, "-----------------------------------------------------------------------\n\r" );
    send_to_room( race->in_room, buf1 );
    for ( odd = 1; odd < 100 ; odd++ )
    for ( horse = 0 ; horse < MAX_HORSE ; horse++ )
	if ( race->horse[ horse ].racing && race->horse[ horse ].curr_odds == odd )
	{
	    buf[0] ='|';
	    buf[1] = ' ';
	    buf[2] ='\0';
	    for ( i = 0 ; i < race->horse[ horse ].position ; i++ )
	    {
		if ( i == 58 )
		    strcat( buf, ":" );
		else
	      	    strcat( buf, " " );
	    }
	    sprintf( buf2, "%2d", race->horse[ horse ].number );
	    strcat( buf, buf2 );
	    for ( i = strlen( buf ) ; i < 70 ; i++ )
	    {
		if ( i == 60 )
		    strcat( buf, ":" );
		else
	      	    strcat( buf, " " );
	    }
	    strcat( buf, "|\n\r" );
	    send_to_room( race->in_room, buf );
	    send_to_room( race->in_room, buf1 );
	}
}

void race_update( void )
{
    BET_DATA *bets;
    BET_DATA *next_bet;
    char buf[MAX_INPUT_LENGTH];
    ROOM_INDEX_DATA * in_room;
    int race,horse,i,position,winner;
    DESCRIPTOR_DATA * d;

    for ( race = 0 ; race < MAX_HORSE_RACE; race++ )
    {

	if ( horse_race_list[ race ].timer == 0 )
	    continue;

        if ( horse_race_list[ race ].timer < ANNOUNCE_RACE )
	{
	     horse_race_list[ race ].timer++;
	     continue;
	}

   	in_room = get_room_index( horse_race_list[ race ].in_room );
	if ( horse_race_list[ race ].timer == ANNOUNCE_RACE )
	{
	     horse_race_list[ race ].timer++;
             for ( d = descriptor_list; d != NULL ; d = d->next )
	     {
	         if ( d->connected <= CON_PLAYING
		     && d->character != NULL
		     && d->character->in_room != NULL
	  	     && d->character->in_room->area == in_room->area )
	           act( "The town crier yells 'The Great Race is about to begin'.", d->character, NULL, NULL, TO_CHAR );
	     }

	    continue;
	}

	if ( horse_race_list[ race ].timer <  START_RACE - 1 )
	{
	    horse_race_list[ race ].timer ++;
	    continue;
	}

	if ( horse_race_list[ race ].timer == START_RACE - 1 )
	{
	    send_to_room( horse_race_list[ race ].in_room, "Betting windows closed.\n\r" );
	    horse_race_list[ race ].timer++;
	    continue;
	}
/*
	if ( horse_race_list[ race ].timer == START_RACE )
	{
	    send_to_room( horse_race_list[ race ].in_room, "The trumpets sound and they are off.\n\r" );
	    horse_race_list[ race ].timer++;
	    continue;
	}

	if ( horse_race_list[ race ].timer < START_RACE + RACE_REPORTS )
	{

	    for ( horse = 0 ; horse < MAX_HORSE ; horse++ )
		for ( i = 0 ; i < 10 ; i++ )
		    if ( horse_race_list[ race ].horse[ horse ].racing && number_range( -1, horse_race_list[ race ].horse[ horse ].curr_odds ) == 0 )
			horse_race_list[ race ].horse[ horse ].position++;

	    position = 0;
	    sprintf( buf, "The track announcer says 'At the %s pole it's ...'\n\r\n\r", position_name[ horse_race_list[ race ].timer - START_RACE - 1 ] );
	    send_to_room( horse_race_list[ race ].in_room, buf );
	    for ( i = 5 * ( horse_race_list[ race ].timer - START_RACE ) ; i >= 0 ; i-- )
	    {
		buf[0] = '\0';
		if ( position == 0 )
	 	    sprintf( buf, "The track announcer says '" );
		else 
	 	    sprintf( buf, "The track announcer says 'followed by " );
                found = FALSE;
		for ( horse = 0 ; horse < MAX_HORSE ; horse++ )
		{
		    if ( horse_race_list[ race ].horse[ horse ].position == i && horse_race_list[ race ].horse[ horse ].racing )
		    {
			if ( found )
			    strcat( buf, ", " );
			strcat( buf, horse_race_list[ race ].horse[ horse ].name );
			found = TRUE;
		    }
		}
		if ( found )
		{
		    if ( position == 0 )
		        strcat( buf, " in the lead.'\n\r" );
		    else
		        strcat( buf, ".'\n\r" );
		    if ( position < 3 )
		        send_to_room( horse_race_list[ race ].in_room, buf );
		    position++;
		}
	    }

	   horse_race_list[ race ].timer++;
	   continue;

	}

	if ( horse_race_list[ race ].timer == START_RACE + RACE_REPORTS )
	{
	    for ( horse = 0 ; horse < MAX_HORSE ; horse++ )
		for ( i = 0 ; i < 5 ; i++ )
		    if ( horse_race_list[ race ].horse[ horse ].racing && number_range( 0, horse_race_list[ race ].horse[ horse ].curr_odds ) == 0 )
			horse_race_list[ race ].horse[ horse ].position++;

	    position = 0;
	    for ( i = 5 * ( horse_race_list[ race ].timer - START_RACE ) ; i >= 0 && position == 0 ; i-- )
	    {
		winner = -1;
	        found = FALSE;
		for ( horse = 0 ; horse < MAX_HORSE ; horse++ )
		{
		    if ( horse_race_list[ race ].horse[ horse ].position == i && horse_race_list[ race ].horse[ horse ].racing )
		    {
			position ++;
			if ( !found )
			{
			    winner = horse;
			    sprintf( buf, "The winner is #%2d %s!", horse_race_list[ race  ].horse[ horse ].number, horse_race_list[ race ].horse[ horse ].name );
			}
			else
			{
			    strcat( buf, " by a nose." );
			    break;
			}
		    }
		}
		if ( position && winner >= 0 )
		{
		    strcat( buf, "\n\r");
		    send_to_room( horse_race_list[ race ].in_room, buf );

		    for ( bets = horse_race_list[ race ].bets ; bets != NULL ; bets = next_bet )
		    {
			next_bet = bets->next;
			if ( bets->horse == winner )
			{
			    bets->next = horse_race_list[ race ].pay_offs;
			    horse_race_list[ race ].pay_offs = bets;
 			}
			else
			   free_bet_data(bets);
		    }
		    horse_race_list[ race ].bets = NULL;

		    setup_race( &horse_race_list[ race ] );
	 	}
	    }
	}
*/
	if ( horse_race_list[ race ].timer == START_RACE )
	{
	    horse_race_list[ race ].timer++;
	    display_horses( &horse_race_list[ race ] );
	    send_to_room( horse_race_list[ race ].in_room, "The trumpets sound and they are off.\n\r" );
	    continue;
	}

	if ( horse_race_list[ race ].timer > START_RACE  )
	{
	    position = 57;
	    winner = -1;
	    for ( horse = 0 ; horse < MAX_HORSE ; horse++ )
		for ( i = 0 ; i < 10 ; i++ )
		    if ( horse_race_list[ race ].horse[ horse ].racing && number_range( -60, horse_race_list[ race ].horse[ horse ].curr_odds + 5 ) < 0 )
			{
			    horse_race_list[ race ].horse[ horse ].position++;
			    if ( horse_race_list[ race ].horse[ horse ].position >= position )
			    {
				winner = horse;
				position = horse_race_list[ race ].horse[ horse ].position;
			    }
			}

	    display_horses( &horse_race_list[ race ] );
	    if ( winner != -1 )
	    {
		sprintf( buf, "The track announcer say 'The winner is #%d - %s!'\n\r", horse_race_list[ race ].horse[ winner ].number, horse_race_list[ race ].horse[ winner ].name );
		send_to_room( horse_race_list[ race ].in_room, buf );
	        for ( bets = horse_race_list[ race ].bets ; bets != NULL ; bets = next_bet )
		    {
			next_bet = bets->next;
			if ( bets->horse == winner )
			{
			    bets->next = horse_race_list[ race ].pay_offs;
			    horse_race_list[ race ].pay_offs = bets;
 			}
			else
			   free_bet_data(bets);
		    }
		    horse_race_list[ race ].bets = NULL;
		    setup_race( &horse_race_list[ race ] );
		    horse_race_list[ race ].timer = 0;
		    continue;
	    }
	   horse_race_list[ race ].timer++;
	   continue;
	}

	horse_race_list[ race ].timer = 0;
    }
}

void do_list_horses( CHAR_DATA *ch, char * argument )
{
    int race,horse,i,odd;
    char buf[MAX_INPUT_LENGTH];

    race = -1;
    for ( i = 0; i < MAX_HORSE_RACE ; i++ )
	if ( horse_race_list[ i ].in_room == ch->in_room->vnum )
	    race = i;

    if ( race == -1 )
    {
	send_to_char( "This isn't a race track!.\n\r", ch );
	return;
    }

    if ( *argument != '\0' && is_number( argument ) && !IS_IMMORTAL(ch))
	horse_race_list[ race ].timer = atoi( argument );

    send_to_char( "Number  Horse               Odds\n\r\n\r", ch );
    for ( odd = 1; odd < 100 ; odd++ )
    for ( horse = 0 ; horse < MAX_HORSE ; horse++ )
    {
	if ( !horse_race_list[ race ].horse[ horse ].racing || horse_race_list[ race ].horse[ horse ].curr_odds != odd )
	    continue;

	sprintf( buf, " %3d   %18s %3d : 1\n\r",horse_race_list[ race ].horse[ horse ].number, horse_race_list[ race ].horse[ horse ].name, horse_race_list[ race ].horse[ horse ].curr_odds );
	send_to_char( buf, ch );
    }
}

void setup_race( HORSE_RACE_TYPE * race )
{
    int horse;
    int i,c,odds;

    race->bets = NULL;

    for ( i = 0 ; i < MAX_HORSE ; i++ )
    {
    	race->horse[ i ].racing = FALSE;
    	race->horse[ i ].position = 0;
    }

    for ( i = 0 ; i < 7 ; i++ )
    {
	horse = -1;
	while ( horse == -1 )
	{
	    horse = number_range( 0, MAX_HORSE );
	    if ( race->horse[ horse ].name[0] == '\0' || race->horse[ horse ].racing )
    		horse = -1;
	    else
	    {
 	        odds = number_range( UMIN( 1, race->horse[ horse ].odds / 2 ), 2 * race->horse[ horse ].odds );
 		for ( c = 0; c < MAX_HORSE ; c++ )
		    if ( race->horse[ c ].racing && race->horse[ c ].curr_odds == odds )
		   	odds = 0;
		if ( !odds )
		    horse = -1;
		else
		{
		    race->horse[ horse ].racing = TRUE;
		    race->horse[ horse ].curr_odds = odds;
		}
	    }
	}
    }
}

void do_bet( CHAR_DATA * ch, char * argument )
{
    BET_DATA * bet;
    char arg[MAX_INPUT_LENGTH];
    char buf[MAX_INPUT_LENGTH];
    long amount;
    sh_int coin_type;
    sh_int horse_num,horse;
    int race,i;
    bool found;

    race = -1;
    for ( i = 0; i < MAX_HORSE_RACE ; i++ )
	if ( horse_race_list[ i ].in_room == ch->in_room->vnum )
	    race = i;

    if ( race == -1 )
    {
	send_to_char( "This isn't a race track!.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );    

    if ( arg[0] == '\0' || !is_number( arg ) )
    {
	found = FALSE;
	for ( bet = horse_race_list[ race ].bets ; bet != NULL ; bet = bet->next )
	    if ( bet->ch == ch )
	    {
		if ( !found )
		{
		    send_to_char( "Your bets include: \n\r\n\r", ch );
		    found = TRUE;
		}
		sprintf( buf, "%ld on horse number %d - %s at %d : 1 odds.\n\r", bet->amount,horse_race_list[ race ].horse[ bet->horse ].number, horse_race_list[ race ].horse[ bet->horse ].name, bet->odds );
		send_to_char( buf, ch );
	    }

	if ( !found )
	    send_to_char( "You don't have any bets on this race.\n\r", ch );

	return;
    }

    if ( horse_race_list[ race ].timer > START_RACE - 2 )
    {
	send_to_char( "The betting window is closed, please wait for next race.\n\r", ch );
	return;
    }

    if ( ( amount = atoi( arg ) ) < 1 )
    {
	send_to_char( "Sorry you cannot bet that amount.\n\r", ch );
	return;
    }

    if ( amount > MAX_BET )
    {
	send_to_char( "Sorry you connot bet more then 50,000.\n\r", ch );
	return;
    }

    argument = one_argument( argument, arg );
    if ( ( coin_type = get_coin_type( arg ) ) == -1 )
	coin_type = ch->in_room->area->world;
    else	
        argument = one_argument( argument, arg );

    if ( ch->gold[ coin_type ] < amount )
    {
	sprintf( buf, "You don't have that many %s\n\r", coin_names[ coin_type ] );
	send_to_char( buf, ch );
	return;
    }

    if ( arg[0] == '\0' || !is_number( arg ) )
    {
	send_to_char( "Usage: bet <amount> <horse>\n\r",ch );
	return;
    }

    horse_num = atoi( arg );

    horse = 0;
    found = FALSE;
    for ( i = 0 ; i < MAX_HORSE ; i++ )
    {
	if ( horse_race_list[ race ].horse[ i ].number == horse_num && horse_race_list[ race ].horse[ i ].racing )
	{
	    horse = i;
	    found = TRUE;
	}
    }

    if ( !found )
    {
	send_to_char( "Invalid horse, please select another.\n\r", ch );
	return;
    }

    if ( horse_race_list[ race ].timer == 0 )
       horse_race_list[ race ].timer = 1;

    ch->gold[ coin_type ] -= amount;
    bet = bet_data_alloc();
    bet->ch = ch;
    bet->odds = horse_race_list[ race ].horse[ horse ].curr_odds;
    bet->coin_type = coin_type;
    bet->horse = horse;
    bet->amount = amount;
    bet->next = horse_race_list[ race ].bets;
    horse_race_list[ race ].bets = bet;

    send_to_char("Your bet has been placed, Thank you.\n\r", ch );
}

void do_collect( CHAR_DATA * ch, char *argument )
{
    BET_DATA * bets;
    BET_DATA * next_bet;
    char buf[MAX_INPUT_LENGTH];
    bool found=FALSE;
    int i;

    for ( i = 0; i < MAX_HORSE_RACE ; i++ )
	if ( horse_race_list[ i ].in_room == ch->in_room->vnum )
	   for( bets = horse_race_list[ i ].pay_offs ; bets != NULL ; bets = next_bet )
	   {
		next_bet = bets->next;
		if ( bets->ch == ch && bets->amount > 0 )
		{
		    ch->gold[ bets->coin_type ] += ( ( ( long ) ( bets->odds ) + 1 ) * bets->amount );
		    sprintf( buf, "You get %ld %s.\n\r", ( ( long ) ( bets->odds ) * bets->amount ), coin_names[ bets->coin_type ] );
		    send_to_char( buf, ch );
		    bets->amount = 0;
		    found = TRUE;
		}
	    }

    if ( !found )
	send_to_char( "You do not have any outstanding bets here.\n\r", ch );

}
   
