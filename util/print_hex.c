#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void usage()
{
    fprintf(stderr, "print hex of file.\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        usage();
    }

    unsigned char c;
    int file_fd = open(argv[1], O_RDONLY);
    while (read(file_fd, &c, 1) == 1) {
        fprintf(stdout, "0x%x,", c);
    }
    fprintf(stdout, "\n");
    close(file_fd);
}