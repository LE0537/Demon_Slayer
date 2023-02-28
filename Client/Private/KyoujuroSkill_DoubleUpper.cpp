#include "stdafx.h"
#include "KyoujuroSkill_DoubleUpper.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Tanjiro.h"
#include "Effect_Manager.h"
#include "KyoujuroDashState.h"
#include "KyoujuroSkill_Common.h"
#include "KyoujuroSkill_DashSlash.h"
#include "KyoujuroSkill_DoubleUpper.h"
#include "KyoujuroJumpState.h"
#include "KyoujuroTargetRushState.h"
#include "KyoujuroAtk_1_State.h"
#include "Camera_Dynamic.h"
#include "Layer.h"
#include "KyoujuroSplSkrStartState.h"
using namespace Kyoujuro;


CSkill_DoubleUpperState::CSkill_DoubleUpperState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_WaterMill"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CSkill_DoubleUpperState::HandleInput(CKyoujuro * pKyojuro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pKyojuro->Get_Model()->Get_Duration_Index(pKyojuro->Get_AnimIndex());
	m_fCurrentDuration = pKyojuro->Get_Model()->Get_CurrentTime_Index(pKyojuro->Get_AnimIndex());

	_float fRatio = m_fCurrentDuration / m_fDuration;

	if (pKyojuro->Get_AnimIndex() == 38)
	{
		if (fRatio >= 0.7f)
		{
			switch (pKyojuro->Get_i1P())
			{
			case 1:
				if (pGameInstance->Key_Pressing(DIK_W)) // 앞
				{
					if (pGameInstance->Key_Pressing(DIK_A)) // 좌
					{
						if (pGameInstance->Key_Pressing(DIK_L))
							return new CDashState(DIR_LF);
					}
					else if (pGameInstance->Key_Pressing(DIK_D)) // 우
					{
						if (pGameInstance->Key_Pressing(DIK_L))
							return new CDashState(DIR_RF);
					}
					else
					{
						if (pGameInstance->Key_Pressing(DIK_L))
							return new CDashState(DIR_STRAIGHT);
					}
				}

				else if (pGameInstance->Key_Pressing(DIK_S)) // 뒤
				{
					if (pGameInstance->Key_Pressing(DIK_A)) // 좌
					{
						if (pGameInstance->Key_Pressing(DIK_L))
							return new CDashState(DIR_LB);
					}
					else if (pGameInstance->Key_Pressing(DIK_D)) // 우 
					{

						if (pGameInstance->Key_Pressing(DIK_L))
							return new CDashState(DIR_RB);

					}
					else
					{
						if (pGameInstance->Key_Pressing(DIK_L))
							return new CDashState(DIR_BACK);
					}
				}


				else if (pGameInstance->Key_Pressing(DIK_A)) // 좌
				{

					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_LEFT);

				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // 우
				{
					if (pGameInstance->Key_Pressing(DIK_L))
						return new CDashState(DIR_RIGHT);
				}
				break;
			case 2:


				if (pGameInstance->Key_Pressing(DIK_UP)) // 앞
				{
					if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
					{

						if (pGameInstance->Key_Pressing(DIK_LSHIFT))
							return new CDashState(DIR_LF);
					}
					else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
					{

						if (pGameInstance->Key_Pressing(DIK_LSHIFT))
							return new CDashState(DIR_RF);


					}
					else
					{
						if (pGameInstance->Key_Pressing(DIK_LSHIFT))
							return new CDashState(DIR_STRAIGHT);
					}
				}

				else if (pGameInstance->Key_Pressing(DIK_DOWN)) // 뒤
				{
					if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
					{

						if (pGameInstance->Key_Pressing(DIK_LSHIFT))
							return new CDashState(DIR_LB);


					}
					else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우 
					{

						if (pGameInstance->Key_Pressing(DIK_LSHIFT))
							return new CDashState(DIR_RB);

					}
					else
					{
						if (pGameInstance->Key_Pressing(DIK_LSHIFT))
							return new CDashState(DIR_BACK);

					}
				}


				else if (pGameInstance->Key_Pressing(DIK_LEFT)) // 좌
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_LEFT);

				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // 우
				{
					if (pGameInstance->Key_Pressing(DIK_LSHIFT))
						return new CDashState(DIR_RIGHT);

				}
				break;
			}
		}
	}

	return CommandCheck(pKyojuro);
}

