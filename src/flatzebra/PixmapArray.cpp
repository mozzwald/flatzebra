/*  $Id: PixmapArray.cpp,v 1.4 2012/03/28 02:55:45 sarrazip Exp $
    PixmapArray.cpp - Object containing an array of Pixmaps.

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

#include <flatzebra/PixmapArray.h>

#include <assert.h>

using namespace std;
using namespace flatzebra;


PixmapArray::PixmapArray(size_t)
  : images(),
    imageSize(0, 0)
{
}


PixmapArray::~PixmapArray()
{
    freeImages();
}


void
PixmapArray::freeImages()
{
    for (vector<SDL_Surface *>::iterator it = images.begin();
                                        it != images.end(); it++)
        SDL_FreeSurface(*it);

    images.clear();
}


void
PixmapArray::setArrayElement(size_t i, SDL_Surface *image)
{
    assert(i < 10000);  // sanity check
    assert(image != NULL);

    if (i >= images.size())
        images.resize(i + 1, NULL);

    images[i] = image;
}


void
PixmapArray::setImageSize(Couple size)
{
    assert(size.x != 0 && size.y != 0);
    imageSize = size;
}
