// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArenaBattle : ModuleRules
{
	public ArenaBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// ArenaBattle ����� ��� ���ϵ��� �ٸ� ���鿡���� ��� �����ϰ� ��
		PublicIncludePaths.AddRange(new string[] { "ArenaBattle" });
		
		// UMG - UI ���� ��� ����ϱ� ���� �߰� �ʿ��� ��� (Widget��)
		// NavigationSystem - NavMesh, PathFinding ��� ����ϱ� ���� �߰� �ʿ�
		// AIModule - AIController, BehaviorTree ��� ����ϱ� ���� �߰� �ʿ�
		// GameplayTasks - �𸮾� �������� �����ϴ� UGameplayTask ��� ����ϱ� ���� �߰� �ʿ�
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG",
			"NavigationSystem", "AIModule", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}
