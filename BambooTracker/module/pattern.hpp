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
#include <cstddef>
#include "step.hpp"

class Pattern
{
public:
	Pattern(int n, size_t defSize);

	void setNumber(int n);
	int getNumber() const;

	int usedCountUp();
	int usedCountDown();
	int getUsedCount() const;

	Step& getStep(int n);

	size_t getSize() const;
	void changeSize(size_t size);

	void insertStep(int n);
	void deletePreviousStep(int n);

	bool existCommand() const;
	std::vector<int> getEditedStepIndices() const;
	std::unordered_set<int> getRegisteredInstruments() const;

	Pattern clone(int asNumber);

	void transpose(int seminotes, std::vector<int> excludeInsts);

	void clear();

private:
	int num_;
	size_t size_;
	std::vector<Step> steps_;
	int usedCnt_;

	Pattern(int n, size_t size, std::vector<Step> steps);
};
