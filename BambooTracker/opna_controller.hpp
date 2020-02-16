#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <deque>
#include "opna.hpp"
#include "instrument.hpp"
#include "effect_iterator.hpp"
#include "chips/chip_misc.h"
#include "chips/scci/scci.h"
#include "enum_hash.hpp"
#include "misc.hpp"

struct RegisterUnit
{
	int address, value;
	bool hasCompleted;
};

struct ToneDetail
{
	int octave;
	Note note;
	int pitch;
};

struct SSGToneNoise
{
	bool isTone_, isNoise_;
	int noisePeriod_;
};

struct SSGWaveform
{
	SSGWaveformType type;
	int data;	// Same format with CommandSequenceUnit::data
};

class OPNAController
{
public:
	OPNAController(chip::Emu emu, int clock, int rate, int duration);

	// Reset and initialize
	void reset();

	// Forward instrument sequence
	void tickEvent(SoundSource src, int ch);

	// Direct register set
	void sendRegisterAddress(int bank, int address);
	void sendRegisterValue(int value);

	// Update register states after tick process
	void updateRegisterStates();

	// Stream type
	void useSCCI(SoundInterfaceManager* manager);
	bool isUsedSCCI() const;

	// Stream samples
	void getStreamSamples(int16_t* container, size_t nSamples);
	void getOutputHistory(int16_t* history);

	static constexpr int OUTPUT_HISTORY_SIZE = 1024;

	// Chip mode
	void setMode(SongType mode);
	SongType getMode() const;

	// Mute
	void setMuteState(SoundSource src, int chInSrc, bool isMute);
	bool isMute(SoundSource src, int chInSrc);

	// Stream details
	int getRate() const;
	void setRate(int rate);
	int getDuration() const;
	void setDuration(int duration);
	void setMasterVolume(int percentage);

	// Export
	void setExportContainer(std::shared_ptr<chip::ExportContainerInterface> cntr = nullptr);

private:
	std::unique_ptr<chip::OPNA> opna_;
	SongType mode_;

	std::vector<RegisterUnit> registerSetBuf_;

	std::unique_ptr<int16_t[]> outputHistory_;
	size_t outputHistoryIndex_;
	std::unique_ptr<int16_t[]> outputHistoryReady_;
	std::mutex outputHistoryReadyMutex_;

	void initChip();

	void fillOutputHistory(const int16_t* outputs, size_t nSamples);
	void transferReadyHistory();

	void checkRealToneByArpeggio(int seqPos, const std::unique_ptr<SequenceIteratorInterface>& arpIt,
								 const std::deque<ToneDetail>& baseTone, ToneDetail& keyTone, bool& needToneSet);
	void checkPortamento(const std::unique_ptr<SequenceIteratorInterface>& arpIt, int prtm, bool isTonePrtm,
						 const std::deque<ToneDetail>& baseTone, ToneDetail& keyTone, bool& needToneSet);
	void checkRealToneByPitch(int seqPos, const std::unique_ptr<CommandSequence::Iterator>& ptIt,
							  int& sumPitch, bool& needToneSet);

	/*----- FM -----*/
public:
	// Key on-off
	void keyOnFM(int ch, Note note, int octave, int pitch, bool isJam = false);
	void keyOnFM(int ch, int echoBuf);
	void keyOffFM(int ch, bool isJam = false);
	void updateEchoBufferFM(int ch, int octave, Note note, int pitch);

	// Set instrument
	void setInstrumentFM(int ch, std::shared_ptr<InstrumentFM> inst);
	void updateInstrumentFM(int instNum);
	void updateInstrumentFMEnvelopeParameter(int envNum, FMEnvelopeParameter param);
	void setInstrumentFMOperatorEnabled(int envNum, int opNum);
	void updateInstrumentFMLFOParameter(int lfoNum, FMLFOParameter param);
	void resetFMChannelEnvelope(int ch);
	void restoreFMEnvelopeFromReset(int ch);

	// Set volume
	void setVolumeFM(int ch, int volume);
	void setTemporaryVolumeFM(int ch, int volume);
	void setMasterVolumeFM(double dB);

