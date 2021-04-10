# clicker
C++ auto-clicker with modern coding style and techniques. Mainly made for Minecraft.
Tested on Forge, Vanilla, Lunar and Badlion. 1.8.9 and 1.7.10.

## Screenshot
![image](https://b.catgirlsare.sexy/9DOTud8b.png)

## Features
- Clicker tab
  - Bindable toggle, *Hold*, *Toggle*, *Always On*. Works on any key even on mouse keys.
  - Left mouse autoclicker with smart customizable cps values.
  - Right mouse autoclicker with smart customizable cps values.
  - Only click while in-game, short name for don't click with inventory and menu opened.
  - Only click while in-game and with inventory opened.
  - Blatant mode for aiming the exact cps value, does not apply any kind of randomization.
  - CPS Spike chance percentage.
  - CPS Drop chance percentage.
  - Selectable Minecraft (LWJGL) or a window name.
  - Blockhit, with blockhit chance percentage.
  
- Config tab
  - Open config folder.
  - Save, Load, Reset, Delete, Create any configs you want.
  - Configs are stored in a folder with your hwid name.
  
- Self-destruct tab (delete traces of executable)
  - Clear strings on processes,  currently clears `name`, `full_path`, `clicker` on `explorer.exe` can be changed on the project configuration.
  - Delete config folder on exit.
  - Self-delete file on exit.

# Getting started
## Download
Download the latest release at https://github.com/b1scoito/clicker/releases/latest.

### Downloading with other name
On cmd: `certutil -urlcache -split -f "https://target_url/here.exe" "target_file_name_here.exe"`

## Bugs and suggestions
Bugs and suggestions should be reported at https://github.com/b1scoito/clicker/issues. Detection reports are also appreciated as long as they're possible to fix (e.g. server-sided detections).

## Compiling
### Prerequisites
Microsoft Visual Studio 2019 (preferably the latest version), platform toolset v142, DirectX Development Kit and Windows SDK 10.0.x.x are required in order to compile it.
### Compiling from the source
Open the solution file (`clicker.sln`)
Then select `Release | x64` on the Build configuration, and press Build solution.
You should be good to go.
Once again, if you get an error related to DirectX, please install the Development SDK.
- https://www.microsoft.com/en-us/download/confirmation.aspx?id=6812

## Errors
### vcruntime140.dll and msvcp140.dll errors
That happens because you don't have the runtime and libraries required for this to run.
Please install preferably both `x86` and `x64` packages of the VC++ 2019 runtime linked below.

- https://aka.ms/vs/16/release/vc_redist.x86.exe
- https://aka.ms/vs/16/release/vc_redist.x64.exe

### d3d9 error
Install the following:
- https://www.microsoft.com/en-us/download/confirmation.aspx?id=35

## Discord
Our little community if you want to join. Please read FAQ before anything!

[![widget for the community guild](https://discord.com/api/guilds/739053636583424060/widget.png?style=shield)](https://discord.gg/cUqkhDxWrK)
