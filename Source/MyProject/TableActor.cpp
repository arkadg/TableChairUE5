// Fill out your copyright notice in the Description page of Project Settings.


#include "TableActor.h"
#include "CUtility.h"
#include "DefaultPlayerController.h"

#define INITIAL_LENGTH_OF_TABLE 400
#define INITIAL_BREADTH_OF_TABLE 400
#define TABLE_SURFACE_WIDTH 30
#define TABLE_SUPPORT_WIDTH 50
#define TABLE_SUPPORT_HEIGHT 200


// Sets default values
ATableActor::ATableActor():m_dCurrentLength(400.0), m_dCurrentBreadth(400.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TableMesh");
	SetRootComponent(CustomMesh);

	CustomMesh->bUseAsyncCooking = true;

}

void ATableActor::CreateTableMesh(double p_dLengthTableSurface, double p_dBreadthTableSurface, double p_dHeightTableSurface, double p_dWidthTableSupport, double p_dHeightTableSupport)
{
	m_vfVertices.Empty();
	m_viTriangles.Empty();
	FVector oVector = CenterPointOfTableSurface;

	SetLengthTableSurface(p_dLengthTableSurface);
	SetBreadthTableSurface(p_dBreadthTableSurface);
	int vertexSizeTableSurface = CUtility::Generate3DQuadMesh(oVector, p_dLengthTableSurface, p_dBreadthTableSurface, p_dHeightTableSurface, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfTableSurface.X - p_dBreadthTableSurface / 2 + p_dWidthTableSupport / 2;
	oVector.Y = CenterPointOfTableSurface.Y - p_dLengthTableSurface / 2 + p_dWidthTableSupport / 2;
	oVector.Z = CenterPointOfTableSurface.Z - p_dHeightTableSurface/2 - p_dHeightTableSupport / 2;
	int vertexSizeSupport1 = CUtility::Generate3DQuadMesh(oVector, p_dWidthTableSupport, p_dWidthTableSupport, p_dHeightTableSupport, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfTableSurface.X - p_dBreadthTableSurface / 2 + p_dWidthTableSupport / 2;
	oVector.Y = CenterPointOfTableSurface.Y + p_dLengthTableSurface / 2 - p_dWidthTableSupport / 2;
	int vertexSizeSupport2 = CUtility::Generate3DQuadMesh(oVector, p_dWidthTableSupport, p_dWidthTableSupport, p_dHeightTableSupport, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfTableSurface.X + p_dBreadthTableSurface / 2 - p_dWidthTableSupport / 2;
	oVector.Y = CenterPointOfTableSurface.Y - p_dLengthTableSurface / 2 + p_dWidthTableSupport / 2;
	int vertexSizeSupport3 = CUtility::Generate3DQuadMesh(oVector, p_dWidthTableSupport, p_dWidthTableSupport, p_dHeightTableSupport, m_vfVertices, m_viTriangles);
	oVector.X = CenterPointOfTableSurface.X + p_dBreadthTableSurface / 2 - p_dWidthTableSupport / 2;
	oVector.Y = CenterPointOfTableSurface.Y + p_dLengthTableSurface / 2 - p_dWidthTableSupport / 2;
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
	CreateTableMesh(INITIAL_LENGTH_OF_TABLE, INITIAL_BREADTH_OF_TABLE, TABLE_SURFACE_WIDTH, TABLE_SUPPORT_WIDTH, TABLE_SUPPORT_HEIGHT);
}

// Called every frame
void ATableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATableActor::SetCenterPointOfTableSurface(double p_dX, double p_dY, double p_dZ)
{
	CenterPointOfTableSurface.X = p_dX;
	CenterPointOfTableSurface.Y = p_dY;
	CenterPointOfTableSurface.Z = p_dZ;
}

void ATableActor::SetCenterPointOfTableSurface(const FVector& p_ofLowerLeftPointOfTableSurface)
{
	CenterPointOfTableSurface = p_ofLowerLeftPointOfTableSurface;
}

void ATableActor::SetLengthTableSurface(const double p_dLength)
{
	m_dCurrentLength = p_dLength;
}

void ATableActor::SetBreadthTableSurface(const double p_dBreadth)
{
	m_dCurrentBreadth = p_dBreadth;
}

FVector& ATableActor::GetCenterPointOfTableSurface()
{
	return CenterPointOfTableSurface;
}

double ATableActor::GetHeight() const
{
	return m_dCurrentHeight;
}

double ATableActor::GetLength() const
{
	return m_dCurrentLength;
}

double ATableActor::GetBreadth() const
{
	return m_dCurrentBreadth;
}

UProceduralMeshComponent* ATableActor::GetProceduralMeshComponent()
{
	return CustomMesh;
}


void ATableActor::UpdateMesh(TouchPoints p_eTouchPoint,FVector2D& p_fvStretch)
{
	bool bChangesDone = false;
	
	TArray<FVector> updatedVertices = m_vfVertices;
	m_vfUpdatedVertices = m_vfVertices;
	FVector diff = FVector::ZeroVector;
	if (p_eTouchPoint == TouchPoints::eLeftBack)
	{
		if (m_oTablePointBackLeft.X + p_fvStretch.X > m_oInitLeftBack.X)
		{
			p_fvStretch.X = m_oInitLeftBack.X - m_oTablePointBackLeft.X;
		}
		if (m_oTablePointBackLeft.Y + p_fvStretch.Y > m_oInitLeftBack.Y)
		{
			p_fvStretch.Y = m_oTablePointBackLeft.Y - m_oInitLeftBack.Y;
		}
		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);
		updatedVertices[0] = updatedVertices[0] + diff;
		updatedVertices[1] = updatedVertices[1] + diff;
		updatedVertices[2].X = updatedVertices[2].X + p_fvStretch.X;
		updatedVertices[3].X = updatedVertices[3].X + p_fvStretch.X;
		updatedVertices[4].Y = updatedVertices[4].Y + p_fvStretch.Y;
		updatedVertices[5].Y = updatedVertices[5].Y + p_fvStretch.Y;
			
		for (int i = 8; i < 16; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		for (int i = 16; i < 24; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		for (int i = 24; i < 32; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		bChangesDone = true;

	}
	else if (p_eTouchPoint == TouchPoints::eLeftFront)
	{
		if (m_oTablePointFrontLeft.X + p_fvStretch.X < m_oInitLeftFront.X)
		{
			p_fvStretch.X = m_oInitLeftFront.X - m_oInitLeftFront.X;
		}
		if (m_oTablePointFrontLeft.Y + p_fvStretch.Y > m_oInitLeftFront.Y)
		{
			p_fvStretch.Y = m_oTablePointFrontLeft.Y - m_oInitLeftFront.Y;
		}
		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);
		updatedVertices[4] = updatedVertices[4] + diff;
		updatedVertices[5] = updatedVertices[5] + diff;
		updatedVertices[0].Y = updatedVertices[0].Y + p_fvStretch.Y;
		updatedVertices[1].Y = updatedVertices[1].Y + p_fvStretch.Y;
		updatedVertices[6].X = updatedVertices[6].X + p_fvStretch.X;
		updatedVertices[7].X = updatedVertices[7].X + p_fvStretch.X;

		for (int i = 8; i < 16; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		for (int i = 24; i < 32; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		for (int i = 32; i < 40; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		bChangesDone = true;
	}
	else if (p_eTouchPoint == TouchPoints::eRightBack)
	{
		if (m_oTablePointBackRight.X + p_fvStretch.X > m_oInitRightBack.X)
		{
			p_fvStretch.X = m_oInitRightBack.X - m_oTablePointBackRight.X;
		}
		if (m_oTablePointBackRight.Y + p_fvStretch.Y < m_oInitRightBack.Y)
		{
			p_fvStretch.Y = m_oInitRightBack.Y - m_oTablePointBackRight.Y;
		}
		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);
		updatedVertices[2] = updatedVertices[2] + diff;
		updatedVertices[3] = updatedVertices[3] + diff;
		updatedVertices[0].X = updatedVertices[0].X + p_fvStretch.X;
		updatedVertices[1].X = updatedVertices[1].X + p_fvStretch.X;
		updatedVertices[6].Y = updatedVertices[6].Y + p_fvStretch.Y;
		updatedVertices[7].Y = updatedVertices[7].Y + p_fvStretch.Y;

		for (int i = 8; i < 16; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		for (int i = 16; i < 24; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		for (int i = 32; i < 40; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		bChangesDone = true;
	}
	else if (p_eTouchPoint == TouchPoints::eRightFront)
	{
		if (m_oTablePointFrontRight.X + p_fvStretch.X < m_oInitRightFront.X)
		{
			p_fvStretch.X = m_oTablePointFrontRight.X - m_oInitRightFront.X;
		}

		if (m_oTablePointFrontRight.Y + p_fvStretch.Y < m_oInitRightFront.Y)
		{
			p_fvStretch.Y = m_oInitRightFront.Y - m_oTablePointFrontRight.Y;
		}

		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);
		updatedVertices[2].Y = updatedVertices[2].Y + p_fvStretch.Y;
		updatedVertices[3].Y = updatedVertices[3].Y + p_fvStretch.Y;
		updatedVertices[4].X = updatedVertices[4].X + p_fvStretch.X;
		updatedVertices[5].X = updatedVertices[5].X + p_fvStretch.X;
		updatedVertices[6] = updatedVertices[6] + diff;
		updatedVertices[7] = updatedVertices[7] + diff;

		for (int i = 16; i < 24; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		for (int i = 24; i < 32; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		for (int i = 32; i < 40; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		bChangesDone = true;
	}

	if (bChangesDone)
	{
		m_vfUpdatedVertices.Empty();
		CustomMesh->UpdateMeshSection_LinearColor(0, updatedVertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
		m_vfUpdatedVertices = updatedVertices;
	}
}

void ATableActor::SetTableBackLeft(const FVector2D& p_oTablePointBackLeft)
{
	m_oTablePointBackLeft = p_oTablePointBackLeft;
}
void ATableActor::SetTableBackRight(const FVector2D& p_oTablePointBackRight)
{
	m_oTablePointBackRight = p_oTablePointBackRight;
}
void ATableActor::SetTableFrontLeft(const FVector2D& p_oTablePointFrontLeft)
{
	m_oTablePointFrontLeft = p_oTablePointFrontLeft;
}
void ATableActor::SetTableFrontRight(const FVector2D& p_oTablePointFrontRight)
{
	m_oTablePointFrontRight = p_oTablePointFrontRight;
}


void ATableActor::GetTableBackLeft(FVector2D& p_oTablePointBackLeft)
{
	p_oTablePointBackLeft = m_oTablePointBackLeft;
}
void ATableActor::GetTableBackRight(FVector2D& p_oTablePointBackRight)
{
	p_oTablePointBackRight = m_oTablePointBackRight;
}
void ATableActor::GetTableFrontLeft(FVector2D& p_oTablePointFrontLeft)
{
	p_oTablePointFrontLeft = m_oTablePointFrontLeft;
}
void ATableActor::GetTableFrontRight(FVector2D& p_oTablePointFrontRight)
{
	p_oTablePointFrontRight = m_oTablePointFrontRight;
}

void ATableActor::SetVertices(const TArray<FVector>& p_arrUpdatedVertices)
{
	if (!p_arrUpdatedVertices.IsEmpty())
		m_vfVertices = p_arrUpdatedVertices;
}

void ATableActor::GetUpdatedVertices(TArray<FVector>& p_arrUpdatedVertices)
{
	if(!m_vfUpdatedVertices.IsEmpty())
		p_arrUpdatedVertices = m_vfUpdatedVertices;
}

void ATableActor::CaptureUpdatedVerticesData(const TArray<FVector>& p_arrUpdatedVertices,TouchPoints eTouchPoint, const FVector2D& p_fvStretch)
{
	switch (eTouchPoint)
	{
	case TouchPoints::eLeftBack:
	{
		m_oTablePointBackLeft = m_oTablePointBackLeft + p_fvStretch;
		m_oTablePointBackRight.X = m_oTablePointBackRight.X + p_fvStretch.X;
		m_oTablePointFrontLeft.Y = m_oTablePointFrontLeft.Y + p_fvStretch.Y;
		break;
	}
	case TouchPoints::eLeftFront:
	{
		m_oTablePointBackLeft.Y = m_oTablePointBackLeft.Y + p_fvStretch.Y;
		m_oTablePointFrontRight.X = m_oTablePointFrontRight.X + p_fvStretch.X;
		m_oTablePointFrontLeft = m_oTablePointFrontLeft + p_fvStretch;
		break;
	}
	case TouchPoints::eRightBack:
	{
		m_oTablePointBackLeft.X = m_oTablePointBackLeft.X + p_fvStretch.X;
		m_oTablePointFrontRight.Y = m_oTablePointFrontRight.Y + p_fvStretch.Y;
		m_oTablePointBackRight = m_oTablePointBackRight + p_fvStretch;
		break;
	}
	case TouchPoints::eRightFront:
	{
		m_oTablePointBackRight.Y = m_oTablePointBackRight.Y + p_fvStretch.Y;
		m_oTablePointFrontLeft.X = m_oTablePointFrontLeft.X + p_fvStretch.X;
		m_oTablePointFrontRight = m_oTablePointFrontRight + p_fvStretch;
		break;
	}
	default:
		break;
	}
	
	SetVertices(p_arrUpdatedVertices);
}