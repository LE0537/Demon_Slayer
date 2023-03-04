#pragma once
#include "GameObj.h"
#include "Client_Defines.h"
#include "Effect_Mesh.h"
#include "Effect_Particle.h"
#include "Effect_Texture.h"
#include "Effect_Particle_New.h"
#include "Characters.h"

BEGIN(Client)

class CEffect : public CGameObj
{
public:
	typedef struct EffectInfo {
		_uint                              iMoveType;

		_float                              fEffectStartTime;
		_float                              fEffectLifeTime;

		_float3                              vPosition;
		_float3                              vRotation;

		_bool                              bRoof;

	}EFFECT_INFO;

	enum DISAPPEAR { DISAPPEAR_DISJOLVE, DISAPPEAR_ALPHA, DISAPPEAR_NONE, DISAPPEAR_END };
	enum EFFECTSHADER {
		SHADER_BLUR, SHADER_DISTORTION, SHADER_ALPHABLEND, SHADER_ALPHATEST,
		SHADER_GRAYSCALE, SHADER_PRIORITY_TEST, SHADER_FIRSTPROJ, SHADER_PROJECTION, SHADER_END
	};
	enum PARTICLETYPE { PART_SPHERER, PART_CIRCLE1, PART_LINE, PART_CONE, PART_LINE2, PART_END };
	enum EFFMOVE {
		EFFMOVE_NONE, EFFMOVE_PLAYER, EFFMOVE_TARGET, EFFMOVE_STOP, EFFMOVE_ZERO, EFFMOVE_RANDOM,
		EFFMOVE_MATRIX, EFFMOVE_MATRIXPIX, EFFMOVE_MATRIX2, EFFMOVE_POS, EFFMOVE_END
	};

private:
	CEffect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEffect(const CEffect& rhs);
	virtual ~CEffect() = default;

public:
	HRESULT Initialize_Prototype(EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo, _uint Effect_Num
		, vector<CEffect_Mesh::MESH_INFO> MeshInfo, vector<CEffect_Particle::PARTICLE_INFO> ParticleInfo, vector<CEffect_Particle_New::PARTICLE_INFO> NewParticleInfo);
	virtual HRESULT Initialize(void* pArg);
	virtual void Tick(_float fTimeDelta);
	virtual void Late_Tick(_float fTimeDelta);

public:
	_uint Get_EffectMove() { return m_EffectInfo.iMoveType; }
	_float4x4 Get_CombinedWorldMatrix() { return m_CombinedWorldMatrix; }
	void Set_ParentWorldMatrix(_matrix ParentMat);

private:
	HRESULT Ready_Components();
	HRESULT Ready_Parts();

private:
	//char                              m_szEffectName[MAX_PATH];

	_int                                 m_iEffectNum = -1; //º¸·ù

	_float                                 m_fEffectTime = 0.f;
	_float                                 m_fEffectStartTime = 0.f;

	_float4                                 m_vDestination;

	_float4x4                              m_CombinedWorldMatrix;
	_float4x4*                              m_ParentWorldMatrix;

	EFFECT_INFO                              m_EffectInfo;

	_bool                                 m_bStart;
	_bool                                 m_bLateStart = true;
	_bool                                 m_bInvisible = false;

	CGameObj*                              m_pTarget;

	vector<class CEffect_Texture*>               m_Textures;
	vector<class CEffect_Mesh*>                  m_Meshes;
	vector<class CEffect_Particle*>               m_Particle;
	vector<class CEffect_Particle_New*>            m_NewParticle;

	vector<CEffect_Texture::TEXTURE_INFO>         m_TextureInfo;
	vector<CEffect_Mesh::MESH_INFO>               m_MeshInfo;
	vector<CEffect_Particle::PARTICLE_INFO>         m_ParticleInfo;
	vector<CEffect_Particle_New::PARTICLE_INFO>      m_NewParticleInfo;

public:
	static CEffect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint Effect_Num, EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo
		, vector<CEffect_Mesh::MESH_INFO> MeshInfo, vector<CEffect_Particle::PARTICLE_INFO> ParticleInfo, vector<CEffect_Particle_New::PARTICLE_INFO> NewParticleInfo);
	virtual CGameObject* Clone(void* pArg = nullptr);
	virtual void Free() override;
};

END