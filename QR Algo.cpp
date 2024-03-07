#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <gmp.h>

// Function to check if a number is a quadratic residue modulo p
bool isQuadraticResidue(mpz_t a, mpz_t p) {
    mpz_t result;
    mpz_init(result);
    mpz_powm_ui(result, a, 2, p);  // result = a^2 mod p
    bool isQR = mpz_legendre(a, p) == 1;  // Check if result is 1
    mpz_clear(result);
    return isQR;
}

// Function to generate a random prime number of specified bit length
void generatePrimes(mpz_t p, int bit_length) {
    gmp_randstate_t state;
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    mpz_urandomb(p, state, bit_length);
    mpz_nextprime(p, p);
    gmp_randclear(state);
}

// Function to generate QRs and QNRs modulo p
void generateQRsAndQNRs(std::vector<mpz_t>& QRs, std::vector<mpz_t>& QNRs, mpz_t p) {
    mpz_t a, result;
    mpz_inits(a, result, NULL);

    for (mpz_set_ui(a, 1); mpz_cmp(a, p) < 0; mpz_add_ui(a, a, 1)) {
        if (isQuadraticResidue(a, p)) {
            mpz_t qr;
            mpz_init_set(qr, a);
            QRs.push_back(qr);
        } else {
            mpz_t qnr;
            mpz_init_set(qnr, a);
            QNRs.push_back(qnr);
        }
    }

    mpz_clears(a, result, NULL);
}

// Function to choose random QRs and QNR
void chooseRandomElements(const std::vector<mpz_t>& QRs, const std::vector<mpz_t>& QNRs, std::vector<mpz_t>& selectedQRs, mpz_t& selectedQNR, int n) {
    srand(time(NULL));
    std::vector<bool> chosen(QRs.size(), false);
    while (selectedQRs.size() < n - 1) {
        int index = rand() % QRs.size();
        if (!chosen[index]) {
            mpz_t qr;
            mpz_init_set(qr, QRs[index]);
            selectedQRs.push_back(qr);
            chosen[index] = true;
        }
    }

    int index = rand() % QNRs.size();
    mpz_init_set(selectedQNR, QNRs[index]);
}

int main() {
    int bit_length = 10;  // Length of the prime number in bits
    int n = 10;  // Size of the database

    mpz_t p;
    mpz_init(p);
    generatePrimes(p, bit_length);

    std::vector<mpz_t> QRs, QNRs;
    generateQRsAndQNRs(QRs, QNRs, p);

    std::vector<mpz_t> selectedQRs;
    mpz_t selectedQNR;
    chooseRandomElements(QRs, QNRs, selectedQRs, selectedQNR, n);

    std::cout << "Prime number P: ";
    mpz_out_str(stdout, 10, p);
    std::cout << std::endl;

    std::cout << "Selected QRs: " << std::endl;
    for (const auto& qr : selectedQRs) {
        mpz_out_str(stdout, 10, qr);
        std::cout << " ";
    }
    std::cout << std::endl;

    std::cout << "Selected QNR: ";
    mpz_out_str(stdout, 10, selectedQNR);
    std::cout << std::endl;

    mpz_clear(p);
    for (auto& qr : QRs) mpz_clear(qr);
    for (auto& qnr : QNRs) mpz_clear(qnr);
    for (auto& qr : selectedQRs) mpz_clear(qr);
    mpz_clear(selectedQNR);

    return 0;
}