On iOS, this project uses **XCFramework**s for both the [SDL](https://github.com/libsdl-org/SDL/releases) and [SDL_image](https://github.com/libsdl-org/SDL_image/releases) libraries, which serve as an "abstraction layer" to the OS and help this project to be multiplatform.

An [XCFramework](https://developer.apple.com/documentation/xcode/creating-a-multi-platform-binary-framework-bundle) is Apple's modern equivalent to what used to be called a "fat framework."  It conveniently wraps various configurations (debug, release) and platforms (iphoneos, iphonesimulator, ipados, etc.) into a single Framework.  While separate libraries for debug/release are a hassle-but-manageable, throw in platforms like simulator (x86 code vs. arm) and it becomes unmanageable.  Unfortunately SDL doesn't distribute binaries for or build its libraries into the XCFramework format, so we have to.  But once you do, it's really nice!

**Building `SDL-iOS.xcframework` and `SDL_image-iOS.xcframework`**

First, follow the instructions in `setupOrUpdateSDLsrc.sh` to grab SDL's source code and build it.  You can execute that file as a script to produce the links it contains commands for.\
Each project has two targets/schemes: `SDL`[`_image`]`-iOS_singleFramework` and `Build_universal_XCFramework`.  Both should build -- the first without error, but note that the XCFramework scheme does show an issue (*error: accessing build database ".../XCBuildData/build.db": disk I/O error*) BUT despite the "Build failed" ending, they still successfully build binaries.

Second and last, run `thenMakeTheFrameworks.sh`...  It may also produce what look like errors* on the command line, but should succeed in finalizing the `.XCFramework` files.  For convenience, this script creates symbolic links to those frameworks in this same directory.

Note: the `.xcodeproj` project files here reflect source code current as of:
 - SDL version 2.28.5
 - SDL_image version 2.8.0

##
\* &ndash; For instance you may see an error indicating something like:<br>
`xcodebuild[84286:16634324] Writing error result bundle`<br>
`    to /var/folders/q8/9lb5rkzj1kd2_cr3w9bs9wl40000gn/T/ResultBundle_2023-07-12_16-06-0002.xcresult`<br>
`xcodebuild: error: Unknown build action 'iphonesimulator'.`<br>
This can occur if you don't have the exact version of the SDK or Simulator installed matching the release target you are trying to deploy to.  This depends, in large part, on what version of Xcode you are running, when you updated to it, and which Xcode updates you may have missed in-between.<br>
The XCFramework will still be built, but as indicated above, won't include support for that particular 'iphonesimulator' piece.
You can fix this by explicitly locating/downloading/installing the SDK version in question.
