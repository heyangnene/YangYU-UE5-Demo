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

	if (!TargetActor)
	{
		TargetActor = UGameplayStatics::GetPlayerPawn(this, 0);
	}

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

	MoveToTarget(DeltaTime);
	TryAttackTarget(DeltaTime);
}

void AYYEnemyBase::FindCoreTarget()
{
	TargetActor = UGameplayStatics::GetActorOfClass(this, AYYCoreObjective::StaticClass());
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

void AYYEnemyBase::TryAttackTarget(float DeltaTime)
{
	if (!TargetActor || !HealthComponent || HealthComponent->IsDead())
	{
		return;
	}

	if (AttackCooldown > 0.0f)
	{
		AttackCooldown -= DeltaTime;
		return;
	}

	// 注意：攻击距离仍然以核心真实位置为准，而不是偏移站位点。
	const float Distance = FVector::Dist2D(GetActorLocation(), TargetActor->GetActorLocation());

	if (Distance > AttackDistance)
	{
		return;
	}

	UYYHealthComponent* TargetHealthComponent = TargetActor->FindComponentByClass<UYYHealthComponent>();

	if (!TargetHealthComponent || TargetHealthComponent->IsDead())
	{
		return;
	}

	TargetHealthComponent->ApplyDamage(AttackDamage);

	UE_LOG(LogTemp, Warning, TEXT("Enemy attacked target: %.1f damage"), AttackDamage);

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