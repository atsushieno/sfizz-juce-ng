/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SfizzJuceNgAudioProcessorEditor  : public AudioProcessorEditor, MidiKeyboardStateListener
{
    friend class ChooseSoundFontButtonListener;

public:
    SfizzJuceNgAudioProcessorEditor (SfizzJuceNgAudioProcessor&);
    ~SfizzJuceNgAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    class ChooseSoundFontButtonListener : public Button::Listener {
        SfizzJuceNgAudioProcessorEditor *parent;
    public:
        ChooseSoundFontButtonListener(SfizzJuceNgAudioProcessorEditor *parent, sfizz_synth_t* synth)
            : parent(parent) {}

        void buttonClicked(Button *button) override;
        void buttonStateChanged(Button *button) override;
    };

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SfizzJuceNgAudioProcessor& processor;
    TextButton file_chooser_button;
    std::unique_ptr<ChooseSoundFontButtonListener> file_chooser_listener;

    MidiKeyboardState midi_state;
    MidiKeyboardComponent midi_keyboard{midi_state, MidiKeyboardComponent::Orientation::horizontalKeyboard};

    void handleNoteOn (MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;
    void handleNoteOff (MidiKeyboardState *source, int midiChannel, int midiNoteNumber, float velocity) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SfizzJuceNgAudioProcessorEditor)
};
