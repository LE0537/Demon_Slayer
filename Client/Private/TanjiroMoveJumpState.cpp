#include "stdafx.h"
#include "TanjiroMoveJumpState.h"

#include "TanjiroMoveState.h"
#include "TanjiroIdlestate.h"
#include "GameInstance.h"
#include "TanjiroJumpMoveAttackState.h"
#include "TanjiroJumpSkillMoveState.h"
#include "TanjiroKaguraJumpSkill_Move.h"
#include "Effect_Manager.h"
#include "UI_Manager.h"

using namespace Tanjiro;

CMoveJumpState::CMoveJumpState(OBJDIR eDir, STATE_TYPE eType, _float fPositionY, _float fJumpTime)
{
	m_eDirection = eDir;
	m_eStateType = eType;
	m_fCurrentPosY = fPositionY;
	m_fJumpTime = fJumpTime;
}

CTanjiroState * CMoveJumpState::HandleInput(CTanjiro * pTanjiro)
{

	CGameInstance* pGameInstance = CGameInstance::Get_Instance();

	switch (pTanjiro->Get_i1P())
	{
	case 1:
		if (m_eStateType != STATE_TYPE::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Down(DIK_J))
				return new CJumpMoveAttackState(TYPE_START);
			else if (pGameInstance->Key_Down(DIK_I))
			{


				if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
				{
					if (pTanjiro->Get_KaguraMode())
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraJumpSkill_MoveState();
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 0);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CJumpSkillMoveState(TYPE_START);
					}
				}

			}

			if (pGameInstance->Key_Pressing(DIK_W)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ��
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LF;
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // ��
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RF;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_STRAIGHT;
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_S)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_A)) // ��
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LB;
				}
				else if (pGameInstance->Key_Pressing(DIK_D)) // �� 
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RB;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_BACK;
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_A)) // ��
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LEFT;
			}
			else if (pGameInstance->Key_Pressing(DIK_D)) // ��
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_RIGHT;
			}
			else
			{
				m_bMove = false;
				m_eNextDir = OBJDIR::DIR_STOP;
			}
		}
		else
		{
			m_bMove = false;
			m_eNextDir = OBJDIR::DIR_STOP;
		}
		break;
	case 2:
		if (m_eStateType != STATE_TYPE::TYPE_DEFAULT)
		{
			if (pGameInstance->Key_Down(DIK_Z))
				return new CJumpMoveAttackState(TYPE_START);
			else if (pGameInstance->Key_Down(DIK_X))
			{
				if (200 <= pTanjiro->Get_PlayerInfo().iSkBar)
				{
					if (pTanjiro->Get_KaguraMode())
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CKaguraJumpSkill_MoveState();
					}
					else
					{
						CUI_Manager* pUI_Manager = GET_INSTANCE(CUI_Manager);
						pUI_Manager->Set_UseSkillCount(1, 1);
						RELEASE_INSTANCE(CUI_Manager);
						pTanjiro->Set_SkillBar(-200);
						return new CJumpSkillMoveState(TYPE_START);
					}
				}

			}
			if (pGameInstance->Key_Pressing(DIK_UP)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LF;
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RF;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_STRAIGHT;
				}
			}

			else if (pGameInstance->Key_Pressing(DIK_DOWN)) // ��
			{
				if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_LB;
				}
				else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // �� 
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_RB;
				}
				else
				{
					m_bMove = true;
					m_eNextDir = OBJDIR::DIR_BACK;
				}
			}


			else if (pGameInstance->Key_Pressing(DIK_LEFT)) // ��
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_LEFT;
			}
			else if (pGameInstance->Key_Pressing(DIK_RIGHT)) // ��
			{
				m_bMove = true;
				m_eNextDir = OBJDIR::DIR_RIGHT;
			}
			else
			{
				m_bMove = false;
				m_eNextDir = OBJDIR::DIR_STOP;
			}
		}
		else
		{
			m_bMove = false;
			m_eNextDir = OBJDIR::DIR_STOP;
		}
		break;
	default:
		break;
	}



	return nullptr;
}

