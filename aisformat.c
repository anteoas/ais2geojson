/* -----------------------------------------------------------------------
   Process AIS messages read from stdin and output JSON structure
   Copyright 2006 by Brian C. Lane
   All Rights Reserved
   ----------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <getopt.h>

#include "aismsg_pos.h"


static struct option options[] =
{
 {"geojson", 0, NULL, 'g'},
 {"csv", 0, NULL, 'c'}
};

void writeGeoJSON(char *ais, aismsg_pos *pos) {
  // ignore message type 5 since it does not have position information
  if(pos->msgid == 5)
    return;

  printf("{\"type\":\"Feature\",");
  printf(
         "" "\"geometry\":{"
         ""   "\"type\":\"Point\","
         ""   "\"coordinates\":[%0.7f,%0.7f]"
         "" "},",
         pos->long_ddd, pos->lat_dd);
  printf(
         "" "\"properties\":{"
         ""   "\"mmsi\":\"%ld\",", pos->userid);
  printf(""   "\"aistype\":%d,", pos->msgid);
  if(pos->cog >= 0)   printf("\"cog\":%0.1f,", pos->cog);
  if(pos->sog >= 0)   printf("\"sog\":%0.1f,", pos->sog);
  if(pos->trueheading >= 0 && pos->trueheading < 360) {
    printf("\"trueHeading\":%d,", pos->trueheading);
  }
  if(pos->smi >= 0)   printf("\"smi\":%d,", pos->smi);
  // always print ts so we don't need to check if we need comma or not
  if(pos->ts == 0) printf("\"ts\":false");
  else {
    // let's try to output ts as an integer first.
    static char* endptr;
    errno = 0;
    long int ts = strtol(pos->ts, &endptr, 10);
    if(errno == 0 && // no errors
       (endptr + 1 == ais)) { // parsed the entire string (pos->ts+strlen(pos->ts) == ais)
      printf("\"ts\":%ld", ts);
    } else {
      printf("\"ts\":\"%s\"", pos->ts);
    }
  }
  printf("}}\n");
}

void writeCSV(char *ais, aismsg_pos *pos) {
  // ignore type 5 for now
  if(pos->msgid == 5) return;

  printf("%s,%ld,%d,%0.7f,%0.7f,%0.1f,%0.1f,%d,%d\n",
         pos->ts, pos->userid, pos->msgid,
         pos->long_ddd, pos->lat_dd,
         pos->cog, pos->sog,
         pos->trueheading,
         pos->smi);
}

// a buf typically looks like this:
// 2015-09-20T17:00:00;!BSVDM,1,1,,B,13mBwe0P00Pa6:hTHmD>4?vT2<08,0*12
int main(int argc, char *argv[]) {
  char          buf[256];
  aismsg_pos _pos, *pos; pos = &_pos;
  int err = 0;

  int geojson = 1;
  void (*write)(char*,aismsg_pos*) = &writeGeoJSON;

  int ch;
  while((ch = getopt_long(argc, argv,"g:c:", options, NULL)) != -1) {
    switch (ch) {
    case 'g':
      geojson = 1;
      write = &writeGeoJSON;
      break;
    case 'c':
      geojson = 0;
      write = &writeCSV;
      break;
    }
  }

  while( !feof(stdin) ) {
    if (fgets( buf, 255, stdin ) == NULL ) break;

    memset(pos, 0, sizeof(aismsg_pos));

    static char* ais;
    static char* prefix;
    // try to split line and find a timestamp prefix
    if((ais = strchr(buf, ';')) != NULL) {
      ais[0] = '\0'; // turn ; into '\0', same buffer nicely becomes two strings
      ais++; // point ais just past the first ;
      pos->ts = buf; // timestamp now points to the buffer beginning (NULL-term were ; was)
    } else { // no ; found on line
      ais = buf;
      prefix = "";
    }

    if((err = buf2pos(ais, pos)) == 0) {
           (*write)(ais, pos);
    }// else printf("%s", ais);
  }
  return 0;
}