CKyoujuroState * CSkill_DoubleUpperState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	pKyojuro->Set_EffectTime(fTimeDelta);
	//pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	if (pKyojuro->Get_Model()->Get_End(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER))
	{
		pKyojuro->Get_Model()->Set_CurrentAnimIndex(37);
		pKyojuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(37));
		pKyojuro->Get_Model()->Set_Loop(37, true);
		pKyojuro->Get_Model()->Set_LinearTime(37, 0.01f);
		pKyojuro->Get_Model()->Set_End(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER);
	}


	if (m_bJump == true)
	{
		//pKyojuro->Get_Model()->Set_CurrentAnimIndex(38);
		//pKyojuro->Set_AnimIndex(static_cast<CKyoujuro::ANIMID>(38));
		//pKyojuro->Get_Model()->Set_Loop(38);
		//pKyojuro->Get_Model()->Set_LinearTime(38, 0.01f);
		return new CIdleState();
	}


	

	//if (pKyojuro->Get_Model()->Get_End(37))
	//{

	//	pKyojuro->Get_Model()->Set_End(37);
	//	return new CIdleState();
	//}




	return nullptr;
}

CKyoujuroState * CSkill_DoubleUpperState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pKyojuro->Get_BattleTarget();
	if (!m_bLook)
	{
		_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		XMStoreFloat4(&m_vLook, vLooAt);
		pKyojuro->Get_Transform()->Set_PlayerLookAt(vLooAt);
		m_bLook = true;
	}

	m_fTime += fTimeDelta;

	if (m_fTime < 0.4f)
	{
		pKyojuro->Get_Transform()->Go_Straight(fTimeDelta * 0.3f, pKyojuro->Get_NavigationCom());

		if (!m_bHit && m_fTime > 0.2f)
		{
			_vector vCollPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 1.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);
				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-70 * pKyojuro->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, pKyojuro);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pKyojuro->Get_BattleTarget()->Get_GodMode() == false)
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
					RELEASE_INSTANCE(CGameInstance);
					m_pTarget->Set_Hp(_int(-50 * pKyojuro->Get_PlayerInfo().fPowerUp));


					//if (!m_bHit)
					m_pTarget->Player_UpperDown(CCharacters::HIT_TYPE::HIT_UPPER, 10.f, 15.f, 2.f);
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_SE_Hit_CommonSkill.wav"), fEFFECT);
					//m_pTarget->Take_Damage(0.8f, true);
					pKyojuro->Set_Combo(1);
					pKyojuro->Set_ComboTime(0.f);
				}
				_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();
				if (pKyojuro->Get_BattleTarget()->Get_GodMode() == false)
				{
					_int iDest = rand() % 5;
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					switch (iDest)
					{
					case 0:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
						break;
					case 1:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
						break;
					case 2:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
						break;
					case 3:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
						break;
					case 4:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
						break;
					default:
						break;
					}


					RELEASE_INSTANCE(CEffect_Manager);

					m_bHit = true;
				}
			}
		}


		CCollider*	pMyCollider = pKyojuro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return nullptr;

		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pKyojuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyojuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyojuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyojuro->Get_NavigationCom());
		}
	}
	else if (m_fTime >= 0.4f)
	{
		if (m_fTime < 0.9f)
			pKyojuro->Get_Transform()->Go_Straight(fTimeDelta * 0.7f, pKyojuro->Get_NavigationCom());
		m_fHitTime += fTimeDelta;
		if (m_iHit < 1 && m_fHitTime > 0.1f)
		{
			_vector vCollPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION); //추가
			_vector vCollLook = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK); //추가
			vCollPos += XMVector3Normalize(vCollLook) * 1.f; //추가
			vCollPos.m128_f32[1] = 1.f; //추가
			m_pCollBox->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vCollPos); //추가
			m_pCollBox->Get_Transform()->Set_PlayerLookAt(XMLoadFloat4(&m_vLook));
			CCollider*	pMyCollider = m_pCollBox->Get_Collider(); //추가
			CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

			if (nullptr == pTargetCollider)
				return nullptr;

			if (pMyCollider->Collision(pTargetCollider))
			{
				_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
				m_pTarget->Get_Transform()->Set_PlayerLookAt(vPos);

				if (m_pTarget->Get_PlayerInfo().bGuard && m_pTarget->Get_PlayerInfo().iGuard > 0)
				{
					m_pTarget->Get_GuardHit(0);
					m_pTarget->Set_GuardHp(_int(-70 * pKyojuro->Get_PlayerInfo().fPowerUp));
					if (m_pTarget->Get_PlayerInfo().iGuard <= 0)
					{
						CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
						pEffectManger->Create_Effect(CEffect_Manager::EFF_GUARD3_BROKEN, pKyojuro);
						RELEASE_INSTANCE(CEffect_Manager);
						m_pTarget->Set_ResetGuardHp();
						m_pTarget->Set_GuardTime(2.f);
					}
				}
				else if (pKyojuro->Get_BattleTarget()->Get_GodMode() == false)
				{
					CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Shake(CCamera_Dynamic::SHAKE_HIT, 0.2f);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
					dynamic_cast<CCamera_Dynamic*>(pGameInstance2->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pKyojuro->Get_Renderer());
					RELEASE_INSTANCE(CGameInstance);
					CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_SE_Hit_CommonSkill.wav"), fEFFECT);
					pKyojuro->Set_Combo(1);
					pKyojuro->Set_ComboTime(0.f);
					m_pTarget->Set_Hp(_int(-50 * pKyojuro->Get_PlayerInfo().fPowerUp));
					//if (!m_bHit)
					m_pTarget->Player_UpperDown(CCharacters::HIT_TYPE::HIT_UPPER, 22.f, 20.f, 9.f);
					//m_pTarget->Take_Damage(0.5f, true);
				}

				_matrix vTagetWorld = m_pTarget->Get_Transform()->Get_WorldMatrix();
				if (pKyojuro->Get_BattleTarget()->Get_GodMode() == false)
				{
					_int iDest = rand() % 5;
					CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);
					switch (iDest)
					{
					case 0:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT, m_pTarget);
						break;
					case 1:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT2, m_pTarget);
						break;
					case 2:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT3, m_pTarget);
						break;
					case 3:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT4, m_pTarget);
						break;
					case 4:
						pEffectManger->Create_Effect(CEffect_Manager::EFF_HIT5, m_pTarget);
						break;
					default:
						break;
					}


					RELEASE_INSTANCE(CEffect_Manager);
					m_fHitTime = 0.f;
					++m_iHit;
				}
			}
		}
		CCollider*	pMyCollider = pKyojuro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();
		if (pMyCollider->Collision(pTargetCollider))
		{
			_float fSpeed = pKyojuro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta * 1.5f;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pKyojuro->Get_NavigationCom()->Cheak_Cell(vPos))
				pKyojuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pKyojuro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pKyojuro->Get_NavigationCom());
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	if (!m_bJump && m_fTime >= 0.4f)
	{
		m_fJumpTime += 0.035f;
		Jump(pKyojuro, m_fJumpTime);
	}
	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);

	if (!m_bEffect)
	{
		CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_2NOBORI_1, pKyojuro);
		pEffectManger->Create_Effect(CEffect_Manager::EFF_RGKSKL_2NOBORI_2, pKyojuro);

		RELEASE_INSTANCE(CEffect_Manager);
		m_bEffect = true;
	}


	return nullptr;
}

