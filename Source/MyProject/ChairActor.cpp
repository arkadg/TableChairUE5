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

	LowerLeftPointOfSeat.Z = 110;

	CustomMesh->bUseAsyncCooking = true;
}

void AChairActor::CreateChairMesh(double p_dLengthSeatSurface ,double p_dHeightSeatSurface, double p_dWidthLeg, double p_dHeightLeg, double p_dHeightBackSupport, double p_dWidthBackSupport)
{
	m_vfVertices.Empty();
	m_viTriangles.Empty();
	FVector oVector = LowerLeftPointOfSeat;
	int vertexSizeSeat = CUtility::Generate3DQuadMesh(oVector, p_dLengthSeatSurface, p_dLengthSeatSurface, p_dHeightSeatSurface, m_vfVertices, m_viTriangles);
	oVector.Z = 0;
	int vertexSizeSupport1 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);
	oVector.Y = LowerLeftPointOfSeat.Y + p_dLengthSeatSurface - p_dWidthLeg;
	int vertexSizeSupport2 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);
	oVector.X = LowerLeftPointOfSeat.X + p_dLengthSeatSurface - p_dWidthLeg;
	int vertexSizeSupport3 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);
	oVector.Y = LowerLeftPointOfSeat.Y;
	int vertexSizeSupport4 = CUtility::Generate3DQuadMesh(oVector, p_dWidthLeg, p_dWidthLeg, p_dHeightLeg, m_vfVertices, m_viTriangles);

	// Back Support Code
	oVector.X = LowerLeftPointOfSeat.X;
	oVector.Y = LowerLeftPointOfSeat.Y;
	oVector.Z = LowerLeftPointOfSeat.Z + p_dHeightSeatSurface;
	int vertexBackSupport1 = CUtility::Generate3DQuadMesh(oVector, p_dWidthBackSupport, p_dWidthBackSupport, p_dHeightBackSupport, m_vfVertices, m_viTriangles);

	oVector.X = LowerLeftPointOfSeat.X + p_dLengthSeatSurface - p_dWidthBackSupport;
	oVector.Y = LowerLeftPointOfSeat.Y;
	int vertexBackSupport2 = CUtility::Generate3DQuadMesh(oVector, p_dWidthBackSupport, p_dWidthBackSupport, p_dHeightBackSupport, m_vfVertices, m_viTriangles);

	oVector.X = LowerLeftPointOfSeat.X;
	oVector.Y = LowerLeftPointOfSeat.Y;
	oVector.Z = LowerLeftPointOfSeat.Z + p_dHeightSeatSurface + p_dHeightBackSupport;
	double dHorizontalSupportWidth = p_dLengthSeatSurface;
	int vertexBackSupport3 = CUtility::Generate3DQuadMesh(oVector, p_dWidthBackSupport, dHorizontalSupportWidth, p_dWidthBackSupport, m_vfVertices, m_viTriangles);

	oVector.X = LowerLeftPointOfSeat.X;
	oVector.Y = LowerLeftPointOfSeat.Y;
	oVector.Z = LowerLeftPointOfSeat.Z + p_dHeightSeatSurface + (p_dHeightBackSupport / 2.5);
	int vertexBackSupport4 = CUtility::Generate3DQuadMesh(oVector, p_dWidthBackSupport, dHorizontalSupportWidth, p_dWidthBackSupport, m_vfVertices, m_viTriangles);


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

void AChairActor::SetLowerLeftPointOfSeatSurface(double p_dX, double p_dY, double p_dZ)
{
	LowerLeftPointOfSeat.X = p_dX;
	LowerLeftPointOfSeat.Y = p_dY;
	LowerLeftPointOfSeat.Z = p_dZ;
}

void AChairActor::SetLowerLeftPointOfSeatSurface(const FVector& p_ofLowerLeftPointOfSeatSurface)
{
	LowerLeftPointOfSeat = p_ofLowerLeftPointOfSeatSurface;
}

