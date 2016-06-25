/*  $Id: RCouple.h,v 1.6 2012/07/06 04:28:45 sarrazip Exp $
    RCouple.h - Class representing a couple of integers.

    flatzebra - Generic 2D Game Engine library
    Copyright (C) 1999-2012 Pierre Sarrazin <http://sarrazip.com/>

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
    02110-1301, USA.
*/

#ifndef _H_RCouple
#define _H_RCouple

#include <flatzebra/Couple.h>

#include <assert.h>
#include <math.h>


namespace flatzebra {


class RCouple
/*  Class representing a RCouple of floating point numbers.
*/
{
public:
    double x, y;
    /*  The components.
    */

    RCouple();
    /*  Builds a RCouple with ZERO values for the components.
    */

    RCouple(double a, double b);
    /*  Initiliazes a RCouple with x equal to a and y equal to b.
    */

    RCouple(const RCouple &c);
    /*  Copies the components of the RCouple 'c' into the current RCouple.
    */

    RCouple(const Couple &c);
    /*  Converts the integer couple 'c' into the current floating
        point RCouple.
    */

    RCouple &operator = (const RCouple &c);
    /*  Replaces the components of the current RCouple by those of 'c'.
    */

    RCouple &operator = (const Couple &c);
    /*  Replaces the components of the current RCouple by a conversion
        of the integer couple 'c' into a floating point couple.
    */

    ~RCouple();
    /*  Does nothing.
    */

    RCouple &zero();
    /*  Assign zero to both components.
        Returns a reference to this object.
        Was void until version 0.1.
    */

    bool isZero() const;
    /*  Returns true iff both components are zero.
    */

    bool isNonZero() const;
    /*  Returns true iff one or both components differ from zero.
    */

    double squaredLength() const;
    /*  Returns the square of the length of the vector represented by the RCouple.
    */

    double length() const;
    /*  Returns the length of the vector represented by the RCouple.
    */

    RCouple normalize() const;
    /*  Returns a vector with the same direction as this one, but of length 1.
        If this vector is the origin, the origin is returned.
    */

    RCouple setLength(double newLength) const;
    /*  Returns a vector with the same direction as this one, but of the
        specified length.
        If this vector is the origin, the origin is returned.
    */

    Couple round() const;
    /*  Returns an integer couple created from the rounded values of
        this floating point couple.
    */

    Couple floor() const;
    /*  Returns an integer couple created from the floored values of
        this floating point couple.
    */

    Couple ceil() const;
    /*  Returns an integer couple created from the ceiled values of
        this floating point couple.
    */

    RCouple &operator += (const RCouple &c);
    RCouple &operator -= (const RCouple &c);
    RCouple &operator *= (double n);
    RCouple &operator /= (double n);
    /*  Return the current object as modified.
    */

    friend RCouple operator + (const RCouple &c1, const RCouple &c2);
    friend RCouple operator - (const RCouple &c1, const RCouple &c2);
    friend RCouple operator * (const RCouple &c1, double n);
    friend RCouple operator * (double n, const RCouple &c1);
    friend RCouple operator / (const RCouple &c1, double n);
    friend bool operator == (const RCouple &c1, const RCouple &c2);
    friend bool operator != (const RCouple &c1, const RCouple &c2);

    static bool rectangleCollision(const RCouple &pos1,
                                      const RCouple &rect1,
                                      const RCouple &pos2,
                                      const RCouple &size2);
    /*  Indicates if the two specified rectangles collide.
        They collide if they have an intersection that has a non-zero surface.
        pos1 and pos2 must be the upper-left coordinates of the rectangles.
    */

    double dotProduct(const RCouple &c) const;
    /*  Returns the scalar product of this couple with the given one.
    */

    RCouple project(const RCouple &c) const;
    /*  Returns the projection of vector 'c' on this vector.
        This vector must not be zero. (See isZero() and isNonZero().)
    */

    RCouple getClosestPointOnSegment(const RCouple &segStart,
                                     const RCouple &segEnd) const;
    /*  Returns the point on the given segment that is closest to this point.
        segStart and segEnd give the end points of the segment, and must not
        be equal.
        If the projection of *this on the segment lies "before" segStart,
        then segStart is returned; if the projection lies "after" segEnd,
        then segEnd is returned.
    */

    bool isOnSegment(const RCouple &segStart,
                     const RCouple &segEnd) const;
    /*  Return true if this point lies on the given segment.
        segStart and segEnd give the end points of the segment, and must not
        be equal.
    */

    static const double tolerance;
    /*  Tolerance used in floating point comparisons when determining
        if two values are "extremely" close.
    */

    bool safeEqual(const RCouple &c) const;
    /*  Indicates if this vector is "extremely" close to 'c'.
    */

    static bool safeEqual(double a, double b);
    /*  Indicates if the two numbers are "extremely" close.
    */

    static bool safeLower(double a, double b);
    /*  Indicates if the two numbers are a < b and not "extremely" close.
    */

    static bool safeLowerOrEqual(double a, double b);
    /*  Indicates if a < b or the two numbers are "extremely" close.
    */

    static bool safeGreater(double a, double b);
    /*  Indicates if the two numbers are a > b and not "extremely" close.
    */

    static bool safeGreaterOrEqual(double a, double b);
    /*  Indicates if a > b or the two numbers are "extremely" close.
    */

};


//
//  IMPLEMENTATION (inline functions)
//


inline RCouple::RCouple()
  : x(0), y(0)
{
}


inline RCouple::RCouple(double a, double b)
  : x(a), y(b)
{
}


inline RCouple::RCouple(const RCouple &c)
  : x(c.x), y(c.y)
{
}


inline RCouple::RCouple(const Couple &c)
  : x(c.x), y(c.y)
{
}


inline RCouple &RCouple::operator = (const RCouple &c)
{
    x = c.x;
    y = c.y;
    return *this;
}


inline RCouple &RCouple::operator = (const Couple &c)
{
    x = c.x;
    y = c.y;
    return *this;
}


inline RCouple::~RCouple()
{
}


inline RCouple &RCouple::zero()
{
    x = y = 0;
    return *this;
}


inline bool RCouple::isZero() const
{
    return (x == 0 && y == 0);
}


inline bool RCouple::isNonZero() const
{
    return (x != 0 || y != 0);
}


inline RCouple &RCouple::operator += (const RCouple &c)
{
    x += c.x;
    y += c.y;
    return *this;
}


inline RCouple &RCouple::operator -= (const RCouple &c)
{
    x -= c.x;
    y -= c.y;
    return *this;
}


inline RCouple &RCouple::operator *= (double n)
{
    x *= n;
    y *= n;
    return *this;
}


inline RCouple &RCouple::operator /= (double n)
{
    x /= n;
    y /= n;
    return *this;
}


inline RCouple operator + (const RCouple &c1, const RCouple &c2)
{
    RCouple c(c1);
    return c += c2;
}


inline RCouple operator - (const RCouple &c1, const RCouple &c2)
{
    RCouple c(c1);
    return c -= c2;
}


inline RCouple operator * (const RCouple &c1, double n)
{
    RCouple c(c1);
    return c *= n;
}


inline RCouple operator * (double n, const RCouple &c1)
{
    RCouple c(c1);
    return c *= n;
}


inline RCouple operator / (const RCouple &c1, double n)
{
    RCouple c(c1);
    return c /= n;
}


inline bool operator == (const RCouple &c1, const RCouple &c2)
{
    return (c1.x == c2.x && c1.y == c2.y);
}


inline bool operator != (const RCouple &c1, const RCouple &c2)
{
    return !(c1 == c2);
}


inline double RCouple::squaredLength() const
{
    return x * x + y * y;
}


inline double RCouple::length() const
{
    return hypot(x, y);
}


inline RCouple RCouple::normalize() const
{
    if (isZero())
        return RCouple();
    return *this / length();
}


inline RCouple RCouple::setLength(double newLength) const
{
    if (isZero())
        return RCouple();
    return *this / length() * newLength;
}


inline Couple RCouple::round() const
{
    return Couple(
        int(x >= 0 ? (x + 0.5) : (x - 0.5)),
        int(y >= 0 ? (y + 0.5) : (y - 0.5)));
}


inline Couple RCouple::floor() const
{
    return Couple(static_cast<int>(::floor(x)), static_cast<int>(::floor(y)));
}

inline Couple RCouple::ceil() const
{
    return Couple(static_cast<int>(::ceil(x)), static_cast<int>(::ceil(y)));
}


inline double RCouple::dotProduct(const RCouple &c) const
{
    return x * c.x + y * c.y;
}


inline RCouple RCouple::project(const RCouple &c) const
{
    assert(isNonZero());
    return (dotProduct(c) / dotProduct(*this)) * *this;
}


inline bool RCouple::safeEqual(const RCouple &c) const
{
    return fabs(x - c.x) < tolerance && fabs(y - c.y) < tolerance;
}


inline bool RCouple::safeEqual(double a, double b)
{
    return fabs(a - b) < tolerance;
}


inline bool RCouple::safeLower(double a, double b)
{
    return !safeEqual(a, b) && a < b;
}


inline bool RCouple::safeLowerOrEqual(double a, double b)
{
    return safeEqual(a, b) || a < b;
}


inline bool RCouple::safeGreater(double a, double b)
{
    return !safeEqual(a, b) && a > b;
}


inline bool RCouple::safeGreaterOrEqual(double a, double b)
{
    return safeEqual(a, b) || a > b;
}


}  // namespace flatzebra


#endif  /* _H_RCouple */
