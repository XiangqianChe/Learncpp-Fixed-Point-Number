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
		m_integer += (m_decimal / 100);
		m_decimal %= 100;
	}
	FixedPoint2(double d)
		:FixedPoint2(
			static_cast<std::int16_t>(std::trunc(d)),
			static_cast<std::int8_t>(std::round(d * 100) - std::trunc(d) * 100)
		)
	{}
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
	FixedPoint2 a{ 0.01 };
	assert(static_cast<double>(a) == 0.01);

	FixedPoint2 b{ -0.01 };
	assert(static_cast<double>(b) == -0.01);

	FixedPoint2 c{ 1.9 }; // make sure we handle single digit decimal
	assert(static_cast<double>(c) == 1.9);

	FixedPoint2 d{ 5.01 }; // stored as 5.0099999... so we'll need to round this
	assert(static_cast<double>(d) == 5.01);

	FixedPoint2 e{ -5.01 }; // stored as -5.0099999... so we'll need to round this
	assert(static_cast<double>(e) == -5.01);

	// Handle case where the argument's decimal rounds to 100 (need to increase base by 1)
	FixedPoint2 f{ 106.9978 }; // should be stored with base 107 and decimal 0
	assert(static_cast<double>(f) == 107.0);

	// Handle case where the argument's decimal rounds to -100 (need to decrease base by 1)
	FixedPoint2 g{ -106.9978 }; // should be stored with base -107 and decimal 0
	assert(static_cast<double>(g) == -107.0);

	return 0;
}