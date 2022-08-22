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
	
public:	
	// Sets default values for this actor's properties
	AChairActor();
	void CreateChairMesh(double p_dLengthSeatSurface, double p_dHeightSeatSurface, double p_dWidthLeg, double p_dHeightLeg,double p_dHeightBackSupport,double p_dWidthBackSupport);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
		FVector LowerLeftPointOfSeat;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	void SetLowerLeftPointOfSeatSurface(double p_dX, double p_dY, double p_dZ);
	void SetLowerLeftPointOfSeatSurface(const FVector& p_ofLowerLeftPointOfSeatSurface);

};
