/**
* @file EasingFunctions.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/01/11
* @brief Ease linear, ease in/out etc.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/


#pragma once


namespace cs460
{
	// Returns tn
	float ease_linear(float tn);

	// Accelerate until accelerateEndTime, linear between accelerateEndTime and
	// deccelerateStartTime, and deccelerate between deccelerateStartTime and end
	float ease_in_out(float tn, float accelerateEndTime = 0.3f, float deccelerateStartTime = 0.7f);
}