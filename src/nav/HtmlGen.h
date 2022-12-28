#ifndef TARRACSH_HTML_CPOOL_GEN_H
#define TARRACSH_HTML_CPOOL_GEN_H
#include <inja/inja.hpp>
#include <unordered_map>
#include "../classfile/ClassFileAnalyzer.h"
#include "../classfile/constpool/ConstantPoolPrinter.h"


namespace org::kapa::tarracsh::nav {

class HtmlGen final: public ConstantPoolPrinter {
public:
    explicit HtmlGen(const ClassFileAnalyzer &classFileAnalyzer);
    ~HtmlGen() override = default;

    void print() override;

    typedef std::unordered_map<std::string, std::string> OutputSubTemplates;


protected:
    void printHeader(const ConstPoolBase& entry, int index) override;
    void printUtf8Info(const Utf8Info &entry, int index) override;
    void printStringInfo(const StringInfo &entry, int index) override;
    void printLongInfo(const LongInfo &entry, int index) override;
    void printDoubleInfo(const DoubleInfo &entry, int index) override;
    void printIntegerInfo(const IntegerInfo &entry, int index) override;
    void printFloatInfo(const FloatInfo &entry, int index) override;
    void printClassInfo(const ClassInfo &entry, int index) override;
    void printMethodrefInfo(const MethodrefInfo &entry, int index) override;
    void printMethodHandleInfo(const MethodHandleInfo &entry, int index) override;
    void printMethodTypeInfo(const MethodTypeInfo &entry, int index) override;
    void printFieldrefInfo(const FieldrefInfo &entry, int index) override;
    void printModuleInfo(const ModuleInfo &entry, int index) override;
    void printRefExtraInfo(const MemberInfo &entry) override;
    void printInterfaceMethodrefInfo(const InterfaceMethodrefInfo &entry, int index) override;
    void printInvokeDynamicInfo(const InvokeDynamicInfo &entry, int index) override;
    void printNameAndTypeInfo(const NameAndTypeInfo &entry, int index) override;
    // void printEntry(const ConstantPoolRecord &entry, int index) override;
    void printTitle() override;

    [[nodiscard]] std::filesystem::path getClassHtmlIndexFilename() const;
    std::string  getImplLinks();
    std::vector<std::string> renderHtmlClassIndex();
    void mainClassToHtmlIndex();
    [[nodiscard]] std::string getNavClassRelDir() const;
    [[nodiscard]] std::string getNavImplRelDir() const;
    [[nodiscard]] std::filesystem::path getHtmlCPoolFilename() const;
    [[nodiscard]] std::vector<std::string> renderCPoolHtml(const std::vector<std::string>& lines) const;
    void linesToHtmlFile(const std::vector<std::string>& lines);


};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
