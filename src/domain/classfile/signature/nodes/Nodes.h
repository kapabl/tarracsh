#ifndef KAPA_JVM_SIGNATURE_NODES_H
#define KAPA_JVM_SIGNATURE_NODES_H

#include <memory>
#include <string>
#include <vector>


#define DECLARE_SINGLETON_TYPE_CLASS( classname, parent ) \
class classname: public parent { \
public: \
    static std::shared_ptr<classname> make() { \
        static std::shared_ptr<classname> _instance = std::make_shared<classname>(); \
        return _instance; \
    } \
};

#define DECLARE_BASE_TYPE_CLASS( name ) DECLARE_SINGLETON_TYPE_CLASS( name##Signature, BaseType )


namespace kapa::tarracsh::domain::classfile::signature::parser {
class Tree {
};

class TypeTree : public Tree {
    void accept(/*TypeTreeVisitor< ? > v*/);
};


class TypeArgument : public TypeTree {

};


class ReturnType : public TypeTree {

};

DECLARE_SINGLETON_TYPE_CLASS(VoidDescriptor, ReturnType)

class TypeSignature : public ReturnType {
};

class BaseType : public TypeSignature {
};

DECLARE_BASE_TYPE_CLASS(Byte)

DECLARE_BASE_TYPE_CLASS(Char)

DECLARE_BASE_TYPE_CLASS(Int)

DECLARE_BASE_TYPE_CLASS(Short)

DECLARE_BASE_TYPE_CLASS(Long)

DECLARE_BASE_TYPE_CLASS(Double)

DECLARE_BASE_TYPE_CLASS(Float)

DECLARE_BASE_TYPE_CLASS(Boolean)


class FieldTypeSignature : public BaseType, public TypeArgument {
};

class FormalTypeParameter : public TypeTree {
    std::string name;
    std::vector<std::shared_ptr<FieldTypeSignature>> bounds;

public:
    FormalTypeParameter(const std::string &n, const std::vector<std::shared_ptr<FieldTypeSignature>> &bs) {
        name = n;
        bounds = bs;
    }

    static std::shared_ptr<FormalTypeParameter>
    make(const std::string &n, const std::vector<std::shared_ptr<FieldTypeSignature>> &bs) {
        return std::make_shared<FormalTypeParameter>(n, bs);
    }

    std::vector<std::shared_ptr<FieldTypeSignature>> getBounds() { return bounds; }
    std::string getName() { return name; }

};

class SimpleClassTypeSignature : public FieldTypeSignature {
private:
    bool dollar;
    std::string name;
    std::vector<std::shared_ptr<TypeArgument>> typeArgs;

public:
    SimpleClassTypeSignature(const std::string &n, const bool dollar, const std::vector<std::shared_ptr<TypeArgument>> &tas) {
        name = n;
        this->dollar = dollar;
        typeArgs = tas;
    }

    static std::shared_ptr<SimpleClassTypeSignature> make(const std::string &n,
                                                          const bool dollar,
                                                          const std::vector<std::shared_ptr<TypeArgument>> &tas) {
        return std::make_shared<SimpleClassTypeSignature>(n, dollar, tas);
    }

    [[nodiscard]] bool getDollar() const { return dollar; }
    std::string getName() { return name; }
    std::vector<std::shared_ptr<TypeArgument>> getTypeArguments() { return typeArgs; }
};

class ArrayTypeSignature : public FieldTypeSignature {
    std::shared_ptr<TypeSignature> componentType;

public:
    ArrayTypeSignature(const std::shared_ptr<TypeSignature> &ct) { componentType = ct; }


    static std::shared_ptr<ArrayTypeSignature> make(const std::shared_ptr<TypeSignature> &ct) {
        return std::make_shared<ArrayTypeSignature>(ct);
    }

    std::shared_ptr<TypeSignature> getComponentType() { return componentType; }
};

class TypeVariableSignature : public FieldTypeSignature {
private:
    std::string identifier;
public:
    TypeVariableSignature(const std::string &id) { identifier = id; }


    static std::shared_ptr<TypeVariableSignature> make(const std::string &id) {
        return std::make_shared<TypeVariableSignature>(id);
    }

    std::string getIdentifier() { return identifier; }
};

DECLARE_SINGLETON_TYPE_CLASS(BottomSignature, FieldTypeSignature)


class ClassTypeSignature : public FieldTypeSignature {
    std::vector<std::shared_ptr<SimpleClassTypeSignature>> _path;

public:
    ClassTypeSignature(std::vector<std::shared_ptr<SimpleClassTypeSignature>> path)
        : _path(std::move(path)) {
    }


    static std::shared_ptr<ClassTypeSignature>
    make(const std::vector<std::shared_ptr<SimpleClassTypeSignature>> &path) {
        auto result = std::make_shared<ClassTypeSignature>(path);
        return result;
    }

