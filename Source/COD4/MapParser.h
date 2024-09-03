#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "Algo/StableSort.h"
#include "MapParser.generated.h"

class FFace
{
public:
    FFace(const FVector InNormal)
    {
        Normal = InNormal;
    }

    void SortVerticesClockwise()
    {
        const FVector Center = GetCenter() + KINDA_SMALL_NUMBER;

        // Do not use Vertices.Sort()!
        Algo::StableSort(Vertices, [&](const FVector& V1, const FVector& V2)
        {
            if (V1.Equals(V2))
            {
                return false;
            }

            const FVector E0 = V1 - Center;
            const FVector E1 = V2 - Center;
            const FVector CP = FVector::CrossProduct(E0, E1);
            
            return FVector::DotProduct(Normal, CP) < 0;
        });
    }

    void Triangulate(TArray<FVector>& OutVertices, TArray<int32>& OutIndices, TArray<FColor>& OutColors, const FColor& Color, const bool bDuplicateVertices = true) const
    {
        // Can be simplified to one loop, but that makes it hard to read.
        
        TArray<int32> FaceIndices;
        for (const auto& Vertex : Vertices)
        {
            // Note: Set bDuplicateVertices to true in case we want different UV/color/other attributes for each face.
            int32 VertexIndex = bDuplicateVertices ? INDEX_NONE : OutVertices.Find(Vertex);
            if (VertexIndex == INDEX_NONE)
            {
                VertexIndex = OutVertices.Add(Vertex);
                OutColors.Add(Color);
            }
            
            FaceIndices.Add(VertexIndex);
        }
        
        for (int32 i = 0; i < Vertices.Num() - 2; i++)
        {
            int32 Index0 = FaceIndices[0];
            int32 Index1 = FaceIndices[i + 1];
            int32 Index2 = FaceIndices[i + 2];
            
            OutIndices.Add(Index0);
            OutIndices.Add(Index2);
            OutIndices.Add(Index1);
        }
    }

private:
    FVector GetCenter() const
    {
        FVector Center = FVector::ZeroVector;
        for (const FVector& Vertex : Vertices)
        {
            Center += Vertex;
        }
    
        return Center / Vertices.Num();
    }

public:
    TArray<FVector> Vertices;
    
private:
    FVector Normal;
};

// FMyPlane created instead of using FPlane because it does not store plane points on definition to have GetCenter(). Subclassing it is too heavy.
class FMyPlane
{
public:
    FMyPlane(const FVector A, const FVector B, const FVector C)
    {
        // COD4/Quake engine uses inches, convert to centimeters.
        constexpr float SCALE_IN_TO_CM = 2.54f;
        
        P1 = A * SCALE_IN_TO_CM;
        P2 = B * SCALE_IN_TO_CM;
        P3 = C * SCALE_IN_TO_CM;
    }
    
    FVector GetCenter() const
    {
        return (P1 + P2 + P3) / 3.0f;
    }

    // Or subclass FPlane and use its GetNormal().
    FVector GetNormal() const
    {
        const FVector E0 = P2 - P1;
        const FVector E1 = P3 - P1;
        return FVector::CrossProduct(E0, E1).GetSafeNormal();
    }

    // Or subclass FPlane and return W; for distance.
    float GetDistance() const
    {
        const FVector N = GetNormal();
        return FVector::DotProduct(P1, N);
    }
    
    bool IntersectThreePlanes(const FMyPlane& Plane1, const FMyPlane& Plane2, FVector& Vertex) const
    {
        const FVector N1 = GetNormal();
        const FVector N2 = Plane1.GetNormal();
        const FVector N3 = Plane2.GetNormal();
        const float Determinant = FVector::DotProduct(N1, FVector::CrossProduct(N2, N3));

        // Can't intersect parallel planes.
        if (FMath::IsNearlyZero(Determinant, KINDA_SMALL_NUMBER))
        {
            return false;
        }
        
        Vertex = (
            FVector::CrossProduct(N2, N3) * GetDistance() +
            FVector::CrossProduct(N3, N1) * Plane1.GetDistance() +
            FVector::CrossProduct(N1, N2) * Plane2.GetDistance()
        ) / Determinant;

        return true;
    }
    
private:
    FVector P1, P2, P3;
};

class FBrush
{
public:
    FBrush(const FString InName)
    {
        Name = InName;
    }

    void AddPlane(const FVector A, const FVector B, const FVector C)
    {
        Planes.Add(FMyPlane(A, B, C));
    }
    
    void ProcessPlanes()
    {
        if(Planes.Num() < 4)
        {
            UE_LOG(LogTemp, Warning, TEXT("AMapParser: Brush needs to have at least 4 planes!"));
            return;
        }

        // This is here because plane logic from FFace has been separated to FMyPlane class.
        Faces.Reserve(Planes.Num());
        for (const auto& Plane : Planes)
        {
            // Use input plane's normal because it has proper outwards facing direction.
            // Normal also can be calculated using produced vertices, but we need Center of Mass of a brush to properly point normal outwards.
            FFace Face(Plane.GetNormal());
            Faces.Add(Face);
        }
        
        for (int i = 0; i < Planes.Num() - 2; i++)
        {
            for (int j = 0; j < Planes.Num() - 1; j++)
            {
                for (int k = 0; k < Planes.Num(); k++)
                {
                    if (i != j && i != k && j != k)
                    {
                        FVector Vertex;
                        if (Planes[k].IntersectThreePlanes(Planes[i], Planes[j], Vertex) && IsVertexLegal(Vertex))
                        {
                            // AddUnique() is important here! Add() produces degenerate tris.
                            Faces[i].Vertices.AddUnique(Vertex);
                            Faces[j].Vertices.AddUnique(Vertex);
                            Faces[k].Vertices.AddUnique(Vertex);
                        }
                    }
                }
            }
        }
    }
    
private:
    // Check if the vertex belongs to the convex shape.
    bool IsVertexLegal(const FVector& Vertex)
    {
        for (const auto& Plane : Planes)
        {
            const FVector Facing = (Vertex - Plane.GetCenter()).GetSafeNormal();
            if (FVector::DotProduct(Facing, Plane.GetNormal()) < -KINDA_SMALL_NUMBER)
            {
                return false;
            }
        }
        return true;
    }

public:
    FString Name;
    TArray<FMyPlane> Planes;
    TArray<FFace> Faces;
};

class FEntity
{
public:
    FEntity(const FString InName)
    {
        Name = InName;
    }

    FString Name;
    TArray<FBrush> Brushes;
    TArray<FVector> Vertices;
    TArray<int32> Indices;
    // Not essential; used only for distinguishing different brushes or faces by color.
    TArray<FColor> Colors;
};

UCLASS()
class COD4_API AMapParser : public AActor
{
    GENERATED_BODY()
    
public:	
    AMapParser();
    virtual bool ShouldTickIfViewportsOnly() const override;
    
    void LoadEntities();
    void AddEntity(FEntity& Entity);
    
    UPROPERTY(VisibleAnywhere)
    UProceduralMeshComponent* ProceduralMeshComponent;
    TArray<FEntity> Entities;

protected:
    virtual void BeginPlay() override;

public:	
    virtual void Tick(float DeltaTime) override;
};
