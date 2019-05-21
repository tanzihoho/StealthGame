// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));

	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //设置碰撞为只可查询
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore); //设置和所有类型的东西都不碰撞
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //设置可以跟Pawn类型的物体重叠
	OverlapComp->SetBoxExtent(FVector(200.0f)); //设置盒子大小
	OverlapComp->SetHiddenInGame(false); //设置为游戏中的隐藏项（不）

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap); //绑定重叠响应事件

	RootComponent = OverlapComp; //设置为主组件

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f);
	DecalComp->SetupAttachment(RootComponent);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, 
	bool bFromSweep, 
	const FHitResult & SweepResult)
{

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == nullptr)
	{
		return;
	}
	if (MyPawn->bISCarryingObjective == true)
	{
		AFPSGameMode* GameMode = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->CompleteMission(MyPawn);
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
	UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone!"));
}

