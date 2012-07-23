/***************************************************************************
 *  File: string.c                                                         *
 *  Usage: various functions for new NOTE system                           *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 *                                                                         *
 *  Much of this code came from CircleMUD.                                 *
 *  CircleMUD is based on DikuMUD, Copyright (C) 1990, 1991.               *
 *                                                                         *
 *  Revised for Merc 2.1 by Jason Dinkel.                                  *
 *  Revised for Envy 1.0 by Jason Dinkel.				   *
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


void string_edit( CHAR_DATA *ch, char **pString )
{
    send_to_char( "-=======================================-\n\r", ch );
    send_to_char( " Entering line editing mode.\n\r", ch );
    send_to_char( " Terminate with a ~ or @ on a blank line.\n\r", ch );
    send_to_char( "-=======================================-\n\r", ch );

    if ( *pString == NULL )
    {
        *pString = &str_empty[0];
    }
    else
    {
        **pString = '\0';
    }

    ch->desc->pString = pString;

    return;
}
void do_rot_string( CHAR_DATA *ch, char *argument )
{
  if ( argument[0] == '\0' )
	return;
  send_to_char(rot_string(argument), ch);
}
char * rot_string( char * arg )
{
    static char buf[MAX_STRING_LENGTH];
    int i=0;

    while ( arg[ i ] != '\0' )
    {
	buf[ i ] = 'a' + ( 13 + LOWER( arg[ i ] ) - 'a' ) % 26;
	i++;
    }

    buf[ i ] = '\0';

    return buf;
}

int str_len( char * astr )
{
    int i=0;

    for ( i = 0 ; *astr != '\0' ; i++ ) astr++;

    return i;
}


void string_append( CHAR_DATA *ch, char **pString )
{
    send_to_char( "-=======- Entering APPEND Mode -========-\n\r", ch );
    send_to_char( "    Type .h on a new line for help\n\r", ch );
    send_to_char( " Terminate with a ~ or @ on a blank line.\n\r", ch );
    send_to_char( "-=======================================-\n\r", ch );

    if ( *pString == NULL )
    {
        *pString = &str_empty[0];
    }
    send_to_char( *pString, ch );
    
    if ( *(*pString + strlen( *pString ) - 1) != '\r' )
    send_to_char( "\n\r", ch );

    ch->desc->pString = pString;

    return;
}

void string_append_2( CHAR_DATA *ch, char **pString )
{
    send_to_char( "-=================- Entering APPEND Mode -================-\n\r", ch );
    send_to_char( "         Type .h on a new line for help\n\r", ch );
    send_to_char( "    Terminate with a ~ or @ on a blank line.\n\r", ch );
    send_to_char( " Once you have terminated the editor type 'c' to continue.\n\r", ch );
    send_to_char( "-=========================================================-\n\r", ch );

    if ( *pString == NULL )
    {
        *pString = &str_empty[0];
    }
    send_to_char( *pString, ch );
    
    if ( *(*pString + strlen( *pString ) - 1) != '\r' )
    send_to_char( "\n\r", ch );

    ch->desc->pString = pString;

    return;
}




char * string_replace( char * orig, char * old, char * newstr )
{
    char xbuf[MAX_BUF];
    int i;

    xbuf[0] = '\0';
    strcpy( xbuf, orig );
    if ( strstr( orig, old ) != NULL )
    {
        i = strlen( orig ) - strlen( strstr( orig, old ) );
        xbuf[i] = '\0';
        strcat( xbuf, newstr );
        strcat( xbuf, &orig[i+strlen( old )] );
    }

    return str_dup( xbuf );
}



void string_add( CHAR_DATA *ch, char *argument )
{
    char buf[MAX_BUF];
    int anum;

    if ( *argument == '.' )
    {
        char arg1 [MAX_INPUT_LENGTH];
        char arg2 [MAX_INPUT_LENGTH];
        char arg3 [MAX_INPUT_LENGTH];

        argument = one_argument( argument, arg1 );
        argument = first_arg( argument, arg2, FALSE );
        argument = first_arg( argument, arg3, FALSE );

        if ( !str_cmp( arg1, ".c" ) )
        {
            send_to_char( "String cleared.\n\r", ch );
            *ch->desc->pString = &str_empty[0];
            return;
        }

        if ( !str_cmp( arg1, ".s" ) )
        {
            send_to_char( "String so far:\n\r", ch );
            send_to_char( *ch->desc->pString, ch );
            return;
        }

        if ( !str_cmp( arg1, ".-" ) )
        {
            strcpy(buf, *ch->desc->pString ); 
	    if ( buf[0] == '\0' )
	    {
		send_to_char("Empty\n\r",ch);
		return;
	    } 
            for(anum=strlen(buf) ; buf[anum]!='\r' && anum>0 ; anum-- );
	    if ( anum == 0 )
	    {
		send_to_char("Empty\n\r",ch);
		return;
	    }
	    for (anum = ( anum ? anum-1 : 0 ) ; buf[anum]!='\r' && anum>0 ; anum--);
	    buf[ anum ? anum+1 : 0 ] = '\0';
	    free_string(*ch->desc->pString );
	    *ch->desc->pString = str_dup( buf );
	    send_to_char("Last line removed\n\r",ch);
            return;
        }

        if ( !str_cmp( arg1, ".v" ) )
        {
	   if (!check_string( *ch->desc->pString ) )
	     {
            send_to_char( "Oops!  There are some bad charactors.\n\r", ch );
            send_to_char( "Only Alpha / Numeric or - ' , . ; ? * charactors allowd.\n\r", ch );
            send_to_char( "Possible control charactors.  Use '.r' to replace\n\r", ch );
            send_to_char( "'bad' charactors or '.c' to clear the string.\n\r", ch );
	     return; 
	     }
            send_to_char( "The string has been verified.  All is well.\n\r", ch );
            return;
        }

        if ( !str_cmp( arg1, ".r" ) )
        {
            if ( arg2[0] == '\0' )
            {
                send_to_char( "usage:  .r \"old string\" \"new string\"\n\r", ch );
                return;
            }

            *ch->desc->pString = string_replace( *ch->desc->pString, arg2, arg3 );
            sprintf( buf, "'%s' replaced with '%s'.\n\r", arg2, arg3 );
            send_to_char( buf, ch );
            return;
        }

        if ( !str_cmp( arg1, ".f" ) )
        {
            *ch->desc->pString = format_string( *ch->desc->pString );
            send_to_char( "String formatted.\n\r", ch );
            return;
        }
        
        if ( !str_cmp( arg1, ".h" ) )
        {
            send_to_char( 
"Sedit help (commands available from the beginning of a new line only):  \n\r", ch );
            send_to_char( 
".r 'old string' 'new string'   - replace a substring with a new string  \n\r", ch );
            send_to_char( 
"                                (requires '', \"\" around the arguments)\n\r", ch );
            send_to_char( 
".h                             - get help (this info)                   \n\r", ch );
            send_to_char( 
".s                             - show string so far                     \n\r", ch );
            send_to_char( 
".f                             - format (word wrap) string              \n\r", ch );
            send_to_char( 
".c                             - clear string so far                    \n\r", ch );
            send_to_char( 
".v                             - verify string  (check for 'bad' charactors) \n\r", ch );
            send_to_char( 
".-                             - erase last line\n\r", ch );
            send_to_char( 
"@  or  ~                       - end string editor (end string)         \n\r", ch );
            return;
        }
            

        send_to_char( "SEdit:  Invalid dot command.\n\r", ch );
        return;
    }

    strcpy( buf, *ch->desc->pString );

    if ( ( argument[ 0 ] == '-' || argument[ 0 ] == '@' ) && argument[ 1 ] == '\0' )
    {
	if ( !check_string( *ch->desc->pString ) )
	  {
        send_to_char( "Invalid charactors in this string.\n\r", ch );
        send_to_char( "Only Alpha / Numeric or - ' , . ; ? * charactors allowd.\n\r", ch );
        send_to_char( "Use the '.r' to replace 'bad' charactors or you\n\r", ch );
        send_to_char( "may have to '.c' clear the string and start over.\n\r", ch );
        send_to_char( "The invalid charactor(s) may be because of control char's\n\r", ch );
        send_to_char( "which may not be visible.\n\r", ch );
	return;
          } 
        ch->desc->pString = NULL;
	if (ch->desc->connected == CON_EDITING )
	ch->desc->connected = CON_PLAYING;
	send_to_char("`w",ch);
        return;
    }

    if ( strlen( buf ) + strlen( argument ) >= ( MAX_STRING_LENGTH - 4 ) )
    {
        send_to_char( "String too long, truncating.\n\r", ch );
        strncat( buf, argument, MAX_STRING_LENGTH );
        free_string( *ch->desc->pString );
        *ch->desc->pString = str_dup( buf );
        ch->desc->pString = NULL;
        return;
    }

    strcat( buf, argument );
    strcat( buf, "\n\r" );
    free_string( *ch->desc->pString );
    *ch->desc->pString = str_dup( buf );
    return;
}
/*
char *format_string( char *oldstring )
{
    char buf[MAX_BUF];
    char newstring[MAX_BUF];
    char *astr;
    char *bstr;
    int c;

    if ( oldstring == NULL || *oldstring == '\0' )
	return &str_empty[0];

    c = 0;

    astr = alloc_mem(sizeof(char)*MAX_STRING_LENGTH);
    *astr='\0';
    strcpy(astr,oldstring);
    bstr = astr;

    buf[ 0 ] = '\0';
    
    while ( *astr != '\0' )
    {

	if ( *astr == '\r' )
	{
	    astr++;
	    continue;
	}

	if ( *astr == '\n' )
	{
	    buf[c++]=' ';
	    while( *astr == ' ' || *astr == '\n' )
	        astr++;
	    continue;
	}

	if ( *astr == ' ' )
	{
	    while( *astr == ' ' || *astr == '\n' )
		astr++;
	

	    if ( ( *astr == '.' && *(astr+1) != '.' ) || *astr == '!' || *astr == '?' )
	    {
	        buf[c++] = *astr;
	 	buf[c++] = ' ';		
	 	buf[c++] = ' ';		
		astr++;
	        while( *astr == ' ' || *astr == '\n' )
		    astr++;
		*astr = UPPER( *astr );
	    }
	    else if ( *astr == ',' || *astr == ';' || *astr == ':' )
	    {
	        buf[c++] = *astr;
	 	buf[c++] = ' ';		
		astr++;
	        while( *astr == ' ' || *astr == '\n' )
		    astr++;
	    }
	    else
		buf[c++] = ' ';

	    continue;
	}


	    if ( ( *astr == '.' && *(astr+1) != '.' ) || *astr == '!' || *astr == '?' )
	    {
	        buf[c++] = *astr;
	 	buf[c++] = ' ';		
	 	buf[c++] = ' ';		
		astr++;
	        while( *astr == ' ' || *astr == '\n' )
		    astr++;
		*astr = UPPER( *astr );
	        continue;
	    }

	    if ( *astr == ',' || *astr == ';' || *astr == ':' )
	    {
	        buf[c++] = *astr;
	 	buf[c++] = ' ';		
		astr++;
	        while( *astr == ' ' || *astr == '\n' )
		    astr++;
		continue;
	    }

	buf[c++] = *astr;
	astr++;

    }
    buf[c++] = '\0';

    astr = str_dup( buf );   
    newstring[0] = '\0';  

    while ( *astr != '\0' )
    {
        *bstr = '\0';
	while ( ( c = ( strchr( astr, ' ' ) == NULL ? str_len(astr) : strchr( astr, ' ' ) - astr ) ) + strlen( bstr ) <= 77 )
	{
	    if ( *bstr != '\0' )
		strcat( bstr, " " );
	    strncat( bstr, astr, c );
	    astr = strpbrk( astr, " " );	
	    if ( astr == NULL )
		astr = &str_empty[0];
	    if ( *astr == ' ' )
		astr++;
	}

	strcat( bstr, "\n\r" );

	if ( !str_cmp( bstr, "\n\r" ) )
	{
	    bug( "Format String: No Spaces found", 0 );
	    strcat( bstr, astr );
	    strcat( bstr, "\n\r" );
	    *astr = '\0';
	}

	strcat( newstring, bstr );
    }

    return str_dup(newstring);    

}
*/
/*
 * Thanks to Kalgen for the new procedure (no more bug!)
 * Original wordwrap() written by Surreality.
 */
