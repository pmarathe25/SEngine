#!/usr/bin/env python3
import sbuildr
import glob

project = sbuildr.Project()

for source in glob.iglob("tests/**/*.cpp", recursive=True):
    project.test(source.split('.')[0], sources=[source], libs=["stdc++", "stest"])

sbuildr.cli(project, default_profiles=["debug"])
