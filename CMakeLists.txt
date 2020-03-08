# I got some inspiration from
# https://stackoverflow.com/questions/17511496/create-a-shared-library-with-cmake

#
# Basic CMake version
#

cmake_minimum_required ( VERSION 3.2 )

#
# General Project settings
# 

# Get the project name from the current directory
get_filename_component ( projectname ${CMAKE_CURRENT_SOURCE_DIR} NAME )

# Name the project
project ( ${projectname}
          VERSION 0.0.1
          # DESCRIPTION "confini is a library to read and write configuration/ini files"
		  LANGUAGES CXX )

#
# General Compiler Settings
#

# Enable C++11 features on gcc/clang
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "(GNU|Clang)")
    #set(CMAKE_CXX_FLAGS "-std=c++11")
	add_compile_options(-std=c++11)
endif()

# Enable Code coverage
if ("${CMAKE_CXX_COMPILER_ID}"  MATCHES "(GNU|Clang)")
  option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)

  if (ENABLE_COVERAGE)
    message("  ENABLE_COVERAGE")
    add_compile_options(--coverage -O0)
	SET( CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS}--coverage" )
  endif()
endif()

# Set warning level
if (MSVC)
  message("  Warning Level: MSVC")
  add_compile_options(/W4)
else()
  message("  Warning Level: GNU")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

#
# Doxygen Documentation
#

find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file ( ${CMAKE_CURRENT_SOURCE_DIR}/src/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY )
    add_custom_target ( doc
                        ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
                        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc
                        COMMENT "Generating API documentation with Doxygen" VERBATIM
                      )
    if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
        add_custom_target ( docopen
                            ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/index.html
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/html
                          )
    elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
        add_custom_target ( docopen
                            xdg-open ${CMAKE_CURRENT_SOURCE_DIR}/doc/html/index.html
                            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/doc/html
                          )
    endif()

endif(DOXYGEN_FOUND)

#
# Library
#

# Add the source files. Avoid file(GLOB SRC_FILES ...)
configure_file(src/${projectname}version.h.in ${CMAKE_BINARY_DIR}/${projectname}version.h @ONLY)
add_library ( ${projectname} SHARED
              include/confini.h
              src/confini.cpp
)

# Add INCLUDE folder
target_include_directories ( ${projectname} PRIVATE include )
target_include_directories ( ${projectname} PRIVATE src )
target_include_directories ( ${projectname} PRIVATE lib )
target_include_directories ( ${projectname} PRIVATE lib/LittleCPPHelper/include )
target_include_directories ( ${projectname} PRIVATE ${CMAKE_BINARY_DIR} )

# Link library
target_link_libraries ( ${projectname} ${CMAKE_EXE_LINKER_FLAGS} )

# Set VERSION property
set_target_properties ( ${projectname}
                        PROPERTIES VERSION ${PROJECT_VERSION}
					               SOVERSION 0
					               PUBLIC_HEADER include/confini.h )

configure_file(src/${projectname}.pc.in ${projectname}.pc @ONLY)

#
# Test
#

# Get Path to some test files
get_filename_component(TESTFOLDER test/ ABSOLUTE DIRECTORY)

configure_file(test/${projectname}testfiles.h.in ${CMAKE_BINARY_DIR}/${projectname}testfiles.h @ONLY)


# Add the source files. Avoid file(GLOB SRC_FILES ...)
add_executable ( ${projectname}_test
                 include/confini.h
                 lib/LittleCPPHelper/include/LittleCPPHelper/string.hpp
                 test/testmain.cpp
                 test/versiontest.cpp
                 test/readtest.cpp
				 test/changetest.cpp
				 test/writetest.cpp
)

# Add INCLUDE folder
target_include_directories ( ${projectname}_test PRIVATE include )
target_include_directories ( ${projectname}_test PRIVATE lib )
target_include_directories ( ${projectname}_test PRIVATE ${CMAKE_BINARY_DIR} )

# Link library
target_link_libraries ( ${projectname}_test ${projectname} ${CMAKE_EXE_LINKER_FLAGS} )

# Define Tests
enable_testing()
add_test ( ${projectname}_test ${CMAKE_BINARY_DIR}/${projectname}_test )

# Run the tests on every build
add_custom_command(TARGET ${projectname}_test POST_BUILD COMMAND ${projectname}_test )

# Dependencies
# Due to a cmake bug (no dependencies project/test) I define my own test target:
add_custom_target ( build-test COMMAND ${CMAKE_CTEST_COMMAND} DEPENDS ${CMAKE_BINARY_DIR}/${projectname}_test )
#add_dependencies( test ${projectname}_test )
add_dependencies( ${projectname}_test ${projectname} )


#
# Installation
#

# Define Installation
include(GNUInstallDirs)

if(WIN32)
	message("  Install: Windows")
    install ( TARGETS ${projectname}
              RUNTIME DESTINATION ${CMAKE_INSTALL_LIBDIR}
              PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
else()
	message("  Install: Linux")
    install ( TARGETS ${projectname}
              LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
              PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )
endif()

install(FILES ${CMAKE_BINARY_DIR}/${subprojectname}/${projectname}.pc
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

# Recursive Header workaround
install( DIRECTORY ${CMAKE_INSTALL_INCLUDEDIR} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR} )

