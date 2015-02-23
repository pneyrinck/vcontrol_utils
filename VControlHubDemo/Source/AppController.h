/*
  ==============================================================================

    AppController.h
    Created: 23 Feb 2015 9:00:18am
    Author:  Roland Rabien

  ==============================================================================
*/

#ifndef APPCONTROLLER_H_INCLUDED
#define APPCONTROLLER_H_INCLUDED

#include "vcmodule_hub.h"

#include "../JuceLibraryCode/JuceHeader.h"

#include "AppComponent.h"

class AppController : public vcmodule_hub_delegate
{
public:
    AppController(AppComponent* appConponent);
    ~AppController();
    
private:
    virtual void moduleAdded(vcmodule_model*) override;
    virtual void moduleRemoved(vcmodule_model*) override;
    virtual void moduleUpdateControlTouch(vcmodule_model*, const char * id, int value) override;
    virtual void moduleUpdateControlValue(vcmodule_model*, const char * id, float floatValue, const char * name) override;
    virtual void moduleUpdateProperty(vcmodule_model* module, const char *propertyName, SVControlPropertyValue* value) override;
    virtual void moduleUpdateControlProperty(vcmodule_model* module, const char * id, const char *propertyName, SVControlPropertyValue* value) override;
    
    AppComponent* appComponent;
    
    ScopedPointer<vcmodule_hub> hub;
};

#endif  // APPCONTROLLER_H_INCLUDED
