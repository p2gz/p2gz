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
P2GZ_ASSETS = os.path.join(os.getcwd(), 'files')
DOL_PATH = os.path.join(os.getcwd(), 'root', 'sys', 'main.dol')

# add new asset folders here as required
P2GZ_CUSTOM_ASSETS = [
    os.path.join(P2GZ_ASSETS, 'new_screen', 'eng', 'res_s_menu_squad'),
    os.path.join(P2GZ_ASSETS, 'new_screen', 'eng', 'res_s_menu_warp'),
    os.path.join(P2GZ_ASSETS, 'new_screen', 'eng', 'hole_in')
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

# patch assets
for p2gz_path, dirs, _ in os.walk(P2GZ_ASSETS):
    iso_path = p2gz_path.replace(P2GZ_ASSETS, ISO_ASSETS)

    for dir in dirs:
        patch_dir = os.path.join(p2gz_path, dir)
        iso_dir = os.path.join(iso_path, dir)
        archive = iso_dir + '.szs'

        # patching existing asset
        if os.path.exists(archive):
            subprocess.run(f'cube extract {archive} -o {iso_dir}', shell=True)

            print(f'Copying {patch_dir} to {iso_dir}')
            shutil.copytree(patch_dir, iso_dir, dirs_exist_ok=True)

            subprocess.run(f'cube pack -d --arc-extension szs {iso_dir}', shell=True)

        # adding custom asset
        elif patch_dir in P2GZ_CUSTOM_ASSETS:
            print(f'Copying {patch_dir} to {iso_dir}')
            shutil.copytree(patch_dir, iso_dir, dirs_exist_ok=True)

            subprocess.run(f'cube pack -d --arc-extension szs {iso_dir}', shell=True)


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
