#include <Windows.h>
#include <stdio.h>
#include <string>
#include "lib/bass.h"

/** 
@brief Contains an audio file and the function to play and pause it,

@todo Could add a lot more features and control to this at a later date.
*/
class Audio
{
public:
	/**
	@brief Creates an audio container that holds a song and audio channel
	
	@param std::string - The file path and name of the music file (mp3's tested)
	@param bool - Should the audio continuously loop or only play once. True for loop.
	*/
	Audio(std::string filename, bool loop);

	~Audio();

	/**
	@brief Play the stored audio file
	
	@param bool - True to play from the begining, false to resume.
	*/
	void play(bool restart);

	/**
	@brief Pause the stored audio file
	*/
	void pause();
private:
	DWORD chan;
	
};