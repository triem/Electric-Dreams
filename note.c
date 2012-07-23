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

bool is_read_note( CHAR_DATA *ch, NOTE_DATA *pnote )
{
   READ_NOTES * note_list_data;

   if ( !str_cmp( ch->name, pnote->sender ) )
	return TRUE;

   if ( ch->pcdata == NULL )
	return FALSE;

    for ( note_list_data = ch->pcdata->read_notes ; note_list_data != NULL ; note_list_data = note_list_data->next )
	if ( note_list_data->date_stamp == pnote->date_stamp )
	    return TRUE;

    return FALSE;
}

bool is_note_to( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    char buf[MAX_INPUT_LENGTH];

    if ( !str_cmp( ch->name, pnote->sender ) )
	return TRUE;

    if ( is_name( "all", pnote->to_list ) )
	return TRUE;

    if ( IS_A_BUILDER(ch) && ( is_name( "builder", pnote->to_list ) 
		|| is_name( "builders", pnote->to_list ) ) )
	return TRUE;

   if ( IS_IMP(ch) && ( is_name( "imp", pnote->to_list )
		|| is_name( "imps", pnote->to_list ) ) )
	return TRUE;

   if ( IS_IMMORTAL(ch) && ( is_name( "immortal", pnote->to_list )
		|| is_name( "immortals", pnote->to_list ) 
		|| is_name( "immorts", pnote->to_list ) 
		|| is_name( "immort", pnote->to_list ) ) )
	return TRUE;

    if ( ch->pcdata->pcclan != NULL )
    {
	sprintf( buf,"'%s'",ch->pcdata->pcclan->clan->title);
	if ( is_name( buf, pnote->to_list ) ) 
	    return TRUE;
    }

    if ( is_name( ch->name, pnote->to_list ) )
	return TRUE;

    return FALSE;
}



void note_attach( CHAR_DATA *ch )
{
    NOTE_DATA *pnote;

    if ( ch->pnote != NULL )
	return;

    pnote = note_data_alloc();

    pnote->next		= NULL;
    strncpy( pnote->sender, ch->name, 30 );
    pnote->date[0]	= '\0';
    pnote->to_list[0]	= '\0';
    pnote->subject[0]	= '\0';
    pnote->text		= &str_empty[0];
    pnote->flags	= 0;
    pnote->read_list[0]	= '\0';
    ch->pnote		= pnote;
    return;
}
void note_remove( CHAR_DATA *ch, NOTE_DATA *pnote )
{
    char to_new[MAX_INPUT_LENGTH];
    char to_one[MAX_INPUT_LENGTH];
    NOTE_DATA *prev;
    char *to_list;

    /*
     * Build a new to_list.
     * Strip out this recipient.
     */
    to_new[0]	= '\0';
    to_list	= pnote->to_list;
    while ( *to_list != '\0' )
    {
	to_list	= one_argument( to_list, to_one );
	if ( to_one[0] != '\0' && str_cmp( ch->name, to_one ) )
	{
	    strcat( to_new, " " );
	    strcat( to_new, to_one );
	}
    }

    /*
     * Just a simple recipient removal?
     */
    if ( str_cmp( ch->name, pnote->sender ) && to_new[0] != '\0' )
    {
	strncpy( pnote->to_list, to_new + 1, 80 );
	return;
    }

    /*
     * Remove note from linked list.
     */
    if ( pnote == note_list )
    {
	note_list = pnote->next;
    }
    else
    {
	for ( prev = note_list; prev != NULL; prev = prev->next )
	{
	    if ( prev->next == pnote )
		break;
	}

	if ( prev == NULL )
	{
	    bug( "Note_remove: pnote not found.", 0 );
	    return;
	}

	prev->next = pnote->next;
    }

    pnote->next	= note_free;
    note_free	= pnote;

    save_notes();
    return;
}

void save_notes( void )
{
    NOTE_DATA * pnote;
    FILE * fp;

    if ( ( fp = fopen( NOTE_FILE, "w" ) ) == NULL )
    {
	perror( NOTE_FILE );
    }
    else
    {
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    fprintf( fp, "Sender  %s~\n", pnote->sender);
	    fprintf( fp, "Date    %s~\n", pnote->date);
	    fprintf( fp, "Stamp   %d\n",  (int) pnote->date_stamp);
	    fprintf( fp, "To      %s~\n", pnote->to_list);
	    fprintf( fp, "Subject %s~\n", pnote->subject);
	    fprintf( fp, "Text\n%s~\n",   pnote->text);
	}
	if ( fp )
	fclose( fp );
    }
    return;
}

