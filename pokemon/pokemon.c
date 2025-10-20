#define _CRT_SECURE_NO_WARNINGS  // Evita warnings em funções como scanf e strcpy
#pragma execution_character_set("utf-8")  // Para MSVC: Força charset UTF-8 na execução

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <windows.h>
#include <conio.h>
#include <locale.h>

// Definições básicas
#define TOTAL_POKEMON 151
#define MAX_NAME_LENGTH 20
#define MAX_MOVES 4
#define MAX_POKEMON_PARTY 6
#define MAX_POKEMON_STORAGE 100
#define MAP_WIDTH 40
#define MAP_HEIGHT 20

// Tipos
typedef enum {
    NORMAL, FIRE, WATER, ELECTRIC, GRASS, ICE, FIGHTING, POISON,
    GROUND, FLYING, PSYCHIC, BUG, ROCK, GHOST, DRAGON
} PokemonType;

typedef enum {
    PHYSICAL, SPECIAL, STATUS
} MoveCategory;

// Estruturas
typedef struct {
    char name[20];
    PokemonType type;
    MoveCategory category;
    int power;
    int accuracy;
    int pp;
    int maxPP;
} Move;

typedef struct {
    int id;
    char name[MAX_NAME_LENGTH];
    PokemonType type1;
    PokemonType type2;
    int baseHP;
    int baseAttack;
    int baseDefense;
    int baseSpeed;
    int baseSpecial;
    int catchRate;
    int experienceYield;
    int evolutionLevel;
    int evolvesTo;
} PokemonData;

typedef struct {
    PokemonData *data;
    int level;
    int currentHP;
    int maxHP;
    int attack;
    int defense;
    int speed;
    int special;
    Move moves[MAX_MOVES];
    int movesCount;
    int status;
    int experience;
    int expToNextLevel;
} Pokemon;

typedef struct {
    char name[20];
    Pokemon party[MAX_POKEMON_PARTY];
    int partySize;
    Pokemon storage[MAX_POKEMON_STORAGE];
    int storageSize;
    int items[20];
    int money;
    int badges;
    int positionX;
    int positionY;
    int mapId;
} Player;

// Dados básicos dos Pokémon (primeiros 20 para exemplo)
PokemonData pokedex[20] = {
    {1, "Bulbasaur", GRASS, POISON, 45, 49, 49, 45, 65, 45, 64, 16, 2},
    {2, "Ivysaur", GRASS, POISON, 60, 62, 63, 60, 80, 45, 142, 32, 3},
    {3, "Venusaur", GRASS, POISON, 80, 82, 83, 80, 100, 45, 236, 0, 0},
    {4, "Charmander", FIRE, NORMAL, 39, 52, 43, 65, 50, 45, 62, 16, 5},
    {5, "Charmeleon", FIRE, NORMAL, 58, 64, 58, 80, 65, 45, 142, 36, 6},
    {6, "Charizard", FIRE, FLYING, 78, 84, 78, 100, 85, 45, 240, 0, 0},
    {7, "Squirtle", WATER, NORMAL, 44, 48, 65, 43, 50, 45, 63, 16, 8},
    {8, "Wartortle", WATER, NORMAL, 59, 63, 80, 58, 65, 45, 142, 36, 9},
    {9, "Blastoise", WATER, NORMAL, 79, 83, 100, 78, 85, 45, 239, 0, 0},
    {10, "Caterpie", BUG, NORMAL, 45, 30, 35, 45, 20, 255, 53, 7, 11},
    {11, "Metapod", BUG, NORMAL, 50, 20, 55, 30, 25, 120, 72, 10, 12},
    {12, "Butterfree", BUG, FLYING, 60, 45, 50, 70, 80, 45, 160, 0, 0},
    {13, "Weedle", BUG, POISON, 40, 35, 30, 50, 20, 255, 52, 7, 14},
    {14, "Kakuna", BUG, POISON, 45, 25, 50, 35, 25, 120, 71, 10, 15},
    {15, "Beedrill", BUG, POISON, 65, 80, 40, 75, 45, 45, 159, 0, 0},
    {16, "Pidgey", NORMAL, FLYING, 40, 45, 40, 56, 35, 255, 50, 18, 17},
    {17, "Pidgeotto", NORMAL, FLYING, 63, 60, 55, 71, 50, 120, 122, 36, 18},
    {18, "Pidgeot", NORMAL, FLYING, 83, 80, 75, 101, 70, 45, 211, 0, 0},
    {19, "Rattata", NORMAL, NORMAL, 30, 56, 35, 72, 25, 255, 51, 20, 20},
    {20, "Raticate", NORMAL, NORMAL, 55, 81, 60, 97, 50, 127, 116, 0, 0}
};

