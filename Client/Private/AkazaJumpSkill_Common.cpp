#include "stdafx.h"
#include "AkazaJumpSkill_Common.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "AkazaShoot.h"
using namespace Akaza;


CJumpSkill_CommmonState::CJumpSkill_CommmonState(STATE_TYPE eType, _uint iCount)
	: m_iCount(iCount)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);
	m_eStateType = eType;
	
	
	RELEASE_INSTANCE(CGameInstance);

}

CAkazaState * CJumpSkill_CommmonState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	
	

	return nullptr;
}

CAkazaState * CJumpSkill_CommmonState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CJumpSkill_CommmonState(TYPE_LOOP);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
		//	return new CJumpSkill_CommmonState(TYPE_END);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			
				//return new CIdleState();
			//return new CJumpSkill_CommmonState(TYPE_DEFAULT);
			break;
		case Client::CAkazaState::TYPE_DEFAULT:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			break;
		case Client::CAkazaState::TYPE_CHANGE:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
	}


	if (m_eStateType == TYPE_LOOP)
	{
		if (pAkaza->Get_Model()->Get_CurrentTime() >= 7.f)
		{
			switch (m_iCount)
			{
			case 0:
				pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_1);
				return new CJumpSkill_CommmonState(TYPE_LOOP, 1);
				break;
			case 1:
				pAkaza->Get_Model()->Reset_Anim(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_1);
				return new CJumpSkill_CommmonState(TYPE_END);
				break;
			}

		}
	}
	else if (m_eStateType == TYPE_END)
		Jump(pAkaza, fTimeDelta);
	
	

	if(m_bNextAnim == true)
		return new CJumpSkill_CommmonState(TYPE_CHANGE);
	

	return nullptr;
}

CAkazaState * CJumpSkill_CommmonState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{

	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);

		m_fMove += fTimeDelta;

	CAkazaShoot::AKAZASHOOTINFO	tInfo;
	tInfo.pPlayer = pAkaza;
	tInfo.pTarget = m_pTarget;


	if (m_eStateType == Client::CAkazaState::TYPE_LOOP)
	{
		if (m_fMove > 0.07f && m_iHit < 4)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_AkazaShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}
	}



	if (m_eStateType == TYPE_END)
	{
		//if(pAkaza->Get_Model()->Get_CurrentTime() <= 20.f)
			pAkaza->Get_Model()->Play_Animation(fTimeDelta);
	
	}
	else if(m_eStateType == TYPE_CHANGE)
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	else
		pAkaza->Get_Model()->Play_Animation(fTimeDelta * 1.3f);

	return nullptr;
}

void CJumpSkill_CommmonState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_JUMP_ATTACK;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_JUMPCOMMON_0);
		pAkaza->Get_Transform()->Set_PlayerLookAt(pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_0);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Akaza_JumpShoot.wav"), fEFFECT);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_JUMPCOMMON_1);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_1, false);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_JUMPCOMMON_2);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_SKILL_JUMPCOMMON_2);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		Initialize_value(pAkaza);
		break;

	case Client::CAkazaState::TYPE_DEFAULT:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_LOOP_END);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_LOOP_END);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_LOOP_END);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		Initialize_value(pAkaza);
		break;

	case Client::CAkazaState::TYPE_CHANGE:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_END);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
		pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_END);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		//Initialize_value(pAkaza);
		break;
	}


}

void CJumpSkill_CommmonState::Exit(CAkaza* pAkaza)
{
	
}

void CJumpSkill_CommmonState::Jump(CAkaza * pAkaza, _float fTimeDelta)
{
	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fOriginPosY = pAkaza->Get_NavigationHeight().y;
	pAkaza->Get_Transform()->Set_Jump(true);

	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;



	vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		pAkaza->Get_Transform()->Set_Jump(false);
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
			 
			m_bNextAnim = true;
			//pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_JUMP_END);
			//pAkaza->Set_AnimIndex(CAkaza::ANIM_JUMP_END);
			//pAkaza->Get_Model()->Set_Loop(CAkaza::ANIMID::ANIM_JUMP_END, false);
			//pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		}
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vecPos))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}

}

void CJumpSkill_CommmonState::Initialize_value(CAkaza * pAkaza)
{

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}


