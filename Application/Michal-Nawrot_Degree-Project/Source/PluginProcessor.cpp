#include "PluginProcessor.h"
#include "PluginEditor.h"

PluginProcessor::PluginProcessor()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    parameters(*this, nullptr, "Parameters", {
    // Reverb parameters
    std::make_unique<juce::AudioParameterFloat>("roomSize", "Room Size", 0.0f, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("damping", "Damping", 0.0f, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("wetLevel", "Wet Level", 0.0f, 1.0f, 0.33f),
    std::make_unique<juce::AudioParameterFloat>("dryLevel", "Dry Level", 0.0f, 1.0f, 0.4f),
    std::make_unique<juce::AudioParameterFloat>("width", "Width", 0.0f, 1.0f, 1.0f),
    std::make_unique<juce::AudioParameterFloat>("freezeMode", "Freeze Mode", 0.0f, 1.0f, 0.0f),
    // Delay parameters
    std::make_unique<juce::AudioParameterFloat>("delayTime", "Delay Time", 0.0f, 2.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("delayFeedback", "Feedback", 0.0f, 0.95f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("delayMix", "Delay Mix", 0.0f, 1.0f, 0.5f),
    std::make_unique<juce::AudioParameterFloat>("delayDamping", "Delay Damping", 0.0f, 1.0f, 0.0f),
    std::make_unique<juce::AudioParameterChoice>("delaySync", "Delay Sync",
        juce::StringArray("Free", "1/1", "1/2", "1/4", "1/8", "1/16"), 0),
        // Flanger parameters
        std::make_unique<juce::AudioParameterFloat>("flangerRate", "Flanger Rate", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("flangerDepth", "Flanger Depth", 0.0f, 1.0f, 0.7f),
        std::make_unique<juce::AudioParameterFloat>("flangerFeedback", "Flanger Feedback", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterFloat>("flangerMix", "Flanger Mix", 0.0f, 1.0f, 0.5f),
        // Chorus parameters
        std::make_unique<juce::AudioParameterFloat>("chorusRate", "Chorus Rate", 0.0f, 1.0f, 0.3f),
        std::make_unique<juce::AudioParameterFloat>("chorusDepth", "Chorus Depth", 0.0f, 1.0f, 0.6f),
        std::make_unique<juce::AudioParameterFloat>("chorusMix", "Chorus Mix", 0.0f, 1.0f, 0.5f),
        std::make_unique<juce::AudioParameterChoice>("chorusVoices", "Chorus Voices",
            juce::StringArray("1", "2", "3", "4"), 1),
            // Vibrato parameters
            std::make_unique<juce::AudioParameterFloat>("vibratoRate", "Vibrato Rate", 0.0f, 1.0f, 0.4f),
            std::make_unique<juce::AudioParameterFloat>("vibratoDepth", "Vibrato Depth", 0.0f, 1.0f, 0.5f),
            // Phaser parameters
            std::make_unique<juce::AudioParameterFloat>("phaserRate", "Phaser Rate", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterFloat>("phaserDepth", "Phaser Depth", 0.0f, 1.0f, 0.6f),
            std::make_unique<juce::AudioParameterFloat>("phaserFeedback", "Phaser Feedback", 0.0f, 1.0f, 0.4f),
            std::make_unique<juce::AudioParameterFloat>("phaserMix", "Phaser Mix", 0.0f, 1.0f, 0.5f),
            std::make_unique<juce::AudioParameterChoice>("phaserStages", "Phaser Stages",
                juce::StringArray("2", "4", "6", "8"), 1),
                // Effect enable parameters
                std::make_unique<juce::AudioParameterBool>("reverbEnabled", "Reverb Enable", true),
                std::make_unique<juce::AudioParameterBool>("delayEnabled", "Delay Enable", true),
                std::make_unique<juce::AudioParameterBool>("flangerEnabled", "Flanger Enable", true),
                std::make_unique<juce::AudioParameterBool>("chorusEnabled", "Chorus Enable", true),
                std::make_unique<juce::AudioParameterBool>("vibratoEnabled", "Vibrato Enable", true),
                std::make_unique<juce::AudioParameterBool>("phaserEnabled", "Phaser Enable", true)
        })
{
    // Add listeners for all parameters
    parameters.addParameterListener("roomSize", this);
    parameters.addParameterListener("damping", this);
    parameters.addParameterListener("wetLevel", this);
    parameters.addParameterListener("dryLevel", this);
    parameters.addParameterListener("width", this);
    parameters.addParameterListener("freezeMode", this);
    parameters.addParameterListener("delayTime", this);
    parameters.addParameterListener("delayFeedback", this);
    parameters.addParameterListener("delayMix", this);
    parameters.addParameterListener("reverbEnabled", this);
    parameters.addParameterListener("delayEnabled", this);
    parameters.addParameterListener("delayDamping", this);
    parameters.addParameterListener("delaySync", this);
    parameters.addParameterListener("flangerRate", this);
    parameters.addParameterListener("flangerDepth", this);
    parameters.addParameterListener("flangerFeedback", this);
    parameters.addParameterListener("flangerMix", this);
    parameters.addParameterListener("flangerEnabled", this);
    parameters.addParameterListener("chorusRate", this);
    parameters.addParameterListener("chorusDepth", this);
    parameters.addParameterListener("chorusMix", this);
    parameters.addParameterListener("chorusVoices", this);
    parameters.addParameterListener("chorusEnabled", this);
    parameters.addParameterListener("vibratoRate", this);
    parameters.addParameterListener("vibratoDepth", this);
    parameters.addParameterListener("vibratoEnabled", this);
    parameters.addParameterListener("phaserRate", this);
    parameters.addParameterListener("phaserDepth", this);
    parameters.addParameterListener("phaserFeedback", this);
    parameters.addParameterListener("phaserMix", this);
    parameters.addParameterListener("phaserStages", this);
    parameters.addParameterListener("phaserEnabled", this);

    // Initialize reverb parameters
    reverbParams.roomSize = 0.5f;
    reverbParams.damping = 0.5f;
    reverbParams.wetLevel = 0.33f;
    reverbParams.dryLevel = 0.4f;
    reverbParams.width = 1.0f;
    reverbParams.freezeMode = 0.0f;
    reverb.setParameters(reverbParams);
}

PluginProcessor::~PluginProcessor()
{
    parameters.removeParameterListener("roomSize", this);
    parameters.removeParameterListener("damping", this);
    parameters.removeParameterListener("wetLevel", this);
    parameters.removeParameterListener("dryLevel", this);
    parameters.removeParameterListener("width", this);
    parameters.removeParameterListener("freezeMode", this);
    parameters.removeParameterListener("delayTime", this);
    parameters.removeParameterListener("delayFeedback", this);
    parameters.removeParameterListener("delayMix", this);
    parameters.removeParameterListener("reverbEnabled", this);
    parameters.removeParameterListener("delayEnabled", this);
    parameters.removeParameterListener("delayDamping", this);
    parameters.removeParameterListener("delaySync", this);
    parameters.removeParameterListener("flangerRate", this);
    parameters.removeParameterListener("flangerDepth", this);
    parameters.removeParameterListener("flangerFeedback", this);
    parameters.removeParameterListener("flangerMix", this);
    parameters.removeParameterListener("flangerEnabled", this);
    parameters.removeParameterListener("chorusRate", this);
    parameters.removeParameterListener("chorusDepth", this);
    parameters.removeParameterListener("chorusMix", this);
    parameters.removeParameterListener("chorusVoices", this);
    parameters.removeParameterListener("chorusEnabled", this);
    parameters.removeParameterListener("vibratoRate", this);
    parameters.removeParameterListener("vibratoDepth", this);
    parameters.removeParameterListener("vibratoEnabled", this);
    parameters.removeParameterListener("phaserRate", this);
    parameters.removeParameterListener("phaserDepth", this);
    parameters.removeParameterListener("phaserFeedback", this);
    parameters.removeParameterListener("phaserMix", this);
    parameters.removeParameterListener("phaserStages", this);
    parameters.removeParameterListener("phaserEnabled", this);
}

void PluginProcessor::parameterChanged(const juce::String& parameterID, float newValue)
{
    if (parameterID == "roomSize")
        reverbParams.roomSize = newValue;
    else if (parameterID == "damping")
        reverbParams.damping = newValue;
    else if (parameterID == "wetLevel")
        reverbParams.wetLevel = newValue;
    else if (parameterID == "dryLevel")
        reverbParams.dryLevel = newValue;
    else if (parameterID == "width")
        reverbParams.width = newValue;
    else if (parameterID == "freezeMode")
        reverbParams.freezeMode = newValue;
    else if (parameterID == "delayTime" || parameterID == "delayFeedback" ||
        parameterID == "delayMix" || parameterID == "delayDamping" ||
        parameterID == "delaySync")
    {
        float delayTime = *parameters.getRawParameterValue("delayTime");
        float feedback = *parameters.getRawParameterValue("delayFeedback");
        float mix = *parameters.getRawParameterValue("delayMix");
        float damping = *parameters.getRawParameterValue("delayDamping");

        // Handle tempo sync
        int syncMode = static_cast<int>(*parameters.getRawParameterValue("delaySync"));
        if (syncMode > 0)
        {
            auto hostBpm = getPlayHead()->getPosition()->getBpm();
            if (hostBpm.hasValue())
            {
                // Calculate delay time based on tempo and note length
                double bpm = *hostBpm;
                double beatsPerSecond = bpm / 60.0;
                double wholeBeatLength = 1.0 / beatsPerSecond;

                switch (syncMode)
                {
                case 1: delayTime = wholeBeatLength; break;        // 1/1
                case 2: delayTime = wholeBeatLength * 0.5; break;  // 1/2
                case 3: delayTime = wholeBeatLength * 0.25; break; // 1/4
                case 4: delayTime = wholeBeatLength * 0.125; break;// 1/8
                case 5: delayTime = wholeBeatLength * 0.0625; break;// 1/16
                }
            }
        }

        delay.setParameters(delayTime, feedback, mix, damping);
    }
    else if (parameterID == "flangerRate" || parameterID == "flangerDepth" ||
        parameterID == "flangerFeedback" || parameterID == "flangerMix")
    {
        float rate = *parameters.getRawParameterValue("flangerRate");
        float depth = *parameters.getRawParameterValue("flangerDepth");
        float feedback = *parameters.getRawParameterValue("flangerFeedback");
        float mix = *parameters.getRawParameterValue("flangerMix");
        flanger.setParameters(rate, depth, feedback, mix);
    }
    else if (parameterID == "chorusRate" || parameterID == "chorusDepth" ||
        parameterID == "chorusMix" || parameterID == "chorusVoices")
    {
        float rate = *parameters.getRawParameterValue("chorusRate");
        float depth = *parameters.getRawParameterValue("chorusDepth");
        float mix = *parameters.getRawParameterValue("chorusMix");
        int voices = static_cast<int>(*parameters.getRawParameterValue("chorusVoices")) + 1; // 0-3 becomes 1-4
        chorus.setParameters(rate, depth, mix, voices);
    }
    else if (parameterID == "vibratoRate" || parameterID == "vibratoDepth")
    {
        float rate = *parameters.getRawParameterValue("vibratoRate");
        float depth = *parameters.getRawParameterValue("vibratoDepth");
        vibrato.setParameters(rate, depth);
    }
    else if (parameterID == "phaserRate" || parameterID == "phaserDepth" ||
        parameterID == "phaserFeedback" || parameterID == "phaserMix" || parameterID == "phaserStages")
    {
        float rate = *parameters.getRawParameterValue("phaserRate");
        float depth = *parameters.getRawParameterValue("phaserDepth");
        float feedback = *parameters.getRawParameterValue("phaserFeedback");
        float mix = *parameters.getRawParameterValue("phaserMix");
        int stages = static_cast<int>(*parameters.getRawParameterValue("phaserStages")) * 2 + 2; // 0-3 becomes 2,4,6,8
        phaser.setParameters(rate, depth, feedback, mix, stages);
    }
    else if (parameterID == "reverbEnabled")
        reverbEnabled = newValue >= 0.5f;
    else if (parameterID == "delayEnabled")
        delayEnabled = newValue >= 0.5f;
    else if (parameterID == "flangerEnabled")
        flangerEnabled = newValue >= 0.5f;
    else if (parameterID == "chorusEnabled")
        chorusEnabled = newValue >= 0.5f;
    else if (parameterID == "vibratoEnabled")
        vibratoEnabled = newValue >= 0.5f;
    else if (parameterID == "phaserEnabled")
        phaserEnabled = newValue >= 0.5f;

    if (parameterID.startsWith("reverb"))
        reverb.setParameters(reverbParams);
}

void PluginProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
    reverb.setSampleRate(sampleRate);
    delay.prepare(sampleRate);
    flanger.prepare(sampleRate);
    chorus.prepare(sampleRate);
    vibrato.prepare(sampleRate);
    phaser.prepare(sampleRate);
}

void PluginProcessor::releaseResources()
{
    // Empty release resources method
}

void PluginProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // Process phaser if enabled
    if (phaserEnabled && totalNumInputChannels == 2)
    {
        float* leftChannel = buffer.getWritePointer(0);
        float* rightChannel = buffer.getWritePointer(1);
        phaser.process(leftChannel, rightChannel, buffer.getNumSamples());
    }

    // Process vibrato if enabled
    if (vibratoEnabled && totalNumInputChannels == 2)
    {
        float* leftChannel = buffer.getWritePointer(0);
        float* rightChannel = buffer.getWritePointer(1);
        vibrato.process(leftChannel, rightChannel, buffer.getNumSamples());
    }

    // Process chorus if enabled
    if (chorusEnabled && totalNumInputChannels == 2)
    {
        float* leftChannel = buffer.getWritePointer(0);
        float* rightChannel = buffer.getWritePointer(1);
        chorus.process(leftChannel, rightChannel, buffer.getNumSamples());
    }

    // Process flanger if enabled
    if (flangerEnabled && totalNumInputChannels == 2)
    {
        float* leftChannel = buffer.getWritePointer(0);
        float* rightChannel = buffer.getWritePointer(1);
        flanger.process(leftChannel, rightChannel, buffer.getNumSamples());
    }

    // Process delay if enabled
    if (delayEnabled && totalNumInputChannels == 2)
    {
        float* leftChannel = buffer.getWritePointer(0);
        float* rightChannel = buffer.getWritePointer(1);
        delay.process(leftChannel, rightChannel, buffer.getNumSamples());
    }

    // Process reverb if enabled
    if (reverbEnabled)
    {
        if (totalNumInputChannels == 1)
        {
            float* channelData = buffer.getWritePointer(0);
            reverb.processMono(channelData, buffer.getNumSamples());
        }
        else
        {
            float* leftChannel = buffer.getWritePointer(0);
            float* rightChannel = buffer.getWritePointer(1);
            reverb.processStereo(leftChannel, rightChannel, buffer.getNumSamples());
        }
    }
}

juce::AudioProcessorEditor* PluginProcessor::createEditor() { return new PluginEditor(*this); }
bool PluginProcessor::hasEditor() const { return true; }
const juce::String PluginProcessor::getName() const { return JucePlugin_Name; }
bool PluginProcessor::acceptsMidi() const { return false; }
bool PluginProcessor::producesMidi() const { return false; }
bool PluginProcessor::isMidiEffect() const { return false; }
double PluginProcessor::getTailLengthSeconds() const { return 0.0; }
int PluginProcessor::getNumPrograms() { return 1; }
int PluginProcessor::getCurrentProgram() { return 0; }
void PluginProcessor::setCurrentProgram(int index) {}
const juce::String PluginProcessor::getProgramName(int index) { return {}; }
void PluginProcessor::changeProgramName(int index, const juce::String& newName) {}
void PluginProcessor::getStateInformation(juce::MemoryBlock& destData) {}
void PluginProcessor::setStateInformation(const void* data, int sizeInBytes) {}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}