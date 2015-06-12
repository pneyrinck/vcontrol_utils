//
//  TestModule.h
//
//  Created by Paul Neyrinck on 1/10/15.
//
//

#ifndef __V_OSC__TestModule__
#define __V_OSC__TestModule__

#define VCONTROL_DEPRECATED
#include <iostream>
#include "JuceHeader.h"
#include "TestModuleGUI.h"
#include "vcontrol.h"

class TestModule{
public:
    TestModule(const char * identifier, const char * vendor, const char * name, VControlModule *parent){
        module = VControlModuleCreate(identifier, name, parent);
        VControlModuleSetModuleVendor(module, vendor);
        VControlModuleSetModuleCategory(module, kVControlCategory_EQ);
        VControlModuleSetControlCallback(module,  TestModule::SetControl, (void*)this);
        VControlModuleSetRecvValueCallback(module, TestModule::RecvValue, (void*)this);
        
        int index = 0;
        VControlModuleAddControl(module, index, "param1", kVControlParameterTypeContinuous);
        VControlModuleUpdateControlPropertyValue(module, index, kVControlProperty_DefaultValue, (SVControlPropertyValue){cVControlPropertyType_Number, 0, 0, 0.5});
        VControlModuleUpdateControlPropertyValue(module, index++, kVControlProperty_Name, (SVControlPropertyValue){cVControlPropertyType_String, "Parameter 1"});
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);

        VControlModuleAddControl(module, index, "param2", kVControlParameterTypeBoolean);
        VControlModuleUpdateControlPropertyValue(module, index++, kVControlProperty_Name, (SVControlPropertyValue){cVControlPropertyType_String, "Parameter 2"});
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        
        VControlModuleAddControl(module, index, "param3", kVControlParameterTypeIndexed);
        VControlModuleUpdateControlPropertyValue(module, index, kVControlProperty_NumberOfSteps, (SVControlPropertyValue){cVControlPropertyType_Number, 0, 5, 5});
        VControlModuleUpdateControlPropertyValue(module, index++, kVControlProperty_Name, (SVControlPropertyValue){cVControlPropertyType_String, "Parameter 3"});
        paramValues.add(0);
        paramMax.add(4);
        paramType.add(kVControlParameterTypeIndexed);
        
        VControlModuleAddControl(module, index, "param4", kVControlParameterTypeMomentary);
        VControlModuleUpdateControlPropertyValue(module, index++, kVControlProperty_Name, (SVControlPropertyValue){cVControlPropertyType_String, "Parameter 4"});
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeMomentary);
        
        VControlModuleAddControl(module, index, "param5", kVControlParameterTypeBoolean);
        VControlModuleUpdateControlPropertyValue(module, index++, kVControlProperty_Name, (SVControlPropertyValue){cVControlPropertyType_String, "Parameter 5"});
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        
        VControlModuleAddControl(module, index, "param6", kVControlParameterTypeContinuous);
        VControlModuleUpdateControlPropertyValue(module, index++, kVControlProperty_Name, (SVControlPropertyValue){cVControlPropertyType_String, "Parameter 6"});
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);

        VControlModuleAddControl(module, index, "param7", kVControlParameterTypeContinuous);
        VControlModuleUpdateControlPropertyValue(module, index++, kVControlProperty_Name, (SVControlPropertyValue){cVControlPropertyType_String, "Parameter 7"});
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        
        VControlModulePublish(module);
        
        gui = new TestModuleGUI(this, name);
    }
    virtual ~TestModule(){
        VControlModuleDestroy(module);
    }
    void setControl(unsigned int index, float floatValue, const char *stringValue){
        paramValues.set(index, floatValue);
        VControlModuleUpdateControlValue(module, index, (SVControlPropertyValue){cVControlPropertyType_Number, 0, int(floatValue), floatValue});
        
        VControlModuleSendValue(module, index, kVControlProperty_Position, floatValue);
        
        gui->update();
    };
    
    void recvValue(unsigned int index, const char* bsonData, int bsonDataSize){
        
        _bson::bsonobj obj(bsonData);
        
        if (obj.hasField("position"))
        {
            _bson::bsonelement element = obj.getField("position");
            if (element.type() == _bson::NumberDouble)
                paramValues.set(index, element.Double());
        }
        
        // bounce the message - a real module would validate the data here
        VControlModuleSendValue(module, index, bsonData, bsonDataSize);
        
        gui->update();
    };
    
	void setControlProperty(unsigned int index, const char* propertyName, float floatValue, const char *stringValue){
		if (stringValue)
			VControlModuleUpdateControlPropertyValue(module, index, propertyName, (SVControlPropertyValue){cVControlPropertyType_String, stringValue});
		else
			VControlModuleUpdateControlPropertyValue(module, index, propertyName, (SVControlPropertyValue){cVControlPropertyType_Number, 0, int(floatValue), floatValue});
	}
    static void SetControl(struct VControlModule* module, void *context, unsigned int index, const char* id, float floatValue, const char *stringValue);
    static void RecvValue(struct VControlModule* module, void *context, unsigned int index, const char* id, const char* bsonData, int bsonDataSize);
    
    VControlModule* module;
    
    ScopedPointer<TestModuleGUI> gui;
    Array<float> paramValues;
    Array<float> paramMax;
    StringArray paramType;
    
    JUCE_LEAK_DETECTOR(TestModule)
};

#endif /* defined(__V_OSC__TestModule__) */
