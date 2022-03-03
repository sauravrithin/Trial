#include<bits/stdc++.h>
using namespace std;
/*class User
{
    public :
        string userName;
        string userRole;

};*/
class Role
{
    public:
        string roleName;
        string seniorRole;
        static int count;
        int roleId;
        list<string> userList;
        list<string> juniors;
        void setId()
        {
            count++;
            this->roleId=count;
        }
        Role()
        {
            roleId=-1;
        }
        Role(string newRole)
        {
            setId();
            roleName=newRole;
            this->seniorRole="\0";
        }
        Role(string newRole,string seniorRole)
        {
            setId();
            roleName=newRole;
            this->seniorRole=seniorRole;
        }
        
};
class Graph
{
    public:
        //string rootRole;
        //int rootId;
        Role rootRole;
        list<Role> hierarchy;
        list<pair<string,string>> allUser;
        void display()
        {
            //cout<<"HI"<<endl;
            queue<string> que;
            que.push(rootRole.roleName);
            while(!que.empty())
            {
                string currRoleName=que.front();//.roleName;
                Role *currRole=findRoleByName(currRoleName);
                for(string juniorRoles : currRole->juniors)
                {
                    que.push(juniorRoles);
                }
                if(rootRole.roleName!=currRoleName)
                cout<<",";
                cout<<currRoleName;
                que.pop();
            }
            cout<<endl;
        }
        void setRoot(Role r)
        {
            rootRole=r;//.roleName;
            //rootId=r.roleId;
            hierarchy.push_back(r);
        }
        Role *findRoleByName(string Name)
        {
            for(Role &senior : hierarchy)
            {
                if(senior.roleName==Name)//==0)
                return &senior;
            }
            cout<<"Role not found\n";
            Role noRole;
            return NULL;
        }
        void addRole(Role r)
        {
            hierarchy.push_back(r);
            Role *senior=findRoleByName(r.seniorRole);
            //Role *newRole=findRoleByName(r.roleName);
            if(senior!=NULL)
            senior->juniors.push_back(r.roleName);    
        }
        void deleteRole(string removeRole,string seniorRole)
        {
            Role *toDelete=findRoleByName(removeRole);
            Role *seniorOfDelete=findRoleByName(toDelete->seniorRole);
            Role *newSenior=findRoleByName(seniorRole);
            seniorOfDelete->juniors.remove(removeRole);
            newSenior->juniors.merge(toDelete->juniors);
            newSenior->userList.merge(toDelete->userList);
            //hierarchy.erase(toDelete);
            list<Role> temp;
            for(Role &r : hierarchy)
            {
                if(r.roleName!=removeRole)
                {
                    temp.push_back(r);
                }
            }
            hierarchy=temp;
            for(auto &userPair : allUser)
            {
                if(userPair.second==removeRole)
                    userPair.second=seniorRole;
            }
        }
        list<string> mergeUser(Role *currRole)
        {
            list<string> subs,currRoleUser;
            currRoleUser=currRole->userList;
            for(auto subRole : currRole->juniors)
            {
                subs.merge(mergeUser(findRoleByName(subRole)));
            }
            subs.merge(currRoleUser);
            return subs;
        }
        list<string> dfsOnUser(string Name)
        {
            list<string> subs,currRoleUser;
            Role *currRole=findRoleByName(Name);
            currRoleUser=currRole->userList;
            for(auto subRole : currRole->juniors)
            {
                subs.merge(dfsOnUser(subRole));
            }
            for(auto &currUser : currRoleUser)
            {
                cout<<currUser<<"->";
                for(auto strIt=subs.begin();strIt!=subs.end();strIt++)
                {
                    if(strIt!=subs.begin())
                    cout<<",";
                    cout<<(*strIt);
                }
                cout<<endl;
            }
            subs.merge(currRoleUser);
            return subs;
        }

        
        int maxHieght(string Name)
        {
            Role *currRole=findRoleByName(Name);
            int maxOfJunior=0;
            if(currRole->juniors.empty())
            return 0;
            else
            {   
                for(string &juniorRole : currRole->juniors)
                    maxOfJunior=max(maxOfJunior,maxHieght(juniorRole));
                return 1+maxOfJunior;
            }
        }
        void lcaOfUsers(Role *currRole,string user1,string user2)
        {
           list<string> subs;
           bool found1=false,found2=false;
           if(currRole->userList.empty())
           {
               for(auto &junior : currRole->juniors)
               {
                    lcaOfUsers(findRoleByName(junior),user1,user2);
               }
           }
           else
           {
               subs=mergeUser(currRole);
               for(auto &str : subs)
                {
                if(str==user1)
                found1=true; 
                if(str==user2)
                found2=true;
                }
                if(found1 && found2)
                {
                    cout<<"Top most common bosses :"<<endl;
                    for(auto str=currRole->userList.begin();str!=currRole->userList.end();str++)
                    {
                        if(str!=currRole->userList.begin())
                        cout<<",";
                        cout<<(*str);
                    }
                    cout<<endl;
                }
           }
        }
};
int Role::count=0;
int main()
{
    int choice,countUser;
    Graph g;
    Role r;
    Role *roleOfUser;
    string newRole,seniorRole,removeRole,Name,user1,user2;
    cout<<"Enter root role name :";
    //cin.ignore();
    getline(cin,newRole);
    r=Role(newRole);
    g.setRoot(r);
    cout<<"Company Owner :"<<newRole<<endl;
    do
    {
        cout<<"-----------------------------------------------------------------------------"<<endl;
        cout<<"OPERATIONS:"<<endl;
        cout<<"\t1.Add Sub Role\n";
        cout<<"\t2.Display Roles\n";
        cout<<"\t3.Delete Role\n";
        cout<<"\t4.Add User\n";
        cout<<"\t5.Display Users\n";
        cout<<"\t6.Display Users and Sub Users\n";
        cout<<"\t7.Delete User\n";
        cout<<"\t8.Number of users from top\n";
        cout<<"\t9.Height of role hierarchy\n";
        cout<<"\t10.Common boss of users\n";
        cout<<"\t0.Exit\n";
        cin>>choice;
        countUser=0;
        if(choice>0 && choice<=10)
        cout<<"-----------------------------------------------------------------------------"<<endl;
        cout<<"Operation to be performed : "<<choice<<endl;
        switch(choice)
        {
            case 0 :break;
            case 1 :cout<<"Enter sub role name :";
                    cin.ignore();
                    getline(cin,newRole);
                    //cout<<newRole<<endl;
                    cout<<"Enter reporting to role name :";
                    //cin.ignore();
                    getline(cin,seniorRole);
                    //cout<<seniorRole<<endl;
                    r=Role(newRole,seniorRole);
                    g.addRole(r);
                    break;
            case 2 :g.display();
                    break;
            case 3 :cin.ignore();
                    cout<<"Enter the role to be deleted :";
                    getline(cin,removeRole);
                    cout<<"Enter the role to be transferred :";
                    getline(cin,seniorRole);
                    g.deleteRole(removeRole,seniorRole);
                    break;
            case 4 :cin.ignore();
                    cout<<"Enter user name :";
                    getline(cin,Name);
                    cout<<"Enter role :";
                    getline(cin,newRole);
                    roleOfUser=g.findRoleByName(newRole);
                    roleOfUser->userList.push_back(Name);
                    g.allUser.push_back(make_pair(Name,newRole));
                    break;
            case 5 :for(auto &userPair : g.allUser)
                    {
                        cout<<userPair.first<<"->"<<userPair.second<<endl;
                    }
                    break;
            case 6 ://g.displaySubUser();
                    g.dfsOnUser(g.rootRole.roleName);
                    break;
            case 7 :cin.ignore();
                    cout<<"Enter user name to be deleted :";
                    getline(cin,Name);
                    newRole="\0";
                    for(auto userPair=g.allUser.begin();userPair!=g.allUser.end();userPair++)
                    {
                        if(userPair->first==Name)
                        {
                            newRole=userPair->second;
                            g.allUser.erase(userPair);
                            break;
                        }
                    }
                    if(newRole!="\0")
                    {
                        roleOfUser=g.findRoleByName(newRole);
                    if(roleOfUser!=NULL)
                        roleOfUser->userList.remove(Name);
                    }
                    else
                    cout<<"Not found!\n";
                    break;
            case 8 :cin.ignore();
                    cout<<"Enter user name :";
                    getline(cin,Name);
                    newRole="\0";
                    for(auto userPair=g.allUser.begin();userPair!=g.allUser.end();userPair++)
                    {
                        if(userPair->first==Name)
                        {
                            newRole=userPair->second;
                            break;
                        }
                    }
                    if(newRole!="\0")
                    {
                        roleOfUser=g.findRoleByName(newRole);
                    }
                    while(roleOfUser->seniorRole!="\0")
                    {
                        roleOfUser=g.findRoleByName(roleOfUser->seniorRole);
                        if(roleOfUser==NULL)
                        break;
                        countUser+=roleOfUser->userList.size();
                    }
                    cout<<"Number of users from top :"<<countUser<<endl;
                    break;
            case 9 :cout<<"Hieght of hierarchy :"<<g.maxHieght(g.rootRole.roleName)+1<<endl;
                    break;
            case 10 :cin.ignore();
                    cout<<"Enter user 1 :";
                    getline(cin,user1);
                    cout<<"Enter user 2 :";
                    getline(cin,user2);
                    g.lcaOfUsers(g.findRoleByName(g.rootRole.roleName),user1,user2);
                    //cout<<"Role LCA :"<<roleOfUser->roleName<<endl;
                    break;
            default :cout<<"Invalid operation choice! Try Again!\n";
        }
    } while (choice!=0);

    return 0;
}