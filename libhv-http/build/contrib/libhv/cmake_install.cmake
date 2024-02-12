# Install script for directory: /home/nikonoff/dev/DCSA/libhv-http/contrib/libhv

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
    set(CMAKE_INSTALL_CONFIG_NAME "")
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

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

# Set default install directory permissions.
if(NOT DEFINED CMAKE_OBJDUMP)
  set(CMAKE_OBJDUMP "/usr/bin/objdump")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/home/nikonoff/dev/DCSA/libhv-http/build/lib/libhv_static.a")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/hv" TYPE FILE FILES
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/hv.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/hconfig.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/hexport.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hplatform.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hdef.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hatomic.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/herr.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/htime.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hmath.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hbase.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hversion.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hsysinfo.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hproc.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hthread.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hmutex.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hsocket.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hlog.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hbuf.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hmain.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/base/hendian.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/ssl/hssl.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/event/hloop.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/event/nlog.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/util/base64.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/util/md5.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/util/sha1.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hmap.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hstring.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hfile.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hpath.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hdir.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hurl.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hscope.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hthreadpool.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hasync.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/hobjectpool.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/ifconfig.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/iniparser.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/json.hpp"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/singleton.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/cpputil/ThreadLocalStorage.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/Buffer.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/Channel.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/Event.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/EventLoop.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/EventLoopThread.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/EventLoopThreadPool.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/Status.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/TcpClient.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/TcpServer.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/UdpClient.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/evpp/UdpServer.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/httpdef.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/wsdef.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/http_content.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/HttpMessage.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/HttpParser.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/WebSocketParser.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/WebSocketChannel.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/server/HttpServer.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/server/HttpService.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/server/HttpContext.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/server/HttpResponseWriter.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/server/WebSocketServer.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/client/HttpClient.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/client/requests.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/client/axios.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/client/AsyncHttpClient.h"
    "/home/nikonoff/dev/DCSA/libhv-http/contrib/libhv/http/client/WebSocketClient.h"
    )
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libhv/libhvConfig.cmake")
    file(DIFFERENT EXPORT_FILE_CHANGED FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libhv/libhvConfig.cmake"
         "/home/nikonoff/dev/DCSA/libhv-http/build/contrib/libhv/CMakeFiles/Export/lib/cmake/libhv/libhvConfig.cmake")
    if(EXPORT_FILE_CHANGED)
      file(GLOB OLD_CONFIG_FILES "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libhv/libhvConfig-*.cmake")
      if(OLD_CONFIG_FILES)
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/libhv/libhvConfig.cmake\" will be replaced.  Removing files [${OLD_CONFIG_FILES}].")
        file(REMOVE ${OLD_CONFIG_FILES})
      endif()
    endif()
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libhv" TYPE FILE FILES "/home/nikonoff/dev/DCSA/libhv-http/build/contrib/libhv/CMakeFiles/Export/lib/cmake/libhv/libhvConfig.cmake")
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^()$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/libhv" TYPE FILE FILES "/home/nikonoff/dev/DCSA/libhv-http/build/contrib/libhv/CMakeFiles/Export/lib/cmake/libhv/libhvConfig-noconfig.cmake")
  endif()
endif()

