import glob
import os
import re
import shutil
import subprocess

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
    
    makefile = os.path.join(decomp_root, 'Makefile')
    if len(files) and os.path.exists(makefile):
        unlinked_files = [os.path.join(decomp_root, f'{match.group(1)}.cpp')
                          for line in open(makefile).readlines()
                          if (match := re.search(r'asm/[^/]+/([^/]+)\.o', line))]
    
    for file in files:
        decomp_file = os.path.join(decomp_root, file)
        p2gz_file = os.path.join(p2gz_root, file)
        
        if os.path.exists(p2gz_file):
            if unlinked_files and decomp_file in unlinked_files:
                print(f'WARNING: {decomp_file} is not linked')
            shutil.copy2(p2gz_file, decomp_file)
            print(f'Replaced {decomp_file} with {p2gz_file}')

subprocess.run('python3 configure.py --no-check', cwd='pikmin2', shell=True)
subprocess.run('ninja', cwd='pikmin2', shell=True)

if not os.path.exists(os.path.join(os.getcwd(), 'root')):
    subprocess.run(f'nodtool extract "{iso}" root', shell=True)

    for file in glob.glob(os.path.join('pikmin2/root/files/thp', '*.thp')):
        os.remove(file)

shutil.copy2('pikmin2/build/pikmin2.usa/main.dol', 'root/sys/main.dol')