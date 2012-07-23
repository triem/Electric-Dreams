#define DK_BLACK    "`k"
#define DK_RED      "`r"
#define DK_GREEN    "`g"
#define DK_YELLOW   "`y"
#define DK_BLUE     "`b"
#define DK_MAGENTA  "`m"
#define DK_CYAN     "`c"
#define DK_WHITE    "`w"
 
#define CODE_DK_BLACK    "`k"
#define CODE_DK_RED      "`r"
#define CODE_DK_GREEN    "`g"
#define CODE_DK_YELLOW   "`y"
#define CODE_DK_BLUE     "`b"
#define CODE_DK_MAGENTA  "`m"
#define CODE_DK_CYAN     "`c"
#define CODE_DK_WHITE    "`w"

#define LT_BLACK    "`K"
#define LT_RED      "`R"
#define LT_GREEN    "`G"
#define LT_YELLOW   "`Y"
#define LT_BLUE     "`B"
#define LT_MAGENTA  "`M"
#define LT_CYAN     "`C"
#define LT_WHITE    "`W"

#define CODE_LT_BLACK    "`K"
#define CODE_LT_RED      "`R"
#define CODE_LT_GREEN    "`G"
#define CODE_LT_YELLOW   "`Y"
#define CODE_LT_BLUE     "`B"
#define CODE_LT_MAGENTA  "`M"
#define CODE_LT_CYAN     "`C"
#define CODE_LT_WHITE    "`W"

#define COLOR_POOF			LT_YELLOW
#define COLOR_EDIT			LT_YELLOW
#define COLOR_SAVE			DK_YELLOW
#define COLOR_WELCOME			LT_MAGENTA
#define COLOR_MISC			LT_WHITE
#define COLOR_SAY			DK_CYAN
#define COLOR_WHO			LT_RED
#define COLOR_PROMPT			DK_WHITE
#define COLOR_INFO			LT_GREEN
#define COLOR_QUIT			LT_YELLOW
#define COLOR_EMOTE			DK_BLUE
#define COLOR_ERROR			LT_RED
#define COLOR_TITLE			LT_YELLOW
#define COLOR_ECHO			LT_RED
#define COLOR_LIST			LT_CYAN
#define COLOR_PLIST			LT_GREEN
#define COLOR_TELL			LT_RED
#define COLOR_MATE			LT_BLUE
#define COLOR_REPLY			LT_RED
#define COLOR_ANSWER			DK_CYAN
#define COLOR_QUESTION			DK_CYAN
#define COLOR_PRAY			LT_WHITE
#define COLOR_IMM			LT_YELLOW
#define COLOR_SHOUT			DK_GREEN
#define COLOR_AUCTION			LT_GREEN
#define COLOR_THINK			LT_MAGENTA
#define COLOR_YELL			DK_YELLOW
#define COLOR_POSE			COLOR_EMOTE
#define COLOR_MAGIC			LT_WHITE
#define COLOR_SOCIAL			DK_MAGENTA
#define COLOR_PKCHAT			DK_RED	
#define COLOR_WHITE			LT_WHITE
#define COLOR_BANK			DK_GREEN
#define COLOR_DIRECTION			LT_BLUE
#define COLOR_WEATHER			LT_BLUE
#define COLOR_OBJECTS			LT_WHITE
#define COLOR_INV_ITEMS			LT_CYAN	
#define COLOR_LEAVE			DK_BLUE
#define COLOR_MOB_ARRIVE		DK_BLUE
#define COLOR_TELLS_GROUP		LT_GREEN
#define COLOR_TALK_TO_IMMORTAL		DK_BLUE
#define COLOR_CHAR_COND_EXCELLENT	LT_GREEN
#define COLOR_CHAR_COND_SCRATCHES	DK_GREEN
#define COLOR_CHAR_COND_SMALL_WNDS	LT_BLUE
#define COLOR_CHAR_COND_QUITE_WNDS	DK_BLUE
#define COLOR_CHAR_COND_NASTY_WNDS	LT_YELLOW
#define COLOR_CHAR_COND_PRETTY_HURT	DK_YELLOW
#define COLOR_CHAR_COND_AWFUL		LT_RED
#define COLOR_CHAR_COND_DEAD		DK_RED
#define COLOR_CHAR_DESC			DK_BLUE
#define COLOR_CHAR_EQ_WEAR_LOCATION	DK_YELLOW
#define COLOR_CHAR_EQ_WEAR_ITEM		DK_MAGENTA
#define COLOR_GLOWING_RED_EYES		DK_RED
#define COLOR_ROOM_CONTENTS		DK_CYAN
#define COLOR_ROOM_PEOPLE		LT_CYAN
#define COLOR_PLAYER_PKER		LT_RED
#define COLOR_CHAR_CARRYING		DK_BLUE
#define COLOR_CHAR_USING		LT_CYAN
#define COLOR_ALIGNMENT_CHANGE		DK_RED
#define COLOR_DEAD			LT_RED
#define COLOR_ROOM_FLAGS_BEGIN		DK_GREEN
#define COLOR_ROOM_FLAGS_END		LT_GREEN
#define COLOR_ROOM_EXITS		DK_GREEN
#define COLOR_ANSI_CODES		LT_GREEN
#define COLOR_LOGIN			LT_CYAN
#define COLOR_NEWBIE_INFO		LT_MAGENTA
#define COLOR_CHAR_MOOD			LT_YELLOW
#define COLOR_CHAR_MOVE			LT_YELLOW

/*
 * Ansi colors and VT100 codes
 * Used in #PLAYER
 *
 * On most machines that use ANSI, namely the IBM PCs, the decimal value for
 * the escape character is 27 (1B hex).     Change this value when needed.
 */

#define BLACK      "`K"    /* These are foreground color codes */
#define RED        "`R"
#define GREEN      "`G"
#define YELLOW     "`Y"
#define BLUE       "`B"
#define PURPLE     "`M"
#define CYAN       "`C"
#define GREY       "`W"

#define B_BLACK    "`k"    /* These are background color codes */
#define B_RED      "`r"
#define B_GREEN    "`g"
#define B_YELLOW   "`y"
#define B_BLUE     "`b"
#define B_PURPLE   "`m"
#define B_CYAN     "`c"
#define B_GREY     "`w"

/* Below are VT100 and ANSI codes (above are ANSI exclusively)       */

#define EEEE       "\x1b#8"                 /* Turns screen to EEEEs */
#define CLRSCR     "\x1b[2j"                /* Clear screen          */
#define CLREOL     "\x1b[K"                 /* Clear to end of line  */

#define UPARR      "\x1b[A"                 /* Up one line           */
#define DOWNARR    "\x1b[B"                 /* Down one line         */
#define RIGHTARR   "\x1b[C"                 /* Right one column      */
#define LEFTARR    "\x1b[D"                 /* Left one column       */
#define HOMEPOS    "\x1b[H"                 /* Home (upper left)     */

#define BOLD       "\x1b[1m"                /* High intensity        */
#define FLASH      "\x1b[5m"                /* Flashing text         */
#define INVERSE    "\x1b[7m"                /* XORed back and fore   */
#define NTEXT      "\x1b[0m"                /* Normal text  (grey)   */