void note_add( NOTE_DATA *pnote )
{
    NOTE_DATA * note;
    NOTE_DATA * note_next;

    if ( note_list == NULL )
    {
        note_list = pnote;
	return;
    }

    if ( note_list->date_stamp > pnote->date_stamp )
    {
	pnote->next = note_list;
	note_list = pnote;
	return;
    }

    for ( note = note_list; note != NULL ; note = note_next )
    {
	note_next = note->next;

	if ( note_next == NULL || note_next->date_stamp > pnote->date_stamp )
	{
	    note->next = pnote;
	    pnote->next = note_next;
	    return;
	}
    }
}

/* used by imps to nuke a note for good */
void note_delete( NOTE_DATA *pnote )
{
    FILE *fp;
    NOTE_DATA *prev;
 
    /*
     * Remove note from linked list.
     */
    if ( pnote == note_list )
    {
        note_list = pnote->next;
    }
    else
    {
        for ( prev = note_list; prev != NULL; prev = prev->next )
        {
            if ( prev->next == pnote )
                break;
        }
 
        if ( prev == NULL )
        {
            bug( "Note_delete: pnote not found.", 0 );
            return;
        }
 
        prev->next = pnote->next;
    }
 
    pnote->next = note_free;
    note_free   = pnote;
 
    /*
     * Rewrite entire list.
     */
    if ( ( fp = fopen( NOTE_FILE, "w" ) ) == NULL )
    {
        perror( NOTE_FILE );
    }
    else
    {
        for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
        {
            fprintf( fp, "Sender  %s~\n", pnote->sender);
            fprintf( fp, "Date    %s~\n", pnote->date);
            fprintf( fp, "Stamp   %d\n",  (int) pnote->date_stamp);
            fprintf( fp, "To      %s~\n", pnote->to_list);
            fprintf( fp, "Subject %s~\n", pnote->subject);
            fprintf( fp, "Text\n%s~\n",   pnote->text);
        }
	if ( fp )
        fclose( fp );
    }
    return;
}

char * remove_re_subject( char * subject )
{
    char * string;

    if ( *subject == '\0' )
	{
	return subject;
	}

    string = subject;
    for (  ; ; string++ )
    {
	for ( ; *string == ' '; string++ );
  	if ( !strncmp("Re:",string,3) || !strncmp("re:",string,3) )
	{
	    string++;
	    string++;
	    string++;
	}
	else
	    return string;
	if ( *string == '\0' )
	{	
	    return " ";
	}
    }
}

