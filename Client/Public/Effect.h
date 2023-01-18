#pragma once
#include "GameObj.h"
#include "Client_Defines.h"
#include "Effect_Mesh.h"
#include "Effect_Particle.h"
#include "Effect_Texture.h"

BEGIN(Client)

class CEffect : public CGameObj
{
public:
	typedef struct EffectInfo {
		_float										fEffectStartTime;
		_float										fEffectLifeTime;

		_float3										vPosition;
		_float3										vRotation;

	}EFFECT_INFO;

private:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	HRESULT Initialize_Prototype(EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();

private:
	//char										m_szEffectName[MAX_PATH];

	_int										m_iTexNum = -1; //º¸·ù

	_float										m_fEffectTime = 0.f;

	EFFECT_INFO									m_EffectInfo;

	vector<class CEffect_Texture*>				m_Textures;
	vector<class CEffect_Mesh*>					m_Meshes;
	vector<class CEffect_Particle*>				m_Particle;

	vector<CEffect_Texture::TEXTURE_INFO>		m_TextureInfo;

public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END