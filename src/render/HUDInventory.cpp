#include "HUDInventory.h"
#include "ModelInfo.h"
#include "ModelIndices.h"
#include "TxdStore.h"
#include "Hud.h"

int32 (&HUDInventory::ms_HUDItemTextures)[HUDITEMTEXTURES] = memory::read<int32[HUDITEMTEXTURES]>(0xCEFF20);
int32 (&HUDInventory::ms_HUDWeaponTextures)[HUDWEAPONTEXTURES] = memory::read<int32[HUDWEAPONTEXTURES]>(0xCEFCC8);

// 5412B0
HUDInventory::HUDInventory() {
    HUDComponent::Reset();
    SetVisibility(true);
    m_nChanges = 0;
    dword9C = 0;
    dwordA0 = 0;
}

// 540E20
void HUDInventory::Reset() {
    HUDComponent::Reset();
    SetVisibility(true);
    m_nChanges = 0;
    dword9C = 0;
    dwordA0 = 0;
}

// 541960
void HUDInventory::Draw() {
    return memory::call_method<HUDInventory *>(0x541960, this);
}

// 540E50
void HUDInventory::Initialize() {
    CTxdStore::PushCurrentTxd();
    int txdSlot = CTxdStore::FindTxdSlot("HUDElems", false);
    CTxdStore::SetCurrentTxd(txdSlot);

    for (int i = MI_FIRSTOBJECT; i <= MI_LASTOBJECT; i++) {
        char const *str = *(char const **)(CModelInfo::Butes(i) + 0x4C);
        if (str)
            ms_HUDItemTextures[i] = MadNoRwTextureRead(str, nullptr);
    }

    for (int i = MI_FIRSTWEAPON; i <= MI_LASTWEAPON; i++) {
        char const *str = *(char const **)(CModelInfo::Butes(i) + 0x4C);
        if (str)
            ms_HUDWeaponTextures[i] = MadNoRwTextureRead(str, nullptr);
    }

    CTxdStore::PopCurrentTxd();
}

// 540F00
void HUDInventory::AddInventoryChange(int32 model, int32 prev, int32 current) {
    int32 newidx = (dword9C + 1) % 5;
    CInventoryChange &chg = m_changes[newidx];
    chg.float14 = 0.f;
    chg.dword0 = 0;
    chg.m_nModel = model;
    chg.m_nPrev = prev;
    chg.m_nCurrent = current;
    chg.m_pButes = CModelInfo::Butes(model);

    dword9C = newidx;
    if (m_nChanges++ <= 0)
        dwordA0 = newidx;
}

// 5410B0
void HUDInventory::TimeoutUnwantedDuplicates(int32 model) {
    return memory::call_method<HUDInventory *>(0x5410B0, this);
}

// 541110
void HUDInventory::RegisterInventoryChange(int32 model, int32 prev, int32 current, bool force) {
    return memory::call_method<HUDInventory *, void, int32, int32, int32, bool>(0x541110, this, model, prev, current,
                                                                                force);
}

// 5413C0
void HUDInventory::DrawInventoryIcon(int32 idx, CInventoryChange &comp) {
    return memory::call_method<HUDInventory *, void, int32, CInventoryChange &>(0x5413C0, this, idx, comp);
}

// 5411B0
bool HUDInventory::DoesItemTextureExist(int32 itemId) {
    if (itemId < MI_FIRSTOBJECT || itemId > MI_LASTOBJECT)
        return false;

    return ms_HUDItemTextures[itemId - MI_FIRSTOBJECT] != 0;
}

// 5411E0
bool HUDInventory::DoesWeaponTextureExist(int32 itemId) {
    if (itemId < MI_FIRSTWEAPON || itemId > MI_LASTWEAPON)
        return false;

    return ms_HUDWeaponTextures[itemId - MI_FIRSTWEAPON] != 0;
}