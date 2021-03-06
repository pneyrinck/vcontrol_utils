/*
  ==============================================================================

    AppController.cpp
    Created: 23 Feb 2015 9:00:18am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "AppController.h"

static String valueToString(SVControlPropertyValue* value)
{
    switch (value->type)
    {
        case cVControlPropertyType_NULL:
            return "NULL";
        case cVControlPropertyType_Number:
            if (value->valueint == value->valuedouble)
                return String(value->valueint);
            else
                return String(value->valuedouble);
        case cVControlPropertyType_String:
            return String::fromUTF8(value->valuestring);
        case cVControlPropertyType_Array:
            return String();
        case cVControlPropertyType_Object:
            return String();
        default:
            return String();
    }
}

AppController::AppController(AppComponent* appConponent_)
  : appComponent(appConponent_)
{
    hub = new vcmodule_hub();
    hub->setDelegate(this);
    
    appComponent->appendText("Hub Created");
}

AppController::~AppController()
{
    hub->setDelegate(nullptr);
}

void AppController::moduleAdded(vcmodule_model* module)
{
    appComponent->appendText("Module Added: " + module->getName());
    
    modules.add(module);
    
    ModuleComponent* comp = new ModuleComponent(hub, module);
    moduleComponents.add(comp);
    
    appComponent->tabs->addTab(module->getName(), Colours::transparentWhite, comp, true);
}

void AppController::moduleRemoved(vcmodule_model* module)
{
    appComponent->appendText("Module Removed: " + module->getName());
    
    int idx = modules.indexOf(module);
    if (idx >= 0)
    {
        modules.remove(idx);
        moduleComponents.remove(idx);
        appComponent->tabs->removeTab(idx);
    }
}

void AppController::moduleUpdateControlTouch(vcmodule_model* module, const char * control_id, int value)
{
    appComponent->appendText("Module Update Control Touch: " + module->getName() + " [" + String::fromUTF8(control_id) + "]: " + String(value));
}

void AppController::moduleUpdateControlValue(vcmodule_model* module, const char * control_id, float floatValue, const char * name)
{
    appComponent->appendText("Module Update Control Value: " + module->getName() + " [" + String::fromUTF8(control_id) + "] " + String(floatValue));

    for (int i = 0; i < moduleComponents.size(); i++)
    {
        if (moduleComponents[i]->hasModule(module))
            moduleComponents[i]->updateControlValue(control_id, floatValue);
    }
}

void AppController::moduleUpdateProperty(vcmodule_model* module, const char * propertyName, SVControlPropertyValue* value)
{
    appComponent->appendText("Module Update Property: " + module->getName() + " [" + String::fromUTF8(propertyName) + "]: " + valueToString(value));
}

void AppController::moduleUpdateControlProperty(vcmodule_model* module, const char * control_id, const char * propertyName, SVControlPropertyValue* value)
{
    appComponent->appendText("Module Update Control Proprty: " + module->getName() + " [" + String::fromUTF8(control_id) + "] " + String::fromUTF8(propertyName) + ": " + valueToString(value));
}
