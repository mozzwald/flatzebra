/*  $Id: Joystick.h,v 1.9 2012/07/06 04:28:45 sarrazip Exp $
    Joystick.h - Joystick or game pad encapsulation.

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

#ifndef _H_Joystick
#define _H_Joystick

#include <SDL.h>

#include <vector>


namespace flatzebra {


// In this version of flatzebra, this class is merely experimental
// and only supports the Xbox 360 USB controller.
//
class Joystick
{
public:

    /* These button numbers are specific to the Xbox 360 USB controller.
       See: http://en.wikipedia.org/wiki/Xbox_360_Controller
       On GNU/Linux, this command can normally be used to see the button
       and joystick numbers: jstest /dev/input/js0
    */
    enum
    {
        A_BTN = 0,
        B_BTN = 1,
        X_BTN = 2,
        Y_BTN = 3,
        START_BTN = 7,
        BACK_BTN = 6,
    };

    /* These axis numbers are specific to the Xbox 360 USB controller.
    */
    enum
    {
        // Main joystick.
        MAIN_X = 0,
        MAIN_Y = 1,

        // Secondary joystick.
        SECONDARY_X = 3,
        SECONDARY_Y = 4
    };

    /*  These trigger numbers are specific to the Xbox 360 USB controller.
    */
    enum
    {
        LT = 2,  // Left Trigger
        RT = 5,  // Right Trigger
    };

    /*  Asks SDL to open joystick number zero.
        Call isJoystickAvailable() after this constructor to check
        if the joystick was detected and successfully initialized.
        Call update() on this object at each frame.
    */
    Joystick();

    /*  Call this after construction to check if the joystick was
        detected and successfully initialized.
    */
    bool isJoystickAvailable() const;

    /*  Asks SDL to close the joystick if it was successfully opened
        by the constructor. Does nothing if no joystick was detected.
    */
    ~Joystick();

    /*  Returns true or false to indicate if the designated button
        is currently pressed.
        Can be called even if no joystick was detected: false will
        be returned.
    */
    bool getButton(int buttonNumber) const;

    /*  Returns true or false to indicate if the designated button
        is currently pressed but was not pressed before the most
        recent call to update().
    */
    bool buttonJustPressed(int buttonNumber) const;

    /*  Returns value between -32768 (left) and 32767 (right).
        stickNumber must be 0 (main joystick) or 1 (secondary joystick).
    */
    int getXAxisValue(int stickNumber = 0) const { return xAxis[stickNumber]; }

    /*  Returns -1 if joystick is near the leftmost position,
        +1 if it is near the rightmost position, or 0 otherwise.
    */
    int getXAxisDisplacement(int stickNumber = 0) const;

    /*  Returns value between -32768 (up) and 32767 (down).
    */
    int getYAxisValue(int stickNumber = 0) const { return yAxis[stickNumber]; }

    /*  Returns -1 if joystick is near the highest position,
        +1 if it is near the lowest position, or 0 otherwise.
    */
    int getYAxisDisplacement(int stickNumber = 0) const;

    /*  Returns value between -32768 (not pressed) and 32767 (fully pressed).
        triggerNumber must be LT or RT (see previous enum).
    */
    int getTriggerValue(int triggerNumber) const;

    /*  Asks SDL to update the joystick state.
        Also remembers the previous state of the buttons, which
        allows the buttonJustPressed() method to work.
    */
    void update();

    /*  Returns the number of buttons on the detected joystick,
        or zero if no joystick was detected.
    */
    size_t getNumButtons() const;

private:

    /*  Value (between 0 and 32767) beyond which an axis is considered
        to be far enough from the center, for use by getXAxisDisplacement()
        and getYAxisDisplacement().
    */
    enum { AXIS_THRESHOLD = 16000 };

    SDL_Joystick *joystick;
    std::vector<bool> joystickButtons;  // previous state of buttons
    Sint16 xAxis[2];  // -32768..32767
    Sint16 yAxis[2];  // -32768..32767

    // Forbidden operations:
    Joystick(const Joystick &);
    Joystick &operator = (const Joystick &);

};


}  // namespace flatzebra


#endif  /* _H_Joystick */
