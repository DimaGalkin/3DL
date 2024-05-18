#pragma once

#include "GPU/types.h"
#include "objects.hpp"
#include "utils.hpp"
#include "linalg.hpp"

namespace ThreeDL {
    /**
     * Encapsulates all parameters required to define a light source in the scene. Has a method to easily convert to a
     * form that can be read by the OpenCL kernel. Designed to be inherited from to create specific types of lights.
    */
    class Light {
        public:
					Light();

					/**
					 * Exports the member variables of the class into a struct that is defined in GPU/types.h.
					*/
					GPULight asGPUType() const;

					/**
					 * Returns the reference to the position of the model so that it may be changed by the user.
					*/
					ThreeDL::vec3& getModelPosition() { return model_.position_; }

					/**
					 * Returns values restrict the user from being able to change protected values. Changing these
					 * values will result in undefined behavior.
					*/
					std::string getName() const { return name_; }
					LightType getType() const { return type_; }
					Object getModel() const { return model_; }

					vec3 position_;
					vec3 direction_; // direction vector of light

					bool model_enabled_ = true;

					float intensity_;
					float color_[3]; // RGB color of light
					float fov_; // field of view of light in degrees

					bool shadows_enabled_ = true;
					uint32_t shadowmap_w_ = 1024;
					uint32_t shadowmap_h_ = 1024;

					~Light() = default;

        protected:
		        Object model_; // stores the model of the light source

		        std::string name_; // name of light to make it identifiable in the GUI

		        LightType type_; // Directional, Point, Ambient
    };

    /**
     * Amibient light source that illuminates the entire scene with a constant color and intensity. It does not have a
     * position or direction, and does not cast shadows.
    */
    class AmbientLight : public Light {
        public:
            AmbientLight(const std::string& name, const uint32_t color, const float intensity);
            AmbientLight() = delete;

            ~AmbientLight() = default;
    };

    /**
     * Point light source that emits light in all directions from a single point in space. It has a position, color,
     * intensity, and can cast shadows. It does not have a direction, and does not have a field of view.
    */
    class PointLight : public Light {
        public:
            PointLight(const std::string& name, const vec3& position, const uint32_t color, const float intensity);
            PointLight() = delete;

            ~PointLight() = default;
    };

		/**
		 * Directional light source that emits light in a single direction from a single point in space. It has a direction,
		 * position, color, intensity, and can cast shadows. It has a field of view.
		*/
    class DirectionalLight : public Light {
        public:
            DirectionalLight(const std::string& name, const vec3& direction, const vec3& position, const float fov, const uint32_t color, const float intensity);
            DirectionalLight() = delete;

            ~DirectionalLight() = default;
    };
};