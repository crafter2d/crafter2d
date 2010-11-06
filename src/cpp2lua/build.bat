
@echo off
echo "building language parser..."

bison -d language.bison -o language.c
flex -I language.flex