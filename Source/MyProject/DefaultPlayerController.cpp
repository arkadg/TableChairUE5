// Fill out your copyright notice in the Description page of Project Settings.


#include "DefaultPlayerController.h"
#include <Kismet/GameplayStatics.h>

#define WIDTH_OF_CHAIR 150
#define INITIAL_WIDTH_OF_TABLE 400
#define HEIGHT_OF_SEAT_CHAIR 100
#define ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB 20.0



ADefaultPlayerController::ADefaultPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	bShowMouseCursor = true;
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;

	m_eTouchPoint = TouchPoints::eTouchNone;
}


// Called when the game starts or when spawned
void ADefaultPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (CreateTable(FVector(1500,-1200,215),FRotator::ZeroRotator))
	{
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green,TEXT("Create Table: SUCCESS"));
		if (!CalculateAndPlaceChairs())
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Create Chairs: FAILURE"));
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Create Table: FAILURE"));
	}
}

// Called every frame
void ADefaultPlayerController::Tick(float deltatime)
{
	Super::Tick(deltatime);

	if (m_bIsPressed)
	{
		if (!m_actTable)
			return;

		FVector2D oTablePointBackLeft;
		m_actTable->GetTableBackLeft(oTablePointBackLeft);
		FVector2D oTablePointFrontLeft;
		m_actTable->GetTableFrontLeft(oTablePointFrontLeft);
		FVector2D oTablePointBackRight;
		m_actTable->GetTableBackRight(oTablePointBackRight);
		FVector2D oTablePointFrontRight;
		m_actTable->GetTableFrontRight(oTablePointFrontRight);

		FHitResult HitResult;
		GetWorld()->GetFirstPlayerController()->GetHitResultUnderCursorForObjects({ UEngineTypes::ConvertToObjectType(ECC_PhysicsBody) }, false, HitResult);
		if (HitResult.GetActor())
		{
			// If the click is on the table actor, then check the impact point
			//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Impact: %s"), *HitResult.ImpactPoint.ToString()));

			// Check whether the click is near any corner
			FVector diffLeftBack = HitResult.ImpactPoint - FVector(oTablePointBackLeft.X, oTablePointBackLeft.Y, 230);
			FVector diffLeftFront = HitResult.ImpactPoint - FVector(oTablePointFrontLeft.X, oTablePointFrontLeft.Y, 230);
			FVector diffRightBack = HitResult.ImpactPoint - FVector(oTablePointBackRight.X, oTablePointBackRight.Y, 230);
			FVector diffRightFront = HitResult.ImpactPoint - FVector(oTablePointFrontRight.X, oTablePointFrontRight.Y, 230);

			if (!m_bStartDrag)
			{
				if (FMath::Abs(diffLeftBack.X) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB && FMath::Abs(diffLeftBack.Y) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB)
				{
					CurrentMouseCursor = EMouseCursor::GrabHandClosed;
					m_eTouchPoint = TouchPoints::eLeftBack;
					m_bStartDrag = true;
					DestroyChairs();
				}
				else if (FMath::Abs(diffLeftFront.X) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB && FMath::Abs(diffLeftFront.Y) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB)
				{
					CurrentMouseCursor = EMouseCursor::GrabHandClosed;
					m_eTouchPoint = TouchPoints::eLeftFront;
					m_bStartDrag = true;
					DestroyChairs();
				}
				else if (FMath::Abs(diffRightBack.X) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB && FMath::Abs(diffRightBack.Y) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB)
				{
					CurrentMouseCursor = EMouseCursor::GrabHandClosed;
					m_eTouchPoint = TouchPoints::eRightBack;
					m_bStartDrag = true;
					DestroyChairs();
				}
				else if (FMath::Abs(diffRightFront.X) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB && FMath::Abs(diffRightFront.Y) <= ACCEPTABLE_WIDTH_FOR_MOUSE_GRAB)
				{
					CurrentMouseCursor = EMouseCursor::GrabHandClosed;
					m_eTouchPoint = TouchPoints::eRightFront;
					m_bStartDrag = true;
					DestroyChairs();
				}
				else if(m_bIsMouseOver)
					CurrentMouseCursor = EMouseCursor::SlashedCircle;
			}
		}
		if (m_bStartDrag)
		{
			// Get Mouse location and direction
			FVector CursorLocation;
			FVector CursorDirection;
			GetWorld()->GetFirstPlayerController()->DeprojectMousePositionToWorld(CursorLocation, CursorDirection);

			FVector end = CursorLocation + CursorDirection * 10000.0f;
			FPlane plane{ FVector(oTablePointBackLeft.X + m_actTable->GetBreadth()/2,oTablePointBackLeft.Y + m_actTable->GetLength()/2,230),{0.0,0.0,1.0}};
			if (FVector::DotProduct({ 0.0f, 0.0f, 0.1f }, CursorDirection) != 0)
			{
				// Use line and plane intersection to see where the mouse is moved considering the same table surface plane.
				const FVector Intersection = FMath::LinePlaneIntersection(CursorLocation, end, plane);
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Intersection: %s"), *Intersection.ToString()));
				FVector fvNearestCornerPoint;
				switch (m_eTouchPoint)
				{
				case TouchPoints::eLeftBack:
					fvNearestCornerPoint = FVector(oTablePointBackLeft.X, oTablePointBackLeft.Y, 230);
					break;
				case TouchPoints::eLeftFront:
					fvNearestCornerPoint = FVector(oTablePointFrontLeft.X, oTablePointFrontLeft.Y, 230);
					break;
				case TouchPoints::eRightBack:
					fvNearestCornerPoint = FVector(oTablePointBackRight.X, oTablePointBackRight.Y, 230);
					break;
				case TouchPoints::eRightFront:
					fvNearestCornerPoint = FVector(oTablePointFrontRight.X, oTablePointFrontRight.Y, 230);
					break;
				default:
					fvNearestCornerPoint = Intersection;
				}
				FVector stretch3D = Intersection - fvNearestCornerPoint;
				FVector2D stretch = FVector2D(stretch3D.X, stretch3D.Y);
				// Mouse moved by stretch.X and stretch.Y 
				//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Stretch: %s"), *stretch.ToString()));

				if (stretch != FVector2D::ZeroVector)
				{
					m_actTable->UpdateMesh(m_eTouchPoint, stretch);
					m_fvStretch = stretch;
				}
				else
				{
					//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Strecth is Zero Vector"));
				}
			}
		}
	}
}

