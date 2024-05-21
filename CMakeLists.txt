cmake_minimum_required(VERSION 3.5.0)
project(Geolocation VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(Geolocation main.cpp GeolocationRequest.cpp GeolocationWifiPoints.cpp)

target_link_libraries(Geolocation PUBLIC wlanapi curl boost_json-mgw13-mt-x64-1_82)

target_include_directories(Geolocation PUBLIC
                            "C:/Users/Jmarcus2004/bin/boost_1_82_0")       
                             
target_link_directories(Geolocation PUBLIC
                        "C:/Users/Jmarcus2004/bin/curl-8.7.1_8-win64-mingw/lib"
                        "C:/Users/Jmarcus2004/bin/build-boost/boost/bin.v2/libs/json/build/gcc-13/release/link-static/threading-multi/visibility-hidden")           

#target_compile_features(Geolocation PUBLIC cxx_std_20)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
