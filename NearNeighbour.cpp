#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;
/*
Name: NearNeighbour.cpp
creator: Luis Contreras

Hello User, I created this pice of code because i didnot want to calculate the last question of one of my assingmnets by hand,
after some truble becuause my edditor was alergic to Java i created this code in C++

*if you add 1 into the following vector, the code will print the nearest 3 datapoints, in flowers to UnknownSet items

*if you add 2 it will group all items into a set of three clusters and will try to run untill convergance

*if you add 3 it will use DB scan and print out size of clusters and datapoints that were conciderd noise
        - 4 will do the same exept it will also print the items inside the clusters

        ** if you have both 4 and 3 the code will assume you wanted 4


you can also change the log level, 0- 5 where 0 is going to print alot of stuff and 5 wownt print anything
*/
//                    1,   2,    3,    4
bool Requests[4] = {true, true, true, true};

short log_lvl=4;




class iris{

    public:
        iris(int idx, std::vector<double> content, std::string speciesStr){
            ID = idx;
            mesurements = content;
            species = speciesStr;
        }

        std::string toString(){
            std::string str = "{ID: " + to_string(ID) + " | [";
            for (int i =0; i< mesurements.size(); i++){
                str+=  to_string(mesurements[i]);
                if (i< mesurements.size() -1){str+= " , ";}
            }
            str +="] Species:" + species + "}";
            return str;
        }

        void print(){
            cout << toString();
        }

        int ID = 0;
        std::vector<double> mesurements = {};
        std::string species = "Blank";
};

double EuclideanDistance(iris destination, iris location){
    double distance = 0;
    //cout << "sqrt(";
    for (int i=0; i < destination.mesurements.size(); i++){
        //cout << "(" << destination.mesurements[i] <<" - " <<location.mesurements[i] <<")^2";
        distance += pow(destination.mesurements[i] - location.mesurements[i],2);
        //if (i< destination.mesurements.size() -1){cout << " + ";}
    }
    //cout << ")\n";
    //cout << "D:" << distance<<endl;
    return sqrt(distance);
}

// dont feal like fixing the alternative
double EuclideanDistance2(vector<double> destination, vector<double> location){
    double distance = 0;
    //cout << "sqrt(";
    for (int i=0; i < destination.size(); i++){
        //cout << "(" << destination.mesurements[i] <<" - " <<location.mesurements[i] <<")^2";
        distance += pow(destination[i] - location[i],2);
        //if (i< destination.mesurements.size() -1){cout << " + ";}
    }
    //cout << ")\n";
    //cout << "D:" << distance<<endl;
    return sqrt(distance);
}
class ClusterContainer{
    public:
        ClusterContainer(int maxsize, iris centerIris){
            MaxSize = maxsize;
            CentralPoint = centerIris;
        }
        
        void updateContainer(double dist, int iris){
            if (MaxSize> Distance.size())
            {
                Distance.push_back(dist);
                irisIdx.push_back(iris);
            }
            else{
                double max = -1.99;
                double maxIdx = -1;
                for (int i =0; i<Distance.size(); i++){
                    if (Distance[i] > max){
                        max = Distance[i];
                        maxIdx = i;
                    }
                }
                if (Distance[maxIdx] > dist){
                    Distance[maxIdx] = dist;
                    irisIdx[maxIdx] = iris;
                }
                
            }
        }

        void print(){
            cout << "Cluster for: " << CentralPoint.toString()<<"\n"; 
            for(int i =0; i<Distance.size(); i++)
            {
                cout << "Distance: " << Distance[i] << "| ID: "<<irisIdx[i] << "\n";
            }
        }

        std::vector<double> Distance={};
        int MaxSize = 0;
        vector<int> irisIdx= {};
        iris CentralPoint = iris(-1,{-1,-1,-1,-1}, "Error");
};

class KClusters {
    public:
        KClusters(int clusters){
            MaxClusters = clusters;
        }

