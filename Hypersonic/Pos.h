#ifndef __POS_H__
#define __POS_H__

#include <cmath>
#include <iostream>

using namespace std;

class Pos
{
public:
	Pos() = default;
	Pos(int x, int y)
		: m_x(x), m_y(y)
	{}

   Pos operator+(const Pos& v2) const
   {
      return Pos(m_x + v2.m_x, m_y + v2.m_y);
   }

   friend Pos& operator+=(Pos &v1, const Pos &v2)
   {
      v1.m_x += v2.m_x;
      v1.m_y += v2.m_y;

      return v1;
   }

   Pos operator*(int scalar)
   {
      return Pos(scalar*m_x, scalar*m_y);
   }

   Pos& operator*=(int scalar)
   {
      m_x *= scalar;
      m_y *= scalar;

      return *this;
   }

   Pos operator-(const Pos& v2) const
   {
      return Pos(m_x - v2.m_x, m_y - v2.m_y);
   }

   friend Pos& operator-=(Pos& v1, const Pos& v2)
   {
      v1.m_x -= v2.m_x;
      v1.m_y -= v2.m_y;

      return v1;
   }

   Pos operator/(int scalar)
   {
      return Pos(m_x / scalar, m_y / scalar);
   }

   Pos& operator/=(int scalar)
   {
      m_x /= scalar;
      m_y /= scalar;

      return *this;
   }

   bool operator==(const Pos& v) const
   {
      return v.m_x == m_x && v.m_y == m_y;
   }

   bool operator!=(const Pos& v) const
   {
      return !(*this == v);
   }

public:
	int m_x{ 0 };
	int m_y{ 0 };
};

ostream& operator<<(ostream& os, const Pos& obj)
{
   os << obj.m_y << " " << obj.m_x;
   return os;
}

#endif
