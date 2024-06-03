# P2GZ - Pikmin 2 Practice ROM

*Work in progress.*

## Development

### Building

Prerequisites:
1. [Lunaboy's RARC Tools](https://www.romhacking.net/utilities/1024/)
1. ninja
1. [nodtool](https://github.com/AxioDL/nod)
1. [pikminBMG](https://github.com/RenolY2/pikminBMG)
1. python3
1. wine (if not on Windows)

Steps:
1. Clone this repository and run `git submodule update --init --recursive` to clone the decomp submodule
1. Place a USA Pikmin 2 ISO in the root folder of this repo
1. Run `python3 build.py`
1. Open `root/sys/main.dol` via Dolphin to see your changes