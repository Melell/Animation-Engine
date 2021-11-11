/**
* @file EasingFunctions.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Ease linear, ease in/out etc.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "EasingFunctions.h"


namespace cs460
{
	// Returns tn
	float ease_linear(float tn)
	{
		return tn;
	}

	// Accelerate until accelerateEndTime, linear between accelerateEndTime and
	// deccelerateStartTime, and deccelerate between deccelerateStartTime and end
	float ease_in_out(float tn, float accelerateEndTime, float deccelerateStartTime)
	{
		// For convenience
		float t1 = accelerateEndTime;
		float t2 = deccelerateStartTime;
		float pi = glm::pi<float>();
		float result = 0.0f;
		float normalizedResult = 0.0f;

		// Accelerating region
		if (tn <= t1)
		{
			float angle = (tn * pi) / (t1 * 2.0f) - pi / 2.0f;
			float factor = (t1 * 2.0f / pi);
			result = factor * (sin(angle) + 1.0f);
		}
		// Deccelerating region
		else if (tn >= t2)
		{
			float start = 2.0f * t1 / pi + t2 - t1;
			float angle = (tn - t2) * pi / (2.0f * (1.0f - t2));
			float factor = (1.0f - t2) * 2.0f / pi;
			result = start + factor * sin(angle);
		}
		// Linear region
		else
		{
			float start = 2.0f * t1 / pi;
			result = start + tn - t1;
		}

		// Normalize curve
		float normalizingDivisor = 2.0f * t1 / pi + t2 - t1 + 2.0f * (1.0f - t2) / pi;
		normalizedResult = result / normalizingDivisor;
		return normalizedResult;
	}
}