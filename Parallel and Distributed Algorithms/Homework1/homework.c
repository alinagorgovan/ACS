#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "homework.h"

int num_threads;
int resize_factor;

/*
    Allocs the matrix for pixels depending on the type of the image;
*/
void allocImage(image *img) {
    int i;
    if (!strcmp("P5", img->type)) {
        // Grayscale image
        img->num_colors = 1;
        img->data = (void **) malloc (img->height * sizeof(gray_pixel *));

        for (i = 0; i < img->height; i++) {
            img->data[i] = (gray_pixel *) malloc (img->width * sizeof(gray_pixel));
        }

    } else if (!strcmp("P6", img->type)) {
        // RGB image
        img->num_colors = 3;
        img->data = (void **) malloc (img->height * sizeof(rgb_pixel *));

        for (i = 0; i < img->height; i++) {
            img->data[i] = (rgb_pixel *) malloc (img->width * sizeof(rgb_pixel));
        }
    }
}
void readInput(const char *fileName, image *img) {
    FILE *fin = fopen(fileName, "r");
    int i;

    // Read image header
    fscanf(fin, "%s\n", img->type);
    fscanf(fin, "%d %d\n", &(img->width), &(img->height));
    fscanf(fin, "%d\n", &(img->max_value));
    // Alloc matrix of pixels
    allocImage(img);
    // Read pixels values from input
    for (i = 0; i < img->height; i++) {
        fread((img->data)[i], img->num_colors , img->width, fin);
    }

    fclose(fin);
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
        fwrite((img->data)[i], img->num_colors, img->width, fout);
    }

    fclose(fout);

}
/*
    Thread function that does the calculation for a part of the matrix
    when the resize_factor is even.
*/
void *resize_even(void *var) {
    // Input and output image pointers
    image *in = ((input_output *)var)->in;
    image *out = ((input_output *)var)->out;
    int thread_id = ((input_output *)var)->thread_id;

    // Split the result matrix by lines so every thread has
    // to compute a specific part of the final matrix
    int start = thread_id * out->height / num_threads;
    int stop = (thread_id + 1) * out->height / num_threads;

    int sum, sum_r, sum_g, sum_b;
    int rf2 = resize_factor * resize_factor;

    // An element in the result matrix is represented by the calculation
    // of a submatrix of the input image with the sizes resize_factor x resize_factor
    for (int i = start; i < stop; i++){
        for (int j = 0; j < out->width; j++) {
            sum = 0;
            if (!strcmp("P5", out->type)) {
                // Compute the sum of the elements in the submatrix
                for(int row = i * resize_factor; row < (i + 1) * resize_factor; row++) {
                    for(int col = j * resize_factor; col < (j + 1) * resize_factor; col++) {
                        sum += ((gray_pixel **)(in->data))[row][col];
                    }
                }
                // Put the computed value in the right position in the
                // final matrix of pixels
                ((gray_pixel **)(out->data))[i][j] = sum / rf2;
            } else if (!strcmp("P6", out->type)) {
                rgb_pixel **input_rgb =  (rgb_pixel **)(in->data);
                rgb_pixel **output_rgb =  (rgb_pixel **)(out->data);
                sum_r = sum_g = sum_b = 0;
                // For every color, compute the sums separtely
                for(int row = i * resize_factor; row < (i + 1) * resize_factor; row++) {
                    for(int col = j * resize_factor; col < (j + 1) * resize_factor; col++) {
                        sum_r += input_rgb[row][col].r;
                        sum_g += input_rgb[row][col].g;
                        sum_b += input_rgb[row][col].b;
                    }
                }
                // In the final matrix, for every color put the calculated sum
                output_rgb[i][j].r = sum_r / rf2;
                output_rgb[i][j].g = sum_g / rf2;
                output_rgb[i][j].b = sum_b / rf2;
            }
        }
    }
    return NULL;
}
/*
    Thread function that does the calculation for a part of the matrix
    when the resize_factor equals three.
*/
void *resize_three(void *var) {

    // Input and output image pointers
    image *in = ((input_output *)var)->in;
    image *out = ((input_output *)var)->out;
    int thread_id = ((input_output *)var)->thread_id;

    // Split the result matrix by lines so every thread has
    // to compute a specific part of the final matrix
    int start = thread_id * out->height / num_threads;
    int stop = (thread_id + 1) * out->height / num_threads;
    // initialize the resize matrix
    int resize_matrix[3][3] = {
        {1, 2, 1},
        {2, 4, 2},
        {1, 2, 1}
    };
    int sum, sum_r, sum_g, sum_b;

    // An element in the result matrix is represented by the calculation
    // of a submatrix of the input image with the sizes resize_factor x resize_factor
    for (int i = start; i < stop; i++){
        for (int j = 0; j < out->width; j++) {
            sum = 0;
            if (!strcmp("P5", out->type)) {
                for(int row = i * resize_factor; row < (i + 1) * resize_factor; row++) {
                    for(int col = j * resize_factor; col < (j + 1) * resize_factor; col++) {
                        // Compute the sum of every element multiplied by the
                        // corresponding element in the resize_matrix
                        sum += ((gray_pixel**)(in->data))[row][col] * resize_matrix[row % 3][col % 3];
                    }
                }
                ((gray_pixel**)(out->data))[i][j] = sum / 16;
            } else if (!strcmp("P6", out->type)) {
                rgb_pixel **input_rgb =  (rgb_pixel**)(in->data);
                rgb_pixel **output_rgb =  (rgb_pixel**)(out->data);
                sum_r = sum_g = sum_b = 0;

                // For every color, compute the sums separtely
                for(int row = i * resize_factor; row < (i + 1) * resize_factor; row++) {
                    for(int col = j * resize_factor; col < (j + 1) * resize_factor; col++) {
                        // Foe every element compute the sum of every color
                        // multiplied by the corresponding element in the
                        // resize_matrix
                        sum_r += input_rgb[row][col].r * resize_matrix[row % 3][col % 3];
                        sum_g += input_rgb[row][col].g * resize_matrix[row % 3][col % 3];
                        sum_b += input_rgb[row][col].b * resize_matrix[row % 3][col % 3];
                    }
                }
                output_rgb[i][j].r = sum_r / 16;
                output_rgb[i][j].g = sum_g / 16;
                output_rgb[i][j].b = sum_b / 16;
            }

        }
    }
    return NULL;
}
void resize(image *in, image *out) {
    strcpy(out->type, in->type);
    out->width = in->width / resize_factor;
    out->height = in->height / resize_factor;
    out->max_value = in->max_value;
    allocImage(out);

    int i;
    pthread_t tid[num_threads];

    // Initialize threads width ids starting from 0
	int thread_id[num_threads];
	for(i = 0;i < num_threads; i++)
		thread_id[i] = i;

    // Initialize a vector of input_output structs with the size
    // num_threads so that every thread function will be called
    // with a different argument keeping this way the thread id
    // different in the structure
    input_output **images = (input_output **) malloc (num_threads * sizeof(input_output *));
    for (i = 0; i < num_threads; i++) {
        images[i] = (input_output*) malloc (sizeof(input_output));
        images[i]->in = (image *) malloc (sizeof(image));
        images[i]->out = (image *) malloc (sizeof(image));
        images[i]->in->data = in->data;
        images[i]->thread_id = thread_id[i];
        strcpy(images[i]->out->type, out->type);
        images[i]->out->height = out->height;
        images[i]->out->width = out->width;
        images[i]->out->data = out->data;

    }
    // Depending on the resize_factor the threads start with the
    // right function
    if (resize_factor % 2 == 0) {
        for(i = 0; i < num_threads; i++) {
    		pthread_create(&(tid[i]), NULL, resize_even, images[i]);
    	}
    } else if (resize_factor == 3) {
        for(i = 0; i < num_threads; i++) {
    		pthread_create(&(tid[i]), NULL, resize_three, images[i]);
    	}
    }
    for(i = 0; i < num_threads; i++) {
        pthread_join(tid[i], NULL);
    }

}
