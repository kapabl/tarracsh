#ifndef SIGNATURE_OR_RULE_H
#define SIGNATURE_OR_RULE_H
#include "Rule.h"
#include "NodeTraits.h"

namespace kapa::tarracsh::domain::classfile::signature {


class Or final : public Rule {
public:
    Or();
    explicit Or(const bool isAnchor);

    explicit Or(const RulePtr &left);
    Or(const Or &other);
    Or(Or &&other) noexcept;
    Or &operator=(const Or &other);
    Or &operator=(Or &&other) noexcept;
    ~Or() override = default;

    void addToOr(RuleVariant ruleVariant);


    template <typename T>
    bool match(SignatureScanner &scanner, std::shared_ptr<T> &node) {
        node.reset(new T());
        auto result = match(scanner, *node);
        return result;
    }

    template <typename T>
    bool match(SignatureScanner &scanner, T &node) {
        auto result = false;
        DEBUG_RULE(this);

        result = OrMatchSelector<is_or_single_capture<T>::value, T>::match(*this, scanner, node);

        return result;
    }


    template <typename T>
    bool match(SignatureScanner &scanner, std::vector<T> &list) {
        auto result = false;
        DEBUG_RULE(this);
        return result;
    }

    template <>
    bool match<std::wstring>(SignatureScanner &scanner, std::wstring &node);

    void captureChildren(bool value) {
        for (auto &rule : _rules) {
            std::visit([&value](auto rulePtr) { rulePtr->setCapture(value); }, rule);
        }
    }

private:
    std::vector<RuleVariant> _rules;

    template <typename T>
    bool one2OneMatch(SignatureScanner &scanner, T &node) {
        auto index = 0u;

        auto result = false;

        visit_struct::for_each(node, [this, &index, &result, &scanner](const char *name, auto &value) {
            if (result) return;

            while (index < _rules.size()) {
                auto &ruleVariant = _rules[index];
                auto matchResult = invokeMatch(ruleVariant, scanner, value);
                result = matchResult.first;

                if (result) return;

                const auto canCapture = matchResult.second;
                if (canCapture) break;
                index++;
            }
            index++;
        });

        return result;
    }

    template <typename T>
    bool one2ManyMatch(SignatureScanner &scanner, T &node) {
        auto index = 0u;

        auto result = false;

        visit_struct::for_each(node, [this, &index, &result, &scanner](const char *name, auto &value) {

            while (index < _rules.size()) {
                auto &ruleVariant = _rules[index];
                auto matchResult = invokeMatch(ruleVariant, scanner, value);
                result = matchResult.first;

                if (result) return;
                index++;
            }
            return;
        });

        return result;
    }

    template <bool IsOrSingleCapture, typename TNode>
    struct OrMatchSelector {
        static bool match(Or &rule, SignatureScanner &scanner, TNode &node) {
            const bool result = rule.one2OneMatch(scanner, node);
            return result;
        }
    };

    template <typename TNode>
    struct OrMatchSelector<true, TNode> {
        static bool match(Or &rule, SignatureScanner &scanner, TNode &node) {
            const bool result = rule.one2ManyMatch(scanner, node);
            return result;
        }
    };
};

}
#endif
