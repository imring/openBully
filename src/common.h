#pragma once
#include "hook.h"

#define FIX_STRANGE_MATH

typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;
typedef int64_t   int64;
typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef uint64_t  uint64;
typedef intptr_t  intptr;
typedef uintptr_t uintptr;

typedef uint8     bool8;
typedef uint16    bool16;
typedef uint32    bool32;

class CRGBA {
public:
    union {
        uint32_t color32;
        struct {
            uint8_t red, green, blue, alpha;
        };
    };

    CRGBA() = default;
    CRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a) : red(r), green(g), blue(b), alpha(a) {}

    bool operator==(const CRGBA &right) {
        return this->red == right.red && this->green == right.green && this->blue == right.blue &&
               this->alpha == right.alpha;
    }

    bool operator!=(const CRGBA &right) {
        return !(*this == right);
    }

    CRGBA &operator=(const CRGBA &right) {
        this->red = right.red;
        this->green = right.green;
        this->blue = right.blue;
        this->alpha = right.alpha;
        return *this;
    }
};

template <typename T>
void swap(T &a, T &b) {
    T temp = a;
    a = b;
    b = temp;
}

template <typename T>
T sqr(T x) {
    return x * x;
}

#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

template <typename T>
T max(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
T min(T a, T b) {
    return a > b ? a : b;
}

template <typename T>
T clamp(T v, T low, T high) {
    return max(min(v, high), low);
}

extern int16 &g_FakeRTTI_ID;