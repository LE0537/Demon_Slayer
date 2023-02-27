#include "stdafx.h"
#include "EnmuAttack_3.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "EnmuIdleState.h"
#include "SoundMgr.h"

using namespace Enmu;

CEnmuAttack3::CEnmuAttack3(STATE_TYPE eType)
{
	m_eStateType = eType;
}

CEnmuState * CEnmuAttack3::HandleInput(CEnmu* pEnmu)
{
	return nullptr;
}

CEnmuState * CEnmuAttack3::Tick(CEnmu* pEnmu, _float fTimeDelta)
{


	if (pEnmu->Get_Model()->Get_End(CEnmu::ANIM_ATTACK_3))
	{
		pEnmu->Get_Model()->Set_End(CEnmu::ANIM_ATTACK_3);
		return new CIdleState();
	}






	return nullptr;
}

CEnmuState * CEnmuAttack3::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{

	pEnmu->Get_Model()->Play_Animation(fTimeDelta);



	return nullptr;
}

void CEnmuAttack3::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_ATK_3;
	pEnmu->Get_Model()->Reset_Anim(CEnmu::ANIMID::ANIM_ATTACK_3);
	pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_ATTACK_3);
	pEnmu->Set_AnimIndex(CEnmu::ANIM_ATTACK_3);
	pEnmu->Get_Model()->Set_LinearTime(CEnmu::ANIM_ATTACK_3, 0.01f);
	pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
	CSoundMgr::Get_Instance()->PlayEffect(TEXT("Enmu_SE_Attack_2.wav"), fEFFECT);

	_uint iRand = rand() % 2;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Attack2_0.wav"), fVOICE);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayVoice(TEXT("Enmu_Attack2_1.wav"), fVOICE);
}

void CEnmuAttack3::Exit(CEnmu* pEnmu)
{
}

