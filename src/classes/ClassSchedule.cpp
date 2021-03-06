#include "hook.h"
#include "ClassSchedule.h"
#include "GlobalButes.h"
#include "ActionTree.h"
#include "TimeCycle.h"
#include "Clock.h"
#include "ScriptMgr.h"
#include "LuaScript.h"
#include "Population.h"

CClassSchedule *g_ClassSchedule = reinterpret_cast<CClassSchedule *>(0x20C35E8);

void CClassSchedule::InjectHooks(void) {
	using namespace memory::hook;

	inject_hook(0x6A8AA0, &CClassSchedule::Init);
	inject_hook(0x6A8E40, &CClassSchedule::Term);
	inject_hook(0x6A8B40, &CClassSchedule::Reset);
	inject_hook(0x6A8F70, &CClassSchedule::Update);
	inject_hook(0x6A8B50, &CClassSchedule::IsTimeInRange);
	inject_hook(0x6A8C90, &CClassSchedule::IsDay);
	inject_hook(0x6A8BB0, &CClassSchedule::IsPeriodActive);
	inject_hook(0x6A8D20, &CClassSchedule::AfterCurfew);
	inject_hook(0x6A8DC0, &CClassSchedule::GetPeriodStartTime);
	inject_hook(0x6A8DF0, &CClassSchedule::GetPeriodEndTime);
	inject_hook(0x6A8C30, &CClassSchedule::FindPeriod);
	inject_hook(0x6A8CC0, &CClassSchedule::GetTimePeriodType);
	inject_hook(0x6A8CF0, &CClassSchedule::GetNextWarningPeriod);
	inject_hook(0x6A8D30, &CClassSchedule::GetTotalPeriodMinutes);
	inject_hook(0x6A8D70, &CClassSchedule::GetMinutesLeftInPeriod);
	inject_hook(0x6A8E50, &CClassSchedule::ChangePeriod);

	inject_hook(0x6A8E20, &ClassScheduleCB);
}

CClassSchedule::CClassSchedule(void) {
	m_count = 0;
	Reset();
}

CClassSchedule::~CClassSchedule() {
	; //unused
}

void ClassScheduleCB(void *p) {
	((CClassSchedule *)p)->Term();
	((CClassSchedule *)p)->Init();
}

void CClassSchedule::Term(void) {
	GlobalButes::UnRegisterCB(ClassScheduleCB, this);
}

void CClassSchedule::Init(void) {
	ActionTreeName *nameTC = &GlobalButes::TimeCycle;
	ActionTreeName *nameTP = &GlobalButes::TimePeriods;

	ActionTreeNamePath path = ActionTreeNamePath(2, nameTC);
	m_count = GlobalButes::FindGetCount(path);

	for (int32_t i = 0; i < m_count; i++) 
		m_pPeriods[i] = reinterpret_cast<TimePeriodButes*>(GlobalButes::Find(path, i, GlobalButes::TimePeriodButes));

	GlobalButes::RegisterCB(ClassScheduleCB, this);

	printf("ClassSchedule was initialized!\n");
}

void CClassSchedule::Reset(void) {
	m_curIndex = 0;
	m_field_58 = 0;
}

bool CClassSchedule::IsTimeInRange(int32_t t1, int32_t t2) {
	int32_t curTime = 3600 * Clock::GetHours() + 60 * Clock::GetMinutes() + Clock::GetSeconds();

	return (t1 <= t2) ? curTime >= t1 && curTime < t2 : curTime >= t1 || curTime < t2;
}

inline bool CClassSchedule::IsActivePeriod(int32_t perIdx) {
	return IsTimeInRange(60 * m_pPeriods[perIdx]->startMin + 3600 * m_pPeriods[perIdx]->startHour,
						 60 * m_pPeriods[perIdx]->endMin + 3600 * m_pPeriods[perIdx]->endHour);
}

