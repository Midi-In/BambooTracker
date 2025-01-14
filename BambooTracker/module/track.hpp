/*
 * Copyright (C) 2018-2020 Rerrah
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

#include <vector>
#include <unordered_set>
#include <memory>
#include <unordered_map>
#include "pattern.hpp"
#include "misc.hpp"

struct TrackAttribute;
struct OrderData;

class Track
{
public:
	Track(int number, SoundSource source, int channelInSource, int defPattenSize);
	Track(const Track& other);
	Track& operator=(const Track& other);
	Track(Track&& other) noexcept;
	Track& operator=(Track&& other) noexcept;

	void setAttribute(int number, SoundSource source, int channelInSource);
	TrackAttribute getAttribute() const;
	OrderData getOrderData(int order);
	size_t getOrderSize() const;
	Pattern& getPattern(int num);
	Pattern& getPatternFromOrderNumber(int num);
	int searchFirstUneditedUnusedPattern() const;
	int clonePattern(int num);
	std::vector<int> getEditedPatternIndices() const;
	std::unordered_set<int> getRegisteredInstruments() const;

	void registerPatternToOrder(int order, int pattern);
	void insertOrderBelow(int order);
	void deleteOrder(int order);
	void swapOrder(int a, int b);

	void changeDefaultPatternSize(size_t size);

	void setEffectDisplayWidth(size_t w);
	size_t getEffectDisplayWidth() const;

	void clearUnusedPatterns();
	void replaceDuplicateInstrumentsInPatterns(std::unordered_map<int, int> map);

	void transpose(int seminotes, std::vector<int> excludeInsts);

private:
	std::unique_ptr<TrackAttribute> attrib_;

	std::vector<int> order_;
	std::vector<Pattern> patterns_;
	size_t effetDisplayWidth_;
};

struct TrackAttribute
{
	int number;
	SoundSource source;
	int channelInSource;
};


struct OrderData
{
	TrackAttribute trackAttribute;
	int order;
	int patten;
};
