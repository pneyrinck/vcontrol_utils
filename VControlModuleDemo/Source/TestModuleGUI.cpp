#include "TestModuleGUI.h"
#include "TestModule.h"

extern ApplicationProperties* appProperties;

class TestModuleContent : public Component,
                          public Slider::Listener,
                          public Button::Listener,
                          public Timer
{
public:
    TestModuleContent(TestModule* testModule_) : testModule(testModule_)
    {
        setSize(1200, 500);
        
        for (int i = 0; i < testModule->paramValues.size(); i++)
        {
            String type = testModule->paramType[i];
            if (type == kVControlParameterTypeContinuous ||
                type == kVControlParameterTypeIndexed)
            {
                Slider* slider = new Slider(Slider::LinearVertical, Slider::NoTextBox);
                addAndMakeVisible(slider);
                slider->setRange(0, testModule->paramMax[i], type == kVControlParameterTypeIndexed ? 1.0 : 0.0);
                slider->addListener(this);
                sliders.add(slider);
                buttons.add(new TextButton());
            }
            if (type == kVControlParameterTypeMomentary)
            {
                TextButton* button = new TextButton("M");
                addAndMakeVisible(button);
                button->addListener(this);
                buttons.add(button);
                sliders.add(new Slider());
            }
            if (type == kVControlParameterTypeBoolean)
            {
                ToggleButton* button = new ToggleButton("B");
                addAndMakeVisible(button);
                button->addListener(this);
                buttons.add(button);
                sliders.add(new Slider());
            }
        }
        
        for (int i = 0; i < testModule->paramValues.size() * 4; i++)
        {
            ToggleButton* t = new ToggleButton();
            t->setRadioGroupId(i / 4 + 1);
            t->addListener(this);
            toggles.add(t);
            addAndMakeVisible(t);
        }
        for (int i = 0; i < testModule->paramValues.size(); i++)
        {
            toggles[i * 4 + 0]->setButtonText("Point");
            toggles[i * 4 + 1]->setButtonText("Boostcut");
            toggles[i * 4 + 2]->setButtonText("Wrap");
            toggles[i * 4 + 3]->setButtonText("Spread");
        }
        
        addAndMakeVisible(jsonEditor = new TextEditor());
        jsonEditor->setMultiLine(true);
        jsonEditor->setReadOnly(true);
        
        resized();
        update();
        
        startTimer(100);
    }
    
    void resized()
    {
        int num = sliders.size();
        if (num > 0)
        {
            int w = getWidth() / 2 / num;
            for (int i = 0; i < num; i++)
            {
                sliders[i]->setBounds(w * i, 0, w, getHeight() - 80);
                buttons[i]->setBounds(w * i, (getHeight() - 80) / 2 - 15, w, 30);
            }
        }
        num = toggles.size();
        if (num > 0)
        {
            int w = getWidth() / 2 / (num / 4);
            for (int i = 0; i < num; i++)
                toggles[i]->setBounds(i / 4 * w, (i % 4) * 20 + getHeight() - 80, w, 20);
        }
        if (jsonEditor)
            jsonEditor->setBounds(getLocalBounds().removeFromRight(getWidth() / 2));
    }
    
    void sliderValueChanged (Slider* slider)
    {
        int idx = sliders.indexOf(slider);
        testModule->paramValues.set(idx, slider->getValue());
        
        CCModuleSetValue(testModule->module, CCJsonPtrAppend(testModule->jsonPtrs[idx], kVControlProperty_ValueNumber), testModule->paramValues[idx]);
    }
    
    void buttonClicked (Button* button)
    {
        if (toggles.contains(button))
        {
            if (button->getToggleState())
            {
                int idx = toggles.indexOf(button);
                
                if (idx % 4 == 0)
                    CCModuleSetValue(testModule->module, CCJsonPtrAppend(testModule->jsonPtrs[idx / 4], kVControlProperty_DisplayMode), kVControlDisplayMode_Point);
                if (idx % 4 == 1)
                    CCModuleSetValue(testModule->module, CCJsonPtrAppend(testModule->jsonPtrs[idx / 4], kVControlProperty_DisplayMode), kVControlDisplayMode_BoostCut);
                if (idx % 4 == 2)
                    CCModuleSetValue(testModule->module, CCJsonPtrAppend(testModule->jsonPtrs[idx / 4], kVControlProperty_DisplayMode), kVControlDisplayMode_Wrap);
                if (idx % 4 == 3)
                    CCModuleSetValue(testModule->module, CCJsonPtrAppend(testModule->jsonPtrs[idx / 4], kVControlProperty_DisplayMode), kVControlDisplayMode_Spread);
            }
            
        }
        if (buttons.contains(button))
        {
            int idx = buttons.indexOf(button);
            
            if (testModule->paramType[idx] == kVControlParameterTypeMomentary)
            {
                testModule->paramValues.set(idx, 1);
                //CCModuleUpdateControlValue(testModule->module, idx, (SVControlPropertyValue){cVControlPropertyType_Number, 0, int(testModule->paramValues[idx]), testModule->paramValues[idx]});
                
                CCModuleSetValue(testModule->module, CCJsonPtrAppend(testModule->jsonPtrs[idx], kVControlProperty_ValueNumber), testModule->paramValues[idx]);
            }
            if (testModule->paramType[idx] == kVControlParameterTypeBoolean)
            {
                testModule->paramValues.set(idx, button->getToggleState() ? 0 : 1);
                //CCModuleUpdateControlValue(testModule->module, idx, (SVControlPropertyValue){cVControlPropertyType_Number, 0, int(testModule->paramValues[idx]), testModule->paramValues[idx]});
                
                CCModuleSetValue(testModule->module, CCJsonPtrAppend(testModule->jsonPtrs[idx], kVControlProperty_ValueNumber), testModule->paramValues[idx]);
            }
        }
    }
    
    void paint (Graphics &g)
    {
        g.fillAll(Colours::white);
    }
    
    void update()
    {
        for (int i = 0; i < sliders.size(); i++)
        {
            String type = testModule->paramType[i];
            if (type == kVControlParameterTypeContinuous ||
                type == kVControlParameterTypeIndexed)
            {
                sliders[i]->setValue(testModule->paramValues[i], dontSendNotification);
            }
            if (type == kVControlParameterTypeBoolean)
            {
                buttons[i]->setToggleState(testModule->paramValues[i] ? true : false, dontSendNotification);
            }
        }
        
    }
    
    void timerCallback()
    {
        const char* txt = CCModuleGetAsJson(testModule->module);
        if (txt)
        {
            String str(txt);
            if (str != jsonEditor->getText())
                jsonEditor->setText(str);
            free((void*)txt);
        }
    }
    
    OwnedArray<Slider> sliders;
    OwnedArray<Button> buttons;
    OwnedArray<Button> toggles;
    ScopedPointer<TextEditor> jsonEditor;
    TestModule* testModule;
};

TestModuleGUI::TestModuleGUI(TestModule* testModule_, String name)
  : DocumentWindow(name, Colours::white, DocumentWindow::allButtons, true ),
    testModule(testModule_)
{
    static int x = 40;
    static int y = 80;
    
    setUsingNativeTitleBar(true);
    setContentOwned(content = new TestModuleContent(testModule), true);
    setBounds(x, y, 400, 400);
    setVisible (true);
    setResizable(true, true);
    toFront(false);
    
    x += 40;
    y += 40;
    
    PropertiesFile* props = appProperties->getUserSettings();
    String windowPos = props->getValue("windowPosition-" + name);
    if (windowPos.isNotEmpty())
        restoreWindowStateFromString(windowPos);
}

TestModuleGUI::~TestModuleGUI()
{
    PropertiesFile* props = appProperties->getUserSettings();
    props->setValue("windowPosition-" + getName(), getWindowStateAsString());
}

void TestModuleGUI::update()
{
    content->update();
}

void TestModuleGUI::closeButtonPressed()
{
    JUCEApplication::quit();
}
