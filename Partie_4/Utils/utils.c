#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_ITER 85

void pixel_to_coord(int px, int py, double *x, double *y, double X_min, double Y_min, double X_max, double Y_max, int width, int height)
{
    *x = X_min + (X_max - X_min) * px / (LARGEUR - 1);
    *y = Y_min + (Y_max - Y_min) * py / (HAUTEUR - 1);
}

Mandel_pic new_mandel(int width, int height, double Xmin, double Ymin, double scale, Mandel_pic* previous_mandel)
{
    Mandel_pic mandel;

    snprintf(mandel.format, sizeof(mandel.format), "P6");

    mandel.largeur = width;
    mandel.hauteur = height;
    mandel.maxval = MAXVAL;

    mandel.X_min = Xmin;
    mandel.Y_min = Ymin;
    mandel.X_max = Xmin + (3.0 * scale);
    mandel.Y_max = Ymin - (3.0 * scale * height / width);
    mandel.scale = scale;
    mandel.pixwidth = 3.0 * scale / width;

    mandel.convrg = (int*)malloc(width * height * sizeof(int));
    if (mandel.convrg == NULL)
    {
        fprintf(stderr, "Erreur d'allocation mémoire pour convrg\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            double x, y;
            pixel_to_coord(i, j, &x, &y, mandel.X_min, mandel.Y_min, mandel.X_max, mandel.Y_max, mandel.largeur, mandel.hauteur);
            if (previous_mandel != NULL) {
                if (interpolate(*previous_mandel, x, y) != -1) {
                    mandel.convrg[j * width + i] = interpolate(*previous_mandel, x, y);
                    continue;
                }
            }
            mandel.convrg[j * width + i] = convergence(x, y);
        }
    }

    return mandel;
}

int convergence(double x, double y)
{
    double complex z = x + I * y;
    double complex u = 0 + 0 * I;

    for (int n = 0; n < MAX_ITER; n++)
    {
        u = u * u + z;
        if (cabs(u) >= 2.0)
        {
            return n;
        }
    }
    return 0;
}

color palette(int c)
{
    color col = {0, 0, 0};
    c = (c*5) % (1785-255); 

    if (c < 255)
    {
        col.r = (unsigned char)(c);
        col.g = (unsigned char)(0);
        col.b = (unsigned char)(0);

        return col;
    }
    if (c <= 510)
    {
        col.r = (unsigned char)(255);
        col.g = (unsigned char)(c - 255);
        col.b = (unsigned char)(0);

        return col;
    }
    if (c <= 765)
    {
        col.r = (unsigned char)(765 - c);
        col.g = (unsigned char)(255);
        col.b = (unsigned char)(0); 
        return col;
    }
    if (c <= 1020)
    {
        col.r = (unsigned char)(0);
        col.g = (unsigned char)(255);
        col.b = (unsigned char)(c - 765);
        return col;
    }
    if (c <= 1275)
    {
        col.r = (unsigned char)(0);
        col.g = (unsigned char)(1275 - c);
        col.b = (unsigned char)(255);           
        return col;
    }
    if (c <= 1530)
    {
        col.r = (unsigned char)(c - 1275);
        col.g = (unsigned char)(0);
        col.b = (unsigned char)(255);           
        return col;
    }
    return col;
}

int interpolate(Mandel_pic mandel, double x, double y) {
    if ((rand() % 100) < 1) {
        return -1;
    }

    if (x < mandel.X_min || x > mandel.X_max || y < mandel.Y_min || y < mandel.Y_max) {
        return -1;
    }

    double fx = (x - mandel.X_min) / mandel.pixwidth;
    double fy = -(y - mandel.Y_min) / mandel.pixwidth;

    int i = (int)round(fx);
    int j = (int)round(fy);
    
    if ((i <= 0) || (i >= LARGEUR - 1) || (j <= 0) || (j >= HAUTEUR - 1)) {
        return -1;
    }
    
    int left = mandel.convrg[j * mandel.largeur + i - 1];
    int right = mandel.convrg[j * mandel.largeur + i + 1];   
    int bottom = mandel.convrg[(j + 1) * mandel.largeur + i];
    int top = mandel.convrg[(j - 1) * mandel.largeur + i];

    if (!(left == right && right == bottom && bottom == top)) {
        return -1;
    }
    
    return mandel.convrg[j * mandel.largeur + i];
}


void save_mandel(const char* filename, Mandel_pic mandel)
{
    FILE* file = fopen(filename, "wb");
    if (file == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    fprintf(file, "%s\n", mandel.format);
    fprintf(file, "%d %d\n", mandel.largeur, mandel.hauteur);
    fprintf(file, "%d\n", mandel.maxval);   

    for (int j = 0; j < mandel.hauteur; j++)
    {
        for (int i = 0; i < mandel.largeur; i++)
        {
            int c = mandel.convrg[j * mandel.largeur + i];
            color col = palette(c);
            fwrite(&col, sizeof(color), 1, file);
        }
    }
    fclose(file);
}

void free_mandel(Mandel_pic* mandel)
{
    if (mandel != NULL)
    {
        free(mandel->convrg);
        mandel->convrg = NULL;
    }
}