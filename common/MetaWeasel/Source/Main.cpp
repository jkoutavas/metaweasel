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

#include "JuceHeader.h"

#include "ImageListModel.h"
#include "MainComponent.h"

#include "core/AppProperties.h"

#if JUCE_WINDOWS
 // somewhere in our application we need to pull in these libraries, Main.cpp is as good
 // as any place
#if JUCE_DEBUG
#pragma comment(lib, "zlibd.lib")
#else
#pragma comment(lib, "zlib.lib")
#endif
#endif

class NewProjectApplication : public JUCEApplication {
public:
    NewProjectApplication() {}

    const String getApplicationName() override {
        return ProjectInfo::projectName;
    }
    const String getApplicationVersion() override {
        return ProjectInfo::versionString;
    }
    bool moreThanOneInstanceAllowed() override {
        return true;
    }

    void initialise(const String& commandLine) override {
#if JUCE_MAC
        mainWindow.reset(new MainWindow(getApplicationName()));
#else
		mainWindow.reset(new MainWindow(""));
#endif
    }

    void shutdown() override {
        mainWindow = nullptr;  // (deletes our window)

        // delete our singletons
        AppProperties::deleteInstance();
        ImageListModel::deleteInstance();
    }

    void systemRequestedQuit() override {
        quit();
    }

    void anotherInstanceStarted(const String& commandLine) override {
        // When another instance of the app is launched while this one is running,
        // this method is invoked, and the commandLine parameter tells you what
        // the other instance's command-line arguments were.
    }

    class MainWindow : public DocumentWindow {
    public:
        MainWindow(String name)
            : DocumentWindow(name, Desktop::getInstance().getDefaultLookAndFeel().findColour(
                                       ResizableWindow::backgroundColourId),
                             DocumentWindow::allButtons) {
            setUsingNativeTitleBar(true);
            setContentOwned(new MainComponent(), true);

            centreWithSize(getWidth(), getHeight());
            setVisible(true);

            setResizable(true, true);
            setResizeLimits(400, 400, 2000, 2000);
        }

        void closeButtonPressed() override {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_JUCE_APPLICATION(NewProjectApplication)
