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

#ifndef INSTRUMENT_EDITOR_FM_FORM_HPP
#define INSTRUMENT_EDITOR_FM_FORM_HPP

#include <QWidget>
#include <QKeyEvent>
#include <QResizeEvent>
#include <QShowEvent>
#include <memory>
#include "bamboo_tracker.hpp"
#include "instrument.hpp"
#include "configuration.hpp"
#include "jam_manager.hpp"
#include "gui/instrument_editor/visualized_instrument_macro_editor.hpp"
#include "gui/color_palette.hpp"

namespace Ui {
	class InstrumentEditorFMForm;
}

class InstrumentEditorFMForm : public QWidget
{
	Q_OBJECT

public:
	InstrumentEditorFMForm(int num, QWidget *parent = nullptr);
	~InstrumentEditorFMForm() override;
	void setInstrumentNumber(int num);
	int getInstrumentNumber() const;
	void setCore(std::weak_ptr<BambooTracker> core);
	void setConfiguration(std::weak_ptr<Configuration> config);
	void updateConfigurationForDisplay();
	void setColorPalette(std::shared_ptr<ColorPalette> palette);

signals:
	void jamKeyOnEvent(JamKey key);
	void jamKeyOffEvent(JamKey key);
	void modified();

protected:
	void keyPressEvent(QKeyEvent* event) override;
	void keyReleaseEvent(QKeyEvent* event) override;
	void showEvent(QShowEvent* event) override;
	void resizeEvent(QResizeEvent* event) override;

private:
	Ui::InstrumentEditorFMForm *ui;
	int instNum_;
	bool isIgnoreEvent_;

	std::weak_ptr<BambooTracker> bt_;
	std::shared_ptr<ColorPalette> palette_;
	std::weak_ptr<Configuration> config_;

	void updateInstrumentParameters();

	//========== Envelope ==========//
signals:
	void envelopeNumberChanged();
	void envelopeParameterChanged(int envNum, int fromInstNum);

public slots:
	void onEnvelopeParameterChanged(int envNum);
	void onEnvelopeNumberChanged();

private:
	void setInstrumentEnvelopeParameters();
	void setInstrumentEnvelopeParameters(QString data);
	void setInstrumentEnvelopeParameters(int envTypeNum, QString data);
	void setInstrumentOperatorParameters(int opNum, QString data);

	void paintAlgorithmDiagram();
	void resizeAlgorithmDiagram();

private slots:
	void copyEnvelope();
	void pasteEnvelope();
	void pasteEnvelopeFrom(int typenum);
	void copyOperator(int opNum);
	void pasteOperator(int opNum);
	void on_envNumSpinBox_valueChanged(int arg1);
	void on_envGroupBox_customContextMenuRequested(const QPoint &pos);

	//========== LFO ==========//
signals:
	void lfoNumberChanged();
	void lfoParameterChanged(int lfoNum, int fromInstNum);

public slots:
	void onLFOParameterChanged(int lfoNum);
	void onLFONumberChanged();

private:
	void setInstrumentLFOParameters();
	void setInstrumentLFOParameters(QString data);

	QString toLFOString() const;

private slots:
	void on_lfoGroupBox_customContextMenuRequested(const QPoint &pos);
	void on_lfoNumSpinBox_valueChanged(int arg1);
	void on_lfoGroupBox_toggled(bool arg1);

	//========== OperatorSequence ==========//
public:
	FMEnvelopeParameter getOperatorSequenceParameter() const;

signals:
	void operatorSequenceNumberChanged();
	void operatorSequenceParameterChanged(FMEnvelopeParameter param, int opSeqNum, int fromInstNum);

public slots:
	void onOperatorSequenceNumberChanged();
	void onOperatorSequenceParameterChanged(FMEnvelopeParameter param, int opSeqNum);

private:
	void setInstrumentOperatorSequenceParameters();
	void setOperatorSequenceEditor();

private slots:
	void onOperatorSequenceTypeChanged(int type);
	void on_opSeqEditGroupBox_toggled(bool arg1);
	void on_opSeqNumSpinBox_valueChanged(int arg1);

	//========== Arpeggio ==========//
public:
	FMOperatorType getArpeggioOperator() const;

signals:
	void arpeggioNumberChanged();
	void arpeggioParameterChanged(int arpNum, int fromInstNum);

public slots:
	void onArpeggioNumberChanged();
	void onArpeggioParameterChanged(int arpNum);

private:
	void setInstrumentArpeggioParameters();

private slots:
	void onArpeggioOperatorChanged(int op);
	void onArpeggioTypeChanged(int index);
	void on_arpEditGroupBox_toggled(bool arg1);
	void on_arpNumSpinBox_valueChanged(int arg1);

	//========== Pitch ==========//
public:
	FMOperatorType getPitchOperator() const;

signals:
	void pitchNumberChanged();
	void pitchParameterChanged(int ptNum, int fromInstNum);

public slots:
	void onPitchNumberChanged();
	void onPitchParameterChanged(int arpNum);

private:
	void setInstrumentPitchParameters();

private slots:
	void onPitchOperatorChanged(int op);
	void onPitchTypeChanged(int index);
	void on_ptEditGroupBox_toggled(bool arg1);
	void on_ptNumSpinBox_valueChanged(int arg1);

	//========== Others ==========//
private:
	void setInstrumentEnvelopeResetParameters();
};

#endif // INSTRUMENT_EDITOR_FM_FORM_HPP
