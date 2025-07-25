#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>
#include "graphic.h"

// Number of threads to use for parallel processing
#define NUM_THREADS 4

// Global data array to store fractal data
unsigned long int* data_array;

// Mutex for synchronizing access to the shared data array
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Function prototypes
void *generate_fractal(void *thread_id);
void binData(double x, double y, unsigned long int* data_array, int w, int h);

int main() {
    double a = -0.827, b = -1.637, c = 1.659, d = -0.943;
    unsigned int w = 500;
    unsigned int h = 500;
    int total = w * h;

    // Allocate memory for data array
    data_array = (unsigned long int*)malloc(total * sizeof(unsigned long int));
    if (data_array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Initialize the data array to zero
    for (int i = 0; i < total; i++) {
        data_array[i] = 0;
    }

    // Create an array to hold thread IDs
    pthread_t threads[NUM_THREADS];

    // Create NUM_THREADS threads to run the fractal generation function
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, generate_fractal, (void *)(intptr_t)i);
    }

    // Join all threads to ensure they finish execution before moving on
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Generate and display the fractal as a PNG image
    draw(data_array, w, h, w * h, BLACK, MAGENTA);

    // Free the allocated memory for the data array
    free(data_array);

    return 0;
}

// Function to generate the fractal in parallel across multiple threads
void *generate_fractal(void *thread_id) {
    int id = (intptr_t)thread_id;
    double a = -0.827, b = -1.637, c = 1.659, d = -0.943;
    double x_n = 0.12, y_n = 0.23;
    double x_n1, y_n1;
    unsigned long int count = 0;

    unsigned long long int max_points = 1000000;
    int skip_points = 1000;
    unsigned long long int bin_points = max_points - skip_points;

    // Split the work between the threads by dividing the number of points
    unsigned long long int points_per_thread = max_points / NUM_THREADS;
    unsigned long long int start = id * points_per_thread;
    unsigned long long int end = (id + 1) * points_per_thread;

    // Generate fractal points for the assigned range of iterations
    for (unsigned long long int i = start; i < end; i++) {
        x_n1 = sin(a * y_n) - cos(b * x_n);
        y_n1 = sin(c * x_n) - cos(d * y_n);

        count++;

        // Only store points after skipping the initial ones
        if (count > skip_points) {
            binData(x_n1, y_n1, data_array, 500, 500);
        }

        // Update the values for the next iteration
        x_n = x_n1;
        y_n = y_n1;
    }

    // Exit the thread after finishing the task
    pthread_exit(NULL);
}

// Function to safely store fractal points into the shared data array
void binData(double x, double y, unsigned long int* data_array, int w, int h) {
    int x_bin = (int)((x + 2.0) / 4.0 * w);
    int y_bin = (int)((y + 2.0) / 4.0 * h);

    if (x_bin == w) x_bin = w - 1;
    if (y_bin == h) y_bin = h - 1;

    // Lock the mutex to ensure safe updating of the shared array
    pthread_mutex_lock(&mutex);

    if (x_bin >= 0 && x_bin < w && y_bin >= 0 && y_bin < h) {
        data_array[y_bin * w + x_bin]++;
    }

    // Unlock the mutex after updating the array
    pthread_mutex_unlock(&mutex);
}
