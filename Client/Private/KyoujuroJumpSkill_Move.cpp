#include "stdafx.h"
#include "KyoujuroJumpSkill_Move.h"
#include "KyoujuroIdleState.h"
#include "GameInstance.h"
#include "KyoujuroWeapon.h"
#include "Layer.h"
#include "Effect_Manager.h"
#include "KyoujuroJumpState.h"
using namespace Kyoujuro;


CJumpSkill_MoveState::CJumpSkill_MoveState(_float fPositionY)
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_BaseAtk"), LEVEL_STATIC, TEXT("Layer_CollBox"), &m_pCollBox)))
		return;


	m_fPositionY = fPositionY;

	RELEASE_INSTANCE(CGameInstance);
}

CKyoujuroState * CJumpSkill_MoveState::HandleInput(CKyoujuro * pKyojuro)
{

	return nullptr;
}

CKyoujuroState * CJumpSkill_MoveState::Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{

	pKyojuro->Get_Model()->Set_Loop(pKyojuro->Get_AnimIndex());

	pKyojuro->Set_JumpState(true);



	m_fTime += fTimeDelta;

	if (m_fTime <= 0.5f)
		Move(pKyojuro, fTimeDelta * 1.5f);
	else
	{
		if (m_bFinishAnim == false)
		{
			if (pKyojuro->Get_Model()->Get_End(pKyojuro->Get_AnimIndex()))
			{
				pKyojuro->Get_Model()->Set_End(pKyojuro->Get_AnimIndex());
				m_bFinishAnim = true;
			
			}
		}
	}

	if (m_bFinishAnim == true)
	{
		Jump(pKyojuro, fTimeDelta);
	}

	static _bool bSetting = false;

	if (pKyojuro->Get_Model()->Get_CurrentTime() >= 25.f && bSetting == false)
	{
		m_vPosition.x = XMVectorGetX(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.y = XMVectorGetY(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vPosition.z = XMVectorGetZ(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
		m_vVelocity.y = 10.f;

		bSetting = true;
	}
		
	if (pKyojuro->Get_Model()->Get_CurrentTime() >= 30.f)
	{
		m_bFinishAnim = true;
	}



	return nullptr;
}

CKyoujuroState * CJumpSkill_MoveState::Late_Tick(CKyoujuro * pKyojuro, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);




	pKyojuro->Get_Model()->Play_Animation(fTimeDelta);

	if (m_bNextAnim == true)
	{
		m_pCollBox->Set_Dead();
		return new CIdleState();
	}




	return nullptr;
}

void CJumpSkill_MoveState::Enter(CKyoujuro * pKyojuro)
{
	m_eStateId = STATE_ID::STATE_SKILL_JUMPMOVE;


	pKyojuro->Get_Model()->Reset_Anim(CKyoujuro::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Get_Model()->Set_CurrentAnimIndex(CKyoujuro::ANIMID::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Set_AnimIndex(CKyoujuro::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Get_Model()->Set_Loop(CKyoujuro::ANIM_SKILL_JUMPMOVE);
	pKyojuro->Get_Model()->Set_FrameNum(CKyoujuro::ANIM_SKILL_JUMPMOVE, 100);



	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;
	m_vPosition.y = XMVectorGetY(pKyojuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

}

void CJumpSkill_MoveState::Exit(CKyoujuro * pKyojuro)
{
	m_pCollBox->Set_Dead();
}

CKyoujuroState * CJumpSkill_MoveState::Move(CKyoujuro * pKyoujuro, _float fTimeDelta)
{
	static _float fY = (m_vPosition.y - m_fPositionY) / 2.f;

	

	CCharacters* m_pTarget = pKyoujuro->Get_BattleTarget();
	_vector vLookAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	pKyoujuro->Get_Transform()->Set_PlayerLookAt(vLookAt);

	_vector vMyPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vDistance = vMyPosition - vTargetPosition;
	_float fDistance = XMVectorGetX(XMVector3Length(vDistance));

	if(fDistance >= 2.5f)
		pKyoujuro->Get_Transform()->Go_Straight(fTimeDelta, pKyoujuro->Get_NavigationCom());

	
	_vector vPosition = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vPosition = XMVectorSetY(vPosition, fY);

	pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);



	return nullptr;
}

CKyoujuroState* CJumpSkill_MoveState::Jump(CKyoujuro* pKyoujuro, _float fTimeDelta)
{
	static _float fGravity = -14.81f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	fVelocity += fGravity *fTimeDelta;
	vPosition.y += fVelocity * fTimeDelta;

	_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fPositionY)
	{
		vPosition.y = m_fPositionY;
		fVelocity = m_fPositionY;

		_vector vecPos = pKyoujuro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

		m_bNextAnim = true;
	}
	else
		pKyoujuro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);


	return nullptr;

}

