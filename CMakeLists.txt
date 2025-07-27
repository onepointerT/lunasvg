cmake_minimum_required(VERSION 3.15)

set(LUNASVG_VERSION_MAJOR 3)
set(LUNASVG_VERSION_MINOR 3)
set(LUNASVG_VERSION_MICRO 1)

project(lunasvg LANGUAGES CXX C VERSION ${LUNASVG_VERSION_MAJOR}.${LUNASVG_VERSION_MINOR}.${LUNASVG_VERSION_MICRO})

option(USE_SYSTEM_PLUTOVG "Use system plutovg library" OFF)

if(USE_SYSTEM_PLUTOVG)
    find_package(plutovg 1.0.0 QUIET)
    if(NOT plutovg_FOUND)
        message(WARNING "Could not find: plutovg>=1.0.0. Falling back to plutovg submodule.")
    endif()
endif()

include_directories( include )
include_directories( lib )
include_directories( lib/libm/src/ce-libm )
include_directories( lib/libm/lib/endian_h )
include_directories( lib/plutovg/include )

add_subdirectory( lib/libm )
set( MATH_LIBRARY m_ce ) # Business as usual

if(NOT plutovg_FOUND)
    message(STATUS "Using plutovg submodule.")
    option(PLUTOVG_BUILD_EXAMPLES "Build examples" OFF)
    file( GLOB PLUTOVG_SOURCES lib/plutovg/source/*.c )
    add_library( plutovg ${PLUTOVG_SOURCES} )
    target_link_libraries( plutovg ${MATH_LIBRARY} )
endif()

#set( CMAKE_C_STANDARD 23 )

set(nanosvg_sources
    source/nanosvg.c
    source/nanosvgrast.c)

set(nanosvg_headers
    include/nanosvg.h
    include/nanosvgrast.h)

add_library(nanosvg ${nanosvg_sources} ${nanosvg_headers})
add_library(nanosvg::nanosvg ALIAS nanosvg)

if(MATH_LIBRARY)
    target_link_libraries(nanosvg PUBLIC ${MATH_LIBRARY})
endif()

set_target_properties(nanosvg PROPERTIES
    SOVERSION ${LUNASVG_VERSION_MAJOR}
    C_VISIBILITY_PRESET hidden
    C_STANDARD_REQUIRED ON
    C_STANDARD 99
)


set(lunasvg_paths_sources
    source/lunapaths.c)

set(lunasvg_paths_headers
    include/lunapaths.h)

add_library(lunasvg_paths ${lunasvg_paths_sources} ${lunasvg_paths_sources})
add_library(lunasvg::lunasvg_paths ALIAS lunasvg_paths)

if(MATH_LIBRARY)
    target_link_libraries(lunasvg_paths PUBLIC ${MATH_LIBRARY} nanosvg)
endif()

set_target_properties(lunasvg_paths PROPERTIES
    SOVERSION ${LUNASVG_VERSION_MAJOR}
    C_VISIBILITY_PRESET hidden
    C_STANDARD_REQUIRED ON
    C_STANDARD 99
)

set(lunasvg_sources
    source/lunasvg.cpp
    source/graphics.cpp
    source/svgelement.cpp
    source/svggeometryelement.cpp
    source/svglayoutstate.cpp
    source/svgpaintelement.cpp
    source/svgparser.cpp
    source/svgproperty.cpp
    source/svgrenderstate.cpp
    source/svgtextelement.cpp
    source/svgpathelement.cpp
#    source/svgpath.cpp
#    source/svgcurve.cpp
)

set(lunasvg_headers
    include/lunasvg.h
    source/graphics.h
    source/svgelement.h
    source/svggeometryelement.h
    source/svglayoutstate.h
    source/svgpaintelement.h
    source/svgparserutils.h
    source/svgproperty.h
    source/svgrenderstate.h
    source/svgtextelement.h
    source/svgpathelement.h
#    source/svgpath.h
#    source/svgcurve.h
)

add_library(lunasvg ${lunasvg_sources} ${lunasvg_headers})
add_library(lunasvg::lunasvg ALIAS lunasvg)

target_link_libraries(lunasvg PRIVATE plutovg nanosvg::nanosvg lunasvg_paths ${MATH_LIBRARY})

set_target_properties(lunasvg PROPERTIES
    SOVERSION ${LUNASVG_VERSION_MAJOR}
    CXX_VISIBILITY_PRESET hidden
    CXX_STANDARD_REQUIRED ON
    CXX_STANDARD 17
)

target_include_directories(lunasvg PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/source
)

target_include_directories(lunasvg PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include/lunasvg>
)

target_compile_definitions(lunasvg PRIVATE LUNASVG_BUILD)
if(NOT BUILD_SHARED_LIBS)
    target_compile_definitions(lunasvg PUBLIC LUNASVG_BUILD_STATIC)
endif()

include(GNUInstallDirs)

include(CMakePackageConfigHelpers)
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/lunasvgConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/lunasvgConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lunasvg
)

write_basic_package_version_file(lunasvgConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/include/lunasvg.h
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/lunasvg
)

install(TARGETS plutovg
    EXPORT lunasvgTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS m_ce
    EXPORT lunasvgTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS nanosvg
    EXPORT lunasvgTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS lunasvg_paths
    EXPORT lunasvgTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(TARGETS lunasvg
    EXPORT lunasvgTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(EXPORT lunasvgTargets
    FILE lunasvgTargets.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lunasvg
    NAMESPACE lunasvg::
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lunasvgConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/lunasvgConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/lunasvg
)

export(EXPORT lunasvgTargets
    FILE ${CMAKE_CURRENT_BINARY_DIR}/lunasvgTargets.cmake
    NAMESPACE lunasvg::
)

file(RELATIVE_PATH lunasvg_pc_prefix_relative
    "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}/pkgconfig"
    "${CMAKE_INSTALL_PREFIX}"
)

set(lunasvg_pc_cflags "")
if(NOT BUILD_SHARED_LIBS)
    string(APPEND lunasvg_pc_cflags " -DLUNASVG_BUILD_STATIC")
endif()

string(CONFIGURE [[
prefix=${pcfiledir}/@lunasvg_pc_prefix_relative@
includedir=${prefix}/@CMAKE_INSTALL_INCLUDEDIR@
libdir=${prefix}/@CMAKE_INSTALL_LIBDIR@

Name: LunaSVG
Description: SVG rendering and manipulation library in C++
Version: @PROJECT_VERSION@

Requires.private: plutovg
Cflags: -I${includedir}/lunasvg@lunasvg_pc_cflags@
Libs: -L${libdir} -llunasvg
]] lunasvg_pc @ONLY)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/lunasvg.pc" "${lunasvg_pc}")

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/lunasvg.pc"
    DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
)

option(LUNASVG_BUILD_EXAMPLES "Build examples" OFF)
if(LUNASVG_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
