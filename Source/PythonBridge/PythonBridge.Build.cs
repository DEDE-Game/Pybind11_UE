using System.IO;
using UnrealBuildTool;

public class PythonBridge : ModuleRules
{
    public PythonBridge(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "InputCore"
            }
        );

        // ��� pybind11 ����·��
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../ThirdParty/pybind11/include"));

        // ��� python371 ����·��
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../Source/ThirdParty/Python3124/include"));
        
        // Add the import library
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../Source/ThirdParty/Python3124/libs/python312.lib"));

        // ��� C++11 ����ѡ��
        CppStandard = CppStandardVersion.Cpp20;


        bUseRTTI = true;
    }
}
