#pragma once
#include "GameObj.h"
#include "Client_Defines.h"

BEGIN(Client)

class CEffect_Texture : public CGameObj
{
public:
	typedef struct TextureInfo {
		char							m_szTextureName[MAX_PATH];

		_uint							iTextureType;
		_int							fSortingFudge;

		_float							fLifeTime;
		_float							fStartTime;
		_float							fRotation;

		_float2							vSize[4];

		_float3							vPosition;

		_float4							vColor;

		_bool							bBillboard = true;

	}TEXTURE_INFO;

private:
	CEffect_Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect_Texture(const CEffect_Texture& rhs);
	virtual ~CEffect_Texture() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);
	virtual HRESULT Render();

private:

	TEXTURE_INFO					m_TextureInfo;

	_float							m_fTime = 0.f;

	_float4x4						m_CombinedWorldMatrix;

	CGameObj*						m_pParents = nullptr;

	_bool							m_bDead = false;

private:
	HRESULT SetUp_ShaderResources();
	HRESULT Ready_Components();

public:
	static CEffect_Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END