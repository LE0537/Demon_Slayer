#include "stdafx.h"
#include "Effect.h"
#include "GameInstance.h"
#include "CollBox.h"
#include "Effect_Manager.h"
#include "Characters.h"

CEffect::CEffect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObj(pDevice, pContext)
{
}

CEffect::CEffect(const CEffect & rhs)
	: CGameObj(rhs),
	m_EffectInfo(rhs.m_EffectInfo),
	m_Textures(rhs.m_Textures),
	m_ParticleInfo(rhs.m_ParticleInfo),
	m_Meshes(rhs.m_Meshes),
	m_Particle(rhs.m_Particle),
	m_TextureInfo(rhs.m_TextureInfo),
	m_MeshInfo(rhs.m_MeshInfo),
	m_NewParticleInfo(rhs.m_NewParticleInfo),
	m_iEffectNum(rhs.m_iEffectNum)
{
}

HRESULT CEffect::Initialize_Prototype(EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo, _uint Effect_Num
	, vector<CEffect_Mesh::MESH_INFO> MeshInfo, vector<CEffect_Particle::PARTICLE_INFO> ParticleInfo, vector<CEffect_Particle_New::PARTICLE_INFO> NewParticleInfo)
{
	m_EffectInfo = EffectInfo;

	for (auto pTexInfo : TextureInfo) {
		m_TextureInfo.push_back(pTexInfo);
	}

	for (auto pMeshInfo : MeshInfo) {
		m_MeshInfo.push_back(pMeshInfo);
	}

	for (auto pParticleInfo : ParticleInfo) {
		m_ParticleInfo.push_back(pParticleInfo);
	}

	for (auto pParticleInfo : NewParticleInfo) {
		m_NewParticleInfo.push_back(pParticleInfo);
	}

	m_iEffectNum = Effect_Num;

	return S_OK;
}

HRESULT CEffect::Initialize(void * pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	_vector vPos;

	if (nullptr != pArg) {
		if (EFFMOVE_TARGET == m_EffectInfo.iMoveType) {
			m_pTarget = (CGameObj*)pArg;
			static_cast<CCollBox*>(m_pTarget)->Set_Effect(this);
		}
		else if (EFFMOVE_MATRIXPIX == m_EffectInfo.iMoveType || EFFMOVE_MATRIX == m_EffectInfo.iMoveType) {
			m_ParentWorldMatrix = (_float4x4*)pArg;
		}
		else if (EFFMOVE_MATRIX2 == m_EffectInfo.iMoveType) {
			m_ParentWorldMatrix = (_float4x4*)pArg;
		}
		else if (EFFMOVE_POS == m_EffectInfo.iMoveType) {
			vPos = *(_vector*)pArg;
		}
		else
			m_pTarget = (CGameObj*)pArg;
	}

	_float3 vRadian;
	vRadian.x = XMConvertToRadians(m_EffectInfo.vRotation.x);
	vRadian.y = XMConvertToRadians(m_EffectInfo.vRotation.y);
	vRadian.z = XMConvertToRadians(m_EffectInfo.vRotation.z);

	m_pTransformCom->RotationAll(vRadian);

	if (EFFMOVE_POS == m_EffectInfo.iMoveType) {
		vPos = vPos + XMVectorSet(m_EffectInfo.vPosition.x, m_EffectInfo.vPosition.y, m_EffectInfo.vPosition.z, 0.f);
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, vPos);
	}
	else {
		m_pTransformCom->Set_State(CTransform::STATE_TRANSLATION, XMVectorSet(m_EffectInfo.vPosition.x, m_EffectInfo.vPosition.y, m_EffectInfo.vPosition.z, 1.f));
	}

	if (EFFMOVE_MATRIXPIX == m_EffectInfo.iMoveType || EFFMOVE_MATRIX == m_EffectInfo.iMoveType || EFFMOVE_MATRIX2 == m_EffectInfo.iMoveType) {
		_matrix mtrParentWorld = XMLoadFloat4x4(m_ParentWorldMatrix);

		mtrParentWorld.r[0] = XMVector3Normalize(mtrParentWorld.r[0]);
		mtrParentWorld.r[1] = XMVector3Normalize(mtrParentWorld.r[1]);
		mtrParentWorld.r[2] = XMVector3Normalize(mtrParentWorld.r[2]);

		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParentWorld);
	}
	else if (EFFMOVE_POS == m_EffectInfo.iMoveType) {
		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix());
	}
	else {
		XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * m_pTarget->Get_Transform()->Get_WorldMatrix());
	}
	//m_pTransformCom->Turn2(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(-180.f));

	if (FAILED(Ready_Parts()))
		return E_FAIL;

	m_bStart = true;

	return S_OK;
}

