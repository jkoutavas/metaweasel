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

#include "AboutComponent.h"

AboutComponent::AboutComponent() {
    _appNameLabel.reset(new Label("appNameLabel", TRANS("<name>\n")));
    addAndMakeVisible(_appNameLabel.get());
    _appNameLabel->setFont(Font(32.0f, Font::plain).withTypefaceStyle("Regular"));
    _appNameLabel->setJustificationType(Justification::centredLeft);
    _appNameLabel->setEditable(false, false, false);
    _appNameLabel->setColour(Label::textColourId, Colour(0xff080808));
    _appNameLabel->setColour(TextEditor::textColourId, Colours::black);
    _appNameLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    _appNameLabel->setBounds(7, 24, 160, 32);

    _copyrightLabel.reset(new Label("copyrightLabel", TRANS("(c) 2018 Heynow Software")));
    addAndMakeVisible(_copyrightLabel.get());
    _copyrightLabel->setFont(Font(20.0f, Font::plain).withTypefaceStyle("Regular"));
    _copyrightLabel->setJustificationType(Justification::centredLeft);
    _copyrightLabel->setEditable(false, false, false);
    _copyrightLabel->setColour(Label::textColourId, Colour(0xff0c0b0b));
    _copyrightLabel->setColour(TextEditor::textColourId, Colours::black);
    _copyrightLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    _copyrightLabel->setBounds(7, 80, 208, 21);

    _versionLabel.reset(new Label("versionLabel", TRANS("<ver>\n")));
    addAndMakeVisible(_versionLabel.get());
    _versionLabel->setFont(Font(18.0f, Font::plain).withTypefaceStyle("Regular"));
    _versionLabel->setJustificationType(Justification::centredLeft);
    _versionLabel->setEditable(false, false, false);
    _versionLabel->setColour(Label::textColourId, Colour(0xff0c0b0b));
    _versionLabel->setColour(TextEditor::textColourId, Colours::black);
    _versionLabel->setColour(TextEditor::backgroundColourId, Colour(0x00000000));

    _versionLabel->setBounds(168, 32, 64, 19);

    _weaselDrawable.reset(Drawable::createFromImageData(BinaryData::weaseldrawing_png,
                                                        BinaryData::weaseldrawing_pngSize));

    setSize(400, 350);

    _appNameLabel->setText(JUCEApplication::getInstance()->getApplicationName(),
                           NotificationType::dontSendNotification);
    _versionLabel->setText("v" + JUCEApplication::getInstance()->getApplicationVersion(),
                           NotificationType::dontSendNotification);
}

//==============================================================================
void AboutComponent::paint(Graphics& g) {
    g.fillAll(Colour(0xfff3f3f8));

    {
        Rectangle<float> weaselBounds(57.0f, 28.0f, 324.0f, 294.0f);
        g.setColour(Colours::black);
        _weaselDrawable->drawWithin(g, weaselBounds,
                     RectanglePlacement(RectanglePlacement::xMid | RectanglePlacement::yMid),
                     1.0f);
    }
}
