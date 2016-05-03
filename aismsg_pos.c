
#define _GNU_SOURCE // http://stackoverflow.com/questions/15334558/compiler-gets-warnings-when-using-strptime-function-c


#include <string.h>
#include <time.h>

#include <portable.h>
#include <nmea.h>
#include <sixbit.h>
#include <vdm_parse.h>


#include "aismsg_pos.h"

// https://github.com/dma-enav/EPD/blob/master/epd-common/src/main/java/dk/dma/epd/common/prototype/ais/VesselPositionData.java#L100
float cog2float(int cog) { return cog >= 3600 ? -1.0 : cog / 10.0; }
float sog2float(int sog) { return sog > 100 ? -1.0 : sog / 10.0; }


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
          pos->cog = cog2float(msg_1.cog);
          pos->sog = sog2float(msg_1.sog);
        } else return 1;
      break;
    case 2:
      if( parse_ais_2( &ais, &msg_2 ) == 0 )
        {
          pos->userid = msg_2.userid;
          pos2ddd( msg_2.latitude, msg_2.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_2.cog);
          pos->sog = sog2float(msg_2.sog);
        } else return 2;
      break;
    case 3:
      if( parse_ais_3( &ais, &msg_3 ) == 0 )
        {
          pos->userid = msg_3.userid;
          pos2ddd( msg_3.latitude, msg_3.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_3.cog);
          pos->sog = sog2float(msg_3.sog);
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
          pos->cog = cog2float(msg_9.cog);
          pos->sog = sog2float(msg_9.sog);
        } else return 0;
      break;
    case 18:
      if( parse_ais_18( &ais, &msg_18 ) == 0 )
        {
          pos->userid = msg_18.userid;
          pos2ddd( msg_18.latitude, msg_18.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_18.cog);
          pos->sog = sog2float(msg_18.sog);
        } else return 18;
      break;
    case 19:
      if( parse_ais_19( &ais, &msg_19 ) == 0 )
        {
          pos->userid = msg_19.userid;
          pos2ddd( msg_19.latitude, msg_19.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_19.cog);
          pos->sog = sog2float(msg_19.sog);
        } else return 19;
      break;
    default:
      return -1;
    }
  } else return -2;

  return 0;
}
