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
#include "corecontrol.h"

class TestModule{
public:
    TestModule(const char * identifier, const char * vendor, const char * name, CoreControlModule *parent){
        module = CCModuleCreate(identifier, name, parent);
        CCModuleSetModuleVendor(module, vendor);
        CCModuleSetModuleCategory(module, kVControlCategory_EQ);
        CCModuleSetRecvValueCallback(module, TestModule::RecvValue, (void*)this);

        const char* jsonPtr;
        
        jsonPtr = CCModuleAddControl(module, "param1", kVControlParameterTypeContinuous);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_DefaultValue), 0.5);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 1");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);

        jsonPtr = CCModuleAddControl(module, "param2", kVControlParameterTypeBoolean);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 2");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = CCModuleAddControl(module, "param3", kVControlParameterTypeIndexed);
        CCModuleSetValue(module, kVControlProperty_NumberOfSteps, 5);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 3");
        paramValues.add(0);
        paramMax.add(4);
        paramType.add(kVControlParameterTypeIndexed);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = CCModuleAddControl(module, "param4", kVControlParameterTypeMomentary);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 4");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeMomentary);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = CCModuleAddControl(module, "param5", kVControlParameterTypeBoolean);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 5");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = CCModuleAddControl(module, "param6", kVControlParameterTypeContinuous);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 6");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);

        jsonPtr = CCModuleAddControl(module, "param7", kVControlParameterTypeContinuous);
        CCModuleSetValue(module, CCJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 7");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);
        
        CCModulePublish(module);
        
        gui = new TestModuleGUI(this, name);
    }
    virtual ~TestModule(){
        CCModuleDestroy(module);
    }
    
    void recvValue(const char* jsonPtr_, const char* jsonData, int jsonDataSize){
        
        String jsonPtr(jsonPtr_);
        cJSON* json = cJSON_Parse(jsonData);
        
        if (jsonPtr.endsWith(kVControlProperty_ValueNumber))
        {
            for (int i = 0; i < jsonPtrs.size(); i++)
            {
                if (jsonPtr.startsWith(String(jsonPtrs[i].c_str())))
                {
                    if (json->type == cJSON_Number)
                    {
                        double v = json->valuedouble;
                        paramValues.set(i, v);
                    }
                }
            }
        }
        
        cJSON_Delete(json);
        
        // bounce the message - a real module would validate the data here
        CCModuleSetValue(module, jsonPtr_, jsonData, jsonDataSize);
        
        gui->update();
    };
    
    static void RecvValue(struct CoreControlModule* module, void *context, const char* jsonPtr, const char* bsonData, int bsonDataSize);
    
    CoreControlModule* module;
    
    ScopedPointer<TestModuleGUI> gui;
    Array<float> paramValues;
    Array<float> paramMax;
    StringArray paramType;
    std::vector<std::string> jsonPtrs;
    
    JUCE_LEAK_DETECTOR(TestModule)
};

#endif /* defined(__V_OSC__TestModule__) */
