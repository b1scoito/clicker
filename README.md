# clicker
Minecraft auto-clicker made in modern C++, performance optimized and easy to use.  
Tested on Forge, Vanilla, Lunar and Badlion. 1.8.9 and 1.7.10.

![image](https://b.catgirlsare.sexy/I5f4OpCJ.png)

# Getting started

## Download
Download the latest release at the [releases section](https://github.com/b1scoito/clicker/releases/latest).

### Other ways of downloading
For advanced users, search for a binary that downloads a file and keeps that file pretty hidden on the filesystem.
- https://lolbas-project.github.io/#/download

## Bugs or suggestions
Bugs or suggestions should be reported at the [issues section](https://github.com/b1scoito/clicker/issues).

## Compiling
### Prerequisites
Microsoft Visual Studio (Preferably the latest version) with C++ installed.

### Compiling from the source
Open the solution file `clicker.sln`, then select `Release | x64` on the Build configuration and press Build solution.

### Compiling errors
If you get errors related to DirectX, please install the DirectX Software Development Kit.
- https://www.microsoft.com/en-us/download/confirmation.aspx?id=6812

## Errors
### vcruntime140.dll and msvcp140.dll errors
That happens because you don't have the runtime and libraries required for this to run.  
Please install preferably both `x86` and `x64` packages of the VC++ 2019 runtime linked below.
- https://aka.ms/vs/16/release/vc_redist.x86.exe
- https://aka.ms/vs/16/release/vc_redist.x64.exe

### d3d9.dll error
Install the following
- https://www.microsoft.com/en-us/download/confirmation.aspx?id=35

## Known issues
- The mouse hook is known to make the cursor teleport on Minecraft, this will be fixed on the next update alongside with other features like changing the way inputs are sent.

## Discord
Our little community if you want to join. Please read FAQ before anything!

[![widget for the community guild](https://discord.com/api/guilds/739053636583424060/widget.png?style=shield)](https://discord.gg/cUqkhDxWrK)
