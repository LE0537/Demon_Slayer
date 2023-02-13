#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CRankEff final : public CUI
{
private:
	CRankEff(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRankEff(const CRankEff& rhs);
	virtual ~CRankEff() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* ���̴� ���������� ���� �����Ѵ�. */

private:
	_uint					m_iRankScore = 0;
	_float					m_fFadeTime = 1.f;
	_bool					m_bStartCheck = false;
public:
	static CRankEff* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END