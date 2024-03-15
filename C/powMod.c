#include <stdio.h>
#include <gmp.h>
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

void test_modular_exponentiation() {
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_t a, e, m, r;
    mpz_init(a);
    mpz_init(e);
    mpz_init(m);
    mpz_init(r);

    for (int i = 0; i < 1000; i++) {
        mpz_urandomb(a, state, 2000);  // Generate a random 2000-bit number for a
        mpz_urandomb(e, state, 2000);  // Generate a random 2000-bit number for e
        mpz_urandomb(m, state, 2000);  // Generate a random 2000-bit number for m
        mpz_set_ui(r, 1);  // Set r to 1

        modular_exponentiation(r, a, e, m);
        // gmp_printf("Test %d: Result: %Zd\n", i + 1, r);
        printf("Test %d\n", i + 1);
    }

    mpz_clear(a);
    mpz_clear(e);
    mpz_clear(m);
    mpz_clear(r);
    gmp_randclear(state);
}

int main() {
    test_modular_exponentiation();
    return 0;
}

// do not forget ---> gcc <file.c> -lgmp -o <executable_name>