        void addPoint(iris newpoint){
            if(flowerClusters.size() < MaxClusters)
            {
                flowerClusters.push_back({newpoint});
            }
            else{
                int minCluster = INT_MAX;
                int AppendIdx = -1;
                for (int i=0; i < flowerClusters.size(); i++)
                {
                    if (flowerClusters[i].size() < minCluster){
                        minCluster=flowerClusters[i].size();
                        AppendIdx = i;
                    }
                }
                
                flowerClusters[AppendIdx].push_back(newpoint);

                
                for (int i =0; i<flowerClusters.size();i++)
                {
                    if (flowerClusterCentroids.size() == i)
                    {
                        flowerClusterCentroids.push_back({});
                    }
                    for (int j=0; j<flowerClusters[i].size();j++){
                        for (int k = 0; k<flowerClusters[i][j].mesurements.size() ; k++)
                        {
                            if(flowerClusterCentroids[i].size() < flowerClusters[i][j].mesurements.size()){
                                flowerClusterCentroids[i].push_back(flowerClusters[i][j].mesurements[k]);
                            }
                            else{
                                flowerClusterCentroids[i][k] = ((flowerClusterCentroids[i][k] * (i)) + flowerClusters[i][j].mesurements[k])/(i+1);
                            }
                        }
                    }
                }
            }
        }

        void learn_KCluster(){
            bool NoChange = true;
            vector<double> centroid = {};
            vector<vector<iris>> tempflower={};
            double temp=0.0;
            while(NoChange){
                tempflower={};
                centroid = {};
                NoChange = false;
                temp=0.0;

                // calculate cluster centroids
                for (int i =0; i<flowerClusters.size();i++)
                {
                    if (flowerClusterCentroids.size() == i)
                    {
                        flowerClusterCentroids.push_back({});
                    }
                    for (int j=0; j<flowerClusters[i].size();j++){
                        for (int k = 0; k<flowerClusters[i][j].mesurements.size() ; k++)
                        {
                            if(flowerClusterCentroids[i].size() < flowerClusters[i][j].mesurements.size()){
                                flowerClusterCentroids[i].push_back(flowerClusters[i][j].mesurements[k]);
                            }
                            else{
                                flowerClusterCentroids[i][k] = ((flowerClusterCentroids[i][k] * (i)) + flowerClusters[i][j].mesurements[k])/(i+1);
                            }
                        }
                    }
                }

                // swamp to closest cluster
                for (int i =0; i<flowerClusterCentroids.size(); i++){
                    tempflower.push_back({});
                }
                for (int i =0; i<flowerClusterCentroids.size(); i++){
                    for (int j=0; j<flowerClusters[i].size();j++){
                        double distance = 99999999;
                        double tmpdistance = 0;
                        double newCluster = -1;
                        for (int k =0; k<flowerClusterCentroids.size(); k++)
                        {
                            tmpdistance = EuclideanDistance2(flowerClusterCentroids[k], flowerClusters[i][j].mesurements);
                            if( tmpdistance < distance)
                            {
                                distance = tmpdistance;
                                newCluster = k;
                            }
                        }
                        
                        //needs to be mooved to other cluster
                        
                        tempflower[newCluster].push_back(flowerClusters[i][j]);
                        if (i !=newCluster) {
                            NoChange = true;
                        }
                    }
                }
                flowerClusters = tempflower;
                if(log_lvl < 3){
                for(int i=0;i<flowerClusterCentroids.size();i++){
                    cout << "Centroid [" << (i)<< "] = {";
                    for(int j=0;j<flowerClusterCentroids[i].size();j++){
                        cout << flowerClusterCentroids[i][j] << "";

                        if (j< flowerClusterCentroids[i].size() -1){cout << " , ";}
                    }
                    cout << "}\n";
                }}
            }
        }

