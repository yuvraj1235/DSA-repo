#include <stdio.h>
#include <stdlib.h>

// Function to add two matrices
void addMatrix(int n, int A[n][n], int B[n][n], int C[n][n]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

// Function to subtract two matrices
void subMatrix(int n, int A[n][n], int B[n][n], int C[n][n]) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

// Recursive function to multiply matrices using divide and conquer
void multiplyMatrix(int n, int A[n][n], int B[n][n], int C[n][n]) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;
    int A11[k][k], A12[k][k], A21[k][k], A22[k][k];
    int B11[k][k], B12[k][k], B21[k][k], B22[k][k];
    int C11[k][k], C12[k][k], C21[k][k], C22[k][k];
    int temp1[k][k], temp2[k][k];

    // Divide matrices into 4 submatrices
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    // C11 = A11*B11 + A12*B21
    multiplyMatrix(k, A11, B11, temp1);
    multiplyMatrix(k, A12, B21, temp2);
    addMatrix(k, temp1, temp2, C11);

    // C12 = A11*B12 + A12*B22
    multiplyMatrix(k, A11, B12, temp1);
    multiplyMatrix(k, A12, B22, temp2);
    addMatrix(k, temp1, temp2, C12);

    // C21 = A21*B11 + A22*B21
    multiplyMatrix(k, A21, B11, temp1);
    multiplyMatrix(k, A22, B21, temp2);
    addMatrix(k, temp1, temp2, C21);

    // C22 = A21*B12 + A22*B22
    multiplyMatrix(k, A21, B12, temp1);
    multiplyMatrix(k, A22, B22, temp2);
    addMatrix(k, temp1, temp2, C22);

    // Combine results into C
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j] = C11[i][j];
            C[i][j + k] = C12[i][j];
            C[i + k][j] = C21[i][j];
            C[i + k][j + k] = C22[i][j];
        }
    }
}

int main() {
    int n;
    printf("Enter size of square matrix (power of 2): ");
    scanf("%d", &n);

    int A[n][n], B[n][n], C[n][n];

    printf("Enter elements of first matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of second matrix:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &B[i][j]);

    multiplyMatrix(n, A, B, C);

    printf("Resultant matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    return 0;
}
