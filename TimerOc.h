#ifndef TIMER_OC_H
#define TIMER_OC_H

#include "platform/platform.h"

#include "platform/mbed_critical.h"
#include "platform/mbed_power_mgmt.h"
#include "timer_oc_api.h"

class TimerOc {

public:
  /** Create a TimerOc connected to the specified pin
   *
   *  @param pin TimerOc pin to connect to
   */
  TimerOc(PinName pin) : _deep_sleep_locked(false) {
    core_util_critical_section_enter();
    timer_oc_init(&_timer_oc, pin);
    core_util_critical_section_exit();
  }

  ~TimerOc() {
    core_util_critical_section_enter();
    unlock_deep_sleep();
    core_util_critical_section_exit();
  }

  /** Set the ouput duty-cycle, specified as a percentage (float)
   *
   *  @param value A floating-point value representing the output duty-cycle,
   *    specified as a percentage. The value should lie between
   *    0.0f (representing on 0%) and 1.0f (representing on 100%).
   *    Values outside this range will be saturated to 0.0f or 1.0f.
   */
  void start(int period_us, int num_repetitions, uint32_t *ticks) {
    core_util_critical_section_enter();
    lock_deep_sleep();
    timer_oc_start(&_timer_oc, period_us, num_repetitions, ticks);
    core_util_critical_section_exit();
  }

  void stop() {
    printf("before cs\n");
    core_util_critical_section_enter();
    lock_deep_sleep();
    timer_oc_stop(&_timer_oc);
    core_util_critical_section_exit();
  }

protected:
  /** Lock deep sleep only if it is not yet locked */
  void lock_deep_sleep() {
    if (_deep_sleep_locked == false) {
      sleep_manager_lock_deep_sleep();
      _deep_sleep_locked = true;
    }
  }

  /** Unlock deep sleep in case it is locked */
  void unlock_deep_sleep() {
    if (_deep_sleep_locked == true) {
      sleep_manager_unlock_deep_sleep();
      _deep_sleep_locked = false;
    }
  }

  timer_oc_t _timer_oc;
  bool _deep_sleep_locked;
};

#endif
