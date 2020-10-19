# LL1

## 数据结构的使用

### PredictionMatrix

#### 实现思路

内部使用了嵌套的字典：

~~~c++
std::map<std::string, std::map<std::string, std::string> > data;
~~~



#### 功能函数

~~~c++
/**
*  返回是否有 Select ( S -> 某 ) = {a, ...}
*
*  @Param S: 预测矩阵的行索引 ( 例子中的 S )
*  @Param a: 预测矩阵的列索引 ( 例子中的 a )
*  @Returns: 是否存在例子中的 Select 集
*/
bool has(const std::string& S, const std::string& a);


/**
*  如果有，则返回 Select ( S -> 某 ) = {a, ...} 的 "某"
*  否则，返回空串"\0"
*  
*  @Param S: 预测矩阵的行索引 ( 例子中的 S )
*  @Param a: 预测矩阵的列索引 ( 例子中的 a )
*  @Returns: 例子中的 "某"
*/
std::string get(const std::string& S, const std::string& a);


/**
*  增加一条 Select ( S -> 某 ) = {a, ...} 
*  
*  @Param S: 预测矩阵的行索引 ( 例子中的 S )
*  @Param a: 预测矩阵的列索引 ( 例子中的 a )
*  @Param exp: 例子中的 "某"
*/
void add(const std::string& S, const std::string& a, const std::string& exp);
~~~

#### 示例程序

~~~c++
#include <iostream>
#include "PredictionMatrix.h"
using namespace std;

int main() {
    // 定义一个预测矩阵
    PredictionMatrix m;
    
    // 查看预测矩阵中，这个行列索引是否合法
    cout << m.has("aa", "bb") << endl;
    // 输出结果: 0

    // 通过行列索引获得预测矩阵中的值
    cout << m.get("aa", "bb") << endl;
    // 输出结果: 

    // 增加一条语法展开式的 Select 集
    m.add("aa", "bb", "CCC");

    // 查看预测矩阵中，这个行列索引是否合法
    cout << m.has("aa", "bb") << endl;
    // 输出结果: 1

    // 通过行列索引获得预测矩阵中的值
    cout << m.get("aa", "bb") << endl;
    // 输出结果: CCC
}
~~~

### Grammar

#### 实现思路

pair 向量，并实现迭代器等

~~~c++
std::vector<std::pair<std::string, std::string> > data;
~~~

#### 功能函数

~~~c++
// 返回起始迭代器
Grammar::iterator begin();

// 返回结束迭代器
Grammar::iterator end();

// 返回语法的数量
int size() const;

// 插入一条新的语法
void insert(const std::string& S, const std::string& a);
~~~

#### 迭代器支持

~~~c++
// 获取语法左端值
std::string& getKey() const;

// 获取语法右端值
std::string& getValue() const;

// 迭代器赋值符号重载
iterator& operator=(const iterator& iter);

// 迭代器判断全等符号重载
bool operator==(const iterator& iter);

// 迭代器判断不等符号重载
bool operator!=(const iterator& iter);

// 迭代器后缀自增符号重载
iterator operator++(int n);

// 迭代器前缀自增符号重载
iterator& operator++();

// 迭代器后缀自减符号重载
iterator operator--(int n);

// 迭代器前缀自减符号重载
iterator& operator--();

// 迭代器加法符号重载
iterator operator+(int n);

// 迭代器减法符号重载
iterator operator-(int n);

// 迭代器加等于符号重载
iterator& operator+=(int n);

// 迭代器减等于符号重载
iterator& operator-=(int n);

// 迭代器解引用符号重载
std::pair<std::string, std::string>& operator*();
~~~

#### 示例程序

~~~c++
#include <iostream>
#include "Grammar.h"
using namespace std;

int main() {
    // 定义语法数据结构
    Grammar a;
	
    // 插入一些值
    a.insert("S", "aBa");
    a.insert("S", "cA");
    a.insert("A", "bb");
    a.insert("B", "CCD");
    a.insert("C", "bb");
    a.insert("D", "bb");
	
    // 遍历操作
    for (Grammar::iterator it = a.begin(); it != a.end(); it++) {
        cout << it.getKey() << "\t" << it.getValue() << endl;
    }
    cout << endl;
	
    // 迭代器取值测试
    cout << (a.begin() + 2).getKey() << "\t" << (a.begin() + 2).getValue() << endl;
    // 输出结果: A       bb
    
    // 迭代器取值测试
    cout << (a.end() - 2).getKey() << "\t" << (a.end() - 2).getValue() << endl;
	// 输出结果: C       bb
    
    return 0;
}
~~~

