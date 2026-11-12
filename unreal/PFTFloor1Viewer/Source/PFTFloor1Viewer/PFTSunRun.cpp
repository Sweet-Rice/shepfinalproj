#include "PFTSunRun.h"

#include "Components/InputComponent.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "HAL/PlatformMisc.h"
#include "InputCoreTypes.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "ShaderCompiler.h"

namespace
{
    const FVector CommonsStart(15124, -7300, 105);
    const FRotator CommonsFacing(0, 105, 0);
    constexpr float HeadStart = 25;
    constexpr float FullSpreadTime = 60;
    constexpr float SpinTime = 1.8f;
    constexpr float MaxHealth = 100;
    constexpr float DamagePerSecond = 20;
    constexpr float DoorRange = 200;
    constexpr float RadiusAtFullSpread = 3200;

    struct FClassroom
    {
        int32 Number;
        FVector Door;
        FVector Approach;
    };

    // Door centers come from the walkthrough mesh bounds. Approach points are on
    // the hallway side so the player can check a closed door.
    const FClassroom Rooms[] = {
        {1225, FVector(12384, -6887, 145), FVector(12384, -6757, 100)},
        {1221, FVector(12216, -6933, 145), FVector(12216, -6800, 100)}
    };
    const FBox2D Shade(FVector2D(13300, -6330), FVector2D(13520, -6120));

    bool IsPreparingMaterials()
    {
        return GShaderCompilingManager && GShaderCompilingManager->IsCompiling();
    }

    float SunArrivalTime(const FVector& Position)
    {
        return HeadStart + (FullSpreadTime - HeadStart)
            * static_cast<float>(FVector::Dist2D(CommonsStart, Position) / RadiusAtFullSpread);
    }

    bool IsInShade(const FVector& Position)
    {
        return Shade.IsInsideOrOn(FVector2D(Position));
    }

    bool SunReached(const FVector& Position, float Elapsed)
    {
        return Elapsed >= SunArrivalTime(Position);
    }

    const FClassroom* FindCheckedDoor(APawn& Pawn)
    {
        FVector Eyes;
        FRotator Look;
        Pawn.GetActorEyesViewPoint(Eyes, Look);
        const FClassroom* Best = nullptr;
        double BestDistance = DoorRange;
        for (const FClassroom& Room : Rooms)
        {
            const FVector ToDoor = Room.Door - Eyes;
            const double Distance = ToDoor.Size();
            const FVector Position = Pawn.GetActorLocation();
            if (Distance >= BestDistance || FVector::DotProduct(ToDoor.GetSafeNormal(), Look.Vector()) <= .65
                || FVector::Dist2D(Position, Room.Approach) >= DoorRange
                || FVector::DotProduct(Room.Approach - Room.Door, Position - Room.Door) <= 0)
            {
                continue;
            }

            const FCollisionQueryParams Query(SCENE_QUERY_STAT(PFTDoor), true, &Pawn);
            FHitResult Hit;
            if (!Pawn.GetWorld()->LineTraceSingleByChannel(Hit, Eyes, Room.Door, ECC_Visibility, Query)
                || FVector::DistSquared(Hit.ImpactPoint, Room.Door) <= FMath::Square(45.f))
            {
                Best = &Room;
                BestDistance = Distance;
            }
        }
        return Best;
    }
}

APFTSunRunMode::APFTSunRunMode()
{
    DefaultPawnClass = APFTSunRunWalker::StaticClass();
    HUDClass = APFTSunRunHUD::StaticClass();
    PrimaryActorTick.bCanEverTick = true;
}

void APFTSunRunMode::RestartPlayer(AController* Player)
{
    RestartPlayerAtTransform(Player, FTransform(CommonsFacing, CommonsStart));
    if (const APawn* Pawn = Player ? Player->GetPawn() : nullptr)
    {
        GroundedStart = Pawn->GetActorLocation();
        ResetRound(EPFTRoundPhase::Wheel);
    }
}

