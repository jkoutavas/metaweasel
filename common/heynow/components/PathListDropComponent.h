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

namespace hn {
class PathListDropComponent : public Component, public FileDragAndDropTarget {
public:
    /** A set of colour IDs to use to change the colour of various aspects of
        this component.

        These constants can be used either via the Component::setColour(), or
        LookAndFeel::setColour() methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour,
       LookAndFeel::findColour
    */
    enum ColourIds {
        backgroundColourId = 0x2000000, /**< The background colour to fill the drop traget with. */
        pathTextColourId = 0x2000001,   /**< The colour for the path's text. */
        promptTextColourId = 0x2000002, /**< The colour for the prompt's text. */
        outlineColourId = 0x2000003,    /**< optional border colour when not dragged over. */
        draggingOutlineColourId = 0x200004 /**< optional border colour when being dragged over. */
    };

public:
    class ListBoxBridge {
    public:
        virtual ~ListBoxBridge() {};
        virtual Component* getComponent() = 0;
        virtual void updateList(const StringArray& paths) = 0;
    };

    PathListDropComponent(const String& prompt, std::unique_ptr<ListBoxBridge> listBox);

public:  // our stuff
    void addPaths(const StringArray& paths);

public:  // Conmponent
    void paint(Graphics&) override;
    void resized() override;

public:  // FileDragAndDropTarget
    bool isInterestedInFileDrag(const StringArray& files) override;
    void fileDragEnter(const StringArray& files, int x, int y) override;
    void fileDragExit(const StringArray& files) override;
    void filesDropped(const StringArray& files, int x, int y) override;

private:
    bool _dragEntered = false;
    std::unique_ptr<ListBoxBridge> _listBox;
    StringArray _paths;
    String _prompt;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PathListDropComponent)
};
}  // namspace hn
