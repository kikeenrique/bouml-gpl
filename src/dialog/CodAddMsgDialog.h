// *************************************************************************
//
// Copyright 2004-2010 Bruno PAGES  .
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

#ifndef CODADDMSGDIALOG_H
#define CODADDMSGDIALOG_H

#include <qdialog.h>
#include <qstringlist.h>

class QComboBox;
class CodObjCanvas;
class CodMsgSupport;
class ColDiagramView;
class OperationData;
class BrowserClass;

class CodAddMsgDialog : public QDialog
{
        Q_OBJECT

    protected:
        CodMsgSupport * in;
        ColDiagramView * view;
        bool forward;
        BrowserClass * cl;
        QComboBox * cbrank;
        QStringList list;
        QValueList<const OperationData *> opers;
        QComboBox * edoper;

        static QSize previous_size;

    public:
        CodAddMsgDialog (CodObjCanvas * fr, CodObjCanvas * t, CodMsgSupport * i,
                         ColDiagramView * v, bool f);
        virtual ~CodAddMsgDialog();

    protected slots:
        void menu_op();
        virtual void polish();
        void accept();
};

#endif
