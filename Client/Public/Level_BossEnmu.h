#pragma once

#include "Client_Defines.h"
#include "Level.h"
#include "GameObject.h"
#include "Camera_Dynamic.h"
#include "Characters.h"

BEGIN(Client)

class CLevel_BossEnmu final : public CLevel
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
	CLevel_BossEnmu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_BossEnmu() = default;



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
	HRESULT	Create_Wind(_float fTimeDelta);
	CRITICAL_SECTION Get_CriticalSection() {
		return m_CriticalSection;
	}
	void Set_Finished() { m_isFinished = true; }
	_bool Get_Finished() const {
		return m_isFinished;
	}
	void Set_Renderer(CRenderer* pRenderer) { m_pRendererCom = pRenderer; }
	CRenderer* Get_Renderer() { return m_pRendererCom; }

private:
	CCamera_Dynamic::CAMERADESC_DERIVED			CameraDesc;

	CCharacters*  m_pPlayer = nullptr;
	CCharacters*  m_pEnmu = nullptr;
	CRenderer*	m_pRendererCom = nullptr;

	_bool		m_bCreateUI = false;

	_float      m_fNextLevelTime = 0.f;
private:
	HANDLE					m_hThread = 0;
	CRITICAL_SECTION		m_CriticalSection;
	_bool					m_isFinished = false;
	_bool					m_bTread = false;
	_bool					m_bCinema = false;
	_bool					m_bCinema2 = false;
	_float					m_fTime = 0.f;

	_bool					m_bEffect = false;
	_float					m_fEffectTime = 1.f;
public:
	static CLevel_BossEnmu* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

END
