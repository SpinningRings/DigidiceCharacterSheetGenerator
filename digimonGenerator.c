//Digidice digimon maker

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define NAME_SIZE 31
#define FILE_NAME_SIZE 81
#define DESCRIPTION_SIZE 151
#define FILE_SIZE 8000
#define MAX_DIE_TYPE 12
#define BONUS_DRAWBACK_SIZE 51
#define TEMP_ARRAY_SIZE 401

typedef struct attributes{
    unsigned short brawn;
    unsigned short brawnBonus;
    unsigned short grace;
    unsigned short graceBonus;
    unsigned short wits;
    unsigned short witsBonus;
    unsigned short heart;
    unsigned short heartBonus;
} attributes;
typedef struct specialAttack{
    char *name;
    char *description;
    char *bonuses;
    char *drawbacks;
    int accuracy;
    int power;
} specialAttack;
typedef struct ability{
    char * name;
    char * description;
} ability;
typedef struct gear{
    char * name;
    char * description;
} gear;
typedef struct skill{
    char * name;
    unsigned short bonus;
} skill;

typedef struct digimon{
    unsigned short stage;
    unsigned short attributePoints;
    unsigned short bonusPoints;
    unsigned short specialAttackPoints;
    unsigned short numSpecialAttacks;
    unsigned short numAbilities;
    unsigned short numGear;
    unsigned short numSkills;
    char *name;
    attributes dgmnAttributes;
    specialAttack * specialAttacks;
    ability * abilities;
    gear * dgmnGear;
    skill * skills;
} digimon;


void nameCharacter(digimon * partner);
void determinePoints(digimon * partner);
void incrementAttributes(digimon * partner);
void spendBonusPoints(digimon * partner);
void buildSpecial(digimon * partner);
void assignAbilities(digimon * partner);
void assignGear(digimon * partner);
void assignSkills(digimon * partner);
char * clearNewline(char * line);
char * buildCharacterSheet(digimon * partner);
void printDigimon(digimon * partner);

int main(){

    digimon partner = {0};
    partner.dgmnAttributes = (attributes) {2, 0, 2, 0, 2, 0, 2, 0};
    partner.specialAttacks = NULL;
    partner.abilities = NULL;
    partner.dgmnGear = NULL;
    partner.skills = NULL;
    partner.numSpecialAttacks = 1;

    printf("%s", "Input digimon evolutionary stage as an integer (1, 2, 3, 4, 5, or 6):\t");
    scanf("%hu", &partner.stage);
    int c = 0;
    while(c = (getchar()) != '\n' && c != EOF);
    while(partner.stage < 1 || partner.stage > 6){
        printf("Invalid input.  Please input a number between 1 (for fresh / baby 1) and 6 (for mega / ultimate):\t");
        scanf("%hu", &partner.stage);
        int c = 0;
        while(c = (getchar()) != '\n' && c != EOF);
    }
    determinePoints(&partner);
    nameCharacter(&partner);
    incrementAttributes(&partner);
    if(partner.bonusPoints > 0){
        spendBonusPoints(&partner);
    }
    buildSpecial(&partner);
    assignAbilities(&partner);
    assignGear(&partner);
    assignSkills(&partner);
    printDigimon(&partner);
    FILE * fPtr;
    char fileName[FILE_NAME_SIZE] = {0};
    size_t j = 0;
    for (size_t i = 0; i < strlen(partner.name); i++){
        if (isalnum(partner.name[i])){
            fileName[j++] = partner.name[i];
        }
    }
    fileName[j] = '\0';
    strncat(fileName, ".txt", FILE_NAME_SIZE);
    fPtr = fopen(fileName, "w");
    if(!fPtr){
        exit(1);
    }
    char * characterSheet = buildCharacterSheet(&partner);
    fprintf(fPtr, "%s", characterSheet);
    fclose(fPtr);
    free(partner.name);
    if(partner.specialAttacks){
        for (unsigned short i = 0; i < partner.numSpecialAttacks; i++){
            free(partner.specialAttacks[i].name);
            free(partner.specialAttacks[i].description);
            free(partner.specialAttacks[i].bonuses);
            free(partner.specialAttacks[i].drawbacks);
        }
        free(partner.specialAttacks);
    }
    if(partner.abilities){
        for (unsigned short i = 0; i < partner.numAbilities; i++){
            free(partner.abilities[i].name);
            free(partner.abilities[i].description);
        }
        free(partner.abilities);
    }
    if(partner.dgmnGear){
        for (unsigned short i = 0; i < partner.numGear; i++){
            free(partner.dgmnGear[i].name);
            free(partner.dgmnGear[i].description);    
        }
        free(partner.dgmnGear);
    }
    if(partner.skills){
        for (unsigned short i = 0; i < partner.numSkills; i++){
            free(partner.skills[i].name);
        }
        free(partner.skills);
    }
    free(characterSheet);
    return 0;
}

