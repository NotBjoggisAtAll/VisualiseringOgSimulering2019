// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PaperFlipbookComponent.h"
#include "Classes/Components/ChildActorComponent.h"

#include "FlyingPigFlipbookActor.generated.h"

class UBrain_ActorComponent;

UCLASS()
class FLYINGPIGPROJECT_00_API AFlyingPigFlipbookActor : public AActor
{
	GENERATED_BODY()
	
private:


public:	
	// Sets default values for this actor's properties
	AFlyingPigFlipbookActor();
	UPaperFlipbookComponent *paperFlipbookComponent;

	UPROPERTY(VisibleAnywhere)
	UBrain_ActorComponent* Brain;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnPigHit(
			UPrimitiveComponent *hitComp,
			AActor *otherActor,
			UPrimitiveComponent *otherComp,
			FVector NormalImpulse,
			const FHitResult& hit
		);

	bool bFailed = false;

	FVector StartLocation;

	void Move(FVector direction, float DeltaTime);

};
