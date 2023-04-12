#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stbi_image/stbi_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "./stbi_image/stbi_write.h"

typedef struct {
    unsigned char x, y, z;
} vec3;

//https://en.wikipedia.org/wiki/Video_Graphics_Array
vec3 bios_color_codes[] =
{
    {0, 0, 0}, {0, 0, 170}, {0, 170, 0}, {0, 170, 170},
    {170, 0, 0}, {170, 0, 170}, {170, 85, 0}, {170, 170, 170},
    {85, 85, 85}, {85, 85, 255}, {85, 255, 85}, {85, 255, 255},
    {255, 85, 85}, {255, 85, 255}, {255, 255, 85}, {255, 255, 255},
    {0, 0, 0}, {16, 16, 16}, {32, 32, 32}, {53, 53, 53},
    {69, 69, 69}, {85, 85, 85}, {101, 101, 101}, {117, 117, 117},
    {138, 138, 138}, {154, 154, 154}, {170, 170, 170}, {186, 186, 186},
    {202, 202, 202}, {223, 223, 223}, {239, 239, 239}, {255, 255, 255},
    {0, 0, 255}, {65, 0, 255}, {130, 0, 255}, {199, 0, 255},
    {255, 0, 255}, {255, 0, 190}, {255, 0, 130}, {255, 0, 65},
    {255, 0, 255}, {255, 65, 0}, {255, 130, 0}, {255, 190, 0},
    {255, 255, 0}, {190, 255, 0}, {130, 255, 0}, {65, 255, 0},
    {0, 255, 0}, {0, 255, 65}, {0, 255, 130}, {0, 255, 190},
    {0, 255, 255}, {0, 190, 255}, {0, 130, 255}, {0, 65, 255},
    {130, 130, 255}, {158, 130, 255}, {190, 130, 255}, {223, 130, 255},
    {255, 130, 255}, {255, 130, 223}, {255, 130, 190}, {255, 130, 158},
    {255, 130, 130}, {255, 158, 130}, {255, 190, 130}, {255, 223, 130},
    {255, 255, 130}, {223, 255, 130}, {190, 255, 130}, {158, 255, 130},
    {130, 255, 130}, {130, 255, 158}, {130, 255, 190}, {130, 255, 223},
    {130, 255, 255}, {130, 223, 255}, {130, 190, 255}, {130, 158, 255},
};

/* :)))))) I wanted to do this fast, not safe as you can tell*/
int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "missing filename.\n");
        exit(EXIT_FAILURE);
    }

    int file_fd = open(argv[1], O_RDONLY);
    unsigned char f[50000];
    size_t s;
    s = read(file_fd, f, 50000);
    fprintf(stderr, "read: %lu bytes\n", s);
    close(file_fd);
    unsigned char output[s * 3];
    for (size_t i = 0, j = 0; i < s; i++, j += 3) {
        vec3 tmp = bios_color_codes[f[i]];
        output[j] = tmp.x;
        output[j+1] = tmp.y;
        output[j+2] = tmp.z;
    }

    stbi_write_png("output.png", 120, 120, 3, output, 120 * 3);
    return 0;

}