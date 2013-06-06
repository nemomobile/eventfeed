include(tests_common.pri)

pro_file_basename = $$basename(_PRO_FILE_)
pro_file_basename ~= s/\\.pro$//

TEMPLATE = app
TARGET = $${pro_file_basename}

QT += testlib

SOURCES = $${pro_file_basename}.cpp

target.path = $${INSTALL_TESTDIR}
INSTALLS += target
