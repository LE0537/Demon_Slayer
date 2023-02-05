#include "stdafx.h"
#include "TanjiroKaguraJumpSkill_Common.h"
#include "TanjiroIdleState.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
using namespace Tanjiro;


CKaguraJumpSkill_CommonState::CKaguraJumpSkill_CommonState()
{
	CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

	

	RELEASE_INSTANCE(CGameInstance);
	//m_fHitTime = 0.1;
}

CTanjiroState * CKaguraJumpSkill_CommonState::HandleInput(CTanjiro * pTanjiro)
{

	return nullptr;
}

CTanjiroState * CKaguraJumpSkill_CommonState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	pTanjiro->Get_Model()->Set_Loop(pTanjiro->Get_AnimIndex());

	if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
	{
		pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		if (m_bNextAnim == true)
			return new CIdleState();
	}


	if (pTanjiro->Get_Model()->Get_CurrentTime() >= 17.f)
		Jump(pTanjiro, fTimeDelta);


	




	return nullptr;
}

CTanjiroState * CKaguraJumpSkill_CommonState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	 
	if (pTanjiro->Get_Model()->Get_CurrentTime() >= 17.f)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.3f);
	else
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CKaguraJumpSkill_CommonState::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = STATE_ID::STATE_SKILL_KAGURA_COMMON;

	pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIMID::ANIM_SKILL_KAGURA_COMMON);
	pTanjiro->Set_AnimIndex(CTanjiro::ANIM_SKILL_KAGURA_COMMON);
	pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_SKILL_KAGURA_COMMON, 0.01f);
	pTanjiro->Get_Transform()->Set_PlayerLookAt(pTanjiro->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	Initialize_value(pTanjiro);
}

void CKaguraJumpSkill_CommonState::Exit(CTanjiro * pTanjiro)
{
	
}

void CKaguraJumpSkill_CommonState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
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

void CKaguraJumpSkill_CommonState::Initialize_value(CTanjiro * pTanjiro)
{
	m_vPosition.x = XMVectorGetX(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vVelocity.x = 0.f;
	m_vVelocity.y = 0.f;
	m_vVelocity.z = 0.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	//m_fOriginPosY = pTanjiro->Get_NavigationHeight().y;
	m_fOriginPosY = 0.f;
}
