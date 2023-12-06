First, follow the instructions in `setupOrUpdateSDLsrc.sh` to grab SDL's source code and build it.
As it says, those builds may produce a strange ("i/O") error, but they'll still build.  You can execute that file as a script to produce the links it contains commands for.

Second and last, run `thenMakeTheFrameworks.sh`.  It may also produce what look like errors on the command line, but it should succeed in building the .XCFramework files.

Note: the .xcodeproj project files here reflect source code current as of:
 - SDL version 2.28.5
 - SDL_image version 2.8.0
