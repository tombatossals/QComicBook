/*
 * This file is a part of QComicBook.
 *
 * Copyright (C) 2005-2006 Pawel Stolowski <pawel.stolowski@wp.pl>
 *
 * QComicBook is free software; you can redestribute it and/or modify it
 * under terms of GNU General Public License by Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY. See GPL for more details.
 */

#include "ComicBookCfgDialog.h"
#include "ComicBookSettings.h"
#include "ComicImageView.h"
#include <QTabWidget>
#include <QDialogButtonBox>
#include <QLayout>
#include <QButtonGroup>
#include <QRadioButton>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QColorDialog>
#include <QGroupBox>
#include <QSpinBox>
#include <QFontDialog>
#include <QFileDialog>
#include <QLineEdit>

using namespace QComicBook;

ComicBookCfgDialog::ComicBookCfgDialog(QWidget *parent, ComicBookSettings *cfg): QDialog(parent), cfg(cfg)
{
	setWindowTitle("QComicBook Settings");
	setModal(true);

	QVBoxLayout *lay = new QVBoxLayout(this);
	tabs = new QTabWidget(this);
	lay->addWidget(tabs);

	setupDisplayTab();
	setupMiscTab();

	//
	// setup buttons
	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, this);
	lay->addWidget(buttons);
	connect(buttons, SIGNAL(accepted()), this, SLOT(accept()));
	connect(buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

ComicBookCfgDialog::~ComicBookCfgDialog()
{
}

void ComicBookCfgDialog::setupDisplayTab()
{
	font = cfg->infoFont();

	QWidget *w = new QWidget(this);
	QVBoxLayout *lay = new QVBoxLayout(w);

	//
	// background color
	QHBoxLayout *lay1 = new QHBoxLayout;
	lay1->addWidget(new QLabel(tr("Background color"), w));
	lay1->addWidget(pb_color = new QPushButton(w));
	pb_color->setFixedWidth(32);

	QPalette palette;
	palette.setColor(pb_color->backgroundRole(), bgcolor = cfg->background());
	pb_color->setPalette(palette);
	
	connect(pb_color, SIGNAL(clicked()), this, SLOT(showBackgroundDialog()));
	lay->addLayout(lay1);

	cb_hidemenu = new QCheckBox(tr("Hide menubar in fullscreen mode"), w);
	cb_hidemenu->setChecked(cfg->fullScreenHideMenu());
	lay->addWidget(cb_hidemenu);
	
	cb_hidetoolbar = new QCheckBox(tr("Hide toolbar in fullscreen mode"), w);
	cb_hidetoolbar->setChecked(cfg->fullScreenHideToolbar());
	lay->addWidget(cb_hidetoolbar);

	cb_hidestatus = new QCheckBox(tr("Hide statusbar in fullscreen mode"), w);
	cb_hidestatus->setChecked(cfg->fullScreenHideStatusbar());
	lay->addWidget(cb_hidestatus);

	cb_smallcursor = new QCheckBox(tr("Small mouse cursor"), w);
	cb_smallcursor->setChecked(cfg->smallCursor());
	lay->addWidget(cb_smallcursor);

        cb_embedpagenumbers = new QCheckBox(tr("Embed page numbers"), w);
        cb_embedpagenumbers->setChecked(cfg->embedPageNumbers());
        lay->addWidget(cb_embedpagenumbers);
		
	//
	// scaling method
	/*QButtonGroup *gr_scaling = new QButtonGroup(2, Qt::Horizontal, tr("Scaling method"), w);
	rb_smooth = new QRadioButton(tr("Smooth"), gr_scaling);
	rb_fast = new QRadioButton(tr("Fast"), gr_scaling);
	if (cfg->pageScaling() == Fast)
		rb_fast->setChecked(true);
	else
		rb_smooth->setChecked(true);
	lay->addWidget(gr_scaling);*/

	//
	// font
	QHBoxLayout *lay3 = new QHBoxLayout;
	lay3->addWidget(new QLabel(tr("Info text font"), w));
	fontname = new QLabel(w);
	fontname->setFrameStyle(QFrame::Panel | QFrame::Sunken);
	fontname->setLineWidth(1);
	lay3->addWidget(fontname, 1);
	QPushButton *pb_font = new QPushButton(tr("Choose"), w);
	connect(pb_font, SIGNAL(clicked()), this, SLOT(showFontDialog()));
	lay3->addWidget(pb_font);
	lay->addLayout(lay3);
	
	lay->addStretch();
	updateFontPreview();
	
	tabs->addTab(w, tr("Display")); 
}

void ComicBookCfgDialog::setupMiscTab()
{
	bool f;

	QWidget *w = new QWidget(this);
	QVBoxLayout *lay = new QVBoxLayout(w);

	QGroupBox *grp0 = new QGroupBox(tr("Cache"), w);
	QVBoxLayout *box1 = new QVBoxLayout;
	grp0->setLayout(box1);
	QHBoxLayout *box11 = new QHBoxLayout;
	box11->addWidget(new QLabel(tr("Cache size"), grp0));
	sb_cachesize = new QSpinBox(grp0);
	sb_cachesize->setRange(1, 128);
	sb_cachesize->setSuffix(" " + tr("Mb"));
	sb_cachesize->setValue(cfg->cacheSize());
	box11->addWidget(sb_cachesize);
	box1->addLayout(box11);
	cb_cacheadjust = new QCheckBox(tr("Auto adjust cache size"), grp0);
	cb_cacheadjust->setChecked(cfg->cacheAutoAdjust());
	box1->addWidget(cb_cacheadjust);
	cb_preload = new QCheckBox(tr("Preload next page"), grp0);
	cb_preload->setChecked(cfg->preloadPages());
	box1->addWidget(cb_preload);
	lay->addWidget(grp0);

	QGroupBox *grp1 = new QGroupBox(tr("Thumbnails"), w);
	QVBoxLayout *box2 = new QVBoxLayout;
	grp1->setLayout(box2);
	cb_thumbs = new QCheckBox(tr("Use disk cache for thumbnails"), grp1);
	cb_thumbs->setChecked(cfg->cacheThumbnails());
	box2->addWidget(cb_thumbs);
	QHBoxLayout *box21 = new QHBoxLayout;
	box21->addWidget(new QLabel(tr("Number of days to keep thumbnails"), grp1));
	sb_thumbsage = new QSpinBox(grp1);
	sb_thumbsage->setRange(0, 365);
	sb_thumbsage->setValue(cfg->thumbnailsAge());
	box21->addWidget(sb_thumbsage);
	box2->addLayout(box21);
	lay->addWidget(grp1);

	cb_twopagesstep = new QCheckBox(tr("Forward and backward two pages in two-pages mode"), w);
	cb_twopagesstep->setChecked(cfg->twoPagesStep());
	lay->addWidget(cb_twopagesstep);
	
	cb_autoinfo = new QCheckBox(tr("Open info dialog after opening comic book"), w);
	cb_autoinfo->setChecked(cfg->autoInfo());
	lay->addWidget(cb_autoinfo);

	cb_splash = new QCheckBox(tr("Show splashscreen"), w);
	cb_splash->setChecked(cfg->showSplash());
	lay->addWidget(cb_splash);
	
	
	/* TODO
	cb_autobookmark = new QCheckBox(tr("Save bookmark for current comicbook on close/exit"), w);
	cb_autobookmark->setChecked(cfg->confirmExit());
	lay->addWidget(cb_autobookmark);*/

	cb_confirmexit = new QCheckBox(tr("Confirm exit"), w);
	cb_confirmexit->setChecked(cfg->confirmExit());
	lay->addWidget(cb_confirmexit);

	//lay->addStretch();
	tabs->addTab(w, tr("Misc"));
}

void ComicBookCfgDialog::accept()
{
	//
	// display
	cfg->background(bgcolor);
	cfg->fullScreenHideMenu(cb_hidemenu->isChecked());
	cfg->fullScreenHideStatusbar(cb_hidestatus->isChecked());
	cfg->fullScreenHideToolbar(cb_hidetoolbar->isChecked());
	cfg->smallCursor(cb_smallcursor->isChecked());
        cfg->embedPageNumbers(cb_embedpagenumbers->isChecked());
	/*if (rb_smooth->isChecked())
		cfg->pageScaling(Smooth);
	else
		if (rb_fast->isChecked())
			cfg->pageScaling(Fast);*/
	cfg->infoFont(font);

	//
	// misc
	cfg->cacheSize(sb_cachesize->value());
	cfg->cacheAutoAdjust(cb_cacheadjust->isChecked());
	cfg->preloadPages(cb_preload->isChecked());
	cfg->cacheThumbnails(cb_thumbs->isChecked());
	cfg->thumbnailsAge(sb_thumbsage->value());
	cfg->twoPagesStep(cb_twopagesstep->isChecked());
	cfg->autoInfo(cb_autoinfo->isChecked());
	cfg->showSplash(cb_splash->isChecked());
	cfg->confirmExit(cb_confirmexit->isChecked());
	
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