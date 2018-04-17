#define TERRAIN_XZ_SCALE 1024.0
#define TERRAIN_Y_SCALE 100.0


vec3 terrainLocalToWorld(vec3 uyv) {
  
  vec3 terrainWorld = uyv;
  terrainWorld.xz -= vec2(0.5, 0.5);
  
  terrainWorld.xz *= TERRAIN_XZ_SCALE;
  terrainWorld.y *= TERRAIN_Y_SCALE;
  
  return terrainWorld;
}

