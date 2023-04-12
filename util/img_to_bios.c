#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>

#define STB_IMAGE_IMPLEMENTATION
#include "./stbi_image/stbi_image.h"

typedef struct {
    unsigned char x, y, z;
} vec3;

//https://en.wikipedia.org/wiki/Video_Graphics_Array
//I started putting in some, but then my hands got tired so I stopped. I'll add some later.
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

double find_distance(vec3 x, vec3 y)
{
    return sqrt(pow(x.x - y.x, 2) + pow(x.y - y.y, 2) + pow(x.z - y.z, 2));
}

unsigned char find_color_code(vec3 color)
{
    int closest_index = 0;
    double closest = 10000.0f;
    double distance;
    for (int i = 0; i < (sizeof(bios_color_codes) / sizeof(vec3)); i++) {
        distance = find_distance(color, bios_color_codes[i]);
        if (distance < closest) {
            closest = distance;
            closest_index = i;
        }
    }

    return (unsigned char) closest_index;
}

void usage()
{
    fprintf(stderr, "Single argument: file name. Use this program to convert an image into BIOS color codes.\n");
    fprintf(stderr, "This program will simply output the new binary form as output.bin\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv)
{
    if (argc != 2) {
        usage();
    }

    unsigned char *output_buffer;
    unsigned char *image_data;
    int width, height, channels;
    image_data = stbi_load(argv[1], &width, &height, &channels, 0);
    if (image_data == NULL) {
        perror("stbi_load()");
        exit(EXIT_FAILURE);
    }

    if (channels != 3) {
        fprintf(stderr, "warning: this image has %d channels, only accepts 3.\n", channels);
    }

    fprintf(stderr, "Width: %d\n", width);
    fprintf(stderr, "Height: %d\n", height);

    output_buffer = malloc(width * height); // remember that we are not using rgb
    if (output_buffer == NULL) {
        perror("malloc()");
        exit(EXIT_FAILURE);
    }

    for (int i = 0, j = 0; i < width * height; j += channels, i++) {
        vec3 curr_pixel;
        curr_pixel.x = image_data[j];
        curr_pixel.y = image_data[j+1];
        curr_pixel.z = image_data[j+2];
        output_buffer[i] = find_color_code(curr_pixel);
    }

    int output_fd = open("output.bin", O_WRONLY | O_CREAT);
    write(output_fd, output_buffer, width * height);
    close(output_fd);

    free(output_buffer);
    stbi_image_free(image_data);
    exit(EXIT_SUCCESS);
}