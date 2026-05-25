//Digidice digimon maker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define NAME_SIZE 31
#define FILE_NAME_SIZE 81
#define DESCRIPTION_SIZE 151
#define FILE_SIZE 2000
#define MAX_DIE_TYPE 12
#define BONUS_DRAWBACK_SIZE 51
#define TEMP_ARRAY_SIZE 201

typedef struct attributes{
    size_t brawn;
    size_t brawnBonus;
    size_t grace;
    size_t graceBonus;
    size_t wits;
    size_t witsBonus;
    size_t heart;
    size_t heartBonus;
} attributes;
typedef struct specialAttack{
    char *name;
    char *description;
    char *bonuses;
    char *drawbacks;
    int accuracy;
    int power;
} specialAttack;
typedef struct abilities{
    char * name;
    char * description;
} abilities;
typedef struct gear{
    char * name;
    char * description;
} gear;
typedef struct skill{
    char * name;
    short bonus;
} skill;

typedef struct digimon{
    char *name;
    short int stage;
    short int atributePoints;
    short int bonusPoints;
    short int specialAttackPoints;
    attributes dgmnAttributes;
    specialAttack * special;
    size_t numSpecialAttacks;
    abilities * dgmnAbilities;
    size_t numAbilities;
    gear * dgmnGear;
    size_t numGear;
    skill * dgmnSkills;
    size_t numSkills;
}digimon;

bool isBuiltSpecial = false;
bool isAssignedAbilities = false;
bool isAssignedGear = false;
bool isAssignedSkills = false;
char * characterSheet;

digimon nameCharacter(digimon partner);
digimon determinePoints(digimon partner);
digimon incrementAttributes(digimon partner);
digimon spendBonus(digimon partner);
digimon buildSpecial(digimon partner);
digimon assignAbilities(digimon partner);
digimon assignGear(digimon partner);
digimon assignSkills(digimon partner);
char * clearNewline(char * line);
char * buildCharacterSheet(digimon partner);
void printDigimon(digimon partner);

int main(){

    digimon partner = {0};
    partner.dgmnAttributes = (attributes) {2, 0, 2, 0, 2, 0, 2, 0};
    partner.numSpecialAttacks = 1;

    printf("%s", "Input digimon evolutionary stage as an integer (1, 2, 3, 4, 5, or 6):\t");
    scanf("%hd", &partner.stage);
    while((getchar()) != '\n');
    while(partner.stage < 1 || partner.stage > 6){
        printf("Invalid input.  Please input a number between 1 (for fresh / baby 1) and 6 (for mega / ultimate):\t");
        scanf("%hd", &partner.stage);
    }
    partner = determinePoints(partner);
    partner = nameCharacter(partner);
    partner = incrementAttributes(partner);
    partner = spendBonus(partner);
    partner = buildSpecial(partner);
    partner = assignAbilities(partner);
    partner = assignGear(partner);
    partner = assignSkills(partner);
    printDigimon(partner);
    FILE * fPtr;
    char * fileName;
    fileName = (char *) malloc(sizeof(char) * FILE_NAME_SIZE);
    if(!fileName){
        exit(1);
    }
    strcpy(fileName, partner.name);
    strcat(fileName, ".txt");
    fPtr = fopen(fileName, "w");
    free(fileName);
    characterSheet = buildCharacterSheet(partner);
    fprintf(fPtr, "%s", characterSheet);
    fclose(fPtr);
    free(partner.name);
    for (size_t i = 0; i < partner.numSpecialAttacks; i++){
        free(partner.special[i].name);
        free(partner.special[i].description);
        free(partner.special[i].bonuses);
        free(partner.special[i].drawbacks);
    }
    free(partner.special);
    for (size_t i = 0; i < partner.numAbilities; i++){
        free(partner.dgmnAbilities[i].name);
        free(partner.dgmnAbilities[i].description);
    }
    free(partner.dgmnAbilities);
    for (size_t i = 0; i < partner.numGear; i++){
        free(partner.dgmnGear[i].name);
        free(partner.dgmnGear[i].description);    
    }
    free(partner.dgmnGear);
    for (size_t i = 0; i < partner.numSkills; i++){
        free(partner.dgmnSkills[i].name);
    }
    free(partner.dgmnSkills);
    free(characterSheet);
    return 0;
}

