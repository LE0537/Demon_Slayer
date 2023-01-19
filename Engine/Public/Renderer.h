#pragma once

/* 화면에 그려져야할 객체들을 모아서 그리는 순서대로 보관한다. 순서대로 렌더함수를 호출해준다. */
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	enum RENDERGROUP {RENDER_PRIORITY,RENDER_SHADOWDEPTH, RENDER_NONALPHABLEND, RENDER_NONLIGHT, RENDER_ALPHABLEND, 
		RENDER_GRAYSCALE, RENDER_BLUR, RENDER_DISTORTION, RENDER_UI,RENDER_UIPOKE ,RENDER_END };
	enum VALUETYPE { VALUE_AO, VALUE_AORADIUS, VALUE_GLOWBLURCOUNT, VALUE_DISTORTION, VALUE_END };
private:
	enum RENDER_ORDER { ORDER_GLOW, ORDER_GRAYSCALE, ORDER_BLUR, ORDER_DISTORTION, ORDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);	
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg)override;

public:
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderGroup_Front(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Render_GameObjects(_bool _bDebug);

public:
	HRESULT Add_Debug(class CComponent* pDebugCom);

public:
	void	Set_Value(VALUETYPE eValueType, _float fValue) 
	{
		if(eValueType >= VALUE_END) 
			return; 
		m_fValue[eValueType] = fValue; 
	}
	void	AO_OnOff(_bool bTrueisOn) { m_bRenderAO = bTrueisOn; }


//	member
private:
	list<class CGameObject*>				m_GameObjects[RENDER_END];
	typedef list<class CGameObject*>		GAMEOBJECTS;

private:
	_bool						m_bRenderDebug = false;
	list<class CComponent*>					m_DebugComponents;

private:
	class CTarget_Manager*					m_pTarget_Manager = nullptr;
	class CLight_Manager*					m_pLight_Manager = nullptr;

	class CVIBuffer_Rect*					m_pVIBuffer = nullptr;
	class CShader*							m_pShader = nullptr;
	_float4x4								m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	_float				m_fFar = 0.f;

private:/* Post Processing */
	_tchar		m_strPPS_MRTName_1[MAX_PATH] = L"";
	_tchar		m_strPPS_MRTName_2[MAX_PATH] = L"";

	_tchar		m_strPPS_RTName_1[MAX_PATH] = L"";
	_tchar		m_strPPS_RTName_2[MAX_PATH] = L"";

private:/* For.Glow*/
	_float						m_fGlowWinCX, m_fGlowWinCY;
	ID3D11DepthStencilView*		m_pGlowDSV = nullptr;

private:/* For.PostProcessing Value */
	_bool		m_bRenderAO = false;
	_float		m_fValue[VALUE_END] = { 1.f,  };



//	Function
private:/* For.Glow*/
	HRESULT Ready_GlowDSV(_float fWinCX, _float fWinCY);
	HRESULT Set_Viewport(_float fWinCX, _float fWinCY);

private:
	HRESULT Render_Priority();
	HRESULT Render_ShadowDepth();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_Lights();
	HRESULT Render_AO();
	HRESULT Render_Blend();
	HRESULT Render_OutLine();
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();

	//	Post Processing Objects Rendering
	HRESULT	Ready_PostProcessing();
	HRESULT Render_LightShaft(const _tchar* pTexName, const _tchar* pMRTName);
	HRESULT Render_Glow(const _tchar* pTexName, const _tchar* pMRTName);
	HRESULT Render_Blur(const _tchar* pTexName, const _tchar* pMRTName);
	HRESULT Render_GrayScale(const _tchar* pTexName, const _tchar* pMRTName);
	HRESULT Render_Distortion(const _tchar* pTexName, const _tchar* pMRTName);
	HRESULT Render_Master(const _tchar* pTexName);
	HRESULT Render_UI();
	HRESULT Render_UIPOKE();

	HRESULT Render_Debug();
public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg = nullptr)override;
	virtual void Free();
};

END