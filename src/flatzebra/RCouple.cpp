/*  $Id: RCouple.cpp,v 1.2 2012/07/06 04:28:45 sarrazip Exp $
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

#include <flatzebra/RCouple.h>

using namespace flatzebra;


const double RCouple::tolerance = 0.0001;


bool
RCouple::rectangleCollision(const RCouple &pos1,
                            const RCouple &size1,
                            const RCouple &pos2,
                            const RCouple &size2)
{
    if (pos1.x + size1.x <= pos2.x)  // s1 at the left of s2
        return false;
    if (pos1.y + size1.y <= pos2.y)  // s1 above s2
        return false;
    if (pos2.x + size2.x <= pos1.x)  // s1 at the right of s2
        return false;
    if (pos2.y + size2.y <= pos1.y)  // s1 below s2
        return false;
    return true;
}


RCouple
RCouple::getClosestPointOnSegment(const RCouple &segStart,
                                  const RCouple &segEnd) const
{
    assert(segStart != segEnd);  // this condition avoids division by zero
    RCouple seg = segEnd - segStart;
    RCouple c = *this - segStart;
    double t = seg.dotProduct(c) / seg.dotProduct(seg);  // project pt on segment
    if (t < 0.0)
        t = 0.0;
    else if (t > 1.0)
        t = 1.0;
    return segStart + t * seg;
}


bool
RCouple::isOnSegment(const RCouple &segStart,
                     const RCouple &segEnd) const
{
    assert(segStart != segEnd);  // this condition avoids division by zero
    RCouple seg = segEnd - segStart;
    RCouple c = *this - segStart;
    double t = seg.dotProduct(c) / seg.dotProduct(seg);  // project pt on segment
    if (t < 0.0 || t > 1.0)
        return false;
    RCouple proj = segStart + t * seg;
    return proj == *this;
}
