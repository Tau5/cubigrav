#include <SDL.h>
class Timer {
 public:
 	Timer();
	void reset_timer(Uint32 timeout);
	bool get_timer_status();
	Uint32 get_timer_difference();
 private:
	Uint32 timerTicks;
};

Timer::Timer() {
	timerTicks = 0;
}
void Timer::reset_timer(Uint32 timeout) {
	timerTicks = SDL_GetTicks() + timeout;
}
bool Timer::get_timer_status() {
	return SDL_TICKS_PASSED(SDL_GetTicks(), timerTicks);
}
Uint32 Timer::get_timer_difference() {
	return timerTicks - SDL_GetTicks();
}