/*
  ==============================================================================

    AppController.cpp
    Created: 23 Feb 2015 9:00:18am
    Author:  Roland Rabien

  ==============================================================================
*/

#include "AppController.h"

AppController::AppController(AppComponent* appConponent_)
  : appComponent(appConponent_)
{
    hub = new vcmodule_hub();
    hub->setDelegate(this);
}

AppController::~AppController()
{
}

void AppController::moduleAdded(vcmodule_model*)
{
}

void AppController::moduleRemoved(vcmodule_model*)
{
}

void AppController::moduleUpdateControlTouch(vcmodule_model*, const char * id, int value)
{
}

void AppController::moduleUpdateControlValue(vcmodule_model*, const char * id, float floatValue, const char * name)
{
}

void AppController::moduleUpdateProperty(vcmodule_model* module, const char *propertyName, SVControlPropertyValue* value)
{
}

void AppController::moduleUpdateControlProperty(vcmodule_model* module, const char * id, const char *propertyName, SVControlPropertyValue* value)
{
}
