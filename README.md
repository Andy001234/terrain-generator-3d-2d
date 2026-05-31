# Terrain generator
Generates a 3d terrain using Perlin Noise.

### Example of generated obj file
<img src="screenshots/3d_terrain_screenshot.png" width="600"/>

### Example of generated 2D map
<img src="screenshots/2d_map_screenshot.bmp" width="600"/>

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
