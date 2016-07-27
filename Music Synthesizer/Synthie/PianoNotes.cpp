#include "stdafx.h"
#include "PianoNotes.h"
#include <map>
#include <string>

using namespace std;

static int g_initialized = false;
static map<wstring, string> g_name2file;

const char* NoteToFile(const WCHAR *name, char dynamic)
{
	string note_to_file = "CompletePiano\\";
	if (!g_initialized)
	{
		for (int i = 0; i < sizeof(pianonotes) / sizeof(PianoNotes); i++)
		{
			g_name2file[pianonotes[i].name] = pianonotes[i].filename;
		}

		g_initialized = true;
	}

	map<wstring, string>::iterator f = g_name2file.find(name);
	if (f == g_name2file.end())
		return 0;

	if (f->second == "pedald" || f->second == "pedalu"){
		note_to_file += f->second;
	}
	else {

		note_to_file += f->second;
		note_to_file += dynamic;

	}

	note_to_file += ".wav";
	char *sender = new char[note_to_file.length() + 1];
	std::strcpy(sender, note_to_file.c_str());
	return sender;

}
