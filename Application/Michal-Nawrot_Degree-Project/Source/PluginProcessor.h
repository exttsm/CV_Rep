#pragma once

#include <JuceHeader.h>

class PluginProcessor : public juce::AudioProcessor,
    public juce::AudioProcessorValueTreeState::Listener
{
public:
    PluginProcessor();
    ~PluginProcessor() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram(int index) override;
    const juce::String getProgramName(int index) override;
    void changeProgramName(int index, const juce::String& newName) override;

    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;

    void parameterChanged(const juce::String& parameterID, float newValue) override;

    juce::AudioProcessorValueTreeState parameters;

private:
    // Effects
    juce::Reverb reverb;
    juce::Reverb::Parameters reverbParams;

    class Delay
    {
    public:
        Delay() : delayBuffer(2, 192000) {}

        void prepare(double sampleRate)
        {
            currentSampleRate = sampleRate;
            delayBuffer.clear();
            writePosition = 0;

            // Initialize filter coefficients
            float frequency = 1000.0f;  // Default cutoff frequency
            float q = 0.707f;           // Default Q factor
            lowPassFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(sampleRate, frequency, q));
        }

        void process(float* left, float* right, int numSamples)
        {
            for (int i = 0; i < numSamples; ++i)
            {
                const float leftIn = left[i];
                const float rightIn = right[i];

                int delayInSamples = static_cast<int>(delayTime * currentSampleRate);
                int readPosition = writePosition - delayInSamples;
                if (readPosition < 0) readPosition += delayBuffer.getNumSamples();

                // Get delayed samples
                float delayedLeft = delayBuffer.getSample(0, readPosition);
                float delayedRight = delayBuffer.getSample(1, readPosition);

                // Apply damping (lowpass filter)
                delayedLeft = lowPassFilter.processSingleSampleRaw(delayedLeft);
                delayedRight = lowPassFilter.processSingleSampleRaw(delayedRight);

                // Write to delay buffer with feedback
                delayBuffer.setSample(0, writePosition, leftIn + delayedLeft * feedback);
                delayBuffer.setSample(1, writePosition, rightIn + delayedRight * feedback);

                // Mix dry and wet
                left[i] = leftIn * (1.0f - mix) + delayedLeft * mix;
                right[i] = rightIn * (1.0f - mix) + delayedRight * mix;

                writePosition++;
                if (writePosition >= delayBuffer.getNumSamples())
                    writePosition = 0;
            }
        }

        void setParameters(float newDelayTime, float newFeedback, float newMix, float newDamping)
        {
            delayTime = newDelayTime;
            feedback = newFeedback;
            mix = newMix;

            // Update filter cutoff based on damping
            float frequency = juce::jmap(newDamping, 0.0f, 1.0f, 20000.0f, 1000.0f);
            lowPassFilter.setCoefficients(juce::IIRCoefficients::makeLowPass(currentSampleRate, frequency, 0.707f));
        }

    private:
        juce::AudioBuffer<float> delayBuffer;
        juce::IIRFilter lowPassFilter;
        double currentSampleRate = 44100.0;
        int writePosition = 0;
        float delayTime = 0.5f;   // seconds
        float feedback = 0.5f;    // 0 to 1
        float mix = 0.5f;         // 0 to 1
    };

    class Flanger
    {
    public:
        Flanger() : delayBuffer(2, 4800) {} // Max ~100ms at 48kHz

        void prepare(double sampleRate)
        {
            currentSampleRate = sampleRate;
            delayBuffer.clear();
            writePosition = 0;
            lfoPhase = 0.0f;
        }

        void process(float* left, float* right, int numSamples)
        {
            for (int i = 0; i < numSamples; ++i)
            {
                const float leftIn = left[i];
                const float rightIn = right[i];

                // Generate LFO value
                float lfoValue = std::sin(lfoPhase);
                lfoPhase += lfoIncrement;
                if (lfoPhase >= 2.0f * juce::MathConstants<float>::pi)
                    lfoPhase -= 2.0f * juce::MathConstants<float>::pi;

                // Calculate modulated delay time (1-10ms range)
                float modulatedDelay = baseDelay + (lfoValue * depth);
                int delayInSamples = static_cast<int>(modulatedDelay * currentSampleRate);
                delayInSamples = juce::jlimit(1, delayBuffer.getNumSamples() - 1, delayInSamples);

                int readPosition = writePosition - delayInSamples;
                if (readPosition < 0) readPosition += delayBuffer.getNumSamples();

                // Get delayed samples
                float delayedLeft = delayBuffer.getSample(0, readPosition);
                float delayedRight = delayBuffer.getSample(1, readPosition);

                // Write to delay buffer with feedback
                delayBuffer.setSample(0, writePosition, leftIn + delayedLeft * feedback);
                delayBuffer.setSample(1, writePosition, rightIn + delayedRight * feedback);

                // Mix dry and wet
                left[i] = leftIn * (1.0f - mix) + delayedLeft * mix;
                right[i] = rightIn * (1.0f - mix) + delayedRight * mix;

                writePosition++;
                if (writePosition >= delayBuffer.getNumSamples())
                    writePosition = 0;
            }
        }

        void setParameters(float newRate, float newDepth, float newFeedback, float newMix)
        {
            // Rate: 0.1 to 10 Hz
            float rate = juce::jmap(newRate, 0.0f, 1.0f, 0.1f, 10.0f);
            lfoIncrement = 2.0f * juce::MathConstants<float>::pi * rate / static_cast<float>(currentSampleRate);

            // Depth: 0 to 5ms modulation
            depth = juce::jmap(newDepth, 0.0f, 1.0f, 0.0f, 0.005f);

            feedback = newFeedback * 0.95f; // Limit feedback to prevent runaway
            mix = newMix;
        }

    private:
        juce::AudioBuffer<float> delayBuffer;
        double currentSampleRate = 44100.0;
        int writePosition = 0;
        float lfoPhase = 0.0f;
        float lfoIncrement = 0.0f;
        float baseDelay = 0.003f; // 3ms base delay
        float depth = 0.002f;     // 2ms depth
        float feedback = 0.5f;
        float mix = 0.5f;
    };

    class Chorus
    {
    public:
        Chorus() : delayBuffer(2, 9600) {} // Max ~200ms at 48kHz

        void prepare(double sampleRate)
        {
            currentSampleRate = sampleRate;
            delayBuffer.clear();
            writePosition = 0;

            // Initialize multiple LFO phases for chorus voices
            for (int i = 0; i < maxVoices; ++i)
            {
                lfoPhases[i] = i * 2.0f * juce::MathConstants<float>::pi / maxVoices;
            }
        }

        void process(float* left, float* right, int numSamples)
        {
            for (int i = 0; i < numSamples; ++i)
            {
                const float leftIn = left[i];
                const float rightIn = right[i];

                float chorusLeft = 0.0f;
                float chorusRight = 0.0f;

                // Process each chorus voice
                for (int voice = 0; voice < voices; ++voice)
                {
                    // Generate LFO value for this voice
                    float lfoValue = std::sin(lfoPhases[voice]);
                    lfoPhases[voice] += lfoIncrement;
                    if (lfoPhases[voice] >= 2.0f * juce::MathConstants<float>::pi)
                        lfoPhases[voice] -= 2.0f * juce::MathConstants<float>::pi;

                    // Calculate modulated delay time (10-50ms range)
                    float voiceDelay = baseDelay + (voice * voiceSpread) + (lfoValue * depth);
                    int delayInSamples = static_cast<int>(voiceDelay * currentSampleRate);
                    delayInSamples = juce::jlimit(1, delayBuffer.getNumSamples() - 1, delayInSamples);

                    int readPosition = writePosition - delayInSamples;
                    if (readPosition < 0) readPosition += delayBuffer.getNumSamples();

                    // Get delayed samples
                    float delayedLeft = delayBuffer.getSample(0, readPosition);
                    float delayedRight = delayBuffer.getSample(1, readPosition);

                    // Add to chorus mix
                    chorusLeft += delayedLeft;
                    chorusRight += delayedRight;
                }

                // Average the chorus voices
                chorusLeft /= static_cast<float>(voices);
                chorusRight /= static_cast<float>(voices);

                // Write to delay buffer (no feedback for chorus)
                delayBuffer.setSample(0, writePosition, leftIn);
                delayBuffer.setSample(1, writePosition, rightIn);

                // Mix dry and wet
                left[i] = leftIn * (1.0f - mix) + chorusLeft * mix;
                right[i] = rightIn * (1.0f - mix) + chorusRight * mix;

                writePosition++;
                if (writePosition >= delayBuffer.getNumSamples())
                    writePosition = 0;
            }
        }

        void setParameters(float newRate, float newDepth, float newMix, int newVoices)
        {
            // Rate: 0.1 to 5 Hz (slower than flanger)
            float rate = juce::jmap(newRate, 0.0f, 1.0f, 0.1f, 5.0f);
            lfoIncrement = 2.0f * juce::MathConstants<float>::pi * rate / static_cast<float>(currentSampleRate);

            // Depth: 0 to 15ms modulation
            depth = juce::jmap(newDepth, 0.0f, 1.0f, 0.0f, 0.015f);

            mix = newMix;
            voices = juce::jlimit(1, maxVoices, newVoices);
        }

    private:
        static const int maxVoices = 4;
        juce::AudioBuffer<float> delayBuffer;
        double currentSampleRate = 44100.0;
        int writePosition = 0;
        float lfoPhases[maxVoices];
        float lfoIncrement = 0.0f;
        float baseDelay = 0.020f;  // 20ms base delay
        float voiceSpread = 0.005f; // 5ms between voices
        float depth = 0.010f;      // 10ms depth
        float mix = 0.5f;
        int voices = 2;
    };

    class Vibrato
    {
    public:
        Vibrato() : delayBuffer(2, 2400) {} // Max ~50ms at 48kHz

        void prepare(double sampleRate)
        {
            currentSampleRate = sampleRate;
            delayBuffer.clear();
            writePosition = 0;
            lfoPhase = 0.0f;
        }

        void process(float* left, float* right, int numSamples)
        {
            for (int i = 0; i < numSamples; ++i)
            {
                const float leftIn = left[i];
                const float rightIn = right[i];

                // Generate LFO value
                float lfoValue = std::sin(lfoPhase);
                lfoPhase += lfoIncrement;
                if (lfoPhase >= 2.0f * juce::MathConstants<float>::pi)
                    lfoPhase -= 2.0f * juce::MathConstants<float>::pi;

                // Calculate modulated delay time (1-20ms range for pitch vibrato)
                float modulatedDelay = baseDelay + (lfoValue * depth);
                int delayInSamples = static_cast<int>(modulatedDelay * currentSampleRate);
                delayInSamples = juce::jlimit(1, delayBuffer.getNumSamples() - 1, delayInSamples);

                int readPosition = writePosition - delayInSamples;
                if (readPosition < 0) readPosition += delayBuffer.getNumSamples();

                // Get delayed samples
                float delayedLeft = delayBuffer.getSample(0, readPosition);
                float delayedRight = delayBuffer.getSample(1, readPosition);

                // Write to delay buffer (no feedback for vibrato)
                delayBuffer.setSample(0, writePosition, leftIn);
                delayBuffer.setSample(1, writePosition, rightIn);

                // Pure vibrato - only wet signal (no dry signal mixed in)
                left[i] = delayedLeft;
                right[i] = delayedRight;

                writePosition++;
                if (writePosition >= delayBuffer.getNumSamples())
                    writePosition = 0;
            }
        }

        void setParameters(float newRate, float newDepth)
        {
            // Rate: 1 to 15 Hz (faster than chorus, typical vibrato range)
            float rate = juce::jmap(newRate, 0.0f, 1.0f, 1.0f, 15.0f);
            lfoIncrement = 2.0f * juce::MathConstants<float>::pi * rate / static_cast<float>(currentSampleRate);

            // Depth: 0 to 10ms modulation (pitch vibrato range)
            depth = juce::jmap(newDepth, 0.0f, 1.0f, 0.0f, 0.010f);
        }

    private:
        juce::AudioBuffer<float> delayBuffer;
        double currentSampleRate = 44100.0;
        int writePosition = 0;
        float lfoPhase = 0.0f;
        float lfoIncrement = 0.0f;
        float baseDelay = 0.005f;  // 5ms base delay
        float depth = 0.005f;      // 5ms depth
    };

    class Phaser
    {
    public:
        Phaser()
        {
            // Initialize all-pass filter coefficients
            for (int stage = 0; stage < maxStages; ++stage)
            {
                for (int channel = 0; channel < 2; ++channel)
                {
                    allpassStates[stage][channel] = 0.0f;
                }
            }
        }

        void prepare(double sampleRate)
        {
            currentSampleRate = sampleRate;
            lfoPhase = 0.0f;

            // Clear all-pass filter states
            for (int stage = 0; stage < maxStages; ++stage)
            {
                for (int channel = 0; channel < 2; ++channel)
                {
                    allpassStates[stage][channel] = 0.0f;
                }
            }
        }

        void process(float* left, float* right, int numSamples)
        {
            for (int i = 0; i < numSamples; ++i)
            {
                const float leftIn = left[i];
                const float rightIn = right[i];

                // Generate LFO value
                float lfoValue = std::sin(lfoPhase);
                lfoPhase += lfoIncrement;
                if (lfoPhase >= 2.0f * juce::MathConstants<float>::pi)
                    lfoPhase -= 2.0f * juce::MathConstants<float>::pi;

                // Calculate modulated all-pass coefficient
                float modulatedCoeff = baseCoeff + (lfoValue * depth);
                modulatedCoeff = juce::jlimit(-0.95f, 0.95f, modulatedCoeff);

                // Process through all-pass filter stages
                float processedLeft = leftIn;
                float processedRight = rightIn;

                for (int stage = 0; stage < stages; ++stage)
                {
                    // Left channel all-pass filter
                    float leftOutput = -modulatedCoeff * processedLeft + allpassStates[stage][0];
                    allpassStates[stage][0] = processedLeft + modulatedCoeff * leftOutput;
                    processedLeft = leftOutput;

                    // Right channel all-pass filter
                    float rightOutput = -modulatedCoeff * processedRight + allpassStates[stage][1];
                    allpassStates[stage][1] = processedRight + modulatedCoeff * rightOutput;
                    processedRight = rightOutput;
                }

                // Add feedback
                processedLeft += processedLeft * feedback;
                processedRight += processedRight * feedback;

                // Mix dry and wet
                left[i] = leftIn * (1.0f - mix) + processedLeft * mix;
                right[i] = rightIn * (1.0f - mix) + processedRight * mix;
            }
        }

        void setParameters(float newRate, float newDepth, float newFeedback, float newMix, int newStages)
        {
            // Rate: 0.1 to 10 Hz
            float rate = juce::jmap(newRate, 0.0f, 1.0f, 0.1f, 10.0f);
            lfoIncrement = 2.0f * juce::MathConstants<float>::pi * rate / static_cast<float>(currentSampleRate);

            // Depth: 0 to 0.9 (all-pass coefficient range)
            depth = juce::jmap(newDepth, 0.0f, 1.0f, 0.0f, 0.7f);

            feedback = newFeedback * 0.95f; // Limit feedback to prevent runaway
            mix = newMix;
            stages = juce::jlimit(1, maxStages, newStages);
        }

    private:
        static const int maxStages = 8;
        double currentSampleRate = 44100.0;
        float lfoPhase = 0.0f;
        float lfoIncrement = 0.0f;
        float baseCoeff = 0.5f;    // Base all-pass coefficient
        float depth = 0.3f;       // Modulation depth
        float feedback = 0.5f;    // Feedback amount
        float mix = 0.5f;         // Dry/wet mix
        int stages = 4;           // Number of all-pass stages
        float allpassStates[maxStages][2]; // State for each stage and channel
    };

    Delay delay;
    Flanger flanger;
    Chorus chorus;
    Vibrato vibrato;
    Phaser phaser;
    bool reverbEnabled = true;
    bool delayEnabled = true;
    bool flangerEnabled = true;
    bool chorusEnabled = true;
    bool vibratoEnabled = true;
    bool phaserEnabled = true;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginProcessor)
};