#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/HUD.h"
#include "Engine/AssetUserData.h"
#include "PFTWalkthrough.generated.h"

// Temporary landing class for obsolete Revit tracking metadata during migration.
// The map builder removes it from the copied meshes before saving them.
UCLASS()
class PFTFLOOR1VIEWER_API UPFTImportedMetadata : public UAssetUserData {
 GENERATED_BODY()
};

UCLASS()
class PFTFLOOR1VIEWER_API APFTMeshGroup : public AActor {
 GENERATED_BODY()
public:
 APFTMeshGroup();
 UPROPERTY(VisibleAnywhere, BlueprintReadOnly) TObjectPtr<class UInstancedStaticMeshComponent> Architecture;
};

UCLASS()
class PFTFLOOR1VIEWER_API APFTWalker : public ACharacter {
 GENERATED_BODY()
public:
 APFTWalker();
 virtual void BeginPlay() override;
 virtual void Tick(float DeltaSeconds) override;
 virtual void SetupPlayerInputComponent(UInputComponent* Input) override;
 bool IsAtBoundary() const { return BoundaryTime>0; }
private:
 void Forward(float Value); void Right(float Value); void Turn(float Value); void Look(float Value);
 void Run(); void Walk();
 FVector LastSafePosition=FVector::ZeroVector;
 float BoundaryTime=0;
 float VerifyTime=0;
 int32 VerifyStage=0;
 bool VerifyGrounded=false;
};

UCLASS()
class PFTFLOOR1VIEWER_API APFTWalkHUD : public AHUD {
 GENERATED_BODY()
 virtual void DrawHUD() override;
};

UCLASS()
class PFTFLOOR1VIEWER_API APFTWalkGameMode : public AGameModeBase {
 GENERATED_BODY()
public: APFTWalkGameMode();
};
