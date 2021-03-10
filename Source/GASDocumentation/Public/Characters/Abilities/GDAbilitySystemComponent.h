// Copyright 2020 Dan Kestranek.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GDAbilitySystemComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FReceivedDamageDelegate, UGDAbilitySystemComponent*, SourceASC, float, UnmitigatedDamage, float, MitigatedDamage);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameplayEffectRemovedDelegate, const FGameplayEffectSpec&);
/**
 * 
 */
UCLASS()
class GASDOCUMENTATION_API UGDAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	bool CharacterAbilitiesGiven = false;
	bool StartupEffectsApplied = false;

	// ~ UAbilitySystemComponent Begin
	virtual void InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)override ;
	virtual FActiveGameplayEffectHandle ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey = FPredictionKey()) override;
	// ~ UAbilitySystemComponent End

	FReceivedDamageDelegate ReceivedDamage;

	FOnGameplayEffectAppliedDelegate OnRealAnyGameplayEffectAddedDelegate;
	FOnGameplayEffectRemovedDelegate OnRealAnyGameplayEffectRemovedDelegate;

	UFUNCTION(Client, Reliable)
		void CallClientGEAddRemoveChange(const FGameplayEffectSpec& GESpec);

	// Called from GDDamageExecCalculation. Broadcasts on ReceivedDamage whenever this ASC receives damage.
	virtual void ReceiveDamage(UGDAbilitySystemComponent* SourceASC, float UnmitigatedDamage, float MitigatedDamage);

private:
	UFUNCTION()
	void RemoveGECallback(const FActiveGameplayEffect& ActivateGE)
	{
		if (/*!bIsNetAuthority && */ActivateGE.PredictionKey.WasReceived() /*&& ActivateGE.PredictionKey.WasReceived() == false*/)
			this->OnRealAnyGameplayEffectRemovedDelegate.Broadcast(ActivateGE.Spec);
	}

};