void CEffect::Tick(_float fTimeDelta)
{
	if (m_bDead) {
		Set_Dead();

		for (auto& pTex : m_Textures)
			pTex->Set_Dead();

		for (auto& pMesh : m_Meshes)
			pMesh->Set_Dead();

		for (auto& pParticle : m_Particle)
			pParticle->Set_Dead();

		for (auto& pParticle : m_NewParticle)
			pParticle->Set_Dead();

		return;
	}

	m_fEffectTime += fTimeDelta;

	if (m_fEffectTime > m_EffectInfo.fEffectStartTime) {

		if (m_bStart) {
			if (m_pTarget != nullptr) {
				if (m_EffectInfo.iMoveType == EFFMOVE_ZERO) {
					_matrix mtrTargetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();
					mtrTargetWorld.r[3].m128_f32[1] = 0.f;
					_matrix mtrWorld = m_pTransformCom->Get_WorldMatrix();

					XMStoreFloat4x4(&m_CombinedWorldMatrix, mtrWorld * mtrTargetWorld);
				}
				else if (m_EffectInfo.iMoveType == EFFMOVE_NONE) {
					_matrix vTargetPos = m_pTarget->Get_Transform()->Get_WorldMatrix();
					_matrix vPos = m_pTransformCom->Get_WorldMatrix();

					XMStoreFloat4x4(&m_CombinedWorldMatrix, vPos * vTargetPos);
				}
				else if (EFFMOVE_MATRIXPIX != m_EffectInfo.iMoveType && EFFMOVE_MATRIX != m_EffectInfo.iMoveType && EFFMOVE_MATRIX2 != m_EffectInfo.iMoveType
					&& EFFMOVE_POS != m_EffectInfo.iMoveType) {
					XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * m_pTarget->Get_Transform()->Get_WorldMatrix());
				}
				m_bStart = false;
			}
		}
		else {
			if (m_EffectInfo.iMoveType != EFFMOVE_NONE && m_EffectInfo.iMoveType != EFFMOVE_ZERO && m_EffectInfo.iMoveType != EFFMOVE_MATRIXPIX
				&& m_EffectInfo.iMoveType != EFFMOVE_MATRIX && m_EffectInfo.iMoveType != EFFMOVE_MATRIX2 && EFFMOVE_POS != m_EffectInfo.iMoveType
				&& m_pTarget != nullptr) {
				_matrix vTargetPos = m_pTarget->Get_Transform()->Get_WorldMatrix();
				_matrix vPos = m_pTransformCom->Get_WorldMatrix();

				XMStoreFloat4x4(&m_CombinedWorldMatrix, vPos * vTargetPos);
			}
		}


		for (auto& pTex : m_Textures)
			pTex->Tick(fTimeDelta);

		for (auto& pMesh : m_Meshes)
			pMesh->Tick(fTimeDelta);

		for (auto& pParticle : m_Particle)
			pParticle->Tick(fTimeDelta);

		for (auto& pParticle : m_NewParticle)
			pParticle->Tick(fTimeDelta);

		if (m_fEffectTime > m_EffectInfo.fEffectStartTime + m_EffectInfo.fEffectLifeTime) {
			Set_Dead();

			for (auto& pTex : m_Textures)
				pTex->Set_Dead();

			for (auto& pMesh : m_Meshes)
				pMesh->Set_Dead();

			for (auto& pParticle : m_Particle)
				pParticle->Set_Dead();

			for (auto& pParticle : m_NewParticle)
				pParticle->Set_Dead();
		}
	}
}

