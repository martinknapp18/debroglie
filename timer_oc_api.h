#ifndef TIMER_OC_API_H
#define TIMER_OC_API_H

#include "device.h"

#ifdef __cplusplus
extern "C" {
#endif

struct timer_oc_s {
    PWMName pwm;
    PinName pin;
    uint32_t prescaler;
    uint32_t period;
    uint32_t pulse;
    uint8_t channel;
    uint8_t inverted;
};


/** TimerOc hal structure. timer_oc_s is declared in the target's hal
 */
typedef struct timer_oc_s timer_oc_t;

/** Initialize the pwm out peripheral and configure the pin
 *
 * @param obj The timer_oc object to initialize
 * @param pin The timer_oc pin to initialize
 */
void timer_oc_init(timer_oc_t *obj, PinName pin);

/** Deinitialize the timer_oc object
 *
 * @param obj The timer_oc object
 */
void timer_oc_free(timer_oc_t *obj);

/** Set the output duty-cycle in range <0.0f, 1.0f>
 *
 * Value 0.0f represents 0 percentage, 1.0f represents 100 percent.
 * @param obj     The timer_oc object
 * @param percent The floating-point percentage number
 */
void timer_oc_start(timer_oc_t *obj, int us_period, int num_repetitions, uint32_t * ticks);
void timer_oc_stop(timer_oc_t *obj);

#ifdef __cplusplus
}
#endif

#endif

