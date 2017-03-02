/// AIS position message abstraction over bcl's
/// https://github.com/bcl/aisparser (BSD license)
///
/// This file provides a simplified aismsg_pos struct that contains
/// the common parts of many message types (1, 2, 3, 18, 19).
///
///
/// sometimes i try to cite the latest spec:
///   [ITU.1371] : https://www.itu.int/dms_pubrec/itu-r/rec/m/R-REC-M.1371-5-201402-I!!PDF-E.pdf

#define _GNU_SOURCE // http://stackoverflow.com/questions/15334558/compiler-gets-warnings-when-using-strptime-function-c

#include <string.h>

#include <portable.h>
#include <nmea.h>
#include <sixbit.h>
#include <vdm_parse.h>


#include "aismsg_pos.h"

// https://github.com/dma-enav/EPD/blob/master/epd-common/src/main/java/dk/dma/epd/common/prototype/ais/VesselPositionData.java#L100
float cog2float(int cog) { return cog >= 3600 ? -1.0 : cog / 10.0; }
float sog2float(int sog) { return sog > 100 ? -1.0 : sog / 10.0; }


// this is great. the [ITU.1371] version 4 says there's a special
// manouver indicator (smi), but our parser library doesn't seem to
// mention it explicitly as it's probably using an older ITU. So we
// pick our the bits we want. See
// https://github.com/dma-ais/AisLib/blob/master/ais-lib-messages/src/main/java/dk/dma/ais/message/AisPositionMessage.java#L159
// and https://www.itu.int/dms_pubrec/itu-r/rec/m/R-REC-M.1371-5-201402-I!!PDF-E.pdf
// page 108.
int regional2smi(char regional) {
  return regional >> 2;
}

// take in null-terminated string buf and fill in a aismsg_pos
// structure. returns 0 of success.
int buf2pos(char* buf, aismsg_pos *pos) {
  static ais_state ais;

  memset( &ais, 0, sizeof( ais_state ) );

  if (assemble_vdm( &ais, buf ) == 0) {
    ais.msgid = (unsigned char) get_6bit( &ais.six_state, 6);
    pos->msgid = ais.msgid;
    pos->smi = -1;

    switch( ais.msgid ) {
    case 1: {
      static aismsg_1  msg_1;
      if( parse_ais_1( &ais, &msg_1 ) == 0 )
        {
          pos->userid = msg_1.userid;
          pos2ddd( msg_1.latitude, msg_1.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_1.cog);
          pos->sog = sog2float(msg_1.sog);
          pos->smi = regional2smi(msg_1.regional);
        } else return 1;
      break;
    }
    case 2: {
      static aismsg_2 msg_2;
      if( parse_ais_2( &ais, &msg_2 ) == 0 )
        {
          pos->userid = msg_2.userid;
          pos2ddd( msg_2.latitude, msg_2.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_2.cog);
          pos->sog = sog2float(msg_2.sog);
          pos->smi = regional2smi(msg_2.regional);
        } else return 2;
      break;
    }
    case 3: {
      static aismsg_3 msg_3;
      if( parse_ais_3( &ais, &msg_3 ) == 0 )
        {
          pos->userid = msg_3.userid;
          pos2ddd( msg_3.latitude, msg_3.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_3.cog);
          pos->sog = sog2float(msg_3.sog);
          pos->smi = regional2smi(msg_3.regional);
        } else return 3;
      break;
    }
    case 4: {
      static aismsg_4 msg_4;
      if( parse_ais_4( &ais, &msg_4 ) == 0 )
        {
          pos->userid = msg_4.userid;
          pos2ddd( msg_4.latitude, msg_4.longitude, &pos->lat_dd, &pos->long_ddd );
        } else return 4;
      break;
    }
    case 9: {
      static aismsg_9 msg_9;
      if( parse_ais_9( &ais, &msg_9 ) == 0 )
        {
          pos->userid = msg_9.userid;
          pos2ddd( msg_9.latitude, msg_9.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_9.cog);
          pos->sog = sog2float(msg_9.sog);
          pos->smi = regional2smi(msg_9.regional);
        } else return 0;
      break;
    }
    case 18: {
      static aismsg_18 msg_18;
      if( parse_ais_18( &ais, &msg_18 ) == 0 )
        {
          pos->userid = msg_18.userid;
          pos2ddd( msg_18.latitude, msg_18.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_18.cog);
          pos->sog = sog2float(msg_18.sog);
          // [ITU.1371] says message has no regional / smi
        } else return 18;
      break;
    }
    case 19: {
      static aismsg_19 msg_19;
      if( parse_ais_19( &ais, &msg_19 ) == 0 )
        {
          pos->userid = msg_19.userid;
          pos2ddd( msg_19.latitude, msg_19.longitude, &pos->lat_dd, &pos->long_ddd );
          pos->cog = cog2float(msg_19.cog);
          pos->sog = sog2float(msg_19.sog);
          // [ITU.1371] says message has not regional / smi
        } else return 19;
      break;
    }
    default:
      return -1;
    }
  } else return -2;

  if(pos->lat_dd >  90) return 100;
  if(pos->lat_dd < -90) return 101;

  if(pos->long_ddd >  180) return 102;
  if(pos->long_ddd < -180) return 103;

  return 0;
}
