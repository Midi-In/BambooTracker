#include "set_key_on_to_step_command.hpp"

SetKeyOnToStepCommand::SetKeyOnToStepCommand(std::weak_ptr<Module> mod, int songNum, int trackNum, int orderNum,
											 int stepNum, int noteNum, bool autosetInst, int instNum)
	: mod_(mod),
	  song_(songNum),
	  track_(trackNum),
	  order_(orderNum),
	  step_(stepNum),
	  note_(noteNum),
	  inst_(instNum),
	  autosetInst_(autosetInst)
{
	auto& st = mod_.lock()->getSong(songNum).getTrack(trackNum).getPatternFromOrderNumber(orderNum).getStep(stepNum);
	prevNote_ = st.getNoteNumber();
	if (autosetInst) prevInst_ = st.getInstrumentNumber();
}

void SetKeyOnToStepCommand::redo()
{
	auto& st = mod_.lock()->getSong(song_).getTrack(track_).getPatternFromOrderNumber(order_).getStep(step_);
	st.setNoteNumber(note_);
	if (autosetInst_) st.setInstrumentNumber(inst_);
}

void SetKeyOnToStepCommand::undo()
{
	auto& st = mod_.lock()->getSong(song_).getTrack(track_).getPatternFromOrderNumber(order_).getStep(step_);
	st.setNoteNumber(prevNote_);
	if (autosetInst_) st.setInstrumentNumber(prevInst_);
}

CommandId SetKeyOnToStepCommand::getID() const
{
	return CommandId::SetKeyOnToStep;
}
