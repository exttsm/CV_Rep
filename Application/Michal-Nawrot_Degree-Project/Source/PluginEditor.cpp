#include "PluginEditor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : AudioProcessorEditor(&p)
    , processorRef(p)
{
    // Set up effect selector
    effectSelector.addItem("Reverb", 1);
    effectSelector.addItem("Delay", 2);
    effectSelector.addItem("Flanger", 3);
    effectSelector.addItem("Chorus", 4);
    effectSelector.addItem("Vibrato", 5);
    effectSelector.addItem("Phaser", 6);
    effectSelector.setSelectedId(1);
    effectSelector.onChange = [this] {
        if (effectSelector.getSelectedId() == 1)
            showReverbControls();
        else if (effectSelector.getSelectedId() == 2)
            showDelayControls();
        else if (effectSelector.getSelectedId() == 3)
            showFlangerControls();
        else if (effectSelector.getSelectedId() == 4)
            showChorusControls();
        else if (effectSelector.getSelectedId() == 5)
            showVibratoControls();
        else if (effectSelector.getSelectedId() == 6)
            showPhaserControls();
        };
    addAndMakeVisible(effectSelector);

    // Set up enable buttons
    reverbEnableButton.setButtonText("Reverb Enable");
    chorusEnableButton.setButtonText("Chorus Enable");
    vibratoEnableButton.setButtonText("Vibrato Enable");
    delayEnableButton.setButtonText("Delay Enable");
    flangerEnableButton.setButtonText("Flanger Enable");
    phaserEnableButton.setButtonText("Phaser Enable");
    addAndMakeVisible(reverbEnableButton);
    addAndMakeVisible(chorusEnableButton);
    addAndMakeVisible(vibratoEnableButton);
    addAndMakeVisible(delayEnableButton);
    addAndMakeVisible(flangerEnableButton);
    addAndMakeVisible(phaserEnableButton);

    // Setup Reverb Controls
    addChildComponent(reverbControls);

    roomSizeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    roomSizeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    reverbControls.addAndMakeVisible(roomSizeSlider);
    roomSizeLabel.setText("Room Size", juce::dontSendNotification);
    roomSizeLabel.attachToComponent(&roomSizeSlider, true);
    reverbControls.addAndMakeVisible(roomSizeLabel);

    dampingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dampingSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    reverbControls.addAndMakeVisible(dampingSlider);
    dampingLabel.setText("Damping", juce::dontSendNotification);
    dampingLabel.attachToComponent(&dampingSlider, true);
    reverbControls.addAndMakeVisible(dampingLabel);

    wetLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    wetLevelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    reverbControls.addAndMakeVisible(wetLevelSlider);
    wetLabel.setText("Wet Level", juce::dontSendNotification);
    wetLabel.attachToComponent(&wetLevelSlider, true);
    reverbControls.addAndMakeVisible(wetLabel);

    dryLevelSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    dryLevelSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    reverbControls.addAndMakeVisible(dryLevelSlider);
    dryLabel.setText("Dry Level", juce::dontSendNotification);
    dryLabel.attachToComponent(&dryLevelSlider, true);
    reverbControls.addAndMakeVisible(dryLabel);

    widthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    widthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    reverbControls.addAndMakeVisible(widthSlider);
    widthLabel.setText("Width", juce::dontSendNotification);
    widthLabel.attachToComponent(&widthSlider, true);
    reverbControls.addAndMakeVisible(widthLabel);

    freezeModeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    freezeModeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    reverbControls.addAndMakeVisible(freezeModeSlider);
    freezeLabel.setText("Freeze", juce::dontSendNotification);
    freezeLabel.attachToComponent(&freezeModeSlider, true);
    reverbControls.addAndMakeVisible(freezeLabel);

    // Setup Delay Controls
    addChildComponent(delayControls);

    delayTimeSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayTimeSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    delayControls.addAndMakeVisible(delayTimeSlider);
    delayTimeLabel.setText("Delay Time", juce::dontSendNotification);
    delayTimeLabel.attachToComponent(&delayTimeSlider, true);
    delayControls.addAndMakeVisible(delayTimeLabel);

    feedbackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    feedbackSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    delayControls.addAndMakeVisible(feedbackSlider);
    feedbackLabel.setText("Feedback", juce::dontSendNotification);
    feedbackLabel.attachToComponent(&feedbackSlider, true);
    delayControls.addAndMakeVisible(feedbackLabel);

    delayMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayMixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    delayControls.addAndMakeVisible(delayMixSlider);
    delayMixLabel.setText("Mix", juce::dontSendNotification);
    delayMixLabel.attachToComponent(&delayMixSlider, true);
    delayControls.addAndMakeVisible(delayMixLabel);

    delayDampingSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    delayDampingSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    delayControls.addAndMakeVisible(delayDampingSlider);
    delayDampingLabel.setText("Damping", juce::dontSendNotification);
    delayDampingLabel.attachToComponent(&delayDampingSlider, true);
    delayControls.addAndMakeVisible(delayDampingLabel);

    delaySyncBox.addItemList(juce::StringArray("Free", "1/1", "1/2", "1/4", "1/8", "1/16"), 1);
    delayControls.addAndMakeVisible(delaySyncBox);
    delaySyncLabel.setText("Sync", juce::dontSendNotification);
    delaySyncLabel.attachToComponent(&delaySyncBox, true);
    delayControls.addAndMakeVisible(delaySyncLabel);

    // Setup Flanger Controls
    addChildComponent(flangerControls);

    flangerRateSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    flangerRateSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    flangerControls.addAndMakeVisible(flangerRateSlider);
    flangerRateLabel.setText("Rate", juce::dontSendNotification);
    flangerRateLabel.attachToComponent(&flangerRateSlider, true);
    flangerControls.addAndMakeVisible(flangerRateLabel);

    flangerDepthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    flangerDepthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    flangerControls.addAndMakeVisible(flangerDepthSlider);
    flangerDepthLabel.setText("Depth", juce::dontSendNotification);
    flangerDepthLabel.attachToComponent(&flangerDepthSlider, true);
    flangerControls.addAndMakeVisible(flangerDepthLabel);

    flangerFeedbackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    flangerFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    flangerControls.addAndMakeVisible(flangerFeedbackSlider);
    flangerFeedbackLabel.setText("Feedback", juce::dontSendNotification);
    flangerFeedbackLabel.attachToComponent(&flangerFeedbackSlider, true);
    flangerControls.addAndMakeVisible(flangerFeedbackLabel);

    flangerMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    flangerMixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    flangerControls.addAndMakeVisible(flangerMixSlider);
    flangerMixLabel.setText("Mix", juce::dontSendNotification);
    flangerMixLabel.attachToComponent(&flangerMixSlider, true);
    flangerControls.addAndMakeVisible(flangerMixLabel);

    // Setup Chorus Controls
    addChildComponent(chorusControls);

    chorusRateSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    chorusRateSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    chorusControls.addAndMakeVisible(chorusRateSlider);
    chorusRateLabel.setText("Rate", juce::dontSendNotification);
    chorusRateLabel.attachToComponent(&chorusRateSlider, true);
    chorusControls.addAndMakeVisible(chorusRateLabel);

    chorusDepthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    chorusDepthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    chorusControls.addAndMakeVisible(chorusDepthSlider);
    chorusDepthLabel.setText("Depth", juce::dontSendNotification);
    chorusDepthLabel.attachToComponent(&chorusDepthSlider, true);
    chorusControls.addAndMakeVisible(chorusDepthLabel);

    chorusMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    chorusMixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    chorusControls.addAndMakeVisible(chorusMixSlider);
    chorusMixLabel.setText("Mix", juce::dontSendNotification);
    chorusMixLabel.attachToComponent(&chorusMixSlider, true);
    chorusControls.addAndMakeVisible(chorusMixLabel);

    chorusVoicesBox.addItemList(juce::StringArray("1", "2", "3", "4"), 1);
    chorusControls.addAndMakeVisible(chorusVoicesBox);
    chorusVoicesLabel.setText("Voices", juce::dontSendNotification);
    chorusVoicesLabel.attachToComponent(&chorusVoicesBox, true);
    chorusControls.addAndMakeVisible(chorusVoicesLabel);

    // Setup Vibrato Controls
    addChildComponent(vibratoControls);

    vibratoRateSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    vibratoRateSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    vibratoControls.addAndMakeVisible(vibratoRateSlider);
    vibratoRateLabel.setText("Rate", juce::dontSendNotification);
    vibratoRateLabel.attachToComponent(&vibratoRateSlider, true);
    vibratoControls.addAndMakeVisible(vibratoRateLabel);

    vibratoDepthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    vibratoDepthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    vibratoControls.addAndMakeVisible(vibratoDepthSlider);
    vibratoDepthLabel.setText("Depth", juce::dontSendNotification);
    vibratoDepthLabel.attachToComponent(&vibratoDepthSlider, true);
    vibratoControls.addAndMakeVisible(vibratoDepthLabel);

    // Setup Phaser Controls
    addChildComponent(phaserControls);

    phaserRateSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    phaserRateSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    phaserControls.addAndMakeVisible(phaserRateSlider);
    phaserRateLabel.setText("Rate", juce::dontSendNotification);
    phaserRateLabel.attachToComponent(&phaserRateSlider, true);
    phaserControls.addAndMakeVisible(phaserRateLabel);

    phaserDepthSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    phaserDepthSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    phaserControls.addAndMakeVisible(phaserDepthSlider);
    phaserDepthLabel.setText("Depth", juce::dontSendNotification);
    phaserDepthLabel.attachToComponent(&phaserDepthSlider, true);
    phaserControls.addAndMakeVisible(phaserDepthLabel);

    phaserFeedbackSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    phaserFeedbackSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    phaserControls.addAndMakeVisible(phaserFeedbackSlider);
    phaserFeedbackLabel.setText("Feedback", juce::dontSendNotification);
    phaserFeedbackLabel.attachToComponent(&phaserFeedbackSlider, true);
    phaserControls.addAndMakeVisible(phaserFeedbackLabel);

    phaserMixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
    phaserMixSlider.setTextBoxStyle(juce::Slider::TextBoxRight, false, 90, 20);
    phaserControls.addAndMakeVisible(phaserMixSlider);
    phaserMixLabel.setText("Mix", juce::dontSendNotification);
    phaserMixLabel.attachToComponent(&phaserMixSlider, true);
    phaserControls.addAndMakeVisible(phaserMixLabel);

    phaserStagesBox.addItemList(juce::StringArray("2", "4", "6", "8"), 1);
    phaserControls.addAndMakeVisible(phaserStagesBox);
    phaserStagesLabel.setText("Stages", juce::dontSendNotification);
    phaserStagesLabel.attachToComponent(&phaserStagesBox, true);
    phaserControls.addAndMakeVisible(phaserStagesLabel);

    // Create parameter attachments
    roomSizeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "roomSize", roomSizeSlider);
    dampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "damping", dampingSlider);
    wetLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "wetLevel", wetLevelSlider);
    dryLevelAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "dryLevel", dryLevelSlider);
    widthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "width", widthSlider);
    freezeModeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "freezeMode", freezeModeSlider);

    delayTimeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "delayTime", delayTimeSlider);
    feedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "delayFeedback", feedbackSlider);
    delayMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "delayMix", delayMixSlider);
    delayDampingAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "delayDamping", delayDampingSlider);
    delaySyncAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.parameters, "delaySync", delaySyncBox);

    flangerRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "flangerRate", flangerRateSlider);
    flangerDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "flangerDepth", flangerDepthSlider);
    flangerFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "flangerFeedback", flangerFeedbackSlider);
    flangerMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "flangerMix", flangerMixSlider);

    chorusRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "chorusRate", chorusRateSlider);
    chorusDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "chorusDepth", chorusDepthSlider);
    chorusMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "chorusMix", chorusMixSlider);
    chorusVoicesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.parameters, "chorusVoices", chorusVoicesBox);

    vibratoRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "vibratoRate", vibratoRateSlider);
    vibratoDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "vibratoDepth", vibratoDepthSlider);

    phaserRateAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "phaserRate", phaserRateSlider);
    phaserDepthAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "phaserDepth", phaserDepthSlider);
    phaserFeedbackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "phaserFeedback", phaserFeedbackSlider);
    phaserMixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
        processorRef.parameters, "phaserMix", phaserMixSlider);
    phaserStagesAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ComboBoxAttachment>(
        processorRef.parameters, "phaserStages", phaserStagesBox);

    reverbEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.parameters, "reverbEnabled", reverbEnableButton);
    chorusEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.parameters, "chorusEnabled", chorusEnableButton);
    vibratoEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.parameters, "vibratoEnabled", vibratoEnableButton);
    delayEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.parameters, "delayEnabled", delayEnableButton);
    flangerEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.parameters, "flangerEnabled", flangerEnableButton);
    phaserEnableAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
        processorRef.parameters, "phaserEnabled", phaserEnableButton);

    // Show initial view
    showReverbControls();

    // Set the plugin's window size
    setSize(600, 500);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));
}

