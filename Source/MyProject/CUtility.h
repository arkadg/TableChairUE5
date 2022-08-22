#ifndef CUTITLITY_H
#define CUTILITY_H

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"


class CUtility
{
private:
	static void ConstructTriangleWithIndices(TArray<int32>& p_viTriangles, int32 p_iIndex1, int32 p_iIndex2, int32 p_iIndex3);
public:
	static int Generate3DQuadMesh(const FVector& p_oLowerLeftPoint, double p_dLength, double p_dBreadth, double p_dHeight, TArray<FVector> &p_vfVertices,TArray<int32> &p_viTriangles);
	static bool GetMaterialFromFile(const FString& p_fsMaterialRef, UMaterialInterface** p_ptrMatInterface);
};

#endif //CUTILITY_H