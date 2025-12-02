#pragma once
#include <cstdint>
#include <stdexcept>
#include <ostream>

namespace ff {

    //to be used externally for template validation (ex. advanced math header)
    template <typename FE>
    concept FieldElementType = requires(FE a, FE b, uint64_t e, std::ostream& os) {
        { FE{} };

        { a + b } -> std::same_as<FE>;
        { a - b } -> std::same_as<FE>;
        { a * b } -> std::same_as<FE>;
        { a / b } -> std::same_as<FE>;

        { a += b } -> std::same_as<FE&>;
        { a -= b } -> std::same_as<FE&>;
        { a *= b } -> std::same_as<FE&>;
        { a /= b } -> std::same_as<FE&>;

        { a.inverse() } -> std::same_as<FE>;
        { a.pow(e) } -> std::same_as<FE>;

        { a == b } -> std::convertible_to<bool>;
        { a != b } -> std::convertible_to<bool>;

        { os << a } -> std::same_as<std::ostream&>;
    };

    //modular fast exponentiation
    template <typename T>
    constexpr T mod_pow (T base, T exp, T mod) {
        T result = 1;
        base = base % mod;

        while (exp > 0) {
            if (exp & 1) {
                result = (result * base) % mod;
            }
            exp >>= 1;
            base = (base * base) % mod;
        }
        return result;
    }

    //core compile-time namespace
    namespace core {

        // compile-time primality test
        template <typename T>
        constexpr bool is_prime_constexpr(T n) {
            if (n < 2) return false;
            if (n % 2 == 0) return n == 2;
            for (T i = 3; i * i <= n; i += 2)
                if (n % i == 0) return false;
            return true;
        }

        template <typename T, T P>
        class FieldElement {
            static_assert(is_prime_constexpr(P), "Modulus P must be a prime number for GF(p)!");

            public:
            T value;
            constexpr explicit FieldElement(T v = 0) : value(v % P) {} //constructor

            //addition
            constexpr FieldElement operator+(const FieldElement& o) const {
                return FieldElement((value + o.value) % P);
            }
            constexpr FieldElement& operator+=(const FieldElement& o) {
                value = (value + o.value) % P;
                return *this;
            }

            //subtraction
            constexpr FieldElement operator-(const FieldElement& o) const {
                return FieldElement((value - o.value) % P);
            }
            constexpr FieldElement& operator-=(const FieldElement& o) {
                value = (P + value - o.value) % P;
                return *this;
            }

            //multiplication
            constexpr FieldElement operator*(const FieldElement& o) const {
                if constexpr (sizeof(T) == 8) { //safe for uint64_t
                    auto tmp = static_cast<__uint128_t> (value * o.value);
                    return FieldElement(static_cast<T>(tmp % P));
                } else { //safe for uint32_t
                    auto tmp = static_cast<uint64_t>(value * o.value);
                    return FieldElement(static_cast<T>(tmp % P));
                }
            }
            constexpr FieldElement& operator*=(const FieldElement& o) {
                value = (value * o.value) % P;
                return *this;
            }

            //modular exponentiation call
            constexpr FieldElement pow(uint64_t exp) const {
                return FieldElement(mod_pow(value, exp, P));
            }

            //modular inverse
            constexpr FieldElement inverse() const {
                if (value == 0) {
                    throw std::invalid_argument("Division by zero!");
                }
                return FieldElement(mod_pow(value, P - 2, P));
            }

            //modular division
            constexpr FieldElement operator/(const FieldElement& o) const {
                return *this * o.inverse();
            }
            constexpr FieldElement& operator/=(const FieldElement& o) {
                return (*this *= o.inverse());
            }

            //comparison operators
            constexpr bool operator==(FieldElement& o) const{
                return value == o.value;
            }
            constexpr bool operator!=(FieldElement& o) const{
                return value != o.value;
            }
        };

        //cout implementation
        template <typename T, T P>
        std::ostream& operator<<(std::ostream& os, const FieldElement<T, P>& a) {
            os << a.value;
            return os;
        }
    }

    //for faster CPUs, 64-bit modular arithmetic
    namespace bit64 {
        template <uint64_t P>
        using FieldElement = core::FieldElement<uint64_t, P>;
    }

    //for low-power CPUs, 32-bit modular arithmetic
    namespace bit32 {
        template <uint32_t P>
        using FieldElement = core::FieldElement<uint32_t, P>;
    }

    //TODO: runtime namespace, ability to implement finite space modulus during runtime
}

