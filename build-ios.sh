
cmakeImpl() {
	cmake -E make_directory "build-ios" && cmake -E chdir "build-ios" cmake -G "Xcode" ../ -DCMAKE_TOOLCHAIN_FILE=./BuildScript/cmake/ios-toolchain/ios.toolchain.cmake -DPLATFORM=OS64COMBINED -DTHE_PROJECT_PREFIX="$1"
}

if [ "$1" == "" ] ; then
	cmakeImpl "SXB"
else
	cmakeImpl "$1"
fi

open ./build-ios/*.xcodeproj 2>/dev/null
