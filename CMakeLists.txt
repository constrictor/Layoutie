cmake_minimum_required(VERSION 2.6)
project(layoutie)

find_package(Sprint REQUIRED)

set (QT_MIN_VERSION "4.7.0")
find_package(Qt4 REQUIRED)

set(LAYOUTIE_SOURCES
	main.cpp
	BoardGrid.cpp
	BoardScene.cpp
	BoardView.cpp
	ComponentFactory.cpp
	MainWindow.cpp
	ProjectView.cpp
	Settings.cpp

	Components/ComponentItem.cpp
	Components/CircleItem.cpp
	Components/SMDItem.cpp
	Components/TextItem.cpp
	Components/TrackItem.cpp
	Components/ZoneItem.cpp
)

set(LAYOUTIE_HEADERS
#	BoardGrid.h
	BoardScene.h
	BoardView.h
#	ComponentFactory.h
	MainWindow.h
	ProjectView.h
#	Settings.h

#	Components/ComponentItem.h
#	Components/CircleItem.h
#	Components/SMDItem.h
#	Components/TextItem.h
#	Components/TrackItem.h
#	Components/ZoneItem.h
)

set(LAYOUTIE_FORMS
	MainWindow.ui
)

set(LAYOUTIE_RESOURCES

)

QT4_WRAP_CPP(LAYOUTIE_HEADERS_MOC ${LAYOUTIE_HEADERS})

QT4_WRAP_UI(LAYOUTIE_FORMS_HEADERS ${LAYOUTIE_FORMS})
QT4_ADD_RESOURCES(LAYOUTIE_RESOURCES_RCC ${LAYOUTIE_RESOURCES})


include(${QT_USE_FILE})
add_definitions(${QT_DEFINITIONS})
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${SPRINT_INCLUDE_DIR} ${QT4_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(layoutie
	${LAYOUTIE_SOURCES}
	${LAYOUTIE_HEADERS_MOC}
	${LAYOUTIE_FORMS_HEADERS}
	${LAYOUTIE_RESOURCES_RCC}
)

target_link_libraries(layoutie ${SPRINT_LIBRARIES} ${QT_LIBRARIES})

set_target_properties(layoutie PROPERTIES COMPILE_FLAGS "-g -Wall -Wextra -Werror -pedantic-errors -Wformat=2 -Winit-self -Wstrict-overflow=5 -Wcast-qual -Wcast-align -Wwrite-strings -Wstrict-null-sentinel  -Woverloaded-virtual -Wsign-promo -std=c++0x -fno-strict-overflow -fstrict-aliasing -Wctor-dtor-privacy -Wnon-virtual-dtor -Wmissing-include-dirs -Wswitch-enum -Wundef -Wshadow -Wlogical-op -Wmissing-declarations -Winline -Wvla")
