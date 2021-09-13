#include "Math.h"
#include <cmath>

float Math::ToRadians(const float& degrees)
{
	return fmod(degrees, 360) * RadianQuotient;
}
