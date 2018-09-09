#ifndef employee_h
#define employee_h

#include <iostream>
#include <string>
#include <sstream>
#include <sqlite3.h>
using namespace std;

struct Data
{
    char type;
    string name;
    int empno,grade;
    float accumpay,hour,sale,rate,basepay;
    friend ostream& operator <<(ostream& out,Data& d)
    {
        out<<"No:"<<d.empno<<'\n';
        out<<"Name:"<<d.name<<'\n';
        out<<"Type:"<<d.type<<'\n';
        out<<"Grade:"<<d.grade<<'\n';
        out<<"AccumPay:"<<d.accumpay<<'\n';
        if(d.hour>=0) out<<"Work Hour:"<<d.hour<<'\n';
        if(d.sale>=0) out<<"Sale:"<<d.sale<<'\n';
        if(d.rate>=0&&d.type=='t') out<<"Technician Rate:"<<d.rate<<'\n';
        if(d.rate>=0&&d.type=='s') out<<"Sale Rate:"<<d.rate<<'\n';
        if(d.basepay>=0) out<<"Base Pay:"<<d.basepay<<'\n';
        out<<'\n';
        return out;
    }
    friend istream& operator >>(istream& in,Data& d)
    {
        in>>d.type>>d.name>>d.empno>>d.grade>>d.accumpay>>d.hour>>d.sale>>d.rate>>d.basepay;
        return in;
    }
};

class Employee
{
protected:
    string name;
    int individualEmpNo;
    int grade;
    float accumPay;
    static int cnt;
public:
    Employee(int grd,float p,string s="NULL"):individualEmpNo(cnt),grade(grd),accumPay(p)
    {
        cnt++;
        name=s;
    }
    virtual Data todata()
    {
        Data d;
        d.type='e';
        d.name=name;
        d.empno=individualEmpNo;
        d.grade=grade;
        d.accumpay=accumPay;
        d.hour=d.sale=d.rate=d.basepay=-1;
        return d;
    }
    virtual void input(void)
    {
        printf("请输入员工姓名:");
        cin>>name;
        printf("请输入员工级别:");
        cin>>grade;
        printf("请输入员工工资:");
        cin>>accumPay;
    }
    virtual void info(void)
    {
        printf("员工编号:%d\n姓名:%s\n等级:%d\n月薪:%.2f\n\n",individualEmpNo,name.c_str(),grade,accumPay);
    }
    virtual float pay(void) const=0;
    virtual void promote(void)=0;
    static int currentNum(void) 
    {
        return cnt;
    }
};
int Employee::cnt=1;

class Manager:virtual public Employee
{
public:
    Manager(int grd,float p=12000,string s="NULL"):Employee(grd,p,s){}
    virtual float pay(void) const
    {
        return accumPay;
    }
    virtual void promote(void)
    {
        float p;
        printf("请输入新工资:");
        cin>>p;
        accumPay=p;
    }
    virtual Data todata()
    {
        Data d;
        d.type='m';
        d.name=name;
        d.empno=individualEmpNo;
        d.grade=grade;
        d.accumpay=pay();
        d.hour=d.sale=d.rate=d.basepay=-1;
        return d;
    }
    bool emergency1,emergency2,emergency3,emergency4,emergency5;
};

class Technician:public Employee
{
private:
    float hour;
    float rate;
public:
    Technician(int grd,float h=0,float r=260,string s="NULL"):Employee(grd,r*h,s),hour(h),rate(r){}
    void input(void)
    {
        printf("请输入员工姓名:");
        cin>>name;
        printf("请输入员工级别:");
        cin>>grade;
        printf("请输入员工工作时长:");
        cin>>hour;
        accumPay=pay();
    }
    void info(void)
    {
        printf("员工编号:%d\n姓名:%s\n等级:%d\n月薪:%.2f\n工作时长:%.2f\n\n",individualEmpNo,name.c_str(),grade,pay(),hour);
    }
    virtual float pay(void) const
    {
        return rate*hour;
    }
    virtual void promote(void)
    {
        float r;
        printf("请输入新时薪:");
        cin>>r;
        rate=r;
    }
    virtual Data todata()
    {
        Data d;
        d.type='t';
        d.name=name;
        d.empno=individualEmpNo;
        d.grade=grade;
        d.accumpay=pay();
        d.hour=hour;
        d.sale=d.basepay=-1;
        d.rate=rate;
        return d;
    }
    bool emergency1,emergency2,emergency3;
};

class Salesman:virtual public Employee
{
protected:
    float sale;
    float rate;
public:
    Salesman(int grd,float s=0,float r=0.05,string n="NULL"):Employee(grd,s*r,n),sale(s),rate(r){}
    void input(void)
    {
        printf("请输入员工姓名:");
        cin>>name;
        printf("请输入员工级别:");
        cin>>grade;
        printf("请输入员工销售额:");
        cin>>sale;
        accumPay=pay();
    }
    void info(void)
    {
        printf("员工编号:%d\n姓名:%s\n等级:%d\n月薪:%.2f\n销售额:%.2f\n\n",individualEmpNo,name.c_str(),grade,pay(),sale);
    }
    virtual float pay(void) const
    {
        return sale*rate;
    }
    virtual void promote(void)
    {
        float r;
        printf("请输入新提成率:");
        cin>>r;
        rate=r;
    }
    virtual Data todata()
    {
        Data d;
        d.type='s';
        d.name=name;
        d.empno=individualEmpNo;
        d.grade=grade;
        d.accumpay=pay();
        d.hour=d.basepay=-1;
        d.sale=sale;
        d.rate=rate;
        return d;
    }
    bool emergency1,emergency2,emergency3;
};

