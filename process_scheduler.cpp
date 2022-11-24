#include <iostream>
#include <queue>
#include <fstream>
#include <string>
#include <cstdlib>
#include <time.h>
#include<map>
#include<set>
//#include<priority_queue>

using namespace std;
//for storing the jobs


class Jobs
{
public:
    int id;
    int priority;
    int arrivialTime;
    int totalTime;
    queue<int> CPU_brusts;
    queue<int> IO_brusts;
    Jobs(int id, int pri, int arrival, vector<int> &cpu, vector<int> &io)
    {
        totalTime=0;
        this->id = id;
        this->priority = pri;
        this->arrivialTime = arrival;
        for (int i = 0; i < cpu.size(); i++)
        {
            totalTime+=cpu[i];
            CPU_brusts.push(cpu[i]);
        }
        for (int i = 0; i < io.size(); i++)
        {
            totalTime=io[i];
            IO_brusts.push(io[i]);
        }
    }
    void show()
    {
        cout<<"------------------------------------------";
        cout<<"id "<<id<<endl<<"priority "<<priority<<endl<<"at "<<arrivialTime<<endl;
        while(!CPU_brusts.empty()){cout<<CPU_brusts.front()<<" ";CPU_brusts.pop();}
        cout<<endl;
        while(!IO_brusts.empty()){cout<<IO_brusts.front()<<" ";IO_brusts.pop();}
        cout<<endl;
        
    }
};

struct Comparetime {
    bool operator()(Jobs* j1, Jobs* j2)
    {
        return j1->arrivialTime > j2->arrivialTime;
    }
};



class ROUND_ROBIN
{
    public:
    priority_queue<Jobs*, vector< Jobs* >, Comparetime> jobs_queue;
    map<int,int>complete;
    set<Jobs*>inIO;
    int i;
    int watingTime,turnaroundTime;
    ROUND_ROBIN(vector<Jobs*>arr)
    {
        for(int i=0;i<arr.size();i++)
        {jobs_queue.push(arr[i]);}
        i=-1;
        watingTime=0;
        turnaroundTime=0;
    }
    void start()
    {
        
        while (true)
        {
            i++;
            gotoIO();
            //this is the idel time for the cpu as no prosses to excuate;
            while(jobs_queue.empty())
            {
                //cout<<"while job queue empty"<<endl;
                i++;
                gotoIO();
                
                
            }
            while(i<jobs_queue.top()->arrivialTime)
            {
                i++;
                gotoIO();
            }
            //if cpu brust is zero
            if(jobs_queue.top()->CPU_brusts.front()==0)
            {
                jobs_queue.top()->CPU_brusts.pop();
                //remove job if no more cpu brusts
                if(jobs_queue.top()->CPU_brusts.empty())
                {
                    complete[jobs_queue.top()->id]=i;

                    //the turnaround time will be completion time minus its arrivial time;
                    int turnaroundTimeNow=(i-(jobs_queue.top()->arrivialTime));
                    turnaroundTime+=turnaroundTimeNow;
                    //wating time will be total time spent inside cpu minus excuation time;
                    watingTime+=(turnaroundTimeNow-(jobs_queue.top()->totalTime));
                    jobs_queue.pop();

                }
                //if there is io move to io set
                else
                {
                    inIO.insert(jobs_queue.top());
                    jobs_queue.pop();
                }
            }
            //decrese the one cpu brust of top process
            else
            {
                jobs_queue.top()->CPU_brusts.front()-=1;
            }
            if(i%20==0)
            {
                if(!jobs_queue.empty())
                {
                    Jobs* temp=jobs_queue.top();
                    jobs_queue.pop();
                    temp->arrivialTime=i;
                    jobs_queue.push(temp);
                }
            }
          
            if(jobs_queue.empty() && inIO.empty())
            {
                showtime();
                break;
            }
        }
        
    }
      void gotoIO()
        {
            if(inIO.empty()){return;}
            set<Jobs*>::iterator itr;
            vector<set<Jobs*>::iterator>arr;
       
            for (itr = inIO.begin(); itr != inIO.end(); itr++) 
            {
                (*itr)->IO_brusts.front()-=1;
                //cout<<"decresed "<<(*itr)->IO_brusts.front()<<"at"<<i<<endl;
                if((*itr)->IO_brusts.front()==0)
                {
                    arr.push_back(itr);
                }
                
            }
            for(int i=0;i<arr.size();i++)
            {
                    (*arr[i])->IO_brusts.pop();
                    //cout<<(*arr[i])->IO_brusts.empty()<<endl;
                    (*arr[i])->arrivialTime=i;
                    //cout<<(*arr[i])->arrivialTime<<endl;
                    jobs_queue.push(*arr[i]);
                    //cout<<jobs_queue.empty()<<endl;
                    inIO.erase(arr[i]);
                    //cout<<inIO.empty()<<endl;
            }
            //cout<<"return ing"<<endl;
       
        }
        void showtime()
        {
            map<int,int>::iterator it;
            for(it=complete.begin();it!=complete.end();it++)
            {
                cout<<"______________________________________"<<endl;
                cout<<it->first<<" "<<it->second<<endl;
                cout<<"______________________________________"<<endl;

            }
                cout<<"______________________________________"<<endl;
                cout<<"______________________________________"<<endl;
                cout<<"avarage turnaround time"<<(turnaroundTime/30.0)<<endl;
                cout<<"avarage waiting time"<<(watingTime/30.0)<<endl;
                cout<<"______________________________________"<<endl;
                cout<<"______________________________________"<<endl;

        }
 

};









