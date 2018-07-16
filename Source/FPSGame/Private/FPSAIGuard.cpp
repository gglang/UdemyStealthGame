// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "AI/Navigation/NavigationSystem.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	aiState = EAIState::Idle;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::HandlePawnSensed);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::HandleNoiseHeard);

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bPatrol)
	{
		return;
	}

	if (!currentPatrolPoint)
	{
		MoveToNextPatrolPoint();
	}

	float currDist = FVector::Distance(GetActorLocation(), currentPatrolPoint->GetTargetLocation());
	if (currDist < 150.0f)
	{
		MoveToNextPatrolPoint();
	}
}

void AFPSAIGuard::HandlePawnSensed(APawn *SeenPawn)
{
	if (!SeenPawn)
	{
		return;
	}

	FRotator NewLookAt = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), SeenPawn->GetActorLocation());
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(ResetRotationTimer);
	GetWorldTimerManager().SetTimer(ResetRotationTimer, this, &AFPSAIGuard::ResetRotation, 3.0f);
	
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 20.0f, 32, FColor::Red, false, 10.0f);

	AFPSCharacter *fpsCharacter = Cast<AFPSCharacter>(SeenPawn);
	AFPSGameMode *GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (fpsCharacter && GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}
	Controller->StopMovement();
	ChangeAIState(EAIState::Alerted);
}

void AFPSAIGuard::HandleNoiseHeard(APawn* NoiseInstigator, const FVector &Location, float Volume)
{
	FRotator NewLookAt = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Location);
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(ResetRotationTimer);
	GetWorldTimerManager().SetTimer(ResetRotationTimer, this, &AFPSAIGuard::ResetRotation, 3.0f);

	DrawDebugSphere(GetWorld(), Location, 20.0f, 32, FColor::Blue, false, 10.0f);
	Controller->StopMovement();
	ChangeAIState(EAIState::Suspicious);
}

void AFPSAIGuard::ResetRotation()
{
	SetActorRotation(OriginalRotation);

	if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}

	ChangeAIState(EAIState::Idle);
}

void AFPSAIGuard::ChangeAIState(EAIState newState)
{
	if (aiState == EAIState::Alerted || aiState == newState)
	{
		return;
	}

	aiState = newState;
	
	OnAIStateChange(newState);
}

void AFPSAIGuard::MoveToNextPatrolPoint()
{
	if (currentPatrolPoint == nullptr || currentPatrolPoint == secondPatrolPoint)
	{
		currentPatrolPoint = firstPatrolPoint;
	}
	else
	{
		currentPatrolPoint = secondPatrolPoint;
	}

	UNavigationSystem::SimpleMoveToActor(GetController(), currentPatrolPoint);
}