        std::string printCentroids(){
            std::string tmp="";
            for(int i=0;i<flowerClusterCentroids.size();i++){
                    tmp+= "Centroid [" + to_string(i)+ "] = {";
                for(int j=0;j<flowerClusterCentroids[i].size();j++){
                    tmp += to_string(flowerClusterCentroids[i][j]) + "";

                    if (j< flowerClusterCentroids[i].size() -1){tmp+= " , ";}
                }
                tmp+= "}\n";
            }
            return tmp;
        }

        void printKluster(){
            for(int i = 0; i < flowerClusters.size();i++){
                cout << "Cluster [" << i <<"], size ("<<flowerClusters[i].size()<<")\n";
                for(int j=0; j<flowerClusters[i].size(); j++){
                    cout << "   "<<j<<": ID=" << flowerClusters[i][j].ID << " | Species" << flowerClusters[i][j].species <<"\n";
                }
            }
        }

        vector<vector<iris>> flowerClusters={};
        vector<vector<double>> flowerClusterCentroids={};
        int MaxClusters= 0;
};

// I was originaly going to add to Kclusters but I realized that it would be much more work than just creating a new class
class TAGiris{
    public:
        TAGiris(iris ir, int ta){
            tag = ta;
            Item = ir;
        }

        iris Item = iris(-1,{-1,-1,-1,-1}, "error");
        int tag=0;
        double distance = -1;
};


class DBSCAN {
    public:
        DBSCAN(int max){
            MaxClusters = max; // random number i am tired
        }
        
        
        void addPoint(iris newpoint){
            flowerClusters.push_back(TAGiris(newpoint, -1));
        
        }

        void DBlearn(double MaxDist, int minPoints)
        {
            
            double distance=0;
            //create initial clusters
            for (int i =0; i <flowerClusters.size(); i++)
            {
                distance=999999999999999;
                if (flowerClusters[i].tag == -1){
                    numberClusters +=1;
                    flowerClusters[i].tag = numberClusters;
                }
                //check inner cluster information
                for( int j=0; j<flowerClusters.size(); j++){
                    distance = EuclideanDistance(flowerClusters[j].Item,flowerClusters[i].Item);
                    if(distance <= MaxDist)
                    {                            
                        flowerClusters[j].tag = numberClusters;
                        flowerClusters[j].distance = distance;
                    }
                        
                }
                
            }
            
        }

        void Clusteify(){
            
        }
        void printDB()
        {
            for (int i =0; i < flowerClusters.size(); i++)
            {
                cout <<"ID"<< flowerClusters[i].Item.ID << " | T: "<<flowerClusters[i].tag<<" | D:" << flowerClusters[i].distance<< "\n"; 
            }
        }
        vector<TAGiris> flowerClusters={};
        vector<vector<double>> flowerClusterCentroids={};
        int MaxClusters= 0;
        int numberClusters = 0;
};

