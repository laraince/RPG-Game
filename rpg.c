#include <stdio.h>
#include <stdbool.h> // for "true" and "false" variables
#include <time.h> // for random numbers, srand
#include <stdlib.h> // for rand() 
#include <string.h> // to read the files name

//********************************* PROTOTYPES ******************************
// To avoid the "undeclared function" error shown in your terminal
typedef struct Hero Hero;
bool padovan(int n);
void dungeon_menu(Hero *Player, int room);
bool combat(Hero *Player, char *enemyName, int difficulty, int dmg, int reward);
void death_screen(Hero *Player);
void rotting_swamp(Hero *Player);
void haunted_mansion(Hero *Player);
void crystal_cave(Hero *Player);
void final_battle(Hero *Player);
void shop(Hero *Player);
void cheat_menu(Hero *Player);
void mission_menu(Hero *Player);
void village_menu(Hero *Player);
int save_game(Hero *Player);
int load_game(Hero *Player);
void story();

//******************************* HERO **************************************
/**
 * @brief Struct representing the Hero and what they have.
 * Contains all stats, mission progress and items.
 */
typedef struct Hero {
    int LifePoints;
    bool sword;       // player could buy this
    bool hero_sword;  // sword from dragon in crystal caves
    bool armor;       // player could buy this
    int healthPotion; // shop item
    int coins; 
    bool swamp_done;   // tracks if the swamp mission is finished
    bool mansion_done; // tracks if the mansion mission is finished
    bool cave_done;    // tracks if the cave mission is finished
    int Completed_missions;
} Hero;

//******************************* HELPERS **************************************

/**
 * @brief Prints the Player's stats for in between specific rooms
 */
void dungeon_menu(Hero *Player, int room) {
    printf("----------------------------------------\n");
    printf(" STATUS | HP: %d/20 | Potions: %d\n", Player->LifePoints, Player->healthPotion);
    printf("----------------------------------------\n");
    printf("You are at the entrance of Room %d.\n", room);
    printf("1. Enter Room %d \n", room);
    printf("2. Drink Health Potion \n");
    printf("3. View Stats & Inventory\n");
    printf("4. Retreat to Village (Cost: 50 Coins)\n");
    printf("Choose an action [1-4]: ");
}

/**
 * @brief Logic for the Dragon's puzzle in the Crystal Cave
 */
bool padovan(int n) {
    int p[] = {1, 1, 1, 2, 2, 3, 4, 5, 7, 9, 12, 16, 21, 28, 37, 49, 65, 86, 114, 151, 200, 265, 351, 465};
    int size = sizeof(p) / sizeof(p[0]); // calculate number of elements in array
    for (int i = 0; i < size; i++) // checks every element to find a match
    {
        if (p[i] == n) //checks if players inut matches
            return true;
    }
    return false; 
}

/**
 * @brief Handles combat logic
 */
bool combat(Hero *Player, char *enemyName, int difficulty, int dmg, int reward) {
    printf("\n--- BATTLE: %s (Difficulty: %d) ---\n", enemyName, difficulty);

    int roll = (rand() % 6) + 1; // Player rolls
    printf("You rolled a %d! \n", roll);
    printf("Press [enter] to attack!\n");
    getchar(); //waits for player
    
    // Bonuses
    if (Player->sword) {
        printf("Sword Bonus: +1\n"); // the one we buy
        roll += 1;
    }
    if (Player->hero_sword) {
        printf("Hero Sword Bonus: +2\n"); // the one we get from the crystal cave
        roll += 2;
    }

    printf("Total Attack: %d\n", roll);

    if (roll >= difficulty) { //If roll is higher or equal to the enemeys health they will perish
        printf(">>> VICTORY! You defeated the %s.\n", enemyName);
        if (reward > 0) {
            printf(">>> You found %d coins!\n", reward);
            Player->coins += reward;
        }
        return true;
    } else {
        printf(">>> ATTACK! The %s hits you.\n", enemyName);
        // Armor check
        if (Player->armor) {
            printf("Your Armor blocked 1 damage!\n");
            dmg -= 1;
            if (dmg < 0) dmg = 0;
        }
        Player->LifePoints -= dmg;
        printf("You took %d damage. Life: %d/20\n", dmg, Player->LifePoints);
        return false;
    }
}
/*
* @brief The Game Over screen
*/

