# DivaVersion

Simple fix for [Hatsune Miku: Project DIVA Mega Mix+](https://store.steampowered.com/app/1761390/Hatsune_Miku_Project_DIVA_Mega_Mix/) Windows version check.

## Method

Hijacking `userenv.dll` and install a hook to `RtlGetVersion` then modify `lpVersionInformation->dwMajorVersion` to `10`.

## Buidling

Use Visual Studio 2022 to build the project. Visual Studio 2019 can also be used, however you will have to configure the project.

After building, `userenv.dll` will be available at `$(SolutionDir)$(Platform)\$(Configuration)`. Example: `SOLUTION_DIR\x64\Release\userenv.dll`

## Installing

Copy `userenv.dll` to Project DIVA game folder (the same folder with `DivaMegaMix.exe`)

## License

This software is licensed under the MIT License. See `LICENSE.txt` for license information.

This software uses [Microsoft Detours](https://github.com/microsoft/detours). See `Detours\LICENSE.md` for license information.
