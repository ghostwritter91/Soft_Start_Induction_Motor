/*
 * time.h
 *
 * Created: 02.02.2023 11:00:44
 *  Author: Paweł
 */ 

#ifndef TIME_INCLUDED
#define TIME_INCLUDED

#include <stdbool.h>

void TIME_Init(void);
bool TIME_IsHalfSecondPassed(void);
bool TIME_IsOneSecondPassed(void);

#endif // TIME_INCLUDED