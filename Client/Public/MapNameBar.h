#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CMapNameBar final : public CUI
{
private:
	CMapNameBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMapNameBar(const CMapNameBar& rhs);
	virtual ~CMapNameBar() = default;

public:
	_bool Get_OffCheck() { return m_bOnCheck; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_float							m_fFadeTime = 0.f;
	_float							m_fStopTime = 0.f;
	_int							m_iMoveCount = 0;
	_bool							m_bFadeCheck = false;
	_bool							m_bOnCheck = false;
	_bool							m_bMsgOnCheck = false;
	_bool							m_bSound = false;
	_bool							m_bSoundEnd = false;
	_bool							m_bSoundCheck = false;
public:
	static CMapNameBar* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
