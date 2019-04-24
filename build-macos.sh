
cmakeImpl() {
	cmake -E make_directory "build-macos" && cmake -E chdir "build-macos" cmake -G "Xcode" ../ -DTHE_PROJECT_PREFIX="$1"
}

if [ "$1" == "" ] ; then
    cmakeImpl "SXB"
else
    cmakeImpl "$1"
fi

open ./build-macos/*.xcodeproj 2>/dev/null
