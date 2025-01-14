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

#include <memory>
#include "module.hpp"
#include "instruments_manager.hpp"
#include "binary_container.hpp"

class ModuleIO
{
public:
	static void saveModule(BinaryContainer& ctr, const std::weak_ptr<Module> mod,
						   const std::weak_ptr<InstrumentsManager> instMan);
	static void loadModule(const BinaryContainer& ctr, std::weak_ptr<Module> mod,
						   std::weak_ptr<InstrumentsManager> instMan);

private:
	ModuleIO();

	static size_t loadModuleSectionInModule(std::weak_ptr<Module> mod, const BinaryContainer& ctr,
											size_t globCsr, uint32_t version);
	static size_t loadInstrumentSectionInModule(std::weak_ptr<InstrumentsManager> instMan,
												const BinaryContainer& ctr, size_t globCsr,
												uint32_t version);
	static size_t loadInstrumentPropertySectionInModule(std::weak_ptr<InstrumentsManager> instMan,
														const BinaryContainer& ctr, size_t globCsr,
														uint32_t version);
	static size_t loadInstrumentPropertyOperatorSequence(FMEnvelopeParameter param,
														 size_t instMemCsr,
														 std::shared_ptr<InstrumentsManager>& instManLocked,
														 const BinaryContainer& ctr, uint32_t version);
	static size_t loadGrooveSectionInModule(std::weak_ptr<Module> mod, const BinaryContainer& ctr,
											size_t globCsr, uint32_t version);
	static size_t loadSongSectionInModule(std::weak_ptr<Module> mod, const BinaryContainer& ctr,
										  size_t globCsr, uint32_t version);
};
