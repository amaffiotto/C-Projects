cmake_minimum_required(VERSION 3.20)
project(scacchi_c C)

set(CMAKE_C_STANDARD 99)

# --- CONFIGURAZIONE RAYLIB ---

# 1. Definiamo dove Homebrew potrebbe aver messo le librerie
set(HOMEBREW_PATH_M1 "/opt/homebrew")
set(HOMEBREW_PATH_INTEL "/usr/local")

# 2. Controlliamo quale percorso esiste sul tuo Mac
if(EXISTS ${HOMEBREW_PATH_M1})
    include_directories("${HOMEBREW_PATH_M1}/include")
    link_directories("${HOMEBREW_PATH_M1}/lib")
elseif(EXISTS ${HOMEBREW_PATH_INTEL})
    include_directories("${HOMEBREW_PATH_INTEL}/include")
    link_directories("${HOMEBREW_PATH_INTEL}/lib")
endif()

# --- FINE CONFIGURAZIONE ---

add_executable(scacchi_c main.c)

# 3. Linkiamo la libreria Raylib all'eseguibile
target_link_libraries(scacchi_c raylib)