// Função setColor
void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Função para centralizar texto
void centerText(const char* text, int width) {
    int len = strlen(text);
    int padding = (width - len) / 2;
    for(int i = 0; i < padding; i++) printf(" ");
    printf("%s", text);
    for(int i = 0; i < padding; i++) printf(" ");
    if((width - len) % 2 != 0) printf(" ");
}

// Função para desenhar linha horizontal
void drawHorizontalLine(int width, int color) {
    setColor(color);
    for(int i = 0; i < width; i++) printf("═");
    setColor(7);
}

// Função para desenhar caixa de texto
void drawTextBox(const char* text, int width, int color) {
    setColor(color);
    printf("╔");
    for(int i = 0; i < width - 2; i++) printf("═");
    printf("╗\n");
    
    printf("║");
    centerText(text, width - 2);
    printf("║\n");
    
    printf("╚");
    for(int i = 0; i < width - 2; i++) printf("═");
    printf("╝\n");
    setColor(7);
}

// Funções básicas do Pokémon
Pokemon createPokemon(int id, int level) {
    Pokemon pokemon;
    pokemon.data = &pokedex[id-1];
    pokemon.level = level;
    
    // Cálculo simplificado de stats
    pokemon.maxHP = ((2 * pokemon.data->baseHP * level) / 100) + level + 10;
    pokemon.attack = ((2 * pokemon.data->baseAttack * level) / 100) + 5;
    pokemon.defense = ((2 * pokemon.data->baseDefense * level) / 100) + 5;
    pokemon.speed = ((2 * pokemon.data->baseSpeed * level) / 100) + 5;
    pokemon.special = ((2 * pokemon.data->baseSpecial * level) / 100) + 5;
    
    pokemon.currentHP = pokemon.maxHP;
    pokemon.experience = 0;
    pokemon.expToNextLevel = level * level * level;
    pokemon.status = 0;
    
    // Movimentos básicos
    pokemon.movesCount = 2;
    strcpy(pokemon.moves[0].name, "Tackle");
    pokemon.moves[0].type = NORMAL;
    pokemon.moves[0].power = 35;
    pokemon.moves[0].accuracy = 95;
    pokemon.moves[0].pp = 35;
    pokemon.moves[0].maxPP = 35;
    
    if(pokemon.data->type1 == FIRE) {
        strcpy(pokemon.moves[1].name, "Ember");
        pokemon.moves[1].type = FIRE;
    } else if(pokemon.data->type1 == WATER) {
        strcpy(pokemon.moves[1].name, "Water Gun");
        pokemon.moves[1].type = WATER;
    } else if(pokemon.data->type1 == GRASS) {
        strcpy(pokemon.moves[1].name, "Vine Whip");
        pokemon.moves[1].type = GRASS;
    } else {
        strcpy(pokemon.moves[1].name, "Scratch");
        pokemon.moves[1].type = NORMAL;
    }
    pokemon.moves[1].power = 40;
    pokemon.moves[1].accuracy = 100;
    pokemon.moves[1].pp = 25;
    pokemon.moves[1].maxPP = 25;
    
    return pokemon;
}

int gainExperience(Pokemon *pokemon, int baseExp) {
    int expGained = (baseExp * pokemon->level) / 7;
    pokemon->experience += expGained;
    
    setColor(14);
    printf("\n╔══════════════════════════════════╗\n");
    printf("║ ");
    setColor(10);
    printf("%s", pokemon->data->name);
    setColor(14);
    printf(" gained %d EXP. Points! ", expGained);
    printf("║\n");
    printf("╚══════════════════════════════════╝\n");
    setColor(7);
    
    if(pokemon->experience >= pokemon->expToNextLevel && pokemon->level < 100) {
        pokemon->level++;
        pokemon->experience = 0;
        pokemon->expToNextLevel = pokemon->level * pokemon->level * pokemon->level;
        
        // Recalcular stats
        pokemon->maxHP = ((2 * pokemon->data->baseHP * pokemon->level) / 100) + pokemon->level + 10;
        pokemon->attack = ((2 * pokemon->data->baseAttack * pokemon->level) / 100) + 5;
        pokemon->defense = ((2 * pokemon->data->baseDefense * pokemon->level) / 100) + 5;
        pokemon->speed = ((2 * pokemon->data->baseSpeed * pokemon->level) / 100) + 5;
        pokemon->special = ((2 * pokemon->data->baseSpecial * pokemon->level) / 100) + 5;
        
        pokemon->currentHP = pokemon->maxHP; // Curar ao subir de nível
        
        setColor(10);
        printf("\n╔══════════════════════════════════╗\n");
        printf("║ ");
        printf("%s grew to level %d!", pokemon->data->name, pokemon->level);
        printf(" ║\n");
        printf("╚══════════════════════════════════╝\n");
        setColor(7);
        Sleep(2000);
        return 1;
    }
    return 0;
}

