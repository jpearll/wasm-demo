
// Import standard and emscripten libraries
#include <stdlib.h>
#include <emmintrin.h> // For SSE2 intrinsics (SIMD)
#include <time.h>
#include <emscripten/emscripten.h>
#include <stdio.h>

// Allow the code to be callable from either C or C++
#ifdef __cplusplus
#define EXTERN extern "C"
#else
#define EXTERN
#endif

// Blurs an image by averaging each pixel with its surrounding pixels
void blur(int width, int height, float* input, float* output) {
    // Defines the blur kernel
    __m128 kernel = _mm_set1_ps(1.0f / 9.0f);
    
    // Iterate over the image, skipping the edges
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            __m128 sum = _mm_setzero_ps();
            
            // Iterate over the kernel's dimensions
            for (int ky = -1; ky <= 1; ky++) {
                for (int kx = -1; kx <= 1; kx++) {
                    // Multiply each surrounding pixel by the kernel and add to the sum
                    __m128 pixel = _mm_set1_ps(input[(y+ky)*width + (x+kx)]);
                    sum = _mm_add_ps(sum, _mm_mul_ps(pixel, kernel));
                }
            }
            
            // Write the blurred pixel to the output image
            output[y*width + x] = _mm_cvtss_f32(sum);
        }
    }
}

// Emscripten exported function to run the blur effect
EXTERN EMSCRIPTEN_KEEPALIVE void call_blur() {
    // Defines the dimensions of the image
    int width = 5*128;
    int height = 5*128;
    
    // Allocate memory for the input and output images
    float* input = (float*)malloc(width * height * sizeof(float));
    float* output = (float*)malloc(width * height * sizeof(float));

    // Fill the input image with random grayscale values
    srand(time(NULL)); // Seed the random number generator
    for (int i = 0; i < width * height; i++) {
        input[i] = (float)rand() / (float)RAND_MAX; // Generate a random float between 0.0 and 1.0
    }

    printf("Blurring\n");

    // Call the blur function
    blur(width, height, input, output);

    printf("Done!\n");
    
    // Clean up the allocated memory
    free(input);
    free(output);
}

// Main function to call the blur function
int main() {
    
    call_blur();
    return 0;
}
