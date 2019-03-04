#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <math.h>

#include "homework1.h"

int num_threads;
int resolution;

void initialize(image *img) {
    img->width = resolution;
    // alloc matrix of pixels and initialize with 255(white)
    img->data = (unsigned char **) malloc (sizeof(unsigned char *) * img->width);
    for (int i = 0; i < img->width; i++) {
        img->data[i] = (unsigned char *) malloc (img->width);
        for (int j = 0; j <img->width; j++) {
            img->data[i][j] = 255;
        }
    }
}
/*
    Thread function that computes a number of lines from the
    pixel matrix.
*/
void* threadFunction(void *var){
	image *img = (image*)var;

    // Start and stop indexes representing the starting and ending line
    // where the thread computes the results in the matrixs
    int start = img->thread_id * img->width / num_threads;
    int stop = (img->thread_id + 1) * img->width / num_threads;

    float dist;
    float s = sqrt(5);
    // cm  =  the relation between resolution and cm units
    float cm = (float) 100 / img->width;
    for (int x = start; x < stop; x++) {
        // transform the x coordonate in cm
        float x_cm = x * cm + cm / 2;
        for (int y = 0; y < img->width; y++) {
            // transform the y coordonate in cm
            float y_cm = y * cm + cm / 2;
            // compute the distance
            dist = abs(-x_cm + 2 * y_cm) / s;
            int yt = img->width - y - 1;
            // If the distance validates the condition than that pixel is
            // black and is modified in the matrix
            if (dist <= 3) {
                img->data[yt][x] = 0;
            }
        }
    }
    return NULL;
}

void render(image *img) {
    int i;

    // Initialize threads with ids starting from 0
	pthread_t tid[num_threads];
	int thread_id[num_threads];
    for(i = 0; i < num_threads; i++)
    thread_id[i] = i;

    // Initialize array o images* so that ethe thread id can be passed
    // to the threadFunction in the image structure
    image **images = (image **) malloc (num_threads * sizeof(image *));

    for(i = 0; i < num_threads; i++) {
        images[i] = (image*) malloc (sizeof(image));
        images[i]->thread_id = thread_id[i];
        images[i]->width = img->width;
        images[i]->data = img->data;
    }

    // Starting threads
	for(i = 0; i < num_threads; i++) {
		pthread_create(&(tid[i]), NULL, threadFunction, images[i]);
	}

	for(i = 0; i < num_threads; i++) {
		pthread_join(tid[i], NULL);
	}
}

void writeData(const char * fileName, image *img) {
    FILE *fout = fopen(fileName, "w");
    fprintf(fout, "P5\n%d %d\n255\n", img->width, img->width);
    for (int i = 0; i < img->width; i++) {
        fwrite(img->data[i], 1, img->width, fout);
    }
    fclose(fout);
}
