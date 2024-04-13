#include "openclutils.hpp"

ThreeDL::OpenCLUtils::OpenCLUtils() {
    cl::Platform::get(&platforms_);
    if (platforms_.empty()) {
        throw std::runtime_error("No OpenCL platforms found");
    }

    platform_ = platforms_[0];
    std::cout << "Using platform: " << platform_.getInfo<CL_PLATFORM_NAME>() << std::endl;

    platform_.getDevices(CL_DEVICE_TYPE_ALL, &devices_);
    if (devices_.empty()) {
        throw std::runtime_error("No OpenCL devices found");
    }

    device_ = devices_[0];
    std::cout << "Using device: " << device_.getInfo<CL_DEVICE_NAME>() << std::endl;

    bool svm_supported = checkSVMCompatibility();
    if (!svm_supported) {
        throw std::runtime_error("SVM not supported on this device");
    }

    context_ = cl::Context({device_});
    queue_ = cl::CommandQueue(context_, device_);
}

std::string ThreeDL::OpenCLUtils::readKernel(const std::string& path) {
//    std::ifstream kernel_file { path };
//
//    if (!kernel_file.is_open()) {
//        throw std::runtime_error("Failed to open file for reading: " + path);
//    }
//
//    std::string kernel_source {std::istreambuf_iterator<char>(kernel_file), std::istreambuf_iterator<char>()};
//
//    kernel_file.close();
    #include "iact.h"
    return kernel_include_source;
}

cl::Program ThreeDL::OpenCLUtils::buildProgram(const std::string& kernel, const std::string& path) const {
    std::string kernel_source = readKernel(path);

    cl::Program::Sources sources;
    sources.emplace_back(kernel_source.c_str(), kernel_source.length());

    cl::Program program (context_, sources);

    if (program.build(devices_) != CL_SUCCESS) {
        throw std::runtime_error(program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device_));
    }

    return program;
}

bool ThreeDL::OpenCLUtils::checkSVMCompatibility() const {
    return device_.getInfo<CL_DEVICE_SVM_CAPABILITIES>() & CL_DEVICE_SVM_COARSE_GRAIN_BUFFER;
}
