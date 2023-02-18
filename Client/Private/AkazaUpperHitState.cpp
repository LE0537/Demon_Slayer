#include "stdafx.h"
#include "AkazaUpperHitState.h"
#include "AkazaIdleState.h"
#include "GameInstance.h"

using namespace Akaza;

CUpperHitState::CUpperHitState(CAkaza::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime)
	: m_fBoundPower(fBoundPower), m_fJumpPower(fJumpPower), m_fKnockBackPower(fKnockBackPower), m_fJumpTime(fJumpTime)
{
	m_eStateType = eType;
	m_eHitType = eHitType;
}

CAkazaState * CUpperHitState::HandleInput(CAkaza* pAkaza)
{

	return nullptr;
}

CAkazaState * CUpperHitState::Tick(CAkaza* pAkaza, _float fTimeDelta)
{
	if (!m_bReset && m_eStateType == STATE_TYPE::TYPE_START)
	{
		if (pAkaza->Get_PlayerInfo().iUnicCount < 3 && pAkaza->Get_PlayerInfo().iUnicBar < pAkaza->Get_PlayerInfo().iUnicMaxBar)
		{
			pAkaza->Set_UnicBar(33);
			if (pAkaza->Get_PlayerInfo().iUnicBar >= pAkaza->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pAkaza->Get_PlayerInfo().iUnicCount < 3)
				{
					pAkaza->Reset_UnicBar();
					pAkaza->Set_UnicCount(1);
				}
				else
					pAkaza->Set_UnicBar(pAkaza->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		m_bReset = true;
	}



	switch (m_eHitType)
	{
	case Client::CCharacters::HIT_UPPER:
		return UpperState(pAkaza, fTimeDelta);
		break;
	case Client::CCharacters::HIT_BOUND:
		return BoundState(pAkaza, fTimeDelta);
		break;
	case Client::CCharacters::HIT_KNOCKBACK:
		return KnockBackState(pAkaza, fTimeDelta);
		break;
	case Client::CCharacters::HIT_UPPER_2:
		return Upper2State(pAkaza, fTimeDelta);
	case Client::CCharacters::HIT_END:
		break;
	default:
		break;
	}


	return nullptr;
}

CAkazaState * CUpperHitState::Late_Tick(CAkaza* pAkaza, _float fTimeDelta)
{


	pAkaza->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

void CUpperHitState::Enter(CAkaza* pAkaza)
{
	m_eStateId = STATE_ID::STATE_JUMP;

	m_vVelocity = { 10.f,10.f,10.f };

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_UPPER_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_UPPER_0);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_UPPER_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_UPPER_1);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex(), true);
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_END:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_UPPER_2);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_UPPER_2);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_DOWN_0);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_DOWN_0);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.01f);
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		pAkaza->Get_Model()->Set_CurrentAnimIndex(CAkaza::ANIMID::ANIM_HIT_DMG_DOWN_1);
		pAkaza->Set_AnimIndex(CAkaza::ANIM_HIT_DMG_DOWN_1);
		pAkaza->Get_Model()->Set_Loop(pAkaza->Get_AnimIndex());
		pAkaza->Get_Model()->Set_LinearTime(pAkaza->Get_AnimIndex(), 0.2f);
		break;
	}

	_uint iRand = rand() % 4;

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_2.wav"), fEFFECT);
	else if (iRand == 2)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_3.wav"), fEFFECT);
	else if (iRand == 3)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("Tanjiro_Hit1_4.wav"), fEFFECT);

	if (iRand == 0)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff1.wav"), fEFFECT);
	else if (iRand == 1)
		CSoundMgr::Get_Instance()->PlayEffect(TEXT("FightEff2.wav"), fEFFECT);



}



void CUpperHitState::Increase_Height(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pAkaza->Get_NavigationHeight().y;
	pAkaza->Get_Transform()->Set_Jump(true);

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 50.f;


	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;



	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.y += m_vVelocity.y * fTimeDelta;


	//m_vPosition.x += m_vVelocity.x * XMVectorGetX(vLookAt) *  fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * XMVectorGetZ(vLookAt) *  fTimeDelta;

	//_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);



	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{
		/*	fDelay += fTimeDelta;

		if(fDelay >= 0.3f)*/
		m_bNextAnim = true;
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_vPosition.y += m_vVelocity.y * fTimeDelta;
			vec = XMVectorSetY(vec, m_vPosition.y);
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}
}

