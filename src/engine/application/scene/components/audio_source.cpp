// MIT License
// 
// Copyright (c) 2020 Jean-Louis Haywood
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "audio_source.hpp"

void Omnific::AudioSource::addAudioStream(std::shared_ptr<AudioStream> audioStream)
{
	this->audioStreams.emplace(audioStream->getName(), audioStream);
}

void Omnific::AudioSource::queueAudioToPlayAndRepeat(std::string audioStreamName, uint8_t count)
{
	if (this->audioStreams.count(audioStreamName) > 0)
		for (int i = 0; i < count; i++)
			this->audioPlayQueue.emplace(this->audioStreams.at(audioStreamName));
}

void Omnific::AudioSource::queueAudioToPlay(std::string audioStreamName)
{
	this->queueAudioToPlayAndRepeat(audioStreamName, 1);
}

void Omnific::AudioSource::clearAudioStreams()
{
	this->audioStreams.clear();
}

std::queue<std::shared_ptr<Omnific::AudioStream>> Omnific::AudioSource::popEntireAudioPlayQueue()
{
	std::queue<std::shared_ptr<AudioStream>> outputQueue = this->audioPlayQueue;
	this->clearAudioPlayQueue();
	return outputQueue;
}

void Omnific::AudioSource::clearAudioPlayQueue()
{
	for (int i = 0; i < this->audioPlayQueue.size(); i++)
		this->audioPlayQueue.pop();
}

std::vector<std::string> Omnific::AudioSource::getAudioStreamNames()
{
	std::vector<std::string> audioStreamNames;

	for (std::unordered_map<std::string, std::shared_ptr<AudioStream>>::iterator it = this->audioStreams.begin();
		it != this->audioStreams.end();
		++it)
	{
		audioStreamNames.push_back(it->first);
	}

	return audioStreamNames;
}

std::shared_ptr<Omnific::AudioStream> Omnific::AudioSource::getAudioStreamByName(std::string audioStreamName)
{
	std::shared_ptr<AudioStream> audioStream;

	if (this->audioStreams.count(audioStreamName))
		audioStream = this->audioStreams.at(audioStreamName);

	return audioStream;
}