#include "stdafx.h"
#include "KyoujuroJumpSkill_Common.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Effect_Manager.h"
using namespace Kyoujuro;


CJumpSkill_CommonState::CJumpSkill_CommonState(_float fPositionY)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;


	m_fPositionY = fPositionY;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CJumpSkill_CommonState::HandleInput(CKyoujuro * pKyojuro)
{

	return nullptr;
}

CKyoujuroState * CJumpSkill_CommonState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	
	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	//if (pKyojuro->Get_Model()->Get_End(pKyojuro->Get_AnimIndex()))
	//{
	//	pKyojuro->Get_Model()->Set_End(pKyojuro->Get_AnimIndex());
	//	return new CIdleState();
	//}

	if (m_bOnGround == false)
		Jump(pKyojuro, fTimeDelta);

	pKyojuro->Set_JumpState(true);

	return nullptr;
}

CKyoujuroState * CJumpSkill_CommonState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pKyojuro->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	pKyojuro->Get_Transform()->Set_PlayerLookAt(vLooAt);

	m_fTime += fTimeDelta;


	RELEASE_INSTANCE(CGameInstance);


	if (m_bOnGround == true)
	{
		m_pCollBox->Set_Dead();
		pKyojuro->Get_Model()->Set_Loop(CKyoujuro::ANIMID::ANIM_SKILL_JUMPCOMMON, true);
		return new CIdleState();
	}


	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);



	

	return nullptr;
}

void CJumpSkill_CommonState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_JUMPCOMMON;

	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_JUMPCOMMON);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_JUMPCOMMON);
	pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_SKILL_JUMPCOMMON);
	pKyojuro->Get_Model()->Set_FrameNum(CKyoujuro::ANIM_SKILL_JUMPCOMMON, 100);


	m_vPosition.x = XMVectorGetX(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vVelocity.x = 0.f;
	m_vVelocity.y = 10.f;
	m_vVelocity.z = 0.f;


}

void CJumpSkill_CommonState::Exit(CKyoujuro * pKyojuro)
{
	m_pCollBox->Set_Dead();
}

CKyoujuroState* CJumpSkill_CommonState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	static _float fJump_Velocity = 10.f;
	static _float fGravity = -19.8f;


	m_vVelocity.y += fGravity * fTimeDelta;
	m_vPosition.x += m_vVelocity.x * fTimeDelta;
	m_vPosition.y += m_vVelocity.y * fTimeDelta;
	m_vPosition.z += m_vVelocity.z * fTimeDelta;
	
	_vector vCurrentPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) < m_fPositionY)
	{
		_vector vPosition;
		m_vPosition.y = m_fPositionY;
		m_vVelocity.y = m_fPositionY;
		m_bOnGround = true;

		vPosition =  XMVectorSet(m_vPosition.x, m_vPosition.y ,m_vPosition.z, 1.f);
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
	}
	else
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION,vPosition);

	return nullptr;
}

