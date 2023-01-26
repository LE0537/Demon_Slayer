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
public:
	CImGuiManager();
	virtual ~CImGuiManager() = default;

public:
	HRESULT		Initialize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);

	void		Tick(_float fTimeDelta);
	void		ShowGui(_float fTimeDelta);
	void		Render();



private:
	void		PostProcessing(_float fTimeDelta);


private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;

	_bool					m_bImguiEnable = true;

	CRenderer*			m_pRendererCom = nullptr;

private:/* For.PostProcessing */
	_float				m_fPostProcessingValue[CRenderer::VALUE_END] = {1.f, };



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

	void		Character_Compare_Frame(_uint _iIndex);
	void		Character_Set_Frame(_uint _iIndex);
private:
	vector<CCharacters*> m_vecObjList;
	vector<CAnimation*> m_vecAnimation;
	vector<_uint> m_vecAnimIndex;

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

public:
	virtual void Free() override;
};

END