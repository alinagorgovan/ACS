#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMOOTH 0
#define BLUR 1
#define SHARPEN 2
#define MEAN 3
#define EMBOSS 4

const int factors[6] = {9, 16, 3, 1, 1, 1};
const int filters[6][3][3] = {
    {{1, 1, 1}, {1, 1, 1}, {1, 1, 1}},
    {{1, 2, 1}, {2, 4, 2}, {1, 2, 1}},
    {{0, -2, 0}, {-2, 11, -2}, {0, -2, 0}},
    {{-1, -1, -1}, {-1, 9, -1}, {-1, -1, -1}},
    {{0, 1, 0}, {0, 0, 0}, {0, -1, 0}},
    {{0, 0, 0}, {0, 1, 0}, {0, 0, 0}}
};

typedef struct {
    char type[3];   // P5(Grayscale) or P6c(RGB)
    int width;
    int height;
    int max_value;  // Maximum value of a pixel
    int number_colors; // 1(Grayscale) or 3(RGB)
    unsigned char *data;    // Matrix of pixels
} image;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_pixel;

/*
    Allocs the matrix for pixels depending on the type of the image;
*/
void allocImage(image *img) {
    if (!strcmp("P5", img->type)) {
        // Grayscale image
        img->number_colors = 1;

    } else if (!strcmp("P6", img->type)) {
        // RGB image
        img->number_colors = 3;
    }
    img->data = (unsigned char *) malloc (img->height * img->width * img->number_colors );
}

image *readInput(const char *fileName) {
    FILE *fin = fopen(fileName, "r");

    image *img = (image *) malloc (sizeof(image));
    // Read image header
    fscanf(fin, "%s\n", img->type);
    fscanf(fin, "%d %d\n", &(img->width), &(img->height));
    fscanf(fin, "%d\n", &(img->max_value));
    // Alloc matrix of pixels
    allocImage(img);
    // Read pixels values from input
    fread(img->data, img->number_colors , img->height * img->width, fin);

    fclose(fin);
    return img;
}

void writeData(const char * fileName, image *img) {
    FILE *fout = fopen(fileName, "w");
    int i;
    // Write image header
    fprintf(fout, "%s\n", img->type);
    fprintf(fout, "%d %d\n", img->width, img->height);
    fprintf(fout, "%d\n", img->max_value);
    // Write pixels values
    for (i = 0; i < img->height; i++) {
        fwrite((unsigned char*)img->data + i * img->number_colors * img->width, img->number_colors, img->width, fout);
    }

    fclose(fout);

}

void apply_filter(int filter, void **matrix, int height, int width, int number_colors) {
    // depending on the type of the image apply the given filter
    if (number_colors == 1)  {
        unsigned char **temp = (unsigned char **) malloc (height * sizeof(unsigned char *));
        for (int i = 0; i < height; i++) {
            temp[i] = (unsigned char *) malloc (width);
        }
        unsigned char **m = (unsigned char **) matrix;
        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {
                float sum = 0;
                for(int row = i - 1; row <= i + 1; row++) {
                    for(int col = j - 1; col <= j + 1; col++) {
                        // Compute the sum of every element multiplied by the filter matrix
                        sum += ((1.0f / factors[filter]) * filters[filter][row - (i-1)][col - (j-1)]) * m[row][col];
                    }
                }
                // Add to temp matrix
                temp[i][j] = sum;
            }
        }
        // After all modifications add to final matrix
        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {
                ((unsigned char**)matrix)[i][j] = temp[i][j];
            }
        }

    } else if (number_colors == 3) {
        rgb_pixel **m = (rgb_pixel **) matrix;
        rgb_pixel **temp = (rgb_pixel **) malloc (height * sizeof(rgb_pixel *));
        for (int i = 0; i < height; i++) {
            temp[i] = (rgb_pixel *) malloc (width * sizeof(rgb_pixel));
        }
        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {
                float sum_r = 0, sum_g = 0, sum_b = 0;
                for(int row = i - 1; row <= i + 1; row++) {
                    for(int col = j - 1; col <= j + 1; col++) {
                        sum_r += ((1.0f / factors[filter]) * filters[filter][row - (i-1)][col - (j-1)]) * m[row][col].r;
                        sum_g += ((1.0f / factors[filter]) * filters[filter][row - (i-1)][col - (j-1)]) * m[row][col].g;
                        sum_b += ((1.0f / factors[filter]) * filters[filter][row - (i-1)][col - (j-1)]) * m[row][col].b;
                    }
                }
                // Add to temporary matrix
                temp[i][j].r = sum_r;
                temp[i][j].g = sum_g;
                temp[i][j].b = sum_b;
            }
        }
        // After all modifications add to final matrix
        for (int i = 1; i < height - 1; i++) {
            for (int j = 1; j < width - 1; j++) {
                ((rgb_pixel**)matrix)[i][j].r = temp[i][j].r;
                ((rgb_pixel**)matrix)[i][j].g = temp[i][j].g;
                ((rgb_pixel**)matrix)[i][j].b = temp[i][j].b;
            }
        }
    }
}
/* Transform the buffer into a matrix with the given dimensions*/
void **buffer_to_matrix(unsigned char *buff, int width, int height, int number_colors) {
    void **m = (void **) malloc (height * sizeof(void *));
    for (int i = 0; i < height; i++) {
        m[i] = (void *) malloc (width * number_colors);
        memcpy(m[i], buff + i * width * number_colors, width * number_colors);
    }
    return m;
}
/* Transform the matrix into a buffer */
unsigned char *matrix_to_buffer(void **matrix, int width, int height, int number_colors) {
    unsigned char *buff = (unsigned char *) malloc (height * width * number_colors);
    for (int i = 0; i < height; i++) {
        memcpy(buff + i * width * number_colors, matrix[i], width * number_colors);
    }
    return buff;
}


