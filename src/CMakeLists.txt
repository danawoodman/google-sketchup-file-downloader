CMAKE_MINIMUM_REQUIRED( VERSION 2.8 )

# define the project name
PROJECT(file_downloader)

# Directories to search for includes
INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR} )
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )

# Ruby
FIND_PROGRAM( RUBY_EXECUTABLE NAMES ruby PATHS $ENV{CMAKE_RUBY_PATH} NO_SYSTEM_ENVIRONMENT_PATH )
FIND_PACKAGE( Ruby REQUIRED )
INCLUDE_DIRECTORIES( ${RUBY_INCLUDE_DIRS} )

# Add a dynamic library
ADD_LIBRARY(
  file_downloader
  MODULE
  ${CMAKE_SOURCE_DIR}/file_downloader.c
  # file_downloader.i
  # file_downloader.h
  file_downloader.c
)

# Link library to the Ruby library.
TARGET_LINK_LIBRARIES( file_downloader ${RUBY_LIBRARY} )

# Platform specific options.
IF(APPLE)
  SET( file_downloader PROPERTIES CMAKE_OSX_ARCHITECTURE "i386;x86_64" )
  SET_TARGET_PROPERTIES( file_downloader PROPERTIES PREFIX "" SUFFIX ".bundle" )
  SET_TARGET_PROPERTIES( file_downloader PROPERTIES LINK_FLAGS "-undefined suppress -flat_namespace")
ELSEIF(WIN32)
  SET( SWIG_DEFINES "-D_WINDOWS" )
  SET( SWIG_COMMON "-Fmicrosoft" )
  SET_TARGET_PROPERTIES( file_downloader PROPERTIES PREFIX "" SUFFIX ".dll")
ELSEIF(UNIX)
  # Nothing to do here as SketchUp doesn't yet work on Linux.
  # SET_TARGET_PROPERTIES( file_downloader PROPERTIES PREFIX "lib" SUFFIX ".so")
ENDIF(APPLE)
