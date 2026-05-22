#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "YYProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
 class YANGYU_API AYYProjectile : public AActor 
 {
	GENERATED_BODY()

public:
	AYYProjectile();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	USphereComponent* CollisionSphere; //碰撞球

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UStaticMeshComponent* ProjectileMesh; //子弹外观

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement; //子弹移动组件

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float Damage = 25.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Projectile")
	float LifeSeconds = 3.0f;

	UFUNCTION()
	void OnProjectileHit(
		UPrimitiveComponent* HitComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		FVector NormalImpulse,
		const FHitResult& Hit
	);
};