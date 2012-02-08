
#include "CabbageCsoundEditor.h"
#include "CabbageEditorCommandManager.h"


#pragma warning(disable: 4996)
#pragma warning(disable: 4244)
//==============================================================================
CsoundEditor::CsoundEditor():hasChanged(false), unSaved(false)
{	
//textEditor = new CodeEditorComponent(csoundDoc, &csoundToker);
opcodes.addLines(String((BinaryData::opcodes_txt)));
csoundDoc.addListener(this);
textEditor = new CodeEditorExtended(csoundDoc, &csoundToker);
textEditor->setBounds(0, 0, getWidth(), getHeight());

output = new TextEditor("output");
output->setMultiLine(true);
output->setScrollbarsShown(true);
output->setReturnKeyStartsNewLine(true);
output->setReadOnly(true);

Font outputFont(T("Bitstream Vera Sans Mono"), 14, 0);
outputFont.setBold(true);
output->setFont(outputFont);
//background colour ID
Colour col(10, 10, 10);
output->setColour(0x1000200, col);
//text colour ID
output->setColour(0x1000201, Colours::white);

horizontalDividerBar = new StretchableLayoutResizerBar(&horizontalLayout, 1, false);

helpLabel = new Label();
helpLabel->setColour(Label::ColourIds::backgroundColourId, Colours::white);
helpLabel->setColour(Label::ColourIds::outlineColourId, Colours::green);
helpLabel->setColour(Label::ColourIds::textColourId, Colours::black);
outputFont.setHeight(14);
helpLabel->setFont(outputFont);
helpLabel->setText(T("Cabbage Csound Editor"), true);

addAndMakeVisible(textEditor);
addAndMakeVisible(horizontalDividerBar);
addAndMakeVisible(helpLabel);
addAndMakeVisible(output);

ScopedPointer<CommandManager> commandManager = CommandManager::getInstance();
commandManager->registerAllCommandsForTarget(this);
commandManager->getKeyMappings()->resetToDefaultMappings();
addKeyListener(commandManager->getKeyMappings());

horizontalLayout.setItemLayout (0,          // for item 0
    -.0, -.9,    // must be between 0 and 100 % in size
    -.7);      // and its preferred size is 70% of the total available space

// The resizer bar
horizontalLayout.setItemLayout (1,
    15, 15,
    15);

// The resizer bar
horizontalLayout.setItemLayout (2,
    35, 35,
    35);

horizontalLayout.setItemLayout (3,          // for item 2
    -0., -.9, // size must be between 0% and 60% of the available space
    -.3);        // and its preferred size is 30% of total available space

}
//==============================================================================
CsoundEditor::~CsoundEditor ()
{
//csoundDoc.setPositionMaintained(false);
}
//==============================================================================
void CsoundEditor::resized()
{
Component* comps[] = { textEditor, horizontalDividerBar, helpLabel, output };
// this will position the components, one beside the other, to fit
// horizontally into the rectangle provided.
horizontalLayout.layOutComponents (comps, 4, 0, 0, getWidth(), getHeight(), true, true);
}


void CsoundEditor::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
	switch (commandID)
    {
    case CommandIDs::fileNew:
		result.setInfo (T("New"), T("Create a new file"), CommandCategories::file, 0);
		result.addDefaultKeypress (T('n'), ModifierKeys::commandModifier);
        break;
    case CommandIDs::fileOpen:
		result.setInfo (T("Open"), T("Open a file"), CommandCategories::file, 0);
		result.addDefaultKeypress (T('o'), ModifierKeys::commandModifier);
        break;
    case CommandIDs::fileSave:
		result.setInfo (T("Save"), T("Save a file"), CommandCategories::file, 0);
		result.addDefaultKeypress (T('s'), ModifierKeys::commandModifier);
        break;
    case CommandIDs::fileSaveAs:
		result.setInfo (T("Save as"), T("Save file as.."), CommandCategories::file, 0);
		result.addDefaultKeypress (T('s'), ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
        break;

    case CommandIDs::editUndo:
		result.setInfo (T("Undo"), T("Undo last action"), CommandCategories::edit, 0);
		result.addDefaultKeypress (T('z'), ModifierKeys::commandModifier);
        break;
    case CommandIDs::editRedo:
		result.setInfo (T("Redo"), T("Redo last action"), CommandCategories::edit, 0);
		result.addDefaultKeypress (T('z'), ModifierKeys::shiftModifier | ModifierKeys::commandModifier);
        break;
    case CommandIDs::editCut:
		result.setInfo (T("Cut"), T("Cut selection"), CommandCategories::edit, 0);
		result.addDefaultKeypress (T('x'), ModifierKeys::commandModifier);
        break;
    case CommandIDs::editCopy:
		result.setInfo (T("Copy"), T("Copy selection"), CommandCategories::edit, 0);
		result.addDefaultKeypress (T('c'), ModifierKeys::commandModifier);
        break;
    case CommandIDs::editPaste:
		result.setInfo (T("Paste"), T("Paste selection"), CommandCategories::edit, 0);
		result.addDefaultKeypress (T('v'), ModifierKeys::commandModifier);
        break;

	}
}

