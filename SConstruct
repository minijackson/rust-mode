#!/usr/bin/python

env = Environment(CXXFLAGS=["-std=c++11", "-O2", "-Wall", "-Wextra", "-fdiagnostics-color=always"])
env.Library("rust-mode", Glob("./src/**/*.cpp"))
env.SharedLibrary("rust-mode", Glob("./src/**/*.cpp"))

SConscript(["tests/SConscript"])
