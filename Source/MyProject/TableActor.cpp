// Fill out your copyright notice in the Description page of Project Settings.


#include "TableActor.h"
#include "CUtility.h"

// Sets default values
ATableActor::ATableActor():m_dCurrentLength(400.0), m_dCurrentBreadth(400.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TableMesh");
	SetRootComponent(CustomMesh);

	SetLowerLeftPointOfTableSurface(1500, -1600, 200);

	CustomMesh->bUseAsyncCooking = true;

}

void ATableActor::CreateTableMesh(double p_dLengthTableSurface, double p_dBreadthTableSurface, double p_dHeightTableSurface, double p_dWidthTableSupport, double p_dHeightTableSupport)
{
	m_vfVertices.Empty();
	m_viTriangles.Empty();
	FVector oVector = LowerLeftPointOfTableSurface;
	SetLengthTableSurface(p_dLengthTableSurface);
	SetBreadthTableSurface(p_dBreadthTableSurface);
	int vertexSizeTableSurface = CUtility::Generate3DQuadMesh(oVector, p_dLengthTableSurface, p_dBreadthTableSurface, p_dHeightTableSurface, m_vfVertices, m_viTriangles);
	oVector.Z = 0;
	int vertexSizeSupport1 = CUtility::Generate3DQuadMesh(oVector, p_dWidthTableSupport, p_dWidthTableSupport, p_dHeightTableSupport, m_vfVertices, m_viTriangles);
	oVector.Y = LowerLeftPointOfTableSurface.Y + p_dLengthTableSurface - p_dWidthTableSupport;
	int vertexSizeSupport2 = CUtility::Generate3DQuadMesh(oVector, p_dWidthTableSupport, p_dWidthTableSupport, p_dHeightTableSupport, m_vfVertices, m_viTriangles);
	oVector.X = LowerLeftPointOfTableSurface.X + p_dBreadthTableSurface - p_dWidthTableSupport;
	int vertexSizeSupport3 = CUtility::Generate3DQuadMesh(oVector, p_dWidthTableSupport, p_dWidthTableSupport, p_dHeightTableSupport, m_vfVertices, m_viTriangles);
	oVector.Y = LowerLeftPointOfTableSurface.Y;
	int vertexSizeSupport4 = CUtility::Generate3DQuadMesh(oVector, p_dWidthTableSupport, p_dWidthTableSupport, p_dHeightTableSupport, m_vfVertices, m_viTriangles);

	CustomMesh->CreateMeshSection_LinearColor(0, m_vfVertices, m_viTriangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);

	FString fsMaterialFile = "/Game/StarterContent/Materials/M_Wood_Walnut.M_Wood_Walnut";
	UMaterialInterface* ptrMatInterface = nullptr;
	if (CUtility::GetMaterialFromFile(fsMaterialFile, &ptrMatInterface))
	{
		CustomMesh->SetMaterial(0, ptrMatInterface);
	}
}

// Called when the game starts or when spawned
void ATableActor::BeginPlay()
{
	Super::BeginPlay();
	CreateTableMesh(400, 400, 30, 50, 200);
}

// Called every frame
void ATableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATableActor::SetLowerLeftPointOfTableSurface(double p_dX, double p_dY, double p_dZ)
{
	LowerLeftPointOfTableSurface.X = p_dX;
	LowerLeftPointOfTableSurface.Y = p_dY;
	LowerLeftPointOfTableSurface.Z = p_dZ;
}

void ATableActor::SetLowerLeftPointOfTableSurface(const FVector& p_ofLowerLeftPointOfTableSurface)
{
	LowerLeftPointOfTableSurface = p_ofLowerLeftPointOfTableSurface;
}

void ATableActor::SetLengthTableSurface(const double p_dLength)
{
	m_dCurrentLength = p_dLength;
}

void ATableActor::SetBreadthTableSurface(const double p_dBreadth)
{
	m_dCurrentBreadth = p_dBreadth;
}

FVector& ATableActor::GetLowerLeftPointOfTableSurface()
{
	return LowerLeftPointOfTableSurface;
}

//double ATableActor::GetHeight() const
//{
//	return m_dCurrentHeight;
//}

double ATableActor::GetLength() const
{
	return m_dCurrentLength;
}

double ATableActor::GetBreadth() const
{
	return m_dCurrentBreadth;
}



