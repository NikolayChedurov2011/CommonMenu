#pragma once

#include "CoreMinimal.h"
#include "CommonRotator.h"
#include "CM_CommonRotator.generated.h"

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_CommonRotator : public UCommonRotator
{
	GENERATED_BODY()

public:
	void SetSelectedOptionByText(const FText& OptionText);
	
private:
};
