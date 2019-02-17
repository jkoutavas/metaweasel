/************************************************************************************************
 *   _    _                                                                                     *
 *  | |  | |                                                             Copyright 2018         *
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

#include "components/PathListDropComponent.h"

#include "ImageListModel.h"

class ImageListBox : public TableListBox, public hn::PathListDropComponent::ListBoxBridge {
public:
    ImageListBox();

    void resized() override;

public:  // ListBoxBridge
    Component* getComponent() override;
    void updateList(const StringArray& paths) override;

private:
    class ListModel : public TableListBoxModel {
    public:
        int getNumRows() override;

        void paintCell(Graphics& g, int rowNumber, int columnId, int width, int height,
                       bool rowIsSelected) override;

        void paintRowBackground(Graphics&, int rowNumber, int width, int height,
                                bool rowIsSelected) override;

        Component* refreshComponentForCell(int row, int columnId, bool isSelected,
                                           Component* component) override;
    };

    std::unique_ptr<ListModel> _listModel;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(ImageListBox)
};
