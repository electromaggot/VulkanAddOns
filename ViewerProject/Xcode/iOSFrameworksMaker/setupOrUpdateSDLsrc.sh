#!/bin/sh

# 0. Go to the directory that stores the SDL source code:
#
#    $ cd ~/VulkanAddOns/ViewerProject/external/SDLsrc
#
#    aka: ../../external/SDLsrc

SDL_SRC=../../external/SDLsrc

# 1. Download latest source code for SDL and SDL_image from:
#        https://github.com/libsdl-org/SDL/releases
#        https://github.com/libsdl-org/SDL_image/releases
# 2. $ tar -xvf SDL-release-*.tar.gz
#    $ tar -xvf SDL_image-release-*.tar.gz
#
# 3. Back in this directory, where this script is:

ln -sf $SDL_SRC/SDL-release-* SDL
ln -sf $SDL_SRC/SDL_image-release-* SDL_image

# 4. $ open iOSFrameworks/SDL-iOS.xcodeproj/
#        and build.
# 5. $ open iOSFrameworks/SDL_image-iOS.xcodeproj/
#        and build.
