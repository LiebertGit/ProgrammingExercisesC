#include <stdio.h>
#include <stdlib.h>

// image library
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct {
    // Number of pixels belonging to the region
    size_t count;

    // Sum of all grayscale values in the region
    // Used to calculate the region's average intensity
    size_t sum;
} RegionStats;


// Function prototypes
void initializeUnionFind(size_t *parent,
                         size_t pixelCount);

void initializeRegionStats(unsigned char *image,
                           RegionStats *stats,
                           size_t pixelCount);

size_t find(size_t *parent,
            size_t pixel);

void unionSets(size_t *parent,
               RegionStats *stats,
               size_t rootA,
               size_t rootB);

void mergeNeighbors(size_t *parent,
                    RegionStats *stats,
                    double maxDifference,
                    int width,
                    int height);

size_t countRegions(size_t *parent,
                    size_t pixelCount);

void saveSegmentationOutput(size_t *parent,
                            int width,
                            int height);

void saveMeanSegmentationImage(size_t *parent,
                               RegionStats *stats,
                               int width,
                               int height);

void openImage(const char *filename);


int main(void)
{
    // Explain the purpose and expected input before processing starts
    printf("Image segmentation program\n");
    printf("--------------------------\n");
    printf("The program will load Mona_Lisa.png as a grayscale image,\n");
    printf("group neighboring pixels with similar grayscale values,\n");
    printf("and generate two segmentation images.\n");
    printf("You will be asked to enter the maximum allowed grayscale difference.\n\n");

    int width;
    int height;
    int channels;

    // Load the input image and convert it to a single grayscale channel
    unsigned char *image = stbi_load(
        "Mona_Lisa.png",
        &width,
        &height,
        &channels,
        1
    );

    // Stop execution if the image could not be loaded
    if (image == NULL) {
        printf("Image loading failed: %s\n", stbi_failure_reason());
        return 1;
    }

    // Convert the image dimensions into the total number of pixels
    size_t pixelCount = (size_t)width * (size_t)height;

    // Allocate the Union-Find parent array
    // Each pixel initially represents its own region
    size_t *parent = malloc(pixelCount * sizeof(size_t));

    if (parent == NULL) {
        printf("Memory allocation failed\n");
        stbi_image_free(image);
        return 1;
    }

    // Initialize the Union-Find structure
    initializeUnionFind(parent, pixelCount);

    // Allocate statistics for every possible region
    RegionStats *stats =
        malloc(pixelCount * sizeof(RegionStats));

    if (stats == NULL) {
        printf("Memory allocation failed\n");
        free(parent);
        stbi_image_free(image);
        return 1;
    }

    // Initialize every region with its corresponding pixel value
    initializeRegionStats(image, stats, pixelCount);

    // Initially every pixel is considered a separate region
    size_t regions = pixelCount;

    double maxDifference;

    // Ask the user how different neighboring regions may be
    // before they are merged
    printf("Enter maximum grayscale difference: ");
    scanf("%lf", &maxDifference);

    printf("\nStarting segmentation...\n");

    while (1) {

        // Store the number of regions before this iteration
        size_t previousRegions = regions;

        // Compare neighboring regions and merge compatible regions
        mergeNeighbors(
            parent,
            stats,
            maxDifference,
            width,
            height
        );

        // Count the remaining root nodes
        // Each root represents one detected region
        regions = countRegions(parent, pixelCount);

        // If no regions were merged, the segmentation is complete
        if (regions == previousRegions) {
            printf("No regions changed. Segmentation finished.\n");
            break;
        }
    }

    printf("Detected regions: %zu\n", regions);

    // Generate the boundary segmentation image
    saveSegmentationOutput(parent, width, height);

    // Generate an image where each region is represented
    // by its average grayscale value
    saveMeanSegmentationImage(parent, stats, width, height);

    // Open the generated images using the operating system
    openImage("segmentation.png");
    openImage("segmentation_mean.png");

    // Release all dynamically allocated memory
    stbi_image_free(image);
    free(stats);
    free(parent);

    return 0;
}

void openImage(const char *filename)
{
    // Use the default image viewer provided by the operating system
#ifdef _WIN32
    char command[256];

    // Windows command for opening a file with its default application
    snprintf(command, sizeof(command), "start \"\" \"%s\"", filename);
    system(command);

#elif __APPLE__
    char command[256];

    // macOS command for opening a file with its default application
    snprintf(command, sizeof(command), "open \"%s\"", filename);
    system(command);

#elif __linux__
    char command[256];

    // Linux command for opening a file with its default application
    snprintf(command, sizeof(command), "xdg-open \"%s\" &", filename);
    system(command);

#endif
}

void initializeUnionFind(size_t *parent,
                         size_t pixelCount)
{
    // Initially, every pixel is the root of its own region
    // A root is identified by parent[i] == i
    for (size_t i = 0; i < pixelCount; i++) {
        parent[i] = i;
    }
}


void initializeRegionStats(unsigned char *image,
                           RegionStats *stats,
                           size_t pixelCount)
{
    // At the beginning, every region contains exactly one pixel
    for (size_t i = 0; i < pixelCount; i++) {

        stats[i].count = 1;

        // The initial region sum is simply the pixel's grayscale value
        stats[i].sum = image[i];
    }
}


size_t find(size_t *parent,
            size_t pixel)
{
    // Follow parent links until the root of the region is found
    if (parent[pixel] != pixel) {

        // Path compression makes future searches faster
        parent[pixel] = find(parent, parent[pixel]);
    }

    return parent[pixel];
}


