Vector<T>
包含以下属性和方法
public:
Vector(); 默认构造函数
Vector(int size); 声明大小的构造函数
Vector(const Vector& r); 拷贝构造函数
virtual ~Vector(); 析构函数
T& operator[](int index) : throw(IndexOutofBounds); 运算符重载 调用值 下标越界会抛出异常IndexOutofBounds
int size(); 返回Vector的大小
int inflate(int addSize); 扩充Vector 增加addSize个元素空间
private:
T *m_pElements;
int m_nSize;

编译运行main.cpp

注：最后有一个溢出例子没有加catch故程序会停止工作。