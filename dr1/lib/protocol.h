#ifndef __DR1STRBUF__H
#include "strbuf.h"
#endif

#ifndef __DR1STREAM__H
#include "stream.h"
#endif

/* Message Types */
#define DR1MSG_IDENT "DR1/1.0\n"
#define DR1MSG_100 "100 LOGIN OK\n"
#define DR1MSG_105 "105 CHARACTER CREATION DIALOG\n"
#define DR1MSG_110 "110 MAP GLYPHS\n"
#define DR1MSG_120 "120 MAP DATA\n"
#define DR1MSG_130 "130 MAP DELTA\n"
#define DR1MSG_135 "135 PROJECTILE %d %d %d %d %d %d\n"
#define DR1MSG_140 "140 ATK %d %d %s\n"
#define DR1MSG_150 "150 DEF %d %d %s %s\n"
#define DR1MSG_170 "170 PLAYER DATA\n"
#define DR1MSG_175 "175 LOCATION %s %d %d\n"
#define DR1MSG_180 "180 TREASURE %d %d %d %d %d %d\n"
#define DR1MSG_190 "190 CMDLIST %s\n"
#define DR1MSG_195 "195 INVENTORY DIALOG\n"

#define DR1MSG_200 "200 OK (%d)\n"
#define DR1MSG_210 "210 SENT (%d)\n"
#define DR1MSG_220 "220 SAVED (%d)\n"
#define DR1MSG_250 "250 OFFER (%d) %d %s\n"
#define DR1MSG_260 "260 ACCEPT (%d) %s\n"
#define DR1MSG_270 "270 DECLINE (%d) %s\n"

#define DR1MSG_310 "310 FAILED (%d) %s\n"
#define DR1MSG_320 "320 DEAD %s\n"

#define DR1MSG_500 "500 BAD USER/PASSWORD\n"
#define DR1MSG_510 "510 PROTOCOL ERROR\n"
#define DR1MSG_520 "520 FILE/DATA ERROR LOADING %s\n"
#define DR1MSG_530 "530 UNKNOWN COMMAND %s IN STATE %s\n"
#define DR1MSG_540 "540 BAD PLAYER STATE\n"
#define DR1MSG_550 "550 BLACKLISTED\n"
#define DR1MSG_560 "560 COMMAND ERROR %d\n"
#define DR1MSG_570 "570 FATAL ERROR %s\n"
#define DR1MSG_580 "580 PLAYER ALREADY EXISTS %s\n"
#define DR1MSG_590 "590 BAD PARAMETER %s\n"

#define SEPARATOR "7608bdb04fee4d6cf23289314582203c\n"

#define DR1CMD_LOGIN "IAM %s %s\n"
#define DR1CMD_NEWPLAYER "NEWPLAYER %s %s\n"
#define DR1CMD_MOVE "MOVE %c\n"
#define DR1CMD_SAY "SAY %s\n"
#define DR1CMD_YELL "YELL %s\n"
#define DR1CMD_MESG "MESG %s %s\n"
#define DR1CMD_ATTACK "ATTACK %d %d\n"

#define DR1CMD_BUY "BUY %s %d\n"
#define DR1CMD_OFFER "OFFER %d\n"

#define DR1CMD_ROLL "ROLL\n"
#define DR1CMD_DONE "DONE\n"
#define DR1CMD_CANCEL "CANCEL\n"
#define DR1CMD_QUIT "QUIT\n"
#define DR1CMD_IMPROVE "IMPROVE %s %s\n"
#define DR1CMD_SWAP "SWAP %s %s\n"
#define DR1CMD_CLASS "CLASS %s\n"
#define DR1CMD_RACE "RACE %s\n"
#define DR1CMD_SEX "SEX %s\n"
#define DR1CMD_NAME "NAME %s\n"

#define DR1ENONAME 1
#define DR1ENOCLASS 2
#define DR1ENOSTATS 3

void
psendMessage( dr1Stream* os, char *msg, ...);

int
precvMessage( char* buf, char *msg, ...);
