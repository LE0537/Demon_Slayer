#pragma once

#include "Client_Defines.h"
#include "Base.h"


BEGIN(Client)

class CCinema_Manager : public CBase
{
	DECLARE_SINGLETON(CCinema_Manager)
public:
	CCinema_Manager();
	virtual ~CCinema_Manager() = default;





public:
	virtual void Free() override;
};

END