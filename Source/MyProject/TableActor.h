// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "TableActor.generated.h"

enum class TouchPoints
{
	eLeftBack = 0,
	eLeftFront,
	eRightBack,
	eRightFront,
	eTouchNone
};


UCLASS()
class MYPROJECT_API ATableActor : public AActor
{
	GENERATED_BODY()

private:

	TArray<FVector> m_vfVertices;
	TArray<FVector> m_vfUpdatedVertices;
	TArray<int32> m_viTriangles;

	FVector CenterPointOfTableSurface;

	double m_dCurrentHeight;
	double m_dCurrentLength;
	double m_dCurrentBreadth;

	FVector2D m_oTablePointBackLeft;
	FVector2D m_oTablePointBackRight;
	FVector2D m_oTablePointFrontLeft;
	FVector2D m_oTablePointFrontRight;


	FVector2D m_oInitLeftBack = FVector2D(1300, -1400);
	FVector2D m_oInitRightBack = FVector2D(1300, -1000);
	FVector2D m_oInitLeftFront = FVector2D(1700, -1400);
	FVector2D m_oInitRightFront = FVector2D(1700, -1000);
	
public:	
	// Sets default values for this actor's properties
	ATableActor();
	void CreateTableMesh(double p_dLengthTableSurface, double p_dBreadthTableSurface, double p_dHeightTableSurface, double p_dWidthTableSupport, double p_dHeightTableSupport);

	void SetCenterPointOfTableSurface(double p_dX, double p_dY, double p_dZ);
	void SetCenterPointOfTableSurface(const FVector& p_ofLowerLeftPointOfTableSurface);
	//void SetHeight(const double p_dHeight);
	void SetLengthTableSurface(const double p_dLength);
	void SetBreadthTableSurface(const double p_dBreadth);

	FVector& GetCenterPointOfTableSurface();
	double GetHeight() const;
	double GetLength() const;
	double GetBreadth() const;

	UProceduralMeshComponent* GetProceduralMeshComponent();

	void SetTableBackLeft(const FVector2D& p_oTablePointBackLeft);
	void SetTableBackRight(const FVector2D& p_oTablePointBackRight);
	void SetTableFrontLeft(const FVector2D& p_oTablePointFrontLeft);
	void SetTableFrontRight(const FVector2D& p_oTablePointFrontRight);

	void GetTableBackLeft(FVector2D& p_oTablePointBackLeft);
	void GetTableBackRight(FVector2D& p_oTablePointBackRight);
	void GetTableFrontLeft(FVector2D& p_oTablePointFrontLeft);
	void GetTableFrontRight(FVector2D& p_oTablePointFrontRight);

	void SetVertices(const TArray<FVector>& p_arrUpdatedVertices);
	void GetUpdatedVertices(TArray<FVector>& p_arrUpdatedVertices);

	void CaptureUpdatedVerticesData(const TArray<FVector>& p_arrUpdatedVertices, TouchPoints eTouchPoint, const FVector2D& p_fvStretch);

	void UpdateMesh(TouchPoints p_eTouchPoint,FVector2D& p_fvStretch);

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
