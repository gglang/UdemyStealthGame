// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "UnrealEngine.h"
#include "Components/DecalComponent.h"
#include "FPSGameMode.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

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

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->SetupAttachment(RootComponent);
	DecalComp->DecalSize = FVector(200.0f);
}

void AFPSExtractionZone::HandleOverlap(class UPrimitiveComponent *OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	AFPSGameMode *GM = Cast<AFPSGameMode>( GetWorld()->GetAuthGameMode() );
	AFPSCharacter *character = Cast<AFPSCharacter>(OtherActor);
	if (!GM || !character)
	{
		return;
	}

	if (character->isCarryingObjective)
	{
		GM->CompleteMission(character);
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, NoObjectiveSound);
	}
}

void AFPSExtractionZone::BeginPlay()
{
	OverlapComp->SetHiddenInGame(false);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
}