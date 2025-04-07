// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArenaBattle : ModuleRules
{
	public ArenaBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// ArenaBattle 모듈의 헤더 파일들을 다른 모듈들에서도 사용 가능하게 함
		PublicIncludePaths.AddRange(new string[] { "ArenaBattle" });
		
		// UMG - UI 관련 기능 사용하기 위해 추가 필요한 모듈 (Widget들)
		// NavigationSystem - NavMesh, PathFinding 기능 사용하기 위해 추가 필요
		// AIModule - AIController, BehaviorTree 기능 사용하기 위해 추가 필요
		// GameplayTasks - 언리얼 엔진에서 제공하는 UGameplayTask 기능 사용하기 위해 추가 필요
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG",
			"NavigationSystem", "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
