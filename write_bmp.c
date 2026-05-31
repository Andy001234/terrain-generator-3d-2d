#include "write_bmp.h"

void createBMPFile(float altitude[1000][1000], int w, int h, int size, int rows_vert, int cols_vert, float waterlvl, float sandmaxlvl, float grassmaxlvl, float rockmaxlvl)
{
    // cream harta 2d asociata modelului 3d  
    FILE *imgf=fopen("map2d.bmp", "wb");
    if(!imgf)
    {
        printf("Eroare creare fisier bmp");
        exit(1);
    }  

    int filesize=54+3*w*h;
    unsigned char header[54]={
        // BMP header
        'B','M', // ID field
        0, 0, 0, 0, // lasam doar valori de 0 si completam dupa octetii 2 3 4 5. Size of the BMP file
        0,0, //Unused
        0,0, //Unused
        54,0,0,0, // Offset where the pixel array (bitmap data) can be found

        //DIB header
        40,0,0,0, // Number of bytes in the DIB header (from this point)
        0,0,0,0, // lasam doar valori de 0 si completam dupa octetii 18 19 20 21. Width of the bitmap in pixels
        0,0,0,0, // lasam doar valori de 0 si completam dupa octetii 22 23 24 25. Height of the bitmap in pixels. Positive for bottom to top pixel order
        1,0, // Number of color planes being used
        24,0 // Number of color planes being used
    };
    // scriem byte cu byte in header
    header[2]=(unsigned char)(filesize);
    header[3]=(unsigned char)(filesize>>8);
    header[4]=(unsigned char)(filesize>>16);
    header[5]=(unsigned char)(filesize>>24);

    header[18]=(unsigned char)(w);
    header[19]=(unsigned char)(w>>8);
    header[20]=(unsigned char)(w>>16);
    header[21]=(unsigned char)(w>>24);

    header[22]=(unsigned char)(h);
    header[23]=(unsigned char)(h>>8);
    header[24]=(unsigned char)(h>>16);
    header[25]=(unsigned char)(h>>24);
    fwrite(header, 1, 54, imgf);
 
    float maxi=-1e9, mini=1e9;
    for(int y=rows_vert-1; y>=0; y--)
        for(int x=0; x<=cols_vert-1; x++)
        {    
            float avg=(altitude[y][x]+altitude[y+1][x]+altitude[y][x+1]+altitude[y+1][x+1])/4.;
            if(avg>maxi)
                maxi=avg;
            if(avg<mini)
                mini=avg;
        }

    for(int y=h-1; y>=0; y--) // bmp creeaza imaginea de jos in sus deci incepem de la ultima linie a hartii pana la prima pentru a inversa efectul
    {
        for(int x=0; x<=w-1; x++)
        {
            int i=y/size; // in ce patrat al hartii ne aflam
            int j=x/size;

            float avgcol=(altitude[i][j]+altitude[i+1][j]+altitude[i][j+1]+altitude[i+1][j+1])/4.; // normalizam val z a unui varf din intervalul [mini, maxi] in intervalul [0, 1]

            unsigned char bgr1[3]={255, 100, 0}; // water
            unsigned char bgr2[3]={0, 200, 255}; // sand
            unsigned char bgr3[3]={0, 255, 0};  // grass
            unsigned char bgr4[3]={128, 128, 128}; // rock
            unsigned char bgr5[3]={255, 255, 255};  // snow
            
            unsigned char bgr[3]={ 0, 0, 0 };
            // folosim aceleasi valori de la culorile triunghiiurilor pentru a avea culori asemanatoare si pe harta 2d
            if(avgcol<waterlvl)
            {
                avgcol=(avgcol-mini)/(waterlvl-mini);
                // avgcol este acum in intervalul 0 1 dar vrem sa fie in intervalul 0.5 1 pentru a nu aparea gradienti de la negru la culoarea regiunii
                avgcol=0.5+0.5*avgcol;

                bgr[0]=(int)bgr1[0]*avgcol;
                bgr[1]=(int)bgr1[1]*avgcol;
                bgr[2]=(int)bgr1[2]*avgcol;
            }
            else if(avgcol>=waterlvl && avgcol<sandmaxlvl)
            {
                avgcol=(avgcol-waterlvl)/(sandmaxlvl-waterlvl);
                avgcol=0.5+0.5*avgcol;

                bgr[0]=(int)bgr2[0]*avgcol;
                bgr[1]=(int)bgr2[1]*avgcol;
                bgr[2]=(int)bgr2[2]*avgcol;
            }
            else if(avgcol>=sandmaxlvl && avgcol<grassmaxlvl)
            {
                avgcol=(avgcol-sandmaxlvl)/(grassmaxlvl-sandmaxlvl);
                avgcol=0.5+0.5*avgcol;

                bgr[0]=(int)bgr3[0]*avgcol;
                bgr[1]=(int)bgr3[1]*avgcol;
                bgr[2]=(int)bgr3[2]*avgcol;
            }
            else if(avgcol>=grassmaxlvl && avgcol<rockmaxlvl)
            {
                avgcol=(avgcol-grassmaxlvl)/(rockmaxlvl-grassmaxlvl);
                avgcol=0.5+0.5*avgcol;

                bgr[0]=(int)bgr4[0]*avgcol;
                bgr[1]=(int)bgr4[1]*avgcol;
                bgr[2]=(int)bgr4[2]*avgcol;
            }
            else if(avgcol>=rockmaxlvl)
            {
                avgcol=(avgcol-rockmaxlvl)/(maxi-rockmaxlvl);
                avgcol=0.5+0.5*avgcol;

                bgr[0]=(int)bgr5[0]*avgcol;
                bgr[1]=(int)bgr5[1]*avgcol;
                bgr[2]=(int)bgr5[2]*avgcol;
            }
            fwrite(bgr, 1, 3, imgf);
        }   
        // adaugam padding ul
        int padding=(4-(3*w)%4)%4;
        unsigned char padd[3]={0, 0, 0};
        fwrite(padd, 1, padding, imgf);
    }
    fclose(imgf);
}