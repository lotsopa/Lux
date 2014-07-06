#ifndef LUX_TIMER_H
#define LUX_TIMER_H

namespace Lux
{
	class Timer
	{
	public:
		Timer();
		~Timer();

		void Reset();
		const bool ElapsedSeconds(double a_Seconds);
		const bool ElapsedMilliseconds(unsigned long a_Ms);
		const bool ElapsedMicroseconds(unsigned long long a_Ms);

		const double GetAsSeconds();
		const double GetAsMilliseconds();
		const double GetAsMicroseconds();

	private:
		long long m_StartTime;
		double m_TimerFrequency;
	};
}

#endif