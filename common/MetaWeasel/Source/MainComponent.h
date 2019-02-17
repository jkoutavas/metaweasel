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

#include <components/DocumentWindow.h>
#include <components/PathListDropComponent.h>

class MainComponent : public Component, private ApplicationCommandTarget, private MenuBarModel {
public:
    MainComponent();
    ~MainComponent();

public:  // Component
    void handleCommandMessage(int commandId) override;
    void paint(Graphics& g) override;
    void resized() override;

private:  // ApplicationCommandTarget
    void getAllCommands(Array<CommandID>& commands) override;
    void getCommandInfo(CommandID commandID, ApplicationCommandInfo& result) override;
    ApplicationCommandTarget* getNextCommandTarget() override;
    bool perform(const InvocationInfo& info) override;

private:  // MenuBarModel
    StringArray getMenuBarNames() override;
    PopupMenu getMenuForIndex(int topLevelMenuIndex, const String& menuName) override;
    void menuItemSelected(int menuItemID, int topLevelMenuIndex) override;

private:  // our stuff
    void openAboutWindow();
    void openImageFolder();

private:
    std::unique_ptr<hn::DocumentWindow> _aboutWindow;
    ApplicationCommandManager _appCommandManager;
    std::unique_ptr<hn::PathListDropComponent> _pathDrop;
    std::unique_ptr<TextButton> _removeBtn;

#if JUCE_WINDOWS
	std::unique_ptr<MenuBarComponent> _menuBar;
#endif

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};
