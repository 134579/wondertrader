// WtRunner.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "WtRunner.h"
#include "..\WTSTools\WTSLogger.h"
#include "..\Share\IniHelper.hpp"

//#include <vld.h>

void main()
{
	WtRunner runner;
	runner.init();

	runner.config("config.json");

	runner.run(false);
}

