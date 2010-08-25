/*
 * This file is a part of QComicBook.
 *
 * Copyright (C) 2005-2009 Pawel Stolowski <pawel.stolowski@wp.pl>
 *
 * QComicBook is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#include "ComicBookCfgDialog.h"
#include "ComicBookSettings.h"
#include "PageViewBase.h"
#include <QColorDialog>
#include <QFontDialog>
#include <QFileDialog>
#include <QDebug>

using namespace QComicBook;

ComicBookCfgDialog::ComicBookCfgDialog(QWidget *parent, ComicBookSettings *cfg): QDialog(parent), cfg(cfg)
{
    setupUi(this);

    //
    // setup buttons
    connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));

    font = cfg->infoFont();
    connect(pb_font, SIGNAL(clicked()), this, SLOT(showFontDialog()));
    updateFontPreview();

    //
    // background color
    QPalette palette;
    palette.setColor(pb_color->backgroundRole(), bgcolor = cfg->background());
    pb_color->setPalette(palette);
    
    connect(pb_color, SIGNAL(clicked()), this, SLOT(showBackgroundDialog()));

    cb_startfullscreen->setChecked(cfg->fullScreenStart());
    cb_fullscreenrotation->setCurrentIndex(cfg->fullScreenRotation());
    cb_smallcursor->setChecked(cfg->smallCursor());
    cb_embedpagenumbers->setChecked(cfg->embedPageNumbers());
    cb_smoothscaling->setChecked(cfg->smoothScaling());

    sb_cachesize->setValue(cfg->cacheSize());
    cb_cacheadjust->setChecked(cfg->cacheAutoAdjust());
    cb_preload->setChecked(cfg->preloadPages());

    cb_autoinfo->setChecked(cfg->autoInfo());
    cb_thumbs->setChecked(cfg->cacheThumbnails());
    sb_thumbsage->setValue(cfg->thumbnailsAge());
    cb_splash->setChecked(cfg->showSplash());
    cb_confirmexit->setChecked(cfg->confirmExit());
    le_tempdir->setText(cfg->tmpDir());
    connect(b_browsetemp, SIGNAL(clicked()), this, SLOT(showTempDirDialog()));
}

ComicBookCfgDialog::~ComicBookCfgDialog()
{
}

void ComicBookCfgDialog::accept()
{
	//
	// display
	cfg->background(bgcolor);
	cfg->fullScreenStart(cb_startfullscreen->isChecked());
	cfg->fullScreenRotation(cb_fullscreenrotation->currentIndex());
	cfg->smallCursor(cb_smallcursor->isChecked());
        cfg->embedPageNumbers(cb_embedpagenumbers->isChecked());
	cfg->smoothScaling(cb_smoothscaling->isChecked());
	cfg->infoFont(font);

	//
	// misc
	cfg->cacheSize(sb_cachesize->value());
	cfg->cacheAutoAdjust(cb_cacheadjust->isChecked());
	cfg->preloadPages(cb_preload->isChecked());
	cfg->cacheThumbnails(cb_thumbs->isChecked());
	cfg->thumbnailsAge(sb_thumbsage->value());
	cfg->autoInfo(cb_autoinfo->isChecked());
	cfg->showSplash(cb_splash->isChecked());
	cfg->confirmExit(cb_confirmexit->isChecked());
        cfg->tmpDir(le_tempdir->text());

        emit displaySettingsChanged(); //FIXME only if needed
	QDialog::accept();
}

void ComicBookCfgDialog::updateFontPreview()
{
	fontname->setText(font.family() + ", " + QString::number(font.pointSize()));
	fontname->setFont(font);
}

void ComicBookCfgDialog::showBackgroundDialog()
{
	QColor c = QColorDialog::getColor(cfg->background(), this);
	if (c.isValid())
	{
		QPalette palette;
		palette.setColor(pb_color->backgroundRole(), bgcolor = c);
		pb_color->setPalette(palette);
	}
}

void ComicBookCfgDialog::showFontDialog()
{
	bool ok;
	font = QFontDialog::getFont(&ok, font, this);
	if (ok)
		updateFontPreview();
}

void ComicBookCfgDialog::showTempDirDialog()
{
    QString dir(QFileDialog::getExistingDirectory(this, tr("Choose temporary directory"), le_tempdir->text()));
    if (!(dir.isNull() || dir.isEmpty()))
    {
        le_tempdir->setText(dir);
    }
}
