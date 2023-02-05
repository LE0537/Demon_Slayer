#include "stdafx.h"
#include "TanjiroKaguraJumpSkill_Move.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CKaguraJumpSkill_MoveState::CKaguraJumpSkill_MoveState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);


	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CTanjiroState * CKaguraJumpSkill_MoveState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CKaguraJumpSkill_MoveState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		if (m_bNextAnim == true)
			return new CIdleState();
	}

	if (pTanjiro->Get_Model()->Get_CurrentTime() <= 53.f)
		Move(pTanjiro, fTimeDelta);
	else
		Jump(pTanjiro, fTimeDelta);




	return nullptr;
}

CTanjiroState * CKaguraJumpSkill_MoveState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	pTanjiro->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CKaguraJumpSkill_MoveState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_JUMP;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_MOVE);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_MOVE);
	Initialize_value(pTanjiro);
}

void CKaguraJumpSkill_MoveState::Exit(CTanjiro * pTanjiro)
{
	

}

void CKaguraJumpSkill_MoveState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vTranslationTargetPos = XMVectorSetY(vTargetPosition, XMVectorGetY(vMyPosition));
	_float fDistance = XMVectorGetX(XMVector3Length(vTranslationTargetPos - vMyPosition));
	_vector vDirection = XMVector3Normalize(vTranslationTargetPos - vMyPosition);

	static _float fGravity = 9.8f;

	m_vVelocity.x += fGravity * fTimeDelta;
	//m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.x += XMVectorGetX(vDirection) *   m_vVelocity.x  * fTimeDelta;
	m_vPosition.z += XMVectorGetZ(vDirection) *   m_vVelocity.z * fTimeDelta;

	_vector vCurrentPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, XMVectorGetY(vCurrentPos), m_vPosition.z, 1.f);

	//if (fDistance <= 3.f)
	//{
	//	m_bNextAnim = true;
	//}
	if (fDistance <= 3.f)
	{
		m_vVelocity.x = 0.f;
		m_vVelocity.y = 0.f;
		m_vVelocity.z = 0.f;
	}
	else
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

}

void CKaguraJumpSkill_MoveState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
{
	static _float fGravity = -100.f;
	static _float fVelocity = 0.f;
	static _float3 vPosition;

	vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));



	m_vVelocity.y += fGravity *fTimeDelta;
	vPosition.y += m_vVelocity.y * fTimeDelta;

	_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vecPos = XMVectorSetY(vecPos, vPosition.y);

	if (vPosition.y <= m_fOriginPosY)
	{
		vPosition.y = m_fOriginPosY;
		fVelocity = m_fOriginPosY;

		_vector vecPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
		vecPos = XMVectorSetY(vecPos, vPosition.y);

		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
		{
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);

			m_bNextAnim = true;
		}
	}
	else
	{
		if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vecPos))
			pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vecPos);
	}

}

void CKaguraJumpSkill_MoveState::Initialize_value(CTanjiro * pTanjiro)
{
	m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 1.f;
	m_vVelocity.y = 1.f;
	m_vVelocity.z = 1.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pShinobu->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;

	_vector vMyPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	m_vTargetPosition = XMVector3Normalize(vTargetPosition - vMyPosition);
	vMyPosition = XMVectorSetY(vMyPosition, 0.f);
	vTargetPosition = XMVectorSetY(vTargetPosition, 0.f);
	m_fDistance = XMVectorGetX(XMVector3Length(vTargetPosition - vMyPosition));
;
}

