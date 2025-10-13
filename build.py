import argparse
import glob
import os
import platform
import shutil
import subprocess
import time
import itertools

# set paths
ISO_ASSETS = os.path.join(os.getcwd(), 'root', 'files')
ISO_SYS_ASSETS = os.path.join(os.getcwd(), 'root', 'sys')
P2GZ_ASSETS = os.path.join(os.getcwd(), 'files')
P2GZ_SYS_ASSETS = os.path.join(os.getcwd(), 'files', 'sys')
DOL_PATH = os.path.join(os.getcwd(), 'root', 'sys', 'main.dol')

# add any new assets (files, folders, etc.) here as required
P2GZ_CUSTOM_ASSETS = [
    os.path.join(P2GZ_ASSETS, 'test_dir', 'sheargrub.png'),
    os.path.join(P2GZ_ASSETS, 'memoryCard', 'memoryCardHeader.szs'),
    os.path.join(P2GZ_ASSETS, 'opening.bnr'),
    os.path.join(P2GZ_SYS_ASSETS, 'boot.bin')
]

# argument parsing
parser = argparse.ArgumentParser()
parser.add_argument('--clean', '-c', action='store_true', help='Build from a clean working directory')
parser.add_argument('--restart-dolphin', '-rd', action='store_true', help='Restart Dolphin with root/sys/main.dol after build')
parser.add_argument('--map', '-m', action='store_true', help='Compile a map file for easier debugging')
args = parser.parse_args()

if args.restart_dolphin:
    if platform.system() == 'Windows':
        subprocess.run('taskkill /IM dolphin.exe /F', shell=True)
    else:
        subprocess.run('pkill -f dolphin-emu', shell=True)

if args.clean:
    shutil.rmtree(os.path.join(os.getcwd(), 'root'))
    subprocess.run('ninja -t clean', shell=True)

start_time = time.time()

# extract iso
try:
    iso = glob.glob(os.path.join(os.getcwd(), '*.iso'))[0]
except IndexError:
    print('ERROR: No .iso file found in the current directory')
    exit()

if not os.path.exists(os.path.join(os.getcwd(), 'root')):
    print(f'Extracting {iso}')
    subprocess.run(f'nodtool extract "{iso}" root', shell=True)

    # add extracted dol to correct directory so dtk can find it
    shutil.copy2('root/sys/main.dol', 'orig/GPVE01/sys/main.dol')

    # remove .thp intro videos to save space
    for file in glob.glob(os.path.join(ISO_ASSETS, 'thp', '*.thp')):
        os.remove(file)

# patch assets into new extracted game folders
for path in P2GZ_CUSTOM_ASSETS:
    iso_path = path.replace(P2GZ_ASSETS, ISO_ASSETS)
    print(f'{iso_path}')

    # Copy existing system files
    if path.find("sys\\") > -1:
        iso_path = path.replace(P2GZ_SYS_ASSETS, ISO_SYS_ASSETS)
        # We should never be adding new files to sys dir, so we can assume we are replacing
        print(f'Copying {path} to {iso_path}')
        shutil.copy(path, iso_path)

    # Copy existing standard files
    elif os.path.exists(iso_path):
        print(f'Copying {path} to {iso_path}')
        shutil.copy(path, iso_path)

    # Add new files + directories
    else:
        path_dir = os.path.dirname(path)
        iso_new_dir = os.path.dirname(iso_path)
        print(f'Creating new directory {path_dir} in {iso_new_dir}')
        shutil.copytree(path_dir, iso_new_dir)
        print(f'Adding {path} to {iso_path}')
        shutil.copy(path, iso_path)


# patch dol
config_cmd = 'python3 configure.py --non-matching'
if args.map:
    config_cmd += ' --map'

subprocess.run(config_cmd, shell=True)

subprocess.run('ninja', shell=True)
shutil.copy2('build/GPVE01/main.dol', 'root/sys/main.dol')

if args.map:
    subprocess.run('rm root/files/pikmin2UP.map')
    shutil.copy2('build/GPVE01/main.elf.MAP', 'root/files/pikmin2UP.map')
    lines = []
    with open('root/files/pikmin2UP.map', 'r') as f:
        lines = itertools.dropwhile(lambda line: not line.startswith('.init section layout'), f.readlines())
    with open('root/files/pikmin2UP.map', 'w') as f:
        f.writelines(lines)


print(f'Done! Build took {round(time.time() - start_time, 2)}s')

if args.restart_dolphin:
    subprocess.Popen(f'Dolphin.exe.lnk --exec {DOL_PATH}', shell=True)
