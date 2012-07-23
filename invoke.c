void rod_of_revenge (CHAR_DATA *ch,char *arg2) 
{ 
  OBJ_DATA *obj;
  CHAR_DATA *victim;
  char buf[MAX_STRING_LENGTH]; 
  obj = get_eq_char(ch,WEAR_WIELD);
  if ( obj == NULL )
  obj = get_eq_char(ch,WEAR_WIELD_TWO_HANDED);

  if (obj != NULL) {
  if (obj->pIndexData->vnum != OBJ_VNUM_RODOFREV)
  { 
  send_to_char ("You fail to invoke the powers of the rod of revenge",ch);
           return;
         }
         if (((victim = get_char_world (ch, arg2)) == NULL))
         {
          act(arg2,ch,NULL,ch,TO_ROOM); 
          act ("A spectral image of $n appears and bonks him on his head, then vanishes.",ch,NULL,ch,TO_ROOM);
          send_to_char("A spectral image of yourself appears before you.\n\r",ch);
          send_to_char("The image says, You Dolt! They are not Here!.\n\r",ch);
          send_to_char("A spectral image bonks you on your head and promptly vanishes.\n\r",ch);
          return;
         }
         if (((victim = get_char_world (ch, arg2)) == NULL) || (IS_NPC(victim)))
         {
          act(arg2,ch,NULL,ch,TO_ROOM); 
          act ("A spectral image of $n appears and bonks him on his head, then vanishes.",ch,NULL,ch,TO_ROOM);
          send_to_char("A spectral image of yourself appears before you.\n\r",ch);
	  sprintf(buf,"The image says, You Dolt! The Rod of Revenge will not work on the %s!\n\r",arg2);
	  send_to_char(buf,ch);
          send_to_char("A spectral image bonks you on your head and promptly vanishes.\n\r",ch);
          return;
         }
         if (victim==ch)
         {send_to_char("You can't seek revenge against yourself.\n\r",ch);
 	  return;} 
         if ((victim->level < ch->level) || (victim->level >= LEVEL_IMMORTAL))
         {
          act("A spectral image of $n appears before you.",ch,NULL,victim,TO_VICT);
          act("The image speaks to you. Only you can see me. Shh.. don\'t say a thing.",victim,NULL,NULL,TO_CHAR); 
          act("Beware, $n has tried to destroy you. You may need my power to defend yourself.",ch,NULL,victim,TO_VICT);
          send_to_char("A spectral image of yourself appears before you.\n\rYou have tried to dishonor me. For that you must pay.\n\r",ch);
          act("A spectral image of $n appears in the room.\n\rThe image speaks to $n.\n\rThe Image\'s Slach DOES UNSPEAKABLE things to $n!\n\rThe Image vanishes.",ch,NULL,ch,TO_ROOM);
          act("The spectral image DOES UNSPEAKABLE THINGS TO YOU!!!\n\rTHAT HURT!!\n\rThe rod vanishes form your inventory and so does the Image.",ch,NULL,NULL,TO_CHAR);
          obj_from_char( obj, "U" );
          obj_to_char( obj, victim);       
          ch->hit -= ch->level*6; 
          send_to_char("You hear a faint voice in your head,\'Use me wisely,\n\relse the same fate will befall you.",victim);
          return;
         }
          act("An eerie yellow light surrounds $N",victim,NULL,victim,TO_ROOM);
          act("$N begins to act like $E is being attacked.",victim,NULL,victim,TO_ROOM);
          act("$N drops dead before your eyes! The aura fades from $S corpse.",victim,NULL,victim,TO_ROOM); 
          act("You begin to glow with an eerie yellow light.\n\rOuch!\n\rSomeone is Killing You!\n\rPANIC! You can\'t see them! You can\'t Hit them!",victim,NULL,NULL,TO_CHAR);
          act("YOU HAVE BEEN KILLED!!!!!",victim,NULL,NULL,TO_CHAR);
          act("The Rod of Revenge Jumps out of your hand!\n\rIt is surrounded by a yellowish light.",ch,NULL,NULL,TO_CHAR);
          obj_from_char(obj, "V");
          act("As the light brightens, the rod fades from existence.\n\r The rod is completely gone.\n\rThe light disappears.",ch,NULL,NULL,TO_CHAR);
          send_to_char("You hear a whisper from within your head,\'Their life is gone now.\n\r    With that, so must I go.\'\n\r",ch);
          raw_kill(victim,TRUE);
          victim->hit = 1;victim->exp -= 500; 
	  obj_to_char(obj,victim);
          send_to_char("A rod fades into reality inside your inventory\n\r",victim);
          send_to_char("The staff seems to whisper into your mind, I am the staff of revenge,\n\ruse me wisely.",victim);
          return;       
             
    } 
  send_to_char ("You do not seem to be wielding the Rod of Revenge",ch);
           return;
}
