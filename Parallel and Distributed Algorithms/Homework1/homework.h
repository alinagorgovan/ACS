#ifndef HOMEWORK_H
#define HOMEWORK_H

typedef struct {
    char type[3];   // P5(Grayscale) or P6c(RGB)
    int width;
    int height;
    int max_value;  // Maximum value of a pixel
    int num_colors; // 1(Grayscale) or 3(RGB)
    void **data;    // Matrix of pixels
} image;

typedef struct {
    unsigned char r;
    unsigned char g;
    unsigned char b;
} rgb_pixel;

typedef struct {
    image *in;      // Input image pointer
    image *out;     // Output image pointer
    int thread_id;  // Id of the current thread
} input_output;

typedef unsigned char gray_pixel;

void readInput(const char * fileName, image *img);

void writeData(const char * fileName, image *img);

void resize(image *in, image * out);

#endif /* HOMEWORK_H */
