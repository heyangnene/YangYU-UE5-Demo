#include "YYWaveSpawner.h"

#include "YYEnemyBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

AYYWaveSpawner::AYYWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AYYWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	StartSpawning();
}

void AYYWaveSpawner::StartSpawning()
{
	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveSpawner has no EnemyClass assigned."));
		return;
	}

	AliveEnemyCount = 0;
	KilledEnemyCount = 0;
	Score = 0;

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AYYWaveSpawner::SpawnEnemy,
		SpawnInterval,
		true,
		0.0f
	);

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner started."));
}

void AYYWaveSpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner stopped."));
}

FVector AYYWaveSpawner::GetRandomSpawnLocation() const
{
	const float RandomAngle = FMath::RandRange(0.0f, 360.0f);
	const float RandomDistance = FMath::RandRange(0.0f, SpawnRadius);

	const FVector Offset =
		GetActorForwardVector() * FMath::Cos(FMath::DegreesToRadians(RandomAngle)) * RandomDistance +
		GetActorRightVector() * FMath::Sin(FMath::DegreesToRadians(RandomAngle)) * RandomDistance;

	FVector SpawnLocation = GetActorLocation() + Offset;
	SpawnLocation.Z += SpawnHeightOffset;

	return SpawnLocation;
}

void AYYWaveSpawner::SpawnEnemy()
{
	if (!EnemyClass)
	{
		return;
	}

	if (AliveEnemyCount >= MaxAliveCount)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride =
		ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	constexpr int32 MaxAttempts = 10;

	for (int32 AttemptIndex = 0; AttemptIndex < MaxAttempts; AttemptIndex++)
	{
		const FVector SpawnLocation = GetRandomSpawnLocation();
		const FRotator SpawnRotation = GetActorRotation();

		AYYEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AYYEnemyBase>(
			EnemyClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams
		);

		if (SpawnedEnemy)
		{
			AliveEnemyCount++;

			SpawnedEnemy->OnDestroyed.AddDynamic(this, &AYYWaveSpawner::HandleSpawnedEnemyDestroyed);

			UE_LOG(LogTemp, Warning, TEXT("Enemy spawned. Alive: %d / %d"), AliveEnemyCount, MaxAliveCount);
			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner failed to spawn enemy after several attempts."));
}

void AYYWaveSpawner::HandleSpawnedEnemyDestroyed(AActor* DestroyedActor)
{
	AliveEnemyCount = FMath::Max(AliveEnemyCount - 1, 0);

	KilledEnemyCount++;
	Score += ScorePerEnemy;

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Enemy destroyed. Alive: %d / %d | Kills: %d | Score: %d"),
		AliveEnemyCount,
		MaxAliveCount,
		KilledEnemyCount,
		Score
	);
}