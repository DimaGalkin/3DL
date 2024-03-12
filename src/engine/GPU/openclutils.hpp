#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include <iostream>

#include <string>
#include <vector>
#include <fstream>

namespace ThreeDL {
    class OpenCLUtils {
    public:
        explicit OpenCLUtils();

        static std::string readKernel(const std::string& path);
        cl::Program buildProgram(const std::string& kernel, const std::string& path) const;

        cl::CommandQueue queue_;
        cl::Context context_;
        cl::Device device_;
        cl::Platform platform_;

        ~OpenCLUtils() = default;

    private:
        std::vector<cl::Platform> platforms_;
        std::vector<cl::Device> devices_;

        bool checkSVMCompatibility() const;
    };
};