void determinePoints(digimon * partner){
    switch (partner->stage)
    {
    case 1:
        break;
    case 2:
        partner->attributePoints = 4;
        break;
    case 3:
        partner->attributePoints = 8;
        partner->bonusPoints = 1;
        partner->specialAttackPoints = 2;
        break;
    case 4:
        partner->attributePoints = 12;
        partner->bonusPoints = 2;
        partner->specialAttackPoints = 4;
        break;
    case 5:
        partner->attributePoints = 16;
        partner->bonusPoints = 3;
        partner->specialAttackPoints = 6;
        break;
    case 6:
        partner->attributePoints = 20;
        partner->bonusPoints = 5;
        partner->specialAttackPoints = 8;
        break;
    default:
        break;
    }
}

void nameCharacter(digimon * partner){
    partner->name = (char *) malloc(sizeof(char) * NAME_SIZE);
    if(!partner->name){
        exit(1);
    }
    printf("%s", "Input digimon name, 30 characters or less:\t");
    fgets(partner->name, NAME_SIZE, stdin);
    clearNewline(partner->name);
    char sentinel = 'n';
    do{
        printf("You typed: %s.\nIs that correct? Y for yes:\t", partner->name);
        scanf("%c", &sentinel);
        sentinel = (char) toupper(sentinel);
        int c = 0;
        while(c = (getchar()) != '\n' && c != EOF);
        if(sentinel != 'Y'){
            printf("%s", "Input digimon name:\t");
            fgets(partner->name, NAME_SIZE, stdin);
            clearNewline(partner->name);
        }
    } while(sentinel != 'Y');
}

void incrementAttributes(digimon * partner){
    char atribute[TEMP_ARRAY_SIZE];
    while (partner->attributePoints > 0){
        printf("%s", "Increment atribute?  Heart, Brawn, Grace, Wits or Print:\t");
        fgets(atribute, TEMP_ARRAY_SIZE, stdin);
        atribute[0] = (char) toupper(atribute[0]);
        switch (atribute[0])
        {
        case 'H':
            partner->dgmnAttributes.heart += 2;
            partner->attributePoints--;
            if(partner->dgmnAttributes.heart > MAX_DIE_TYPE){
                partner->dgmnAttributes.heart = MAX_DIE_TYPE;
                partner->dgmnAttributes.heartBonus++;
            }
            break;
        case 'B':
            partner->dgmnAttributes.brawn += 2;
            partner->attributePoints--;
            if(partner->dgmnAttributes.brawn > MAX_DIE_TYPE){
                partner->dgmnAttributes.brawn = MAX_DIE_TYPE;
                partner->dgmnAttributes.brawnBonus++;
            }
            break;
        case 'G':
            partner->dgmnAttributes.grace += 2;
            partner->attributePoints--;
            if(partner->dgmnAttributes.grace > MAX_DIE_TYPE){
                partner->dgmnAttributes.grace = MAX_DIE_TYPE;
                partner->dgmnAttributes.graceBonus++;
            }
            break;
        case 'W':
            partner->dgmnAttributes.wits += 2;
            partner->attributePoints--;
            if(partner->dgmnAttributes.wits > MAX_DIE_TYPE){
                partner->dgmnAttributes.wits = MAX_DIE_TYPE;
                partner->dgmnAttributes.witsBonus++;
            }
            break;
        case 'P':
            printDigimon(partner);
            break;
        default:
            break;
        }
    }
}

