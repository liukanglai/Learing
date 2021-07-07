# class

        #include<iostream>
        #include<cstring>
        
        using namespace std;
        
        class Student
        {
            private:
            int num;//学号
            char name[100];//名字
            int score;//成绩
            public:
            Student(int n,char *str,int s);
            int print();
            int Set(int n,char *str,int s);
        };
        
        Student::Student(int n,char *str,int s)
        {
             num = n;
             strcpy(name,str);
             score = s;
        cout<<"Constructor"<<endl;
        }
        
        int Student::print()
        {
            cout<<num<<" "<<name<<" "<<score;
            return 0;
        }
        
        int Student::Set(int n,char *str,int s)
        {
             num = n;
             strcpy(name,str);
             score = s;
        }
        
        int main()
        {
            Student A(100,"dotcpp",11);
            A.print();
            return 0;
        }
