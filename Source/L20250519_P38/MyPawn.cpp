// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"

#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "MyActor.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMyPawn::AMyPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	RootComponent = Box;

	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(Box);

	//	static 그래픽자료형 변수(TEXT("경로"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Body(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Body.SM_P38_Body'"));
	if (SM_Body.Succeeded())
	{
		Body->SetStaticMesh(SM_Body.Object);
	}


	Left = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left"));
	Left->SetupAttachment(Body);

	Right = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right"));
	Right->SetupAttachment(Body);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Propeller(TEXT("/Script/Engine.StaticMesh'/Game/P38/Meshes/SM_P38_Propeller.SM_P38_Propeller'"));
	if (SM_Propeller.Succeeded())
	{
		Left->SetStaticMesh(SM_Propeller.Object);
		Right->SetStaticMesh(SM_Propeller.Object);
	}

	Left->SetRelativeLocation(FVector(37.571327f, -21.000000f, 1.328775f));
	Right->SetRelativeLocation(FVector(37.571327f, 21.000000f, 1.328775f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(Box);
	SpringArm->TargetArmLength = 170.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->SocketOffset = FVector(0, 0, 41.0f);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(Box);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	Boost = 0.5f;
	MoveSpeed = 10.0f;
	RotateSpeed = 60.0f;

	Movement->MaxSpeed = MoveSpeed;

	Tags.Add(TEXT("Player"));

}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

}


// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddMovementInput(GetActorForwardVector(), Boost);

	Left->AddLocalRotation(FRotator(0, 0, 7200 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));
	Right->AddLocalRotation(FRotator(0, 0, 7200 * UGameplayStatics::GetWorldDeltaSeconds(GetWorld())));

}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction(TEXT("Fire"), EInputEvent::IE_Pressed, this, &AMyPawn::Fire);

	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Pressed, this, &AMyPawn::Doboost);

	PlayerInputComponent->BindAction(TEXT("Boost"), EInputEvent::IE_Released, this, &AMyPawn::Unboost);

	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AMyPawn::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AMyPawn::Roll);

}

void AMyPawn::Pitch(float Value)
{
	AddActorLocalRotation(FRotator(FMath::Clamp(Value, -1.0f, 1.0f) * RotateSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()),
		0,
		0)
	);
}

void AMyPawn::Roll(float Value)
{
	AddActorLocalRotation(FRotator(0,
		0,
		FMath::Clamp(Value, -1.0f, 1.0f) * RotateSpeed * UGameplayStatics::GetWorldDeltaSeconds(GetWorld()))
	);
}

void AMyPawn::Fire()
{
	//문법 : CDO 포인터를 가르침
	//의미 : 클래스 이름을 저장하고 싶다. 
	//RocketTemplate = AMyActor::StaticClass();
	GetWorld()->SpawnActor<AMyActor>(RocketTemplate, Arrow->K2_GetComponentToWorld());
}

void AMyPawn::Doboost()
{
	Boost = 1.0f;
}

void AMyPawn::Unboost()
{
	Boost = 0.5f;
}

