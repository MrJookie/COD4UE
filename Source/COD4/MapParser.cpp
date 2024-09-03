#include "MapParser.h"
#include "DrawDebugHelpers.h"

template <typename TClass>
FColor GetUniqueColor(const TClass& TypeClass)
{
    const uint32 HashValue = GetTypeHash(&TypeClass);
    
    const uint8 R = (HashValue & 0xFF0000) >> 16;
    const uint8 G = (HashValue & 0x00FF00) >> 8;
    const uint8 B = HashValue & 0x0000FF;

    // Make color darker by mul.
    constexpr float Darkness = 0.5f;
    return FColor(R * Darkness, G * Darkness, B * Darkness);
}

AMapParser::AMapParser()
{
    PrimaryActorTick.bCanEverTick = true;
    
    ProceduralMeshComponent = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("Brush"));
    RootComponent = ProceduralMeshComponent;
}

void AMapParser::LoadEntities()
{
    // Just to add once entities in Tick() rather than in BeginPlay()/constructor.
    if(Entities.Num() > 0)
    {
        return;
    }

    UE_LOG(LogTemp, Display, TEXT("AMapParser: Adding entities."));

    /*
    // map_source/prefabs/misc_models/me_concrete_barrier.map
    FEntity Entity_0("me_concrete_barrier");
    {
        //FBrush Brush_0("brush_0_mantle_over");
        //{
        //	Brush_0.AddPlane(FVector(37, 13, 19), FVector(-36, 13, 19), FVector(-36, -16, 19));
        //	Brush_0.AddPlane(FVector(-36, -14, 37), FVector(-36, 15, 37), FVector(37, 15, 37));
        //	Brush_0.AddPlane(FVector(-36, -11, 39), FVector(37, -11, 39), FVector(37, -11, 17));
        //	Brush_0.AddPlane(FVector(42, -14, 39), FVector(42, 15, 39), FVector(42, 15, 17));
        //	Brush_0.AddPlane(FVector(37, 11, 39), FVector(-36, 11, 39), FVector(-36, 11, 17));
        //	Brush_0.AddPlane(FVector(-41, 16, 39), FVector(-41, -13, 39), FVector(-41, -13, 17));
        //}
        //Entity_0.Brushes.Add(Brush_0);
    
        FBrush Brush_1("brush_1_clip_nosight_rock");
        {
            Brush_1.AddPlane(FVector(-36, -6, 0), FVector(37, -6, 0), FVector(37, 6, 0));
            Brush_1.AddPlane(FVector(-36, 5, 31), FVector(37, 5, 31), FVector(37, -7, 31));
            Brush_1.AddPlane(FVector(37, -7, 0), FVector(-36, -7, 0), FVector(-36, -7, 17));
            Brush_1.AddPlane(FVector(37, -11, 46), FVector(37, 11, 46), FVector(37, 11, 7));
            Brush_1.AddPlane(FVector(-36, 7, 0), FVector(37, 7, 0), FVector(37, 7, 17));
            Brush_1.AddPlane(FVector(-36, 11, 44), FVector(-36, -11, 44), FVector(-36, -11, 5));
        }
        Entity_0.Brushes.Add(Brush_1);

        FBrush Brush_2("brush_2_clip_nosight_rock");
        {
            Brush_2.AddPlane(FVector(-36, -6, 31), FVector(37, -6, 31), FVector(37, 6, 31));
            Brush_2.AddPlane(FVector(-36, 4, 35), FVector(37, 4, 35), FVector(37, -4, 35));
            Brush_2.AddPlane(FVector(-36, -4, 35), FVector(37, -4, 35), FVector(37, -7, 31));
            Brush_2.AddPlane(FVector(37, -11, 50), FVector(37, 11, 50), FVector(37, 11, 11));
            Brush_2.AddPlane(FVector(37, 4, 35), FVector(-36, 4, 35), FVector(-36, 7, 31));
            Brush_2.AddPlane(FVector(-36, 11, 48), FVector(-36, -11, 48), FVector(-36, -11, 9));
        }
        Entity_0.Brushes.Add(Brush_2);
    }
    
    AddEntity(Entity_0);

    return;
    */

    // map_source/prefabs/misc_models/vehicle_delivery_truck.map
    FEntity Entity_1("vehicle_delivery_truck");
    {
        FBrush Brush_0("brush_0_clip_nosight_metal");
        {
            Brush_0.AddPlane(FVector(-22.5, 133, 11), FVector(-24.5, 133, 11), FVector(-24.5, 101, 11));
            Brush_0.AddPlane(FVector(-24.5, 101, 19), FVector(-24.5, 133, 19), FVector(-22.5, 133, 19));
            Brush_0.AddPlane(FVector(-24.5, -38, 17), FVector(-22.5, -38, 17), FVector(-22.5, -38, 13));
            Brush_0.AddPlane(FVector(23.5, 101, 21), FVector(23.5, 133, 21), FVector(23.5, 133, 17));
            Brush_0.AddPlane(FVector(-24.5, 133, 19), FVector(-24.5, 101, 19), FVector(-24.5, 101, 15));
            Brush_0.AddPlane(FVector(23.5, -23, 19), FVector(23.5, -23, 12), FVector(55.5, -23, 19));
        }
        Entity_1.Brushes.Add(Brush_0);
        
        FBrush Brush_1("brush_1_clip_nosight_metal");
        {
            Brush_1.AddPlane(FVector(47.5f, 133.0f, 0.0f), FVector(-48.5f, 133.0f, 0.0f), FVector(-48.5f, 101.0f, 0.0f));
            Brush_1.AddPlane(FVector(-48.5f, 101.0f, 19.0f), FVector(-48.5f, 133.0f, 19.0f), FVector(47.5f, 133.0f, 19.0f));
            Brush_1.AddPlane(FVector(-48.5f, 101.0f, 19.0f), FVector(47.5f, 101.0f, 19.0f), FVector(47.5f, 101.0f, -3.0f));
            Brush_1.AddPlane(FVector(39.5f, 101.0f, 19.0f), FVector(39.5f, 133.0f, 19.0f), FVector(39.5f, 133.0f, -3.0f));
            Brush_1.AddPlane(FVector(47.5f, 133.0f, 19.0f), FVector(-48.5f, 133.0f, 19.0f), FVector(-48.5f, 133.0f, -3.0f));
            Brush_1.AddPlane(FVector(1333.5f, 113.0f, 0.0f), FVector(1333.5f, 101.0f, 10.0f), FVector(1365.5f, 113.0f, 0.0f));
            Brush_1.AddPlane(FVector(1333.5f, 133.0f, 10.0f), FVector(1333.5f, 120.0f, 0.0f), FVector(1365.5f, 133.0f, 10.0f));
            Brush_1.AddPlane(FVector(24.5f, -995.0f, 4.0f), FVector(24.5f, -995.0f, 0.0f), FVector(24.5f, -963.0f, 0.0f));
        }
        Entity_1.Brushes.Add(Brush_1);
        
        FBrush Brush_2("brush_2_clip_nosight_metal");
        {
            Brush_2.AddPlane(FVector(47.5, 133, 0), FVector(-48.5, 133, 0), FVector(-48.5, 101, 0));
            Brush_2.AddPlane(FVector(-48.5, 101, 19), FVector(-48.5, 133, 19), FVector(47.5, 133, 19));
            Brush_2.AddPlane(FVector(-48.5, 101, 19), FVector(47.5, 101, 19), FVector(47.5, 101, -3));
            Brush_2.AddPlane(FVector(47.5, 133, 19), FVector(-48.5, 133, 19), FVector(-48.5, 133, -3));
            Brush_2.AddPlane(FVector(-40.5, 133, 19), FVector(-40.5, 101, 19), FVector(-40.5, 101, -3));
            Brush_2.AddPlane(FVector(1333.5, 113, 0), FVector(1333.5, 101, 10), FVector(1365.5, 113, 0));
            Brush_2.AddPlane(FVector(1333.5, 133, 10), FVector(1333.5, 120, 0), FVector(1365.5, 133, 10));
            Brush_2.AddPlane(FVector(-24.5, -995, 1), FVector(-24.5, -995, 5), FVector(-24.5, -963, 5));
        }
        Entity_1.Brushes.Add(Brush_2);

        FBrush Brush_3("brush_3_clip_nosight_metal");
        {
            Brush_3.AddPlane(FVector(-22.5, 133, 11), FVector(-24.5, 133, 11), FVector(-24.5, 101, 11));
            Brush_3.AddPlane(FVector(-24.5, 101, 19), FVector(-24.5, 133, 19), FVector(-22.5, 133, 19));
            Brush_3.AddPlane(FVector(23.5, 101, 21), FVector(23.5, 133, 21), FVector(23.5, 133, 17));
            Brush_3.AddPlane(FVector(-22.5, 125, 19), FVector(-24.5, 125, 19), FVector(-24.5, 125, 15));
            Brush_3.AddPlane(FVector(-24.5, 133, 19), FVector(-24.5, 101, 19), FVector(-24.5, 101, 15));
            Brush_3.AddPlane(FVector(21.5, 112, 19), FVector(12.5, 112, 19), FVector(21.5, 112, 51));
        }
        Entity_1.Brushes.Add(Brush_3);

        FBrush Brush_4("brush_4_clip_nosight_metal");
        {
            Brush_4.AddPlane(FVector(47.5, 167, 19), FVector(-48.5, 167, 19), FVector(-48.5, -15, 19));
            Brush_4.AddPlane(FVector(-48.5, -15, 109), FVector(-48.5, 167, 109), FVector(47.5, 167, 109));
            Brush_4.AddPlane(FVector(39.5, -15, 109), FVector(39.5, 167, 109), FVector(39.5, 167, 19));
            Brush_4.AddPlane(FVector(47.5, 167, 109), FVector(-48.5, 167, 109), FVector(-48.5, 167, 19));
            Brush_4.AddPlane(FVector(-40.5, 167, 109), FVector(-40.5, -15, 109), FVector(-40.5, -15, 19));
            Brush_4.AddPlane(FVector(49, -35, 51), FVector(49, -35, 63), FVector(81, -35, 63));
            Brush_4.AddPlane(FVector(47.5, -35, 63), FVector(47.5, -15, 109), FVector(79.5, -35, 63));
        }
        Entity_1.Brushes.Add(Brush_4);
        
        FBrush Brush_5("brush_5_clip_nosight_metal");
        {
            Brush_5.AddPlane(FVector(47.5, 167, 19), FVector(-48.5, 167, 19), FVector(-48.5, -15, 19));
            Brush_5.AddPlane(FVector(-48.5, -55, 111), FVector(47.5, -55, 111), FVector(47.5, -55, 21));
            Brush_5.AddPlane(FVector(39.5, -15, 109), FVector(39.5, 167, 109), FVector(39.5, 167, 19));
            Brush_5.AddPlane(FVector(-40.5, 167, 109), FVector(-40.5, -15, 109), FVector(-40.5, -15, 19));
            Brush_5.AddPlane(FVector(49, -35, 63), FVector(49, -35, 51), FVector(81, -35, 63));
            Brush_5.AddPlane(FVector(47.5, -55, 51), FVector(47.5, -35, 63), FVector(79.5, -55, 51));
        }
        Entity_1.Brushes.Add(Brush_5);

        FBrush Brush_6("brush_6_clip_nosight_metal");
        {
            Brush_6.AddPlane(FVector(47.5, -15, 0), FVector(-48.5, -15, 0), FVector(-48.5, -47, 0));
            Brush_6.AddPlane(FVector(-48.5, -47, 19), FVector(-48.5, -15, 19), FVector(47.5, -15, 19));
            Brush_6.AddPlane(FVector(-48.5, -47, 19), FVector(47.5, -47, 19), FVector(47.5, -47, -3));
            Brush_6.AddPlane(FVector(39.5, -47, 19), FVector(39.5, -15, 19), FVector(39.5, -15, -3));
            Brush_6.AddPlane(FVector(47.5, -15, 19), FVector(-48.5, -15, 19), FVector(-48.5, -15, -3));
            Brush_6.AddPlane(FVector(1333.5, -35, 0), FVector(1333.5, -47, 10), FVector(1365.5, -35, 0));
            Brush_6.AddPlane(FVector(1333.5, -15, 10), FVector(1333.5, -28, 0), FVector(1365.5, -15, 10));
            Brush_6.AddPlane(FVector(24.5, -1143, 4), FVector(24.5, -1143, 0), FVector(24.5, -1111, 0));
        }
        Entity_1.Brushes.Add(Brush_6);

        FBrush Brush_7("brush_7_clip_nosight_metal");
        {
            Brush_7.AddPlane(FVector(47.5, -15, 0), FVector(-48.5, -15, 0), FVector(-48.5, -47, 0));
            Brush_7.AddPlane(FVector(-48.5, -47, 19), FVector(-48.5, -15, 19), FVector(47.5, -15, 19));
            Brush_7.AddPlane(FVector(-48.5, -47, 19), FVector(47.5, -47, 19), FVector(47.5, -47, -3));
            Brush_7.AddPlane(FVector(47.5, -15, 19), FVector(-48.5, -15, 19), FVector(-48.5, -15, -3));
            Brush_7.AddPlane(FVector(-40.5, -15, 19), FVector(-40.5, -47, 19), FVector(-40.5, -47, -3));
            Brush_7.AddPlane(FVector(1333.5, -35, 0), FVector(1333.5, -47, 10), FVector(1365.5, -35, 0));
            Brush_7.AddPlane(FVector(1333.5, -15, 10), FVector(1333.5, -28, 0), FVector(1365.5, -15, 10));
            Brush_7.AddPlane(FVector(-24.5, -1143, 1), FVector(-24.5, -1143, 5), FVector(-24.5, -1111, 5));
        }
        Entity_1.Brushes.Add(Brush_7);

        FBrush Brush_8("brush_8_clip_nosight_metal");
        {
            Brush_8.AddPlane(FVector(-22.5, 133, 11), FVector(-24.5, 133, 11), FVector(-24.5, 101, 11));
            Brush_8.AddPlane(FVector(-24.5, 101, 19), FVector(-24.5, 133, 19), FVector(-22.5, 133, 19));
            Brush_8.AddPlane(FVector(5.5, 102, 21), FVector(5.5, 134, 21), FVector(5.5, 134, 17));
            Brush_8.AddPlane(FVector(-7.5, 133, 19), FVector(-7.5, 101, 19), FVector(-7.5, 101, 15));
            Brush_8.AddPlane(FVector(23.5, -23, 12), FVector(23.5, -23, 19), FVector(55.5, -23, 19));
            Brush_8.AddPlane(FVector(12.5, 112, 19), FVector(21.5, 112, 19), FVector(21.5, 112, 51));
        }
        Entity_1.Brushes.Add(Brush_8);
    }
    
    AddEntity(Entity_1);
}