digimon determinePoints(digimon partner){
    switch (partner.stage)
    {
    case 1:
        break;
    case 2:
        partner.atributePoints = 4;
        break;
    case 3:
        partner.atributePoints = 8;
        partner.bonusPoints = 1;
        partner.specialAttackPoints = 2;
        break;
    case 4:
        partner.atributePoints = 12;
        partner.bonusPoints = 2;
        partner.specialAttackPoints = 4;
        break;
    case 5:
        partner.atributePoints = 16;
        partner.bonusPoints = 3;
        partner.specialAttackPoints = 6;
        break;
    case 6:
        partner.atributePoints = 20;
        partner.bonusPoints = 5;
        partner.specialAttackPoints = 8;
        break;
    default:
        break;
    }
    return partner;
}

digimon nameCharacter(digimon partner){
    partner.name = (char *) malloc(sizeof(char) * NAME_SIZE);
    if(!partner.name){
        exit(1);
    }
    printf("%s", "Input digimon name, 30 characters or less:\t");
    fgets(partner.name, NAME_SIZE, stdin);
    partner.name = clearNewline(partner.name);
    char sentinel = 'n';
    do{
        printf("You typed: %s.\nIs that correct? Y for yes:\t", partner.name);
        scanf("%c", &sentinel);
        sentinel = (char) toupper(sentinel);
        if (sentinel != 'Y'){
            while((getchar()) != '\n');
            printf("%s", "Input digimon name:\t");
            fgets(partner.name, 50, stdin);
            partner.name = clearNewline(partner.name);
        }
    } while(sentinel != 'Y');
    return (partner);
}

digimon incrementAttributes(digimon partner){
    while((getchar()) != '\n');
    char atribute[11];
    while (partner.atributePoints > 0){
        printf("%s", "Increment atribute?  Heart, Brawn, Grace, Wits or Print:\t");
        fgets(atribute, 10, stdin);
        atribute[0] = (char) toupper(atribute[0]);
        switch (atribute[0])
        {
        case 'H':
            partner.dgmnAttributes.heart += 2;
            partner.atributePoints--;
            if (partner.dgmnAttributes.heart > MAX_DIE_TYPE){
                partner.dgmnAttributes.heart = MAX_DIE_TYPE;
                partner.dgmnAttributes.heartBonus++;
            }
            break;
        case 'B':
            partner.dgmnAttributes.brawn += 2;
            partner.atributePoints--;
            if (partner.dgmnAttributes.brawn > MAX_DIE_TYPE){
                partner.dgmnAttributes.brawn = MAX_DIE_TYPE;
                partner.dgmnAttributes.brawnBonus++;
            }
            break;
        case 'G':
            partner.dgmnAttributes.grace += 2;
            partner.atributePoints--;
            if (partner.dgmnAttributes.grace > MAX_DIE_TYPE){
                partner.dgmnAttributes.grace = MAX_DIE_TYPE;
                partner.dgmnAttributes.graceBonus++;
            }
            break;
        case 'W':
            partner.dgmnAttributes.wits += 2;
            partner.atributePoints--;
            if (partner.dgmnAttributes.wits > MAX_DIE_TYPE){
                partner.dgmnAttributes.wits = MAX_DIE_TYPE;
                partner.dgmnAttributes.witsBonus++;
            }
            break;
        case 'P':
            printDigimon(partner);
            break;
        default:
            break;
        }
    }
    return partner;
}

