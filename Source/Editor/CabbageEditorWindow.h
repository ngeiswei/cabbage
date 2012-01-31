/*
  ==============================================================================

    This file was auto-generated by the Jucer!

    It contains the basic outline for a simple desktop window.

  ==============================================================================
*/

#ifndef __MAINWINDOW_H_B41956AA__
#define __MAINWINDOW_H_B41956AA__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "CabbageCsoundEditor.h"


//==============================================================================
class CabbageEditorWindow   : public DocumentWindow,
								public ActionBroadcaster,
								public ActionListener
{
public:
    //==============================================================================
    CabbageEditorWindow();
    ~CabbageEditorWindow();

    void closeButtonPressed();
	CsoundEditor* contentComponent;

	void openCsoundFile(File input){
		contentComponent->openFile(input);
	}

	void actionListenerCallback (const String& message);

private:
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (CabbageEditorWindow)
};


#endif  // __MAINWINDOW_H_B41956AA__
