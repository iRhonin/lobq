from distutils.core import setup, Extension
from setuptools import find_packages


extensions = [
    Extension(
        "fastqueue",
        ["lobqueue/src/queue.c"],
    ),
]

dependencies = [
    'numpy',
]

setup(
    name="lobqueue",
    version="0.1",
    install_requires=dependencies,
    packages=find_packages(),
    ext_modules=extensions,
)