void death_screen(Hero *Player) { //the death screen
    printf("\n--------------------------------\n");
    printf("          GAME OVER \n");
    printf("----------------------------------\n");
    printf("Your hero has fallen in battle.\n");
    printf("My your next adventure be more bountiful...\n");
    Player->LifePoints = 0;
    return;
}

//******************************* DUNGEONS **************************************
// ROTTING SWAMP **************************************
/*
* @brief The Rotting Swamp
* Goal: Defeat 3 Orc Generals
*/
void rotting_swamp(Hero *Player) {
    printf("\n--- Entering the Rotting Swamp ---\n");
    printf("Goal: Defeat 3 Orc Generals.\n");
    

    int generals_defeated = 0; //tracking the goal
    int action = 0; //players choice resests after each dungeon

    int room = 1;
    while (room <= 10) {
      // max 10 rooms are entered 
        if (Player->LifePoints <= 0) 
        return; //stop if dead
        
        // room menu
        dungeon_menu(Player, room); // show options
        
        if (scanf("%d", &action) != 1) { //if scnaf gets a letter (invalid), scanf fails and returns 0
            while(getchar() != '\n') {} 
            continue;
        }
        getchar(); 

        if (action == 2) { //Drink Potion
            if (Player->healthPotion > 0) {
                if (Player->LifePoints < 20) {
                    Player->healthPotion--;
                    int heal = (rand() % 6) + 1; // Roll 1-6
                    Player->LifePoints += heal;
                    if (Player->LifePoints > 20) Player->LifePoints = 20;
                    printf("\n>>> You drank a potion and rolled a %d! HP: %d/20\n", heal, Player->LifePoints);
                } else {
                    printf("\n>>> You are already at full health!\n");
                }
            } else {
                printf("\n>>> You have no potions!\n");
            }
            continue; // Loop back to menu
        }
        
        if (action == 3) {
            printf("\n--- STATUS ---\n");
            printf("Coins: %d\n", Player->coins); // 0= no , 1= yea
            printf("Sword: %d\n", Player->sword);
            printf("Armor: %d\n", Player->armor);
            printf("Generals Defeated: %d/3\n", generals_defeated);
            continue;
        }
        
        if (action == 4) { // Escape and easy clear mission
            if (Player->coins >= 50) {
                Player->coins -= 50;
                printf(">> You paid 50 coins and escaped safely.\n");

                Player->swamp_done = true;
                Player->Completed_missions++;

                return;
            } else {
                printf(">> Not enough coins! Get back into the battle.\n");
            }
            continue;
        }
        
        // If the action is not 1 the menu will repeat itself
        if (action != 1) continue;

        // Exploring the rooms
        printf("\n - Room %d - \n", room);

        // Instruction: "If the mission requirement has not been reached by chance before 10 rooms, these conditions must be FORCED."
        int generals_needed = 3 - generals_defeated; 
        int rooms_left = 10 - room + 1; //chances left
        int enemy_type; // we have 6 

        if (rooms_left <= generals_needed) { // forcing the orc's to appear
            printf("You see a General nearby... ");
            enemy_type = 6; 
        } else {
            enemy_type = (rand() % 6) + 1; // Random event If we have rooms left
        }

        switch (enemy_type) {
        case 1: 
            combat(Player, "Wild Dog", 2, 1, 0); // 2 = Diffuculty, 1 = Damage, 0 = coins
            break;
        case 2: 
            combat(Player, "Goblin", 3, 2, 2); 
            break;
        case 3: 
            combat(Player, "Skeleton", 4, 2, 4); 
            break;
        case 4: 
            combat(Player, "Orc Grunt", 3, 4, 6); 
            break;
        case 5: {
            printf("You stepped in a Poisonous Bog!\n"); // trap
            int trap_dmg = 2;
            if (Player->armor) trap_dmg -= 1; // Armor protects
            Player->LifePoints -= trap_dmg;
            printf("You took poison damage: -%d Health\n", trap_dmg);
            break;
        }
        case 6: // MISSION
            if (combat(Player, "ORC GENERAL", 5, 3, 12) == true) { // Name, Difficulty 5, Damage 3, Reward 12 coins
                generals_defeated++;
                printf(" -> GENERALS DEFEATED: %d\n", generals_defeated);
            }
            break;
        }

        // the winn Condition
        if (generals_defeated >= 3) { 
            printf("\n*** SWAMP CLEARED! You defeated 3 Generals! ***\n");
            Player->swamp_done = true;
            Player->Completed_missions++;
            return; // return to Village
        }

        // death Check
        if (Player->LifePoints <= 0) { 
            death_screen(Player); 
            return; 
        }
    
        room++; // Only increment room if we actually explored
    } // close loop
    printf("\n>> Mission Failed! You ran out of rooms. Returning to village...\n");
} //close swamp

