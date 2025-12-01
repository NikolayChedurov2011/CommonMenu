#include "Widgets/Options/DataObjects/CM_ListDataObject_StringResolution.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Settings/CM_GameUserSettings.h"

void UCM_ListDataObject_StringResolution::InitResolutionValues()
{
	TArray<FIntPoint> AvailableResolutions{};
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(AvailableResolutions);

	AvailableResolutions.Sort([](const FIntPoint& A, const FIntPoint& B)->bool
	{
		return A.SizeSquared() < B.SizeSquared();
	});
	
	for (const FIntPoint& Resolution : AvailableResolutions)
	{
		AddDynamicOption(ResolutionToValueString(Resolution), ResolutionToDisplayText(Resolution));
	}

	MaximumAllowedResolution = ResolutionToValueString(AvailableResolutions.Last());

	SetDefaultValueFromString(MaximumAllowedResolution);
}

void UCM_ListDataObject_StringResolution::OnDataObjectInitialized()
{
	Super::OnDataObjectInitialized();

	if (!TrySetDisplayTextFromStringValue(CurrentStringValue))
	{
		CurrentDisplayText = ResolutionToDisplayText(UCM_GameUserSettings::Get()->GetScreenResolution());
	}
}

FString UCM_ListDataObject_StringResolution::ResolutionToValueString(const FIntPoint& Resolution) const
{
	// Resolution value from Dynamic Getter (X=1920,Y=1080)
	return FString::Printf(TEXT("(X=%i,Y=%i)"), Resolution.X, Resolution.Y);
}

FText UCM_ListDataObject_StringResolution::ResolutionToDisplayText(const FIntPoint& Resolution) const
{
	const FString DisplayString { FString::Printf(TEXT("%i x %i"), Resolution.X, Resolution.Y) };

	return FText::FromString(DisplayString);
}
