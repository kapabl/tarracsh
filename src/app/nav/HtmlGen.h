#ifndef TARRACSH_HTML_CPOOL_GEN_H
#define TARRACSH_HTML_CPOOL_GEN_H
#include <inja/inja.hpp>
#include <unordered_map>
#include <unordered_set>

#include "../domain/classfile/ClassFileAnalyzer.h"
#include "../domain/classfile/constpool/ConstantPoolPrinter.h"


namespace kapa::tarracsh::nav {

class HtmlGen final: public ConstantPoolPrinter {
public:
    explicit HtmlGen(const ClassFileAnalyzer &classFileAnalyzer);
    ~HtmlGen() override = default;
    void print() override;
    static void init();

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
    void printTitle() override;

private:
    std::string _mainClassname;
    std::string _implementation;
    std::filesystem::path _classRootDir;
    std::filesystem::path _classRelDir;
    static inja::Template _implementationsTemplate;
    static inja::Template _classTemplate;
    static inja::Environment _injaEnvironment;
    // std::unordered_set<std::string> _classRefSet;
    std::set<std::string> _classRefSet;
    std::vector<std::string> _htmlOutput;
    std::vector<std::string> _cpoolHtmlEntries;
    std::vector<std::string> _classRefsHtml;

    [[nodiscard]] std::string generateEntryLink(int index) const;
    [[nodiscard]] std::filesystem::path getClassRootDir() const;
    

    [[nodiscard]] std::filesystem::path getClassHtmlIndexFilename() const;
    [[nodiscard]] std::vector<std::string> getImplementations() const;
    [[nodiscard]] std::vector<std::string> renderHtmlClassIndex();
    void generateImplIndexHtml();
    [[nodiscard]] std::filesystem::path getClassHtmlFilename() const;
    std::string generateSuperclassInfo() const;

    void renderClassHtml();
    void generateCPoolEntries();
    std::string generateClassLink(const std::string &classname) const;
    void generateClassRefs();
    void generateClassHtml();


    static std::string render(const inja::Template& compiledTemplate, const inja::json& json);
};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
