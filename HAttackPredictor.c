/*To calculate the probability of heart disease using various parameters like test results and lifestyle.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define MAXNUM 5            //Macro representing the total number of parameters to calculate probability

//variable declarations

float net;                  //The net probability
float prob[MAXNUM]={0};     //array to store the probabilities after using bayes theorem
float probT[MAXNUM+1]={0};  //array to store probability of cause occuring given that there has been heart attack
float probF[MAXNUM+1]={0};  //array to store probability of cause occuring given that there has not been a heart attack
int n=0;                    //total number of samples
int countT[MAXNUM+1]={0};   //countT is count of causes when there is a heart attack.
int countF[MAXNUM+1]={0};   //countF is count of causes when there is no heart attack.
char choice[MAXNUM];        //array to store the choices made by the user

//function declarations

void menu();                                       //function for the user interface
void input();                                      //function for user to input data to predict risk
void training(char *file);                         //function to train the program using sample data
void recalibration(char result);                   //function for machine learning
void display_parameters(float prob[]);             //function to display all the dependent probabilities
void result();                                     //function to calculate and display the result
float bayes(float probT,float probF,float probE);  //function to implement bayes theorem


void main(int argc,char *argv[])                   //Main
{
 training(argv[1]); 
 menu();
}

void menu()                                       //function for the user interface.
{
 int r=2;
 char decision,file[20];
 do
 {   
   if (r==2)
       printf("\n\n1. Predict risk\n2. Provide sample data through file\n3. Exit\n\n");
   r=0;
   scanf("%c",&decision);
   switch(decision)
   {
     case '1':input();
              r=2;
              break;
     case '2':printf("Enter the filename: ");
              scanf("%s",file);
              training(file);
              r=2;
              break;
     case '3':r=1;
              break;
   }
  }while(r==0 || r==2);
} 

void display_parameters(float prob[])
{
 printf("\n\nThe probabilities are:\n1. Due to cholestrol level: %f\n2. Due to family history: %f\n3. Due to lack of exercise: %f\n4. Due to consumption of tobacco: %f\n5. Due to high calorie intake: %f",prob[0],prob[1],prob[2],prob[3],prob[4]);
} 

void input()                                      //takes input from the user to predict the risk of heart disease
{ 
 printf("\n\nIs the patient's cholestrol level above 200 ? (y/n)");
 while ((choice[0] = getchar()) != 'y' && choice[0] != 'n');
 printf("\nHas anyone in the patients family suffered from heart disease ? (y/n) ");
 while ((choice[1] = getchar()) != 'y' && choice[1] != 'n');
 printf("\nDoes the patient exercise daily ? (y/n)");
 while ((choice[2] = getchar()) != 'y' && choice[2] != 'n');
 printf("\nHas the patient consumed a significant amount of tobacco ? (y/n)");
 while ((choice[3] = getchar()) != 'y' && choice[3] != 'n');
 printf("\nDoes the patient's diet consist of more than 2000 calories ? (y/n)");
 while ((choice[4] = getchar()) != 'y' && choice[4] != 'n');
 choice[2]= (choice[2]=='y'?'n':'y');             //Because exercise is positive trait and negation has to be taken
 result();
}

void result()
{
 float total1=0.0,total2=0.0;
 char res;
 for(int i=0;i<MAXNUM;i++) 
 {
     total1+=(choice[i]=='y'?log(probT[i]):log(1-probT[i]));  //log of the values is being added to prevent underflow
     total2+=(choice[i]=='y'?log(probF[i]):log(1-probF[i]));
 }
 net=bayes(pow(10,total1),pow(10,total2),probT[MAXNUM]);
 printf("\n\nThe probability of heart disease is %f %%",net*100);
 printf("\nIs the correct result available ?");
 while ((res = getchar()) != 'y' && res != 'n');
 if (res=='y')
 {
    printf("Did the patient suffer from a heart attack ?");
    while ((res = getchar()) != 'y' && res != 'n');
    recalibration(res);
 }
} 

void training(char *file)                         //To train the program with sample data
{
 char buffer[256],*info;
 int i=0; 
 FILE *fptr;
 if ((fptr=fopen(file,"r")) == NULL)
 {
    fprintf (stderr, "can't open %s\n", file);
    exit (-1);
 }
 while(feof(fptr)==0)
 { i=0;
   if (fgets(buffer,128,fptr) == NULL)            //breaks out of the loop if the file ends
       break;
   
   strtok(buffer,",");                            //counts the number of times the symptom occurs when heartattack occurs
   if(buffer[0]=='1') 
   {
      countT[MAXNUM]++; 
      while(i<(MAXNUM))
        { 
          info=strtok(NULL,",");
          if(*info=='1')
             countT[i]++; 
          i++; 
         }
      n++;
   }
   else if(buffer[0]=='0') 
   {
      countF[MAXNUM]++;                           //counts the number of times the symptom occurs when heartattack does not
      while(i<(MAXNUM))
        { 
          info=strtok(NULL,",");
          if(*info=='1')
             countF[i]++; 
          i++; 
         }
      n++;
   }
 }
 fclose(fptr);
 printf("Count\nCholestrol\tHeredity\tNo Exercise\tTobacco\t\tCalories\nTrue\tFalse\tTrue\tFalse\tTrue\tFalse\tTrue\tFalse\tTrue\tFalse\n");
 for(i=0;i<MAXNUM;i++)
 {
    probT[i]=(float)(1+countT[i])/(float)(2+countT[MAXNUM]); //1 is added in the numerator and 2 in the denominator to avoid NULL 
    probF[i]=(float)(1+countF[i])/(float)(2+countF[MAXNUM]); //if countT[i] is zero
    printf("%d\t %d\t",countT[i],countF[i]);
 }
 printf("\n\nCount of\nHeart attack No Heart Attack\n%d\t\t%d\n",countT[MAXNUM],countF[MAXNUM]);
 probT[MAXNUM]=(float)countT[MAXNUM]/(float)n;
 probF[MAXNUM]=1-probT[MAXNUM];
 for(int i=0;i<MAXNUM;i++)
      prob[i]=bayes(probT[i],probF[i],probT[MAXNUM]);
 display_parameters(prob);
} 

void recalibration(char result) //function to recalibrate the probability
{
 if(result=='y')
   {
    countT[MAXNUM]++,n++;
    for(int i=0;i<MAXNUM;i++)
       if(choice[i]=='y') 
       {
          countT[i]++;  
          probT[i]=(float)(countT[i]+1)/(float)(2+countT[MAXNUM]);
       }
       else if(choice[i]=='n')
          probT[i]=(float)(countT[i]+1)/(float)(2+countT[MAXNUM]);
     probT[MAXNUM]=(float)countT[MAXNUM]/(float)n;
     probF[MAXNUM]=1.0-probT[MAXNUM];
    }
  else if(result=='n')
   {
    countF[MAXNUM]++,n++;
    for(int i=0;i<MAXNUM;i++)
       if(choice[i]=='y') 
       {
          countF[i]++;  
          probF[i]=(float)(countF[i]+1)/(float)(2+countF[MAXNUM]);
       }
       else if(choice[i]=='n')
          probF[i]=(float)(countF[i]+1)/(float)(2+countF[MAXNUM]);
    probF[MAXNUM]=(float)countF[MAXNUM]/(float)n;
    probT[MAXNUM]=1.0-probF[MAXNUM];
   }
  for(int i=0;i<MAXNUM;i++)
      prob[i]=bayes(probT[i],probF[i],probT[MAXNUM]);
}


float bayes(float probT,float probF,float probE) //probT and probF probability and negation of the dependent event.
{
 float result;
 result= (probT*probE) / (probT*probE + probF*(1-probE));
 return result;
}

              
