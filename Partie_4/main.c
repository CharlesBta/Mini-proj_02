#include <stdio.h>
#include <stdlib.h>
#include "./Utils/utils.h"
#include <time.h>
#include <math.h>


Mandel_pic generate_mandelbrot(double x, double y, double scale, char* filename, Mandel_pic* previous_mandel) {
    Mandel_pic mandel = new_mandel(LARGEUR, HAUTEUR, x - (1.0 * scale), y - (1.0 * scale), scale, previous_mandel);
    save_mandel(filename, mandel);
    return mandel;
}

int main(void) {
    double x_t = -0.743643887037151;
    double y_t = 0.13182590420533;
    double x0 = -2.;
    double y0 = -1.;
    double scale0 = 1.;
    double factor = 0.95;
    double scale = scale0;
    char filename[256];
    Mandel_pic* mandel = NULL;
    Mandel_pic temp_mandel; 
    
    for (int i = 1; i < 200; i++) {
        printf("Generating image %d...\n", i);
        sprintf(filename, "./output/mandelbrot_%d.ppm", i);
        double x = (x0 - x_t) * pow(factor, i) + x_t;
        double y = (y0 - y_t) * pow(factor, i) + y_t;
        temp_mandel = new_mandel(LARGEUR, HAUTEUR, x, y, scale, mandel);
        save_mandel(filename, temp_mandel);
        if (mandel == NULL) {
            mandel = (Mandel_pic*)malloc(sizeof(Mandel_pic));
        } else {
            free_mandel(mandel);
        }
        *mandel = temp_mandel;
        scale = scale0 * pow(factor, i);
    }

    if (mandel != NULL) {
        free_mandel(mandel);
        free(mandel);
    }

    return 0;
}

