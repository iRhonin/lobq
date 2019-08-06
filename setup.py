import re
from distutils.core import setup, Extension
from os.path import join, dirname

from setuptools import find_packages
from Cython.Build import build_ext


# reading package version (same way the sqlalchemy does)
with open(join(dirname(__file__), 'lobqueue', '__init__.py')) as v_file:
    package_version = re.\
        compile(r".*__version__ = '(.*?)'", re.S).\
        match(v_file.read()).\
        group(1)

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
    'cython',
]

setup(
    name='lobqueue',
    version=package_version,
    install_requires=dependencies,
    packages=find_packages(),
    cmdclass = {'build_ext': build_ext},
    ext_modules=extensions,
    test_suit='sad.tests',
    extra_compile_args = ["-O0"],
)
