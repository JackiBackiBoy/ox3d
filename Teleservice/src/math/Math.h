#ifndef MATH_HEADER
#define MATH_HEADER

class Math
{
public:
	Math(const Math&) = delete;

	static float ToRadians(const float& degrees);
	static constexpr double Pi = 3.14159265358979323846;

private:
	Math() {};
	static Math* m_Instance;
	static constexpr double RadianQuotient = 3.14159265358979323846 / 180;
};
#endif