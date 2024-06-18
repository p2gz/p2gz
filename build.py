import argparse
import glob
import json
import os
import platform
import shutil
import subprocess
import time


DECOMP_ROOT = os.path.join(os.getcwd(), 'pikmin2')
DECOMP_INCLUDE = os.path.join(os.getcwd(), 'pikmin2', 'include')
P2GZ_INCLUDE = os.path.join(os.getcwd(), 'include')
DECOMP_SRC = os.path.join(os.getcwd(), 'pikmin2', 'src')
P2GZ_SRC = os.path.join(os.getcwd(), 'src')
ISO_ASSETS = os.path.join(os.getcwd(), 'root', 'files')
P2GZ_ASSETS = os.path.join(os.getcwd(), 'files')
DOL_PATH = os.path.join(os.getcwd(), 'root', 'sys', 'main.dol')

P2GZ_CUSTOM_ASSETS = [
    os.path.join(P2GZ_ASSETS, 'new_screen', 'eng', 'res_s_menu_squad')
]

parser = argparse.ArgumentParser()
parser.add_argument('--clean', '-c', action='store_true', help='Build from a clean working directory')
parser.add_argument('--restart-dolphin', '-rd', action='store_true', help='Restart Dolphin with root/sys/main.dol after build')
args = parser.parse_args()

if args.restart_dolphin:
    if platform.system() == 'Windows':
        subprocess.run('taskkill /IM dolphin.exe /F', shell=True)
    else:
        subprocess.run('pkill -f dolphin-emu', shell=True)

if args.clean:
    shutil.rmtree(os.path.join(os.getcwd(), 'root'))
    subprocess.run('git clean -fdx && git reset --hard', cwd=DECOMP_ROOT, shell=True)

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

    for file in glob.glob(os.path.join(ISO_ASSETS, 'thp', '*.thp')):
        os.remove(file)

# patch headers
for p2gz_path, _, files in os.walk(P2GZ_INCLUDE):
    decomp_path = p2gz_path.replace(P2GZ_INCLUDE, DECOMP_INCLUDE)

    for file in files:
        decomp_header = os.path.join(decomp_path, file)
        p2gz_header = os.path.join(p2gz_path, file)

        if os.path.exists(decomp_header):
            print(f'Replaced {decomp_header} with {p2gz_header}')
        else:
            print(f'Added {p2gz_header} to {decomp_path}')
        shutil.copy2(p2gz_header, decomp_header)

libs = json.load(open(os.path.join(DECOMP_ROOT, 'libs.json')))

# patch cpp files
for p2gz_path, _, files in os.walk(P2GZ_SRC):
    decomp_path = p2gz_path.replace(P2GZ_SRC, DECOMP_SRC)

    for file in files:
        decomp_cpp = os.path.join(decomp_path, file)
        p2gz_cpp = os.path.join(p2gz_path, file)
        filename = os.path.splitext(file)[0]
        
        if os.path.exists(decomp_cpp) and not decomp_path.endswith('p2gz'):
            print(f'Replaced {decomp_cpp} with {p2gz_cpp}')
        elif decomp_path.endswith('p2gz'):
            print(f'Added {p2gz_cpp} to {decomp_path}')
            if libs[-1]['lib'] != 'p2gz':
                libs.append({
                    'lib': 'p2gz',
                    'cflags': '$cflags_pikmin',
                    'mw_version': '2.6',
                    'host': True,
                    'objects': []
                })
            if not any([filename in object[0] for object in libs[-1]['objects']]):
                libs[-1]['objects'].append([f'p2gz/{filename}', True])
            os.makedirs(os.path.dirname(decomp_cpp), exist_ok=True)
        else:
            print(f'ERROR: Extraneous file in {decomp_path}: {file}')
            exit()

        shutil.copy2(p2gz_cpp, decomp_cpp)
        
        for lib in libs:
            for object in lib['objects']:
                if object[0].endswith(filename):
                    if not object[1]:
                        print(f'WARNING: {decomp_cpp} is not linked')
                        object[1] = True
                    break

with open(os.path.join(DECOMP_ROOT, 'libs.json'), 'w') as f:
    f.write(json.dumps(libs, indent=2) + '\n')

# patch assets
for p2gz_path, dirs, _ in os.walk(P2GZ_ASSETS):
    iso_path = p2gz_path.replace(P2GZ_ASSETS, ISO_ASSETS)

    for dir in dirs:
        patch_dir = os.path.join(p2gz_path, dir)
        iso_dir = os.path.join(iso_path, dir)
        archive = iso_dir + '.szs'

        # patching existing asset
        if os.path.exists(archive):
            existing_directories = set(next(os.walk(iso_path))[1])
            subprocess.run(f'ArcExtract {archive}', shell=True, stdout=open(os.devnull, 'w'))
            extracted_archive = os.path.join(iso_path, (set(next(os.walk(iso_path))[1]) - existing_directories).pop())

            print(f'Copying {patch_dir} to {extracted_archive}')
            shutil.copytree(patch_dir, extracted_archive, dirs_exist_ok=True,
                            ignore=lambda _, contents: [file for file in contents if file.endswith('json')])
            
            for root, _, files in os.walk(patch_dir):
                for file in files:
                    if file.endswith('json'):
                        subprocess.run(f'python3 pikminBMGtool.py PACK {os.path.join(patch_dir, file)} \
                                       {os.path.join(extracted_archive, file.replace("json", "bmg"))}', shell=True, stdout=open(os.devnull, 'w'))
            
            subprocess.run(f'ArcPack {extracted_archive}', shell=True, stdout=open(os.devnull, 'w'))
            os.remove(archive)
            os.rename(f'{extracted_archive}.arc', archive)
            shutil.rmtree(extracted_archive)
        
        # adding custom asset
        elif patch_dir in P2GZ_CUSTOM_ASSETS:
            print(f'Copying {patch_dir} to {iso_dir}')
            shutil.copytree(patch_dir, iso_dir, dirs_exist_ok=True,
                            ignore=lambda _, contents: [file for file in contents if file.endswith('json')])
            
            for root, _, files in os.walk(patch_dir):
                for file in files:
                    if file.endswith('json'):
                        subprocess.run(f'python3 pikminBMGtool.py PACK {os.path.join(patch_dir, file)} \
                                       {os.path.join(extracted_archive, file.replace("json", "bmg"))}', shell=True, stdout=open(os.devnull, 'w'))
            
            subprocess.run(f'ArcPack {iso_dir}', shell=True, stdout=open(os.devnull, 'w'))
            shutil.rmtree(iso_dir)
            if os.path.exists(f'{iso_dir}.szs'):
                os.remove(f'{iso_dir}.szs')
            os.rename(f'{iso_dir}.arc', f'{iso_dir}.szs')
            

# patch dol
subprocess.run('python3 configure.py --no-check', cwd=DECOMP_ROOT, shell=True)
subprocess.run('ninja', cwd=DECOMP_ROOT, shell=True)
shutil.copy2('pikmin2/build/pikmin2.usa/main.dol', 'root/sys/main.dol')

print(f'Done! Build took {round(time.time() - start_time, 2)}s')

if args.restart_dolphin:
    subprocess.Popen(f'Dolphin.exe.lnk --exec {DOL_PATH}', shell=True)