void spendBonusPoints(digimon * partner){
    char buffer[TEMP_ARRAY_SIZE];
    while (partner->bonusPoints > 0){
        printf("%s", "Spend bonus points: Special, Ability, Skill or Gear?\t");
        fgets(buffer, TEMP_ARRAY_SIZE, stdin);
        buffer[0] = (char) toupper(buffer[0]);
        buffer[1] = (char) toupper(buffer[1]);
        switch (buffer[0])
        {
        case 'S':
            if(buffer[1] == 'P'){
                partner->numSpecialAttacks++;
                partner->bonusPoints--;
            }
            else if(buffer[1] == 'K'){
                partner->numSkills++;
                partner->bonusPoints--;
            }
            break;
        case 'A':
            partner->numAbilities++;
            partner->bonusPoints--;
            break;
        case 'G':
            partner->numGear++;
            partner->bonusPoints--;
            break;
        default:
            break;
        }
    }
    if(partner->numAbilities > 0){
        partner->abilities = (ability *) malloc(sizeof(ability) * partner->numAbilities);
        if(!partner->abilities){
            exit(1);
        }
    }
    else {
        partner->abilities = NULL;
    }
    if(partner->numGear > 0){
        partner->dgmnGear = (gear *) malloc(sizeof(gear) * partner->numGear);
        if(!partner->dgmnGear){
            exit(1);
        }
    }
    else {
        partner->dgmnGear = NULL;
    }
    if(partner->numSkills > 0){
        partner->skills = (skill *) malloc(sizeof(skill) * partner->numSkills);
        if(!partner->skills){
            exit(1);
        }
    }
    else {
        partner->skills = NULL;
    }
    if (partner->abilities){
        for (unsigned short i = 0; i < partner->numAbilities; i++){
            partner->abilities[i] = (ability) {0};
        }
    }
    if(partner->dgmnGear){
        for (unsigned short i = 0; i < partner->numGear; i++){
            partner->dgmnGear[i] = (gear) {0};
        }
    }
    if(partner->skills){
        for (unsigned short i = 0; i < partner->numSkills; i++){
            partner->skills[i] = (skill) {0};
        }
    }
    if(partner->specialAttacks){
        for (unsigned short i = 0; i < partner->numSpecialAttacks; i++){
            partner->specialAttacks[i] = (specialAttack) {0};
        }
    }
}

