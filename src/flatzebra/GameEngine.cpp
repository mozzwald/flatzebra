/*  $Id: GameEngine.cpp,v 1.15 2010/08/15 02:16:11 sarrazip Exp $
    GameEngine.cpp - Abstract class representing an SDL Game Engine.

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

#include <flatzebra/GameEngine.h>

#include "font_13x7.xpm"

#include <assert.h>

using namespace std;
using namespace flatzebra;


GameEngine::GameEngine(Couple screenSizeInPixels,
                        const string &wmCaption,
                        bool fullScreen,
                        bool _processActiveEvent) throw(string)
  : theScreenSizeInPixels(screenSizeInPixels),
    theSDLScreen(NULL),
    fixedWidthFontPixmap(NULL),
    theDepth(0),
    usingFullScreen(false),
    processActiveEvent(_processActiveEvent)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0)
        throw string(SDL_GetError());

    SDL_WM_SetCaption(wmCaption.c_str(), wmCaption.c_str());

    string errorMsg = setVideoMode(screenSizeInPixels, fullScreen);
    if (!errorMsg.empty())
    {
        SDL_Quit();
        throw errorMsg;
    }

    assert(inFullScreenMode() == fullScreen);

    // Deselect unused event types:
    for (int i = SDL_NOEVENT; i < SDL_NUMEVENTS; i++)
    {
        switch (i)
        {
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            case SDL_QUIT:
                break;
            case SDL_ACTIVEEVENT:
                if (processActiveEvent)
                    break;
            default:
                SDL_EventState((Uint8) i, SDL_IGNORE);
        }
    }

    // Prepare a fixed width font pixmap:
    try
    {
        Couple dummy;
        loadPixmap(font_13x7_xpm, fixedWidthFontPixmap, dummy);
        assert(fixedWidthFontPixmap != NULL);
    }
    catch (PixmapLoadError)
    {
        throw string("Could not load fixed width font pixmap");
    }


    initWuLineAlgorithm();
}


GameEngine::~GameEngine()
{
    SDL_FreeSurface(fixedWidthFontPixmap);
    SDL_FreeSurface(theSDLScreen);
    SDL_Quit();
}


string
GameEngine::setVideoMode(Couple screenSizeInPixels, bool fullScreen)
{
    Uint32 flags = SDL_HWSURFACE | SDL_ANYFORMAT;
    if (fullScreen)
        flags |= SDL_FULLSCREEN;

    theDepth = SDL_VideoModeOK(
                        screenSizeInPixels.x, screenSizeInPixels.y,
                        32, flags);
    if (theDepth <= 0)
        return string("video mode not available");

    theSDLScreen = SDL_SetVideoMode(
                            screenSizeInPixels.x, screenSizeInPixels.y,
                        theDepth, flags);
    if (theSDLScreen == NULL)
        throw string(SDL_GetError());

    // Hide the mouse pointer when in full-screen mode.
    SDL_ShowCursor(fullScreen ? SDL_DISABLE : SDL_ENABLE);

    usingFullScreen = fullScreen;
    return string();
}


void GameEngine::run(int millisecondsPerFrame)
{
    for (;;)
    {
        Uint32 lastTime = SDL_GetTicks();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_KEYDOWN)
                processKey(event.key.keysym.sym, true);   // virtual function
            else if (event.type == SDL_KEYUP)
                processKey(event.key.keysym.sym, false);  // virtual function
            else if (processActiveEvent && event.type == SDL_ACTIVEEVENT)
            {
                // If app is now inactive (iconified or lost focus),
                // go wait for reactivation, i.e., stop calling tick().
                // This avoids using the CPU while the app is not active.
                //
                if (event.active.gain == 0 && !waitForReactivation())
                    return;  // wait ended with quit or with wait error
            }
            else if (event.type == SDL_QUIT)
                return;
        }

        if (!tick())  // virtual function
            return;

        SDL_Flip(theSDLScreen);

        // Pause for the rest of the current animation frame.
        Uint32 limit = lastTime + millisecondsPerFrame;
        Uint32 delay = limit - SDL_GetTicks();
        if (delay <= (Uint32) millisecondsPerFrame)
            SDL_Delay(delay);
    }
}


bool
GameEngine::waitForReactivation()
{
    // Virtual call: notify derived class that app is now inactive.
    //
    processActivation(false);

    // Allow effect of drawing commands made by processActivation() to appear.
    //
    SDL_Flip(theSDLScreen);

    // Sleep on SDL event loop until reactivation.
    //
    SDL_Event event;
    while (SDL_WaitEvent(&event))
    {
        // Ignore events other than quitting and reactivation.
        //
        switch (event.type)
        {
            case SDL_QUIT:
                processActivation(true);  // notify derived class that app is now active
                return false;  // ask caller to quit
            case SDL_ACTIVEEVENT:
                if (event.active.gain != 0)  // if app now active
                {
                    processActivation(true);
                    return true;  // ask caller to continue
                }
                break;
        }
    }
    return false;  // unexpected wait error: quit
}


/*virtual*/
void
GameEngine::processActivation(bool /*appActive*/)
{
}


