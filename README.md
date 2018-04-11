# Really Experimental MK platform
> As the name implies, this is currently experimental code.

[![Travis Build Status](https://travis-ci.org/bassosimone/libremk_platform.svg?branch=master)](https://travis-ci.org/bassosimone/libremk_platform) [![Appveyor Win32 Build Status](https://ci.appveyor.com/api/projects/status/github/bassosimone/libremk_platform?branch=master&svg=true)](https://ci.appveyor.com/project/bassosimone/libremk-platform) [![Coverage Status](https://coveralls.io/repos/github/bassosimone/libremk_platform/badge.svg?branch=master)](https://coveralls.io/github/bassosimone/libremk_platform?branch=master)

The platform is the layer of code on which Measurement Kit is based. The main and only
header is `<remk/platform.hpp>`. The core class is `remk::platform::Context`. It is
a C++17 class that allows to:

1. Write portable networking code because the Windows/Unix differences are
   taken into account by its API. For example, you can obtain the last error
   that occurred by calling `ctx->get_last_error()`. It also contains some
   useful functions like `ctx->strtonum()` that are not always available on
   all platforms. Where possible the API is the same that you would use on
   Unix, except for some cases where that is not possible (e.g. the above
   mentioned function to get the last error that occurred, which is patterned
   after the Windows API because `errno` on Windows behaves differently).

2. Mock most of its API for measurement or testing purposes. Most of its APIs
   are virtual methods. By overriding these methods, you can construct test
   cases or invoke other implementations of the same methods. For example, one
   use case is replacing `ctx->getaddrinfo()` with an implementation of
   `getaddrinfo()` based on [c-ares](https://github.com/c-ares/c-ares) that
   collects timing and packet-content information. This allows one to write
   network tests using the above mentioned Unix-like API, without having
   to wonder about collecting low-level data, which can instead be collected
   by overriding the implementation of the called methods.

3. Write less code, thanks to syntactic sugar functionality, like
   `ctx->connect_tcp()`, that perform common tasks.

4. Write more correct code, thanks to wrapper classes like `DeferClosesocket`
   that guarantee that resources are cleared in a RAII fashion.

5. Carry around configuration information in a way compatible with the current
   implementation of Measurement Kit (see the `SettingsMixin` which is one of
   the classes that `Context` inherits from).

6. Log and emit other opaque events that occurr during network tests (see the
   `LoggerAndEmitterMixin` another of the classes that `Context` inherits from).

7. Interrupt long waits or perform periodic actions, by overriding the
   `ctx->select()` method properly.

8. Write more secure code, by providing a logging interface that does not
   use the `printf()` style of printing values.

9. Classify network-related errors that we care about by mapping them onto
   the same strings currently used by Measurement Kit as of v0.8.x.

10. Collect precise timing information using a steady clock.

In short, this library is the engine to write tests. To collect measurements
and perform other actions, Measurement Kit should properly override the
methods of this class to implement more advanced behavior.

## Expected evolution

This is currently developed as a separate library to ease development. It is
expected that this library is integrated in Measurement Kit either by vendoring
the sources or by linking to an external repository.

## Building

There are no external dependencies apart from a C++17 library. We use CMake
to build. The CMake script should perform all the platform checks and use
either platform provided APIs or provide suitable replacements.

To build a static library, from a shell (on Unix) or a MSVC development
prompt (on Windows), type:

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

## Examples

See [example/http_client.cpp](example/http_client.cpp).
