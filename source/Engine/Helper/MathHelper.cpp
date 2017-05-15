#pragma once
#include "stdafx.hpp"
#include "MathHelper.hpp"

string to_string(vec3 in)
{
	return to_string(in.x) + ", " + to_string(in.y) + ", " + to_string(in.z);
}

std::vector<glm::vec3> GetIcosahedronPositions(float size)
{
	float ratio = (1.f + sqrt(5.f)) / 2.f;
	float scale = size / glm::length(glm::vec2(ratio, 1.f));
	ratio *= scale;

	std::vector<glm::vec3> ico;
	//X plane
	ico.push_back(glm::vec3(ratio, 0, -scale));		//rf 0
	ico.push_back(glm::vec3(-ratio, 0, -scale));	//lf 1
	ico.push_back(glm::vec3(ratio, 0, scale));		//rb 2
	ico.push_back(glm::vec3(-ratio, 0, scale));		//lb 3 
	//Y plane													 
	ico.push_back(glm::vec3(0, -scale, ratio));		//db 4
	ico.push_back(glm::vec3(0, -scale, -ratio));	//df 5
	ico.push_back(glm::vec3(0, scale, ratio));		//ub 6
	ico.push_back(glm::vec3(0, scale, -ratio));		//uf 7
	//Z plane													 
	ico.push_back(glm::vec3(-scale, ratio, 0));		//lu 8
	ico.push_back(glm::vec3(-scale, -ratio, 0));	//ld 9
	ico.push_back(glm::vec3(scale, ratio, 0));		//ru 10
	ico.push_back(glm::vec3(scale, -ratio, 0));		//rd 11

	return ico;
}
std::vector<uint32> GetIcosahedronIndices()
{
	std::vector<uint32> ret
	{
		1, 3, 8,
		1, 3, 9,
		0, 2, 10,
		0, 2, 11,

		5, 7, 0,
		5, 7, 1,
		4, 6, 2,
		4, 6, 3,

		9, 11, 4,
		9, 11, 5,
		8, 10, 6,
		8, 10, 7,

		1, 7, 8,
		1, 5, 9,
		0, 7, 10,
		0, 5, 11,

		3, 6, 8,
		3, 4, 9,
		2, 6, 10,
		2, 4, 11
	};
	return ret;
}
std::vector<uint32> GetIcosahedronIndicesBFC()
{
	std::vector<uint32> ret
	{
		1,8,3,
		1,3,9,
		0,2, 10,
		0,11,2,

		5,0,7,
		5,7,1,
		4,6,2,
		4,3,6,

		9, 4,11,
		9,11,5,
		8,10,6,
		8, 7,10,

		1,7,8,
		1,9,5,
		0,10,7,
		0,5,11,

		3,8,6,
		3,4,9,
		2,6 ,10,
		2,11,4
	};
	return ret;
}