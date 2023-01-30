#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameObject.h"
#include "Camera_Dynamic.h"
#include "Characters.h"

BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
public:
	typedef struct tagCharacterDesc
	{
		_int		i1P2P;

		_float4x4		matWorld;
		_uint			iNaviIndex;
		_bool			bSub;
		CCharacters*    pSubChar;
	}CHARACTERDESC;
private:
	CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_GamePlay() = default;



public:
	virtual HRESULT Initialize();
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Player(const _tchar* pLayerTag);
	HRESULT Ready_Layer_BackGround(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Effect(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Camera(const _tchar* pLayerTag);
	HRESULT Ready_Layer_Monster(const _tchar* pLayerTag);

	HRESULT Load_StaticObjects(char* pFileName);
	HRESULT Load_Map(const _tchar* pLayerTag, char* pFileName);

private:
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;

	CGameObject* m_pPlayer = nullptr;

	_bool		m_bCreateUI = false;
public:
	static CLevel_GamePlay* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END

