/****************************************************************************
** MainWindow meta object code from reading C++ file 'qt_main_window.h'
**
** Created: Mon Mar 21 21:07:52 2005
**      by: The Qt MOC ($Id: moc_qt_main_window.cpp,v 1.3 2005/03/22 21:44:38 jason Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../Include/qt_main_window.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.2.0b2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *MainWindow::className() const
{
    return "MainWindow";
}

QMetaObject *MainWindow::metaObj = 0;
static QMetaObjectCleanUp cleanUp_MainWindow( "MainWindow", &MainWindow::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString MainWindow::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString MainWindow::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "MainWindow", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* MainWindow::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QWidget::staticMetaObject();
    static const QUMethod slot_0 = {"Open", 0, 0 };
    static const QUMethod slot_1 = {"Save", 0, 0 };
    static const QUMethod slot_2 = {"SaveAs", 0, 0 };
    static const QUMethod slot_3 = {"Freeze", 0, 0 };
    static const QUMethod slot_4 = {"Reset", 0, 0 };
    static const QUMethod slot_5 = {"Clear", 0, 0 };
    static const QUMethod slot_6 = {"Edit", 0, 0 };
    static const QUMethod slot_7 = {"NormalView", 0, 0 };
    static const QUMethod slot_8 = {"DownView", 0, 0 };
    static const QUMethod slot_9 = {"About", 0, 0 };
    static const QUParameter param_slot_10[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod slot_10 = {"Update", 1, param_slot_10 };
    static const QUMethod slot_11 = {"UpdateSystem", 0, 0 };
    static const QUMethod slot_12 = {"ChangeGravityX", 0, 0 };
    static const QUMethod slot_13 = {"ChangeGravityY", 0, 0 };
    static const QUMethod slot_14 = {"ChangeGravityZ", 0, 0 };
    static const QUMethod slot_15 = {"ChangeDrag", 0, 0 };
    static const QUMethod slot_16 = {"ChangeTimestep", 0, 0 };
    static const QUMethod slot_17 = {"DisplayHeight", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Open()", &slot_0, QMetaData::Public },
	{ "Save()", &slot_1, QMetaData::Public },
	{ "SaveAs()", &slot_2, QMetaData::Public },
	{ "Freeze()", &slot_3, QMetaData::Public },
	{ "Reset()", &slot_4, QMetaData::Public },
	{ "Clear()", &slot_5, QMetaData::Public },
	{ "Edit()", &slot_6, QMetaData::Public },
	{ "NormalView()", &slot_7, QMetaData::Public },
	{ "DownView()", &slot_8, QMetaData::Public },
	{ "About()", &slot_9, QMetaData::Public },
	{ "Update(QString)", &slot_10, QMetaData::Public },
	{ "UpdateSystem()", &slot_11, QMetaData::Public },
	{ "ChangeGravityX()", &slot_12, QMetaData::Public },
	{ "ChangeGravityY()", &slot_13, QMetaData::Public },
	{ "ChangeGravityZ()", &slot_14, QMetaData::Public },
	{ "ChangeDrag()", &slot_15, QMetaData::Public },
	{ "ChangeTimestep()", &slot_16, QMetaData::Public },
	{ "DisplayHeight()", &slot_17, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainWindow", parentObject,
	slot_tbl, 18,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_MainWindow.setMetaObject( metaObj );
    return metaObj;
}

void* MainWindow::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "MainWindow" ) )
	return this;
    return QWidget::qt_cast( clname );
}

bool MainWindow::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Open(); break;
    case 1: Save(); break;
    case 2: SaveAs(); break;
    case 3: Freeze(); break;
    case 4: Reset(); break;
    case 5: Clear(); break;
    case 6: Edit(); break;
    case 7: NormalView(); break;
    case 8: DownView(); break;
    case 9: About(); break;
    case 10: Update((QString)static_QUType_QString.get(_o+1)); break;
    case 11: UpdateSystem(); break;
    case 12: ChangeGravityX(); break;
    case 13: ChangeGravityY(); break;
    case 14: ChangeGravityZ(); break;
    case 15: ChangeDrag(); break;
    case 16: ChangeTimestep(); break;
    case 17: DisplayHeight(); break;
    default:
	return QWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool MainWindow::qt_emit( int _id, QUObject* _o )
{
    return QWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool MainWindow::qt_property( int id, int f, QVariant* v)
{
    return QWidget::qt_property( id, f, v);
}

bool MainWindow::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
