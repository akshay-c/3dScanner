#include <time.h>
#include <gtk/gtk.h>
#include "idle.hpp"

clock_t MarkedTimeStamp;
clock_t DelayTimeStamp;

void CheckEvents(int FrameRate)
{
    do
    {
        while(gtk_events_pending())
        {
            gtk_main_iteration ();
        }
    }while((clock() - MarkedTimeStamp) < (CLOCKS_PER_SEC/FrameRate));
    // g_print("Time:%ld",CLOCKS_PER_SEC);
    MarkedTimeStamp = clock();
}

void EventSleep(int delay)
{
    DelayTimeStamp = clock();
    do
    {
        while(gtk_events_pending())
        {
            gtk_main_iteration ();
        }
    }while((clock() - DelayTimeStamp) < (CLOCKS_PER_SEC*delay/5));
    // g_print("Time:%ld",CLOCKS_PER_SEC);
    DelayTimeStamp = clock();
}