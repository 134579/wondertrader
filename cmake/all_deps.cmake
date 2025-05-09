add_compile_options(-fPIC)

add_compile_definitions(BOOST_BIND_GLOBAL_PLACEHOLDERS)
set(BOOST_INCLUDE_LIBRARIES "asio;smart_ptr;utility;filesystem;thread;interprocess;xpressive;circular_buffer;property_tree")
CPMAddPackage(
  NAME Boost
  URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
  OPTIONS "BOOST_ENABLE_CMAKE ON"
)

CPMAddPackage("gh:fmtlib/fmt#10.2.1")
CPMAddPackage(URI "gh:gabime/spdlog#v1.13.0" OPTIONS "SPDLOG_FMT_EXTERNAL ON")


CPMAddPackage(
  NAME nanomsg
  GITHUB_REPOSITORY nanomsg/nanomsg
  GIT_TAG 1.2.1
  OPTIONS "NN_STATIC_LIB ON"
)

target_include_directories(nanomsg PUBLIC ${nanomsg_SOURCE_DIR}/src)

link_libraries(Boost::asio Boost::smart_ptr Boost::filesystem Boost::utility Boost::thread Boost::interprocess Boost::xpressive Boost::circular_buffer Boost::property_tree)
link_libraries(fmt)
link_libraries(spdlog)
link_libraries(nanomsg)
