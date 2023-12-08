cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

project(HelloVulkanSDL CXX)


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

set(VulkanViewer
    "../src/*.cpp"
    "../src/*.h"
)
source_group("VulkanViewer" FILES ${VulkanViewer})

set(VulkanViewer__Settings
    "../src/Settings/*.cpp"
    "../src/Settings/*.h"
    "../src/Settings/*.hpp"
)
source_group("VulkanViewer\\Settings" FILES ${VulkanViewer__Settings})

set(Graphics__Shaders
    "../graphics/shaders/*.frag"
    "../graphics/shaders/*.vert"
)
source_group("Graphics\\Shaders" FILES ${Graphics__Shaders})

set(Graphics__MeshObjects
    "../graphics/meshObjects/*.cpp"
)
source_group("Graphics\\MeshObjects" FILES ${Graphics__MeshObjects})

set(Platform
    "../src/VulkanModule/Platform/*.cpp"
    "../src/VulkanModule/Platform/*.h"
)
source_group("Platform" FILES ${Platform})

set(Platform__FileSystem
    "../src/VulkanModule/Platform/FileSystem/*.cpp"
    "../src/VulkanModule/Platform/FileSystem/*.h"
)
source_group("Platform\\FileSystem" FILES ${Platform__FileSystem})

set(Platform__GUISystem
    "../src/VulkanModule/Platform/GUISystem/stubs/*.cpp"
    "../src/VulkanModule/Platform/GUISystem/stubs/*.h"
)
source_group("Platform\\GUISystem" FILES ${Platform__GUISystem})

set(Platform__ImageHandling
    "../src/VulkanModule/Platform/ImageHandling/*.h"
)
source_group("Platform\\ImageHandling" FILES ${Platform__ImageHandling})

set(Platform__Logger
    "../src/VulkanModule/Platform/Logger/*.cpp"
    "../src/VulkanModule/Platform/Logger/*.h"
)
source_group("Platform\\Logger" FILES ${Platform__Logger})

set(Platform__Linux
    "src/*.h"
)
source_group("Platform\\Linux" FILES ${Platform__Linux})

set(Platform__OSAbstraction
    "../src/VulkanModule/Platform/OSAbstraction/*.cpp"
    "../src/VulkanModule/Platform/OSAbstraction/*.h"
)
source_group("Platform\\OSAbstraction" FILES ${Platform__OSAbstraction})

set(Platform__ControlScheme
    "../src/VulkanModule/Platform/ControlScheme/*.h"
)
source_group("Platform\\ControlScheme" FILES ${Platform__ControlScheme})

set(Vulkan__Adjunct
    "../src/VulkanModule/Adjunct/*.cpp"
    "../src/VulkanModule/Adjunct/*.h"
)
source_group("Vulkan\\Adjunct" FILES ${Vulkan__Adjunct})

set(Vulkan__Adjunct__Renderables
    "../src/VulkanModule/Adjunct/Renderables/*.cpp"
    "../src/VulkanModule/Adjunct/Renderables/*.h"
)
source_group("Vulkan\\Adjunct\\Renderables" FILES ${Vulkan__Adjunct__Renderables})

set(Vulkan__Adjunct__VertexTypes
    "../src/VulkanModule/Adjunct/VertexTypes/*.cpp"
    "../src/VulkanModule/Adjunct/VertexTypes/*.h"
)
source_group("Vulkan\\Adjunct\\VertexTypes" FILES ${Vulkan__Adjunct__VertexTypes})

set(Vulkan__Assist
    "../src/VulkanModule/Assist/*.cpp"
    "../src/VulkanModule/Assist/*.h"
source_group("Vulkan\\Assist" FILES ${Vulkan__Assist})

set(Vulkan__Objects
    "../src/VulkanModule/Objects/*.cpp"
    "../src/VulkanModule/Objects/*.h"
)
source_group("Vulkan\\Objects" FILES ${Vulkan__Objects})

set(Vulkan__Setup
    "../src/VulkanModule/Setup/*.cpp"
    "../src/VulkanModule/Setup/*.h"
)
source_group("Vulkan\\Setup" FILES ${Vulkan__Setup})

set(ALL_FILES
    ${VulkanViewer}
    ${VulkanViewer__Settings}
    ${Graphics__Shaders}
    ${Graphics__MeshObjects}
    ${Platform}
    ${Platform__FileSystem}
    ${Platform__GUISystem}
    ${Platform__ImageHandling}
    ${Platform__Logger}
    ${Platform__Linux}
    ${Platform__OSAbstraction}
    ${Platform__ControlScheme}
    ${Vulkan__Adjunct}
    ${Vulkan__Adjunct__Renderables}
    ${Vulkan__Adjunct__VertexTypes}
    ${Vulkan__Assist}
    ${Vulkan__Objects}
    ${Vulkan__Setup}
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
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Setup;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Assist;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Adjunct;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Adjunct/Renderables;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Adjunct/VertexTypes;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Objects;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/OSAbstraction;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/ImageHandling;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/Logger;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/FileSystem;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/GUISystem;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/GUISystem/stubs;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../src/VulkanModule/Platform/ControlScheme;"
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
        COMMAND if [ ! -d "build/compiledShaders" ]\; then mkdir "build/compiledShaders" \; cd "../graphics/shaders" \; ./compileLinux.sh\; fi
        COMMAND if [ ! -L "build/textures" ]\; then cd build\; ln -s "../../graphics/assets/textures" \; fi
        COMMENT "Post-build events complete."
    )
endif()


#if("${CMAKE_PLATFORM_NAME}" STREQUAL "x64")		# Custom build events
#    add_custom_command(
#        OUTPUT "compiledShaders"
#        COMMAND cd ../graphics/shaders
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

