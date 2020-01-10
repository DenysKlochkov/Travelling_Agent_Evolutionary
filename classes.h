#pragma once


#define RADIUS 6371
#define BIG_NUMBER 10000

using namespace std;
class CityGroup{
private:
class City{
private:
      string Name;
      double longt; //west/east
      double lat;  //north/south
public:
      
      City(string n="", double lon=0.0, double la=0.0):Name(n), longt(lon),lat(la){};
      ~City(){};
      double doubleToRadians(double val)const {return (double)(val*M_PI/180);}
      double GetDistance(City other) const {

                   double dlon = doubleToRadians(other.longt) - doubleToRadians(longt);
                   double dlat = doubleToRadians(other.lat)   - doubleToRadians(lat);

                   double a =  pow(sin(dlat / 2), 2) + cos(doubleToRadians(lat)) * cos(doubleToRadians(other.lat)) * pow(sin(dlon / 2), 2);
                   double angle = 2 * asin(sqrt(a));

                   return (double)(RADIUS*angle );
       }
      string GetName()const{return Name;}

};

const int GrSize; //10,20,30
City * CityArray;
double ** Distances;



public:
           friend ostream & operator << (ostream &out, const CityGroup &c);
           friend class Route;
      
      CityGroup( const int& Gsize, const char * filename ):GrSize(Gsize){
            ifstream mFile;
            mFile.open(filename);
           //assert(mFile!=NULL);

            CityArray = new City[GrSize];
            Distances = new double*[GrSize];
            for(int i=0;i<GrSize;i++)Distances[i]=new double[GrSize];
          

            string tmp_name;
            string tmp_long;
            string tmp_lat;
      

            for(int i=0;i<GrSize;i++){
                  mFile>>tmp_name;
                  mFile>>tmp_long;
                  mFile>>tmp_lat;
                //  cout<<tmp_name<<", "<<tmp_long<<", "<<tmp_lat<<", "<<ParseDistanceString(tmp_long)<<", "<<ParseDistanceString(tmp_lat)<<"\n";
                  CityArray[i]=City(tmp_name,ParseDistanceString(tmp_long),ParseDistanceString(tmp_lat));
            }
              mFile.close();
            for(int i=0;i<GrSize;i++){
            for(int ii=0;ii<GrSize;ii++){
                  if(i==ii)Distances[i][ii]=0;
                  else   Distances[i][ii]=CityArray[i].GetDistance(CityArray[ii]);
            }

            }
      }


      ~CityGroup(){
            delete [] CityArray;

            for(int i=0;i<GrSize;i++)delete [] Distances[i];
                delete [] Distances;
      }

       static double ParseDistanceString(string t) 
      //parses 21°02'E into 21,033 
            {
                        double high=0;
                        double low=0;
                        high+=(double)(t[0]-'0');
                        high*=10;
                        high+=(double)(t[1]-'0');

                        low+=(double)(t[4]-'0');
                        low*=10;
                        low+=(double)(t[5]-'0');
                        low=low/60;
                      
                        return high+low;
            }
             double GetDistance(int from, int to) const{
                  if(from==to)return 0.0;
                  return Distances[from][to];
            }
            string GetCityName(int i)const{return CityArray[i].GetName();}
};

ostream & operator << (ostream &out, const CityGroup &c){
             for(int i=0;i<c.GrSize;i++){
                  cout<<i<<setw(12)<<c.GetCityName(i)<<": ";
            for(int ii=0;ii<c.GrSize;ii++){
            cout<<setw(6)<<c.GetDistance(i,ii)<<'\t';
            }
            cout<<'\n';}
}
class Route{
private:
      CityGroup * myCityGroup;
      int * cities_seq;
      int citiesN;
      double distance;
     
public:
      static int cross_diffusion_ratio;
      friend ostream & operator << (ostream &out, const Route &r);


      Route( CityGroup * mCG){
            myCityGroup=mCG;
            citiesN=mCG->GrSize;
            cities_seq=new int[citiesN]();
            (*this)[0]=0;
            int next=0;
            for(int i=1;i<citiesN;i++){

                  if(i<citiesN/2)while(this->CityIsPresent(next))next=rand()%citiesN;
                  else while(this->CityIsPresent(next))next=(next+1)%citiesN;
                  (*this)[i]=next;
            }
            calcDistance();
        


      }
      ~Route(){
                 delete [] cities_seq;
                   cities_seq=NULL;
      }
      Route( const Route & other){
            myCityGroup=other.myCityGroup;
            citiesN=other.citiesN;
            distance=other.distance;
            cities_seq=new int[citiesN]();
            for(int i=0;i<citiesN;i++){
                  cities_seq[i]=other.cities_seq[i];
            }
      }
     static void setCDR(int CDR){
            cross_diffusion_ratio=CDR;
      }
      Route& operator =(const Route & other){
                  if(&other==this)return *this;
                  delete [] cities_seq;
                  citiesN=other.citiesN;
                  distance=other.distance;
                  myCityGroup=other.myCityGroup;
                  cities_seq=new int[citiesN]();
                   for(int i=0;i<citiesN;i++){

                  cities_seq[i]=other.cities_seq[i];
            }
                   return *this;
      }

