#pragma once

#include "Client_Defines.h"
#include "Base.h"
#include "Renderer.h"
#include "Characters.h"

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
private:
	vector<CCharacters*> m_vecObjList;

public:
	virtual void Free() override;
};

END