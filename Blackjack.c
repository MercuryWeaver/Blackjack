#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>                //Used for srand((unsigned) time(NULL)) command
#include <process.h>             //Used for system("cls") command
#include <locale.h>
#include <wchar.h>  // For wprintf and wchar_t
#include <string.h>  // Added for memcpy
#include <windows.h>

#define RANKING 13
#define SUIT    4
#define spade   06                 //Used to print spade symbol
#define club    05                  //Used to print club symbol
#define diamond 04               //Used to print diamond symbol
#define heart   03                 //Used to print heart symbol
#define BLKB "\e[40m"

//Initialize
typedef struct {
    int Rank;
    int Suit;
} Card;

typedef struct {
    Card *Cards;
    int DeckSize;
    int Points;
}  Hand;

//Global Vars
Hand Player;
Hand Dealer;
const char* suits[] = {"♠", "♥", "♦", "♣"};
const char* ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

// Function to reset the card pool
void ResetPool(int pool[SUIT][RANKING]) {
    for (int i = 0; i < SUIT; i++) {
        for (int j = 0; j < RANKING; j++) {
            pool[i][j] = j + 1;
        }
    }
}

void PrintCard(Hand Hand) {
    setlocale(LC_ALL, "en_US.UTF-8");
    for (int i = 0; i < Hand.DeckSize; i++){
        printf("[%s%s]  ", suits[Hand.Cards[i].Suit], ranks[Hand.Cards[i].Rank-1]);
    }
    printf("\n\n\n");
}

//This only takes a single card as param
void Single_PrintCard(Card Card){
    setlocale(LC_ALL, "en_US.UTF-8");
    printf("[%s%s]  ", suits[Card.Suit], ranks[Card.Rank-1]);
}

void PrintCardDealerFirst() {
    setlocale(LC_ALL, "en_US.UTF-8");
    const char* suits[] = {"♠", "♥", "♦", "♣"};
    const char* ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};
    printf("[%s%s]  ", suits[Dealer.Cards[0].Suit], ranks[Dealer.Cards[0].Rank-1]);
    printf("[??]");
    printf("\n\n\n");
}

// Function to display the current game state
void DisplayGameState(int showDealerFullHand) {
    system("cls");
    printf("\n\n\n");
    printf("/////////////////////////////////////////////////\n");
    
    if (showDealerFullHand) {
        printf("    Dealer's Hand: (%ipt)  ", Dealer.Points);
        PrintCard(Dealer);
    } else {
        printf("    Dealer's Hand:         ");
        PrintCardDealerFirst();
    }
    
    printf("\n\n\n");
    printf("    Player's Hand: (%ipt)  ", Player.Points);
    PrintCard(Player);
    printf("/////////////////////////////////////////////////\n");
}

Card DrawCards(int pool[SUIT][RANKING]){
    Card Card;
    int PICK_SUIT, PICK_RANK;
    
    do {
        PICK_SUIT = rand() % SUIT;
        PICK_RANK = rand() % RANKING;
    } while (pool[PICK_SUIT][PICK_RANK] == 0);
    
    Card.Rank = pool[PICK_SUIT][PICK_RANK];
    Card.Suit = PICK_SUIT;
    pool[PICK_SUIT][PICK_RANK] = 0; //Set to 0 so that we know its been taken
    return Card;
}

void Start_Game(int pool[SUIT][RANKING]){
    // Free existing cards if any
    if (Player.Cards != NULL) {
        free(Player.Cards);
    }
    if (Dealer.Cards != NULL) {
        free(Dealer.Cards);
    }
    
    //Handles all the initializations needed
    Player.Cards = (Card*)malloc(2*sizeof(Card));
    for(int i = 0; i < 2; i++){
        Player.Cards[i] = DrawCards(pool);
    }
    Player.DeckSize = 2;

    Dealer.Cards = (Card*)malloc(2*sizeof(Card));
    for(int i = 0; i < 2; i++){
        Dealer.Cards[i] = DrawCards(pool);
    }
    Dealer.DeckSize = 2;

    Player.Points = 0;
    Dealer.Points = 0;
    for (int i = 0; i < Player.DeckSize; i++){
        if (Player.Cards[i].Rank < 11 && Player.Cards[i].Rank != 1){
            Player.Points += Player.Cards[i].Rank;
        } else if (Player.Cards[i].Rank == 1){
            Player.Points += 11;
        } else {
            Player.Points += 10;
        }
    }
    for (int i = 0; i < Dealer.DeckSize; i++){
        if (Dealer.Cards[i].Rank < 11 && Dealer.Cards[i].Rank != 1){
            Dealer.Points += Dealer.Cards[i].Rank;
        } else if (Dealer.Cards[i].Rank == 1){
            Dealer.Points += 11;
        } else {
            Dealer.Points += 10;
        }
    }
}

int CheckCardWorth(Card Card){
    int Points = 0;
        if (Card.Rank < 11 && Card.Rank != 1){
            Points += Card.Rank;
        } else if (Card.Rank == 1){
            Points += 11;
        } else {
            Points += 10;
        }
    return Points;
}