class FCFS
{
    public:
    priority_queue<Jobs*, vector< Jobs* >, Comparetime> jobs_queue;
    map<int,int>complete;
    set<Jobs*>inIO;
    int i;
    int watingTime,turnaroundTime;
    FCFS(vector<Jobs*>arr)
    {
        for(int i=0;i<arr.size();i++)
        {jobs_queue.push(arr[i]);}
        i=-1;
        watingTime=0;
        turnaroundTime=0;
    }
    void start()
    {
        
        while (true)
        {
            i++;
            gotoIO();
            //this is the idel time for the cpu as no prosses to excuate;
            while(jobs_queue.empty())
            {
                //cout<<"while job queue empty"<<endl;
                i++;
                gotoIO();
                
                
            }
            while(i<jobs_queue.top()->arrivialTime)
            {
                i++;
                gotoIO();
            }
            //if cpu brust is zero
            if(jobs_queue.top()->CPU_brusts.front()==0)
            {
                jobs_queue.top()->CPU_brusts.pop();
                //remove job if no more cpu brusts
                if(jobs_queue.top()->CPU_brusts.empty())
                {
                    complete[jobs_queue.top()->id]=i;

                    //the turnaround time will be completion time minus its arrivial time;
                    int turnaroundTimeNow=(i-(jobs_queue.top()->arrivialTime));
                    turnaroundTime+=turnaroundTimeNow;
                    //wating time will be total time spent inside cpu minus excuation time;
                    watingTime+=(turnaroundTimeNow-(jobs_queue.top()->totalTime));
                    jobs_queue.pop();

                }
                //if there is io move to io set
                else
                {
                    inIO.insert(jobs_queue.top());
                    jobs_queue.pop();
                }
            }
            //decrese the one cpu brust of top process
            else
            {
                jobs_queue.top()->CPU_brusts.front()-=1;
            }
          
            if(jobs_queue.empty() && inIO.empty()){showtime();break;}
        }
        
    }
      void gotoIO()
        {
            if(inIO.empty()){return;}
            set<Jobs*>::iterator itr;
            vector<set<Jobs*>::iterator>arr;
       
            for (itr = inIO.begin(); itr != inIO.end(); itr++) 
            {
                (*itr)->IO_brusts.front()-=1;
                //cout<<"decresed "<<(*itr)->IO_brusts.front()<<"at"<<i<<endl;
                if((*itr)->IO_brusts.front()==0)
                {
                    arr.push_back(itr);
                }
                
            }
            for(int i=0;i<arr.size();i++)
            {
                    (*arr[i])->IO_brusts.pop();
                    //cout<<(*arr[i])->IO_brusts.empty()<<endl;
                    (*arr[i])->arrivialTime=i;
                    //cout<<(*arr[i])->arrivialTime<<endl;
                    jobs_queue.push(*arr[i]);
                    //cout<<jobs_queue.empty()<<endl;
                    inIO.erase(arr[i]);
                    //cout<<inIO.empty()<<endl;
            }
            //cout<<"return ing"<<endl;
       
        }
        void showtime()
        {
            map<int,int>::iterator it;
            for(it=complete.begin();it!=complete.end();it++)
            {
                cout<<"______________________________________"<<endl;
                cout<<it->first<<" "<<it->second<<endl;
                cout<<"______________________________________"<<endl;

            }
                cout<<"______________________________________"<<endl;
                cout<<"______________________________________"<<endl;
                cout<<"avarage turnaround time"<<(turnaroundTime/30.0)<<endl;
                cout<<"avarage waiting time"<<(watingTime/30.0)<<endl;
                cout<<"______________________________________"<<endl;
                cout<<"______________________________________"<<endl;

        }
 

};




