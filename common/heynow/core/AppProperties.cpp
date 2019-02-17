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

#include "AppProperties.h"

juce_ImplementSingleton(AppProperties);

AppProperties::AppProperties() {
    PropertiesFile::Options options;
    options.applicationName = ProjectInfo::projectName;
    options.filenameSuffix = "settings";
    options.osxLibrarySubFolder = "Preferences";
    options.commonToAllUsers = false;
    options.storageFormat = PropertiesFile::storeAsBinary;
    userProps = new PropsFile(options);
}

AppProperties::~AppProperties() {
    clearSingletonInstance();
}

#if 0
#pragma mark - property bridging
#endif

String AppProperties::getValue(StringRef keyName, const String& defaultReturnValue) {
    return userProps->getValue(keyName, defaultReturnValue);
}

int AppProperties::getIntValue(StringRef keyName, int defaultReturnValue) {
    return userProps->getIntValue(keyName, defaultReturnValue);
}

bool AppProperties::getBoolValue(StringRef keyName, bool defaultReturnValue) {
    return userProps->getBoolValue(keyName, defaultReturnValue);
}

XmlElement* AppProperties::getXmlValue(StringRef keyName) {
    return userProps->getXmlValue(keyName);
}

void AppProperties::setValue(const String& keyName, const var& value) {
    userProps->setValue(keyName, value);
}

void AppProperties::setValue(const String& keyName, const XmlElement* xml) {
    userProps->setValue(keyName, xml);
}

void AppProperties::removeValue(StringRef keyName) {
    userProps->removeValue(keyName);
}

bool AppProperties::containsKey(StringRef keyName) {
    return userProps->containsKey(keyName);
}

void AppProperties::clear() {
    userProps->clear();
}
