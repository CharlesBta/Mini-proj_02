#include <stdio.h>
#include <stdlib.h>
#include "./Utils/utils.h"
#include <time.h>


Mandel_pic generate_mandelbrot(double x, double y, double scale, char* filename, Mandel_pic* previous_mandel) {
    Mandel_pic mandel = new_mandel(LARGEUR, HAUTEUR, x - (1.0 * scale), y - (1.0 * scale), scale, previous_mandel);
    save_mandel(filename, mandel);
    return mandel;
}

int main(void) {
    double x = -0.743643887037151;
    double y = 0.13182590420533;
    double scale = 0.01;
    char filename[256];
    Mandel_pic* mandel = NULL;
    Mandel_pic temp_mandel; 
    for (int i = 1; i < 100; i++) {
        sprintf(filename, "./output/mandelbrot_%d.ppm", i);
        temp_mandel = generate_mandelbrot(x, y, scale, filename, mandel);
        if (mandel == NULL) {
            mandel = (Mandel_pic*)malloc(sizeof(Mandel_pic));
        } else {
            free_mandel(mandel);
        }
        *mandel = temp_mandel;
        scale *= 0.95;
    }

    if (mandel != NULL) {
        free_mandel(mandel);
        free(mandel);
    }

    return 0;
}

