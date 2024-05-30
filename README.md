# P2GZ - Pikmin 2 Practice ROM

*Work in progress.*

## Development

### Building

Prerequisites:
1. ninja
1. nodtool
1. python3

Steps:
1. Clone this repository and run `git submodule update --init --recursive` to clone the decomp submodule
1. Place a USA Pikmin 2 ISO in the root folder of this repo
1. Run `python3 build.py`
1. Open `root/sys/main.dol` via Dolphin to see your changes