/*  $Id: RSprite.cpp,v 1.5 2010/07/31 05:29:45 sarrazip Exp $
    RSprite.cpp - Sprite with floating point coordinates in a 2D game.

    flatzebra - Generic 2D Game Engine library
    Copyright (C) 1999-2003 Pierre Sarrazin <http://sarrazip.com/>

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

#include <flatzebra/RSprite.h>

#include <assert.h>

using namespace std;
using namespace flatzebra;


///////////////////////////////////////////////////////////////////////////////


template <class T>
inline T &
boundVariable(T &var, T lowerLimit, T upperLimit)
{
    if (var < lowerLimit)
        var = lowerLimit;
    else if (var > upperLimit)
        var = upperLimit;
    return var;
}


///////////////////////////////////////////////////////////////////////////////


RSprite::RSprite(const PixmapArray &pixmapArray,
                const RCouple &_pos,
                const RCouple &_speed,
                const RCouple &_accel,
                const RCouple &_collBoxPos,
                const RCouple &_collBoxSize)
  : currentPixmapIndex(0),
    values(NULL),
    numUserValues(0),
    thePixmapArray(&pixmapArray),
    pos(_pos),
    speed(_speed),
    accel(_accel),
    size(pixmapArray.getImageSize()),
    timeToLive(0),
    collBoxPos(_collBoxPos),
    collBoxSize(_collBoxSize)
{
}


RSprite::~RSprite()
{
}


bool RSprite::collidesWithRSprite(const RSprite &s) const
{
    const RSprite &s1 = *this;
    const RSprite &s2 = s;
    const RCouple &pos1  = s1.getPos() + s1.collBoxPos;
    const RCouple &size1 = s1.collBoxSize;
    const RCouple &pos2  = s2.getPos() + s2.collBoxPos;
    const RCouple &size2 = s2.collBoxSize;

    return RCouple::rectangleCollision(pos1, size1, pos2, size2);
}


void RSprite::boundPosition(Couple settingSizeInPixels)
/*  If the position of sprite 's' is out of the setting, then this
    position is adjusted to bring the sprite back in.
*/
{
    int x = int(pos.x);
    int y = int(pos.y);
    ::boundVariable(x, 0, settingSizeInPixels.x - size.x);
    ::boundVariable(y, 0, settingSizeInPixels.y - size.y);
    pos = RCouple(x, y);

    // Assert that the sprite is still in the setting:
    assert(pos.x >= 0);
    assert(pos.x + size.x <= settingSizeInPixels.x);
    assert(pos.y >= 0);
    assert(pos.y + size.y <= settingSizeInPixels.y);
}
