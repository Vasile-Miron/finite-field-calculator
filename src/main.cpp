#include <iostream>
#include <sstream>
#include <finiteFieldElement.hpp>

int main() {

    //TODO: implement calculator program using the finiteFieldElement header
    //TODO: consider and create advanced math header for finite field operations

    std::ios::sync_with_stdio(false);
    uint64_t mod;
    std::ostringstream err;

    //input validation loop
    while (true) {
        std::cout << "Enter prime modulus p: ";

        if (!(std::cin >> mod)) {
            err << "Invalid input: Expected a positive integer.";
            std::cin.clear();
            std::cin.ignore(9999, '\n');
        } else {
            try {
                ff::runtime::FieldElement::setModulus(mod);
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


    return 0;
}