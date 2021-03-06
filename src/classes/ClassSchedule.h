#pragma once
#include <cstdint>
#include "GlobalButes.h"

#define NUM_PERIODS 12

class CClassSchedule {
	TimePeriodButes *m_pPeriods[NUM_PERIODS];
	char m_pad[32]; //unused
	int32_t m_curIndex; 
	int32_t m_count; 
	int32_t m_field_58;
public:
	CClassSchedule(void);
	~CClassSchedule();

	void Init(void);
	void Reset(void);
	void Term(void);
	void Update(void);
	void ChangePeriod(void);
	int32_t FindPeriod(char const *str);
	int32_t GetTimePeriodType(int32_t perIdx);
	int32_t GetNextWarningPeriod(void);
	int32_t GetTotalPeriodMinutes(int32_t perIdx);
	int32_t GetMinutesLeftInPeriod(int32_t perIdx);
	bool IsPeriodActive(int32_t perIdx, int32_t m1, int32_t m2);
	bool IsActivePeriod(int32_t perIdx);
	bool IsTimeInRange(int32_t t1, int32_t t2);
	bool AfterCurfew(void);
	bool IsDay(int32_t t, bool flag);
	void GetPeriodStartTime(int32_t perIdx, int32_t &h, int32_t &m);
	void GetPeriodEndTime(int32_t perIdx, int32_t &h, int32_t &m);
	char const *GetPeriodText(int32_t perIdx);

	static void InjectHooks(void);
};

extern CClassSchedule *g_ClassSchedule;

void ClassScheduleCB(void *p);