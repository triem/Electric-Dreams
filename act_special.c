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
#include <string.h>
#include <ctype.h>

void do_beeper (CHAR_DATA *ch, char *argument)
{
   char arg[MAX_STRING_LENGTH];
   CHAR_DATA *victim;
   
   argument = one_argument(argument, arg);
   if (arg[0]=='\0')
       {send_to_char ("Who should i beep???\n\r",ch);
        return;
       }
  if ( (victim = get_char_world(ch,arg)) == NULL)
       { send_to_char ("You can't beep someone who isn't here.",ch);
         return;
       }
  if ( !can_see(ch, victim) ) 
       { send_to_char ("You can't beep someone who isn't here.",ch);
         return;
       }

  cact ("\a\a\a\a$n is beeping you!!",ch,NULL,victim,TO_VICT,"`r");
  cact ("$n has been beeped.",victim,NULL,ch,TO_VICT,"`W");
}
void do_balance (CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];

   if( ch->in_room->area->world != race_table[ ch->race ][ ch->sex ].world )
   {
	send_to_char( "This isn't your branch.\n\r", ch );
	return;
   }

   if (ch->bank == 1) 
         { sprintf (buf,"Our records show you have only 1 coin in our vaults\n\r.");
         }
   else
        { sprintf (buf,"Our records show you have %ld coins in our vault\n\r.",ch->bank);
        }
   send_to_char("`g",ch);
   send_to_char(buf,ch);
   send_to_char("`w",ch);
}

void do_deposit (CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   int coins;
   char arg[MAX_STRING_LENGTH];
   int coin_type;

   if( ch->in_room->area->world != race_table[ ch->race ][ ch->sex ].world )
   {
	send_to_char( "This isn't your branch.\n\r", ch );
	return;
   }

   argument = one_argument(argument,arg);
   if (arg[0]=='\0' || arg[0] =='-')
   { 
	send_to_char ("Deposit how much?\n\r",ch);
        return;
   }

   coins = atoi(arg);
   argument = one_argument(argument,arg);
   if ( ( coin_type = get_coin_type( arg ) ) < 0 )
       coin_type = race_table[ ch->race ][ ch->sex ].world;

   if (coins > 5000000) 
   {
	send_to_char("Max deposit of 5 million coins.\n\r",ch); 
	return;
   } 

   if (coins > ch->gold[ coin_type ]) 
   {
	send_to_char ("You can't deposit more then you have!\n\r",ch); 
	return;
   } 

   if (coins > 0) 
   {
         ch->gold[ coin_type ] -= coins;
         ch->bank += bank_exchange_rate[ coin_type ][ race_table[ ch->race ][ ch->sex ].world ] * coins / 100;
         sprintf (buf,"The teller looks up, smiles and says to you, 'Thank you for depositing\n\r%i coins into our fine bank. Come again.'",coins);
         cact (buf,ch,NULL,NULL,TO_CHAR,"`g");
         cact ("$n deposits some coins into $s Super Saver account.",ch,NULL,NULL,TO_ROOM,"`g");
         sprintf (buf,"%s deposited %d coins of type %d.",ch->name,coins,coin_type);
         log_string (buf); 
   }
}