std::vector<iris> flowers =  {
    iris(0,  {5.1, 3.5, 1.4, 0.2}, "Iris-setosa"),
    iris(1,  {4.9, 3.0, 1.4, 0.2}, "Iris-setosa"),
    iris(2,  {4.7, 3.2, 1.3, 0.2}, "Iris-setosa"),
    iris(3,  {4.6, 3.1, 1.5, 0.2}, "Iris-setosa"),
    iris(4,  {5.0, 3.6, 1.4, 0.2}, "Iris-setosa"),
    iris(5,  {5.4, 3.9, 1.7, 0.4}, "Iris-setosa"),
    iris(6,  {4.6, 3.4, 1.4, 0.3}, "Iris-setosa"),
    iris(7,  {5.0, 3.4, 1.5, 0.2}, "Iris-setosa"),
    iris(8,  {4.4, 2.9, 1.4, 0.2}, "Iris-setosa"),
    iris(9,  {4.9, 3.1, 1.5, 0.1}, "Iris-setosa"),
    iris(10, {7.0, 3.2, 4.7, 1.4}, "Iris-versicolor"),
    iris(11, {6.4, 3.2, 4.5, 1.5}, "Iris-versicolor"),
    iris(12, {6.9, 3.1, 4.9, 1.5}, "Iris-versicolor"),
    iris(13, {5.5, 2.3, 4.0, 1.3}, "Iris-versicolor"),
    iris(14, {6.5, 2.8, 4.6, 1.5}, "Iris-versicolor"),
    iris(15, {5.7, 2.8, 4.5, 1.3}, "Iris-versicolor"),
    iris(16, {6.3, 3.3, 4.7, 1.6}, "Iris-versicolor"),
    iris(17, {4.9, 2.4, 3.3, 1.0}, "Iris-versicolor"),
    iris(18, {6.6, 2.9, 4.6, 1.3}, "Iris-versicolor"),
    iris(19, {5.2, 2.7, 3.9, 1.4}, "Iris-versicolor"),
    iris(20, {6.3, 3.3, 6.0, 2.5}, "Iris-virginica"),
    iris(21, {5.8, 2.7, 5.1, 1.9}, "Iris-virginica"),
    iris(22, {7.1, 3.0, 5.9, 2.1}, "Iris-virginica"),
    iris(23, {6.3, 2.9, 5.6, 1.8}, "Iris-virginica"),
    iris(24, {6.5, 3.0, 5.8, 2.2}, "Iris-virginica"),
    iris(25, {7.6, 3.0, 6.6, 2.1}, "Iris-virginica"),
    iris(26, {4.9, 2.5, 4.5, 1.7}, "Iris-virginica"),
    iris(27, {7.3, 2.9, 6.3, 1.8}, "Iris-virginica"),
    iris(28, {6.7, 2.5, 5.8, 1.8}, "Iris-virginica"),
    iris(29, {7.2, 3.6, 6.1, 2.5}, "Iris-virginica")
};

std::vector<iris> UnknownSet = {
    iris(30, {5.4, 3.7, 1.5, 0.2}, "Flower X"),
    iris(31, {5.9, 3.0, 5.1, 1.8}, "Flower Y"),
};


int main()
{
    if(log_lvl < 1){
        for (long unsigned int i=0; i < flowers.size(); i++){
            cout << flowers[i].ID << " | " << flowers[i].species << "\n";
        }
    }
    vector<ClusterContainer> UnknownContainers = {ClusterContainer(3,UnknownSet[0]),ClusterContainer(3,UnknownSet[1])};
    vector<double> temp = {0.0,0.0};

    if (Requests[1]){
        for( int j = 0; j < flowers.size(); j++){
            temp[0] =  EuclideanDistance(flowers[j], UnknownSet[0]);
            temp[1] =  EuclideanDistance(flowers[j], UnknownSet[1]);
            
            if (log_lvl<2){
            cout <<"Distance("<< 0 << "," << j <<"): "<< temp[0] <<" | ";
            cout <<"Distance("<< 1 << "," << j <<"): "<< temp[1] <<"\n";
            }
            UnknownContainers[0].updateContainer(temp[0], flowers[j].ID);
            UnknownContainers[1].updateContainer(temp[1], flowers[j].ID);

        }
        cout <<"\nDistances and IDs\n";
        UnknownContainers[0].print();
        UnknownContainers[1].print();
    }
    
    if (Requests[2])
    {
        cout << "\n K - Cluster\n";
        KClusters FirstCluster = KClusters(3);
        for (int i=0; i < flowers.size(); i++)
        {
            FirstCluster.addPoint(flowers[i]);
        }
        
        cout << "Before: \n" <<FirstCluster.printCentroids() << "\n";
        FirstCluster.learn_KCluster();
        cout << "After: \n" << FirstCluster.printCentroids() << "\n";
        FirstCluster.printKluster();
    }

    if (Requests[3])
    {
        cout << "\n DB - Cluster\n";
        DBSCAN SecondCluster = DBSCAN(30);
        for (int i=0; i < flowers.size(); i++)
        {
            SecondCluster.addPoint(flowers[i]);
        }
        
        SecondCluster.DBlearn(0.75,2);
        //cout << "After: \n" << FirstCluster.printCentroids() << "\n";
        SecondCluster.printDB();
    }
}