void APFTSunRunMode::SetMovementLocked(APlayerController& Player, bool bLocked) const
{
    Player.ResetIgnoreMoveInput();
    Player.SetIgnoreMoveInput(bLocked);
}

void APFTSunRunMode::ResetRound(EPFTRoundPhase NextPhase)
{
    Phase = NextPhase;
    SpinRemaining = 0;
    Elapsed = 0;
    Health = MaxHealth;
    FeedbackRemaining = 0;
    DoorFeedback.Reset();
    bDoorCheckQueued = false;

    if (APlayerController* Player = GetWorld()->GetFirstPlayerController())
    {
        if (ACharacter* Walker = Player->GetCharacter())
        {
            Walker->SetActorLocationAndRotation(GroundedStart, CommonsFacing, false, nullptr, ETeleportType::TeleportPhysics);
            Walker->GetCharacterMovement()->StopMovementImmediately();
            Player->SetControlRotation(CommonsFacing);
            SetMovementLocked(*Player, Phase != EPFTRoundPhase::Running);
        }
    }
}

void APFTSunRunMode::Spin()
{
    if (Phase == EPFTRoundPhase::Won || Phase == EPFTRoundPhase::Lost)
    {
        ResetRound(EPFTRoundPhase::Wheel);
    }
    if (Phase == EPFTRoundPhase::Wheel && SpinRemaining <= 0 && !IsPreparingMaterials())
    {
        AssignedRoom = FMath::RandHelper(UE_ARRAY_COUNT(Rooms));
        SpinRemaining = SpinTime;
    }
}

void APFTSunRunMode::Retry()
{
    if (Phase == EPFTRoundPhase::Won || Phase == EPFTRoundPhase::Lost)
    {
        ResetRound(EPFTRoundPhase::Running);
    }
}

void APFTSunRunMode::CheckDoor()
{
    bDoorCheckQueued = Phase == EPFTRoundPhase::Running;
}

void APFTSunRunMode::AdvanceRound(APawn& Pawn, float DeltaSeconds)
{
    FeedbackRemaining = FMath::Max(0.f, FeedbackRemaining - DeltaSeconds);
    if (bDoorCheckQueued)
    {
        bDoorCheckQueued = false;
        if (const FClassroom* Room = FindCheckedDoor(Pawn))
        {
            if (Room->Number == Rooms[AssignedRoom].Number)
            {
                Phase = EPFTRoundPhase::Won;
                return;
            }
            DoorFeedback = FString::Printf(TEXT("That's room %d."), Room->Number);
        }
        else
        {
            DoorFeedback = TEXT("Get close and face a classroom door.");
        }
        FeedbackRemaining = 2.5f;
    }

    const float PreviousElapsed = Elapsed;
    Elapsed += DeltaSeconds;
    const FVector Position = Pawn.GetActorLocation();
    if (!IsInShade(Position) && SunReached(Position, Elapsed))
    {
        const float ExposedSeconds = Elapsed - FMath::Max(PreviousElapsed, SunArrivalTime(Position));
        Health = FMath::Max(0.f, Health - DamagePerSecond * ExposedSeconds);
        if (Health <= 0)
        {
            Phase = EPFTRoundPhase::Lost;
        }
    }
}

void APFTSunRunMode::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    APlayerController* Player = GetWorld()->GetFirstPlayerController();
    APFTWalker* Walker = Player ? Cast<APFTWalker>(Player->GetPawn()) : nullptr;
    if (!Walker)
    {
        return;
    }
    if (!bVerified && GetWorld()->GetTimeSeconds() >= 1 && FParse::Param(FCommandLine::Get(), TEXT("PFTVerifySunRun")))
    {
        bVerified = true;
        if (!VerifyRound(*Walker) || !StartVerificationWalk(*Walker, 0))
        {
            FPlatformMisc::RequestExitWithStatus(false, 1);
        }
        return;
    }

    if (!VerificationPath.IsEmpty() && !DriveVerificationWalk(*Walker))
    {
        return;
    }

    if (Phase == EPFTRoundPhase::Wheel && SpinRemaining > 0)
    {
        SpinRemaining = FMath::Max(0.f, SpinRemaining - DeltaSeconds);
        if (SpinRemaining == 0)
        {
            Phase = EPFTRoundPhase::Running;
            SetMovementLocked(*Player, false);
        }
    }
    else if (Phase == EPFTRoundPhase::Running)
    {
        AdvanceRound(*Walker, DeltaSeconds);
        if (Phase != EPFTRoundPhase::Running)
        {
            SetMovementLocked(*Player, true);
        }
    }
}