	// Set effect
	void setPanFM(int ch, int value);
	void setArpeggioEffectFM(int ch, int second, int third);
	void setPortamentoEffectFM(int ch, int depth, bool isTonePortamento = false);
	void setVibratoEffectFM(int ch, int period, int depth);
	void setTremoloEffectFM(int ch, int period, int depth);
	void setVolumeSlideFM(int ch, int depth, bool isUp);
	void setDetuneFM(int ch, int pitch);
	void setNoteSlideFM(int ch, int speed, int seminote);
	void setTransposeEffectFM(int ch, int seminote);
	void setFBControlFM(int ch, int value);
	void setTLControlFM(int ch, int op, int value);
	void setMLControlFM(int ch, int op, int value);
	void setARControlFM(int ch, int op, int value);
	void setDRControlFM(int ch, int op, int value);
	void setRRControlFM(int ch, int op, int value);
	void setBrightnessFM(int ch, int value);

	// For state retrieve
	void haltSequencesFM(int ch);

	// Chip details
	bool isKeyOnFM(int ch) const;
	bool isTonePortamentoFM(int ch) const;
	bool enableFMEnvelopeReset(int ch) const;
	ToneDetail getFMTone(int ch) const;

private:
	std::shared_ptr<InstrumentFM> refInstFM_[6];
	std::unique_ptr<EnvelopeFM> envFM_[6];
	bool isKeyOnFM_[9];
	uint8_t fmOpEnables_[6];
	std::deque<ToneDetail> baseToneFM_[9];
	ToneDetail keyToneFM_[9];
	int sumPitchFM_[9];
	int baseVolFM_[9], tmpVolFM_[9];
	/// bit0: right on/off
	/// bit1: left on/off
	uint8_t panFM_[6];
	bool isMuteFM_[9];
	bool enableEnvResetFM_[9], hasResetEnvFM_[9];
	int lfoFreq_;
	int lfoStartCntFM_[6];
	bool hasPreSetTickEventFM_[9];
	bool needToneSetFM_[9];
	std::unordered_map<FMEnvelopeParameter, std::unique_ptr<CommandSequence::Iterator>> opSeqItFM_[6];
	std::unique_ptr<SequenceIteratorInterface> arpItFM_[9];
	std::unique_ptr<CommandSequence::Iterator> ptItFM_[9];
	bool isArpEffFM_[9];
	int prtmFM_[9];
	bool isTonePrtmFM_[9];
	std::unique_ptr<WavingEffectIterator> vibItFM_[9];
	std::unique_ptr<WavingEffectIterator> treItFM_[9];
	int volSldFM_[9], sumVolSldFM_[9];
	int detuneFM_[9];
	std::unique_ptr<NoteSlideEffectIterator> nsItFM_[9];
	int sumNoteSldFM_[9];
	bool noteSldFMSetFlag_[9];
	int transposeFM_[9];
	bool isFBCtrlFM_[6], isTLCtrlFM_[6][4], isMLCtrlFM_[6][4], isARCtrlFM_[6][4];
	bool isDRCtrlFM_[6][4], isRRCtrlFM_[6][4];
	bool isBrightFM_[6][4];

	void initFM();

	void setMuteFMState(int ch, bool isMuteFM);
	bool isMuteFM(int ch);

	int toInternalFMChannel(int ch) const;
	uint8_t getFMKeyOnOffChannelMask(int ch) const;
	uint32_t getFMChannelOffset(int ch, bool forPitch = false) const;
	FMOperatorType toChannelOperatorType(int ch) const;
	std::vector<FMEnvelopeParameter> getFMEnvelopeParametersForOperator(FMOperatorType op) const;

	void updateFMVolume(int ch);

	void writeFMEnvelopeToRegistersFromInstrument(int inch);
	void writeFMEnveropeParameterToRegister(int inch, FMEnvelopeParameter param, int value);

	void writeFMLFOAllRegisters(int inch);
	void writeFMLFORegister(int inch, FMLFOParameter param);
	void checkLFOUsed();

	void setFrontFMSequences(int ch);
	void releaseStartFMSequences(int ch);
	void tickEventFM(int ch);

	void checkOperatorSequenceFM(int ch, int type);
	void checkVolumeEffectFM(int ch);

	inline void checkRealToneFMByArpeggio(int ch, int seqPos)
	{
		checkRealToneByArpeggio(seqPos, arpItFM_[ch], baseToneFM_[ch], keyToneFM_[ch], needToneSetFM_[ch]);
	}

	inline void checkPortamentoFM(int ch)
	{
		checkPortamento(arpItFM_[ch], prtmFM_[ch], isTonePrtmFM_[ch], baseToneFM_[ch],
						keyToneFM_[ch], needToneSetFM_[ch]);
	}

