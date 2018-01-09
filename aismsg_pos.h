
// common struct for several AIS position message
struct aismsg_pos {

  int msgid; // ais message type (1, 2, 3, 18, 19 etc)

  double lat_dd;
  double long_ddd;

  float cog;
  float sog;
  int trueheading;
  int smi; // 2 bits special-man-indicator

  long   userid;
  char* ts;
};

typedef struct aismsg_pos aismsg_pos;


int buf2pos(char*, aismsg_pos *);

