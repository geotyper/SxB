#!/bin/bash

cd "$(dirname "$0")"/../

if [ -d "Solution/macOS/ozz-animation-prefix/src/ozz-animation-build/ozz.xcodeproj" ] ; then
	open "Solution/macOS/ozz-animation-prefix/src/ozz-animation-build/ozz.xcodeproj"
fi

if [ -d "Solution/macOS/SFML-prefix/src/SFML-build/SFML.xcodeproj" ] ; then
	open "Solution/macOS/SFML-prefix/src/SFML-build/SFML.xcodeproj"
fi
