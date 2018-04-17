#include "terrain_constants.glsl"

void sync() {
  memoryBarrier();
  barrier();
}

layout(local_size_x = 32, local_size_y = 32, local_size_z = 1) in;

uniform layout(set = 0, binding = 0, r32f) image2D heightMap;

struct TerrainDatum {
  vec4 height;
};

layout(std430, set = 0, binding = 1) buffer DataBuffer {
  
  TerrainDatum data[];

} dataBuffer;

ivec2 gridSize = imageSize(heightMap);
ivec2 myCoords = ivec2(gl_GlobalInvocationID.xy);
  
float sampleHeightmap(in ivec2 coords) {
  if (coords.x < 0) coords.x += gridSize.x;
  if (coords.y < 0) coords.y += gridSize.y;
  if (coords.x >= gridSize.x) coords.x -= gridSize.x;
  if (coords.y >= gridSize.y) coords.y -= gridSize.y;
  
  return imageLoad(heightMap, coords).r;
}

void writeHeightmap(float val) {
  vec4 temp;
  temp.x = val;
  imageStore(heightMap, myCoords, temp); 
}

struct Neighborhood {
  float me;
  float ns[4];
};

Neighborhood getNeighborhood()
{
  Neighborhood neigh;
  neigh.me = sampleHeightmap(myCoords);
  neigh.ns[0] = sampleHeightmap(myCoords + ivec2(1, 0));
  neigh.ns[1] = sampleHeightmap(myCoords + ivec2(0, 1));
  neigh.ns[2] = sampleHeightmap(myCoords + ivec2(-1, 0));
  neigh.ns[3] = sampleHeightmap(myCoords + ivec2(0, -1));
  return neigh;
}

#include "../tree/tree_common.glsl"

layout(std430, set = 0, binding = 2) buffer TreeBuffer {
  
  uint counter;
  Rig[] rigs;
 
} treeBuffer;
