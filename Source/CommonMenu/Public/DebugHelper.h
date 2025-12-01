#pragma once

namespace DebugHelper
{
	static void Print(const FString& Msg, int32 Key = -1, const float TimeToDisplay = 7.f, const FColor& Color = FColor::MakeRandomColor())
	{
		if (!GEngine)
		{
			return;
		}

		GEngine->AddOnScreenDebugMessage(Key, TimeToDisplay, Color, Msg);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}
}