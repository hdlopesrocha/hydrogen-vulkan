#include "math.hpp"

bool Math::isBetween(float x, float min, float max) {
	return min < x && x < max;
}

int Math::clamp(int val, int min, int max) {
	return val < min ? min : val > max ? max : val;
}

float Math::clamp(float val, float min, float max) {
	return val < min ? min : val > max ? max : val;
}