// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "UnrealEngine.h"
#include "Engine.h"

// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComp;
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComp");
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	BlackHoleCoreComp = CreateDefaultSubobject<USphereComponent>("BlackHoleCoreComp");
	BlackHoleCoreComp->SetupAttachment(MeshComp);
	BlackHoleCoreComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BlackHoleCoreComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BlackHoleCoreComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	BlackHoleCoreComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent *> overlapping;
	SphereComp->GetOverlappingComponents(overlapping);

	for (UPrimitiveComponent *overlap : overlapping)
	{
		overlap->AddRadialForce(GetActorLocation(), 1000000, -3000, ERadialImpulseFalloff::RIF_Constant, true);
	}
}

void AFPSBlackHole::OnOverlapBegin(class UPrimitiveComponent *OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}