void PluginEditor::showReverbControls()
{
    reverbControls.setVisible(true);
    delayControls.setVisible(false);
    flangerControls.setVisible(false);
    chorusControls.setVisible(false);
    vibratoControls.setVisible(false);
    phaserControls.setVisible(false);
    resized(); // Trigger a layout update
}

void PluginEditor::showDelayControls()
{
    reverbControls.setVisible(false);
    delayControls.setVisible(true);
    flangerControls.setVisible(false);
    chorusControls.setVisible(false);
    vibratoControls.setVisible(false);
    phaserControls.setVisible(false);
    resized(); // Trigger a layout update
}

void PluginEditor::showFlangerControls()
{
    reverbControls.setVisible(false);
    delayControls.setVisible(false);
    flangerControls.setVisible(true);
    chorusControls.setVisible(false);
    vibratoControls.setVisible(false);
    phaserControls.setVisible(false);
    resized(); // Trigger a layout update
}

void PluginEditor::showChorusControls()
{
    reverbControls.setVisible(false);
    delayControls.setVisible(false);
    flangerControls.setVisible(false);
    chorusControls.setVisible(true);
    vibratoControls.setVisible(false);
    phaserControls.setVisible(false);
    resized(); // Trigger a layout update
}

void PluginEditor::showVibratoControls()
{
    reverbControls.setVisible(false);
    delayControls.setVisible(false);
    flangerControls.setVisible(false);
    chorusControls.setVisible(false);
    vibratoControls.setVisible(true);
    phaserControls.setVisible(false);
    resized(); // Trigger a layout update
}

