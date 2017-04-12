/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#include "Time.hpp"

namespace unitTest
{
	Time::Time() :
		m_microseconds( 0 )
	{}

	Time::Time( const int64_t & microseconds ) :
		m_microseconds( microseconds )
	{}

	Time::~Time()
	{}


	float Time::AsSeconds() const
	{
		return m_microseconds / 1000000.f;
	}

	int32_t Time::AsMilliseconds() const
	{
		return static_cast<int32_t>( m_microseconds / 1000 );
	}

	int64_t Time::AsMicroseconds() const
	{
		return m_microseconds;
	}

	Time & Time::operator=( const Time & other )
	{
		this->m_microseconds = other.m_microseconds;

		return *this;
	}

	Time Seconds( float val )
	{
		return Time( static_cast<int64_t>( val * 1000000 ) );
	}

	Time Milliseconds( const int32_t & val )
	{
		return Time( static_cast<int64_t>( val ) * 1000 );
	}

	Time Microseconds( const int64_t & val )
	{
		return Time( val );
	}

	void Sleep( const Time & time )
	{
		std::this_thread::sleep_for( std::chrono::microseconds( time.m_microseconds ) );
	}

	Time & Time::operator+=( Time & other )
	{
		this->m_microseconds += other.m_microseconds;
		return *this;
	}

	Time & Time::operator-=( Time & other )
	{
		this->m_microseconds -= other.m_microseconds;

		return *this;
	}

	Time & Time::operator*=( Time & other )
	{
		this->m_microseconds *= other.m_microseconds;

		return *this;
	}

	Time & Time::operator/=( Time & other )
	{
		this->m_microseconds /= other.m_microseconds;
		return *this;
	}

	bool operator==( Time & left, Time & right )
	{
		return left.m_microseconds == right.m_microseconds;
	}
	bool operator!=( Time & left, Time & right )
	{
		return !( left == right );
	}
	bool operator<( Time & left, Time & right )
	{
		return left.m_microseconds < right.m_microseconds;
	}
	bool operator>( Time & left, Time & right )
	{
		return left.m_microseconds > right.m_microseconds;
	}

	Time & operator+( Time left, Time & right )
	{
		left += right;
		return left;
	}
	Time & operator-( Time left, Time & right )
	{
		left -= right;
		return left;
	}
	Time & operator*( Time left, Time & right )
	{
		left *= right;
		return left;
	}
	Time & operator/( Time left, Time & right )
	{
		left /= right;
		return left;
	}
}
