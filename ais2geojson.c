/* -----------------------------------------------------------------------
   Process AIS messages read from stdin and output JSON structure
   Copyright 2006 by Brian C. Lane
   All Rights Reserved
   ----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "aismsg_pos.h"


// a buf typically looks like this:
// 2015-09-20T17:00:00;!BSVDM,1,1,,B,13mBwe0P00Pa6:hTHmD>4?vT2<08,0*12
int main(int argc, char *argv[]) {
  char          buf[256];
  aismsg_pos _pos, *pos; pos = &_pos;
  int err = 0;

  char
    config_smi = 1,
    config_cog = 1,
    config_sog = 1;

  while( !feof(stdin) ) {
    if (fgets( buf, 255, stdin ) == NULL ) break;

    memset(pos, 0, sizeof(aismsg_pos));

    if((err = buf2pos(buf, pos)) == 0) {
      printf("{\"type\":\"Feature\",");
      printf(
             // "" "\"aistype\": %d,"
             "" "\"geometry\":{"
             ""   "\"type\":\"Point\","
             ""   "\"coordinates\":[%0.6f,%0.6f]"
             "" "},",
             pos->msgid,
             pos->long_ddd, pos->lat_dd);
      printf(
             "" "\"properties\":{"
             ""   "\"mmsi\":\"%ld\",", pos->userid);
      printf(""   "\"aistype\":%ld,", pos->msgid);
      if(config_cog && pos->cog >= 0)   printf("\"cog\":%0.1f,", pos->cog);
      if(config_sog && pos->sog >= 0)   printf("\"sog\":%0.1f,", pos->sog);
      if(config_smi && pos->smi)   printf("\"smi\":%0.1f,", pos->smi);
      // always print ts so we don't need to check if we need comma or not
      if(pos->ts == 0) printf("\"ts\":false");
      else             printf("\"ts\":%ld", pos->ts);
      printf("}}\n");
    } // else printf("error in buf2aismsg_pos %d\n", err);
  }
  return 0;
}

