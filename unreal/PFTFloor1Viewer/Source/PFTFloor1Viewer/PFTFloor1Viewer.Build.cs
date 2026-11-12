using UnrealBuildTool;
public class PFTFloor1Viewer : ModuleRules {
 public PFTFloor1Viewer(ReadOnlyTargetRules Target) : base(Target) {
  PrivateDependencyModuleNames.AddRange(new string[]{"MoviePlayer","Slate","SlateCore"});
  PCHUsage=PCHUsageMode.UseExplicitOrSharedPCHs;
  PublicDependencyModuleNames.AddRange(new string[]{"Core","CoreUObject","Engine","InputCore"});
 }
}