class SalesManager:public Manager,public Salesman
{
private:
    float basepay;
public:
    SalesManager(int grd,float bpay=8000,float s=0,float r=0.04,string n="NULL"):Employee(grd,bpay+s*r,n),Manager(grd,bpay+s*r),Salesman(grd,s,r),basepay(bpay){}
    virtual float pay(void) const
    {
        return basepay+rate*sale;
    }
    virtual void promote(void)
    {
        float b;
        printf("请输入新底薪:");
        cin>>b;
        basepay=b;
        float r;
        printf("请输入新提成率:");
        cin>>r;
        rate=r;
    }
    void info(void)
    {
        printf("员工编号:%d\n姓名:%s\n等级:%d\n月薪:%.2f\n所辖人员销售额:%.2f\n\n",individualEmpNo,name.c_str(),grade,pay(),sale);
    }
    void input(void)
    {
        printf("请输入员工姓名:");
        cin>>name;
        printf("请输入员工级别:");
        cin>>grade;
        printf("请输入所辖员工销售额:");
        cin>>sale;
        accumPay=pay();
    }
    virtual Data todata()
    {
        Data d;
        d.type='a';
        d.name=name;
        d.empno=individualEmpNo;
        d.grade=grade;
        d.accumpay=pay();
        d.hour=-1;
        d.sale=sale;
        d.rate=rate;
        d.basepay=basepay;
        return d;
    }
    bool safety1,safety2,safety3,safety4;
};

class DatabaseError
{
public:
    DatabaseError(int errcode,sqlite3 *db,string err="Mysterious Error!"):result(errcode),reason(err),errdb(db){ }
    void flee(void) const
    {
        cout<<"Can't open file:"<<reason<<endl;
        sqlite3_close(errdb);
        abort();
    }
    void carryon(void) const
    {
        cout<<"Operation falied:"<<reason<<endl;
    }
private:
    int result;
    string reason;
    sqlite3 *errdb;
};

string insertcmd(Data &d,char type,sqlite3 *db)
{
    stringstream ss;
    string st;
    if(type=='m')
    {
        ss<<"insert into manager (no,name,grade,accumpay) values ("<<d.empno<<",'"<<d.name<<"',"<<d.grade<<","<<d.accumpay<<")\n";
        getline(ss, st);
        return st;
    }
    else if (type=='t')
    {
        ss<<"insert into technician (no,name,grade,accumpay,worktime,payrate) values ("<<d.empno<<",'"<<d.name<<"',"<<d.grade<<","<<d.accumpay<<","<<d.hour<<","<<d.rate<<")\n";
        getline(ss, st);
        return st;
    }
    else if (type=='s')
    {
        ss<<"insert into salesman (no,name,grade,accumpay,sale,payrate) values ("<<d.empno<<",'"<<d.name<<"',"<<d.grade<<","<<d.accumpay<<","<<d.sale<<","<<d.rate<<")\n";
        getline(ss, st);
        return st;
    }
    else if (type=='a')
    {
        ss<<"insert into salesmanager (no,name,grade,accumpay,basepay,sale,payrate) values ("<<d.empno<<",'"<<d.name<<"',"<<d.grade<<","<<d.accumpay<<","<<d.basepay<<","<<d.sale<<","<<d.rate<<")\n";
        getline(ss, st);
        return st;
    }
    else
    {
        st='\0';
        return st;
    }
}

string updatecmd(Data &d,char type,sqlite3 *db,char sub,int upno)
{
    stringstream ss;
    string st;
    if(sub=='Y')
    {
        if(type=='m')
        {
            ss<<"update manager set accumpay="<<d.accumpay<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else if (type=='t')
        {
            ss<<"update technician set payrate="<<d.rate<<",accumpay="<<d.accumpay<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else if (type=='s')
        {
            ss<<"update salesman set payrate="<<d.rate<<",accumpay="<<d.accumpay<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else if (type=='a')
        {
            ss<<"update salesmanager set payrate="<<d.rate<<",basepay="<<d.basepay<<",accumpay="<<d.accumpay<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else
        {
            st='\0';
            return st;
        }
    }
    else
    {
        if(type=='m')
        {
            ss<<"update manager set name='"<<d.name<<"',grade="<<d.grade<<",accumpay="<<d.accumpay<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else if (type=='t')
        {
            ss<<"update technician set name='"<<d.name<<"',grade="<<d.grade<<",accumpay="<<d.accumpay<<",hour="<<d.hour<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else if (type=='s')
        {
            ss<<"update salesman set name='"<<d.name<<"',grade="<<d.grade<<",accumpay="<<d.accumpay<<",sale="<<d.sale<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else if (type=='a')
        {
            ss<<"update salesmanager set name='"<<d.name<<"',grade="<<d.grade<<",accumpay="<<d.accumpay<<",sale="<<d.sale<<" where no="<<upno<<"\n";
            getline(ss, st);
            return st;
        }
        else
        {
            st='\0';
            return st;
        }
    }
}

#endif /* employee_h */