digimon spendBonus(digimon partner){
    char buffer[TEMP_ARRAY_SIZE];
    for (unsigned short i = 0; i < partner.bonusPoints; i++){
        printf("%s", "Spend bonus points: Special, Ability, Skill or Gear?\t");
        fgets(buffer, TEMP_ARRAY_SIZE, stdin);
        buffer[0] = (char) toupper(buffer[0]);
        buffer[1] = (char) toupper(buffer[1]);
        switch (buffer[0])
        {
        case 'S':
            if (buffer[1] == 'P'){
                partner.numSpecialAttacks++;
            }
            else if (buffer[1] == 'K'){
                partner.numSkills++;
            }
            break;
        case 'A':
            partner.numAbilities++;
            break;
        case 'G':
            partner.numGear++;
            break;
        default:
            continue;
        }
    }
    partner.dgmnAbilities = (abilities *) malloc(sizeof(abilities) * partner.numAbilities);
    if(!partner.dgmnAbilities){
        exit(1);
    }
    partner.dgmnGear = (gear *) malloc(sizeof(gear) * partner.numGear);
    if(!partner.dgmnGear){
        exit(1);
    }
    partner.dgmnSkills = (skill *) malloc(sizeof(skill) * partner.numSkills);
    if(!partner.dgmnSkills){
        exit(1);
    }
    partner.special = (specialAttack *) malloc(sizeof(specialAttack) * partner.numSpecialAttacks);
    if(!partner.special){
        exit(1);
    }
    for (size_t i = 0; i < partner.numAbilities; i++){
        partner.dgmnAbilities[i] = (abilities) {0};
    }
    for (size_t i = 0; i < partner.numGear; i++){
        partner.dgmnGear[i] = (gear) {0};
    }
    for (size_t i = 0; i < partner.numSkills; i++){
        partner.dgmnSkills[i] = (skill) {0};
    }
    for (size_t i = 0; i < partner.numSpecialAttacks; i++){
        partner.special[i] = (specialAttack) {0};
    }
    return partner;
}

digimon buildSpecial(digimon partner){
    char * attackStat = (char *) malloc(sizeof(char) * 15);
    if(!attackStat){
        exit(1);
    }
    for(size_t i = 0; i < partner.numSpecialAttacks; i++){
        partner.special[i].name = (char *) malloc(sizeof(char) * NAME_SIZE);
        if(!partner.special[1].name){
            exit(1);
        }
        partner.special[i].description = (char *) malloc(sizeof(char) * DESCRIPTION_SIZE);
        if(!partner.special[i].description){
            exit(1);
        }
        partner.special[i].bonuses = (char *) malloc(sizeof(char) * BONUS_DRAWBACK_SIZE);
        if(!partner.special[i].bonuses){
            exit(1);
        }
        partner.special[i].drawbacks = (char *) malloc(sizeof(char) * BONUS_DRAWBACK_SIZE);
        if(!partner.special[i].drawbacks){
            exit(1);
        }
        partner.special[i].bonuses[0] = '\0';
        partner.special[i].drawbacks[0] = '\0';
        partner.special[i].accuracy = 2;
        partner.special[i].power = 2;
        printf("Name special attack number %lu, 30 characters or less:\t", i + 1);
        fgets(partner.special[i].name, NAME_SIZE, stdin);
        partner.special[i].name = clearNewline(partner.special[i].name);
        printf("Describe special attack number %lu, 100 characters or less:\t", i + 1);
        fgets(partner.special[i].description, DESCRIPTION_SIZE, stdin);
        partner.special[i].description = clearNewline(partner.special[i].description);
        printf("List special attack number %lu bonus(es), 50 characters or less:\t", i + 1);
        fgets(partner.special[i].bonuses, BONUS_DRAWBACK_SIZE, stdin);
        partner.special[i].bonuses = clearNewline(partner.special[i].bonuses);
        printf("List special attack number %lu drawback(s) if applicable, 50 characters or less:\t", i + 1);
        fgets(partner.special[i].drawbacks, BONUS_DRAWBACK_SIZE, stdin);
        partner.special[i].drawbacks = clearNewline(partner.special[i].drawbacks);
        for(short j = 0; j < partner.specialAttackPoints; j++){
            printf("%s", "Augment Accuracy or Power?\t");
            fgets(attackStat, 15, stdin);
            attackStat[0] = (char) toupper(attackStat[0]);
            switch (attackStat[0])
            {
            case 'A':
                partner.special[i].accuracy += 2;
                break;
            case 'P':
                partner.special[i].power += 2;
                break;
            default:
                continue;
            }
        }
    }
    free(attackStat);
    isBuiltSpecial = true;
    return partner;
}