CTanjiroState * CMoveJumpState::Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fDurationTime = 0.f; // �ִϸ��̼� �� �ð�
	_float fCurrentTime = 0.f; // �ִϸ��̼� ���� �ð�

	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
	{
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_START);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_START);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_LOOP_END);
		pTanjiro->Get_Model()->Set_Loop(CTanjiro::ANIM_JUMP_END);

		if (m_eStateType == CTanjiroState::TYPE_CHANGE)
		{
			return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_LOOP);
		}


		else if (pTanjiro->Get_Model()->Get_End(pTanjiro->Get_AnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CTanjiroState::TYPE_START:
				printf_s("Start Jump \n");
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

				return new CMoveJumpState(m_eDirection, STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_LOOP:
				printf_s("Loop Jump \n");
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());

				return new CMoveJumpState(m_eDirection, STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_END:
				printf_s("End jump \n");
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_DEFAULT:
				printf_s("Default Jump \n");
				pTanjiro->Get_Model()->Set_End(pTanjiro->Get_AnimIndex());
				if (m_bMove == false)
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

		if (m_eStateType == CTanjiroState::TYPE_CHANGE)
		{
			return new CMoveState(m_eNextDir, STATE_TYPE::TYPE_LOOP);
		}
		else if (pTanjiro->Get_ModelADV()->Get_End(pTanjiro->Get_ADVAnimIndex()))
		{
			switch (m_eStateType)
			{
			case Client::CTanjiroState::TYPE_START:
				printf_s("Start Jump \n");
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());

				return new CMoveJumpState(m_eDirection, STATE_TYPE::TYPE_LOOP, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_LOOP:
				printf_s("Loop Jump \n");
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());

				return new CMoveJumpState(m_eDirection, STATE_TYPE::TYPE_END, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_END:
				printf_s("End jump \n");
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
				//return new CJumpstate(STATE_TYPE::TYPE_DEFAULT, m_fCurrentPosY, m_fJumpTime);
				break;
			case Client::CTanjiroState::TYPE_DEFAULT:
				printf_s("Default Jump \n");
				pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
				if (m_bMove == false)
					return new CIdleState(STATE_ID::STATE_JUMP);
				break;
			}
			pTanjiro->Get_ModelADV()->Set_End(pTanjiro->Get_ADVAnimIndex());
		}
	}





	

	return nullptr;
}

CTanjiroState * CMoveJumpState::Late_Tick(CTanjiro * pTanjiro, _float fTimeDelta)
{
	if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
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
			pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 3.f);
		}
		else if (m_eStateType == TYPE_LOOP)
			pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
		else if (m_eStateType == TYPE_DEFAULT)
			pTanjiro->Get_Model()->Play_Animation(fTimeDelta * 1.5f);
		else
			pTanjiro->Get_Model()->Play_Animation(fTimeDelta);
	}
	else
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
			pTanjiro->Get_ModelADV()->Play_Animation(fTimeDelta * 3.f);
		}
		else if (m_eStateType == TYPE_LOOP)
			pTanjiro->Get_ModelADV()->Play_Animation(fTimeDelta * 1.5f);
		else if (m_eStateType == TYPE_DEFAULT)
			pTanjiro->Get_ModelADV()->Play_Animation(fTimeDelta * 1.5f);
		else
			pTanjiro->Get_ModelADV()->Play_Animation(fTimeDelta);
	}
	m_fJumpTime += 0.035f;

	if (m_eStateType != TYPE_DEFAULT && m_eStateType != TYPE_CHANGE)
		Jump(pTanjiro, fTimeDelta + m_fJumpTime);

	if (m_bMove == true)
		Move(pTanjiro, fTimeDelta);

	return nullptr;
}

