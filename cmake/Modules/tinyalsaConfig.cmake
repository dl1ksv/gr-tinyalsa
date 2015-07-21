INCLUDE(FindPkgConfig)
PKG_CHECK_MODULES(PC_TINYALSA tinyalsa)

FIND_PATH(
    TINYALSA_INCLUDE_DIRS
    NAMES tinyalsa/api.h
    HINTS $ENV{TINYALSA_DIR}/include
        ${PC_TINYALSA_INCLUDEDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/include
          /usr/local/include
          /usr/include
)

FIND_LIBRARY(
    TINYALSA_LIBRARIES
    NAMES gnuradio-tinyalsa
    HINTS $ENV{TINYALSA_DIR}/lib
        ${PC_TINYALSA_LIBDIR}
    PATHS ${CMAKE_INSTALL_PREFIX}/lib
          ${CMAKE_INSTALL_PREFIX}/lib64
          /usr/local/lib
          /usr/local/lib64
          /usr/lib
          /usr/lib64
)

INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(TINYALSA DEFAULT_MSG TINYALSA_LIBRARIES TINYALSA_INCLUDE_DIRS)
MARK_AS_ADVANCED(TINYALSA_LIBRARIES TINYALSA_INCLUDE_DIRS)