	inline void checkRealToneFMByPitch(int ch, int seqPos)
	{
		checkRealToneByPitch(seqPos, ptItFM_[ch], sumPitchFM_[ch], needToneSetFM_[ch]);
	}

	void writePitchFM(int ch);

	void setInstrumentFMProperties(int ch);

	static bool isCarrier(int op, int al);
	static std::vector<int> getOperatorsInLevel(int level, int al);

	inline FMEnvelopeParameter getParameterTL(int op) const
	{
		switch (op) {
		case 0:	return FMEnvelopeParameter::TL1;
		case 1:	return FMEnvelopeParameter::TL2;
		case 2:	return FMEnvelopeParameter::TL3;
		case 3:	return FMEnvelopeParameter::TL4;
		default:	throw std::invalid_argument("Invalid operator value.");
		}
	}

	inline FMEnvelopeParameter getParameterML(int op) const
	{
		switch (op) {
		case 0:	return FMEnvelopeParameter::ML1;
		case 1:	return FMEnvelopeParameter::ML2;
		case 2:	return FMEnvelopeParameter::ML3;
		case 3:	return FMEnvelopeParameter::ML4;
		default:	throw std::invalid_argument("Invalid operator value.");
		}
	}

	inline FMEnvelopeParameter getParameterAR(int op) const
	{
		switch (op) {
		case 0:	return FMEnvelopeParameter::AR1;
		case 1:	return FMEnvelopeParameter::AR2;
		case 2:	return FMEnvelopeParameter::AR3;
		case 3:	return FMEnvelopeParameter::AR4;
		default:	throw std::invalid_argument("Invalid operator value.");
		}
	}

	inline FMEnvelopeParameter getParameterDR(int op) const
	{
		switch (op) {
		case 0:	return FMEnvelopeParameter::DR1;
		case 1:	return FMEnvelopeParameter::DR2;
		case 2:	return FMEnvelopeParameter::DR3;
		case 3:	return FMEnvelopeParameter::DR4;
		default:	throw std::invalid_argument("Invalid operator value.");
		}
	}

	inline FMEnvelopeParameter getParameterRR(int op) const
	{
		switch (op) {
		case 0:	return FMEnvelopeParameter::RR1;
		case 1:	return FMEnvelopeParameter::RR2;
		case 2:	return FMEnvelopeParameter::RR3;
		case 3:	return FMEnvelopeParameter::RR4;
		default:	throw std::invalid_argument("Invalid operator value.");
		}
	}

	inline uint8_t calculateTL(int ch, uint8_t data) const
	{
		int v = (tmpVolFM_[ch] == -1) ? baseVolFM_[ch] : tmpVolFM_[ch];
		return (data > 127 - v) ? 127 : static_cast<uint8_t>(data + v);
	}

	/*----- SSG -----*/
public:
	// Key on-off
	void keyOnSSG(int ch, Note note, int octave, int pitch, bool isJam = false);
	void keyOnSSG(int ch, int echoBuf);
	void keyOffSSG(int ch, bool isJam = false);
	void updateEchoBufferSSG(int ch, int octave, Note note, int pitch);

	// Set instrument
	void setInstrumentSSG(int ch, std::shared_ptr<InstrumentSSG> inst);
	void updateInstrumentSSG(int instNum);

	// Set volume
	void setVolumeSSG(int ch, int volume);
	void setTemporaryVolumeSSG(int ch, int volume);
	void setMasterVolumeSSG(double dB);

	// Set effect
	void setArpeggioEffectSSG(int ch, int second, int third);
	void setPortamentoEffectSSG(int ch, int depth, bool isTonePortamento = false);
	void setVibratoEffectSSG(int ch, int period, int depth);
	void setTremoloEffectSSG(int ch, int period, int depth);
	void setVolumeSlideSSG(int ch, int depth, bool isUp);
	void setDetuneSSG(int ch, int pitch);
	void setNoteSlideSSG(int ch, int speed, int seminote);
	void setTransposeEffectSSG(int ch, int seminote);
	void setToneNoiseMixSSG(int ch, int value);
	void setNoisePitchSSG(int ch, int pitch);
	void setHardEnvelopePeriod(int ch, bool high, int period);
	void setAutoEnvelopeSSG(int ch, int shift, int shape);

	// For state retrieve
	void haltSequencesSSG(int ch);

