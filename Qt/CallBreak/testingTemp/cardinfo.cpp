#include "cardinfo.h"


#include<QDebug>
#include <algorithm>
#include <random>

CardInfo::CardInfo() {
    shuffleCard();
}

void CardInfo::shuffleCard()
{
    // qDebug()<<"Deck before shuffle: "<<deck;
    // Seed with a real random value, if available
    std::random_device rd;

    // Choose a random number generator (Mersenne Twister)
    std::mt19937 g(rd());

    // Shuffle the QVector
    std::shuffle(deck.begin(), deck.end(), g);

    // qDebug()<<"deck after shuffle: "<<deck;
}

void CardInfo::distributeCards(int arr[], int playerNumber)
{
    //qDebug()<<"Welcome to Distribute cards"  << deck.size();
    int k=0;

    for(int i=playerNumber; i<deck.size(); i+=4)
    {
        //qDebug()<<"deck[i]: "<<deck[i];
        arr[k]=deck[i];
        k++;
        if(k==13)
        {
            break;
        }
    }

}
