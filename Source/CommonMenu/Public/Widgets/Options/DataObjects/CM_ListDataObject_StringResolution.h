#pragma once

#include "CoreMinimal.h"
#include "CM_ListDataObject_String.h"
#include "CM_ListDataObject_StringResolution.generated.h"

UCLASS()
class COMMONMENU_API UCM_ListDataObject_StringResolution : public UCM_ListDataObject_String
{
	GENERATED_BODY()

public:
	void InitResolutionValues();

	FORCEINLINE FString GetMaximumAllowedResolution() const { return MaximumAllowedResolution; }

protected:
	/*-- UCM_ListDataObject_String --*/
	virtual void OnDataObjectInitialized() override;
	/*-- End UCM_ListDataObject_String --*/

private:
	FString ResolutionToValueString(const FIntPoint& Resolution) const;
	FText ResolutionToDisplayText(const FIntPoint& Resolution) const;

	FString MaximumAllowedResolution{};
};
