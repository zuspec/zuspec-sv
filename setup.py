#****************************************************************************
#* setup.py for zuspec-arl-eval
#****************************************************************************
import os
import sys
from setuptools import Extension, find_namespace_packages


proj_dir = os.path.dirname(os.path.abspath(__file__))

try:
    sys.path.insert(0, os.path.join(proj_dir, "python/zsp_sv"))
    from __version__ import VERSION, BASE
    base = BASE
    version = VERSION
except ImportError as e:
    print("No build num: %s" % str(e))
    base="0.0.1"
    version=base

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
  packages=find_namespace_packages(where='python'),
  package_dir={'' : 'python'},
  author = "Matthew Ballance",
  author_email = "matt.ballance@gmail.com",
  description = ("Core ARL data model library"),
  long_description = """
  Provides a library interface for creating and evaluating ARL models at an API level
  """,
  license = "Apache 2.0",
  keywords = ["SystemVerilog", "Verilog", "RTL", "Python"],
  url = "https://github.com/zuspec/zuspec-sv",
  package_data = {'zsp_arl_eval': [
      'core.pxd',
      'decl.pxd'
  ]},
  install_requires=[
    "zuspec-parser>=%s" % base, 
    "zuspec-fe-parser>=%s" % base, 
    "zuspec-arl-dm>=%s" % base,
    "zuspec-arl-eval>=%s" % base,
    'zuspec-cli'
  ],
  entry_points={
      'ivpm.pkginfo': [
          'zuspec-sv = zsp_sv.pkginfo.PkgInfo'
      ], 
      "zuspec.ext": [
          'ext = zsp_sv.__ext__'
      ],
      "dv_flow.mgr": [
          'zsp = zsp_sv.__dfm__'
      ]
  },
  setup_requires=[
    'cython',
    'ivpm',
    'ciostream',
    'debug-mgr',
    'vsc-dm',
    'zuspec-arl-dm',
    'zuspec-arl-eval',
    'zuspec-parser',
    'zuspec-fe-parser',
    'setuptools_scm',
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

