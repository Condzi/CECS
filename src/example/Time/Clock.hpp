/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/
#pragma once

#include <chrono>

#include "Time.hpp"

namespace unitTest
{
	class Clock final
	{
	public:
		Clock();
		~Clock();

		/*
			Returns time in microseconds
		*/
		Time GetEleapsedTime();
		/*
			Restarts clock and returns eleapsed time in microseconds
		*/
		Time Restart();

	private:
		std::chrono::high_resolution_clock::time_point m_startTime;
	};
}