// HAUNTED MANSION **************************************
/**
 * @brief The Haunted Mansion
 * Goal: Defeat the Vampire and find the Castle Key.
 */
void haunted_mansion(Hero *Player) {
    printf("\n--- Entering the Haunted Mansion ---\n");
    printf("Goal: Defeat the Vampire and find the Castle Key.\n");

    // We need to track two separate goals for this mission
    bool vampire_dead = false;
    bool key_found = false;
    int action = 0;

    int room = 1;
    while (room <= 10) {
      // max 10 rooms are entered 
        if (Player->LifePoints <= 0) return; // Stop if dead
        
        dungeon_menu(Player, room);
        
        // (same logic as swamp)
        if (scanf("%d", &action) != 1) { 
            while (getchar() != '\n') {} //clear input buffer (caused warnings)
            
            continue;
    }

        getchar(); 

        if (action == 2) { // drink potion
            if (Player->healthPotion > 0) {
                if (Player->LifePoints < 20) {
                    Player->healthPotion--;
                    int heal = (rand() % 6) + 1; // roll dice
                    Player->LifePoints += heal;
                    if (Player->LifePoints > 20) Player->LifePoints = 20;
                    printf("\n>>> Potion used! Rolled %d. HP: %d/20\n", heal, Player->LifePoints);
                } else printf("\n>>> Full health.\n");
            } else printf("\n>>> No potions.\n");
            continue;
        }
       if (action == 3) { 
            printf("\n--- STATUS ---\n");
            printf("Vampire: %s\n", vampire_dead ? "Dead" : "Alive");
            printf("Castle Key: %s\n", key_found ? "Found" : "Missing");
            continue; 
        }
        if (action == 4) { //the easy option
             if (Player->coins >= 50) {
                Player->coins -= 50;
                printf(">> You paid 50 coins and escaped safely.\n");

                Player->mansion_done = true; 
                Player->Completed_missions++; 
                return;
            }
            printf(">> Not enough coins! Get back into the battle.\n");
            continue;
        }
        if (action != 1) continue;

        // Exploring the rooms
        printf("\n - Room %d - \n", room);

        int goals_needed = 0;
        if (!vampire_dead) goals_needed++; //when the vampire is down, the goal is increased
        if (!key_found) goals_needed++; //when we find the key, the goal is increased 
        int rooms_left = 10 - room + 1;
        int enemy_type; 

        // forcing logic
        if (rooms_left <= goals_needed) {
            printf("(A dark presence locks the door...) ");
            if (!vampire_dead && !key_found) enemy_type = (rand() % 2 == 0) ? 5 : 6; 
            else if (!vampire_dead) enemy_type = 5; //vampire is case 5
            else enemy_type = 6; // we have 6 enemies
        } else {
            enemy_type = (rand() % 6) + 1; 
        }

        switch (enemy_type) {
        case 1: 
            printf("The floor collapses! Dark Hatch.\n"); // Trap
            int fall_dmg = 3;
            if (Player->armor) fall_dmg -= 1; //if the player has armor they will recieve less damage
            Player->LifePoints -= fall_dmg;
            printf("You take %d damage.\n", fall_dmg);
            break;
        case 2: 
            combat(Player, "Giant Bat", 2, 2, 1); // 2 = Diffuculty, 2 = Damage, 1 = coins
            break;
        case 3: 
            combat(Player, "Zombie", 3, 3, 3); 
            break;
        case 4: 
            combat(Player, "Ghost", 4, 2, 4); 
            break;
        case 5: // Mission goal 1
            if (!vampire_dead) { 
                if (combat(Player, "MASTER VAMPIRE", 5, 4, 10)) {
                    vampire_dead = true; // the gaols needed increases
                    printf(">>> VAMPIRE DEFEATED!\n");
                }
            } else printf("An empty coffin...\n"); //If we encountered the vampire before
            break;
        case 6: // Mission goal 2
            if (!key_found) {
                if (combat(Player, "KEY GUARDIAN", 5, 5, 10)) {
                    key_found = true; // the goals needed increases
                    printf(">>> YOU FOUND THE CASTLE KEY!\n");
                }
            } else printf("Just a pile of bones...\n"); //If we encountered the guardian before
            break;
        }

        // Win Condition
        if (vampire_dead && key_found) { //Is the vampire deead and is the key found?
             printf("\n*** MANSION CLEARED! ***\n");
             Player->mansion_done = true;
             Player->Completed_missions++;
             return; 
        }
        
        if (Player->LifePoints <= 0) { 
            death_screen(Player);
            return; 
        }
        room++;

    } //close loop
    printf("\n>> Mission Failed! You ran out of rooms. Returning to village...\n");
} //close haunted mansion