void AMapParser::AddEntity(FEntity& Entity)
{
    int PlanesFacesCount = 0;
    for (auto& Brush : Entity.Brushes)
    {
        PlanesFacesCount += Brush.Planes.Num();
        
        Brush.ProcessPlanes();
        
        for (auto& Face : Brush.Faces)
        {
            if(Face.Vertices.Num() < 3)
            {
                UE_LOG(LogTemp, Warning, TEXT("AMapParser: Not enough vertices to form a triangle!"));
                continue;
            }
            
            Face.SortVerticesClockwise();
            
            constexpr bool bColorFaces = false;
            FColor Color = bColorFaces ? GetUniqueColor(Face) :  GetUniqueColor(Brush);
            constexpr bool bDuplicateVertices = true;
            Face.Triangulate(Entity.Vertices, Entity.Indices, Entity.Colors, Color, bDuplicateVertices);
        }
    }
    
    UE_LOG(LogTemp, Display, TEXT("AMapParser: Entity added. Brushes: %d, Planes/Faces: %d, Vertices: %d, Indices: %d"), Entity.Brushes.Num(), PlanesFacesCount, Entity.Vertices.Num(), Entity.Indices.Num());
    
    const int EntityIndex = Entities.Add(Entity);
    //ProceduralMeshComponent->ClearAllMeshSections();
    //ProceduralMeshComponent->ClearMeshSection(EntityIndex);
    ProceduralMeshComponent->CreateMeshSection(EntityIndex, Entity.Vertices, Entity.Indices, TArray<FVector>(), TArray<FVector2D>(), Entity.Colors, TArray<FProcMeshTangent>(), true);
}

