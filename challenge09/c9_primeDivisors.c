#include <stdio.h>
#include <stdlib.h>

// Wrapper function that allocates memory and starts the recursive factorization.
size_t *primeFactors(size_t num, size_t *count);

// Recursive helper function.
// num      -> remaining number to factorize
// prime    -> current divisor being tested
// factors  -> pointer to the dynamically allocated array of prime factors
// count    -> number of factors stored so far
// capacity -> current size of the allocated array
void primeFactorsHelper(size_t num,
                        size_t prime,
                        size_t **factors,
                        size_t *count,
                        size_t *capacity);

int main(void)
{
    size_t num;

    printf("Enter positive number to give out prime factors:\n");
    scanf("%zu", &num);

    // Prime factorization is only defined for numbers >= 2.
    if (num < 2)
    {
        printf("Number must be at least 2.\n");
        return 1;
    }

    size_t count;

    // Calculate the prime factors.
    size_t *prime_factors = primeFactors(num, &count);

    if (prime_factors == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // Print all stored prime factors.
    for (size_t i = 0; i < count; i++)
    {
        printf("%zu ", prime_factors[i]);
    }

    printf("\n");

    // Release dynamically allocated memory.
    free(prime_factors);

    return 0;
}

size_t *primeFactors(size_t num, size_t *count)
{
    // Initial array capacity. The array grows automatically if needed.
    size_t capacity = 4;

    // No factors have been found yet.
    *count = 0;

    // Allocate memory for the factor array.
    size_t *factors = malloc(capacity * sizeof(size_t));

    if (factors == NULL)
    {
        return NULL;
    }

    // Start the recursive search using the first possible prime factor (2).
    primeFactorsHelper(num,
                       2,
                       &factors,
                       count,
                       &capacity);

    return factors;
}

void primeFactorsHelper(size_t num,
                        size_t prime,
                        size_t **factors,
                        size_t *count,
                        size_t *capacity)
{
    // Base case: all prime factors have been extracted.
    if (num == 1)
    {
        return;
    }

    // If no divisor up to sqrt(num) exists,
    // the remaining number must be prime.
    if (prime * prime > num)
    {
        if (*count == *capacity)
        {
            *capacity *= 2;

            size_t *temp = realloc(*factors,
                                   (*capacity) * sizeof(size_t));

            if (temp == NULL)
            {
                printf("realloc failed\n");
                return;
            }

            *factors = temp;
        }

        (*factors)[*count] = num;
        (*count)++;

        return;
    }

    // Check whether the current divisor is a prime factor.
    if (num % prime == 0)
    {
        // Grow the array if there is no free space left.
        if (*count == *capacity)
        {
            *capacity *= 2;

            size_t *temp = realloc(*factors,
                                   (*capacity) * sizeof(size_t));

            if (temp == NULL)
            {
                printf("realloc failed\n");
                return;
            }

            *factors = temp;
        }

        // Store the discovered prime factor.
        (*factors)[*count] = prime;
        (*count)++;

        // Remove the factor from the remaining number.
        num /= prime;

        // Continue with the same prime because it may divide again
        // (e.g. 36 = 2 × 2 × 3 × 3).
        primeFactorsHelper(num,
                           prime,
                           factors,
                           count,
                           capacity);
    }
    else
    {
        // Move to the next possible divisor.
        // After 2, only odd numbers need to be tested.
        if (prime == 2)
        {
            prime = 3;
        }
        else
        {
            prime += 2;
        }

        primeFactorsHelper(num,
                           prime,
                           factors,
                           count,
                           capacity);
    }
}
