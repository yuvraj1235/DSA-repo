#include <stdio.h>
#include <stdlib.h>
#include <math.h> // For pow() function

// --- Configuration ---
#define TABLE_SIZE 10
#define EMPTY_SLOT -1

// Global hash table (array of integers)
int hashTable[TABLE_SIZE];

// --- Utility Functions for Mid-Square ---

// Counts the number of digits in a given number
int count_digits(long long n) {
    if (n == 0) return 1;
    int count = 0;
    while (n != 0) {
        n /= 10;
        count++;
    }
    return count;
}

// Extracts 'count' digits starting from 'start_pos' (1-indexed from the left)
int get_digits_range(long long num, int start_pos, int count) {
    int i, digit, number = 0, length;
    length = count_digits(num);
    
    // Ensure the range is valid (Simplified: takes the middle digits)
    if (start_pos < 1 || start_pos + count - 1 > length) {
        // Fallback for simple systems: Use the last digit modulo TABLE_SIZE
        if (TABLE_SIZE > 0) return num % TABLE_SIZE;
        return 0;
    }

    // Step 1: Remove digits to the right of the end position
    // Example: num=1936, length=4, count=2 (want '93'). start_pos=2. end_pos_from_right = 4-3 = 1.
    int power_to_remove_right = length - (start_pos + count - 1);
    long long temp = num / (long long)pow(10, power_to_remove_right);

    // Step 2: Extract the desired 'count' digits from the right
    long long result = temp % (long long)pow(10, count);
    
    return (int)result;
}


// --- Mid-Square Hash Function ---
int midSquareHash(int key) {
    // 1. Square the key. Use long long to prevent overflow for keys up to ~65535.
    long long squaredKey = (long long)key * key;

    // 2. Extract the middle digits.
    int keysquare_len = count_digits(squaredKey);
    
    // Number of digits to extract for the hash index (log10(TABLE_SIZE))
    int hash_digits_count = 1; // Simplification: we'll extract 1 digit.
    
    // Calculate the start position for the middle digit(s).
    // The middle digit's position (1-indexed from left) is floor(length/2) + 1
    int mid_pos = keysquare_len / 2;
    int start_pos = mid_pos + 1; 

    int hash_index = get_digits_range(squaredKey, start_pos, hash_digits_count);

    // 3. Take modulo TABLE_SIZE to get the final index
    return hash_index % TABLE_SIZE;
}

// --- Closed Hashing (Linear Probing) Insertion ---
void insert(int key) {
    int index = midSquareHash(key);
    int original_index = index;
    int i = 0;

    // Linear Probing: Probe sequentially (index + i) % TABLE_SIZE
    do {
        if (hashTable[index] == EMPTY_SLOT) {
            // Found an empty slot
            hashTable[index] = key;
            printf("Inserted %d at index %d\n", key, index);
            return;
        }
        
        // This check prevents infinite loop if the key is already present 
        // and we want to prevent duplicates.
        if (hashTable[index] == key) {
            printf("Key %d already exists at index %d (No insert)\n", key, index);
            return;
        }

        // Move to the next slot (linear probe)
        i++;
        index = (original_index + i) % TABLE_SIZE;

    } while (index != original_index); // Stop if we've looped through the whole table

    // If the loop finishes without insertion, the table is full
    printf("Hash Table is full! Cannot insert %d\n", key);
}


// --- Display Function ---
void displayTable() {
    printf("\n--- Hash Table Contents (Size %d) ---\n", TABLE_SIZE);
    for (int i = 0; i < TABLE_SIZE; ++i) {
        printf("Index %2d: ", i);
        if (hashTable[i] == EMPTY_SLOT) {
            printf("EMPTY\n");
        } else {
            printf("%d\n", hashTable[i]);
        }
    }
    printf("---------------------------------------\n");
}

// --- Main Driver Program ---
int main() {
    // Initialize the hash table: all slots are EMPTY_SLOT
    for (int i = 0; i < TABLE_SIZE; ++i) {
        hashTable[i] = EMPTY_SLOT;
    }

    printf("Starting Hash Table Operations:\n");

    // Example keys to insert
    insert(44); // 44*44 = 1936. Middle digit is 9. Hash: 9
    insert(12); // 12*12 = 144. Middle digit is 4. Hash: 4
    insert(23); // 23*23 = 529. Middle digit is 2. Hash: 2
    insert(10); // 10*10 = 100. Middle digit is 0. Hash: 0
    insert(43); // 43*43 = 1849. Middle digit is 8. Hash: 8
    
    // Collision Example:
    insert(13); // 13*13 = 169. Middle digit is 6. Hash: 6
    insert(36); // 36*36 = 1296. Middle digit is 9. Collision at index 9! Probes to 0 (full!), then 1. Hash: 1

    displayTable();

    return 0;
}