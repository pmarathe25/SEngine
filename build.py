#!/usr/bin/env python3
import sbuildr
import glob

project = sbuildr.Project(version="0.1.0")

libsengine = project.library("sengine", sources=glob.glob("src/**/*.cpp", recursive=True), libs=["stdc++", "slog"])

for source in glob.iglob("tests/*.cpp"):
    project.test(source.split('.')[0], sources=[source], libs=["stdc++", "stest", libsengine])

sbuildr.cli(project, default_profiles=["debug"])
