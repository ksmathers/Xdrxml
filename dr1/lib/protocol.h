#ifndef __DR1STRBUF__H
#include "strbuf.h"
#endif

#ifndef __DR1STREAM__H
#include "stream.h"
#endif

/* Message Types */
#define DR1MSG_IDENT "DR1/1.0"
#define DR1MSG_100 "100 LOGIN OK"
#define DR1MSG_101 "101 PONG"
#define DR1MSG_105 "105 CHARACTER CREATION DIALOG"
#define DR1MSG_110 "110 MAP GLYPHS"
#define DR1MSG_120 "120 MAP DATA"
#define DR1MSG_130 "130 MAP DELTA"
#define DR1MSG_135 "135 PROJECTILE %d %d %d %d %d %d"
#define DR1MSG_140 "140 ATK %d %d %s"
#define DR1MSG_150 "150 DEF %d %d %s %s"
#define DR1MSG_170 "170 PLAYER DATA"
#define DR1MSG_175 "175 LOCATION mapname:%s x:%d y:%d"
#define DR1MSG_180 "180 TREASURE %d %d %d %d %d %d"
#define DR1MSG_190 "190 CMDLIST %s"
#define DR1MSG_195 "195 INVENTORY DIALOG"

#define DR1MSG_200 "200 OK (%d)"
#define DR1MSG_210 "210 SENT (%d)"
#define DR1MSG_220 "220 SAVED (%d)"
#define DR1MSG_250 "250 OFFER (%d) %d %s"
#define DR1MSG_260 "260 ACCEPT (%d) %s"
#define DR1MSG_270 "270 DECLINE (%d) %s"

#define DR1MSG_310 "310 FAILED (%d) %s"
#define DR1MSG_320 "320 DEAD %s"


#define DR1MSG_500 "500 BAD USER/PASSWORD"
#define DR1MSG_510 "510 PROTOCOL ERROR"
#define DR1MSG_520 "520 FILE/DATA ERROR LOADING %s"
#define DR1MSG_530 "530 UNKNOWN COMMAND %s IN STATE %s"
#define DR1MSG_540 "540 BAD PLAYER STATE"
#define DR1MSG_550 "550 BLACKLISTED"
#define DR1MSG_560 "560 COMMAND ERROR %d"
#define DR1MSG_570 "570 FATAL ERROR %s"
#define DR1MSG_580 "580 PLAYER ALREADY EXISTS %s"
#define DR1MSG_590 "590 BAD PARAMETER %s"

#define SEPARATOR "7608bdb04fee4d6cf23289314582203c"

#define DR1CMD_LOGIN "IAM %s %s"
#define DR1CMD_NEWPLAYER "NEWPLAYER %s %s"
#define DR1CMD_MOVE "MOVE %c"
#define DR1CMD_OPEN "OPEN"
#define DR1CMD_SAY "SAY %s"
#define DR1CMD_YELL "YELL %s"
#define DR1CMD_MESG "MESG %s %s"
#define DR1CMD_ATTACK "ATTACK %d %d"

#define DR1CMD_BUY "BUY %s %d"
#define DR1CMD_OFFER "OFFER %d"

#define DR1CMD_ROLL "ROLL"
#define DR1CMD_DONE "DONE"
#define DR1CMD_CANCEL "CANCEL"
#define DR1CMD_QUIT "QUIT"
#define DR1CMD_IMPROVE "IMPROVE %s %s"
#define DR1CMD_SWAP "SWAP %s %s"
#define DR1CMD_CLASS "CLASS %s"
#define DR1CMD_RACE "RACE %s"
#define DR1CMD_SEX "SEX %s"
#define DR1CMD_NAME "NAME %s"

#define DR1ENONAME 1
#define DR1ENOCLASS 2
#define DR1ENOSTATS 3
#define DR1EATTR 4

/* return TRUE if the message in the buffer matches the first 3 chars of 
   the message type 'msg'
*/
int pisMessage( char *buf, char *msg);

void
psendMessage( dr1Stream* os, char *msg, ...);

int
precvMessage( char* buf, char *msg, ...);
