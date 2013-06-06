include(../tests_common.pri)

TEMPLATE = subdirs
SUBDIRS = \
        ut_eventstorage.pro \

configure($${PWD}/runtest.sh.in)
runtest_sh.path = $${INSTALL_TESTDIR}
runtest_sh.files = runtest.sh
runtest_sh.CONFIG = no_check_exist executable
INSTALLS += runtest_sh

configure($${PWD}/tests.xml.in)
tests_xml.path = $${INSTALL_TESTDIR}
tests_xml.files = tests.xml
tests_xml.CONFIG = no_check_exist
INSTALLS += tests_xml
