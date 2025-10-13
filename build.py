import argparse
import glob
import os
import platform
import shutil
import subprocess
import time
import itertools

# set paths
DOL_PATH = os.path.join(os.getcwd(), 'root', 'sys', 'main.dol')
P2GZ_ASSETS = os.path.join(os.getcwd(), 'assets')
NEW_ISO_ASSETS = os.path.join(os.getcwd(), 'root')


# Any asset that is a compressed .szs file; this will point to the UNCOMPRESSED directory
# i.e. (P2GZ_ASSETS, 'files', 'memoryCard', 'memoryCardHeader') -> files\memoryCard\memoryCardHeader\ directory
# contents within this directory will all be compressed into memoryCardHeader.szs at build time
P2GZ_CUSTOM_ASSETS_COMPRESSED = [
    os.path.join(P2GZ_ASSETS, 'files', 'memoryCard', 'memoryCardHeader')
]

# Any asset that is NOT compressed - this is just the file path to the file itself to be replaced
P2GZ_CUSTOM_ASSETS_UNCOMPRESSED = [
    os.path.join(P2GZ_ASSETS, 'files', 'opening.bnr'),
    os.path.join(P2GZ_ASSETS, 'sys', 'boot.bin'),
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
    for file in glob.glob(os.path.join(NEW_ISO_ASSETS, 'files', 'thp', '*.thp')):
        os.remove(file)

# patch compressed assets (anything with a .szs file type)
for compressed_dir in P2GZ_CUSTOM_ASSETS_COMPRESSED:
    # Creates a directory for the szs file - anything in the directory are the szs file's uncompressed contents
    iso_dir = compressed_dir.replace(P2GZ_ASSETS, NEW_ISO_ASSETS)
    iso_archive = iso_dir + '.szs'
    asset_archive = compressed_dir + '.szs'
    

    # patching existing asset
    if os.path.exists(iso_archive):
        subprocess.run(f'cube extract {iso_archive} -o {iso_dir}', shell=True)

        print(f'Copying {asset_archive} to {iso_archive}')
        shutil.copytree(compressed_dir, iso_dir, dirs_exist_ok=True)

        subprocess.run(f'cube pack -d --arc-extension szs {iso_dir}', shell=True)

    # adding custom asset
    else:
        print(f'Copying {asset_archive} to {iso_archive}')
        shutil.copytree(compressed_dir, iso_dir, dirs_exist_ok=True)

        subprocess.run(f'cube pack -d --arc-extension szs {iso_dir}', shell=True)

# patch non-compressed assets
for path in P2GZ_CUSTOM_ASSETS_UNCOMPRESSED:
    iso_path = path.replace(P2GZ_ASSETS, NEW_ISO_ASSETS)

    # Copy existing uncompressed file
    if os.path.exists(iso_path):
        print(f'Copying {path} to {iso_path}')
        shutil.copy(path, iso_path)

    # Add new directory + file
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
