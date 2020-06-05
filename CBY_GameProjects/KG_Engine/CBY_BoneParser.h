#pragma once
#include "CBY_Parser.h"

namespace CBY
{
	class CBY_BoneParser :public CBY_Parser
	{
	public:
		bool Load(const TCHAR* szFileName);

	public:
		CBY_BoneParser();
		virtual ~CBY_BoneParser();
	};
}

