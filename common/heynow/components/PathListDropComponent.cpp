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

#include "PathListDropComponent.h"

namespace hn {

PathListDropComponent::PathListDropComponent(const String& prompt,
                                             std::unique_ptr<ListBoxBridge> listBox)
    : _listBox(std::move(listBox)), _prompt(prompt) {}

void PathListDropComponent::paint(Graphics& g) {
    g.fillAll(isColourSpecified(backgroundColourId) ? findColour(backgroundColourId)
                                                    : Colours::white);

    if (_dragEntered) {
        g.setColour(isColourSpecified(draggingOutlineColourId) ? findColour(draggingOutlineColourId)
                                                               : Colours::green);

    } else {
        g.setColour(isColourSpecified(outlineColourId) ? findColour(outlineColourId)
                                                       : Colours::lightgrey);
    }

    g.drawRect(getLocalBounds(), 2);

    if (_paths.isEmpty()) {
        g.setColour(isColourSpecified(promptTextColourId) ? findColour(promptTextColourId)
                                                          : Colours::lightblue);
        g.setFont(Font(48.0f, Font::italic));
        g.drawText(_prompt, getLocalBounds(), Justification::centred, true);
    }
}

void PathListDropComponent::resized() {
    auto bounds = getLocalBounds();
    _listBox->getComponent()->setBounds(bounds);
}

#if 0
#pragma mark - FileDragAndDropTarget
#endif

bool PathListDropComponent::isInterestedInFileDrag(const StringArray& files) {
    return isEnabled();
}

void PathListDropComponent::fileDragEnter(const StringArray& /*files*/, int /*x*/, int /*y*/) {
    _dragEntered = true;
    repaint();
}

void PathListDropComponent::fileDragExit(const StringArray& /*files*/) {
    _dragEntered = false;
    repaint();
}

void PathListDropComponent::filesDropped(const StringArray& files, int /*x*/, int /*y*/) {
    _dragEntered = false;
    repaint();

    addPaths(files);
}

#if 0
#pragma mark - our stuff
#endif

void PathListDropComponent::addPaths(const StringArray& paths) {
    auto oldSize = _paths.size();
    _paths.mergeArray(paths);
    if (_paths.size() != oldSize) {
        _listBox->updateList(_paths);
        if (!_listBox->getComponent()->getParentComponent()) {
            addAndMakeVisible(_listBox->getComponent());
        }
        repaint();
    }
}

}  // namespace hn
