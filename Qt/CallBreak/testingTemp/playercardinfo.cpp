#include "playercardinfo.h"

playerCardInfo::playerCardInfo() {}





void playerCardInfo::initialiseMembers()
{
    //intialise variables and arrays=0; and each color array such as spadeCard,heartCard to be -1;
    progressingSet=0;

    //function initialsing arrays parameter as array,its length, value intialise (0/-1);
    initialiseArrays(&playerCard[0],13,0);
    initialiseArrays(&colorCard[0][0],4,2,-1);
    initialiseArrays(&kingAndAce[0][0],4,2,0);
}


void initialiseArrays(int* arr, int len, int value)  //function initialsing arrays
{
    for(int i=0; i<len; i++)
    {
        *(arr+i)=value;
    }
}


void initialiseArrays(int* arr, int len1,int len2, int value)  //color Card initialsing arrays
{
    for(int j=0; j<len1; j++)
    {
        for(int i=0; i<len2; i++)
        {
            *(arr+(j*len2)+i)=value;
        }
    }
}

//call at starting of game
void playerCardInfo::setCard(int playerNumber,CardInfo* c)
{
    qDebug()<<"PlayerNumber: "<<playerNumber;
    initialiseMembers();


    c->distributeCards(&playerCard[0], playerNumber); // function that fill cardvalues and this find in CardInfo class

    sortCard();

    // c->showCard(playerCard);

    set_Initial_And_FinalIndex();



}

void playerCardInfo::sortCard()
{
    sortCards(playerCard);
}


void playerCardInfo::setCard(int index,int value)
{
    playerCard[index]=value;
}



void playerCardInfo::setCard(int value)
{
    int color=(value-2)/13;

    int start=colorCard[color][0];
    int end=colorCard[color][1];

    for(int i=start; i<=end; i++)
    {
        if(playerCard[i]==value)
        {
            playerCard[i]=-1;
            i=end;
        }
    }
}



int playerCardInfo::getCard(int cardIndex)
{
    return playerCard[cardIndex];
}


void playerCardInfo::set_Initial_And_FinalIndex()
{
    initialiseArrays(&colorCard[0][0],4,2,-1);
    initialiseArrays(&kingAndAce[0][0],4,2,0);
    int start=0;
    //int *p=&start;

    //this function return last index +1 of color for which we find
    helper_Of_set_Initial_And_FinalIndex(&start,&playerCard[0],&colorCard[0][0], 3); //for spade
    helper_Of_set_Initial_And_FinalIndex(&start,&playerCard[0],&colorCard[0][0], 2); //for heart
    helper_Of_set_Initial_And_FinalIndex(&start,&playerCard[0],&colorCard[0][0], 1); //for diamond
    helper_Of_set_Initial_And_FinalIndex(&start,&playerCard[0],&colorCard[0][0], 0); //for club



    //this function show only initial and final index of eaxh color
    //showInitialIndex();
}

void  helper_Of_set_Initial_And_FinalIndex(int* start, int* playerCard, int* colorCard, int color)
{
    int i=*(start);
    for(i=*(start); i<13; i++)
    {

        if(*(playerCard+i) >=(2+13*color) && *(playerCard+i) <=(2+13*color+12) )
        {
            if(*((colorCard+2*color)+0) == -1)
            {
                *((colorCard+2*color)+0) = i;
                *((colorCard+2*color)+1) = i;
            }
            else
            {
                if(*((colorCard+2*color)+1) <i)
                {
                    *((colorCard+2*color)+1) =i;
                }
            }

        }
        else
        {
            break;
        }
    }
    *(start)=i;

}


//index tell which card want to set, color which array, state means inital or final
void playerCardInfo::set_Initial_And_FinalIndex(int color)
{

    int min=colorCard[color][0],max=colorCard[color][1]; int flag=0;


    for(int i=colorCard[color][0]; i<=colorCard[color][1]; i++)
    {
        if(playerCard[i]!=-1)
        {
            if(min<=i && flag==0)
            {
                min=i;
                max=i;
                flag=1;
            }
            else if(max<i)
            {
                max=i;
            }
        }
    }
    if(min==colorCard[color][0] && (max==colorCard[color][1] && playerCard[min]==-1) )
    {
        max=-1;
        min=max;
    }
    colorCard[color][0]=min;
    colorCard[color][1]=max;

}

// get initial and final index
int playerCardInfo::get_Initial_And_FinalIndex(int color, int index)
{
    return colorCard[color][index];
}



void playerCardInfo::updateProgressingSet()
{
    progressingSet++;
}

int playerCardInfo::getProgressingSet()
{
    return progressingSet;
}

//store no of set in each round here roundNo is not used
void playerCardInfo::setScoreCard(int No_Set)
{
    scoreCard.push_back(No_Set);
    set=No_Set;
}



//to get scorecard value
float playerCardInfo::getScoreCard(int roundNo)
{
    return scoreCard[roundNo];
}




void playerCardInfo::update_Details_Round(int roundNo)
{
    float ActualSet=findActualSet(scoreCard[roundNo],progressingSet);
    scoreCard.push_back(ActualSet);
}


