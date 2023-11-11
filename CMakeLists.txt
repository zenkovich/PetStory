cmake_minimum_required(VERSION 3.6)
project(PetStory LANGUAGES CXX)

# Options
option(O2_SELFPROFILE "Enable profiling o2 benchmark." OFF)
option(O2_EDITOR "Enables o2 editor." ON)

# Common definitions
set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (UNIX)
    set(O2_PLATFORM "Linux")
elseif (WIN32)
    set(O2_PLATFORM "Windows")
endif()

set(BIN_OUTPUTDIR "${CMAKE_CURRENT_SOURCE_DIR}/Bin/${O2_PLATFORM}")

# o2 libraries
add_subdirectory(o2)
add_compile_definitions(${O2_COMPILE_DEFINITIONS_EXPORT})

# Game library
add_library(GameLib STATIC)

file(GLOB_RECURSE GameLib_SOURCES "Sources/Game/*.cpp" "Sources/Game/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GameLib_SOURCES})

set(GameLib_INCLUDES "Sources/Game")

target_sources(GameLib PRIVATE ${GameLib_SOURCES})
target_include_directories(GameLib PUBLIC ${GameLib_INCLUDES})

target_link_libraries(GameLib PUBLIC o2Framework)   

add_dependencies(GameLib o2Framework)     

if(WIN32)
    add_custom_command(TARGET GameLib PRE_BUILD
                       COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/o2/CodeTool/Bin/o2CodeTool 
                            -project GameLib
                            -sources "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game" 
                            -parent_projects "${CMAKE_CURRENT_SOURCE_DIR}/o2/Framework/Sources/o2/CodeToolCache.xml")
endif()

if(MSVC)
    target_compile_options(GameLib PRIVATE "/MP")
endif()

# Editor library
add_library(EditorLib STATIC)

file(GLOB_RECURSE EditorLib_SOURCES "Sources/Editor/*.cpp" "Sources/Editor/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${EditorLib_SOURCES})

set(EditorLib_INCLUDES "Sources/Editor")

target_sources(EditorLib PRIVATE ${EditorLib_SOURCES})
target_include_directories(EditorLib PUBLIC ${EditorLib_INCLUDES})

target_link_libraries(EditorLib PUBLIC
    GameLib 
    o2Editor
)    

add_dependencies(EditorLib GameLib o2Editor)    

if(WIN32)
    add_custom_command(TARGET EditorLib PRE_BUILD
                       COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/o2/CodeTool/Bin/o2CodeTool 
                            -project EditorLib
                            -sources "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Editor" 
                            -parent_projects "${CMAKE_CURRENT_SOURCE_DIR}/o2/Framework/Sources/o2/CodeToolCache.xml")
endif()

if(MSVC)
    target_compile_options(EditorLib PRIVATE "/MP")
endif()

# Assets builder
add_executable(AssetsBuilder)
add_compile_definitions(${O2_COMPILE_DEFINITIONS_EXPORT})
target_sources(AssetsBuilder PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/o2/AssetsBuildTool/Sources/o2AssetBuilder/AssetsBuilderRunner.cpp)

target_link_libraries(AssetsBuilder 
    PRIVATE
    o2AssetsBuilder
    o2Framework
    GameLib
)

if(MSVC)
    target_compile_options(AssetsBuilder PRIVATE "/MP" "/Zc:__cplusplus")
    set_target_properties(AssetsBuilder PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_OUTPUTDIR}
    )       
elseif (UNIX)
    target_compile_options(AssetsBuilder PRIVATE ${DEMO_WARNING_OPTION} -Wno-pedantic)
endif()

# Build assets step
add_custom_target(
    BuildAssets
    COMMAND
    ${BIN_OUTPUTDIR}/AssetsBuilder -platform ${O2_PLATFORM} -source "${CMAKE_CURRENT_SOURCE_DIR}/Assets/" -target "${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/Data/" -target-tree "${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/Data.json" -compressor-config "${CMAKE_CURRENT_SOURCE_DIR}/o2/CompressToolsConfig.json"
    COMMENT "Build o2 assets: -platform ${O2_PLATFORM} -source \"${CMAKE_CURRENT_SOURCE_DIR}/Assets/\" -target \"${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/Data/\" -target-tree \"${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/Data.json\" -compressor-config \"${CMAKE_CURRENT_SOURCE_DIR}/o2/CompressToolsConfig.json\""
)

add_dependencies(BuildAssets o2AssetsBuilder AssetsBuilder)

# Game App
add_executable(PetStory)

if (WIN32)
    target_sources(PetStory PRIVATE "Platforms/Windows/AppMain.cpp")
endif()

target_link_libraries(PetStory PUBLIC GameLib)  
add_dependencies(PetStory GameLib BuildAssets)   

set_target_properties(PetStory PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_OUTPUTDIR}
)       

if(MSVC)
  set_property(TARGET PetStory PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:PetStory>")
endif()

# Editor App
if(O2_EDITOR)
    # editor assets build step
    add_custom_target(
        BuildEditorAssets
        COMMAND
        ${BIN_OUTPUTDIR}/AssetsBuilder -platform ${O2_PLATFORM} -source "${CMAKE_CURRENT_SOURCE_DIR}/o2/Editor/Assets/" -target "${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/EditorData/" -target-tree "${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/EditorData.json" -compressor-config "${CMAKE_CURRENT_SOURCE_DIR}/o2/CompressToolsConfig.json"
        COMMENT "Build o2 editor assets: -platform ${O2_PLATFORM} -source \"${CMAKE_CURRENT_SOURCE_DIR}/o2/Editor/Assets/\" -target \"${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/EditorData/\" -target-tree \"${CMAKE_CURRENT_SOURCE_DIR}/BuiltAssets/${O2_PLATFORM}/EditorData.json\" -compressor-config \"${CMAKE_CURRENT_SOURCE_DIR}/o2/CompressToolsConfig.json\""
    )

    add_dependencies(BuildEditorAssets o2AssetsBuilder AssetsBuilder)

    # Editor executable
    add_executable(Editor)

    if (WIN32)
        target_sources(Editor PRIVATE "Platforms/Windows/EditorMain.cpp")
    endif()

    target_link_libraries(Editor PUBLIC EditorLib)  
    add_dependencies(Editor EditorLib BuildAssets BuildEditorAssets)      

    set_target_properties(Editor PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUTDIR}
        RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO ${BIN_OUTPUTDIR}
    )  

    if(MSVC)
      set_property(TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Editor>")
    endif()  
endif() 

# Groups in IDE
set_target_properties(GameLib PROPERTIES FOLDER PetStory)
set_target_properties(EditorLib PROPERTIES FOLDER PetStory)
set_target_properties(AssetsBuilder PROPERTIES FOLDER PetStory)
set_target_properties(BuildAssets PROPERTIES FOLDER PetStory)
set_target_properties(BuildEditorAssets PROPERTIES FOLDER PetStory)

set_target_properties(PetStory PROPERTIES FOLDER Apps)
set_target_properties(Editor PROPERTIES FOLDER Apps)

# Default startup target
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Editor)