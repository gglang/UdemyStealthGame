// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

class UBoxComponent;
class UDecalComponent;

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent *MeshComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UBoxComponent *BoxTriggerComp;

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UDecalComponent *DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	UParticleSystem *LaunchParticleSystem;

	UPROPERTY(EditDefaultsOnly, Category = "Parameters")
	float LaunchSpeed;

public:	
	UFUNCTION()
	void HandleOverlap(class UPrimitiveComponent *OverlappedComponent, AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

// 	void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
