// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
//
// Author: Rahul Agnihotri
// Description: Simple JUCE additive synthesiser

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#include <math.h>
#include <stdlib.h>
#include <unistd.h>

class MainContentComponent :
    public AudioAppComponent,
    private ToggleButton::Listener
{
    public:
    MainContentComponent() : onOff (0), samplingRate(0.0)
    {
        //on/off button for the files
        // configuring on/off button and adding it to the main window
        addAndMakeVisible(onOffButton);
        onOffButton.addListener(this);

        // configuring on/off label and adding it to the main window
        addAndMakeVisible(onOffLabel);
        onOffLabel.setText ("Make Magic!", dontSendNotification);
        onOffLabel.attachToComponent (&onOffButton, true);

        setSize (150, 150);
        nChans = 1;
        setAudioChannels (0, nChans); // no inputs, one output
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    void resized() override
    {
        // placing the UI elements in the main window
        const int sliderLeft = 80;
        onOffButton.setBounds (100, 50, getWidth()/2, getHeight()/2);
    }
        
    //function to set freq and gain when keyboard key press is made
    bool keyPressed(const KeyPress & key)
    {
        double j;
        int k = rand() % 29+60;
        midi = k;
        j = pow(2,(midi-69)/12) * 440;
        sine[0].setFrequency(j);
        sine[1].setFrequency(0);
        sine[2].setFrequency(0);
        gain[0] = 0.4;
                
    }
    
    //function to set freq and gain when mouse position is moved
    void mouseMove(const MouseEvent & event)
    {
        double j;
        j = pow(2,(midi+4-69)/12) * 440;
        sine[1].setFrequency(j);
        gain[1] = 0.25;
    }
    
    //function to set freq and gain value when mouse button is pressed
    void mouseDown(const MouseEvent & event)
    {
        int x;
        double j,k;
        j = pow(2,(midi+7-69)/12) * 440;
        sine[2].setFrequency(j);
        gain[2] = 0.25;
    }
    
    
    void buttonClicked (Button* button)
    {
        // turns audio on or off
        if(button == &onOffButton && onOffButton.getToggleState())
        {            
            sine[0].setFrequency(0);
            sine[1].setFrequency(0);
            sine[2].setFrequency(0);
            sine[3].setFrequency(0);
            onOff = 1;
        }
        else{
            onOff = 0;
        }
    }

    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        samplingRate = sampleRate;
        for(int i = 0; i<3; i++)
            sine[i].setSamplingRate(sampleRate);
    }

    void releaseResources() override
    {
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // getting the audio output buffer to be filled
        float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);

        // computing one block
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            if(onOff == 1) buffer[sample] = sine[0].tick() * gain[0] + sine[1].tick() * gain[1] + sine[2].tick() * gain[2];
            else buffer[sample] = 0.0;
        }
    }


    private:
    // UI Elements
    ToggleButton onOffButton;
    Label onOffLabel;

    Sine sine[3]; // the sine wave oscillator

    // Global Variables
    double gain[3];
    double midi;
    int onOff, samplingRate, nChans;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
