add_compile_options(-fPIC)

set(BOOST_ENABLE_CMAKE ON)
set(BOOST_INCLUDE_LIBRARIES "asio;smart_ptr;utility;filesystem;thread")
set(BOOST_CHARCONV_NO_QUADMATH ON)
CPMAddPackage(
  NAME Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
  # OPTIONS "BUILD_SHARED_LIBS ON BUILD_STATIC_LIBS OFF"
)



CPMAddPackage("gh:fmtlib/fmt#10.2.1")
set(SPDLOG_FMT_EXTERNAL ON)
CPMAddPackage("gh:gabime/spdlog#v1.13.0")


CPMAddPackage(
  NAME nanomsg
  GITHUB_REPOSITORY nanomsg/nanomsg
  GIT_TAG 1.2.1
  OPTIONS "NN_STATIC_LIB ON"
)

target_include_directories(nanomsg PUBLIC ${nanomsg_SOURCE_DIR}/src)

link_libraries(fmt)
link_libraries(spdlog)
link_libraries(nanomsg)
link_libraries(Boost::asio Boost::smart_ptr Boost::filesystem Boost::utility Boost::thread)

# add_library(boost_filesystem ALIAS Boost::filesystem)
# add_library(boost_thread ALIAS Boost::thread)
