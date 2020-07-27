#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#define MAX 200
#define true 1
typedef struct questions
{
    int line;
    char question[256];
    char optionA[256];
    char optionB[256];
    char optionC[256];
    char optionD[256];
    char ans[256];

} QUESTION;
char b[] = {'A', 'B', 'C', 'D'};
int sc; //for storing the amount of money scored by the player
char anss;
const char ESC = 27; //used in making text bold
void kbcHeader();
void screen1();                                                                                     //function to display main screen
void generateLineNum(int line[], int len);                                                          //function to generate line numbers for accessing file
void shuffle(int line[], int len);                                                                  //function to randomly shuffle line numbers
void readdata(int linenumber, char question[], char a[], char b[], char c[], char d[], char ans[]); //function to read qyestion,options and correct answer from file
int correct(int x);                                                                                 //function to check if answer is correct
int incorrect(int x);                                                                               //function to check if answer if incorrect and reduce the score to last intermediate level
void fifty();                                                                                       //function to implement 50-50 lifeline
void reset();                                                                                       //for resetting b[] so it can be used again while implementing 50-50
int valid(char *answer);                                                                            //to check whther the option entered by user is valid or not
QUESTION qarray[16];                                                                                //16 questions - 1 extra for FLIP THE QUESTION LIFELINE
int main()
{
    do
    {
        sc = 0; //reset score to 0 before every round
        system("clear"); //to clear screen
        kbcHeader();
        screen1();
        char m;
        scanf("%c", &m);
        system("clear");
        char str[4];
        int que = 0, flag1 = 0, flag2 = 0;/*que is used to store question number being given to player, flag1 is used for checking condition of fifty fifty and flag2 for flip the question*/
        int flag = 0; //to check whether total questions have been asked or wrong answer was entered or all questions were answered correctly
        int quit = 0;
        int linenum[120] = {0}; //array to store line numbers of questions as saved in input file
        generateLineNum(linenum, sizeof(linenum) / sizeof(linenum[0]));
        shuffle(linenum, sizeof(linenum) / sizeof(linenum[0]));
        //loop to store line number of questions
        for (int i = 0; i < 16; i++)
        {
            qarray[i].line = linenum[i];
        }
        //loop to read data into the array of questions
        for (int i = 0; i < 16; i++)
        {
            readdata(qarray[i].line, qarray[i].question, qarray[i].optionA, qarray[i].optionB, qarray[i].optionC, qarray[i].optionD, qarray[i].ans);
        }
        char answer[4];
        // loop to display questions
        for (int i = 0; i < 16 && que <=15; i++)
        {
            que += 1;
            system("clear");
            kbcHeader();
            printf("MONEY WON: %d\n",sc);
            if (flag1 == 0)
            {
                printf("PRESS 1 TO USE 50-50\n");
            }
            if (flag2 == 0)
            {
                printf("PRESS 2 TO USE FLIP THE QUESTION\n");
            }
            if (flag1 == 1)
            {
                printf("YOU HAVE USED 50-50 LIFELINE\n");
            }
            if (flag2 == 1)
            {
                printf("YOU HAVE USED FLIP THE QUESTION LIFELINE\n");
            }
            printf("Type ");
            printf("%c[1m", ESC); /* turn on bold */
            printf(" QUIT");
            printf("%c[0m", ESC); /* turn off bold */
            printf(" to quit the game\n\n\n\n");
            printf("QUESTION %d: ", que);
            printf("%s", qarray[i].question);
            sleep(1);
            printf("%s", qarray[i].optionA);
            sleep(1);
            printf("%s", qarray[i].optionB);
            sleep(1);
            printf("%s", qarray[i].optionC);
            sleep(1);
            printf("%s", qarray[i].optionD);
            sleep(1);
            //implementing timer if question number is [1,5]
            if(que<=5)
            {
                printf("You have 45 seconds to answer the question. Answer entered after 45 seconds will not be considered and game will end.\n");
                sleep(2);
                printf("Your time starts now!\n");
            }
            printf("ENTER YOUR CHOICE:\n");
            time_t start = time(NULL); //starting the timer
            scanf("%s", answer);        
            while (valid(answer)) //prompting user to enter a valid option if option entered is invalid
            {
                printf("ENTER A VALID OPTION:\n");
                scanf("%s", answer);
            }
            //checking whether the user wants to quit the game. User will not be allowed to quit the game after time limit is crossed, he will simply lose his money.
            if (strcmp(answer, "QUIT") == 0 || strcmp(answer, "quit") == 0)
            {
                time_t end = time(NULL);
                if(end-start>45 && que<=5)
                {
                    sc = incorrect(que-1);
                    printf("Time limit exceeded. Game ends. You earned Rs. %d\n",sc);
                }
                quit = 1;
                break;
            }
            time_t end = time(NULL);
            if(end-start>45 && que<=5)
            {
                sc = incorrect(que-1);
                printf("Time limit exceeded. Game ends. You earned Rs. %d\n",sc);
                sleep(5);
                flag = 1;
                break;
            }
            //if the option is valid and time limit is not exceeded
            else
            {
                if (answer[0] == '1' && flag1 == 0) //implementing 50-50 and checking whether 50-50 has been used before or not
                {
                    printf("IMPLEMENTING 50-50 LIFELINE\n");
                    sleep(2);
                    anss = qarray[i].ans[0];
                    fifty();
                    for (int j = 0; j < 4; j++)
                    {
                        if (b[j] == ' ')
                            printf("%c)\n", j + 65);
                        else//printing two random options with one correct answer
                        {
                            if (j == 0)
                                printf("%s", qarray[i].optionA);
                            if (j == 1)
                            {
                                printf("%s", qarray[i].optionB);
                            }
                            if (j == 2)
                            {
                                printf("%s", qarray[i].optionC);
                            }
                            if (j == 3)
                            {
                                printf("%s", qarray[i].optionD);
                            }
                        }
                    }
                    scanf("\n%s", answer);
                    while (valid(answer)) //prompting user to enter a valid option if option entered is invalid
                    {
                        printf("ENTER A VALID OPTION:\n");
                        scanf("%s", answer);
                    }
                    time_t end = time(NULL);
                    if(end-start>50 && que<=5) //increasing the time limit by 5 seconds to compensate for time consumed in displaying and implementing
                    {
                        sc = incorrect(que-1);
                        printf("Time limit exceeded. Game ends. You earned Rs. %d\n",sc);
                        sleep(5);
                        flag = 1;
                        break;
                    }
                    flag1 = 1;
                }
                if (answer[0] == '1' && flag1 == 1)//checking if 50-50 was used before or not
                {
                    printf("50-50 already used\n");
                    printf("Please enter a valid choice:\n");
                    scanf("\n%s", answer);
                    while (valid(answer))
                    {
                        printf("ENTER A VALID OPTION:\n");
                        scanf("%s", answer);
                    }
                    time_t end = time(NULL);
                    if(end-start>45 && que<=5)
                    {
                        sc = incorrect(que-1);
                        printf("Time limit exceeded. Game ends. You earned Rs. %d\n",sc);
                        sleep(5);
                        flag = 1;
                        break;
                    }
                }

                if (answer[0] == '2' && flag2 == 0) //implementing flip the question and checking whether flip the question have been used before or not
                {
                    printf("IMPLEMENTING FLIP THE QUESTION...\n");
                    sleep(2);
                    i++;
                    que--;
                    flag2 = 1;//changing the value of flag2 for flagging the use of lifeline by the user
                    continue;
                }
                if (answer[0] == '2' && flag2 == 1)
                {
                    printf("FLIP THE QUESTION already used\n");
                    printf("Please enter a valid choice:\n");
                    scanf("\n%s", answer);
                    while (valid(answer))
                    {
                        printf("ENTER A VALID OPTION:\n");
                        scanf("%s", answer);
                    }
                    time_t end = time(NULL);
                    if(end-start>45 && que<=5)
                    {
                        sc = incorrect(que-1);
                        printf("Time limit exceeded. Game ends. You earned Rs. %d\n",sc);
                        sleep(5);
                        flag = 1;
                        break;
                    }
                }
                if (toupper(answer[0]) == qarray[i].ans[0]) //condition for checking correct answer
                {
                    if(que==15) //checking if the last question is correct
                    {
                        flag = 2;
                        sc = correct(que-1);
                        printf("CONGRATULATIONS! YOU HAVE HAVE WON Rs. %d AND WON THE GAME!! YOU ARE A COROREPATI NOW!!\n",sc);
                        sleep(5);
                        break;
                    }
                    else
                    {
                       //if any other question is answered correctly
                        flag = 0;
                        sc = correct(que - 1);
                        printf("CORRECT ANSWER! CONGRATULATIONS YOU HAVE WON Rs. %d \n ", sc);
                        sleep(5);

                    }
                    
                }
                else //incase of any wrong answer the round will end
                {
                    flag = 1;
                    sc = incorrect(que - 1);
                    printf("WRONG ANSWER! The correct answer is %s\n", qarray[i].ans);
                    printf("You have earned Rs. %d\n", sc);
                    sleep(5);
                    break;
                }
            }
        }
        system("clear");
        if (quit) //user quit the last round
        {
            reset();
            kbcHeader();
            printf("YOU QUIT THE LAST GAME. YOU WON Rs.%d\n", sc);
            printf("IF YOU WANT TO END THE GAME TYPE ");
            printf("%c[1m", ESC); /* turn on bold */
            printf("END");
            printf("%c[0m", ESC); /* turn off bold */
            printf(" to exit the game else enter any other letter/word\n");
            scanf("%s", str);
            if (strcmp(str, "END") == 0 || strcmp(str, "end") == 0)
            {
                system("clear");
                break;
            }
            else
            {
                continue;
            }
        }
        else if (flag==1) //user entered wrong answer and lost the last round
        {
            reset();
            kbcHeader();
            printf("YOU LOST THE LAST GAME. DO YOU WANT TO PLAY ANOTHER ROUND OR END THE GAME?\n");
            printf("TYPE ");
            printf("%c[1m", ESC); /* turn on bold */
            printf("END");
            printf("%c[0m", ESC); /* turn off bold */
            printf(" to exit the game else enter any other letter/word\n");
            scanf("%s", str);
            if (strcmp(str, "END") == 0 || strcmp(str, "end") == 0)
            {
                system("clear");
                break;
            }
            else
            {
                continue;
            }
        }
        else if(flag==2) //user answered all 15 questions correctly
        {
            reset();
            kbcHeader();
            printf("CONGRATULATIONS!! YOU ANSWERED ALL QUESTIONS CORRECTLY IN THE LAST ROUND!\n");
            printf("DO YOU WANT TO PLAY ANOTHER ROUND OR END THE GAME?\n");
            printf("TYPE ");
            printf("%c[1m", ESC); /* turn on bold */
            printf("END");
            printf("%c[0m", ESC); /* turn off bold */
            printf(" to exit the game else enter any other letter/word\n");
            scanf("%s", str);
            if (strcmp(str, "END") == 0 || strcmp(str, "end") == 0)
            {
                system("clear");
                break;
            }
            else
            {
                continue;
            }
            

        }
    } while (true); //ensures game is played atleast once
    return 0;
}

