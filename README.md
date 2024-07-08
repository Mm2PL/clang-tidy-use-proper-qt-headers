# UseProperQtHeaders

This is a `clang-tidy` plugin that warns on use of includes like `<qstring.h>`.

## Compilation

This should be as simple as `mkdir build && cd build && cmake .. && make -j$(nproc)`.

## Usage

To use this plugin:
 - Ensure it's loaded it into your clang-tidy. Add `-load
   libUseProperQtHeaders.so` to your clang-tidy command.
 - Ensure that the check `pqh-useproperqtheaders` is enabled.

## Licensing

This project is licensed under GPL v3 exclusively.

As a starting off point I used an example [from Coveo Open-Source
clang-tidy-plugin-examples repo][example]. That repo is available under the
Apache license.

The `.clang-format` was shamelessly taken from [Chatterino, which is licensed
under MIT][c2license].

[example]: https://github.com/coveooss/clang-tidy-plugin-examples/
[c2license]: https://github.com/Chatterino/chatterino2/blob/master/LICENSE