// CRYSTAL CAVE **************************************
/**
 * @brief Crystal Caves dungeon.
 * The goal is to recover their sword
 */
void crystal_cave(Hero *Player) {
    printf("\n--- Entering the Crystal Cave ---\n");
    printf("Goal: Recover the Hero’s sword.\n");

    bool dragon_found = false; 
    int action = 0;

    int room = 1;
    while (room <= 10) {
      // max 10 rooms are entered 
        if (Player->LifePoints <= 0) return; // Stop if dead 
        
        dungeon_menu(Player, room);

        // (same logic)
        if (scanf("%d", &action) != 1) { 
            while(getchar() != '\n') {}
            continue;
        }
        getchar(); 

        if (action == 2) { //potion
            if (Player->healthPotion > 0) {
                if (Player->LifePoints < 20) {
                    Player->healthPotion--;
                    int heal = (rand() % 6) + 1; // roll dice
                    Player->LifePoints += heal;
                    if (Player->LifePoints > 20) Player->LifePoints = 20;
                    printf("\n>>> Potion used! Rolled %d. HP: %d/20\n", heal, Player->LifePoints);
                } else printf("\n>>> Full health.\n");
            } else printf("\n>>> No potions.\n");
            continue;
        }
        if (action == 3) {
            printf("\n--- STATS ---\n");
            printf("Dragon: %s\n", dragon_found ? "Found" : "Not Found");
            continue;
        }
        if (action == 4) {
             if (Player->coins >= 50) {
                Player->coins -= 50;
                printf(">> You paid 50 coins and escaped safely.\n");

                Player->cave_done = true;
                Player->Completed_missions++;
                return;
            }
            printf(">> Not enough coins!\n");
            continue;
        }
        if (action != 1) continue;

        printf("\n - Room %d - \n", room); //display room

        int needed = 1 - (dragon_found ? 1 : 0); // 1 if dragon is found 0 if not
        int rooms_left = 10 - room + 1;
        int enemy_type;

        if (rooms_left <= needed) { // forcing logic, if dragon not found = 1 is equal to what we need
            printf("You sense ancient magic nearby...\n");
            enemy_type = 6; //triggers case 6 : the dragon itself
        } else {
            enemy_type = (rand() % 6) + 1; //if we have rooms left we have random generator
        }

        switch (enemy_type) {
        case 1: printf("Empty Room. Lucky...\n"); break;
        case 2: // Trap
            printf("Falling Crystals! 2 damage.\n");
            int c_dmg = 2; if (Player->armor) c_dmg -= 1;
            Player->LifePoints -= c_dmg;
            break;
        case 3: // Trap
            printf("Unsafe Bridge. Lost 3 coins.\n");
            Player->coins -= 3;
            if (Player->coins < 0) Player->coins = 0; 
            break;
        case 4:
            printf("Mysterious Chest. Flip a coin! (H/T): ");
            char flip; scanf(" %c", &flip);
            int luck = rand(); 
            if (luck % 2 == 0) {
                printf("Heads! 10 coins.\n"); Player->coins += 10;
            } else {
                printf("Tails! Trap (2 damage).\n");
                int t_dmg = 2; 
                if (Player->armor) t_dmg -= 1;
                Player->LifePoints -= t_dmg;
            }
            getchar(); 
            break;
        case 5: // Trap
            printf("Steep Cliff. You slip.\n");
            int roll = (rand() % 6) + 1;
            printf("Rolled a %d damage.\n", roll);
            int f_dmg = roll; 
            if(Player->armor) f_dmg -= 1; //player takes less damage thanks to the armor
            Player->LifePoints -= f_dmg;
            break;
        case 6: // MISSION TARGET
            printf("ANCIENT DRAGON'S LAIR!\n");
            dragon_found = true; //the dragon is found so It's challange starts
            bool correct = false;
            do {
                int challengeNum = (rand() % 465) + 1; //pick random num 1-465
                char answer[10];
                printf("Dragon asks: 'Is %d in Padovan sequence?' (Yes/No): ", challengeNum); //the riddle
                scanf("%9s", answer); //because char answer[10];
                bool is_p = padovan(challengeNum); //is this number in padovan?
                bool yes = (answer[0] == 'Y' || answer[0] == 'y');
                bool no = (answer[0] == 'N' || answer[0] == 'n');

                if ((is_p && yes) || (!is_p && no)) correct = true;

                if (correct) {
                    printf("Correct! 0 damage.\n");
                    printf("\n*** CRYSTAL CAVE CLEARED! ***\n");
                    printf("You now have the Hero’s Sword! (+2 Attack) \n");
                    Player->coins += 12;
                    Player->sword = true;
                    Player->hero_sword = true;
                    Player->cave_done = true;
                    Player->Completed_missions++;
                    return; 
                } else {
                    printf("WRONG! Fire breath (10 damage)!\n");
                    int d_dmg = 10; 
                    if(Player->armor) d_dmg -= 1;
                    Player->LifePoints -= d_dmg;
                    if(Player->LifePoints <= 0) correct = true; // Exit loop to die
                }
            } while (!correct);
            break;
        }

        if (Player->LifePoints <= 0) { 
            death_screen(Player); 
            return; 
        }
        
        room++;
    } //close loop
    printf("\n>> Mission Failed! You ran out of rooms. Returning to village...\n");
} //close crystal cave

