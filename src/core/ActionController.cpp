#include "ActionController.h"

ActionController::ActionController() {
	XCALL(0x5F3E50);
}

ActionController::~ActionController() {
	XCALL(0x5F40F0); //ActionController::ClearAll called here, not the real address of dtor
}