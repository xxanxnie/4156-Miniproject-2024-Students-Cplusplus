# Welcome Students of 4156
Please follow the assignment specifications on Courseworks when completing this project.

# Development Environment

Platform: Mac M2  
Compiler: Apple Clang version 14.0.3 (clang-1403.0.22.14.1)  
Target: arm64-apple-darwin22.6.0

# Test Coverage Report

Coverage Tools:  
- GCOV: Coverage tool for C/C++ code  
- LCOV: Graphical front-end for GCOV

rm -rf *
make clean  
cmake ..  
make  
make run_tests  
make coverage  

# Static Analysis Tool
 
Done using Cppcheck:

brew install cppcheck

cd IndividualMiniprojectC++/src

then run the following command in the terminal:
cppcheck --enable=all --force \
    --suppress=missingIncludeSystem \
    --suppress=syntaxError \
    --std=c++14 \
    --suppress="*:../external_libraries/Crow-1.2.0-Darwin/*" \
    --suppress="*:../external_libraries/boost_1_86_0/*" \
    *.cpp > ../cppcheck-result.txt

result of cppcheck will be outputed to cppcheck-result.txt
