#include "PFTSunRun.h"

#include "Algo/Reverse.h"
#include "Components/CapsuleComponent.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

namespace
{
    // Search the loaded map for a continuous floor and standing capsule clearance.
    // Each edge is swept to catch thin walls and closed doors between samples.
    double WalkableDistance(APFTWalker& Walker, const FVector& Start, const FVector& Goal, TArray<FVector>* Path = nullptr)
    {
        constexpr int32 Step = 65;
        const FCollisionQueryParams Query(SCENE_QUERY_STAT(PFTSunRunRoute), true, &Walker);
        UWorld* World = Walker.GetWorld();
        const FCollisionShape Capsule = FCollisionShape::MakeCapsule(30, 87);
        const auto FindFloor = [&](const FVector2D& XY, FVector& Standing)
        {
            FHitResult Hit;
            if (!World->LineTraceSingleByChannel(Hit, FVector(XY, 200), FVector(XY, -140), ECC_Visibility, Query)
                || Hit.ImpactNormal.Z < .8 || Hit.ImpactPoint.Z > 125)
            {
                return false;
            }
            Standing = Hit.ImpactPoint + FVector(0, 0, 90);
            return !World->OverlapBlockingTestByChannel(Standing, FQuat::Identity, ECC_Pawn, Capsule, Query);
        };
        const auto ClearEdge = [&](const FVector& A, const FVector& B)
        {
            return FMath::Abs(A.Z - B.Z) < 35
                && !World->SweepTestByChannel(A, B, FQuat::Identity, ECC_Pawn, Capsule, Query);
        };

        FVector StartOnFloor;
        FVector GoalOnFloor;
        if (!FindFloor(FVector2D(Start), StartOnFloor) || !FindFloor(FVector2D(Goal), GoalOnFloor))
        {
            return -1;
        }
        struct FStep
        {
            FIntPoint Grid;
            FVector Position;
            double Distance;
            int32 Parent;
        };
        TArray<FStep> Queue{{FIntPoint::ZeroValue, StartOnFloor, 0, INDEX_NONE}};
        TSet<FIntPoint> Visited{FIntPoint::ZeroValue};
        const FIntPoint Neighbors[] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        for (int32 Index = 0; Index < Queue.Num(); ++Index)
        {
            const FStep Current = Queue[Index];
            if (FVector::Dist2D(Current.Position, GoalOnFloor) < Step * 1.5 && ClearEdge(Current.Position, GoalOnFloor))
            {
                if (Path)
                {
                    Path->Reset();
                    Path->Add(GoalOnFloor);
                    for (int32 Parent = Index; Parent != INDEX_NONE; Parent = Queue[Parent].Parent)
                    {
                        Path->Add(Queue[Parent].Position);
                    }
                    Algo::Reverse(*Path);
                }
                return Current.Distance + FVector::Dist2D(Current.Position, GoalOnFloor);
            }
            for (const FIntPoint& Offset : Neighbors)
            {
                const FIntPoint Next = Current.Grid + Offset;
                if (Next.X < -80 || Next.X > 34 || Next.Y < -19 || Next.Y > 75 || Visited.Contains(Next))
                {
                    continue;
                }
                const FVector2D XY(Start.X + Next.X * Step, Start.Y + Next.Y * Step);
                FVector Standing;
                if (FindFloor(XY, Standing) && ClearEdge(Current.Position, Standing))
                {
                    Visited.Add(Next);
                    Queue.Add({Next, Standing, Current.Distance + Step, Index});
                }
            }
        }
        return -1;
    }
}