//******************************* FINAL BOSS **************************************
/**
 * @brief The Final Boss Fight using Rock-Paper-Scissors logic.
 * 1 = Aggressive beats 3 =Magic
 * 2 = Defensive beats 1 = Aggressive
 * 3 = Magic beats 2 = Defensive
 */
void final_battle(Hero *Player) {
    printf("\n-------------------------------------------\n");
    printf("          THE DARK LORD'S CASTLE             \n");
    printf("---------------------------------------------\n");
    printf("The Dark Lord stands before you!\n");
    printf("He challenges you to a duel!\n");
    
    int boss_hp = 3; // You need to win 3 times to kill him
    
    while(boss_hp > 0 && Player->LifePoints > 0) {
        printf("\n-----------------------------------\n");
        printf("Dark Lord HP: %d | Hero HP: %d\n", boss_hp, Player->LifePoints);
        printf("Choose your combat stance:\n");
        printf("1. Aggressive Strike (Rock)\n");
        printf("2. Defensive Block (Paper)\n");
        printf("3. Magic Cast (Scissors)\n");
        printf("Choice [1-3]: ");
        
        int choice;
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n'); 
            continue;
        }
        getchar();
        
        if (choice < 1 || choice > 3) {
            printf("Invalid stance!\n");
            continue;
        }

        int boss_choice = (rand() % 3) + 1; // Boss chooses randomly 1-3
        
        char *moves[] = {"", "Aggressive", "Defensive", "Magic"};
        printf("\n>> You used %s! Dark Lord used %s!\n", moves[choice], moves[boss_choice]);
        
        // LOGIC: Rock Paper Scissors
        if (choice == boss_choice) {
            printf(">> CLASH! It's a draw. No damage dealt.\n");
        }
        // winning condition: 1 beats 3, 2 beats 1, 3 beats 2
        else if ((choice == 1 && boss_choice == 3) || 
                 (choice == 2 && boss_choice == 1) || 
                 (choice == 3 && boss_choice == 2)) {
            printf(">> HIT! You wounded the Dark Lord!\n");
            boss_hp--;
        } 
        
        else { // losing condition
            printf(">> OUCH! The Dark Lord's counter-attack hits you!\n");
            int dmg = 5;
            if (Player->armor) {
                printf("(Armor reduced damage by 1)\n");
                dmg--;
            }
            Player->LifePoints -= dmg;
        }
    }
    
    if (Player->LifePoints > 0) {
        printf("\n*********************************************\n");
        printf("       THE DARK LORD IS DEFEATED!            \n");
        printf("*********************************************\n");

        printf("\nYou return home as a hero....\n");
        printf("Peace at home, peace in the world\n");
        printf("\nCONGRATULATIONS! YOU WIN THE GAME!\n");
        printf("Press Enter to return to main menu...");
        getchar();
    } else {
        death_screen(Player);
    }
}
//*****************************************************************
/**
 * @brief Displays the shop interface where the player can buy items.
 *
 * Allows the user to spend coins on a Sword, Armor, or Health Potions.
 * Checks if the player has enough coins before making a purchase.
 */

