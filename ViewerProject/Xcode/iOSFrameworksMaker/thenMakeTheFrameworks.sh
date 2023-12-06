#!/bin/sh

cd iOSFrameworks

./XCFrameworkBuild.sh SDL

./XCFrameworkBuild.sh SDL_image

cd ..

ln -sf iOSFrameworks/outFrameworksUniversal/SDL-iOS.xcframework
ln -sf iOSFrameworks/outFrameworksUniversal/SDL_image-iOS.xcframework
