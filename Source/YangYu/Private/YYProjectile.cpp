#include "YYProjectile.h"
#include "YYHealthComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AYYProjectile::AYYProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	bReplicates = true;
	SetReplicateMovement(true);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere")); // 创建碰撞球组件
	RootComponent = CollisionSphere;

	CollisionSphere->InitSphereRadius(12.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionSphere->OnComponentHit.AddDynamic(this, &AYYProjectile::OnProjectileHit); // 绑定碰撞事件

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh")); // 创建子弹外观组件
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement")); // 创建子弹移动组件
	ProjectileMovement->InitialSpeed = 2500.0f;
	ProjectileMovement->MaxSpeed = 2500.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AYYProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSeconds); // 设置子弹的生命周期，超过这个时间后子弹会自动销毁
}

void AYYProjectile::OnProjectileHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor, // 子弹碰撞到的其他Actor
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	// 只有服务器可以处理伤害和销毁。
	// Client 只负责显示服务器同步过来的子弹和血量结果。
	if (!HasAuthority())
	{
		return;
	}

	if (!OtherActor || OtherActor == this || OtherActor == GetOwner())
	{
		Destroy();
		return;
	}

	UYYHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UYYHealthComponent>();

	if (HealthComponent)
	{
		HealthComponent->ApplyDamage(Damage);
	}

	Destroy();
}