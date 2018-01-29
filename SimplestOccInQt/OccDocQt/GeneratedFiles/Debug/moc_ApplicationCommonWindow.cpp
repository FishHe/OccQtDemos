/****************************************************************************
** Meta object code from reading C++ file 'ApplicationCommonWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../ApplicationCommonWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ApplicationCommonWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_ApplicationCommonWindow_t {
    QByteArrayData data[22];
    char stringdata0[289];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ApplicationCommonWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ApplicationCommonWindow_t qt_meta_stringdata_ApplicationCommonWindow = {
    {
QT_MOC_LITERAL(0, 0, 23), // "ApplicationCommonWindow"
QT_MOC_LITERAL(1, 24, 8), // "onNewDoc"
QT_MOC_LITERAL(2, 33, 15), // "DocumentCommon*"
QT_MOC_LITERAL(3, 49, 0), // ""
QT_MOC_LITERAL(4, 50, 13), // "onCloseWindow"
QT_MOC_LITERAL(5, 64, 8), // "onUseVBO"
QT_MOC_LITERAL(6, 73, 15), // "onCloseDocument"
QT_MOC_LITERAL(7, 89, 6), // "theDoc"
QT_MOC_LITERAL(8, 96, 18), // "onSelectionChanged"
QT_MOC_LITERAL(9, 115, 7), // "onAbout"
QT_MOC_LITERAL(10, 123, 13), // "onViewToolBar"
QT_MOC_LITERAL(11, 137, 15), // "onViewStatusBar"
QT_MOC_LITERAL(12, 153, 12), // "onToolAction"
QT_MOC_LITERAL(13, 166, 15), // "onCreateNewView"
QT_MOC_LITERAL(14, 182, 17), // "onWindowActivated"
QT_MOC_LITERAL(15, 200, 8), // "QWidget*"
QT_MOC_LITERAL(16, 209, 1), // "w"
QT_MOC_LITERAL(17, 211, 22), // "windowsMenuAboutToShow"
QT_MOC_LITERAL(18, 234, 20), // "windowsMenuActivated"
QT_MOC_LITERAL(19, 255, 7), // "checked"
QT_MOC_LITERAL(20, 263, 13), // "onSetMaterial"
QT_MOC_LITERAL(21, 277, 11) // "theMaterial"

    },
    "ApplicationCommonWindow\0onNewDoc\0"
    "DocumentCommon*\0\0onCloseWindow\0onUseVBO\0"
    "onCloseDocument\0theDoc\0onSelectionChanged\0"
    "onAbout\0onViewToolBar\0onViewStatusBar\0"
    "onToolAction\0onCreateNewView\0"
    "onWindowActivated\0QWidget*\0w\0"
    "windowsMenuAboutToShow\0windowsMenuActivated\0"
    "checked\0onSetMaterial\0theMaterial"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ApplicationCommonWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   84,    3, 0x0a /* Public */,
       4,    0,   85,    3, 0x0a /* Public */,
       5,    0,   86,    3, 0x0a /* Public */,
       6,    1,   87,    3, 0x0a /* Public */,
       8,    0,   90,    3, 0x0a /* Public */,
       9,    0,   91,    3, 0x0a /* Public */,
      10,    0,   92,    3, 0x0a /* Public */,
      11,    0,   93,    3, 0x0a /* Public */,
      12,    0,   94,    3, 0x0a /* Public */,
      13,    0,   95,    3, 0x0a /* Public */,
      14,    1,   96,    3, 0x0a /* Public */,
      17,    0,   99,    3, 0x0a /* Public */,
      18,    1,  100,    3, 0x0a /* Public */,
      20,    1,  103,    3, 0x0a /* Public */,

 // slots: parameters
    0x80000000 | 2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 2,    7,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   19,
    QMetaType::Void, QMetaType::Int,   21,

       0        // eod
};

void ApplicationCommonWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ApplicationCommonWindow *_t = static_cast<ApplicationCommonWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: { DocumentCommon* _r = _t->onNewDoc();
            if (_a[0]) *reinterpret_cast< DocumentCommon**>(_a[0]) = std::move(_r); }  break;
        case 1: _t->onCloseWindow(); break;
        case 2: _t->onUseVBO(); break;
        case 3: _t->onCloseDocument((*reinterpret_cast< DocumentCommon*(*)>(_a[1]))); break;
        case 4: _t->onSelectionChanged(); break;
        case 5: _t->onAbout(); break;
        case 6: _t->onViewToolBar(); break;
        case 7: _t->onViewStatusBar(); break;
        case 8: _t->onToolAction(); break;
        case 9: _t->onCreateNewView(); break;
        case 10: _t->onWindowActivated((*reinterpret_cast< QWidget*(*)>(_a[1]))); break;
        case 11: _t->windowsMenuAboutToShow(); break;
        case 12: _t->windowsMenuActivated((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 13: _t->onSetMaterial((*reinterpret_cast< int(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< DocumentCommon* >(); break;
            }
            break;
        case 10:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    }
}

const QMetaObject ApplicationCommonWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_ApplicationCommonWindow.data,
      qt_meta_data_ApplicationCommonWindow,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *ApplicationCommonWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ApplicationCommonWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ApplicationCommonWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int ApplicationCommonWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
