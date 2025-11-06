#pragma once
#include "Type.h"

#define CLASS_IDENTIFICATION(inCode, inClass)\
enum {kClassId = inCode}; \
virtual uint32 GetClassId() const {return kClassId;} \
static std::shared_ptr<GameObject> CreateInstance() {return std::make_shared<inClass>(); }