void APFTSunRunWalker::SetupPlayerInputComponent(UInputComponent* Input)
{
    Super::SetupPlayerInputComponent(Input);
    Input->BindKey(EKeys::Enter, IE_Pressed, this, &APFTSunRunWalker::Spin);
    Input->BindKey(EKeys::R, IE_Pressed, this, &APFTSunRunWalker::Retry);
    Input->BindKey(EKeys::E, IE_Pressed, this, &APFTSunRunWalker::CheckDoor);
    Input->BindKey(EKeys::Escape, IE_Pressed, this, &APFTSunRunWalker::TogglePause).bExecuteWhenPaused = true;
}

bool APFTSunRunWalker::CanJumpInternal_Implementation() const
{
    return Controller && !Controller->IsMoveInputIgnored() && Super::CanJumpInternal_Implementation();
}

void APFTSunRunWalker::Spin()
{
    if (APFTSunRunMode* Mode = GetWorld()->GetAuthGameMode<APFTSunRunMode>())
    {
        Mode->Spin();
    }
}

void APFTSunRunWalker::Retry()
{
    if (APFTSunRunMode* Mode = GetWorld()->GetAuthGameMode<APFTSunRunMode>())
    {
        Mode->Retry();
    }
}

void APFTSunRunWalker::TogglePause()
{
    if (APlayerController* Player = Cast<APlayerController>(Controller))
    {
        Player->SetPause(!GetWorld()->IsPaused());
    }
}

void APFTSunRunWalker::CheckDoor()
{
    if (APFTSunRunMode* Mode = GetWorld()->GetAuthGameMode<APFTSunRunMode>())
    {
        Mode->CheckDoor();
    }
}