	// Chip details
	bool isKeyOnSSG(int ch) const;
	bool isTonePortamentoSSG(int ch) const;
	ToneDetail getSSGTone(int ch) const;

private:
	std::shared_ptr<InstrumentSSG> refInstSSG_[3];
	bool isKeyOnSSG_[3];
	uint8_t mixerSSG_;
	std::deque<ToneDetail> baseToneSSG_[3];
	ToneDetail keyToneSSG_[3];
	int sumPitchSSG_[3];
	SSGToneNoise tnSSG_[3];
	int baseVolSSG_[3], tmpVolSSG_[3];
	bool isBuzzEffSSG_[3];
	bool isHardEnvSSG_[3];
	bool isMuteSSG_[3];
	bool hasPreSetTickEventSSG_[3];
	bool needEnvSetSSG_[3];
	bool setHardEnvIfNecessary_[3];
	bool needMixSetSSG_[3];
	bool needToneSetSSG_[3];
	bool needSqMaskFreqSetSSG_[3];
	std::unique_ptr<CommandSequence::Iterator> wfItSSG_[3];
	SSGWaveform wfSSG_[3];
	std::unique_ptr<CommandSequence::Iterator> envItSSG_[3];
	CommandSequenceUnit envSSG_[3];
	std::unique_ptr<CommandSequence::Iterator> tnItSSG_[3];
	std::unique_ptr<SequenceIteratorInterface> arpItSSG_[3];
	std::unique_ptr<CommandSequence::Iterator> ptItSSG_[3];
	bool isArpEffSSG_[3];
	int prtmSSG_[3];
	bool isTonePrtmSSG_[3];
	std::unique_ptr<WavingEffectIterator> vibItSSG_[3];
	std::unique_ptr<WavingEffectIterator> treItSSG_[3];
	int volSldSSG_[3], sumVolSldSSG_[3];
	int detuneSSG_[3];
	std::unique_ptr<NoteSlideEffectIterator> nsItSSG_[3];
	int sumNoteSldSSG_[3];
	bool noteSldSSGSetFlag_;
	int transposeSSG_[3];
	int toneNoiseMixSSG_[3];
	int noisePitchSSG_;
	int hardEnvPeriodHighSSG_, hardEnvPeriodLowSSG_;

	void initSSG();

	void setMuteSSGState(int ch, bool isMuteFM);
	bool isMuteSSG(int ch);

	void setFrontSSGSequences(int ch);
	void releaseStartSSGSequences(int ch);
	void tickEventSSG(int ch);

	void writeWaveformSSGToRegister(int ch, int seqPos);
	void writeSquareWaveform(int ch);

	void writeToneNoiseSSGToRegister(int ch, int seqPos);
	void writeToneNoiseSSGToRegisterNoReference(int ch);

	void writeEnvelopeSSGToRegister(int ch, int seqPos);

	inline void checkRealToneSSGByArpeggio(int ch, int seqPos)
	{
		checkRealToneByArpeggio(seqPos, arpItSSG_[ch], baseToneSSG_[ch], keyToneSSG_[ch], needToneSetSSG_[ch]);
	}

	inline void checkPortamentoSSG(int ch)
	{
		checkPortamento(arpItSSG_[ch], prtmSSG_[ch], isTonePrtmSSG_[ch], baseToneSSG_[ch],
						keyToneSSG_[ch], needToneSetSSG_[ch]);
	}

	inline void checkRealToneSSGByPitch(int ch, int seqPos)
	{
		checkRealToneByPitch(seqPos, ptItSSG_[ch], sumPitchSSG_[ch], needToneSetSSG_[ch]);
	}

	void writePitchSSG(int ch);

	void setRealVolumeSSG(int ch);

	inline uint8_t judgeSSEGRegisterValue(int v)
	{
		return (v == -1) ? 0 : (0x08 + static_cast<uint8_t>(v));
	}

	/*----- Drum -----*/
public:
	// Key on-off
	void setKeyOnFlagDrum(int ch);
	void setKeyOffFlagDrum(int ch);

	// Set volume
	void setVolumeDrum(int ch, int volume);
	void setMasterVolumeDrum(int volume);
	void setTemporaryVolumeDrum(int ch, int volume);

	// Set effect
	void setPanDrum(int ch, int value);

private:
	uint8_t keyOnFlagDrum_, keyOffFlagDrum_;
	int volDrum_[6], mVolDrum_, tmpVolDrum_[6];
	/// bit0: right on/off
	/// bit1: left on/off
	uint8_t panDrum_[6];
	bool isMuteDrum_[6];

