// Copyright 2020 Dan Kestranek.


#include "Characters/Abilities/GDAbilitySystemComponent.h"
#include "Kismet\GameplayStatics.h"
void UGDAbilitySystemComponent::ReceiveDamage(UGDAbilitySystemComponent * SourceASC, float UnmitigatedDamage, float MitigatedDamage)
{
	ReceivedDamage.Broadcast(SourceASC, UnmitigatedDamage, MitigatedDamage);
}

void UGDAbilitySystemComponent::InitAbilityActorInfo(AActor* InOwnerActor, AActor* InAvatarActor)
{
	Super::InitAbilityActorInfo(InOwnerActor, InAvatarActor);
	if (!ActiveGameplayEffects.OnActiveGameplayEffectRemovedDelegate.IsBoundToObject(this))
	{
		ActiveGameplayEffects.OnActiveGameplayEffectRemovedDelegate.AddUObject(this, &UGDAbilitySystemComponent::RemoveGECallback);
	}
}

void UGDAbilitySystemComponent::CallClientGEAddRemoveChange_Implementation(const FGameplayEffectSpec& GESpec)
{
	if (GetOwnerRole() == ROLE_Authority) // Does it need ???
	{
		// there must be listen server 
		return;
	}

	// Added Instant GE
	if (GESpec.Def->DurationPolicy == EGameplayEffectDurationType::Instant)
	{
		// added 
		OnRealAnyGameplayEffectAddedDelegate.Broadcast(this, GESpec, FActiveGameplayEffectHandle());
		// just remove immediately
		OnRealAnyGameplayEffectRemovedDelegate.Broadcast(GESpec);
		return;
	}

	FActiveGameplayEffect* ActiveGE = nullptr;
	for (FActiveGameplayEffect& ActiveEffect : &ActiveGameplayEffects)
	{
		if (ActiveEffect.Spec.Def == GESpec.Def)
		{
			ActiveGE = &ActiveEffect;
			break;
		}
	}

	if(ActiveGE != nullptr) // Added Duration or Infinite
	{
		OnRealAnyGameplayEffectAddedDelegate.Broadcast(this, ActiveGE->Spec, ActiveGE->Handle);
	}
}


FActiveGameplayEffectHandle UGDAbilitySystemComponent::ApplyGameplayEffectSpecToSelf(const FGameplayEffectSpec& GameplayEffect, FPredictionKey PredictionKey)
{
	FActiveGameplayEffectHandle Handle = Super::ApplyGameplayEffectSpecToSelf(GameplayEffect, PredictionKey);

	if (GetOwnerRole() == ROLE_Authority) //No PredictionKey
	{
		// Server Call Add delegate
		OnRealAnyGameplayEffectAddedDelegate.Broadcast(this, GameplayEffect, Handle);
		if (GameplayEffect.Def->DurationPolicy == EGameplayEffectDurationType::Instant)
		{
			OnRealAnyGameplayEffectRemovedDelegate.Broadcast(GameplayEffect);
		}
		//if (GEngine)
		//{
		//	GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, FString::Printf(TEXT("OwnerRole ---> %d, Ownerremote role ---> %d"), int(GetOwnerRole()), int(GetOwner()->GetRemoteRole())));
		//	auto Controller = UGameplayStatics::GetPlayerController(this, 0);
		//	if(Controller)
		//		GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, FString::Printf(TEXT("Controller :OwnerRole ---> %d, Ownerremote role ---> %d"), int(Controller->GetLocalRole()), int(Controller->GetRemoteRole())));

		//}
		CallClientGEAddRemoveChange(GameplayEffect);
	}

	return Handle;
}