void shop(Hero *Player) {
    printf("\n---------------- SHOP ----------------\n");
    printf("\nWelcome! What can I get for ya'?\n");
    printf("You have %d coins currently.\n", Player->coins);
    printf("----------------------------------------\n");
    printf(" ITEMS FOR SALE:\n");
    printf("1. Sword (+1 Attack) -> 5 Coins\n");
    printf("2. Armor (-1 Incoming Damage) -> 10 Coins\n");
    printf("3. Health Potion (Healing Item) -> 4 Coins\n");
    printf("4. Leave\n");
    printf("----------------------------------------\n");
    printf("Choose: ");
    int choice;
    scanf("%d", &choice);
    getchar();

    switch(choice) {
        case 1:
            if (Player->coins >= 5) {
                Player->coins -= 5;
                Player->sword = true; //change to the player has sword
                printf(">> Purchase successful! You equipped the Sword.\n");
            } else 
            printf("Need 5 coins.\n");
            break;
        case 2:
            if (Player->coins >= 10) {
                Player->coins -= 10;
                Player->armor = true; //change to the player has armor
                printf(">> Purchase successful! You equipped the Armor.\n");
            } else 
            printf("Need 10 coins.\n");
            break;
        case 3:
            if (Player->coins >= 4) {
                Player->coins -= 4;
                Player->healthPotion++;
                printf(">> Purchase successful! You put the Potion in your bag.\n");
            } else 
            printf("Need 4 coins.\n");
            break;
        case 4: 
            return;
    }
}
/**
 * @brief Displays the adventure selection screen.
 *
 * Shows available dungeons based on the player's progress.
 */
void mission_menu(Hero *Player) {
    printf("\n--- DUNGEONS ---\n");
    
    // Only show if NOT done
    if (!Player->swamp_done) printf("1. Rotting Swamp\n");
    if (!Player->mansion_done) printf("2. Haunted Mansion\n");
    if (!Player->cave_done) printf("3. Crystal Cave\n");
    
    // Final Battle: Only if all 3 are done
    if (Player->swamp_done && Player->mansion_done && Player->cave_done) {
        printf("4. Final Battle: The Dark Lord\n");
    }

    printf("0. Back\n");
    printf("Choice: \n");
    int choice;
    scanf("%d", &choice);
    getchar();

    if (choice == 0) return;

if (choice == 1 && !Player->swamp_done) {
    // If the swamp mission isnt done
    rotting_swamp(Player); // Enter the Rotting Swamp dungeon
}
else if (choice == 2 && !Player->mansion_done) {
    // If the mansion mission isnt done
    haunted_mansion(Player); 
}
else if (choice == 3 && !Player->cave_done) {
    // If crystal cave mission isnt done
    crystal_cave(Player);
}
else if (choice == 4 && Player->swamp_done && Player->mansion_done && Player->cave_done) {
    // If the player has completed all three missions
    final_battle(Player); // Enter the Final Battle
}
else { // any other choice is invalid or the mission is already completed
    printf("Invalid or Completed Mission.\n"); }
}
/**
 * @brief The main hub of the game (The Village).
 *
 * Runs a loop that allows the player to Rest (heal), Shop, go on the missions, or save the game.
 * Also constantly checks if the player is dead (if (Player->LifePoints <= 0)) to break the loop.
 */

