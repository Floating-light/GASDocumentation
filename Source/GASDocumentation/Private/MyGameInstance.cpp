// Copyright 2020 Dan Kestranek.


#include "MyGameInstance.h"

void UMyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Purple, FString::Printf(TEXT("Load time cost : %f,  MapName: %s"), LoadTime, *MapName));
	}

	//FLoadingScreenAttributes 
}
