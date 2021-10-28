# c3di-libs
A library of tools and modules used by various projects in the C3DI group. These include:

### libCFG
Objects to help parse and configure parameter sets from sources such as the command line or XML files. 

### libmmv
Objects and helper classes for working with data represented in 3D volumes, eg. CT stacks or SEM stacks. 

### libThreadPool
A simple threadpool implementation.

### libTimer
A cross platform high resolution timing tool. 

### memoryManager
Memory leak detection for use in test suites, not intended for production environments.

### gtest
The GoogleTest framework for constructing unit and integration tests. 

## Steps to integrate
The easiest way to integrate c3di-libs into another Git project is to register it as a Git Submodule:
```
git submodule add https://github.com/dfki-asr/c3di-libs.git
```
When cloning or checking out a project with a git submodule for the first time:
```
git submodule init
git submodule update
```

To incorporate the c3di-libs submodule into your project's CMAKE build:
```
add_subdirectory(c3di-libs)
```

The following CMAKE variables are exported by c3di-libs into the parent CMAKE scope, thus they should be accessible in your parent project's CMAKE environment:
```
Includes:
  LIBCFG_INCLUDE_DIR
  LIBMMV_INCLUDE_DIR
  LIBTP_INCLUDE_DIR
  LIBTIMER_INCLUDE_DIR
  LIBMEMORYMANAGER_INCLUDE_DIR

Libraries (for use in eg. target_link_libraries() ):
  libCFG
  libmmv
  libThreadPool
  libTimer
  MemoryManager
  gtest
  gtest_main
```
As an example, to add the libmmv and libCFG libraries to a project:
```
add_subdirectory(c3di-libs)

include_directories(${LIBCFG_INCLUDE_DIR} ${LIBMMV_INCLUDE_DIR}

target_link_libraries(${PROJECT_NAME} libmmv libCFG)

```
