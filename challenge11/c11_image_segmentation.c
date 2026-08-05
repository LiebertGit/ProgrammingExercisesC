#include <stdio.h>
#include <stdlib.h>

//image biblio
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct {
    //total amount of pixel
    size_t count;
    //their summed grey value
    size_t sum;
}   RegionStats;

void initializeUnionFind(size_t *parent, size_t pixelCount);
void initializeRegionStats(unsigned char *image, 
                            RegionStats *stats, 
                            size_t pixelCount);

int main(void) {
    int width;      //pixel count x
    int height;     //pixel count y
    int channels;   //how many values a pixel has originally

    //stores grey values [0...255] into an array
    unsigned char *image = stbi_load(
            "grey_scale_pixel_coarse.png",
            &width,
            &height,
            &channels,
            1
    );

    //image load check
    if (image == NULL) {
        printf("Image loading failed\n");
        return 1;
    }

    //total pixel count
    size_t pixelCount = width * height;
    //each pixel is its own root
    size_t *parent = malloc(pixelCount * sizeof(size_t));
    
    //array initialion check
    if(parent == NULL){
        printf("Memory allocation failed\n");
        stbi_image_free(image);
        return 1;
    }

    initializeUnionFind(parent, pixelCount);

    //storage for each pixel stat 
    RegionStats *stats = malloc(pixelCount * sizeof(RegionStats));
    //check malloc success, if fail free storage
    if (stats == NULL) {
        printf("Memory allocation failed\n");
        free (parent);
        stbi_image_free(image);
        return 1;
    }
    initializeRegionStats(image, stats, pixelCount);

    //format and value checks of the image
    printf("Width: %d\n", width);
    printf("Height: %d\n", height);
    printf("Original channels: %d\n", channels);
    printf("First pixel value: %u\n", image[0]);

    //free storage
    stbi_image_free(image);
    free(stats);
    free(parent);

    return 0;   
}

void initializeUnionFind(size_t *parent, size_t pixelCount){
    //each entry pixel as its own root
    for (size_t i = 0; i < pixelCount; i++){
        parent[i] = i;
    }
}

void initializeRegionStats(unsigned char *image, 
                            RegionStats *stats, 
                            size_t pixelCount){
    for(size_t i = 0; i < pixelCount; i++){
        //eachs tructure starts with its root pixel
        stats[i].count = 1;
        //with value of the root pixel
        stats[i].sum = image[i];
    }
}