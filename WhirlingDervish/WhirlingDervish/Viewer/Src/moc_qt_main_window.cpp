/****************************************************************************
** MainWindow meta object code from reading C++ file 'qt_main_window.h'
**
** Created: Fri Apr 8 11:10:55 2005
**      by: The Qt MOC ($Id: moc_qt_main_window.cpp,v 1.3 2005/04/08 15:50:58 covertar Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../Include/qt_main_window.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.1.1. It"
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
    static const QUMethod slot_0 = {"Pause", 0, 0 };
    static const QUMethod slot_1 = {"ViewGenome", 0, 0 };
    static const QUMethod slot_2 = {"NormalView", 0, 0 };
    static const QUMethod slot_3 = {"DownView", 0, 0 };
    static const QUMethod slot_4 = {"About", 0, 0 };
    static const QUMethod slot_5 = {"UpdateSystem", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Pause()", &slot_0, QMetaData::Public },
	{ "ViewGenome()", &slot_1, QMetaData::Public },
	{ "NormalView()", &slot_2, QMetaData::Public },
	{ "DownView()", &slot_3, QMetaData::Public },
	{ "About()", &slot_4, QMetaData::Public },
	{ "UpdateSystem()", &slot_5, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"MainWindow", parentObject,
	slot_tbl, 6,
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
    case 0: Pause(); break;
    case 1: ViewGenome(); break;
    case 2: NormalView(); break;
    case 3: DownView(); break;
    case 4: About(); break;
    case 5: UpdateSystem(); break;
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
