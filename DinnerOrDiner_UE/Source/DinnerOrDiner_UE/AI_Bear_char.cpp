// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_Bear_char.h"
#include "Components/CapsuleComponent.h"
#include "Bear_AIController.h"
#include <Kismet/GameplayStatics.h>
#include "Engine.h"
#include <GameFramework/MovementComponent.h>
#include "Math/Vector.h"


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
	AI_SeeRadius =			1500.0f;
	AI_PawnSpotRadius =		500.0f;
	AI_speed =				200.0f;
	AI_velocity =			FVector(0.0f, 0.0f, 0.0f);
	AI_position =			GetActorLocation();
	c_player =				Cast<ADinnerOrDiner_UECharacter>(
								 UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));// get ref to the player

	
	
	
	
}

// Called every frame
void AAI_Bear_char::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PerformRaycast();

	FVector Vecdistance = GetActorLocation() - c_player->GetActorLocation();
	float distance = Vecdistance.Size();
	if (distance <= AI_PawnSpotRadius)
	{
		isSeeking = true;
		
		if (isColliding)
		{
			
			//StopSeeking();
			
			AI_velocity = c_player->GetActorLocation() - GetActorLocation();
			AI_velocity.Normalize();
			AI_velocity *= AI_speed;
			AI_position -= AI_velocity * DeltaTime;
			SetActorLocation(AI_position);
		}
		else
		{
			SeekPlayer(DeltaTime);
		}

		
		
		
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

	
	
		
	RotateTowards(c_player->GetActorLocation());

	// AI movement
	AI_velocity = c_player->GetActorLocation() - GetActorLocation();
	AI_velocity.Normalize();
	AI_velocity *= AI_speed;
	AI_position += AI_velocity * time;
	
	SetActorLocation(AI_position);

	

}

void AAI_Bear_char::SeekTargetPosition(float time, AActor* targetActor_)
{
	

	RotateTowards(targetActor_->GetActorLocation());
	AI_velocity = targetActor_->GetActorLocation() - GetActorLocation();
	AI_velocity.Normalize();
	AI_velocity *= AI_speed;
	AI_position += AI_velocity * time;
	SetActorLocation(AI_position);
	
}

void AAI_Bear_char::StopSeeking()
{
	
	isSeeking = false;
	AI_velocity = FVector();
}

void AAI_Bear_char::PerformRaycast()
{
	FHitResult* HitResult = new FHitResult();
	FVector StartTrace = GetMesh()->GetComponentLocation() + FVector(0.0f, 0.0f, 100.0f);// 100.0 is the offset in the Z because unreal uses the Z axis as up/down instead of Y... why..?
	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	FVector EndTrace = ((ForwardVector * (GetCapsuleComponent()->GetScaledCapsuleRadius() * 0.55f)) + StartTrace);// since we add the StartTrace to the EndTrace we don't need to add an offset unless you want the line to be on angle
	FCollisionQueryParams* TraceParams = new FCollisionQueryParams();

	

	if (GetWorld()->LineTraceSingleByChannel(*HitResult, StartTrace, EndTrace, ECC_Visibility, *TraceParams))//ECC_Visibility
	{
		DrawDebugLine(GetWorld(), StartTrace, EndTrace, FColor(255, 0, 0), false, 5.0f);
		//GetWorld()->DebugDrawTraceTag;

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("You hit: %s"), *HitResult->GetActor()->GetName()));
		
		isColliding = true;
	}
	else
	{
		isColliding = false;
	}

}

void AAI_Bear_char::RotateTowards(FVector ActorLocation_)
{
	// AI transform rotation
	FVector LocA = GetActorLocation();
	FVector LocB = ActorLocation_;

	FVector NewLocation = (LocA + LocB) * 0.5f;
	FRotator NewRotation = (LocB - LocA).Rotation();

	FTransform NewTransform{ NewRotation, NewLocation };

	SetActorTransform(NewTransform);
}

UPawnMovementComponent* AAI_Bear_char::GetMovementComponent() const
{
	return AI_MovementComponent;
}






