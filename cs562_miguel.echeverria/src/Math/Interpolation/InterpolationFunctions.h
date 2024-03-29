/**
* @file InterpolationFunctions.h
* @author Miguel Echeverria , 540000918 , miguel.echeverria@digipen.edu
* @date 2020/15/10
* @brief Contains some functions for interpolating between two values given a normalized parameter.
*
* @copyright Copyright (C) 2020 DigiPen Institute of Technology .
*/

#pragma once



namespace cs460
{
	// Linear interpolation between start and end based on a normalized parameter tn
	template<typename T>
	T lerp(const T& start, const T& end, float tn)
	{
		return start + (end - start) * tn;
	}

	template<typename T>
	T lerp_first_derivative(const T& start, const T& end, float tn)
	{
		return end - start;
	}



	// Interpolation between start and end using a cubic hermite spline based on a normalized parameter tn.
	// startTangent and endTangent are the tangents of start and end respectively
	template<typename T>
	T hermite_interpolation(const T& start, const T& startTangent, const T& end, const T& endTangent, float tn)
	{
		float tnSquared = tn * tn;
		float tnCubed = tn * tnSquared;

		const T& cubicCoefficient = (start - end) * 2.0f + startTangent + endTangent;
		const T& squareCoefficient = (end - start) * 3.0f - startTangent * 2.0f - endTangent;
		const T& linearCoefficient = startTangent;
		const T& independentTerm = start;

		return cubicCoefficient * tnCubed + squareCoefficient * tnSquared + linearCoefficient * tn + independentTerm;
	}

	template<typename T>
	T hermite_first_derivative(const T& start, const T& startTangent, const T& end, const T& endTangent, float tn)
	{
		float tnSquared = tn * tn;
		float tnCubed = tn * tnSquared;

		const T& squareCoefficient = (start - end) * 2.0f + startTangent + endTangent;
		const T& linearCoefficient = (end - start) * 3.0f - startTangent * 2.0f - endTangent;
		const T& independentTerm = startTangent;

		return 3.0f * squareCoefficient * tnSquared + 2.0f * linearCoefficient * tn + independentTerm;
	}

	template<typename T>
	T hermite_second_derivative(const T& start, const T& startTangent, const T& end, const T& endTangent, float tn)
	{
		float tnSquared = tn * tn;
		float tnCubed = tn * tnSquared;

		const T& linearCoefficient = (start - end) * 2.0f + startTangent + endTangent;
		const T& independentTerm = (end - start) * 3.0f - startTangent * 2.0f - endTangent;

		return 6.0f * linearCoefficient * tn + 2.0f * independentTerm;
	}



	// Interpolation between start and end using a cubic spline (Bezier curve), based on a normalized parameter tn.
	// control1 and control2 are the control points for start and end respectively. These are approximated instead
	// of interpolated.
	template<typename T>
	T bezier_interpolation(const T& start, const T& control1, const T& control2, const T& end, float tn)
	{
		float invTn = 1 - tn;
		float factor0 = invTn * invTn * invTn;
		float factor1 = 3.0f * tn * invTn * invTn;
		float factor2 = 3.0f * tn * tn * invTn;
		float factor3 = tn * tn * tn;

		return factor0 * start + factor1 * control1 + factor2 * control2 + factor3 * end;
	}

	template<typename T>
	T bezier_first_derivative(const T& start, const T& control1, const T& control2, const T& end, float tn)
	{
		float tnSq = tn * tn;
		float invTn = 1.0f - tn;
		float invTnSq = invTn * invTn;
		float factor0 = -3.0f * invTnSq;
		float factor1 = 3.0f * invTnSq - 6.0f * tn * invTn;
		float factor2 = 6.0f * tn * invTn - 3.0f * tnSq;
		float factor3 = 3.0f * tnSq;

		return factor0 * start + factor1 * control1 + factor2 * control2 + factor3 * end;
	}

	template<typename T>
	T bezier_second_derivative(const T& start, const T& control1, const T& control2, const T& end, float tn)
	{
		float invTn = 1 - tn;
		float factor0 = 6.0f * invTn;
		float factor1 = -12.0f * invTn + 6.0f * tn;
		float factor2 = 6.0f * invTn - 12.0f * tn;
		float factor3 = 6.0f * tn;

		return factor0 * start + factor1 * control1 + factor2 * control2 + factor3 * end;
	}



	


	// ------------------------------------- PIECEWISE INTERPOLATION FUNCTIONS ----------------------------------------

	// Linearly interpolate between a set of keyframes (assume keyframes will be vec3s) based
	// on a time value t, which doesn't need to be normalized.
	glm::vec3 piecewise_lerp(const std::vector<float>& keys, const std::vector<float>& values, float t, unsigned derivativeOrder = 0);


	// Perform spherical linear interpolation between a set of keyframes based on a time value t, which doesn't
	// need to be normalized. Assume the values given as data are quaternions (4 floating point values)
	glm::quat piecewise_slerp(const std::vector<float>& keys, const std::vector<float>& values, float t);


	// Use step interpolation method from gltf to get a value
	glm::vec3 piecewise_step(const std::vector<float>& keys, const std::vector<float>& values, float t);


	// Perform hermite cubic spline interpolation between a set of keyframes based on a time value t, which doesn't
	// need to be normalized. Assumes the values given as data are vec3s (3 floating point values) -> 3 vec3s per
	// time key: in-tangent, property, out-tangent
	glm::vec3 piecewise_hermite(const std::vector<float>& keys, const std::vector<float>& values, float t, unsigned derivativeOrder = 0);


	// Perform catmull-rom cubic spline interpolation between a set of keyframes based on a time value t, which doesn't
	// need to be normalized. Assumes the values given as data are vec3s (3 floating point values) -> 1 vec3 per time
	// key: property. The in-tangent and out-tangent are computed in this function from the given values.
	glm::vec3 piecewise_catmull_rom(const std::vector<float>& keys, const std::vector<float>& values, float t, unsigned derivativeOrder = 0);


	// Perform bezier curve interpolation between a set of keyframes based on a time value t, which doesn't need
	// to be normalized. Assumes the values given as data are vec3s (3 floating point values) -> 3 vec3s per time
	// key: in-control_point, property, out-control_point
	glm::vec3 piecewise_bezier(const std::vector<float>& keys, const std::vector<float>& values, float t, unsigned derivativeOrder = 0);

	// ------------------------------------- PIECEWISE INTERPOLATION FUNCTIONS ----------------------------------------
}