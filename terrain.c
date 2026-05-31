#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

#include "perlin.h"
#include "write_obj.h"
#include "write_bmp.h"

#define MAX_VERTS 1000000
 
float altitude[1000][1000];
float points[nmax][nmax]; 

 
int main()
{     
    int w=900, h=900, size=5;
    int rows_vert=h/size+1; // +1 pentru ca numaram intersectiile liniilor unei matrici w/size x h/size, nu celulele matricei
    int cols_vert=w/size+1; // numaram nr de varfuri de pe linie/coloana
    float ampl=300, pixelsizeinperlin=size*0.1/35; // sau 0.05
    float waterlvl=0, sandmaxlvl=10, grassmaxlvl=45, rockmaxlvl=120, snowlvl=120;
    long long seed=time(NULL);

    if(rows_vert*cols_vert>MAX_VERTS)
    {
        printf("Too many vertices");
        exit(1);
    }
 
    init_perlin_noise(points, seed);
    for(int i=0; i<=rows_vert-1; i++) 
        for(int j=0; j<=cols_vert-1; j++)  
            altitude[i][j]=ampl*perlin_noise(points, j*pixelsizeinperlin, i*pixelsizeinperlin);

    createObjFile(altitude, w, h, size, rows_vert, cols_vert, waterlvl, sandmaxlvl, grassmaxlvl, rockmaxlvl);
    createBMPFile(altitude, w, h, size, rows_vert, cols_vert, waterlvl, sandmaxlvl, grassmaxlvl, rockmaxlvl);

    printf("Fisierele au fost create cu succes!\n");
    
    return 0;
}
// gcc terrain.c perlin.c perlin.h write_obj.c write_obj.h write_bmp.c write_bmp.h -o terrain -lm