#include "Controllers/CM_MainMenu_PlayerController.h"

#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"

#include "Settings/CM_GameUserSettings.h"


void ACM_MainMenu_PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	TArray<AActor*> FindCameras{};
	UGameplayStatics::GetAllActorsOfClassWithTag(this, ACameraActor::StaticClass(), FName{"Default"}, FindCameras);

	if (!FindCameras.IsEmpty())
	{
		SetViewTarget(FindCameras[0]);
	}

	UCM_GameUserSettings* CM_GameUserSettings { UCM_GameUserSettings::Get() };

	if (CM_GameUserSettings->GetLastCPUBenchmarkResult() == -1.f || CM_GameUserSettings->GetLastGPUBenchmarkResult() == -1.f)
	{
		CM_GameUserSettings->RunHardwareBenchmark();
		CM_GameUserSettings->ApplyHardwareBenchmarkResults();
	}
}
