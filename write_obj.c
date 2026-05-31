#include "write_obj.h"

void createObjFile(float altitude[1000][1000], int w, int h, int size, int rows_vert, int cols_vert, float waterlvl, float sandmaxlvl, float grassmaxlvl, float rockmaxlvl)
{
    FILE *file=fopen("terrain.obj", "wt");
    if(!file)
    {
        printf("Eroare creare fisier obj");
        exit(1);
    }

    // folosim fisierul de texturi
    fprintf(file, "mtllib texturi.mtl\n");

    // scriere in fisiere
    for(int i=0; i*size<=h; i++)
        for(int j=0; j*size<=w; j++) 
            fprintf(file, "v %f %f %f\n", 1.*j*size, altitude[i][j], 1.*i*size); 
        
    // nivelul apei
    int total_verts=rows_vert*cols_vert;

    fprintf(file, "v 0 0 0\nv %f 0 0\nv 0 0 %f\nv %f 0 %f\n", 1.*w, 1.*h, 1.*w, 1.*h);
    fprintf(file, "usemtl water\n");
    fprintf(file, "f %d %d %d\nf %d %d %d\n", total_verts+1, total_verts+3, total_verts+2, total_verts+3, total_verts+4, total_verts+2);
    
    for(int k=0; k<4; k++) // 4 materiale
    {
        if(k==0)
            fprintf(file, "usemtl sand\n");
        else if(k==1)
            fprintf(file, "usemtl grass\n");
        else if(k==2)
            fprintf(file, "usemtl rock\n");
        else if(k==3)
            fprintf(file, "usemtl snow\n");

        
        for(int i=1; i<=rows_vert-1; i++)     // incepem de la 1 pentru ca varfurile sunt indexate de la 1 si mergem pana la penultima linie si
            for(int j=1; j<=cols_vert-1; j++) //   coloana a varfurilor pt ca din ele putem afla tot patratul pe care il impartim in 2 triunghiuri
            {
                float avgColor1=(altitude[i-1][j-1]+altitude[i][j-1]+altitude[i-1][j])/3.;
                float avgColor2=(altitude[i][j-1]+altitude[i][j]+altitude[i-1][j])/3.;

                if(avgColor1<sandmaxlvl && k==0)  
                    fprintf(file, "f %d %d %d\n", (i-1)*cols_vert+j, i*cols_vert+j, (i-1)*cols_vert+j+1);  
                
                if(avgColor2<sandmaxlvl && k==0)
                    fprintf(file, "f %d %d %d\n", i*cols_vert+j, i*cols_vert+j+1, (i-1)*cols_vert+j+1);

                if(avgColor1>=sandmaxlvl && avgColor1<grassmaxlvl && k==1)
                    fprintf(file, "f %d %d %d\n", (i-1)*cols_vert+j, i*cols_vert+j, (i-1)*cols_vert+j+1);  

                if(avgColor2>=sandmaxlvl && avgColor2<grassmaxlvl && k==1)
                    fprintf(file, "f %d %d %d\n", i*cols_vert+j, i*cols_vert+j+1, (i-1)*cols_vert+j+1);

                if(avgColor1>=grassmaxlvl && avgColor1<rockmaxlvl && k==2)
                    fprintf(file, "f %d %d %d\n", (i-1)*cols_vert+j, i*cols_vert+j, (i-1)*cols_vert+j+1);  

                if(avgColor2>=grassmaxlvl && avgColor2<rockmaxlvl && k==2)
                    fprintf(file, "f %d %d %d\n", i*cols_vert+j, i*cols_vert+j+1, (i-1)*cols_vert+j+1);

                if(avgColor1>=rockmaxlvl && k==3)
                    fprintf(file, "f %d %d %d\n", (i-1)*cols_vert+j, i*cols_vert+j, (i-1)*cols_vert+j+1);  

                if(avgColor2>=rockmaxlvl && k==3)
                    fprintf(file, "f %d %d %d\n", i*cols_vert+j, i*cols_vert+j+1, (i-1)*cols_vert+j+1);
            }
    }

    fclose(file);
}