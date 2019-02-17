/************************************************************************************************
 *   _    _                                                                                     *
 *  | |  | |                                                             Copyright 2018-2019    *
 *  | |__| |   ___   _   _   _ __     ___   __      __                   Heynow Software, LLC   *
 *  |  __  |  / _ \ | | | | | '_ \   / _ \  \ \ /\ / /                                          *
 *  | |  | | |  __/ | |_| | | | | | | (_) |  \ V  V /                    author: Jay Koutavas   *
 *  |_|  |_|  \___|  \__, | |_| |_|  \___/    \_/\_/                     email: jay@heynow.com  *
 *                    __/ |                                                                     *
 *                   |___/   S  O  F  T  W  A  R  E                                             *
 *                                                                                              *
 *  This file is part of the "Meta Weasel" application. This application is free software; you  *
 *  can redistribute it and/or modify it under the terms of the GNU General Public License as   *
 *  published by the Free Software Foundation; either version 3, or (at your option) any        *
 *  later version.                                                                              *
 *                                                                                              *
 *  This application is distributed in the hope that it will be useful, but WITHOUT ANY         *
 *  WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  *
 *  PURPOSE. See the GNU General Public License for more details.                               *
 *                                                                                              *
 *  You should have received a copy of the GNU General Public License along with this           *
 *  application's source code; see the file COPYING. If not, see <http://www.gnu.org/licenses/>.*
 *                                                                                              *
 ************************************************************************************************/

#pragma once

#include "JuceHeader.h"

namespace hn {

class DocumentWindow : public juce::DocumentWindow {
public:
    using CloseFunc = std::function<void()>;
    DocumentWindow(const String& name, Colour backgroundColour, int requiredButtons,
                   CloseFunc closeFunc);

public:  // DocumentWindow
    /* Note: Be careful if you override any DocumentWindow methods - the base
       class uses a lot of them, so by overriding you might break its functionality.
       It's best to do all your work in your content component instead, but if
       you really have to override any DocumentWindow methods, make sure your
       subclass also calls the superclass's method.
    */

    void closeButtonPressed() override;

    bool keyPressed(const KeyPress& key) override;

public: // our stuff

    /** if set to true, then pressing the escape key will cause the close button to be triggered */
    void escapeKeyTriggersCloseButton(bool state) {
        _escapeKeyTriggersCloseButton = state;
    }

private:
    CloseFunc _closeFunc = nullptr;
    bool _escapeKeyTriggersCloseButton = false;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DocumentWindow)
};

}  // namespace hn