void CMoveJumpState::Enter(CTanjiro * pTanjiro)
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
			if (iRand == 0)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_0.wav"), g_fVoice);
			else if (iRand == 1)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_1.wav"), g_fVoice);
			else if (iRand == 2)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_2.wav"), g_fVoice);

			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpStart.wav"), g_fEffect);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_START, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_START);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
			pTanjiro->Get_Model()->Set_LinearTime(CTanjiro::ANIM_JUMP_LOOP_END, 0.01f);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_LOOP_END);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpEnd.wav"), g_fEffect);
			break;
		case Client::CTanjiroState::TYPE_CHANGE:
			pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_MOVE_LOOP);
			pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);

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
			if (iRand == 0)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_0.wav"), g_fVoice);
			else if (iRand == 1)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_1.wav"), g_fVoice);
			else if (iRand == 2)
				CSoundMgr::Get_Instance()->PlayVoice(TEXT("Tanjiro_Spirited_2.wav"), g_fVoice);

			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpStart.wav"), g_fEffect);
			break;
		case Client::CTanjiroState::TYPE_LOOP:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(1);
			pTanjiro->Get_ModelADV()->Set_LinearTime(1, 0.01f);
			pTanjiro->Set_ADVAnimIndex(1);
			break;
		case Client::CTanjiroState::TYPE_END:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(2);
			pTanjiro->Get_ModelADV()->Set_LinearTime(2, 0.01f);
			pTanjiro->Set_ADVAnimIndex(2);
			break;
		case Client::CTanjiroState::TYPE_DEFAULT:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(3);
			pTanjiro->Set_ADVAnimIndex(3);
			CSoundMgr::Get_Instance()->PlayEffect(TEXT("SE_JumpEnd.wav"), g_fEffect);
			break;
		case Client::CTanjiroState::TYPE_CHANGE:
			pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(5);
			pTanjiro->Set_ADVAnimIndex(5);

			break;
		}
	}
}

void CMoveJumpState::Exit(CTanjiro * pTanjiro)
{
}

