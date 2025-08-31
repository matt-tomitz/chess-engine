from setuptools import setup
from distutils.extension import Extension
from Cython.Build import cythonize

extensions = [
    Extension("pyposition", sources=["pyposition.pyx"], extra_compile_args=["-O3"], language="c++")
]
setup(
    name='pyposition',
    ext_modules=cythonize(extensions),
)