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

#ifndef JUCE_PROPSFILE_H_INCLUDED
#define JUCE_PROPSFILE_H_INCLUDED

#include "JuceHeader.h"

class PropsFile : public PropertySet, public ChangeBroadcaster, private Timer {
public:
    explicit PropsFile(const PropertiesFile::Options& options);
    ~PropsFile();

    const File& getFile() const {
        return file;
    }
    bool reload();
    bool saveIfNeeded();
    bool save();

protected:
    void propertyChanged() override;

private:
    File file;
    PropertiesFile::Options options;
    bool loadedOk, needsWriting;

    typedef const ScopedPointer<InterProcessLock::ScopedLockType> ProcessScopedLock;
    InterProcessLock::ScopedLockType* createProcessLock() const;

    void timerCallback() override;
    bool saveAsXml();
    bool saveAsBinary();
    bool loadAsXml();
    bool loadAsBinary();
    bool loadAsBinary(InputStream&);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PropsFile)
};

#endif  // JUCE_PROPSFILE_H_INCLUDED
