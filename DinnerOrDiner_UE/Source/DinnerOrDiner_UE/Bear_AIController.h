// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Bear_AIController.generated.h"

/**
 * 
 */
UCLASS()
class DINNERORDINER_UE_API ABear_AIController : public AAIController
{
	GENERATED_BODY()

		class UBehaviorTreeComponent* BehaviorComp;

		UBlackboardComponent* BlackboardComp;

		UPROPERTY(EditDefaultsOnly, Category = AI)
			FName LocationToGoKey;

		UPROPERTY(EditDefaultsOnly, Category = AI)
			FName PlayerKey;

		TArray<AActor*> PatrolPoints;

		virtual void OnPossess(APawn* Pawn_) override;


public:

	int32 CurrentTargetPoint = 0;

	ABear_AIController();

	void SetPlayerCaught(APawn* Pawn_);

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }
	FORCEINLINE TArray<AActor*> GetPartolPoints() const { return PatrolPoints; }
	
};
