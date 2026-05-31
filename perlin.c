#include "perlin.h"

float fade(float t)
{
    return 6*pow(t, 5)-15*pow(t, 4)+10*pow(t, 3);
}

void init_perlin_noise(float gradientGrid[nmax][nmax], const long long seed)
{
    srand(seed);

    int angle[8]={45, 135, 225, 315, 0, 90, 180, 270};

    // atribuim fiecarui punct din matricea de gradient un unghi
    for(int i=0; i<nmax; i++)
        for(int j=0; j<nmax; j++)
            gradientGrid[i][j]=PI/180*angle[rand()%8]; //  PI/180*(rand()%360); // unghiul vectorului asociat punctului i j este ales random    
}

float perlin_noise(float points[nmax][nmax], float x, float y)
{  
    // gasim celula / coltul stanga sus
    int i=floor(y), j=floor(x);
    if(i>=nmax-1 || j>=nmax-1) // valoare in afara matricei
        return 0;

    float cx, cy, px, py; // vectorul corner si vectorul point
    float dot0, dot1, dot2, dot3; // produsele vectoriale pentru fiecare colt

    // coltul stanga sus
    cx=cos(points[i][j]);
    cy=sin(points[i][j]);
    px=x-j;
    py=y-i;
    dot0=cx*px+cy*py;

    // coltul dreapta sus
    cx=cos(points[i][j+1]);
    cy=sin(points[i][j+1]);
    px=x-(j+1);
    py=y-i;
    dot1=cx*px+cy*py;

    // coltul stanga jos
    cx=cos(points[i+1][j]);
    cy=sin(points[i+1][j]);
    px=x-j;
    py=y-(i+1);
    dot2=cx*px+cy*py;

    // coltul dreapta jos
    cx=cos(points[i+1][j+1]);
    cy=sin(points[i+1][j+1]);
    px=x-(j+1);
    py=y-(i+1);
    dot3=cx*px+cy*py;
 
    // aplicam fade pentru a disparea artefactele / patratele
    x=fade(x-j);
    y=fade(y-i);
    
    // interpolam liniar
    float top, bottom;
    top=(1-x)*dot0+x*dot1;
    bottom=(1-x)*dot2+x*dot3;
    return (1-y)*top+y*bottom;
}