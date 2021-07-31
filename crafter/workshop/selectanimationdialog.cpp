#include "selectanimationdialog.h"

#include <QInputDialog>

SelectAnimationDialog::SelectAnimationDialog()
{

}

QString SelectAnimationDialog::exec() {
    QStringList animationTypes;
    animationTypes << "idle" << "walk" << "run" << "jump" << "slide" << "die";
    QInputDialog dialog;
    dialog.setComboBoxItems(animationTypes);
    if (dialog.exec() == QDialog::Accepted) {
        return dialog.textValue();
    }
    return "";
}
