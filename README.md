# Really Experimental MK platform

[![Travis Build Status](https://travis-ci.org/bassosimone/libremk_platform.svg?branch=master)](https://travis-ci.org/bassosimone/libremk_platform) [![Appveyor Win32 Build Status](https://ci.appveyor.com/api/projects/status/github/bassosimone/libremk_platform?branch=master&svg=true)](https://ci.appveyor.com/project/bassosimone/libremk-platform)

The purpose of this repository is to provide portable, hookable replacements for
common socket and related APIs. The objective is to provide a common API
that can be used both on Unix and on Windows to implement basic networking stuff
with the option to provide different implementations of such APIs.

## Motivation

Measurement Kit code should use this as its lowest level API. Dependencies and
code contributed to [Measurement Kit](
https://github.com/measurement-kit/measurement-kit) should be refactored
&mdash; perhaps using [coccinelle](https://github.com/coccinelle/coccinelle)
&mdash; to use the replacements provided in this library rather than the
corresponding libc API. This means, for example, using `ctx->getaddrinfo()`
instead of `getaddrinfo()`, where `ctx` is a pointer pointing to a
`remk::platform::Context` instance.

The usage of these replacements has three advantages:

1. these replacements are coded with Unix / Windows portability in mind, with
   some minor changes (e.g. using `ctx->get_last_error()` instead
   of accessing `errno` directly), so the code can also work on Windows;

2. these replacements can be mocked, allowing us, e.g., to replace
   `getaddrinfo()` with, say, a [c-ares](https://github.com/c-ares/c-ares)-based
   engine that stores the actual packets received and sent;

3. additionally, mocking allows for better testing.

The main disadvantage is that code needs to be refactored as follows:

1. you need to carry around a `ctx` context;

2. you need to replace calls to system networking APIs with indirect
   calls through the `ctx` context.

## API description

This is written in C++14. The main class is `remk::platform::Context` in
`include/remk/platform/context.h`. It provides these functionality:

1. logging (in the `LoggerMixin` class);

2. mocking of system APIs (in the `SystemMixin` class);

3. syntactic sugar APIs build on top of that.

The `Context` class inherits publicly from the above mentioned mixins.

## Building

There are currently no dependencies apart from system headers. We use CMake
to build. The CMake script should perform all the platform checks.

To build a static library, do:

```
cmake .
cmake --build .
```

To build a shared library on Unix, replace the first command with:

```
cmake -DBUILD_SHARED_LIBS=ON .
```

On Windows, you also need to force CMake to export symbols:

```
cmake -DCMAKE_WINDOWS_EXPORT_ALL_SYMBOLS=ON -DBUILD_SHARED_LIBS=ON ..
```

(Make sure you're in a developer prompt, tho.)

## Examples

See `example/http_client.cpp`.
