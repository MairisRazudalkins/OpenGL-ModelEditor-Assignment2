#pragma once

#ifndef Utils;
#define Utils

#define M_PI 3.14159265358979323846

namespace Utils
{
	float Clamp(float value, float min, float max)
	{
		if (value < min) value = min;
		else if (value > max) value = max;
		return value;
	}

	float Clamp(int value, int min, int max)
	{
		if (value < min) value = min;
		else if (value > max) value = max;
		return value;
	}

	float Radians(float angle)
	{
		return float(M_PI * (angle - 90.0f) / 180.0f);
	}
}

#endif