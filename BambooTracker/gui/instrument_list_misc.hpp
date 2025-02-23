/*
 * Copyright (C) 2020 Rerrah
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

#ifndef INSTRUMENT_LIST_MISC_HPP
#define INSTRUMENT_LIST_MISC_HPP

#include <unordered_map>
#include <QString>
#include <QListWidgetItem>
#include <QIcon>
#include "instrument.hpp"
#include "enum_hash.hpp"

static const std::unordered_map<InstrumentType, const char*> ICON_SRC = {
	{ InstrumentType::FM, ":/icon/inst_fm" },
	{ InstrumentType::SSG, ":/icon/inst_ssg" },
	{ InstrumentType::ADPCM, ":/icon/inst_adpcm" },
	{ InstrumentType::Drumkit, ":/icon/inst_kit" }
};

inline QString makeInstrumentListText(int num, QString name)
{
	return QString("%1: %2").arg(num, 2, 16, QChar('0')).toUpper().arg(name);
}

inline QListWidgetItem* createInstrumentListItem(int num, InstrumentType type, QString name)
{
	QListWidgetItem *item = new QListWidgetItem(QIcon(ICON_SRC.at(type)), makeInstrumentListText(num, name));
	item->setSizeHint(QSize(130, 17));
	item->setData(Qt::UserRole, num);
	return item;
}

#endif // INSTRUMENT_LIST_MISC_HPP
