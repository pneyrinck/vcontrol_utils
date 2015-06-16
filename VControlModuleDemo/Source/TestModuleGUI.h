#pragma once

#include "JuceHeader.h"

class TestModule;
class TestModuleContent;

class TestModuleGUI : public DocumentWindow
{
public:
    TestModuleGUI(TestModule* testModule, String name);
    ~TestModuleGUI();
    
    void update();
    void closeButtonPressed() override;
    
private:
    TestModule* testModule;
    TestModuleContent* content;
};