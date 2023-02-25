#include "stdafx.h"
#include "EnmuUpperHitState.h"
#include "EnmuIdleState.h"
#include "GameInstance.h"
#include "Layer.h"
#include "Camera_Dynamic.h"

using namespace Enmu;

CUpperHitState::CUpperHitState(CEnmu::HIT_TYPE eHitType, STATE_TYPE eType, _float fBoundPower, _float fJumpPower, _float fKnockBackPower, _float fJumpTime)
	: m_fBoundPower(fBoundPower), m_fJumpPower(fJumpPower), m_fKnockBackPower(fKnockBackPower), m_fJumpTime(fJumpTime)
{
	m_eStateType = eType;
	m_eHitType = eHitType;
}

CEnmuState * CUpperHitState::HandleInput(CEnmu* pEnmu)
{

	return nullptr;
}

CEnmuState * CUpperHitState::Tick(CEnmu* pEnmu, _float fTimeDelta)
{
	if (!m_bReset && m_eStateType == STATE_TYPE::TYPE_START)
	{
		if (pEnmu->Get_PlayerInfo().iUnicCount < 3 && pEnmu->Get_PlayerInfo().iUnicBar < pEnmu->Get_PlayerInfo().iUnicMaxBar)
		{
			pEnmu->Set_UnicBar(33);
			if (pEnmu->Get_PlayerInfo().iUnicBar >= pEnmu->Get_PlayerInfo().iUnicMaxBar)
			{
				if (pEnmu->Get_PlayerInfo().iUnicCount < 3)
				{
					pEnmu->Reset_UnicBar();
					pEnmu->Set_UnicCount(1);
				}
				else
					pEnmu->Set_UnicBar(pEnmu->Get_PlayerInfo().iUnicMaxBar);
			}
		}
		m_bReset = true;
	}

	m_fCurrentDuration += (1.f / 60.f);
	if (m_fCurrentDuration >= 2.f)
	{
		g_bDeathTime = false;
		m_fCurrentDuration = 0.f;
	}

	switch (m_eHitType)
	{
	case Client::CCharacters::HIT_UPPER:
		return UpperState(pEnmu, fTimeDelta);
		break;
	case Client::CCharacters::HIT_BOUND:
		return BoundState(pEnmu, fTimeDelta);
		break;
	case Client::CCharacters::HIT_KNOCKBACK:
		return KnockBackState(pEnmu, fTimeDelta);
		break;
	case Client::CCharacters::HIT_UPPER_2:
		return Upper2State(pEnmu, fTimeDelta);
		break;
	case Client::CCharacters::HIT_END:
		break;
	default:
		break;
	}





	return nullptr;
}

CEnmuState * CUpperHitState::Late_Tick(CEnmu* pEnmu, _float fTimeDelta)
{


	pEnmu->Get_Model()->Play_Animation(fTimeDelta);


	return nullptr;
}

void CUpperHitState::Enter(CEnmu* pEnmu)
{
	m_eStateId = STATE_ID::STATE_JUMP;

	m_vVelocity = { 10.f,10.f,10.f };

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_UPPER_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_UPPER_0);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.01f);

		if (pEnmu->Get_PlayerInfo().iHp <= 0)
		{
			CGameInstance* pGameInstanceCam = GET_INSTANCE(CGameInstance);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Set_Zoom(CCamera_Dynamic::ZOOM_LOW);
			dynamic_cast<CCamera_Dynamic*>(pGameInstanceCam->Find_Layer(LEVEL_GAMEPLAY, TEXT("Layer_Camera"))->Get_LayerFront())->Blur_Low(pEnmu->Get_Renderer());
			RELEASE_INSTANCE(CGameInstance);
			g_bDeathTime = true;
		}
		break;
	case Client::CEnmuState::TYPE_LOOP:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_UPPER_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_UPPER_1);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex(), true);
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.01f);
		break;
	case Client::CEnmuState::TYPE_END:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_UPPER_2);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_UPPER_2);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.01f);
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_DOWN_0);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_DOWN_0);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.01f);
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		pEnmu->Get_Model()->Set_CurrentAnimIndex(CEnmu::ANIMID::ANIM_HIT_DMG_DOWN_1);
		pEnmu->Set_AnimIndex(CEnmu::ANIM_HIT_DMG_DOWN_1);
		pEnmu->Get_Model()->Set_Loop(pEnmu->Get_AnimIndex());
		pEnmu->Get_Model()->Set_LinearTime(pEnmu->Get_AnimIndex(), 0.2f);
		break;
	}


}



void CUpperHitState::Increase_Height(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pEnmu->Get_NavigationHeight().y;
	pEnmu->Get_Transform()->Set_Jump(true);

	static _float fJump_Velocity = 10.f;
	static _float fGravity = 50.f;


	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	vMyPosition = XMVectorSetY(vMyPosition, XMVectorGetY(vTargetPosition));

	_vector vLookAt = XMVector3Normalize(vTargetPosition - vMyPosition) * -1.f;



	m_vVelocity.x += fGravity * fTimeDelta;
	m_vVelocity.y += fGravity * fTimeDelta;
	m_vVelocity.z += fGravity * fTimeDelta;

	m_vPosition.y += m_vVelocity.y * fTimeDelta;


	//m_vPosition.x += m_vVelocity.x * XMVectorGetX(vLookAt) *  fTimeDelta;
	//m_vPosition.z += m_vVelocity.z * XMVectorGetZ(vLookAt) *  fTimeDelta;

	//_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);

	if (XMVectorGetY(vCurrentPos) > 5.f)
	{
		/*	fDelay += fTimeDelta;

		if(fDelay >= 0.3f)*/
		m_bNextAnim = true;
	}
	else
	{
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			m_vPosition.y += m_vVelocity.y * fTimeDelta;
			vec = XMVectorSetY(vec, m_vPosition.y);
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}
}

