#pragma once

#include <string>
#include <memory>
#include <map>
#include <vector>
#include "instruments_manager.hpp"
#include "envelope_fm.hpp"
#include "lfo_fm.hpp"
#include "command_sequence.hpp"
#include "misc.hpp"

class InstrumentsManager;

class AbstractInstrument
{
public:
	virtual ~AbstractInstrument() = default;

	int getNumber() const;
	void setNumber(int n);
	SoundSource getSoundSource() const;
	std::string getName() const;
	void setName(std::string name);
	bool isRegisteredWithManager() const;
	virtual std::unique_ptr<AbstractInstrument> clone() = 0;

protected:
	InstrumentsManager* owner_;
	int number_;
	std::string name_;	// UTF-8
	AbstractInstrument(int number, SoundSource source, std::string name, InstrumentsManager* owner);

private:
	SoundSource source_;
};


class InstrumentFM : public AbstractInstrument
{
public:
	InstrumentFM(int number, std::string name, InstrumentsManager* owner);
	std::unique_ptr<AbstractInstrument> clone() override;

	void setEnvelopeNumber(int n);
	int getEnvelopeNumber() const;
	int getEnvelopeParameter(FMEnvelopeParameter param) const;
	bool getOperatorEnabled(int n) const;

	void setLFOEnabled(bool enabled);
	bool getLFOEnabled() const;
	void setLFONumber(int n);
	int getLFONumber() const;
	int getLFOParameter(FMLFOParameter param) const;

	void setOperatorSequenceEnabled(FMEnvelopeParameter param, bool enabled);
	bool getOperatorSequenceEnabled(FMEnvelopeParameter param) const;
	void setOperatorSequenceNumber(FMEnvelopeParameter param, int n);
	int getOperatorSequenceNumber(FMEnvelopeParameter param) const;
	std::vector<CommandSequenceUnit> getOperatorSequenceSequence(FMEnvelopeParameter param) const;
	std::vector<Loop> getOperatorSequenceLoops(FMEnvelopeParameter param) const;
	Release getOperatorSequenceRelease(FMEnvelopeParameter param) const;
	std::unique_ptr<CommandSequence::Iterator> getOperatorSequenceSequenceIterator(FMEnvelopeParameter param) const;

	void setArpeggioEnabled(FMOperatorType op, bool enabled);
	bool getArpeggioEnabled(FMOperatorType op) const;
	void setArpeggioNumber(FMOperatorType op, int n);
	int getArpeggioNumber(FMOperatorType op) const;
	int getArpeggioType(FMOperatorType op) const;
	std::vector<CommandSequenceUnit> getArpeggioSequence(FMOperatorType op) const;
	std::vector<Loop> getArpeggioLoops(FMOperatorType op) const;
	Release getArpeggioRelease(FMOperatorType op) const;
	std::unique_ptr<CommandSequence::Iterator> getArpeggioSequenceIterator(FMOperatorType op) const;

	void setPitchEnabled(FMOperatorType op, bool enabled);
	bool getPitchEnabled(FMOperatorType op) const;
	void setPitchNumber(FMOperatorType op, int n);
	int getPitchNumber(FMOperatorType op) const;
	int getPitchType(FMOperatorType op) const;
	std::vector<CommandSequenceUnit> getPitchSequence(FMOperatorType op) const;
	std::vector<Loop> getPitchLoops(FMOperatorType op) const;
	Release getPitchRelease(FMOperatorType op) const;
	std::unique_ptr<CommandSequence::Iterator> getPitchSequenceIterator(FMOperatorType op) const;

	void setEnvelopeResetEnabled(FMOperatorType op, bool enabled);
	bool getEnvelopeResetEnabled(FMOperatorType op) const;

private:
	int envNum_;
	bool lfoEnabled_;
	int lfoNum_;
	std::map<FMEnvelopeParameter, bool> opSeqEnabled_;
	std::map<FMEnvelopeParameter, int> opSeqNum_;
	std::map<FMOperatorType, bool> arpEnabled_;
	std::map<FMOperatorType, int> arpNum_;
	std::map<FMOperatorType, bool> ptEnabled_;
	std::map<FMOperatorType, int> ptNum_;

	std::map<FMOperatorType, bool> envResetEnabled_;
};


class InstrumentSSG : public AbstractInstrument
{
public:
	InstrumentSSG(int number, std::string name, InstrumentsManager* owner);
	std::unique_ptr<AbstractInstrument> clone() override;

	void setWaveFormEnabled(bool enabled);
	bool getWaveFormEnabled() const;
	void setWaveFormNumber(int n);
	int getWaveFormNumber() const;
	std::vector<CommandSequenceUnit> getWaveFormSequence() const;
	std::vector<Loop> getWaveFormLoops() const;
	Release getWaveFormRelease() const;
	std::unique_ptr<CommandSequence::Iterator> getWaveFormSequenceIterator() const;

	void setToneNoiseEnabled(bool enabled);
	bool getToneNoiseEnabled() const;
	void setToneNoiseNumber(int n);
	int getToneNoiseNumber() const;
	std::vector<CommandSequenceUnit> getToneNoiseSequence() const;
	std::vector<Loop> getToneNoiseLoops() const;
	Release getToneNoiseRelease() const;
	std::unique_ptr<CommandSequence::Iterator> getToneNoiseSequenceIterator() const;

	void setEnvelopeEnabled(bool enabled);
	bool getEnvelopeEnabled() const;
	void setEnvelopeNumber(int n);
	int getEnvelopeNumber() const;
	std::vector<CommandSequenceUnit> getEnvelopeSequence() const;
	std::vector<Loop> getEnvelopeLoops() const;
	Release getEnvelopeRelease() const;
	std::unique_ptr<CommandSequence::Iterator> getEnvelopeSequenceIterator() const;

	void setArpeggioEnabled(bool enabled);
	bool getArpeggioEnabled() const;
	void setArpeggioNumber(int n);
	int getArpeggioNumber() const;
	int getArpeggioType() const;
	std::vector<CommandSequenceUnit> getArpeggioSequence() const;
	std::vector<Loop> getArpeggioLoops() const;
	Release getArpeggioRelease() const;
	std::unique_ptr<CommandSequence::Iterator> getArpeggioSequenceIterator() const;

	void setPitchEnabled(bool enabled);
	bool getPitchEnabled() const;
	void setPitchNumber(int n);
	int getPitchNumber() const;
	int getPitchType() const;
	std::vector<CommandSequenceUnit> getPitchSequence() const;
	std::vector<Loop> getPitchLoops() const;
	Release getPitchRelease() const;
	std::unique_ptr<CommandSequence::Iterator> getPitchSequenceIterator() const;

private:
	bool wfEnabled_;
	int wfNum_;
	bool tnEnabled_;
	int tnNum_;
	bool envEnabled_;
	int envNum_;
	bool arpEnabled_;
	int arpNum_;
	bool ptEnabled_;
	int ptNum_;
};
