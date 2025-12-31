#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"

#define MAX_ITER 85

PixelMap *createPixelMap(int largeur, int hauteur, int maxval)
{
    PixelMap *pixelMap = (PixelMap *)malloc(sizeof(PixelMap));
    if (pixelMap == NULL)
    {
        return NULL;
    }

    snprintf(pixelMap->format, sizeof(pixelMap->format), "P6");

    pixelMap->largeur = largeur;
    pixelMap->hauteur = hauteur;
    pixelMap->maxval = maxval;

    pixelMap->pixels = (Pixel *)malloc(largeur * hauteur * sizeof(Pixel));
    if (pixelMap->pixels == NULL)
    {
        free(pixelMap);
        return NULL;
    }

    return pixelMap;
}

void freePixelMap(PixelMap *pixelMap)
{
    if (pixelMap != NULL)
    {
        free(pixelMap->pixels);
        free(pixelMap);
    }
}

void writePixelMapToFile(const char *filename, const PixelMap *pixelMap)
{
    FILE *fichier = fopen(filename, "wb");
    if (fichier == NULL)
    {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
    fprintf(fichier, "%s\n", pixelMap->format);
    fprintf(fichier, "%d %d\n", pixelMap->largeur, pixelMap->hauteur);
    fprintf(fichier, "%d\n", pixelMap->maxval);
    int largeur = pixelMap->largeur;
    int hauteur = pixelMap->hauteur;
    fwrite(pixelMap->pixels, sizeof(Pixel), largeur * hauteur, fichier);
    fclose(fichier);
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

void pixel_to_coord(int px, int py, double *x, double *y)
{
    *x = X_MIN + (X_MAX - X_MIN) * px / (LARGEUR - 1);
    *y = Y_MIN + (Y_MAX - Y_MIN) * py / (HAUTEUR - 1);
}

void drawMandelbrot(PixelMap *pixelMap, unsigned char r, unsigned char b)
{
    int largeur = pixelMap->largeur;
    int hauteur = pixelMap->hauteur;

    for (int py = 0; py < hauteur; py++)
    {
        for (int px = 0; px < largeur; px++)
        {
            double x, y;
            pixel_to_coord(px, py, &x, &y);
            int n = convergence(x, y);

            int index = py * largeur + px;
            color col = palette(n);
            pixelMap->pixels[index].r = col.r;
            pixelMap->pixels[index].g = col.g;
            pixelMap->pixels[index].b = col.b;
        }
    }
}