void CUpperHitState::Fall_Height(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pEnmu->Get_NavigationHeight().y;


	m_fJumpTime += 0.035f;

	static _float fGravity = 50.f;
	static _float fVelocity = 21.25f;
	static _float fStartHeight = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	//fStartHeight + m_vVelocity.y * fTimeDelta + 0.5 * fGravity * fTimeDelta * fTimeDelta;


	// m_vVelocity.x = velocity

	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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

	//_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;
		fVelocity = m_fCurrentPosY;

		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pEnmu->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::KnockBack_Player(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pEnmu->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	static _float fVelocity = 5.f;

	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);


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


	//_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pEnmu->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Bound_Player(CEnmu* pEnmu, _float fTimeDelta)
{
	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pEnmu->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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


	//_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pEnmu->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}

}

void CUpperHitState::Upper_Player(CEnmu* pEnmu, _float fTimeDelta)
{


	pEnmu->Set_NavigationHeight(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_fCurrentPosY = pEnmu->Get_NavigationHeight().y;
	m_fJumpTime += 0.035f;

	static _float fStartHeight = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	m_vPosition.x = XMVectorGetX(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.y = XMVectorGetY(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));
	m_vPosition.z = XMVectorGetZ(pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION));

	_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vMyPosition = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
	_vector vTargetPosition = pEnmu->Get_BattleTarget()->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

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


	//_vector vCurrentPos = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);

	_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);


	if (m_vPosition.y <= m_fCurrentPosY)
	{
		m_vPosition.y = m_fCurrentPosY;


		_vector vPosition = XMVectorSet(m_vPosition.x, m_vPosition.y, m_vPosition.z, 1.f);
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);

		m_bNextAnim = true;

		pEnmu->Get_Transform()->Set_Jump(false);
	}
	else
	{
		if (pEnmu->Get_NavigationCom()->Cheak_Cell(vPosition))
		{
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vPosition);
		}
		else
		{
			_vector vec = pEnmu->Get_Transform()->Get_State(CTransform::STATE_TRANSLATION);
			vec = XMVectorSetY(vec, m_vPosition.y);
			pEnmu->Get_Transform()->Set_State(CTransform::STATE_TRANSLATION, vec);
		}
	}





}

CEnmuState * CUpperHitState::KnockBackState(CEnmu* pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			pEnmu->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_END:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
				return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CEnmuState::TYPE_CHANGE:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		KnockBack_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		KnockBack_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_END:
		KnockBack_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}





	return nullptr;
}

CEnmuState * CUpperHitState::BoundState(CEnmu* pEnmu, _float fTimeDelta)
{

	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			pEnmu->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_END:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			if (m_bNextAnim == true)
			{
			
				return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			}
			break;
		case Client::CEnmuState::TYPE_CHANGE:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			if(m_fDelay >= 0.5f)
			return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		Bound_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		Bound_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_END:
		Bound_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}

CEnmuState * CUpperHitState::UpperState(CEnmu* pEnmu, _float fTimeDelta)
{

	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_END:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			
			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_CHANGE:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());

			if(m_fDelay >=0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}

	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		Upper_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		Upper_Player(pEnmu, fTimeDelta);

		if (m_bNextAnim)
		{
			pEnmu->Set_GodMode(true);
			return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
		}
		break;
	case Client::CEnmuState::TYPE_END:
		//Bound_Player(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}

	return nullptr;
}

CEnmuState * CUpperHitState::Upper2State(CEnmu * pEnmu, _float fTimeDelta)
{
	if (pEnmu->Get_Model()->Get_End(pEnmu->Get_AnimIndex()))
	{
		switch (m_eStateType)
		{
		case Client::CEnmuState::TYPE_START:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			return new CUpperHitState(m_eHitType, TYPE_LOOP, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_LOOP:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
			//return new CUpperHitState(m_eHitType, TYPE_END, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_END:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());

			return new CUpperHitState(m_eHitType, TYPE_CHANGE, m_fBoundPower, m_fJumpPower, m_fKnockBackPower, m_fJumpTime);
			break;
		case Client::CEnmuState::TYPE_CHANGE:
			pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());

			if (m_fDelay >= 0.5f)
				return new CIdleState(STATE_HIT);
			break;
		default:
			break;
		}
		pEnmu->Get_Model()->Set_End(pEnmu->Get_AnimIndex());
	}





	switch (m_eStateType)
	{
	case Client::CEnmuState::TYPE_START:
		Increase_Height(pEnmu, fTimeDelta);
		break;
	case Client::CEnmuState::TYPE_LOOP:
		Fall_Height(pEnmu, fTimeDelta);

		if (m_bNextAnim == true)
			return new CUpperHitState(m_eHitType, TYPE_END, 0.f, 0.f, 0.f);
		break;
	case Client::CEnmuState::TYPE_END:
		break;
	case Client::CEnmuState::TYPE_DEFAULT:
		break;
	case Client::CEnmuState::TYPE_CHANGE:
		m_fDelay += fTimeDelta;
		break;
	default:
		break;
	}



	return nullptr;
}


void CUpperHitState::Exit(CEnmu* pEnmu)
{
	pEnmu->Set_HitTime(0.3f);
	//pEnmu->Get_Model()->Reset_Anim(pEnmu->Get_AnimIndex());
}



