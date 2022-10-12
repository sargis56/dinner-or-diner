// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bear_char.h"
#include "Components/CapsuleComponent.h"
#include "Bear_AIController.h"
#include <Kismet/GameplayStatics.h>


// Sets default values
AAI_Bear_char::AAI_Bear_char()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


}

// Called when the game starts or when spawned
void AAI_Bear_char::BeginPlay()
{
	Super::BeginPlay();

	//init senses and other vars
	AI_SeeRadius = 1500.0f;
	AI_PawnSpotRadius = 500.0f;
	AI_speed = 200.0f;
	AI_velocity = FVector(0.0f, 0.0f, 0.0f);
	AI_position = GetActorLocation();
	c_player = Cast<ADinnerOrDiner_UECharacter>(
					UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));// get ref to the player
	
}

// Called every frame
void AAI_Bear_char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Vecdistance = GetActorLocation() - c_player->GetActorLocation();
	float distance = Vecdistance.Size();
	if (distance <= AI_PawnSpotRadius)
	{
		SeekPlayer(DeltaTime);
		GEngine->AddOnScreenDebugMessage(-1, 0.017f, FColor::Green, TEXT("Chasing Player"));
	}
	else if (distance <= AI_SeeRadius)
	{
		GEngine->AddOnScreenDebugMessage(-1, 0.017f, FColor::Green, TEXT("I think I see Something?"));
	}
	else if (distance > AI_PawnSpotRadius)
	{
		StopSeeking();
		GEngine->AddOnScreenDebugMessage(-1, 0.017f, FColor::Red, TEXT("Waiting For EIS"));
	}

}

// Called to bind functionality to input
void AAI_Bear_char::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AAI_Bear_char::SeekPlayer(float time)
{
	AI_velocity = c_player->GetActorLocation() - GetActorLocation();
	AI_velocity.Normalize();
	AI_velocity *= AI_speed;
	AI_position += AI_velocity * time;
	SetActorLocation(AI_position);

	

}

void AAI_Bear_char::SeekTargetPosition(float time, AActor* targetActor_)
{
	AI_velocity = targetActor_->GetActorLocation() - GetActorLocation();
	AI_velocity.Normalize();
	AI_velocity *= AI_speed;
	AI_position += AI_velocity * time;
	SetActorLocation(AI_position);
}

void AAI_Bear_char::StopSeeking()
{
	AI_velocity = FVector();
}



