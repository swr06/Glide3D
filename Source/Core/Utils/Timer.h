#pragma once

#include <chrono>
#include <string>
#include <sstream>
#include "../Application/Logger.h"

namespace Glide3D
{
	enum class TimerDurationCast
	{
		Nanoseconds = 0,
		Microseconds,
		Milliseconds,
		Seconds
	};


	class Timer
	{
	public : 

		Timer() {}

		void Start(TimerDurationCast cast = TimerDurationCast::Milliseconds)
		{
			 m_StartTime = std::chrono::high_resolution_clock::now();
			 m_TimerStarted = true;
			 m_Cast = cast;
		}

		void End(const std::string& output_text)
		{
			if (!m_TimerStarted) { throw "Glide3D::Timer()::End() called without the timer being started!"; return; }

			m_EndTime = std::chrono::high_resolution_clock::now();
			
			long long Total_Time = 0;

			switch (m_Cast)
			{
				case TimerDurationCast::Nanoseconds :
				{
					Total_Time = std::chrono::duration_cast<std::chrono::nanoseconds>(m_EndTime - m_StartTime).count();
					break;
				}

				case TimerDurationCast::Microseconds:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::microseconds>(m_EndTime - m_StartTime).count();
					break;
				}

				case TimerDurationCast::Milliseconds:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count();
					break;
				}

				case TimerDurationCast::Seconds:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::seconds>(m_EndTime - m_StartTime).count();
					break;
				}

				default : 
				{
					Total_Time = std::chrono::duration_cast<std::chrono::seconds>(m_EndTime - m_StartTime).count();
					break;
				}
			}

			
			Logger::Log(output_text + std::to_string(Total_Time));
		}

		long long End()
		{
			if (!m_TimerStarted) { throw "Glide3D::Timer()::End() called without the timer being started!"; return 0; }

			m_EndTime = std::chrono::high_resolution_clock::now();

			long long Total_Time = 0;

			switch (m_Cast)
			{
				case TimerDurationCast::Nanoseconds:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::nanoseconds>(m_EndTime - m_StartTime).count();
					break;
				}

				case TimerDurationCast::Microseconds:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::microseconds>(m_EndTime - m_StartTime).count();
					break;
				}

				case TimerDurationCast::Milliseconds:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::milliseconds>(m_EndTime - m_StartTime).count();
					break;
				}

				case TimerDurationCast::Seconds:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::seconds>(m_EndTime - m_StartTime).count();
					break;
				}

				default:
				{
					Total_Time = std::chrono::duration_cast<std::chrono::seconds>(m_EndTime - m_StartTime).count();
					break;
				}
			}

			return Total_Time;
		}

		~Timer() {}

	private :

		std::chrono::steady_clock::time_point m_StartTime;
		std::chrono::steady_clock::time_point m_EndTime;
		TimerDurationCast m_Cast = TimerDurationCast::Milliseconds;
		bool m_TimerStarted = false;
	};
}