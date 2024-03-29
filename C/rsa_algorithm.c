#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <time.h>


void modular_exponentiation(mpz_t result, const mpz_t base, const mpz_t exponent, const mpz_t modulus) {
    mpz_t a, e, r;
    mpz_init_set(a, base);
    mpz_init_set(e, exponent);
    mpz_init_set(r, result);

    mpz_mod(a, a, modulus);

    while (mpz_cmp_si(e, 0) > 0) {
        if (mpz_odd_p(e)) {
            mpz_mul(r, r, a);
            mpz_mod(r, r, modulus);
        }

        mpz_mul(a, a, a);
        mpz_mod(a, a, modulus);

        mpz_fdiv_q_ui(e, e, 2);
    }

    mpz_set(result, r);

    mpz_clear(a);
    mpz_clear(e);
    mpz_clear(r);
}

// Function to check if a number is even
int is_even(mpz_t n) {
    return mpz_even_p(n);
}

// Function to generate a random number in the range [min, max]
void rand_range(mpz_t result, mpz_t min, mpz_t max, gmp_randstate_t rand_state) {
    mpz_t range;
    mpz_init(range);
    mpz_sub(range, max, min);
    mpz_add_ui(range, range, 1);

    mpz_urandomm(result, rand_state, range);
    mpz_add(result, result, min);

    mpz_clear(range);
}

// Miller-Rabin primality test
int miller_test(mpz_t n, int confidence) {
    if (is_even(n) || mpz_cmp_ui(n, 3) < 0) {
        return 0; // False
    }
    if (mpz_cmp_ui(n, 3) == 0) {
        return 1; // True
    }

    mpz_t k, m, a, x, n_minus_1, two;
    mpz_inits(k, m, a, x, n_minus_1, two, NULL);
    mpz_set_ui(two, 2);
    mpz_sub_ui(n_minus_1, n, 1);
    mpz_set(m, n_minus_1);

    // Find k and m such that n - 1 = 2^k * m
    while (mpz_divisible_p(m, two)) {
        mpz_add_ui(k, k, 1);
        mpz_divexact(m, m, two);
    }

    gmp_randstate_t gmp_randstate;
    gmp_randinit_default(gmp_randstate);
    gmp_randseed_ui(gmp_randstate, time(NULL));

    for (int i = 0; i < confidence; i++) {
        // Generate a random number a in the range [2, n - 2]
        rand_range(a, two, n_minus_1);

        // Compute x = a^m % n
        mpz_powm(x, a, m, n);

        if (mpz_cmp_ui(x, 1) == 0 || mpz_cmp(x, n_minus_1) == 0) {
            continue;
        }

        int j;
        for (j = 0; j < mpz_get_ui(k) - 1; j++) {
            mpz_powm_ui(x, x, 2, n);
            if (mpz_cmp(x, n_minus_1) == 0) {
                break;
            }
        }
        if (j == mpz_get_ui(k) - 1) {
            return 0; // False
        }
    }

    // Clean up
    mpz_clears(k, m, a, x, n_minus_1, two, NULL);
    gmp_randclear(gmp_randstate);

    return 1; // True
}

// Function to check if a number is probably prime
int is_probably_prime(mpz_t n) {
    // Check if n is divisible by any of the small primes
    int small_primes[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29};
    mpz_t remainder;
    mpz_init(remainder);

    for (int i = 0; i < 10; i++) {
        mpz_mod_ui(remainder, n, small_primes[i]);
        if (mpz_cmp_ui(remainder, 0) == 0) {
            mpz_clear(remainder);
            return 0; // False
        }
    }

    // Use Miller-Rabin test for further checking
    int result = miller_test(n, 10);
    mpz_clear(remainder);
    return result;
}

// Function to generate a prime number of a given number of bits
void generate_prime(mpz_t result, int bits) {
    mpz_t min, max, n_test;
    mpz_inits(min, max, n_test, NULL);

    // Set the range for the random number generation
    mpz_ui_pow_ui(min, 2, bits - 1);
    mpz_ui_pow_ui(max, 2, bits);
    mpz_sub_ui(max, max, 1);

    // Initialize the random state
    gmp_randstate_t rand_state;
    gmp_randinit_default(rand_state);
    gmp_randseed_ui(rand_state, time(NULL));

    do {
        rand_range(n_test, min, max, rand_state);
        if (is_even(n_test)) {
            mpz_sub_ui(n_test, n_test, 1);
        }
    } while (!is_probably_prime(n_test));

    // Set the result
    mpz_set(result, n_test);

    // Clean up
    mpz_clears(min, max, n_test, NULL);
    gmp_randclear(rand_state);
}

void extended_gcd(mpz_t x, mpz_t y, mpz_t gcd, const mpz_t a, const mpz_t b) {
    if (mpz_cmp_ui(b, 0) == 0) {
        mpz_set_ui(x, 1);
        mpz_set_ui(y, 0);
        mpz_set(gcd, a);
    } else {
        mpz_t b_mod_a, a_div_b, temp_x, temp_y;
        mpz_inits(b_mod_a, a_div_b, temp_x, temp_y, NULL);
        mpz_mod(b_mod_a, a, b);
        mpz_fdiv_q(a_div_b, a, b);

        extended_gcd(temp_x, temp_y, gcd, b, b_mod_a);
        mpz_set(x, temp_y);
        mpz_mul(a_div_b, a_div_b, temp_y);
        mpz_sub(y, temp_x, a_div_b);

        mpz_clears(b_mod_a, a_div_b, temp_x, temp_y, NULL);
    }
}

mpz_t mod_inverse(const mpz_t phi, const mpz_t e) {
    mpz_t x, y, gcd, inverse;
    mpz_inits(x, y, gcd, inverse, NULL);

    extended_gcd(x, y, gcd, e, phi);
    if (mpz_cmp_ui(gcd, 1) != 0) {
        mpz_clears(x, y, gcd, inverse, NULL);
        fprintf(stderr, "Inverse modulaire n'existe pas\n");
        exit(EXIT_FAILURE);
    } else {
        mpz_mod(inverse, x, phi);
        mpz_clears(x, y, gcd, NULL);
        return inverse;
    }
}


int main() {
    return 0;
}