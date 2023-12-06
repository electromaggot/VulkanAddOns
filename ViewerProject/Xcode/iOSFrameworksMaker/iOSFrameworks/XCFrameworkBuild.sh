#!/bin/sh
# credit to: https://stackoverflow.com/a/65613941
# Universal Script

set -e

FRAMEWORK_NAME="$1"
IOS_SCHEME_NAME="$1_singleFramework"

if [ -d "${SRCROOT}/build" ]; then
	rm -rf "${SRCROOT}/build"
fi

SIMULATOR_ARCHIVE_PATH="${SRCROOT}/build/${FRAMEWORK_NAME}-iphonesimulator.xcarchive"
DEVICE_ARCHIVE_PATH="${SRCROOT}/build/${FRAMEWORK_NAME}-iphoneos.xcarchive"

OUTPUT_DIR="${SRCROOT}/outFrameworksUniversal/"

# Simulator xcarchieve
xcodebuild archive \
  -scheme ${IOS_SCHEME_NAME} \
  -archivePath ${SIMULATOR_ARCHIVE_PATH} \
  -configuration ${CONFIGURATION} \
  -sdk iphonesimulator \
  -project ${FRAMEWORK_NAME}.xcodeproj \
  SKIP_INSTALL=NO

# Device xcarchieve
xcodebuild archive \
  -scheme ${IOS_SCHEME_NAME} \
  -archivePath ${DEVICE_ARCHIVE_PATH} \
  -sdk iphoneos \
  -configuration ${CONFIGURATION} \
  -project ${FRAMEWORK_NAME}.xcodeproj \
  SKIP_INSTALL=NO

# Clean up old output directory
rm -rf "${OUTPUT_DIR}/${FRAMEWORK_NAME}.xcframework"

# Create xcframwork combine of all frameworks
xcodebuild -create-xcframework \
  -framework ${SIMULATOR_ARCHIVE_PATH}/Products/Library/Frameworks/${FRAMEWORK_NAME}.framework \
  -framework ${DEVICE_ARCHIVE_PATH}/Products/Library/Frameworks/${FRAMEWORK_NAME}.framework \
  -output ${OUTPUT_DIR}/${FRAMEWORK_NAME}.xcframework

# Delete the most recent build.
if [ -d "${SRCROOT}/build" ]; then
	rm -rf "${SRCROOT}/build"
fi
