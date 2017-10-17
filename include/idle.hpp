#ifndef __IDLE_H
#define __IDLE_H
#include <time.h>

extern clock_t  MarkedTimeStamp;
extern clock_t  DelayTimeStamp;

void CheckEvents(int);
void EventSleep(int);
void CamIdle(int);

#endif
