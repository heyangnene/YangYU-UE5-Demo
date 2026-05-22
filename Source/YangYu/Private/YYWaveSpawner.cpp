#include "YYWaveSpawner.h"

#include "YYEnemyBase.h"
#include "Engine/World.h"
#include "TimerManager.h"

AYYWaveSpawner::AYYWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = false;		//不需要每帧更新，所以将bCanEverTick设置为false。
}

void AYYWaveSpawner::BeginPlay()		//当游戏开始生成敌人
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

	AliveEnemyCount = 0;		//重置当前存在的敌人数量。

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,		//定时器句柄，用于后续控制定时器。
		this,		//定时器所属对象，即当前的AYYWaveSpawner实例。
		&AYYWaveSpawner::SpawnEnemy,		//定时器触发时调用的函数，这里是SpawnEnemy函数。
		SpawnInterval,		//定时器的时间间隔，即每隔SpawnInterval秒调用一次SpawnEnemy函数。
		true,		
		0.0f		//表示立即开始生成敌人。
	);		//设置一个定时器，每隔SpawnInterval秒调用一次SpawnEnemy函数，开始生成敌人。

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner started."));
}

void AYYWaveSpawner::StopSpawning()
{
	GetWorldTimerManager().ClearTimer(SpawnTimerHandle);

	UE_LOG(LogTemp, Warning, TEXT("WaveSpawner stopped."));
}

FVector AYYWaveSpawner::GetRandomSpawnLocation() const		//生成一个随机的生成位置，基于当前生成器的位置和方向。
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

		AYYEnemyBase* SpawnedEnemy = GetWorld()->SpawnActor<AYYEnemyBase>(		//尝试生成敌人
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

	UE_LOG(LogTemp, Warning, TEXT("Enemy destroyed. Alive: %d / %d"), AliveEnemyCount, MaxAliveCount);
}