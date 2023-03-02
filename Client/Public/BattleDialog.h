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
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

public:
	void Battle_RuiDialog();
	void Battle_RuiSound();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

private:
	_bool					m_bIsPlaying = false;
	_bool					m_bSoundCheck = false;
	_float					m_fFontFade = 0.f;
	_bool					m_bFontFadeCheck = false;
	_float					m_g_fDialogStartTime = 0.f;
public:
	static CBattleDialog* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
