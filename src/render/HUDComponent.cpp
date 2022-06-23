#include "Hud.h"
#include "HUDComponent.h"

// 53CDC0
HUDComponent::HUDComponent() {
    float1C = 1.f;
    m_bVisibility = false;
    byte14 = 0;
    byte15 = 0;
    dword4 = 1;
    dwordC = 0;
    dword10 = 0;
    dword18 = 0;
}

// 8A20A0
void HUDComponent::ReadFromArchive(BinArchive *archive) {}
// 8A20A0
void HUDComponent::WriteToArchive(BinArchive *archive) {}

// 539090
bool HUDComponent::IsType(int16 type) {
    return GetHUDComponent() == type;
}

// 5390D0
bool HUDComponent::GetVisibility() {
    return m_bVisibility;
}

// 53CEB0
void HUDComponent::SetVisibility(bool val) {
    bool prev = m_bVisibility;
    m_bVisibility = val;

    if (val) {
        if (!prev) {
            CHud::_SetAlpha(0);
            OnOpen();
        }
    } else {
        if (prev)
            OnClose();
    }
    dword4 = 1;
    if (!val)
        byte14 = 0;
}

// 5390E0
void HUDComponent::_fB18(uint32 arg0) {
    dword4 = arg0;
    m_bVisibility = true;
}

// 53CF00
void HUDComponent::Reset() {
    SetVisibility(false);
    dword4 = 1;
    byte14 = 0;
    byte15 = 0;
    dword18 = 0;
}

// 5FC690
void HUDComponent::_fB24() {
    dword4 = 1;
}

// 5FC690
void HUDComponent::_fB28() {
    dword4 = 1;
}

// 49A650
void HUDComponent::OnOpen() {}
// 49A650
void HUDComponent::OnClose() {}

int16 HUDComponent::GetHUDComponent() {
    static int16 s_id = g_FakeRTTI_ID++;
    return s_id;
}