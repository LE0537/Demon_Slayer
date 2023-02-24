#include "stdafx.h"
#include "TanjiroJumpstate.h"
#include "GameInstance.h"
#include "TanjiroIdleState.h"
#include "TanjiroMoveState.h"
#include "TanjiroJumpAttackState.h"
#include "TanjiroJumpSkillCommonState.h"
#include "TanjiroKaguraJumpSkill_Common.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"
using namespace Tanjiro;

CJumpstate::CJumpstate(STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CTanjiroState * CJumpstate::HandleInput(CTanjiro * pTanjiro)
{
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		CGameInstance* pGameInstance = CGameInstance::Get_Instance();

		switch (pTanjiro->Get_i1P())
		{
		case 1:
			if (pGameInstance->Key_Down(DIK_I))
			{
				if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
				{
					if (pTanjiro->Get_KaguraMode() == true)
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraJumpSkill_CommonState();
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CJumpSkillCommonState(TYPE_START);
					}
				}

			}
			else if (pGameInstance->Key_Down(DIK_J))
			{
				return new CJumpAttackState(TYPE_START);
			}
			break;
		case 2:
			if (pGameInstance->Key_Down(DIK_X))
			{
				if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
				{
					if (pTanjiro->Get_KaguraMode() == true)
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraJumpSkill_CommonState();
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CJumpSkillCommonState(TYPE_START);
					}
				}
			}
			else if (pGameInstance->Key_Down(DIK_Z))
			{
				return new CJumpAttackState(TYPE_START);
			}
			break;
		default:
			break;
		}
	}
	return nullptr;
}

CTanjiroState * CJumpstate::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // 애니메이션 총 시간
	_float fCurrentTime = 0.f; // 애니메이션 현재 시간

	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);


		if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CTanjiroState::TYPE_START:
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				return new CJumpstate(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_LOOP:
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				return new CJumpstate(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_END:
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_DEFAULT:
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				return new CIdleState(STATE_ID::STATE_JUMP);
				break;
			}
			pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
		}
	}
	else
	{
		pTanjiro->Get_ModelADV()->Set_Loop(0);
		pTanjiro->Get_ModelADV()->Set_Loop(1);
		pTanjiro->Get_ModelADV()->Set_Loop(2);
		pTanjiro->Get_ModelADV()->Set_Loop(3);


		if (pTanjiro->Get_ModelADV()->Get_End(pTanjiro->Get_ADVAnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CTanjiroState::TYPE_START:
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
				return new CJumpstate(STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_LOOP:
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
				return new CJumpstate(STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_END:
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
				//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_DEFAULT:
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
				return new CIdleState(STATE_ID::STATE_JUMP);
				break;
			}
			pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
		}
	}

	return nullptr;
}

CTanjiroState * CJumpstate::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{

	if (m_eStateType == TYPE_START)
	{
		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_UP, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 3.f);
	}
	//else if (m_eStateType == TYPE_LOOP)
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	//else if (m_eStateType == TYPE_DEFAULT)
	//	pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
	//else
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	else
		pTanjiro->Get_ModelADV()->Play_Animation(fTimeDelta);

	m_fJumpTime += 0.035f;

	if (m_eStateType != TYPE_DEFAULT)
		Jump(pTanjiro, fTimeDelta + m_fJumpTime);

	return nullptr;
}

void CJumpstate::Enter(CTanjiro * pTanjiro)
{
	m_eStateId = CTanjiroState::STATE_JUMP;

	_uint iRand = rand() % 3;
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_START);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_START, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_START);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpStart.wav"), fEFFECT);
			if (iRand == 0)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_0.wav"), fVOICE);
			else if (iRand == 1)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_1.wav"), fVOICE);
			else if (iRand == 2)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_2.wav"), fVOICE);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_START, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_END, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END, true);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpEnd.wav"), fEFFECT);
			break;
		}
	}
	else
	{
		switch (m_eStateType)
		{
		case Client::CTanjiroState::TYPE_START:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(0);
			pTanjiro->Get_ModelADV()->Set_LinearTime(0, 0.01f);
			pTanjiro->Set_ADVAnimIndex(0);
			pTanjiro->Get_ModelADV()->Set_Loop(0);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpStart.wav"), fEFFECT);
			if (iRand == 0)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_0.wav"), fVOICE);
			else if (iRand == 1)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_1.wav"), fVOICE);
			else if (iRand == 2)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_2.wav"), fVOICE);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(1);
			pTanjiro->Get_ModelADV()->Set_LinearTime(1, 0.01f);
			pTanjiro->Set_ADVAnimIndex(1);
			pTanjiro->Get_ModelADV()->Set_Loop(1);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(2);
			pTanjiro->Get_ModelADV()->Set_LinearTime(2, 0.01f);
			pTanjiro->Set_ADVAnimIndex(2);
			pTanjiro->Get_ModelADV()->Set_Loop(2, true);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(3);
			pTanjiro->Set_ADVAnimIndex(3);
			pTanjiro->Get_ModelADV()->Set_Loop(3);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpEnd.wav"), fEFFECT);
			break;
		}
	}
}

void CJumpstate::Exit(CTanjiro * pTanjiro)
{
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Get_Model()->Reset_Anim(CTanjiro::ANIM_JUMP_END);
	}
	else
	{
		pTanjiro->Get_ModelADV()->Reset_Anim(0);
		pTanjiro->Get_ModelADV()->Reset_Anim(1);
		pTanjiro->Get_ModelADV()->Reset_Anim(2);
		pTanjiro->Get_ModelADV()->Reset_Anim(3);
	}
}


CTanjiroState* CJumpstate::Jump(CTanjiro* pTanjiro, _float fTimeDelta)
{
	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pTanjiro->Get_NavigationHeight().y;
	pTanjiro->Get_Transform()->Set_Jump(true);

	static _float fStartHeight = m_fCurrentPosY;
	static _float fEndHeight = m_fCurrentPosY;
	static _float fVelocity = 20.f;
	static _float fGravity = 30.f;

	pTanjiro->Set_NavigationHeight(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	fStartHeight = pTanjiro->Get_NavigationHeight().y;
	fEndHeight = pTanjiro->Get_NavigationHeight().y;


	_vector      vPosition = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_float fSpeed = 0.f;
	fSpeed = fStartHeight + fVelocity * fTimeDelta - (0.5f * fGravity * fTimeDelta * fTimeDelta);
	vPosition = XMVectorSetY(vPosition, fSpeed);
	_float y = XMVectorGetY(vPosition);


	if (y <= fEndHeight)
	{
		vPosition = XMVectorSetY(vPosition, fEndHeight);
		m_fJumpTime = 0.f;
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		m_eStateType = CTanjiroState::TYPE_DEFAULT;
		if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		{
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_END, 0.01f);
			pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);
		}
		else
		{
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(3);
			pTanjiro->Set_ADVAnimIndex(3);
			pTanjiro->Get_ModelADV()->Set_LinearTime(3, 0.01f);
			pTanjiro->Get_ModelADV()->Set_Loop(3);
		}
		pTanjiro->Get_Transform()->Set_Jump(false);

		if (!m_bEffect)
		{
			CEffect_Manager* pEffectManger = GET_INSTANCE(CEffect_Manager);

			pEffectManger->Create_Effect(CEffect_Manager::EFF_JUMP_DOWN, pTanjiro);

			RELEASE_INSTANCE(CEffect_Manager);
			m_bEffect = true;
		}
	}
	else
		pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);


	return nullptr;
}


