typedef signed char int8_t
typedef unsigned char uint8_t
typedef signed int int16_t
typedef unsigned int uint16_t
typedef signed long int int32_t
typedef unsigned long int uint32_t
typedef signed long long int int64_t
typedef unsigned long long int uint64_t

typedef int16_t intptr_t
typedef uint16_t uintptr_t

typedef int8_t int_least8_t
typedef uint8_t uint_least8_t
typedef int16_t int_least16_t
typedef uint16_t uint_least16_t
typedef int32_t int_least32_t
typedef uint32_t uint_least32_t
typedef int64_t int_least64_t
typedef uint64_t uint_least64_t

typedef int8_t int_fast8_t
typedef uint8_t uint_fast8_t
typedef int16_t int_fast16_t
typedef uint16_t uint_fast16_t
typedef int32_t int_fast32_t
typedef uint32_t uint_fast32_t
typedef int64_t int_fast64_t
typedef uint64_t uint_fast64_t

typedef int64_t intmax_t
typedef uint64_t uintmax_t

#define INT8_MAX 0x7f
#define INT8_MIN (-INT8_MAX - 1)
#define UINT8_MAX (__CONCAT(INT8_MAX, U) * 2U + 1U)
#define INT16_MAX 0x7fff
#define INT16_MIN (-INT16_MAX - 1)
#define UINT16_MAX (__CONCAT(INT16_MAX, U) * 2U + 1U)
#define INT32_MAX 0x7fffffffL
#define INT32_MIN (-INT32_MAX - 1L)
#define UINT32_MAX (__CONCAT(INT32_MAX, U) * 2UL + 1UL)
#define INT64_MAX 0x7fffffffffffffffLL
#define INT64_MIN (-INT64_MAX - 1LL)
#define UINT64_MAX (__CONCAT(INT64_MAX, U) * 2ULL + 1ULL)

#define INT_LEAST8_MAX INT8_MAX
#define INT_LEAST8_MIN INT8_MIN
#define UINT_LEAST8_MAX UINT8_MAX
#define INT_LEAST16_MAX INT16_MAX
#define INT_LEAST16_MIN INT16_MIN
#define UINT_LEAST16_MAX UINT16_MAX
#define INT_LEAST32_MAX INT32_MAX
#define INT_LEAST32_MIN INT32_MIN
#define UINT_LEAST32_MAX UINT32_MAX
#define INT_LEAST64_MAX INT64_MAX
#define INT_LEAST64_MIN INT64_MIN
#define UINT_LEAST64_MAX UINT64_MAX

#define INT_FAST8_MAX INT8_MAX
#define INT_FAST8_MIN INT8_MIN
#define UINT_FAST8_MAX UINT8_MAX
#define INT_FAST16_MAX INT16_MAX
#define INT_FAST16_MIN INT16_MIN
#define UINT_FAST16_MAX UINT16_MAX
#define INT_FAST32_MAX INT32_MAX
#define INT_FAST32_MIN INT32_MIN
#define UINT_FAST32_MAX UINT32_MAX
#define INT_FAST64_MAX INT64_MAX
#define INT_FAST64_MIN INT64_MIN
#define UINT_FAST64_MAX UINT64_MAX
#define INTPTR_MAX INT16_MAX
#define INTPTR_MIN INT16_MIN
#define UINTPTR_MAX UINT16_MAX

#define INTMAX_MAX INT64_MAX
#define INTMAX_MIN INT64_MIN
#define UINTMAX_MAX UINT64_MAX

#define PTRDIFF_MAX INT16_MAX
#define PTRDIFF_MIN INT16_MIN
#define SIG_ATOMIC_MAX INT8_MAX
#define SIG_ATOMIC_MIN INT8_MIN
#define SIZE_MAX (__CONCAT(INT16_MAX, U))

#define INT8_C(value) ((int8_t) value)
#define UINT8_C(value) ((uint8_t) __CONCAT(value, U))
#define INT16_C(value) value
#define UINT16_C(value) __CONCAT(value, U)
#define INT32_C(value) __CONCAT(value, L)
#define UINT32_C(value) __CONCAT(value, UL)
#define INT64_C(value) __CONCAT(value, LL)
#define UINT64_C(value) __CONCAT(value, ULL)
#define INTMAX_C(value) __CONCAT(value, LL)
#define UINTMAX_C(value) __CONCAT(value, ULL)

