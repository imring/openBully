#include "HUDInventory.h"
#include "ModelInfo.h"
#include "ModelIndices.h"
#include "TxdStore.h"
#include "Hud.h"

int *HUDInventory::ms_HUDItemTextures = reinterpret_cast<int *>(0xCEFF20);
int *HUDInventory::ms_HUDWeaponTextures = reinterpret_cast<int *>(0xCEFCC8);

// HUDComponent
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

// HUDInventory
// 5412B0
HUDInventory::HUDInventory() {
    HUDComponent::Reset();
    SetVisibility(true);
    dword98 = 0;
    dword9C = 0;
    dwordA0 = 0;
}

// 540E20
void HUDInventory::Reset() {
    HUDComponent::Reset();
    SetVisibility(true);
    dword98 = 0;
    dword9C = 0;
    dwordA0 = 0;
}

// 541960
void HUDInventory::Draw() {
    XCALL(0x541960)
}

void HUDInventory::Initialize() {
    CTxdStore::PushCurrentTxd();
    int txdSlot = CTxdStore::FindTxdSlot("HUDElems", false);
    CTxdStore::SetCurrentTxd(txdSlot);

    for (int i = MI_FIRSTOBJECT; i <= MI_LASTOBJECT; i++) {
        char const *str = *(char const **)(CModelInfo::Butes(i) + 0x4C);
        if (str)
            HUDInventory::ms_HUDItemTextures[i] = MadNoRwTextureRead(str, nullptr);
    }

    for (int i = MI_FIRSTWEAPON; i <= MI_LASTWEAPON; i++) {
        char const *str = *(char const **)(CModelInfo::Butes(i) + 0x4C);
        if (str)
            HUDInventory::ms_HUDWeaponTextures[i] = MadNoRwTextureRead(str, nullptr);
    }

    CTxdStore::PopCurrentTxd();
}

bool HUDInventory::DoesItemTextureExist(int itemId) {
    if (itemId < MI_FIRSTOBJECT || itemId > MI_LASTOBJECT)
        return false;

    return HUDInventory::ms_HUDItemTextures[itemId - MI_FIRSTOBJECT] != 0;
}

bool HUDInventory::DoesWeaponTextureExist(int itemId) {
    if (itemId < MI_FIRSTWEAPON || itemId > MI_LASTWEAPON)
        return false;

    return HUDInventory::ms_HUDWeaponTextures[itemId - MI_FIRSTWEAPON] != 0;
}

// 5413C0
void HUDInventory::DrawInventoryIcon(int32 idx, CInventoryChange &comp) {
    XCALL(0x5413C0);
}