void kbcHeader()
{
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
    printf("%c[1m", ESC); /* turn on bold */
    printf("\t\t\t\t\t\t\t\t\t\t\tKAUN BANEGA CROREPATI\n");
    printf("%c[0m", ESC); /* turn off bold */
    printf("------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");
}
void screen1()
{
    printf("%c[1m", ESC); /* turn on bold */
    printf("\t\t\t\t\t\t\t\t\t\t\t  RULES OF THE GAME\n");
    printf("%c[0m", ESC); /* turn off bold */
    sleep(2);
    printf("\t1.There are total 15 questions in this game and the prize money corresponding to each question is :\n");
    sleep(1);
    printf("%c[1m", ESC); /* turn on bold */
    printf("\t\t#Q1 - Rs 1000 \n");
    printf("%c[0m", ESC); /* turn off bold */
    sleep(1);
    printf("\t\tQ2 - Rs 2000 \n");
    printf("\t\tQ3 - Rs 3000 \n");
    printf("\t\tQ4 - Rs 5000 \n");
    printf("%c[1m", ESC); /* turn on bold */
    printf("\t\t#Q5 - Rs 10,000 \n");
    printf("%c[0m", ESC); /* turn off bold */
    sleep(1);
    printf("\t\tQ6 - Rs 20,000 \n");
    printf("\t\tQ7 - Rs 40,000 \n");
    printf("\t\tQ8 - Rs 80,000 \n");
    printf("\t\tQ9 - Rs 1,60,000 \n");
    printf("%c[1m", ESC); /* turn on bold */
    printf("\t\t#Q10 - Rs 3,20,000 \n");
    printf("%c[0m", ESC); /* turn off bold */
    sleep(1);
    printf("\t\tQ11 - Rs 6,40,000 \n");
    printf("\t\tQ12 - Rs 12,50,000 \n");
    printf("\t\tQ13 - Rs 25,00,000 \n");
    printf("\t\tQ14 - Rs 50,00,000 \n");
    printf("\t\tQ15 - Rs 1,00,00,000 \n");
    sleep(1);
    printf("\t2.The player has 2 lifelines in each game :50-50 and FLIP THE QUESTION. They can be used in same question or different questions or not at all!\n");
    sleep(1);
    printf("\t\t50-50: Two options of the same question can be chosen \n");
    sleep(1);
    printf("\t\tFLIP THE QUESTION: Current question will be replaced by a new question \n");
    sleep(1);
    printf("\t3.YOU CAN USE THE LIFELINES BY PRESSING 1 FOR 50-50 AND 2 FOR FLIP THE QUESTION\n");
    sleep(1);
    printf("\t4. There are three intermediate levels in the game: Q1, Q5 and Q10.\n");
    sleep(1);
    printf("\t5. For every correct answer the prize money corresponding the question will be added to your final total.\n");
    sleep(1);
    printf("\t6. 1st 5 questions are time based. The time limit is ");
    printf("%c[1m", ESC);
    printf("45");
    printf("%c[0m", ESC);
    printf(" seconds. the player will be able to enter answer after 45 seconds but it won't be evaluated and the game will end. In case of using \n\t\tlifelines, the time limit is adjusted accordingly.\n");
    sleep(1);
    printf("\t7. If the option selected by the user is incorrect the game will exit and the final amount won by the player will be corresponding to the last intermediate level crossed by him/her.\n");
    sleep(1);
    printf("\t8. Only enter the option number (A,B,C or D) in any case. Any other format of answer will not be accepted.If an valid answer is entered, the user will be prompted to answer again until a valid\n\t\tanswer is entered. After 50-50 implementation it is considered obvious that user will not enter any of the wrong answers. If he/she still does they will be considered incorrect.\n");
    sleep(2);
    printf("\n\n");
    printf("%c[1m", ESC); /* turn on bold */
    printf("\t\t\t\t\t\t\t\t\t\t\t Press ANY key to conitnue!\n");
    printf("%c[0m", ESC); /* turn off bold */
}

