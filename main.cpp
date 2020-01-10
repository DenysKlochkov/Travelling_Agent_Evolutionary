#include <iostream>
#include <iomanip> 
#include <string>
#include <sstream>
#include <fstream>
#include <cmath>
#include <cstring>
#include <algorithm> 
#include <vector>
#include <stdlib.h>    
#include <cassert>

#include "classes.h"


#define CROSS_DIFFUSION_RATIO 4



using namespace std;
class MyEvolutionaryAlg{
private:

     
     
      vector<Route> Population;
      

      int pSize;
      int oSize;
      int mRate;

      int genCounter;
      int stopCounter;
      int fGen;
      
      double selPress;
      bool algType; 
      //true for (m+l) 
      //false for(m,l) 
      CityGroup * mCG;
       Route l_b;
       double last_best;
public:
     

     MyEvolutionaryAlg( int popSize, int offspringSize, int mutationRate, int finalGenerations, double selectivePressure, bool algorythmType, CityGroup * myCG, const char * outfileName):
      //                                                                   number of generations for stop condition
      pSize(popSize), oSize(offspringSize), mRate(mutationRate),genCounter(0),stopCounter(0), fGen(finalGenerations), selPress(selectivePressure), algType(algorythmType), mCG(myCG), l_b(Route(myCG))
      {
            
            ofstream mFile;
            mFile.open(outfileName);

            //(0)creating initial population
            Population.push_back(l_b);
            for(int i=0;i<pSize-1;i++){
                  Population.push_back( Route(myCG));
            }

            sort(Population.begin(),Population.end());
            last_best=Population[0].GetDistance();
            int matingPool[oSize];

            vector<Route> newPopulation;
            double mean, sd;

            if(algType==true){Route::setCDR(CROSS_DIFFUSION_RATIO);}
            else Route::setCDR(5*CROSS_DIFFUSION_RATIO);

            while(1){
                  
            

                  //(1)selecting matingPool
                  mean=getMean(&Population, pSize);
                  sd=getSd(&Population,  pSize, mean);
                  if(sd==0){
                        for(int i=0;i<oSize;i++)
                        matingPool[i]=0;
                  }//if our population consists of multiple copies of one route, we skip the selection process
                  else
                  for(int i=0, counter=0;counter!=oSize;){
                        double factor = (Population[i].GetDistance() - mean)/sd*selectivePressure;
                        double ratio;
                        //for Route with factor 0(mean) gives 50% chance to be selected
                        //for Route with factor 5 gives   86% chance  to be selected
                        //for Route with factor -5 gives  14% chance  to be selected
                        if(factor>=0)ratio=(factor+1)/(factor+2);
                              else ratio= 1.0-(1-factor)/(2-factor);
                                   
                        int adjustedRatio = (int)round(ratio*BIG_NUMBER);
                        if( rand()%BIG_NUMBER <= adjustedRatio){
                              matingPool[counter++]=i;
                        }
                        if(++i==pSize)i=0;
                  }

                  //(2)breeding
                  newPopulation.clear();
                  int next;
                  for(int i=0;i<oSize;i++){
                        next=i+1;
                        if(next==oSize)next=0;
                        if(algorythmType==true)
                        //for m,l we cross the best Route with other routes
                          
                        //for m+l (algType==true) new routes are inserted into the same container),
                        //for m,l (algType==false) different container is used.  
                        {   
                         next=rand()%oSize;

                              Route newChild =Population[matingPool[i]]*Population[matingPool[next]];
                              newChild.Mutate(mRate);  
                              Population.push_back(newChild);
                              }

                        else 
                          {   Route  newChild =Population[0]*Population[matingPool[next]];
                                       newChild.Mutate(mRate); 
                                       newPopulation.push_back(newChild);
                       }
                    
                       
                  }   
                  //(3)preparing for selection
                             
               //   for(int i=0;i<oSize;i++)cout<<newPopulation[i];
                  
                  if(algType==false){
                       //for m,l we save the best Route into the next generation 
                           sort(Population.begin(),Population.end());
                           newPopulation[0]=Population[0];
                        Population.clear();
                        Population=newPopulation;
                 //       for(int i=0;i<oSize;i++)cout<<Population[i];
                        newPopulation.clear();
                        
                     
                  }

                  //(4)chosing next generation, checking the stop condition and doing accounting
                  sort(Population.begin(),Population.end());
                       for(int i=0;i<pSize;i++)cout<<Population[i];
                  Population.erase(Population.begin()+pSize,Population.end());
                  double generationBest = Population[0].GetDistance();
               
                        cout<<"xxxxxxxxxxxxxxxxx"<<genCounter<<'|'<<stopCounter<<"xxxxxxxxxxxxxxxxx\n";
                  mFile<<(++genCounter)<<'\t'<<generationBest<<'\n';
                  if(generationBest<last_best){
                        last_best=generationBest;
                        l_b=Population[0];
                        stopCounter=0;
                  }
                  else if(++stopCounter==fGen){
                        cout<<"Best solution after "<<genCounter<<" generations \n";
                        l_b.print();
                        cout<<"Result is written to "<<outfileName<<'\n';
                  break;}





                                 
                  }
      



                  }


      
};
   
int main(int argc, char * argv[]){
      srand(time(NULL));
      if(argc==1){
                        cout<<"Please provide the following arguments\n"
                        <<"cities_count          - number of cities visited, 1-30\n"
                        <<"generation_size       - int>0\n"
                        <<"offsrping_size  - int>0, >generation_size\n"
                        <<"mutation_rate         - the higher this rate - the less likely mutations are int>0\n"
                        <<"stop_generations      - highest allowed number of succesive genetations without any change, after than the algorythm stops  int>0\n"
                        <<"evolutionary_pressure - the higher this pressure - the higher mating pool selection elitism is double>0\n"
                        <<"algorythm_type        - 0 for (m,l), 1 for (m+l) \n";
                        return 0;
      }
      int cities_count=(argc>1)?atoi(argv[1]):30;
      int generation_size=(argc>2)?atoi(argv[2]):60;
      int offspring_size=(argc>3)?atoi(argv[3]):70;

      int mutation_rate =(argc>4)?atoi(argv[4]):40;               //inverse law
      int stop_generations=(argc>5)?atoi(argv[5]):150; 
      double evolutionary_pressure=(argc>6)?atof(argv[6]):1; 
      bool algorythm_type=(argc>7)?atof(argv[7]):false; 
      CityGroup MCG(cities_count,(const char *)"pl_cities_30.txt");
    // cout<<MCG;

      MyEvolutionaryAlg mea(generation_size,offspring_size,mutation_rate,stop_generations,evolutionary_pressure,algorythm_type, &MCG,(const char * )"out.txt");

      system("gnuplot optim.p");
      system("display optimalization.png");
      return 0;
}