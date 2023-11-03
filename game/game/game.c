#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_ENEMIES 5
#define MAX_ITEMS 3
#define MAX_WEAPONS 3
#define MAX_CLASSES 3
#define MAX_SKILLS 3
#define XP_NEEDED 100
#define MAX_LEVELS 3
#define MAX_NPCS 3
#define NPC_DIALOGUE_LENGTH 255



typedef struct {
    char name[50];
    int attackBoost;
    int healthBoost;
    int cost;
} Pet;

typedef struct {
    char description[255];
    int xpReward;
    int goldReward;
    int requiredLevel;
} Quest;

typedef struct {
    char name[50];
    char dialogue[NPC_DIALOGUE_LENGTH];
    int hasQuest;
    Quest quest;
} NPC;

NPC npcs[MAX_NPCS] = {
    {"Old Man", "Greetings, young traveler. Have you heard of the legendary dragon?", 1, {"Find the legendary dragon", 150, 100, 3}},
    {"Merchant", "Looking for some rare items? Maybe I can help you, for a price.", 0},
    {"Mysterious Stranger", "There are secrets in this world. Seek and you shall find.", 1, {"Discover the hidden temple", 200, 150, 2}}
};

Pet pets[3] = {
    {"Wolf", 10, 0, 100},
    {"Eagle", 5, 10, 80},
    {"Dragon", 15, 15, 150}
};

Quest quests[3] = {
    {"Defeat 5 goblins", 50, 30, 2},
    {"Buy a weapon from the shop", 30, 20, 1},
    {"Reach level 3", 100, 50, 1}
};

Pet playerPet = { "None", 0, 0, 0 };
int questCompleted[3] = { 0, 0, 0 };

typedef struct {
    char name[50];
    int health;
    int baseAttack;
} HeroClass;

HeroClass classes[MAX_CLASSES] = {
    {"Warrior", 120, 30},
    {"Mage", 80, 40},
    {"Archer", 100, 35}
};

typedef struct {
    char name[50];
    int damage;
    int cooldown;
} Skill;

Skill skills[MAX_SKILLS] = {
    {"Power Strike", 50, 3},
    {"Heal", -30, 4},
    {"Double Shot", 40, 2}
};

typedef struct {
    char name[50];
    int health;
    int attack;
    int itemDropRate;
    int xpReward;
    int goldReward;
} Character;

typedef struct {
    char name[50];
    int healthBoost;
    int price;
} Item;

typedef struct {
    char name[50];
    int attackBoost;
    int price;
} Weapon;

Character enemies[MAX_ENEMIES] = {
    {"Goblin", 50, 15, 30, 20, 10},
    {"Orc", 70, 20, 20, 40, 20},
    {"Dragon", 150, 30, 10, 100, 50},
    {"Skeleton", 60, 18, 25, 30, 15},
    {"Witch", 90, 25, 15, 50, 25}
};

Item items[MAX_ITEMS] = {
    {"Small Potion", 20, 5},
    {"Medium Potion", 40, 10},
    {"Large Potion", 60, 20}
};

Weapon weapons[MAX_WEAPONS] = {
    {"Sword", 10, 30},
    {"Bow", 15, 50},
    {"Staff", 20, 70}
};

int cooldowns[MAX_SKILLS] = { 0, 0, 0 };

void displayCharacter(Character c) {
    printf("Name: %s\n", c.name);
    printf("Health: %d\n", c.health);
    printf("Attack: %d\n", c.attack);
}

void displayItem(Item i) {
    printf("Item: %s (Price: %d)\n", i.name, i.price);
    printf("Health Boost: %d\n", i.healthBoost);
}

void displayWeapon(Weapon w) {
    printf("Weapon: %s (Price: %d)\n", w.name, w.price);
    printf("Attack Boost: %d\n", w.attackBoost);
}

void decreaseCooldowns() {
    for (int i = 0; i < MAX_SKILLS; i++) {
        if (cooldowns[i] > 0) {
            cooldowns[i]--;
        }
    }
}

void displayQuest(Quest q) {
    printf("Quest: %s\nXP Reward: %d\nGold Reward: %d\nRequired Level: %d\n", q.description, q.xpReward, q.goldReward, q.requiredLevel);
}

void completeQuest(int questIndex, int* playerXP, int* playerGold) {
    if (!questCompleted[questIndex]) {
        *playerXP += quests[questIndex].xpReward;
        *playerGold += quests[questIndex].goldReward;
        questCompleted[questIndex] = 1;
        printf("Quest completed!\n");
    }
}

