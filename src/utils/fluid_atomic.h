
#ifndef _FLUID_ATOMIC_H
#define _FLUID_ATOMIC_H

#include <stdbool.h>

#if __STDC_VERSION__ >= 201112L
#define STATIC_ASSERT _Static_assert
#else
#define STATIC_ASSERT(expr, msg) typedef char __static_assertion_ ## __COUNTER__[(expr) ? 1 : -1]
#endif

#if (__STDC_VERSION__ >= 201112L) && !defined(__STDC_NO_ATOMICS__)
#include <stdatomic.h>

#define fluid_atomic_int_add(atomic, val) atomic_fetch_add((atomic), (val))
#define fluid_atomic_int_get(atomic) atomic_load((atomic))
#define fluid_atomic_int_set(atomic, val) atomic_store((atomic), (val))
#define fluid_atomic_int_inc(atomic) atomic_fetch_add((atomic), 1)
#define fluid_atomic_int_compare_and_exchange(atomic, oldval, newval) \
    atomic_compare_exchange_strong((atomic), (oldval), (newval))

#elif defined(__GCC_HAVE_SYNC_COMPARE_AND_SWAP_4)

typedef struct { volatile int value; } atomic_int;
typedef struct { volatile unsigned value; } atomic_uint;

#define fluid_atomic_int_add(atomic, val) ({                          \
            STATIC_ASSERT(sizeof((atomic)->value) == sizeof(int),     \
                          "Atomic must be the size of an int");       \
            __sync_fetch_and_add(&(atomic)->value, (val));})

#define fluid_atomic_int_get(atomic) ({                           \
            STATIC_ASSERT(sizeof((atomic)->value) == sizeof(int), \
                          "Atomic must be the size of an int");   \
            __sync_synchronize();                                 \
            (atomic)->value;})

#define fluid_atomic_int_set(atomic, newval) ({                       \
                STATIC_ASSERT(sizeof((atomic)->value) == sizeof(int), \
                              "Atomic must be the size of an int");   \
                (atomic)->value = (newval);                           \
                __sync_synchronize();})

#define fluid_atomic_int_inc(atomic) ({                           \
            STATIC_ASSERT(sizeof((atomic)->value) == sizeof(int), \
                          "Atomic must be the size of an int");   \
            __sync_synchronize();                                 \
            __sync_fetch_and_add(&(atomic)->value, 1);})

#define fluid_atomic_int_compare_and_exchange(atomic, oldval, newval) ({ \
            STATIC_ASSERT(sizeof((atomic)->value) == sizeof(int),       \
                          "Atomic must be the size of an int");         \
            __sync_bool_compare_and_swap(&(atomic)->value, (oldval), (newval));})

#endif


#endif /* _FLUID_ATOMIC_H */
