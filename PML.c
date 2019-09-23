#include<stdio.h>
#include<math.h>
#include <time.h>
#include <stdlib.h>
int n,n1,n2[8],n3,nhl;
double w1[11][11],w2[8][11][11],w3[11];

void reTrain(void);
void train(void);
void print_weights();
double d_S(double x);
double S(double x);
void test();


int i,j,k,l;

double I[101][11],O[101],l1[101][11],l1_d[101][11],l2[8][101][11],l2_d[8][101][11],l3[101],l3_d[101],s,err,alpha=1;

int main()
{

	srand(time(NULL));
	printf("Enter number of neurons in input layer : ");
	scanf("%d",&n1);
	printf("Enter number of hidden layer : ");
	scanf("%d",&nhl);
	for(i=0;i<nhl;i++){
		printf("Enter number of neurons in hidden layer %d: ",i);
		scanf("%d",&n2[i]);
	}

	printf("Enter number of neurons in hidden Output layer : ");
	scanf("%d",&n3);
	printf("Enter number of entries : ");
	scanf("%d",&n); printf("\n");
	for(i=1;i<=n;i++)
	{
		printf("entry %d\n",i);
		for(j=1;j<=n1;j++)
		{
			printf("enter x[%d] : ",j);
			scanf("%lf",&I[i][j]);
		}
		printf("enter y : ");
		scanf("%lf",&O[i]); printf("\n");
	}
	
	reTrain();


	printf("\n\n");
	i=0;
	while(i!=6)
	{
		printf("1) Test\n2) See weights\n3) Continue training\n4) Retrain\n5) Check learning rate \n6) Exit");
		printf("\nenter choice : ");
		scanf("%d",&i);
		//if (GetAsyncKeyState(VK_UP)) {s=alpha*10; if(fabs(s)<11) {alpha=s;}}
		//else if (GetAsyncKeyState(VK_DOWN)) {s=alpha/10; if(fabs(s)>=pow(10,-15)) {alpha=s;}}
		if(i==1){test();}
		else if(i==2){print_weights();}
		else if(i==3){train();}
		else if(i==4){reTrain();}
		else if(i==5){printf("\nalpha=%0.15lf",alpha);}
		printf("\n\n");
	}
	getchar();
	return 0;
}

void reTrain(void){
    for(i=1;i<=n3;i++)
	{	
		for(j=1;j<=n1;j++)	//intializing weights randomly
		{
			w1[j][i]=(sqrt(-2.0*log((double)rand()/RAND_MAX)))*(cos(6.28318530718*(double)rand()/RAND_MAX));
		}
		for(j=0;j<nhl;j++){
			for(k=1;k<=n2[j];k++)	//intializing weights randomly
			{
				w2[j][k][i]=(sqrt(-2.0*log((double)rand()/RAND_MAX)))*(cos(6.28318530718*(double)rand()/RAND_MAX));
			}
		}

		w3[i]=(sqrt(-2.0*log((double)rand()/RAND_MAX)))*(cos(6.28318530718*(double)rand()/RAND_MAX));
	}
    train();
}

