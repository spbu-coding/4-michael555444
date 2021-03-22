#include <stdio.h>
#include <stdbool.h>

#define MAX_COMPARE 100

struct BMP {

    int width;
    int height;
    unsigned int file_size;
    unsigned int offset;
    unsigned int image_size;
    unsigned int h_resolution;
    unsigned int v_resolution;
    unsigned int number_of_colors;
    unsigned int number_of_important_colors;
    unsigned short type;
    unsigned short bits_per_pixel;

};

int read_uint(unsigned int *x, FILE *file) {

    unsigned char input[4];

    if (fread(input, 4, 1, file) != 1) {

        if (feof(file) != 0) printf("\nBad end\n");

        else printf("\nCan't read file\n");

        return 0;

    }

    *x = (input[3] << 24 | input[2] << 16 | input[1] << 8 | input[0]);

    return 1;

}

int read_int(int *x, FILE *file) {

    char input[4];

    if (fread(input, 4, 1, file) != 1) {

        if (feof(file) != 0) {

            printf("\nBad end\n");

        }

        else {

            printf("\nCan't read file\n");

        }

        return 0;

    }

    *x = (input[3] << 24 | input[2] << 16 | input[1] << 8 | input[0]);

    return 1;

}

int read_ushort(unsigned short *x, FILE *file) {

    unsigned char input[2];

    if (fread(input, 2, 1, file) != 1) {

        if (feof(file) != 0) {

            printf("\nBad end\n");

        }

        else {

            printf("\nCan't read file\n");

        }

        return 0;

    }

    *x = (input[1] << 8 | input[0]);

    return 1;

}


bool check_type(unsigned short type) {

    char types[6][2] = {"BM"};

    if (memcmp(&type, types, 2) == 0) {

        return true;

    }

    return false;

}


int get_bmp_info(FILE *file, struct BMP *bmp_info) {

    unsigned int bmp_size;
    unsigned int reserved;
    unsigned int bmp_version;
    unsigned short number_of_color_planes;
    unsigned int compression_method;

    fseek(file, 0, SEEK_END);
    bmp_size = ftell(file);

    bool err = false;

    if (read_ushort(&bmp_info->type, file) == 0) {

        err = true;

    }

    if (!check_type(bmp_info->type)) {

        printf("\nBMP type only\n");

        return -1;

    }

    if (read_uint(&bmp_info->file_size, file) == 0) {

        err = true;

    }

    bmp_info->file_size = bmp_size;

    if (read_uint(&reserved, file) == 0) {

        err = true;

    }

    if (reserved != 0) {

        printf("\nUnsupported structure\n");

        return -2;

    }

    if (read_uint(&bmp_info->offset, file) == 0) {

        err = true;

    }


    if (read_uint(&bmp_version, file) == 0) {

        err = true;

    }

    if (read_int(&bmp_info->width, file) == 0) {

        err = true;

    }

    if (read_int(&bmp_info->height, file) == 0) {

        err = true;

    }


    if (read_ushort(&number_of_color_planes, file) == 0) {

        err = true;

    }

    if (number_of_color_planes != 1) {

        printf("\nUnsupported structure\n");

        return -2;

    }

    if (read_ushort(&bmp_info->bits_per_pixel, file) == 0) {

        err = true;

    }

    if (bmp_info->bits_per_pixel != 8 && bmp_info->bits_per_pixel != 24) {

        printf("\nOnly 8 or 24 bits BMP file supported\n");

        return -1;

    }

    if (read_uint(&compression_method, file) == 0) {

        err = true;

    }

    if (compression_method != 0) {

        printf("\nCan't read file\n");

        return -1;

    }

    if (read_uint(&bmp_info->image_size, file) == 0) {

        err = true;

    }

    if (bmp_info->image_size != 0) {

        unsigned int calculated_size = bmp_info->file_size - bmp_info->offset;

        if (calculated_size != bmp_info->image_size) {

            printf("\nDifferent file sizes\n");

            return -2;

        }
    }

    if (read_uint(&bmp_info->h_resolution, file) == 0) {

        err = true;

    }

    if (read_uint(&bmp_info->v_resolution, file) == 0) {

        err = true;

    }

    if (read_uint(&bmp_info->number_of_colors, file) == 0) {

        err = true;

    }
    if (read_uint(&bmp_info->number_of_important_colors, file) == 0) {

        err = true;

    }

    if (err) {

        printf("\nCan't read file\n");

        return -2;

    }

    return 0;

}

