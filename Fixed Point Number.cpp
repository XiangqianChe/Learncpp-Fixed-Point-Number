#include <cassert>
#include <iostream>

class FixedPoint2
{
public:
	FixedPoint2(std::int16_t integer = 0, std::int8_t decimal = 0)
		:m_integer{ integer }, m_decimal{ decimal }
	{
		if (m_integer < 0 && m_decimal > 0) m_decimal = -m_decimal;
		if (m_integer > 0 && m_decimal < 0) m_integer = -m_integer;
	}
	explicit operator double() const
	{
		return (m_integer + m_decimal / 100.00);
	}
	friend std::ostream& operator<<(std::ostream& out, const FixedPoint2& fp2)
	{
		out << static_cast<double>(fp2);
		return out;
	}
private:
	std::int16_t m_integer{};
	std::int8_t m_decimal{};
};

int main()
{
	FixedPoint2 a{ 34, 56 };
	std::cout << a << '\n';
	std::cout << static_cast<double>(a) << '\n';
	assert(static_cast<double>(a) == 34.56);

	FixedPoint2 b{ -2, 8 };
	assert(static_cast<double>(b) == -2.08);

	FixedPoint2 c{ 2, -8 };
	assert(static_cast<double>(c) == -2.08);

	FixedPoint2 d{ -2, -8 };
	assert(static_cast<double>(d) == -2.08);

	FixedPoint2 e{ 0, -5 };
	assert(static_cast<double>(e) == -0.05);

	FixedPoint2 f{ 0, 10 };
	assert(static_cast<double>(f) == 0.1);

	return 0;
}