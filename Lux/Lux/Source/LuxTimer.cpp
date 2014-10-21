#include "LuxPCH.h"
#include "LuxTimer.h"

Lux::Utility::Timer::Timer()
{
	Reset();
}

Lux::Utility::Timer::~Timer()
{

}

void Lux::Utility::Timer::Reset()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		LUX_LOG(logERROR) << "QueryPerformanceFrequency failed.";

	m_TimerFrequency = (double)(li.QuadPart) / 1000.0;

	QueryPerformanceCounter(&li);
	m_StartTime = li.QuadPart;
}

const bool Lux::Utility::Timer::ElapsedSeconds(double a_Seconds)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	long long currTime = li.QuadPart;
	double elapsed = (double)(currTime - m_StartTime) / (m_TimerFrequency * 1000.0);

	if (elapsed >= a_Seconds)
	{
		return true;
	}

	return false;
}

const bool Lux::Utility::Timer::ElapsedMilliseconds(unsigned long a_Ms)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	long long currTime = li.QuadPart;
	double elapsed = (double)(currTime - m_StartTime) / m_TimerFrequency;

	if (elapsed >= a_Ms)
	{
		return true;
	}

	return false;
}

const bool Lux::Utility::Timer::ElapsedMicroseconds(unsigned long long a_Ms)
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	long long currTime = li.QuadPart;
	double elapsed = (double)(currTime - m_StartTime) / (m_TimerFrequency / 1000.0);

	if (elapsed >= a_Ms)
	{
		return true;
	}

	return false;
}

const double Lux::Utility::Timer::GetAsSeconds()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	long long currTime = li.QuadPart;
	double elapsed = (double)(currTime - m_StartTime) / (m_TimerFrequency * 1000.0);
	return elapsed;
}

const double Lux::Utility::Timer::GetAsMilliseconds()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	long long currTime = li.QuadPart;
	double elapsed = (double)(currTime - m_StartTime) / m_TimerFrequency;
	return elapsed;
}

const double Lux::Utility::Timer::GetAsMicroseconds()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	long long currTime = li.QuadPart;
	double elapsed = (double)(currTime - m_StartTime) / (m_TimerFrequency / 1000.0);
	return elapsed;
}
