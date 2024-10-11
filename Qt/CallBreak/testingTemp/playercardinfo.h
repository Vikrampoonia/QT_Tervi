#ifndef PLAYERCARDINFO_H
#define PLAYERCARDINFO_H

#include <vector>
#include <QDebug>
#include "cardinfo.h"

void initialiseArrays(int* , int , int );  //function initialsing arrays
void initialiseArrays(int* , int ,int , int ); //function initialise color card array
float findActualSet(int ,int ); //find actual set mean AND NoSet= Number of set made by player in that round
void helper_Of_set_Initial_And_FinalIndex(int* , int* , int* , int ); //used to reduce number of codes lines of set_Initial and FinalIndex function
int helper_Of_suggestionSet(int* , int* , int* ,int* , int );
void sortCards(int []);
void MergeArray(int [],int , int , int );
void divideArray(int [], int , int );


class playerCardInfo
{

    int playerCard[13]; //cards array

    int progressingSet; //track number of set in current round made by player

    std::vector<float>scoreCard; //containg score of each round

    //arrays used to tell valid card so it help to reduce time complixity take reference from playerCard
    //containing inital and final index of each color
    int colorCard[4][2];

    //containing info of king and ace containing or not of each color 1=containing  0=Not containing
    int kingAndAce[4][2];


public:

    playerCardInfo();

    int set;

    //initialise member of class
    void initialiseMembers();

    // used to allocate cards into playerCard array and map it with card Color starting of each round
    void setCard(int ,CardInfo* );

    //used to set value -1 to tell that card is thrown
    void setCard(int ,int );

    //set value -1 with the help of value
    void setCard(int );

    // return value associated with that card and take input from user
    int getCard(int );

    void sortCard(); //to invoke cardInfo sort array


    //set value of each card start and final index in those array value allocate in starting of each round and use in show valid card
    void set_Initial_And_FinalIndex();

    //Update value of each card start and final index in those array value allocate after starting of each round means when initil or final index card thrown then update its value
    void set_Initial_And_FinalIndex(int );

    // get initial and final index
    int get_Initial_And_FinalIndex(int , int );

    //update progressing Set
    void updateProgressingSet();

    //set current number of set of each round
    void setScoreCard(int );

    //return progressing set
    int getProgressingSet();

    //update after roundComplete (totalscore,scorecard)
    void update_Details_Round(int );

    //to get scorecard value
    float getScoreCard(int );

    //give suggestion that how many set you can made
    int suggestionSet();

    // void showInitialIndex();


    // return whether king or ace is present or not index for king or ace
    int getKingAndAce(int ,int );

    // set value of king and ace
    void setKingAndAce(int ,int );

    ~playerCardInfo() {}
};

#endif // PLAYERSINFO_H
