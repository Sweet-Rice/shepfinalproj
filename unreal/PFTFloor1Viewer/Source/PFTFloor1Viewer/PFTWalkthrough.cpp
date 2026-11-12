#include "PFTWalkthrough.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Canvas.h"
#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Components/InputComponent.h"
#include "Misc/CommandLine.h"
#include "Misc/Parse.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "HAL/PlatformMisc.h"

APFTMeshGroup::APFTMeshGroup() {
 Architecture=CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Architecture"));
 SetRootComponent(Architecture); Architecture->SetMobility(EComponentMobility::Static);
 Architecture->SetCanEverAffectNavigation(false);
}
APFTWalker::APFTWalker() {
 PrimaryActorTick.bCanEverTick=true;
 GetCapsuleComponent()->InitCapsuleSize(30.f,88.f);
 auto* Camera=CreateDefaultSubobject<UCameraComponent>(TEXT("Eyes"));
 Camera->SetupAttachment(GetCapsuleComponent()); Camera->SetRelativeLocation(FVector(0,0,64));
 Camera->bUsePawnControlRotation=true; Camera->FieldOfView=85;
 GetCharacterMovement()->MaxWalkSpeed=230; GetCharacterMovement()->MaxStepHeight=42;
 GetCharacterMovement()->JumpZVelocity=300; GetCharacterMovement()->AirControl=.15f;
}
void APFTWalker::BeginPlay() {
 Super::BeginPlay();
 // Find a real first-floor surface and enough standing room around the lobby start.
 FCollisionQueryParams Query(SCENE_QUERY_STAT(PFTSpawn),true,this);
 FVector SpawnAnchor=GetActorLocation();
 if(SpawnAnchor.X<4000 || SpawnAnchor.X>20000) SpawnAnchor=FVector(10000,-4800,180);
 bool Found=false;
 for(int Ring=0;Ring<12 && !Found;++Ring) {
  for(int Side=0;Side<8 && !Found;++Side) {
   const float Angle=Side*PI/4;
   FVector XY=SpawnAnchor+FVector(FMath::Cos(Angle)*Ring*100,FMath::Sin(Angle)*Ring*100,0);
   FHitResult Hit;
   if(GetWorld()->LineTraceSingleByChannel(Hit,FVector(XY.X,XY.Y,250),FVector(XY.X,XY.Y,-140),ECC_Visibility,Query)
      && Hit.ImpactNormal.Z>.8 && Hit.ImpactPoint.Z<160) {
    FVector Candidate=Hit.ImpactPoint+FVector(0,0,90);
    if(!GetWorld()->OverlapBlockingTestByChannel(Candidate,FQuat::Identity,ECC_Pawn,FCollisionShape::MakeCapsule(30,87),Query)) {
     SetActorLocation(Candidate,false,nullptr,ETeleportType::TeleportPhysics); Found=true;
    }
   }
  }
 }
 LastSafePosition=GetActorLocation();
 UE_LOG(LogTemp,Display,TEXT("PFT WALK: spawn %s grounded candidate=%d"),*LastSafePosition.ToString(),Found);
 if(APlayerController* PC=Cast<APlayerController>(Controller)) {
  PC->bShowMouseCursor=false; PC->SetInputMode(FInputModeGameOnly());
 }
}
void APFTWalker::Tick(float DeltaSeconds) {
 Super::Tick(DeltaSeconds); BoundaryTime=FMath::Max(0.f,BoundaryTime-DeltaSeconds);
 const FVector Here=GetActorLocation();
 const float Feet=Here.Z-GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
 // Architectural height remains intact. Traversal alone is limited to the ground floor.
 if(Feet>220 || Feet< -180) {
  SetActorLocation(LastSafePosition,false,nullptr,ETeleportType::TeleportPhysics);
  GetCharacterMovement()->StopMovementImmediately(); BoundaryTime=2.5f;
 } else if(GetCharacterMovement()->IsMovingOnGround() && Feet<180) LastSafePosition=Here;
 if(FParse::Param(FCommandLine::Get(),TEXT("PFTVerify"))) {
  VerifyTime+=DeltaSeconds;
  if(VerifyStage==0 && VerifyTime>2) {
   VerifyGrounded=GetCharacterMovement()->IsMovingOnGround();
   SetActorLocation(FVector(Here.X,Here.Y,750),false,nullptr,ETeleportType::TeleportPhysics);
   VerifyStage=1;
  } else if(VerifyStage==1 && VerifyTime>2.2f) {
   bool Guard=GetActorLocation().Z<400;
   FString Result=FString::Printf(TEXT("grounded=%d\nupper_floor_guard=%d\nposition=%s\n"),VerifyGrounded,Guard,*GetActorLocation().ToString());
   FFileHelper::SaveStringToFile(Result,*(FPaths::ProjectSavedDir()/TEXT("walkthrough_runtime_check.txt")));
   UE_LOG(LogTemp,Display,TEXT("PFT VERIFY: %s"),*Result);
   FPlatformMisc::RequestExitWithStatus(false,VerifyGrounded && Guard ? 0 : 1);
   VerifyStage=2;
  }
 }
}
void APFTWalker::Forward(float V){ if(Controller) AddMovementInput(FRotationMatrix(FRotator(0,Controller->GetControlRotation().Yaw,0)).GetUnitAxis(EAxis::X),V); }
void APFTWalker::Right(float V){ if(Controller) AddMovementInput(FRotationMatrix(FRotator(0,Controller->GetControlRotation().Yaw,0)).GetUnitAxis(EAxis::Y),V); }
void APFTWalker::Turn(float V){AddControllerYawInput(V);}
void APFTWalker::Look(float V){AddControllerPitchInput(V);}
void APFTWalker::Run(){GetCharacterMovement()->MaxWalkSpeed=390;}
void APFTWalker::Walk(){GetCharacterMovement()->MaxWalkSpeed=230;}
void APFTWalker::SetupPlayerInputComponent(UInputComponent* Input) {
 Super::SetupPlayerInputComponent(Input);
 Input->BindAxis("Forward",this,&APFTWalker::Forward); Input->BindAxis("Right",this,&APFTWalker::Right);
 Input->BindAxis("Turn",this,&APFTWalker::Turn); Input->BindAxis("Look",this,&APFTWalker::Look);
 Input->BindAction("Run",IE_Pressed,this,&APFTWalker::Run); Input->BindAction("Run",IE_Released,this,&APFTWalker::Walk);
 Input->BindAction<ACharacter>("Jump",IE_Pressed,this,&ACharacter::Jump); Input->BindAction<ACharacter>("Jump",IE_Released,this,&ACharacter::StopJumping);
}
void APFTWalkHUD::DrawHUD() {
 Super::DrawHUD(); if(!Canvas)return;
 DrawText(TEXT("PFT  |  FIRST FLOOR     WASD walk  /  mouse look  /  Shift run  /  Esc or Shift+F1 release"),FLinearColor(.9,.92,.95,.8),22,22,GEngine->GetSmallFont());
 DrawLine(Canvas->ClipX*.5f-3,Canvas->ClipY*.5f,Canvas->ClipX*.5f+3,Canvas->ClipY*.5f,FLinearColor(1,1,1,.5));
 if(auto* Walker=Cast<APFTWalker>(GetOwningPawn()); Walker && Walker->IsAtBoundary())
  DrawText(TEXT("Walkthrough access is limited to the first floor"),FLinearColor(1,.8,.4),22,48,GEngine->GetSmallFont());
}
APFTWalkGameMode::APFTWalkGameMode(){DefaultPawnClass=APFTWalker::StaticClass();HUDClass=APFTWalkHUD::StaticClass();}