        Route& operator =( Route && other){
                  if(&other==this)return *this;
                  delete [] cities_seq;
                  cities_seq=other.cities_seq;
                  citiesN=other.citiesN;
                  distance=other.distance;
                  myCityGroup=other.myCityGroup;

                  other.cities_seq=nullptr;
                   return *this;
      }
            

    
      void calcDistance(){
            distance=0.0;
            for(int i=0;i<citiesN;i++){
                  int next = i+1;
                  if(next==citiesN)next=0;
                  distance+=myCityGroup->GetDistance(cities_seq[i],cities_seq[next]);
                  
            }

      }
      double GetDistance() const {
            return distance;
      }
    

      bool CityIsPresent(int n) const {
            for(int i =0; i<citiesN;i++)
                  if((*this)[i]==n)return true;
            
            return false;
      }

      int operator[](int n) const {
            if(n>=citiesN){//TODO:error handling
            }
            return cities_seq[n];
      }
      int& operator[](int n)  {
            if(n>=citiesN){//TODO:error handling
            }
            return cities_seq[n];
      }

      void print(){
            for(int i=0;i<citiesN;i++)
                  cout<<myCityGroup->GetCityName(i)<<"--";
            cout<<myCityGroup->GetCityName(0)<<'('<<distance<<')'<<'\n';
      }
   

//Crossover
      Route operator *(const Route& other){
            int first = rand()%citiesN;
            int last=0;
            Route child(other);
            for(int i=0;i<citiesN;i++)child[i]=-1;

            while (last<first)last=rand()%citiesN;
            for(int i=first;i<=last;i++){
                   child[i]=(*this)[i];
            }
       
            int write_from_top=0;
            int write_from_bottom=citiesN-1;
            int write_next=1;
            child[0]=0;
     
            while(1){
          //          cout<<write_next<<'|'<<write_from_top<<'|'<<write_from_bottom<<'|'<<first<<"x"<<last<<'\n';
                  if(write_next==first)write_next=last+1;
                  if(write_next==citiesN)break;
                  if(rand()%cross_diffusion_ratio!=cross_diffusion_ratio-1)
                      

                        {if(!child.CityIsPresent(other[write_from_top])){
                              child[write_next++]=other[write_from_top];
                            
                        }
                          write_from_top++;
                        }
                  else
                        {if(!child.CityIsPresent(other[write_from_bottom])){
                              child[write_next++]=other[write_from_bottom];
  


                        }
                          write_from_bottom--;
                        }      

      
                       if(write_from_top==citiesN)write_from_top=0;

                       if(write_from_bottom==-1)write_from_bottom=citiesN-1;

    //              cout<<child;                      
            }

      
      
  //        cout<<"First/Last "<<first<<'/'<<last<<" Parents\n"<<(*this)<<other<<"Child\n"<<child;


           
                  child.calcDistance();
            return child;

      }

            bool operator < ( const Route & other){
                  return (distance<other.distance);
            }

      void Mutate( int chance){
            if(rand()%chance==0){

                  //we never swap 0th element
                  for(int i=1;i<citiesN/2;i+=2)
                  if(rand()%2)mySwap(i,citiesN-i);
                  int offset = rand()%(citiesN/2);
                  for(int i=1;i<citiesN-offset;i++)
                  {int next = i+offset;
                        if(next>=citiesN)next-=(citiesN-1);
                        mySwap(i,next);
                  }
                  calcDistance();
            }
      }
 
      void mySwap( int one, int two){
            int buf;
            buf=(*this)[one];
                  (*this)[one]=(*this)[two];
                  (*this)[two]=buf;
      }

};

int Route::cross_diffusion_ratio=0;

   ostream & operator << (ostream &out, const Route &r){
            for(int i=0;i<r.citiesN;i++){
                  out<<r[i]<<'-';
            }
            out<<r[0]<<'('<<r.GetDistance()<<')'<<'\n';
      }

 
  double getMean(vector<Route> * vect,int size){
            double mean=0.0;
            for(int i=0; i<size;i++)mean+=(*vect)[i].GetDistance();
                  return mean/size;
      }
      double getSd(vector<Route> * vect,int size,double mean){
            double sd=0.0;
            for(int i=0;i<size;i++)sd+=pow(((*vect)[i].GetDistance()-mean),2);
            return sqrt(sd/size);      
      }
