#include <Windows.h>
#include <stdio.h>
#include <string>
#include "lib/bass.h"

class Audio
{
public:
	Audio(std::string filename, bool loop);
	~Audio();

	void play(bool restart);
	void pause();
private:
	DWORD chan;
	
};