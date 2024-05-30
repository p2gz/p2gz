import glob
import os
import requests
import shutil
import subprocess
import zipfile
from io import BytesIO

DECOMP_ROOT = os.path.join(os.getcwd(), 'pikmin2/src')
P2GZ_ROOT = os.path.join(os.getcwd(), 'src')

try:
    iso = glob.glob(os.path.join(os.getcwd(), '*.iso'))[0]
except IndexError:
    print('No .iso file found in the current directory')
    exit()

for p2gz_root, _, _ in os.walk(P2GZ_ROOT):
    decomp_root = p2gz_root.replace(P2GZ_ROOT, DECOMP_ROOT)
    if not os.path.exists(decomp_root):
        print(f'Extraneous directory in p2gz: {decomp_root} does not exist in decomp')
        exit()

for decomp_root, _, _ in os.walk(DECOMP_ROOT):
    p2gz_root = decomp_root.replace(DECOMP_ROOT, P2GZ_ROOT)
    if not os.path.exists(p2gz_root):
        print(f'Directory missing in p2gz: {p2gz_root} does not exist in p2gz')
        exit()

for decomp_root, _, files in os.walk(DECOMP_ROOT):
    p2gz_root = decomp_root.replace(DECOMP_ROOT, P2GZ_ROOT)
    
    for file in files:
        decomp_file = os.path.join(decomp_root, file)
        p2gz_file = os.path.join(p2gz_root, file)
        
        if os.path.exists(p2gz_file):
            shutil.copy2(p2gz_file, decomp_file)
            print(f'Replaced {decomp_file} with {p2gz_file}')

if not os.path.exists(os.path.join(os.getcwd(), 'ninja.exe')):
    ninja_zip = requests.get('https://github.com/ninja-build/ninja/releases/download/v1.12.1/ninja-win.zip').content
    with zipfile.ZipFile(BytesIO(ninja_zip)) as f:
        f.extractall()

subprocess.run('python configure.py --no-check', cwd='pikmin2')
subprocess.run('ninja', cwd='pikmin2')

if not os.path.exists(os.path.join(os.getcwd(), 'nodtool.exe')):
    nodtool = requests.get('https://github.com/AxioDL/nod/releases/download/v1.0/nodtool.v1.win64.exe')
    with open('nodtool.exe', 'wb') as f:
        f.write(nodtool.content)

if not os.path.exists(os.path.join(os.getcwd(), 'root')):
    subprocess.run(f'nodtool extract "{iso}" root')
    
shutil.copy2('pikmin2/build/pikmin2.usa/main.dol', 'root/sys/main.dol')