// Setup the input component
void ADefaultPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (InputComponent)
	{
		InputComponent->BindAction("Press", IE_Pressed, this, &ThisClass::MousePress);
		InputComponent->BindAction("Press", IE_Released, this, &ThisClass::MouseRelease);
	}
}

/// <summary>
/// Create the table as per passed location and rotator
/// </summary>
/// <param name="p_fvLoc">Location of table</param>
/// <param name="p_frRot">Rotator of table</param>
/// <returns>true if success, else failure</returns>
bool ADefaultPlayerController::CreateTable(const FVector& p_fvLoc, const FRotator& p_frRot) 
{
	bool bIsSuccess = true;

	FActorSpawnParameters SpawnInfo;
	m_actTable = GetWorld()->SpawnActor<ATableActor>(p_fvLoc, p_frRot, SpawnInfo);
	if (nullptr != m_actTable)
	{
		m_actTable->GetProceduralMeshComponent()->SetWorldLocation(p_fvLoc);
		m_actTable->GetProceduralMeshComponent()->SetWorldRotation(p_frRot);
		m_actTable->EnableInput(this);
		m_actTable->SetActorEnableCollision(true);
		m_actTable->GetProceduralMeshComponent()->SetCollisionObjectType(ECC_PhysicsBody);
		m_actTable->OnBeginCursorOver.AddDynamic(this, &ThisClass::HandleCursorOverlapBegin);
		m_actTable->OnEndCursorOver.AddDynamic(this, &ThisClass::HandleCursorOverlapEnd);
	}
	else
		return false;

	double dLength = m_actTable->GetLength();
	double dBreadth = m_actTable->GetBreadth();

	m_actTable->SetTableBackLeft(FVector2D(p_fvLoc.X - dBreadth / 2, p_fvLoc.Y - dLength / 2));
	m_actTable->SetTableBackRight(FVector2D(p_fvLoc.X - dBreadth / 2, p_fvLoc.Y + dLength / 2));
	m_actTable->SetTableFrontLeft(FVector2D(p_fvLoc.X + dBreadth / 2, p_fvLoc.Y - dLength / 2));
	m_actTable->SetTableFrontRight(FVector2D(p_fvLoc.X + dBreadth / 2, p_fvLoc.Y + dLength / 2));

	return bIsSuccess;

}