	void initDrum();

	void setMuteDrumState(int ch, bool isMute);
	bool isMuteDrum(int ch);

	void updateKeyOnOffStatusDrum();

	/*----- ADPCM -----*/
public:
	// Key on-off
	void keyOnADPCM(Note note, int octave, int pitch, bool isJam = false);
	void keyOnADPCM(int echoBuf);
	void keyOffADPCM(bool isJam = false);
	void updateEchoBufferADPCM(int octave, Note note, int pitch);

	// Set instrument
	void setInstrumentADPCM(std::shared_ptr<InstrumentADPCM> inst);
	void updateInstrumentADPCM(int instNum);
	void clearSamplesADPCM();
	/// return: [0]: start address, [1]: stop address
	std::vector<size_t> storeSampleADPCM(std::vector<uint8_t> sample);

	// Set volume
	void setVolumeADPCM(int volume);
	void setTemporaryVolumeADPCM(int volume);

	// Set effect
	void setPanADPCM(int value);
//	void setArpeggioEffectADPCM(int second, int third);
//	void setPortamentoEffectADPCM(int depth, bool isTonePortamento = false);
//	void setVibratoEffectADPCM(int period, int depth);
//	void setTremoloEffectADPCM(int period, int depth);
//	void setVolumeSlideADPCM(int depth, bool isUp);
//	void setDetuneADPCM(int pitch);
//	void setNoteSlideADPCM(int speed, int seminote);
//	void setTransposeEffectADPCM(int seminote);

	// For state retrieve
	void haltSequencesADPCM();

	// Chip details
	bool isKeyOnADPCM() const;
	bool isTonePortamentoADPCM() const;
	ToneDetail getADPCMTone() const;

private:
	std::shared_ptr<InstrumentADPCM> refInstADPCM_;
	bool isKeyOnADPCM_;
	std::deque<ToneDetail> baseToneADPCM_;
	ToneDetail keyToneADPCM_;
	int sumPitchADPCM_;
	int baseVolADPCM_, tmpVolADPCM_;
	uint8_t panADPCM_;
	bool isMuteADPCM_;
	bool hasPreSetTickEventADPCM_;
	bool needEnvSetADPCM_;
	bool needToneSetADPCM_;
	size_t dramSize_;
	size_t storePointADPCM_;	// Move by 4 bytes
	std::unique_ptr<CommandSequence::Iterator> envItADPCM_;
	std::unique_ptr<CommandSequence::Iterator> tnItADPCM_;
	std::unique_ptr<SequenceIteratorInterface> arpItADPCM_;
	std::unique_ptr<CommandSequence::Iterator> ptItADPCM_;
	bool isArpEffADPCM_;
	int prtmADPCM_;
	bool isTonePrtmADPCM_;
	std::unique_ptr<WavingEffectIterator> vibItADPCM_;
	std::unique_ptr<WavingEffectIterator> treItADPCM_;
	int volSldADPCM_, sumVolSldADPCM_;
	int detuneADPCM_;
	std::unique_ptr<NoteSlideEffectIterator> nsItADPCM_;
	int sumNoteSldADPCM_;
	bool noteSldADPCMSetFlag_;
	int transposeADPCM_;

	void initADPCM();

	void setMuteADPCMState(bool isMuteFM);
	bool isMuteADPCM();

	void setFrontADPCMSequences();
	void releaseStartADPCMSequences();
	void tickEventADPCM();

	void writeEnvelopeADPCMToRegister(int seqPos);

	inline void checkRealToneADPCMByArpeggio(int seqPos)
	{
		checkRealToneByArpeggio(seqPos, arpItADPCM_, baseToneADPCM_, keyToneADPCM_, needToneSetADPCM_);
	}

	inline void checkPortamentoADPCM()
	{
		checkPortamento(arpItADPCM_, prtmADPCM_, isTonePrtmADPCM_, baseToneADPCM_,
						keyToneADPCM_, needToneSetADPCM_);
	}

	inline void checkRealToneADPCMByPitch(int seqPos)
	{
		checkRealToneByPitch(seqPos, ptItADPCM_, sumPitchADPCM_, needToneSetADPCM_);
	}

	void writePitchADPCM();

	void setRealVolumeADPCM();
};