void generateLineNum(int line[], int len)
{
    line[0] = 0;
    for (int i = 1; i < len; i++)
    {
        line[i] = line[i - 1] + 6;
    }
}

void shuffle(int line[], int len)
{
    srand(time(NULL)); 
    //swapping random numbers in array
    for (int i = 0; i < len; i++)
    {
        int j = i + rand() % (120 - i);
        int temp = line[i];
        line[i] = line[j];
        line[j] = temp;
    }
}

void readdata(int linenumber, char question[256], char a[256], char b[256], char c[256], char d[256], char ans[256])
{
    FILE *fp;
    fp = fopen("KBC QUESTION BANK.txt", "r");
    if (fp == NULL)
    {
        printf("Oops! Unable to open file. Make sure the source code and file are saved in the same folder and try again!");
        return;
    }
    else
    {
        int count = 1;
        char text[256];
        while (count <= (linenumber + 6))
        {
            fgets(text, 256, fp);
            if (count == linenumber + 1)
                strcpy(question, text);
            else if (count == linenumber + 2)
                strcpy(a, text);
            else if (count == linenumber + 3)
                strcpy(b, text);
            else if (count == linenumber + 4)
                strcpy(c, text);
            else if (count == linenumber + 5)
                strcpy(d, text);
            else if (count == linenumber + 6)
                strcpy(ans, text);
            count++;
        }
        fclose(fp);
        return;
    }
}

