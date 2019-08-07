import re
from distutils.core import setup, Extension
from os.path import join, dirname

from setuptools import find_packages
from Cython.Build import build_ext

__version__ = 'v0.1.0-dev'

extensions = [
    Extension(
        'lobq',
        ['lobq/lobq.c'],
    ),
    Extension(
        "cyq",
        ["cyq/cyq.pyx"]
    )
]

dependencies = [
    'numpy',
    'pandas',
    'cython',
    'jupyterlab',

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
    test_suit='sad.tests',
    extra_compile_args = ["-O3"],
)
