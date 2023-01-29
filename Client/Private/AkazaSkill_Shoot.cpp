#include "stdafx.h"
#include "AkazaSkill_Shoot.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Effect_Manager.h"

using namespace Akaza;


CSkill_ShootState::CSkill_ShootState(STATE_TYPE eType)
{
	m_eStateType = eType;
	



}

CAkazaState * CSkill_ShootState::HandleInput(CAkaza* pAkaza)
{
	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pAkaza->Get_i1P())
	{
	case 1:
		if (pGameInstance->Key_Down(DIK_I))
			m_bCombo = true;
		break;
	case 2:
		if (pGameInstance->Key_Down(DIK_X))
			m_bCombo = true;
		break;
	}
	


	return nullptr;
}

CAkazaState * CSkill_ShootState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			if(m_bCombo == true)
				return new CSkill_ShootState(CAkazaState::TYPE_LOOP);
			else if(m_bCombo ==false)
				return new CSkill_ShootState(CAkazaState::TYPE_END);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CSkill_ShootState(CAkazaState::TYPE_END);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CIdleState();
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	return nullptr;
}

CAkazaState * CSkill_ShootState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	CGameInstance*		pGameInstance = GET_INSTANCE(CGameInstance);

	CCharacters* m_pTarget = pAkaza->Get_BattleTarget();
	_vector vLooAt = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	vLooAt.m128_f32[1] = 0.f;
	pAkaza->Get_Transform()->LookAt(vLooAt);

	m_fDelay += fTimeDelta;
	if(m_fDelay > 0.23f)
		m_fMove += fTimeDelta;
	
	CAkazaShoot::AKAZASHOOTINFO	tInfo;
	tInfo.pPlayer = pAkaza;
	tInfo.pTarget = m_pTarget;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		if (m_fMove > 0.23f && m_iHit < 3)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_AkazaShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fMove = 0.f;
			++m_iHit;
		}
		break;
	case Client::CAkazaState::TYPE_LOOP:
		if (m_fDelay > 0.15f && m_iHit < 2)
		{
			CGameInstance*		pGameInstance2 = GET_INSTANCE(CGameInstance);

			if (FAILED(pGameInstance2->Add_GameObject(TEXT("Prototype_GameObject_AkazaShoot"), LEVEL_STATIC, TEXT("Layer_CollBox"), &tInfo)))
				return nullptr;

			RELEASE_INSTANCE(CGameInstance);
			m_fDelay = 0.f;
			++m_iHit;
		}
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	}


	RELEASE_INSTANCE(CGameInstance);

	pAkaza->Get_Model()->Play_Animation(fTimeDelta);

	return nullptr;
}

void CSkill_ShootState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_SKILL_SHOOT;

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_SHOOT_0);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_SHOOT_0, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_SHOOT_0);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_SHOOT_1);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_SHOOT_1, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_SHOOT_1);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIM_SKILL_SHOOT_2);
		pAkaza->Get_Model()->Set_LinearTime(CAkaza::ANIM_SKILL_SHOOT_2, 0.01f);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_SKILL_SHOOT_2);
		break;
	}

}

void CSkill_ShootState::Exit(CAkaza* pAkaza)
{
	
}

