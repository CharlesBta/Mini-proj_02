#include <stdio.h>
#include <stdlib.h>
#include "./Utils/utils.h"
#include <time.h>


Mandel_pic generate_mandelbrot(double x, double y, double scale, char* filename, Mandel_pic* previous_mandel) {
    Mandel_pic mandel = new_mandel(LARGEUR, HAUTEUR, x - (1.0 * scale), y - (1.0 * scale), scale, previous_mandel);
    save_mandel(filename, mandel);
    return mandel;
}

int main(int argc, char *argv[]) {
    double x = -0.743643887037151;
    double y = 0.13182590420533;
    double scale = 0.01;
    int nb_files = 100;
    int first_file = 1;
    char filename[256];
    Mandel_pic* mandel = NULL;
    Mandel_pic temp_mandel;

    if (argc == 6) {
        x = atof(argv[1]);
        y = atof(argv[2]);
        scale = atof(argv[3]);
        nb_files = atoi(argv[4]);
        first_file = atoi(argv[5]);
    } else if (argc != 1) {
        printf("Usage: %s [Xmin Ymin Scale n_files first_file] (all as numbers)\n", argv[0]);
        return 1;
    }

    if (nb_files > 25) {
        int total_files = nb_files;
        int nb_processes = 4;
        int files_per_processes = total_files / nb_processes;
        for (int p = 0; p < nb_processes; p++) {
            int start_num = first_file + p * files_per_processes;
            int count = (p == nb_processes - 1) ? (total_files - p * files_per_processes) : files_per_processes;
            double tmp_scale = scale;
            for (int j = 0; j < p * files_per_processes; j++) {
                tmp_scale *= 0.95;
            }
            char cmd[512];
            snprintf(cmd, sizeof(cmd), "./main %lf %lf %lf %d %d &", x, y, tmp_scale, count, start_num);
            printf("Launching: %s\n", cmd);
            system(cmd);
        }
        return 0;
    }

    for (int i = 0; i < nb_files; i++) {
        sprintf(filename, "./output/mandelbrot_%d.ppm", first_file + i);
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

