# Install script for directory: /Users/alex/Documents/ROOTCuts

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTCuts")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/Debug/ROOTCuts")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTCuts")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/Release/ROOTCuts")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTCuts")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/MinSizeRel/ROOTCuts")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTCuts")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/RelWithDebInfo/ROOTCuts")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTCuts")
      endif()
    endif()
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTEffs")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/Debug/ROOTEffs")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTEffs")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/Release/ROOTEffs")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTEffs")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/MinSizeRel/ROOTEffs")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    list(APPEND CMAKE_ABSOLUTE_DESTINATION_FILES
     "/Users/alex/Documents/ROOTCuts/ROOTEffs")
    if(CMAKE_WARN_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(WARNING "ABSOLUTE path INSTALL DESTINATION : ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
    if(CMAKE_ERROR_ON_ABSOLUTE_INSTALL_DESTINATION)
        message(FATAL_ERROR "ABSOLUTE path INSTALL DESTINATION forbidden (by caller): ${CMAKE_ABSOLUTE_DESTINATION_FILES}")
    endif()
file(INSTALL DESTINATION "/Users/alex/Documents/ROOTCuts" TYPE EXECUTABLE FILES "/Users/alex/Documents/ROOTCuts/xcode/RelWithDebInfo/ROOTEffs")
    if(EXISTS "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/root_build/lib"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Users/alex/delphes"
        "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}/Users/alex/Documents/ROOTCuts/ROOTEffs")
      endif()
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/alex/Documents/ROOTCuts/xcode/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
