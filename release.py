import subprocess
import shutil
import os

version = "NO_VERSION"
with open("include/p2gz/p2gz.h", "r") as f:
    for line in f.readlines():
        if line.startswith("#define P2GZ_VERSION"):
            version = (line.split(" ")[-1]).strip().strip('\"')
            break
    assert("NO VERSION FOUND IN p2gz.h")

print(f"VERSION: {version}")
with open("release_assets/version.txt", "w+") as f:
    f.write(version)

print("Building P2GZ...")
subprocess.run("python3 build.py --clean --map", shell=True)

if os.path.exists(f"p2gz-{version}.zip"):
    os.remove(f"p2gz-{version}.zip")

if os.path.exists("release"):
    shutil.rmtree("release")
os.makedirs("release/Patch")

for file in os.scandir("release_assets"):
    print(f"copying {file.path} to release/{file.name}")
    shutil.copy2(file.path, f"release/{file.name}")

files_to_copy = [
    "root/sys/main.dol",
    "root/sys/boot.bin",
    "root/files/pikmin2UP.map",
    "root/files/opening.bnr",
    "root/files/banner",
    "root/files/memoryCard/memoryCardHeader.szs",
    "root/files/menu_images.szs",
    "root/files/user/Ebisawa/title/title.szs",
]

for file_path in files_to_copy:
    dst_path = f"release/Patch/{file_path}"
    print(f"copying {file_path} to {dst_path}")
    os.makedirs("/".join(dst_path.split('/')[:-1]), exist_ok=True)
    if '.' in file_path:
        shutil.copy2(file_path, dst_path)
    else:
        shutil.copytree(file_path, dst_path)

print("Compressing...")
shutil.make_archive(f"p2gz-{version}", "zip", "release")

print("Done!")