void village_menu(Hero *Player) {
    int choice;
    do {
          if (Player->LifePoints <= 0) // condition check
            return; //if player has died they will be taken to the opening screen
        printf("\n--- VILLAGE ---\n");
        printf("1. Rest (5 Coins) [HP -> 20]\n");
        printf("2. Shop\n");
        printf("3. Go to Missions\n");
        printf("4. Save Game\n");
        printf("5. Exit to Menu\n");
        printf("Choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch(choice) {
            case 1: // REST
                if (Player->coins >= 5) {
                    Player->coins -= 5;
                    Player->LifePoints = 20;
                    printf("You rested at the inn. HP restored!\n");
                } else {
                    printf("Not enough coins!\n");
                }
                break;
            case 2: 
                shop(Player); 
                break;
            case 3: 
                mission_menu(Player);
                break;
            case 4: 
                save_game(Player);
                break;
            case 5: 
                printf("Returning to main menu...\n");
                // goes back to main then loops or exits
                return; 
        }
    } while (choice != 5);
}
//******************************* CHEATS **************************************
/**
 * @brief The secret menu only accessed with the konami code
 */
void cheat_menu(Hero *Player) {
    int choice;
    printf("---> Cheat Menu <---\n");
    
    // Load the file you want to change
    printf(">> Load the save file you want to edit.\n");
    if (load_game(Player) != 0) { // If there are no existing files
        printf(">> Error: Could not load file. Aborting.\n");
        return;
    }
    while(1) {
        printf("\nCurrent Data: \n");
        printf("HP: %d | Coins: %d | Boss Unlocked: %s\n", 
               Player->LifePoints, Player->coins, 
               (Player->Completed_missions >= 3) ? "YES" : "NO"); // checking conditons
        printf("-------------------------\n");
        printf("1. Edit Coin Amount\n");
        printf("2. Edit HP Amount\n");
        printf("3. Unlock Final Mission (Complete All)\n");
        printf("4. Save Changes & Exit\n");
        printf("5. Cancel (Exit without saving)\n");
        printf("Choose: ");
        
        if (scanf("%d", &choice) != 1) {
            while(getchar() != '\n') {} 
            continue;
        }
        getchar();

        if (choice == 1) {
            printf("Enter new coin amount: ");
            scanf("%d", &Player->coins);
            getchar();
            if (Player->coins < 0) Player->coins = 0; //just in case the coins fall to a negative number
        }
        else if (choice == 2) {
            printf("Enter new HP amount: ");
            scanf("%d", &Player->LifePoints);
            getchar();
        }
        else if (choice == 3) { 
            printf(">> Unlocking all missions...\n");
            Player->swamp_done = true;
            Player->mansion_done = true;
            Player->cave_done = true;
            Player->Completed_missions = 3; 
        }
        else if (choice == 4) {
            printf(">> Saving your hacked data...\n");
            save_game(Player); 
            printf(">> Success! Return to the main menu and Load Game to play.\n");
            return;
        }
        else if (choice == 5) {
            printf(">> Exiting without saving.\n");
            return;
        }
        else {
            printf("Invalid choice.\n");
        }
    }
} 


//******************************* SAVE & LOAD *******************************
/**
 * @brief Saves the current hero state to a file.
 * Asks the user which save slot (1-5) and writes the Hero's statistics
 */
int save_game(Hero *Player) {
    char filename[50]; // creates a string (max 50 chars long) to hold filename
    int saveNum; //which save slot?
    
    printf("Save Slot (1-5): "); //save slots -can be changed-
    if (scanf("%d", &saveNum) != 1) {
      return 1;
    }
    getchar();


    sprintf(filename, "Saves/save%d.txt", saveNum); //file path string
    FILE *f = fopen(filename, "w"); //opens the file for writing.
    if (!f) { printf("Error: Create 'Saves' folder first!\n"); //check if opened correctly
    return 1; 
    }

    time_t t = time(NULL); //gets current date and time
    struct tm tm = *localtime(&t); // tm : day month hour minute 

    fprintf(f, "%02d/%02d %02d:%02d\n", tm.tm_mday, tm.tm_mon + 1, tm.tm_hour, tm.tm_min);
    fprintf(f, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
            Player->LifePoints, Player->sword, Player->hero_sword, Player->armor, 
            Player->healthPotion, Player->coins, Player->Completed_missions,
            Player->swamp_done, Player->mansion_done, Player->cave_done); //heros stats
    
    fclose(f); //closes file to save changes
    printf("Saved to Slot %d.\n", saveNum);
    return 0;
}

