s/\<\(\(unsigned\s\+\)\?\(char\|int\|long\)\|u\?int[0-9]\+_t\|float\|double\|bool\)\>\s\+\([a-zA-Z0-9_]\+\)\s*\[\([^]]\+\)]\s*/Guard<\1, \5> \4/
