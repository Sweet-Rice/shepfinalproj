using UnrealBuildTool;
public class PFTFloor1ViewerEditorTarget : TargetRules {
 public PFTFloor1ViewerEditorTarget(TargetInfo Target) : base(Target) {
  Type=TargetType.Editor; DefaultBuildSettings=BuildSettingsVersion.V7; IncludeOrderVersion=EngineIncludeOrderVersion.Unreal5_8;
  ExtraModuleNames.Add("PFTFloor1Viewer");
 }
}
