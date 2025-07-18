#pragma once

#include "PluginProcessor.h"

class PluginEditor : public juce::AudioProcessorEditor
{
public:
    explicit PluginEditor(PluginProcessor&);
    ~PluginEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    void showReverbControls();
    void showDelayControls();
    void showFlangerControls();
    void showChorusControls();
    void showVibratoControls();
    void showPhaserControls();

    PluginProcessor& processorRef;

    // Effect selection
    juce::ComboBox effectSelector;

    // Effect enable buttons
    juce::ToggleButton reverbEnableButton;
    juce::ToggleButton chorusEnableButton;
    juce::ToggleButton vibratoEnableButton;
    juce::ToggleButton delayEnableButton;
    juce::ToggleButton flangerEnableButton;
    juce::ToggleButton phaserEnableButton;

    // Reverb controls
    juce::Component reverbControls;
    juce::Slider roomSizeSlider;
    juce::Slider dampingSlider;
    juce::Slider wetLevelSlider;
    juce::Slider dryLevelSlider;
    juce::Slider widthSlider;
    juce::Slider freezeModeSlider;

    juce::Label roomSizeLabel;
    juce::Label dampingLabel;
    juce::Label wetLabel;
    juce::Label dryLabel;
    juce::Label widthLabel;
    juce::Label freezeLabel;

    // Delay controls
    juce::Component delayControls;
    juce::Slider delayTimeSlider;
    juce::Slider feedbackSlider;
    juce::Slider delayMixSlider;
    juce::Slider delayDampingSlider;
    juce::ComboBox delaySyncBox;

    juce::Label delayTimeLabel;
    juce::Label feedbackLabel;
    juce::Label delayMixLabel;
    juce::Label delayDampingLabel;
    juce::Label delaySyncLabel;

    // Flanger controls
    juce::Component flangerControls;
    juce::Slider flangerRateSlider;
    juce::Slider flangerDepthSlider;
    juce::Slider flangerFeedbackSlider;
    juce::Slider flangerMixSlider;

    juce::Label flangerRateLabel;
    juce::Label flangerDepthLabel;
    juce::Label flangerFeedbackLabel;
    juce::Label flangerMixLabel;

    // Chorus controls
    juce::Component chorusControls;
    juce::Slider chorusRateSlider;
    juce::Slider chorusDepthSlider;
    juce::Slider chorusMixSlider;
    juce::ComboBox chorusVoicesBox;

    juce::Label chorusRateLabel;
    juce::Label chorusDepthLabel;
    juce::Label chorusMixLabel;
    juce::Label chorusVoicesLabel;

    // Vibrato controls
    juce::Component vibratoControls;
    juce::Slider vibratoRateSlider;
    juce::Slider vibratoDepthSlider;

    juce::Label vibratoRateLabel;
    juce::Label vibratoDepthLabel;

    // Phaser controls
    juce::Component phaserControls;
    juce::Slider phaserRateSlider;
    juce::Slider phaserDepthSlider;
    juce::Slider phaserFeedbackSlider;
    juce::Slider phaserMixSlider;
    juce::ComboBox phaserStagesBox;

    juce::Label phaserRateLabel;
    juce::Label phaserDepthLabel;
    juce::Label phaserFeedbackLabel;
    juce::Label phaserMixLabel;
    juce::Label phaserStagesLabel;

    // Parameter attachments
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> roomSizeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> wetLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> dryLevelAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> widthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> freezeModeAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayTimeAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> feedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> delayDampingAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> delaySyncAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> flangerRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> flangerDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> flangerFeedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> flangerMixAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> chorusMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> chorusVoicesAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> vibratoDepthAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserRateAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserDepthAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserFeedbackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> phaserMixAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ComboBoxAttachment> phaserStagesAttachment;

    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> reverbEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> chorusEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> vibratoEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> delayEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> flangerEnableAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> phaserEnableAttachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginEditor)
};