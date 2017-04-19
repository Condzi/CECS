/*
	Conrad 'Condzi' Kubacki 2017
	https://github.com/condzi
*/

#pragma once

namespace ex
{
	struct PositionComponent final
	{
		float x, y;

		PositionComponent(float xx = 0.0f, float yy = 0.0f) : 
			x(xx), y(yy)
		{}
	};
}