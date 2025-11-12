#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>

typedef struct Point {
    double x, y;
} Point;

// Compare functions for qsort
int compareX(const void *a, const void *b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return (p1->x - p2->x) > 0 ? 1 : -1;
}

int compareY(const void *a, const void *b) {
    Point *p1 = (Point *)a, *p2 = (Point *)b;
    return (p1->y - p2->y) > 0 ? 1 : -1;
}

// Compute Euclidean distance
double dist(Point p1, Point p2) {
    return sqrt((p1.x - p2.x)*(p1.x - p2.x) +
                (p1.y - p2.y)*(p1.y - p2.y));
}

// Brute force for small sets
double bruteForce(Point P[], int n) {
    double min = DBL_MAX;
    for (int i = 0; i < n; ++i)
        for (int j = i + 1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}

// Find min distance in strip[]
double stripClosest(Point strip[], int size, double d) {
    double min = d;
    qsort(strip, size, sizeof(Point), compareY);

    for (int i = 0; i < size; ++i)
        for (int j = i + 1; j < size && (strip[j].y - strip[i].y) < min; ++j)
            if (dist(strip[i], strip[j]) < min)
                min = dist(strip[i], strip[j]);
    return min;
}

// Recursive function
double closestUtil(Point Px[], Point Py[], int n) {
    if (n <= 3)
        return bruteForce(Px, n);

    int mid = n / 2;
    Point midPoint = Px[mid];

    Point Pyl[mid + 1], Pyr[n - mid - 1];
    int li = 0, ri = 0;
    for (int i = 0; i < n; i++) {
        if (Py[i].x <= midPoint.x)
            Pyl[li++] = Py[i];
        else
            Pyr[ri++] = Py[i];
    }

    double dl = closestUtil(Px, Pyl, mid);
    double dr = closestUtil(Px + mid, Pyr, n - mid);
    double d = (dl < dr) ? dl : dr;

    Point strip[n];
    int j = 0;
    for (int i = 0; i < n; i++)
        if (fabs(Py[i].x - midPoint.x) < d)
            strip[j++] = Py[i];

    return fmin(d, stripClosest(strip, j, d));
}

// Main function
double closest(Point P[], int n) {
    Point Px[n], Py[n];
    for (int i = 0; i < n; i++)
        Px[i] = Py[i] = P[i];

    qsort(Px, n, sizeof(Point), compareX);
    qsort(Py, n, sizeof(Point), compareY);

    return closestUtil(Px, Py, n);
}

int main() {
    Point P[] = {{2, 3}, {12, 30}, {40, 50}, {5, 1}, {12, 10}, {3, 4}};
    int n = sizeof(P) / sizeof(P[0]);

    printf("The smallest distance is %.4f\n", closest(P, n));
    return 0;
}
