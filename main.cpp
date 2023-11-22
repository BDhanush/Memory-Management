#include <bits/stdc++.h>

using namespace std;

#define mod 1000000007
#define val 1000000001
#define debug(x) cout<< #x <<"="<<x<<endl
#define in(x) cin>>x
#define all(x) x.begin(),x.end()
#define PI 3.1415926535897932384626

void fit(int);

int repoli;

bool meas=false;

vector<int> memory(1024+1,0);

map<int,pair<int,int>> job;
map<int,int> jobsize;

vector<double> average_frag,avg_job_size,avg_hole_size,avg_number_of_holes_before,avg_jobs_no,avg_number_of_holes;
int frag_maxi=INT_MIN,frag_mini=INT_MAX,jobs_maxi=INT_MIN,jobs_mini=INT_MAX,holes_maxi=INT_MIN,holes_mini=INT_MAX;
int number_of_holes_before=0;
int con_noremoval=0,con_removal=0;
int con_noremoval_maxi=INT_MIN,con_removal_maxi=INT_MIN;

int generate(int n)
{
    if(n==1)
    {
        return 1+rand()%1024;
    }
    if(n==2)
    {
        return 1+rand()%100;
    }
    if(n==3)
    {
        return 500+rand()%501;
    }
}

void remove_job(int n)
{
    auto it=job.begin();
    advance(it,rand()%job.size());
    fill(memory.begin()+it->second.first,memory.begin()+it->second.second+1,0);
    job.erase(it);
    con_noremoval=0;
    con_removal++;
    return fit(n);
}

void first_fit(int n)
{
    int pi=0;
    for(int i=1;i<1025-jobsize[n]+1;i++)
    {
        auto it=find(memory.begin()+i,memory.begin()+i+jobsize[n],1);
        if(it==(memory.begin()+i+jobsize[n]))
        {
            fill(memory.begin()+i,memory.begin()+i+jobsize[n],1);
            job[n]={i,i+jobsize[n]-1};
            return;
        }else{
            pi=i-1;
            i=it-memory.begin();
            if(pi!=i-1)
            {
                number_of_holes_before++;
            }
        }
    }
    return remove_job(n);
}

void best_fit(int n)
{
    vector<int> ones;
    ones.push_back(0);
    for(int i=1;i<1025;i++)
    {
        if(memory[i])
        {
            ones.push_back(i);
        }
    }
    ones.push_back(1025);
    int best=INT_MAX,startfill=-1;
    for(int i=1;i<ones.size();i++)
    {
        if(ones[i]-ones[i-1]-1>0)
        {
            number_of_holes_before++;
        }
        if(ones[i]-ones[i-1]-1>=jobsize[n])
        {
            if(best>ones[i]-ones[i-1]-1)
            {
                best=ones[i]-ones[i-1]-1;
                startfill=ones[i-1]+1;
            }
        }
    }
    if(startfill==-1)
    {
        return remove_job(n);
    }
    fill(memory.begin()+startfill,memory.begin()+startfill+jobsize[n],1);
    job[n]={startfill,startfill+jobsize[n]-1};
}

void worst_fit(int n)
{
    vector<int> ones;
    ones.push_back(0);
    for(int i=1;i<1025;i++)
    {
        if(memory[i])
        {
            ones.push_back(i);
        }
    }
    ones.push_back(1025);
    int best=INT_MIN,startfill=-1;
    for(int i=1;i<ones.size();i++)
    {
        if(ones[i]-ones[i-1]-1>0)
        {
            number_of_holes_before++;
        }
        if(ones[i]-ones[i-1]-1>=jobsize[n])
        {
            if(best<ones[i]-ones[i-1]-1)
            {
                best=ones[i]-ones[i-1]-1;
                startfill=ones[i-1]+1;
            }
        }
    }
    if(startfill==-1)
    {
        return remove_job(n);
    }
    fill(memory.begin()+startfill,memory.begin()+startfill+jobsize[n],1);
    job[n]={startfill,startfill+jobsize[n]-1};
}

void fit(int n)
{
    con_noremoval++;
    if(repoli==1)
    {
        return best_fit(n);
    }
    if(repoli==2)
    {
        return worst_fit(n);
    }
    if(repoli==3)
    {
        return first_fit(n);
    }
}

