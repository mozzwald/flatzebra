/*  $Id: PixmapArray.h,v 1.5 2012/07/08 02:57:14 sarrazip Exp $
    PixmapArray.h - Object containing an array of Pixmaps.

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

#ifndef _H_PixmapArray
#define _H_PixmapArray

#include <flatzebra/Couple.h>

#include <SDL.h>
#include <SDL_image.h>

#include <vector>


namespace flatzebra {


class PixmapArray
/*  Object containing an array of Pixmaps.
*/
{
public:

    /*  Creates a pixmap array.
        This object is the owner of the pixmaps, and the destructor will
        take care of freeing them.
        The argument is ignored.
        setArrayElement() will grow the internal vector as needed.
    */
    PixmapArray(size_t = 0);

    /*  Calls freeImages().
    */
    ~PixmapArray();

    /*  Destroys the images and frees the resources taken by the pixmaps.
    */
    void freeImages();

    /*  Returns the pixmap of the image at index 'i' of the
        arrays given to the constructor of this object.
        'i' must be lower than the value returned by getNumImages().
    */
    SDL_Surface *getImage(size_t i) const;
    size_t getNumImages() const;

    /*  'image' must not be null.
    */
    void setArrayElement(size_t i, SDL_Surface *image);

    /*  Sets or gets the size in pixels of the images in the pixmap array.
        All images in the array are assumed to be of the same size.
        Neither size.x nor size.y are allowed to be zero.
    */
    void setImageSize(Couple size);
    Couple getImageSize() const;

private:

    std::vector<SDL_Surface *> images;
    Couple imageSize;  // size in pixels of the images; all assumed same size


    /*  Forbidden operations:
    */
    PixmapArray(const PixmapArray &x);
    PixmapArray &operator = (const PixmapArray &x);

};


/*  INLINE METHODS
*/

inline SDL_Surface *
PixmapArray::getImage(size_t i) const { return images[i]; }
inline size_t
PixmapArray::getNumImages() const { return images.size(); }
inline Couple
PixmapArray::getImageSize() const { return imageSize; }


}  // namespace flatzebra


#endif  /* _H_PixmapArray */