int correct(int x)
{
    int score;
    int money[15] = {1000, 2000, 3000, 5000, 10000, 20000, 40000, 80000, 160000, 320000, 640000, 1250000, 2500000, 5000000, 100000000};     
    if (x < 16)
    {
        score = money[x];       // loop for increasing prize money on every correct answer
    }
    return score;
}

int incorrect(int x)
{
    int score;
    int money[15] = {1000, 2000, 3000, 5000, 10000, 20000, 40000, 80000, 160000, 320000, 640000, 1250000, 2500000, 5000000, 100000000};     
    if (x == 0)
        score = 0;          
    else if (x < 5)
    {
        score = money[0];       // money won if correctly answered ques<5   
    }
    else if (x < 10)
    {
        score = money[4];       // money won if correctly answered ques<10
    }
    else
        score = money[9];       // money won if correctly answered ques>=10

    return score;
}

void fifty()
{
    int x = anss - 65;
    int i = 0;
    while (i != 2)
    {
        srand(time(NULL));
        int r = rand();
        r = r % 4;
        if (r != x && b[r] != ' ')
        {
            b[r] = ' ';
            i++;
        }
    }
}
void reset()
{
    int v = 0;
    for (v= 0; v < 4; v++)
        b[v] = v+ 65;
}

int valid(char *answer)
{
    if (answer[0] == 'a' || answer[0] == 'A')
        return 0;
    else if (answer[0] == 'b' || answer[0] == 'B')
        return 0;
    else if (answer[0] == 'c' || answer[0] == 'C')
        return 0;
    else if (answer[0] == 'd' || answer[0] == 'D')
        return 0;
    else if (answer[0] == '1' || answer[0] == '2')
        return 0;
    else if (strcmp(answer, "QUIT") == 0 || strcmp(answer, "quit") == 0)
        return 0;
    else
        return 1;
}