void CsoundEditor::getAllCommands (Array <CommandID>& commands)
{
    const CommandID ids[] = {
								CommandIDs::fileNew,
								CommandIDs::fileOpen,
								CommandIDs::fileSave,
								CommandIDs::fileSaveAs,
								
								CommandIDs::editUndo,
								CommandIDs::editCopy,
								CommandIDs::editCut,
								CommandIDs::editPaste,
								CommandIDs::editRedo,
	};
	commands.addArray (ids, sizeof (ids) / sizeof (ids [0]));
}

bool CsoundEditor::perform (const InvocationInfo& info)
{

	switch (info.commandID)
    {
	//---------------------------------------------------------------------------------------------
	case CommandIDs::fileNew:
		{
			newFile("effect");
			break;
		}
	case CommandIDs::fileOpen:
		{			
			openFile();
			break;
		}
	case CommandIDs::fileSave:
		{
			saveFile();
			//toFront(true);
			repaint();
			break;
		}
	case CommandIDs::fileSaveAs:
		{
			break;
		}

	case CommandIDs::editUndo:
		{			
			break;
		}
	case CommandIDs::editCut:
		{			
			break;
		}
	case CommandIDs::editCopy:
		{			
			break;
		}
	case CommandIDs::editPaste:
		{			
			break;
		}

	case CommandIDs::editRedo:
		{			
			break;
		}
	}
return true;
}

//==============================================================================
const PopupMenu CsoundEditor::getMenuForIndex (int topLevelMenuIndex, const String& menuName)
{
ScopedPointer<CommandManager> commandManager = CommandManager::getInstance();
PopupMenu m1;
if(topLevelMenuIndex==0)
	{
	 m1.addCommandItem(commandManager, CommandIDs::fileNew);	 
	 m1.addCommandItem(commandManager, CommandIDs::fileOpen);
	 m1.addCommandItem(commandManager, CommandIDs::fileSave);
	 m1.addCommandItem(commandManager, CommandIDs::fileSaveAs);
	 m1.addSeparator();
	 return m1;
	}
else if(topLevelMenuIndex==1)
	{
	m1.addCommandItem(commandManager, CommandIDs::editUndo);
	m1.addCommandItem(commandManager, CommandIDs::editRedo);
	m1.addCommandItem(commandManager, CommandIDs::editCut);
	m1.addCommandItem(commandManager, CommandIDs::editCopy);
	m1.addCommandItem(commandManager, CommandIDs::editPaste);
	m1.addSeparator();

	return m1;
	}
else return m1;
}
//==============================================================================
void CsoundEditor::codeDocumentChanged (const CodeDocument::Position &affectedTextStart, const CodeDocument::Position &affectedTextEnd)
{
hasChanged = true;
String lineFromCsd = textEditor->editor->getDocument().getLine(affectedTextStart.getLineNumber());
String test1, test2, opcodeName, opcodeInfo, opcodeSyntax;
for(int i=0;i<opcodes.size();i++){
	test1 = opcodes[i].substring(1, 20);
	test2 = test1.substring(0, test1.indexOf("\""));
	opcodeName = T(" ")+test2;

	if(opcodeName.length()>2)
	if(lineFromCsd.contains(opcodeName)){
		test1 = opcodes[i].substring(opcodes[i].indexOf(";")+1, 500);
		test2 = test1.substring(test1.indexOf(";")+1, 500);
		opcodeInfo = test2.substring(0, test2.indexOf(";"));
		opcodeSyntax = test2.substring(test2.indexOf(";")+1, 1000);
		helpLabel->setText(opcodeName+T(" - ")+opcodeInfo+String("\n")+String("Syntax: ")+opcodeSyntax, true);
	}
	}
}

//==============================================================================
void CsoundEditor::openFile()
{
FileChooser openFC(T("Open a .csd file..."), File::nonexistent, T("*.csd"));
if(openFC.browseForFileToOpen())
	csoundDoc.replaceAllContent(openFC.getResult().loadFileAsString());
	openCsdFile = openFC.getResult();
	String filename = openCsdFile.getFullPathName();
	sendActionMessage(T("fileOpen|")+filename);
}

void CsoundEditor::openFile(File input)
{
	csoundDoc.replaceAllContent(input.loadFileAsString());
	openCsdFile = input;
}

//==============================================================================
void CsoundEditor::saveFile()
{
if(unSaved)
	saveFileAs();
else{
	openCsdFile.replaceWithText(csoundDoc.getAllContent());
	sendActionMessage("fileSaved");
	hasChanged = false;
	}
}

//==============================================================================
void CsoundEditor::saveFileAs()
{
FileChooser openFC(T("Save a .csd file..."), File::nonexistent, T("*.csd"));

if(openFC.browseForFileToSave(true)){
	File saveFile(openFC.getResult().getFullPathName());
	saveFile.replaceWithText(csoundDoc.getAllContent());
	unSaved = false;
}
textEditor->repaint();	
}

//===============================================================================
void CsoundEditor::newFile(String type)
{
if(type=="effect"){
unSaved = true;
String untitledCSD= "<CsoundSynthesizer>\n"
"<CsOptions>\n"
"-n -d\n"
"</CsOptions>\n"
"<CsInstruments>\n"
"sr = 44100\n"
"ksmps = 64\n"
"nchnls = 2\n"
"\n"
"instr 1\n"
"\n"
"\n"
"endin\n"
"\n"
"</CsInstruments>  \n"
"<CsScore>\n"
"f1 0 1024 10 1\n"
"i1 0 300\n"
"</CsScore>\n"
"</CsoundSynthesizer>";
csoundDoc.replaceAllContent(untitledCSD);
}

}