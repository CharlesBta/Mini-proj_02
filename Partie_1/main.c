#include <stdio.h>
#include <stdlib.h>
#include "./Utils/utils.h"

int main(void) 
{
    PixelMap* pixelMap = createPixelMap(LARGEUR, HAUTEUR, MAXVAL);
    if (pixelMap == NULL) {
        fprintf(stderr, "Erreur lors de la création de la PixelMap\n");
        return 1;
    }

    drawMandelbrot(pixelMap, 0, 0);
    writePixelMapToFile("image.ppm", pixelMap);
    freePixelMap(pixelMap);
    return 0;
}