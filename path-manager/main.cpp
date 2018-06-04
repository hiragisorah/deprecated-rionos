#include <iostream>
#include <fstream>
#include "path-manager.h"

PathManager manager;
std::ofstream output;

unsigned int indent = 0;

void Indent(void)
{
	for (unsigned int n = 0; n < indent; ++n) output << "	";
}

void ScorpStart(void)
{
	Indent();
	output << "{" << std::endl;
	indent++;
}

void ScorpEnd(void)
{
	indent--;
	Indent();
	output << "}" << std::endl;
}

void ScorpEnds(void)
{
	indent--;
	Indent();
	output << "};" << std::endl;
}

void ProcessFolderRecursive(void)
{
	auto folders = manager.Folders();
	for (auto & folder : folders)
	{
		Indent();
		output << "namespace " << folder.stem() << std::endl;

		ScorpStart();

		manager.SetPath(folder);

		auto files = manager.Files();

		if (manager.FileCnt() > 0)
		{
			Indent();
			output << "constexpr const char * paths[] =" << std::endl;

			ScorpStart();

			for (auto & file : files)
			{
				Indent();
				std::string replace = file.string();
				std::replace(replace.begin(), replace.end(), '\\', '/');
				output << "\"" << replace.c_str() << "\"," << std::endl;
			}

			ScorpEnds();
		}

		Indent();
		output << "enum class PATH" << std::endl;

		ScorpStart();

		for (unsigned int n = 0; n < manager.FileCnt(); ++n)
		{
			Indent();
			output << files[n].stem() << "," << std::endl;
		}

		ScorpEnds();

		ProcessFolderRecursive();

		ScorpEnd();
	}
}

int main(void)
{
	output.open("resource.h");

	manager.SetPath("Resource");

	output << "#pragma once" << std::endl << std::endl;

	output << "#pragma warning(disable: 4129)" << std::endl << std::endl;

	output << "namespace Resource" << std::endl;

	ScorpStart();

	ProcessFolderRecursive();

	ScorpEnd();

	output.close();

	return 0;
}