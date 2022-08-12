#include <SDL.h>
class Timer {
 public:
 	Timer();
	void reset_timer(Uint32 timeout);
	void update_timer(int ticks);
	bool get_timer_status();
	Uint32 get_timer_difference();
 private:
	Uint32 timerTicks;
	Uint32 realTicks;
};

Timer::Timer() {
	timerTicks = 0;
	realTicks = 0;
}

void Timer::update_timer(int ticks = 1) {
	realTicks += ticks;
}
void Timer::reset_timer(Uint32 timeout) {
	timerTicks = realTicks + timeout;
}
bool Timer::get_timer_status() {
	return timerTicks < realTicks;
}
Uint32 Timer::get_timer_difference() {
	return timerTicks - realTicks;
}