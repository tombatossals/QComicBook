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

/*! \file comicmain.h */

#ifndef __COMIC_MAIN_H
#define __COMIC_MAIN_H

#include <QMainWindow>
#include "ui_ComicMainWindow.h"

class QMenu;
class QLabel;
class QKeyEvent;
class QAction;

namespace QComicBook
{
	class ImgDirSink;
	class ComicBookSettings;
	class PageViewBase;
	class ThumbnailsWindow;
	class Bookmarks;
	class StatusBar;
        class PageLoaderThread;
        class Page;
        class ThumbnailLoaderThread;
        class RecentFilesMenu;

	//! The main window of QComicBook.
	class ComicMainWindow: public QMainWindow, private Ui::ComicMainWindow
	{
		Q_OBJECT

		private:
			ImgDirSink *sink;
			PageViewBase *view;
			ThumbnailsWindow *thumbswin;
			Bookmarks *bookmarks;
			StatusBar *statusbar;
			ComicBookSettings *cfg;
			int currpage; //!<current page number
					
			bool savedToolbarState;
			QMenu *context_menu;
			RecentFilesMenu *menuRecentFiles;
                        QAction *actionToggleThumbnails;
                        QAction *actionExitFullScreen;
			QLabel *pageinfo; //!<page info displayed in right-click context menu
			QString lastdir; //!<last opened directory for Open File/Directory dialog
                        PageLoaderThread *pageLoader;
                        ThumbnailLoaderThread *thumbnailLoader;
                        		
		protected:
			virtual void dragEnterEvent(QDragEnterEvent *e);
			virtual void dropEvent(QDropEvent *e);
			virtual void keyPressEvent(QKeyEvent *e);
			virtual void closeEvent(QCloseEvent *e);

			bool confirmExit();
			void enableComicBookActions(bool f=true);
			void saveSettings();

                        void setupContextMenu();
			void setupComicImageView();

		protected slots:
                        void pageLoaded(const Page &page);
                        void pageLoaded(const Page &page1, const Page &page2);
			void sinkReady(const QString &path);
			void sinkError(int code);
			void updateCaption();
			void recentSelected(const QString &fname);
			void bookmarkSelected(QAction *action);
			void thumbnailsWindowShown();
			void savePageAs();
                        void reconfigureDisplay();
                        void currentPageChanged(int n);
                        void setPageSize(QAction *action);

		public slots:
			void firstPage();
			void lastPage();
			void nextPage();
			void prevPage();
			void prevPageBottom();
			void forwardPages();
			void backwardPages();
			void jumpToPage(int n, bool force=false);
			void exitFullscreen();
			void browseDirectory();
			void browseArchive();
			void open(const QString &path, int page=0);
			void openNext();
			void openPrevious();
			void showAbout();
			void showInfo();
			void showSysInfo();
			void showConfigDialog();
			void showJumpToPage(const QString &number=QString::null);
			void closeSink();
			void setBookmark();
			void removeBookmark();
			void openBookmarksManager();
			void toggleScrollbars(bool f);
			void toggleTwoPages(bool f);
			void toggleFullScreen();
			void toggleContinousScroll(bool f);
			void toggleJapaneseMode(bool f);
			void reloadPage();

		public:
			ComicMainWindow(QWidget *parent);
			virtual ~ComicMainWindow();
	};
}

#endif