int compare_info(struct BMP *info1, struct BMP *info2) {

    if (abs(info1->width) != abs(info2->width)) {

        printf("\nDifferent width\n");

        return -1;

    }

    if (info1->height != info2->height) {

        printf("\nDifferent height\n");

        return -1;

    }

    if (info1->bits_per_pixel != info2->bits_per_pixel) {

        printf("\nDifferent bits\n");

        return -1;

    }

    if ((info1->number_of_colors != info2->number_of_colors) && (info1->bits_per_pixel == 8)) {

        printf("\nDifferent colors in palette\n");

        return -1;

    }

    return 0;

}

int compare_pixels(FILE *file1, FILE *file2, struct BMP *bmp_info, unsigned int *palette_size) {

    unsigned int count_of_pixels = 0;

    if (bmp_info->width < 0) {

        bmp_info->width = abs(bmp_info->width);

    }

    if (bmp_info->height < 0) {

        bmp_info->height = abs(bmp_info->height);

    }

    if (bmp_info->bits_per_pixel == 8) {

        unsigned int palette1[256];

        if (fread(palette1, sizeof(unsigned int), *palette_size, file1) != *palette_size) {

            printf("\nCan't read palette at first image\n");

            return -1;

        }

        unsigned int palette2[256];

        if (fread(palette2, sizeof(unsigned int), *palette_size, file2) != *palette_size) {

            printf("\nCan't read palette at second image\n");

            return -1;

        }

        unsigned char pixel1 = 0, pixel2 = 0;

        for (int i = 0; i < bmp_info->height; i++) {

            for (int j = 0; j < bmp_info->width; j++) {

                if (fread(&pixel1, 1, 1, file1) != 1) {

                    printf("\nCan't read pixels at first image\n");

                    return -1;

                }

                if (fread(&pixel2, 1, 1, file2) != 1) {

                    printf("\nCan't read pixels at second image\n");

                    return -1;

                }

                if (palette1[pixel1] != palette2[pixel2]) {

                    printf("%d %d\n", j, i);
                    count_of_pixels++;

                }

                if (count_of_pixels >= MAX_COMPARE) {

                    break;

                }
            }

            if (count_of_pixels >= MAX_COMPARE) {

                break;

            }
        }
    }

    else {

        char pixel1[3], pixel2[3];

        for (int i = 0; i < bmp_info->height; i++) {

            for (int j = 0; j < bmp_info->width; j++) {

                if (fread(pixel1, 1, 3, file1) != 3) {

                    printf("\nCan't read pixels at first image\n");

                    return -1;

                }

                if (fread(pixel2, 1, 3, file2) != 3) {

                    printf("\nCan't read pixels at second image\n");

                    return -1;

                }

                int rgbp1 = (0x00 << 24 | pixel1[2] << 16 | pixel1[1] << 8 | pixel1[0]);
                int rgbp2 = (0x00 << 24 | pixel2[2] << 16 | pixel2[1] << 8 | pixel2[0]);

                if (rgbp1 != rgbp2) {

                    printf("%d %d\n", j, i);
                    count_of_pixels++;

                }

                if (count_of_pixels >= MAX_COMPARE) {

                    break;

                }
            }

            if (count_of_pixels >= MAX_COMPARE) {

                break;

            }
        }
    }

    return 0;

}
