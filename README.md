# Terrain generator
Generates a 3d terrain using Perlin Noise.

![3D Model](screenshots/3d_terrain_screenshot.png)
![2d Map](screenshots/2d_map_screenshot.bmp)
# Build
```
gcc terrain.c perlin.c write_obj.c write_bmp.c -o terrain -lm
```

# Run
```
./terrain
```
---
Two files are generated: 
- `terrain.obj` - 3D model (uses `texturi.mtl` for materials)
- `map2d.bmp` - 2D map colored by altitude zone (water, sand, grass, rock, snow)
