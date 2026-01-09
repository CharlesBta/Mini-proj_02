#include <stdio.h>
#include <stdlib.h>
#include "./Utils/utils.h"
#include <time.h>

int main(void) {
    srand( time( NULL ) );
    Mandel_pic mandel = new_mandel(LARGEUR, HAUTEUR, -0.755232, 0.121387, 0.01, NULL);
    save_mandel("interpolatedv2.ppm", mandel);
    Mandel_pic mandel2 = new_mandel(LARGEUR, HAUTEUR, -0.752914, 0.123475, 0.00738, &mandel);
    save_mandel("interpolatedv2_2.ppm", mandel2);
    free_mandel(&mandel);
    free_mandel(&mandel2);
    return 0;
}
