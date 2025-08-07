from skbuild import setup
import pybind11

setup(
    name="vrp_solver",
    version="0.1.0",
    description="Vehicle routing problem solver module for using C++ algorithms in Python",
    author="Cooper Gau",
    packages=["vrp_solver"],
    cmake_source_dir=".",
    python_requires=">=3.7",
    cmake_args=[
        f"-Dpybind11_DIR={pybind11.get_cmake_dir()}"
    ],
)