int main() {
    char playerName[50];
    Character player;
    int enemyIndex, itemIndex, weaponIndex, classIndex, skillChoice, levelChoice;
    int choice;
    int level = 1;
    int xp = 0;
    int gold = 0;
    Item inventory[10] = { {"", 0, 0} };
    Weapon equippedWeapon = { "Fists", 0, 0 };
    int inventoryCount = 0;


    srand(time(NULL));

    printf("Welcome to Text RPG!\n");
    printf("Choose your class:\n");
    for (int i = 0; i < MAX_CLASSES; i++) {
        printf("%d. %s (Health: %d, Attack: %d)\n", i + 1, classes[i].name, classes[i].health, classes[i].baseAttack);
    }
    scanf("%d", &classIndex);
    getchar();

    if (classIndex > 0 && classIndex <= MAX_CLASSES) {
        player.health = classes[classIndex - 1].health;
        player.attack = classes[classIndex - 1].baseAttack;
    }
    else {
        printf("Invalid choice. Defaulting to Warrior.\n");
        player.health = classes[0].health;
        player.attack = classes[0].baseAttack;
    }

    printf("Enter your hero's name: ");
    fgets(playerName, sizeof(playerName), stdin);
    playerName[strcspn(playerName, "\n")] = '\0';
    strcpy(player.name, playerName);

    printf("\nHello, %s!\n", player.name);
    displayCharacter(player);

    while (player.health > 0) {

        printf("\nChoose an action:\n1. Explore Dungeon\n2. Shop\n3. View Inventory & Equip\n4. Heal Up (Costs 20 gold)\n5. Number of availible quests\n6. Buy a pet\n7. Talk to NPC\n");
        printf("\nYour current health: %d\n", player.health);
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Choose a dungeon level (difficulty increases):\n");
            for (int i = 1; i <= MAX_LEVELS; i++) {
                printf("%d. Level %d\n", i, i);
            }
            scanf("%d", &levelChoice);

            if (levelChoice <= 0 || levelChoice > MAX_LEVELS) {
                printf("Invalid choice. Returning to main menu.\n");
                continue;
            }

            enemyIndex = rand() % MAX_ENEMIES;
            Character enemy = enemies[enemyIndex];
            enemy.health *= levelChoice;
            enemy.attack *= levelChoice;
            printf("\nYou encounter: \n");
            displayCharacter(enemy);

            while (player.health > 0 && enemy.health > 0) {
                printf("\nChoose an action:\n1. Attack\n2. Run\n3. Use Item\n4. Use Skill\n\n");
                scanf("%d", &choice);

                switch (choice) {
                case 1:
                    enemy.health -= (player.attack + equippedWeapon.attackBoost);
                    printf("\n%s dealt %d damage to %s\n", player.name, player.attack + equippedWeapon.attackBoost, enemy.name);
                    if (enemy.health <= 0) {
                        printf("%s was defeated!\n", enemy.name);
                        xp += enemy.xpReward;
                        gold += enemy.goldReward;
                        printf("You earned %d gold. Total: %d\n", enemy.goldReward, gold);
                        if (xp >= XP_NEEDED) {
                            level++;
                            player.attack += 5;
                            player.health += 10;
                            xp -= XP_NEEDED;
                            printf("Level Up! Level: %d\n", level);
                        }
                        break;
                    }
                    player.health -= enemy.attack;
                    printf("%s dealt %d damage to %s\n", enemy.name, enemy.attack, player.name);
                    break;
                case 2:
                    printf("%s ran away from %s\n", player.name, enemy.name);
                    goto END_OF_BATTLE;
                case 3:
                    if (inventoryCount == 0) {
                        printf("Your inventory is empty!\n");
                    }
                    else {
                        printf("Choose an item to use:\n");
                        for (int i = 0; i < inventoryCount; i++) {
                            printf("%d. %s\n", i + 1, inventory[i].name);
                        }
                        int itemChoice;
                        scanf("%d", &itemChoice);
                        if (itemChoice > 0 && itemChoice <= inventoryCount) {
                            player.health += inventory[itemChoice - 1].healthBoost;
                            printf("You used %s! Your health is now %d.\n", inventory[itemChoice - 1].name, player.health);
                            for (int j = itemChoice - 1; j < inventoryCount - 1; j++) {
                                inventory[j] = inventory[j + 1];
                            }
                            inventoryCount--;
                        }
                        else {
                            printf("Invalid choice.\n");
                        }
                    }
                    break;
                case 4: // Выбор навыка для использования
                    printf("Choose a skill to use:\n");
                    for (int i = 0; i < MAX_SKILLS; i++) {
                        printf("%d. %s (Damage: %d, Cooldown: %d)\n", i + 1, skills[i].name, skills[i].damage, skills[i].cooldown);
                    }
                    scanf("%d", &skillChoice);
                    if (skillChoice > 0 && skillChoice <= MAX_SKILLS) {
                        if (cooldowns[skillChoice - 1] == 0) {
                            if (skills[skillChoice - 1].damage < 0) {
                                player.health -= skills[skillChoice - 1].damage;
                                printf("You used %s! Your health decreased by %d and is now %d.\n", skills[skillChoice - 1].name, -skills[skillChoice - 1].damage, player.health);
                            }
                            else {
                                enemy.health -= skills[skillChoice - 1].damage;
                                printf("You used %s! %s's health decreased by %d and is now %d.\n", skills[skillChoice - 1].name, enemy.name, skills[skillChoice - 1].damage, enemy.health);
                            }
                            cooldowns[skillChoice - 1] = skills[skillChoice - 1].cooldown;
                        }
                        else {
                            printf("Skill is on cooldown!\n");
                        }
                    }
                    else {
                        printf("Invalid choice.\n");
                    }
                    decreaseCooldowns();
                    break;
                default:
                    printf("Invalid choice!\n");
                }
            }
            if (enemy.health <= 0 && enemyIndex == 0) {
                static int goblinsDefeated = 0;
                goblinsDefeated++;
                if (goblinsDefeated == 5) {
                    completeQuest(0, &xp, &gold);
                }
            }
            if (weaponIndex >= 0 && weaponIndex < 3) {
                completeQuest(1, &xp, &gold);
            }
            if (level == 3) {
                completeQuest(2, &xp, &gold);
            }


        END_OF_BATTLE:
            if (player.health <= 0) {
                printf("%s was defeated...\n", player.name);
                break;
            }
            break;
        case 2:
            printf("\nWelcome to the shop! Gold: %d\n", gold);
            printf("1. Buy Items\n2. Buy Weapons\n3. Exit Shop\n");
            scanf("%d", &choice);
            switch (choice) {
            case 1:
                for (int i = 0; i < MAX_ITEMS; i++) {
                    printf("%d. ", i + 1);
                    displayItem(items[i]);
                }
                printf("%d. Exit\n", MAX_ITEMS + 1);
                scanf("%d", &itemIndex);
                if (itemIndex > 0 && itemIndex <= MAX_ITEMS) {
                    if (gold >= items[itemIndex - 1].price) {
                        gold -= items[itemIndex - 1].price;
                        inventory[inventoryCount++] = items[itemIndex - 1];
                        printf("You bought %s!\n", items[itemIndex - 1].name);
                    }
                    else {
                        printf("Not enough gold!\n");
                    }
                }
                break;
            case 2:
                for (int i = 0; i < MAX_WEAPONS; i++) {
                    printf("%d. ", i + 1);
                    displayWeapon(weapons[i]);
                }
                printf("%d. Exit\n", MAX_WEAPONS + 1);
                scanf("%d", &weaponIndex);
                if (weaponIndex > 0 && weaponIndex <= MAX_WEAPONS) {
                    if (gold >= weapons[weaponIndex - 1].price) {
                        gold -= weapons[weaponIndex - 1].price;
                        equippedWeapon = weapons[weaponIndex - 1];
                        printf("You bought and equipped %s!\n", weapons[weaponIndex - 1].name);
                    }
                    else {
                        printf("Not enough gold!\n");
                    }
                }
                break;
            case 3:
                break;
            default:
                printf("Invalid choice!\n");
                break;
            }
            break;
        case 3:
            printf("\nInventory:\n");
            for (int i = 0; i < inventoryCount; i++) {
                printf("%d. %s\n", i + 1, inventory[i].name);
            }
            printf("\nEquipped Weapon: %s\n", equippedWeapon.name);
            break;
        case 4:
            if (gold >= 20) {
                player.health = classes[classIndex - 1].health;
                gold -= 20;
                printf("You've been fully healed!\n");
            }
            else {
                printf("Not enough gold!\n");
            }
            break;
        case 5: // Выводим список доступных квестов
            printf("\nAvailable Quests:\n");
            for (int i = 0; i < 3; i++) {
                if (!questCompleted[i] && level >= quests[i].requiredLevel) {
                    printf("%d. ", i + 1);
                    displayQuest(quests[i]);
                }
            }
            break;
        case 6: // Покупка питомца
            printf("\nAvailable Pets:\n");
            for (int i = 0; i < 3; i++) {
                printf("%d. %s (Attack Boost: %d, Health Boost: %d, Cost: %d)\n", i + 1, pets[i].name, pets[i].attackBoost, pets[i].healthBoost, pets[i].cost);
            }
            int petChoice;
            scanf("%d", &petChoice);
            if (petChoice > 0 && petChoice <= 3) {
                if (gold >= pets[petChoice - 1].cost) {
                    gold -= pets[petChoice - 1].cost;
                    playerPet = pets[petChoice - 1];
                    player.attack += playerPet.attackBoost;
                    player.health += playerPet.healthBoost;
                    printf("You bought %s as your pet!\n", playerPet.name);
                }
                else {
                    printf("Not enough gold!\n");
                }
            }
            break;
        case 7:
            printf("Choose an NPC to talk to:\n");
            for (int i = 0; i < MAX_NPCS; i++) {
                printf("%d. %s\n", i + 1, npcs[i].name);
            }
            int npcChoice;
            scanf("%d", &npcChoice);
            if (npcChoice > 0 && npcChoice <= MAX_NPCS) {
                printf("%s says: %s\n", npcs[npcChoice - 1].name, npcs[npcChoice - 1].dialogue);
                if (npcs[npcChoice - 1].hasQuest) {
                    printf("You received a quest: %s\n", npcs[npcChoice - 1].quest.description);

                    
                }
            }
            else {
                printf("Invalid choice.\n");
            }
            break;


        default:
            printf("Invalid choice!\n");
            break;
        }
    }

    printf("\nGame Over!\n");
    return 0;
}

