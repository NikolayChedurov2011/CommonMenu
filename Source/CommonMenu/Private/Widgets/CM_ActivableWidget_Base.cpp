#include "Widgets/CM_ActivableWidget_Base.h"

APlayerController* UCM_ActivableWidget_Base::GetOwningCommonMenuPlayerController()
{
	if (CachedOwningPlayerController)
	{
		return  CachedOwningPlayerController.Get();
	}

	CachedOwningPlayerController = GetOwningPlayer<APlayerController>();

	return CachedOwningPlayerController? CachedOwningPlayerController.Get() : nullptr;
}