void train(void){
int compt=0;
err = 1;
    for(i=1;compt<=20;i++)
	{	
		for(j=1;j<=n;j++)	//n data entries
		{
			for(k=1;k<=n3;k++)	//forming l1
			{
				s=0;
				for(l=1;l<=n2[0/*couche*/];l++)
				{
				   
					s=s+(I[j][l]*w1[l][k]);
				}
				l1[j][k]=S(s);

			}
		}
		
		for(j=1;j<=n;j++)	//n data entries
		{
			for(k=1;k<=n3;k++)	//forming l2
			{
				s=0;
				for(l=1;l<=n2[0/*couche*/];l++)
				{
				   
					s=s+(l1[j][l]*w2[0/*couche*/][l][k]);
				}
				l2[0/*couche*/][j][k]=S(s);

			}
		}
		

		for(int jj=1;jj<nhl;jj++)	//n data entries
		{
			for(j=1;j<=n;j++)	//n data entries
			{
				for(k=1;k<=n3;k++)	//forming l2
				{
					s=0;
					for(l=1;l<=n2[jj];l++)
					{
						s=s+(l2[jj - 1][j][l]*w2[jj][l][k]);
					}
					l2[jj][j][k]=S(s);

				}
			}
		}
		err=0;
		
		for(j=1;j<=n;j++)	//n data entries
		{
			s=0;
			for(k=1;k<=n3;k++)	//forming l3
			{
				s=s+(l2[nhl - 1][j][k]*w3[k]);
			}
			l3[j]=S(s);
			err=err+fabs(l3[j]-O[j]);
			l3_d[j]=(l3[j]-O[j])*d_S(l3[j]); //forming l3 delta
		}
		
		if(i==100000) {printf("error : %0.15lf\n",err/n);i=0;compt++;} //mean absolute error
		
		if(nhl > 2){
			for(j=1;j<=n;j++)	//n data entries
			{
				for(k=1;k<=n2[nhl - 1];k++)	//forming l2 delta
				{
					l2_d[nhl - 1][j][k]=l3_d[j]*w3[k]*d_S(l2[nhl - 1][j][k]);
				}
			}
		}
		

		for(int jj = nhl - 2;jj > 1;jj--)	//n data entries
		{
			for(j=1;j<=n;j++)	//n data entries
			{
				for(k=1;k<=n2[jj];k++)	//forming l2 delta
				{
					l2_d[jj][j][k]=l2_d[jj][j][k] *w2[jj][j][k]*d_S(l2[jj][j][k]);
				}
			}//printf("flag\n");
		}//printf("\n");
		for(j=1;j<=n;j++)	//n data entries
		{
			for(k=1;k<=n2[0/*couche*/];k++)	//forming l2 delta
			{
				l2_d[0/*couche*/][j][k]=l2_d[0/*couche*/][j][k]*w2[1/*couche*/][j][k]*d_S(l2[0/*couche*/][j][k]);
			}
		}
		

		
		
		s=0;
		for(j=1;j<=n;j++)	//n data entries
		{
			for(k=1;k<=n1;k++)	//forming l1 delta
			{	for(l=1;l<=n2[0/*couche*/];l++)
				{
					s=s+l2_d[0/*couche*/][j][l]*w2[0/*couche*/][k][l] * d_S(l1[j][k]);
				}
				s=s/n2[0/*couche*/];
				l1_d[j][k]=s;
			}
		}
		for(j=1;j<=n1;j++)	//updating w1
		{
			for(k=1;k<=n2[0/*couche*/];k++)
			{
				s=0;
				for(l=1;l<=n;l++)
				{
					s=s+(I[l][j]*l1_d[l][k]);
				}
				w1[j][k]=w1[j][k]-(alpha*s);
			}
		}
		
		for(j=1;j<=n2[0/*couche*/];j++)	//updating w2
		{
			for(k=1;k<=n3;k++)
			{
				s=0;
				for(l=1;l<=n;l++)
				{
					s=s+(l1[l][j]*l2_d[0/*couche*/][l][k]);
				}
				w2[0/*couche*/][j][k]=w2[0/*couche*/][j][k]-(alpha*s);
			}
		}

		for(int jj= 1;jj<nhl;jj++)
		{
			for(j=1;j<=n2[jj];j++)	//updating w2
			{
				for(k=1;k<=n3;k++)
				{
					s=0;
					for(l=1;l<=n;l++)
					{
						s=s+(l2[jj - 1][l][j]*l2_d[jj][l][k]);
					}
					w2[jj][j][k]=w2[jj][j][k]-(alpha*s);
				}
			}
		}
		for(j=1;j<=n3;j++)	//updating w3
		{
			s=0;
			for(k=1;k<=n;k++)
			{
				s=s+(l2[nhl - 1][k][j]*l3_d[k]);
			}
			w3[j]=w3[j]-(alpha*s);
		}
	}

}



double S(double x)
{ return 1/(1+exp(-x)); }

double d_S(double x)
{ return x*(1-x); }

void print_weights()
{
	int i,j;
	printf("\n\nInput layer-Hidden layer");
	for(i=1;i<=n1;i++)
	{
		printf("\nNeuron %d",i);
		for(j=1;j<=n3;j++)
		{
			printf("\n\tw[%d]=%0.15lf",j,w1[i][j]);
		}
	}
	
	for(int jj=0;jj<nhl;jj++)	//n data entries
	{	if(jj%2==0)printf("\033[1;31m");
	printf("\n\nHidden layer");
	for(i=1;i<=n2[jj];i++)
	{
		printf("\nNeuron %d",i);
		for(j=1;j<=n3;j++)
		{
			printf("\n\tw[%d]=%0.15lf",j,w2[jj][i][j]);
		}
	}if(jj%2==0)printf("\033[0m"); 
	}
	
	printf("\n\nHidden layer-Output");
	for(i=1;i<=n3;i++)
	{
		printf("\n\tw[%d]=%0.15lf",i,w3[i]);
	}
}


void test()
{
	int i,j;
	double s1,l11[11],l22[8][11],I1[11];
	printf("\nEnter test values\n");
	for(i=1;i<=n1;i++)
	{
		printf("enter x[%d] : ",i);
		scanf("%lf",&I1[i]);
	}
	for(i=1;i<=n2[0/*couche*/];i++)
	{
		s=0;
		for(j=1;j<=n1;j++)
		{
			s=s+(I1[j]*w1[j][i]);
		}
		l11[i]=S(s);
	}
	for(i=1;i<=n3;i++)
	{
		s=0;
		for(j=1;j<=n2[0/*couche*/];j++)
		{
			s=s+(l11[j]*w2[0/*couche*/][j][i]);
		}
		l22[0/*couche*/][i]=S(s);
	}
	for(int jj=1;jj<nhl;jj++)	//n data entries
	{
		for(i=1;i<=n3;i++)
		{
			s=0;
			for(j=1;j<=n2[jj];j++)
			{
				s=s+(l22[jj - 1][j]*w2[jj][j][i]);
			}
			l22[jj][i]=S(s);
		}
	}


	s=0;
	for(i=1;i<=n3;i++)
	{
		s=s+(l22[nhl - 1][i]*w3[i]);
	}
	printf("Output : %0.15lf",S(s));
}
