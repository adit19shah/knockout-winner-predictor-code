#include<bits/stdc++.h>
using namespace std;

#define int long long 
#define fast ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0)
#define precise(x) fixed << setprecision(x) 
 
// allocate enough space as global arrays to avoid segmentation fault.
// Space complexity = O(3^n) where n=number of teams.
// we have allocated space to run answer for 16 teams. 
// More space needs to be allocated if more teams are to considered

int pow3[20]; 
int fact[20];
int ways[20];
double prob[20][20]; 
double dp1[50000000]; 
double dp2[50000000];
double ans[20];


int count_set_bits(int mask)
{
    int i,j,cnt;
    cnt=0;
    while(mask!=0)
    {
        if(mask%2==1)
        {
            cnt++;
        }
        mask=mask/2;
    }
    return cnt;
}


bool valid_Mask(int mask)
{
    //The number of set bits in the mask should be power of 2
    int i,j;

    // i is number of set bits
    i=__builtin_popcount(mask);

    if(i==0)
    {
        return 0;
    }
    
    while(i>1)
    {
        if(i%2==1)
        {
            //number of set bits not a power of 2
            return 0;
        }
        i=i/2;
    }
    
    // mask is a valid mask
    return 1;
}

int get_Bit(int mask,int i)
{
    // this function is for getting value of bit in dp2
    // normal bitwise operators don't work
    // since we have 3 possible values - 0,1 and 2 

    int b;
    mask=mask%pow3[i+1];
    b=mask/pow3[i];
    return b;
}


double calculate_Probability(int mask,int n)
{
    int i,j,k,b1,b2,b;

    // don't relcalculate if already memoised
    if(dp2[mask]!=-1)
    {
        return dp2[mask];
    }
        
    double p=0.0;
    int new_mask;
    
    //double d=1.00/(double)b;
    
    for(i=n-1;i>=0;i--)
    {
        b1=get_Bit(mask,i); // last losing bit
        if(b1==2)
        {
            for(j=0;j<n;j++)
            {
                b2=get_Bit(mask,j); //all winning bits
                if(b2==1)
                {
                    new_mask=mask-1*pow3[j]-2*pow3[i];
                    p+=prob[j][i]*calculate_Probability(new_mask,n);
                }
            }
            break;
        }
    }

    //memoise the answer for dp2
    dp2[mask]=p;
    return p;
}


double Prob_Mask_To_Submask(int mask,int submask,int n)
{
    int i,j,k;
    double p; // stores final probability

    // 0 bit --> team already eliminated
    // 1 bit --> team expected to win
    // 2 bit --> team expected to lose

    int new_mask=0;
    for(i=0;i<n;i++)        
    {
        if(((mask&(1<<i))!=0) && ((submask&(1<<i))!=0)) 
        {
            //team i wins
            new_mask+=1*pow3[i];
        }
        else if(((mask&(1<<i))!=0) && ((submask&(1<<i))==0)) 
        {
            //team i loses
            new_mask+=2*pow3[i];
        }
        else 
        {
            //team i has already lost
            new_mask+=0*pow3[i];
        }
    }
    
    p=calculate_Probability(new_mask,n);
    return p;

}

void solve(int n)
{
    int i,j,k,m;
    double d;
    
    //store powers of 3s for future use
    pow3[0]=1;
    for(i=1;i<=n;i++)
    {
        pow3[i]=pow3[i-1]*3;
    }

    //storing ways to play games with 2*i teams for future use
    ways[1]=1;
    for(i=2;i<=n/2;i++)
    {
        ways[i]=ways[i-1]*(2*i-1);
    }
    
    //intialize dp1 for bottom-up approach
    for(i=0;i<(1<<n);i++)
    {
        dp1[i]=0.00;
    }

    // base case for dp1, initially all teams are in game
    dp1[(1<<n)-1]=1.00;

    //initialise dp2 for top-down approach
    for(i=0;i<pow3[n];i++)
    {
        dp2[i]=-1.00;   //initialize with some dummmy value
    }

    //base case for dp2, finally all players will be either winners or losers
    dp2[0]=1.00;

    // 0 bit --> team already eliminated
    // 1 bit --> team still in tournament
    for(i=(1<<n)-1;i>=0;i--)
    {
        if(valid_Mask(i)==1)
        {
            //Optimal way of enumerating submasks
            for(j=i;j>0;j=(j-1)&i)
            {
                //Only half teams should proceed in next round
                if(count_set_bits(i)==2*count_set_bits(j))
                {
                    m=__builtin_popcountll(i);
                    //ways[m/2] indicates number of ways to play m/2 games
                    k=ways[m/2];
                    d=1.00/(double)k;
                    dp1[j]+=d*dp1[i]*Prob_Mask_To_Submask(i,j,n);
                }
            }
        }
    }

    // store final answer 
    for(i=0;i<n;i++)
    {
        ans[i]=dp1[(1<<i)];
    }
    
}

signed main()
{
    //for fast input output
    fast;
    //give ouputs with decimal precision of 4
    cout<<precise(4);

    ///*
    // This section can be uncommented if you want to 
    // give input as a file and get output in a file
    
    #ifndef ONLINE_JUDGE
    // for getting input from input.txt
    freopen("input.txt", "r", stdin);
    // for writing output to output.txt
    freopen("output.txt", "w", stdout);
    #endif
    //*/
    
    int i,j,k,n;
    //input number of teams
    cin>>n;
    
    //give input probability
    //prob[i][j]=probability of winning for team i over team j
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            cin>>prob[i][j];
        }
    }

    
    cout<<"The number of teams are "<<n<<"."<<endl;
    cout<<endl;
    
    cout<<"The probability table is as follows: "<<endl;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            cout<<prob[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;
    

    //solve the question for n teams with given probability table
    solve(n);
    
    //print answer
    for(i=0;i<n;i++)
    {
        cout<<"Probability of team "<<i+1<<" winning the tournament is "<<ans[i]<<"."<<endl;
    }
    
}