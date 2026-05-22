#include "YYCoreObjective.h"

#include "YYHealthComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AYYCoreObjective::AYYCoreObjective()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);

	CoreMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoreMesh"));
	CoreMesh->SetupAttachment(RootComponent);
	CoreMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	CoreMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	CoreMesh->SetRelativeScale3D(FVector(0.25f, 0.25f, 0.25f));

	HealthComponent = CreateDefaultSubobject<UYYHealthComponent>(TEXT("HealthComponent"));
}

void AYYCoreObjective::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AYYCoreObjective::HandleCoreDestroyed);
		HealthComponent->OnHealthChanged.AddDynamic(this, &AYYCoreObjective::HandleHealthChanged);
	}
}

void AYYCoreObjective::HandleCoreDestroyed()
{
	UE_LOG(LogTemp, Warning, TEXT("Core Destroyed! Mission Failed."));
}

void AYYCoreObjective::HandleHealthChanged(float CurrentHealth, float MaxHealth)
{
	UE_LOG(LogTemp, Warning, TEXT("Core Health: %.1f / %.1f"), CurrentHealth, MaxHealth);
}

void AYYCoreObjective::RepairCore(float RepairAmount)
{
	if (!HealthComponent)
	{
		return;
	}

	if (RepairAmount <= 0.0f)
	{
		return;
	}

	if (HealthComponent->GetCurrentHealth() >= HealthComponent->GetMaxHealth())
	{
		UE_LOG(LogTemp, Warning, TEXT("Core is already full. No repair needed."));
		return;
	}

	HealthComponent->Heal(RepairAmount);

	UE_LOG(LogTemp, Warning, TEXT("Core Repaired: +%.1f"), RepairAmount);
}