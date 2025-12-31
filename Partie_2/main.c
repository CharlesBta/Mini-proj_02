#include <stdio.h>
#include <stdlib.h>
#include "./Utils/utils.h"

int main(void) 
{
    Mandel_pic mandel = new_mandel(LARGEUR, HAUTEUR, X_MIN, Y_MIN, 1.0f);
    save_mandel("test.ppm", mandel);
    free_mandel(&mandel);
    return 0;
}