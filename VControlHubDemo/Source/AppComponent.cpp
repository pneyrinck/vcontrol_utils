/*
  ==============================================================================

  This is an automatically generated GUI class created by the Introjucer!

  Be careful when adding custom code to these files, as only the code within
  the "//[xyz]" and "//[/xyz]" sections will be retained when the file is loaded
  and re-saved.

  Created with Introjucer version: 3.1.1

  ------------------------------------------------------------------------------

  The Introjucer is part of the JUCE library - "Jules' Utility Class Extensions"
  Copyright 2004-13 by Raw Material Software Ltd.

  ==============================================================================
*/

//[Headers] You can add your own extra header files here...
//[/Headers]

#include "AppComponent.h"


//[MiscUserDefs] You can add your own user definitions and misc code here...
//[/MiscUserDefs]

//==============================================================================
AppComponent::AppComponent ()
{
    addAndMakeVisible (tabs = new TabbedComponent (TabbedButtonBar::TabsAtTop));
    tabs->setTabBarDepth (30);
    tabs->setCurrentTabIndex (-1);

    addAndMakeVisible (logText = new TextEditor ("logText"));
    logText->setMultiLine (true);
    logText->setReturnKeyStartsNewLine (false);
    logText->setReadOnly (true);
    logText->setScrollbarsShown (true);
    logText->setCaretVisible (false);
    logText->setPopupMenuEnabled (true);
    logText->setText (String::empty);


    //[UserPreSize]
    lines = 0;
    //[/UserPreSize]

    setSize (600, 400);


    //[Constructor] You can add your own custom stuff here..
    //[/Constructor]
}

AppComponent::~AppComponent()
{
    //[Destructor_pre]. You can add your own custom destruction code here..
    //[/Destructor_pre]

    tabs = nullptr;
    logText = nullptr;


    //[Destructor]. You can add your own custom destruction code here..
    //[/Destructor]
}

//==============================================================================
void AppComponent::paint (Graphics& g)
{
    //[UserPrePaint] Add your own custom painting code here..
    //[/UserPrePaint]

    g.fillAll (Colours::white);

    //[UserPaint] Add your own custom painting code here..
    //[/UserPaint]
}

void AppComponent::resized()
{
    //[UserPreResize] Add your own custom resize code here..
    //[/UserPreResize]

    tabs->setBounds (8, proportionOfHeight (0.0095f), getWidth() - 16, proportionOfHeight (0.7109f));
    logText->setBounds (8, proportionOfHeight (0.7299f), getWidth() - 16, proportionOfHeight (0.2595f));
    //[UserResized] Add your own custom resize handling here..
    //[/UserResized]
}



//[MiscUserCode] You can add your own definitions of your custom methods or any other code here...
void AppComponent::appendText(String text)
{
    String msg;
    msg = Time::getCurrentTime().toString(false, true) + " " + text;

    logMessages.add(msg);
    triggerAsyncUpdate();
    
    lines++;
}

void AppComponent::handleAsyncUpdate()
{
    if (lines > 500)
    {
        lines = 0;
        logText->clear();
    }
    
    logText->insertTextAtCaret(logMessages.joinIntoString("\n") + "\n");
    logText->moveCaretToEnd();
    
    logMessages.clear();
}
//[/MiscUserCode]


//==============================================================================
#if 0
/*  -- Introjucer information section --

    This is where the Introjucer stores the metadata that describe this GUI layout, so
    make changes in here at your peril!

BEGIN_JUCER_METADATA

<JUCER_COMPONENT documentType="Component" className="AppComponent" componentName=""
                 parentClasses="public Component, public AsyncUpdater" constructorParams=""
                 variableInitialisers="" snapPixels="8" snapActive="1" snapShown="1"
                 overlayOpacity="0.330" fixedSize="0" initialWidth="600" initialHeight="400">
  <BACKGROUND backgroundColour="ffffffff"/>
  <TABBEDCOMPONENT name="tabs" id="492d5c86b8cc949e" memberName="tabs" virtualName=""
                   explicitFocusOrder="0" pos="8 0.948% 16M 71.09%" orientation="top"
                   tabBarDepth="30" initialTab="-1"/>
  <TEXTEDITOR name="logText" id="368c04ae0e7e3ba5" memberName="logText" virtualName=""
              explicitFocusOrder="0" pos="8 72.986% 16M 25.948%" initialText=""
              multiline="1" retKeyStartsLine="0" readonly="1" scrollbars="1"
              caret="0" popupmenu="1"/>
</JUCER_COMPONENT>

END_JUCER_METADATA
*/
#endif


//[EndFile] You can add extra defines here...
//[/EndFile]
