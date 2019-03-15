// common struct for several AIS position message
struct aismsg_pos {
  char msgid; // ais message type (1, 2, 3, 18, 19)

  char nav_status;
  unsigned char rot;

  double lat_dd;
  double long_ddd;

  float cog;
  float sog;
  int trueheading;
  int smi; // 2 bits special-man-indicator

  long   userid;
  char* ts;

  // ais mesage 5 fields
  unsigned long imo;
  char callsign[8];
  char name[21];
  unsigned char ship_type;
  int dim_bow;
  int dim_stern;
  char dim_port;
  char dim_starboard;
  char pos_type;
  unsigned long eta;
  unsigned char draught;
  char dest[21];
};

typedef struct aismsg_pos aismsg_pos;

int buf2pos(char*, aismsg_pos *);
