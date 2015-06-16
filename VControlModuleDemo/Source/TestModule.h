//
//  TestModule.h
//
//  Created by Paul Neyrinck on 1/10/15.
//
//

#ifndef __V_OSC__TestModule__
#define __V_OSC__TestModule__

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
        VControlModuleSetRecvValueCallback(module, TestModule::RecvValue, (void*)this);

        const char* jsonPtr;
        
        jsonPtr = VControlModuleAddControl(module, "param1", kVControlParameterTypeContinuous);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_DefaultValue), 0.5);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 1");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);

        jsonPtr = VControlModuleAddControl(module, "param2", kVControlParameterTypeBoolean);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 2");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param3", kVControlParameterTypeIndexed);
        VControlModuleSendValue(module, kVControlProperty_NumberOfSteps, 5);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 3");
        paramValues.add(0);
        paramMax.add(4);
        paramType.add(kVControlParameterTypeIndexed);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param4", kVControlParameterTypeMomentary);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 4");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeMomentary);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param5", kVControlParameterTypeBoolean);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 5");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param6", kVControlParameterTypeContinuous);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 6");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);

        jsonPtr = VControlModuleAddControl(module, "param7", kVControlParameterTypeContinuous);
        VControlModuleSendValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 7");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);
        
        VControlModulePublish(module);
        
        gui = new TestModuleGUI(this, name);
    }
    virtual ~TestModule(){
        VControlModuleDestroy(module);
    }
    
    void recvValue(const char* jsonPtr, const char* bsonData, int bsonDataSize){
        
        _bson::bsonobj obj(bsonData);
        
        if (obj.hasField(kVControlProperty_ValueNumber))
        {
            int index = 0; // todo: rmr
            _bson::bsonelement element = obj.getField(kVControlProperty_ValueNumber);
            if (element.type() == _bson::NumberDouble)
                paramValues.set(index, element.Double());
        }
        
        // bounce the message - a real module would validate the data here
        VControlModuleSendValue(module, jsonPtr, bsonData, bsonDataSize);
        
        gui->update();
    };
    
    static void RecvValue(struct VControlModule* module, void *context, const char* jsonPtr, const char* bsonData, int bsonDataSize);
    
    VControlModule* module;
    
    ScopedPointer<TestModuleGUI> gui;
    Array<float> paramValues;
    Array<float> paramMax;
    StringArray paramType;
    std::vector<std::string> jsonPtrs;
    
    JUCE_LEAK_DETECTOR(TestModule)
};

#endif /* defined(__V_OSC__TestModule__) */
