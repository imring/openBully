#pragma once
#include "common.h"

struct BinArchive;

class IArchivableBin {
public:
    virtual ~IArchivableBin() = default;
    virtual void ReadFromArchive(BinArchive *archive) = 0;
    virtual void WriteToArchive(BinArchive *archive) = 0;
};

class HUDComponent : public IArchivableBin {
protected:
    uint32 dword4;
    bool m_bVisibility;
    uint32 dwordC;
    uint32 dword10;
    uint8 byte14;
    uint8 byte15;
    uint32 dword18;
    float float1C;

public:
    HUDComponent();

    // 53CF30
    ~HUDComponent() override = default;
    void ReadFromArchive(BinArchive *archive) override;
    void WriteToArchive(BinArchive *archive) override;

    virtual bool IsType(int16 type);
    virtual bool GetVisibility();
    virtual void SetVisibility(bool val);
    virtual void _fB18(uint32 arg0);
    virtual void Reset();
    virtual void Draw() = 0;
    virtual void _fB24();
    virtual void _fB28();
    virtual void OnOpen();
    virtual void OnClose();

    static int16 GetHUDComponent();
};