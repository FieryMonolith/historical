/****************************************************************************
** KeyBinding meta object code from reading C++ file 'qt_key_binding.h'
**
** Created: Fri Apr 8 11:17:52 2005
**      by: The Qt MOC ($Id: moc_qt_key_binding.cpp,v 1.3 2005/04/08 15:50:58 covertar Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../Include/qt_key_binding.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KeyBinding::className() const
{
    return "KeyBinding";
}

QMetaObject *KeyBinding::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KeyBinding( "KeyBinding", &KeyBinding::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KeyBinding::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KeyBinding", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KeyBinding::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KeyBinding", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KeyBinding::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = BindingForm::staticMetaObject();
    static const QUMethod slot_0 = {"SaveBindings", 0, 0 };
    static const QUMethod slot_1 = {"LoadBindings", 0, 0 };
    static const QUMethod slot_2 = {"Change", 0, 0 };
    static const QUMethod slot_3 = {"Apply", 0, 0 };
    static const QUMethod slot_4 = {"Cancel", 0, 0 };
    static const QUMethod slot_5 = {"Done", 0, 0 };
    static const QUParameter param_slot_6[] = {
	{ "which", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_6 = {"SystemGroup", 1, param_slot_6 };
    static const QUParameter param_slot_7[] = {
	{ "which", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_7 = {"MouseGroup", 1, param_slot_7 };
    static const QUParameter param_slot_8[] = {
	{ "which", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_8 = {"EyepointGroup", 1, param_slot_8 };
    static const QMetaData slot_tbl[] = {
	{ "SaveBindings()", &slot_0, QMetaData::Public },
	{ "LoadBindings()", &slot_1, QMetaData::Public },
	{ "Change()", &slot_2, QMetaData::Public },
	{ "Apply()", &slot_3, QMetaData::Public },
	{ "Cancel()", &slot_4, QMetaData::Public },
	{ "Done()", &slot_5, QMetaData::Public },
	{ "SystemGroup(int)", &slot_6, QMetaData::Public },
	{ "MouseGroup(int)", &slot_7, QMetaData::Public },
	{ "EyepointGroup(int)", &slot_8, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"KeyBinding", parentObject,
	slot_tbl, 9,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KeyBinding.setMetaObject( metaObj );
    return metaObj;
}

void* KeyBinding::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KeyBinding" ) )
	return this;
    return BindingForm::qt_cast( clname );
}

bool KeyBinding::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: SaveBindings(); break;
    case 1: LoadBindings(); break;
    case 2: Change(); break;
    case 3: Apply(); break;
    case 4: Cancel(); break;
    case 5: Done(); break;
    case 6: SystemGroup((int)static_QUType_int.get(_o+1)); break;
    case 7: MouseGroup((int)static_QUType_int.get(_o+1)); break;
    case 8: EyepointGroup((int)static_QUType_int.get(_o+1)); break;
    default:
	return BindingForm::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KeyBinding::qt_emit( int _id, QUObject* _o )
{
    return BindingForm::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KeyBinding::qt_property( int id, int f, QVariant* v)
{
    return BindingForm::qt_property( id, f, v);
}

bool KeyBinding::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
