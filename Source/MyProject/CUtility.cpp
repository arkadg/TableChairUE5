#include "CUtility.h"


/// <summary>
/// Generate the 3d quadrilateral mesh points. The points are then added to create the triangles using the respective indices.
/// </summary>
/// <param name="p_oCenterPoint">Center point of the 3d Quad</param>
/// <param name="p_dLength">Length of the quad (in Y Axis)</param>
/// <param name="p_dBreadth">Breadth of the quad (in X Axis)</param>
/// <param name="p_dHeight">Height of the quad(in Z Axis)</param>
/// <param name="p_fvVertices">Array containing the list of vertices for rendering the 3D Quad mesh</param>
/// <param name="p_viTriangles">Array of indices which is used for creating the triangles</param>
/// <returns></returns> Number of vertices to be rendered after adding the new vertices 
/// 
int CUtility::Generate3DQuadMesh(const FVector& p_oCenterPoint, double p_dLength, double p_dBreadth, double p_dHeight, TArray<FVector>& p_vfVertices, TArray<int32> &p_viTriangles)
{

	// By default the center point vector is a Zero Vector.
	// The mesh is created considering (0,0,0) location. But the actual location is set from the client side.
	int currentIndex = p_vfVertices.Num();

	p_vfVertices.Add(FVector(p_oCenterPoint.X - p_dBreadth/2, p_oCenterPoint.Y - p_dLength/2, p_oCenterPoint.Z - p_dHeight/2)); //lower left - 0
	p_vfVertices.Add(FVector(p_oCenterPoint.X - p_dBreadth / 2, p_oCenterPoint.Y - p_dLength / 2, p_oCenterPoint.Z + p_dHeight/2)); //upper left - 1
	p_vfVertices.Add(FVector(p_oCenterPoint.X - p_dBreadth / 2, p_oCenterPoint.Y + p_dLength / 2, p_oCenterPoint.Z - p_dHeight / 2)); //lower right - 2 
	p_vfVertices.Add(FVector(p_oCenterPoint.X - p_dBreadth / 2, p_oCenterPoint.Y + p_dLength / 2, p_oCenterPoint.Z + p_dHeight / 2)); //upper right - 3

	p_vfVertices.Add(FVector(p_oCenterPoint.X + p_dBreadth/2, p_oCenterPoint.Y - p_dLength / 2, p_oCenterPoint.Z - p_dHeight / 2)); //lower front left - 4
	p_vfVertices.Add(FVector(p_oCenterPoint.X + p_dBreadth/2, p_oCenterPoint.Y - p_dLength / 2, p_oCenterPoint.Z + p_dHeight / 2)); //upper front left - 5

	p_vfVertices.Add(FVector(p_oCenterPoint.X + p_dBreadth / 2, p_oCenterPoint.Y + p_dLength / 2, p_oCenterPoint.Z + p_dHeight / 2)); //upper front right - 6
	p_vfVertices.Add(FVector(p_oCenterPoint.X + p_dBreadth / 2, p_oCenterPoint.Y + p_dLength / 2, p_oCenterPoint.Z - p_dHeight / 2)); //lower front right - 7

	ConstructTriangleWithIndices(p_viTriangles,currentIndex, currentIndex + 2, currentIndex + 3);
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 3, currentIndex + 1, currentIndex);

	//Left face of cube
	ConstructTriangleWithIndices(p_viTriangles,currentIndex, currentIndex + 1, currentIndex + 4);
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 4, currentIndex + 1, currentIndex + 5);

	//Front face of cube
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 4, currentIndex + 5, currentIndex + 7);
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 7, currentIndex + 5, currentIndex + 6);

	//Right face of cube
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 7, currentIndex + 6, currentIndex + 3);
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 3, currentIndex + 2, currentIndex + 7);

	//Top face
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 1, currentIndex + 3, currentIndex + 5);
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 6, currentIndex + 5, currentIndex + 3);

	//bottom face
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 2, currentIndex, currentIndex + 4);
	ConstructTriangleWithIndices(p_viTriangles,currentIndex + 4, currentIndex + 7, currentIndex + 2);

	return p_vfVertices.Num();
}

/// <summary>
/// Add the indices in proper order to construct the triangles for the rendering
/// </summary>
/// <param name="p_viTriangles">Array containing the indices for rendering the triangles</param>
/// <param name="p_iVertex1">Index 1</param>
/// <param name="p_iVertex2">Index 2</param>
/// <param name="p_iVertex3">Index 3</param>
/// 
void CUtility::ConstructTriangleWithIndices(TArray<int32>& p_viTriangles,int32 p_iIndex1, int32 p_iIndex2, int32 p_iIndex3)
{
	p_viTriangles.Add(p_iIndex1);
	p_viTriangles.Add(p_iIndex2);
	p_viTriangles.Add(p_iIndex3);
}

/// <summary>
///  Gets the material from file
/// </summary>
/// <param name="p_fsMaterialRef">Material Reference Path</param>
/// <param name="p_ptrMatInterface">Material Interface Reference to pointer</param>
/// <returns></returns>
bool CUtility::GetMaterialFromFile(const FString& p_fsMaterialRef, UMaterialInterface** p_ptrMatInterface)
{
	if (p_fsMaterialRef.IsEmpty())
		return false;

	*p_ptrMatInterface = nullptr;

	const TCHAR* mFile = *p_fsMaterialRef;
	*p_ptrMatInterface = LoadObject<UMaterialInterface>(NULL, mFile);

	if (*p_ptrMatInterface == nullptr)
		return false;

	return true;
}


