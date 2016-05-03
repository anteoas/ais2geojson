
// common struct for several AIS position message
struct aismsg_pos {
  double lat_dd;
  double long_ddd;

  float cog;
  float sog;
  int smi;

  long   userid;
  long   ts;
};

typedef struct aismsg_pos aismsg_pos;


int buf2pos(char*, aismsg_pos *);

