// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class MP_Aptos : ModuleRules
{
    public MP_Aptos(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(
            new string[] {
				// ... add public include paths required here ...
			}
            );


        PrivateIncludePaths.AddRange(
            new string[] {
				// ... add other private include paths required here ...
			}
            );

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "ApplicationCore" });
        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "MP_AptosLibrary",
                "Projects",
				// ... add other public dependencies that you statically link with here ...
			}
            );

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
				// ... add any modules that your module loads dynamically here ...
			}
            );

        if (Target.Platform == UnrealTargetPlatform.Win64)
        {
            string ThirdPartyPath = Path.Combine(ModuleDirectory, "../ThirdParty/AptosSDK/");
            string BinariesPath = Path.Combine(ThirdPartyPath, "Binaries/");
            //string destinationBinaryDirectory = Path.Combine(Target.ProjectFile.Directory.FullName, "Binaries", "Win64");

            // Copy DLLs to output
            //RuntimeDependencies.Add(Path.Combine(BinariesPath, "Aptos.dll"), Path.Combine(destinationBinaryDirectory, "Aptos.dll"));
            //RuntimeDependencies.Add(Path.Combine(BinariesPath, "AptosUILogic.dll"), Path.Combine(destinationBinaryDirectory, "AptosUILogic.dll"));

            RuntimeDependencies.Add(Path.Combine(BinariesPath, "Aptos.dll"));
            RuntimeDependencies.Add(Path.Combine(BinariesPath, "AptosUILogic.dll"));
            // Copy LIBs (optional, if needed for linking)
            PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "cryptopp.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "bip3x.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "Aptos.lib"));
            PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "AptosUILogic.lib"));
        }

        //if (Target.Platform == UnrealTargetPlatform.Win64)
        //{
        //    // Path to ThirdParty folder
        //    string BaseDirectory = Path.Combine(ModuleDirectory, "Source/ThirdParty/AptosSDK/Binaries");
        //    string ThirdPartyPath = Path.Combine(ModuleDirectory, "..", "ThirdParty", "AptosSDK");
        //    string BinariesPath = Path.Combine(ThirdPartyPath, "Binaries");

        //    // Include headers (add this later if you have an Include folder)
        //    PublicSystemIncludePaths.Add(Path.Combine(ThirdPartyPath, "Include"));

        //    // Link static libraries (if .lib files exist alongside DLLs)
        //    PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "Aptos.lib")); // Optional, if exists
        //    PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "AptosUILogic.lib")); // Optional, if exists
        //    PublicAdditionalLibraries.Add(Path.Combine(BinariesPath, "cryptopp.lib"));

        //    // Delay-load DLLs (optional, helps with runtime loading order)
        //    PublicDelayLoadDLLs.Add("Aptos.dll");
        //    PublicDelayLoadDLLs.Add("AptosUILogic.dll");

        //    // Ensure DLLs are staged with the executable
        //    RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/AptosSDK/Binaries/Aptos.dll");
        //    RuntimeDependencies.Add("$(PluginDir)/Source/ThirdParty/AptosSDK/Binaries/AptosUILogic.dll");

        //}
        //if (Target.Platform == UnrealTargetPlatform.Win64) // Assuming 64-bit Windows
        //{
        //    //string AptosUiLogicPath = Path.Combine(ModuleDirectory, "../../../build/Release", "AptosUILogic.dll");
        //    //string AptosLibPath = Path.Combine(ModuleDirectory, "../../../build/Release", "Aptos.dll");
        //    string AptosUiLogic_LPath = Path.Combine(ModuleDirectory, "../../../build/Release", "AptosUILogic.lib");
        //    string AptosLib_LPath = Path.Combine(ModuleDirectory, "../../../build/Release", "Aptos.lib");
        //    string destinationDirectory = Target.ProjectFile.Directory.FullName;
        //    string destinationBinaryDirectory = destinationDirectory + "/Binaries/Win64";

        //    //File.Copy(AptosUiLogicPath, Path.Combine(destinationDirectory, "AptosUILogic.dll"), true);
        //    //File.Copy(AptosLibPath, Path.Combine(destinationDirectory, "Aptos.dll"), true);
        //    //File.Copy(AptosUiLogicPath, Path.Combine(destinationBinaryDirectory, "AptosUILogic.dll"), true);
        //    //File.Copy(AptosLibPath, Path.Combine(destinationBinaryDirectory, "Aptos.dll"), true);
        //    File.Copy(AptosUiLogic_LPath, Path.Combine(destinationDirectory, "AptosUILogic.lib"), true);
        //    File.Copy(AptosLib_LPath, Path.Combine(destinationDirectory, "Aptos.lib"), true);

        //    PublicIncludePaths.AddRange(new string[] { Path.Combine(ModuleDirectory, "../../../") });
        //    PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "Aptos.lib"));
        //    PublicAdditionalLibraries.Add(Path.Combine(destinationDirectory, "AptosUILogic.lib"));
        //    PublicDelayLoadDLLs.Add(Path.Combine(destinationDirectory, "Aptos.dll"));
        //    PublicDelayLoadDLLs.Add(Path.Combine(destinationDirectory, "AptosUILogic.dll"));

        //}
    }
}
