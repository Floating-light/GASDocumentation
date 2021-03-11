// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GASDOCUMENTATION_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	virtual void LoadComplete(const float LoadTime, const FString& MapName) override;

};
