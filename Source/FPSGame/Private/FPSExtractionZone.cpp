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

	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly); //������ײΪֻ�ɲ�ѯ
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Ignore); //���ú��������͵Ķ���������ײ
	OverlapComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap); //���ÿ��Ը�Pawn���͵������ص�
	OverlapComp->SetBoxExtent(FVector(200.0f)); //���ú��Ӵ�С
	OverlapComp->SetHiddenInGame(false); //����Ϊ��Ϸ�е����������

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap); //���ص���Ӧ�¼�

	RootComponent = OverlapComp; //����Ϊ�����

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