/*
char *format_string( char *oldstring)
{
  char xbuf[MAX_BUF];
  char xbuf2[MAX_BUF];
  char *rdesc;
  int i=0;
  bool cap=TRUE;
  
  xbuf[0]=xbuf2[0]='\0';
  
  i=0;
  
  for (rdesc = oldstring; *rdesc != '\0' ; rdesc++)
  {
    if (*rdesc=='\n')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc=='\r') ;
    else if (*rdesc==' ')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc==')')
    {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!'))
      {
        xbuf[i-2]=*rdesc;
        xbuf[i-1]=' ';
        xbuf[i]=' ';
        i++;
      }
      else
      {
        xbuf[i]=*rdesc;
        i++;
      }
    }
    else if (*rdesc=='.' || *rdesc=='?' || *rdesc=='!') {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!')) {
        xbuf[i-2]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i-1]=' ';
          xbuf[i]=' ';
          i++;
        }
        else
        {
          xbuf[i-1]='\"';
          xbuf[i]=' ';
          xbuf[i+1]=' ';
          i+=2;
          rdesc++;
        }
      }
      else
      {
        xbuf[i]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i+1]=' ';
          xbuf[i+2]=' ';
          i += 3;
        }
        else
        {
          xbuf[i+1]='\"';
          xbuf[i+2]=' ';
          xbuf[i+3]=' ';
          i += 4;
          rdesc++;
        }
      }
      cap = TRUE;
    }
    else
    {
      xbuf[i]=*rdesc;
      if ( cap )
        {
          cap = FALSE;
          xbuf[i] = UPPER( xbuf[i] );
        }
      i++;
    }
  }
  xbuf[i]='\0';
  strcpy(xbuf2,xbuf);
  
  rdesc=str_dup(xbuf2);
  
  xbuf[0]='\0';
  
  for ( ; ; )
  {
    for (i=0; i<77; i++)
    {
      if ( *(rdesc+i) == '\0' ) break;
    }
    if (i<77)
    {
      break;
    }
    for (i=(xbuf[0]?76:73) ; i ; i--)
    {
      if (*(rdesc+i)==' ') break;
    }
    if (i)
    {
      *(rdesc+i)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"\n\r");
      rdesc += i+1;
      while (*rdesc == ' ') rdesc++;
    }
    else
    {
      bug ("No spaces", 0);
      *(rdesc+75)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"-\n\r");
      rdesc += 76;
    }
  }
  while (*(rdesc+i) != '\0' && (*(rdesc+i)==' '||
                        *(rdesc+i)=='\n'||
                        *(rdesc+i)=='\r'))
    i--;
  *(rdesc+i+1)='\0';
  strcat(xbuf,rdesc);
  if ( strlen(xbuf) > 2 && xbuf[strlen(xbuf)-2] != '\n')
    strcat(xbuf,"\n\r");
  free_string(oldstring);
  return(str_dup(xbuf));
}
*/
char *format_string_2( char *oldstring)
{
  char xbuf[MAX_BUF];
  char xbuf2[MAX_BUF];
  char *rdesc;
  int i=0;
  bool cap=TRUE;
  
  xbuf[0]=xbuf2[0]='\0';
  
  i=0;
  
  for (rdesc = oldstring; *rdesc != '\0' ; rdesc++)
  {
    if (*rdesc=='\n')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc=='\r') ;
    else if (*rdesc==' ')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc==')')
    {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!'))
      {
        xbuf[i-2]=*rdesc;
        xbuf[i-1]=' ';
        xbuf[i]=' ';
        i++;
      }
      else
      {
        xbuf[i]=*rdesc;
        i++;
      }
    }
    else if (*rdesc=='.' || *rdesc=='?' || *rdesc=='!') {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!')) {
        xbuf[i-2]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i-1]=' ';
          xbuf[i]=' ';
          i++;
        }
        else
        {
          xbuf[i-1]='\"';
          xbuf[i]=' ';
          xbuf[i+1]=' ';
          i+=2;
          rdesc++;
        }
      }
      else
      {
        xbuf[i]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i+1]=' ';
          xbuf[i+2]=' ';
          i += 3;
        }
        else
        {
          xbuf[i+1]='\"';
          xbuf[i+2]=' ';
          xbuf[i+3]=' ';
          i += 4;
          rdesc++;
        }
      }
      cap = TRUE;
    }
    else
    {
      xbuf[i]=*rdesc;
      if ( cap )
        {
          cap = FALSE;
          xbuf[i] = UPPER( xbuf[i] );
        }
      i++;
    }
  }
  xbuf[i]='\0';
  strcpy(xbuf2,xbuf);
  
  rdesc=str_dup(xbuf2);
  
  xbuf[0]='\0';
  
  for ( ; ; )
  {
    for (i=0; i<=77; i++)
    {
      if ( *(rdesc+i) == '\0' ) break;
    }
    if (i<=77)
    {
      break;
    }
    for (i=(xbuf[0]?76:73) ; i ; i--)
    {
      if (*(rdesc+i)==' ') break;
    }
    if (i)
    {
      *(rdesc+i)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"\n\r");
      rdesc += i+1;
      while (*rdesc == ' ') rdesc++;
    }
    else
    {
      bug ("No spaces", 0);
      *(rdesc+75)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"-\n\r");
      rdesc += 76;
    }
  }
  while (*(rdesc+i) != '\0' && (*(rdesc+i)==' '||
                        *(rdesc+i)=='\n'||
                        *(rdesc+i)=='\r'))
    i--;
  *(rdesc+i+1)='\0';
  strcat(xbuf,rdesc);
  if ( strlen(xbuf) > 2 && xbuf[strlen(xbuf)-2] != '\n')
    strcat(xbuf,"\n\r");
  return(str_dup(xbuf));
}

