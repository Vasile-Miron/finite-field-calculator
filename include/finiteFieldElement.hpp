#pragma once
#include <cstdint>
#include <stdexcept>
#include <ostream>
#include <type_traits>

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
    constexpr T modPow (T base, T exp, T mod) {
        using safeType = std::conditional_t<(sizeof(T) == 8), __uint128_t, uint64_t>;

        safeType result = 1;
        auto b = static_cast<safeType> (base) % mod;

        while (exp > 0) {
            if (exp & 1)
                result = (result * b) % mod;
            exp >>= 1;
            b = (b * b) % mod;
            }

        return static_cast<T>(result);
    }

    //core compile-time namespace
    namespace core {

        // compile-time primality test
        template <typename T>
        constexpr bool isPrimeConstexpr(const T& n) noexcept {
            if (n < 2) return false;
            if (!(n & 1)) return n == 2;

            for (T i = 3; i <= n / i; i += 2)
                if (n % i == 0) return false;
            return true;
        }

        template <typename T, T modulus>
        class FieldElement {
            static_assert(isPrimeConstexpr(modulus), "Modulus P must be a prime number for GF(p)!");

            public:
            T value;
            constexpr explicit FieldElement(T v = 0) : value(v % modulus) {} //constructor

            //addition
            [[nodiscard]] constexpr FieldElement operator+(const FieldElement& o) const noexcept {
                return FieldElement((value + o.value) % modulus);
            }
            constexpr FieldElement& operator+=(const FieldElement& o) noexcept {
                value = (value + o.value) % modulus;
                return *this;
            }

            //subtraction
            [[nodiscard]] constexpr FieldElement operator-(const FieldElement& o) const noexcept {
                return FieldElement((modulus + value - o.value) % modulus);
            }
            constexpr FieldElement& operator-=(const FieldElement& o) noexcept {
                value = (modulus + value - o.value) % modulus;
                return *this;
            }

            //multiplication
            [[nodiscard]] constexpr FieldElement operator*(const FieldElement& o) const noexcept {
                using safeType = std::conditional_t<(sizeof(T) == 8), __uint128_t, uint64_t>;

                const auto tmp = static_cast<safeType> (value) * o.value;
                return FieldElement(static_cast<T>(tmp % modulus));
            }
            constexpr FieldElement& operator*=(const FieldElement& o) noexcept {
                *this = *this * o;
                return *this;
            }

            //modular exponentiation call
            [[nodiscard]] constexpr FieldElement pow(uint64_t exp) const noexcept {
                return FieldElement(modPow(value, exp, modulus));
            }

            //modular inverse
            [[nodiscard]] FieldElement inverse() const {
                if (value == 0) {
                    throw std::invalid_argument("Division by zero!");
                }
                return FieldElement(modPow(value, modulus - 2, modulus));
            }

            //modular division
            [[nodiscard]] constexpr FieldElement operator/(const FieldElement& o) const {
                return *this * o.inverse();
            }
            constexpr FieldElement& operator/=(const FieldElement& o) {
                return (*this *= o.inverse());
            }

            //comparison operators
            constexpr bool operator==(const FieldElement& o) const noexcept {
                return value == o.value;
            }
            constexpr bool operator!=(const FieldElement& o) const noexcept {
                return value != o.value;
            }
        };

        //cout implementation
        template <typename T, T P>
        inline std::ostream& operator<<(std::ostream& os, const FieldElement<T, P>& a) {
            os << a.value;
            return os;
        }
    }

    //for faster CPUs, 64-bit modular arithmetic
    namespace bit64 {
        template <uint64_t modulus>
        using FieldElement = core::FieldElement<uint64_t, modulus>;
    }

    //for low-power CPUs, 32-bit modular arithmetic
    namespace bit32 {
        template <uint32_t modulus>
        using FieldElement = core::FieldElement<uint32_t, modulus>;
    }

    namespace runtime {

        // runtime primality test
        inline bool isPrime(const uint64_t& n) noexcept {
            if (n < 2) return false;
            if (n % 2 == 0) return n == 2;

            for (uint64_t i = 3; i <= n / i; i += 2)
                if (n % i == 0) return false;
            return true;
        }

        class FieldElement {
            public:
            static inline uint64_t modulus = 2;

            //modulus constructor
            static void setModulus(const uint64_t p) {
                if (!isPrime(p))
                    throw std::invalid_argument("Modulus P must be a prime number for GF(p)!");
                modulus = p;
            }

            uint64_t value;
            explicit FieldElement(const uint64_t v = 0) : value(v % modulus) {}

            //addition
            [[nodiscard]] FieldElement operator+(const FieldElement& o) const noexcept {
                return FieldElement((value + o.value) % modulus);
            }
            FieldElement& operator+=(const FieldElement& o) noexcept {
                value = (value + o.value) % modulus;
                return *this;
            }

            //subtraction
            [[nodiscard]] FieldElement operator-(const FieldElement& o) const noexcept {
                return FieldElement((modulus + value - o.value) % modulus);
            }
            FieldElement& operator-=(const FieldElement& o) noexcept {
                value = (modulus + value - o.value) % modulus;
                return *this;
            }

            //multiplication
            [[nodiscard]] FieldElement operator*(const FieldElement& o) const noexcept {
                const auto tmp = static_cast<__uint128_t>(value) * o.value;
                return FieldElement(static_cast<uint64_t>(tmp % modulus));
            }
            FieldElement& operator*=(const FieldElement& o) noexcept {
                const auto tmp = static_cast<__uint128_t>(value) * o.value;
                value = static_cast<uint64_t>(tmp % modulus);
                return *this;
            }

            //modular exponentiation call
            [[nodiscard]] FieldElement pow(const uint64_t exp) const noexcept {
                return FieldElement(modPow(value, exp, modulus));
            }

            //modular inverse
            [[nodiscard]] FieldElement inverse() const {
                if (value == 0) {
                    throw std::invalid_argument("Division by zero!");
                }
                return FieldElement(modPow(value, modulus - 2, modulus));
            }

            //modular division
            [[nodiscard]] FieldElement operator/(const FieldElement& o) const {
                return *this * o.inverse();
            }
            FieldElement& operator/=(const FieldElement& o) {
                return (*this *= o.inverse());
            }

            //comparison operators
            bool operator==(const FieldElement& o) const noexcept{
                return value == o.value;
            }
            bool operator!=(const FieldElement& o) const noexcept {
                return value != o.value;
            }
        };

        //cout implementation
        inline std::ostream& operator<<(std::ostream& os, const FieldElement& a) {
            os << a.value;
            return os;
        }
    }
}

//TODO documentation & good comments