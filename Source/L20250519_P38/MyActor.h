// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyActor.generated.h"

class UProjectileMovementComponent;
class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class L20250519_P38_API AMyActor : public AActor
{

	GENERATED_BODY()

	
public:	
	// Sets default values for this actor's properties
	AMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Compoennts")
	TObjectPtr<UBoxComponent> Box;
	//UBoxComponent* Box;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Compoennts")
	TObjectPtr<UStaticMeshComponent> Body;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Compoennts")
	TObjectPtr<UProjectileMovementComponent> Movement;

	UFUNCTION(BlueprintCallable)
	void Test();

};
