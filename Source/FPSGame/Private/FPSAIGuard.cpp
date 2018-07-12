// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

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
	UE_LOG(LogTemp, Warning, TEXT("DWARF EYES OPENED"));
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

	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 20.0f, 32, FColor::Red, false, 10.0f);
}
