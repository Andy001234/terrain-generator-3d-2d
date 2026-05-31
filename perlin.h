#include <math.h>
#include <stdlib.h>
#include <time.h>

#define nmax 100
#define PI (4*atan(1))

float fade(float t);
void init_perlin_noise(float points[nmax][nmax], const long long seed);
float perlin_noise(float points[nmax][nmax], float x, float y);