/************************************************************************************************
 *   _    _                                                                                     *
 *  | |  | |                                                             Copyright 2018-2019    *
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

#include "MainComponent.h"

#include "AboutComponent.h"
#include "ImageListBox.h"
#include "ImageListModel.h"

#include "core/AppProperties.h"

enum { eAboutOpenCmdID = 1000, eFileOpenCmdID };

static String aboutMenuItemTitle() {
    return "About " + JUCEApplication::getInstance()->getApplicationName() + "...";
}

MainComponent::MainComponent() {
    auto& lf = dynamic_cast<LookAndFeel_V4&>(getLookAndFeel());
    lf.setColourScheme(LookAndFeel_V4::getLightColourScheme());

    _appCommandManager.registerAllCommandsForTarget(this);
    setApplicationCommandManagerToWatch(&_appCommandManager);
    _appCommandManager.setFirstCommandTarget(this);

#if JUCE_MAC
    PopupMenu pop;
    pop.addItem(eAboutOpenCmdID, aboutMenuItemTitle());
    MenuBarModel::setMacMainMenu(this, &pop);
#endif

#if JUCE_WINDOWS
    _menuBar.reset(new MenuBarComponent(this));
    addAndMakeVisible(_menuBar.get());
#endif

    _pathDrop = std::make_unique<hn::PathListDropComponent>(
        translate("Drop files and folders here"), std::make_unique<ImageListBox>());
    addAndMakeVisible(_pathDrop.get());

    _removeBtn = std::make_unique<TextButton>(translate("Remove descriptions"));
    _removeBtn->setEnabled(false);
    addAndMakeVisible(_removeBtn.get());

    int height = 400;
#if JUCE_WINDOWS
    height += lf.getDefaultMenuBarHeight();
#endif
    setSize(600, height);

    ImageListModel::getInstance()->addListener([this](auto model) {  //
        _removeBtn->setEnabled(model->size());
    });

    _removeBtn->onClick = [] {  //
        ImageListModel::getInstance()->removeAllDescriptions();
    };
}

MainComponent::~MainComponent() {
#if JUCE_MAC
    MenuBarModel::setMacMainMenu(nullptr);
#endif
    setApplicationCommandManagerToWatch(nullptr);
}

#if 0
#pragma mark - Component
#endif

void MainComponent::handleCommandMessage(int commandId) {}

void MainComponent::paint(Graphics& g) {
    g.fillAll(Colours::dodgerblue);
}

void MainComponent::resized() {
    auto bounds = getLocalBounds();

#if JUCE_WINDOWS
    auto menuHeight = 24;
    _menuBar->setBounds(0, 0, getWidth(), menuHeight);
#else
    auto menuHeight = 0;
#endif

    bounds.removeFromTop(24);
    _removeBtn->setBounds(
        bounds.withLeft(getWidth() - 189).withY(menuHeight + 12).withWidth(180).withHeight(32));

    bounds.removeFromTop(32 + menuHeight);
    _pathDrop->setBounds(bounds);
}

#if 0
#pragma mark - ApplicationCommandTarget
#endif

void MainComponent::getAllCommands(Array<CommandID>& commands) {
    // this returns the set of all commands that this target can perform
    const CommandID ids[] = {
        eAboutOpenCmdID,
        eFileOpenCmdID,
#if JUCE_WINDOWS
        juce::StandardApplicationCommandIDs::quit
#endif
    };

    commands.addArray(ids, numElementsInArray(ids));
}

void MainComponent::getCommandInfo(const CommandID commandID, ApplicationCommandInfo& result) {
    const String category("general");

    switch (commandID) {
        case eAboutOpenCmdID:
            result.setInfo(aboutMenuItemTitle(), "", category, 0);
            break;

        case eFileOpenCmdID:
            result.setInfo("Open Image Folder...", "", category, 0);
            result.addDefaultKeypress('o', ModifierKeys::commandModifier);
            break;

#if JUCE_WINDOWS
        case juce::StandardApplicationCommandIDs::quit:
            result.setInfo("Quit", "", category, 0);
            result.addDefaultKeypress('q', ModifierKeys::commandModifier);
            break;
#endif
    }
}

ApplicationCommandTarget* MainComponent::getNextCommandTarget() {
    return nullptr;
}

bool MainComponent::perform(const InvocationInfo& info) {
    switch (info.commandID) {
#if JUCE_WINDOWS
        case eAboutOpenCmdID:
            openAboutWindow();
            break;

        case juce::StandardApplicationCommandIDs::quit:
            JUCEApplication::quit();
            break;
#endif

        case eFileOpenCmdID:
            openImageFolder();
            break;
    }

    return true;
}

#if 0
#pragma mark - MenuBarModel
#endif

StringArray MainComponent::getMenuBarNames() {
    StringArray names;

#if JUCE_WINDOWS
    names.add(JUCEApplication::getInstance()->getApplicationName());
#endif

    names.add("File");

    return names;
}

PopupMenu MainComponent::getMenuForIndex(int /*topLevelMenuIndex*/, const String& menuName) {
    PopupMenu menu;

#if JUCE_WINDOWS
    if (menuName == JUCEApplication::getInstance()->getApplicationName()) {
        menu.addCommandItem(&_appCommandManager, eAboutOpenCmdID);
    }
#endif

    if (menuName == "File") {
        menu.addCommandItem(&_appCommandManager, eFileOpenCmdID);
#if JUCE_WINDOWS
        menu.addSeparator();
        menu.addCommandItem(&_appCommandManager, juce::StandardApplicationCommandIDs::quit);
#endif
    }

    return menu;
}

void MainComponent::menuItemSelected(int menuItemID, int /*topLevelMenuIndex*/) {
#if JUCE_MAC
    switch (menuItemID) {
        case eAboutOpenCmdID:
            openAboutWindow();
            break;

        default:
            break;
    }
#endif
}

#if 0
#pragma mark - our stuff
#endif

void MainComponent::openAboutWindow() {
    if (_aboutWindow) {
        _aboutWindow->toFront(true);
        return;
    }

    String title = "About ";
    title << ProjectInfo::projectName;
    _aboutWindow = std::make_unique<hn::DocumentWindow>(
        title, Desktop::getInstance().getDefaultLookAndFeel().findColour(
                   ResizableWindow::backgroundColourId),
        DocumentWindow::closeButton, [this]() {  //
            _aboutWindow.reset();
        });
    auto* c = new AboutComponent;
    _aboutWindow->setContentOwned(c, true);
    _aboutWindow->escapeKeyTriggersCloseButton(true);
    _aboutWindow->centreWithSize(c->getWidth(), c->getHeight());
    _aboutWindow->setVisible(true);
}

void MainComponent::openImageFolder() {
    String lastOpenedFolder = AppProperties::getInstance()->getValue("lastOpenedFolder");
    File folder(lastOpenedFolder);
    if (!folder.exists()) {
        folder = File::getSpecialLocation(File::userPicturesDirectory);
    }

    FileChooser fc(translate("Choose an image folder"), folder);
    if (fc.browseForDirectory()) {
        auto path = fc.getResult().getFullPathName();
        AppProperties::getInstance()->setValue("lastOpenedFolder", path);
        _pathDrop->addPaths(path);
    }
}