struct ComparePriority {
    bool operator()(Jobs* j1, Jobs* j2)
    {
        return j1->priority > j2->priority;
    }
};

class PRIORITY
{
    public:
    priority_queue<Jobs*, vector< Jobs* >, ComparePriority> jobs_queue;
    map<int,int>complete;
    set<Jobs*>inIO;
    int i;
    int watingTime,turnaroundTime;
    PRIORITY(vector<Jobs*>arr)
    {
        for(int i=0;i<arr.size();i++)
        {jobs_queue.push(arr[i]);}
        i=-1;
        watingTime=0;
        turnaroundTime=0;
    }
    void start()
    {
        
        while (true)
        {
            i++;
            gotoIO();
            //this is the idel time for the cpu as no prosses to excuate;
            while(jobs_queue.empty())
            {
                //cout<<"while job queue empty"<<endl;
                i++;
                gotoIO();
                
                
            }
            while(i<jobs_queue.top()->arrivialTime)
            {
                i++;
                gotoIO();
            }
            //if cpu brust is zero
            if(jobs_queue.top()->CPU_brusts.front()==0)
            {
                jobs_queue.top()->CPU_brusts.pop();
                //remove job if no more cpu brusts
                if(jobs_queue.top()->CPU_brusts.empty())
                {
                    complete[jobs_queue.top()->id]=i;

                    //the turnaround time will be completion time minus its arrivial time;
                    int turnaroundTimeNow=(i-(jobs_queue.top()->arrivialTime));
                    turnaroundTime+=turnaroundTimeNow;
                    //wating time will be total time spent inside cpu minus excuation time;
                    watingTime+=(turnaroundTimeNow-(jobs_queue.top()->totalTime));
                    jobs_queue.pop();

                }
                //if there is io move to io set
                else
                {
                    inIO.insert(jobs_queue.top());
                    jobs_queue.pop();
                }
            }
            //decrese the one cpu brust of top process
            else
            {
                jobs_queue.top()->CPU_brusts.front()-=1;
            }
          
            if(jobs_queue.empty() && inIO.empty()){showtime();break;}
        }
        
    }
      void gotoIO()
        {
            if(inIO.empty()){return;}
            set<Jobs*>::iterator itr;
            vector<set<Jobs*>::iterator>arr;
       
            for (itr = inIO.begin(); itr != inIO.end(); itr++) 
            {
                (*itr)->IO_brusts.front()-=1;
                //cout<<"decresed "<<(*itr)->IO_brusts.front()<<"at"<<i<<endl;
                if((*itr)->IO_brusts.front()==0)
                {
                    arr.push_back(itr);
                }
                
            }
            for(int i=0;i<arr.size();i++)
            {
                    (*arr[i])->IO_brusts.pop();
                    //cout<<(*arr[i])->IO_brusts.empty()<<endl;
                    (*arr[i])->arrivialTime=i;
                    //cout<<(*arr[i])->arrivialTime<<endl;
                    jobs_queue.push(*arr[i]);
                    //cout<<jobs_queue.empty()<<endl;
                    inIO.erase(arr[i]);
                    //cout<<inIO.empty()<<endl;
            }
            //cout<<"return ing"<<endl;
       
        }
        void showtime()
        {
            map<int,int>::iterator it;
            for(it=complete.begin();it!=complete.end();it++)
            {
                cout<<"______________________________________"<<endl;
                cout<<it->first<<" "<<it->second<<endl;
                cout<<"______________________________________"<<endl;

            }
                cout<<"______________________________________"<<endl;
                cout<<"______________________________________"<<endl;
                cout<<"avarage turnaround time"<<(turnaroundTime/30.0)<<endl;
                cout<<"avarage waiting time"<<(watingTime/30.0)<<endl;
                cout<<"______________________________________"<<endl;
                cout<<"______________________________________"<<endl;

        }
 

};











