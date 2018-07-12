// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	BoxTriggerComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTriggerComp"));
	BoxTriggerComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxTriggerComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	BoxTriggerComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	BoxTriggerComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	BoxTriggerComp->SetupAttachment(RootComponent);

	LaunchSpeed = 100.0f;
	LaunchPitch = 45.0f;
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
	BoxTriggerComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
}

void AFPSLaunchPad::HandleOverlap(class UPrimitiveComponent *OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Handling Overlap"));

	FRotator launchDirection = GetActorRotation();
	launchDirection.Pitch += LaunchPitch;
	FVector launchVelocity = launchDirection.Vector() * LaunchSpeed;

	AFPSCharacter *playerCharacter = Cast<AFPSCharacter>(OtherActor);
	if (playerCharacter)
	{
		playerCharacter->LaunchCharacter(launchVelocity, true, true);
		UE_LOG(LogTemp, Warning, TEXT("Launching character at: %s"), *launchVelocity.ToString());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchParticleSystem, GetActorTransform());
	}
	else if (OverlappedComponent && OverlappedComponent->IsSimulatingPhysics())
	{
		OverlappedComponent->AddImpulse(launchVelocity, NAME_None, true);
		UE_LOG(LogTemp, Warning, TEXT("Launching NON character at: %s"), *launchVelocity.ToString());

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchParticleSystem, GetActorTransform());
	}
}
