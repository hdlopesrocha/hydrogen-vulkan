#include "math.hpp"

bool Math::isBetween(float x, float min, float max) {
	return min < x && x < max;
}

