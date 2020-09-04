#pragma once

inline float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}

inline int clamp(int a, int min, int max)
{
	return (a < min) ? min : ((a > max) ? max : a);
}