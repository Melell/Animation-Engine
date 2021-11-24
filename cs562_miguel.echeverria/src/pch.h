#pragma once

// STL libraries
#include <iostream>
#include <vector>
#include <string>
#include <list>
#include <unordered_map>
#include <map>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <functional>

namespace fs = std::filesystem;

// GLM header files
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// IMGUI/IMGUIZMO
#include <imgui/imgui.h>
#include <imgui/ImGuizmo.h>

// Project files
#include "Utilities/DebugCallbacks.h"
#include "Utilities/Screenshot.h"
#include "Utilities/Rtti.h"
#include "Composition/IBase.h"
#include "Composition/ISerializable.h"