// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

class UPawnSensingComponent;

UENUM()
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted
};

UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent *PawnSensingComponent;

	UFUNCTION()
	void ResetRotation();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnAIStateChange(EAIState newState);

	void ChangeAIState(EAIState newState);

	UPROPERTY(EditInstanceOnly, Category = "AI")
	bool bPatrol;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol") )
	AActor *firstPatrolPoint;

	UPROPERTY(EditInstanceOnly, Category = "AI", meta = (EditCondition = "bPatrol") )
	AActor *secondPatrolPoint;

	AActor *currentPatrolPoint;

	void MoveToNextPatrolPoint();

private:
	FTimerHandle ResetRotationTimer;
	FRotator OriginalRotation;
	EAIState aiState;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void HandlePawnSensed(APawn *SeenPawn);

	UFUNCTION()
	void HandleNoiseHeard(APawn* NoiseInstigator, const FVector &Location, float Volume);

};
