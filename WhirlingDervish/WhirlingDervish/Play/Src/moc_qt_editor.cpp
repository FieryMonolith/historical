/****************************************************************************
** Editor meta object code from reading C++ file 'qt_editor.h'
**
** Created: Tue Oct 12 09:36:19 2004
**      by: The Qt MOC ($Id: moc_qt_editor.cpp,v 1.1.1.1 2004/10/14 22:49:33 jason Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../Include/qt_editor.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *Editor::className() const
{
    return "Editor";
}

QMetaObject *Editor::metaObj = 0;
static QMetaObjectCleanUp cleanUp_Editor( "Editor", &Editor::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString Editor::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Editor", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString Editor::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "Editor", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* Editor::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"Alter", 0, 0 };
    static const QUMethod slot_1 = {"Close", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "Alter()", &slot_0, QMetaData::Private },
	{ "Close()", &slot_1, QMetaData::Private }
    };
    static const QUParameter param_signal_0[] = {
	{ 0, &static_QUType_QString, 0, QUParameter::In }
    };
    static const QUMethod signal_0 = {"Text", 1, param_signal_0 };
    static const QMetaData signal_tbl[] = {
	{ "Text(QString)", &signal_0, QMetaData::Private }
    };
    metaObj = QMetaObject::new_metaobject(
	"Editor", parentObject,
	slot_tbl, 2,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_Editor.setMetaObject( metaObj );
    return metaObj;
}

void* Editor::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "Editor" ) )
	return this;
    return QDialog::qt_cast( clname );
}

// SIGNAL Text
void Editor::Text( QString t0 )
{
    activate_signal( staticMetaObject()->signalOffset() + 0, t0 );
}

bool Editor::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: Alter(); break;
    case 1: Close(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool Editor::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: Text((QString)static_QUType_QString.get(_o+1)); break;
    default:
	return QDialog::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool Editor::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool Editor::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
