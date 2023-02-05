/*
 * time.c
 *
 * Created: 02.02.2023 11:00:18
 *  Author: Paweł
 */ 

#include "time.h"
#include "pwm_basic.h"

static volatile bool one_second = false;

static void TIME_Callback(void) {
	one_second = true;
}

void TIME_Init(void) {
	PWM_0_register_callback((pwm_irq_cb_t)TIME_Callback);
}

bool TIME_IsOneSecondPassed(void) {
	if(one_second) {
		one_second = false;
		return true;
	}
	else {
		return false;
	}
}
