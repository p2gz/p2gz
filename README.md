# P2GZ - Pikmin 2 Practice ROM

*A work-in-progress practice tool for Pikmin 2 speedrunning, based on the Pikmin 2 decompilation project.*

NB: you will need to supply your own copy of a Pikmin 2 USA .iso (GPVE01)

Index
-----

- [Dependencies](#dependencies)
  - [Windows](#windows)
  - [macOS](#macos)
  - [Linux](#linux)
- [Building](#building)

Dependencies
------------

Pre-requisites:
1. python3
1. ninja
1. [nodtool](https://github.com/AxioDL/nod)
1. [cube](https://github.com/mayabyte/cube)
1. wine (if not on windows)

### Windows

On Windows, it's **highly recommended** to use native tooling. WSL or msys2 are **not** required.  

- Install [Python](https://www.python.org/downloads/) and add it to `%PATH%`.
  - Also available from the [Windows Store](https://apps.microsoft.com/store/detail/python-311/9NRWMJP3717K).
- Download [ninja](https://github.com/ninja-build/ninja/releases) and add it to `%PATH%`.
  - Quick install via pip: `pip install ninja`

### macOS

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages):

  ```sh
  brew install ninja
  ```

- Install [wine-crossover](https://github.com/Gcenx/homebrew-wine):

  ```sh
  brew install --cask --no-quarantine gcenx/wine/wine-crossover
  ```

After OS upgrades, if macOS complains about `Wine Crossover.app` being unverified, you can unquarantine it using:

```sh
sudo xattr -rd com.apple.quarantine '/Applications/Wine Crossover.app'
```

### Linux

- Install [ninja](https://github.com/ninja-build/ninja/wiki/Pre-built-Ninja-packages).
- For non-x86(_64) platforms: Install wine from your package manager.
  - For x86(_64), [wibo](https://github.com/decompals/wibo), a minimal 32-bit Windows binary wrapper, will be automatically downloaded and used.

Building
--------

- Clone the repository:

  ```sh
  git clone https://github.com/p2gz/p2gz.git
  ```

- Place your USA Pikmin 2 (GPVE01) .iso in the main directory.

- Run:
  ```sh
  python3 build.py
  ```

- Open `root/sys/main.dol` via Dolphin to see your changes.

  - Alternatively, add a shortcut to your Dolphin executable in the main directory as `Dolphin.exe.lnk` and run:
    ```sh
    python3 build.py --restart-dolphin
    ```

To add new source code files to the DOL:

- Uncomment the final `"lib": "moddingU"` bracket in `configure.py` - change the name to whatever you like, and feel free to add more than one new lib using this as a template.
- Add any new files along with their paths as `Object(Matching, folder/file.cpp)` where indicated.
- Within the `link_order_callback` function below the object configuration, uncomment and add each new file with its path within `src`, as indicated.

To add or replace compressed asset files (anything with a .szs extension):

- uncompresses your szs asset (preferabbly with cube)
- Save all of the uncompressed elements inside the directory `files/path/to/asset/`.
  - `asset/` is another directory that is the name of your asset, i.e. `sheargrub.szs` will become `files/path/to/sheargrub/`
- Add `files/path/to/asset` to the `P2GZ_CUSTOM_ASSETS_COMPRESSED` array in `build.py`.

To add or replace uncompressed asset files:

- Save the new file(s) in `files/path/to/asset`.
- Add `files/path/to/asset` to the `P2GZ_CUSTOM_ASSETS_UNCOMPRESSED` array in `build.py`.

Once built, the new DOL will exist at `root/sys/main.dol`, along with a Dolphin-readable directory of all game files.
