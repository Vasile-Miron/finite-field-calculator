#include <iostream>
#include <sstream>
#include <finiteFieldElement.hpp>

using FE = ff::runtime::FieldElement;

uint64_t modInput () {
    std::ostringstream err;
    uint64_t modulus = 2;

    while (true) {
        std::cout << "Enter prime modulus p: ";

        if (!(std::cin >> modulus)) {
            err << "Invalid input: Expected a positive integer.";
            std::cin.clear();
            std::cin.ignore(9999, '\n');
        } else {
            try {
                ff::runtime::FieldElement::setModulus(modulus);
                std::cout << "Initialized finite field GF(" << modulus << ") successfully!" << std::endl;
                break;
            } catch (const std::invalid_argument& e) {
                err << "Invalid modulus: " << e.what();
            }
        }

        if (!err.str().empty()) {
            std::cout << " " << err.str() << "\n\n" << std::flush;
            err.str("");
            err.clear();
        }
    }
    return modulus;
}

void termsInput (FE& a, FE& b, const uint64_t& modulus) {
    uint64_t aRaw;
    uint64_t bRaw;

    std::cout << std::endl;
    while (true) {
        std::cout << "Enter term a in GF(" << modulus << "): ";

        if (!(std::cin >> aRaw)) {
            std::cout << "Invalid input: Expected an integer.\n\n" << std::flush;
            std::cin.clear();
            std::cin.ignore(9999, '\n');
        } else break;
    }
    a = FE(aRaw);
    std::cout << "Initialized term GF(" << modulus << ") a = " << a << " successfully!" << std::endl;

    while (true) {
        std::cout << "Enter term b in GF(" << modulus << "): ";

        if (!(std::cin >> bRaw)) {
            std::cout << "Invalid input: Expected an integer.\n\n" << std::flush;
            std::cin.clear();
            std::cin.ignore(9999, '\n');
        } else break;
    }
    b = FE(bRaw);
    std::cout << "Initialized term GF(" << modulus << ") b = " << b << " successfully!" << std::endl;
}

int main() {

    //TODO: implement calculator program using the finiteFieldElement header
    //TODO: consider and create advanced math header for finite field operations

    std::ios::sync_with_stdio(false);
    const uint64_t modulus = modInput();
    FE a, b;

    termsInput(a, b, modulus);


    return 0;
}