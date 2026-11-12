using UnrealBuildTool;
public class PFTFloor1ViewerTarget : TargetRules {
 public PFTFloor1ViewerTarget(TargetInfo Target) : base(Target) {
  Type=TargetType.Game; DefaultBuildSettings=BuildSettingsVersion.V7; IncludeOrderVersion=EngineIncludeOrderVersion.Unreal5_8;
  ExtraModuleNames.Add("PFTFloor1Viewer");
 }
}