digimon assignAbilities(digimon partner){
    for (size_t i = 0; i < partner.numAbilities; i++){
        partner.dgmnAbilities[i].name = (char *) malloc(sizeof(char) * NAME_SIZE);
        if(!partner.dgmnAbilities[i].name){
            exit(1);
        }
        partner.dgmnAbilities[i].description = (char *) malloc(sizeof(char) * DESCRIPTION_SIZE);
        if(!partner.dgmnAbilities[i].description){
            exit(1);
        }
        printf("Name ability #%zu, 30 characters or less:\t", i + 1);
        fgets(partner.dgmnAbilities[i].name, NAME_SIZE, stdin);
        partner.dgmnAbilities[i].name = clearNewline(partner.dgmnAbilities[i].name);
        printf("Describe ability #%zu, 100 characters or less:\t", i + 1);
        fgets(partner.dgmnAbilities[i].description, DESCRIPTION_SIZE, stdin);
        partner.dgmnAbilities[i].description = clearNewline(partner.dgmnAbilities[i].description);
    }
    isAssignedAbilities = true;
    return partner;
}

digimon assignGear(digimon partner){
    for (size_t i = 0; i < partner.numGear; i++){
        partner.dgmnGear[i].name = (char *) malloc(sizeof(char) * NAME_SIZE);
        if(!partner.dgmnGear[i].name){
            exit(1);
        }
        partner.dgmnGear[i].description = (char *) malloc(sizeof(char) * DESCRIPTION_SIZE);
        if(!partner.dgmnGear[i].description){
            exit(1);
        }
        printf("Name gear #%lu, 30 characters or less:\t", i + 1);
        fgets(partner.dgmnGear[i].name, NAME_SIZE, stdin);
        partner.dgmnGear[i].name = clearNewline(partner.dgmnGear[i].name);
        printf("Describe gear #%lu, 100 characters or less:\t", i + 1);
        fgets(partner.dgmnGear[i].description, DESCRIPTION_SIZE, stdin);
        partner.dgmnGear[i].description = clearNewline(partner.dgmnGear[i].description);
    }
    isAssignedGear = true;
    return partner;
}

digimon assignSkills(digimon partner){
    for (size_t i = 0; i < partner.numSkills; i++){
        partner.dgmnSkills[i].name = (char *) malloc(sizeof(char) * 16);
        if(!partner.dgmnSkills[i].name){
            exit(1);
        }
        printf("Name Skill #%zu, 30 characters or less:\t", i + 1);
        fgets(partner.dgmnSkills[i].name, NAME_SIZE, stdin);
        partner.dgmnSkills[i].name = clearNewline(partner.dgmnSkills[i].name);
        partner.dgmnSkills[i].bonus = 2;
        }
    isAssignedSkills = true;
    return partner;
}

char * clearNewline(char * line){
    size_t i = 0;
    while (line[i] != '\0'){
        i++;
    }
    if (i > 0 && line[i - 1] == '\n'){
        line[i - 1] = '\0';
    }
    return line;
}

void printDigimon(digimon partner){
    printf("\n\nName: %s\n", partner.name);
    printf("Atribute points remaining: %d\n", partner.atributePoints);
    printf("Bonus points remaining: %d\n", partner.bonusPoints);
    printf("Special Attack Points remaining: %d\n", partner.specialAttackPoints);
    printf("%s", "----------Atributes----------\n");
    printf("Heart: 2d%lu + %lu\n" , partner.dgmnAttributes.heart, partner.dgmnAttributes.heartBonus);
    printf("Brawn: 2d%lu + %lu\n" , partner.dgmnAttributes.brawn, partner.dgmnAttributes.brawnBonus);
    printf("Wits: 2d%lu + %lu\n" , partner.dgmnAttributes.wits, partner.dgmnAttributes.witsBonus);
    printf("Grace: 2d%lu + %lu\n" , partner.dgmnAttributes.grace, partner.dgmnAttributes.graceBonus);
    printf("%s", "----------Defences----------\n");
    printf("Evasion: %lu\n", ((partner.dgmnAttributes.wits + partner.dgmnAttributes.grace) / 2 + partner.dgmnAttributes.witsBonus + partner.dgmnAttributes.graceBonus));
    printf("Toughness: %lu\n", ((partner.dgmnAttributes.brawn + partner.dgmnAttributes.heart) / 2 + partner.dgmnAttributes.brawnBonus + partner.dgmnAttributes.heartBonus));
    if (isBuiltSpecial){
        printf("%s", "----------Special Attack(s)----------\n");
        for (size_t i = 0; i < partner.numSpecialAttacks; i++){
            printf("Name: %s\tAccuracy: 2d%d\tPower: 2d%d\n", partner.special[i].name, partner.special[i].accuracy, partner.special[i].power);
            printf("Bonus(es): %s\n", partner.special[i].bonuses);
            printf("Drawback(s): %s\n", partner.special[i].drawbacks ? partner.special[i].drawbacks : "None");
            printf("Description: %s\n", partner.special[i].description);
        }
    }
    if (isAssignedAbilities){
        printf("%s\n", "----------Abilities----------");
        for (size_t i = 0; i < partner.numAbilities; i++){
            printf("%s:\t%s\n", partner.dgmnAbilities[i].name, partner.dgmnAbilities[i].description);
        } 
    }

    if (isAssignedGear){
        printf("%s\n", "----------Gear----------");
        for (size_t i = 0; i < partner.numGear; i++){
            printf("%s\n%s\n", partner.dgmnGear[i].name, partner.dgmnGear[i].description);
        }
    }

    if (isAssignedSkills){
        printf("%s\n", "----------Skills----------");
        for (size_t i = 0; i < partner.numSkills; i++){
            printf("%s\t+%d\n", partner.dgmnSkills[i].name, partner.dgmnSkills[i].bonus);
        }
    }
}

