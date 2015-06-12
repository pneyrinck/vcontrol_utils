//
//  TestModule.cpp
//
//  Created by Paul Neyrinck on 1/10/15.
//
//

#include "TestModule.h"

void TestModule::SetControl(struct VControlModule* module, void *context, unsigned int index, const char* id, float floatValue, const char *stringValue)
{
    if (context) ((TestModule*)context)->setControl(index, floatValue, stringValue);
}


void TestModule::RecvValue(struct VControlModule* module, void *context, unsigned int index, const char* id, const char* bsonData, int bsonDataSize)
{
    if (context) ((TestModule*)context)->recvValue(index, bsonData, bsonDataSize);
}