/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes, parenthesis (barring ) ('s) and percentages.
 */
char *first_arg( char *argument, char *arg_first, bool fCase )
{
    char cEnd;

    while ( *argument == ' ' )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"'
      || *argument == '%'  || *argument == '(' )
    {
        if ( *argument == '(' )
        {
            cEnd = ')';
            argument++;
        }
        else cEnd = *argument++;
    }

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
    if ( fCase ) *arg_first = LOWER(*argument);
            else *arg_first = *argument;
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( *argument == ' ' )
	argument++;

    return argument;
}



char * string_unpad( char * argument )
{
    char buf[MAX_BUF];
    char *s;

    s = argument;

    while ( *s == ' ' )
        s++;

    strcpy( buf, s );
    s = buf;

    if ( *s != '\0' )
    {
        while ( *s != '\0' )
            s++;
        s--;

        while( *s == ' ' )
            s--;
        s++;
        *s = '\0';
    }

    free_string( argument );
    return str_dup( buf );
}



char * string_proper( char * argument )
{
    char *s;

    s = argument;

    while ( *s != '\0' )
    {
        if ( *s != ' ' )
        {
            *s = UPPER(*s);
            while ( *s != ' ' && *s != '\0' )
                s++;
        }
        else
        {
            s++;
        }
    }

    return argument;
}



