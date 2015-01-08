#include "Audio.h"
#include "cgg/cgg.h"

Audio::Audio(std::string filename, bool loop)
{
	chan = NULL;
	int loopFlag = loop ? BASS_SAMPLE_LOOP : 0;
	chan = BASS_StreamCreateFile(FALSE, filename.c_str(), 0, 0, loopFlag);
	if (!chan)
	{
		std::string errorMsg = "Audio load fail: " + filename + ", Error Code: " + std::to_string(BASS_ErrorGetCode());
		cgg::loge(errorMsg.c_str());
	}
}

Audio::~Audio()
{
	BASS_StreamFree(chan);
}

void Audio::play(bool restart)
{
	BASS_ChannelPlay(chan, restart);
}

void Audio::pause()
{
	BASS_ChannelPause(chan);
}