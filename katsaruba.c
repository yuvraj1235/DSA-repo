#include <stdio.h>
#include <math.h>

int getLength(long long n) {
    int len = 0;
    while (n) {
        len++;
        n /= 10;
    }
    return len;
}

// Karatsuba multiplication function
long long karatsuba(long long x, long long y) {
    int lenX = getLength(x);
    int lenY = getLength(y);

    // Base case
    if (x < 10 || y < 10)
        return x * y;

    int n = (lenX > lenY ? lenX : lenY);
    int m = n / 2;

    long long highX = x / pow(10, m);
    long long lowX = x % (long long)pow(10, m);
    long long highY = y / pow(10, m);
    long long lowY = y % (long long)pow(10, m);

    long long z0 = karatsuba(lowX, lowY);
    long long z1 = karatsuba((lowX + highX), (lowY + highY));
    long long z2 = karatsuba(highX, highY);

    return (z2 * pow(10, 2 * m)) + ((z1 - z2 - z0) * pow(10, m)) + z0;
}

int main() {
    long long x, y;
    printf("Enter two numbers: ");
    scanf("%lld %lld", &x, &y);

    printf("Karatsuba Multiplication Result: %lld\n", karatsuba(x, y));
    return 0;
}
