#ifndef _EXPERIMENT_REALTIME_MINIG_H_
#define _EXPERIMENT_REALTIME_MINIG_H_

#include "mbed.h"

#include "minig.h"

class RealTimeMiniG : public MiniG {
public:
  RealTimeMiniG();
  void run();
};

#endif // _EXPERIMENT_REALTIME_MINIG_H_