    std::vector<std::shared_ptr<SimpleClassTypeSignature>> &getPath() { return _path; }
};

class Signature : public Tree {
public:
    std::vector<FormalTypeParameter> getFormalTypeParameters();
};


class ClassSignature : public Signature {
    std::vector<std::shared_ptr<FormalTypeParameter>> formalTypeParams;
    std::shared_ptr<ClassTypeSignature> superclass;
    std::vector<std::shared_ptr<ClassTypeSignature>> superInterfaces;

public:
    ClassSignature(const std::vector<std::shared_ptr<FormalTypeParameter>> &ftps,
                   const std::shared_ptr<ClassTypeSignature>& sc,
                   const std::vector<std::shared_ptr<ClassTypeSignature>>& sis) {
        formalTypeParams = ftps;
        superclass = sc;
        superInterfaces = sis;
    }


    static std::shared_ptr<ClassSignature> make(const std::vector<std::shared_ptr<FormalTypeParameter>> &ftps,
                                                const std::shared_ptr<ClassTypeSignature> &sc,
                                                const std::vector<std::shared_ptr<ClassTypeSignature>> &sis) {
        return std::make_shared<ClassSignature>(ftps, sc, sis);
    }

    std::vector<std::shared_ptr<FormalTypeParameter>> getFormalTypeParameters() {
        return formalTypeParams;
    }

    std::shared_ptr<ClassTypeSignature> getSuperclass() { return superclass; }
    std::vector<std::shared_ptr<ClassTypeSignature>> getSuperInterfaces() { return superInterfaces; }
};

class Wildcard : public TypeArgument {
private:
    std::vector<std::shared_ptr<FieldTypeSignature>> upperBounds;
    std::vector<std::shared_ptr<FieldTypeSignature>> lowerBounds;
public:
    Wildcard(const std::vector<std::shared_ptr<FieldTypeSignature>> &ubs,
             const std::vector<std::shared_ptr<FieldTypeSignature>> &lbs) {
        upperBounds = ubs;
        lowerBounds = lbs;
    }

    static std::vector<std::shared_ptr<FieldTypeSignature>> getEmptyBounds() {
        static std::vector<std::shared_ptr<FieldTypeSignature>> emptyBounds;
        return emptyBounds;
    }

    static std::shared_ptr<Wildcard> make(const std::vector<std::shared_ptr<FieldTypeSignature>> &ubs,
                                          const std::vector<std::shared_ptr<FieldTypeSignature>> &lbs) {
        return std::make_shared<Wildcard>(ubs, lbs);
    }

    std::vector<std::shared_ptr<FieldTypeSignature>> getUpperBounds() {
        return upperBounds;
    }

    std::vector<std::shared_ptr<FieldTypeSignature>> getLowerBounds() {
        if (lowerBounds.size() == 1 && lowerBounds[0].get() == BottomSignature::make().get())
            return getEmptyBounds();
        return lowerBounds;
    }
};

class MethodTypeSignature : public Signature {
    std::vector<std::shared_ptr<FormalTypeParameter>> formalTypeParams;
    std::vector<std::shared_ptr<TypeSignature>> parameterTypes;
    std::shared_ptr<ReturnType> returnType;
    std::vector<std::shared_ptr<FieldTypeSignature>> exceptionTypes;
public:
    MethodTypeSignature(const std::vector<std::shared_ptr<FormalTypeParameter>> &ftps,
                        const std::vector<std::shared_ptr<TypeSignature>> &pts,
                        const std::shared_ptr<ReturnType> &rt,
                        const std::vector<std::shared_ptr<FieldTypeSignature>> &ets) {
        formalTypeParams = ftps;
        parameterTypes = pts;
        returnType = rt;
        exceptionTypes = ets;
    }


    static std::shared_ptr<MethodTypeSignature> make(const std::vector<std::shared_ptr<FormalTypeParameter>> &ftps,
                                                     const std::vector<std::shared_ptr<TypeSignature>> &pts,
                                                     const std::shared_ptr<ReturnType> &rt,
                                                     const std::vector<std::shared_ptr<FieldTypeSignature>> &ets) {
        return std::make_shared<MethodTypeSignature>(ftps, pts, rt, ets);
    }

    std::vector<std::shared_ptr<FormalTypeParameter>> getFormalTypeParameters() {
        return formalTypeParams;
    }

    std::vector<std::shared_ptr<TypeSignature>> getParameterTypes() { return parameterTypes; }
    std::shared_ptr<ReturnType> getReturnType() { return returnType; }
    std::vector<std::shared_ptr<FieldTypeSignature>> getExceptionTypes() { return exceptionTypes; }

    // void accept(Visitor< ? > v) { v.visitMethodTypeSignature(this); }
};
}

#undef DECLARE_BASE_TYPE_CLASS

#endif
