TEST_TARGETS += thetest

CLEAN_TARGETS += $(ALL_LIBRARIES) *.log *.success

ifeq ($(strip $(ROOTTEST_HOME)),)
	export ROOTTEST_HOME := $(shell expr "$(CURDIR)" : '\(.*/roottest/\)')
endif

include $(ROOTTEST_HOME)/scripts/Rules.mk


testWithFailure:
ifeq ($(FAIL),)
	$(WarnFailTest)
endif


thetest : thetest.success

thetest.success: thetest.clog
	$(SuccessTestDiff) && touch $@

