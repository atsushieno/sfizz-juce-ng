/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SfizzJuceNgAudioProcessorEditor::SfizzJuceNgAudioProcessorEditor (SfizzJuceNgAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    auto synth = dynamic_cast<SfizzJuceNgAudioProcessor*>(&p)->synth_;
    file_chooser_button.setButtonText("(Choose SoundFont file)");
    file_chooser_listener.reset(new ChooseSoundFontButtonListener(this, synth));
    file_chooser_button.addListener(file_chooser_listener.get());

    addAndMakeVisible(file_chooser_button);

    midi_state.addListener(this);
    addAndMakeVisible(midi_keyboard);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setResizable(true, true);
    setSize (400, 300);
}

SfizzJuceNgAudioProcessorEditor::~SfizzJuceNgAudioProcessorEditor()
{
}

//==============================================================================
void SfizzJuceNgAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
    auto numVoicesText = String::formatted ("%d", sfizz_get_num_active_voices(processor.synth_));
    g.drawFittedText (numVoicesText, getLocalBounds(), Justification::centred, 1);
}

void SfizzJuceNgAudioProcessorEditor::resized()
{
    file_chooser_button.setSize(200, 30);
    midi_keyboard.setBounds(0, getHeight() - 100, getWidth(), 100);
}

void SfizzJuceNgAudioProcessorEditor::handleNoteOn (MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    sfizz_send_note_on(processor.synth_, 0, midiNoteNumber, (char) (velocity * 128));
}

void SfizzJuceNgAudioProcessorEditor::handleNoteOff (MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity)
{
    sfizz_send_note_off(processor.synth_, 0, midiNoteNumber, (char) (velocity * 128));
}

void SfizzJuceNgAudioProcessorEditor::ChooseSoundFontButtonListener::buttonClicked(Button *button) {
    FileChooser fileChooser{"Select SoundFont file"};
    if (fileChooser.browseForFileToOpen()) {
        parent->file_chooser_button.setButtonText(fileChooser.getResult().getFileName());
        sfizz_load_file(parent->processor.synth_, fileChooser.getResult().getFullPathName().toRawUTF8());
        sfizz_set_num_voices(parent->processor.synth_, 128);
    }
}

void SfizzJuceNgAudioProcessorEditor::ChooseSoundFontButtonListener::buttonStateChanged(Button *button) {
    Listener::buttonStateChanged(button);
}
