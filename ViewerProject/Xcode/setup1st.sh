#!/bin/bash

#
# Run this script once after you first clone the project to create
#  directories and symbolic links that the build process expects.
#
# IMPORTANT!: Change EXTERNAL_LIBRARY_PATH below to match where your VulkanSDK and
#  SDL directories reside, relative to the 'external' directory in project root.
#

EXTERNAL_LIBRARY_PATH=../../../3rdParty

EXTERNAL_DIRECTORY_NAME=external

if [ ! -d "$EXTERNAL_LIBRARY_PATH" ]; then
  echo "Warning: directory '$EXTERNAL_LIBRARY_PATH' not found."
  echo "You must set EXTERNAL_LIBRARY_PATH, by editing it inside this script,"
  echo " to the location of your VulkanSDK and SDL directories; assuming"
  echo " you installed them into a common parent directory."
  exit
fi

echo "Creating '$EXTERNAL_DIRECTORY_NAME' directory with LINKS to Frameworks..."

if [ ! -d ../$EXTERNAL_DIRECTORY_NAME ]; then
  mkdir ../$EXTERNAL_DIRECTORY_NAME
fi

cd ../$EXTERNAL_DIRECTORY_NAME

ln -sfv "$EXTERNAL_LIBRARY_PATH/VulkanSDK"

ln -sfv "$EXTERNAL_LIBRARY_PATH/SDL/SDL"

ln -sfv "$EXTERNAL_LIBRARY_PATH/SDL/SDL_image"

ln -sfv "$EXTERNAL_LIBRARY_PATH/OpenGL/glm-0.9.9.8" ./glm

echo "Done."

