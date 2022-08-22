// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TableActor.generated.h"

UCLASS()
class MYPROJECT_API ATableActor : public AActor
{
	GENERATED_BODY()

private:

	TArray<int32> m_viTriangles;
	TArray<FVector> m_vfVertices;

	//double m_dCurrentHeight;
	double m_dCurrentLength;
	double m_dCurrentBreadth;
	
public:	
	// Sets default values for this actor's properties
	ATableActor();
	void CreateTableMesh(double p_dLengthTableSurface, double p_dBreadthTableSurface, double p_dHeightTableSurface, double p_dWidthTableSupport, double p_dHeightTableSupport);

	void SetLowerLeftPointOfTableSurface(double p_dX, double p_dY, double p_dZ);
	void SetLowerLeftPointOfTableSurface(const FVector& p_ofLowerLeftPointOfTableSurface);
	//void SetHeight(const double p_dHeight);
	void SetLengthTableSurface(const double p_dLength);
	void SetBreadthTableSurface(const double p_dBreadth);

	FVector& GetLowerLeftPointOfTableSurface();
	//double GetHeight() const;
	double GetLength() const;
	double GetBreadth() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		FVector LowerLeftPointOfTableSurface;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UProceduralMeshComponent* CustomMesh;

	UPROPERTY(EditAnywhere)
		UMaterialInterface* Material;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
