#include "pch.h"

#define STB_IMAGE_IMPLEMENTATION			// To instantiate later the stb_image implementation
#define STB_IMAGE_WRITE_IMPLEMENTATION		// To instantiate later the stb_image_write implementation
#define TINYGLTF_IMPLEMENTATION				// To instantiate later the tiny_gltf implementation

// Tell tinygltf to not include json.hpp, stb_image.h, and stb_image_write.h, as we will include them ourselves
#define TINYGLTF_NO_INCLUDE_JSON
#define TINYGLTF_NO_INCLUDE_STB_IMAGE
#define TINYGLTF_NO_INCLUDE_STB_IMAGE_WRITE

#include <stb/stb_image.h>
#include <stb/stb_image_write.h>
#include <json/json.hpp>

#include <gltf/tiny_gltf.h>