void CMoveJumpState::Move(CTanjiro * pTanjiro, _float fTimeDelta)
{
	_float fCamAngle = pTanjiro->Get_CamAngle();

	//switch (m_eDirection)
	//{
	//case Client::DIR_STRAIGHT:
	//	pTanjiro->Get_Transform()->Set_RotationY(0.f + fCamAngle);
	//	break;
	//case Client::DIR_LEFT:
	//	pTanjiro->Get_Transform()->Set_RotationY(270.f + fCamAngle);
	//	break;
	//case Client::DIR_RIGHT:
	//	pTanjiro->Get_Transform()->Set_RotationY(90.f + fCamAngle);
	//	break;
	//case Client::DIR_BACK:
	//	pTanjiro->Get_Transform()->Set_RotationY(180.f + fCamAngle);
	//	break;
	//case Client::DIR_LF:
	//	pTanjiro->Get_Transform()->Set_RotationY(305.f + fCamAngle);
	//	break;
	//case Client::DIR_RF:
	//	pTanjiro->Get_Transform()->Set_RotationY(45.f + fCamAngle);
	//	break;
	//case Client::DIR_LB:
	//	pTanjiro->Get_Transform()->Set_RotationY(225.f + fCamAngle);
	//	break;
	//case Client::DIR_RB:
	//	pTanjiro->Get_Transform()->Set_RotationY(135.f + fCamAngle);
	//	break;
	//case Client::DIR_STOP:
	//	break;
	//}

	//switch (m_eDirection)
	//{
	//case Client::DIR_STRAIGHT:
	//	pTanjiro->Get_Transform()->Set_RotationY(0.f);
	//	break;
	//case Client::DIR_LEFT:
	//	pTanjiro->Get_Transform()->Set_RotationY(270.f);
	//	break;
	//case Client::DIR_RIGHT:
	//	pTanjiro->Get_Transform()->Set_RotationY(90.f);
	//	break;
	//case Client::DIR_BACK:
	//	pTanjiro->Get_Transform()->Set_RotationY(180.f);
	//	break;
	//case Client::DIR_LF:
	//	pTanjiro->Get_Transform()->Set_RotationY(305.f);
	//	break;
	//case Client::DIR_RF:
	//	pTanjiro->Get_Transform()->Set_RotationY(45.f);
	//	break;
	//case Client::DIR_LB:
	//	pTanjiro->Get_Transform()->Set_RotationY(225.f);
	//	break;
	//case Client::DIR_RB:
	//	pTanjiro->Get_Transform()->Set_RotationY(135.f);
	//	break;
	//case Client::DIR_STOP:
	//	break;
	//}

	if (m_eDirection != DIR_STOP && m_bMove == true)
	{
		pTanjiro->Get_Transform()->Go_Straight(fTimeDelta, pTanjiro->Get_NavigationCom());

		CCharacters* m_pTarget = pTanjiro->Get_BattleTarget();
		CCollider*	pMyCollider = pTanjiro->Get_SphereCollider();
		CCollider*	pTargetCollider = m_pTarget->Get_SphereCollider();

		if (nullptr == pTargetCollider)
			return;

		if (pMyCollider->Collision(pTargetCollider))
		{

			_float fSpeed = pTanjiro->Get_Transform()->Get_TransformDesc().fSpeedPerSec * fTimeDelta;

			_vector vTargetPos = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			_vector vPos = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

			_vector vTargetLook = XMVector3Normalize(vTargetPos - vPos);
			_vector vMyLook = vTargetLook * -1.f;

			_vector vPow = XMVector3Dot(pTanjiro->Get_Transform()->Get_State(CTransform::STATE_LOOK), vTargetLook);

			_float fPow = XMVectorGetX(XMVector3Normalize(vPow));

			vPos += vMyLook * (fSpeed - fSpeed * fPow);
			vTargetPos += vTargetLook * fSpeed * fPow;
			_vector vPlayerPosY = pTanjiro->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vPos.m128_f32[1] = vPlayerPosY.m128_f32[1];
			_vector vTargetPosY = m_pTarget->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vTargetPos.m128_f32[1] = vTargetPosY.m128_f32[1];
			if (pTanjiro->Get_NavigationCom()->Cheak_Cell(vPos))
				pTanjiro->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPos);
			if (m_pTarget->Get_NavigationCom()->Cheak_Cell(vTargetPos))
				m_pTarget->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vTargetPos);
			else
				pTanjiro->Get_Transform()->Go_Backward(fTimeDelta / 2.f, pTanjiro->Get_NavigationCom());

		}
	}
}

CTanjiroState*  CMoveJumpState::Jump(CTanjiro * pTanjiro, _float fTimeDelta)
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
		pTanjiro->Get_Transform()->Set_Jump(false);
		if (g_iLevel != LEVEL_ADVRUI && g_iLevel != LEVEL_ADVAKAZA)
		{
			if (m_bMove == false)
			{
				m_eStateType = CTanjiroState::TYPE_DEFAULT;
				pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_JUMP_END);
				pTanjiro->Set_AnimIndex(CTanjiro::ANIM_JUMP_END);
			}
			else
			{
				m_eStateType = CTanjiroState::TYPE_CHANGE;
				pTanjiro->Get_Model()->Set_CurrentAnimIndex(CTanjiro::ANIM_MOVE_LOOP);
				pTanjiro->Set_AnimIndex(CTanjiro::ANIM_MOVE_LOOP);
			}
		}
		else
		{
			if (m_bMove == false)
			{
				m_eStateType = CTanjiroState::TYPE_DEFAULT;
				pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(3);
				pTanjiro->Set_ADVAnimIndex(3);
			}
			else
			{
				m_eStateType = CTanjiroState::TYPE_CHANGE;
				pTanjiro->Get_ModelADV()->Set_CurrentAnimIndex(5);
				pTanjiro->Set_ADVAnimIndex(5);
			}
		}
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



