#pragma once
#include "Client_Defines.h"
#include "GameObj.h"
#include "Characters.h"

BEGIN(Client)

class CUI : public CGameObj 
{
public:
	typedef struct tagLoadUIInfo
	{
		_bool			bReversal;
		_uint			iTextureNum;
		_float2			vPos;
		_float2			vScale;
		_float			vRot;
	}LOADUIINFO;

	typedef struct tagThrowUIInfo
	{
		_bool			bReversal;
		_bool			bSelCheck;
		_uint			iTextureNum;
		_uint			iLayerNum;
		_float2			vPos;
		_float2			vScale;
		_float			vRot;
		_uint			iLevelIndex;
		CCharacters*	pTarget;
		CUI*			pUITarget;
	}THROWUIINFO;

protected:
	CUI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CUI(const CUI& rhs);
	virtual ~CUI() = default;

public:
	_float Get_fX() { return m_fX; }
	_float Get_fY() { return m_fY; }
	_uint  Get_ImgNum() { return m_iImgNum; }
	//_bool Get_SelectCheck() { return m_bSelectCheck; }
	_bool Get_SelectVS() { return m_bSelectVS; }

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();


protected:
	_float					m_fX, m_fY, m_fSizeX, m_fSizeY;
	_float4x4				m_ViewMatrix, m_ProjMatrix;
	_bool					m_bSelectFirst = false;
	_bool					m_bSelectSecond = false;
	_bool					m_bSelComplete = false;
	_bool					m_bSelectVS = false;
	_uint					m_iImgNum = 0;
	LOADUIINFO				m_LoadUIinfo;
	THROWUIINFO				m_ThrowUIinfo;

public:
	static CUI* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END
