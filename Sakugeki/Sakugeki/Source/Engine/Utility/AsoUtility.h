#pragma once
#include <vector>
#include <string>
#include <DxLib.h>
#include "../Math/Vector2.h"
#include "../Math/Quaternion.h"

class AsoUtility
{

public:

	// 文字列の分割
	static std::vector<std::string> Split(std::string& line, char delimiter);
};

