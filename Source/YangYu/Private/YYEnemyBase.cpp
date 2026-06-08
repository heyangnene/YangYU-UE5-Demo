#include "YYEnemyBase.h"

#include "YYCoreObjective.h"
#include "YYHealthComponent.h"

#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AYYEnemyBase::AYYEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;

	AIControllerClass = AAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->InitCapsuleSize(40.0f, 90.0f);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionObjectType(ECC_Pawn);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->bUseControllerDesiredRotation = false;

	EnemyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetupAttachment(RootComponent);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	EnemyMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	EnemyMesh->SetRelativeScale3D(FVector(0.8f, 0.8f, 1.8f));

	HealthComponent = CreateDefaultSubobject<UYYHealthComponent>(TEXT("HealthComponent"));
}

void AYYEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (!GetController())
	{
		SpawnDefaultController();
	}

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

	UpdateAggroState();

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

UYYHealthComponent* AYYEnemyBase::GetPlayerHealthComponent() const
{
	if (!PlayerTargetActor)
	{
		return nullptr;
	}

	TArray<UYYHealthComponent*> HealthComponents;
	PlayerTargetActor->GetComponents<UYYHealthComponent>(HealthComponents);

	for (UYYHealthComponent* Component : HealthComponents)
	{
		if (Component && Component->GetFName() == TEXT("PlayerHealthComponent"))
		{
			return Component;
		}
	}

	return nullptr;
}

void AYYEnemyBase::UpdateAggroState()
{
	bIsChasingPlayer = false;

	if (!PlayerTargetActor)
	{
		return;
	}

	UYYHealthComponent* PlayerHealthComponent = GetPlayerHealthComponent();

	if (!PlayerHealthComponent || PlayerHealthComponent->IsDead())
	{
		return;
	}

	const float PlayerDistance = FVector::Dist2D(GetActorLocation(), PlayerTargetActor->GetActorLocation());

	if (PlayerDistance <= PlayerDetectDistance)
	{
		bIsChasingPlayer = true;
	}
}

AActor* AYYEnemyBase::GetCurrentMoveTarget() const
{
	if (bIsChasingPlayer && PlayerTargetActor)
	{
		return PlayerTargetActor;
	}

	return TargetActor;
}

AActor* AYYEnemyBase::GetCurrentAttackTarget() const
{
	if (PlayerTargetActor)
	{
		UYYHealthComponent* PlayerHealthComponent = GetPlayerHealthComponent();

		if (PlayerHealthComponent && !PlayerHealthComponent->IsDead())
		{
			const float PlayerDistance = FVector::Dist2D(GetActorLocation(), PlayerTargetActor->GetActorLocation());

			if (PlayerDistance <= PlayerAttackDistance)
			{
				return PlayerTargetActor;
			}
		}
	}

	return TargetActor;
}

void AYYEnemyBase::MoveToTarget(float DeltaTime)
{
	if (!HealthComponent || HealthComponent->IsDead())
	{
		return;
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	}

	AActor* MoveTarget = GetCurrentMoveTarget();

	if (!MoveTarget)
	{
		return;
	}

	AAIController* AIController = Cast<AAIController>(GetController());

	if (!AIController)
	{
		return;
	}

	const bool bMovingToPlayer = MoveTarget == PlayerTargetActor;

	const FVector TargetLocation = bMovingToPlayer
		? MoveTarget->GetActorLocation()
		: MoveTarget->GetActorLocation() + TargetOffset;

	const float CurrentStopDistance = bMovingToPlayer
		? PlayerAttackDistance * 0.8f
		: StopDistance;

	const float Distance = FVector::Dist2D(GetActorLocation(), TargetLocation);

	if (Distance <= CurrentStopDistance)
	{
		AIController->StopMovement();
		return;
	}

	AIController->MoveToLocation(
		TargetLocation,
		CurrentStopDistance,
		true,
		true,
		true,
		false,
		nullptr,
		true
	);

	FVector Direction = TargetLocation - GetActorLocation();
	Direction.Z = 0.0f;

	if (!Direction.IsNearlyZero())
	{
		const FRotator DirectionRotation = Direction.Rotation();
		SetActorRotation(FRotator(0.0f, DirectionRotation.Yaw, 0.0f));
	}
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
		TargetHealthComponent = GetPlayerHealthComponent();

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
	AAIController* AIController = Cast<AAIController>(GetController());

	if (AIController)
	{
		AIController->StopMovement();
		AIController->UnPossess();
	}

	if (GetCharacterMovement())
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->DisableMovement();
	}

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (EnemyMesh)
	{
		EnemyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SetActorTickEnabled(false);

	if (DestroyDelay <= 0.0f)
	{
		Destroy();
		return;
	}

	SetLifeSpan(DestroyDelay);
}