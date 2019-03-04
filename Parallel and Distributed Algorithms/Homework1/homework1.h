#ifndef HOMEWORK_H1
#define HOMEWORK_H1

typedef struct {
    int width;
    unsigned char **data;
    int thread_id;
}image;

void initialize(image *im);
void render(image *im);
void writeData(const char * fileName, image *img);

#endif /* HOMEWORK_H1 */