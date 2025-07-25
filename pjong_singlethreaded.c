#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "graphic.h"

void pjong(double a, double b, double c, double d);
void binData(double x, double y, unsigned long int* data_array, int w, int h);

int main() {
    double a = -0.827;
    double b = -1.637;
    double c = 1.659;
    double d = -0.943;
    pjong(a, b, c, d);
    return 0;
}

void pjong(double a, double b, double c, double d) {
    unsigned long int count = 0;
    double x_n = 0.12;
    double y_n = 0.23;
    double x_n1, y_n1;

    // Picture dimensions for png file
    unsigned int w = 500;
    unsigned int h = 500;
    int total = w * h;

    // Allocate memory 
    unsigned long int* data_array = (unsigned long int*)malloc(total * sizeof(unsigned long int));
    if (data_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Initialize the allocated memory to zero
    for (int i = 0; i < total; i++) {
        data_array[i] = 0;
    }

    // Define the number of points to store
    unsigned long long int max_points = 1000000;  // Maximum number of points including skipped points
    int skip_points = 1000;    // Number of points to skip
    unsigned long long int bin_points= max_points - skip_points;

    // Generate data points
    for (unsigned long long int i = 0; i < max_points; i++) {
        x_n1 = sin(a * y_n) - cos(b * x_n);
        y_n1 = sin(c * x_n) - cos(d * y_n);

        count++;

        // Store points after skipping the initial 100 points
        if (count > skip_points) {
            binData(x_n1, y_n1, data_array, w, h);
        }

        x_n = x_n1;
        y_n = y_n1;
    }

    //Calling draw function to create png file
    draw(data_array, w, h, bin_points, BLACK, MAGENTA);

    // Free allocated memory
    free(data_array);
}

void binData(double x, double y, unsigned long int* data_array, int w, int h) {
    // Logic to ensure that (-2,-2) gets converted to (0,0) and (2,2) gets converted to (w,h)
    int x_bin = (int)((x + 2.0) / 4.0 * w);
    int y_bin = (int)((y + 2.0) / 4.0 * h);


    // Adjusting x_bin and y_bin to stay within bounds
    if (x_bin == w) {
        x_bin = w - 1;
    }
    if (y_bin == h) {
        y_bin = h - 1;
    }

    // Checking bounds and updating the data array
    if (x_bin >= 0 && x_bin < w && y_bin >= 0 && y_bin < h) {
        data_array[y_bin * w + x_bin]++;
    }    
    else {
        fprintf(stderr, "Index out of bounds: x_bin = %d, y_bin = %d\n", x_bin, y_bin); // For error checking
    }


}
