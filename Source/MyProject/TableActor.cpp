// Fill out your copyright notice in the Description page of Project Settings.


#include "TableActor.h"
#include "CUtility.h"
#include "DefaultPlayerController.h"

#define INITIAL_LENGTH_OF_TABLE 400
#define INITIAL_BREADTH_OF_TABLE 400
#define TABLE_SURFACE_WIDTH 30
#define TABLE_SUPPORT_WIDTH 50
#define TABLE_SUPPORT_HEIGHT 200

#define VERTEX_INDEX_LEFT_BACK_START 8
#define VERTEX_INDEX_LEFT_BACK_END 15
#define VERTEX_INDEX_RIGHT_BACK_START 16
#define VERTEX_INDEX_RIGHT_BACK_END 23
#define VERTEX_INDEX_LEFT_FRONT_START 24
#define VERTEX_INDEX_LEFT_FRONT_END 31
#define VERTEX_INDEX_RIGHT_FRONT_START 32
#define VERTEX_INDEX_RIGHT_FRONT_END 39

// Sets default values
ATableActor::ATableActor():m_dCurrentLength(400.0), m_dCurrentBreadth(400.0)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CustomMesh = CreateDefaultSubobject<UProceduralMeshComponent>("TableMesh");
	SetRootComponent(CustomMesh);

	CustomMesh->bUseAsyncCooking = true;

}


/// <summary>
/// Creates the table mesh including the surface and the legs
/// </summary>
/// <param name="p_dLengthTableSurface">Length of the Table Surface (Y Axis)</param>
/// <param name="p_dBreadthTableSurface">Breadth of the Table Surface (Y Axis)</param>
/// <param name="p_dHeightTableSurface">Height of the Table Surface</param>
/// <param name="p_dWidthTableSupport">Width of the Table Support</param>
/// <param name="p_dHeightTableSupport">Height of the Table Support</param>
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

	//Creates the table
	CreateTableMesh(INITIAL_LENGTH_OF_TABLE, INITIAL_BREADTH_OF_TABLE, TABLE_SURFACE_WIDTH, TABLE_SUPPORT_WIDTH, TABLE_SUPPORT_HEIGHT);
}

// Called every frame
void ATableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

/// <summary>
/// Sets the table surface length
/// </summary>
/// <param name="p_dLength"></param>
void ATableActor::SetLengthTableSurface(const double p_dLength)
{
	m_dCurrentLength = p_dLength;
}

/// <summary>
/// Sets the table surface breadth
/// </summary>
/// <param name="p_dBreadth"></param>
void ATableActor::SetBreadthTableSurface(const double p_dBreadth)
{
	m_dCurrentBreadth = p_dBreadth;
}

/// <summary>
/// Get the length of the table surface
/// </summary>
/// <returns>Length of table surface</returns>
double ATableActor::GetLength() const
{
	return m_dCurrentLength;
}

/// <summary>
/// Get the breadth of the table surface
/// </summary>
/// <returns>Length of table breadth</returns>
double ATableActor::GetBreadth() const
{
	return m_dCurrentBreadth;
}

/// <summary>
/// Procedural Mesh Component of the Table Actor
/// </summary>
/// <returns>Gets Procedural Mesh Component pointer</returns>
UProceduralMeshComponent* ATableActor::GetProceduralMeshComponent() const
{
	return CustomMesh;
}

