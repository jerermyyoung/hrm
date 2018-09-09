#include "employee.h"
#include <fstream>

Employee** h =new Employee*[24];

int main()
{
    char ch;
    fstream out;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int choice,empno,rc,ncols,hr,sl;
    char subchoice,schoice;
    string name,filename,cmd,num;
    stringstream temp;
    bool run=true;
    Data d;
    Employee *p=nullptr;
    
    printf("输入数据库文件的文件名和路径。默认存储在程序所在路径，文件名为hrm.db。回车选择默认值\n");
    getline(cin, filename);
    if (filename.empty()) {
        try {
            rc=sqlite3_open("hrm.db", &db);
            if (rc) {
                throw DatabaseError(rc,db,sqlite3_errmsg(db));
            }
        } catch (DatabaseError err) {
            err.flee();
        }
    }
    else {
        try {
            rc=sqlite3_open(filename.c_str(), &db);
            if (!rc) {
                throw DatabaseError(rc,db,sqlite3_errmsg(db));
            }
        } catch (DatabaseError err) {
            err.flee();
        }
    }

    cmd="create table manager (no integer primary key,name text,grade integer,accumpay real)";
    try {
        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
        if (rc!=SQLITE_OK) {
            throw DatabaseError(rc,db,sqlite3_errmsg(db));
        }
    } catch (DatabaseError err) {
        err.carryon();
    }
    rc=sqlite3_step(stmt);
    
    cmd="create table technician (no integer primary key,name text,grade integer,accumpay real,worktime real,payrate real)";
    
    try {
        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
        if (rc!=SQLITE_OK) {
            throw DatabaseError(rc,db,sqlite3_errmsg(db));
        }
    } catch (DatabaseError err) {
        err.carryon();
    }
    rc=sqlite3_step(stmt);
    
    cmd="create table salesman (no integer primary key,name text,grade integer,accumpay real,sale real,payrate real)";
    
    try {
        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
        if (rc!=SQLITE_OK) {
            throw DatabaseError(rc,db,sqlite3_errmsg(db));
        }
    } catch (DatabaseError err) {
        err.carryon();
    }
    rc=sqlite3_step(stmt);
    
    cmd="create table salesmanager (no integer primary key,name text,grade integer,accumpay real,basepay real,sale real,payrate real)";
    
    try {
        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
        if (rc!=SQLITE_OK) {
            throw DatabaseError(rc,db,sqlite3_errmsg(db));
        }
    } catch (DatabaseError err) {
        err.carryon();
    }
    rc=sqlite3_step(stmt);
    
    h[0]=new Manager(3);
    for(int i=1;i<=10;i++) h[i]=new Technician(2,0);
    h[11]=new SalesManager(2);
    for(int i=12;i<24;i++) h[i]=new Salesman(1,0);
    
    int emp = 24;
    for (int i=0; i<emp; i++)
    {
        d=h[i]->todata();
        cmd=insertcmd(d, d.type, db);
        try {
            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
            if (rc!=SQLITE_OK) {
                throw DatabaseError(rc,db,sqlite3_errmsg(db));
            }
        } catch (DatabaseError err) {
            err.carryon();
        }
        rc=sqlite3_step(stmt);
    }
    
    while(run)
    {
        printf("欢迎使用人力资源管理系统\n");
        printf("菜单:\n1.输入员工信息\n2.修改员工信息\n3.打印员工信息\n4.查询员工信息\n5.保存员工信息（纯文本文件）\n6.退出程序\n");
        cin>>choice;
        switch(choice)
        {
            case 1:
                printf("请输入员工职位：经理输入m,技术人员输入t,销售人员输入s，销售经理输入a\n");
                cin>>subchoice;
                switch (subchoice){
                    case 'm':
                        p=new Manager(3);
                        break;
                    case 't':
                        p=new Technician(2);
                        break;
                    case 's':
                        p=new Salesman(1);
                        break;
                    case 'a':
                        p=new SalesManager(3);
                        break;
                    default:
                        break;
                }
                if(!(p==nullptr))
                {
                    p->input();
                    d=p->todata();
                    cmd=insertcmd(d, d.type, db);
                    try {
                        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                        if (rc!=SQLITE_OK) {
                            throw DatabaseError(rc,db,sqlite3_errmsg(db));
                        }
                    } catch (DatabaseError err) {
                        err.carryon();
                    }
                    rc=sqlite3_step(stmt);
                }
                emp++;
                break;
            case 2:
                empno=-1;
                while(empno<1||empno>emp)
                {
                    printf("请输入员工编号:");
                    cin>>empno;
                    if(empno<1||empno>emp)
                    {
                        printf("编号有误，请重试\n");
                    }
                }
                temp.str("");
                temp<<empno;
                num=temp.str();
                subchoice=' ';
                
                cmd="select * from manager where no="+num+"";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                while (rc==SQLITE_ROW) {
                    if (sqlite3_column_int(stmt, 0)==empno) {
                        subchoice='m';
                        cout<<"Manager\n";
                        for (int i=0; i<ncols; i++) {
                            cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                    }
                    rc=sqlite3_step(stmt);
                }
                
                cmd="select * from technician where no="+num+"";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                while (rc==SQLITE_ROW) {
                    if (sqlite3_column_int(stmt, 0)==empno) {
                        subchoice='t';
                        cout<<"Technician\n";
                        for (int i=0; i<ncols; i++) {
                            cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                        hr=sqlite3_column_double(stmt, 4);
                    }
                    rc=sqlite3_step(stmt);
                }
                
                cmd="select * from salesman where no="+num+"";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                while (rc==SQLITE_ROW) {
                    if (sqlite3_column_int(stmt, 0)==empno) {
                        subchoice='s';
                        cout<<"Salesman\n";
                        for (int i=0; i<ncols; i++) {
                            cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                        sl=sqlite3_column_double(stmt, 4);
                    }
                    rc=sqlite3_step(stmt);
                }
                
                cmd="select * from salesmanager where no="+num+"";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                while (rc==SQLITE_ROW) {
                    if (sqlite3_column_int(stmt, 0)==empno) {
                        subchoice='a';
                        cout<<"Salesmanager\n";
                        for (int i=0; i<ncols; i++) {
                            cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                        sl=sqlite3_column_double(stmt, 5);
                    }
                    rc=sqlite3_step(stmt);
                }
                
                switch (subchoice){
                    case 'm':
                        p=new Manager(3);
                        break;
                    case 't':
                        p=new Technician(2);
                        break;
                    case 's':
                        p=new Salesman(1);
                        break;
                    case 'a':
                        p=new SalesManager(3);
                        break;
                    default:
                        break;
                }
                schoice=subchoice;
                printf("是否为工资调级？是请输入Y，否请输入N\n");
                cin>>subchoice;
                if(subchoice=='Y')
                {
                    p->promote();
                    d=p->todata();
                    if(schoice=='t')
                        d.accumpay=hr*d.rate;
                    else if (schoice=='s')
                        d.accumpay=sl*d.rate;
                    else if (schoice=='a')
                        d.accumpay=sl*d.rate+d.basepay;
                        
                    cmd=updatecmd(d, d.type, db, subchoice, empno);
                    try {
                        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                        if (rc!=SQLITE_OK) {
                            throw DatabaseError(rc,db,sqlite3_errmsg(db));
                        }
                    } catch (DatabaseError err) {
                        err.carryon();
                    }
                    rc=sqlite3_step(stmt);
                }
                else
                {
                    p->input();
                    d=p->todata();
                    cmd=updatecmd(d, d.type, db, subchoice, empno);
                    try {
                        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                        if (rc!=SQLITE_OK) {
                            throw DatabaseError(rc,db,sqlite3_errmsg(db));
                        }
                    } catch (DatabaseError err) {
                        err.carryon();
                    }
                    rc=sqlite3_step(stmt);
                }
                
                break;
            case 3:
                ch=getchar();
                cmd="select * from manager";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                cout<<"Manager\n\n";
                while (rc==SQLITE_ROW) {
                    for (int i=0; i<ncols; i++) {
                        cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                    }
                    cout<<'\n';
                    rc=sqlite3_step(stmt);
                }
                
                cmd="select * from technician";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                cout<<"Technician\n\n";
                while (rc==SQLITE_ROW) {
                    for (int i=0; i<ncols; i++) {
                        cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                    }
                    cout<<'\n';
                    rc=sqlite3_step(stmt);
                }
                
                cmd="select * from salesman";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                cout<<"Salesman\n\n";
                while (rc==SQLITE_ROW) {
                    for (int i=0; i<ncols; i++) {
                        cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                    }
                    cout<<'\n';
                    rc=sqlite3_step(stmt);
                }
                
                cmd="select * from salesmanager";
                try {
                    rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                    if (rc!=SQLITE_OK) {
                        throw DatabaseError(rc,db,sqlite3_errmsg(db));
                    }
                } catch (DatabaseError err) {
                    err.carryon();
                }
                rc=sqlite3_step(stmt);
                ncols=sqlite3_column_count(stmt);
                cout<<"Salesmanager\n\n";
                while (rc==SQLITE_ROW) {
                    for (int i=0; i<ncols; i++) {
                        cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                    }
                    cout<<'\n';
                    rc=sqlite3_step(stmt);
                }
                
                printf("按任意键回到主菜单\n");
                while(!getchar()) ;
                break;
            case 4:
                printf("请选择查询方式：输入姓名查询请输入s，输入编号查询请输入n\n");
                cin>>subchoice;
                switch (subchoice){
                    case 's':
                        printf("请输入员工姓名:");
                        cin>>name;
                        
                        cmd="select * from manager where name='"+name+"'";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (!strcmp((char*) sqlite3_column_text(stmt, 1),name.c_str())) {
                                cout<<"Manager\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }
                        
                        cmd="select * from technician where name='"+name+"'";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (!strcmp((char*) sqlite3_column_text(stmt, 1),name.c_str())) {
                                cout<<"Technician\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }
                        
                        cmd="select * from salesman where name='"+name+"'";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (!strcmp((char*) sqlite3_column_text(stmt, 1),name.c_str())) {
                                cout<<"Salesman\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }
                        
                        cmd="select * from salesmanager where name='"+name+"'";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (!strcmp((char*) sqlite3_column_text(stmt, 1),name.c_str())) {
                                cout<<"Salesmanager\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }

                        break;
                    case 'n':
                        printf("请输入员工编号:");
                        cin>>empno;
                        temp.str("");
                        temp<<empno;
                        num=temp.str();
                        
                        cmd="select * from manager where no="+num+"";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (sqlite3_column_int(stmt, 0)==empno) {
                                cout<<"Manager\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }
                        
                        cmd="select * from technician where no="+num+"";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (sqlite3_column_int(stmt, 0)==empno) {
                                cout<<"Technician\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }
                        
                        cmd="select * from salesman where no="+num+"";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (sqlite3_column_int(stmt, 0)==empno) {
                                cout<<"Salesman\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }
                        
                        cmd="select * from salesmanager where no="+num+"";
                        try {
                            rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                            if (rc!=SQLITE_OK) {
                                throw DatabaseError(rc,db,sqlite3_errmsg(db));
                            }
                        } catch (DatabaseError err) {
                            err.carryon();
                        }
                        rc=sqlite3_step(stmt);
                        ncols=sqlite3_column_count(stmt);
                        while (rc==SQLITE_ROW) {
                            if (sqlite3_column_int(stmt, 0)==empno) {
                                cout<<"Salesmanager\n";
                                for (int i=0; i<ncols; i++) {
                                    cout<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                                }
                                cout<<'\n';
                            }
                            rc=sqlite3_step(stmt);
                        }
    
                        break;
                    default:
                        break;
                }
                ch=getchar();
                printf("按任意键回到主菜单\n");
                while(!getchar()) ;
                break;
            case 5:
                printf("请输入文件名（若需存到程序所在目录之外请输入路径）\n");
                cin>>filename;
                if(!filename.empty())
                {
                    out.open(filename,ios::out);
                    cmd="select * from manager";
                    try {
                        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                        if (rc!=SQLITE_OK) {
                            throw DatabaseError(rc,db,sqlite3_errmsg(db));
                        }
                    } catch (DatabaseError err) {
                        err.carryon();
                    }
                    rc=sqlite3_step(stmt);
                    ncols=sqlite3_column_count(stmt);
                    out<<"Manager\n\n";
                    while (rc==SQLITE_ROW) {
                        for (int i=0; i<ncols; i++) {
                            out<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                        out<<'\n';
                        rc=sqlite3_step(stmt);
                    }
                    
                    cmd="select * from technician";
                    try {
                        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                        if (rc!=SQLITE_OK) {
                            throw DatabaseError(rc,db,sqlite3_errmsg(db));
                        }
                    } catch (DatabaseError err) {
                        err.carryon();
                    }
                    rc=sqlite3_step(stmt);
                    ncols=sqlite3_column_count(stmt);
                    out<<"Technician\n\n";
                    while (rc==SQLITE_ROW) {
                        for (int i=0; i<ncols; i++) {
                            out<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                        out<<'\n';
                        rc=sqlite3_step(stmt);
                    }
                    
                    cmd="select * from salesman";
                    try {
                        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                        if (rc!=SQLITE_OK) {
                            throw DatabaseError(rc,db,sqlite3_errmsg(db));
                        }
                    } catch (DatabaseError err) {
                        err.carryon();
                    }
                    rc=sqlite3_step(stmt);
                    ncols=sqlite3_column_count(stmt);
                    out<<"Salesman\n\n";
                    while (rc==SQLITE_ROW) {
                        for (int i=0; i<ncols; i++) {
                            out<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                        out<<'\n';
                        rc=sqlite3_step(stmt);
                    }
                    
                    cmd="select * from salesmanager";
                    try {
                        rc=sqlite3_prepare_v2(db, cmd.c_str(), (int) cmd.length(), &stmt, nullptr);
                        if (rc!=SQLITE_OK) {
                            throw DatabaseError(rc,db,sqlite3_errmsg(db));
                        }
                    } catch (DatabaseError err) {
                        err.carryon();
                    }
                    rc=sqlite3_step(stmt);
                    ncols=sqlite3_column_count(stmt);
                    out<<"Salesmanager\n\n";
                    while (rc==SQLITE_ROW) {
                        for (int i=0; i<ncols; i++) {
                            out<<sqlite3_column_name(stmt, i)<<":"<<sqlite3_column_text(stmt, i)<<'\n';
                        }
                        out<<'\n';
                        rc=sqlite3_step(stmt);
                    }
                    ch=getchar();
                    out.close();
                    printf("文件另存为完毕，按任意键回到主菜单\n");
                    while(!getchar()) ;
                }
                else
                {
                    ch=getchar();
                    printf("请输入文件名称!\n");
                    while(!getchar()) ;
                }
                break;
            case 6:
                run=false;
                printf("欢迎再次使用，再见！\n");
                break;
        }
    }
    return 0;
}