void CSkill_DoubleUpperState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_DOUBLEUPPER;
	pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER);
	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_DOUBLEUPPER);

	m_fCurrentPosY = XMVectorGetY(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	pKyojuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER);
	pKyojuro->Get_Model()->Set_LinearTime(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER, 0.2f);

	CSoundMgr::Get_Instance()->PlayVoice(TEXT("Kyojuro_DoubleUpper.wav"), fVOICE);
	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Kyojuro_SE_DubleUpperSkill.wav"), fEFFECT);
}
CKyoujuroState* CSkill_DoubleUpperState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	pKyoujuro->Set_NavigationHeight(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pKyoujuro->Get_NavigationHeight().y;
	pKyoujuro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;

	fStartHeight = pKyoujuro->Get_NavigationHeight().y;
	fEndHeight = pKyoujuro->Get_NavigationHeight().y;

	_vector      vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);
	//m_fCurrentPosY = y;

	if (y <= fEndHeight)
	{
		pKyoujuro->Get_Transform()->Set_Jump(false);
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_bJump = true;
	}
	else
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}
void CSkill_DoubleUpperState::Exit(CKyoujuro * pKyojuro)
{
	//pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIMID::ANIM_SKILL_DOUBLEUPPER);
	m_pCollBox->Set_Dead();
}

