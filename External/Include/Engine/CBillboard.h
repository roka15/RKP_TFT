#pragma once
#include "CRenderComponent.h"
enum class GAUGE_TYPE;
class CBillboard :
    public CRenderComponent
{
private:
    GAUGE_TYPE   m_GaugeType;
public:
    virtual void finaltick() override;
    virtual void render() override;
    virtual void render(UINT _iSubset) override;

    virtual void SaveToLevelFile(FILE* _File) override;
    virtual void LoadFromLevelFile(FILE* _File) override;

    CLONE(CBillboard);
public:
    void SetGaugeOption(GAUGE_TYPE _eType) { m_GaugeType = _eType; }
public:
    CBillboard();
    CBillboard(const CBillboard& _ref);
    ~CBillboard();
};

