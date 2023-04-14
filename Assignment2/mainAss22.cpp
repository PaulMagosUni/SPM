#include <vector>
#include <iostream>
#include <thread>
#include <future>
#include <utimer.cpp>

using namespace std;

vector<thread*> tids;

auto f = [] ( int x ){ return x*x; };

auto body(vector<double> *vec, int start, int end)
{
    {
        // long usecs;
        // utimer t0(&usecs, threadId);
        for (int i=start; i<end; i++)
        {
            (*vec)[i] = f((*vec)[i]);
        }
    }
}


void myTry(vector<double> * vec, int start, int end, int threshold, int split_fact){
    int sfact = split_fact;
    if (end-start > threshold){
        int delta = (end-start)/sfact; 
        int start2 = start, end2 = start;
        end2 += delta;
        for(int i=0; i<sfact; i++){
            // printf("start 2 : %d, start : %d, end 2 : %d, end : %d\n", start2, start, end2, end);
            myTry(vec, start2, end2, threshold, split_fact);
            start2 = end2 + 1;
            end2 = ((end2 + delta) > end)? end : end2 + delta;
        }
    }else{
        tids.push_back(new thread(body, vec, start, end));
    }// cout << "ThreadRes: "  << threadsRes.get() << endl; 
}



int main(int argc, char* argv[]){

    if(argc == 2 && strcmp(argv[1],"-help")==0) {
        cout << "Usage is: " << argv[0] << " vector_len seed threshold printflag threads" << endl; 
        return(0);
    }

    int vec_len = (argc > 1 ? atoi(argv[1]) : 100);  // len of the vector(s)
    int s = (argc > 2 ? atoi(argv[2]) : 123); // seed 
    int threshold = (argc > 3 ? atoi(argv[3]) : 10); // threshold 
    bool pf=(argc > 4 ? (argv[4][0]=='t' ? true : false) : true);  
    int split_fact = (argc > 5 ? atoi(argv[5]) : threshold); // split_factor  
    
    vector<double> v(vec_len);
    vector<double> w(vec_len);
    const int max = 8; 
    srand(s); 
    for(int i=0; i<vec_len; i++){   
        v[i] = (double) (rand() % max);
        w[i] = v[i];
    }

    int threads =0;
    long usecs; 
    {
        utimer t0("Computation",&usecs); 
        myTry(&v, 0, vec_len, threshold, split_fact);
        threads = tids.size();
        for(auto t : tids) t->join();
    }

    cout << "End (spent " << usecs << " usecs using " << threads << " threads for " << vec_len << " elements)" << endl;
    if(pf)                                    // print results (if needed)
        for(int i=0; i<vec_len; i++) cout <<i<<": "<< w[i]<<" -> "<< v[i] << endl;
}