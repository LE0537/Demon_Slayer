#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "Camera_Dynamic.h"
#include "Level_GamePlay.h"
#include "MeshObj_Smell_Inst.h"
BEGIN(Client)

class CLevel_AdvRui final : public CLevel
{
private:
	CLevel_AdvRui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_AdvRui() = default;

public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);

	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_InteractionUI(const _tchar* pLayerTag);
	HRESULT Load_StaticObjects(char* pFileName);
	HRESULT Load_Map(const _tchar* pLayerTag, char* pFileName);
	HRESULT Load_Weed(char* pFileName);
	HRESULT Load_Smell_1(char* pFileName);
	HRESULT Load_Smell_2(char* pFileName);
	HRESULT Load_Smell_3(char* pFileName);
	HRESULT Load_Smell_4(char* pFileName);
	HRESULT	Check_Smell();

private:
	_bool			 m_bCreateUI = false;

	CRenderer*		m_pRendererCom = nullptr;

	_int			m_iQuestIndex = 0;
	_bool           m_bQuest[3] = { false };
	_bool			m_bRescue[2] = { false };

private:
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;
	CLevel_GamePlay::CHARACTERDESC	      tCharacterDesc;
	CMeshObj_Smell_Inst::MESHOBJ_SMELL_INSTANCING_DESC tSmellDesc[4][3];
	CCharacters*  m_pPlayer = nullptr;
public:
	static CLevel_AdvRui* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

