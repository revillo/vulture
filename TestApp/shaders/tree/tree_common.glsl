#define MAX_JOINTS 20
#define MAX_LINKS 20

struct Joint {
  vec4 pos;
  vec4 quat;
};

struct Link {
  uint pis[2];
  uint cs[2];
};

struct Rig {
  uvec4 info;
  Joint joints[MAX_JOINTS];
  Link links[MAX_LINKS];
};

#define MAX_TREES 20000