void
GameEngine::loadPixmap(const char **xpmData, PixmapArray &pa, size_t index) const
                                                throw(PixmapLoadError)
{
    // Masks are not be relevant with SDL.

    SDL_Surface *pixmap;
    Couple size;
    loadPixmap(xpmData, pixmap, size);
    pa.setArrayElement(index, pixmap);
    pa.setImageSize(size);
}


void
GameEngine::loadPixmap(const char **xpmData,
                SDL_Surface *&pixmap,
                Couple &pixmapSize) const throw(PixmapLoadError)
{
    pixmapSize.zero();

    if (xpmData == NULL || xpmData[0] == NULL)
        throw PixmapLoadError(PixmapLoadError::INVALID_ARGS, NULL);

    pixmap = IMG_ReadXPMFromArray(const_cast<char **>(xpmData));
    if (pixmap == NULL)
        throw PixmapLoadError(PixmapLoadError::UNKNOWN, NULL);

    pixmapSize.x = pixmap->w;
    pixmapSize.y = pixmap->h;
}


void
GameEngine::writeString(const char *s, Couple pos, SDL_Surface *surface)
{
    assert(fixedWidthFontPixmap != NULL);
    if (s == NULL)
        return;
    if (surface == NULL)
        surface = theSDLScreen;
    Couple fontDim = getFontDimensions();
    SDL_Rect dest = { Sint16(pos.x), Sint16(pos.y), Uint16(fontDim.x), Uint16(fontDim.y) };
    for (size_t i = 0; s[i] != '\0'; i++, dest.x += Sint16(fontDim.x))
    {
        unsigned char c = (unsigned char) s[i];
        if (c < 32 || (c >= 127 && c <= 160))  // if ctrl char or undef char
            c = 32;  // replace by space

        /*  Compute the subrectangle of fixedWidthFontPixmap that
            contains the character:
        */
        int x = (c % 16) * fontDim.x;
        int y = (c - 32) / 16;
        if (y >= 8)
            y -= 2;
        y *= fontDim.y;

        SDL_Rect src  = { Sint16(x), Sint16(y), Uint16(fontDim.x), Uint16(fontDim.y) };
        SDL_BlitSurface(fixedWidthFontPixmap, &src, surface, &dest);
    }
}


void
GameEngine::writeStringCentered(const char *s, Couple pos, SDL_Surface *surface)
{
    Couple fontDim = getFontDimensions();
    Couple stringSizeInPixels(strlen(s) * fontDim.x, fontDim.y);
    writeString(s, pos - stringSizeInPixels / 2, surface);
}


void
GameEngine::writeStringXCentered(
                        const char *s, Couple pos, SDL_Surface *surface)
{
    Couple fontDim = getFontDimensions();
    int stringWidthInPixels = strlen(s) * fontDim.x;
    Couple adjustedPos(pos.x - stringWidthInPixels / 2, pos.y);
    writeString(s, adjustedPos, surface);
}


void
GameEngine::writeStringRightJustified(
                        const char *s, Couple pos, SDL_Surface *surface)
{
    Couple fontDim = getFontDimensions();
    int stringWidthInPixels = strlen(s) * fontDim.x;
    Couple adjustedPos(pos.x - stringWidthInPixels, pos.y);
    writeString(s, adjustedPos, surface);
}


///////////////////////////////////////////////////////////////////////////////


void
GameEngine::putpixel(SDL_Surface *surface, int x, int y, Uint32 pixel)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp)
    {
        case 1:
            *p = Uint8(pixel);
            break;

        case 2:
            * (Uint16 *) p = Uint16(pixel);
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            {
                p[0] = Uint8((pixel >> 16) & 0xFF);
                p[1] = Uint8((pixel >> 8) & 0xFF);
                p[2] = Uint8(pixel & 0xFF);
            }
            else
            {
                p[0] = Uint8(pixel & 0xFF);
                p[1] = Uint8((pixel >> 8) & 0xFF);
                p[2] = Uint8((pixel >> 16) & 0xFF);
            }
            break;

        case 4:
            * (Uint32 *) p = pixel;
            break;
    }
}


void
GameEngine::initWuLineAlgorithm()
{
    /* generate gamma correction table */
    const size_t numLevelsGammaTable = 256;
    const double Draw_gamma = 2.35;
    for (size_t i=0; i<numLevelsGammaTable; i++)
        gamma_table[i] = (unsigned char) ((numLevelsGammaTable - 1) * pow(
                    i / (double) (numLevelsGammaTable - 1), 1.0 / Draw_gamma));
}


