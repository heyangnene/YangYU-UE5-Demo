#include "YYEnemyBase.h"

#include "YYCoreObjective.h"
#include "YYHealthComponent.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AYYEnemyBase::AYYEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	RootComponent = CollisionBox;

	CollisionBox->SetBoxExtent(FVector(40.0f, 40.0f, 90.0f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBox->SetCollisionObjectType(ECC_Pawn);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Block);

	// Avoid enemies blocking or pushing the player / each other too aggressively.
	CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(RootComponent);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	EnemyMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.8f));

	HealthComponent = CreateDefaultSubobject<UYYHealthComponent>(TEXT("HealthComponent"));
}

void AYYEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (HealthComponent)
	{
		HealthComponent->OnDeath.AddDynamic(this, &AYYEnemyBase::HandleDeath);
	}

	FindCoreTarget();
	FindPlayerTarget();
	GenerateTargetOffset();
}

void AYYEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!TargetActor)
	{
		FindCoreTarget();

		if (TargetActor)
		{
			GenerateTargetOffset();
		}
	}

	if (!PlayerTargetActor)
	{
		FindPlayerTarget();
	}

	MoveToTarget(DeltaTime);
	TryAttackTarget(DeltaTime);
}

void AYYEnemyBase::FindCoreTarget()
{
	TargetActor = UGameplayStatics::GetActorOfClass(this, AYYCoreObjective::StaticClass());
}

void AYYEnemyBase::FindPlayerTarget()
{
	PlayerTargetActor = UGameplayStatics::GetPlayerPawn(this, 0);
}

void AYYEnemyBase::GenerateTargetOffset()
{
	const float RandomAngle = FMath::RandRange(0.0f, 360.0f);

	TargetOffset = FVector(
		FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * TargetOffsetRadius,
		FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * TargetOffsetRadius,
		0.0f
	);
}

void AYYEnemyBase::MoveToTarget(float DeltaTime)
{
	if (!TargetActor || !HealthComponent || HealthComponent->IsDead())
	{
		return;
	}

	const FVector CurrentLocation = GetActorLocation();
	const FVector TargetLocation = TargetActor->GetActorLocation() + TargetOffset;

	FVector Direction = TargetLocation - CurrentLocation;
	Direction.Z = 0.0f;

	const float Distance = Direction.Size();

	if (Distance <= StopDistance)
	{
		return;
	}

	Direction.Normalize();

	const FVector NewLocation = CurrentLocation + Direction * MoveSpeed * DeltaTime;

	FHitResult HitResult;
	SetActorLocation(NewLocation, true, &HitResult);

	const FRotator DirectionRotation = Direction.Rotation();
	SetActorRotation(FRotator(0.0f, DirectionRotation.Yaw, 0.0f));
}

AActor* AYYEnemyBase::GetCurrentAttackTarget() const
{
	if (PlayerTargetActor)
	{
		const float PlayerDistance = FVector::Dist2D(GetActorLocation(), PlayerTargetActor->GetActorLocation());

		if (PlayerDistance <= PlayerAttackDistance)
		{
			return PlayerTargetActor;
		}
	}

	return TargetActor;
}

void AYYEnemyBase::TryAttackTarget(float DeltaTime)
{
	if (!HealthComponent || HealthComponent->IsDead())
	{
		return;
	}

	if (AttackCooldown > 0.0f)
	{
		AttackCooldown -= DeltaTime;
		return;
	}

	AActor* AttackTarget = GetCurrentAttackTarget();

	if (!AttackTarget)
	{
		return;
	}

	const bool bIsAttackingPlayer = AttackTarget == PlayerTargetActor;

	const float CurrentAttackDistance = bIsAttackingPlayer ? PlayerAttackDistance : AttackDistance;
	const float CurrentAttackDamage = bIsAttackingPlayer ? PlayerAttackDamage : AttackDamage;

	const float Distance = FVector::Dist2D(GetActorLocation(), AttackTarget->GetActorLocation());

	if (Distance > CurrentAttackDistance)
	{
		return;
	}

	UYYHealthComponent* TargetHealthComponent = nullptr;

	if (bIsAttackingPlayer)
	{
		TArray<UYYHealthComponent*> HealthComponents;
		AttackTarget->GetComponents<UYYHealthComponent>(HealthComponents);

		for (UYYHealthComponent* Component : HealthComponents)
		{
			if (Component && Component->GetFName() == TEXT("PlayerHealthComponent"))
			{
				TargetHealthComponent = Component;
				break;
			}
		}

		if (!TargetHealthComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("Enemy tried to attack player, but PlayerHealthComponent was not found."));
			return;
		}
	}
	else
	{
		TargetHealthComponent = AttackTarget->FindComponentByClass<UYYHealthComponent>();
	}

	if (!TargetHealthComponent || TargetHealthComponent->IsDead())
	{
		return;
	}

	TargetHealthComponent->ApplyDamage(CurrentAttackDamage);

	if (bIsAttackingPlayer)
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy attacked player: %.1f damage"), CurrentAttackDamage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Enemy attacked core: %.1f damage"), CurrentAttackDamage);
	}

	AttackCooldown = AttackInterval;
}

void AYYEnemyBase::HandleDeath()
{
	if (DestroyDelay <= 0.0f)
	{
		Destroy();
		return;
	}
	
	SetLifeSpan(DestroyDelay);
}