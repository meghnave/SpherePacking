#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#define false 0
#define true 1
//#include<graphics.h>
#include<time.h>
//#include<omp.h>
/*global varibales*/
float x[500],y[500],z[500],r[500],placedx[500],placedy[500],placedz[500];
float lambda[500];
static int n,placed=0;
float l,w,h;
double tt,ttime=0;

/*Procedure to place first two spheres*/
int framePro()
{
    placedx[0]=(-0.5*l)+r[0];
    placedy[0]=(-0.5*w)+r[0];
    placedz[0]=(-0.5*h)+r[0];

    placedx[1]=(-0.5*l)+r[0]+2*(sqrt(r[0]*r[1]));
    placedy[1]=(-0.5*w)+r[1];
    placedz[1]=(-0.5*h)+r[1];

    if( (placedx[1]+r[1])<(l/2) && (placedy[0]+r[0])<(w/2) && (placedz[0]+r[0])<(h/2) )
    return true;
    else { return false;}

}

/*Procedure to check if a circle lies within the rectangle*/
int isInside (double x,double y,double z,double r)
{
	if((x+r)>w/2 || (x-r)<(-w/2) || (y+r)>l/2 || ((y-r)<(-l/2)) || ((z-r)<(-h/2)) || ((z+r)>(h/2)) )
		return false;
	else
	      {	return true;}
}

/* The core procedure to place the other speheres*/
int corePro(int j)
{

double tempx,tempy,tempz;
double tentx=-1,tenty=-1,tentz=-1;
double tentx1,tenty1,tentz1;
double mindist=0,tentlambda=0;
double dist,a,s,e;
int i,m;
int collide;
//printf("value of placed: %d\n",placed);
/*Try to place the incoming sphere in touch with the spheres already placed*/
for(i=0;i<placed;i++)
{
    tempx=placedx[i]+r[i]+r[j];
    tempy=placedy[i]+r[i]+r[j];
    tempz=placedz[i]+r[i]+r[j];

    collide=false;

    a=0;

/*Rotate the location found by angles in succession of 0.1 radian to find corner placement*/
    while(a<M_PI)
    {
	//printf("collide=%d",collide);
	a=a+0.1;

	tentx1=tempx+(r[i]+r[j])*cos(a);
	tenty1=tempy+(r[i]+r[j])*sin(a);
	tentz1=tempz;

	if((isInside(tentx1,tenty1,tentz1,r[j]))==false)
		continue;

	collide=false;
	/*Check if it collides with any of the placed spheres*/
	for(m=0;m<placed;m++)
	{

	    dist=sqrt((placedx[m]-tentx1)*(placedx[m]-tentx1)+(placedy[m]-tenty1)*(placedy[m]-tenty1)+(placedz[m]-tentz1)*(placedz[m]-tentz1));
		dist=dist-r[m]-r[j];
	    if(dist<0)
	    {
		//printf("dist=%f\n",dist);	
		collide=true;
		break;
	    }
	}
		/*Find optimal placement according to minimum lambda*/
		if(collide==false)
		{	
		
	#pragma omp parallel num_threads(12) default(none) firstprivate(tentx1,tenty1,tentz1,j,placed) shared(tentx,tenty,tentz,placedx,placedy,placedz,mindist,r) 
	{
int m; int dist;
	#pragma omp for nowait schedule(dynamic,8)
			for(m=0;m<placed;m++)
			{
			dist=sqrt((placedx[m]-tentx1)*(placedx[m]-tentx1)+(placedy[m]-tenty1)*(placedy[m]-tenty1)+(placedz[m]-tentz1)*(placedz[m]-tentz1));
			dist=dist-r[m]-r[j];
	    		if((mindist==0) && (dist>0))
		    {
			mindist=dist;
			//tentlambda=1-mindist/r[j];
			tentx=tentx1;
			tenty=tenty1;
			tentz=tentz1;
		    }
			/*pick coordinates with minimum density*/
		else if((dist<mindist) && (dist>0))
		    {
			mindist=dist;
			//tentlambda=1-mindist/r[j];
			tentx=tentx1;
			tenty=tenty1;
			tentz=tentz1;
		    }
			}
		} //if end

    } //while end



} //for end
}// parallel end
/*Put the optimum placement in the array*/
if(tentx==-1 && tenty==-1)
    return false;
else{
    placedx[j]=tentx;
    placedy[j]=tenty;
    placedz[j]=tentz;
    lambda[j]=tentlambda;
    return true;
	}
}//func end


int main()
{

    int i,j,k,temp;
    srand(time(NULL));
    int doneFrame;
    int doneCore;
	double start,end,mid;
	double time;

   printf("\nEnter the number of spheres: ");
scanf("%d",&n);

    /*get details of cuboid*/
    printf("Enter length, width, height of rectangle\n");
    scanf("%f%f%f",&l,&w,&h);

 //start=omp_get_wtime();
start=clock();

    printf("Radii of cirles are chosen randomly!!");
    for(i=0;i<n;i++)
    {
	//printf("\nRadii "); scanf("%f",&r[i]);
	r[i]=rand()%10+1;
    }
//mid=clock();
//time=(double)(mid-start)/CLOCKS_PER_SEC;

    /*sort the spheres in decreasing order*/
    for(i=0;i<n-1;i++)
    {
	for(j=0;j<n-i-1;j++)
	{
	    if(r[j]<r[j+1])
	    {
		//swap
		temp=r[j];
		r[j]=r[j+1];
		r[j+1]=temp;

		temp=x[j];
		x[j]=x[j+1];
		x[j+1]=temp;

		temp=y[j];
		y[j]=y[j+1];
		y[j+1]=temp;

	    }
	}
    } //end sort


    doneCore=false;

	start=clock();
   /*place first two spheres*/
    doneFrame=framePro();
	if(doneFrame) placed=2;

   /*place remaining spheres using core procedure*/	
    if(doneFrame && n>2)
    {
	for(i=2;i<n;i++)
	{
	    doneCore=false;
	    doneCore=corePro(i);
	    if(doneCore) placed=placed+1;
		else break;
	}
    }


	//end=omp_get_wtime();
	end=clock();
	time=(double)(end-start)/CLOCKS_PER_SEC;
	
      /*check if core procedure worked and tell placed spheres*/
    if(!doneCore) printf("\nCan't place all spheres in core procedure! Failureeee!\n");
    else
    {
	printf("\nAll placed! Placed at-\n");
	/*for(i=0;i<placed;i++)
	    printf("Circle with radius %f placed at x=%f , y=%f, z=%f\n",r[i],placedx[i],placedy[i],placedz[i]);*/
    }
    printf("\n");
	time/=4;
//printf("\nTTIME: %f",ttime);
printf("\nTime taken for the program to run: %f seconds\n",time);
    /*int gd=DETECT,gm;
    initgraph(&gd,&gm, NULL);
	rectangle(w/2,l/2,-w/2,-l/2);
	for(i=0;i<placed;i++)
	{
		circle(placedx[i],placedy[i],r[i]);
	}
   //getch();*/
	//delay(5000);
	//closegraph();
//	getch();
    return 0;
}

