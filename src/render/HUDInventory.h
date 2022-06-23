#pragma once
#include "hook.h"
#include "common.h"
#include "HUDComponent.h"
#include "GlobalButes.h"

#define HUDITEMTEXTURES 80
#define HUDWEAPONTEXTURES 150

class HUDInventory : public HUDComponent {
public:
    class CInventoryChange {
    public:
        uint32 dword0;
        int32 m_nModel;
        ObjectButes *m_pButes;
        int32 m_nPrev;
        int32 m_nCurrent;
        float float14; // timer?
    };

protected:
    CInventoryChange m_changes[5];
    int32 m_nChanges;
    int32 dword9C; // last changed?
    int32 dwordA0; // index for drawing the change?

public:
    static int32 (&ms_HUDItemTextures)[HUDITEMTEXTURES];
    static int32 (&ms_HUDWeaponTextures)[HUDWEAPONTEXTURES];

    HUDInventory();

    // 0x5412F0
    ~HUDInventory() override = default;
    void Reset() override;
    void Draw() override;

    void Initialize();
    void AddInventoryChange(int32 model, int32 prev, int32 current);
    void TimeoutUnwantedDuplicates(int32 model);
    void RegisterInventoryChange(int32 model, int32 prev, int32 current, bool force);
    void DrawInventoryIcon(int32 index, CInventoryChange &comp);

    static bool DoesItemTextureExist(int32 itemId);
    static bool DoesWeaponTextureExist(int32 itemId);
};