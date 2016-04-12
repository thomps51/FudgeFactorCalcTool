#ifndef PROGRESS_BAR_H
#define PROGRESS_BAR_H
#include <chrono>
#include <iostream>
class progress_bar
{
public:
  
  progress_bar(uint64_t ticks)
    : _total_ticks(ticks), _ticks_occurred(0),
      _begin(std::chrono::steady_clock::now()),
      barWidth(55),progress(0.0),progressTicks(1.0/ticks)
    {
      int pos = barWidth * progress;
      std::cout << "[";
      for (int i = 0; i < barWidth; ++i) {
        if (i < pos) std::cout << "=";
        else if (i == pos) std::cout << ">";
        else std::cout << " ";
      }
      std::cout << "] " << int(progress * 100.0) << " %\r";
      std::cout.flush(); 
    }
  void tick()
  {
    using namespace std::chrono;
    // test to see if enough progress has elapsed
    //  to warrant updating the progress bar
    //  that way we aren't wasting resources printing
    //  something that hasn't changed
    if (true)
    {
        progress+=progressTicks;
        _ticks_occurred+=1;
        duration time_taken = Clock::now() - _begin;
        float percent_done = (float)_ticks_occurred/_total_ticks;
        duration time_left = time_taken * (1.0/percent_done - 1.0);

        minutes minutes_left = duration_cast<minutes>(time_left);
        seconds seconds_left = duration_cast<seconds>(time_left - minutes_left);
        int pos = barWidth * progress;
        std::cout << "[";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        long minLeft=minutes_left.count();
        long secLeft=seconds_left.count();
        if (minLeft < 0) minLeft=0;
        if (secLeft < 0) secLeft=0;
        std::cout << "] " << int(progress * 100.0) << " \% : " << minLeft << "m" << secLeft <<"s" <<  " left  \r";
        std::cout.flush();  
    }
  }
  void end()
  {
        using namespace std::chrono;
        duration time_taken = Clock::now() - _begin;
        minutes minutes_taken = duration_cast<minutes>(time_taken);
        seconds seconds_taken = duration_cast<seconds>(time_taken - minutes_taken);
        long minTaken=minutes_taken.count();
        long secTaken=seconds_taken.count();
        int pos = barWidth * 100;
        std::cout << "[";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "=";
            else if (i == pos) std::cout << ">";
            else std::cout << " ";
        }
        std::cout << "] " << int(100.0) << " \% : "<< minTaken <<"m"<<secTaken<<"s taken" << std::endl;
  }
private:
  typedef std::chrono::steady_clock Clock;
  typedef Clock::time_point time_point;
  typedef Clock::period period;
  typedef std::chrono::duration<float, period> duration;
  std::uint64_t _total_ticks;
  std::uint64_t _ticks_occurred;
  time_point _begin;
  int barWidth;
  float progress;
  float progressTicks;
};
#endif
