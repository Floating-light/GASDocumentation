// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "AsyncGEAddedRemoved.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGEAddedRemoved);

//GameplayEffectTypes.h
/**
 * 
 */
UCLASS(BlueprintType, meta = (ExposedAsyncProxy))
class GASDOCUMENTATION_API UAsyncGEAddedRemoved : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable)
	FOnGEAddedRemoved OnGEAdded;
	
	UPROPERTY(BlueprintAssignable)
	FOnGEAddedRemoved OnGERemoved;

	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UAsyncGEAddedRemoved* ListenForGEAddedRemoved(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> TagsOfGE);

	virtual void OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle);

	virtual void OnRemoveGameplayEffectCallback(const FGameplayEffectSpec& EffectRemoved);

	UFUNCTION(BlueprintCallable)
		void EndTask();
protected:
	UPROPERTY()
	class UGDAbilitySystemComponent* ASC;

	UGameplayEffect* TargetGE;
};
