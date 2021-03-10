// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AsyncGEAddedRemoved.h"
#include "Characters/Abilities/GDAbilitySystemComponent.h"


UAsyncGEAddedRemoved* UAsyncGEAddedRemoved::ListenForGEAddedRemoved(UAbilitySystemComponent* AbilitySystemComponent, TSubclassOf<UGameplayEffect> TagsOfGE)
{
	UAsyncGEAddedRemoved* Listener = NewObject<UAsyncGEAddedRemoved>();
	UGDAbilitySystemComponent* GDASC = Cast<UGDAbilitySystemComponent>(AbilitySystemComponent);
	Listener->ASC = GDASC;
	Listener->TargetGE = TagsOfGE.GetDefaultObject();


	if (!IsValid(GDASC) || TagsOfGE == nullptr)
	{
		//Listener->End();
		Listener->EndTask();
		return nullptr;
	}

	GDASC->OnRealAnyGameplayEffectAddedDelegate.AddUObject(Listener, &UAsyncGEAddedRemoved::OnActiveGameplayEffectAddedCallback);
	if (!GDASC->OnRealAnyGameplayEffectRemovedDelegate.IsBoundToObject(Listener))
	{
		GDASC->OnRealAnyGameplayEffectRemovedDelegate.AddUObject(Listener, &UAsyncGEAddedRemoved::OnRemoveGameplayEffectCallback);
	}
	return Listener;
}

void UAsyncGEAddedRemoved::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	if (SpecApplied.Def == this->TargetGE)
	{
		OnGEAdded.Broadcast();
	}
}

void UAsyncGEAddedRemoved::OnRemoveGameplayEffectCallback(const FGameplayEffectSpec& EffectRemoved)
{
	if (EffectRemoved.Def == this->TargetGE)
	{
		OnGERemoved.Broadcast();
	}
}

void UAsyncGEAddedRemoved::EndTask()
{
	if (IsValid(ASC))
	{
		ASC->OnRealAnyGameplayEffectAddedDelegate.RemoveAll(this);
		ASC->OnRealAnyGameplayEffectRemovedDelegate.RemoveAll(this);
	}
	SetReadyToDestroy();
	MarkPendingKill();
}
