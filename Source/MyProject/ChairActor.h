// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "ChairActor.generated.h"

UCLASS()
class MYPROJECT_API AChairActor : public AActor
{
	GENERATED_BODY()

private:
	TArray<int32> m_viTriangles;
	TArray<FVector> m_vfVertices;

	FVector CenterPointOfSeat;
	
public:	
	// Sets default values for this actor's properties
	AChairActor();
	void CreateChairMesh(double p_dLengthSeatSurface, double p_dHeightSeatSurface, double p_dWidthLeg, double p_dHeightLeg,double p_dHeightBackSupport,double p_dWidthBackSupport);
	UProceduralMeshComponent* GetProceduralMeshComponent();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
