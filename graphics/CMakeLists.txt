# TODO: untested on Windows
# if (CMAKE_SYSTEM_NAME MATCHES Darwin)
if (NOT MSVC)

# Folders for generated files
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(JSON_PRO_DIR ${BUILD_DIR}/json_pro)
set(NEW_SVG_PRO_DIR ${BUILD_DIR}/new_svg_pro)
set(OLD_SVG_PRO_DIR ${BUILD_DIR}/old_svg_pro)
set(PDF_PRO_DIR ${BUILD_DIR}/pdf_pro)

# Create the generated directory
file(MAKE_DIRECTORY ${JSON_PRO_DIR})
file(MAKE_DIRECTORY ${NEW_SVG_PRO_DIR})
file(MAKE_DIRECTORY ${OLD_SVG_PRO_DIR})
file(MAKE_DIRECTORY ${PDF_PRO_DIR})

# Decompress and copy over all references
foreach(FOLDER json_ref old_svg_ref pdf_ref new_svg_ref)
    file(GLOB ARCHIVES ${FOLDER}/*zip )
    foreach(ARCHIVE ${ARCHIVES} )
        file(ARCHIVE_EXTRACT INPUT ${ARCHIVE} DESTINATION ${FOLDER})
    endforeach()
endforeach()

# Copy over the files needed for testing
file(COPY macros DESTINATION ${BUILD_DIR})
file(COPY testGraphics.C DESTINATION ${BUILD_DIR})

# ROOTTEST_GENERATE_EXECUTABLE(testGraphics testGraphics.C LIBRARIES Core Hist Graf Gpad WebGui6)

# High level description of the tests
# - j == JSON
# - o == old SVG
# - p == old PDF
# - s == new SVG
# - a == all

set(TEST_DESCRIPTIONS
    "AtlasExample a graphics"
    "analyze a graphics"
    "archi a graphics"
    "arrows a graphics"
    "compile a graphics"
    "crown a graphics"
    "basic3d a graphics"
    "canvas a graphics"
    "ellipse a graphics"
    "eval a graphics"
    "event a graphics"
    "feynman a graphics"
    "first a graphics"
    "formula1 a graphics"
    "framework a graphics"
    "gaxis a graphics"
    "gaxis3 a graphics"
    "greyscale a graphics"
    "latex a graphics"
    "latex2 a graphics"
    "latex3 a graphics"
    "latex4 a graphics"
    "latex5 a graphics"
    "markerwarning a graphics"
    "mass_spectrum a graphics"
    "pavetext a graphics"
    "piechart a graphics"
    "quarks a graphics"
    "schroedinger_hydrogen a graphics"
    "tmathtext a graphics"
    "tmathtext2 a graphics"
    "triangles a graphics"
    "transparency a graphics"
    "annotation3d a graphs"
    "approx a graphs"
    "exclusiongraph a graphs"
    "exclusiongraph2 a graphs"
    "gerrors a graphs"
    "gerrors2 a graphs"
    "gmultierrors a graphs"
    "graph a graphs"
    "graph2derrorsfit a graphs"
    "graphApply a graphs"
    "graphShade a graphs"
    "graphpolar a graphs"
    "graphpolar2 a graphs"
    "graphpolar3 a graphs"
    "graphreverse a graphs"
    "hlGraph1 a graphs"
    "labels1 a graphs"
    "labels2 a graphs"
    "multigraph a graphs"
    "scatter a graphs"
    "surfaces a graphs"
    "timeonaxis a graphs"
    "timeonaxis2 a graphs"
    "timeonaxis3 a graphs"
    "waves a graphs"
    "zdemo a graphs"
    "zones a graphs"
    "DynamicSlice a hist"
    "Fibonacci a hist"
    "NormalizeHistogram a hist"
    "ZoomHistogram a hist"
    "candledecay a hist"
    "candleplot a hist"
    "candleplotstack a hist"
    "candleplotwhiskers a hist"
    "candlescaled a hist"
    "fillhistosauto2p a hist"
    "fillrandom a hist"
    "h2_cut a hist"
    "h2proj a hist"
    "histpalettecolor a hist"
    "hksimple a hist"
    "hlHisto1 a hist"
    "hlHisto2 a hist"
    "hlabels1 a hist"
    "hstack a hist"
    "legendautoplaced a hist"
    "logscales a hist"
    "movepalette a hist"
    "multicolor a hist"
    "ratioplot1 a hist"
    "ratioplot2 a hist"
    "ratioplot3 a hist"
    "ratioplot4 a hist"
    "ratioplot5 a hist"
    "ratioplot6 a hist"
    "ratioplotOld a hist"
    "rebin a hist"
    "reverseaxis a hist"
    "testSmooth a hist"
    "th2polyBoxes a hist"
    "th2polyEurope a hist"
    "th2polyHoneycomb a hist"
    "th2polyUSA a hist"
    "twoscales a hist"
    "xyplot a hist"
    "CrystalBall a math"
    "BreitWigner a math"
    "GammaFun a math"
    "chi2test a math"
    "hlquantiles a math"
    "mathBeta a math"
    "mathGammaNormal a math"
    "mathLaplace a math"
    "mathStudent a math"
    "mathcoreCDF a math"
    "mathcoreStatFunc a math"
    "normalDist a math"
    "quantiles a math"
    "vavilov a math"
    "gaussian_ratio_plot a proposal"
)

foreach(TEST_DESCRIPTION ${TEST_DESCRIPTIONS})

    string(REGEX MATCH "([^ ]+) ([^ ]+) ([^ ]+)" _ ${TEST_DESCRIPTION})
    set(macro ${CMAKE_MATCH_1})
    set(test_type ${CMAKE_MATCH_2})
    set(macro_folder ${CMAKE_MATCH_3})

    if(${test_type} STREQUAL "j")
        ROOTTEST_ADD_TEST(${macro}_j
        MACRO testGraphics.C
        MACROARG \"${macro}\",\"j\",\"${macro_folder}\",\"${BUILD_DIR}\")
    
    elseif(${test_type} STREQUAL "o")
        ROOTTEST_ADD_TEST(${macro}_o
        MACRO testGraphics.C
        MACROARG \"${macro}\",\"o\",\"${macro_folder}\",\"${BUILD_DIR}\")
    
    elseif(${test_type} STREQUAL "p")
        ROOTTEST_ADD_TEST(${macro}_p
        MACRO testGraphics.C
        MACROARG \"${macro}\",\"p\",\"${macro_folder}\",\"${BUILD_DIR}\")
    
    elseif(${test_type} STREQUAL "s")
        ROOTTEST_ADD_TEST(${macro}_s
        MACRO testGraphics.C
        MACROARG \"${macro}\",\"s\",\"${macro_folder}\",\"${BUILD_DIR}\")
    
    # TODO: Headless chrome browser in ROOT CI and swbug hanging browser, 
    # maybe also some new references

    # elseif(${test_type} STREQUAL "a")
    #     foreach (X "j" "o" "p" "s")
    #         ROOTTEST_ADD_TEST(${macro}_${X}
    #             MACRO testGraphics.C
    #             MACROARG \"${macro}\",\"${X}\",\"${macro_folder}\",\"${BUILD_DIR}\")
    #     endforeach()
    
    # New SVG disabled
    elseif(${test_type} STREQUAL "a")
        foreach (X "j" "o" "p")
            ROOTTEST_ADD_TEST(${macro}_${X}
                MACRO testGraphics.C
                MACROARG \"${macro}\",\"${X}\",\"${macro_folder}\",\"${BUILD_DIR}\")
        endforeach()
    else()
    ROOTTEST_ADD_TEST(${macro}_${test_type}
        MACRO testGraphics.C
        MACROARG \"${macro}\",\"${test_type}\",\"${macro_folder}\",\"${BUILD_DIR}\")

    endif()
    
endforeach()

endif() # MSVC