void CUpperHitState::Fall_Height(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pAkaza->Get_NavigationHeight().y;


	m_fJumpTime += 0.035f;

	static _float fGravity = 50.f;
	static _float fVelocity = 21.25f;
	static _float fStartHeight = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	//fStartHeight + m_vVelocity.y * fTimeDelta + 0.5 * fGravity * fTimeDelta * fTimeDelta;


	// m_vVelocity.x = velocity

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;



	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	//m_vVelocity.y += fGravity * fTimeDelta * 0.5f;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;

	m_vPosition.y -= m_vVelocity.y * fTimeDelta;

	m_vVelocity.x = 10.f;
	m_vVelocity.z = 10.f;

	m_vPosition.x += XMVectorGetX(vLookAt) * m_vVelocity.x * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_vVelocity.z * fTimeDelta * 1.1f;

	m_vVelocity.y = 10.f;
	m_vPosition.y = fStartHeight + m_vVelocity.y * m_fJumpTime - (0.5f * 15.f * m_fJumpTime * m_fJumpTime);

	//_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;
		fVelocity = m_fCurrentPosY;

		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pAkaza->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::KnockBack_Player(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pAkaza->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fVelocity = 5.f;

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;

	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	m_vVelocity.y += 50.f * fTimeDelta;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;



	m_vPosition.x += XMVectorGetX(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;

	m_vPosition.y = fStartHeight + m_fBoundPower * m_fJumpTime - (0.5f * m_fJumpPower * m_fJumpTime * m_fJumpTime);

	m_vPosition.y -= 3.f;

	//m_vPosition.y -= XMVectorGetY(vTest) * fTimeDelta;

	//m_vPosition.y = fStartHeight + 30.f * m_fJumpTime - (0.5 * 40.f * m_fJumpTime * m_fJumpTime);
#ifdef _DEBUG
	printf("PosY : %f, knockbackPower : %f, BoundPower : %f, jumpTime : %f, jumpPower : %f \n", m_vPosition.y, m_fKnockBackPower, m_fBoundPower, m_fJumpTime, m_fJumpPower);
#endif // _DEBUG


	//_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pAkaza->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Bound_Player(CAkaza* pAkaza, _float fTimeDelta)
{
	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pAkaza->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;

	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	//m_vVelocity.y += fGravity * fTimeDelta * 0.5f;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;

	m_vPosition.x += XMVectorGetX(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;



	m_vPosition.y = fStartHeight + m_fBoundPower * m_fJumpTime - (0.5f * m_fJumpPower * m_fJumpTime * m_fJumpTime);

#ifdef _DEBUG
	printf("PosY : %f, knockbackPower : %f, BoundPower : %f, jumpTime : %f, jumpPower : %f \n", m_vPosition.y, m_fKnockBackPower, m_fBoundPower, m_fJumpTime, m_fJumpPower);
#endif // _DEBUG


	//_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pAkaza->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Upper_Player(CAkaza* pAkaza, _float fTimeDelta)
{


	pAkaza->Set_NavigationHeight(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pAkaza->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pAkaza->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;

	//m_vVelocity.x += fGravity * fTimeDelta * 0.8f;
	//m_vVelocity.y += fGravity * fTimeDelta * 0.5f;
	//m_vVelocity.z += fGravity * fTimeDelta * 0.8f;

	// bound = 10
	// jump power 15

	m_vPosition.x += XMVectorGetX(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;
	m_vPosition.z += XMVectorGetZ(vLookAt) * m_fKnockBackPower * fTimeDelta * 1.1f;

	m_vPosition.y = fStartHeight + m_fBoundPower * m_fJumpTime - (0.5f * m_fJumpPower * m_fJumpTime * m_fJumpTime);

#ifdef _DEBUG
	printf("PosY : %f, knockbackPower : %f, BoundPower : %f, jumpTime : %f, jumpPower : %f \n", m_vPosition.y, m_fKnockBackPower, m_fBoundPower, m_fJumpTime, m_fJumpPower);
#endif // _DEBUG


	//_vector vCurrentPos = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pAkaza->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pAkaza->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pAkaza->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pAkaza->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}





}

CAkazaState * CUpperHitState::KnockBackState(CAkaza* pAkaza, _float fTimeDelta)
{
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			pAkaza->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
				return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CAkazaState::TYPE_CHANGE:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		KnockBack_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		KnockBack_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_END:
		KnockBack_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}





	return nullptr;
}

CAkazaState * CUpperHitState::BoundState(CAkaza* pAkaza, _float fTimeDelta)
{

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			pAkaza->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
			
				return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CAkazaState::TYPE_CHANGE:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
			return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		Bound_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		Bound_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_END:
		Bound_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}

CAkazaState * CUpperHitState::UpperState(CAkaza* pAkaza, _float fTimeDelta)
{

	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			
			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_CHANGE:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			if(m_fDelay >=0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		Upper_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		Upper_Player(pAkaza, fTimeDelta);

		if (m_bNextAnim)
		{
			pAkaza->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
		}
		break;
	case Client::CAkazaState::TYPE_END:
		//Bound_Player(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}

	return nullptr;
}

CAkazaState * CUpperHitState::Upper2State(CAkaza * pAkaza, _float fTimeDelta)
{
	if (pAkaza->Get_Model()->Get_End(pAkaza->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CAkazaState::TYPE_START:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_LOOP:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_END:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CAkazaState::TYPE_CHANGE:
			pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());

			if (m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pAkaza->Get_Model()->Set_End(pAkaza->Get_AnimIndex());
	}





	switch (m_eStateType)
	{
	case Client::CAkazaState::TYPE_START:
		Increase_Height(pAkaza, fTimeDelta);
		break;
	case Client::CAkazaState::TYPE_LOOP:
		Fall_Height(pAkaza, fTimeDelta);

		if (m_bNextAnim == true)
			return new CUpperHitState(m_eHitType, TYPE_END, 0.f, 0.f, 0.f);
		break;
	case Client::CAkazaState::TYPE_END:
		break;
	case Client::CAkazaState::TYPE_DEFAULT:
		break;
	case Client::CAkazaState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}


void CUpperHitState::Exit(CAkaza* pAkaza)
{
	pAkaza->Set_HitTime(0.3f);
	//pAkaza->Get_Model()->Reset_Anim(pAkaza->Get_AnimIndex());
}



