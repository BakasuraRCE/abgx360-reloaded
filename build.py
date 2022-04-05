import os
import ctypes
import shutil
import subprocess

if not ctypes.windll.shell32.IsUserAnAdmin():
    print('Not enough priviledge, restart as admin...')
    exit()
else:
    print('Elevated privilege acquired')

subprocess.run([
    'cmake', '-G', 'Visual Studio 17 2022', '-A', 'Win32',
    '-DCMAKE_TOOLCHAIN_FILE=./vcpkg/scripts/buildsystems/vcpkg.cmake',
    '-S', '.', '-B', 'build-Win32'
    ], shell=True, check=True)


subprocess.run([
    'cmake', '--build', 'build-Win32',
    '--config', 'Release'
    ], shell=True, check=True)