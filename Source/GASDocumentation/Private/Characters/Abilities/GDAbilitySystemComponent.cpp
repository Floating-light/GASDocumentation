// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/GDAbilitySystemComponent.h"

void UGDAbilitySystemComponent::ReceiveDamage(UGDAbilitySystemComponent * SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}



FActiveGameplayEffectHandle UGDAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey)
{
	FActiveGameplayEffectHandle Handle = Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);

	if (GetOwnerRole() == ROLE_Authority)
	{
		OnGEAppliedDelegateToSelfServerAndClient.Broadcast(this, ActiveGameplayEffects.GetActiveGameplayEffect(Handle)->Spec, Handle);
		FGameplayTagContainer Tags;
		GameplayEffect.GetAllGrantedTags(Tags);
		
		//FString server = IsOwnerActorAuthoritative() ? TEXT("Server") : TEXT("Client");
		//GEngine->AddOnScreenDebugMessage(-1, 100.0f, FColor::Yellow, FString::Printf(TEXT("%s: Call ApplyGameplayEffectSpecToSelf ------------------->>>%s"),*server, *Tags.ToString()));
	}

	return Handle;
}
