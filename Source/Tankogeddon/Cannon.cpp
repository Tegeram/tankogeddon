// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"
#include "Projectile.h"
#include "Tankogeddon.h"
#include "DrawDebugHelpers.h"
#include "ActorPoolSubsystem.h"


// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = SceeneCpm;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
}

void ACannon::Fire()
{
	if (!IsReadyToFire())
	{
		return;
	}

	bIsReadyToFire = false;
	--NumAmmo;
	
	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire special - projectile"));

		//AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(ProjectileClass, SpawnTransform));
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}

	}
	else if (Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire special - trace"));

		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = TraceStart + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		//TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				HitResult.Actor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
		}
	}
	else if (Type == ECannonType::FireRocket)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Green, TEXT("Fire - rocket"));

		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(RocketClass, SpawnTransform));
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 10.f / FireRate, false);
	UE_LOG(LogTankogeddon, Log, TEXT("Fire! Ammo left: %d"), NumAmmo);
}

void ACannon::FireSpecial()
{
	if (!HasSpecialFire() || !IsReadyToFire())
	{
		return;
	}

	bIsReadyToFire = false;
	--NumAmmo;

	ShotsLeft = NumShotsInSeries;
	Shot();
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 10.f / FireRate, false);
	UE_LOG(LogTankogeddon, Log, TEXT("FireSpecial! Ammo left: %d"), NumAmmo);
}

bool ACannon::IsReadyToFire()
{
	return bHasSpecialFire && NumAmmo > 0 && ShotsLeft == 0;
}

bool ACannon::HasSpecialFire() const
{
	return bHasSpecialFire;	
}

void ACannon::SetVisibility(bool bIsVisible)
{
	Mesh->SetHiddenInGame(!bIsVisible);
}

void ACannon::AddAmmo(int32 InNumAmmo)
{
	NumAmmo = NumAmmo + InNumAmmo;
	if (NumAmmo > MaxAmmo)
	{
		NumAmmo = MaxAmmo;
	}
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFire = true;
	NumAmmo = MaxAmmo;
	ShotsLeft = 0;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(SeriesTimerHandle);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}

void ACannon::Shot()
{
	check(ShotsLeft > 0)
	if(Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire special - projectile"));

		//AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(ProjectileClass,SpawnTransform));
		if (Projectile)
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}
	}
	else if (Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire special - trace"));

		FHitResult HitResult;
		FVector TraceStart = ProjectileSpawnPoint->GetComponentLocation();
		FVector TraceEnd = TraceStart + ProjectileSpawnPoint->GetForwardVector() * FireRange;
		FCollisionQueryParams TraceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
		//TraceParams.bTraceComplex = true;
		TraceParams.bReturnPhysicalMaterial = false;
		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), TraceStart, HitResult.Location, FColor::Red, false, 0.5f, 0, 5.f);
			if (HitResult.Actor.IsValid() && HitResult.Component.IsValid(), HitResult.Component->GetCollisionObjectType() == ECC_Destructible)
			{
				HitResult.Actor->Destroy();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), TraceStart, TraceEnd, FColor::Red, false, 0.5f, 0, 5.f);
		}
	}
	else if (Type == ECannonType::FireRocket)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 10.f, FColor::Green, TEXT("Fire - rocket"));

		UActorPoolSubsystem* Pool = GetWorld()->GetSubsystem<UActorPoolSubsystem>();
		FTransform SpawnTransform(ProjectileSpawnPoint->GetComponentRotation(), ProjectileSpawnPoint->GetComponentLocation(), FVector::OneVector);
		AProjectile* Projectile = Cast<AProjectile>(Pool->RetreiveActor(RocketClass, SpawnTransform));
		{
			Projectile->SetInstigator(GetInstigator());
			Projectile->Start();
		}
	}

	if (--ShotsLeft > 0)
	{
		const float NextShotTime = SeriesLength / (NumShotsInSeries - 1);
		GetWorld()->GetTimerManager().SetTimer(SeriesTimerHandle, this, &ACannon::Shot, NextShotTime, false);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 10.f / FireRate, false);
	}
}