CKyoujuroState * CSkill_DoubleUpperState::CommandCheck(CKyoujuro * pKyoujuro)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	m_fDuration = pKyoujuro->Get_Model()->Get_Duration_Index(38);
	m_fCurrentDuration = pKyoujuro->Get_Model()->Get_CurrentTime_Index(38);

	_float fRatio = m_fCurrentDuration / m_fDuration;


	if (fRatio >= 0.7f)
	{
		switch (pKyoujuro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Pressing(DIK_E))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				return new CSplSkrStartState(TYPE_START);
			}

			if (pGameInstance->Key_Down(DIK_J))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_I)) // 스킬 키 
			{
				if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_O))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DoubleUpperState();
					}
					else if (pGameInstance->Key_Pressing(DIK_W) || pGameInstance->Key_Pressing(DIK_A) || pGameInstance->Key_Pressing(DIK_S) || pGameInstance->Key_Pressing(DIK_D))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DashSlashState(); // move skill
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_CommonState();
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_L))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_SPACE))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			break;
		case 2:
			if (pGameInstance->Key_Pressing(DIK_RSHIFT))
			{
				//	pTanjiro->Get_BattleTarget()->Play_Scene();
				return new CSplSkrStartState(TYPE_START);
			}

			if (pGameInstance->Key_Down(DIK_Z))
				return new CAtk_1_State();

			if (pGameInstance->Key_Pressing(DIK_X)) // 스킬 키 
			{
				if (pKyoujuro->Get_PlayerInfo().iSkBar >= 200)
				{
					if (pGameInstance->Key_Pressing(DIK_C))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DoubleUpperState();
					}
					else if (pGameInstance->Key_Pressing(DIK_LEFT) || pGameInstance->Key_Pressing(DIK_RIGHT) || pGameInstance->Key_Pressing(DIK_UP) || pGameInstance->Key_Pressing(DIK_DOWN))
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);

						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_DashSlashState(); // move skill

					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pKyoujuro->Set_SkillBar(-200);
						return new CSkill_CommonState();
					}
				}
			}
			else if (pGameInstance->Key_Pressing(DIK_LSHIFT))
			{
				return new CTargetRushState(TYPE_START);
			}
			else if (pGameInstance->Key_Pressing(DIK_LCONTROL))
			{
				return new CJumpState(TYPE_START, 0.f, 0.f);
			}
			break;
		}

	}

	return nullptr;
}

