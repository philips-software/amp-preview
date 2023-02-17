<div align="center">

# amp-preview (Possibly Real-time Embedded Views)

[![Continuous Integration](https://github.com/philips-software/amp-preview/workflows/Continuous%20Integration/badge.svg)](https://github.com/philips-software/amp-preview/actions) [![Linting & Formatting](https://github.com/philips-software/amp-preview/actions/workflows/linting-formatting.yml/badge.svg)](https://github.com/philips-software/amp-preview/actions/workflows/linting-formatting.yml) [![Static Analysis](https://github.com/philips-software/amp-preview/actions/workflows/static-analysis.yml/badge.svg)](https://github.com/philips-software/amp-preview/actions/workflows/static-analysis.yml)

[![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://choosealicense.com/licenses/mit/)

</div>

**Description**: amp-preview is a C++ framework for creating embedded user interfaces.

## Dependencies

Preview requires:
- A recent C++ compiler that supports C++17 at minimum (for a host build it should support std::filesystem).
- CMake 3.24 or higher.
- Embedded Infrastructure Library [philips-software.github.io/amp-embedded-infra-lib](https://philips-software.github.io/amp-embedded-infra-lib/).

Preview is know to build under the following configurations:
- Windows from Visual Studio 2017 onwards.
- Linux from GCC 7 onwards.
- OSX from XCode 11 and target platform 10.15 onwards.

## How to build the software

Preview can be built by itself, for example to execute the included micro-tests, or it can be built as part of a larger project. This paragraph describes how to build Preview by itself.

```
cmake -B Build
cmake --build Build
```

## How to test the software

After Preview has been built the included automated tests can be run with CTest like this:

```
ctest -D Experimental -C Debug
```

## Code examples

Code examples can be found under the [examples](examples) folder.

## Contributing

[![Conventional Commits](https://img.shields.io/badge/Conventional%20Commits-1.0.0-%23FE5196?logo=conventionalcommits&logoColor=white)](https://conventionalcommits.org)

amp-preview uses semantic versioning and conventional commits.

Please refer to our [Contributing](CONTRIBUTING.md) guide when you want to contribute to this project.

## License

amp-preview is licenced under the [MIT](https://choosealicense.com/licenses/mit/) license. See [LICENSE file](LICENSE.md).
