#include "SeasonManager.h"

//from android: 0x3260DC
int32_t SeasonManager::m_chapterSeasonTable[NUM_CHAPTERS] = {1, 1, 2, 3, 3, 3, 0};

SeasonManager *g_SeasonManager = reinterpret_cast<SeasonManager*>(0xA147E0);

SeasonManager::SeasonManager(void) { 
	m_nChapter = 0;
	field_4 = -1;
	field_8 = 0;
}

//unused
SeasonManager::~SeasonManager() {
	;
}

void SeasonManager::SetSeasonalPedModels(int32_t) {
	
}

void SeasonManager::SetChapter(uint8_t chapter) {
	
}