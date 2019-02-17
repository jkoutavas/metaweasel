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

#include "ImageListBox.h"
#include "ImageListModel.h"

enum { eFilenameColumnId = 1, eDescriptionColumnId };

ImageListBox::ImageListBox() {
    auto* h = new TableHeaderComponent();
    h->addColumn(translate("Filename"), eFilenameColumnId, 1);
    h->addColumn(translate("ImageDescription"), eDescriptionColumnId, 1);
    setHeader(h);
    setHeaderHeight(30);
    setRowHeight(50);

    _listModel = std::make_unique<ListModel>();
    setModel(_listModel.get());

    ImageListModel::getInstance()->addListener([this](auto model) {
        updateContent();
    });
}

void ImageListBox::resized() {
    TableListBox::resized();
    getHeader().setColumnWidth(eFilenameColumnId, getWidth() / 2);
    getHeader().setColumnWidth(eDescriptionColumnId, getWidth() / 2);
}

#if 0
#pragma mark - ListBoxBridge
#endif

Component* ImageListBox::getComponent() {
    return this;
}

void ImageListBox::updateList(const StringArray& paths) {
    ImageListModel::getInstance()->setPaths(paths);
}

#if 0
#pragma mark - ListModel
#endif

int ImageListBox::ListModel::getNumRows() {
    return static_cast<int>(ImageListModel::getInstance()->size());
}

void ImageListBox::ListModel::paintCell(Graphics& g, int rowNumber, int columnId, int width,
                                        int height, bool rowIsSelected) {}

void ImageListBox::ListModel::paintRowBackground(Graphics& g, int rowNumber, int width, int height,
                                                 bool rowIsSelected) {
    g.setColour(Colours::silver);
    g.drawHorizontalLine(height - 1, 0.0f, static_cast<float>(width));
}

Component* ImageListBox::ListModel::refreshComponentForCell(int row, int columnId, bool isSelected,
                                                            Component* existingComponentToUpdate) {
    auto cell = dynamic_cast<Label*>(existingComponentToUpdate);
    jassert(existingComponentToUpdate == nullptr || cell != nullptr);

    if (!cell) {
        cell = new Label();  // Lifetime is managed by ListBox
        cell->setColour(Label::textColourId, Colours::black);
    }

    auto listItem = ImageListModel::getInstance()->at(row);
    switch (columnId) {
        case eFilenameColumnId: {
            File file(listItem.first);
            cell->setText(file.getFileName(), dontSendNotification);
            break;
        }

        case eDescriptionColumnId:
            cell->setText(listItem.second, dontSendNotification);
            break;
    }

    return cell;
}
