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

        // 添加 pybind11 包含路径
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../ThirdParty/pybind11/include"));

        // 添加 python371 包含路径
        PublicIncludePaths.Add(Path.Combine(ModuleDirectory, "../../Source/ThirdParty/Python3124/include"));
        
        // Add the import library
        PublicAdditionalLibraries.Add(Path.Combine(ModuleDirectory, "../../Source/ThirdParty/Python3124/libs/python312.lib"));

        // 添加 C++11 编译选项
        CppStandard = CppStandardVersion.Cpp20;


        bUseRTTI = true;
    }
}