void PluginEditor::showPhaserControls()
{
    reverbControls.setVisible(false);
    delayControls.setVisible(false);
    flangerControls.setVisible(false);
    chorusControls.setVisible(false);
    vibratoControls.setVisible(false);
    phaserControls.setVisible(true);
    resized(); // Trigger a layout update
}

void PluginEditor::resized()
{
    auto area = getLocalBounds();
    const int topSectionHeight = 110; // Increased to 110 for bigger buttons
    const int labelWidth = 100;
    const int sliderHeight = 40;
    const int margin = 10;

    // Top section with effect selector and enable buttons
    auto topSection = area.removeFromTop(topSectionHeight);

    // Effect selector on the left - aligned to top with fixed height
    auto selectorArea = topSection.removeFromLeft(200).reduced(margin);
    effectSelector.setBounds(selectorArea.removeFromTop(25)); // Fixed dropdown height of 25px

    // Enable buttons on the right in rows
    auto buttonArea = topSection.reduced(margin);
    int buttonWidth = buttonArea.getWidth() / 2; // Split into 2 columns
    int buttonHeight = 28; // Increased from 25 to 28 for bigger buttons

    // First row of buttons
    auto firstButtonRow = buttonArea.removeFromTop(buttonHeight);
    reverbEnableButton.setBounds(firstButtonRow.removeFromLeft(buttonWidth).reduced(2));
    chorusEnableButton.setBounds(firstButtonRow.removeFromLeft(buttonWidth).reduced(2));

    // Second row of buttons
    buttonArea.removeFromTop(4); // Space between rows
    auto secondButtonRow = buttonArea.removeFromTop(buttonHeight);
    flangerEnableButton.setBounds(secondButtonRow.removeFromLeft(buttonWidth).reduced(2));
    delayEnableButton.setBounds(secondButtonRow.removeFromLeft(buttonWidth).reduced(2));

    // Third row of buttons - use fixed height
    buttonArea.removeFromTop(4); // Space between rows
    auto thirdButtonRow = buttonArea.removeFromTop(buttonHeight); // Use fixed buttonHeight
    vibratoEnableButton.setBounds(thirdButtonRow.removeFromLeft(buttonWidth).reduced(2));
    phaserEnableButton.setBounds(thirdButtonRow.removeFromLeft(buttonWidth).reduced(2));

    // Main controls area
    area.removeFromTop(margin);

    // Setup reverb controls bounds
    auto reverbArea = area;
    roomSizeSlider.setBounds(reverbArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    reverbArea.removeFromTop(margin);
    dampingSlider.setBounds(reverbArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    reverbArea.removeFromTop(margin);
    wetLevelSlider.setBounds(reverbArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    reverbArea.removeFromTop(margin);
    dryLevelSlider.setBounds(reverbArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    reverbArea.removeFromTop(margin);
    widthSlider.setBounds(reverbArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    reverbArea.removeFromTop(margin);
    freezeModeSlider.setBounds(reverbArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    reverbControls.setBounds(area);

    // Setup delay controls bounds
    auto delayArea = area;
    delayTimeSlider.setBounds(delayArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    delayArea.removeFromTop(margin);
    feedbackSlider.setBounds(delayArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    delayArea.removeFromTop(margin);
    delayMixSlider.setBounds(delayArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    delayArea.removeFromTop(margin);
    delayDampingSlider.setBounds(delayArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    delayArea.removeFromTop(margin);
    delaySyncBox.setBounds(delayArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    delayControls.setBounds(area);

    // Setup flanger controls bounds
    auto flangerArea = area;
    flangerRateSlider.setBounds(flangerArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    flangerArea.removeFromTop(margin);
    flangerDepthSlider.setBounds(flangerArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    flangerArea.removeFromTop(margin);
    flangerFeedbackSlider.setBounds(flangerArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    flangerArea.removeFromTop(margin);
    flangerMixSlider.setBounds(flangerArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    flangerControls.setBounds(area);

    // Setup chorus controls bounds
    auto chorusArea = area;
    chorusRateSlider.setBounds(chorusArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    chorusArea.removeFromTop(margin);
    chorusDepthSlider.setBounds(chorusArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    chorusArea.removeFromTop(margin);
    chorusMixSlider.setBounds(chorusArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    chorusArea.removeFromTop(margin);
    chorusVoicesBox.setBounds(chorusArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    chorusControls.setBounds(area);

    // Setup vibrato controls bounds
    auto vibratoArea = area;
    vibratoRateSlider.setBounds(vibratoArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    vibratoArea.removeFromTop(margin);
    vibratoDepthSlider.setBounds(vibratoArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    vibratoControls.setBounds(area);

    // Setup phaser controls bounds
    auto phaserArea = area;
    phaserRateSlider.setBounds(phaserArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    phaserArea.removeFromTop(margin);
    phaserDepthSlider.setBounds(phaserArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    phaserArea.removeFromTop(margin);
    phaserFeedbackSlider.setBounds(phaserArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    phaserArea.removeFromTop(margin);
    phaserMixSlider.setBounds(phaserArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    phaserArea.removeFromTop(margin);
    phaserStagesBox.setBounds(phaserArea.removeFromTop(sliderHeight).withTrimmedLeft(labelWidth));
    phaserControls.setBounds(area);
}