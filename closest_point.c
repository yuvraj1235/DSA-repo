#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>

// Structure to represent a point in 2D space
typedef struct Point {
    double x;
    double y;
} Point;

// ---------------------- Utility Functions ----------------------

// Function to calculate the distance between two points
double dist(Point p1, Point p2) {
    // Note: The -lm flag is essential for these math functions!
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

// Comparison function for sorting points based on X-coordinate
int compareX(const void* a, const void* b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    return 0;
}

// Comparison function for sorting points based on Y-coordinate
int compareY(const void* a, const void* b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    return 0;
}

// Function to find the minimum of two doubles
double min(double a, double b) {
    return (a < b) ? a : b;
}

// ---------------------- Brute Force & Strip Closest (The core logic remains the same) ----------------------

// A simple brute force method for a small number of points
double bruteForce(Point P[], int n) {
    double min_val = DBL_MAX;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            min_val = min(min_val, dist(P[i], P[j]));
        }
    }
    return min_val;
}

// Find the closest pair in the 'strip' (points close to the midline)
double stripClosest(Point strip[], int size, double d) {
    double min_val = d;

    // Sort the strip points based on Y-coordinate
    qsort(strip, size, sizeof(Point), compareY);

    for (int i = 0; i < size; ++i) {
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min_val; ++j) {
            min_val = min(min_val, dist(strip[i], strip[j]));
        }
    }

    return min_val;
}

// ---------------------- Divide and Conquer (Core) ----------------------

// Recursive function to find the smallest distance
double closestUtil(Point Px[], int n) {
    if (n <= 3) {
        return bruteForce(Px, n);
    }

    int mid = n / 2;
    Point midPoint = Px[mid];

    double dL = closestUtil(Px, mid);
    double dR = closestUtil(Px + mid, n - mid);

    double d = min(dL, dR);

    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++) {
        if (fabs(Px[i].x - midPoint.x) < d) {
            strip[j] = Px[i];
            j++;
        }
    }

    return min(d, stripClosest(strip, j, d));
}

// ---------------------- Main Function ----------------------

double closestPair(Point P[], int n) {
    // Step 1: Sort points based on the X-coordinate
    qsort(P, n, sizeof(Point), compareX);

    // Step 2: Call the recursive utility function
    return closestUtil(P, n);
}

// ---------------------- User Input and Execution ----------------------

int main() {
    int n;

    // 1. Get the number of points
    printf("Closest Point Finder (Divide and Conquer)\n");
    printf("-----------------------------------------\n");
    printf("Enter the total number of points (N): ");
    if (scanf("%d", &n) != 1 || n < 2) {
        printf("Error: Please enter a number greater than 1.\n");
        return 1;
    }

    // 2. Dynamically allocate memory for the points
    Point *P = (Point *)malloc(n * sizeof(Point));
    if (P == NULL) {
        printf("Error: Memory allocation failed.\n");
        return 1;
    }
    
    // 3. Get coordinates from the user
    printf("\n");
    for (int i = 0; i < n; i++) {
        printf("Enter coordinates for Point %d (x y): ", i + 1);
        if (scanf("%lf %lf", &P[i].x, &P[i].y) != 2) {
            printf("Error: Invalid input format.\n");
            free(P);
            return 1;
        }
    }

    // 4. Calculate and display the result
    printf("\nCalculating the minimum distance...\n");
    double min_distance = closestPair(P, n);

    printf("\n✅ **Result**\n");
    printf("   The minimum distance between any two points is: **%.4f**\n", min_distance);
    
    // 5. Clean up memory
    free(P);

    return 0;
}