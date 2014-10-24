IF(APPLE)
  FIND_PATH(OPENSSL_INCLUDE_DIR
    NAMES openssl/ssl.h
    PATHS /usr/local/opt/openssl/include
    NO_DEFAULT_PATH)
  FIND_LIBRARY(OPENSSL_LIBRARIES
    NAMES ssl
    PATHS /usr/local/opt/openssl/lib
    NO_DEFAULT_PATH
    NO_CMAKE_ENVIRONMENT_PATH
    NO_CMAKE_PATH
    NO_SYSTEM_ENVIRONMENT_PATH
    NO_CMAKE_SYSTEM_PATH
    CMAKE_FIND_FRAMEWORK NEVER)
ELSE()
  FIND_PATH(OPENSSL_INCLUDE_DIR NAMES openssl/ssl.h)
  FIND_LIBRARY(OPENSSL_LIBRARIES NAMES ssl libssl)
ENDIF(APPLE)

INCLUDE(FindPackageHandleStandardArgs)
Find_package_handle_standard_args(SSL DEFAULT_MSG OPENSSL_LIBRARIES OPENSSL_INCLUDE_DIR)

IF(OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)
  SET(OPENSSL_FOUND TRUE)
ELSE(OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)
  SET(OPENSSL_FOUND FALSE)
ENDIF(OPENSSL_INCLUDE_DIR AND OPENSSL_LIBRARIES)

IF(OPENSSL_FOUND)
  MESSAGE(STATUS "Found openssl: ${OPENSSL_LIBRARIES}, ${OPENSSL_INCLUDE_DIR}")
ELSE(OPENSSL_FOUND)
  MESSAGE(FATAL_ERROR "Could not find openssl.")
ENDIF(OPENSSL_FOUND)

SET(OPENSSL_MINIMUM_VERSION "1.0.1")

IF(OPENSSL_FOUND)
  file(STRINGS "${OPENSSL_INCLUDE_DIR}/openssl/opensslv.h" openssl_version_str REGEX "^#define[\t ]+OPENSSL_VERSION_NUMBER[\t ]+0x[0-9][0-9][0-9][0-9][0-9][0-9].*")
  
  string(REGEX REPLACE "^.*OPENSSL_VERSION_NUMBER[\t ]+0x([0-9]).*$" "\\1" OPENSSL_VERSION_MAJOR "${openssl_version_str}")
  string(REGEX REPLACE "^.*OPENSSL_VERSION_NUMBER[\t ]+0x[0-9][0-9]([0-9][0-9]).*$" "\\1" OPENSSL_VERSION_MINOR  "${openssl_version_str}")
  string(REGEX REPLACE "^.*OPENSSL_VERSION_NUMBER[\t ]+0x[0-9][0-9][0-9]([0-9][0-9]).*$" "\\1" OPENSSL_VERSION_PATCH "${openssl_version_str}")
    
  string(REGEX REPLACE "^0" "" OPENSSL_VERSION_MINOR "${OPENSSL_VERSION_MINOR}")
  string(REGEX REPLACE "^0" "" OPENSSL_VERSION_PATCH "${OPENSSL_VERSION_PATCH}")

  # hack: the above regex-replaces consume versions of the form "00" too greedily, 
  # giving us an empty string, so make sure they're 0 if empty.
  if(OPENSSL_VERSION_MINOR STREQUAL "")
    SET(OPENSSL_VERSION_MINOR "0")
  endif(OPENSSL_VERSION_MINOR STREQUAL "")

  if(OPENSSL_VERSION_PATCH STREQUAL "")
    SET(OPENSSL_VERSION_PATCH "0")
  endif(OPENSSL_VERSION_PATCH STREQUAL "")
    
  set(OPENSSL_VERSION "${OPENSSL_VERSION_MAJOR}.${OPENSSL_VERSION_MINOR}.${OPENSSL_VERSION_PATCH}")

  if(${OPENSSL_VERSION} VERSION_LESS ${OPENSSL_MINIMUM_VERSION})
    message(FATAL_ERROR "OpenSSL version found (${OPENSSL_VERSION}) is less then the minimum required (${OpenSSL_FIND_VERSION}), aborting.")
  endif(${OPENSSL_VERSION} VERSION_LESS ${OPENSSL_MINIMUM_VERSION})
  
ENDIF (OPENSSL_FOUND)
