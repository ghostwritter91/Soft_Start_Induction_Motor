#include <atmel_start.h>
#include "time.h"

#define ADC_TIME_CTRL_CH 3
#define ADC_LOAD_DET_CH 4

#define MOTOR_TIME_OF_WORK_S 50

typedef enum {
	LEVEL_1,
	LEVEL_2,
	LEVEL_3,
	LEVEL_4,
	LEVEL_5,
} eLEVLES;
static eLEVLES current_level = LEVEL_1;

static uint16_t motor_time_of_work_counter_s = 0;
static bool is_motor_working = false;
static uint16_t MOTOR_PwmThresholds[] = {0x01, 0x05, 0x09, 0x13, 0x40};
/* How long will each level take in seconds - 5 options available depends on TIME_CTRL level*/
static uint16_t MOTOR_ThresholdsDuration_s[5][5] = {{3, 3, 3, 5, 3},
											      {2, 2, 2, 5, 2},
											      {2, 2, 2, 2, 2},
											      {1, 1, 1, 1, 1},
											      {0, 0, 0, 0, 0}};

/* Calculates duration of each period based on set level */
static uint16_t	MOTOR_GetTimeForThreshold(uint8_t threshiold);

/* Based on the ADC value set by user (TIME_CTRL) read what is the actual MOTOR_LevelsDuration_s and show it on LEDs*/
static eLEVLES MOTOR_GetLevel(uint16_t time_ctrl_val);

/* Soft start of the motor */
static void MOTOR_SoftStart(void);

/* 0 - one diode turned on
 * 1 - two diodes turned on
 * 2 - three diodes turned on
 * etc.. */
static void LED_SetIndication(const uint8_t num_of_leds_to_turn_on);

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	ENABLE_INTERRUPTS();
	
	/*PWM timer is also used as a time base to get a tick every 0.5 s*/
	TIME_Init();

	/* Replace with your application code */
	while (1) {
		if(TIME_IsOneSecondPassed()) {
			if(!is_motor_working) {
				current_level = MOTOR_GetLevel(ADC_0_get_conversion(ADC_TIME_CTRL_CH));
				LED_SetIndication(current_level);
			}
			else {
				motor_time_of_work_counter_s++;
				MOTOR_SoftStart();
			}
			if((ADC_0_get_conversion(ADC_LOAD_DET_CH) < 200) && !is_motor_working) {
				/*Start motor*/
				is_motor_working = true;
				PWM_0_enable_output_ch0();
			}
			if(motor_time_of_work_counter_s >= MOTOR_TIME_OF_WORK_S) {
				is_motor_working = false;
				motor_time_of_work_counter_s = 0;
				PWM_0_load_duty_cycle_ch0(0x00); //Reset PWM
				PWM_0_disable_output_ch0();
				PWM_OUT_set_level(false);
			}
		}
	}
}

static void LED_SetIndication(const uint8_t num_of_leds_to_turn_on) {
	static uint8_t turned_on_leds = 5; //set 5 at first to pass return condition after launch
	
	if(turned_on_leds == num_of_leds_to_turn_on) {
		return;
	}
	else {
		turned_on_leds = num_of_leds_to_turn_on;
		LED_1_set_level(true);
		LED_2_set_level(true);
		LED_3_set_level(true);
		LED_4_set_level(true);
		LED_5_set_level(true);
	}
	
	switch(num_of_leds_to_turn_on) {
		case 0:
			LED_1_set_level(false);
			break;
		case 1:
			LED_1_set_level(false);
			LED_2_set_level(false);
			break;
		case 2:
			LED_1_set_level(false);
			LED_2_set_level(false);
			LED_3_set_level(false);
			break;
		case 3:
			LED_1_set_level(false);
			LED_2_set_level(false);
			LED_3_set_level(false);
			LED_4_set_level(false);
			break;
		case 4:
			LED_1_set_level(false);
			LED_2_set_level(false);
			LED_3_set_level(false);
			LED_4_set_level(false);
			LED_5_set_level(false);
			break;
		default:
			break;
	}
}

static eLEVLES MOTOR_GetLevel(uint16_t time_ctrl_val) {
	/*0 is about 330, because there is potentiometer 10K and 4K7 resistor in series so 1024*(4K7/14K7) = ~330*/
	if(time_ctrl_val < 520) {
		return LEVEL_1;
	}
	else if(time_ctrl_val < 650) {
		return LEVEL_2;
	}
	else if(time_ctrl_val < 760) {
		return LEVEL_3;
	}
	else if(time_ctrl_val < 940) {
		return LEVEL_4;
	}
	else {
		return LEVEL_5;
	}
}

static uint16_t	MOTOR_GetTimeForThreshold(uint8_t threshiold) {
	uint16_t time = 0;
	for(int i = 0; i < threshiold + 1; i++) {
		time += MOTOR_ThresholdsDuration_s[current_level][i];
	}
	return time;
}

static void MOTOR_SoftStart(void) {
	if(motor_time_of_work_counter_s < MOTOR_GetTimeForThreshold(0)) {
		PWM_0_load_duty_cycle_ch0(MOTOR_PwmThresholds[0]);
		LED_SetIndication(0);
	}
	else if (motor_time_of_work_counter_s < MOTOR_GetTimeForThreshold(1)) {
		PWM_0_load_duty_cycle_ch0(MOTOR_PwmThresholds[1]);
		LED_SetIndication(1);
	}
	else if (motor_time_of_work_counter_s < MOTOR_GetTimeForThreshold(2)) {
		PWM_0_load_duty_cycle_ch0(MOTOR_PwmThresholds[2]);
		LED_SetIndication(2);
	}
	else if (motor_time_of_work_counter_s < MOTOR_GetTimeForThreshold(3)) {
		PWM_0_load_duty_cycle_ch0(MOTOR_PwmThresholds[3]);
		LED_SetIndication(3);
	}
	else {
		PWM_0_load_duty_cycle_ch0(MOTOR_PwmThresholds[4]);
		LED_SetIndication(4);
	}
}