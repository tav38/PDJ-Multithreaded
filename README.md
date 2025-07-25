# **PDJ-Multithreaded**

## **Overview**

This project implements a fractal generation algorithm based on the **Peter De Jong** map. The original version of the code is **single-threaded** and generates fractals by iterating through a set of points. However, as the number of points increases, the computation time grows significantly. To improve performance, the code has been parallelized using **pthreads** to utilize multiple CPU cores for faster computation, especially when working with large datasets.

## **Features**

* **Single-Threaded Version**: Generates fractals sequentially, processing one point at a time.
* **Multi-Threaded Version**: Parallelizes the fractal generation using **pthreads**, splitting the workload across multiple threads to improve performance.
* **Fractal Visualization**: Generates a **PNG image** of the fractal after computation using custom visualization functions.
* **Thread Synchronization**: Uses **mutexes** to ensure thread-safe updates to the shared data array.

## **Usage**

### **Single-Threaded Execution**

Run the code to generate a fractal based on the Peter De Jong map. The fractal will be generated and saved as a PNG image.

### **Multi-Threaded Execution**

This version utilizes **pthreads** to divide the computation across multiple threads, improving performance on multi-core systems.

