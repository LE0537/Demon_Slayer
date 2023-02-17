#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CMsgTextBase final : public CUI
{
private:
	CMsgTextBase(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMsgTextBase(const CMsgTextBase& rhs);
	virtual ~CMsgTextBase() = default;

public:
	_bool Get_MsgOnCheck() { return m_bMsgOnCheck; }

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
	_bool					m_bMsgOnCheck = false;
	_float					m_fFadeTime = 0.f;
	_int					m_iMoveCount = 0;
	_bool					m_bMoveCheck = false;
	_float					m_fFirstMsg = 0.f;
	_bool					m_bFirstMsgCheck = false;

public:
	static CMsgTextBase* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
