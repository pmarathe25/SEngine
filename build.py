#!/usr/bin/env python3
import sbuildr
import sbuildr.dependencies.builders as builders
import sbuildr.dependencies.fetchers as fetchers

import glob
import os

cppstdlib = sbuildr.Library("stdc++")
project = sbuildr.Project()

slog = sbuildr.dependencies.Dependency(fetchers.GitFetcher("https://github.com/pmarathe25/SLog", tag="v0.2.1"), builders.SBuildrBuilder())

stest = sbuildr.dependencies.Dependency(fetchers.GitFetcher("https://github.com/pmarathe25/STest", tag="v0.3.2"), builders.SBuildrBuilder())
for source in glob.iglob("tests/**/*.cpp", recursive=True):
    project.test(os.path.splitext(os.path.basename(source))[0], sources=[source], libs=[cppstdlib, stest.library("stest")])

project.export()
