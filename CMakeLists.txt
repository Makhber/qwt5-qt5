cmake_minimum_required( VERSION 3.16.3 )

project( qwt5-qt5
  VERSION 5.2.3
  DESCRIPTION "Qt Widgets for Technical Applications adapted to work with Qt5"
  HOMEPAGE_URL "https://github.com/Makhber/qwt5-qt5"
  LANGUAGES CXX )

include(GNUInstallDirs)

set( CMAKE_CXX_STANDARD 17)
set( CMAKE_CXX_STANDARD_REQUIRED TRUE )
set( CMAKE_CXX_EXTENSIONS OFF )

set( CMAKE_AUTOUIC OFF )
set( CMAKE_AUTORCC OFF )
set( CMAKE_AUTOMOC ON )

if( MSVC )
  # /wd4456 Silent "declaration of %1 hides %2 ..."
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /W4 /permissive- /wd4456" )
  if( CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-nonportable-include-path" )
  else()
    add_compile_options( "/MP" )
  endif()
else()
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fexceptions -Wall -Wextra -pedantic" )
  set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic" )
endif()

find_package( Qt5
  COMPONENTS
  Core
  Gui
  Widgets
  PrintSupport
  REQUIRED
  )

option( QWT_PLOT "Enable all classes, that are needed to use the QwtPlot widget" ON )

option( QWT_WIDGETS "Enable all classes, that are needed to use the all other widgets \
  (sliders, dials, ...)" ON )

option( QWT_SVG_ITEM "Display SVG images on the plot canvas" ON )
if( QWT_SVG_ITEM )
  find_package( Qt5 COMPONENTS Svg REQUIRED )
endif()

option( QWT_DESIGNER "Build Designer Plugin" ON )

option( QWT_BUILD_EXAMPLES "Build Examples" ON )

if( NOT (${PROJECT_SOURCE_DIR} STREQUAL ${CMAKE_SOURCE_DIR}) )
  set( QWT_DESIGNER OFF CACHE BOOL "Build Designer Plugin" FORCE )
  set( QWT_BUILD_EXAMPLES OFF CACHE BOOL "Build Examples" FORCE )
endif()

if( QWT_DESIGNER )
  find_package( Qt5 COMPONENTS Designer REQUIRED )
endif()

add_subdirectory( src )

if( QWT_DESIGNER  )
  add_subdirectory( designer )
endif()

if( QWT_BUILD_EXAMPLES )
  add_subdirectory( examples )
endif()

if( "${CMAKE_SOURCE_DIR}" STREQUAL "${PROJECT_SOURCE_DIR}" )
  configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/qwt5-qt5.pc.in" "${CMAKE_CURRENT_BINARY_DIR}/qwt5-qt5.pc" @ONLY )
  install( FILES "${CMAKE_CURRENT_BINARY_DIR}/qwt5-qt5.pc" DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig )

  install( DIRECTORY ${PROJECT_SOURCE_DIR}/doc/html/ DESTINATION ${CMAKE_INSTALL_DOCDIR}/html )
  if( UNIX )
    install( DIRECTORY ${PROJECT_SOURCE_DIR}/doc/man/ DESTINATION ${CMAKE_INSTALL_MANDIR} )
  endif()
endif()
