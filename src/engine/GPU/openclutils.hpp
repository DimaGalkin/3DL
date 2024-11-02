#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 300
#include <CL/opencl.hpp>

#include <string>
#include <vector>

/**
 * @brief 3D Graphics Library
 *
 * ThreeDL is a simple 3D Graphics Library that allows OBJ files to be light drawn and
 * rendered with minimal code written by the user.
*/
namespace ThreeDL {
    /**
     * @breif Internal class used by the library to aid with loading and building OpenCL C
     * kernels. This class is also responsible for setting up OpenCL.
    */
    class OpenCLUtils {
        public:
            /**
             * @breif Finds and initialieses a GPU device.
             *
             * Debug info is printed about the amount of memory the selected device has.
            */
            explicit OpenCLUtils();

            /**
             * @breif Reads the contents of the GPU source code at compile time
             *
             * @return string containing all code that will be ran on the GPU
            */
            static std::string readKernel();

            /**
             * @breif Compiles the GPU code loaded to make it excecutable
             *
             * @return Executable program
            */
            cl::Program buildProgram() const;

            cl::CommandQueue queue_; // Queue of commands to execute
            cl::Context context_; // OpenCL context for creation of queues
            cl::Device device_; // Selected GPU to render scene
            cl::Platform platform_; // Platform GPU was selected from

            ~OpenCLUtils() = default;

        private:
            std::vector<cl::Platform> platforms_; // All possible platforms
            std::vector<cl::Device> devices_; // All possible GPUs

            bool checkSVMCompatibility() const;
    };
};