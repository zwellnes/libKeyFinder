#*************************************************************************
#
# Copyright 2011-2013 Ibrahim Sha'ath
#
# This file is part of LibKeyFinder.
#
# LibKeyFinder is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LibKeyFinder is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LibKeyFinder.  If not, see <http://www.gnu.org/licenses/>.
#
#*************************************************************************

QT -= gui
CONFIG -= qt

TARGET = keyfinder
TEMPLATE = lib

VERSION = 0.6.2

DEFINES += LIBKEYFINDER_LIBRARY

HEADERS += \
    audiodata.h \
    binode.h \
    chromagram.h \
    chromatransform.h \
    chromatransformfactory.h \
    constants.h \
    downsampler.h \
    exception.h \
    fftadapter.h \
    keyclassifier.h \
    keyfinder.h \
    keyfinderresult.h \
    lowpassfilter.h \
    lowpassfilterfactory.h \
    parameters.h \
    ringbuffer.h \
    segmentation.h \
    spectrumanalyser.h \
    toneprofiles.h \
    windowfunctions.h \
    workspace.h \
    keyfinder_api.h

SOURCES += \
    audiodata.cpp \
    chromagram.cpp \
    chromatransform.cpp \
    chromatransformfactory.cpp \
    downsampler.cpp \
    fftadapter.cpp \
    keyclassifier.cpp \
    keyfinder.cpp \
    lowpassfilter.cpp \
    lowpassfilterfactory.cpp \
    parameters.cpp \
    ringbuffer.cpp \
    segmentation.cpp \
    spectrumanalyser.cpp \
    toneprofiles.cpp \
    windowfunctions.cpp \
    workspace.cpp \
    keyfinder_api.cpp

OTHER_FILES += README

unix|macx{
  LIBS += -lfftw3 -lboost_system -lboost_thread
}

unix{
  header.path = /usr/include
  header.files = keyfinder_api.h
  INSTALLS += header
}

macx{
  DEPENDPATH += /usr/local/lib
  INCLUDEPATH += /usr/local/include
  CONFIG -= ppc ppc64
  CONFIG += x86 x86_64
# installs
  QMAKE_LFLAGS_SONAME  = -Wl,-install_name,/usr/local/lib/
  headers.path = /usr/local/include/$$TARGET
  headers.files = $$HEADERS
  INSTALLS += headers
}

win32{
  # FFTW3 (dynamic lib)
  LIBS += -L$$PWD/vs2010-external_libs/fftw-3.3.3-dll32/ -llibfftw3-3
  INCLUDEPATH += $$PWD/vs2010-external_libs/fftw-3.3.3-dll32
  DEPENDPATH += $$PWD/vs2010-external_libs/fftw-3.3.3-dll32

  # Lib Boost (static libs)
  INCLUDEPATH += $$PWD/vs2010-external_libs/boost_1_53_0
  DEPENDPATH += $$PWD/vs2010-external_libs/boost_1_53_0
  LIBS += -L$$PWD/vs2010-external_libs/boost_1_53_0/lib32/

  # Copy FFTW3 DLL
  DLLS = \
      $${PWD}/vs2010-external_libs/fftw-3.3.3-dll32/libfftw3-3.dll
  DESTDIR_WIN = $${DESTDIR}
  CONFIG(debug, debug|release) {
    DESTDIR_WIN += debug
  } else {
    DESTDIR_WIN += release
  }
  DLLS ~= s,/,\\,g
  DESTDIR_WIN ~= s,/,\\,g
  for(FILE, DLLS){
      QMAKE_POST_LINK += $${QMAKE_COPY} $$quote($${FILE}) $$quote($${DESTDIR_WIN}) $$escape_expand(\\n\\t)
  }

  # Copy API file.
  QMAKE_POST_LINK += $${QMAKE_COPY} $$quote(keyfinder_api.h) $$quote($${DESTDIR_WIN}) $$escape_expand(\\n\\t)
}

# the rest auto generated by Qt Creator...

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE1558240
    TARGET.CAPABILITY =
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = LibKeyFinder.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
