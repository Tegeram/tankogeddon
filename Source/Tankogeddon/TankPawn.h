// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"


UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UCameraComponent* Camera;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	class UArrowComponent* CannonSpawnPoint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 100.f;

	//UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	//float MoveRightSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
	float RotationSpeed = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float MovementSmootheness = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement")
	float RotationSmootheness = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	float TurretRotationSmootheness = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire params")
	float FireCount = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret")
	TSubclassOf<class ACannon> DefaultCannonClass;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	class ATankPlayerController* TankController;

public:
	void SetupCannon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveForward(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRight(float InAxisValue);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void SetTurretTargetPosition(const FVector& TargetPosition);

	UFUNCTION(BlueprintCallable, Category = "Turret")
	void Fire();
	
	UFUNCTION(BlueprintCallable, Category = "Turret")
	void FireSpecial();

	//UFUNCTION(BlueprintCallable, Category = "Movement")
	//void MoveRight(float InAxisValue);

private:
	UPROPERTY()
	class ACannon* Cannon = nullptr;

	float TargetMoveForwardAxis = 0.f;
	float CurrentMoveForwardAxis = 0.f;
	//float TargetMoveRightAxis = 0.f;
	float TargetRotateRightAxis = 0.f;
	float CurrentRotateRightAxis = 0.f;

	FVector TurretTargetPosition;
};
