import re
from distutils.core import setup, Extension
from os.path import join, dirname

from setuptools import find_packages


# reading package version (same way the sqlalchemy does)
with open(join(dirname(__file__), 'lobqueue', '__init__.py')) as v_file:
    package_version = re.\
        compile(r".*__version__ = '(.*?)'", re.S).\
        match(v_file.read()).\
        group(1)

extensions = [
    Extension(
        'fastqueue',
        ['lobqueue/src/queue.c'],
    ),
]

dependencies = [
    'numpy',
]

setup(
    name='lobqueue',
    version=package_version,
    install_requires=dependencies,
    packages=find_packages(),
    ext_modules=extensions,
)
