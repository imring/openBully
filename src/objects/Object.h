#pragma once
#include "patcher.h"
#include "Physical.h"

class CObject : public CPhysical{
public:
	void operator delete(void* p);
};