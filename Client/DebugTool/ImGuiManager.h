#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Renderer.h"
#include "Characters.h"
#include "Animation.h"

BEGIN(Engine)
class CGraphic_Device;
class CRenderer;
class CComponent;
END

BEGIN(Client)
class CImGuiManager : public CBase
{
	DECLARE_SINGLETON(CImGuiManager)
private:
	enum	CAMTYPE { CAM_EYE, CAM_AT, CAM_END };

public:
	CImGuiManager();
	virtual ~CImGuiManager() = default;
	
public:
	HRESULT		Setting_PostProcessingValue(_int iValueType, _float fValue)
	{
		if (CRenderer::VALUE_END < iValueType ||
			0 > iValueType)
			return E_FAIL;

		m_fPostProcessingValue[iValueType] = fValue;

		return S_OK;
	}
	void		Play_TestScene() { m_bCutScene = true; }

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void		Tick(_float fTimeDelta);
	void		ShowGui(_float fTimeDelta);
	void		Render();



private:
	void		PostProcessing(_float fTimeDelta);

private:/* For.CamAction */
	void		Camera_Action(_float fTimeDelta);
	void		Sort_CamNodes(CAMTYPE eCamType);
	void		Sort_CamNodes(_int iIndex, CAMTYPE eCamType);

	_bool			Window_LoadCams(_bool* bOpen);
	_bool			Window_SaveCams(_bool* bOpen);
	void			Read_CamActionFiles();


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	_bool					m_bImguiEnable = true;

	CRenderer*			m_pRendererCom = nullptr;

	_int				m_iLevel = LEVEL_STATIC;

private:/* For.PostProcessing */
	_float				m_fPostProcessingValue[CRenderer::VALUE_END] = {1.f, };

private:/*For.CamAction*/
	CGameObject*				m_pCamera = nullptr;
		
	_uint						m_iNumCamFiles = 0;
	char*						m_strCamFiles[250];
	_int						m_iNumCam[CAM_END] = { 0, };

	vector<_float4>				m_vecCam[CAM_END];
	vector<class CCamLine*>		m_vecCamObjects[CAM_END];

	vector<_float>				m_vecCamTime;
	_int						m_iNumCamTime = 0;



	////////////// Minjun
public:
	void		AnimationDebug(_float fTimeDelta);
	void		Add_LiveCharacter(CCharacters* pCharacter);
	void		LiveCharacterList();
	void		CharacterAnimationList(_uint _iIndex);
	void		Clear_CharacterList();
	void		Clear_AnimationList();
	void		Character_Compare_Duration(_uint _iIndex);
	void		Character_Set_Duration(_uint _iIndex);
	void		Set_CurrentPosition(_float3 vPosition) { m_vCurrentPosition = vPosition; };
	void		Character_Compare_Frame(_uint _iIndex);
	void		Character_Set_Frame(_uint _iIndex);
	void		Set_Distance(_float fDist) { m_fEnmuBossDist = fDist; }
	// EnmuBossLevel

	void		EnmuBossAnimDebug();
	void		EnmuBossCharacterList(_uint _iIndex);

private:
	vector<CCharacters*> m_vecObjList;
	vector<CAnimation*> m_vecAnimation;
	vector<_uint> m_vecAnimIndex;
	_float2 m_fMotionBlur;

	int m_iFrame = 100;
	int m_iCurrentFrame = 0;
	int m_iStartFrame = 0;
	int m_iEndFrame = m_iFrame;
	_float m_fFrameTime = 1.f;
	_float m_fSettingFrameTime = 0.f;
	int m_iUsingStartFrame = 0;
	int m_iUsingEndFrame = m_iFrame;
	_float m_fDuration = 0.f;
	_float m_fCurrentDuration = 0.f;
	_float m_fEnmuBossDist = 0.f;
	_bool m_bReset = false;
	_float3 m_vCurrentPosition;

private:
	_bool	m_bCutScene = false;
public:
	virtual void Free() override;
};

END