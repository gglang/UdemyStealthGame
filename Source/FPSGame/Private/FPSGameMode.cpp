// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn *InsigatorPawn)
{
	
	if (InsigatorPawn)
	{
		InsigatorPawn->DisableInput(nullptr);

		if (SpectatingViewpointClass)
		{
			TArray<AActor *> NewSpectatingViewpoints;
			UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, NewSpectatingViewpoints);

			if (NewSpectatingViewpoints.Num() > 0)
			{
				APlayerController *PC = Cast<APlayerController>(InsigatorPawn->GetController());
				if (PC)
				{
					PC->SetViewTargetWithBlend(NewSpectatingViewpoints[0], 0.5f, VTBlend_Cubic);
				}
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Please set SpectatingViewpointClass on FPSGameMode, cannot change view target."));
		}

		OnMissionCompleted();
	}
}
