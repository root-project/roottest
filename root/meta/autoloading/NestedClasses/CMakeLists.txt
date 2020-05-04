ROOTTEST_GENERATE_REFLEX_DICTIONARY(nestedTemplate
                                    nestedTemplate.h
                                    SELECTION sel1.xml)

ROOTTEST_GENERATE_REFLEX_DICTIONARY(nestedTemplateClone
                                    nestedTemplate_clone.h
                                    SELECTION sel2.xml)

ROOTTEST_ADD_TEST(nestedTemplate
                  MACRO execnestedTemplate.C
                  OUTREF nestedTemplate.ref
                  DEPENDS ${GENERATE_REFLEX_TEST})

ROOTTEST_GENERATE_REFLEX_DICTIONARY(nestedClasses
                                    nestedClasses.h
                                    SELECTION nestedClasses_selection.xml)
if(MSVC)
    set(OUTREF_FILE execNestedClasses_win32.ref)
else()
    set(OUTREF_FILE execNestedClasses.ref)
endif()
ROOTTEST_ADD_TEST(nestedClasses
                  MACRO execNestedClasses.C
                  OUTREF ${OUTREF_FILE}
                  DEPENDS ${GENERATE_REFLEX_TEST})
