#include "Widgets/CM_ActivableWidget_Base.h"

#include "Controllers/CM_PlayerController_Base.h"

ACM_PlayerController_Base* UCM_ActivableWidget_Base::GetOwningCommonMenuPlayerController()
{
	if (CachedOwningPlayerController)
	{
		return  CachedOwningPlayerController.Get();
	}

	CachedOwningPlayerController = GetOwningPlayer<ACM_PlayerController_Base>();

	return CachedOwningPlayerController? CachedOwningPlayerController.Get() : nullptr;
}