float findActualSet(int currentSet,int NoSet)
{

    float diff;
    if(NoSet>=currentSet)
    {
        diff=NoSet-currentSet;
        (diff>9)?diff/=100:diff/=10;
        diff=(float)(currentSet)+diff;
    }
    else
    {
        diff=-currentSet;
    }
    return diff;
}


int playerCardInfo::suggestionSet()
{

    //set_Initial_And_FinalIndex();

    int priorityCardSet=0; //Set we make with the help of priority Card
    int faceCardSet=0;

    //int No_Of_Spade_Card=0;

    //for club card
    if(colorCard[0][1]-colorCard[0][0] + 1 <=2)
    {

        priorityCardSet+=(3-(colorCard[0][1]-colorCard[0][0] + 1));
    }

    //for Diamond card
    if(colorCard[1][1]-colorCard[1][0] + 1 <=2)
    {

        priorityCardSet+=(3-(colorCard[1][1]-colorCard[1][0] + 1));
    }

    //for heart card
    if(colorCard[2][1]-colorCard[2][0] + 1 <=2)
    {

        priorityCardSet+=(3-(colorCard[2][1]-colorCard[2][0] + 1));
    }

    //for spade first check we have enough spade card or not if enough then check for faceCardSet

    if(colorCard[3][1]-colorCard[3][0] + 1 <=priorityCardSet)
    {

        priorityCardSet=colorCard[3][1]-colorCard[3][0] + 1;
    }
    else
    {
        for(int i=colorCard[3][0]; i<=(colorCard[3][1]-priorityCardSet); i++)
        {
            if(playerCard[i]==53 || playerCard[i]==52 || playerCard[i]==51)
            {
                faceCardSet++;
            }
            if(playerCard[i]==53)
            {
                kingAndAce[3][1]=1;
            }
            else if(playerCard[i]==52)
            {
                kingAndAce[3][0]=1;
            }
        }
    }

    //face card for other colors
    // for heart
    faceCardSet=helper_Of_suggestionSet(&faceCardSet, &playerCard[0], &colorCard[0][0],&kingAndAce[2][0],2);
    // for Diamond
    faceCardSet=helper_Of_suggestionSet(&faceCardSet, &playerCard[0], &colorCard[0][0],&kingAndAce[1][0], 1);
    // for club
    faceCardSet=helper_Of_suggestionSet(&faceCardSet, &playerCard[0], &colorCard[0][0],&kingAndAce[0][0], 0);


    return ((faceCardSet+priorityCardSet)==0)?1:(faceCardSet+priorityCardSet);
}



int helper_Of_suggestionSet(int* faceCardSet, int* playerCard, int* colorCard, int* kingAndAce,int color)
{

    for(int i=*((colorCard+2*color)+0); i<=*((colorCard+2*color)+1); i++)
    {

        if(*(playerCard+i)==(2+(13*color)+12)) //ace
        {

            *(faceCardSet)=*(faceCardSet)+1;
            *(kingAndAce+1)=1; // ace is present
        }
        else if(*(playerCard+i)==(2+(13*color)+11)) // king
        {
            *(kingAndAce+0)=1; //king is present
            if(*((colorCard+2*color)+1)-*((colorCard+2*color)+0) >5)
            {
                break;
            }
            else
            {
                if(*((colorCard+2*color)+1)-*((colorCard+2*color)+0) >=1)
                {
                    if(*(playerCard+i-1)==(2+(13*color)+12))
                    {

                        *(faceCardSet)=*(faceCardSet)+1;
                        break;
                    }
                    else
                    {
                        for(int j=i+1; j<*((colorCard+2*color)+1)+1; j++)
                        {
                            if(*(playerCard+j)>(2+(13*color)+7))
                            {
                                *(faceCardSet)=*(faceCardSet)+1;
                                break;
                            }
                        }
                    }
                }
            }
            break;
        }

    }
    return *faceCardSet;
}




int playerCardInfo::getKingAndAce(int color,int index)
{
    return kingAndAce[color][index];
}


void playerCardInfo::setKingAndAce(int color,int index)
{
    kingAndAce[color][index]=0;
}


void sortCards(int arr[]) //implementing merge sort algorithm
{
    int start=0, end=12;
    divideArray(arr, start, end);
}

void MergeArray(int arr[],int start, int mid, int end)
{
    int n1=mid-start+1;
    int n2=end-mid;

    int a1[n1];
    int a2[n2];

    for(int i=0; i<n1; i++)
    {
        a1[i]=arr[start+i];
    }

    for(int i=0; i<n2; i++)
    {
        a2[i]=arr[mid+i+1];
    }
    int k=start;
    int i=0,j=0;
    while(i<n1 && j<n2)
    {
        if(a1[i]>a2[j])
        {
            arr[k++]=a1[i++];
        }
        else
        {
            arr[k++]=a2[j++];
        }

    }

    while(i<n1)
    {
        arr[k++]=a1[i++];
    }
    while(j<n2)
    {
        arr[k++]=a2[j++];
    }
}

void divideArray(int arr[], int start, int end)
{

    if(start<end)
    {
        int mid=start+(end-start)/2;
        divideArray(arr, start, mid);
        divideArray(arr, mid+1, end);
        MergeArray(arr,start,mid,end);
    }

}

