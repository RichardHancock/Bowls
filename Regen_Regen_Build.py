__author__ = 'Richard Hancock'

import os

"""Horrible code done quickly"""


"""
This section would have been usefull but 32bit doesn't work anyway, so this is pointless (Left here just incase)
win32 = False
win64 = False

prefs = open("regen_build.prefs",'r+')
pref_lines = prefs.readlines()
if pref_lines[4] == "True":
    win32 = True
if pref_lines[5] == "True":
    win64 = True

prefs.close()


if win32 == False and win64 == False:
    print("Error no builds selected")
    input()
    sys.exit(-1)
"""

DEBUG = False
DEBUG_PATH = "C:/Users/Richard/Desktop/Bowls/Bowls/"


def find_argument_in_vcxproj(array, search_string):
    i = 0
    last_found = -1
    for current_line in array:
        if current_line.find(search_string) != -1:
            print("Found "+search_string+" on line "+str(i+1)+" Contains: "+current_line)
            last_found = i

        i += 1

    last_found += 2
    return last_found


def find_all_in_vcxproj(array, search_string):
    i = 0
    locations = []
    for current_line in array:
        if current_line.find(search_string) != -1:
            print("Found "+search_string+" on line "+str(i+1)+" Contains: "+current_line)
            if array.count(i) == 0:
                locations.append(i)

        i += 1

    return locations


def append_lib(lineToModify):
    pos = lineToModify.rfind(";")
    firstSec = lineToModify[:pos+1]
    secondSec = "Kinect10.lib;bass.lib;winmm.lib;" + lineToModify[pos+1:]
    result = firstSec + secondSec
    return result

def append_lib_path(lineToModify):
    pos = lineToModify.rfind("<")
    firstSec = lineToModify[:pos]
    secondSec = ";$(SolutionDir)\\Bowls\\game\\lib\\;" + lineToModify[pos:]
    result = firstSec + secondSec
    return result

def check_for_vcx_version(array):
    i = 0
    for e, path in array:
        if DEBUG:
            array[i][0] = os.path.exists(DEBUG_PATH + path)
        else:
            array[i][0] = os.path.exists(path)
        i += 1


vcx_paths = \
    [
        [False, "game/game.2010.vcxproj"],
        [False, "game/game.2012.vcxproj"],
        [False, "game/game.2013.vcxproj"]
    ]

"""
newLines32 = [
    "<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Release|Win32'">\n",
    "<IncludePath>$(VC_IncludePath);$(WindowsSDK_IncludePath);$(KINECTSDK10_DIR)\\inc;</IncludePath>\n",
    "<LibraryPath>$(VC_LibraryPath_x86);$(WindowsSDK_LibraryPath_x86);$(KINECTSDK10_DIR)\\lib\\x86;</LibraryPath>\n",
    "</PropertyGroup>\n",
    "<PropertyGroup Condition="'$(Configuration)|$(Platform)'=='Debug|Win32'">\n",
    "<IncludePath>$(VC_IncludePath);$(WindowsSDK_IncludePath);$(KINECTSDK10_DIR)\\inc;</IncludePath>\n",
    "<LibraryPath>$(VC_LibraryPath_x86);$(WindowsSDK_LibraryPath_x86);$(KINECTSDK10_DIR)\\lib\\x86;</LibraryPath>\n",
    "</PropertyGroup>\n"
]
"""
newLines64 = [
    "<PropertyGroup Condition=\"\'$(Configuration)|$(Platform)\'==\'Release|x64\'\">\n",
    "<IncludePath>$(VC_IncludePath);$(WindowsSDK_IncludePath);$(KINECTSDK10_DIR)\\inc;</IncludePath>\n",
    "<LibraryPath>$(VC_LibraryPath_x64);$(WindowsSDK_LibraryPath_x64);$(KINECTSDK10_DIR)\\lib\\amd64;</LibraryPath>\n",
    "</PropertyGroup>\n",
    "<PropertyGroup Condition=\"\'$(Configuration)|$(Platform)\'==\'Debug|x64\'\">\n",
    "<IncludePath>$(VC_IncludePath);$(WindowsSDK_IncludePath);$(KINECTSDK10_DIR)\\inc;</IncludePath>\n",
    "<LibraryPath>$(VC_LibraryPath_x64);$(WindowsSDK_LibraryPath_x64);$(KINECTSDK10_DIR)\\lib\\amd64;</LibraryPath>\n",
    "</PropertyGroup>\n"
]

vcIncPathsSearch = "$(UserRootDir)"
librarySearch = "/AdditionalDependencies"
libraryPathSearch = "/AdditionalLibraryDirectories"
check_for_vcx_version(vcx_paths)

for fileExists, filePath in vcx_paths:
    if fileExists:
        if DEBUG:
            full_path = DEBUG_PATH + filePath
            file = open(full_path, 'r+')
        else:
            file = open(filePath, 'r+')

        file.seek(0)
        lines = file.readlines()

        vcIncPathsIndex = find_argument_in_vcxproj(lines, vcIncPathsSearch)
        for newLine in newLines64:
            lines.insert(vcIncPathsIndex, newLine)
            vcIncPathsIndex += 1

        libraryIndexs = find_all_in_vcxproj(lines, librarySearch)
        for libraryIndex in libraryIndexs:
            lines[libraryIndex] = append_lib(lines[libraryIndex])

        libraryPathIndexes = find_all_in_vcxproj(lines, libraryPathSearch)
        for libraryPathIndex in libraryPathIndexes:
            lines[libraryPathIndex] = append_lib_path(lines[libraryPathIndex])

        """Overwrite original file"""
        file.seek(0)
        file.truncate()
        for line in lines:
            file.write(line)

        file.close()
