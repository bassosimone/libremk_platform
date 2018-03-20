# Really Experimental MK platform

[![Travis Build Status](https://travis-ci.org/bassosimone/libremk_platform.svg?branch=master)](https://travis-ci.org/bassosimone/libremk_platform) [![Appveyor Win32 Build Status](https://ci.appveyor.com/api/projects/status/github/bassosimone/libremk-platform?branch=master&svg=true)](https://ci.appveyor.com/project/bassosimone/libremk-platform)

The purpose of this repository is to provide portable, hookable replacements for
common socket and related APIs. The objective is to provide a common API
that can be used both on Unix and on Windows to implement basic networking stuff
with the option to provide thread-local replacements for each API.

Measurement Kit code should use this as its lowest level API. Dependencies and
code contributed to [Measurement Kit](
https://github.com/measurement-kit/measurement-kit) may be mechanichally
refactored using [coccinelle](https://github.com/coccinelle/coccinelle) to use
the replacements provided in this library rather than the corresponding libc
API. This means, for example, using `remk_platform_getaddrinfo()` instead
of `getaddrinfo()`.

The usage of these replacements has three advantages:

1. these replacements are coded with Unix / Windows portability in mind, with
   some minor changes (e.g. using `remk_platform_get_last_error()` instead
   of accessing `errno` directly), so the code can also work on Windows;

2. these replacements can be mocked on a thread-local basis, allowing, e.g., to
   replace the `getaddrinfo()` of a specific thread with another that uses as
   an engine, say, [c-ares](https://github.com/c-ares/c-ares). If we assume that
   specific dependencies are refactored using coccinelle before being compiled
   and then they're linked statically, we can extend this to allow us to see
   system level events occurring in complex code bases we do not control;

3. it is possible to write tests where we fail system level APIs without having
   to perform refactoring of code we didn't write in the first place (e.g. a
   test that was contributed as third party to Measurement Kit).

The main disadvantages are that you need to write more (namely you need to
add the `remk_platform_` prefix to several APIs) and that you loose a little
bit of performance, because you need to check a couple of pointers when
dispatching a call, to decide whether to call the real API or a replacement.

## Building on Unix

```
mkdir build
cd build
cmake -DBUILD_SHARED_LIBS=ON ..
make
```

Omit `-DBUILD_SHARED_LIBS=ON` to build a static library.

## Building on Windows

```
md build
cd build
cmake.exe -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DBUILD_SHARED_LIBS=ON ..
nmake.exe
```

Omit `-DBUILD_SHARED_LIBS=ON` to build a static library.
