cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

set(CMAKE_C_COMPILER /usr/bin/clang-15)
set(CMAKE_CPP_COMPILER /usr/bin/clang-cpp-15)
set(CMAKE_CXX_COMPILER /usr/bin/clang++-15)
set(CMAKE_LINKER /usr/bin/ld.lld-15)

project(VulkanViewer CXX)


if(NOT CMAKE_PLATFORM_NAME)				# Set target architecture type if empty.
    set(CMAKE_PLATFORM_NAME "x64")
endif()
message("${CMAKE_PLATFORM_NAME} architecture in use")

if(NOT ("${CMAKE_PLATFORM_NAME}" STREQUAL "x64"
     OR "${CMAKE_PLATFORM_NAME}" STREQUAL "x86"))
    message(FATAL_ERROR "${CMAKE_PLATFORM_NAME} arch is not supported!")
endif()

set(CMAKE_CONFIGURATION_TYPES				# Global configuration types
    "Debug"
    "Release"
    CACHE STRING "" FORCE
)

include(CMake/Utils.cmake)				# Common utils

include(CMake/GlobalSettingsInclude.cmake OPTIONAL)	# Additional Global Settings(add specific info there)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)		# Use solution folders feature

set(PROJECT_NAME VulkanViewer)				# Sub-projects

set(EXECUTABLE_OUTPUT_PATH build)			# sub-directory destination for built binary


#################### Source groups ######################

AUX_SOURCE_DIRECTORY("src/" Platform_Linux)

AUX_SOURCE_DIRECTORY("../src/" VulkanViewer)
AUX_SOURCE_DIRECTORY("../src/Settings/" VulkanViewer_Settings)

AUX_SOURCE_DIRECTORY("../../shaders/" Graphics_Shaders)
AUX_SOURCE_DIRECTORY("../../testObjects/" Graphics_MeshObjects)

AUX_SOURCE_DIRECTORY("../../Model3D/" Model3D)
AUX_SOURCE_DIRECTORY("../../Model3D/External" Model3D_External)

AUX_SOURCE_DIRECTORY("../../gxEngine/" GraphicsEngine)
AUX_SOURCE_DIRECTORY("../../gxEngine/ControlSchemes" GraphicsEngine_ControlSchemes)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/" Platform)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/FileSystem/" Platform_FileSystem)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/GUISystem/stubs/" Platform_GUISystem)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/ImageHandling/" Platform_ImageHandling)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/Logger/" Platform_Logger)

set(Platform_OSAbstraction
    "../../../VulkanModule/Platform/OSAbstraction/PlatformSDL.cpp"
)
source_group("Platform\\OSAbstraction" FILES ${Platform_OSAbstraction})

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/ControlScheme/" Platform_ControlScheme)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Adjunct/" Vulkan_Adjunct)

set(Vulkan_Adjunct_Renderables
    "../../../VulkanModule/Adjunct/Renderables/AddOns.cpp"
    "../../../VulkanModule/Adjunct/Renderables/FixedRenderable.cpp"
)
source_group("Vulkan\\Adjunct\\Renderables" FILES ${Vulkan_Adjunct_Renderables})

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Adjunct/VertexTypes/" Vulkan_Adjunct_VertexTypes)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Assist/" Vulkan_Assist)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Objects/" Vulkan_Objects)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Setup/" Vulkan_Setup)

set(ALL_FILES
    ${VulkanViewer}
    ${VulkanViewer_Settings}
    ${Graphics_Shaders}
    ${Graphics_MeshObjects}
    ${Model3D}
    ${Model3D_External}
    ${GraphicsEngine}
    ${GraphicsEngine_ControlSchemes}
    ${Platform}
    ${Platform_FileSystem}
    ${Platform_GUISystem}
    ${Platform_ImageHandling}
    ${Platform_Logger}
    ${Platform_Linux}
    ${Platform_OSAbstraction}
    ${Platform_ControlScheme}
    ${Vulkan_Adjunct}
    ${Vulkan_Adjunct_Renderables}
    ${Vulkan_Adjunct_VertexTypes}
    ${Vulkan_Assist}
    ${Vulkan_Objects}
    ${Vulkan_Setup}
)

#########################################################


add_executable(${PROJECT_NAME} ${ALL_FILES})		# Target

set(ROOT_NAMESPACE VulkanViewer)


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Output directory
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/${CMAKE_PLATFORM_NAME}/$<CONFIG>/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/${CMAKE_PLATFORM_NAME}/$<CONFIG>/"
    )
endif()
if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        OUTPUT_DIRECTORY_DEBUG   "${CMAKE_SOURCE_DIR}/build/"
        OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/build/$<CONFIG>/"
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
elseif("${CMAKE_PLATFORM_NAME}" STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES
        INTERPROCEDURAL_OPTIMIZATION_RELEASE "TRUE"
    )
endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Include directories
    target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Third-Party/Include;"
        "/usr/include/SDL2;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2_image/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/Settings;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../testObjects;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../Model3D;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../Model3D/External;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../gxEngine;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../gxEngine/ControlSchemes;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Setup;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Assist;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Adjunct;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Adjunct/Renderables;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Adjunct/VertexTypes;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Objects;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform/OSAbstraction;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform/ImageHandling;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform/Logger;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform/FileSystem;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform/GUISystem;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform/GUISystem/stubs;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../../../VulkanModule/Platform/ControlScheme;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/imgui;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/imgui/examples;"
    )
endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Compile definitions
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "SDL_MAIN_HANDLED;"
        "_MBCS"
    )
elseif("${CMAKE_PLATFORM_NAME}" STREQUAL "x86")
    target_compile_definitions(${PROJECT_NAME} PRIVATE
        "_MBCS"
    )
endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Post build events
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND if [ ! -d "build" ]\; then mkdir build\; fi
        COMMAND if [ ! -d "build/compiledShaders" ]\; then mkdir "build/compiledShaders" \; cd "../../shaders" \; ./compileLinux.sh\; fi
        COMMAND if [ ! -L "build/textures" ]\; then cd build\; ln -s "../../../testAssets/textures" \; fi
        COMMAND if [ ! -L "build/models" ]\; then cd build\; ln -s "../../../testAssets/models" \; fi
        COMMENT "Post-build events complete."
    )
endif()


#if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Custom build events
#    add_custom_command(
#        OUTPUT "compiledShaders"
#        COMMAND cd ../../shaders
#        COMMAND compileLinux.sh
#        COMMENT "Shaders compiled."
#    )
#endif()


if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Dependencies
    set(ADDITIONAL_LIBRARY_DEPENDENCIES
        "vulkan;"
        "SDL2;"
        "SDL2_image;"
        "m;"
        "stdc++"
    )
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE "${ADDITIONAL_LIBRARY_DEPENDENCIES}")

if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")
    target_link_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Lib;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/VulkanSDK/Third-Party/Bin;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2/lib/${CMAKE_PLATFORM_NAME};"
        "${CMAKE_CURRENT_SOURCE_DIR}/../External/SDL2_image/lib/${CMAKE_PLATFORM_NAME}"
    )
endif()

#add_link_options("-mcmodel=medium")

target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            #/std:c++latest;
            #/sdl;
            #/W3;
            #-fPIC;
            -ggdb3
            -std=c++1z
            #^because^ otherwise:  -Wno-c++17-extensions
            -m64;
            -mcmodel=medium;
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            -Wno-vla;
            #-Wno-c99-designator;
            #-Wno-deprecated-volatile;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )

target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
                #-mcmodel=medium
            >
            #/DEBUG:FULL;
            -m64;
            -fPIE;
            #-mcmodel=medium
        )

# END OF FILE

