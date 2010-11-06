
@echo off
echo "building language parser..."

bison --defines=language.h language.bison -o language.cpp
flex -I -olanguage.lex.cpp language.flex 