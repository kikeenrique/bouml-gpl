// *************************************************************************
//
// Copyleft 2004-2008 Bruno PAGES  .
//
// This file is part of the BOUML Uml Toolkit.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
//
// e-mail : bouml@free.fr
// home   : http://bouml.free.fr
//
// *************************************************************************

#ifndef UMLWINDOW_H
#define UMLWINDOW_H

#include <qmainwindow.h>
#include <qstringlist.h>

class QCanvasView;
class QWorkspace;
class QToolBar;
class QPopupMenu;
class QSplitter;
class QToolButton;

class BrowserView;
class BrowserNode;
class MultiLineEdit;

#include "UmlEnum.h"

// singleton

class UmlWindow : public QMainWindow {
  Q_OBJECT
    
  protected:
    static UmlWindow * the;
  
    QWorkspace * ws;
    BrowserView * browser;
    MultiLineEdit * comment;
    BrowserNode * commented;	// the commented object
    QToolBar * projectTools;
    QPopupMenu * projectMenu;
    QPopupMenu * windowsMenu;
    QPopupMenu * toolMenu;
    QPopupMenu * langMenu;
    QPopupMenu * miscMenu;
    QPopupMenu * fontSizeMenu;
    QPopupMenu * formatMenu;
    int use_cpp_id;
    int use_java_id;
    int use_php_id;
    int use_python_id;
    int use_idl_id;
    int verbose_gen_id;
    int preserve_bodies_id;
    int add_operation_profile_id;
    int shortcut_id;
    int show_browser_stereotypes_id;
    QStringList historic;
    CanvasFormat format;
    QSplitter * spl1;
    QSplitter * spl2;
    char style;	// '?' unknown, 'm' = motif, '+' = motif+, 'w' = windows
    QToolButton * prev;
    QToolButton * next;
    QValueList<BrowserNode *> select_historic;

  public:
    UmlWindow();
    ~UmlWindow();
  
    void load(QString fn, bool forcesaveas = FALSE);
    bool can_close();
    
    static void set_commented(BrowserNode * bn);
    static void update_comment_if_needed(BrowserNode * bn);
    static void set_message(const QString &);
    static QWorkspace * get_workspace();
    static void clear();
    static void historic_add(QString fn);
    static void save_it();
    static bool saveas_it();
    static void close_it();
    static void load_it(QString fn);
    static CanvasFormat default_format();
    static void set_default_format(CanvasFormat);
    static void abort_line_construction();
    static void clear_select_historic();
    
  protected:
    void is_selected(BrowserNode *);
    virtual void closeEvent(QCloseEvent *);
    void save_session();
    void read_session();
  
  private slots:
    void newProject();
    void newFromTemplate();
    void load();
    void save();
    void saveAs();
    void print();
    void close();
    void quit();
    void browser_search();
    void next_select();
    void prev_select();
    
    void edit_gen_settings();
    void edit_stereotypes();
    void edit_class_settings();
    void edit_drawing_settings();
    
    void use_cpp();
    void use_java();
    void use_php();
    void use_python();
    void use_idl();
    void verbose();
    void preserve();
    void addoperationprofile();
    void edit_shortcuts();
    
    void motif_style();
    void motifplus_style();
    //void sgi_style();
    //void cde_style();
    void windows_style();
    
    void show_stereotypes();

    void about();
    void aboutQt();
    
    void show_trace();
    void cpp_generate();
    void java_generate();
    void php_generate();
    void python_generate();
    void idl_generate();
    void java_catalog();
    //void doc_generate();
    void cpp_reverse();
    void java_reverse();
    void php_reverse();
    void python_reverse();
    void cpp_roundtrip();
    void java_roundtrip();
    void php_roundtrip();
    void python_roundtrip();
    void run_tool(int param);
    void tool_settings();
    void import_tool_settings();
    void plug_out_upgrade();
    
    void comment_changed();
    void preferred_geometry();

    void toolMenuAboutToShow();
    void projectMenuAboutToShow();
    void historicActivated(int id);
    
    void langMenuAboutToShow();
    
    void miscMenuAboutToShow();
    
    void fontSizeMenuAboutToShow();
    void setFontSize(int);
    
    void formatMenuAboutToShow();
    void setFormat(int);
    
    void windowsMenuAboutToShow();
    void windowsMenuActivated(int id);

    virtual void keyPressEvent(QKeyEvent * e);

};

#endif