int arg_count( char *argument )
{
    int total;
    char *s;

    total = 0;
    s = argument;

    while ( *s != '\0' )
    {
        if ( *s != ' ' )
        {
            total++;
            while ( *s != ' ' && *s != '\0' )
                s++;
        }
        else
        {
            s++;
        }
    }

    return total;
}
char *format_string( char *oldstring /*, bool fSpace */)
{
  char xbuf[MAX_BUF];
  char xbuf2[MAX_BUF];
  char *rdesc;
  int i=0;
  bool cap=TRUE;
  
  xbuf[0]=xbuf2[0]=0;
  
  i=0;
  
  for (rdesc = oldstring; *rdesc; rdesc++)
  {
    if (*rdesc=='\n')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc=='\r') ;
    else if (*rdesc==' ')
    {
      if (xbuf[i-1] != ' ')
      {
        xbuf[i]=' ';
        i++;
      }
    }
    else if (*rdesc==')')
    {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!'))
      {
        xbuf[i-2]=*rdesc;
        xbuf[i-1]=' ';
        xbuf[i]=' ';
        i++;
      }
      else
      {
        xbuf[i]=*rdesc;
        i++;
      }
    }
    else if (*rdesc=='.' || *rdesc=='?' || *rdesc=='!') {
      if (xbuf[i-1]==' ' && xbuf[i-2]==' ' && 
          (xbuf[i-3]=='.' || xbuf[i-3]=='?' || xbuf[i-3]=='!')) {
        xbuf[i-2]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i-1]=' ';
          xbuf[i]=' ';
          i++;
        }
        else
        {
          xbuf[i-1]='\"';
          xbuf[i]=' ';
          xbuf[i+1]=' ';
          i+=2;
          rdesc++;
        }
      }
      else
      {
        xbuf[i]=*rdesc;
        if (*(rdesc+1) != '\"')
        {
          xbuf[i+1]=' ';
          xbuf[i+2]=' ';
          i += 3;
        }
        else
        {
          xbuf[i+1]='\"';
          xbuf[i+2]=' ';
          xbuf[i+3]=' ';
          i += 4;
          rdesc++;
        }
      }
      cap = TRUE;
    }
    else
    {
      xbuf[i]=*rdesc;
      if ( cap )
        {
          cap = FALSE;
          xbuf[i] = UPPER( xbuf[i] );
        }
      i++;
    }
  }
  xbuf[i]=0;
  strcpy(xbuf2,xbuf);
  
  rdesc=xbuf2;
  
  xbuf[0]=0;
  
  for ( ; ; )
  {
    for (i=0; i<77; i++)
    {
      if (!*(rdesc+i)) break;
    }
    if (i<77)
    {
      break;
    }
    for (i=(xbuf[0]?76:73) ; i ; i--)
    {
      if (*(rdesc+i)==' ') break;
    }
    if (i)
    {
      *(rdesc+i)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"\n\r");
      rdesc += i+1;
      while (*rdesc == ' ') rdesc++;
    }
    else
    {
      bug ("No spaces", 0);
      *(rdesc+75)=0;
      strcat(xbuf,rdesc);
      strcat(xbuf,"-\n\r");
      rdesc += 76;
    }
  }
  while (*(rdesc+i) && (*(rdesc+i)==' '||
                        *(rdesc+i)=='\n'||
                        *(rdesc+i)=='\r'))
    i--;
  *(rdesc+i+1)=0;
  strcat(xbuf,rdesc);
  if (xbuf[strlen(xbuf)-2] != '\n')
    strcat(xbuf,"\n\r");

  free_string(oldstring);
  return(str_dup(xbuf));
}
