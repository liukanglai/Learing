cmake_minimum_required(VERSION 3.19)
project(demo_cuda CUDA)

set(CMAKE_CUDA_STANDARD 14)

add_executable(demo_cuda main.cu)

set_target_properties(
        demo_cuda
        PROPERTIES
        CUDA_SEPARABLE_COMPILATION ON)