void unionSets(size_t *parent,
               RegionStats *stats,
               size_t rootA,
               size_t rootB)
{
    // Attach the second region to the first region
    parent[rootB] = rootA;

    // Combine the statistics of both regions
    // so that the new root contains the complete region information
    stats[rootA].count += stats[rootB].count;
    stats[rootA].sum += stats[rootB].sum;
}


void mergeNeighbors(size_t *parent,
                    RegionStats *stats,
                    double maxDifference,
                    int width,
                    int height)
{
    // Visit every pixel in row-major order
    // and compare it with its left and top neighbors
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            // Convert the 2D pixel position into a 1D array index
            size_t index = (size_t)y * width + x;

            // Check the pixel directly to the left
            if (x > 0) {

                size_t left = index - 1;

                // Find the current roots because pixels may
                // already belong to larger regions
                size_t rootPixel = find(parent, index);
                size_t rootLeft = find(parent, left);

                // Only compare pixels belonging to different regions
                if (rootPixel != rootLeft) {

                    // Calculate the current average grayscale
                    // value of both regions
                    double meanPixel =
                        (double)stats[rootPixel].sum /
                        stats[rootPixel].count;

                    double meanLeft =
                        (double)stats[rootLeft].sum /
                        stats[rootLeft].count;

                    // Calculate the absolute difference
                    double difference =
                        meanPixel - meanLeft;

                    if (difference < 0) {
                        difference = -difference;
                    }

                    // Merge the regions if their averages
                    // are sufficiently similar
                    if (difference <= maxDifference) {

                        unionSets(
                            parent,
                            stats,
                            rootPixel,
                            rootLeft
                        );
                    }
                }
            }

            // Check the pixel directly above
            if (y > 0) {

                size_t top = index - width;

                // Find the current roots
                size_t rootPixel = find(parent, index);
                size_t rootTop = find(parent, top);

                if (rootPixel != rootTop) {

                    double meanPixel =
                        (double)stats[rootPixel].sum /
                        stats[rootPixel].count;

                    double meanTop =
                        (double)stats[rootTop].sum /
                        stats[rootTop].count;

                    // Calculate the absolute difference
                    double difference =
                        meanPixel - meanTop;

                    if (difference < 0) {
                        difference = -difference;
                    }

                    // Merge regions whose average grayscale values
                    // are within the user-defined threshold
                    if (difference <= maxDifference) {

                        unionSets(
                            parent,
                            stats,
                            rootPixel,
                            rootTop
                        );
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

    // Every root represents one separate region
    for (size_t i = 0; i < pixelCount; i++) {

        if (parent[i] == i) {
            regions++;
        }
    }

    return regions;
}


void saveSegmentationOutput(size_t *parent,
                            int width,
                            int height)
{
    // Calculate the total number of pixels
    size_t pixelCount =
        (size_t)width * (size_t)height;

    // Use three channels because the PNG is written as RGB
    unsigned char *output =
        malloc(pixelCount * 3);

    if (output == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    // Also create a text representation of the segmentation
    FILE *textFile =
        fopen("segmentation.txt", "w");

    if (textFile == NULL) {
        printf("Failed to open segmentation.txt\n");
        free(output);
        return;
    }

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            size_t i =
                (size_t)y * width + x;

            int boundary = 0;

            // A boundary exists if the right neighbor
            // belongs to a different region
            if (x < width - 1) {

                size_t right = i + 1;

                if (find(parent, i) !=
                    find(parent, right)) {

                    boundary = 1;
                }
            }

            // A boundary also exists if the bottom neighbor
            // belongs to a different region
            if (y < height - 1) {

                size_t bottom = i + width;

                if (find(parent, i) !=
                    find(parent, bottom)) {

                    boundary = 1;
                }
            }

            // Represent region boundaries as black pixels
            // and all other pixels as white
            if (boundary) {

                output[i * 3]     = 0;
                output[i * 3 + 1] = 0;
                output[i * 3 + 2] = 0;

            } else {

                output[i * 3]     = 255;
                output[i * 3 + 1] = 255;
                output[i * 3 + 2] = 255;
            }

            // Write the same segmentation as text
            fprintf(textFile, boundary ? "x" : ".");
        }

        fprintf(textFile, "\n");
    }

    fclose(textFile);

    // Save the generated segmentation as a PNG
    if (!stbi_write_png(
            "segmentation.png",
            width,
            height,
            3,
            output,
            width * 3))
    {
        printf("Failed to write segmentation image\n");

    } else {

        printf("Segmentation image saved as segmentation.png\n");
    }

    free(output);
}

void saveMeanSegmentationImage(size_t *parent,
                               RegionStats *stats,
                               int width,
                               int height)
{
    size_t pixelCount =
        (size_t)width * (size_t)height;

    // Allocate an RGB output image
    unsigned char *output =
        malloc(pixelCount * 3);

    if (output == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {

            size_t i =
                (size_t)y * width + x;

            // Find the region containing this pixel
            size_t root = find(parent, i);

            // Calculate the average grayscale value
            // of the entire region
            double mean =
                (double)stats[root].sum /
                stats[root].count;

            unsigned char value =
                (unsigned char)mean;

            // Use the same grayscale value for all RGB channels
            output[i * 3]     = value;
            output[i * 3 + 1] = value;
            output[i * 3 + 2] = value;
        }
    }

    // Save the mean-region image as a PNG
    if (!stbi_write_png(
            "segmentation_mean.png",
            width,
            height,
            3,
            output,
            width * 3))
    {
        printf("Failed to write mean segmentation image\n");

    } else {
        printf("Mean segmentation image saved as segmentation_mean.png\n");
    }

    free(output);
}