void APFTSunRunHUD::DrawHUD()
{
    Super::DrawHUD();
    const APFTSunRunMode* Mode = GetWorld()->GetAuthGameMode<APFTSunRunMode>();
    const APawn* Pawn = GetOwningPawn();
    if (!Mode || !Canvas || !Pawn)
    {
        return;
    }

    const float Width = Canvas->ClipX;
    const float Height = Canvas->ClipY;
    const FLinearColor Ink(.94f, .97f, 1.f);
    const FLinearColor Gold(1.f, .73f, .31f);
    const FLinearColor Aqua(.38f, .92f, .83f);
    const FLinearColor Panel(.015f, .025f, .04f, .91f);
    const FLinearColor Red(1.f, .31f, .17f);
    const int32 Room = Rooms[Mode->AssignedRoom].Number;

    if (GetWorld()->IsPaused())
    {
        const float Left = Width / 2 - 142;
        const float Top = Height / 2 - 54;
        DrawRect(Panel, Left, Top, 284, 108);
        DrawText(TEXT("Paused"), Ink, Left + 22, Top + 16, GEngine->GetLargeFont(), 1.4f);
        DrawText(TEXT("Esc   Resume"), Ink, Left + 22, Top + 66, GEngine->GetMediumFont());
        return;
    }

    if (Mode->Phase == EPFTRoundPhase::Wheel)
    {
        const float Left = Width / 2 - 164;
        const float Top = Height / 2 - 142;
        DrawRect(Panel, Left, Top, 328, 284);
        DrawText(TEXT("PFT Sun Run"), Ink, Left + 26, Top + 18, GEngine->GetLargeFont(), 1.45f);
        const FVector2D Center(Width / 2, Top + 137);
        const float Progress = 1 - Mode->SpinRemaining / SpinTime;
        const float Angle = Mode->SpinRemaining > 0
            ? (8 - Mode->AssignedRoom) * PI * (1 - FMath::Square(1 - Progress)) : 0.f;
        for (int32 Step = 0; Step < 48; ++Step)
        {
            const float A = Step * 2 * PI / 48;
            const float B = (Step + 1) * 2 * PI / 48;
            DrawLine(Center.X + 64 * FMath::Cos(A), Center.Y + 64 * FMath::Sin(A),
                Center.X + 64 * FMath::Cos(B), Center.Y + 64 * FMath::Sin(B), Gold, 2);
        }
        for (int32 Index = 0; Index < UE_ARRAY_COUNT(Rooms); ++Index)
        {
            const float A = Angle + Index * PI;
            const FVector2D Label = Center + 34 * FVector2D(FMath::Cos(A), FMath::Sin(A));
            DrawText(FString::FromInt(Rooms[Index].Number), Ink, Label.X - 17, Label.Y - 8, GEngine->GetSmallFont(), 1.25f);
        }
        DrawLine(Center.X + 66, Center.Y, Center.X + 78, Center.Y, Gold, 4);
        const TCHAR* Prompt = IsPreparingMaterials() ? TEXT("Preparing the building materials...")
            : (Mode->SpinRemaining > 0 ? TEXT("Assigning your classroom...") : TEXT("Enter   Spin for your classroom"));
        DrawText(Prompt,
            Ink, Left + 26, Top + 225, GEngine->GetMediumFont(), 1.f);
        return;
    }

    const FVector Position = Pawn->GetActorLocation();
    const bool bExpanding = Mode->Elapsed >= HeadStart;
    const bool bShade = IsInShade(Position);
    const bool bBurning = !bShade && SunReached(Position, Mode->Elapsed);
    if (bBurning && Mode->Phase == EPFTRoundPhase::Running)
    {
        DrawRect(FLinearColor(1.f, .28f, .04f, .17f), 0, 0, Width, Height);
    }
    DrawRect(Panel, 22, 22, 284, 110);
    DrawText(FString::Printf(TEXT("Find room %d"), Room), Ink, 36, 32, GEngine->GetLargeFont(), 1.35f);
    FString Status = FString::Printf(TEXT("Sun expands in %ds"), FMath::CeilToInt(HeadStart - Mode->Elapsed));
    if (bExpanding)
    {
        Status = bShade ? TEXT("In shade") : (bBurning ? TEXT("In sunlight. Find shade.")
            : FString::Printf(TEXT("Sun reaches here in %ds"), FMath::CeilToInt(SunArrivalTime(Position) - Mode->Elapsed)));
    }
    DrawText(Status, bShade && bExpanding ? Aqua : (bBurning ? Red : Gold), 36, 67, GEngine->GetMediumFont(), 1.f);
    DrawRect(FLinearColor(.13f, .17f, .22f), 36, 100, 196, 9);
    DrawRect(bBurning ? Red : Aqua, 36, 100, 196 * Mode->Health / MaxHealth, 9);
    DrawText(FString::Printf(TEXT("%d HP"), FMath::CeilToInt(Mode->Health)), Ink, 241, 95, GEngine->GetSmallFont());

    if (Mode->Phase == EPFTRoundPhase::Won || Mode->Phase == EPFTRoundPhase::Lost)
    {
        const float Left = Width / 2 - 172;
        const float Top = Height / 2 - 75;
        DrawRect(Panel, Left, Top, 344, 150);
        const bool bWon = Mode->Phase == EPFTRoundPhase::Won;
        DrawText(bWon ? FString::Printf(TEXT("Room %d found"), Room) : TEXT("The sun caught you"),
            bWon ? Aqua : Gold, Left + 22, Top + 22, GEngine->GetLargeFont());
        DrawText(FString::Printf(TEXT("R   Retry room %d"), Room), Ink, Left + 22, Top + 75, GEngine->GetMediumFont());
        DrawText(TEXT("Enter   Spin again"), Ink, Left + 22, Top + 104, GEngine->GetMediumFont());
        return;
    }

    FVector Eyes;
    FRotator Look;
    Pawn->GetActorEyesViewPoint(Eyes, Look);
    const auto DrawMarker = [&](const FVector& Marker, const FString& Label, const FLinearColor& Color, double Range)
    {
        const FVector Offset = Marker - Eyes;
        if (Offset.SizeSquared() > FMath::Square(Range) || FVector::DotProduct(Offset, Look.Vector()) <= 0)
        {
            return;
        }
        FVector2D Screen;
        const APlayerController* Player = GetOwningPlayerController();
        const FCollisionQueryParams Query(SCENE_QUERY_STAT(PFTHudMarker), true, Pawn);
        if (Player && Player->ProjectWorldLocationToScreen(Marker, Screen)
            && Screen.X > 50 && Screen.X < Width - 50 && Screen.Y > 30 && Screen.Y < Height - 30
            && !GetWorld()->LineTraceTestByChannel(Eyes, Marker, ECC_Visibility, Query))
        {
            DrawRect(Panel, Screen.X - 41, Screen.Y - 11, 82, 22);
            DrawText(Label, Color, Screen.X - 31, Screen.Y - 7, GEngine->GetSmallFont());
        }
    };
    for (const FClassroom& Classroom : Rooms)
    {
        const FVector HallOffset = (Classroom.Approach - Classroom.Door).GetSafeNormal2D() * 40;
        DrawMarker(Classroom.Door + HallOffset + FVector(0, 0, 65),
            FString::Printf(TEXT("Room %d"), Classroom.Number), Ink, 500);
    }
    DrawMarker(FVector(Shade.GetCenter(), 72), TEXT("Shade"), Aqua, 680);

    DrawLine(Width / 2 - 3, Height / 2, Width / 2 + 3, Height / 2, Ink);
    DrawLine(Width / 2, Height / 2 - 3, Width / 2, Height / 2 + 3, Ink);
    if (const FClassroom* Door = FindCheckedDoor(*GetOwningPawn()))
    {
        DrawText(FString::Printf(TEXT("E   Check room %d"), Door->Number), Ink, Width / 2 - 78, Height / 2 + 22, GEngine->GetMediumFont());
    }
    DrawRect(Panel, 22, Height - 49, 435, 27);
    DrawText(TEXT("WASD Move    Shift Sprint    Mouse Look    E Check door    Esc Pause"), Ink, 34, Height - 43, GEngine->GetSmallFont());
    if (Mode->FeedbackRemaining > 0)
    {
        DrawRect(Panel, 22, 139, 284, 29);
        DrawText(Mode->DoorFeedback, Ink, 36, 146, GEngine->GetMediumFont());
    }
    else if (FVector::DistSquared2D(Position, CommonsStart) < FMath::Square(1000.f))
    {
        DrawRect(Panel, 22, 139, 284, 42);
        DrawText(TEXT("Follow the Commons hall toward the stairs."), Ink, 36, 146, GEngine->GetSmallFont());
        DrawText(TEXT("Stay on the first floor."), Gold, 36, 163, GEngine->GetSmallFont());
    }
}