char * buildCharacterSheet(digimon partner){
    characterSheet = (char *) malloc(sizeof(char) * FILE_SIZE);
    if(!characterSheet){
        exit(1);
    }
    characterSheet[0] = '\0';
    char * temp = (char *) malloc(sizeof(char) * TEMP_ARRAY_SIZE);
    if(!temp){
        exit(1);
    }
    temp[0] = '\0';
    size_t evasion = ((partner.dgmnAttributes.wits + partner.dgmnAttributes.grace) / 2 + partner.dgmnAttributes.witsBonus + partner.dgmnAttributes.graceBonus);
    size_t toughness = ((partner.dgmnAttributes.brawn + partner.dgmnAttributes.heart) / 2 + partner.dgmnAttributes.brawnBonus + partner.dgmnAttributes.heartBonus);
    sprintf(characterSheet, "Name:\t%s\n\n----------Attributes----------\nHeart: 2d%lu + %lu\nBrawn: 2d%lu + %lu\nWits: 2d%lu + %lu\nGrace: 2d%lu + %lu\n\n----------Defenses----------\n\nEvasion:\t%lu\nToughness\t%lu\n", 
        partner.name, partner.dgmnAttributes.heart, partner.dgmnAttributes.heartBonus, partner.dgmnAttributes.brawn, partner.dgmnAttributes.brawnBonus, partner.dgmnAttributes.wits, partner.dgmnAttributes.witsBonus, partner.dgmnAttributes.grace, partner.dgmnAttributes.graceBonus, evasion, toughness);
    if (isBuiltSpecial){
        strcat(characterSheet, "\n----------Special Attack(s)----------\n");
        for (size_t i = 0; i < partner.numSpecialAttacks; i++){
            sprintf(temp, "Name: %s\tAccuracy: 2d%d\tPower: 2d%d\nBonus(es):\t%s\nDrawback(s):\t%s\nDescription:\t%s\n", partner.special[i].name, partner.special[i].accuracy, partner.special[i].power, partner.special[i].bonuses, partner.special[i].drawbacks, partner.special[i].description);
            strcat(characterSheet, temp);
        }
    }
    if (isAssignedAbilities){
        strcat(characterSheet, "\n----------Abilities----------\n");
        for (size_t i = 0; i < partner.numAbilities; i++){
            sprintf(temp, "%s:\t%s\n", partner.dgmnAbilities[i].name, partner.dgmnAbilities[i].description);
            strcat(characterSheet, temp);
        } 
    }
    if  (isAssignedGear){
        strcat(characterSheet, "\n----------Gear----------\n");
        for (size_t i = 0; i < partner.numGear; i++){
            sprintf(temp, "%s:\t%s\n", partner.dgmnGear[i].name, partner.dgmnGear[i].description);
            strcat(characterSheet, temp);
        }
    }

    if (isAssignedSkills){
        strcat(characterSheet, "\n----------Skills----------\n");
        for (size_t i = 0; i < partner.numSkills; i++){
            sprintf(temp, "%s:\t%d\n", partner.dgmnSkills[i].name, partner.dgmnSkills[i].bonus);
            strcat(characterSheet, temp);
        }
    }
    free(temp);
    return characterSheet;
}
