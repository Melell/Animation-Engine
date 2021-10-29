/**
* @file InterpolationFunctions.cpp
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Contains some functions for interpolating between two values given a normalized parameter.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#include "pch.h"
#include "InterpolationFunctions.h"


namespace cs460
{
	// Linearly interpolate between a set of keyframes (assume keyframes will be vec3s) based
	// on a time value t, which doesn't need to be normalized.
	glm::vec3 piecewise_lerp(const std::vector<float>& keys, const std::vector<float>& values, float t)
	{
		const int componentCount = 3;

		// Clamp the value
		if (t < keys[0])
			return glm::make_vec3(values.data());

		if (t > keys.back())
			return glm::make_vec3(&values[values.size() - componentCount]);


		unsigned frameIdx = 1;
		while (t > keys[frameIdx])
			++frameIdx;

		// Get the endpoints of the segment we are in
		glm::vec3 val0 = glm::make_vec3(values.data() + (frameIdx - 1) * componentCount);
		glm::vec3 val1 = glm::make_vec3(values.data() + frameIdx * componentCount);

		// Normalize the time according to the current interval
		float intervalDuration = keys[frameIdx] - keys[frameIdx - 1];
		float localTime = t - keys[frameIdx - 1];
		float tn = localTime / intervalDuration;

		return lerp(val0, val1, tn);
	}


	// Perform spherical linear interpolation between a set of keyframes based on a time value t, which doesn't
	// need to be normalized. Assume the values given as data are quaternions (4 floating point values)
	glm::quat piecewise_slerp(const std::vector<float>& keys, const std::vector<float>& values, float t)
	{
		const int componentCount = 4;

		// Clamp the value
		if (t < keys[0])
			return glm::make_quat(values.data());

		if (t > keys.back())
			return glm::make_quat(&values[values.size() - componentCount]);


		unsigned frameIdx = 1;
		while (t > keys[frameIdx])
			++frameIdx;

		// Get the endpoints of the segment we are in
		glm::quat val0 = glm::make_quat(values.data() + (frameIdx - 1) * componentCount);
		glm::quat val1 = glm::make_quat(values.data() + frameIdx * componentCount);

		// Normalize the time according to the current interval
		float intervalDuration = keys[frameIdx] - keys[frameIdx - 1];
		float localTime = t - keys[frameIdx - 1];
		float tn = localTime / intervalDuration;

		return glm::slerp(val0, val1, tn);
	}


	// Use step interpolation method from gltf to get a value
	glm::vec3 piecewise_step(const std::vector<float>& keys, const std::vector<float>& values, float t)
	{
		const int componentCount = 3;

		// Clamp the value
		if (t < keys[0])
			return glm::make_vec3(values.data());

		if (t > keys.back())
			return glm::make_vec3(&values[values.size() - componentCount]);


		unsigned frameIdx = 1;
		while (t > keys[frameIdx])
			++frameIdx;

		return glm::make_vec3(values.data() + (frameIdx - 1) * componentCount);
	}


	// Perform hermite cubic spline interpolation between a set of keyframes based on a time value t, which doesn't
	// need to be normalized. Assume the values given as data are vec3s (3 floating point values)
	glm::vec3 piecewise_hermite(const std::vector<float>& keys, const std::vector<float>& values, float t)
	{
		// Data will be given as: in-tangent, property, out-tangent

		const int componentCount = 3;

		// Clamp the value
		if (t < keys[0])
			return glm::make_vec3(values.data() + componentCount);

		if (t > keys.back())
			return glm::make_vec3(&values[values.size() - componentCount * 2]);


		unsigned frameIdx = 1;
		while (t > keys[frameIdx])
			++frameIdx;

		// Get the endpoints of the segment we are in
		const float* val0 = values.data() + (frameIdx - 1) * componentCount * 3;
		const float* val1 = values.data() + frameIdx * componentCount * 3;

		// Normalize the time according to the current interval
		float intervalDuration = keys[frameIdx] - keys[frameIdx - 1];
		float localTime = t - keys[frameIdx - 1];
		float tn = localTime / intervalDuration;

		const glm::vec3& start = glm::make_vec3(val0 + componentCount);
		const glm::vec3& startOutTangent = glm::make_vec3(val0 + componentCount * 2) * intervalDuration;
		const glm::vec3& end = glm::make_vec3(val1 + componentCount);
		const glm::vec3& endInTangent = glm::make_vec3(val1) * intervalDuration;

		return hermite_interpolation(start, startOutTangent, end, endInTangent, tn);
	}


	// Perform catmull-rom cubic spline interpolation between a set of keyframes based on a time value t, which doesn't
	// need to be normalized. Assumes the values given as data are vec3s (3 floating point values) -> 1 vec3 per time
	// key: property. The in-tangent and out-tangent are computed in this function from the given values.
	glm::vec3 piecewise_catmull_rom(const std::vector<float>& keys, const std::vector<float>& values, float t)
	{
		const int componentCount = 3;

		// Clamp the value
		if (t < keys[0])
			return glm::make_vec3(values.data());

		if (t > keys.back())
			return glm::make_vec3(&values[values.size() - componentCount]);


		int frameIdx = 1;
		while (t > keys[frameIdx])
			++frameIdx;

		// Get the endpoints of the segment we are in (val1 and 2, as well as the other necessary points for computing the tangents)
		glm::vec3 val0;
		glm::vec3 val1 = glm::make_vec3(values.data() + (frameIdx - 1) * componentCount);
		glm::vec3 val2 = glm::make_vec3(values.data() + frameIdx * componentCount);
		glm::vec3 val3;

		glm::vec3 tangent1;
		glm::vec3 tangent2;


		// Use the phantom point method for the tangent of the first point
		if (frameIdx - 2 < 0)
		{
			// First tangent with phantom point method
			val3 = glm::make_vec3(values.data() + (frameIdx + 1) * componentCount);
			tangent1 = 0.5f * ((val2 - val1) + (val2 - val3));

			// Second tangent
			tangent2 = 0.5f * (val3 - val1);
		}
		// Use the phantom point method for the tangent of the last point
		else if (frameIdx + 1 >= keys.size())
		{
			// First tangent
			val0 = glm::make_vec3(values.data() + (frameIdx - 2) * componentCount);
			tangent1 = 0.5f * (val2 - val0);

			// Second tangent with phantom point method
			tangent2 = -0.5f * ((val1 - val2) + (val1 - val0));
		}
		// Compute normally
		else
		{
			val0 = glm::make_vec3(values.data() + (frameIdx - 2) * componentCount);
			val3 = glm::make_vec3(values.data() + (frameIdx + 1) * componentCount);

			tangent1 = 0.5f * (val2 - val0);
			tangent2 = 0.5f * (val3 - val1);
		}


		// Normalize the time according to the current interval
		float intervalDuration = keys[frameIdx] - keys[frameIdx - 1];
		float localTime = t - keys[frameIdx - 1];
		float tn = localTime / intervalDuration;

		return hermite_interpolation(val1, tangent1, val2, tangent2, tn);
	}


	// Perform bezier curve interpolation between a set of keyframes based on a time value t, which doesn't need
	// to be normalized. Assumes the values given as data are vec3s (3 floating point values) -> 3 vec3s per time
	// key: in-control_point, property, out-control_point
	glm::vec3 piecewise_bezier(const std::vector<float>& keys, const std::vector<float>& values, float t)
	{
		// Data will be given as: in-control_point, property, out-control_point

		const int componentCount = 3;

		// Clamp the value
		if (t < keys[0])
			return glm::make_vec3(values.data() + componentCount);

		if (t > keys.back())
			return glm::make_vec3(&values[values.size() - componentCount * 2]);


		unsigned frameIdx = 1;
		while (t > keys[frameIdx])
			++frameIdx;

		// Get the endpoints of the segment we are in
		const float* val0 = values.data() + (frameIdx - 1) * componentCount * 3;
		const float* val1 = values.data() + frameIdx * componentCount * 3;

		// Normalize the time according to the current interval
		float intervalDuration = keys[frameIdx] - keys[frameIdx - 1];
		float localTime = t - keys[frameIdx - 1];
		float tn = localTime / intervalDuration;

		const glm::vec3& start = glm::make_vec3(val0 + componentCount);
		const glm::vec3& cp1 = glm::make_vec3(val0 + componentCount * 2);
		const glm::vec3& cp2 = glm::make_vec3(val1);
		const glm::vec3& end = glm::make_vec3(val1 + componentCount);

		return bezier_interpolation(start, cp1, cp2, end, tn);
	}
}