#include "PFTSunRunVerify.inl"

bool APFTSunRunMode::VerifyRound(APFTWalker& Walker)
{
    APlayerController& Player = *GetWorld()->GetFirstPlayerController();
    const bool bSpawn = Walker.GetCharacterMovement()->IsMovingOnGround()
        && FVector::Dist2D(CommonsStart, GroundedStart) < 150;
    Spin();
    Tick(SpinTime / 2);
    const bool bWheelLocked = Phase == EPFTRoundPhase::Wheel && Player.IsMoveInputIgnored()
        && !Walker.CanJump() && Elapsed == 0;
    const int32 RolledRoom = AssignedRoom;
    Tick(SpinTime);
    const bool bHandoff = Phase == EPFTRoundPhase::Running && !Player.IsMoveInputIgnored() && Elapsed == 0;
    Tick(HeadStart - .1f);
    const bool bHeadStart = Health == MaxHealth;
    Tick(1);
    const bool bExposure = Health < MaxHealth;

    const auto AimAt = [&](const FClassroom& Room)
    {
        Walker.SetActorLocation(Room.Approach);
        Player.SetControlRotation((Room.Door - Walker.GetPawnViewLocation()).Rotation());
    };

    AimAt(Rooms[AssignedRoom]);
    const float HealthAheadOfFront = Health;
    Tick(1);
    const bool bAheadOfFront = Health == HealthAheadOfFront;

    const FVector ShadeCenter(Shade.GetCenter(), 100);
    const double ShadeDistance = WalkableDistance(Walker, GroundedStart, ShadeCenter);
    Walker.SetActorLocation(ShadeCenter);
    Elapsed = 57;
    Tick(3);
    const bool bShade = ShadeDistance > 0 && Health == HealthAheadOfFront && Elapsed == FullSpreadTime;
    Walker.SetActorLocation(GroundedStart);
    Tick(1);
    const bool bLeaveShade = Health < HealthAheadOfFront;
    UE_LOG(LogTemp, Display, TEXT("PFT SUN RUN: shade_walkable_distance_cm=%.0f"), ShadeDistance);

    bool bRoutes = true;
    bool bWrongDoor = true;
    bool bLookAway = true;
    bool bLateWin = true;
    bool bRetry = true;
    for (int32 Index = 0; Index < UE_ARRAY_COUNT(Rooms); ++Index)
    {
        const int32 ExpectedRoom = (RolledRoom + Index) % UE_ARRAY_COUNT(Rooms);
        AssignedRoom = ExpectedRoom;
        const FClassroom& Target = Rooms[ExpectedRoom];
        const FClassroom& Other = Rooms[(ExpectedRoom + 1) % UE_ARRAY_COUNT(Rooms)];
        const double Distance = WalkableDistance(Walker, GroundedStart, Target.Approach);
        UE_LOG(LogTemp, Display, TEXT("PFT SUN RUN: room=%d walkable_distance_cm=%.0f"), Target.Number, Distance);
        bRoutes &= Distance > 0;

        Elapsed = FullSpreadTime;
        Health = MaxHealth;
        AimAt(Other);
        CheckDoor();
        Tick(.01f);
        bWrongDoor &= Phase == EPFTRoundPhase::Running && AssignedRoom == ExpectedRoom && Elapsed > FullSpreadTime
            && DoorFeedback == FString::Printf(TEXT("That's room %d."), Other.Number);

        AimAt(Target);
        Player.SetControlRotation(Player.GetControlRotation() + FRotator(0, 180, 0));
        CheckDoor();
        Tick(.01f);
        bLookAway &= Phase == EPFTRoundPhase::Running && DoorFeedback == TEXT("Get close and face a classroom door.");

        AimAt(Target);
        Health = 1;
        CheckDoor();
        Tick(1);
        bLateWin &= Phase == EPFTRoundPhase::Won && Health == 1 && Player.IsMoveInputIgnored();

        Retry();
        bRetry &= Phase == EPFTRoundPhase::Running && Health == MaxHealth && Elapsed == 0
            && AssignedRoom == ExpectedRoom && DoorFeedback.IsEmpty() && !Player.IsMoveInputIgnored()
            && FVector::Dist2D(Walker.GetActorLocation(), GroundedStart) < 2;
    }

    Tick(HeadStart + MaxHealth / DamagePerSecond + 1);
    AimAt(Rooms[AssignedRoom]);
    CheckDoor();
    Tick(1);
    const bool bLost = Phase == EPFTRoundPhase::Lost && Health == 0 && !bDoorCheckQueued && Player.IsMoveInputIgnored();
    Spin();
    const bool bSpinAgain = Phase == EPFTRoundPhase::Wheel && Health == MaxHealth && Elapsed == 0
        && SpinRemaining > 0 && Player.IsMoveInputIgnored();

    const bool bPassed = bSpawn && bWheelLocked && bHandoff && bHeadStart && bExposure && bAheadOfFront
        && bRoutes && bShade && bLeaveShade && bWrongDoor && bLookAway && bLateWin && bRetry && bLost && bSpinAgain;
    UE_LOG(LogTemp, Display, TEXT("PFT SUN RUN: %s spawn=%d wheel_locked=%d handoff=%d head_start=%d exposure=%d ahead_of_front=%d routes=%d shade=%d leave_shade=%d wrong_door=%d look_away=%d late_win=%d retry=%d loss=%d spin_again=%d"),
        bPassed ? TEXT("PASS") : TEXT("FAIL"), bSpawn, bWheelLocked, bHandoff, bHeadStart, bExposure, bAheadOfFront,
        bRoutes, bShade, bLeaveShade, bWrongDoor, bLookAway, bLateWin, bRetry, bLost, bSpinAgain);
    return bPassed;
}

