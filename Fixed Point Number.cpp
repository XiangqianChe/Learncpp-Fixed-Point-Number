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
	FixedPoint2 operator-() const
	{
		return { static_cast<std::int16_t>(-m_integer), static_cast<std::int8_t>(-m_decimal) };
	}
	explicit operator double() const
	{
		return (m_integer + m_decimal / 100.00);
	}
	friend bool operator==(const FixedPoint2& fp1, const FixedPoint2& fp2)
	{
		return (fp1.m_integer == fp2.m_integer) && (fp1.m_decimal == fp2.m_decimal);
	}
private:
	std::int16_t m_integer{};
	std::int8_t m_decimal{};
};
FixedPoint2 operator+(const FixedPoint2& fp1, const FixedPoint2& fp2)
{
	return { static_cast<double>(fp1) + static_cast<double>(fp2) };
}
std::ostream& operator<<(std::ostream& out, const FixedPoint2& fp)
{
	out << static_cast<double>(fp);
	return out;
}
std::istream& operator>>(std::istream& in, FixedPoint2& fp)
{
	double d{};
	in >> d;
	fp = FixedPoint2{ d };
	return in;
}
int main()
{
	assert(FixedPoint2{ 0.75 } == FixedPoint2{ 0.75 });    // Test equality true
	assert(!(FixedPoint2{ 0.75 } == FixedPoint2{ 0.76 })); // Test equality false

	// Test additional cases -- h/t to reader Sharjeel Safdar for these test cases
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ 1.23 } == FixedPoint2{ 1.98 });    // both positive, no decimal overflow
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ 1.50 } == FixedPoint2{ 2.25 });    // both positive, with decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ -1.23 } == FixedPoint2{ -1.98 }); // both negative, no decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ -1.50 } == FixedPoint2{ -2.25 }); // both negative, with decimal overflow
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ -1.23 } == FixedPoint2{ -0.48 });  // second negative, no decimal overflow
	assert(FixedPoint2{ 0.75 } + FixedPoint2{ -1.50 } == FixedPoint2{ -0.75 });  // second negative, possible decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ 1.23 } == FixedPoint2{ 0.48 });   // first negative, no decimal overflow
	assert(FixedPoint2{ -0.75 } + FixedPoint2{ 1.50 } == FixedPoint2{ 0.75 });   // first negative, possible decimal overflow

	FixedPoint2 a{ -0.48 };
	assert(static_cast<double>(a) == -0.48);
	assert(static_cast<double>(-a) == 0.48);

	std::cout << "Enter a number: "; // enter 5.678
	std::cin >> a;
	std::cout << "You entered: " << a << '\n';
	assert(static_cast<double>(a) == 5.68);

	return 0;
}