void buildSpecial(digimon * partner){
    if(partner->numSpecialAttacks > 0){
        partner->specialAttacks = (specialAttack *) malloc(sizeof(specialAttack) * partner->numSpecialAttacks);
        if(!partner->specialAttacks){
            exit(1);
        }
    }
    else {
        partner->specialAttacks = NULL;
    }
    char * attackStat = (char *) malloc(sizeof(char) * TEMP_ARRAY_SIZE);
    if(!attackStat){
        exit(1);
    }
    for(unsigned short i = 0; i < partner->numSpecialAttacks; i++){
        partner->specialAttacks[i].name = (char *) malloc(sizeof(char) * NAME_SIZE);
        if(!partner->specialAttacks[i].name){
            exit(1);
        }
        partner->specialAttacks[i].description = (char *) malloc(sizeof(char) * DESCRIPTION_SIZE);
        if(!partner->specialAttacks[i].description){
            exit(1);
        }
        partner->specialAttacks[i].bonuses = (char *) malloc(sizeof(char) * BONUS_DRAWBACK_SIZE);
        if(!partner->specialAttacks[i].bonuses){
            exit(1);
        }
        partner->specialAttacks[i].drawbacks = (char *) malloc(sizeof(char) * BONUS_DRAWBACK_SIZE);
        if(!partner->specialAttacks[i].drawbacks){
            exit(1);
        }
        partner->specialAttacks[i].bonuses[0] = '\0';
        partner->specialAttacks[i].drawbacks[0] = '\0';
        partner->specialAttacks[i].accuracy = 2;
        partner->specialAttacks[i].power = 2;
        printf("Name special attack number %hd, 30 characters or less:\t", i + 1);
        fgets(partner->specialAttacks[i].name, NAME_SIZE, stdin);
        partner->specialAttacks[i].name = clearNewline(partner->specialAttacks[i].name);
        printf("Describe special attack number %hd, 100 characters or less:\t", i + 1);
        fgets(partner->specialAttacks[i].description, DESCRIPTION_SIZE, stdin);
        partner->specialAttacks[i].description = clearNewline(partner->specialAttacks[i].description);
        printf("List special attack number %hd bonus(es), 50 characters or less:\t", i + 1);
        fgets(partner->specialAttacks[i].bonuses, BONUS_DRAWBACK_SIZE, stdin);
        partner->specialAttacks[i].bonuses = clearNewline(partner->specialAttacks[i].bonuses);
        printf("List special attack number %hd drawback(s) if applicable, 50 characters or less:\t", i + 1);
        fgets(partner->specialAttacks[i].drawbacks, BONUS_DRAWBACK_SIZE, stdin);
        partner->specialAttacks[i].drawbacks = clearNewline(partner->specialAttacks[i].drawbacks);
        for(unsigned short j = 0; j < partner->specialAttackPoints; j++){
            printf("%s", "Augment Accuracy or Power?\t");
            fgets(attackStat, TEMP_ARRAY_SIZE, stdin);
            attackStat[0] = (char) toupper(attackStat[0]);
            switch (attackStat[0])
            {
            case 'A':
                partner->specialAttacks[i].accuracy += 2;
                break;
            case 'P':
                partner->specialAttacks[i].power += 2;
                break;
            default:
                continue;
            }
        }
    }
    free(attackStat);
}

void assignAbilities(digimon * partner){
    for (unsigned short i = 0; i < partner->numAbilities; i++){
        partner->abilities[i].name = (char *) malloc(sizeof(char) * NAME_SIZE);
        if(!partner->abilities[i].name){
            exit(1);
        }
        partner->abilities[i].description = (char *) malloc(sizeof(char) * DESCRIPTION_SIZE);
        if(!partner->abilities[i].description){
            exit(1);
        }
        printf("Name ability #%hd, 30 characters or less:\t", i + 1);
        fgets(partner->abilities[i].name, NAME_SIZE, stdin);
        partner->abilities[i].name = clearNewline(partner->abilities[i].name);
        printf("Describe ability #%hd, 100 characters or less:\t", i + 1);
        fgets(partner->abilities[i].description, DESCRIPTION_SIZE, stdin);
        partner->abilities[i].description = clearNewline(partner->abilities[i].description);
    }
}

void assignGear(digimon * partner){
    for (unsigned short i = 0; i < partner->numGear; i++){
        partner->dgmnGear[i].name = (char *) malloc(sizeof(char) * NAME_SIZE);
        if(!partner->dgmnGear[i].name){
            exit(1);
        }
        partner->dgmnGear[i].description = (char *) malloc(sizeof(char) * DESCRIPTION_SIZE);
        if(!partner->dgmnGear[i].description){
            exit(1);
        }
        printf("Name gear #%hd, 30 characters or less:\t", i + 1);
        fgets(partner->dgmnGear[i].name, NAME_SIZE, stdin);
        partner->dgmnGear[i].name = clearNewline(partner->dgmnGear[i].name);
        printf("Describe gear #%hd, 100 characters or less:\t", i + 1);
        fgets(partner->dgmnGear[i].description, DESCRIPTION_SIZE, stdin);
        partner->dgmnGear[i].description = clearNewline(partner->dgmnGear[i].description);
    }
}

