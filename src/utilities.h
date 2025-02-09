#ifndef UTILITIES_H
#define UTILITIES_H

#include "legguino.h"

#define TEST 0

void read_until_no_more(void);
void print_received_data(byte answer[3]);
void simulate_tester(void);

#if !defined(UNUSED)
#define UNUSED(x) (void)(x)
#endif

#endif /* UTILITIES_H */