bool APFTSunRunMode::StartVerificationWalk(APFTWalker& Walker, int32 RoomIndex)
{
    VerificationPath.Reset();
    if (WalkableDistance(Walker, GroundedStart, Rooms[RoomIndex].Approach, &VerificationPath) <= 0)
    {
        UE_LOG(LogTemp, Error, TEXT("PFT SUN RUN: no walking path for room %d"), Rooms[RoomIndex].Number);
        return false;
    }

    AssignedRoom = RoomIndex;
    ResetRound(EPFTRoundPhase::Running);
    VerificationWaypoint = 1;
    return true;
}

bool APFTSunRunMode::DriveVerificationWalk(APFTWalker& Walker)
{
    const FClassroom& Room = Rooms[AssignedRoom];
    if (Phase == EPFTRoundPhase::Won)
    {
        UE_LOG(LogTemp, Display, TEXT("PFT SUN RUN: walked to room %d in %.1fs (%.0f HP)"), Room.Number, Elapsed, Health);
        if (AssignedRoom + 1 < UE_ARRAY_COUNT(Rooms))
        {
            if (!StartVerificationWalk(Walker, AssignedRoom + 1))
            {
                FPlatformMisc::RequestExitWithStatus(false, 1);
            }
        }
        else
        {
            VerificationPath.Reset();
            UE_LOG(LogTemp, Display, TEXT("PFT SUN RUN: PASS both walking routes"));
            FPlatformMisc::RequestExitWithStatus(false, 0);
        }
        return false;
    }

    const FVector Here = Walker.GetActorLocation();
    if (Phase != EPFTRoundPhase::Running || Elapsed > FullSpreadTime)
    {
        UE_LOG(LogTemp, Error, TEXT("PFT SUN RUN: walk to %d stopped at waypoint %d of %d, %s, %.1fs"),
            Room.Number, VerificationWaypoint, VerificationPath.Num(), *Here.ToString(), Elapsed);
        VerificationPath.Reset();
        FPlatformMisc::RequestExitWithStatus(false, 1);
        return false;
    }

    if (VerificationWaypoint < VerificationPath.Num()
        && FVector::DistSquared2D(Here, VerificationPath[VerificationWaypoint]) < FMath::Square(14.f))
    {
        ++VerificationWaypoint;
    }
    if (VerificationWaypoint < VerificationPath.Num())
    {
        const FVector Direction = (VerificationPath[VerificationWaypoint] - Here).GetSafeNormal2D();
        Walker.GetController()->SetControlRotation(Direction.Rotation());
        Walker.AddMovementInput(Direction, 1.f);
    }
    else
    {
        Walker.GetController()->SetControlRotation((Room.Door - Walker.GetPawnViewLocation()).Rotation());
        CheckDoor();
    }
    return true;
}