/// <summary>
/// Update mesh as per the new mouse position
/// </summary>
/// <param name="p_eTouchPoint">Grab handle touch point</param>
/// <param name="p_fvStretch">Mouse pointer moved from the reference point</param>
void ATableActor::UpdateMesh(TouchPoints p_eTouchPoint,FVector2D& p_fvStretch)
{
	bool bChangesDone = false;
	
	TArray<FVector> updatedVertices = m_vfVertices;
	m_vfUpdatedVertices = m_vfVertices;
	FVector diff = FVector::ZeroVector;
	if (p_eTouchPoint == TouchPoints::eLeftBack)
	{
		// the stretch should be such so that it is greater than the initial table area
		if (m_oTablePointBackLeft.X + p_fvStretch.X > m_oInitLeftBack.X)
		{
			p_fvStretch.X = m_oInitLeftBack.X - m_oTablePointBackLeft.X;
		}
		if (m_oTablePointBackLeft.Y + p_fvStretch.Y > m_oInitLeftBack.Y)
		{
			p_fvStretch.Y = m_oTablePointBackLeft.Y - m_oInitLeftBack.Y;
		}

		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);

		// Update the back lower left and upper left points
		updatedVertices[0] = updatedVertices[0] + diff;
		updatedVertices[1] = updatedVertices[1] + diff;

		//update X axis of back lower right and upper right
		updatedVertices[2].X = updatedVertices[2].X + p_fvStretch.X;
		updatedVertices[3].X = updatedVertices[3].X + p_fvStretch.X;

		//update Y axis of front lower left and upper left
		updatedVertices[4].Y = updatedVertices[4].Y + p_fvStretch.Y;
		updatedVertices[5].Y = updatedVertices[5].Y + p_fvStretch.Y;
			
		// Update points for left back support
		for (int i = VERTEX_INDEX_LEFT_BACK_START; i <= VERTEX_INDEX_LEFT_BACK_END; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		// Update points for right back support
		for (int i = VERTEX_INDEX_RIGHT_BACK_START; i <= VERTEX_INDEX_RIGHT_BACK_END; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		// Update points for left front support
		for (int i = VERTEX_INDEX_LEFT_FRONT_START; i <= VERTEX_INDEX_LEFT_FRONT_END; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		bChangesDone = true;

	}
	else if (p_eTouchPoint == TouchPoints::eLeftFront)
	{
		// the stretch should be such so that it is greater than the initial table area
		if (m_oTablePointFrontLeft.X + p_fvStretch.X < m_oInitLeftFront.X)
		{
			p_fvStretch.X = m_oInitLeftFront.X - m_oInitLeftFront.X;
		}
		if (m_oTablePointFrontLeft.Y + p_fvStretch.Y > m_oInitLeftFront.Y)
		{
			p_fvStretch.Y = m_oTablePointFrontLeft.Y - m_oInitLeftFront.Y;
		}
		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);

		//update front lower left and upper left points
		updatedVertices[4] = updatedVertices[4] + diff;
		updatedVertices[5] = updatedVertices[5] + diff;

		// Update Y Axis of back lower and upper left points
		updatedVertices[0].Y = updatedVertices[0].Y + p_fvStretch.Y;
		updatedVertices[1].Y = updatedVertices[1].Y + p_fvStretch.Y;

		// Update X Axis of front lower and upper right points
		updatedVertices[6].X = updatedVertices[6].X + p_fvStretch.X;
		updatedVertices[7].X = updatedVertices[7].X + p_fvStretch.X;

		// Update points for left back support
		for (int i = VERTEX_INDEX_LEFT_BACK_START; i <= VERTEX_INDEX_LEFT_BACK_END; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		// Update points for left front support
		for (int i = VERTEX_INDEX_LEFT_FRONT_START; i <= VERTEX_INDEX_LEFT_FRONT_END; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		// Update points for right front support
		for (int i = VERTEX_INDEX_RIGHT_FRONT_START; i <= VERTEX_INDEX_RIGHT_FRONT_END; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		bChangesDone = true;
	}
	else if (p_eTouchPoint == TouchPoints::eRightBack)
	{
		// the stretch should be such so that it is greater than the initial table area
		if (m_oTablePointBackRight.X + p_fvStretch.X > m_oInitRightBack.X)
		{
			p_fvStretch.X = m_oInitRightBack.X - m_oTablePointBackRight.X;
		}
		if (m_oTablePointBackRight.Y + p_fvStretch.Y < m_oInitRightBack.Y)
		{
			p_fvStretch.Y = m_oInitRightBack.Y - m_oTablePointBackRight.Y;
		}

		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);

		//update back lower and upper right points
		updatedVertices[2] = updatedVertices[2] + diff;
		updatedVertices[3] = updatedVertices[3] + diff;

		// update X axis of back lower and upper left points
		updatedVertices[0].X = updatedVertices[0].X + p_fvStretch.X;
		updatedVertices[1].X = updatedVertices[1].X + p_fvStretch.X;

		// update Y axis of front lower and upper right points
		updatedVertices[6].Y = updatedVertices[6].Y + p_fvStretch.Y;
		updatedVertices[7].Y = updatedVertices[7].Y + p_fvStretch.Y;

		// Update points for left back support
		for (int i = VERTEX_INDEX_LEFT_BACK_START; i <= VERTEX_INDEX_LEFT_BACK_END; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		// Update points for right back support
		for (int i = VERTEX_INDEX_RIGHT_BACK_START; i <= VERTEX_INDEX_RIGHT_BACK_END; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		// Update points for front right support
		for (int i = VERTEX_INDEX_RIGHT_FRONT_START; i <= VERTEX_INDEX_RIGHT_FRONT_END; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		bChangesDone = true;
	}
	else if (p_eTouchPoint == TouchPoints::eRightFront)
	{
	// the stretch should be such so that it is greater than the initial table area
		if (m_oTablePointFrontRight.X + p_fvStretch.X < m_oInitRightFront.X)
		{
			p_fvStretch.X = m_oTablePointFrontRight.X - m_oInitRightFront.X;
		}

		if (m_oTablePointFrontRight.Y + p_fvStretch.Y < m_oInitRightFront.Y)
		{
			p_fvStretch.Y = m_oInitRightFront.Y - m_oTablePointFrontRight.Y;
		}

		diff = FVector(p_fvStretch.X, p_fvStretch.Y, 0);

		// Update Y Axis for right back lower and upper points
		updatedVertices[2].Y = updatedVertices[2].Y + p_fvStretch.Y;
		updatedVertices[3].Y = updatedVertices[3].Y + p_fvStretch.Y;

		// Update X Axis for left front lower and upper points
		updatedVertices[4].X = updatedVertices[4].X + p_fvStretch.X;
		updatedVertices[5].X = updatedVertices[5].X + p_fvStretch.X;

		// Update points for front right support points
		updatedVertices[6] = updatedVertices[6] + diff;
		updatedVertices[7] = updatedVertices[7] + diff;

		for (int i = VERTEX_INDEX_RIGHT_BACK_START; i <= VERTEX_INDEX_RIGHT_BACK_END; i++)
		{
			updatedVertices[i].Y = updatedVertices[i].Y + p_fvStretch.Y;
		}

		for (int i = VERTEX_INDEX_LEFT_FRONT_START; i <= VERTEX_INDEX_LEFT_FRONT_END; i++)
		{
			updatedVertices[i].X = updatedVertices[i].X + p_fvStretch.X;
		}

		for (int i = VERTEX_INDEX_RIGHT_FRONT_START; i <= VERTEX_INDEX_RIGHT_FRONT_END; i++)
		{
			updatedVertices[i] = updatedVertices[i] + diff;
		}

		bChangesDone = true;
	}

	if (bChangesDone)
	{
		// Update the mesh as per the updated vertices
		m_vfUpdatedVertices.Empty();
		CustomMesh->UpdateMeshSection_LinearColor(0, updatedVertices, TArray<FVector>(), TArray<FVector2D>(), TArray<FLinearColor>(), TArray<FProcMeshTangent>());
		m_vfUpdatedVertices = updatedVertices;
	}
}

/// <summary>
/// Sets the Back Left Point of Table Surface
/// </summary>
/// <param name="p_oTablePointBackLeft">Back Left Point of Table Surface</param>
void ATableActor::SetTableBackLeft(const FVector2D& p_oTablePointBackLeft)
{
	m_oTablePointBackLeft = p_oTablePointBackLeft;
}

/// <summary>
/// Sets the right back point of table surface
/// </summary>
/// <param name="p_oTablePointBackRight">Right back point of table surface</param>
void ATableActor::SetTableBackRight(const FVector2D& p_oTablePointBackRight)
{
	m_oTablePointBackRight = p_oTablePointBackRight;
}

/// <summary>
/// Sets the front left point of table surface
/// </summary>
/// <param name="p_oTablePointFrontLeft">Front left point of table surface</param>
void ATableActor::SetTableFrontLeft(const FVector2D& p_oTablePointFrontLeft)
{
	m_oTablePointFrontLeft = p_oTablePointFrontLeft;
}

/// <summary>
/// Sets the front right point of table surface
/// </summary>
/// <param name="p_oTablePointFrontRight">Front right of table surface</param>
void ATableActor::SetTableFrontRight(const FVector2D& p_oTablePointFrontRight)
{
	m_oTablePointFrontRight = p_oTablePointFrontRight;
}


/// <summary>
/// Gets the Back Left Point of Table Surface
/// </summary>
/// <param name="p_oTablePointBackLeft">Back Left Point of Table Surface</param>
void ATableActor::GetTableBackLeft(FVector2D& p_oTablePointBackLeft)
{
	p_oTablePointBackLeft = m_oTablePointBackLeft;
}


/// <summary>
/// Gets the right back point of table surface
/// </summary>
/// <param name="p_oTablePointBackRight">Right back point of table surface</param>
void ATableActor::GetTableBackRight(FVector2D& p_oTablePointBackRight)
{
	p_oTablePointBackRight = m_oTablePointBackRight;
}

/// <summary>
/// Gets the front left point of table surface
/// </summary>
/// <param name="p_oTablePointFrontLeft">Front left point of table surface</param>
void ATableActor::GetTableFrontLeft(FVector2D& p_oTablePointFrontLeft)
{
	p_oTablePointFrontLeft = m_oTablePointFrontLeft;
}

/// <summary>
/// Gets the front right point of table surface
/// </summary>
/// <param name="p_oTablePointFrontRight">Front right of table surface</param>
void ATableActor::GetTableFrontRight(FVector2D& p_oTablePointFrontRight)
{
	p_oTablePointFrontRight = m_oTablePointFrontRight;
}

/// <summary>
/// Sets the vertices after mesh update
/// </summary>
/// <param name="p_arrUpdatedVertices">Updated vertices</param>
void ATableActor::SetVertices(const TArray<FVector>& p_arrUpdatedVertices)
{
	if (!p_arrUpdatedVertices.IsEmpty())
		m_vfVertices = p_arrUpdatedVertices;
}

/// <summary>
/// Gets the updated vertices after mesh update
/// </summary>
/// <param name="p_arrUpdatedVertices">Updated vertices</param>
void ATableActor::GetUpdatedVertices(TArray<FVector>& p_arrUpdatedVertices)
{
	if(!m_vfUpdatedVertices.IsEmpty())
		p_arrUpdatedVertices = m_vfUpdatedVertices;
}

/// <summary>
/// Capture and update the vertex data after the mesh is updated 
/// </summary>
/// <param name="p_arrUpdatedVertices">updated vertices</param>
/// <param name="eTouchPoint">Grab handle touch point</param>
/// <param name="p_fvStretch">difference of mouse pointer location from reference point</param>
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