// Source: http://mail.lokigames.com/ml/sdl/0288.html
//
void
GameEngine::wu_line(SDL_Surface *surface,
                    Uint32 x0, Uint32 y0, Uint32 x1, Uint32 y1,
                    Uint32 fgc, Uint32 bgc)
{
  const int nlevels = 256;
  const int nbits = 8;

  Uint32 intshift, erracc,erradj;
  Uint32 erracctmp, wgt, wgtcompmask;
  int dx, dy, tmp, xdir, i;
  Uint32 colors[nlevels];
  SDL_Rect r;
  SDL_Color fg, bg;

  SDL_GetRGB(fgc, surface->format, &fg.r, &fg.g, &fg.b);
  SDL_GetRGB(bgc, surface->format, &bg.r, &bg.g, &bg.b);

  /* generate the colors by linear interpolation, applying gamma correction */
  for (i=0; i<nlevels; i++) {
    Uint8 r, g, b;

    r = gamma_table[fg.r - (i*(fg.r - bg.r))/(nlevels-1)];
    g = gamma_table[fg.g - (i*(fg.g - bg.g))/(nlevels-1)];
    b = gamma_table[fg.b - (i*(fg.b - bg.b))/(nlevels-1)];
    colors[i] = SDL_MapRGB(surface->format, r, g, b);
  }
  if (y0 > y1) {
    tmp = y0; y0 = y1; y1 = tmp;
    tmp = x0; x0 = x1; x1 = tmp;
  }
  /* draw the initial pixel in the foreground color */
  putpixel(surface, x0, y0, fgc);

  dx = x1 - x0;
  xdir = (dx >= 0) ? 1 : -1;
  dx = (dx >= 0) ? dx : -dx;

  /* special-case horizontal, vertical, and diagonal lines which need no
     weighting because they go right through the center of every pixel. */
  if ((dy = y1 - y0) == 0) {
    /* horizontal line */
    r.x = (x0 < x1) ? x0 : x1;
    r.y = y0;
    r.w = dx;
    r.h = 1;
    SDL_FillRect(surface, &r, fgc);
    return;
  }

  if (dx == 0) {
    /* vertical line */
    r.x = x0;
    r.y = y0;
    r.w = 1;
    r.h = dy;
    SDL_FillRect(surface, &r, fgc);
    return;
  }

  if (dx == dy) {
    for (; dy != 0; dy--) {
      x0 += xdir;
      y0++;
      putpixel(surface, x0, y0, fgc);
    }
    return;
  }
  /* line is not horizontal, vertical, or diagonal */
  erracc = 0;                        /* err. acc. is initially zero */
  /* # of bits by which to shift erracc to get intensity level */
  intshift = 32 - nbits;
  /* mask used to flip all bits in an intensity weighting */
  wgtcompmask = nlevels - 1;
  /* x-major or y-major? */
  if (dy > dx) {
    /* y-major.  Calculate 16-bit fixed point fractional part of a pixel that
       X advances every time Y advances 1 pixel, truncating the result so that
       we won't overrun the endpoint along the X axis */
    erradj = ((uint64_t)dx << 32) / (uint64_t)dy;
    /* draw all pixels other than the first and last */
    while (--dy) {
      erracctmp = erracc;
      erracc += erradj;
      if (erracc <= erracctmp) {
        /* rollover in error accumulator, x coord advances */
        x0 += xdir;
      }
      y0++;                        /* y-major so always advance Y */
      /* the nbits most significant bits of erracc give us the intensity
         weighting for this pixel, and the complement of the weighting for
         the paired pixel. */
      wgt = erracc >> intshift;
      putpixel(surface, x0, y0, colors[wgt]);
      putpixel(surface, x0+xdir, y0, colors[wgt^wgtcompmask]);
    }
    /* draw the final pixel, which is always exactly intersected by the line
       and so needs no weighting */
    putpixel(surface, x1, y1, fgc);
    return;
  }
  /* x-major line.  Calculate 16-bit fixed-point fractional part of a pixel
     that Y advances each time X advances 1 pixel, truncating the result so
     that we won't overrun the endpoint along the X axis. */
  erradj = ((uint64_t)dy << 32) / (uint64_t)dx;
  /* draw all pixels other than the first and last */
  while (--dx) {
    erracctmp = erracc;
    erracc += erradj;
    if (erracc <= erracctmp) {
      /* accumulator turned over, advance y */
      y0++;
    }
    x0 += xdir;                        /* x-major so always advance X */
    /* the nbits most significant bits of erracc give us the intensity
       weighting for this pixel, and the complement of the weighting for
       the paired pixel. */
    wgt = erracc >> intshift;
    putpixel(surface, x0, y0, colors[wgt]);
    putpixel(surface, x0, y0+1, colors[wgt^wgtcompmask]);
  }
  /* draw final pixel, always exactly intersected by the line and doesn't
     need to be weighted. */
  putpixel(surface, x1, y1, fgc);
}