/// <summary>
/// Create the chair as per passed location and rotator
/// </summary>
/// <param name="p_fvLoc">Location of table</param>
/// <param name="p_frRot">Rotator of table</param>
/// <returns>true if success, else failure</returns>
bool ADefaultPlayerController::CreateChair(const FVector& p_fvLoc, const FRotator& p_frRot)
{
	bool bIsSuccess = true;

	FActorSpawnParameters SpawnInfo;
	AChairActor* actChair = GetWorld()->SpawnActor<AChairActor>(p_fvLoc, p_frRot, SpawnInfo);
	if (nullptr != actChair)
	{
		actChair->GetProceduralMeshComponent()->SetWorldLocation(p_fvLoc);
		actChair->GetProceduralMeshComponent()->SetWorldRotation(p_frRot);
		actChair->SetActorEnableCollision(false);
	}
	else
		return false;

	m_arrChairs.Add(actChair);

	return bIsSuccess;
}

/// <summary>
/// Create the initial chair chair setup 
/// </summary>
/// Note: Currently this is not used!!!
/// <returns>true if success, else failure</returns>
bool ADefaultPlayerController::CreateInitialChairs()
{
	bool bIsSuccess = true;

	FVector oLocChair1 = FVector(1300, -1200, 110);
	FRotator oRotChair1 = FRotator::ZeroRotator;
	FVector oLocChair2 = FVector(1500, -1000, 110);
	FRotator oRotChair2 = FRotator(0, -90, 0);
	FVector oLocChair3 = FVector(1500, -1400, 110);
	FRotator oRotChair3 = FRotator(0,90,0);
	FVector oLocChair4 = FVector(1700, -1200, 110);
	FRotator oRotChair4 = FRotator(0,180, 0);

	m_arrChairs.Empty();

	if (!CreateChair(oLocChair1, oRotChair1))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocChair1.ToString()));
		return false;
	}
	if (!CreateChair(oLocChair2, oRotChair2))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocChair2.ToString()));
		return false;
	}
	if (!CreateChair(oLocChair3, oRotChair3))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocChair3.ToString()));
		return false;
	}
	if (!CreateChair(oLocChair4, oRotChair4))
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocChair4.ToString()));
		return false;
	}

	return bIsSuccess;
}

/// <summary>
/// Destroy the existing chairs
/// </summary>
/// <returns>true if success, else failure</returns>
bool ADefaultPlayerController::DestroyChairs()
{
	bool bIsDestroyed = true;
	for (auto chair : m_arrChairs)
	{
		bIsDestroyed = chair->Destroy();
	}

	m_arrChairs.Empty();

	return bIsDestroyed;
}

