cmake_minimum_required(VERSION 3.23)
project(PetStory LANGUAGES CXX)

# Common definitions
set(CMAKE_CXX_STANDARD 20)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (WIN32)
    set(BIN_OUTPUTDIR "${CMAKE_CURRENT_SOURCE_DIR}/Bin/Win")
endif()

add_compile_definitions(SCRIPTING_BACKEND_JERRYSCRIPT _CRT_SECURE_NO_WARNINGS)

if (WIN32)
    add_compile_definitions(PLATFORM_WINDOWS)
endif()

# o2 libraries
add_subdirectory(o2)

# Game library
add_library(GameLib STATIC)

file(GLOB_RECURSE GameLib_SOURCES "Sources/Game/*.cpp" "Sources/Game/*.h")
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${GameLib_SOURCES})

set(GameLib_INCLUDES "Sources/Game")

target_sources(GameLib PRIVATE ${GameLib_SOURCES})
target_include_directories(GameLib PUBLIC ${GameLib_INCLUDES})

target_link_libraries(GameLib PUBLIC "$<LINK_LIBRARY:WHOLE_ARCHIVE,o2Framework>")   

add_dependencies(GameLib o2Framework)     

if(WIN32)
    add_custom_command(TARGET GameLib PRE_BUILD
                       COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/o2/CodeTool/Bin/CodeTool.exe 
                            -sources "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game" 
                            -parent_projects "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2/CodeToolCache.xml" -r)
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

target_link_libraries(EditorLib PUBLIC "$<LINK_LIBRARY:WHOLE_ARCHIVE,GameLib>" 
                                       "$<LINK_LIBRARY:WHOLE_ARCHIVE,o2Editor>")    

add_dependencies(EditorLib GameLib o2Editor)    

if(WIN32)
    add_custom_command(TARGET EditorLib PRE_BUILD
                       COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/o2/CodeTool/Bin/CodeTool.exe 
                            -sources "${CMAKE_CURRENT_SOURCE_DIR}/Sources/Game" 
                            -parent_projects "${CMAKE_CURRENT_SOURCE_DIR}/Framework/Sources/o2/CodeToolCache.xml")
endif()

if(MSVC)
    target_compile_options(EditorLib PRIVATE "/MP")
endif()

# Game App
add_executable(PetStory)

if (WIN32)
    target_sources(PetStory PRIVATE "Platforms/Windows/AppMain.cpp")
endif()

target_link_libraries(PetStory PUBLIC "$<LINK_LIBRARY:WHOLE_ARCHIVE,GameLib>")  
add_dependencies(PetStory GameLib)   

set_target_properties(PetStory PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASEWITHDEBUGINFO ${BIN_OUTPUTDIR}
)       

if(MSVC)
  set_property(TARGET PetStory PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:PetStory>")
endif()

# Editor App
add_executable(Editor)

if (WIN32)
    target_sources(Editor PRIVATE "Platforms/Windows/EditorMain.cpp")
endif()

target_link_libraries(Editor PUBLIC "$<LINK_LIBRARY:WHOLE_ARCHIVE,EditorLib>")  
add_dependencies(Editor EditorLib)      

set_target_properties(Editor PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${BIN_OUTPUTDIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASEWITHDEBUGINFO ${BIN_OUTPUTDIR}
)  

if(MSVC)
  set_property(TARGET Editor PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:Editor>")
endif()  

# Groups in IDE
set_target_properties(GameLib PROPERTIES FOLDER PetStory)
set_target_properties(EditorLib PROPERTIES FOLDER PetStory)

set_target_properties(PetStory PROPERTIES FOLDER Apps)
set_target_properties(Editor PROPERTIES FOLDER Apps)