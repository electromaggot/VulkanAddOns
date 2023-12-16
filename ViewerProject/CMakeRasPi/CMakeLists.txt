cmake_minimum_required(VERSION 3.16.0 FATAL_ERROR)

set(CMAKE_SYSTEM_VERSION 10.0 CACHE STRING "" FORCE)

set(CMAKE_C_COMPILER /usr/bin/clang-14)
set(CMAKE_CPP_COMPILER /usr/bin/clang-cpp-14)
set(CMAKE_CXX_COMPILER /usr/bin/clang++-14)
set(CMAKE_LINKER /usr/bin/ld.lld-14)

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

AUX_SOURCE_DIRECTORY("src/" Platform__Linux)

AUX_SOURCE_DIRECTORY("../src/" VulkanViewer)
AUX_SOURCE_DIRECTORY("../src/Settings/" VulkanViewer__Settings)

AUX_SOURCE_DIRECTORY("../../shaders/" Graphics__Shaders)
AUX_SOURCE_DIRECTORY("../../testObjects/" Graphics__MeshObjects)

AUX_SOURCE_DIRECTORY("../../Model3D/" Model3D)
AUX_SOURCE_DIRECTORY("../../Model3D/External" Model3D__External)

AUX_SOURCE_DIRECTORY("../../gxEngine/" GraphicsEngine)
AUX_SOURCE_DIRECTORY("../../gxEngine/ControlSchemes" GraphicsEngine__ControlSchemes)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/" Platform)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/FileSystem/" Platform__FileSystem)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/GUISystem/stubs/" Platform__GUISystem)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/ImageHandling/" Platform__ImageHandling)
AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/Logger/" Platform__Logger)

set(Platform__OSAbstraction
    "../../../VulkanModule/Platform/OSAbstraction/PlatformSDL.cpp"
)
source_group("Platform\\OSAbstraction" FILES ${Platform__OSAbstraction})

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Platform/ControlScheme/" Platform__ControlScheme)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Adjunct/" Vulkan__Adjunct)

set(Vulkan__Adjunct__Renderables
    "../../../VulkanModule/Adjunct/Renderables/AddOns.cpp"
    "../../../VulkanModule/Adjunct/Renderables/FixedRenderable.cpp"
)
source_group("Vulkan\\Adjunct\\Renderables" FILES ${Vulkan__Adjunct__Renderables})

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Adjunct/VertexTypes/" Vulkan__Adjunct__VertexTypes)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Assist/" Vulkan__Assist)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Objects/" Vulkan__Objects)

AUX_SOURCE_DIRECTORY("../../../VulkanModule/Setup/" Vulkan__Setup)

set(ALL_FILES
    ${VulkanViewer}
    ${VulkanViewer__Settings}
    ${Graphics__Shaders}
    ${Graphics__MeshObjects}
    ${Model3D}
    ${Model3D_External}
    ${GraphicsEngine}
    ${GraphicsEngine__ControlSchemes}
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
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/VulkanSDK/Include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/VulkanSDK/Third-Party/Include;"
        "/usr/include/SDL2;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/SDL2/include;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/SDL2_image/include;"
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
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui/examples;"
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
        COMMAND if [ ! -L "build/compiledShaders" ]\; then cd build\; ln -s "../../CMakeLinux/build/compiledShaders" \; cd "../../../shaders" \; ./compileLinux.sh\; fi
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
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/VulkanSDK/Lib;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/VulkanSDK/Third-Party/Bin;"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/SDL2/lib/${CMAKE_PLATFORM_NAME};"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external/SDL2_image/lib/${CMAKE_PLATFORM_NAME}"
    )
endif()


target_compile_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Debug>:
                /Od
            >
            $<$<CONFIG:Release>:
                /O2;
                /Oi;
                /Gy
            >
            -ggdb3
            ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT};
            -Wno-vla;
            ${DEFAULT_CXX_EXCEPTION_HANDLING}
        )

target_link_options(${PROJECT_NAME} PRIVATE
            $<$<CONFIG:Release>:
                /OPT:REF;
                /OPT:ICF;
            >
            -fPIE;
        )

# END OF FILE

