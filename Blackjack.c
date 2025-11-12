#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>                //Used for srand((unsigned) time(NULL)) command
#include <process.h>             //Used for system("cls") command
#include <locale.h>
#include <wchar.h>  // For wprintf and wchar_t

#define RANKING 13
#define SUIT    4
#define spade   06                 //Used to print spade symbol
#define club    05                  //Used to print club symbol
#define diamond 04               //Used to print diamond symbol
#define heart   03                 //Used to print heart symbol
#define RESULTS "Blackjack.txt"  //File name is Blackjack
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

void PrintFaceDown(){
    setlocale(LC_ALL, "en_US.UTF-8");
    
}

int Pool[SUIT][RANKING] = 
{
    {1,2,3,4,5,6,7,8,9,10,11,12,13}, //SPADE 1
    {1,2,3,4,5,6,7,8,9,10,11,12,13}, //HEART 2
    {1,2,3,4,5,6,7,8,9,10,11,12,13}, //DIAMOND 3
    {1,2,3,4,5,6,7,8,9,10,11,12,13}, //CLUBS 4
};

Card DrawCards(){
    Card Card;
    int PICK_SUIT = rand() % 3;
    int PICK_RANK = rand() % 12;
    if (Pool[PICK_SUIT][PICK_RANK] != 0){
        Card.Rank = Pool[PICK_SUIT][PICK_RANK];
        Card.Suit = PICK_SUIT+1;
        Pool[PICK_SUIT][PICK_RANK] = 0; //Set to 0 so that we know its been taken
        return Card;
    } else {
        return DrawCards();             //Retry and draw another when it is 0
    }
}
void Start_Game(){
    //Handles all the initializations needed
    Player.Cards = (Card*)malloc(2*sizeof(Card));
    for(int i = 0; i < 2; i++){
        Player.Cards[i] = DrawCards();
    }
    Player.DeckSize = 2;

    Dealer.Cards = (Card*)malloc(2*sizeof(Card));
    for(int i = 0; i < 2; i++){
        Dealer.Cards[i] = DrawCards();
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


void Hit(){
    //This creates a temporary hand that is a copy of the player's hand
    //A single new card is then added to the new hand before the temporary hand replaces the player's hand
    printf("\nYou chose to hit\n");
    Hand Temp;
    Temp.Cards = (Card*)malloc((Player.DeckSize+1)*sizeof(Card));
    memcpy(Temp.Cards, Player.Cards, sizeof(Card)*Player.DeckSize);
    Temp.Cards[Player.DeckSize] = DrawCards();
    Temp.DeckSize = Player.DeckSize+1;
    free(Player.Cards);
    Player = Temp;
    Update_PLR_Points();
    //End of drawing

    printf("You drew\n");
    Single_PrintCard(Player.Cards[Player.DeckSize-1]);
    printf("Worth %i\n", CheckCardWorth(Player.Cards[Player.DeckSize-1]));

    if (Player.Points > 21){
        printf("Unfortunately you totalled %i, Bust!\n", Player.Points);
        system("pause");
        exit(0);
    } else {
        printf("Fortune has certainly smiled on you\n");
    }
}

void Stay(){
    Hand Temp;
    Temp.Cards = (Card*)malloc((Dealer.DeckSize+1)*sizeof(Card));
    memcpy(Temp.Cards, Dealer.Cards, sizeof(Card)*Dealer.DeckSize);
    Temp.Cards[Dealer.DeckSize] = DrawCards();
    Temp.DeckSize = Dealer.DeckSize+1;
    free(Dealer.Cards);
    Dealer = Temp;
    Update_DLR_Points();

    printf("Dealer drew\n");
    Single_PrintCard(Dealer.Cards[Dealer.DeckSize-1]);
    printf("Worth %i\n", CheckCardWorth(Dealer.Cards[Dealer.DeckSize-1]));
    printf("\n\n\n");
    printf("/////////////////////////////////////////////////\n");
    printf("    Dealer's Hand: (%ipt)  ", Dealer.Points);
    PrintCard(Dealer);
    printf("\n\n\n");
    printf("    Player's Hand: (%ipt)  ", Player.Points);
    PrintCard(Player);

    if (Dealer.Points >= 17 && Dealer.Points < 22) {
        //
        if (Dealer.Points > Player.Points){
            printf("Dealer has won!\n", Player.Points);
            system("pause");
            exit(0);
        } else if (Dealer.Points == Player.Points) {
            printf("Push! it is a stalemate!\n", Player.Points);
            system("pause");
            exit(0);
        } else if (Dealer.Points < Player.Points){
            printf("You won!", Player.Points);
            system("pause");
            exit(0);
        }
    } else if (Dealer.Points > 21) {
        printf("Dealer Bust! You won!\n", Player.Points);
        system("pause");
        exit(0);  
    } else if ( Dealer.Points < 17) {
        Stay();
    }

}

    void GameLoop(){
        printf("\n\n\n");
        printf("/////////////////////////////////////////////////\n");
        printf("    Dealer's Hand:         ");
        PrintCardDealerFirst();
        printf("\n\n\n");
        printf("    Player's Hand: (%ipt)  ", Player.Points);
        PrintCard(Player);
        printf("Choose your action: (s)Stay / (h)Hit ");
        char Input;
        scanf("%c", &Input);

        switch (Input){
            case 'h':
                Hit();
                GameLoop();
                break;
            case 's':

                printf("\nYou chose to Stay\n");
                printf("The Dealer will now reveal his second card\n\n");
                printf("    Dealer's Hand: (%ipt)  ", Dealer.Points);
                PrintCard(Dealer);
                printf("\n/////////////////////////////////////////////////\n");
                printf("The Dealer will now draw his cards\n");
                Stay();
                exit(0);
            default:
                printf("Invalid input!");
                GameLoop();
                break;
        }
    }


int main(){
    srand(time(NULL));
    printf("    Game has started!   \n\n\n");
    printf("    Player and Dealer will now draw two cards each!   \n\n\n");
    Start_Game();
    GameLoop();
}