void assignSkills(digimon * partner){
    for (unsigned short i = 0; i < partner->numSkills; i++){
        partner->skills[i].name = (char *) malloc(sizeof(char) * NAME_SIZE);
        if(!partner->skills[i].name){
            exit(1);
        }
        printf("Name Skill #%hd, 30 characters or less:\t", i + 1);
        fgets(partner->skills[i].name, NAME_SIZE, stdin);
        partner->skills[i].name = clearNewline(partner->skills[i].name);
        partner->skills[i].bonus = 2;
    }
}

char * clearNewline(char * line){
    line[strcspn(line, "\n")] = '\0';
    return line;
}

void printDigimon(digimon * partner){
    printf("\n\nName: %s\n", partner->name);
    printf("Atribute points remaining: %d\n", partner->attributePoints);
    printf("Bonus points remaining: %d\n", partner->bonusPoints);
    printf("Special Attack Points remaining: %d\n", partner->specialAttackPoints);
    printf("%s", "----------Atributes----------\n");
    printf("Heart: 2d%hd + %hd\n" , partner->dgmnAttributes.heart, partner->dgmnAttributes.heartBonus);
    printf("Brawn: 2d%hd + %hd\n" , partner->dgmnAttributes.brawn, partner->dgmnAttributes.brawnBonus);
    printf("Wits: 2d%hd + %hd\n" , partner->dgmnAttributes.wits, partner->dgmnAttributes.witsBonus);
    printf("Grace: 2d%hd + %hd\n" , partner->dgmnAttributes.grace, partner->dgmnAttributes.graceBonus);
    printf("%s", "----------Defences----------\n");
    printf("Evasion: %hd\n", ((partner->dgmnAttributes.wits + partner->dgmnAttributes.grace) / 2 + partner->dgmnAttributes.witsBonus + partner->dgmnAttributes.graceBonus));
    printf("Toughness: %hd\n", ((partner->dgmnAttributes.brawn + partner->dgmnAttributes.heart) / 2 + partner->dgmnAttributes.brawnBonus + partner->dgmnAttributes.heartBonus));
    if(partner->specialAttacks){
        printf("%s", "----------Special Attack(s)----------\n");
        for (unsigned short i = 0; i < partner->numSpecialAttacks; i++){
            printf("Name: %s\tAccuracy: 2d%d\tPower: 2d%d\n", partner->specialAttacks[i].name, partner->specialAttacks[i].accuracy, partner->specialAttacks[i].power);
            printf("Bonus(es): %s\n", partner->specialAttacks[i].bonuses);
            printf("Drawback(s): %s\n", partner->specialAttacks[i].drawbacks ? partner->specialAttacks[i].drawbacks : "None");
            printf("Description: %s\n", partner->specialAttacks[i].description);
        }
    }
    if(partner->abilities){
        printf("%s\n", "----------Abilities----------");
        for (unsigned short i = 0; i < partner->numAbilities; i++){
            printf("%s:\t%s\n", partner->abilities[i].name, partner->abilities[i].description);
        } 
    }

    if(partner->dgmnGear){
        printf("%s\n", "----------Gear----------");
        for (unsigned short i = 0; i < partner->numGear; i++){
            printf("%s\n%s\n", partner->dgmnGear[i].name, partner->dgmnGear[i].description);
        }
    }

    if(partner->skills){
        printf("%s\n", "----------Skills----------");
        for (unsigned short i = 0; i < partner->numSkills; i++){
            printf("%s\t+%d\n", partner->skills[i].name, partner->skills[i].bonus);
        }
    }
}