#define INT16_C(value) value

#define INT16_MAX 0x7fff

#define INT16_MIN (-INT16_MAX - 1)

#define INT32_C(value) __CONCAT(value, L)

#define INT32_MAX 0x7fffffffL

#define INT32_MIN (-INT32_MAX - 1L)

#define INT64_C(value) __CONCAT(value, LL)

#define INT64_MAX 0x7fffffffffffffffLL

#define INT64_MIN (-INT64_MAX - 1LL)

#define INT8_C(value) ((int8_t) value)

#define INT8_MAX 0x7f

#define INT8_MIN (-INT8_MAX - 1)

#define INT_FAST16_MAX INT16_MAX

#define INT_FAST16_MIN INT16_MIN

#define INT_FAST32_MAX INT32_MAX

#define INT_FAST32_MIN INT32_MIN

#define INT_FAST64_MAX INT64_MAX

#define INT_FAST64_MIN INT64_MIN

#define INT_FAST8_MAX INT8_MAX

#define INT_FAST8_MIN INT8_MIN

#define INT_LEAST16_MAX INT16_MAX

#define INT_LEAST16_MIN INT16_MIN

#define INT_LEAST32_MAX INT32_MAX

#define INT_LEAST32_MIN INT32_MIN

#define INT_LEAST64_MAX INT64_MAX

#define INT_LEAST64_MIN INT64_MIN

#define INT_LEAST8_MAX INT8_MAX

#define INT_LEAST8_MIN INT8_MIN

#define INTMAX_C(value) __CONCAT(value, LL)

#define INTMAX_MAX INT64_MAX

#define INTMAX_MIN INT64_MIN

#define INTPTR_MAX INT16_MAX

#define INTPTR_MIN INT16_MIN

#define PTRDIFF_MAX INT16_MAX

#define PTRDIFF_MIN INT16_MIN

#define SIG_ATOMIC_MAX INT8_MAX

#define SIG_ATOMIC_MIN INT8_MIN

#define SIZE_MAX (__CONCAT(INT16_MAX, U))

#define UINT16_C(value) __CONCAT(value, U)

#define UINT16_MAX (__CONCAT(INT16_MAX, U) * 2U + 1U)

#define UINT32_C(value) __CONCAT(value, UL)

#define UINT32_MAX (__CONCAT(INT32_MAX, U) * 2UL + 1UL)

#define UINT64_C(value) __CONCAT(value, ULL)

#define UINT64_MAX (__CONCAT(INT64_MAX, U) * 2ULL + 1ULL)

#define UINT8_C(value) ((uint8_t) __CONCAT(value, U))

#define UINT8_MAX (__CONCAT(INT8_MAX, U) * 2U + 1U)

#define UINT_FAST16_MAX UINT16_MAX

#define UINT_FAST32_MAX UINT32_MAX

#define UINT_FAST64_MAX UINT64_MAX

#define UINT_FAST8_MAX UINT8_MAX

#define UINT_LEAST16_MAX UINT16_MAX

#define UINT_LEAST32_MAX UINT32_MAX

#define UINT_LEAST64_MAX UINT64_MAX

#define UINT_LEAST8_MAX UINT8_MAX

#define UINTMAX_C(value) __CONCAT(value, ULL)

#define UINTMAX_MAX UINT64_MAX

#define UINTPTR_MAX UINT16_MAX

typedef signed long int int32_t

typedef signed long long int int64_t

typedef signed char int8_t

typedef int16_t int_fast16_t

typedef int32_t int_fast32_t

typedef int64_t int_fast64_t

typedef int8_t int_fast8_t

typedef int16_t int_least16_t

typedef int32_t int_least32_t

typedef int64_t int_least64_t

typedef int8_t int_least8_t

typedef int64_t intmax_t

typedef int16_t intptr_t

typedef unsigned int uint16_t

typedef unsigned long int uint32_t

typedef unsigned long long int uint64_t

typedef unsigned char uint8_t

typedef uint16_t uint_fast16_t

typedef uint32_t uint_fast32_t

typedef uint64_t uint_fast64_t

typedef uint8_t uint_fast8_t

typedef uint16_t uint_least16_t

typedef uint32_t uint_least32_t

typedef uint64_t uint_least64_t
