#pragma once

#include "CoreMinimal.h"
#include "CM_ListDataObject_Base.h"
#include "CM_ListDataObject_Collection.generated.h"

UCLASS()
class COMMONMENU_API UCM_ListDataObject_Collection : public UCM_ListDataObject_Base
{
	GENERATED_BODY()

public:
	void AddChildListData(UCM_ListDataObject_Base* ChildListData);

	/*-- UCM_ListDataObject_Base --*/
	virtual TArray<UCM_ListDataObject_Base*> GetAllChildListData() const override;
	virtual bool HasAnyChildListData() const override;
	/*-- End UCM_ListDataObject_Base --*/
	
private:

	UPROPERTY(Transient)
	TArray<UCM_ListDataObject_Base*> ChildListDataArray{};
};
