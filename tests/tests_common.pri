isEmpty(TESTS_COMMON_PRI_INCLUDED) {
TESTS_COMMON_PRI_INCLUDED = 1

include(../common.pri)

subproject = $$dirname(_PRO_FILE_)
subproject = $$basename(subproject)
INSTALL_TESTDIR = /opt/tests/$${subproject}$${DASH_QT_VERSION}
INSTALL_TESTDATADIR = $${INSTALL_TESTDIR}/data

CONFIG_SUBST += INSTALL_TESTDIR
CONFIG_SUBST += INSTALL_TESTDATADIR

}
