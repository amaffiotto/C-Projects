#include "raylib.h"
#include <math.h>
#include <stdlib.h>  // <--- AGGIUNGI QUESTA RIGA

// --- DEFINIZIONI ---
#define TILE_SIZE 100
#define BOARD_SIZE 8

// Tipi di pezzi
#define EMPTY 0
#define PAWN 1
#define ROOK 2
#define KNIGHT 3
#define BISHOP 4
#define QUEEN 5
#define KING 6

// --- STATO DEL GIOCO ---
// Numeri positivi = BIANCHI, Numeri negativi = NERI
// Esempio: 2 = Torre Bianca, -2 = Torre Nera
int board[BOARD_SIZE][BOARD_SIZE] = {
    {-2, -3, -4, -5, -6, -4, -3, -2}, // Neri (Riga 0)
    {-1, -1, -1, -1, -1, -1, -1, -1}, // Pedoni Neri
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 0,  0,  0,  0,  0,  0,  0,  0},
    { 1,  1,  1,  1,  1,  1,  1,  1}, // Pedoni Bianchi
    { 2,  3,  4,  5,  6,  4,  3,  2}  // Bianchi (Riga 7)
};

int turn = 1; // 1 = Tocca ai Bianchi, -1 = Tocca ai Neri
int selectedX = -1; // Nessun pezzo selezionato
int selectedY = -1;

// --- FUNZIONI DI LOGICA ---

// Controlla se un percorso è libero (per Torri, Alfieri, Regine)
bool IsPathClear(int startX, int startY, int endX, int endY) {
    int dx = endX - startX;
    int dy = endY - startY;
    int steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

    int xDir = (dx == 0) ? 0 : dx / abs(dx);
    int yDir = (dy == 0) ? 0 : dy / abs(dy);

    for (int i = 1; i < steps; i++) {
        if (board[startY + i * yDir][startX + i * xDir] != EMPTY) {
            return false;
        }
    }
    return true;
}

// Logica di movimento semplificata
bool IsValidMove(int startX, int startY, int endX, int endY) {
    int piece = board[startY][startX];
    int target = board[endY][endX];
    int type = abs(piece);
    int dx = endX - startX;
    int dy = endY - startY;

    // 1. Non puoi mangiare i tuoi pezzi
    if (target != 0 && (piece > 0) == (target > 0)) return false;

    // 2. Logica per tipo di pezzo
    switch (type) {
        case PAWN: {
            int direction = (piece > 0) ? -1 : 1; // Bianchi vanno SU (-1), Neri GIÙ (+1)
            // Movimento in avanti di 1
            if (dx == 0 && dy == direction && target == EMPTY) return true;
            // Movimento in avanti di 2 (solo partenza)
            if (dx == 0 && dy == 2 * direction && target == EMPTY) {
                if ((piece > 0 && startY == 6) || (piece < 0 && startY == 1)) return true;
            }
            // Cattura diagonale
            if (abs(dx) == 1 && dy == direction && target != EMPTY) return true;
            return false;
        }
        case ROOK: // Muove in linea retta
            if (dx != 0 && dy != 0) return false;
            return IsPathClear(startX, startY, endX, endY);

        case BISHOP: // Muove in diagonale
            if (abs(dx) != abs(dy)) return false;
            return IsPathClear(startX, startY, endX, endY);

        case KNIGHT: // Muove a L (2+1) - Il cavallo salta, non controlla il path
            if ((abs(dx) == 2 && abs(dy) == 1) || (abs(dx) == 1 && abs(dy) == 2)) return true;
            return false;

        case QUEEN: // Torre + Alfiere
            if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) return false;
            return IsPathClear(startX, startY, endX, endY);

        case KING: // 1 passo in ogni direzione
            if (abs(dx) <= 1 && abs(dy) <= 1) return true;
            return false;
    }
    return false;
}

// --- RENDERING ---

void DrawPiece(int piece, int x, int y) {
    if (piece == 0) return;

    // Mappa i numeri in lettere (P=Pedone, R=Torre, N=Cavallo, ecc.)
    const char* symbols[] = {" ", "P", "R", "N", "B", "Q", "K"};
    int type = abs(piece);

    // Colore del testo: Nero o Bianco (con contorno grigio per visibilità)
    Color textColor = (piece > 0) ? WHITE : BLACK;

    // Disegna la lettera al centro
    DrawText(symbols[type], x * TILE_SIZE + 35, y * TILE_SIZE + 20, 60, textColor);

    // Piccola ombra per contrasto
    if (piece > 0) DrawText(symbols[type], x * TILE_SIZE + 37, y * TILE_SIZE + 22, 60, DARKGRAY);
}

int main(void) {
    InitWindow(BOARD_SIZE * TILE_SIZE, BOARD_SIZE * TILE_SIZE, "Scacchi C Raylib - Basic");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // --- GESTIONE INPUT ---
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            int mouseX = GetMouseX() / TILE_SIZE;
            int mouseY = GetMouseY() / TILE_SIZE;

            // Se clicco dentro la scacchiera
            if (mouseX >= 0 && mouseX < 8 && mouseY >= 0 && mouseY < 8) {

                // CASO 1: Seleziono un pezzo
                if (selectedX == -1) {
                    int piece = board[mouseY][mouseX];
                    // Posso selezionare solo i miei pezzi
                    if (piece != 0 && ((turn == 1 && piece > 0) || (turn == -1 && piece < 0))) {
                        selectedX = mouseX;
                        selectedY = mouseY;
                    }
                }
                // CASO 2: Provo a muovere
                else {
                    // Se clicco sullo stesso pezzo, deseleziono
                    if (mouseX == selectedX && mouseY == selectedY) {
                        selectedX = -1;
                        selectedY = -1;
                    } else {
                        // Provo a muovere
                        if (IsValidMove(selectedX, selectedY, mouseX, mouseY)) {
                            // Esegui mossa
                            board[mouseY][mouseX] = board[selectedY][selectedX];
                            board[selectedY][selectedX] = 0;

                            // Cambio turno
                            turn *= -1;

                            // Deseleziono
                            selectedX = -1;
                            selectedY = -1;
                        }
                    }
                }
            }
        }

        // --- DISEGNO ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (int y = 0; y < BOARD_SIZE; y++) {
            for (int x = 0; x < BOARD_SIZE; x++) {
                // 1. Disegna sfondo scacchiera
                Color col = ((x + y) % 2 == 0) ? (Color){235, 236, 208, 255} : (Color){119, 149, 86, 255};

                // Evidenzia selezione
                if (x == selectedX && y == selectedY) col = YELLOW;

                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE, col);

                // 2. Disegna pezzo
                DrawPiece(board[y][x], x, y);
            }
        }

        // Indicatore turno
        if (turn == 1) DrawText("Turno: BIANCO", 10, 10, 20, GRAY);
        else DrawText("Turno: NERO", 10, 10, 20, WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}