void CClassSchedule::ChangePeriod(void) {
	char const *name;
	name = m_pPeriods[m_curIndex]->str2;
	LuaScript *pScript = gScriptManager->LoadScript(*(char const **)(TimeCycleButes::s_pTimeCycleButes + 0x1C), true);

	if (name) {
		pScript->ExecuteFunctionCall(name, 0, false);
		gScriptManager->UpdateScript(pScript);
	}

	if (m_count > 0) {
		int32_t idx = 0;
		while ((idx < m_count) && !IsActivePeriod(idx))
			idx++;

		m_curIndex = idx;
	}

	(*(CPopulation **)CPopulation::m_spInstance)->OnChangePeriod();
	name = m_pPeriods[m_curIndex]->str1;
	
	if (name) {
		pScript->ExecuteFunctionCall(name, 0, false);
		gScriptManager->UpdateScript(pScript);
	}
}

void CClassSchedule::Update(void) {
	if (!IsActivePeriod(m_curIndex)) {
		ChangePeriod();

		if (m_curIndex == 0)
			m_field_58++;
	}
}

int32_t CClassSchedule::FindPeriod(char const *str) {
	int32_t idx = 0;
	while (idx < m_count) {
		if (!strcmp(str, m_pPeriods[idx]->text))
			return idx;
		idx++;
	}

	return -1;
}

char const *CClassSchedule::GetPeriodText(int32_t perIdx) {
	return m_pPeriods[perIdx]->text;
}

int32_t CClassSchedule::GetTimePeriodType(int32_t perIdx) {
	if (perIdx < 0 || perIdx >= m_count)
		return m_pPeriods[m_curIndex]->type;
	else
		return m_pPeriods[perIdx]->type;
}

int32_t CClassSchedule::GetNextWarningPeriod(void) {
	int32_t idx = m_curIndex;
	while (idx < m_count) {
		if (m_pPeriods[idx]->type == 1 || m_pPeriods[idx]->type == 3)
			return idx;
		idx++;
	}

	return -1;
}

int32_t CClassSchedule::GetTotalPeriodMinutes(int32_t perIdx) {
	int32_t t1 = m_pPeriods[perIdx]->startMin + 60 * m_pPeriods[perIdx]->startHour;
	int32_t t2 = m_pPeriods[perIdx]->endMin + 60 * m_pPeriods[perIdx]->endHour;

	return t1 > t2 ? t2 - t1 + 1440 : t2 - t1;
}

int32_t CClassSchedule::GetMinutesLeftInPeriod(int32_t perIdx) {
	int32_t t1 = m_pPeriods[perIdx]->endMin + 60 * m_pPeriods[perIdx]->endHour;
	int32_t t2 = Clock::GetMinutes() + 60 * Clock::GetHours();

	return t2 > t1 ? t1 - t2 + 1440 : t1 - t2;
}

bool CClassSchedule::IsDay(int32_t t, bool flag) {
	if (flag)
		return m_field_58 % *(int32_t*)(TimeCycleButes::s_pTimeCycleButes + 0x18) == t;
	else
		return m_field_58 == t;
}

bool CClassSchedule::AfterCurfew(void) {
	return m_pPeriods[m_curIndex]->type == 3;
}

void CClassSchedule::GetPeriodStartTime(int32_t perIdx, int32_t &h, int32_t &m) {
	h = m_pPeriods[perIdx]->startHour;
	m = m_pPeriods[perIdx]->startMin;
}

void CClassSchedule::GetPeriodEndTime(int32_t perIdx, int32_t &h, int32_t &m) {
	h = m_pPeriods[perIdx]->endHour;
	m = m_pPeriods[perIdx]->endMin;
}

bool CClassSchedule::IsPeriodActive(int32_t perIdx, int32_t m1, int32_t m2) {
	if (perIdx < 0 || perIdx >= m_count)
		return false;

	int32_t t1, t2;
	t1 = 60 * m_pPeriods[perIdx]->startMin + 3600 * m_pPeriods[perIdx]->startHour - m1;

	if (m2 == -1)
		t2 = 60 * m_pPeriods[perIdx]->endMin + 3600 * m_pPeriods[perIdx]->endHour;
	else
		t2 = 60 * (m2 + m_pPeriods[perIdx]->startMin) + 3600 * m_pPeriods[perIdx]->startHour;

	return CClassSchedule::IsTimeInRange(t1, t2);
}