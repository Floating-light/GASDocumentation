// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/AsyncGEAddedRemoved.h"


UAsyncGEAddedRemoved* UAsyncGEAddedRemoved::ListenForGEAddedRemoved(UAbilitySystemComponent* AbilitySystemComponent, FGameplayTagContainer TagsOfGE)
{
	UAsyncGEAddedRemoved* Listener = NewObject<UAsyncGEAddedRemoved>();
	Listener->ASC = AbilitySystemComponent;
	Listener->TagsOfGE = TagsOfGE;

	if (!IsValid(AbilitySystemComponent) || TagsOfGE.Num() < 1)
	{
		//Listener->End();
		return nullptr;
	}

	AbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf.AddUObject(Listener, &UAsyncGEAddedRemoved::OnActiveGameplayEffectAddedCallback);
	AbilitySystemComponent->OnAnyGameplayEffectRemovedDelegate().AddUObject(Listener, &UAsyncGEAddedRemoved::OnRemoveGameplayEffectCallback);

	// by tags Events
	TArray<FGameplayTag> AnyTagsGEHave;
	TagsOfGE.GetGameplayTagArray(AnyTagsGEHave);

	for (FGameplayTag Tag : AnyTagsGEHave)
	{
		//AbilitySystemComponent->RegisterGameplayTagEvent();
	}
	return Listener;
}

void UAsyncGEAddedRemoved::OnActiveGameplayEffectAddedCallback(UAbilitySystemComponent* Target, const FGameplayEffectSpec& SpecApplied, FActiveGameplayEffectHandle ActiveHandle)
{
	FGameplayTagContainer AssetTags;
	SpecApplied.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	SpecApplied.GetAllGrantedTags(GrantedTags);

	TArray<FGameplayTag> TargetTags;
	TagsOfGE.GetGameplayTagArray(TargetTags);

	for (FGameplayTag tag : TargetTags)
	{
		if (AssetTags.HasTagExact(tag) || GrantedTags.HasTagExact(tag))
		{
			OnGEAdded.Broadcast();
		}
	}
}

void UAsyncGEAddedRemoved::OnRemoveGameplayEffectCallback(const FActiveGameplayEffect& EffectRemoved)
{
	FGameplayTagContainer AssetTags;
	EffectRemoved.Spec.GetAllAssetTags(AssetTags);

	FGameplayTagContainer GrantedTags;
	EffectRemoved.Spec.GetAllGrantedTags(GrantedTags);

	TArray<FGameplayTag> TargetTags;
	TagsOfGE.GetGameplayTagArray(TargetTags);
	for (FGameplayTag tag : TargetTags)
	{
		if (AssetTags.HasTagExact(tag) || GrantedTags.HasTagExact(tag))
		{
			OnGERemoved.Broadcast();
		}
	}
}