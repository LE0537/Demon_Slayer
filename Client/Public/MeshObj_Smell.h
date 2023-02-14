#pragma once
#include "GameObj.h"

BEGIN(Engine)
class CModel;

END

BEGIN(Client)

class CMeshObj_Smell final : public CGameObj
{
public:
	typedef struct tagMeshObj_SmellDesc
	{
		_float4x4	matWorld;
		_float		fGlowPower;
		_uint		iCurrentLevel;

		_uint		iModelIndex;

		_bool		bAlphaBlend;

		CMeshObj_Smell*		pMe = nullptr;

	}MESHOBJ_SMELLDESC;

private:
	CMeshObj_Smell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMeshObj_Smell(const CMeshObj_Smell& rhs);
	virtual ~CMeshObj_Smell() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:
	CModel*		m_pModelCom = nullptr;

private:
	MESHOBJ_SMELLDESC			m_tMyDesc;

private:
	_float		m_fTimeMul = 0.f;
	_float		m_fAliveTime = 0.f;

	CTexture*	m_pNoiseTextureCom = nullptr;

private:
	HRESULT Ready_Components();
	HRESULT SetUp_ShaderResources(); /* 셰이더 전역변수에 값을 전달한다. */
	
public:
	static CMeshObj_Smell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END