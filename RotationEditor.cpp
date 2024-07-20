#include "RotationEditor.h"
#include "CompassButton.h"
#include "ArrowButton.h"

RotationEditor::RotationEditor() : UINode({
    256 + 16, 84,
    69 * 2, 24 * 2,
    "Resources/UI/RotateEditor",
    CGA_TRANSPARENT,
    StyleDirection::TOP,
    StyleDirection::RIGHT
})
{
    addChild(new CompassButton);
    addChild(new ArrowButton);
}