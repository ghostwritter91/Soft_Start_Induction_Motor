/**
 * \file
 *
 * \brief PWM Basic driver implementation.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/**
 * \defgroup doc_driver_pwm_basic PWM Basic Driver
 * \ingroup doc_driver_pwm
 *
 * \section doc_driver_pwm_normal_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <pwm_basic.h>

/** Function pointer to callback function called by IRQ.
    NULL=default value: No callback function is to be used.
*/
pwm_irq_cb_t PWM_0_cb = NULL;

/**
 * \brief Initialize PWM
 * If module is configured to disabled state, the clock to the PWM is disabled
 * if this is supported by the device's clock system.
 *
 * \return Initialization status.
 */
int8_t PWM_0_init()
{

	/* Enable TC0 */
	PRR &= ~(1 << PRTIM0);

	TCCR0A = //(1 << COM0A1) | (0 << COM0A0)   /* Clear OCA on Compare Match, set OCA on BOTTOM (non-inverting mode) */
	         (0 << COM0B1) | (0 << COM0B0) /* Normal port operation, OCB disconnected */
	         | (1 << WGM01) | (0 << WGM00);  /* TC16 Mode 14 Fast PWM */

	TCCR0B = (1 << WGM03) | (1 << WGM02)                /* TC16 Mode 14 Fast PWM */
	         | 0 << ICNC0                               /* Input Capture Noise Canceler: disabled */
	         | 0 << ICES0                               /* Input Capture Edge Select: disabled */
	         | (1 << CS02) | (0 << CS01) | (1 << CS00); /* IO clock divided by 1024 */

	/* clock = 8MHz, prescaler 1024, 8MHz/1024 = 7812,15Hz (0,000128s), top value is 80 so 80*0,000128s = 0,01024s (~100Hz)*/
	ICR0 = 0x50; /* Counter top value: 0x50 */

	OCR0A = 0x00; /* Output compare A: 0x01 */

	TIMSK0 = 0 << OCIE0B   /* Output Compare B Match Interrupt Enable: disabled */
	         | 0 << OCIE0A /* Output Compare A Match Interrupt Enable: disabled */
	         | 0 << ICIE0  /* Input Capture Interrupt Enable: disabled */
	         | 1 << TOIE0; /* Overflow Interrupt Enable: enabled */
			 
	/* Use alternative pins for PWM output - PA3 */
	GTCCR |= (0x01 << 1);

	return 0;
}

/**
 * \brief Enable PWM_0
 * 1. If supported by the clock system, enables the clock to the PWM
 * 2. Enables the PWM module by setting the enable-bit in the PWM control register
 *
 * \return Nothing
 */
void PWM_0_enable()
{
	/* Enable TC0 */
	PRR &= ~(1 << PRTIM0);
}

/**
 * \brief Disable PWM_0
 * 1. Disables the PWM module by clearing the enable-bit in the PWM control register
 * 2. If supported by the clock system, disables the clock to the PWM
 *
 * \return Nothing
 */
void PWM_0_disable()
{
	/* Disable TC0 */
	PRR |= (1 << PRTIM0);
}

/**
 * \brief Enable PWM output on channel 0
 *
 * \return Nothing
 */
void PWM_0_enable_output_ch0()
{

	TCCR0A |= ((1 << COM0A1) | (0 << COM0A0));
}

/**
 * \brief Disable PWM output on channel 0
 *
 * \return Nothing
 */
void PWM_0_disable_output_ch0()
{

	TCCR0A &= ~((0 << COM0A1) | (0 << COM0A0));
}

/**
 * \brief Enable PWM output on channel 1
 *
 * \return Nothing
 */
void PWM_0_enable_output_ch1()
{

	TCCR0A |= ((0 << COM0B1) | (0 << COM0B0));
}

/**
 * \brief Disable PWM output on channel 1
 *
 * \return Nothing
 */
void PWM_0_disable_output_ch1()
{

	TCCR0A &= ~((0 << COM0B1) | (0 << COM0B0));
}

/**
 * \brief Load COUNTER register in PWM_0
 *
 * \param[in] counter_value The value to load into COUNTER
 *
 * \return Nothing
 */
void PWM_0_load_counter(PWM_0_register_t counter_value)
{
	TCNT0 = counter_value;
}

/**
 * \brief Load TOP register in PWM_0.
 * The physical register may different names, depending on the hardware and module mode.
 *
 * \param[in] counter_value The value to load into TOP.
 *
 * \return Nothing
 */
void PWM_0_load_top(PWM_0_register_t top_value)
{
	ICR0 = top_value;
}

/**
 * \brief Load duty cycle register in for channel 0.
 * The physical register may have different names, depending on the hardware.
 * This is not the duty cycle as percentage of the whole period, but the actual
 * counter compare value.
 *
 * \param[in] counter_value The value to load into the duty cycle register.
 *
 * \return Nothing
 */
void PWM_0_load_duty_cycle_ch0(PWM_0_register_t duty_value)
{
	OCR0A = duty_value;
}

/**
 * \brief Load duty cycle register in for channel 1.
 * The physical register may have different names, depending on the hardware.
 * This is not the duty cycle as percentage of the whole period, but the actual
 * counter compare value.
 *
 * \param[in] counter_value The value to load into the duty cycle register.
 *
 * \return Nothing
 */
void PWM_0_load_duty_cycle_ch1(PWM_0_register_t duty_value)
{
	OCR0B = duty_value;
}

/**
 * \brief Register a callback function to be called at the end of the overflow ISR.
 *
 * \param[in] f Pointer to function to be called
 *
 * \return Nothing.
 */
void PWM_0_register_callback(pwm_irq_cb_t f)
{
	PWM_0_cb = f;
}

ISR(TIM0_OVF_vect)
{
	static volatile uint8_t callback_count = 0;

	// Clear the interrupt flag
	TIFR0 = TOV0;

	/* callback function - interrupt every ~100Hz (10ms); 10ms * PWM_0_INTERRUPT_CB_RATE(100) = 1s */
	if ((++callback_count >= PWM_0_INTERRUPT_CB_RATE) && (PWM_0_INTERRUPT_CB_RATE != 0)) {
		if (PWM_0_cb != NULL) {
			PWM_0_cb();
		}
		callback_count = 0;
	}
}
