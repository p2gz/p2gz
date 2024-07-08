# P2GZ - Pikmin 2 Practice ROM

*Work in progress.*

## Development

### Building

Prerequisites:
1. ninja
1. [nodtool](https://github.com/AxioDL/nod)
1. [cube](https://github.com/mayabyte/cube)
1. python3
1. wine (if not on Windows)

Steps:
1. Clone this repository and run `git submodule update --init --recursive` to clone the decomp submodule
1. Place a USA Pikmin 2 ISO in the root folder of this repo
1. Run `python3 build.py`
1. Open `root/sys/main.dol` via Dolphin to see your changes

To modify code: 
- Copy any code files from the submodule/decomp to their corresponding path in `src/` to modify them and/or link them.
    - e.g. to modify the file `plugProjectKandoU/piki.cpp` from decomp, copy it to `src/plugProjectKandoU/piki.cpp`
    - NB: make sure any unlinked files are equivalent before copying them across as they will be linked on build.
- Any new files that don't exist in the original DOL need to go in `src/p2gz/` to be added to the build tree.

To modify assets:
- Extract any assets to an equivalent path in `files/` to match the archive's location in `root/files/`
    - To modify the game text archive in `root/files/message/mesRes_eng.szs`, extract it to `files/message/mesRes_eng/` using cube (`cube extract root/files/message/mesRes_eng.szs -o files/message/mesRes_eng/`). This will also extract `pikmin2.bmg` inside the SZS into `pikmin2.bmg.json`, unless you specify otherwise with cube flags.
    - Currently, only `.szs` archives (as folders) and `.bmg` text files (as `.json` files) will be repackaged on build, additional file types will be added later.