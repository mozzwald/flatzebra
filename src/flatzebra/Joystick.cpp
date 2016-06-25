/*  $Id: Joystick.cpp,v 1.7 2012/07/06 04:28:45 sarrazip Exp $
    Joystick.cpp - Joystick or game pad encapsulation.

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

#include "Joystick.h"


using namespace std;
using namespace flatzebra;


Joystick::Joystick()
:   joystick(NULL),
    joystickButtons(),
    xAxis(),
    yAxis()
{
    xAxis[0] = xAxis[1] = 0;
    yAxis[0] = yAxis[1] = 0;

    if (SDL_Init(SDL_INIT_JOYSTICK) == 0 && SDL_NumJoysticks() > 0)
    {
        joystick = SDL_JoystickOpen(0);
        if (joystick != NULL)
            joystickButtons.resize(size_t(SDL_JoystickNumButtons(joystick)), false);
    }
}


bool
Joystick::isJoystickAvailable() const
{
    return joystick != NULL;
}


size_t
Joystick::getNumButtons() const
{
    return joystickButtons.size();
}


Joystick::~Joystick()
{
    if (joystick != NULL)
        SDL_JoystickClose(joystick);
}


bool
Joystick::getButton(int buttonNumber) const
{
    if (joystick == NULL)  // if no joystick detected
        return false;
    if (buttonNumber < 0 || buttonNumber >= static_cast<int>(joystickButtons.size()))
        return false;
    return SDL_JoystickGetButton(joystick, buttonNumber) != 0;
}


bool
Joystick::buttonJustPressed(int buttonNumber) const
{
    return getButton(buttonNumber) && !joystickButtons[static_cast<size_t>(buttonNumber)];
}


int
Joystick::getXAxisDisplacement(int stickNumber) const
{
    if (xAxis[stickNumber] < -AXIS_THRESHOLD)
        return -1;
    if (xAxis[stickNumber] > +AXIS_THRESHOLD)
        return +1;
    return 0;
}


int
Joystick::getYAxisDisplacement(int stickNumber) const
{
    if (yAxis[stickNumber] < -AXIS_THRESHOLD)
        return -1;
    if (yAxis[stickNumber] > +AXIS_THRESHOLD)
        return +1;
    return 0;
}


int
Joystick::getTriggerValue(int triggerNumber) const
{
    return SDL_JoystickGetAxis(joystick, triggerNumber);
}


void
Joystick::update()
{
    if (joystick == NULL)
        return;

    // Remember current joystick button states for next frame.
    for (size_t i = joystickButtons.size(); i--; )
        joystickButtons[static_cast<size_t>(i)] = (getButton(i) != 0);

    // Update joystick button states.
    SDL_JoystickUpdate();

    xAxis[0] = SDL_JoystickGetAxis(joystick, MAIN_X);
    yAxis[0] = SDL_JoystickGetAxis(joystick, MAIN_Y);
    xAxis[1] = SDL_JoystickGetAxis(joystick, SECONDARY_X);
    yAxis[1] = SDL_JoystickGetAxis(joystick, SECONDARY_Y);
}
