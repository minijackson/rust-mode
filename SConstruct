#!/usr/bin/python

env = Environment(CXXFLAGS=["-std=c++11", "-Wall", "-Wextra", "-fdiagnostics-color=always"])

libmode = ARGUMENTS.get("libmode", "release")

if libmode == "release":
    env["CXXFLAGS"].extend(["-O2"])
elif libmode == "debug":
    env["CXXFLAGS"].extend(["-g"])
elif libmode == "profile":
    env["CXXFLAGS"].extend(["-pg"])
    env["LINKFLAGS"] = ["-pg"]
elif libmode == "coverall":
    env["CXXFLAGS"].extend(["-fprofile-arcs", "-ftest-coverage"])
    env["LIBS"] = ["gcov"]
else:
    print("Error: unknown libmode value")

env.Library("rust-mode", Glob("./src/**/*.cpp"))
env.SharedLibrary("rust-mode", Glob("./src/**/*.cpp"))

SConscript(["tests/SConscript"])
