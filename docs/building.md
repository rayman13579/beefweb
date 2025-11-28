# Building instructions

Beefweb has two distinctive parts: server plugin (backend) and web user interface (frontend).

Server plugin is built using `cmake` and web interface is built using `webpack`.

`webpack` is fully integrated into CMake build process,
but could be run manually if only want to work on frontend only.

### foobar2000
Visual Studio is used to build backend. Currently tested with VS2022 Community.

During installation make sure to choose _Desktop development with C++_ workload.

Recent versions of VS include CMake, there is no need to install it separately.

`patch` program should be available in `PATH`.

Alternatively you can install it locally:
```
> scripts\install\patch.cmd
```

The command above will download and unpack `patch.exe` to `apps\patch`.

Open Visual Studio command prompt, navigate to project directory, run the following commands:

Configure:

```
> mkdir build\Release
> cd build\Release
> cmake ..\.. -A x64
```

This command creates Visual Studio solution `beefweb.sln`, which could be built in VS in usual way.
If you want to build 32-bit version replace `x64` with `Win32` in `cmake` command above.

Alternatively you can build from console:
```
> cmake --build . --config Release
```

And create package:
```
> cpack -C Release
```

`foo_beefweb-*.zip` will be created in `build\Release` directory.
Change extension to `.fb2k-component` to install into foobar2000.

If you want to develop backend change `Release` to `Debug` in the commands above
and add `-DENABLE_TESTS=ON` to cmake parameters.

Note: due to build system limitations you need to have separate build directories
for each build type (Debug, Release, etc).

### Investigating build issues

If one of the `ext_xxx` projects fails to build
usually this means issues with external project (downloading or building).

See `build/<BUILD_TYPE>/cpp/extlibs-root/src/ext_xxx-stamp` directory for various `.log` files.


### Running API tests

Full build needs to be performed before running API tests.

`curl` should be available in `PATH`.

Additionally player binaries should be installed:

```
> scripts\install\foobar2000.cmd v2.24-x64
```


This command supports various parameters via environment variables:

`BEEFWEB_TEST_BUILD_TYPE` - which build type to use (defaults to `Debug`)

`BEEFWEB_TEST_FOOBAR2000_VERSION` - foobar2000 version to use (defaults to `v2.24-x64`)

These two variables could be useful if you don't want to use default directory layout `build/<BUILD_TYPE>`:

`BEEFWEB_BINARY_DIR` - CMake binary directory (defaults to `build/<BUILD_TYPE>`)

`BEEFWEB_BINARY_DIR_BASE` - parent of CMake binary directories for all build types (defaults to `build`)

When both are specified `BEEFWEB_BINARY_DIR` takes precedence.

If the specified path is not absolute it is resolved relative to source code root directory.

When building frontend these two variables could be used instead of `--env outputDir`.