void do_exchange (CHAR_DATA *ch, char *argument)
{
   char buf[MAX_STRING_LENGTH];
   int coins;
   char arg[MAX_STRING_LENGTH];
   int coin_type;

   argument = one_argument(argument,arg);
   if (arg[0]=='\0' || arg[0] =='-')
       { send_to_char ("Exchange how much?\n\r",ch);
         return;
       }
   coins = atoi(arg);
   argument = one_argument(argument,arg);
   if ( ( coin_type = get_coin_type( arg ) ) < 0 )
      coin_type = race_table[ ch->race ][ ch->sex ].world; 

   if (coins > 5000000) {send_to_char("Max exchange of 5 million coins.\n\r",ch); return;} 
   if (coins > ch->gold[ coin_type ]) {send_to_char ("You can't exchange more then you have!\n\r",ch); return;}; 
   if (coins > 0) 
       {
         ch->gold[ coin_type ] -= coins;
         ch->gold[ ch->in_room->area->world ] += bank_exchange_rate[ coin_type ] [ ch->in_room->area->world ] * coins / 100;
	 sprintf( buf,"You exchanges your %d %s for %d %s", coins, coin_names[ coin_type ], bank_exchange_rate[ coin_type ] [ ch->in_room->area->world ] * coins / 100, coin_names[ ch->in_room->area->world ]);
         cact (buf,ch,NULL,NULL,TO_CHAR,"`g");
         cact ("The Teller says with a smile 'Thank You! and have a nice day.'\n\r",ch,NULL,NULL,TO_CHAR,"`g");
        }
      
}
void do_withdraw (CHAR_DATA *ch, char *argument)
{ 
  char buf[MAX_STRING_LENGTH],arg[MAX_STRING_LENGTH];
  int coins=0;
  int coin_type;

   if( ch->in_room->area->world != race_table[ ch->race ][ ch->sex ].world )
   {
	send_to_char( "This isn't your branch.\n\r", ch );
	return;
   }

  argument = one_argument(argument,arg);
  if (arg[0]=='\0' || arg[0]=='-')
     { send_to_char ("Withdraw how mutch?\n\r",ch);
       return;
     }
  coin_type = race_table[ ch->race ][ ch->sex ].world; 
  coins = atoi(arg);
  if (coins < 0) return;
  if (coins > 5000000) {send_to_char ("Maximum withdrawl is 5 million coins\n\r",ch); return;} 
  if (coins > ch->bank)
     {sprintf (buf,"The teller examines your withdrawl request and laughs.\n\rThe teller says, 'You wish you had this much money.'");
      cact(buf,ch,NULL,NULL,TO_CHAR,"`g");
      cact("The teller informs $n that $e has insufficient funds.",ch,NULL,NULL,TO_ROOM,"`g");
      return;
     }
  ch->gold[ coin_type ] += coins;
  ch->bank -= coins;
  sprintf (buf,"The teller examines your withdrawl request and reaches down into\n\rher cash box, pulling out your %i coins.\n\rThe teller looks up and says, 'Thanks for doing business with our bank!'",coins);
  cact (buf,ch,NULL,NULL,TO_CHAR,"`g");
  cact ("$n withdraws some money from $s Super Saver account.",ch,NULL,NULL,TO_ROOM,"`g");
}
void do_cointrans (CHAR_DATA *ch, char *argument)
{ char buf[MAX_STRING_LENGTH],arg[MAX_STRING_LENGTH],arg2[MAX_STRING_LENGTH];
  int coins;
  CHAR_DATA *victim;

   if( ch->in_room->area->world != race_table[ ch->race ][ ch->sex ].world )
   {
	send_to_char( "This isn't your branch.\n\r", ch );
	return;
   }

  argument = one_argument (argument,arg);
  argument = one_argument (argument,arg2);
  if (isdigit(arg[0]) && isalpha(arg2[0])) 
      {
        coins =  atoi(arg);
        if (coins < 0) return;
        if ((victim = get_char_world (ch, arg2)) == NULL)
        { send_to_char ("The recipient must be online.\n\r",ch);
          return;
        }
     }
  else if (isdigit(arg2[0]) && isalpha(arg[0]))
       {
         coins = atoi(arg2);
         if ((victim = get_char_world (ch,arg)) == NULL)
         { send_to_char ("The recipient must be online.\n\r",ch);
           return;
         }
       }
       else
          {send_to_char ("You must supply a name and a value. Any order.",ch);
           return; 
          }
  if (ch->bank < coins)
       { cact("The Teller reviews your request for a transfer and laughs.\n\rThe teller looks up to you and says, 'You don't have that much gold.'",ch,NULL,NULL,TO_CHAR,"`g");
         cact("The teller informs $n that $e has insufficient funds.",ch,NULL,ch,TO_ROOM,"`g");
         return;
       }
  ch->bank -= coins;
  victim->bank += bank_exchange_rate[ race_table[ ch->race ][ ch->sex ].world ] [ race_table[ victim->race ][ victim->sex ].world ] * coins / 100; 
  sprintf(buf,"\a[%d] coins have been transferred form the account of [%s] to the\n\raccount of [%s]",coins,ch->name,victim->name);
  cact(buf,ch,NULL,NULL,TO_CHAR,"`g");
  cact(buf,victim,NULL,NULL,TO_CHAR,"`g"); 
  log_string(buf);
  cact ("You see $n transfer some funds to $N.",ch,NULL,victim,TO_ROOM,"`g");
} 
            
void do_bankset (CHAR_DATA *ch, char *argument)
{ CHAR_DATA *victim;
  long coins;
  char arg[MAX_STRING_LENGTH], arg2[MAX_STRING_LENGTH];
  argument = one_argument (argument, arg);
  argument = one_argument (argument, arg2);
 
  if (IS_OPTIVISION(ch) ) 
  {
        if (( victim = get_char_world(ch,arg)) == NULL)
  	   { send_to_char ("They are not here.\n\r",ch); return;}
  	coins = atoi(arg2);
  	victim->bank = coins;
  	return;
  }
   send_to_char ("Huh?\n\r",ch); return;
}