// Sistema de mapas melhorado
#define GRASS 0
#define PATH 1
#define TREE 2
#define WATER 3
#define BUILDING 4
#define HOUSE 5
#define LAB 6
#define POKECENTER 7

int maps[1][MAP_HEIGHT][MAP_WIDTH] = {
    { // Pallet Town melhorado
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,2},
        {2,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,2},
        {2,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,7,7,7,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2},
        {2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2}
    }
};

void initMaps() {
    // Já inicializado acima
}

void displayMap(Player *player) {
    system("cls");
    
    // Header do mapa
    setColor(11);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                  ");
    setColor(14);
    printf("PALLET TOWN");
    setColor(11);
    printf("                  ║\n");
    printf("╠══════════════════════════════════════════════╣\n");
    setColor(7);
    printf("║ Position: (%2d, %2d)                            ║\n", player->positionX, player->positionY);
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    for(int y = 0; y < MAP_HEIGHT; y++) {
        printf("  ");
        for(int x = 0; x < MAP_WIDTH; x++) {
            if(x == player->positionX && y == player->positionY) {
                setColor(12);
                printf("☻");
                setColor(7);
                continue;
            }
            
            switch(maps[player->mapId][y][x]) {
                case GRASS:
                    setColor(10);
                    printf("♣");
                    break;
                case PATH:
                    setColor(8);
                    printf("·");
                    break;
                case TREE:
                    setColor(2);
                    printf("♠");
                    break;
                case WATER:
                    setColor(9);
                    printf("≈");
                    break;
                case BUILDING:
                    setColor(6);
                    printf("█");
                    break;
                case HOUSE:
                    setColor(14);
                    printf("▓");
                    break;
                case LAB:
                    setColor(11);
                    printf("█");
                    break;
                case POKECENTER:
                    setColor(13);
                    printf("╬");
                    break;
                default:
                    printf(" ");
            }
        }
        printf("\n");
    }
    
    setColor(15);
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║ ");
    setColor(10);
    printf("W/A/S/D");
    setColor(15);
    printf(": Move  ");
    setColor(12);
    printf("M");
    setColor(15);
    printf(": Menu  ");
    setColor(11);
    printf("P");
    setColor(15);
    printf(": Pokemon  ");
    setColor(14);
    printf("Q");
    setColor(15);
    printf(": Quit    ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
}

void movePlayer(Player *player, int dx, int dy) {
    int newX = player->positionX + dx;
    int newY = player->positionY + dy;
    
    // Check boundaries
    if(newX < 0 || newX >= MAP_WIDTH || newY < 0 || newY >= MAP_HEIGHT) {
        return;
    }
    
    // Check if tile is walkable
    int targetTile = maps[player->mapId][newY][newX];
    if(targetTile == TREE || targetTile == WATER) {
        setColor(12);
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║           You can't go that way!             ║\n");
        printf("╚══════════════════════════════════════════════╝\n");
        Sleep(1000);
        return;
    }
    
    player->positionX = newX;
    player->positionY = newY;
}

int checkWildEncounter(Player *player) {
    int currentTile = maps[player->mapId][player->positionY][player->positionX];
    
    // Only encounter in grass
    if(currentTile != GRASS) {
        return 0;
    }
    
    // 15% chance of encounter
    if(rand() % 100 < 15) {
        // Pokémon selvagens em Pallet Town
        int possiblePokemon[] = {16, 19, 10}; // Pidgey, Rattata, Caterpie
        return possiblePokemon[rand() % 3];
    }
    
    return 0;
}

// Sistema de batalha melhorado
void drawHealthBar(int current, int max, int width) {
    int bars = (int)((float)current / max * width);
    printf("[");
    for(int i = 0; i < width; i++) {
        if(i < bars) {
            if((float)current / max > 0.5) setColor(10);
            else if((float)current / max > 0.25) setColor(14);
            else setColor(12);
            printf("█");
        } else {
            setColor(8);
            printf("░");
        }
    }
    setColor(7);
    printf("] %3d/%-3d", current, max);
}

float typeEffectiveness(PokemonType attack, PokemonType defense) {
    // Eficácia básica
    if(attack == FIRE && defense == GRASS) return 2.0;
    if(attack == WATER && defense == FIRE) return 2.0;
    if(attack == GRASS && defense == WATER) return 2.0;
    if(attack == FIRE && defense == WATER) return 0.5;
    if(attack == WATER && defense == GRASS) return 0.5;
    if(attack == GRASS && defense == FIRE) return 0.5;
    return 1.0;
}

int calculateDamage(Pokemon *attacker, Pokemon *defender, Move *move) {
    int level = attacker->level;
    int attack = attacker->attack;
    int defense = defender->defense;
    int power = move->power;
    
    float modifier = 1.0;
    float effectiveness = typeEffectiveness(move->type, defender->data->type1);
    modifier *= effectiveness;
    
    // STAB
    if(move->type == attacker->data->type1) {
        modifier *= 1.5;
    }
    
    // Random factor
    modifier *= (0.85 + (rand() % 16) / 100.0);
    
    int damage = (((2 * level / 5 + 2) * power * attack / defense) / 50) + 2;
    damage *= modifier;
    
    if(effectiveness >= 2.0) {
        setColor(10);
        printf("It's super effective!\n");
    } else if(effectiveness <= 0.5) {
        setColor(12);
        printf("It's not very effective...\n");
    }
    
    setColor(7);
    return (damage < 1) ? 1 : damage;
}

void displayBattle(Pokemon *player, Pokemon *enemy) {
    system("cls");
    
    // Header da batalha
    setColor(12);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                ");
    setColor(14);
    printf("POKéMON BATTLE");
    setColor(12);
    printf("                ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    // Enemy Pokémon
    setColor(12);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ ENEMY: ");
    setColor(15);
    printf("%-15s", enemy->data->name);
    setColor(12);
    printf(" Lv.%-3d                   ║\n", enemy->level);
    printf("║ HP: ");
    drawHealthBar(enemy->currentHP, enemy->maxHP, 30);
    printf(" ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n\n");
    
    // Player Pokémon  
    setColor(10);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ YOUR: ");
    setColor(15);
    printf("%-15s", player->data->name);
    setColor(10);
    printf(" Lv.%-3d                   ║\n", player->level);
    printf("║ HP: ");
    drawHealthBar(player->currentHP, player->maxHP, 30);
    printf(" ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
}

int battle(Player *player, Pokemon *wildPokemon) {
    Pokemon *currentPokemon = &player->party[0];
    int battleOver = 0;
    int ranAway = 0;
    
    while(!battleOver) {
        displayBattle(currentPokemon, wildPokemon);
        
        setColor(14);
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║ What will ");
        setColor(10);
        printf("%s", currentPokemon->data->name);
        setColor(14);
        printf(" do?                  ║\n");
        printf("╠══════════════════════════════════════════════╣\n");
        printf("║ ");
        setColor(11);
        printf("1. Fight");
        setColor(14);
        printf("     ");
        setColor(12);
        printf("2. Pokemon");
        setColor(14);
        printf("                       ║\n");
        printf("║ ");
        setColor(13);
        printf("3. Bag");
        setColor(14);
        printf("       ");
        setColor(10);
        printf("4. Run");
        setColor(14);
        printf("                          ║\n");
        printf("╚══════════════════════════════════════════════╝\n");
        printf("Choice: ");
        
        int choice;
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: { // Fight
                printf("\n╔══════════════════════════════════════════════╗\n");
                printf("║               Choose a move:                 ║\n");
                printf("╠══════════════════════════════════════════════╣\n");
                for(int i = 0; i < currentPokemon->movesCount; i++) {
                    printf("║ ");
                    setColor(11);
                    printf("%d. %-12s", i+1, currentPokemon->moves[i].name);
                    setColor(7);
                    printf(" PP %2d/%-2d                  ║\n",
                           currentPokemon->moves[i].pp,
                           currentPokemon->moves[i].maxPP);
                }
                printf("║ ");
                setColor(8);
                printf("5. Back");
                setColor(7);
                printf("                                    ║\n");
                printf("╚══════════════════════════════════════════════╝\n");
                printf("Move: ");
                
                int moveChoice;
                scanf("%d", &moveChoice);
                moveChoice--;
                
                if(moveChoice == 4) continue;
                if(moveChoice < 0 || moveChoice >= currentPokemon->movesCount) {
                    printf("Invalid move!\n");
                    Sleep(1000);
                    continue;
                }
                
                Move *selectedMove = &currentPokemon->moves[moveChoice];
                
                // Player attacks
                setColor(10);
                printf("\n╔══════════════════════════════════════════════╗\n");
                printf("║ ");
                printf("%s used %s!", currentPokemon->data->name, selectedMove->name);
                printf("                 ║\n");
                printf("╚══════════════════════════════════════════════╝\n");
                
                int damage = calculateDamage(currentPokemon, wildPokemon, selectedMove);
                wildPokemon->currentHP -= damage;
                if(wildPokemon->currentHP < 0) wildPokemon->currentHP = 0;
                
                Sleep(2000);
                
                if(wildPokemon->currentHP <= 0) {
                    setColor(12);
                    printf("\n╔══════════════════════════════════════════════╗\n");
                    printf("║ Enemy %s fainted!                     ║\n", wildPokemon->data->name);
                    printf("╚══════════════════════════════════════════════╝\n");
                    battleOver = 1;
                    break;
                }
                
                // Enemy attacks
                setColor(12);
                int enemyMove = rand() % wildPokemon->movesCount;
                printf("\n╔══════════════════════════════════════════════╗\n");
                printf("║ Enemy %s used %s!              ║\n", wildPokemon->data->name, wildPokemon->moves[enemyMove].name);
                printf("╚══════════════════════════════════════════════╝\n");
                
                damage = calculateDamage(wildPokemon, currentPokemon, &wildPokemon->moves[enemyMove]);
                currentPokemon->currentHP -= damage;
                if(currentPokemon->currentHP < 0) currentPokemon->currentHP = 0;
                
                Sleep(2000);
                
                if(currentPokemon->currentHP <= 0) {
                    setColor(12);
                    printf("\n╔══════════════════════════════════════════════╗\n");
                    printf("║ %s fainted!                          ║\n", currentPokemon->data->name);
                    printf("╚══════════════════════════════════════════════╝\n");
                    battleOver = 1;
                }
                break;
            }
            case 4: // Run
                if(rand() % 2) {
                    setColor(14);
                    printf("\n╔══════════════════════════════════════════════╗\n");
                    printf("║           Got away safely!                   ║\n");
                    printf("╚══════════════════════════════════════════════╝\n");
                    ranAway = 1;
                    battleOver = 1;
                } else {
                    setColor(12);
                    printf("\n╔══════════════════════════════════════════════╗\n");
                    printf("║             Can't escape!                    ║\n");
                    printf("╚══════════════════════════════════════════════╝\n");
                    Sleep(1000);
                }
                break;
            default:
                printf("Not implemented yet!\n");
                Sleep(1000);
                break;
        }
    }
    
    return ranAway ? -1 : (currentPokemon->currentHP > 0 ? 1 : 0);
}

// Sistema de captura melhorado
void captureSequence(Player *player, Pokemon *wildPokemon) {
    if(player->items[0] <= 0) {
        setColor(12);
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║       You don't have any Poké Balls!        ║\n");
        printf("╚══════════════════════════════════════════════╝\n");
        Sleep(2000);
        return;
    }
    
    player->items[0]--;
    
    setColor(11);
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║           You used a Poké Ball!             ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    
    // Animação melhorada
    printf("\n");
    for(int i = 0; i < 3; i++) {
        printf("    ");
        for(int j = 0; j < 10; j++) {
            setColor(11 + i);
            printf("★");
            Sleep(50);
        }
        printf("\n");
        Sleep(300);
    }
    
    // Chance de captura baseada na vida restante
    float captureChance = (float)wildPokemon->currentHP / wildPokemon->maxHP;
    if(rand() % 100 < (100 - (captureChance * 100)) / 2) {
        setColor(10);
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║        Gotcha! %s was caught!           ║\n", wildPokemon->data->name);
        printf("╚══════════════════════════════════════════════╝\n");
        
        if(player->partySize < MAX_POKEMON_PARTY) {
            player->party[player->partySize] = *wildPokemon;
            player->partySize++;
            printf("\n╔══════════════════════════════════════════════╗\n");
            printf("║   %s was added to your party!      ║\n", wildPokemon->data->name);
            printf("╚══════════════════════════════════════════════╝\n");
        } else {
            player->storage[player->storageSize] = *wildPokemon;
            player->storageSize++;
            printf("\n╔══════════════════════════════════════════════╗\n");
            printf("║   %s was sent to PC storage!       ║\n", wildPokemon->data->name);
            printf("╚══════════════════════════════════════════════╝\n");
        }
    } else {
        setColor(12);
        printf("\n╔══════════════════════════════════════════════╗\n");
        printf("║      Oh no! The Pokémon broke free!         ║\n");
        printf("╚══════════════════════════════════════════════╝\n");
    }
    Sleep(2000);
}

// Funções do menu melhoradas
void displayPokemonMenu(Player *player) {
    system("cls");
    setColor(11);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║              YOUR POKéMON PARTY             ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    for(int i = 0; i < player->partySize; i++) {
        setColor(14);
        printf("╔══════════════════════════════════════════════╗\n");
        printf("║ #%-2d ", i+1);
        setColor(10);
        printf("%-15s", player->party[i].data->name);
        setColor(14);
        printf(" Lv.%-3d", player->party[i].level);
        printf("                  ║\n");
        
        printf("║ HP: ");
        drawHealthBar(player->party[i].currentHP, player->party[i].maxHP, 30);
        printf(" ║\n");
        
        if(player->party[i].level < 100) {
            printf("║ EXP: %4d/%-4d", player->party[i].experience, player->party[i].expToNextLevel);
            printf("                         ║\n");
        }
        printf("╚══════════════════════════════════════════════╝\n\n");
    }
    
    setColor(7);
    printf("Press any key to continue...");
    getch();
}

void displayBagMenu(Player *player) {
    char *itemNames[] = {
        "Poké Ball", "Great Ball", "Ultra Ball", "Potion", 
        "Super Potion", "Hyper Potion", "Revive", "Ether"
    };
    
    system("cls");
    setColor(13);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                    BAG                       ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    for(int i = 0; i < 8; i++) {
        setColor(11);
        printf("║ ");
        setColor(15);
        printf("%-15s", itemNames[i]);
        setColor(11);
        printf(": %-3d", player->items[i]);
        printf("                           ║\n");
    }
    printf("╚══════════════════════════════════════════════╝\n");
    
    setColor(7);
    printf("\nPress any key to continue...");
    getch();
}

void displayPlayerInfo(Player *player) {
    system("cls");
    setColor(14);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║               PLAYER INFO                   ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    setColor(11);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ Name:   %-20s           ║\n", player->name);
    printf("║ Money:  $%-20d           ║\n", player->money);
    printf("║ Badges: %-20d           ║\n", player->badges);
    printf("║ Pokémon:%-20d           ║\n", player->partySize);
    printf("╚══════════════════════════════════════════════╝\n");
    
    setColor(7);
    printf("\nPress any key to continue...");
    getch();
}

void saveGame(Player *player) {
    setColor(10);
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║        Save feature not implemented yet!     ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    Sleep(2000);
}

void loadGame(Player *player) {
    setColor(11);
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║        Load feature not implemented yet!     ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    Sleep(2000);
}

void gameMenu(Player *player) {
    int choice;
    do {
        system("cls");
        setColor(14);
        printf("╔══════════════════════════════════════════════╗\n");
        printf("║               POKéMON MENU                  ║\n");
        printf("╚══════════════════════════════════════════════╝\n\n");
        
        setColor(15);
        printf("╔══════════════════════════════════════════════╗\n");
        printf("║ ");
        setColor(11);
        printf("1. Pokémon");
        setColor(15);
        printf("                                  ║\n");
        printf("║ ");
        setColor(12);
        printf("2. Bag");
        setColor(15);
        printf("                                      ║\n");
        printf("║ ");
        setColor(13);
        printf("3. Player Info");
        setColor(15);
        printf("                              ║\n");
        printf("║ ");
        setColor(10);
        printf("4. Save Game");
        setColor(15);
        printf("                                ║\n");
        printf("║ ");
        setColor(9);
        printf("5. Load Game");
        setColor(15);
        printf("                                ║\n");
        printf("║ ");
        setColor(8);
        printf("6. Return to Game");
        setColor(15);
        printf("                          ║\n");
        printf("╚══════════════════════════════════════════════╝\n");
        printf("Choice: ");
        
        scanf("%d", &choice);
        
        switch(choice) {
            case 1:
                displayPokemonMenu(player);
                break;
            case 2:
                displayBagMenu(player);
                break;
            case 3:
                displayPlayerInfo(player);
                break;
            case 4:
                saveGame(player);
                break;
            case 5:
                loadGame(player);
                break;
        }
    } while(choice != 6);
}

// Telas do jogo melhoradas
void showTitleScreen() {
    system("cls");
    
    // Arte ASCII melhorada
    setColor(12);
    printf("\n\n");
    printf("██████╗  ██████╗ ██╗  ██╗███████╗███╗   ███╗ ██████╗ ███╗   ██╗\n");
    printf("██╔══██╗██╔═══██╗██║ ██╔╝██╔════╝████╗ ████║██╔═══██╗████╗  ██║\n");
    printf("██████╔╝██║   ██║█████╔╝ █████╗  ██╔████╔██║██║   ██║██╔██╗ ██║\n");
    printf("██╔═══╝ ██║   ██║██╔═██╗ ██╔══╝  ██║╚██╔╝██║██║   ██║██║╚██╗██║\n");
    printf("██║     ╚██████╔╝██║  ██╗███████╗██║ ╚═╝ ██║╚██████╔╝██║ ╚████║\n");
    printf("╚═╝      ╚═════╝ ╚═╝  ╚═╝╚══════╝╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝\n");
    
    setColor(10);
    printf("\n");
    printf("╔══════════════════════════════════════════════════════════════╗\n");
    printf("║                     RED Version - CMD Edition                ║\n");
    printf("╚══════════════════════════════════════════════════════════════╝\n");
    
    setColor(14);
    printf("\n\n            Press any key to continue...");
    getch();
}

int mainMenu() {
    system("cls");
    setColor(14);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║                 POKéMON RED                 ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    setColor(15);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ ");
    setColor(11);
    printf("1. New Game");
    setColor(15);
    printf("                                  ║\n");
    printf("║ ");
    setColor(12);
    printf("2. Load Game");
    setColor(15);
    printf("                                 ║\n");
    printf("║ ");
    setColor(13);
    printf("3. Options");
    setColor(15);
    printf("                                  ║\n");
    printf("║ ");
    setColor(8);
    printf("4. Exit");
    setColor(15);
    printf("                                      ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    printf("\nChoice: ");
    
    int choice;
    scanf("%d", &choice);
    return choice;
}

void starterSelection(Player *player) {
    system("cls");
    setColor(14);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║              PROFESSOR OAK                  ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    printf("Hello there! Welcome to the world of POKéMON!\n\n");
    printf("This world is inhabited by creatures called POKéMON!\n\n");
    printf("For some people, POKéMON are pets. Others use them for fights.\n\n");
    printf("Myself... I study POKéMON as a profession.\n\n");
    Sleep(4000);
    
    printf("First, what is your name?\n\n");
    printf("Name: ");
    scanf("%s", player->name);
    
    printf("\nRight! So your name is %s!\n\n", player->name);
    Sleep(2000);
    
    printf("%s! Your very own POKéMON legend is about to unfold!\n\n", player->name);
    printf("A world of dreams and adventures with POKéMON awaits! Let's go!\n");
    Sleep(4000);
    
    system("cls");
    printf("Choose your starter Pokémon:\n\n");
    
    setColor(10);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ 1. Bulbasaur - The Grass/Poison type         ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    setColor(12);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ 2. Charmander - The Fire type                ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    setColor(11);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║ 3. Squirtle - The Water type                 ║\n");
    printf("╚══════════════════════════════════════════════╝\n\n");
    
    setColor(15);
    printf("Choice: ");
    
    int choice;
    scanf("%d", &choice);
    
    Pokemon starter;
    switch(choice) {
        case 1:
            starter = createPokemon(1, 5); // Bulbasaur
            break;
        case 2:
            starter = createPokemon(4, 5); // Charmander
            break;
        case 3:
            starter = createPokemon(7, 5); // Squirtle
            break;
        default:
            starter = createPokemon(1, 5); // Default Bulbasaur
    }
    
    player->party[0] = starter;
    player->partySize = 1;
    player->money = 3000;
    player->badges = 0;
    player->storageSize = 0;
    // Initialize items
    for(int i = 0; i < 20; i++) player->items[i] = 0;
    player->items[0] = 5; // 5 Poké Balls
    
    system("cls");
    setColor(14);
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║           You chose %s!                  ║\n", starter.data->name);
    printf("║                                              ║\n");
    printf("║     Take good care of your new POKéMON!      ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    Sleep(3000);
}

// Função principal
int main() {
    // Ajustes para codificação UTF-8 no console (corrige símbolos loucos)
    setlocale(LC_ALL, "pt_BR.UTF-8");  // Locale para PT-BR com UTF-8
    SetConsoleOutputCP(65001);  // Codepage UTF-8 para saída
    SetConsoleCP(65001);  // Codepage UTF-8 para entrada

    // Alternativa: Se UTF-8 não funcionar, use CP437 (comente as linhas acima e descomente abaixo)
    // SetConsoleOutputCP(437);
    // SetConsoleCP(437);

    srand(time(NULL));
    
    showTitleScreen();
    
    Player player;
    int gameRunning = 1;
    
    // Initialize game world
    initMaps();
    
    while(gameRunning) {
        int menuChoice = mainMenu();
        
        switch(menuChoice) {
            case 1: { // New Game
                starterSelection(&player);
                
                // Initialize player position
                player.positionX = 20;
                player.positionY = 10;
                player.mapId = 0;
                
                // Start first battle (Rival)
                Pokemon rivalPokemon;
                if(strcmp(player.party[0].data->name, "Bulbasaur") == 0)
                    rivalPokemon = createPokemon(4, 5); // Charmander
                else if(strcmp(player.party[0].data->name, "Charmander") == 0)
                    rivalPokemon = createPokemon(7, 5); // Squirtle
                else
                    rivalPokemon = createPokemon(1, 5); // Bulbasaur
                
                system("cls");
                setColor(12);
                printf("╔══════════════════════════════════════════════╗\n");
                printf("║           Rival Gary wants to fight!         ║\n");
                printf("╚══════════════════════════════════════════════╝\n");
                Sleep(2000);
                
                int result = battle(&player, &rivalPokemon);
                
                if(result == 1) {
                    setColor(10);
                    printf("\n╔══════════════════════════════════════════════╗\n");
                    printf("║             You defeated Gary!               ║\n");
                    printf("╚══════════════════════════════════════════════╝\n");
                    // Give experience
                    gainExperience(&player.party[0], rivalPokemon.data->experienceYield);
                } else {
                    setColor(12);
                    printf("\n╔══════════════════════════════════════════════╗\n");
                    printf("║              You lost to Gary...             ║\n");
                    printf("╚══════════════════════════════════════════════╝\n");
                }
                Sleep(3000);
                
                // Start game loop
                int inGame = 1;
                while(inGame) {
                    displayMap(&player);
                    
                    // Check for wild encounters
                    int wildPokemonId = checkWildEncounter(&player);
                    if(wildPokemonId > 0) {
                        Pokemon wildPokemon = createPokemon(wildPokemonId, 3 + rand() % 3);
                        setColor(14);
                        printf("\n╔══════════════════════════════════════════════╗\n");
                        printf("║         A wild %s appeared!            ║\n", wildPokemon.data->name);
                        printf("╚══════════════════════════════════════════════╝\n");
                        Sleep(2000);
                        
                        // Give option to battle or capture
                        printf("\n╔══════════════════════════════════════════════╗\n");
                        printf("║ ");
                        setColor(11);
                        printf("1. Battle");
                        setColor(15);
                        printf("  ");
                        setColor(12);
                        printf("2. Capture");
                        setColor(15);
                        printf("  ");
                        setColor(8);
                        printf("3. Run");
                        setColor(15);
                        printf("             ║\n");
                        printf("╚══════════════════════════════════════════════╝\n");
                        printf("Choice: ");
                        int encounterChoice;
                        scanf("%d", &encounterChoice);
                        
                        if(encounterChoice == 1) {
                            result = battle(&player, &wildPokemon);
                            if(result == 1) {
                                // Gain experience
                                gainExperience(&player.party[0], wildPokemon.data->experienceYield);
                            }
                        } else if(encounterChoice == 2) {
                            captureSequence(&player, &wildPokemon);
                        } else {
                            if(rand() % 2) {
                                setColor(14);
                                printf("\n╔══════════════════════════════════════════════╗\n");
                                printf("║             Got away safely!                 ║\n");
                                printf("╚══════════════════════════════════════════════╝\n");
                            } else {
                                setColor(12);
                                printf("\n╔══════════════════════════════════════════════╗\n");
                                printf("║              Couldn't escape!                ║\n");
                                printf("╚══════════════════════════════════════════════╝\n");
                                result = battle(&player, &wildPokemon);
                                if(result == 1) {
                                    gainExperience(&player.party[0], wildPokemon.data->experienceYield);
                                }
                            }
                        }
                    }
                    
                    // Get input
                    char input;
                    printf("\nEnter command: ");
                    scanf(" %c", &input);
                    
                    switch(input) {
                        case 'w': case 'W':
                            movePlayer(&player, 0, -1);
                            break;
                        case 's': case 'S':
                            movePlayer(&player, 0, 1);
                            break;
                        case 'a': case 'A':
                            movePlayer(&player, -1, 0);
                            break;
                        case 'd': case 'D':
                            movePlayer(&player, 1, 0);
                            break;
                        case 'm': case 'M':
                            gameMenu(&player);
                            break;
                        case 'q': case 'Q':
                            inGame = 0;
                            break;
                    }
                }
                break;
            }
            case 4:
                gameRunning = 0;
                break;
            default:
                printf("Feature not implemented yet!\n");
                Sleep(1000);
                break;
        }
    }
    
    setColor(7);
    printf("\n╔══════════════════════════════════════════════╗\n");
    printf("║       Thanks for playing Pokemon Red!       ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    return 0;
}