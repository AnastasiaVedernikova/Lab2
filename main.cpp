#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <thread>
#include <mutex>

using namespace std;
mutex mx;
vector<string> words1;//data from input file
map<string, int> mp;

template <typename T1, typename T2>
struct decreasing_sort {
    typedef pair<T1, T2> type;
    bool operator ()(type const& a, type const& b) const {
        return a.second > b.second;
    }
};

int calculate(const std::vector<string>& words1, int start, int end, std::map<std::string, int>& mp)
//function for calculating of words in file using threads and mutex for synchronization
{   mx.lock();
    for (int i = start; i < end; i++) {

        ++mp[words1[i]];
    }

    mx.unlock();
    return 0;

}

int read_write_file(string filename) {
    clock_t t1, t2, t3, t4, t5;//control of time

    vector<pair<string, int> > words2;//pairs like (word : number of this word in text)

    ifstream f;
    string output;

    ofstream f_in_alph_order;
    ofstream f_in_numerical_order;


    t1 = clock();

    //--opening file

    f.open(filename.c_str());

    while (f >> output)
    {
        words1.push_back(output);
    }


    t4 = clock();
    //work with threads
    int  number_of_threads;
    cout << "Please enter a number of threads: ";
    cin >> number_of_threads;

    thread my_threads[number_of_threads];

    //partition of input data between threads

    vector<int> parts_of_words1; //0 7 15
    int step = int(words1.size())/number_of_threads;
    int iter = 0;
    int pointer= 0;
    parts_of_words1.push_back(0);
    while(iter != number_of_threads){
        if (iter == number_of_threads-1){
            parts_of_words1.push_back(int(words1.size()));
            break;
        }else {
            pointer += step;
            parts_of_words1.push_back(pointer);
            iter++;
        }
    }

    t2 = clock();//start of calculating by threads
    for (int i=0; i < parts_of_words1.size()-1; ++i) {
        my_threads[i] = thread(calculate, cref(words1), parts_of_words1[i],
                               parts_of_words1[i+1], ref(mp));

    }
    for (auto& t : my_threads) t.join();//end of threads' work


    for (map<string, int> :: iterator i = mp.begin(); i != mp.end(); ++i) {
        words2.push_back(make_pair(i->first, i->second));
    }
    f.close();


    t3 = clock();//start of sorting
    vector<pair<string, int> > words3(mp.begin(), mp.end());
    sort(words3.begin(), words3.end(), decreasing_sort<string, int>());

    //writing in files

    f_in_alph_order.open("/home/anastasia/CLionProjects/Lab/alphabet.txt");

    for (size_t i = 0; i < words2.size(); ++i) {

        f_in_alph_order << words2[i].first << "\n";
    }
    f_in_alph_order.close();
    ///home/anastasia/CLionProjects/Lab/
    f_in_numerical_order.open("/home/anastasia/CLionProjects/Lab/numerical.txt");
    for (size_t i = 0; i < words3.size(); ++i) {
        //cout<<words3[i].first<<endl;
        f_in_numerical_order << words3[i].first <<"\n";
    }
    f_in_numerical_order.close();


    t5 = clock();

    // calculating of time
    string ms = " ms ";

    cout<<"Time of reading file: "<< float(t4 - t1)/(CLOCKS_PER_SEC / 1000) << ms<< endl;
    cout<<"Time of calculating: "<< (float(t3 - t2))/ (CLOCKS_PER_SEC / 1000) << ms << endl;
    cout<<"Time of sorting and writing in file: "<< float((t5 - t3))/(CLOCKS_PER_SEC / 1000)<< ms << endl;
    cout<<"General time: "<< float((t5 - t1))/(CLOCKS_PER_SEC / 1000)<< ms << endl;
    return 0;
}


void print_if_present(const std::map<char,int> &m, char el)
{
    std::map<char,int>::const_iterator itr = m.find(el);
    if( itr != m.end() )
    {
        cout << "Element " << el << " is present, equal to "
             << itr->second << endl;
    }else{
        cout << "Element " << el << " is not present" << endl;
    }
}

int main(int argc, char* argv[]) {

    read_write_file("/home/anastasia/CLionProjects/Lab/data.txt");

    return 0;
}