int main(int argc, char * argv[])
{
    int rank;
    int nProcesses;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProcesses);

    image *img;
    int width, height, number_colors;
    unsigned char *buffer;
    int start, stop;
    void **data;
    int number_lines = 0;

    if (rank == 0) {
        img = readInput(argv[1]);
        width = img->width;
        height = img->height;
        number_colors = img->number_colors;

        for (int i = 1; i < nProcesses; i++) {
            // Compute the number of lines each process has to compute
            if ((height - 2) % nProcesses == 0) {
                start = i * (height - 2) / nProcesses;
                stop = (i + 1) * (height - 2) / nProcesses + 1;
            } else {
                int r = (height - 2) % nProcesses;
                if (r > i) {
                    start = i * ((height - 2) / nProcesses + 1);
                    stop = (i + 1) * ((height - 2) / nProcesses + 1) + 1;
                } else {
                    start = i * (height - 2) / nProcesses + 1;
                    stop = (i + 1) * (height - 2) / nProcesses + 2;
                }
                if (i == nProcesses - 1)
                    stop--;
            }
            int number_lines = stop - start + 1;
            // Send to process the details and the part of the image for processing
            MPI_Send(&number_colors, 1, MPI_INT, i, 1, MPI_COMM_WORLD);
            MPI_Send(&width, 1, MPI_INT, i, 2, MPI_COMM_WORLD);
            MPI_Send(&height, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
            MPI_Send(&number_lines, 1, MPI_INT, i, 4, MPI_COMM_WORLD);
            MPI_Send(img->data + start * width * number_colors, width * number_lines * number_colors, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD);
        }
        // compute the part of the image the process with rank 0 has to compute
        start = 0;
        if ((height - 2) % nProcesses == 0) {
            stop = (height - 2) / nProcesses + 1;
        } else {
            stop = (height - 2) / nProcesses + 2;
        }
        number_lines = stop - start + 1;
        data = buffer_to_matrix(img->data, width, number_lines, number_colors);
    } else {
        // Get data from process with rank 0
        MPI_Recv(&number_colors, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&width, 1, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&height, 1, MPI_INT, 0, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(&number_lines, 1, MPI_INT, 0, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        buffer = (unsigned char *) malloc (width * number_lines * number_colors);
        MPI_Recv(buffer, width * number_lines * number_colors, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        data = buffer_to_matrix(buffer, width, number_lines, number_colors);
    }

    // apply filters
    for (int i = 3; i < argc; i++) {
        if (!strcmp("smooth", argv[i])) {
            apply_filter(SMOOTH, data, number_lines, width, number_colors);
        } else if (!strcmp("blur", argv[i])) {
            apply_filter(BLUR, data, number_lines, width, number_colors);
        } else if (!strcmp("sharpen", argv[i])) {
            apply_filter(SHARPEN, data, number_lines, width, number_colors);
        } else if (!strcmp("mean", argv[i])) {
            apply_filter(MEAN, data, number_lines, width, number_colors);
        } else if (!strcmp("emboss", argv[i])) {
            apply_filter(EMBOSS, data, number_lines, width, number_colors);
        }

        // Swap borders between processes
        if (nProcesses > 1 && argc > 4) {
            if (rank == 0) {
                MPI_Sendrecv(data[number_lines - 2], width * number_colors, MPI_UNSIGNED_CHAR, 1, 0,
                    data[number_lines - 1], width * number_colors, MPI_UNSIGNED_CHAR, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else if (rank == nProcesses - 1) {
                MPI_Sendrecv(data[1], width * number_colors, MPI_UNSIGNED_CHAR, rank - 1, 0,
                    data[0], width * number_colors, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Sendrecv(data[number_lines - 2], width * number_colors, MPI_UNSIGNED_CHAR, rank + 1, 0,
                    data[number_lines - 1], width * number_colors, MPI_UNSIGNED_CHAR, rank + 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Sendrecv(data[1], width * number_colors, MPI_UNSIGNED_CHAR, rank - 1, 0,
                    data[0], width * number_colors, MPI_UNSIGNED_CHAR, rank - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
        }
    }

    buffer = matrix_to_buffer(data, width, number_lines, number_colors);
    if (rank != 0) {
        // send to process with rank 0 the final computations
        MPI_Send(buffer + width * number_colors, width * (number_lines - 2) * number_colors, MPI_UNSIGNED_CHAR, 0, 0, MPI_COMM_WORLD);
    } else {
        memcpy(img->data, buffer, number_lines * width * number_colors);
        for (int i = 1; i < nProcesses; i++) {

            if ((height - 2) % nProcesses == 0) {
                start = i * (height - 2) / nProcesses;
                stop = (i + 1) * (height - 2) / nProcesses + 1;
            } else {
                int r = (height - 2) % nProcesses;
                if (r > i) {
                    start = i * ((height - 2) / nProcesses + 1);
                    stop = (i + 1) * ((height - 2) / nProcesses + 1) + 1;
                } else {
                    start = i * (height - 2) / nProcesses + 1;
                    stop = (i + 1) * (height - 2) / nProcesses + 2;
                }
                if (i == nProcesses - 1)
                    stop--;
            }
            number_lines = stop - start + 1;

            // get the lines from the other processes in the right part of the matrix
            MPI_Recv(img->data + (start + 1) * width * number_colors, width * (number_lines - 2) * number_colors, MPI_UNSIGNED_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        writeData(argv[2], img);
    }

    MPI_Finalize();
    return 0;
}
