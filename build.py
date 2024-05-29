import os
import shutil
import subprocess

DECOMP_ROOT = os.path.join(os.getcwd(), 'pikmin2/src')
P2GZ_ROOT = os.path.join(os.getcwd(), 'src')

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

subprocess.run('python configure.py', cwd='pikmin2')
subprocess.run('ninja', cwd='pikmin2')