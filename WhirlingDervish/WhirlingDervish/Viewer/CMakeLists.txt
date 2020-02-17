PROJECT(Viewer)

INCLUDE_DIRECTORIES(
	./Include
	./Interface
	./UI
	../Tools/Interface
	../Physics/Interface
	../Genome/Interface
	../Creature/Interface
	../Math/Interface
	../System/Interface
	../Shapes/Interface
	../Graphics/Interface
	../Environment/Interface
	/usr/local/qt/include
)

SET(SOURCES Src/main.cpp)

IF(USE_QT)
	SET(QT_SOURCES
		Src/qt_editor.cpp
		Src/qt_ijgl.cpp
		Src/qt_key_binding.cpp
		Src/qt_main_window.cpp
	)

	SET(MOC_FILES
		Src/moc_qt_editor.cpp
		Src/moc_qt_ijgl.cpp
		Src/moc_qt_key_binding.cpp
		Src/moc_qt_main_window.cpp
		UI/moc_bindingform.cpp
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_editor.cpp
		moc Include/qt_editor.h -o Src/moc_qt_editor.cpp
		DEPENDS Include/qt_editor.h
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_ijgl.cpp
		moc Include/qt_ijgl.h -o Src/moc_qt_ijgl.cpp
		DEPENDS Include/qt_ijgl.h
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_key_binding.cpp
		moc Include/qt_key_binding.h -o Src/moc_qt_key_binding.cpp
		DEPENDS Include/qt_key_binding.h
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_main_window.cpp
		moc Include/qt_main_window.h -o Src/moc_qt_main_window.cpp
		DEPENDS Include/qt_main_window.h
	)

	ADD_CUSTOM_TARGET(UI/bindingform.cpp
		uic -o UI/bindingform.cpp -impl bindingform.h UI/bindingform.ui
		DEPENDS UI/bindingform.ui
	)

	ADD_CUSTOM_TARGET(UI/bindingform.h
		uic UI/bindingform.ui -o UI/bindingform.h
		DEPENDS UI/bindingform.ui
	)

	ADD_CUSTOM_TARGET(UI/moc_bindingform.cpp
		moc -o UI/moc_bindingform.cpp UI/bindingform.h
		DEPENDS UI/bindingform.ui
	)

	SET(SOURCES ${SOURCES} ${QT_SOURCES} ${MOC_FILES} UI/bindingform.cpp)
ENDIF(USE_QT)

IF(USE_FLTK)
	SET(FLTK_SOURCES
		Src/qt_editor.cpp
		Src/qt_ijgl.cpp
		Src/qt_key_binding.cpp
		Src/qt_main_window.cpp
	)

	SET(FLTK_MOC_FILES
		Src/moc_qt_editor.cpp
		Src/moc_qt_ijgl.cpp
		Src/moc_qt_key_binding.cpp
		Src/moc_qt_main_window.cpp
		UI/moc_bindingform.cpp
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_editor.cpp
		moc Include/qt_editor.h -o Src/moc_qt_editor.cpp
		DEPENDS Include/qt_editor.h
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_ijgl.cpp
		moc Include/qt_ijgl.h -o Src/moc_qt_ijgl.cpp
		DEPENDS Include/qt_ijgl.h
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_key_binding.cpp
		moc Include/qt_key_binding.h -o Src/moc_qt_key_binding.cpp
		DEPENDS Include/qt_key_binding.h
	)

	ADD_CUSTOM_TARGET(Src/moc_qt_main_window.cpp
		moc Include/qt_main_window.h -o Src/moc_qt_main_window.cpp
		DEPENDS Include/qt_main_window.h
	)

	ADD_CUSTOM_TARGET(UI/bindingform.cpp
		uic -o UI/bindingform.cpp -impl bindingform.h UI/bindingform.ui
		DEPENDS UI/bindingform.ui
	)

	ADD_CUSTOM_TARGET(UI/bindingform.h
		uic UI/bindingform.ui -o UI/bindingform.h
		DEPENDS UI/bindingform.ui
	)

	ADD_CUSTOM_TARGET(UI/moc_bindingform.cpp
		moc -o UI/moc_bindingform.cpp UI/bindingform.h
		DEPENDS UI/bindingform.ui
	)

	SET(SOURCES ${SOURCES} ${FLTK_SOURCES} ${FLTK_MOC_FILES}
		    UI/bindingform.cpp)
ENDIF(USE_FLTK)

ADD_LIBRARY(viewer ${SOURCES})

IF(USE_QT)
	ADD_DEPENDENCIES(viewer UI/bindingform.h UI/bindingform.cpp
			 ${MOC_FILES})
ENDIF(USE_QT)

SET(LIBRARY_OUTPUT_PATH ./Lib CACHE INTERNAL "library")
