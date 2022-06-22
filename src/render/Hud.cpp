#include "hook.h"
#include "Hud.h"

bool CHud::TimeShouldStop(void) {
	XCALL(0x513FE0);
}

void CHud::_SetAlpha(int32 val) {}