#pragma once

#include "CoreMinimal.h"
#include "CommonTabListWidgetBase.h"
#include "CM_TabListWidget_Base.generated.h"

class UCM_CommonButton_Base;

UCLASS(Abstract, BlueprintType, meta=(DisableNativeTick))
class COMMONMENU_API UCM_TabListWidget_Base : public UCommonTabListWidgetBase
{
	GENERATED_BODY()
	
public:
	void RequestRegisterTab(const FName& TabID, const FText& TabDisplayName);

private:
	//~ Begin UWidget Interface
#if WITH_EDITOR	
	virtual void ValidateCompiledDefaults(class IWidgetCompilerLog& CompileLog) const override;
#endif
	//~ End UWidget Interface
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common Menu Tab List Settings", meta=(AllowPrivateAccess=true, ClampMin=1, ClampMax=10))
	int32 DebugEditorPreviewTabCount {3};
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Common Menu Tab List Settings", meta=(AllowPrivateAccess=true))
	TSubclassOf<UCM_CommonButton_Base> TabButtonEntryWidgetClass{};
};
