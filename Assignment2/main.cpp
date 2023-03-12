#include <vector>
#include <iostream>
#include <thread>
#include <utimer.cpp>

using namespace std;

auto f = [] ( int x ){ return x*x; };

auto body(vector<double> *vec, int start, int end, int threshold, int splitfactor){
    // If the lenght of the vector is greater than the threshold i split it in len/threshold
}

int main(int argc, char* argv[]){

    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " vector_len seed threshold splitfactor printflag" << endl; 
        return(0);
    }

    int vec_len = (argc > 1 ? atoi(argv[1]) : 100);  // len of the vector(s)
    int s = (argc > 2 ? atoi(argv[2]) : 123); // seed 
    int threshold = (argc > 3 ? atoi(argv[3]) : 10); // threshold to split
    int splitfactor = (argc > 4 ? atoi(argv[4]) : 10); // how many parts to split the vector
    
    vector<double> v(vec_len);
    const int max = 8; 
    srand(s); 
    for(int i=0; i<vec_len; i++)                  
        v[i] = (double) (rand() % max);

    vector<thread*> tids;
    tids.push_back(new thread(body, &v, 0, vec_len, threshold, splitfactor));
}