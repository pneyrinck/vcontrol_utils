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
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_DefaultValue), 0.5);
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 1");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);

        jsonPtr = VControlModuleAddControl(module, "param2", kVControlParameterTypeBoolean);
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 2");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param3", kVControlParameterTypeIndexed);
        VControlModuleSetValue(module, kVControlProperty_NumberOfSteps, 5);
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 3");
        paramValues.add(0);
        paramMax.add(4);
        paramType.add(kVControlParameterTypeIndexed);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param4", kVControlParameterTypeMomentary);
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 4");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeMomentary);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param5", kVControlParameterTypeBoolean);
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 5");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeBoolean);
        jsonPtrs.push_back(jsonPtr);
        
        jsonPtr = VControlModuleAddControl(module, "param6", kVControlParameterTypeContinuous);
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 6");
        paramValues.add(0);
        paramMax.add(1);
        paramType.add(kVControlParameterTypeContinuous);
        jsonPtrs.push_back(jsonPtr);

        jsonPtr = VControlModuleAddControl(module, "param7", kVControlParameterTypeContinuous);
        VControlModuleSetValue(module, VControlJsonPtrAppend(jsonPtr, kVControlProperty_Name), "Parameter 7");
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
        VControlModuleSetValue(module, jsonPtr_, jsonData, jsonDataSize);
        
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
