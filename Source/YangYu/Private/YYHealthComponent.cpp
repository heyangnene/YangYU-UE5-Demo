#include "YYHealthComponent.h"

UYYHealthComponent::UYYHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UYYHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	bIsDead = false;

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void UYYHealthComponent::ApplyDamage(float DamageAmount)
{
	if (bIsDead || DamageAmount <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - DamageAmount, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		bIsDead = true;
		OnDeath.Broadcast();
	}
}

void UYYHealthComponent::Heal(float HealAmount)
{
	if (bIsDead || HealAmount <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Clamp(CurrentHealth + HealAmount, 0.0f, MaxHealth);

	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}