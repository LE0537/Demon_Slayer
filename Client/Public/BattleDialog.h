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

	_bool Get_DialogStart() { return m_bStartDialog; }
	void Set_DialogStart(_bool _bCheck) { m_bStartDialog = _bCheck; }
	
	_bool Get_DialogEnd() { return m_bEndDialog; }
	void Set_DialogEnd(_bool _bCheck) { m_bEndDialog = _bCheck; }

	_bool Get_SplDialogStart() { return m_bSplDialogStart; }
	void Set_SplDialogStart(_bool _bCheck) { m_bSplDialogStart = _bCheck; }

	void Set_SplCharNum(_uint _CharNum) { m_iCharNum = _CharNum; }
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
	void Battle_EnmuDialog();
	void Battle_BossEnmuDialog();
	void Battle_Akaza();
	void Spl_Dialog();
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
	_float					m_fDelay = 0.f;
	_float					m_fDelay2 = 0.f;
	_bool					m_bStartDialog = false;
	_bool					m_bEndDialog = false;
	_bool					m_bSplDialogStart = false;
	_uint					m_iCharNum = 0;
	_bool					m_bResetCheck = false;
public:
	static CBattleDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
