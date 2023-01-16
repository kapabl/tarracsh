#ifndef TARRACSH_HTML_CPOOL_GEN_H
#define TARRACSH_HTML_CPOOL_GEN_H
#include <set>
#include <inja/inja.hpp>

#include "../domain/classfile/ClassFileParser.h"
#include "../ConstantPoolPrinter.h"


namespace kapa::tarracsh::app::classfile::constantpool::printer::nav {

class HtmlGen final: public classfile::constantpool::printer::ConstantPoolPrinter {
public:
    explicit HtmlGen(const domain::classfile::ClassFileParser &classFileParser);
    ~HtmlGen() override = default;
    void print() override;
    static void init();

protected:
    void printHeader(const domain::classfile::constantpool::ConstPoolBase& entry, int index) override;
    void printUtf8Info(const domain::classfile::constantpool::Utf8Info &entry, int index) override;
    void printStringInfo(const domain::classfile::constantpool::StringInfo &entry, int index) override;
    void printLongInfo(const domain::classfile::constantpool::LongInfo &entry, int index) override;
    void printDoubleInfo(const domain::classfile::constantpool::DoubleInfo &entry, int index) override;
    void printIntegerInfo(const domain::classfile::constantpool::IntegerInfo &entry, int index) override;
    void printFloatInfo(const domain::classfile::constantpool::FloatInfo &entry, int index) override;
    void printClassInfo(const domain::classfile::constantpool::ClassInfo &entry, int index) override;
    void printMethodrefInfo(const domain::classfile::constantpool::MethodrefInfo &entry, int index) override;
    void printMethodHandleInfo(const domain::classfile::constantpool::MethodHandleInfo &entry, int index) override;
    void printMethodTypeInfo(const domain::classfile::constantpool::MethodTypeInfo &entry, int index) override;
    void printFieldrefInfo(const domain::classfile::constantpool::FieldrefInfo &entry, int index) override;
    void printModuleInfo(const domain::classfile::constantpool::ModuleInfo &entry, int index) override;
    void printRefExtraInfo(const domain::classfile::constantpool::MemberInfo &entry) override;
    void printInterfaceMethodrefInfo(const domain::classfile::constantpool::InterfaceMethodrefInfo &entry, int index) override;
    void printInvokeDynamicInfo(const domain::classfile::constantpool::InvokeDynamicInfo &entry, int index) override;
    void printNameAndTypeInfo(const domain::classfile::constantpool::NameAndTypeInfo &entry, int index) override;
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
    [[nodiscard]] std::string generateSuperclassInfo() const;

    void renderClassHtml();
    void generateCPoolEntries();
    [[nodiscard]] std::string generateClassLink(const std::string &classname) const;
    void generateClassRefs();
    void generateClassHtml();


    static std::string render(const inja::Template& compiledTemplate, const inja::json& json);
};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