void do_note( CHAR_DATA *ch, char *argument )
{
    DESCRIPTOR_DATA *d;
    char buf[MAX_BUF];
    char arg[MAX_INPUT_LENGTH];
    char arg2[MAX_INPUT_LENGTH];
    NOTE_DATA *pnote;
    READ_NOTES * read_notes;
    int vnum;
    int anum;
    int flag;
    buf[0] = '\0';
    if ( IS_NPC(ch) )
	return;

    argument = one_argument( argument, arg );

    if (arg[0] == '\0')
    {
	do_note(ch,"read");
	return;
    }

    if ( ( flag = get_note_flag( arg ) ) != 0 && IS_IMMORTAL( ch ) )
    {
	note_attach( ch );
	TOGGLE_BIT( ch->pnote->flags, flag );
	send_to_char( "Note flag toggled.\n\r", ch );
	return;
    }


    if ( !str_prefix( arg, "list" ) )
    {
 	bool found = FALSE;
	bool fUnread = FALSE;
        BUFFER *output;
        output = new_buf();
	vnum = 0;
	argument = one_argument(argument,arg2);
	if ( !str_cmp( arg2, "unread" ) )
	{
	    argument = one_argument(argument,arg2);
	    fUnread = TRUE;
	}
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) )
	      {
		if ( ( arg2[0]=='\0' || is_name(arg2,pnote->sender) || is_name(arg2,pnote->to_list) || !str_cmp( arg, "all" ) )
		  && ( !fUnread || !is_read_note( ch, pnote ) ) )
	       {
		found = TRUE;
		sprintf( buf, "[%3d%s]  [%10s]  [ %s ]\n\r",
		    vnum, 
		    ( fUnread || !is_read_note( ch, pnote ) ) ? "N" : " ",
		     pnote->sender,
                   pnote->subject );
		add_buf( output, buf );
	       }
	      vnum++;
	    }
	}
	if ( !found && fUnread )
	    send_to_char( "No unread notes.\n\r", ch );
	else if ( !found && arg2[0] != '\0' )
	    send_to_char( "No notes from them.\n\r", ch );
	else
	    send_to_char( "No notes to list.\n\r", ch );

	page_to_char(buf_string(output),ch);
        free_buf(output);
	return;
    }

    if ( !str_prefix( arg, "create" ) || !str_prefix( arg, "enter" ) )
    {
	send_to_char("`Y", ch);
	ch->desc->connected = CON_EDITING;
	if ( ch->pnote == NULL )
	note_attach( ch );
	string_append(ch, &ch->pnote->text);
	return;
    }
    if ( !str_prefix( arg, "read" ) )
    {
	bool fAll=FALSE;
        BUFFER *output;
        output = new_buf();

	if ( !str_cmp( argument, "all" ) )
	{
	    fAll = TRUE;
	    anum = 0;
	}
	
	else if ( argument[0] == '\0' || !str_prefix(argument, "next"))
	/* read next unread note */
	{
	    vnum = 0;
	    for ( pnote = note_list; pnote != NULL; pnote = pnote->next) 
	    {
		if (is_note_to(ch,pnote) && str_cmp(ch->name,pnote->sender)
		&& !is_read_note( ch, pnote ) )
            	{
                    sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
                    	vnum,
                    	pnote->sender,
                    	pnote->subject,
                    	pnote->date,
                    	pnote->to_list);
                    send_to_char( buf, ch );
		if ( IS_SET( pnote->flags, NOTE_RECIEPT )
		  && !fAll
		  && !is_name( ch->name, pnote->read_list ) )
		{
		    sprintf( buf, "%s %s", pnote->read_list, ch->name );
		    strncpy( pnote->read_list, buf, 80 );
		    save_notes();
		}
		    if ( !str_cmp( pnote->sender, ch->name ) 
			&& pnote->flags != 0 
			&& IS_IMMORTAL( ch ) )
		    {
			sprintf( buf, "Flags: %s\n\r", note_flag_name( pnote->flags ) );
			send_to_char( buf, ch );
			if ( IS_SET( pnote->flags, NOTE_RECIEPT ) )
			{
			    sprintf( buf, "Read by: %s\n\r", pnote->read_list );
			    send_to_char( buf, ch );
			}
		    }
		    add_buf( output, pnote->text );
		    page_to_char(buf_string(output),ch);
		    read_notes = read_notes_alloc();
		    read_notes->date_stamp = pnote->date_stamp;
		    read_notes->next = ch->pcdata->read_notes;
		    ch->pcdata->read_notes = read_notes;
        	    free_buf(output);
                    return;
		}
		else if (is_note_to(ch,pnote))
		    vnum++;
            }
	    send_to_char("You have no unread notes.\n\r",ch);	
            free_buf(output);
	    return;
	}

	else if ( is_number( argument ) )
	{
	    fAll = FALSE;
	    anum = atoi( argument );
	}
	else
	{
	    send_to_char( "Note read which number?\n\r", ch );
            free_buf(output);
	    return;
		}

		vnum = 0;
		for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
		{
		    if ( is_note_to( ch, pnote ) && ( vnum++ == anum || fAll ) )
		    {
			sprintf( buf, "[%3d] %s: %s\n\r%s\n\rTo: %s\n\r",
			    vnum - 1,
			    pnote->sender,
			    pnote->subject,
			    pnote->date,
			    pnote->to_list
			    );
			send_to_char( buf, ch );
			if ( IS_SET( pnote->flags, NOTE_RECIEPT )
			  && !fAll
			  && !is_name( ch->name, pnote->read_list ) )
			{
			    sprintf( buf, "%s %s", pnote->read_list, ch->name );
			    strncpy( pnote->read_list, buf, 80 );
			    save_notes( );
			}
			    if ( !str_cmp( pnote->sender, ch->name ) 
				&& pnote->flags != 0 
				&& IS_IMMORTAL( ch ) )
			    {
				sprintf( buf, "Flags: %s\n\r", note_flag_name( pnote->flags ) );
				send_to_char( buf, ch );
				if ( IS_SET( pnote->flags, NOTE_RECIEPT ) )
				{
				    sprintf( buf, "Read by: %s\n\r", pnote->read_list );
				    send_to_char( buf, ch );
				}
			    }
			add_buf( output, pnote->text );
			page_to_char(buf_string(output),ch);
			if ( !is_read_note( ch, pnote ) )
			{
			    read_notes = read_notes_alloc();
			    read_notes->date_stamp = pnote->date_stamp;
			    read_notes->next = ch->pcdata->read_notes;
		    ch->pcdata->read_notes = read_notes;
		}
        	free_buf(output);
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
        free_buf(output);
	return;
    }

    if ( !str_prefix( arg, "reply" ) )
    {

	if ( is_number( argument ) )
	{
	    anum = atoi( argument );
	}
	else
	{
	    send_to_char( "Note reply which number?\n\r", ch );
	    return;
	}

	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) && ( vnum++ == anum ) )
	    {
		note_attach( ch );
		buf[0] = '\0';
		strcpy( buf, "Re: " );
		strcat( buf, remove_re_subject( pnote->subject ) );
		strncpy( ch->pnote->subject, buf, 30 );
		buf[0] = '\0';
		strcpy( buf , cut_name( ch->name, pnote->to_list ) );
		strcat( buf, pnote->sender );
		strncpy( ch->pnote->to_list, buf, 80 );
		sprintf( buf, "Replying to note %d\n\r", vnum - 1 );
		send_to_char( buf, ch );
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
	return;
    }


    if ( !str_prefix( arg, "edit" ) )
    {

	if ( is_number( argument ) )
	{
	    anum = atoi( argument );
	}
	else
	{
	    send_to_char( "Edit which note number?\n\r", ch );
	    return;
	}

	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) && ( vnum++ == anum ) )
	    {
		if ( str_cmp( ch->name, pnote->sender ) && !IS_OPTIVISION(ch) )
		{
		    send_to_char( "You didn't create that note.\n\r", ch );
		    return;
		}
		note_attach( ch );
		strncpy( ch->pnote->subject, pnote->subject, 30 );
		strncpy( ch->pnote->to_list, pnote->to_list, 80 );
		strncpy( ch->pnote->sender, pnote->sender, 30 );
		free_string( ch->pnote->text );
		ch->pnote->text = str_dup( pnote->text );
		note_delete( pnote );
		sprintf( buf, "Editing note %d\n\r", vnum - 1 );
		send_to_char( buf, ch );
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
	return;
    }


    if ( !str_cmp( arg, "+" ) )
    {
	note_attach( ch );
	strcpy( buf, ch->pnote->text );
	if ( strlen(buf) + strlen(argument) >= MAX_BUF - 200 )
	{
	    send_to_char( "Note too long.\n\r", ch );
	    return;
	}

	strcat( buf, argument );
	strcat( buf, "\n\r" );
	free_string( ch->pnote->text );
	ch->pnote->text = str_dup( buf );
	send_to_char( "Ok.\n\r", ch );
	return;
    }
    if ( !str_cmp( arg, "-" ) )
    {
	note_attach( ch );
	strcpy( buf, ch->pnote->text );
	if ( buf[0] == '\0' )
	{
	    send_to_char( "Note has not been started.\n\r", ch );
	    return;
	}

 	for ( anum=strlen(buf) ; buf[anum]!='\r' && anum>0 ; anum-- );
	if ( anum == 0 )
	{
	    send_to_char( "No previous line to delete.\n\r", ch );
	    return;
	}
 	for ( anum = ( anum ? anum-1 : 0 ) ; buf[anum]!='\r' && anum>0 ; anum-- );

	buf[ anum ? anum+1 : 0 ] = '\0';

	free_string( ch->pnote->text );
	ch->pnote->text = str_dup( buf );
	send_to_char( "Ok.\n\r", ch );
	return;
    }
    if ( !str_prefix( arg, "subject" ) )
    {
	note_attach( ch );
	strncpy( ch->pnote->subject, argument, 30 );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "to" ) )
    {
	note_attach( ch );
	if ( is_name( "clan", argument ) && ch->pcdata->pcclan != NULL )
	{
	    strcpy( buf, cut_name("clan",argument));
	    strcat( buf, " '");
	    strcat( buf, ch->pcdata->pcclan->clan->title);
	    strcat( buf, "'");
	    strncpy( ch->pnote->to_list, buf, 80 );
	}
	else
	    strncpy( ch->pnote->to_list, argument, 80 );
	send_to_char( "Ok.\n\r", ch );
	return;
    }
    if ( !str_prefix( arg, "from" ) && IS_OPTIVISION(ch) )
    {
	note_attach( ch );
	strncpy( ch->pnote->sender, argument, 30 );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "clear" ) )
    {
	if ( ch->pnote != NULL )
	{
	    ch->pnote->subject[0]	= '\0';
	    ch->pnote->to_list[0]	= '\0';
	    ch->pnote->date[0]		= '\0';
	    ch->pnote->sender[0]	= '\0';
	    ch->pnote->next	= note_free;
	    note_free		= ch->pnote;
	    ch->pnote		= NULL;
	}

	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "show" ) )
    {
	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}

	sprintf( buf, "%s: %s\n\rTo: %s\n\r",
	    ch->pnote->sender,
	    ch->pnote->subject,
	    ch->pnote->to_list
	    );
	send_to_char( buf, ch );
	send_to_char( ch->pnote->text, ch );
	return;
    }

    if ( !str_prefix( arg, "post" ) || !str_prefix(arg, "send"))
    {
	char *strtime;

	if ( ch->pnote == NULL )
	{
	    send_to_char( "You have no note in progress.\n\r", ch );
	    return;
	}

	if (!str_cmp(ch->pnote->to_list,""))
	{
	    send_to_char(
		"You need to provide a recipient (name, all, or immortal).\n\r",
		ch);
	    return;
	}

	if (!str_cmp(ch->pnote->subject,""))
	{
	    send_to_char("You need to provide a subject.\n\r",ch);
	    return;
	}
        for ( d = descriptor_list; d != NULL; d = d->next )
        {

	     if ( d->connected <= CON_PLAYING &&
	          d->character != ch &&
		  is_note_to( d->character, ch->pnote )
		  && can_see(d->character,ch ))
	    {
	        sprintf( buf, "A mailman comes up to and tells you you have a new note from %s.\n\r",ch->name);
		send_to_char( buf, d->character );
	    }
	}

	ch->pnote->next			= NULL;
	strtime				= ctime( &current_time );
	strtime[strlen(strtime)-1]	= '\0';
	strncpy( ch->pnote->date, strtime, 30 );
	ch->pnote->date_stamp		= current_time;

	note_add( ch->pnote );
	ch->pnote	= NULL;
	save_notes( );
	send_to_char( "Ok.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "remove" ) )
    {
	if ( !is_number( argument ) )
	{
	    send_to_char( "Note remove which number?\n\r", ch );
	    return;
	}

	anum = atoi( argument );
	vnum = 0;
	for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
	{
	    if ( is_note_to( ch, pnote ) && vnum++ == anum )
	    {
		note_remove( ch, pnote );
		send_to_char( "Ok.\n\r", ch );
		return;
	    }
	}

	send_to_char( "No such note.\n\r", ch );
	return;
    }

    if ( !str_prefix( arg, "delete" ) && IS_OPTIVISION(ch))
    {
        if ( !is_number( argument ) )
        {
            send_to_char( "Note delete which number?\n\r", ch );
            return;
        }
 
        anum = atoi( argument );
        vnum = 0;
        for ( pnote = note_list; pnote != NULL; pnote = pnote->next )
        {
            if ( is_note_to( ch, pnote ) && vnum++ == anum )
            {
                note_delete( pnote );
                send_to_char( "Ok.\n\r", ch );
                return;
            }
        }
 
        send_to_char( "No such note.\n\r", ch );
        return;
    }

    send_to_char( "Huh?  Type 'help note' for usage.\n\r", ch );
    return;
}
