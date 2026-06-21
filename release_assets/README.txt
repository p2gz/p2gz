==== P2GZ Installation ====

P2GZ is an advanced practice ROM for Pikmin 2. All code is based on the Pikmin 2 Decompilation Project
and is open source at https://github.com/p2gz/p2gz.

You will need a legally obtained GameCube USA Pikmin 2 game ISO to build P2GZ. CISO and GCT files should also work.
The P2GZ authors cannot provide this - you must source it yourself. Other game versions (Wii/Switch)
are not supported. ISOs from other game regions (NTSC-J/PAL) will not work.

Install process:
1. Copy your Pikmin 2 ISO into the same folder as this README.
2.
    On Windows: in the file explorer, drag the game ISO onto the file `patch.bat`.
    On MacOS/Linux: open a terminal in this folder and run `./patch.sh <YourPikmin2.iso>` or
      `./patch_macOS.sh <YourPikmin2.iso>` as appropriate. If you see a "permission denied" error, run
      `chmod +x patch.sh nodtool.linux` or `chmod +x patch_macOS.sh nodtool.macos` as appropriate and try again.
3. A console window should open and begin patching the ISO.
4. The created P2GZ ISO will be placed in the same folder. Enjoy!