TEMPLATE = app

UI_DIR  = obj/Gui
MOC_DIR = obj/Moc
OBJECTS_DIR = obj/Obj

CONFIG += qt
TARGET = ./Malenia
QT += core gui opengl widgets multimedia multimediawidgets

unix {
LIBS += -L../lib/ -Wl,-rpath=./:./HCNetSDKCom:../lib -lhcnetsdk -ldhnetsdk -ldhplay -lPlayCtrl -lAudioRender -lSuperRender -lpq -lpqxx -lchilkat-9.5.0 
}

HEADERS += ../headers/Malenia.h \
           ../headers/LiveViewPrompt.h \
           ../headers/PlaybackPrompt.h \
           ../headers/Database.h \
           ../headers/IDevice.h \
           ../headers/IChannel.h \
           ../headers/IStream.h \
           ../headers/Hikvision.h \
           ../headers/Dahua.h \
           ../headers/Provision.h \
           ../headers/XMEye.h \
     
SOURCES += ../headers/external/md5.cpp \
           ../headers/external/pugixml.cpp \
           ../src/Malenia.cpp \
           ../src/LiveViewPrompt.cpp \
           ../src/PlaybackPrompt.cpp \
           ../src/Database.cpp \
           ../src/IDevice.cpp \
           ../src/IChannel.cpp \
           ../src/IStream.cpp \
           ../src/Hikvision.cpp \
           ../src/Dahua.cpp \
           ../src/Provision.cpp \
           ../src/XMEye.cpp \
           ../src/main.cpp
	  
FORMS += ../src/Malenia.ui \
         ../src/LiveViewPrompt.ui \
         ../src/PlaybackPrompt.ui \

INCLUDEPATH += ../src  \
               ../headers \