//Updating points
void Update_PLR_Points(){
    Player.Points = 0;
    for (int i = 0; i < Player.DeckSize; i++){
        if (Player.Cards[i].Rank < 11 && Player.Cards[i].Rank != 1){
            Player.Points += Player.Cards[i].Rank;
        } else if (Player.Cards[i].Rank == 1){
            Player.Points += 11;
        } else {
            Player.Points += 10;
        }
    }
}

void Update_DLR_Points(){
    Dealer.Points = 0;
    for (int i = 0; i < Dealer.DeckSize; i++){
        if (Dealer.Cards[i].Rank < 11 && Dealer.Cards[i].Rank != 1){
            Dealer.Points += Dealer.Cards[i].Rank;
        } else if (Dealer.Cards[i].Rank == 1){
            Dealer.Points += 11;
        } else {
            Dealer.Points += 10;
        }
    }
}

void Hit(int pool[SUIT][RANKING]){
    //This creates a temporary hand that is a copy of the player's hand
    //A single new card is then added to the new hand before the temporary hand replaces the player's hand
    printf("\nYou chose to hit\n");
    Sleep(1000);
    
    Hand Temp;
    Temp.Cards = (Card*)malloc((Player.DeckSize+1)*sizeof(Card));
    memcpy(Temp.Cards, Player.Cards, sizeof(Card)*Player.DeckSize);
    Temp.Cards[Player.DeckSize] = DrawCards(pool);
    Temp.DeckSize = Player.DeckSize+1;
    free(Player.Cards);
    Player = Temp;
    Update_PLR_Points();
    //End of drawing

    DisplayGameState(0); // Show dealer's first card only
    printf("You drew: ");
    Single_PrintCard(Player.Cards[Player.DeckSize-1]);
    printf(" (Worth %i points)\n\n", CheckCardWorth(Player.Cards[Player.DeckSize-1]));
    Sleep(1500);
    if (Player.Points > 21){
        printf("Unfortunately you totalled %i, Bust!\n", Player.Points);
    } else {
        printf("Your total is now %i points\n", Player.Points);
    }
}

void Stay(int pool[SUIT][RANKING]){
    // First reveal dealer's second card
    DisplayGameState(1); // Show dealer's full hand
    printf("Dealer reveals the second card\n\n");
    Sleep(2000);
    
    while (Dealer.Points < 17) {
        Hand Temp;
        Temp.Cards = (Card*)malloc((Dealer.DeckSize+1)*sizeof(Card));
        memcpy(Temp.Cards, Dealer.Cards, sizeof(Card)*Dealer.DeckSize);
        Temp.Cards[Dealer.DeckSize] = DrawCards(pool);
        Temp.DeckSize = Dealer.DeckSize+1;
        free(Dealer.Cards);
        Dealer = Temp;
        Update_DLR_Points();

        DisplayGameState(1); // Show dealer's full hand
        printf("Dealer drew: ");
        Single_PrintCard(Dealer.Cards[Dealer.DeckSize-1]);
        printf(" (Worth %i points)\n\n", CheckCardWorth(Dealer.Cards[Dealer.DeckSize-1]));
        Sleep(2000);

        if (Dealer.Points > 21) {
            printf("Dealer Bust! You won!\n");
            return;
        }
    }

    DisplayGameState(1); // Final state
    if (Dealer.Points >= 17 && Dealer.Points < 22) {
        if (Dealer.Points > Player.Points){
            printf("Dealer has %i, you have %i. Dealer wins!\n", Dealer.Points, Player.Points);
        } else if (Dealer.Points == Player.Points) {
            printf("Both have %i. Push! It's a tie!\n", Player.Points);
        } else if (Dealer.Points < Player.Points){
            printf("You have %i, dealer has %i. You win!\n", Player.Points, Dealer.Points);
        }
    }
}

int GameLoop(int pool[SUIT][RANKING]){
    DisplayGameState(0); // Show dealer's first card only    
    printf("Choose your action: (s)Stay / (h)Hit ");
    char Input;
    scanf(" %c", &Input);  // Added space before %c to consume any leftover newline

    switch (Input){
        case 'h':
            Hit(pool);
            if (Player.Points > 21) {
                return 1; // Game over - player bust
            }
            return GameLoop(pool); // Continue game
        case 's':
            printf("\nYou chose to Stay\n");
            Sleep(1000);
            Stay(pool);
            return 1; // Game over
        default:
            printf("Invalid input! Please try again.\n");
            Sleep(1000);
            return GameLoop(pool); // Continue game with new input
    }
}

void PlayGame() {
    int pool[SUIT][RANKING];
    ResetPool(pool);
    
    system("cls");
    printf("    Game has started!   \n\n\n");
    Sleep(1000);
    printf("    Player and Dealer will now draw two cards each!   \n\n\n");
    Sleep(1000);
    Start_Game(pool);
    GameLoop(pool);
}

int main(){
    srand(time(NULL));
    char playAgain;
    
    do {
        PlayGame();
        
        printf("\n\nWould you like to play again? (y/n): ");
        scanf(" %c", &playAgain);

    } while (playAgain == 'y' || playAgain == 'Y');
    
    // Free any remaining allocated memory
    if (Player.Cards != NULL) {
        free(Player.Cards);
    }
    if (Dealer.Cards != NULL) {
        free(Dealer.Cards);
    }
    
    printf("Thanks for playing!\n");
    return 0;
}