void CEffect::Late_Tick(_float fTimeDelta)
{
	if (m_bDead) {
		return;
	}

	if (EFFMOVE_MATRIX != m_EffectInfo.iMoveType && EFFMOVE_MATRIXPIX != m_EffectInfo.iMoveType && m_pTarget != nullptr
		&& EFFMOVE_MATRIX2 != m_EffectInfo.iMoveType && EFFMOVE_POS != m_EffectInfo.iMoveType)
		m_fEffectStartTime += m_pTarget->Get_EffectTime();

	switch (m_iEffectNum) {
	case CEffect_Manager::EFF_HIT:
	case CEffect_Manager::EFF_HIT2:
	case CEffect_Manager::EFF_HIT3:
	case CEffect_Manager::EFF_HIT4:
	case CEffect_Manager::EFF_HIT5:
	case CEffect_Manager::EFF_PCHANGE_DOWN:
	case CEffect_Manager::EFF_PCHANGE_UP:
	case CEffect_Manager::EFF_GUARD1:
	case CEffect_Manager::EFF_GUARD2:
	case CEffect_Manager::EFF_GUARD3:
	case CEffect_Manager::EFF_GUARD3_BROKEN:
	case CEffect_Manager::EFF_JUMP_DOWN:
	case CEffect_Manager::EFF_JUMP_UP:
	case CEffect_Manager::EFF_RUN:
	case CEffect_Manager::EFF_RUSH_START:
	case CEffect_Manager::EFF_RUSH_MOVE:
	case CEffect_Manager::EFF_RUSH_HIT:
	case CEffect_Manager::EFF_POWER_CHARGING:
	case CEffect_Manager::EFF_POWER_UP:
	case CEffect_Manager::EFF_POWER_UP_PLAYER:
	case CEffect_Manager::EFF_POWER_UP_PLAYER_PERFACT:
	case CEffect_Manager::EFF_GAMESTART:
	case CEffect_Manager::EFF_DASH_TAN_MOVE:
	case CEffect_Manager::EFF_DASH_TAN_STOP:
	case CEffect_Manager::EFF_DASH_TAN_MOVEFB:
	case CEffect_Manager::EFF_DASH_TAN_STOPFB:
	case CEffect_Manager::EFF_SPL_HINO_START:
	case CEffect_Manager::EFF_RUIDAD_ANGRY:
	case CEffect_Manager::EFF_SPL_HINO_STARTSLASH:
	case CEffect_Manager::EFF_SPL_HINO_ENDGROUND:
	case CEffect_Manager::EFF_SPL_HINO_ENDPLAYER:
	case CEffect_Manager::EFF_SPL_HINO_MO1_SLASH1:
	case CEffect_Manager::EFF_SPL_HINO_MO1_SLASH2:
	case CEffect_Manager::EFF_SPL_HINO_MO1_SWORD:
	case CEffect_Manager::EFF_SPL_HINO_MO2_BOOM1:
	case CEffect_Manager::EFF_SPL_HINO_MO2_BOOM2:
	case CEffect_Manager::EFF_SPL_HINO_MO2_BOOM3:
	case CEffect_Manager::EFF_SPL_HINO_MO2_BOOM4:
	case CEffect_Manager::EFF_SPL_HINO_MO2_BOOM5:
	case CEffect_Manager::EFF_SPL_HINO_MO2_BOOM6:
	case CEffect_Manager::EFF_SPL_HINO_MO2_PROJ1:
	case CEffect_Manager::EFF_SPL_HINO_MO2_PROJ2:
	case CEffect_Manager::EFF_SPL_HINO_MO2_PROJ3:
	case CEffect_Manager::EFF_SPL_HINO_MO2_PROJ4:
	case CEffect_Manager::EFF_SPL_HINO_MO2_PROJ5:
	case CEffect_Manager::EFF_SPL_HINO_MO2_SLASH1:
	case CEffect_Manager::EFF_SPL_HINO_MO2_SLASH2:
	case CEffect_Manager::EFF_SPL_HINO_MO2_SLASH3:
	case CEffect_Manager::EFF_SPL_HINO_MO2_SLASH4:
	case CEffect_Manager::EFF_SPL_HINO_MO2_SLASH5:
	case CEffect_Manager::EFF_SPL_HINO_MO2_SLASH6:
	case CEffect_Manager::EFF_SPL_HINO_MO2_SWORD:
	case CEffect_Manager::EFF_SPL_HINO_MO3_PROJ1:
	case CEffect_Manager::EFF_SPL_HINO_MO3_SLASH1:
	case CEffect_Manager::EFF_SPL_HINO_MO4_PROJ1:
	case CEffect_Manager::EFF_SPL_HINO_MO4_SLASH1:
	case CEffect_Manager::EFF_SPL_HINO_MO5_PROJ1:
	case CEffect_Manager::EFF_SPL_HINO_MO5_PROJ2:
	case CEffect_Manager::EFF_SPL_HINO_MO5_SLASH1:
	case CEffect_Manager::EFF_SPL_HINO_MO5_SLASH2:
	case CEffect_Manager::EFF_SPL_HINO_MO5_SLASH3:
	case CEffect_Manager::EFF_SPL_HINO_MO5_GOUND:
	case CEffect_Manager::EFF_SPL_HINO_MO3_SLASH2:
	case CEffect_Manager::EFF_SPL_RUI_MO1_WEB1:
	case CEffect_Manager::EFF_SPL_RUI_MO2_PROJ1:
	case CEffect_Manager::EFF_SPL_RUI_MO2_REDWEB1:
	case CEffect_Manager::EFF_SPL_RUI_MO2_TREADL1:
	case CEffect_Manager::EFF_SPL_RUI_MO2_TREADR1:
	case CEffect_Manager::EFF_SPL_RUI_MO3_REDWEB1:
	case CEffect_Manager::EFF_SPL_RUI_MO3_TREADL1:
	case CEffect_Manager::EFF_SPL_RUI_MO3_TREADR1:
	case CEffect_Manager::EFF_SPL_RUI_MO3_WEB1:
	case CEffect_Manager::EFF_SPL_RUI_MO4_PROJ1:
	case CEffect_Manager::EFF_SPL_RUI_MO4_REDWEB1:
	case CEffect_Manager::EFF_SPL_RUI_MO4_REDWEB2:
	case CEffect_Manager::EFF_SPL_RUI_MO4_TREADL1:
	case CEffect_Manager::EFF_SPL_RUI_MO4_TREADR1:
	case CEffect_Manager::EFF_ENMU_ATK1:
	case CEffect_Manager::EFF_ENMU_ATK2_1:
	case CEffect_Manager::EFF_ENMU_ATK2_2:
	case CEffect_Manager::EFF_ENMU_ATK3_1:
	case CEffect_Manager::EFF_ENMU_ATK3_2:
	case CEffect_Manager::EFF_ENMU_ATK3_3:
	case CEffect_Manager::EFF_ENMU_BALL:
	case CEffect_Manager::EFF_ENMU_SKILL_BALLSTART:
	case CEffect_Manager::EFF_SPL_REN_MO1_SWORD1:
	case CEffect_Manager::EFF_SPL_REN_MO2_BG:
	case CEffect_Manager::EFF_SPL_REN_MO2_SWORD1:
	case CEffect_Manager::EFF_SPL_REN_MO2_AURA1:
	case CEffect_Manager::EFF_SPL_REN_MO2_AURA2:
	case CEffect_Manager::EFF_FADE:
	case CEffect_Manager::EFF_SPL_REN_MO3_GROUND1:
	case CEffect_Manager::EFF_SPL_REN_MO4_BG1:
	case CEffect_Manager::EFF_SPL_REN_MO4_BG2:
	case CEffect_Manager::EFF_SPL_REN_MO4_PLAYER:
	case CEffect_Manager::EFF_SPL_REN_MO5_BG1:
	case CEffect_Manager::EFF_SPL_REN_MO5_FIRE1:
	case CEffect_Manager::EFF_SPL_REN_MO6_BG:
	case CEffect_Manager::EFF_SPL_REN_MO6_HIT:
	case CEffect_Manager::EFF_SPL_REN_MO6_HITBG:
	case CEffect_Manager::EFF_SPL_REN_MO7_BG1:
	case CEffect_Manager::EFF_SPL_REN_MO7_FIRE1:
	case CEffect_Manager::EFF_SPL_REN_MO7_FIRE2:
	case CEffect_Manager::EFF_SPL_REN_MO9_BOOM:
	case CEffect_Manager::EFF_SPL_REN_MO9_GROUND:
	case CEffect_Manager::EFF_SPL_REN_MO9_PROJ1:
	case CEffect_Manager::EFF_SPL_REN_MO9_PORJ2:
	case CEffect_Manager::EFF_SPL_REN_MO9_TONEIDO:
	case CEffect_Manager::EFF_SPL_REN_MO10_FLASH:
	case CEffect_Manager::EFF_ENMUBOSS_PAT1_FLASH:
	case CEffect_Manager::EFF_ENMUBOSS_PAT2_CHOK:
	case CEffect_Manager::EFF_ENMUBOSS_PAT2_FLASH:
	case CEffect_Manager::EFF_ENMUBOSS_PAT2_GROUND:
	case CEffect_Manager::EFF_ENMUBOSS_PAT3_FLASH:
	case CEffect_Manager::EFF_ENMUBOSS_PAT3_GROUND:
	case CEffect_Manager::EFF_ENMUBOSS_PAT4_FLASH:
	case CEffect_Manager::EFF_ENMUBOSS_PAT4_HAND:
	case CEffect_Manager::EFF_ENMUBOSS_PAT4_GROUND:
	case CEffect_Manager::EFF_ENMUBOSS_PAT5_GROUND:
	case CEffect_Manager::EFF_ENMUBOSS_PAT5_GROUND2:
	case CEffect_Manager::EFF_ENMUBOSS_PAT5_WIND:
	case CEffect_Manager::EFF_ENMUBOSS_PAT5_HAND:
	case CEffect_Manager::EFF_ENMUBOSS_PAT2_FLASH2:
	case CEffect_Manager::EFF_ENMUBOSS_PAT3_FLASH2:
	case CEffect_Manager::EFF_ENMUBOSS_PAT4_FLASH2:
	case CEffect_Manager::EFF_TRAIN_WIND:
	case CEffect_Manager::EFF_TRAIN_SMOKE:
	case CEffect_Manager::EFF_SPL_AKA_MO1_HIT:
	case CEffect_Manager::EFF_SPL_AKA_MO2_GROUND:
	case CEffect_Manager::EFF_SPL_AKA_MO3_GROUND:
	case CEffect_Manager::EFF_SPL_AKA_MO5_DASH1:
	case CEffect_Manager::EFF_SPL_AKA_MO5_HAND1:
	case CEffect_Manager::EFF_SPL_AKA_MO5_HIT1:
	case CEffect_Manager::EFF_SPL_AKA_MO5_HIT2:
	case CEffect_Manager::EFF_SPL_AKA_MO5_KICK1:
	case CEffect_Manager::EFF_SPL_AKA_MO5_RUN1:
	case CEffect_Manager::EFF_SPL_AKA_MO6_PROJ1:
	case CEffect_Manager::EFF_SPL_AKA_MO6_SLASH1:
	case CEffect_Manager::EFF_SPL_AKA_MO6_SLASH2:
	case CEffect_Manager::EFF_SPL_AKA_MO7_BACKLIGHT:
	case CEffect_Manager::EFF_SPL_AKA_MO7_BG1:
	case CEffect_Manager::EFF_SPL_AKA_MO7_BG2:
	case CEffect_Manager::EFF_SPL_AKA_MO7_HAND:
	case CEffect_Manager::EFF_SPL_AKA_MO7_HAND2:
	case CEffect_Manager::EFF_SPL_AKA_MO7_HAND3:
	case CEffect_Manager::EFF_SPL_AKA_MO7_PROJ1:
	case CEffect_Manager::EFF_SPL_AKA_MO8_BG1:
	case CEffect_Manager::EFF_SPL_AKA_MO8_HAND1:
	case CEffect_Manager::EFF_SPL_AKA_MO8_PROJ1:
	case CEffect_Manager::EFF_SPL_AKA_MO9_AURA1:
	case CEffect_Manager::EFF_SPL_AKA_MO9_BG1:
	case CEffect_Manager::EFF_SPL_AKA_MO4_DIST:
	case CEffect_Manager::EFF_SPL_AKA_MO9_PROJ1:
	case CEffect_Manager::EFF_SPL_AKA_MO9_BOOM1:
		if (m_fEffectTime > m_EffectInfo.fEffectStartTime) {
			if (m_bLateStart) {
				if (EFFMOVE_MATRIXPIX == m_EffectInfo.iMoveType || EFFMOVE_MATRIX == m_EffectInfo.iMoveType || EFFMOVE_MATRIX2 == m_EffectInfo.iMoveType) {
					_matrix mtrParentWorld = XMLoadFloat4x4(m_ParentWorldMatrix);

					mtrParentWorld.r[0] = XMVector3Normalize(mtrParentWorld.r[0]);
					mtrParentWorld.r[1] = XMVector3Normalize(mtrParentWorld.r[1]);
					mtrParentWorld.r[2] = XMVector3Normalize(mtrParentWorld.r[2]);

					XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParentWorld);
				}
				m_bLateStart = false;
			}
			else {
				if (EFFMOVE_MATRIX == m_EffectInfo.iMoveType || EFFMOVE_MATRIX2 == m_EffectInfo.iMoveType) {
					_matrix mtrParentWorld = XMLoadFloat4x4(m_ParentWorldMatrix);

					mtrParentWorld.r[0] = XMVector3Normalize(mtrParentWorld.r[0]);
					mtrParentWorld.r[1] = XMVector3Normalize(mtrParentWorld.r[1]);
					mtrParentWorld.r[2] = XMVector3Normalize(mtrParentWorld.r[2]);

					XMStoreFloat4x4(&m_CombinedWorldMatrix, m_pTransformCom->Get_WorldMatrix() * mtrParentWorld);
				}
			}

			for (auto& pTex : m_Textures)
				pTex->Late_Tick(fTimeDelta);

			for (auto& pMesh : m_Meshes)
				pMesh->Late_Tick(fTimeDelta);

			for (auto& pParticle : m_Particle)
				pParticle->Late_Tick(fTimeDelta);

			for (auto& pParticle : m_NewParticle)
				pParticle->Late_Tick(fTimeDelta);
		}
		break;
	default:
		if (m_fEffectTime > m_EffectInfo.fEffectStartTime) {
			int i = 0;
			for (auto& pTex : m_Textures) {
				if (m_fEffectStartTime >= m_EffectInfo.fEffectStartTime + m_TextureInfo[i].fStartTime)
					pTex->Late_Tick(fTimeDelta);
				i++;
			}

			i = 0;
			for (auto& pMesh : m_Meshes) {
				if (m_fEffectStartTime >= m_EffectInfo.fEffectStartTime + m_MeshInfo[i].fStartTime)
					pMesh->Late_Tick(fTimeDelta);
				i++;
			}

			i = 0;
			for (auto& pParticle : m_Particle) {
				if (m_fEffectStartTime >= m_EffectInfo.fEffectStartTime + m_ParticleInfo[i].fStartTime)
					pParticle->Late_Tick(fTimeDelta);
				i++;
			}

			i = 0;
			for (auto& pParticle : m_NewParticle) {
				if (m_fEffectStartTime >= m_EffectInfo.fEffectStartTime + m_NewParticleInfo[i].fStartTime)
					pParticle->Late_Tick(fTimeDelta);
				i++;
			}
		}
		break;
	}


}

