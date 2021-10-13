// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/SceneComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

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
	if (!bIsReadyToFire)
	{
		return;
	}

	bIsReadyToFire = false;
	
	if (Type == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - projectile"));
	}
	else if (Type == ECannonType::FireTrace)
	{
		GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire - trace"));
	}
	
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 2.f / FireRate, false);
}

void ACannon::FireSpecial()
{
	if (!bIsReadyToFire)
	{
		return;
	}	

	float LocalFireSerias = FireSerias;

	while (LocalFireSerias > 0)
	{
		bIsReadyToFire = false;
		
		if (Type == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(INDEX_NONE, 2.f, FColor::Green, TEXT("Fire special - projectile"));
		}

		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 2.f / FireRate, false);

		LocalFireSerias -= 1;
	}
}

bool ACannon::IsReadyToFire()
{
	return bIsReadyToFire;
}

// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	
	bIsReadyToFire = true;
}

void ACannon::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
}

void ACannon::Reload()
{
	bIsReadyToFire = true;
}


