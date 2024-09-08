#****************************************************************************
#* setup.py for zuspec-arl-eval
#****************************************************************************
import os
import sys
from setuptools import Extension, find_namespace_packages

version="0.0.1"

proj_dir = os.path.dirname(os.path.abspath(__file__))

try:
    sys.path.insert(0, os.path.join(proj_dir, "python/zsp_sv"))
    from __build_num__ import BUILD_NUM
    version += ".%s" % str(BUILD_NUM)
except ImportError as e:
    print("No build num: %s" % str(e))

isSrcBuild = False
try:
    from ivpm.setup import setup
    print("Found IVPM")
    isSrcBuild = os.path.isdir(os.path.join(proj_dir, "src"))
except ImportError as e:
    from setuptools import setup
    print("Failed to load IVPM: %s" % str(e))

zuspec_sv_dir = proj_dir

ext = Extension("zsp_sv.core",
            sources=[
                os.path.join(zuspec_sv_dir, 'python', "core.pyx"), 
            ],
            language="c++",
            include_dirs=[
#                os.path.join(zuspec_sv_dir, 'src'),
                os.path.join(zuspec_sv_dir, 'python'),
                os.path.join(zuspec_sv_dir, 'src', 'include'),
                # os.path.join(packages_dir, 'vsc-dm', 'src', 'include'),
                # os.path.join(packages_dir, 'vsc-dm', 'python'),
                # os.path.join(packages_dir, 'vsc-solvers', 'src', 'include'),
                # os.path.join(packages_dir, 'vsc-solvers', 'python'),
                # os.path.join(packages_dir, 'zuspec-arl-dm', 'src', 'include'),
                # os.path.join(packages_dir, 'zuspec-arl-dm', 'python'),
                # os.path.join(packages_dir, 'debug-mgr', 'src', 'include'),
                # os.path.join(packages_dir, 'debug-mgr', 'python'),
            ]
        )
ext.cython_directives={'language_level' : '3'}

setup_args = dict(
  name = "zuspec-sv",
  version=version,
  packages=['zsp_sv'],
  package_dir = {'' : 'python'},
  author = "Matthew Ballance",
  author_email = "matt.ballance@gmail.com",
  description = ("Core ARL data model library"),
  long_description = """
  Provides a library interface for creating and evaluating ARL models at an API level
  """,
  license = "Apache 2.0",
  keywords = ["SystemVerilog", "Verilog", "RTL", "Python"],
  url = "https://github.com/zuspec/zuspec-sv",
  install_requires=[
    'zuspec-parser',
    'zuspec-fe-parser',
    'zuspec-arl-dm',
    'zuspec-arl-eval',
    'zuspec-cli'
  ],
  entry_points={
      'ivpm.pkginfo': [
          'zuspec-sv = zsp_sv.pkginfo.PkgInfo'
      ], 
      "zuspec.ext": [
          'ext = zsp_sv.__ext__'
      ]
  },
  setup_requires=[
    'setuptools_scm',
    'cython',
  ],
  ext_modules=[ ext ]
)

if isSrcBuild:
    setup_args["ivpm_extdep_pkgs"] = [
        "pyapi-compat-if",
        "zuspec-parser", 
        "zuspec-fe-parser", 
        "zuspec-arl-dm",
        "zuspec-arl-eval",
        "vsc-solvers",
        "vsc-dm"]
    setup_args["ivpm_extra_data"] = {
        "zsp_sv": [
            ("src/include", "share"),
            ("build/{libdir}/{libpref}zsp-sv{dllext}", ""),
        ]
    }

setup(**setup_args)

