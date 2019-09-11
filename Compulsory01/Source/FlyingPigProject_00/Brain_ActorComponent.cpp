// Fill out your copyright notice in the Description page of Project Settings.


#include "Brain_ActorComponent.h"
#include "ArtificialNN/ArtificialNN.h"
#include "FlyingPigFlipbookActor.h"
#include "Engine/Engine.h"
// Sets default values for this component's properties
UBrain_ActorComponent::UBrain_ActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UBrain_ActorComponent::BeginPlay()
{
	Super::BeginPlay();

	ann = new ArtificialNN(4, 2, 3, 24, 0.4f);
	pig = Cast<AFlyingPigFlipbookActor>(GetOwner());

}


// Called every frame
void UBrain_ActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	timer += DeltaTime;
	if (!pig)
	{
		GEngine->AddOnScreenDebugMessage(
			0, 60.0f, FColor::Cyan,
			FString("The brain can't find the Pig.")
		);
		return;
	}


	TArray<double> states;
	TArray<double> qs;

	FHitResult HitUp;
	FHitResult HitDown;
	FCollisionQueryParams params;
	params.AddIgnoredActor(pig);

	GetWorld()->LineTraceSingleByChannel(HitUp, pig->GetActorLocation(), pig->GetActorLocation() + FVector(0, 0, 10000), ECC_WorldStatic, params);

	GetWorld()->LineTraceSingleByChannel(HitDown, pig->GetActorLocation(), pig->GetActorLocation() + FVector(0, 0, -10000), ECC_WorldStatic, params);


	auto DistancePigStart = FVector::Distance(pig->GetActorLocation(), pig->StartLocation);


	states.Add(HitUp.Distance);
	states.Add(HitDown.Distance);
	states.Add(DistancePigStart);
	states.Add(pig->GetVelocity().Z);

	qs = SoftMax(ann->CalcOutput(states));
	double maxQ = FMath::Max(qs);
	int maxQIndex = qs.Find(maxQ);
	exploreRate = FMath::Clamp(exploreRate - exploreDecay, minExploreRate, maxExploreRate);

	auto impulse = qs[maxQIndex] * MaxVelocity;


	FString message = FString::Printf(
		TEXT("\n\n\n\
			Stats\n\
			Impulse: %f\n\
			Fails: %d\n\
			Decay Rate: %f\n\
			Last Best Balance: %f\n\
			This Balance: %f"),
		impulse, failCount, exploreRate, maxBalanceTime, timer);

	GEngine->AddOnScreenDebugMessage(
		0, 0.5f, FColor::Cyan,
		message
	);

	//if (maxQIndex == 1)
		pig->Move(FVector(0, 0, 1) * impulse, DeltaTime);


	if (pig->bFailed == true)
	{
		reward = -1.0f;
	}
	else
	{
		reward = 0.1f;
	}

	Replay* lastMemory = new Replay(
		HitUp.Distance,
		HitDown.Distance,
		DistancePigStart,
		pig->GetVelocity().Z,
		reward);

	if (replayMemory.Num() > mCapacity)
		replayMemory.RemoveAt(0);

	replayMemory.Add(lastMemory);

	if (pig->bFailed == true)
	{
		for (int i = replayMemory.Num() - 1; i >= 0; i--)
		{
			TArray<double> t_outputsOld;
			TArray<double> t_outputsNew;
			t_outputsOld = SoftMax(ann->CalcOutput(replayMemory[i]->states));

			double maxQOld = FMath::Max(t_outputsOld);
			int action = t_outputsOld.Find(maxQOld);

			double feedback;
			if (i == replayMemory.Num() - 1 || replayMemory[i]->reward == -1)
			{
				feedback = replayMemory[i]->reward;
			}
			else
			{
				t_outputsNew = SoftMax(ann->CalcOutput(replayMemory[i + 1]->states));
				maxQ = FMath::Max(t_outputsNew);
				feedback = (replayMemory[i]->reward + discount * maxQ);
			}

			t_outputsOld[action] = feedback;
			ann->Train(replayMemory[i]->states, t_outputsOld);
		}

		//Here, we need to keep the best time and reset the ball
		if (timer > maxBalanceTime)
		{
			maxBalanceTime = timer;
		}

		timer = 0;

		GetOwner()->SetActorLocation(pig->StartLocation);
		pig->paperFlipbookComponent->SetSimulatePhysics(false);
		pig->paperFlipbookComponent->SetSimulatePhysics(true);
		replayMemory.Empty();
		pig->bFailed = false;
		failCount++;
	}
}

TArray<double> UBrain_ActorComponent::SoftMax(TArray<double> values)
{
	//double max = values.Max();
	double max = FMath::Max<double>(values);

	float scale = 0.0f;
	for (int i = 0; i < values.Num(); ++i)
		scale += FMath::Exp((float)(values[i] - max));

	TArray<double> result;
	for (int i = 0; i < values.Num(); ++i)
		result.Add(FMath::Exp((float)(values[i] - max)) / scale);

	return result;
}
