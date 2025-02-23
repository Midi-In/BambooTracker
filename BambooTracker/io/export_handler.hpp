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

#include <vector>
#include "gd3_tag.hpp"
#include "s98_tag.hpp"
#include "binary_container.hpp"

class ExportHandler
{
public:
	static void writeVgm(BinaryContainer& container, int target, std::vector<uint8_t> samples, uint32_t clock, uint32_t rate,
						 bool loopFlag, uint32_t loopPoint, uint32_t loopSamples, uint32_t totalSamples,
						 bool gd3TagEnabled, GD3Tag tag);
	static void writeS98(BinaryContainer& container, int target, std::vector<uint8_t> samples, uint32_t clock, uint32_t rate,
						 bool loopFlag, uint32_t loopPoint, bool tagEnabled, S98Tag tag);

private:
	ExportHandler();
};

enum ExportTargetFlag
{
	/* target bits 0-3 : FM type */
	Export_NoneFm = 0,
	Export_YM2608 = 1,
	Export_YM2612 = 2,
	Export_YM2203 = 4,
	Export_FmMask = Export_NoneFm|Export_YM2608|Export_YM2612|Export_YM2203,
	/* target bit 4-5 : SSG type */
	Export_InternalSsg = 0,
	Export_AY8910Psg = 16,
	Export_YM2149Psg = 32,
	Export_SsgMask = Export_InternalSsg|Export_AY8910Psg|Export_YM2149Psg,
};
