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
    static inja::Template _implementationsTemplate;
    static inja::Template _cpoolTemplate;
    static inja::Environment _environment;

    [[nodiscard]] std::string generateEntryLink(int index) const;
    [[nodiscard]] std::string generateLink(const std::string& classname) const;
    [[nodiscard]] std::filesystem::path getClassRootDir() const;



    [[nodiscard]] std::filesystem::path getClassHtmlIndexFilename() const;
    [[nodiscard]] std::vector<std::string> getImplementations() const;
    [[nodiscard]] std::vector<std::string> renderHtmlClassIndex();
    void mainClassToHtmlIndex();
    [[nodiscard]] std::string getNavClassRelDir() const;
    [[nodiscard]] std::filesystem::path getHtmlCPoolFilename() const;
    [[nodiscard]] std::vector<std::string> renderCPoolHtml(const std::vector<std::string>& lines) const;
    void linesToHtmlFile(const std::vector<std::string>& lines) const;

    static std::string render(const inja::Template& compiledTemplate, const inja::json& json);
};


}
#endif //TARRACSH_CONSTANT_POOL_PRINTER_H