void CEffect::Set_ParentWorldMatrix(_matrix ParentMat)
{
	_matrix mtrWorld = m_pTransformCom->Get_WorldMatrix();

	_matrix mtrParentWorld = mtrWorld * ParentMat;

	mtrParentWorld.r[0] = XMVector3Normalize(mtrParentWorld.r[0]);
	mtrParentWorld.r[1] = XMVector3Normalize(mtrParentWorld.r[1]);
	mtrParentWorld.r[2] = XMVector3Normalize(mtrParentWorld.r[2]);

	XMStoreFloat4x4(&m_CombinedWorldMatrix, mtrWorld * mtrParentWorld);
}

HRESULT CEffect::Ready_Components()
{
	/* For.Com_Transform */
	CTransform::TRANSFORMDESC      TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Components(TEXT("Com_Transform"), LEVEL_STATIC, TEXT("Prototype_Component_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::Ready_Parts()
{
	CGameInstance*      pGameInstance = GET_INSTANCE(CGameInstance);

	// 咆胶贸 积己
	for (auto TexInfo : m_TextureInfo) {
		CGameObject*      pTexture = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_EffectTexture"));

		static_cast<CEffect_Texture*>(pTexture)->Set_Parents(this);
		static_cast<CEffect_Texture*>(pTexture)->Set_TexInfo(TexInfo);

		m_Textures.push_back((CEffect_Texture*)pTexture);
	}

	// 皋浆 积己
	for (auto MeshInfo : m_MeshInfo) {
		CGameObject*      pMesh = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_EffectMesh"));

		static_cast<CEffect_Mesh*>(pMesh)->Set_Parents(this);
		static_cast<CEffect_Mesh*>(pMesh)->Set_MeshInfo(MeshInfo);

		m_Meshes.push_back((CEffect_Mesh*)pMesh);
	}

	// 颇萍努 积己
	for (auto ParticleInfo : m_ParticleInfo) {
		CGameObject*      pParticle = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_EffectParticle"));

		static_cast<CEffect_Particle*>(pParticle)->Set_Parents(this);
		static_cast<CEffect_Particle*>(pParticle)->Set_ParticleInfo(ParticleInfo);

		m_Particle.push_back((CEffect_Particle*)pParticle);
	}

	// New颇萍努 积己
	for (auto ParticleInfo : m_NewParticleInfo) {
		CGameObject*      pParticle = pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_EffectParticleNew"));

		static_cast<CEffect_Particle_New*>(pParticle)->Set_Parents(this);
		static_cast<CEffect_Particle_New*>(pParticle)->Set_ParticleInfo(ParticleInfo);

		m_NewParticle.push_back((CEffect_Particle_New*)pParticle);
	}

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

CEffect * CEffect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, _uint Effect_Num, EFFECT_INFO EffectInfo, vector<CEffect_Texture::TEXTURE_INFO> TextureInfo
	, vector<CEffect_Mesh::MESH_INFO> MeshInfo, vector<CEffect_Particle::PARTICLE_INFO> ParticleInfo, vector<CEffect_Particle_New::PARTICLE_INFO> NewParticleInfo)
{
	CEffect*   pInstance = new CEffect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(EffectInfo, TextureInfo, Effect_Num, MeshInfo, ParticleInfo, NewParticleInfo)))
	{
		ERR_MSG(TEXT("Failed to Created : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CEffect::Clone(void * pArg)
{
	CGameObject*   pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		ERR_MSG(TEXT("Failed to Cloned : CEffect"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	for (auto& pTex : m_Textures)
		Safe_Release(pTex);

	for (auto& pMesh : m_Meshes)
		Safe_Release(pMesh);

	for (auto& pParticle : m_Particle)
		Safe_Release(pParticle);

	for (auto& pParticle : m_NewParticle)
		Safe_Release(pParticle);

	Safe_Release(m_pTransformCom);
}