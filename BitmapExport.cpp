/*
 *    This file is part of layoutie.
 *
 *    layoutie is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    layoutie is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with layoutie.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "BitmapExport.h"
#include "ui_BitmapExport.h"
#include "BoardScene.h"

#include "Settings.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QGLWidget>

static const float cCentimetersPerInch = 2.54; // cm per inch

BitmapExport::BitmapExport(QWidget* inParent, const SLFormat::Board* inBoard)
    : QDialog(inParent), ui(new Ui::BitmapExport), mBoard(inBoard)
{
    ui->setupUi(this);

    if (QGLFormat::hasOpenGL())
    {
        ui->graphicsView->setViewport(new QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::Rgba | QGL::AlphaChannel)));
        ui->graphicsView->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
        ui->graphicsView->setRenderHint(QPainter::HighQualityAntialiasing);

        ui->graphicsView->setFrameStyle(QFrame::NoFrame); // semitransparent frame causes artifacts when using OpenGL
    }

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);

    mScene = new BoardScene(const_cast<SLFormat::Board*>(mBoard), ui->graphicsView);
    ui->graphicsView->setScene(mScene);

    mLayerCheckBoxes[0] = ui->layer0CheckBox;
    mLayerCheckBoxes[1] = ui->layer1CheckBox;
    mLayerCheckBoxes[2] = ui->layer2CheckBox;
    mLayerCheckBoxes[3] = ui->layer3CheckBox;
    mLayerCheckBoxes[4] = ui->layer4CheckBox;
    mLayerCheckBoxes[5] = ui->layer5CheckBox;
    mLayerCheckBoxes[6] = ui->layer6CheckBox;

    for (unsigned layer = 0; layer < SLFormat::Board::cNumberLayers; layer++)
    {
        mLayerCheckBoxes[layer]->setText(gSettings().layerName(layer));
        mLayerCheckBoxes[layer]->setChecked(mBoard->isLayerVisible(layer));
        mLayerCheckBoxes[layer]->setEnabled(mBoard->isLayerEnabled(layer));
        connect(mLayerCheckBoxes[layer], SIGNAL(toggled(bool)), SLOT(updateView()));
    }

    connect(ui->showGridCheckBox, SIGNAL(toggled(bool)), SLOT(updateView()));
    connect(ui->drillHolesCheckBox, SIGNAL(toggled(bool)), SLOT(updateView()));

    updateView();

    connect(ui->resolutionSlider, SIGNAL(valueChanged(int)), SLOT(resolutionSliderChanged(int)));
    connect(ui->resolutionEdit, SIGNAL(textChanged(QString)), SLOT(resolutionEditChanged(QString)));
    connect(ui->resolutionUnitsComboBox, SIGNAL(currentIndexChanged(int)), SLOT(resolutionUnitsChanged()));
    resolutionUnitsChanged();
    resolutionSliderChanged(ui->resolutionSlider->value());

    connect(ui->buttonBox, SIGNAL(accepted()), SLOT(accepted()));
    connect(ui->buttonBox, SIGNAL(rejected()), SLOT(rejected()));

    gSettings().restoreSettings(this);
}

BitmapExport::~BitmapExport()
{
    gSettings().saveSettings(this);
    delete ui;
}

void BitmapExport::resizeEvent(QResizeEvent* inEvent)
{
    QDialog::resizeEvent(inEvent);
    int borderWidth = ui->graphicsView->lineWidth() + 5;
    float scaleFactor = (ui->graphicsView->rect().width() - 2 * borderWidth) / mScene->sceneRect().width();
    float scaleFactor2 = (ui->graphicsView->rect().height() - 2 * borderWidth) / mScene->sceneRect().height();
    if (scaleFactor > scaleFactor2)
        scaleFactor = scaleFactor2;
    ui->graphicsView->resetTransform();
    ui->graphicsView->scale(scaleFactor, scaleFactor);
}

void BitmapExport::updateView()
{
    mScene->showGrid(ui->showGridCheckBox->isChecked());
    mScene->showDrillHoles(ui->drillHolesCheckBox->isChecked());

    for (unsigned layer = 0; layer < SLFormat::Board::cNumberLayers; layer++)
    {
        mScene->showLayer(layer, mLayerCheckBoxes[layer]->isChecked());
    }

    ui->graphicsView->update();
}

void BitmapExport::resolutionUnitsChanged()
{
    const int maxDPI = 1200;
    const int minDPI = 300;
    const int singleStep = 10;
    const int pageStep = 100;

    const bool inches = ui->resolutionUnitsComboBox->currentIndex() == 0;
    if (inches)
    {
        int res = ui->resolutionSlider->value() * cCentimetersPerInch; // translate to inches from cm
        ui->resolutionSlider->setMinimum(minDPI);
        ui->resolutionSlider->setMaximum(maxDPI);
        ui->resolutionSlider->setSingleStep(singleStep);
        ui->resolutionSlider->setPageStep(pageStep);
        ui->resolutionSlider->setValue(res);
    }
    else
    {
        int res = ui->resolutionSlider->value() / cCentimetersPerInch; // translate to centimeters from inches
        ui->resolutionSlider->setMinimum(minDPI / cCentimetersPerInch);
        ui->resolutionSlider->setMaximum(maxDPI / cCentimetersPerInch);
        ui->resolutionSlider->setSingleStep(singleStep / cCentimetersPerInch);
        ui->resolutionSlider->setPageStep(pageStep / cCentimetersPerInch);
        ui->resolutionSlider->setValue(res);
    }
}

void BitmapExport::resolutionEditChanged(const QString& inValue)
{
    bool ok;
    int value = inValue.toInt(&ok);
    if (ok)
    {
        ui->resolutionSlider->setValue(value);
    }
}

void BitmapExport::resolutionSliderChanged(int inValue)
{
    ui->resolutionEdit->setText(QString::number(inValue));
}

void BitmapExport::accepted()
{
    bool ok;
    double pixelsPerCm = ui->resolutionEdit->text().toInt(&ok);
    if (!ok)
    {
        QMessageBox::critical(this, tr("Error"), tr("Wrong image resolution.\nPlease set the correct value."));
        ui->resolutionEdit->setFocus();
        return;
    }
    const bool inches = ui->resolutionUnitsComboBox->currentIndex() == 0;
    if (inches)
        pixelsPerCm /= cCentimetersPerInch;

    QString fileName = QFileDialog::getSaveFileName(this, tr("Export bitmap"), QDir::currentPath(), tr("BMP image (*.bmp);;JPG image (*.jpg), PNG image (*.png)"));
    if (fileName.isEmpty())
    {
        return;
    }

    qApp->setOverrideCursor(QCursor(Qt::WaitCursor));

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        qApp->restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error"), tr("Failed to save image.\n%1.") . arg(file.errorString()));
        return;
    }

    int w = mBoard->size().x / 1000 * pixelsPerCm;
    int h = mBoard->size().y / 1000 * pixelsPerCm;
    QImage image(w, h, QImage::Format_ARGB32);
    QPainter p(&image);
    if (ui->antialiasingCheckBox->isChecked())
        p.setRenderHint(QPainter::Antialiasing);
    mScene->render(&p);
    if (!image.save(&file))
    {
        qApp->restoreOverrideCursor();
        QMessageBox::critical(this, tr("Error"), tr("Failed to save image.\n%1.") . arg(file.errorString()) );
        return;
    }

    setResult(QDialog::DialogCode::Accepted);
    close();
    qApp->restoreOverrideCursor();
}

void BitmapExport::rejected()
{
    setResult(QDialog::DialogCode::Rejected);
    close();
}

void BitmapExport::getResolution(int& outUnits, int& outRes)
{
    outUnits = ui->resolutionUnitsComboBox->currentIndex();

    bool ok;
    outRes = ui->resolutionEdit->text().toInt(&ok);
    if (!ok)
    {
        outRes = ui->resolutionSlider->value();
    }
}

void BitmapExport::setResolution(int inUnits, int inRes)
{
    if (inUnits < 0 || inUnits >= ui->resolutionUnitsComboBox->count())
        inUnits = 0;
    ui->resolutionUnitsComboBox->setCurrentIndex(inUnits);

    if (inRes < ui->resolutionSlider->minimum() || inRes > ui->resolutionSlider->maximum())
        inRes = ui->resolutionSlider->minimum();
    ui->resolutionSlider->setValue(inRes);
}

