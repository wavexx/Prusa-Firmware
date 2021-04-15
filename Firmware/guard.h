#pragma once

#include <stdint.h>

#define GUARD_UNDER
#define GUARD_OVER
#define GUARD_RECURSION

typedef uint8_t GuardType;
static constexpr GuardType GuardValue = 0xA5;

void guard_reset(void* start, uint8_t size, uint16_t len);
void guard_init_str(void* start, uint8_t size, uint16_t len, const char* str);
void* guard_check_reset(void* start, uint8_t size, uint16_t len);
void* guard_check_index(void* start, uint8_t size, uint16_t len, uint16_t i);

template<typename T, uint16_t N> class Guard
{
    struct __attribute__((packed))
    {
#ifdef GUARD_UNDER
        volatile GuardType guard1;
#endif
        T buf[N];
#ifdef GUARD_OVER
        volatile GuardType guard2;
#endif
    };

 public:
    void reset()
    {
        guard_reset(this, sizeof(T), N);
    }

    void* addr()
    {
        return this;
    }


    Guard()
    {
        reset();
    }

    Guard(const char* str)
    {
        guard_init_str(this, sizeof(T), N, str);
    }

    Guard(T v0)
    {
        buf[0] = v0;
        reset();
    }

    Guard(T v0, T v1)
    {
        buf[0] = v0;
        buf[1] = v1;
        reset();
    }

    Guard(T v0, T v1, T v2)
    {
        buf[0] = v0;
        buf[1] = v1;
        buf[2] = v2;
        reset();
    }

    Guard(T v0, T v1, T v2, T v3)
    {
        buf[0] = v0;
        buf[1] = v1;
        buf[2] = v2;
        buf[3] = v3;
        reset();
    }

    Guard(T v0, T v1, T v2, T v3, T v4, T v5, T v6)
    {
        buf[0] = v0;
        buf[1] = v1;
        buf[2] = v2;
        buf[3] = v3;
        buf[4] = v4;
        buf[5] = v5;
        buf[6] = v6;
        reset();
    }

    ~Guard()
    {
        guard_check_reset(this, sizeof(T), N);
    }

    uint16_t size() const
    {
        return sizeof(buf);
    }

    void zero()
    {
        memset(buf, 0, sizeof(buf));
    }

    T* get()
    {
        return guard_check_reset(this, sizeof(T), N);
    }

    operator T*()
    {
        return guard_check_reset(this, sizeof(T), N);
    }

    T& operator[](int i)
    {
        return *(T*)guard_check_index(this, sizeof(T), N, i);
    }
};


void* guard_memcpy_check(void* dst, void* src, uint16_t size, uint8_t t_size);

template<typename T, uint16_t N>
void*
memcpy(Guard<T, N>& dst, const Guard<T, N>& src, uint16_t size)
{
    return guard_memcpy_check(dst.addr(), src.addr(), size, sizeof(dst));
}


class RecSentinel
{
    uint8_t* addr;

public:
    static constexpr uint8_t def_limit = 2;

    RecSentinel(uint8_t& addr, uint8_t limit = def_limit);
    ~RecSentinel();
};


#ifdef GUARD_RECURSION
#define REC_SENTINEL_N(name, n) static uint8_t __ ## name ## _d = 0; RecSentinel __ ## name ## _s(__ ## name ## _d, n)
#else
#define REC_SENTINEL_N(name, n)
#endif
#define REC_SENTINEL(name) REC_SENTINEL_N(name, RecSentinel::def_limit)
