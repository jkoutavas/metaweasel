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

#include "DocumentWindow.h"

namespace hn {

DocumentWindow::DocumentWindow(const String& name, Colour backgroundColour, int requiredButtons,
                               CloseFunc closeFunc)
    : juce::DocumentWindow(name, backgroundColour, requiredButtons, true), _closeFunc(closeFunc) {
    setUsingNativeTitleBar(true);
    setName(name);
}

void DocumentWindow::closeButtonPressed() {
    if (_closeFunc) {
        _closeFunc();
    }
}

bool DocumentWindow::keyPressed(const KeyPress& key) {
    if (key == KeyPress::escapeKey && _escapeKeyTriggersCloseButton) {
        closeButtonPressed();
        return true;
    }

    return juce::DocumentWindow::keyPressed(key);
}

}  // namespace hn
