// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	
	PawnSensingComponent->OnSeePawn.AddDynamic(this, &AFPSAIGuard::HandlePawnSensed);
	PawnSensingComponent->OnHearNoise.AddDynamic(this, &AFPSAIGuard::HandleNoiseHeard);
	UE_LOG(LogTemp, Warning, TEXT("DWARF READY"));
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::HandlePawnSensed(APawn *SeenPawn)
{
	UE_LOG(LogTemp, Warning, TEXT("DWARF FUCK SAW SOMETHING"));
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
}

void AFPSAIGuard::HandleNoiseHeard(APawn* NoiseInstigator, const FVector &Location, float Volume)
{
	UE_LOG(LogTemp, Warning, TEXT("DWARF FUCK HEARD SOMETHING"));

	FRotator NewLookAt = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Location);
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);

	GetWorldTimerManager().ClearTimer(ResetRotationTimer);
	GetWorldTimerManager().SetTimer(ResetRotationTimer, this, &AFPSAIGuard::ResetRotation, 3.0f);

	DrawDebugSphere(GetWorld(), Location, 20.0f, 32, FColor::Blue, false, 10.0f);
}

void AFPSAIGuard::ResetRotation()
{
	SetActorRotation(OriginalRotation);
}