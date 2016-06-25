/*  $Id: PixmapLoadError.cpp,v 1.2 2010/06/05 20:50:42 sarrazip Exp $
    PixmapLoadError.cpp - Exception describing an error when loading a pixmap

    flatzebra - Generic 2D Game Engine library
    Copyright (C) 1999, 2000, 2001 Pierre Sarrazin <http://sarrazip.com/>

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

#include <flatzebra/PixmapLoadError.h>

using namespace std;
using namespace flatzebra;


PixmapLoadError::PixmapLoadError(Code c, const char *filename)
  : theCode(c),
    theFilename(filename != NULL ? filename : "")
{
}


PixmapLoadError::PixmapLoadError(const PixmapLoadError &x)
  : theCode(x.theCode),
    theFilename(x.theFilename)
{
}


PixmapLoadError::~PixmapLoadError()
{
}


PixmapLoadError::Code PixmapLoadError::getCode() const
{
    return theCode;
}


string PixmapLoadError::getFilename() const
{
    return theFilename;
}
