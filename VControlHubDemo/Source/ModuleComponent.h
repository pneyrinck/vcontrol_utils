/*
  ==============================================================================

    ModuleComponent.h
    Created: 25 Feb 2015 8:09:35am
    Author:  Roland Rabien

  ==============================================================================
*/

#ifndef MODULECOMPONENT_H_INCLUDED
#define MODULECOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

#include "vcmodule_hub.h"

class ModuleComponent : public Component,
                        public Timer,
                        public Slider::Listener
{
public:
    ModuleComponent(vcmodule_hub* hub, vcmodule_model* module);
    ~ModuleComponent();
    
    void updateControlValue(String controlId, double floatValue);
    
    bool hasModule(vcmodule_model* keyModule) { return subModules.contains(keyModule); }

private:
    void timerCallback() override;
    void resized() override;
    void sliderValueChanged(Slider* slider) override;

    void updateComponents(vcmodule_model* module);

    vcmodule_hub* hub;
    vcmodule_model* rootModule;
    
    Array<vcmodule_model*> subModules;
    
    OwnedArray<Slider> sliders;
    StringArray params;
};



#endif  // MODULECOMPONENT_H_INCLUDED
