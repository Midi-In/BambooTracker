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

#include <cstdint>
#include "misc.hpp"

class PitchConverter
{
public:
	static constexpr int SEMINOTE_PITCH = 32;

	static uint16_t getPitchFM(Note note, int octave, int pitch, int finePitch);
	static uint16_t getPitchSSGSquare(Note note, int octave, int pitch, int finePitch);
	static uint16_t getPitchSSGSquare(int n);
	static uint16_t getPitchSSGTriangle(Note note, int octave, int pitch, int finePitch);
	static uint16_t getPitchSSGSaw(Note note, int octave, int pitch, int finePitch);

	static int calculatePitchIndex(int octave, Note note, int pitch);

private:
	static const uint16_t centTableFM_[3072];
	static const uint16_t centTableSSGSquare_[3072];
	static const uint16_t centTableSSGTriangle_[3072];
	static const uint16_t centTableSSGSaw_[3072];

	PitchConverter();
};
