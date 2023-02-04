#pragma once

#include "Client_Defines.h"
#include "UI.h"

BEGIN(Client)

class CRoundUI final : public CUI
{
private:
	CRoundUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRoundUI(const CRoundUI& rhs);
	virtual ~CRoundUI() = default;

public:
	_bool Get_StartCheck() { return m_bStartCheck; }
	_bool Get_RoundOnOff() { return m_bRoundOnOff; }
	_bool Get_Draw() { return m_bDraw; }

	void Set_1PRoundEnd(_bool bTrue) { m_b1PRoundEnd = bTrue; }
	void Set_2PRoundEnd(_bool bTrue) { m_b2PRoundEnd = bTrue; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */

public:
	void ZoomIn(_float fTimeDelta);
	void ZoomOut(_float fTimeDelta);
	void Fade_In(_float fTimeDelta);
	void Fade_Out(_float fTimeDelta);
	void Case_ReturnEnd();
	void UI_Function(_float fTimeDelta);
	void ZeroTiemr_RoundUI(_float fTImeDelta);
	void ZeroHp_RoundUI(_float fTimeDelta);
	void Battle_End(_float fTimeDelta);

private:
	CTexture*				m_pTextureCom2 = nullptr;
	CTexture*				m_pTextureCom3 = nullptr;
	CTexture*				m_pTextureCom4 = nullptr;

private:
	_uint					m_iStartEndImgNum = 0;
	_uint					m_iJudgmentImgNum = 0;
	_uint					m_iWinImgNum = 0;
	_uint					m_i1PRoundWin = 0;
	_uint					m_i2PRoundWin = 0;

	_bool					m_bZoomCheck = false;
	_bool					m_bRoundOnOff = true;

	_bool					m_bRoundCheck = true;
	_bool					m_bStartCheck = false;
	_bool					m_bJudgmentCheck = false;
	_bool					m_bWinCheck = false;
	_bool					m_bBattleEndCheck = false;

	_bool					m_bScaleCheck = false;
	_bool					m_bDraw = false;
	_bool					m_b1PRoundEnd = false;
	_bool					m_b2PRoundEnd = false;

	_bool					m_bResultOn = false;

	_float					m_fDelayTime = 0.f;
	_float					m_fStopTime = 0.f;
	_float					m_fFadeTime = 0.f;
	_float					m_fZoomTime = 0.f;
	
	RESULTINFO				m_ResultInfo;

public:
	static CRoundUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END