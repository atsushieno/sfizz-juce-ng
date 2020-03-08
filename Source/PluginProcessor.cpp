/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SfizzJuceNgAudioProcessor::SfizzJuceNgAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    synth_ = sfizz_create_synth();
}

SfizzJuceNgAudioProcessor::~SfizzJuceNgAudioProcessor()
{
    sfizz_free(synth_);
}

//==============================================================================
const String SfizzJuceNgAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SfizzJuceNgAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SfizzJuceNgAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SfizzJuceNgAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SfizzJuceNgAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SfizzJuceNgAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SfizzJuceNgAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SfizzJuceNgAudioProcessor::setCurrentProgram (int index)
{
}

const String SfizzJuceNgAudioProcessor::getProgramName (int index)
{
    return {};
}

void SfizzJuceNgAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SfizzJuceNgAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sfizz_set_volume(synth_, 4.0);
    sfizz_set_sample_rate(synth_, sampleRate);
    sfizz_set_samples_per_block(synth_, samplesPerBlock);
}

void SfizzJuceNgAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SfizzJuceNgAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SfizzJuceNgAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    MidiBuffer::Iterator iter{midiMessages};
    MidiMessage msg;
    int msgAt;
    while (iter.getNextEvent(msg, msgAt)) {
        if (msg.isNoteOn())
            // FIXME: calculate appropriate delay (but it's up to host...?)
            sfizz_send_note_on(synth_, msg.getTimeStamp(), msg.getNoteNumber(), msg.getVelocity());
        else if (msg.isNoteOff())
            sfizz_send_note_off(synth_, msg.getTimeStamp(), msg.getNoteNumber(), msg.getVelocity());
        else if (msg.isController())
            sfizz_send_cc(synth_, msg.getTimeStamp(), msg.getControllerNumber(), (char) msg.getControllerValue());
        else if (msg.isPitchWheel())
            sfizz_send_pitch_wheel(synth_, msg.getTimeStamp(), msg.getPitchWheelValue());
        else if (msg.isAftertouch())
            sfizz_send_aftertouch(synth_, msg.getTimeStamp(), (char) msg.getAfterTouchValue());
        else if (msg.isTempoMetaEvent())
            sfizz_send_tempo(synth_, msg.getTimeStamp(), msg.getTempoSecondsPerQuarterNote());
    }

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    sfizz_render_block(synth_, buffer.getArrayOfWritePointers(), buffer.getNumChannels(), buffer.getNumSamples());
}

//==============================================================================
bool SfizzJuceNgAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SfizzJuceNgAudioProcessor::createEditor()
{
    return new SfizzJuceNgAudioProcessorEditor (*this);
}

//==============================================================================
void SfizzJuceNgAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SfizzJuceNgAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SfizzJuceNgAudioProcessor();
}