/// <summary>
/// Calculate and place chairs as per the current length and breadth of the table
/// </summary>
/// <returns>true if success, else failure</returns>
bool ADefaultPlayerController::CalculateAndPlaceChairs()
{
	if (nullptr == m_actTable)
		return false;

	FVector2D oTablePointBackLeft;
	m_actTable->GetTableBackLeft(oTablePointBackLeft);
	FVector2D oTablePointFrontLeft;
	m_actTable->GetTableFrontLeft(oTablePointFrontLeft);
	FVector2D oTablePointBackRight;
	m_actTable->GetTableBackRight(oTablePointBackRight);
	FVector2D oTablePointFrontRight;
	m_actTable->GetTableFrontRight(oTablePointFrontRight);

	double dDiffXAxis = oTablePointBackLeft.X - oTablePointFrontLeft.X; // breadth of the updated rectangle
	double dDiffYAxis = oTablePointBackLeft.Y - oTablePointBackRight.Y; // length of the updated rectangle

	if (FMath::Abs(dDiffXAxis) < INITIAL_WIDTH_OF_TABLE || FMath::Abs(dDiffYAxis) < INITIAL_WIDTH_OF_TABLE)
		return false;

	// choosing the gap between chairs to be atleast 100, width of chair is 150
	// so the quation becomes 150*x + (x+1)*100 = VALUE
	double uiNumOfChairsXAxis = static_cast<unsigned int>((FMath::Abs(dDiffXAxis) - 100.0)) / 250;
	double uiGapValueXAxis = 100 + (static_cast<unsigned int>((FMath::Abs(dDiffXAxis) - 100.0)) % 250)/(uiNumOfChairsXAxis + 1);

	double uiNumOfChairsYAxis = static_cast<unsigned int>((FMath::Abs(dDiffYAxis) - 100.0)) / 250;
	double uiGapValueYAxis = 100 + (static_cast<unsigned int>((FMath::Abs(dDiffYAxis) - 100.0)) % 250) / (uiNumOfChairsYAxis + 1);

	for (unsigned int i = 0; i < uiNumOfChairsXAxis; i++)
	{
		FVector oLocLeft = FVector(oTablePointBackLeft.X + uiGapValueXAxis * (i + 1) + WIDTH_OF_CHAIR * i + WIDTH_OF_CHAIR / 2, oTablePointBackLeft.Y,HEIGHT_OF_SEAT_CHAIR);
		FRotator oRotLeft = FRotator(0, 90, 0);
		FVector oLocRight = FVector(oTablePointBackRight.X + uiGapValueXAxis * (i + 1) + WIDTH_OF_CHAIR * i + WIDTH_OF_CHAIR / 2, oTablePointBackRight.Y, HEIGHT_OF_SEAT_CHAIR);
		FRotator oRotRight = FRotator(0, -90, 0);
		 
		if(!CreateChair(oLocLeft, oRotLeft))
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocLeft.ToString()));

		if(!CreateChair(oLocRight, oRotRight))
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocRight.ToString()));

	}

	for (unsigned int i = 0; i < uiNumOfChairsYAxis; i++)
	{
		FVector oLocBack = FVector(oTablePointBackLeft.X, oTablePointBackLeft.Y + uiGapValueYAxis * (i + 1) + WIDTH_OF_CHAIR * i + WIDTH_OF_CHAIR / 2, HEIGHT_OF_SEAT_CHAIR);
		FRotator oRotBack = FRotator(0, 0, 0);
		FVector oLocFront = FVector(oTablePointFrontLeft.X , oTablePointFrontLeft.Y + uiGapValueYAxis * (i + 1) + WIDTH_OF_CHAIR * i + WIDTH_OF_CHAIR / 2, HEIGHT_OF_SEAT_CHAIR);
		FRotator oRotFront = FRotator(0, 180, 0);

		if(!CreateChair(oLocBack, oRotBack))
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocBack.ToString()));

		if(!CreateChair(oLocFront, oRotFront))
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Error in creating chair at %s"), *oLocFront.ToString()));

	}

	return true;
}

/// <summary>
/// Mouse Press Callback
/// </summary>
void ADefaultPlayerController::MousePress()
{
	m_eTouchPoint = TouchPoints::eTouchNone;
	m_bIsPressed = true;
}

/// <summary>
/// Mouse Release Callback
/// </summary>
void ADefaultPlayerController::MouseRelease()
{
	m_bIsPressed = false;
	if (m_bStartDrag)
	{
		if (nullptr != m_actTable)
		{
			TArray<FVector> updatedVertices;
			m_actTable->GetUpdatedVertices(updatedVertices);

			if(m_eTouchPoint != TouchPoints::eTouchNone)
				m_actTable->CaptureUpdatedVerticesData(updatedVertices,m_eTouchPoint,m_fvStretch);

			CalculateAndPlaceChairs();
		}

		m_bStartDrag = false;
	}
	m_eTouchPoint = TouchPoints::eTouchNone;
	if (m_bIsMouseOver)
		CurrentMouseCursor = EMouseCursor::GrabHand;
	else
		CurrentMouseCursor = EMouseCursor::Default;
}

/// <summary>
/// Function which gets called when the cursor overlap on the actor begins(i.e. table in this case)
/// </summary>
/// <param name="touchedComp">Touched component</param>
void ADefaultPlayerController::HandleCursorOverlapBegin(AActor* touchedComp)
{
	m_bIsMouseOver = true;
	if(m_bStartDrag)
		CurrentMouseCursor = EMouseCursor::GrabHandClosed;
	else
		CurrentMouseCursor = EMouseCursor::GrabHand;
}


/// <summary>
/// Function which gets called when the cursor overlap on the actor ends(i.e. table in this case)
/// </summary>
/// <param name="touchedComp">Touched component</param>
void ADefaultPlayerController::HandleCursorOverlapEnd(AActor* touchedComp)
{
	m_bIsMouseOver = false;

	if (m_bStartDrag == false)
		CurrentMouseCursor = EMouseCursor::Default;
}