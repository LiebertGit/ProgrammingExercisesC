#include <stdio.h>
#include <stdlib.h>

//image biblio
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct {
    //total amount of pixel
    size_t count;
    //their summed grey value
    size_t sum;
}   RegionStats;

void initializeUnionFind(size_t *parent, 
                            size_t pixelCount);
void initializeRegionStats(unsigned char *image, 
                            RegionStats *stats, 
                            size_t pixelCount);
size_t find(size_t *parent, size_t pixel);
void unionSets(size_t *parent, 
                RegionStats *stats, 
                size_t rootA, 
                size_t rootB);
void mergeNeighbors(size_t *parent,
                    RegionStats *stats,
                    unsigned char *image,
                    int width,
                    int height);
size_t countRegions(size_t *parent, 
                    size_t pixelCount);
void saveSegmentationImage(size_t *parent,
                            int width,
                            int heigt);

int main(void) 
{
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

    //every pixel becomes its own root
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

    //mergeNeighbors(parent, stats, image, width, height);

    size_t regions = pixelCount;
    size_t iteration = 0;

    while (1) {

        
        printf("START iteration\n");
        fflush(stdout);

        iteration++;

        size_t previousRegions = regions;

        mergeNeighbors(parent, stats, image, width, height);

        regions = countRegions(parent, pixelCount);

        printf("\nIteration: %zu\n", iteration);
        printf("Regions: %zu\n", regions);

        if (regions == previousRegions) {
            printf("No regions changed. Segmentation finished.\n");
            break;
        }

        printf("Press ENTER for the next iteration...\n");
        getchar();
        
        printf("END iteration\n");
        fflush(stdout);


    }

    //format and value checks of the image
    printf("Width: %d\n", width);
    printf("Height: %d\n", height);
    printf("Original channels: %d\n", channels);
    printf("First pixel value: %u\n", image[0]);

    saveSegmentationImage(parent, width, height);

    //free storage
    stbi_image_free(image);
    free(stats);
    free(parent);

    return 0;   
}

void initializeUnionFind(size_t *parent, 
                            size_t pixelCount)
{
    //each entry pixel as its own root
    for (size_t i = 0; i < pixelCount; i++){
        parent[i] = i;
    }
}

void initializeRegionStats(unsigned char *image, 
                            RegionStats *stats, 
                            size_t pixelCount)
{    
        for(size_t i = 0; i < pixelCount; i++){
        //eachs tructure starts with its root pixel
        stats[i].count = 1;
        //with value of the root pixel
        stats[i].sum = image[i];
    }
}

size_t find(size_t *parent, size_t pixel)
{    
    if(parent[pixel] != pixel){
        parent[pixel] = find(parent, parent[pixel]);
    }

    return parent[pixel];
}

void unionSets(size_t *parent, 
                RegionStats *stats, 
                size_t rootA, 
                size_t rootB)
{    
    parent[rootB] = rootA;                
    stats[rootA].count += stats[rootB].count;
    stats[rootA].sum += stats[rootB].sum;                 
}

void mergeNeighbors(size_t *parent,
                    RegionStats *stats,
                    unsigned char *image,
                    int width,
                    int height)
{
    //convert 2D-presentation into 1D
    //since the stack in one dimensional 
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            size_t index = y * width + x;

            if(x > 0){
                size_t left = index -1;

                size_t rootPixel = find(parent, index);
                size_t rootLeft = find(parent, left);

                if(rootPixel != rootLeft){
                    double meanPixel = (double)stats[rootPixel].sum /
                                        stats[rootPixel].count;

                    double meanLeft = (double)stats[rootLeft].sum /
                                        stats[rootLeft].count;

                    double difference = meanPixel - meanLeft;
                    if (difference < 0) {
                        difference = (-1) * difference ;
                    }

                    // printf("Difference = %.2f\n", difference);

                    // printf("Mean Pixel = %.2f | Mean Left = %.2f\n",
                    //         meanPixel,
                    //         meanLeft);

                    if (difference <= 5.0){
                        unionSets(parent, stats, rootPixel, rootLeft);

                        printf("Merged %zu and %zu\n",
                                rootPixel,
                                rootLeft);
                    }    
                } else {
                    
                }
            }

            if(y > 0){
                size_t top = index - width;

                size_t rootPixel = find(parent, index);
                size_t rootTop = find(parent, top);

                if(rootPixel != rootTop){

                    double meanPixel = (double)stats[rootPixel].sum /
                                        stats[rootPixel].count;

                    double meanTop = (double)stats[rootTop].sum /
                                        stats[rootTop].count;

                    double difference = meanPixel - meanTop;

                    if(difference < 0){
                        difference = (-1) * difference;
                    }
                    
                    // printf("Difference = %.2f\n", difference);
                    
                    // printf("Mean Pixel = %.2f | Mean Top = %.2f\n",
                    //         meanPixel,
                    //         meanTop);

                    if(difference == 10.0){

                        unionSets(parent,
                                stats,
                                rootPixel,
                                rootTop);

                        printf("Merged %zu and %zu\n",
                            rootPixel,
                            rootTop);
                    }        
                }
            }

        }
    }
}

size_t countRegions(size_t *parent, 
                    size_t pixelCount)
{
    size_t regions = 0;

    for (size_t i = 0; i < pixelCount; i++) {
        if(parent[i] == i) {
            regions++;
        }
    }

    return regions;
}

void saveSegmentationImage(size_t *parent,
                            int width,
                            int height)
{
    size_t pixelCount = (size_t)width * (size_t)height;

    unsigned char *output = malloc(pixelCount * 3);

    if (output == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    for(size_t i = 0; i < pixelCount; i++) {

        size_t root = find(parent, i);

        unsigned char red   = root % 256;
        unsigned char green = (root / 256) % 256;
        unsigned char blue  = (root / 65536) % 256;

        output[i * 3]     = red;
        output[i * 3 + 1] = green;
        output[i * 3 + 2] = blue;
    }


    // Write RGB image to a PNG file
    if (!stbi_write_png(
            "segmentation.png",
            width,
            height,
            3,
            output,
            width * 3)) {

        printf("Failed to write segmentation image\n");
    } else {
        printf("Segmentation image saved as segmentation.png\n");
    }

    free(output);
}