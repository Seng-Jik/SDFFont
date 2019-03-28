
#include "FontRenderer.h"
#include "FontCollector.h"
#include "SDFGenerator.h"
#include <cctype>
#include <set>

using namespace System;
using namespace System::IO;
using namespace CommandLineParser;


int main(array<System::String ^> ^args)
{
	String^ srcfont = nullptr;
	String^ dst = nullptr;
	String^ charatersText = nullptr;

	bool sdf = true;

	try {
		CommandLineParser::CommandLine cmd(args);
		auto p1 = gcnew CommandLineParser::ShortOption("srcfont");
		p1->ParamNumber = 1;
		auto p2 = gcnew CommandLineParser::ShortOption("dst");
		p2->ParamNumber = 1;
		auto p3 = gcnew CommandLineParser::ShortOption("chars");
		p3->ParamNumber = 1;
		cmd.Add(p1);
		cmd.Add(p2);
		cmd.Add(p3);
		cmd.Add(gcnew CommandLineParser::ShortOption("nosdf"));


		for each (ParsedOptions^ param in cmd)
		{
			if (param->Option == "srcfont")
				srcfont = param->Param[0]->Replace("\"","");

			else if (param->Option == "dst")
				dst = param->Param[0];


			else if (param->Option == "chars")
				charatersText = param->Param[0];

			else if (param->Option == "nosdf")
				sdf = false;

			else
				throw gcnew Exception();
		}

		if (
			srcfont == nullptr || 
			dst == nullptr || 
			charatersText == nullptr)
			throw gcnew Exception();
	}
	catch (Exception^)
	{
		Console::WriteLine(L"SDFFont -srcfont <font_family_name> -dst <dst_files> -chars <charstxt>");
		return 1;
	}

	auto render = gcnew FontRenderer(srcfont);
	SDFGenerator sdfGen(32);
	FontCollector collector(2016, 2016, sdf ? 96 : 96 * 2, dst,600);

	//sdfGen.GenSDF(render->Render(L'°¡'))->Save("Debug.bmp");
	String^ chars = System::IO::File::ReadAllText(charatersText);
	chars = chars->Trim();
	std::set<wchar_t> charsSet;

	for each(auto i in chars)
		charsSet.insert(i);

	int count = 0;
	for (auto i : charsSet)
	{
		count++;
		System::Drawing::Rectangle r;
		try {
			auto p = render->Render(i,!sdf);
			if (sdf) p = sdfGen.GenSDF(p);
			collector.AddChar(p, i);
		}
		catch (Exception^ e)
		{
			Console::WriteLine("Warning:" + e->Message);
		}

		String^ s = "";
		s += count;
		s += L'/';
		s += charsSet.size();
		s += L'\t';
		s += i;
		Console::WriteLine(s);

		GC::Collect();
	}


	collector.~FontCollector();
	

	return 0;
}
