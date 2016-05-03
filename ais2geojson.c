/* -----------------------------------------------------------------------
   Process AIS messages read from stdin and output JSON structure
   Copyright 2006 by Brian C. Lane
   All Rights Reserved
   ----------------------------------------------------------------------- */

#define _GNU_SOURCE // http://stackoverflow.com/questions/15334558/compiler-gets-warnings-when-using-strptime-function-c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <portable.h>
#include <nmea.h>
#include <sixbit.h>
#include <vdm_parse.h>

// common struct for several AIS position message
struct aismsg_pos {
  double lat_dd;
  double long_ddd;
  long   userid;
  long   ts;
};

typedef struct aismsg_pos aismsg_pos;

// take in null-terminated string buf and fill in a aismsg_pos
// structure. returns 0 of success.
int buf2pos(char* buf, aismsg_pos *pos) {
  static ais_state ais;
  static aismsg_1  msg_1;
  static aismsg_2  msg_2;
  static aismsg_3  msg_3;
  static aismsg_4  msg_4;
  static aismsg_9  msg_9;
  static aismsg_18 msg_18;
  static aismsg_19 msg_19;

  memset( &ais, 0, sizeof( ais_state ) );

  static struct tm time;
  if(strptime(buf, "%Y-%m-%dT%T", &time) != 0)
    pos->ts = mktime(&time);

  if (assemble_vdm( &ais, buf ) == 0) {
    ais.msgid = (unsigned char) get_6bit( &ais.six_state, 6);

    switch( ais.msgid ) {
    case 1:
      if( parse_ais_1( &ais, &msg_1 ) == 0 )
        {
          pos->userid = msg_1.userid;
          pos2ddd( msg_1.latitude, msg_1.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 1;
      break;
    case 2:
      if( parse_ais_2( &ais, &msg_2 ) == 0 )
        {
          pos->userid = msg_2.userid;
          pos2ddd( msg_2.latitude, msg_2.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 2;
      break;
    case 3:
      if( parse_ais_3( &ais, &msg_3 ) == 0 )
        {
          pos->userid = msg_3.userid;
          pos2ddd( msg_3.latitude, msg_3.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 3;
      break;
    case 4:
      if( parse_ais_4( &ais, &msg_4 ) == 0 )
        {
          pos->userid = msg_4.userid;
          pos2ddd( msg_4.latitude, msg_4.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 4;
      break;
    case 9:
      if( parse_ais_9( &ais, &msg_9 ) == 0 )
        {
          pos->userid = msg_9.userid;
          pos2ddd( msg_9.latitude, msg_9.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 0;
      break;
    case 18:
      if( parse_ais_18( &ais, &msg_18 ) == 0 )
        {
          pos->userid = msg_18.userid;
          pos2ddd( msg_18.latitude, msg_18.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 18;
      break;
    case 19:
      if( parse_ais_19( &ais, &msg_19 ) == 0 )
        {
          pos->userid = msg_19.userid;
          pos2ddd( msg_19.latitude, msg_19.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 19;
      break;
    default:
      return -1;
    }
  } else return -2;

  return 0;
}

// a buf typically looks like this:
// 2015-09-20T17:00:00;!BSVDM,1,1,,B,13mBwe0P00Pa6:hTHmD>4?vT2<08,0*12
int main(int argc, char *argv[]) {
  char          buf[256];
  aismsg_pos _pos, *pos; pos = &_pos;
  int err = 0;

  while( !feof(stdin) ) {
    if (fgets( buf, 255, stdin ) == NULL ) break;

    memset(pos, 0, sizeof(aismsg_pos));

    if((err = buf2pos(buf, pos)) == 0) {
      printf("{\"type\":\"Feature\","
             "" "\"properties\":{"
             ""   "\"ts\":%ld,"
             ""   "\"mmsi\":\"%09ld\""
             "" "},"
             "" "\"geometry\":{"
             ""   "\"type\":\"Point\","
             ""   "\"coordinates\":[%0.6f,%0.6f]"
             "" "}"
             "}\n",
             pos->long_ddd, pos->lat_dd,
             pos->ts, pos->userid );
    } // else printf("error in buf2aismsg_pos %d\n", err);
  }
  return 0;
}

