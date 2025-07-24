#!/bin/bash
find ./src -regex '.*\.\(cpp\|c\|h\|hpp\)' -exec clang-format -style=file -i {} +