cmake_minimum_required(VERSION 3.27)
project(21071IIA)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
include_directories(DiogoAlgoritmos)

add_executable(21071IIA
        DiogoAlgoritmos/JogoDoGalo.cpp
        DiogoAlgoritmos/JogoDoGalo.h
        DiogoAlgoritmos/OitoDamas.cpp
        DiogoAlgoritmos/OitoDamas.h
        DiogoAlgoritmos/Particao.cpp
        DiogoAlgoritmos/Particao.h
        DiogoAlgoritmos/ProcuraEngine.cpp
        DiogoAlgoritmos/Puzzle8.cpp
        DiogoAlgoritmos/Puzzle8.h
        DiogoAlgoritmos/TProcuraAdversa.cpp
        DiogoAlgoritmos/TProcuraAdversa.h
        DiogoAlgoritmos/TProcuraConstrutiva.cpp
        DiogoAlgoritmos/TProcuraConstrutiva.h
        DiogoAlgoritmos/TProcuraConstrutivaComCiclos.cpp
        DiogoAlgoritmos/TProcuraConstrutivaComCiclos.h
        DiogoAlgoritmos/TProcuraMelhorativa.cpp
        DiogoAlgoritmos/TProcuraMelhorativa.h
        DiogoAlgoritmos/TRand.cpp
        DiogoAlgoritmos/TRand.h
        DiogoAlgoritmos/TVector.h
        1802925EFolioA.cpp
        JogoDoGalo.cpp
        JogoDoGalo.h
        OitoDamas.cpp
        OitoDamas.h
        Particao.cpp
        Particao.h
        ProcuraEngine.cpp
        Puzzle8.cpp
        Puzzle8.h
        TProcuraAdversa.cpp
        TProcuraAdversa.h
        TProcuraConstrutiva.cpp
        TProcuraConstrutiva.h
        TProcuraConstrutivaComCiclos.cpp
        TProcuraConstrutivaComCiclos.h
        TProcuraMelhorativa.cpp
        TProcuraMelhorativa.h
        TRand.cpp
        TRand.h
        TVector.h
        xerifes.cpp)

cmake_minimum_required(VERSION 3.10)
project(21071IIA)

set(CMAKE_CXX_STANDARD 17)
# Localiza a SFML
find_package(SFML 2.5 COMPONENTS graphics audio window system REQUIRED)

# Adiciona o executável do seu projeto
add_executable(21071IIA_SFML 1802925EFolioA.cpp  )

# Vincula seu executável com as bibliotecas da SFML
target_link_libraries(21071IIA_SFML sfml-graphics sfml-audio sfml-window sfml-system)

# Adiciona o executável do seu projeto
add_executable(21071IIA_SFML_xerifes xerifes.cpp  )