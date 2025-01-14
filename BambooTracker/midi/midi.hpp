/*
 * Copyright (C) 2019-2020 Rerrah
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#pragma once
#include "RtMidi/RtMidi.hpp"
#include <string>
#include <vector>
#include <memory>
#include <mutex>
#include <cstdint>

class MidiInterface
{
public:
	static MidiInterface &instance();
	~MidiInterface();

private:
	MidiInterface();

public:
	RtMidi::Api currentApi() const;
	std::string currentApiName() const;
	std::vector<std::string> getAvailableApi() const;
	bool switchApi(std::string api, std::string* errDetail = nullptr);
	bool switchApi(RtMidi::Api api, std::string* errDetail = nullptr);	// Use internal
	bool supportsVirtualPort() const;
	bool supportsVirtualPort(std::string api) const;
	std::vector<std::string> getRealInputPorts();
	std::vector<std::string> getRealInputPorts(const std::string& api);

	void closeInputPort();
	bool openInputPort(unsigned port, std::string* errDetail = nullptr);
	bool openInputPortByName(const std::string &portName, std::string* errDetail = nullptr);

	typedef void (InputHandler)(double, const uint8_t *, size_t, void *);
	void installInputHandler(InputHandler *handler, void *userData);
	void uninstallInputHandler(InputHandler *handler, void *userData);

private:
	static void onMidiInput(double timestamp, std::vector<unsigned char> *message, void *userData);

	std::unique_ptr<RtMidiIn> inputClient_;
	bool hasOpenInputPort_;
	std::mutex inputHandlersMutex_;
	std::vector<std::pair<InputHandler *, void *>> inputHandlers_;

	static std::unique_ptr<MidiInterface> instance_;
};
