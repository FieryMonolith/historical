/****************************************************************************
** BindingForm meta object code from reading C++ file 'bindingform.h'
**
** Created: Mon Oct 11 23:36:21 2004
**      by: The Qt MOC ($Id: moc_bindingform.cpp,v 1.1.1.1 2004/10/14 22:49:33 jason Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "bindingform.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *BindingForm::className() const
{
    return "BindingForm";
}

QMetaObject *BindingForm::metaObj = 0;
static QMetaObjectCleanUp cleanUp_BindingForm( "BindingForm", &BindingForm::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString BindingForm::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BindingForm", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString BindingForm::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "BindingForm", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* BindingForm::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QDialog::staticMetaObject();
    static const QUMethod slot_0 = {"languageChange", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "languageChange()", &slot_0, QMetaData::Protected }
    };
    metaObj = QMetaObject::new_metaobject(
	"BindingForm", parentObject,
	slot_tbl, 1,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_BindingForm.setMetaObject( metaObj );
    return metaObj;
}

void* BindingForm::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "BindingForm" ) )
	return this;
    return QDialog::qt_cast( clname );
}

bool BindingForm::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: languageChange(); break;
    default:
	return QDialog::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool BindingForm::qt_emit( int _id, QUObject* _o )
{
    return QDialog::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool BindingForm::qt_property( int id, int f, QVariant* v)
{
    return QDialog::qt_property( id, f, v);
}

bool BindingForm::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
