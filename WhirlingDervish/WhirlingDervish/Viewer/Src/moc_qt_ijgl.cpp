/****************************************************************************
** iJGL meta object code from reading C++ file 'qt_ijgl.h'
**
** Created: Fri Apr 8 11:11:07 2005
**      by: The Qt MOC ($Id: moc_qt_ijgl.cpp,v 1.3 2005/04/08 15:50:58 covertar Exp $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "../Include/qt_ijgl.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.1.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *iJGL::className() const
{
    return "iJGL";
}

QMetaObject *iJGL::metaObj = 0;
static QMetaObjectCleanUp cleanUp_iJGL( "iJGL", &iJGL::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString iJGL::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "iJGL", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString iJGL::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "iJGL", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* iJGL::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QGLWidget::staticMetaObject();
    static const QUMethod slot_0 = {"IncreaseRotationSpeed", 0, 0 };
    static const QUMethod slot_1 = {"DecreaseRotationSpeed", 0, 0 };
    static const QUMethod slot_2 = {"IncreaseForwardSpeed", 0, 0 };
    static const QUMethod slot_3 = {"DecreaseForwardSpeed", 0, 0 };
    static const QUMethod slot_4 = {"MoveForward", 0, 0 };
    static const QUMethod slot_5 = {"MoveBackward", 0, 0 };
    static const QUMethod slot_6 = {"MoveLeft", 0, 0 };
    static const QUMethod slot_7 = {"MoveRight", 0, 0 };
    static const QUMethod slot_8 = {"MoveUp", 0, 0 };
    static const QUMethod slot_9 = {"MoveDown", 0, 0 };
    static const QUMethod slot_10 = {"YawLeft", 0, 0 };
    static const QUMethod slot_11 = {"YawRight", 0, 0 };
    static const QUMethod slot_12 = {"PitchUp", 0, 0 };
    static const QUMethod slot_13 = {"PitchDown", 0, 0 };
    static const QUMethod slot_14 = {"ToggleGrid", 0, 0 };
    static const QUMethod slot_15 = {"ToggleView", 0, 0 };
    static const QUMethod slot_16 = {"NormalView", 0, 0 };
    static const QUMethod slot_17 = {"DownView", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "IncreaseRotationSpeed()", &slot_0, QMetaData::Public },
	{ "DecreaseRotationSpeed()", &slot_1, QMetaData::Public },
	{ "IncreaseForwardSpeed()", &slot_2, QMetaData::Public },
	{ "DecreaseForwardSpeed()", &slot_3, QMetaData::Public },
	{ "MoveForward()", &slot_4, QMetaData::Public },
	{ "MoveBackward()", &slot_5, QMetaData::Public },
	{ "MoveLeft()", &slot_6, QMetaData::Public },
	{ "MoveRight()", &slot_7, QMetaData::Public },
	{ "MoveUp()", &slot_8, QMetaData::Public },
	{ "MoveDown()", &slot_9, QMetaData::Public },
	{ "YawLeft()", &slot_10, QMetaData::Public },
	{ "YawRight()", &slot_11, QMetaData::Public },
	{ "PitchUp()", &slot_12, QMetaData::Public },
	{ "PitchDown()", &slot_13, QMetaData::Public },
	{ "ToggleGrid()", &slot_14, QMetaData::Public },
	{ "ToggleView()", &slot_15, QMetaData::Public },
	{ "NormalView()", &slot_16, QMetaData::Public },
	{ "DownView()", &slot_17, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"iJGL", parentObject,
	slot_tbl, 18,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_iJGL.setMetaObject( metaObj );
    return metaObj;
}

void* iJGL::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "iJGL" ) )
	return this;
    return QGLWidget::qt_cast( clname );
}

bool iJGL::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: IncreaseRotationSpeed(); break;
    case 1: DecreaseRotationSpeed(); break;
    case 2: IncreaseForwardSpeed(); break;
    case 3: DecreaseForwardSpeed(); break;
    case 4: MoveForward(); break;
    case 5: MoveBackward(); break;
    case 6: MoveLeft(); break;
    case 7: MoveRight(); break;
    case 8: MoveUp(); break;
    case 9: MoveDown(); break;
    case 10: YawLeft(); break;
    case 11: YawRight(); break;
    case 12: PitchUp(); break;
    case 13: PitchDown(); break;
    case 14: ToggleGrid(); break;
    case 15: ToggleView(); break;
    case 16: NormalView(); break;
    case 17: DownView(); break;
    default:
	return QGLWidget::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool iJGL::qt_emit( int _id, QUObject* _o )
{
    return QGLWidget::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool iJGL::qt_property( int id, int f, QVariant* v)
{
    return QGLWidget::qt_property( id, f, v);
}

bool iJGL::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