int load_game(Hero *Player) {
    char filename[50];
    int saveNum; //slot
    char dateStr[50]; //saves date
    int data[10]; //saved stats

    printf("Check Slot (1-5): ");
    printf("Save Slot (1-5): ");
    if (scanf("%d", &saveNum) != 1) {
      return 1;
    }
    getchar();
    sprintf(filename, "Saves/save%d.txt", saveNum); //path to the save file

    FILE *f = fopen(filename, "r"); //opens the file
    if (!f) { 
        printf("Slot %d empty.\n", saveNum); 
        return 1; }

    fgets(dateStr, 50, f); 
    char buffer[200];
    if (fgets(buffer, 200, f)) {
        char *token = strtok(buffer, ","); // strtok to split by commas.
        for(int i=0; i<10 && token; i++) {
            data[i] = atoi(token);
            token = strtok(NULL, ",");
        }
    }
    fclose(f);

    //example preview
    printf("\n--- PREVIEW ---\n"); // prints preview header
    printf("Date: %s", dateStr); // print the date from save
    printf("HP: %d | Coins: %d\n", data[0], data[5]); // prints HP and Coins

    printf("1. Load\n");
    printf("2. Delete\n");
    printf("0. Cancel\n");
    printf("Choice: ");

    int c;
    scanf("%d", &c); // read user input
    getchar();                        
    
    if (c == 2) { remove(filename); printf("Deleted.\n"); return 1; }
    if (c != 1) return 1;

    Player->LifePoints = data[0];
    Player->sword = data[1];
    Player->hero_sword = data[2];
    Player->armor = data[3];
    Player->healthPotion = data[4];
    Player->coins = data[5];
    if (Player->coins < 0) Player->coins = 0; //coins shouldnt be less than 0
    Player->Completed_missions = data[6];
    Player->swamp_done = data[7];
    Player->mansion_done = data[8];
    Player->cave_done = data[9];
    return 0;
}
/**
 * @brief Prints the games story once a new game is created
 */
void story (void) {
    printf("Long ago, a now forgotten kingdom called Zeta, "
        "was attacked by an fearsome enemy called The Dark Lord.\n");
    printf("Warriors were sought from all corners of the realm.\n");
    printf("Among the strongest, bravest, and wisest, the village chief chose you.\n");
    printf("For only one worthy could stand against the coming darkness.\n");
    printf("It's time to prove yourself, Knight. Your people need you.\n");

    printf("Press anything to begin the game.\n");
    getchar(); // removes the '\n' left by scanf
}
//******************************* MAIN **************************************
/**
 * @brief The main entry point of the program.
 * Initializes the random number generator and runs the infinite Title Screen loop.
 */
int main(void) {

    srand(time(NULL));
    Hero Player;
    Hero TempPlayer; 

    // Konami Code: wwssadadba-sapce- like es1
    char code[] = {'w','w','s','s','a','d','a','d','b','a',' '};
    int codeIndex = 0;
    int choice = 0;
    char c;

 while (true) {
    choice = 0; // Reset choice every time we come back to menu

    printf("\n*** THE CHOSEN ONE ***\n");
    printf("1. New Game\n");
    printf("2. Load Game\n");
    printf("Choose: ");

    // Input Loop
    while (choice == 0) {
        c = getchar(); 
        if (c == '\n') 
            continue;
        
        // Secret Code Checker
        if (c == code[codeIndex]) codeIndex++;
        else { codeIndex = 0; if (c == code[0]) codeIndex = 1; }

        if (codeIndex == 11) { choice = 3; break; } // Unlock Cheats

        // Menu Selection
        if (c == '1') { choice = 1; break; }
        if (c == '2') { choice = 2; break; }
    }

    switch(choice) {
        case 1: // New Game, set the stats
            Player.LifePoints = 20;
            Player.sword = 0; 
            Player.hero_sword = 0; 
            Player.armor = 0;
            Player.coins = 0; 
            Player.healthPotion = 0; 
            Player.Completed_missions = 0;
            Player.swamp_done = 0; 
            Player.mansion_done = 0; 

            Player.cave_done = 0;
            story();
            village_menu(&Player);
            break;

        case 2: // Load Game
            if (load_game(&Player) == 0) {
                 village_menu(&Player);
            } else {
                 printf("Load failed or cancelled.\n");
            }
            break;

        case 3: // Cheats
            printf("\n[CHEATS UNLOCKED]\n");
            TempPlayer.LifePoints = 20; 
            TempPlayer.coins = 0;
            cheat_menu(&TempPlayer);
            break;
    }  
 }
 return 0;
}