vector<Jobs*>allJobs1,allJobs2,allJobs3;

int randomGenrator()
{
   
    return rand();
}

Jobs* stringToJob(string str)
{
    vector<string> arr;
    vector<int>cpubrust;
    vector<int>ioburst;
    int i = 0;
    string s = "";
    do
    {
        s.clear();
        while ((str[i] !=' ') && (i < str.size()))
        {
            s += str[i];
            i++;
        }
        arr.push_back(s);
        i++;
    } 
    while (s != "-1");
    int id=stoi(arr[0]);
    int p=stoi(arr[1]);
    int at=stoi(arr[2]);
     i=3;
    while(i<arr.size())
    {
        if(arr[i]=="-1")
        {
            break;
        }
        if(i%2==0 )
        {
            ioburst.push_back(stoi(arr[i]));

        }
        else
        {
            cpubrust.push_back(stoi(arr[i]));
        }
        i++;
    }
    Jobs *temp=new Jobs(id,p,at,cpubrust,ioburst);
    //temp->show();
    allJobs1.push_back(temp);
    allJobs2.push_back(temp);
    allJobs3.push_back(temp);
    return temp;

}

int main()
{
    srand(time(0));
    // to store different jobs
    vector<Jobs> jobs;
    //
    map<int,bool>ids;
    
    ofstream myfile;
    int numberOfJobs = 20;
    myfile.open("jobs.txt");
    while (numberOfJobs > 0)
    {
        string str = "";
        int id ;
        do{
         id = randomGenrator() % 10000 + 1;
        }while(ids.find(id)!=ids.end());
        str += to_string(id) + " ";
        int priority = randomGenrator() % 10;
        str += to_string(priority) + " ";
        int arivalTime = randomGenrator() % 100;
        str += to_string(arivalTime) + " ";
        int numberOfBrust = randomGenrator() % 10 + 1;
        vector<int> cpuBruts(numberOfBrust);
        vector<int> ioBruts(numberOfBrust - 1);
        for (int i = 0; i < numberOfBrust; i++)
        {
            cpuBruts[i] = randomGenrator() % 20 + 1;
        }
        for (int i = 0; i < numberOfBrust - 1; i++)
        {
            ioBruts[i] = randomGenrator() % 20 + 1;
        }
        str += to_string(cpuBruts[0]) + " ";
        for (int i = 0; i < numberOfBrust - 1; i++)
        {
            str += to_string(ioBruts[i]) + " ";
            str += to_string(cpuBruts[i + 1]) + " ";
        }
        str += "-1";

        myfile << str << "\n";

        numberOfJobs--;
    }
    myfile.close();
    string line;

    ifstream myfile1("jobs.txt");
    if (myfile1.is_open())
    {
        while (getline(myfile1, line))
        {

            // getline(myfile,line);
            stringToJob(line);
            cout << line << '\n';
        }
        myfile1.close();
    }

    else
        {cout << "Unable to open file";}
        FCFS temp(allJobs1);
        temp.start();
        cout<<endl;
        PRIORITY temp2(allJobs2);
        temp2.start();
        cout<<endl;
        ROUND_ROBIN temp3(allJobs3);
        temp3.start();

    

    return 0;
}