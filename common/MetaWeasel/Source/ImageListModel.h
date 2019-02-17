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

#include <utility> // std::pair<>
#include <vector>

class ImageListModel {

public:
    using ImageListItem = std::pair<String, String>;
    using ImageList = std::vector<ImageListItem>;
    using ListenerFunc = std::function<void(const ImageListModel* model)>;

    void addListener(ListenerFunc listenerFunc) {
        _listeners.push_back(listenerFunc);
    }

    const ImageListItem& at(int idx) {
        return _model.at(idx);
    }

    void removeAllDescriptions();
    
    size_t size() const {
        return _model.size();
    }

    void setPaths(const StringArray& paths);

private:
    void attemptToAddPathToModel(const String& path);
    void notifyListeners() const;

    std::vector<ListenerFunc> _listeners;
    ImageList _model;
    ThreadPool _threadPool;

public:
    JUCE_DECLARE_SINGLETON(ImageListModel, false)
};
