# Make sure LLVM is installed and the bin directory is set in PATH.
# Run the following command from the root directory of the project within bash terminal.
# ./contrib/run_clang-format.sh

find \
    source/QuickCut \
    source/QuickCutConsole \
    source/QuickCutService \
    source/QuickCutShared \
    -regex '.*\.\(cpp\|h\|hpp\|cc\|cxx\)' -exec \
    clang-format -style=file -i {} \;
