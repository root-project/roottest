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

# MAC OS platforms
if (CMAKE_SYSTEM_NAME MATCHES Darwin)
set(TEST_DESCRIPTIONS
    #"AtlasExample a graphics" Excluded because of MAC failures (2. round, j)
    "AtlasExample o graphics"
    "AtlasExample p graphics"
    #"analyze a graphics" Excluded because of MAC failures (2. round, o p)
    "analyze j graphics"
    #"archi a graphics" Excluded because of MAC failures (2. round, o p)
    "archi j graphics"
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
    #"piechart a graphics" Excluded because of MAC failures (2. round, o)
    "piechart j graphics"
    "piechart p graphics"
    "quarks a graphics"
    #"schroedinger_hydrogen a graphics" Excluded because of MAC failures (2. round, o p)
    "schroedinger_hydrogen j graphics"
    "tmathtext a graphics"
    "tmathtext2 a graphics"
    "triangles a graphics"
    "transparency a graphics"
    #"annotation3d a graphs" Excluded because of MAC failures (2. round, o p)
    "annotation3d j graphs"
    "approx a graphs"
    #"exclusiongraph a graphs" Excluded because of MAC failures (2. round, o p)
    "exclusiongraph j graphs"
    "exclusiongraph2 a graphs"
    "gerrors a graphs"
    #"gerrors2 a graphs" Excluded because of MAC failures (2. round, o p)
    "gerrors2 j graphs"
    "gmultierrors a graphs"
    "graph a graphs"
    #"graph2derrorsfit a graphs" Excluded because of MAC failures (2. round, o p)
    "graph2derrorsfit j graphs"
    #"graphApply a graphs" Excluded because of MAC failures (2. round, o p)
    "graphApply j graphs"
    "graphShade a graphs"
    #"graphpolar a graphs" Excluded because of MAC failures (2. round, o p)
    "graphpolar j graphs"
    "graphpolar2 a graphs"
    "graphpolar3 a graphs"
    #"graphreverse a graphs" Excluded because of MAC failures (2. round, o p)
    "graphreverse j graphs"
    "hlGraph1 a graphs"
    "labels1 a graphs"
    "labels2 a graphs"
    #"multigraph a graphs" Excluded because of MAC failures (2. round,j o p)
    "scatter a graphs"
    #"surfaces a graphs" Excluded because of MAC failures (2. round, o p)
    "surfaces j graphs"
    #"timeonaxis a graphs" Excluded because of linux fails (1. round)
    #"timeonaxis2 a graphs" Excluded because of linux fails (1. round)
    #"timeonaxis3 a graphs" Excluded because of linux fails (1. round)
    "waves a graphs"
    "zdemo a graphs"
    "zones a graphs"
    "DynamicSlice a hist"
    #"Fibonacci a hist" Excluded because of MAC failures (2. round,j)
    "Fibonacci o hist"
    "Fibonacci p hist"
    "NormalizeHistogram a hist"
    "ZoomHistogram a hist"
    "candledecay a hist"
    #"candleplot a hist" Excluded because of linux fails (1. round)
    #"candleplotstack a hist" Excluded because of linux fails (1.round)
    "candleplotwhiskers a hist"
    #"candlescaled a hist" Excluded because of MAC failures (2. round, o)
    "candlescaled j hist"
    "candlescaled p hist"
    "fillhistosauto2p a hist"
    "fillrandom a hist"
    "h2_cut a hist"
    #"h2proj a hist" Excluded because of MAC failures (2. round, p)
    "h2proj j hist"
    "h2proj o hist"
    "histpalettecolor a hist"
    "hksimple a hist"
    "hlHisto1 a hist"
    "hlHisto2 a hist"
    "hlabels1 a hist"
    #"hstack a hist" Excluded because of MAC failures (2. round, o p)
    "hstack j hist"
    "legendautoplaced a hist"
    "logscales a hist"
    "movepalette a hist"
    "multicolor a hist"
    "ratioplot1 a hist"
    #"ratioplot2 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot2 j hist"
    #"ratioplot3 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot3 j hist"
    #"ratioplot4 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot4 j hist"
    #"ratioplot5 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot5 j hist"
    #"ratioplot6 a hist" Excluded because of MAC failures (2. round, o)
    "ratioplot6 j hist"
    "ratioplot6 p hist"
    "ratioplotOld a hist"
    #"rebin a hist" Excluded because of MAC failures (2. round, j o p)
    #"reverseaxis a hist" Excluded because of MAC failures (2. round, o p)
    "reverseaxis j hist"
    #"testSmooth a hist" Excluded because of MAC failures (2. round, o p)
    "testSmooth j hist"
    "th2polyBoxes a hist"
    "th2polyEurope a hist"
    #"th2polyHoneycomb a hist" Excluded because of MAC failures (2. round, j o p)
    "th2polyUSA a hist"
    #"twoscales a hist" Excluded because of MAC failures (2. round, o p)
    "twoscales j hist"
    "xyplot a hist"
    "CrystalBall a math"
    "BreitWigner a math"
    #"GammaFun a math" Excluded because of MAC failures (2. round, o p)
    "GammaFun j math"
    "chi2test a math"
    "hlquantiles a math"
    "mathBeta a math"
    "mathGammaNormal a math"
    #"mathLaplace a math" Excluded because of MAC failures (2. round, o)
    "mathLaplace j math"
    "mathLaplace p math"
    #"mathStudent a math" Excluded because of MAC failures (2. round, o p)
    "mathStudent j math"
    #"mathcoreCDF a math" Excluded because of MAC failures (2. round, o p)
    "mathcoreCDF j math"
    #"mathcoreStatFunc a math" Excluded because of MAC failures (2. round, o p)
    "mathcoreStatFunc j math"
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
    
    # elseif(${test_type} STREQUAL "s")
    #     ROOTTEST_ADD_TEST(${macro}_s
    #     MACRO testGraphics.C
    #     MACROARG \"${macro}\",\"s\",\"${macro_folder}\",\"${BUILD_DIR}\")
    
    # TODO: Headless chrome browser in ROOT CI and debug hanging browser, 
    # maybe also some new references for MAC OS

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

# Linux platforms
else()
set(TEST_DESCRIPTIONS
    #"AtlasExample a graphics" Excluded because of MAC failures (2. round, j)
    "AtlasExample o graphics"
    "AtlasExample p graphics"
    "AtlasExample s graphics"
    #"analyze a graphics" Excluded because of MAC failures (2. round, o p)
    "analyze j graphics"
    "analyze s graphics"
    #"archi a graphics" Excluded because of MAC failures (2. round, o p)
    "archi j graphics"
    "archi s graphics"
    "arrows a graphics"
    "compile a graphics"
    "crown a graphics"
    "basic3d a graphics"
    "canvas a graphics"
    "ellipse a graphics"
    "eval a graphics"
    "event a graphics"
    "feynman a graphics"
    "first a graphics" #s
    "formula1 a graphics" #S
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
    #"piechart a graphics" Excluded because of MAC failures (2. round, o)
    "piechart j graphics"
    "piechart p graphics"
    "piechart s graphics"
    "quarks a graphics"
    #"schroedinger_hydrogen a graphics" Excluded because of MAC failures (2. round, o p)
    "schroedinger_hydrogen j graphics"
    "schroedinger_hydrogen s graphics"
    "tmathtext a graphics"
    "tmathtext2 a graphics"
    "triangles a graphics"
    "transparency a graphics"
    #"annotation3d a graphs" Excluded because of MAC failures (2. round, o p)
    "annotation3d j graphs"
    "annotation3d s graphs"
    "approx a graphs"
    #"exclusiongraph a graphs" Excluded because of MAC failures (2. round, o p)
    "exclusiongraph j graphs"
    "exclusiongraph s graphs"
    "exclusiongraph2 a graphs"
    "gerrors a graphs"
    #"gerrors2 a graphs" Excluded because of MAC failures (2. round, o p)
    "gerrors2 j graphs"
    "gerrors2 s graphs"
    "gmultierrors a graphs"
    "graph a graphs"
    #"graph2derrorsfit a graphs" Excluded because of MAC failures (2. round, o p)
    "graph2derrorsfit j graphs"
    "graph2derrorsfit s graphs"
    #"graphApply a graphs" Excluded because of MAC failures (2. round, o p)
    "graphApply j graphs"
    "graphApply s graphs"
    "graphShade a graphs"
    #"graphpolar a graphs" Excluded because of MAC failures (2. round, o p)
    "graphpolar j graphs"
    "graphpolar s graphs"
    "graphpolar2 a graphs"
    "graphpolar3 a graphs"
    #"graphreverse a graphs" Excluded because of MAC failures (2. round, o p)
    "graphreverse j graphs"
    "graphreverse s graphs"
    "hlGraph1 a graphs"
    "labels1 a graphs"
    "labels2 a graphs"
    #"multigraph a graphs" Excluded because of MAC failures (2. round,j o p)
    "scatter a graphs"
    #"surfaces a graphs" Excluded because of MAC failures (2. round, o p)
    "surfaces j graphs"
    "surfaces s graphs"
    #"timeonaxis a graphs" Excluded because of linux fails (1. round)
    #"timeonaxis2 a graphs" Excluded because of linux fails (1. round)
    #"timeonaxis3 a graphs" Excluded because of linux fails (1. round)
    "waves a graphs"
    "zdemo a graphs"
    "zones a graphs"
    "DynamicSlice a hist"
    #"Fibonacci a hist" Excluded because of MAC failures (2. round,j)
    "Fibonacci o hist"
    "Fibonacci p hist"
    "Fibonacci s hist"
    "NormalizeHistogram a hist"
    "ZoomHistogram a hist"
    "candledecay a hist"
    #"candleplot a hist" Excluded because of linux fails (1. round)
    #"candleplotstack a hist" Excluded because of linux fails (1.round)
    "candleplotwhiskers a hist"
    #"candlescaled a hist" Excluded because of MAC failures (2. round, o)
    "candlescaled j hist"
    "candlescaled p hist"
    "candlescaled s hist"
    "fillhistosauto2p a hist"
    "fillrandom a hist"
    "h2_cut a hist"
    #"h2proj a hist" Excluded because of MAC failures (2. round, p)
    "h2proj j hist"
    "h2proj o hist"
    "h2proj s hist"
    "histpalettecolor a hist"
    "hksimple a hist"
    "hlHisto1 a hist"
    "hlHisto2 a hist"
    "hlabels1 a hist"
    #"hstack a hist" Excluded because of MAC failures (2. round, o p)
    "hstack j hist"
    "hstack s hist"
    "legendautoplaced a hist"
    "logscales a hist"
    "movepalette a hist"
    "multicolor a hist"
    "ratioplot1 a hist"
    #"ratioplot2 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot2 j hist"
    #"ratioplot3 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot3 j hist"
    #"ratioplot4 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot4 j hist"
    #"ratioplot5 a hist" Excluded because of MAC failures (2. round, o p)
    "ratioplot5 j hist"
    #"ratioplot6 a hist" Excluded because of MAC failures (2. round, o)
    "ratioplot6 j hist"
    "ratioplot6 p hist"
    "ratioplotOld a hist"
    #"rebin a hist" Excluded because of MAC failures (2. round, j o p)
    #"reverseaxis a hist" Excluded because of MAC failures (2. round, o p)
    "reverseaxis j hist"
    "reverseaxis s hist"
    #"testSmooth a hist" Excluded because of MAC failures (2. round, o p)
    "testSmooth j hist"
    "testSmooth s hist"
    "th2polyBoxes a hist"
    "th2polyEurope a hist"
    #"th2polyHoneycomb a hist" Excluded because of MAC failures (2. round, j o p)
    "th2polyUSA a hist"
    #"twoscales a hist" Excluded because of MAC failures (2. round, o p)
    "twoscales j hist"
    "twoscales s hist"
    "xyplot a hist"
    "CrystalBall a math"
    "BreitWigner a math"
    #"GammaFun a math" Excluded because of MAC failures (2. round, o p)
    "GammaFun j math"
    "GammaFun s math"
    "chi2test a math"
    "hlquantiles a math"
    "mathBeta a math"
    "mathGammaNormal a math"
    #"mathLaplace a math" Excluded because of MAC failures (2. round, o)
    "mathLaplace j math"
    "mathLaplace p math"
    "mathLaplace s math"
    #"mathStudent a math" Excluded because of MAC failures (2. round, o p)
    "mathStudent j math"
    "mathStudent s math"
    #"mathcoreCDF a math" Excluded because of MAC failures (2. round, o p)
    "mathcoreCDF j math"
    "mathcoreCDF s math"
    #"mathcoreStatFunc a math" Excluded because of MAC failures (2. round, o p)
    "mathcoreStatFunc j math"
    "mathcoreStatFunc s math"
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
    
    # TODO: Debug hanging browser, maybe also some new references

    elseif(${test_type} STREQUAL "a")
        foreach (X "j" "o" "p" "s")
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

endif() # MAC OS or Linux

endif() # MSVC
