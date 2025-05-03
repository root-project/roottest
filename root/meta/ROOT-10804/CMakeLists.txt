ROOTTEST_GENERATE_REFLEX_DICTIONARY(ROOT10804 ROOT10804.h SELECTION selection.xml)
set(depends ${GENERATE_REFLEX_TEST})

# Class names starting with '!': check that they do *not* have a protoclass.
set(CLASSES_TO_TEST
    "Outer::Inline::Class"
    "Outer::Class"

    "list<Outer::Inline::Class>"
    "list<Outer::Class>"
    "std::list<Outer::Inline::Class>"
    "std::list<Outer::Class>"

    "Outer::Container<Outer::Inline::Class>"
    "Outer::Container<Outer::Class>"

    "Outer::Inline::Template<Outer::Inline::Class>"
    "Outer::Template<Outer::Inline::Class>"
    "Outer::Inline::Template<Outer::Class>"
    "Outer::Template<Outer::Class>"

    "Outer::Container<Outer::Inline::Template<Outer::Inline::Class>>"
    "Outer::Container<Outer::Inline::Template<Outer::Class>>"
    "Outer::Container<Outer::Template<Outer::Inline::Class>>>"

    "Outer::Container<Outer::Template<Outer::Class>>"

    # Container is not `Inline::`:
    "!Outer::Inline::Container<Outer::Inline::Template<Outer::Inline::Class>>"

    # Float16_t won't get a demangled alternate name,
    # only as-written-in-selection.xml
    "Outer::Template<Float16_t>"

    # Found because of fwd decl of Inline as inline:
    "Outer::Inline::Template<Float16_t>"

    # Double32_t won't get a demangled alternate name,
    # only as-written-in-selection.xml
    "Outer::Container<Outer::Template<Double32_t>>"
    # Found because of fwd decl of Inline as inline:
    "Outer::Container<Outer::Inline::Template<Double32_t>>"

    # Cross-check: not selected.
    "!Outer::Container<int>"
    "!Outer::Container<Outer::Template<Float16_t>>"
    "!Outer::Template<float>" # but did select Template<Float16_t>!
    )

list(LENGTH CLASSES_TO_TEST LEN_CLASSES_TO_TEST)
math(EXPR LAST_CLASS_TO_TEST ${LEN_CLASSES_TO_TEST}-1)
# foreach takes *last*, not num-elements :-(
foreach(INDEX RANGE ${LAST_CLASS_TO_TEST})
  list(GET CLASSES_TO_TEST ${INDEX} TEST_CLASS)

  ROOTTEST_ADD_TEST(execROOT10804_${INDEX}
                    MACRO execROOT10804.C
                    MACROARG "\"${TEST_CLASS}\""
                    OUTREF execROOT10804.oref
                    ERRREF execROOT10804.eref
                    DEPENDS ${depends})
endforeach()
