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

#include "ImageListModel.h"

#include <exiv2/exiv2.hpp>

constexpr const char* kImageDescriptionKey = "Exif.Image.ImageDescription";

void ImageListModel::attemptToAddPathToModel(const String& path) {
    try {
        Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(path.toStdString());
        if (image.get() != 0) {
            image->readMetadata();
            Exiv2::ExifData& exifData = image->exifData();
            if (!exifData.empty()) {
                String desc = exifData[kImageDescriptionKey].toString();
                if (desc.length()) {
                    _model.push_back(std::make_pair(path, desc));
                }
            }
        }
    } catch (...) {
    };
}

void ImageListModel::setPaths(const StringArray& paths) {
    _threadPool.addJob([this, paths] {
        _model.clear();
        for (auto path : paths) {
            File file(path);
            if (file.isDirectory()) {
                DirectoryIterator iter(file, true);
                while (iter.next()) {
                    attemptToAddPathToModel(iter.getFile().getFullPathName());
                };
            } else {
                attemptToAddPathToModel(path);
            }
        }
        MessageManager::callAsync([this] {
            notifyListeners();
        });
    });
}

void ImageListModel::notifyListeners() const {
    for (auto listener : _listeners) {
        listener(this);
    }
}

void ImageListModel::removeAllDescriptions() {
    try {
        for (auto pair : _model) {
            Exiv2::Image::AutoPtr image = Exiv2::ImageFactory::open(pair.first.toStdString());
            if (image.get() != 0) {
                image->readMetadata();
                Exiv2::ExifData& exifData = image->exifData();
                if (!exifData.empty()) {
                    String desc = exifData[kImageDescriptionKey].toString();
                    if (desc.length()) {
                        auto key = Exiv2::ExifKey(kImageDescriptionKey);
                        auto pos = exifData.findKey(key);
                        if (pos != exifData.end()) {
                            exifData.erase(pos);
                            image->setExifData(exifData);
                            image->writeMetadata();
                        }
                    }
                }
            }
        }
    } catch(...) {}

    _model.clear();
    notifyListeners();
}

JUCE_IMPLEMENT_SINGLETON(ImageListModel)
