
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

#include "PropsFile.h"

namespace PropertyFileConstants {
static const int magicNumber = (int)ByteOrder::littleEndianInt("PROP");
static const int magicNumberCompressed = (int)ByteOrder::littleEndianInt("CPRP");
static const int magicNumberEncrypted = (int)ByteOrder::littleEndianInt("ENCP");

static const char* const fileTag = "PROPERTIES";
static const char* const valueTag = "VALUE";
static const char* const nameAttribute = "name";
static const char* const valueAttribute = "val";
}

PropsFile::PropsFile(const PropertiesFile::Options& o)
    : PropertySet(o.ignoreCaseOfKeyNames),
      file(o.getDefaultFile()),
      options(o),
      loadedOk(false),
      needsWriting(false) {
    reload();
}

PropsFile::~PropsFile() {
    saveIfNeeded();
}

InterProcessLock::ScopedLockType* PropsFile::createProcessLock() const {
    return options.processLock != nullptr
               ? new InterProcessLock::ScopedLockType(*options.processLock)
               : nullptr;
}

bool PropsFile::reload() {
    ProcessScopedLock pl(createProcessLock());

    if (pl != nullptr && !pl->isLocked()) return false;  // locking failure..

    loadedOk = (!file.exists()) || loadAsBinary() || loadAsXml();
    return loadedOk;
}

bool PropsFile::saveIfNeeded() {
    const ScopedLock sl(getLock());
    return (!needsWriting) || save();
}

bool PropsFile::save() {
    const ScopedLock sl(getLock());

    stopTimer();

    if (options.doNotSave || file == File() || file.isDirectory() ||
        !file.getParentDirectory().createDirectory())
        return false;

    if (options.storageFormat == PropertiesFile::storeAsXML) return saveAsXml();

    return saveAsBinary();
}

bool PropsFile::loadAsXml() {
    XmlDocument parser(file);
    ScopedPointer<XmlElement> doc(parser.getDocumentElement(true));

    if (doc != nullptr && doc->hasTagName(PropertyFileConstants::fileTag)) {
        doc = parser.getDocumentElement();

        if (doc != nullptr) {
            forEachXmlChildElementWithTagName(*doc, e, PropertyFileConstants::valueTag) {
                const String name(e->getStringAttribute(PropertyFileConstants::nameAttribute));

                if (name.isNotEmpty()) {
                    getAllProperties().set(
                        name, e->getFirstChildElement() != nullptr
                                  ? e->getFirstChildElement()->createDocument("", true)
                                  : e->getStringAttribute(PropertyFileConstants::valueAttribute));
                }
            }

            return true;
        }

        // must be a pretty broken XML file we're trying to parse here,
        // or a sign that this object needs an InterProcessLock,
        // or just a failure reading the file.  This last reason is why
        // we don't jassertfalse here.
    }

    return false;
}

bool PropsFile::saveAsXml() {
    XmlElement doc(PropertyFileConstants::fileTag);
    const StringPairArray& props = getAllProperties();

    for (int i = 0; i < props.size(); ++i) {
        XmlElement* const e = doc.createNewChildElement(PropertyFileConstants::valueTag);
        e->setAttribute(PropertyFileConstants::nameAttribute, props.getAllKeys()[i]);

        // if the value seems to contain xml, store it as such..
        if (XmlElement* const childElement = XmlDocument::parse(props.getAllValues()[i]))
            e->addChildElement(childElement);
        else
            e->setAttribute(PropertyFileConstants::valueAttribute, props.getAllValues()[i]);
    }

    ProcessScopedLock pl(createProcessLock());

    if (pl != nullptr && !pl->isLocked()) return false;  // locking failure..

    if (doc.writeToFile(file, String())) {
        needsWriting = false;
        return true;
    }

    return false;
}

bool PropsFile::loadAsBinary() {
    FileInputStream fileStream(file);

    return fileStream.openedOk() ? loadAsBinary(fileStream) : false;
}

bool PropsFile::loadAsBinary(InputStream& input) {
    BufferedInputStream in(input, 2048);

    int numValues = in.readInt();

    while (--numValues >= 0 && !in.isExhausted()) {
        const String key(in.readString());
        String value(in.readString());

        jassert(key.isNotEmpty());
        if (key.isNotEmpty()) getAllProperties().set(key, value);
    }

    return true;
}

bool PropsFile::saveAsBinary() {
    ProcessScopedLock pl(createProcessLock());

    if (pl != nullptr && !pl->isLocked()) return false;  // locking failure..

    TemporaryFile tempFile(file);
    ScopedPointer<OutputStream> out(tempFile.getFile().createOutputStream());

    if (out != nullptr) {
        if (options.storageFormat == PropertiesFile::storeAsCompressedBinary) {
            out->writeInt(PropertyFileConstants::magicNumberCompressed);
            out->flush();

            out = new GZIPCompressorOutputStream(out.release(), 9, true);
        } else {
            // have you set up the storage option flags correctly?
            jassert(options.storageFormat == PropertiesFile::storeAsBinary);
        }

        const StringPairArray& props = getAllProperties();
        const int numProperties = props.size();
        const StringArray& keys = props.getAllKeys();
        const StringArray& values = props.getAllValues();

        out->writeInt(numProperties);

        for (int i = 0; i < numProperties; ++i) {
            out->writeString(keys[i]);
            out->writeString(values[i]);
        }

        out = nullptr;

        if (tempFile.overwriteTargetFileWithTemporary()) {
            needsWriting = false;
            return true;
        }
    }

    return false;
}

void PropsFile::timerCallback() {
    saveIfNeeded();
}

void PropsFile::propertyChanged() {
    sendChangeMessage();

    needsWriting = true;

    if (options.millisecondsBeforeSaving > 0)
        startTimer(options.millisecondsBeforeSaving);
    else if (options.millisecondsBeforeSaving == 0)
        saveIfNeeded();
}
