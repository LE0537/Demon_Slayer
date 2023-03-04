#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CBattleDialog final : public CUI
{
private:
	CBattleDialog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CBattleDialog(const CBattleDialog& rhs);
	virtual ~CBattleDialog() = default;

public:
	_bool Get_DialogSwitch() { return m_bDialogSwitch; }
	void Set_DialogOn() { m_bDialogSwitch = true; }
	void Set_DialogOff() { m_bDialogSwitch = false; }

	_bool Get_Dialog3() { return m_bDialog3Start; }
	void Set_Dialog3On() { m_bDialog3Start = true; }
	void Set_Dialog3Off() { m_bDialog3Start = false; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Battle_RuiDialog();
	void Battle_RuiDialog2();
	void Battle_RuiDialog3();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_bool					m_bIsPlaying = false;
	_bool					m_bSoundCheck = false;
	_bool					m_bDialogSwitch = false;
	_float					m_fFontFade = 0.f;
	_bool					m_bFontFadeCheck = false;
	_float					m_g_fDialogStartTime = 0.f;
	_bool					m_bDialogCheck = false;
	_bool					m_bSoundValumCheck = 0.f;
	_bool					m_bDialog3Start = false;

public:
	static CBattleDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
