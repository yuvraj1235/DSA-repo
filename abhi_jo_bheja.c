#include <stdio.h>
#include <math.h>

// Function to multiply two numbers using Divide and Conquer
long long multiply(int x, int y) {
    // Base case for recursion (single digit multiplication)
    if (x < 10 || y < 10)
        return x * y;

    // Calculate the size of the numbers
    int n = fmax((int)log10(x) + 1, (int)log10(y) + 1);
    int half = n / 2;

    int pow10_half = (int)pow(10, half);

    // Split x and y into two halves
    int x_high = x / pow10_half;
    int x_low = x % pow10_half;
    int y_high = y / pow10_half;
    int y_low = y % pow10_half;

    // Recursive calls
    long long p1 = multiply(x_high, y_high);
    long long p2 = multiply(x_low, y_low);
    long long p3 = multiply(x_high + x_low, y_high + y_low);

    // Karatsuba formula: (p1 * 10^(2*half)) + ((p3 - p1 - p2) * 10^half) + p2
    return (p1 * (long long)pow(10, 2 * half)) +
           ((p3 - p1 - p2) * (long long)pow(10, half)) +
           p2;
}

int main() {
    int num1, num2;
    printf("Enter first 4-digit number: ");
    scanf("%d", &num1);
    printf("Enter second 4-digit number: ");
    scanf("%d", &num2);

    long long result = multiply(num1, num2);
    printf("Product: %lld\n", result);

    return 0;
}