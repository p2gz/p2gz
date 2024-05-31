import glob
import os
import re
import shutil
import subprocess


DIRECTORY_PAIRS = [(os.path.join(os.getcwd(), 'pikmin2', 'include'), os.path.join(os.getcwd(), 'include')),
                   (os.path.join(os.getcwd(), 'pikmin2', 'src'), os.path.join(os.getcwd(), 'src')),
                   (os.path.join(os.getcwd(), 'root', 'files'), os.path.join(os.getcwd(), 'files'))]


def patch_directory(source_dir, patch_dir):
    patched_files = []

    for patch_root, _, files in os.walk(patch_dir):
        source_root = patch_root.replace(patch_dir, source_dir)

        for file in files:
            source_file = os.path.join(source_root, file)
            patch_file = os.path.join(patch_root, file)

            if os.path.exists(source_file):
                print(f'Replaced {source_file} with {patch_file}')
            else:
                print(f'Added {patch_file} to {source_root}')
            patched_files.append(shutil.copy2(patch_file, source_file)) 

    return patched_files


def main():
    try:
        iso = glob.glob(os.path.join(os.getcwd(), '*.iso'))[0]
    except IndexError:
        print('No .iso file found in the current directory')
        exit()

    if not os.path.exists(os.path.join(os.getcwd(), 'root')):
        subprocess.run(f'nodtool extract "{iso}" root', shell=True)

        for file in glob.glob(os.path.join('root', 'files', 'thp', '*.thp')):
            os.remove(file)
    
    patched_files = []
    for source_dir, patch_dir in DIRECTORY_PAIRS:
        patched_files.extend(patch_directory(source_dir, patch_dir))
    
    unlinked_src_files = []
    for decomp_src, _, files in os.walk(os.path.join(os.getcwd(), 'pikmin2', 'src')):
        makefile = os.path.join(decomp_src, 'Makefile')
        if len(files) and os.path.exists(makefile):
            unlinked_src_files.extend([os.path.join(decomp_src, f'{match.group(1)}.cpp')
                                       for line in open(makefile).readlines()
                                       if (match := re.search(r'asm/[^/]+/([^/]+)\.o', line))])
            
    for file in patched_files:
        if file in unlinked_src_files:
            print(f'WARNING: {file} is not linked')

    subprocess.run('python3 configure.py --no-check', cwd='pikmin2', shell=True)
    subprocess.run('ninja', cwd='pikmin2', shell=True)
    
    shutil.copy2('pikmin2/build/pikmin2.usa/main.dol', 'root/sys/main.dol')


if __name__ == '__main__':
    main()