void calculate(int i)
{
    average_frag.push_back(count(memory.begin()+1,memory.end(),0));
    frag_maxi=max((int)average_frag.back(),frag_maxi);
    frag_mini=min((int)average_frag.back(),frag_mini);
    double avg=0;
    for(auto i:job)
    {
        avg+=i.second.second+1-i.second.first;
    }
    avg/=job.size();
    avg_job_size.push_back(avg);
    avg=0;
    vector<int> ones;
    ones.push_back(0);
    for(int i=1;i<1025;i++)
    {
        if(memory[i])
        {
            ones.push_back(i);
        }
    }
    ones.push_back(1025);
    int holes=0;
    for(int i=1;i<ones.size();i++)
    {
        if(ones[i]-ones[i-1]-1>0)
        {
            holes++;
        }
        avg+=ones[i]-ones[i-1]-1;
    }
    if(holes!=0)
        avg/=holes;
    avg_hole_size.push_back(avg);
    avg_number_of_holes.push_back(holes);
    holes_maxi=max(holes_maxi,(int)avg_number_of_holes.back());
    holes_mini=min(holes_mini,(int)avg_number_of_holes.back());
    avg=0;
    avg_number_of_holes_before.push_back(number_of_holes_before);
    avg_jobs_no.push_back(job.size());
    jobs_maxi=max(jobs_maxi,(int)avg_jobs_no.back());
    jobs_mini=min(jobs_mini,(int)avg_jobs_no.back());
    con_noremoval_maxi=max(con_noremoval_maxi,con_noremoval);
    con_removal_maxi=max(con_removal_maxi,con_removal);

}

void simulation()
{
    int test=1;
    int n;
    //cin>>test;
    cin>>n>>repoli;
    for(int i=0;i<2000;i++)
    {
        if(i==1000)
        {
            meas=true;
        }
        int new_job=generate(n);
        jobsize[i]=new_job;
        number_of_holes_before=0;
        con_removal=0;
        fit(i);
        if(meas)
            calculate(i);
//        for(auto i:job)
//        {
//            cout<<i.first<<" "<<i.second.first<<" "<<i.second.second<<"\n";
//        }
//        cout<<"\n";
    }
    cout << (test==0?"":"Average fragmentation, ") << ((accumulate(all(average_frag),0.0))/average_frag.size())/1024*100 << " %\n";
    cout << (test==0?"":"Average hole size, ") << accumulate(all(avg_hole_size),0.0)/avg_hole_size.size() << "\n";
    cout << (test==0?"":"Average holes examined to satisfy partition, ") << accumulate(all(avg_number_of_holes_before),0.0)/avg_number_of_holes.size() << "\n";
    cout << (test==0?"":"Highest fragmentation, ") << (double)frag_maxi/1024*100 << " %\n";
    cout << (test==0?"":"Lowest fragmentation, ") << (double)frag_mini/1024*100 << " %\n";
    cout << (test==0?"":"Average number of jobs, ") << accumulate(all(avg_jobs_no),0.0)/avg_jobs_no.size() << "\n";
    cout << (test==0?"":"Highest number of jobs, ") << jobs_maxi << "\n";
    cout << (test==0?"":"Lowest number of jobs, ") << jobs_mini << "\n";
    cout << (test==0?"":"Average job size, ") << accumulate(all(avg_job_size),0.0)/avg_job_size.size() << "\n";
    cout << (test==0?"":"Average number of holes, ") << accumulate(all(avg_number_of_holes),0.0)/avg_number_of_holes.size() << "\n";
    cout << (test==0?"":"Highest holes recorded, ") << holes_maxi << "\n";
    cout << (test==0?"":"Lowest holes recorded, ") << holes_mini << "\n";
    cout << (test==0?"":"Largest number of partitions created in a row without evictions, ") << con_noremoval_maxi << "\n";
    cout << (test==0?"":"Largest number of evictions to satisfy a single partition request, ") << con_removal_maxi << "\n";

}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);
    cout.tie(NULL);

    int t=1;
    //cin>>t;
    srand(time(NULL));

    while(t--)
    {
        simulation();
    }
    return 0;
}