void AMapParser::BeginPlay()
{
    Super::BeginPlay();
}

void AMapParser::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    LoadEntities();

    const FTransform& ActorTransform = GetActorTransform();

    for (const auto& Entity : Entities)
    {
        for (const auto& Vertex: Entity.Vertices)
        {
            // Transform the local vertex position to world space
            const FVector WorldPosition = ActorTransform.TransformPosition(Vertex);
            DrawDebugPoint(GetWorld(), WorldPosition, 7.0f, FColor::Red, false, -1.0f, 0);
        }
        
        for (int32 i = 0; i < Entity.Indices.Num(); i += 3)
        {
            const int32 Index0 = Entity.Indices[i];
            const int32 Index1 = Entity.Indices[i + 1];
            const int32 Index2 = Entity.Indices[i + 2];

            const FVector V1 = ActorTransform.TransformPosition(Entity.Vertices[Index0]);
            const FVector V2 = ActorTransform.TransformPosition(Entity.Vertices[Index1]);
            const FVector V3 = ActorTransform.TransformPosition(Entity.Vertices[Index2]);
            
            DrawDebugLine(GetWorld(), V1, V2, FColor::White, false, -1.0f, 0, 0.5f);
            DrawDebugLine(GetWorld(), V2, V3, FColor::White, false, -1.0f, 0, 0.5f);
            DrawDebugLine(GetWorld(), V3, V1, FColor::White, false, -1.0f, 0, 0.5f);
        }
    }
}

bool AMapParser::ShouldTickIfViewportsOnly() const
{
    return true;
}
