import re
from distutils.core import setup, Extension
from os.path import join, dirname

from setuptools import find_packages
from Cython.Build import build_ext

__version__ = '0.1.1.dev0'

extensions = [
    Extension(
        'lobq',
        ['lobq/lobq.c'],
    ),
    Extension(
        "cyq",
        ["lobq_profiler/cyq/cyq.pyx"]
    )
]

dependencies = [
    'numpy',
    'pandas',
    'cython',

    # For running tests
    'pytest',
]

setup(
    name='lobq',
    version=__version__,
    install_requires=dependencies,
    packages=find_packages(),
    cmdclass = {'build_ext': build_ext},
    ext_modules=extensions,
    test_suit='tests',
    extra_compile_args = ["-O3"],
)
