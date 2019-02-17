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

#ifndef APPPROPERTIES_H_INCLUDED
#define APPPROPERTIES_H_INCLUDED

#include "PropsFile.h"

class AppProperties {
public:
    juce_DeclareSingleton(AppProperties, true);
    AppProperties();
    ~AppProperties();

public:
    String getValue(StringRef keyName, const String& defaultReturnValue = String());
    int getIntValue(StringRef keyName, int defaultReturnValue = 0);
    bool getBoolValue(StringRef keyName, bool defaultReturnValue = false);
    XmlElement* getXmlValue(StringRef keyName);
    void setValue(const String& keyName, const var& value);
    void setValue(const String& keyName, const XmlElement* xml);
    void removeValue(StringRef keyName);
    bool containsKey(StringRef keyName);
    void clear();

private:
    ScopedPointer<PropsFile> userProps;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AppProperties);
};

#endif  // APPPROPERTIES_H_INCLUDED
