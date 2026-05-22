#include "YYProjectile.h"
#include "YYHealthComponent.h"

#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AYYProjectile::AYYProjectile()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));//创建碰撞球组件
	RootComponent = CollisionSphere;

	CollisionSphere->InitSphereRadius(12.0f);
	CollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionSphere->SetCollisionObjectType(ECC_WorldDynamic);
	CollisionSphere->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionSphere->OnComponentHit.AddDynamic(this, &AYYProjectile::OnProjectileHit);//绑定碰撞事件

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMesh"));//创建子弹外观组件
	ProjectileMesh->SetupAttachment(RootComponent);
	ProjectileMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));//创建子弹移动组件
	ProjectileMovement->InitialSpeed = 2500.0f;
	ProjectileMovement->MaxSpeed = 2500.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->ProjectileGravityScale = 0.0f;
}

void AYYProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(LifeSeconds);//设置子弹的生命周期，超过这个时间后子弹会自动销毁
}

void AYYProjectile::OnProjectileHit(
	UPrimitiveComponent* HitComponent,
	AActor* OtherActor, //子弹碰撞到的其他Actor
	UPrimitiveComponent* OtherComp,
	FVector NormalImpulse,
	const FHitResult& Hit
)
{
	if (!OtherActor || OtherActor == this || OtherActor == GetOwner()) //如果碰撞到的Actor无效、是自己或者是自己的拥有者（通常是发射子弹的角色），则不处理碰撞，直接销毁子弹
	{
		Destroy();
		return;
	}
	
	UYYHealthComponent* HealthComponent = OtherActor->FindComponentByClass<UYYHealthComponent>();

	if (HealthComponent) //如果碰撞到的Actor有健康组件，则对其造成伤害
	{
		HealthComponent->ApplyDamage(Damage);
	}

	Destroy();
}