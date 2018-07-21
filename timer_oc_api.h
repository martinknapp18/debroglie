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

typedef struct timer_oc_s timer_oc_t;

void timer_oc_init(timer_oc_t *obj, PinName pin);
void timer_oc_free(timer_oc_t *obj);
void timer_oc_start(timer_oc_t *obj, int us_period, int num_repetitions, uint32_t * ticks);
void timer_oc_stop(timer_oc_t *obj);

#ifdef __cplusplus
}
#endif

#endif

