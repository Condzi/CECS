/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include "Clock.hpp"

namespace unitTest
{
	Clock::Clock()
	{
		m_startTime = std::chrono::steady_clock::now();
	}

	Clock::~Clock()
	{
	}


	Time Clock::GetEleapsedTime()
	{
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::steady_clock::now();
		return Time( std::chrono::duration_cast<std::chrono::microseconds>( endTime - m_startTime ).count() );
	}

	Time Clock::Restart()
	{
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::steady_clock::now();
		Time eleapsed = std::chrono::duration_cast<std::chrono::microseconds>( endTime - m_startTime ).count();

		m_startTime = std::chrono::steady_clock::now();

		return eleapsed;
	}
}
