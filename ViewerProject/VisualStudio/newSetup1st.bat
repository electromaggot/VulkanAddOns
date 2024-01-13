REM
REM Simpler version of Setup1st.bat
REM
REM Download latest:  VulkanSDK
REM and VC dirs for:  SDL2
REM             and:  SDL2_image
REM
REM  into directory:  ..\External
REM      (see below)
REM
REM Then run this script.
REM
REM 11 Jan 2024
REM

cd ..\External

REM remove existing, if need update
REM -------------------------------

rmdir VulkanSDK
rmdir /S /Q SDL
rmdir SDL_image

REM re-establish links
REM ------------------
REM (note that the below version numbers will change!)

mklink /J VulkanSDK VulkanSDK-1.3.268.0

mklink /J SDL SDL2-2.28.5

mklink /J SDL_image SDL2_image-2.8.2
