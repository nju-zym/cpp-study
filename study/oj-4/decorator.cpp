#include <algorithm>  // Needed for string manipulation in decorators
#include <iostream>
#include <string>

// 抽象组件 - 数据源
class DataSource {
   public:
    virtual ~DataSource()                           = default;
    virtual void writeData(std::string const& data) = 0;
    virtual std::string readData()                  = 0;
};

// 具体组件 - 标准输入数据源
class StdDataSource : public DataSource {
   public:
    void writeData(std::string const& data) override {
        std::cout << data << '\n';
    }

    std::string readData() override {
        std::string data;
        // 从标准输入读取一个字符串，遇到空格停止
        std::cin >> data;
        return data;
    }
};

// 装饰器基类
class DataSourceDecorator : public DataSource {
   protected:
    DataSource* wrappee;

   public:
    explicit DataSourceDecorator(DataSource* source) : wrappee(source) {}
    ~DataSourceDecorator() override {
        delete wrappee;
    }

    DataSourceDecorator(DataSourceDecorator const&)            = delete;
    DataSourceDecorator& operator=(DataSourceDecorator const&) = delete;
    DataSourceDecorator(DataSourceDecorator&&)                 = delete;
    DataSourceDecorator& operator=(DataSourceDecorator&&)      = delete;

    void writeData(std::string const& data) override {
        wrappee->writeData(data);
    }
    std::string readData() override {
        return wrappee->readData();
    }
};

// 具体装饰器 - 加密
class EncryptionDecorator : public DataSourceDecorator {
   public:
    explicit EncryptionDecorator(DataSource* source) : DataSourceDecorator(source) {}

    void writeData(std::string const& data) override {
        std::string encryptedData = "ENCRYPTED(" + data + ")";
        wrappee->writeData(encryptedData);
    }

    std::string readData() override {
        std::string data   = wrappee->readData();
        std::string prefix = "ENCRYPTED(";
        std::string suffix = ")";
        if (data.rfind(prefix, 0) == 0 && data.length() >= prefix.length() + suffix.length()
            && data.substr(data.length() - suffix.length()) == suffix) {
            return data.substr(prefix.length(), data.length() - prefix.length() - suffix.length());
        }
        return data;
    }
};

// 具体装饰器 - 压缩
class CompressionDecorator : public DataSourceDecorator {
   public:
    explicit CompressionDecorator(DataSource* source) : DataSourceDecorator(source) {}

    void writeData(std::string const& data) override {
        std::string compressedData = "COMPRESSED[" + data + "]";
        wrappee->writeData(compressedData);
    }

    std::string readData() override {
        std::string data   = wrappee->readData();
        std::string prefix = "COMPRESSED[";
        std::string suffix = "]";
        if (data.rfind(prefix, 0) == 0 && data.length() >= prefix.length() + suffix.length()
            && data.substr(data.length() - suffix.length()) == suffix) {
            return data.substr(prefix.length(), data.length() - prefix.length() - suffix.length());
        }
        return data;
    }
};

// Optional: Add a main function for testing as described
int main() {
    // 创建基础实例
    DataSource* source = new StdDataSource();
    // 为实例添加新的功能，先加密后压缩
    source = new EncryptionDecorator(new CompressionDecorator(source));
    // 使用修饰后的实例
    std::string content = source->readData();
    source->writeData("data");
    // 输出：COMPRESSED[ENCRYPTED(data)]
}