char * buildCharacterSheet(digimon * partner){
    size_t characterCounter = 0;
    
    if (partner->specialAttacks){
        for (size_t i = 0; i < partner->numSpecialAttacks; i++){
            characterCounter += strlen(partner->specialAttacks[i].name) + strlen(partner->specialAttacks[i].description) + strlen(partner->specialAttacks[i].bonuses) + strlen(partner->specialAttacks[i].drawbacks);
        }
    } 

    if (partner->abilities){
        for (size_t i = 0; i < partner->numAbilities; i++){
            characterCounter += strlen(partner->abilities[i].name) + strlen(partner->abilities[i].description);
        }
    }
     
    if (partner->dgmnGear){
        for (size_t i = 0; i < partner->numGear; i++){
            characterCounter += strlen(partner->dgmnGear[i].name) + strlen(partner->dgmnGear[i].description);
        }
    }

    if (partner->skills){
        for (size_t i = 0; i < partner->numSkills; i++){
            characterCounter += strlen(partner->skills[i].name);
        }
    }
    
    char * characterSheet = (char *) malloc(sizeof(char) * (FILE_SIZE + characterCounter));
    if(!characterSheet){
        exit(1);
    }
    characterSheet[0] = '\0';
    char * temp = (char *) malloc(sizeof(char) * TEMP_ARRAY_SIZE);
    if(!temp){
        exit(1);
    }
    temp[0] = '\0';
    unsigned short half = 2;
    unsigned short evasion = (unsigned short) ((partner->dgmnAttributes.wits + partner->dgmnAttributes.grace) / half + partner->dgmnAttributes.witsBonus + partner->dgmnAttributes.graceBonus);
    unsigned short toughness = (unsigned short) ((partner->dgmnAttributes.brawn + partner->dgmnAttributes.heart) / half + partner->dgmnAttributes.brawnBonus + partner->dgmnAttributes.heartBonus);
    sprintf(characterSheet, "Name:\t%s\n\n----------Attributes----------\nHeart: 2d%hd + %hd\nBrawn: 2d%hd + %hd\nWits: 2d%hd + %hd\nGrace: 2d%hd + %hd\n\n----------Defenses----------\n\nEvasion:\t%hd\nToughness\t%hd\n", 
        partner->name, partner->dgmnAttributes.heart, partner->dgmnAttributes.heartBonus, partner->dgmnAttributes.brawn, partner->dgmnAttributes.brawnBonus, partner->dgmnAttributes.wits, partner->dgmnAttributes.witsBonus, partner->dgmnAttributes.grace, partner->dgmnAttributes.graceBonus, evasion, toughness);
    if(partner->specialAttacks){
        strcat(characterSheet, "\n----------Special Attack(s)----------\n");
        for (unsigned short i = 0; i < partner->numSpecialAttacks; i++){
            sprintf(temp, "Name: %s\tAccuracy: 2d%d\tPower: 2d%d\nBonus(es):\t%s\nDrawback(s):\t%s\nDescription:\t%s\n", partner->specialAttacks[i].name, partner->specialAttacks[i].accuracy, partner->specialAttacks[i].power, partner->specialAttacks[i].bonuses, partner->specialAttacks[i].drawbacks, partner->specialAttacks[i].description);
            strcat(characterSheet, temp);
        }
    }
    if(partner->abilities){
        strcat(characterSheet, "\n----------Abilities----------\n");
        for (unsigned short i = 0; i < partner->numAbilities; i++){
            sprintf(temp, "%s:\t%s\n", partner->abilities[i].name, partner->abilities[i].description);
            strcat(characterSheet, temp);
        } 
    }
    if(partner->dgmnGear){
        strcat(characterSheet, "\n----------Gear----------\n");
        for (unsigned short i = 0; i < partner->numGear; i++){
            sprintf(temp, "%s:\t%s\n", partner->dgmnGear[i].name, partner->dgmnGear[i].description);
            strcat(characterSheet, temp);
        }
    }

    if(partner->skills){
        strcat(characterSheet, "\n----------Skills----------\n");
        for (unsigned short i = 0; i < partner->numSkills; i++){
            sprintf(temp, "%s:\t%d\n", partner->skills[i].name, partner->skills[i].bonus);
            strcat(characterSheet, temp);
        }
    }
    free(temp);
    return characterSheet;
}
