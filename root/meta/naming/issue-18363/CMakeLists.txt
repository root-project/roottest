ROOTTEST_GENERATE_REFLEX_DICTIONARY(size_tTemplateDict Objects.h SELECTION selection.xml FIXTURES_SETUP size_tTemplate_lib)

ROOTTEST_ADD_TEST(size_tTemplateName
                  MACRO size_tTemplateNameTest.C
                  OUTREF size_tTemplateNameTest.ref
		  FIXTURES_REQUIRED size_tTemplate_lib)
