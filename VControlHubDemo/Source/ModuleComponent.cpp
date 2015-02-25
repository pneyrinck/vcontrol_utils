/*
  ==============================================================================

    ModuleComponent.cpp
    Created: 25 Feb 2015 8:09:35am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "ModuleComponent.h"

ModuleComponent::ModuleComponent(vcmodule_hub* hub_, vcmodule_model* module_)
  : hub(hub_),
    rootModule(module_)
{
    updateComponents(nullptr);
    startTimer(100);
}

ModuleComponent::~ModuleComponent()
{
}


void ModuleComponent::timerCallback()
{
    updateComponents(nullptr);
    if (params.size() > 0)
        stopTimer();
}

void ModuleComponent::updateComponents(vcmodule_model* module)
{
    if (module == nullptr)
    {
        sliders.clear();
        params.clear();
        subModules.clear();
        
        module = rootModule;
    }
    
    subModules.add(module);
    
    for (int i = 0; i < module->getNumControls(); i++)
    {
        String path = module->getPath() + "/" + module->getControlIdentifier(i);
        params.add(path);
        
        Slider* slider = new Slider(Slider::LinearVertical, Slider::NoTextBox);
        addAndMakeVisible(slider);
        slider->addListener(this);
        slider->setRange(0, 1);
        sliders.add(slider);
        
        slider->setValue(module->getControlValue(i));
    }
    
    for (int i = 0; i < hub->getNumSubmodules(module); i++)
    {
        vcmodule_model* child = hub->getSubmodule(module, i);
        updateComponents(child);
    }
    
    resized();
}

void ModuleComponent::resized()
{
    int num = sliders.size();
    if (num > 0)
    {
        int w   = getWidth() / num;
        for (int i = 0; i < num; i++)
        {
            sliders[i]->setBounds(w * i, 0, w, getHeight());
        }
    }
}

void ModuleComponent::sliderValueChanged(Slider* slider)
{
    int idx = sliders.indexOf(slider);
    if (idx >= 0)
    {
        hub->setControlValue(params[idx].toRawUTF8(), slider->getValue());
    }
}

void ModuleComponent::updateControlValue(String controlId, double floatValue)
{
    int idx = params.indexOf(controlId);
    if (idx >= 0)
    {
        sliders[idx]->setValue(floatValue, dontSendNotification);
    }
}