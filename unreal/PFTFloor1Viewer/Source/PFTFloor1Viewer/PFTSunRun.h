#pragma once

#include "PFTWalkthrough.h"
#include "PFTSunRun.generated.h"

enum class EPFTRoundPhase { Wheel, Running, Won, Lost };

UCLASS()
class APFTSunRunMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    APFTSunRunMode();
    void RestartPlayer(AController* Player) override;
    void Tick(float DeltaSeconds) override;
    void Spin();
    void Retry();
    void CheckDoor();

private:
    friend class APFTSunRunHUD;
    void ResetRound(EPFTRoundPhase NextPhase);
    void AdvanceRound(APawn& Pawn, float DeltaSeconds);
    bool VerifyRound(APFTWalker& Walker);
    void SetMovementLocked(APlayerController& Player, bool bLocked) const;

    EPFTRoundPhase Phase = EPFTRoundPhase::Wheel;
    FVector GroundedStart = FVector::ZeroVector;
    int32 AssignedRoom = 0;
    float SpinRemaining = 0;
    float Elapsed = 0;
    float Health = 100;
    float FeedbackRemaining = 0;
    FString DoorFeedback;
    bool bDoorCheckQueued = false;
    bool bVerified = false;
};

UCLASS()
class APFTSunRunWalker : public APFTWalker
{
    GENERATED_BODY()

public:
    void SetupPlayerInputComponent(UInputComponent* Input) override;
    bool CanJumpInternal_Implementation() const override;

private:
    void Spin();
    void Retry();
    void CheckDoor();
};

UCLASS()
class APFTSunRunHUD : public AHUD
{
    GENERATED_BODY()

public:
    void DrawHUD() override;
};
