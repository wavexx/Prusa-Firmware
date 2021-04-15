#include "guard.h"

#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>

#define GUARD_GUARD1 ((GuardType*)start)

#ifdef GUARD_UNDER
#define GUARD_BUF ((void*)((char*)start + sizeof(GuardType)))
#else
#define GUARD_BUF (start)
#endif

#define GUARD_GUARD2 ((GuardType*)((char*)GUARD_BUF + size*len))

void guard_reset(void* start, uint8_t size, uint16_t len)
{
#ifdef GUARD_UNDER
    *GUARD_GUARD1 = GuardValue;
#endif
#ifdef GUARD_OVER
    *GUARD_GUARD2 = GuardValue;
#endif
}

extern "C" void __attribute__((noinline,noclone,no_icf,externally_visible)) guard_hook()
{
    printf_P(PSTR("GUARD "));
}

void guard_init_str(void* start, uint8_t size, uint16_t len, const char* str)
{
    void* buf = GUARD_BUF;
    uint16_t slen = strlen(str) + 1;
    if(slen > len)
    {
        guard_hook();
        printf_P(PSTR("= %06p %06p %u %u\n"), start, buf, slen, len);
    }
    memcpy(buf, str, slen);
    guard_reset(start, size, len);
}

void* guard_check_reset(void* start, uint8_t size, uint16_t len)
{
    void* buf = GUARD_BUF;
#ifdef GUARD_UNDER
    GuardType& guard1 = *GUARD_GUARD1;
    if(guard1 != GuardValue)
    {
        guard_hook();
        printf_P(PSTR("- %06p %06p %06p\n"), start, buf, &guard1);
        guard1 = GuardValue;
    }
#endif
#ifdef GUARD_OVER
    GuardType& guard2 = *GUARD_GUARD2;
    if(guard2 != GuardValue)
    {
        guard_hook();
        printf_P(PSTR("+ %06p %06p %06p\n"), start, buf, &guard2);
        guard2 = GuardValue;
    }
#endif
    return buf;
}

void* guard_check_index(void* start, uint8_t size, uint16_t len, uint16_t i)
{
    guard_check_reset(start, size, len);
    void* buf = GUARD_BUF;
    if(i >= len)
    {
        guard_hook();
        printf_P(PSTR("[ %06p %06p %d %d\n"), start, buf, i, len);
    }
    return (char*)buf + size*i;
}


void* guard_memcpy_check(void* dst, void* src, uint16_t size, uint8_t t_size)
{
    memcpy(dst, src, size);
    if(size != t_size)
    {
        guard_hook();
        printf_P(PSTR("= %06p %06p %d %d\n"), dst, src, size, t_size);
    }
    return dst;
}


RecSentinel::RecSentinel(uint8_t& addr, uint8_t limit)
{
    this->addr = &addr;
    if(++addr > limit)
    {
        guard_hook();
        printf_P(PSTR("r %06p %d %d\n"), this->addr, addr, limit);
    }
}

RecSentinel::~RecSentinel()
{
    if(!(*this->addr)--)
    {
        guard_hook();
        printf_P(PSTR("d %06p %d\n"), this->addr, addr);
    }
}
