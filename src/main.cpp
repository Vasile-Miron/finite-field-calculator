#include <iostream>
#include <sstream>
#include <finiteFieldElement.hpp>

using FE = ff::runtime::FieldElement;

//modulus input validation
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

//terms input validation
void termsInput (FE& a, FE& b, int32_t& bInt, const uint64_t& modulus, const uint8_t& opIndex) {
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
    switch (opIndex) {
        case 5:
            while (true) {
                std::cout << "Enter exponent b";

                if (!(std::cin >> bInt)) {
                    std::cout << "Invalid input: Expected an integer.\n\n" << std::flush;
                    std::cin.clear();
                    std::cin.ignore(9999, '\n');
                } else break;
            }

        case 6: return;

        default:
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
}

//operation index input validation
uint8_t operationSelect (const uint8_t& indexLimit) {
    uint8_t opIndex = 0; //default

    while (true) {
        std::cout << "What operation would you like to do?\n";
        std::cout << "1. Addition\n";
        std::cout << "2. Subtraction\n";
        std::cout << "3. Multiplication\n";
        std::cout << "4. Division\n";
        std::cout << "5. Exponentiation\n";
        std::cout << "6. Inverse\n";
        std::cout << "Input the number of your operation (0 means no operation) : ";

        if (!(std::cin >> opIndex)) {
            std::cout << "Invalid input: Expected a positive integer between 1 and " << indexLimit << ".\n\n" << std::flush;
            std::cin.clear();
            std::cin.ignore(9999, '\n');
        } else if (opIndex > indexLimit) {
            std::cout << "Invalid input: Expected a positive integer between 1 and " << indexLimit << ".\n\n" << std::flush;
        } else break;
    }

    return opIndex;
}

int main() {

    //TODO: implement calculator program using the finiteFieldElement header
    //TODO: consider and create advanced math header for finite field operations

    std::ios::sync_with_stdio(false);
    const uint64_t modulus = modInput();
    constexpr uint8_t indexLimit = 6;
    int32_t bInt = 0;
    FE a, b;

    const uint8_t opIndex = operationSelect(indexLimit);
    termsInput(a, b, bInt, modulus, opIndex);

    return 0;
}