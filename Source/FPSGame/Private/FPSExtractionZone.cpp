// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "UnrealEngine.h"


// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	// Apparently changes to component setups update when you compile in unreal, but adding things like bindings,
	// or calling functions on components WILL NOT UPDATE until you destroy and recreate any BPs this code affects - put that kind of stuff in BeginPlay
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(200.0));
	RootComponent = OverlapComp;
}

void AFPSExtractionZone::HandleOverlap(class UPrimitiveComponent *OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, TEXT("CUCKED"));
	UE_LOG(LogTemp, Log, TEXT("Something entered the extraction zone..."));
}

void AFPSExtractionZone::BeginPlay()
{
	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
}