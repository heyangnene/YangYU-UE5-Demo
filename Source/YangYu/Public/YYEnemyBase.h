#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "YYEnemyBase.generated.h"

class UStaticMeshComponent;
class UYYHealthComponent;

UCLASS()
class YANGYU_API AYYEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AYYEnemyBase();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UStaticMeshComponent* EnemyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	UYYHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy")
	float DestroyDelay = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Movement")
	float MoveSpeed = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Movement")
	float StopDistance = 80.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float AttackDistance = 260.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float AttackDamage = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float AttackInterval = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float AttackWindUpTime = 0.3f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	bool bIsPreparingAttack = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	AActor* PendingAttackTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float PendingAttackDamage = 0.0f;

	FTimerHandle AttackWindUpTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float PlayerAttackDistance = 220.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float PlayerAttackDamage = 15.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Aggro")
	float PlayerDetectDistance = 600.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Aggro")
	bool bIsChasingPlayer = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Attack")
	float AttackCooldown = 0.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Target")
	AActor* TargetActor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Target")
	AActor* PlayerTargetActor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy|Target")
	float TargetOffsetRadius = 220.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy|Target")
	FVector TargetOffset = FVector::ZeroVector;

	UFUNCTION()
	void HandleDeath();

	void FindCoreTarget();

	void FindPlayerTarget();

	void GenerateTargetOffset();

	void UpdateAggroState();

	void MoveToTarget(float DeltaTime);

	void TryAttackTarget(float DeltaTime);

	void ExecutePreparedAttack();

	AActor* GetCurrentMoveTarget() const;

	AActor* GetCurrentAttackTarget() const;

	UYYHealthComponent* GetPlayerHealthComponent() const;

public:
	UFUNCTION(BlueprintPure, Category = "Enemy")
	UYYHealthComponent* GetHealthComponent() const { return HealthComponent; }
};