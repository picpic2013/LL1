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

