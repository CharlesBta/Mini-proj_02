#define X_MIN -2.0
#define Y_MIN 1.0
#define X_MAX 1.0
#define Y_MAX -1.0

#define TARGET_X -0.99
#define TARGET_Y 0.3

#define LARGEUR 900
#define HAUTEUR 600
#define MAXVAL 255

// structure de l'image
typedef struct 
{
    char format[3]; // Format ex: "P3" + "\n" -> 3 chars
    int largeur; // Largeur de l'image en ASCII + "\0" -> max 4 chars
    int hauteur; // Hauteur de l'image en ASCII + "\0" -> max 4 chars
    int maxval; // Valeur max des couleurs en ASCII + "\n" -> max 3 chars ex: "255"

    double X_min;
    double X_max;
    double Y_min;
    double Y_max;
    float scale;
    double pixwidth;
    int* convrg;
} Mandel_pic;

typedef struct color {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} color;

Mandel_pic new_mandel(int width, int height, double Xmin, double Ymin, double scale);

int convergence(double x, double y);

color palette(int c);

void save_mandel(const char* filename, Mandel_pic mandel);

void free_mandel(Mandel_pic* mandel);