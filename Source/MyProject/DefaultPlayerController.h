// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TableActor.h"
#include "ChairActor.h"
#include "DefaultPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ADefaultPlayerController : public APlayerController
{
	GENERATED_BODY()

private:
	bool CreateTable(const FVector& p_fvLoc, const FRotator& p_frRot);
	bool CreateChair(const FVector& p_fvLoc, const FRotator& p_frRot);
	bool CreateInitialChairs();
	bool DestroyChairs();
	bool CalculateAndPlaceChairs();

	FVector2D m_fvStretch;

	TArray<AChairActor*> m_arrChairs;
	ATableActor* m_actTable = nullptr;
	
	bool m_bIsPressed = false;
	bool m_bStartDrag = false;
	bool m_bIsMouseOver = false;

	TouchPoints m_eTouchPoint;

public:
	ADefaultPlayerController();

	virtual void BeginPlay() override;
	virtual void Tick(float deltatime) override;

	void MousePress();
	void MouseRelease();

	UFUNCTION()
	void HandleCursorOverlapBegin(AActor* touchedComp);

	UFUNCTION()
	void HandleCursorOverlapEnd(AActor* touchedComp);

protected:

	virtual void SetupInputComponent() override;

};
