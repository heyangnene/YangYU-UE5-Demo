#include "YYWaveSpawner.h"

#include "YYEnemyBase.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

AYYWaveSpawner::AYYWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(false);
}

void AYYWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		StartSpawning();
	}
}

void AYYWaveSpawner::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AYYWaveSpawner, AliveEnemyCount);
	DOREPLIFETIME(AYYWaveSpawner, KilledEnemyCount);
	DOREPLIFETIME(AYYWaveSpawner, Score);
	DOREPLIFETIME(AYYWaveSpawner, TotalWaves);
	DOREPLIFETIME(AYYWaveSpawner, CurrentWave);
	DOREPLIFETIME(AYYWaveSpawner, SpawnedInCurrentWave);
	DOREPLIFETIME(AYYWaveSpawner, KilledInCurrentWave);
	DOREPLIFETIME(AYYWaveSpawner, bAllWavesCompleted);
}

void AYYWaveSpawner::StartSpawning()
{
	if (!HasAuthority())
	{
		return;
	}

	if (!EnemyClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("WaveSpawner has no EnemyClass assigned."));
		return;
	}

	AliveEnemyCount = 0;
	KilledEnemyCount = 0;
	Score = 0;

	CurrentWave = 0;
	SpawnedInCurrentWave = 0;
	KilledInCurrentWave = 0;
	bAllWavesCompleted = false;

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveBreakTimerHandle);

	StartNextWave();

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner started."));
}

void AYYWaveSpawner::StopSpawning()
{
	if (!HasAuthority())
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
	GetWorldTimerManager().ClearTimer(WaveBreakTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner stopped."));
}

void AYYWaveSpawner::StartNextWave()
{
	if (!HasAuthority())
	{
		return;
	}

	if (bAllWavesCompleted)
	{
		return;
	}

	CurrentWave++;

	SpawnedInCurrentWave = 0;
	KilledInCurrentWave = 0;

	if (CurrentWave > TotalWaves)
	{
		bAllWavesCompleted = true;
		StopSpawning();

		UE_LOG(LogTemp, Warning, TEXT("All waves completed."));
		return;
	}

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AYYWaveSpawner::SpawnEnemy,
		SpawnInterval,
		true,
		0.0f
	);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Wave %d / %d started. Enemy Count: %d"),
		CurrentWave,
		TotalWaves,
		GetCurrentWaveEnemyCount()
	);
}

void AYYWaveSpawner::FinishCurrentWave()
{
	if (!HasAuthority())
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Wave %d / %d completed."),
		CurrentWave,
		TotalWaves
	);

	if (CurrentWave >= TotalWaves)
	{
		bAllWavesCompleted = true;
		StopSpawning();

		UE_LOG(LogTemp, Warning, TEXT("All waves completed."));
		return;
	}

	GetWorldTimerManager().SetTimer(
		WaveBreakTimerHandle,
		this,
		&AYYWaveSpawner::StartNextWave,
		WaveBreakTime,
		false
	);
}

int32 AYYWaveSpawner::GetCurrentWaveEnemyCount() const
{
	if (CurrentWave <= 1)
	{
		return Wave1EnemyCount;
	}

	if (CurrentWave == 2)
	{
		return Wave2EnemyCount;
	}

	if (CurrentWave >= 3)
	{
		return Wave3EnemyCount;
	}

	return Wave1EnemyCount;
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
	if (!HasAuthority())
	{
		return;
	}

	if (!EnemyClass)
	{
		return;
	}

	if (bAllWavesCompleted)
	{
		return;
	}

	if (CurrentWave <= 0 || CurrentWave > TotalWaves)
	{
		return;
	}

	const int32 CurrentWaveEnemyCount = GetCurrentWaveEnemyCount();

	if (SpawnedInCurrentWave >= CurrentWaveEnemyCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
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
			SpawnedInCurrentWave++;

			SpawnedEnemy->OnDestroyed.AddDynamic(this, &AYYWaveSpawner::HandleSpawnedEnemyDestroyed);

			UE_LOG(
				LogTemp,
				Warning,
				TEXT("Enemy spawned. Wave: %d / %d | Spawned: %d / %d | Alive: %d / %d"),
				CurrentWave,
				TotalWaves,
				SpawnedInCurrentWave,
				CurrentWaveEnemyCount,
				AliveEnemyCount,
				MaxAliveCount
			);

			if (SpawnedInCurrentWave >= CurrentWaveEnemyCount)
			{
				GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
			}

			return;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner failed to spawn enemy after several attempts."));
}

void AYYWaveSpawner::HandleSpawnedEnemyDestroyed(AActor* DestroyedActor)
{
	if (!HasAuthority())
	{
		return;
	}

	AliveEnemyCount = FMath::Max(AliveEnemyCount - 1, 0);

	KilledEnemyCount++;
	KilledInCurrentWave++;
	Score += ScorePerEnemy;

	const int32 CurrentWaveEnemyCount = GetCurrentWaveEnemyCount();

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("Enemy destroyed. Wave: %d / %d | Alive: %d / %d | Wave Kills: %d / %d | Total Kills: %d | Score: %d"),
		CurrentWave,
		TotalWaves,
		AliveEnemyCount,
		MaxAliveCount,
		KilledInCurrentWave,
		CurrentWaveEnemyCount,
		KilledEnemyCount,
		Score
	);

	if (!bAllWavesCompleted &&
		SpawnedInCurrentWave >= CurrentWaveEnemyCount &&
		KilledInCurrentWave >= CurrentWaveEnemyCount &&
		AliveEnemyCount <= 0)
	{
		FinishCurrentWave();
	}
}