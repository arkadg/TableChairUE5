// Fill out your copyright notice in the Description page of Project Settings.


#include "ChairActor.h"
#include "CUtility.h"

// Sets default values
AChairActor::AChairActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("ChairMesh");
	SetRootComponent(CustomMesh);

	CustomMesh->bUseAsyncCooking = true;
}

void AChairActor::CreateChairMesh(double p_dLengthSeatSurface ,double p_dHeightSeatSurface, double p_dWidthLeg, double p_dHeightLeg, double p_dHeightBackSupport, double p_dWidthBackSupport)
{
	m_vfVertices.Empty();
	m_viTriangles.Empty();
	FVector oVector = CenterPointOfSeat;
	int vertexSeatSurface = CUtility::Generate3DQuadMesh(oVector, p_dLengthSeatSurface, p_dLengthSeatSurface, p_dHeightSeatSurface, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfSeat.X - p_dLengthSeatSurface / 2 + p_dWidthLeg / 2;
	oVector.Y = CenterPointOfSeat.Y - p_dLengthSeatSurface / 2 + p_dWidthLeg / 2;
	oVector.Z = CenterPointOfSeat.Z - p_dHeightSeatSurface / 2 - p_dHeightLeg / 2;
	int vertexSizeSupport1 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfSeat.X - p_dLengthSeatSurface / 2 + p_dWidthLeg / 2;
	oVector.Y = CenterPointOfSeat.Y + p_dLengthSeatSurface / 2 - p_dWidthLeg / 2;
	int vertexSizeSupport2 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfSeat.X + p_dLengthSeatSurface / 2 - p_dWidthLeg / 2;
	oVector.Y = CenterPointOfSeat.Y - p_dLengthSeatSurface / 2 + p_dWidthLeg / 2;
	int vertexSizeSupport3 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfSeat.X + p_dLengthSeatSurface / 2 - p_dWidthLeg / 2;
	oVector.Y = CenterPointOfSeat.Y + p_dLengthSeatSurface / 2 - p_dWidthLeg / 2;
	int vertexSizeSupport4 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);

	// Back Support Code
	oVector.X = CenterPointOfSeat.X - p_dLengthSeatSurface / 2 + p_dWidthBackSupport / 2;
	oVector.Y = CenterPointOfSeat.Y - p_dLengthSeatSurface / 2 + p_dWidthBackSupport / 2;
	oVector.Z = CenterPointOfSeat.Z + p_dHeightSeatSurface / 2 + p_dHeightBackSupport / 2;
	int vertexBackSupport1 = CUtility::Generate3DQuadMesh(oVector, p_dWidthBackSupport, p_dWidthBackSupport, p_dHeightBackSupport, m_vfVertices, m_viTriangles);

	oVector.X = CenterPointOfSeat.X - p_dLengthSeatSurface / 2 + p_dWidthBackSupport / 2;
	oVector.Y = CenterPointOfSeat.Y + p_dLengthSeatSurface / 2 - p_dWidthBackSupport / 2;
	int vertexBackSupport2 = CUtility::Generate3DQuadMesh(oVector, p_dWidthBackSupport, p_dWidthBackSupport, p_dHeightBackSupport, m_vfVertices, m_viTriangles);

	oVector.X = CenterPointOfSeat.X - p_dLengthSeatSurface / 2 + p_dWidthBackSupport / 2;
	oVector.Y = CenterPointOfSeat.Y;
	oVector.Z = CenterPointOfSeat.Z + p_dHeightSeatSurface / 2 + p_dHeightBackSupport  - p_dWidthBackSupport / 2;
	double dHorizontalSupportWidth = p_dLengthSeatSurface;
	int vertexBackSupport3 = CUtility::Generate3DQuadMesh(oVector, dHorizontalSupportWidth, p_dWidthBackSupport, p_dWidthBackSupport, m_vfVertices, m_viTriangles);

	oVector.Z = CenterPointOfSeat.Z + p_dHeightSeatSurface/2 + (p_dHeightBackSupport / 3);
	int vertexBackSupport4 = CUtility::Generate3DQuadMesh(oVector, dHorizontalSupportWidth, p_dWidthBackSupport, p_dWidthBackSupport, m_vfVertices, m_viTriangles);


	CustomMesh->CreateMeshSection_LinearColor(0, m_vfVertices, m_viTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	FString fsMaterialFile = "/Game/StarterContent/Materials/M_Metal_Rust.M_Metal_Rust'";
	UMaterialInterface* ptrMatInterface = nullptr;
	if (CUtility::GetMaterialFromFile(fsMaterialFile, &ptrMatInterface))
	{
		CustomMesh->SetMaterial(0, ptrMatInterface);
	}

}

// Called when the game starts or when spawned
void AChairActor::BeginPlay()
{
	Super::BeginPlay();
	CreateChairMesh(150, 15, 20, 110,90 , 20);
}

// Called every frame
void AChairActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChairActor::SetCenterPointOfSeatSurface(double p_dX, double p_dY, double p_dZ)
{
	CenterPointOfSeat.X = p_dX;
	CenterPointOfSeat.Y = p_dY;
	CenterPointOfSeat.Z = p_dZ;
}

void AChairActor::SetCenterPointOfSeatSurface(const FVector& p_ofLowerLeftPointOfSeatSurface)
{
	CenterPointOfSeat = p_ofLowerLeftPointOfSeatSurface;
}

UProceduralMeshComponent* AChairActor::GetProceduralMeshComponent()
{
	return CustomMesh;
}